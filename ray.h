#ifndef RAY_H
#define RAY_H

#include "utils.h"
#include "vector.h"

namespace raytracer {

class Ray {
    public:
        // Members
        Vector3 origin, direction;
        Vector3 inverse_direction;
        int sign[3];

        // Constructor
        Ray(const Vector3 &origin_, const Vector3 &direction_)
            : origin(origin_), direction(direction_)
        {}

};

struct RayHit {
    int object_index = -1;
    double distance = infinity;
    Vector3 position;
};

} // end namespace

#endif // RAY_H
