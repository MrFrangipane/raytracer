#include "tracer.h"

namespace frangiray {

// Reset Render
void Tracer::reset_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->_render_pixel_mutex);

    // Reset Values
    _render_pixel_x = 0;
    _render_pixel_y = 0;
    _render_call_counter = 0;
    _render_iteration = 0;
    _render_resolution_divider = OVERSCAN_STEP;
    _pixel_size = (std::size_t)pow(2, _render_resolution_divider);
    is_over_rendering = true;
}

// Render
void Tracer::render()
{
    // Core count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;
    // Atomic
    std::vector<std::future<void>> futures;

    // Each Core
    while (core_count--)
    {
        // Add to pool of threads
        futures.emplace_back(
            // Async call to loop
            std::async([=]()
            {
                while(true)
                {
                    trace_from_camera();
                }
            })
        );
    }
}


// Pixel to render
std::size_t Tracer::pixel_to_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->_render_pixel_mutex);

    if (is_over_rendering)
    {
        _render_pixel_x = (_render_call_counter * _pixel_size) % width;
        _render_pixel_y = ((_render_call_counter * _pixel_size) / width) * _pixel_size;

        std::size_t render_pixel_index = (_render_pixel_x + _render_pixel_y * width);

        if (render_pixel_index >= pixel_count)
        {
            _render_resolution_divider--;
            _pixel_size = (std::size_t)pow(2, _render_resolution_divider);
            if (_render_resolution_divider == 0) is_over_rendering = false;
            render_pixel_index = 0;
            _render_pixel_x = 0;
            _render_pixel_y = 0;
            _render_call_counter = 0;
        }

        _render_call_counter++;

        return render_pixel_index;
    }
    else
    {
        if (_render_call_counter >= pixel_count)
        {
            _render_iteration++;
            _render_call_counter = 0;
        }
        return _render_call_counter++;
    }
}


// Contribute to Pixel
void Tracer::contribute_to_pixel(const std::size_t pixel_index, const Pixel &pixel)
{
    // Mutex Lock
    //std::lock_guard<std::mutex> guard(this->_render_pixel_mutex);

    if (is_over_rendering)
    {
        std::size_t offset;
        for (int x = 0; x <= _pixel_size; x++) {
        for (int y = 0; y <= _pixel_size; y++) {
            // Offset
            offset = pixel_index + x + y * width;
            // Safely write pixel
            if (offset < pixel_count) *buffer.pixels[offset] = pixel;
        }}
    }
    else
    {
        *buffer.pixels[pixel_index] = pixel;
        // buffer.pixels[pixel_index] * _render_iteration + pixel) / (_render_iteration + 1)
    }
}


// Trace from camera
void Tracer::trace_from_camera()
{
    // Pixel to render
    Pixel pixel;
    std::size_t pixel_index = pixel_to_render();

    // Get x, y coord of pixel
    std::size_t x = pixel_index % width;
    std::size_t y = pixel_index / width;

    // Camera
    std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(scene->node_at(0));
    f_real scale = tan(deg_to_rad(camera->fov * 0.5));
    f_real aspect_ratio = (f_real)width / (f_real)height;

    // Deviation
    f_real deviation_x = random_real() / 2;
    f_real deviation_y = random_real() / 2;

    // Camera Ray
    f_real direction_x = (2 * (x + deviation_x) / (f_real)width - 1) * scale;
    f_real direction_y = (1 - 2 * (y + deviation_y) / (f_real)height) * scale / aspect_ratio;
    Vector ray_direction(direction_x, direction_y, -1);
    ray_direction = ray_direction.as_direction_multiplied(camera->local_to_world);
    ray_direction.normalize();

    Ray camera_ray(camera->position(), ray_direction);

    // Trace
    trace(camera_ray, pixel);

    // Write buffer
    contribute_to_pixel(pixel_index, pixel);
}


// Trace
void Tracer::trace(const Ray &ray, Pixel &target_pixel, const int recursion_depth)
{
    // Recursion Limit
    if (recursion_depth > MAX_RECURSION) return;

    // Primary Ray Cast
    RayHit primary_hit;
    ray_cast(ray, primary_hit);

    // Exit If no hit
    if (primary_hit.distance <= 0 || primary_hit.distance >= F_INFINITY) return;

    // Get Hit object
    std::shared_ptr<AbstractNode> primary_hit_node = scene->node_at(primary_hit.node_index);

    // Surface Attributes
    SurfaceAttributes primary_hit_surface = primary_hit_node->surface_attributes_at(primary_hit.position);

    // Store Object ID
    target_pixel.object_id = primary_hit.node_index + 1;

    target_pixel.beauty.r = primary_hit_surface.normal[0] * 255;
    target_pixel.beauty.g = primary_hit_surface.normal[1] * 255;
    target_pixel.beauty.b = primary_hit_surface.normal[2] * 255;
}


// Ray Cast
void Tracer::ray_cast(const Ray ray, RayHit &target_ray_hit)
{
    // Init values
    f_real min_hit_distance = F_INFINITY;
    std::size_t hit_node_index = 0;
    std::size_t node_count = scene->node_count();

    // Each Node
    for (int node_index = 0; node_index < node_count; node_index++)
    {
        std::shared_ptr<AbstractNode> node = scene->node_at(node_index);
        // Skip if not traceable
        if (!node->traceable()) continue;

        // Hit distance
        f_real hit_distance = node->intersection_distance(ray);

        // If closer for this object
        if (hit_distance > 0 && hit_distance < min_hit_distance)
        {
            min_hit_distance = hit_distance;
            hit_node_index = node_index;
        }
    }

    // Update Hit Position
    target_ray_hit.distance = min_hit_distance;
    target_ray_hit.node_index = hit_node_index;
    target_ray_hit.position = ray.origin + ray.direction * min_hit_distance;
}

}
