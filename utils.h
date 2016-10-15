#ifndef UTILS_H
#define UTILS_H
#define _USE_MATH_DEFINES

#include "math.h"
#include <cmath>
#include <limits>

namespace raytracer {

const double infinity = std::numeric_limits<double>::max();

inline
double deg_to_rad(const double &angle)
{ return angle * M_PI / 180; }

} // end namespace

#endif // UTILS_H
