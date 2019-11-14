#ifndef POSE_CONFIG_H_
#define POSE_CONFIG_H_

#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <cmath>
#include <random>
#include <functional>
#include <fstream>

#include <json.hpp>


using namespace std;
using json = nlohmann::json;

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

    AngleParam(const json& j) {
        j.at("ids").get_to(joints);
        j.at("min").get_to(min_angle);
        j.at("max").get_to(max_angle);
        j.at("type").get_to(dim);
        j.at("coefficients").get_to(coefficients);
    }
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

struct PosParam {
    vector<int> joints;
    vector<float> min_pos;
    vector<float> max_pos;
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

class PoseConfig {
  public:
    PoseConfig();
    PoseConfig(string);
    ~PoseConfig() {}

    void LoadFile(string);
    unique_ptr<PoseParameters> GenerateParams();

  private:
    vector<string> joint_names_;
    vector<AngleParam> rotations_;
    PosParam hand_offset_;
};

#endif
