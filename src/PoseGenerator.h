#ifndef POSE_GENERATOR_H_
#define POSE_GENERATOR_H_

#include <memory>
#include <random>

#include "opencv2/opencv.hpp"
#include "hand_renderer.h"
#include "hand_pose.h"
#include "OGRE/OgreVector3.h"

// Generates random hand poses. Meshes, keybpoints, and depth images can be
// generated from the random poses.
class PoseGenerator {
  public:
    struct PoseSample {
        std::shared_ptr<Ogre::Vector3> mesh_data;
        int vertex_count;
        cv::Mat depth_buffer;
        cv::Mat pixel_buffer;
        libhand::HandRenderer::JointPositionMap joint_position_map;
    };

    PoseGenerator();
    ~PoseGenerator();

    static const int kDefaultWidth = 320;
    static const int kDefaultHeight = 320;
    static constexpr float kDefaultCameraDistance = 2.0;

    void Setup();

    // Method to generate random hand pose
    void GeneratePose(int);

    // Get sample based on the currently generated pose
    PoseSample GetSample();

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
};

#endif  // POSE_GENERATOR_H_
