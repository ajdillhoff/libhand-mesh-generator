#include "PoseConfig.h"


int main(int argc, char** argv) {
    PoseConfig p = PoseConfig();
    p.LoadFile("test.json");

    unique_ptr<PoseParameters> params = p.GenerateParams();

    return 0;
}
