#ifndef RAY_H
#define RAY_H

#include "math/vector.h"

namespace frangiray {

struct Ray {
    // Members
    Vector origin, direction;

    // Constructor
    Ray(const Vector &origin_, const Vector &direction_)
        : origin(origin_), direction(direction_)
    {}
};

}

#endif // RAY_H
