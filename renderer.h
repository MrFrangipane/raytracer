#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include <atomic>
#include <future>
#include <random>
#include "utils.h"
#include "ray.h"
#include "matrix.h"
#include "color.h"
#include "buffer.h"
#include "camera.h"
#include "scene.h"

namespace raytracer {

//Color trace(const Camera* &camera, const std::size_t &x, const std::size_t &y);

void render(std::shared_ptr<Scene> scene, std::shared_ptr<Buffer> buffer, const std::size_t core_count);

} // end namespace

#endif // RENDERER_H
