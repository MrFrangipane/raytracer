#ifndef RAY_H
#define RAY_H

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
        {
            inverse_direction = 1 / direction;
            sign[0] = (inverse_direction.x < 0);
            sign[1] = (inverse_direction.y < 0);
            sign[2] = (inverse_direction.z < 0);
        }

};

} // end namespace

#endif // RAY_H
