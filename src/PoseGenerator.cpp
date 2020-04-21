#include "PoseGenerator.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "opencv2/opencv.hpp"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreMatrix3.h"
#include "hand_renderer.h"
#include "scene_spec.h"
#include "ndarray_converter.h"

using namespace std;
using namespace Ogre;
using namespace libhand;


float gen_rand(float min_deg, float max_deg) {
    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});

    auto deg = rand() * (max_deg - min_deg) + min_deg;
    return deg * (M_PI / 180.0);
}

std::map<string, std::vector<double>> convert_joint_map(libhand::HandRenderer::JointPositionMap joint_position_map) {
    std::map<string, std::vector<double>> converted_map;

    for (auto const& element : joint_position_map) {
        converted_map[element.first] = {
            element.second[0],
            element.second[1],
            element.second[2]
        };
    }

    return converted_map;
}

PoseGenerator::PoseGenerator() {
    distance_to_camera_ = kDefaultCameraDistance;
}

PoseGenerator::PoseGenerator(string scene_path, string pose_config_path, string shape_config_path) {
    distance_to_camera_ = kDefaultCameraDistance;
    Setup(scene_path, pose_config_path, shape_config_path);
}

PoseGenerator::~PoseGenerator() {}

void PoseGenerator::Setup(string scene_path, string pose_config_path, string shape_config_path) {
    hand_renderer_.Setup(640, 480);

    // Process file
    SceneSpec scene_spec(scene_path);

    // Renderer loads the scene
    hand_renderer_.LoadScene(scene_spec);

    // HandPose object that we will use to generate random poses
    hand_pose_ = FullHandPose(scene_spec.num_bones());

    Matrix3 proj = hand_renderer_.camera_spec().GetRotMatrix();

    pose_config_ = posegen::PoseConfig(pose_config_path);
    shape_config_ = posegen::ShapeConfig(shape_config_path);
}

void PoseGenerator::GeneratePose(unique_ptr<posegen::PoseParameters> pose_params, unique_ptr<posegen::ShapeParameters> shape_params) {
    for (int i = 0; i < pose_params->bend.size(); i++) {
        hand_pose_.bend(i) = pose_params->bend[i];
        hand_pose_.side(i) = pose_params->side[i];
        hand_pose_.twist(i) = pose_params->twist[i];
    }

    for (int i = 0; i < shape_params->swell.size(); i++) {
        hand_pose_.swelling(i) = shape_params->swell[i];
        hand_pose_.dilation(i) = shape_params->dilate[i];
        hand_pose_.elongation(i) = shape_params->elongate[i];
    }

    // Apply the hand pose
    hand_renderer_.SetHandPose(hand_pose_);

    // Position test
    Ogre::Vector3 v(pose_params->hand_offset[0], pose_params->hand_offset[1], pose_params->hand_offset[2]);
    hand_renderer_.SetHandPosition(v);

    // Render the hand
    hand_renderer_.RenderHand();
}

// Returns a sample based on the currently generated pose.
// TODO: Implement error checking
//PoseGenerator::PoseSample PoseGenerator::GetSample() {
unique_ptr<PoseSample> PoseGenerator::GetSample() {
    GeneratePose(pose_config_.GenerateParams(), shape_config_.GenerateParams());

    unique_ptr<PoseSample> sample(new PoseSample());
    sample->depth_buffer = hand_renderer_.depth_buffer_cv();
    sample->pixel_buffer = hand_renderer_.pixel_buffer_cv();
    libhand::HandRenderer::JointPositionMap temp;
    hand_renderer_.walk_bones(temp);
    sample->joint_position_map = convert_joint_map(temp);

    sample->depth_buffer.setTo(0, sample->depth_buffer == std::numeric_limits<float>::infinity());

    cv::Mat test;

    double min = 0.1;
    double max = 8.0;
    cv::Mat mask = sample->depth_buffer != 0;

    double min_t;
    double max_t;
    cv::minMaxLoc(sample->depth_buffer, &min_t, &max_t, 0, 0, mask);

    sample->depth_buffer.convertTo(test, CV_16UC1, 1000.0);
    sample->depth_buffer = test;

    return sample;
}

PYBIND11_MODULE(PoseGenerator, m) {
    NDArrayConverter::init_numpy();
    pybind11::class_<PoseSample>(m, "PoseSample")
        .def(pybind11::init<>())
        .def_readwrite("depth", &PoseSample::depth_buffer)
        .def_readwrite("color", &PoseSample::pixel_buffer)
        .def_readwrite("joints", &PoseSample::joint_position_map);
    pybind11::class_<PoseGenerator>(m, "PoseGenerator")
        .def(pybind11::init<>())
        .def(pybind11::init<std::string &, std::string &, std::string &>())
        .def("Setup", &PoseGenerator::Setup)
        .def("GetSample", &PoseGenerator::GetSample);
}
