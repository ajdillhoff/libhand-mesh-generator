#ifndef POSE_GENERATOR_H_
#define POSE_GENERATOR_H_

#include <memory>
#include <random>

#include "opencv2/opencv.hpp"
#include "hand_renderer.h"
#include "hand_pose.h"
#include "OGRE/OgreVector3.h"

#include "PoseConfig.h"
#include "PosePrototype.h"
#include "ShapeConfig.h"
#include "ShapePrototype.h"


struct PoseSample {
    cv::Mat depth_buffer;
    cv::Mat pixel_buffer;
    std::map<std::string, std::vector<double>> joint_position_map;
};

// Generates random hand poses. Meshes, keybpoints, and depth images can be
// generated from the random poses.
class PoseGenerator {
  public:

    PoseGenerator();
    PoseGenerator(string, string, string);
    ~PoseGenerator();

    static const int kDefaultWidth = 320;
    static const int kDefaultHeight = 320;
    static constexpr float kDefaultCameraDistance = 2.0;

    void Setup(string, string, string);

    // Method to generate random hand pose
    void GeneratePose(unique_ptr<posegen::PoseParameters>, unique_ptr<posegen::ShapeParameters>);

    // Get sample based on the currently generated pose
    unique_ptr<PoseSample> GetSample();

    // Save keypoints method
    // Save depth method
    // Save all method using some predefined path

    // Configuration parameters
    // - joints to randomize
    // - parameters of each joint to randomize
    // - path to save generated data

  private:
    float distance_to_camera_;
    libhand::HandRenderer hand_renderer_;
    int vertex_count_;
    std::shared_ptr<Ogre::Vector3> mesh_data_;
    libhand::FullHandPose hand_pose_;
    std::mt19937 rng_{ (std::random_device())() };
    posegen::PoseConfig pose_config_;
    posegen::ShapeConfig shape_config_;
};

#endif  // POSE_GENERATOR_H_
