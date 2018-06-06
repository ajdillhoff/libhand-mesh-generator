#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

#include "OGRE/OgreVector3.h"
#include "opencv2/opencv.hpp"
#include "file_dialog.h"
#include "hand_pose.h"
#include "hand_renderer.h"
#include "scene_spec.h"

using namespace Ogre;
using namespace libhand;

using namespace std;

int main(int argc, char **argv) {
    // initial distance from hand
    float init_r = 6;

    try {
        HandRenderer hand_renderer;
        hand_renderer.Setup(800, 600);

        // Load scene spec file
        /* FileDialog dialog; */
        /* dialog.SetTitle("Please select a scene spec file"); */
        /* string file_name = dialog.Open(); */
        string file_name = "/home/alex/dev/projects/libhand-public/hand_model_no_arm/scene_spec.yml";
        /* string file_name = "/home/alex/dev/examples/libhand/hand_model/scene_spec.yml"; */

        // Process file
        SceneSpec scene_spec(file_name);

        // Renderer loads the scene
        hand_renderer.LoadScene(scene_spec);

        // HandPose object that we will use to generate random poses
        FullHandPose hand_pose(scene_spec.num_bones());

        // Seed the RNG
        mt19937 rng(time(0));

        // Open a window using OpenCV
        string window_name = "Hand";
        cv::namedWindow(window_name, CV_WINDOW_AUTOSIZE);

        HandCameraSpec camera_spec(init_r);

        while (true) {
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
            hand_renderer.set_camera_spec(camera_spec);

            // Render the hand
            hand_renderer.RenderHand();

            // Get an openCV matrix of the rendered hand
            cv::Mat pic = hand_renderer.pixel_buffer_cv();

            // Tell OpenCV to show it
            cv::imshow(window_name, pic);
            cv::waitKey();

            // TODO: Only starting out with bends for now, skip the last 3 joints
            for (int i = 0; i < hand_pose.num_joints() - 3; i++) {
                // Generate a random number between 0 and 2PI
                float r = -(M_PI / 2) * static_cast<float>(rand()) / (static_cast<float>(RAND_MAX + 1.));

                // Set the values
                hand_pose.bend(i) = r;
            }

            // Apply the hand pose
            hand_renderer.SetHandPose(hand_pose);
        }

        // Test GetMeshVertices
        /* cout << "[DEBUG] Loading vertices\n"; */
        /* Vector3* vertices; */
        /* size_t vertex_count = 0; */
        /* hand_renderer.GetMeshVertices(vertex_count, vertices); */

        /* cout << "[DEBUG] Loaded vertices. Count: " << vertex_count << endl; */

        /* int N = 100; */
        /* // print the first N vertices */
        /* cout << "[DEBUG] Printing " << N << " vertices:" << endl; */
        /* for (int i = 0; i < N; i++) { */
        /*     Vector3 vertex = vertices[i]; */
        /*     cout << "vertex " << i << " length " << vertex.length() << endl; */
        /*     cout << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")" << endl; */
        /* } */

    } catch (const std::exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
