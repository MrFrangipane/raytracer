#ifndef UTILS_H
#define UTILS_H
#define _USE_MATH_DEFINES

#include "math.h"
#include <cmath>
#include <limits>
#include "vector.h"

namespace raytracer {

const double infinity = std::numeric_limits<double>::max();

inline
double deg_to_rad(const double &angle)
{ return angle * M_PI / 180; }

inline
Vector3 random_direction()
{ return Vector3(((double)rand() / RAND_MAX) - 0.5, ((double)rand() / RAND_MAX) - 0.5, ((double)rand() / RAND_MAX) - 0.5).normalize(); }

} // end namespace

#endif // UTILS_H
