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
    /* string file_name = "/home/ajdillhoff/dev/projects/libhand-public/fingers/index/scene_spec.yml"; */
    /* string file_name = "/home/alex/dev/examples/libhand/hand_model/scene_spec.yml"; */
    string file_name = "/home/ajdillhoff/dev/projects/libhand-public/sphere/scene_spec.yml";

    // Process file
    SceneSpec scene_spec(file_name);

    // Renderer loads the scene
    hand_renderer_.LoadScene(scene_spec);

    // HandPose object that we will use to generate random poses
    hand_pose_ = FullHandPose(scene_spec.num_bones());
}

// Returns a pose generated based on the current generator parameters.
void PoseGenerator::GeneratePose() {

    uniform_real_distribution<float> gen(-1, 1);
    /* float phi = (M_PI / 4) * gen(rng); */
    /* float theta = M_PI * -gen(rng); */
    /* float tilt = (M_PI / 2) * -gen(rng); */

    /* HandCameraSpec camera_spec(distance_to_camera_); */
    /* camera_spec.theta = theta; */
    /* camera_spec.phi = phi; */
    /* camera_spec.tilt = 0.0; */

    // Set the camera_spec, controls the perspective we view the hand from
    /* hand_renderer_.set_camera_spec(camera_spec); */

    // Set the values
    /* hand_pose_.side(0) = (M_PI / 2) * gen(rng_); */
    /* hand_pose_.twist(0) = (M_PI / 2) * gen(rng_); */
    /* gen = uniform_real_distribution<float>(0, 1); */
    /* hand_pose_.bend(1) = -(M_PI / 2) * gen(rng_); */

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
