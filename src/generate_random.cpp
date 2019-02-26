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
    file.open("../sphere/train/targets.txt");
    int num_samples = 1;

    p_gen->Setup();

    for (int i = 0; i < num_samples; i++) {
        p_gen->GeneratePose();
        PoseGenerator::PoseSample pose_sample = p_gen->GetSample();

        // Testing cv::setTo to get rid of the inf values
        pose_sample.depth_buffer.setTo(0, pose_sample.depth_buffer == std::numeric_limits<float>::infinity());

        cv::Mat test;

        double min = 0.5;
        double max = 8.0;
        cv::Mat mask = pose_sample.depth_buffer != 0;

        double min_t;
        double max_t;
        cv::minMaxLoc(pose_sample.depth_buffer, &min_t, &max_t, 0, 0, mask);
        cout << "min_t = " << min_t << ", max_t = " << max_t << endl;

        cout << "min = " << min << ", max = " << max << endl;

        double scale = 255 / (max - min);
        double b = -min * scale;

        pose_sample.depth_buffer.convertTo(test, CV_8UC1, scale, b);

        boost::format depth_fmt("../sphere/train/depth/%04d.png");
        boost::format color_fmt("../sphere/train/color/%04d.png");
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
