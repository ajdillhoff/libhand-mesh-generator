#include "PoseConfig.h"

#include <iostream>
#include <random>

namespace posegen {
PoseConfig::PoseConfig() {

}

PoseConfig::PoseConfig(string file_name) {
    LoadFile(file_name);
}

int PoseConfig::LoadFile(string file_name) {
    ifstream param_file(file_name);
    json data;
    param_file >> data;

    vector<string> paths = data.at("prototypes");

    for (string p : paths) {
        prototypes_.push_back(PosePrototype(p));
    }

    return 0;
}

unique_ptr<PoseParameters> PoseConfig::GenerateParams() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(0, prototypes_.size() - 1);
    return prototypes_[uni(rng)].GenerateParams();
}
}
