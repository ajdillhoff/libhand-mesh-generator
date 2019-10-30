#include "PoseGenerator.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "opencv2/opencv.hpp"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreMatrix3.h"
#include "hand_renderer.h"
#include "scene_spec.h"

using namespace std;
using namespace Ogre;
using namespace libhand;

PoseGenerator::PoseGenerator() {
    distance_to_camera_ = kDefaultCameraDistance;
}

PoseGenerator::~PoseGenerator() {}

void PoseGenerator::Setup() {
    hand_renderer_.Setup(640, 480);

    // Load scene spec file
    /* FileDialog dialog; */
    /* dialog.SetTitle("Please select a scene spec file"); */
    /* string file_name = dialog.Open(); */
    //string file_name = "/home/alex/dev/projects/libhand-public/hand_model_no_wrist/rhd/scene_spec.yml";
    string file_name = "/home/alex/dev/projects/libhand-public/hand_model_no_arm/scene_spec.yml";

    // Process file
    SceneSpec scene_spec(file_name);

    // Renderer loads the scene
    hand_renderer_.LoadScene(scene_spec);

    // HandPose object that we will use to generate random poses
    hand_pose_ = FullHandPose(scene_spec.num_bones());

    Matrix3 proj = hand_renderer_.camera_spec().GetRotMatrix();
}

// Returns a pose generated based on the current generator parameters.
void PoseGenerator::GeneratePose(int i) {

    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});
    auto rand2 = std::bind(std::uniform_real_distribution<float>{-1.0, 1.0},
            std::default_random_engine{std::random_device()()});
    auto gen = uniform_int_distribution<int>(0, 3);

    // Set the values
    auto random_integer = gen(rng_);

    // carpals
    hand_pose_.side(16) = rand2() * (M_PI / 2);
    hand_pose_.twist(16) = rand2() * (M_PI / 6);
    hand_pose_.bend(16) = rand() * -(M_PI / 3);

    // metacarpals
    hand_pose_.bend(15) = rand2() * (M_PI / 6);
    hand_pose_.side(15) = rand2() * (M_PI / 12);

    // pinky
    hand_pose_.bend(0) = rand() * -(M_PI / 4);
    hand_pose_.bend(1) = rand() * -(M_PI / 4);
    hand_pose_.bend(2) = rand() * -(M_PI / 6);

    // ringer finger
    hand_pose_.bend(3) = rand() * -(M_PI / 4);
    hand_pose_.bend(4) = rand() * -(M_PI / 4);
    hand_pose_.bend(5) = rand() * -(M_PI / 6);

    // middle finger
    hand_pose_.bend(6) = rand() * -(M_PI / 4);
    hand_pose_.bend(7) = rand() * -(M_PI / 4);
    hand_pose_.bend(8) = rand() * -(M_PI / 6);

    // index finger
    hand_pose_.bend(9) = rand() * -(M_PI / 6);
    hand_pose_.bend(10) = rand() * -(M_PI / 4);
    hand_pose_.bend(11) = rand() * -(M_PI / 6);

    // thumb
    hand_pose_.bend(12) = rand() * -(M_PI / 4);
    hand_pose_.bend(13) = rand2() * -(M_PI/ 12);
    //hand_pose_.side(14) = rand() * (M_PI / 4);
    //hand_pose_.side(15) = rand() * (M_PI / 4);

    // Apply the hand pose
    hand_renderer_.SetHandPose(hand_pose_);

    // Position test
    float x_offset = rand2() * 0.1;
    float y_offset = 0.1 + rand2() * 0.1;
    float z_offset = -0.25 + rand2() * 0.25;
    Ogre::Vector3 v(x_offset, y_offset, z_offset);
    hand_renderer_.SetHandPosition(v);

    // Render the hand
    hand_renderer_.RenderHand();
}

// Returns a sample based on the currently generated pose.
// TODO: Implement error checking
PoseGenerator::PoseSample PoseGenerator::GetSample() {
    // Test GetMeshVertices
    size_t vertex_count = 0;
    Vector3* mesh_data;
    hand_renderer_.GetMeshVertices(vertex_count, mesh_data);

    // libhand uses `new` to create the mesh data, we will let `shared_ptr`
    // handle the lifetime from here on.
    shared_ptr<Vector3> mesh_data_(mesh_data);

    PoseSample d = PoseSample();
    d.mesh_data = mesh_data_;
    d.vertex_count = vertex_count;
    d.depth_buffer = hand_renderer_.depth_buffer_cv().clone();
    d.pixel_buffer = hand_renderer_.pixel_buffer_cv().clone();
    hand_renderer_.walk_bones(d.joint_position_map);

    // DEBUG
    Vector3 c = hand_renderer_.CamPositionRelativeToHand();

    return d;
}
