#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#define _USE_MATH_DEFINES

#include "math.h"
#include <random>
#include <memory>
#include <limits>
#include <shared_mutex>

namespace frangiray {

typedef double f_real;
typedef std::shared_lock<std::shared_mutex> f_shared_lock;
typedef std::unique_lock<std::shared_mutex> f_unique_lock;

const f_real F_INFINITY = std::numeric_limits<f_real>::max();


inline
f_real deg_to_rad(const f_real &angle)
{ return angle * M_PI / 180; }


inline
f_real random_real() {
    static thread_local std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(-10000, 10000);
    return ((f_real)distribution(generator) / 10000);
}


inline
uint32_t r_g_b_a(int r, int g, int b, int a) {
    return ((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
}


}

#endif // TYPEDEFS_H
