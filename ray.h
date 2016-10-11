#ifndef RAY_H
#define RAY_H
#include "vector3.h"

namespace raytracer {

struct Ray {
    Vector3 origin();
    Vector3 direction();
};

} // end namespace

#endif // RAY_H
