#ifndef TRACER_H
#define TRACER_H

#include <memory>
#include <vector>
#include <thread>
#include <future>
#include <mutex>
#include "math.h"
#include "common/typedefs.h"
#include "common/constants.h"
#include "image/buffer.h"
#include "math/vector.h"
#include "math/ray.h"
#include "math/rayhit.h"
#include "node/scene.h"
#include "node/abstractnode.h"
#include "node/surfaceattributes.h"


namespace frangiray {

class Tracer
{
public:
    // Members
    const std::shared_ptr<Scene> scene;
    std::size_t width = 0;
    std::size_t height = 0;
    std::size_t pixel_count = 0;
    std::size_t camera_index = 0;
    Buffer buffer;
    bool is_over_rendering = true;

    // Constructors
    Tracer();

    Tracer(std::shared_ptr<Scene> &scene_, const size_t camera_index_, const size_t width_, const size_t height_)
        : scene(scene_),
          camera_index(camera_index_),
          width(width_), height(height_), pixel_count(width_ * height_),
          buffer(width_, height_) {}

    // Methods
    void reset_render();
    void render();
    std::size_t pixel_to_render();
    void contribute_to_pixel(const std::size_t pixel_index, const Pixel &pixel);
    void trace_from_camera();
    void trace(const Ray &ray, Pixel &target_pixel, const int recursion_depth = 0);
    void ray_cast(const Ray ray, RayHit &target_ray_hit);

private:
    // Members
    mutable std::mutex _render_pixel_mutex;
    std::size_t _render_pixel_x = 0;
    std::size_t _render_pixel_y = 0;
    std::size_t _render_call_counter = 0;
    std::size_t _render_iteration = 0;
    std::size_t _render_resolution_divider = OVERSCAN_STEP;
    std::size_t _pixel_size = (std::size_t)pow(2, _render_resolution_divider);
};

}

#endif // TRACER_H
