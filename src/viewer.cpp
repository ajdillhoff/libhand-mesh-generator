#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/format.hpp>
#include "opencv2/opencv.hpp"
#include "PoseGenerator.h"

using namespace std;

int main(int argc, char **argv) {
    unique_ptr<PoseGenerator> p_gen(new PoseGenerator());
    p_gen->Setup();

    p_gen->GeneratePose();
    PoseGenerator::PoseSample pose_sample = p_gen->GetSample();

    // Testing cv::setTo to get rid of the inf values
    pose_sample.depth_buffer.setTo(0, pose_sample.depth_buffer == std::numeric_limits<float>::infinity());

    cv::Mat test;

    double min;
    double max;
    cv::minMaxIdx(pose_sample.depth_buffer, &min, &max);

    pose_sample.depth_buffer.convertTo(test, CV_8UC1, 255 / (max - min), -min);

    string win_name = "depth_window";
    cv::namedWindow(win_name);

    cv::imshow(win_name, test);
    cv::imshow(win_name, pose_sample.pixel_buffer);
    cv::waitKey();

    return 0;
}
