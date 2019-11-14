#include "PoseConfig.h"

#include <iostream>

float gen_rand_deg(float min_deg, float max_deg) {
    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});

    auto deg = rand() * (max_deg - min_deg) + min_deg;
    return deg * (M_PI / 180.0);
}

PoseConfig::PoseConfig() {

}

PoseConfig::PoseConfig(string file_name) {
    LoadFile(file_name);
}

void PoseConfig::LoadFile(string file_name) {
    ifstream param_file(file_name);
    json data;
    param_file >> data;

    for (auto& element : data.items()) {
        AngleParam temp = AngleParam(element.value());
        rotations_.push_back(temp);
    }
}

unique_ptr<PoseParameters> PoseConfig::GenerateParams() {
    unique_ptr<PoseParameters> params(new PoseParameters);

    for (int i = 0; i < rotations_.size(); i++) {
        float angle = gen_rand_deg(rotations_[i].min_angle, rotations_[i].max_angle);
        for (int j = 0; j < rotations_[i].joints.size(); j++) {
            int joint_idx = rotations_[i].joints[j];
            RotationType t = rotations_[i].dim[j];
            switch (t) {
                case kBend:
                    params->bend[joint_idx] += (angle * rotations_[i].coefficients[j]);
                    break;
                case kSide:
                    params->side[joint_idx] += (angle * rotations_[i].coefficients[j]);
                    break;
                case kTwist:
                    params->twist[joint_idx] += (angle * rotations_[i].coefficients[j]);
                    break;
                default:
                    break;
            }
        }
    }

    return params;
}
