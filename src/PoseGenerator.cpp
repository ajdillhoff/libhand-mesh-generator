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


float gen_rand(float min_deg, float max_deg) {
    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});

    auto deg = rand() * (max_deg - min_deg) + min_deg;
    return deg * (M_PI / 180.0);
}

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
    string file_name = "/home/alex/dev/projects/libhand-public/nyu_synth/scene_spec.yml";

    // Process file
    SceneSpec scene_spec(file_name);

    // Renderer loads the scene
    hand_renderer_.LoadScene(scene_spec);

    // HandPose object that we will use to generate random poses
    hand_pose_ = FullHandPose(scene_spec.num_bones());

    Matrix3 proj = hand_renderer_.camera_spec().GetRotMatrix();
}

void PoseGenerator::GeneratePose(unique_ptr<PoseParameters> params) {
    for (int i = 0; i < params->bend.size(); i++) {
        hand_pose_.bend(i) = params->bend[i];
        hand_pose_.side(i) = params->side[i];
        hand_pose_.twist(i) = params->twist[i];
    }

    // Apply the hand pose
    hand_renderer_.SetHandPose(hand_pose_);

    // Position test
    Ogre::Vector3 v(params->hand_offset[0], params->hand_offset[1], params->hand_offset[2]);
    hand_renderer_.SetHandPosition(v);

    // Render the hand
    hand_renderer_.RenderHand();
}

// Returns a pose generated based on the current generator parameters.
void PoseGenerator::GeneratePose(int i) {

    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});
    auto rand2 = std::bind(std::uniform_real_distribution<float>{-1.0, 1.0},
            std::default_random_engine{std::random_device()()});

    // carpals
    hand_pose_.side(16) = rand2() * (M_PI / 4);
    hand_pose_.bend(16) = rand() * -(M_PI / 4);
    hand_pose_.twist(16) = rand2() * M_PI / 4;

    // metacarpals
    hand_pose_.bend(15) = rand2() * (M_PI / 6);
    hand_pose_.side(15) = rand2() * (M_PI / 12);

    // thumb
    auto flex = 0.0;
    if (rand() < 0.5) {
        flex = gen_rand(-15.0, 10.0);
    } else {
        flex = gen_rand(10.0, 25.0);
    }

    hand_pose_.bend(12) = gen_rand(-15.0, 5.0);
    hand_pose_.side(12) = flex;
    hand_pose_.bend(13) = flex / 2;
    hand_pose_.bend(14) = gen_rand(-90.0, 0.0);

    // spread fingers
    auto spread = gen_rand(-22.0, 12);
    hand_pose_.side(0) = (spread + gen_rand(-5.0, 5.0)) * 1.55;
    hand_pose_.side(3) = (spread + gen_rand(-5.0, 5.0)) * 0.75;
    hand_pose_.side(6) = (spread + gen_rand(-5.0, 5.0)) * -0.75;
    hand_pose_.side(9) = (spread + gen_rand(-5.0, 5.0)) * -1.55;

    // Bend fingers towards palm: Joint 1
    auto flex1 = gen_rand(-40.0, -60.0);
    hand_pose_.bend(0) = (flex1 + gen_rand(-10.0, 10.0));
    hand_pose_.bend(3) = (flex1 + gen_rand(-10.0, 10.0));
    hand_pose_.bend(6) = (flex1 + gen_rand(-10.0, 10.0));

    // Bend fingers towards palm: Joint 2
    auto flex2 = gen_rand(-40.0, -60.0);
    hand_pose_.bend(1) = (flex2 + gen_rand(-10.0, 10.0));
    hand_pose_.bend(4) = (flex2 + gen_rand(-10.0, 10.0));
    hand_pose_.bend(7) = (flex2 + gen_rand(-10.0, 10.0));

    // Bend fingers towards palm: Joint 3
    auto flex3 = gen_rand(-40.0, -60.0);
    hand_pose_.bend(2) = (flex3 + gen_rand(-10.0, 10.0));
    hand_pose_.bend(5) = (flex3 + gen_rand(-10.0, 10.0));
    hand_pose_.bend(8) = (flex3 + gen_rand(-10.0, 10.0));

    // Pointer finger
    hand_pose_.bend(10) = gen_rand(-5.0, 15.0);
    hand_pose_.bend(11) = gen_rand(-5.0, 5.0);
    hand_pose_.bend(12) = gen_rand(-5.0, 5.0);

    // Apply the hand pose
    hand_renderer_.SetHandPose(hand_pose_);

    // Position test
    float x_offset = rand2() * 0.1;
    float y_offset = rand2() * 0.1;
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
