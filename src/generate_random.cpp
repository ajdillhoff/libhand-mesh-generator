#include <memory>
#include <iostream>
#include <fstream>

#include "opencv2/opencv.hpp"
#include "PoseGenerator.h"

using namespace std;

int main(int argc, char **argv) {
    unique_ptr<PoseGenerator> p_gen(new PoseGenerator());

    p_gen->GeneratePose();
    PoseGenerator::PoseSample pose_sample = p_gen->GetSample();

    cout << "[DEBUG] generate_random.cpp main\tVertex Count: " << pose_sample.vertex_count << endl;

    for (const auto &p : pose_sample.joint_position_map) {
        cout << p.first << "\t" << p.second << endl;
    }

    string win_name = "depth_window";
    cv::namedWindow(win_name);

    // Testing cv::setTo to get rid of the inf values
    pose_sample.depth_buffer.setTo(0, pose_sample.depth_buffer == std::numeric_limits<float>::infinity());

    cv::Mat test;

    double min;
    double max;
    cv::minMaxIdx(pose_sample.depth_buffer, &min, &max);

    pose_sample.depth_buffer.convertTo(test, CV_8UC1, 255 / (max - min), -min);

    cv::imshow(win_name, test);
    cv::waitKey();

    // Write to file
    ofstream file;
    file.open("../samples/target.txt");
    for (const auto &p : pose_sample.joint_position_map) {
        file << p.first << " " << p.second[0] << " " << p.second[1] << " " << p.second[2] << std::endl;
    }
    file.close();
    cv::imwrite("../samples/test.png", test);

    std::cout << "Files written." << std::endl;

    return 0;
}
