#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "opencv2/opencv.hpp"
#include "PoseGenerator.h"
#include "PoseConfig.h"

using namespace std;


struct Args {
    string config_path = "config.json";
    string output_path = "out/";
    int num_samples = 0;
};


void PrintHelp() {
    cout <<
        "-c, --config_path: Path to configuration file.\n"
        "-n, --num_samples: Number of samples to generate.\n"
        "-o, --output_path: Path to save generated samples.\n";
}


Args ProcessArgs(int argc, char** argv) {
    const char* const short_opts = "c:n:o:";
    const option long_opts[] = {
        {"config_path", no_argument, nullptr, 'c'},
        {"num_samples", no_argument, nullptr, 'n'},
        {"output_path", no_argument, nullptr, 'o'}
    };

    Args a = Args();

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (opt == -1) {
            break;
        }

        switch (opt) {
            case 'c':
                a.config_path = optarg;
                break;
            case 'n':
                a.num_samples = stoi(optarg);
                break;
            case 'o':
                a.output_path = optarg;
                break;
            default:
                PrintHelp();
                exit(1);
                break;
        }
    }

    return a;
}


int main(int argc, char **argv) {
    Args a = ProcessArgs(argc, argv);

    unique_ptr<PoseGenerator> p_gen(new PoseGenerator());
    ofstream file;
    boost::filesystem::path dir(a.output_path);
    boost::filesystem::create_directory(dir);
    file.open(a.output_path + "targets.txt");
    int num_samples = a.num_samples;
    posegen::PoseConfig config = posegen::PoseConfig("../config.json");

    p_gen->Setup();

    for (int i = 0; i < num_samples; i++) {
        p_gen->GeneratePose(config.GenerateParams());
        PoseGenerator::PoseSample pose_sample = p_gen->GetSample();

        // Testing cv::setTo to get rid of the inf testues
        pose_sample.depth_buffer.setTo(0, pose_sample.depth_buffer == std::numeric_limits<float>::infinity());

        cv::Mat test;

        double min = 0.1;
        double max = 8.0;
        cv::Mat mask = pose_sample.depth_buffer != 0;

        double min_t;
        double max_t;
        cv::minMaxLoc(pose_sample.depth_buffer, &min_t, &max_t, 0, 0, mask);

        //double scale = 255 / (max - min);
        //double b = -min * scale;

        //pose_sample.depth_buffer.convertTo(test, CV_8UC1, scale, b);
        pose_sample.depth_buffer.convertTo(test, CV_16UC1, 1000.0);

        boost::filesystem::path depth_dir("../nyu_synth_256k/val/depth/");
        boost::filesystem::create_directory(depth_dir);
        boost::filesystem::path color_dir("../nyu_synth_256k/val/color/");
        boost::filesystem::create_directory(color_dir);
        boost::format depth_fmt("../nyu_synth_256k/val/depth/%d.png");
        boost::format color_fmt("../nyu_synth_256k/val/color/%d.png");
        depth_fmt % i;
        color_fmt % i;
        cv::imwrite(depth_fmt.str(), test);
        cv::imwrite(color_fmt.str(), pose_sample.pixel_buffer);

        // Write annotations to file
        for (const auto &p : pose_sample.joint_position_map) {
            file << p.first << " " << p.second[0] << " " << p.second[1] << " " << p.second[2] * -1000.0 << std::endl;
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
