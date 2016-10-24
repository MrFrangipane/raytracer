#ifndef RAYHIT_H
#define RAYHIT_H

#include "common/typedefs.h"
#include "math/vector.h"

namespace frangiray {

struct RayHit {
    size_t node_index = 0;
    f_real distance = F_INFINITY;
    Vector position;
};

}
#endif // RAYHIT_H
