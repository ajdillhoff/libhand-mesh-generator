#include "ShapePrototype.h"

float gen_rand_val(float min_val, float max_val) {
    auto rand = std::bind(std::uniform_real_distribution<float>{0, 1},
            std::default_random_engine{std::random_device()()});

    auto val = rand() * (max_val - min_val) + min_val;
    return val;
}

namespace posegen {
void from_json(const json &j, ShapeParam& a) {
    j.at("ids").get_to(a.joints);
    j.at("min").get_to(a.min_val);
    j.at("max").get_to(a.max_val);
    j.at("type").get_to(a.dim);
    j.at("coefficients").get_to(a.coefficients);
}

void ShapePrototype::LoadFile(string file_name) {
    ifstream param_file(file_name);
    json data;
    param_file >> data;

    data.at("name").get_to(name_);
    data.at("shapeparams").get_to(augmentations_);
}

ShapePrototype::ShapePrototype() {}

ShapePrototype::ShapePrototype(string config_path) {
    LoadFile(config_path);
}

unique_ptr<ShapeParameters> ShapePrototype::GenerateParams() {
    unique_ptr<ShapeParameters> params(new ShapeParameters);

    for (int i = 0; i < augmentations_.size(); i++) {
        float val = gen_rand_val(augmentations_[i].min_val, augmentations_[i].max_val);
        for (int j = 0; j < augmentations_[i].joints.size(); j++) {
            int joint_idx = augmentations_[i].joints[j];
            AugmentType t = augmentations_[i].dim[j];
            switch (t) {
                case kSwell:
                    params->swell[joint_idx] += (val * augmentations_[i].coefficients[j]);
                    break;
                case kDilate:
                    params->dilate[joint_idx] += (val * augmentations_[i].coefficients[j]);
                    break;
                case kElongate:
                    params->elongate[joint_idx] += (val * augmentations_[i].coefficients[j]);
                    break;
                default:
                    break;
            }
        }
    }

    return params;
}
}
