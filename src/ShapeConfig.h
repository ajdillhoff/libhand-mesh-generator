#ifndef SHAPE_CONFIG_H_
#define SHAPE_CONFIG_H_

#include <string>

#include <json.hpp>
#include "ShapePrototype.h"


using namespace std;
using json = nlohmann::json;

namespace posegen {
class ShapeConfig {
  public:
    ShapeConfig();
    ShapeConfig(string);
    ~ShapeConfig() {}

    int LoadFile(string);
    unique_ptr<ShapeParameters> GenerateParams();

  private:
    vector<ShapePrototype> prototypes_;
};
}

#endif
