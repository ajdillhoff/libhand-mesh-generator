#include "PoseGenerator.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

#include "opencv2/opencv.hpp"
#include "OGRE/OgreVector3.h"
#include "hand_pose.h"
#include "hand_renderer.h"
#include "scene_spec.h"

using namespace std;
using namespace Ogre;
using namespace libhand;

PoseGenerator::PoseGenerator() {
    distance_to_camera_ = kDefaultCameraDistance;
}

PoseGenerator::~PoseGenerator() {}

// Returns a pose generated based on the current generator parameters.
void PoseGenerator::GeneratePose() {
    hand_renderer_.Setup(kDefaultWidth, kDefaultHeight);

    // Load scene spec file
    /* FileDialog dialog; */
    /* dialog.SetTitle("Please select a scene spec file"); */
    /* string file_name = dialog.Open(); */
    string file_name = "/home/ajdillhoff/dev/projects/libhand-public/hand_model_no_arm/scene_spec.yml";
    /* string file_name = "/home/alex/dev/examples/libhand/hand_model/scene_spec.yml"; */

    // Process file
    SceneSpec scene_spec(file_name);

    // Renderer loads the scene
    hand_renderer_.LoadScene(scene_spec);

    // HandPose object that we will use to generate random poses
    FullHandPose hand_pose(scene_spec.num_bones());

    // Seed the RNG
    mt19937 rng(time(0));

    // Open a window using OpenCV
    string window_name = "Hand";
    cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

    HandCameraSpec camera_spec(distance_to_camera_);

    /* float theta = M_PI * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX + 1.)); */
    /* float phi = M_PI * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX + 1.)); */
    /* float tilt = M_PI * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX + 1.)); */
    uniform_real_distribution<float> gen(-1, 1);
    float phi = M_PI * gen(rng);

    gen = uniform_real_distribution<float>(-1, 0);
    float theta = M_PI * gen(rng);
    float tilt = M_PI * -gen(rng);

    camera_spec.theta = theta;
    camera_spec.phi = phi;
    camera_spec.tilt = tilt;

    // Set the camera_spec, controls the perspective we view the hand from
    hand_renderer_.set_camera_spec(camera_spec);

    /* // Get an openCV matrix of the rendered hand */
    /* cv::Mat pic = hand_renderer_.pixel_buffer_cv(); */

    /* // Tell OpenCV to show it */
    /* cv::imshow(window_name, pic); */
    /* cv::waitKey(); */

    // TODO: Only starting out with bends for now, skip the last 3 joints
    /* for (int i = 0; i < hand_pose.num_joints() - 3; i++) { */
        // Generate a random number between 0 and 2PI
        float r = -(M_PI / 4) * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX + 1.));

        // Set the values
        hand_pose.bend(9) = r;
    /* } */

    // Apply the hand pose
    hand_renderer_.SetHandPose(hand_pose);

    // Render the hand
    hand_renderer_.RenderHand();
}

// Returns a sample based on the currently generated pose.
// TODO: Implement error checking
PoseGenerator::PoseSample PoseGenerator::GetSample() {
    // Test GetMeshVertices
    cout << "[DEBUG] Loading vertices\n";
    size_t vertex_count = 0;
    Vector3* mesh_data;
    hand_renderer_.GetMeshVertices(vertex_count, mesh_data);

    // libhand uses `new` to create the mesh data, we will let `shared_ptr`
    // handle the lifetime from here on.
    shared_ptr<Vector3> mesh_data_(mesh_data);

    PoseSample d = PoseSample();
    d.mesh_data = mesh_data_;
    d.vertex_count = vertex_count;
    d.depth_buffer = hand_renderer_.depth_buffer_cv();
    hand_renderer_.walk_bones(d.joint_position_map);

    cout << "[DEBUG] Loaded vertices. Count: " << vertex_count << endl;
    /* int N = 100; */
    /* // print the first N vertices */
    /* cout << "[DEBUG] Printing " << N << " vertices:" << endl; */
    /* for (int i = 0; i < N; i++) { */
    /*     Vector3 vertex = vertices[i]; */
    /*     cout << "vertex " << i << " length " << vertex.length() << endl; */
    /*     cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << endl; */
    return d;
}
