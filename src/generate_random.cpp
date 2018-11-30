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
    ofstream file;
    file.open("../train/targets.txt");
    int num_samples = 5000;

    p_gen->Setup();

    for (int i = 0; i < num_samples; i++) {
        p_gen->GeneratePose();
        PoseGenerator::PoseSample pose_sample = p_gen->GetSample();

        // Testing cv::setTo to get rid of the inf values
        pose_sample.depth_buffer.setTo(0, pose_sample.depth_buffer == std::numeric_limits<float>::infinity());

        cv::Mat test;

        double min;
        double max;
        cv::minMaxIdx(pose_sample.depth_buffer, &min, &max);

        pose_sample.depth_buffer.convertTo(test, CV_8UC1, 255 / (max - min), -min);

        boost::format depth_fmt("../train/depth/%04d.png");
        boost::format color_fmt("../train/color/%04d.png");
        depth_fmt % i;
        color_fmt % i;
        cv::imwrite(depth_fmt.str(), test);
        cv::imwrite(color_fmt.str(), pose_sample.pixel_buffer);

        // Write annotations to file
        for (const auto &p : pose_sample.joint_position_map) {
            file << p.first << " " << p.second[0] << " " << p.second[1] << " " << p.second[2] << std::endl;
        }

        cout << depth_fmt.str() << " written." << endl;
    }

    /* string win_name = "depth_window"; */
    /* cv::namedWindow(win_name); */

    /* cv::imshow(win_name, test); */
    /* cv::imshow(win_name, pose_sample.pixel_buffer); */
    /* cv::waitKey(); */

    file.close();

    std::cout << "Files written." << std::endl;

    return 0;
}
