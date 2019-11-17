#ifndef POSE_CONFIG_H_
#define POSE_CONFIG_H_

#include <string>

#include <json.hpp>
#include "PosePrototype.h"


using namespace std;
using json = nlohmann::json;

namespace posegen {
class PoseConfig {
  public:
    PoseConfig();
    PoseConfig(string);
    ~PoseConfig() {}

    int LoadFile(string);
    unique_ptr<PoseParameters> GenerateParams();

  private:
    vector<PosePrototype> prototypes_;
};
}

#endif
