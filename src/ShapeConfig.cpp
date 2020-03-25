#include "ShapeConfig.h"

#include <iostream>
#include <random>

namespace posegen {
ShapeConfig::ShapeConfig() {

}

ShapeConfig::ShapeConfig(string file_name) {
    LoadFile(file_name);
}

int ShapeConfig::LoadFile(string file_name) {
    ifstream param_file(file_name);
    json data;
    param_file >> data;

    vector<string> paths = data.at("prototypes");

    for (string p : paths) {
        prototypes_.push_back(ShapePrototype(p));
    }

    return 0;
}

unique_ptr<ShapeParameters> ShapeConfig::GenerateParams() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(0, prototypes_.size() - 1);
    return prototypes_[uni(rng)].GenerateParams();
}
}
