#ifndef UTILS_H
#define UTILS_H
#define _USE_MATH_DEFINES

#include "math.h"
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include "vector.h"

namespace raytracer {

const double infinity = std::numeric_limits<double>::max();

inline
double deg_to_rad(const double &angle)
{ return angle * M_PI / 180; }

inline
double random_double() {
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(-10000,10000);
    return ((double)distribution(generator) / 10000);
}

inline
Vector3 random_direction()
{
    return Vector3(
        random_double(),
        random_double(),
        random_double()
    ).normalize();
}

} // end namespace

#endif // UTILS_H
