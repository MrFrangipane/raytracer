#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <memory>
#include <limits>
#include <shared_mutex>

namespace frangiray {

typedef double f_real;
typedef std::shared_lock<std::shared_mutex> f_shared_lock;
typedef std::unique_lock<std::shared_mutex> f_unique_lock;

const f_real F_INFINITY = std::numeric_limits<f_real>::max();

}

#endif // TYPEDEFS_H
