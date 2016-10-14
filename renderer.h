#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <atomic>
#include <future>
#include <random>
#include "ray.h"
#include "matrix.h"
#include "color.h"
#include "buffer.h"
#include "camera.h"

namespace raytracer {

Color trace();

void render(std::shared_ptr<Buffer> buffer, const std::size_t core_count);

} // end namespace

#endif // RENDERER_H
