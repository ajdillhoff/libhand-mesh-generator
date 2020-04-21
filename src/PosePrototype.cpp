#include "PosePrototype.h"

float gen_rand_deg(float min_deg, float max_deg) {
    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});

    auto deg = rand() * (max_deg - min_deg) + min_deg;
    return deg * (M_PI / 180.0);
}

float get_rand_pos(float min_pos, float max_pos) {
    auto rand = std::bind(std::uniform_real_distribution<float>{min_pos, max_pos},
         std::default_random_engine{std::random_device()()});

    return rand();
}

namespace posegen {
void from_json(const json &j, AngleParam& a) {
    j.at("ids").get_to(a.joints);
    j.at("min").get_to(a.min_angle);
    j.at("max").get_to(a.max_angle);
    j.at("type").get_to(a.dim);
    j.at("coefficients").get_to(a.coefficients);
}

void from_json(const json &j, PosParam& p) {
    j.at("x_range").get_to(p.x_range);
    j.at("y_range").get_to(p.y_range);
    j.at("z_range").get_to(p.z_range);
}

void PosePrototype::LoadFile(string file_name) {
    ifstream param_file(file_name);
    json data;
    param_file >> data;

    data.at("name").get_to(name_);
    data.at("angleparams").get_to(rotations_);
    data.at("hand_offset").get_to(hand_offset_);
}

PosePrototype::PosePrototype() {}

PosePrototype::PosePrototype(string config_path) {
    LoadFile(config_path);
}

unique_ptr<PoseParameters> PosePrototype::GenerateParams() {
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

    // Position offset
    params->hand_offset[0] = get_rand_pos(hand_offset_.x_range[0], hand_offset_.x_range[1]);
    params->hand_offset[1] = get_rand_pos(hand_offset_.y_range[0], hand_offset_.y_range[1]);
    params->hand_offset[2] = get_rand_pos(hand_offset_.z_range[0], hand_offset_.z_range[1]);

    return params;
}
}
