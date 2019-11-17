#ifndef POSE_POSEPROTOTYPE_H_
#define POSE_POSEPROTOTYPE_H_

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
enum RotationType {
    kBend = 0,
    kSide = 1,
    kTwist = 2,
};

struct AngleParam {
    vector<int> joints;
    float min_angle;
    float max_angle;
    vector<RotationType> dim;
    vector<float> coefficients;
};

struct PoseParameters {
    vector<float> bend;
    vector<float> side;
    vector<float> twist;
    vector<float> hand_offset;

    PoseParameters() {
        bend = vector<float>(17, 0.0);
        side = vector<float>(17, 0.0);
        twist = vector<float>(17, 0.0);
        hand_offset = vector<float>(3, 0.0);
    }
};

struct PosParam {
    vector<int> joints;
    vector<float> min_pos;
    vector<float> max_pos;
};

template <typename T>
inline ostream& operator<< (ostream& out, const vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

inline ostream& operator<< (ostream& os, const AngleParam& param) {
    return os << "Joint IDs: " << param.joints << endl
              << "Min angle: " << param.min_angle << endl
              << "Max angle: " << param.max_angle << endl
              << "Rotations: " << param.dim << endl
              << "Coefficients: " << param.coefficients << endl;
}

class PosePrototype {
  public:
    PosePrototype();
    PosePrototype(string);
    ~PosePrototype() {}

    void LoadFile(string);
    unique_ptr<PoseParameters> GenerateParams();

    string name() { return name_; }

  private:
    string name_;
    vector<AngleParam> rotations_;
};
}

#endif
