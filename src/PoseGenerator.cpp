#include "PoseGenerator.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "opencv2/opencv.hpp"
#include "OGRE/OgreVector3.h"
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
    hand_renderer_.Setup(320, 320);

    // Load scene spec file
    /* FileDialog dialog; */
    /* dialog.SetTitle("Please select a scene spec file"); */
    /* string file_name = dialog.Open(); */
    string file_name = "/home/ajdillhoff/dev/projects/libhand-public/fingers/index/scene_spec.yml";
    /* string file_name = "/home/ajdillhoff/dev/projects/libhand-public/hand2j/scene_spec.yml"; */
    /* string file_name = "/home/ajdillhoff/dev/projects/libhand-public/cylinderv3/scene_spec.yml"; */
    /* string file_name = "/home/ajdillhoff/dev/projects/libhand-public/hand_model_no_wrist/rhd/scene_spec.yml"; */

    // Process file
    SceneSpec scene_spec(file_name);

    // Renderer loads the scene
    hand_renderer_.LoadScene(scene_spec);

    // HandPose object that we will use to generate random poses
    hand_pose_ = FullHandPose(scene_spec.num_bones());
}

// Returns a pose generated based on the current generator parameters.
void PoseGenerator::GeneratePose(int i) {

    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});
    auto gen = uniform_int_distribution<int>(0, 3);


    // Set the values
    auto random_integer = gen(rng_);
    hand_pose_.side(0) = random_integer * 90.0 * (M_PI / 180);
    hand_pose_.side(1) = M_PI / 6;
    /* if (r > 0.66) { */
    /*     hand_pose_.side(0) = 90.0 * (M_PI / 180); */
    /* } else if (r > 0.33 && r <= 0.66) { */
    /*     hand_pose_.side(0) = 45.0 * (M_PI / 180); */
    /* } else { */
    /*     hand_pose_.side(0) = 0.0; */
    /* } */

    // Apply the hand pose
    hand_renderer_.SetHandPose(hand_pose_);

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

    return d;
}
