#ifndef POSE_GENERATOR_H_
#define POSE_GENERATOR_H_

#include <memory>

#include "opencv2/opencv.hpp"
#include "hand_renderer.h"
#include "OGRE/OgreVector3.h"

// Generates random hand poses. Meshes, keybpoints, and depth images can be
// generated from the random poses.
class PoseGenerator {
  public:
    struct PoseSample {
        std::shared_ptr<Ogre::Vector3> mesh_data;
        int vertex_count;
        cv::Mat depth_buffer;
        libhand::HandRenderer::JointPositionMap joint_position_map;
    };

    PoseGenerator();
    ~PoseGenerator();

    static const int kDefaultWidth = 600;
    static const int kDefaultHeight = 600;
    static constexpr float kDefaultCameraDistance = 6.;

    // Method to generate random hand pose
    void GeneratePose();

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
};

#endif  // POSE_GENERATOR_H_
