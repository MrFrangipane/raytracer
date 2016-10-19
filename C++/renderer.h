#ifndef RENDERER_H
#define RENDERER_H

#define MAX_RECURSION 5

#include <memory>
#include <vector>
#include <atomic>
#include <future>
#include <random>
#include "vector.h"
#include "utils.h"
#include "ray.h"
#include "matrix.h"
#include "color.h"
#include "buffer.h"
#include "camera.h"
#include "scene.h"

namespace raytracer {

RayHit ray_cast(const Ray &ray, const std::vector<std::shared_ptr<Traceable>> &traceables);

Color trace_from_camera(
        const Color &background_color,
        const std::vector<std::shared_ptr<Traceable>> &traceables,
        const Vector3 &origin,
        const double &scale,
        const double &aspect_ratio,
        const std::shared_ptr<Camera> &camera,
        const std::size_t &x,
        const std::size_t &y
    );

Color trace(
        const Color &background_color,
        const std::vector<std::shared_ptr<Traceable>> &traceables,
        const Vector3 &origin,
        const Vector3 &direction,
        const int recursion_depth = 0
    );

void render(const std::shared_ptr<Scene> &scene, std::shared_ptr<Buffer> &buffer, const std::size_t core_count);

} // end namespace

#endif // RENDERER_H
