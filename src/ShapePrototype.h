#ifndef SHAPE_POSEPROTOTYPE_H_
#define SHAPE_POSEPROTOTYPE_H_

#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <cmath>
#include <random>
#include <functional>
#include <fstream>
#include <iostream>

#include <json.hpp>

using namespace std;
using json = nlohmann::json;

namespace posegen {
enum AugmentType {
    kSwell = 0,
    kDilate = 1,
    kElongate = 2,
};

struct ShapeParam {
    vector<int> joints;
    float min_val;
    float max_val;
    vector<AugmentType> dim;
    vector<float> coefficients;
};

struct ShapeParameters {
    vector<float> swell;
    vector<float> dilate;
    vector<float> elongate;

    ShapeParameters() {
        swell = vector<float>(21, 1.0);
        dilate = vector<float>(21, 1.0);
        elongate = vector<float>(21, 1.0);
    }
};

class ShapePrototype {
  public:
    ShapePrototype();
    ShapePrototype(string);
    ~ShapePrototype() {}

    void LoadFile(string);
    unique_ptr<ShapeParameters> GenerateParams();

    string name() { return name_; }

  private:
    string name_;
    vector<ShapeParam> augmentations_;
};
}

#endif
