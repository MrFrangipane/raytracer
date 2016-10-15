#include "renderer.h"
#include <iostream>


namespace raytracer {

// Given a Scene, trace a pixel
Color trace(
        const std::vector<std::shared_ptr<Traceable>>* traceables,
        const Vector3 &origin,
        const double &scale,
        const double &aspect_ratio,
        const Camera* camera,
        const std::size_t &x,
        const std::size_t &y
    )
{
    // Pixel
    Color pixel;

    // Ray Direction
    double direction_x = (2 * (x + 0.5) / (double)camera->render_width - 1) * scale;
    double direction_y = (1 - 2 * (y + 0.5) / (double)camera->render_height) * scale / aspect_ratio;
    Vector3 direction(direction_x, direction_y, -1);
    direction = direction.as_direction_multiplied(camera->camera_to_world);
    direction.normalize();

    // Hit Distance
    double max_distance = kInfinity;
    double hit_object_index = -1;

    // Each Primitive
    for (int object_index = 0; object_index < traceables->size(); object_index++)
    {
        std::shared_ptr<Traceable> traceable = traceables->at(object_index);
        double hit_distance = traceable->hit_distance(origin, direction);

        if (hit_distance > 0 && hit_distance < max_distance)
        {
            max_distance = hit_distance;
            hit_object_index = object_index;
        }
    }

    // Object hit
    if (hit_object_index > -1)
    {
        pixel.red = traceables->at(hit_object_index)->diffuse_color.red;
        pixel.green = traceables->at(hit_object_index)->diffuse_color.green;
        pixel.blue = traceables->at(hit_object_index)->diffuse_color.blue;
    }

    return pixel;
}


// Given a Scene and a Buffer, trace all pixels
void render(const std::shared_ptr<Scene> scene, std::shared_ptr<Buffer> buffer, const std::size_t core_count)
{
    // Core count
    std::size_t cores = core_count;
    // Atomic
    volatile std::atomic<std::size_t> atomic_pixel_index(0);
    std::vector<std::future<void>> futures;

    // Pointer to first pixel of buffer
    Color* buffer_pixel = &buffer.get()->pixels[0];

    // Camera
    Camera* camera = scene.get()->cameras[0].get();

    double camera_scale = tan(deg_to_rad(camera->fov * 0.5));
    double camera_aspect_ratio = camera->render_width / (float)camera->render_height;

    Vector3 camera_origin(0, 0, 0);
    camera_origin = camera_origin.as_point_multiplied(camera->camera_to_world);

    // Traceables
    std::vector<std::shared_ptr<Traceable>>* traceables = &scene.get()->traceables;

    // Each Core
    while (cores--)
    {
        // Add to pool of threads
        futures.emplace_back(
            // Async call to loop
            std::async([=, &atomic_pixel_index]()
            {
                while (true)
                {
                    // Get Pixel to render
                    std::size_t pixel_index = atomic_pixel_index++;

                    // Exit if all pixel rendered
                    if (pixel_index >= buffer.get()->pixel_count)
                        break;

                    // Get x, y coord of pixel
                    std::size_t x = pixel_index % buffer.get()->width;
                    std::size_t y = pixel_index / buffer.get()->width;
                    // Trace and write to buffer memory
                    buffer_pixel[pixel_index] = trace(
                        traceables,
                        camera_origin,
                        camera_scale,
                        camera_aspect_ratio,
                        camera,
                        x, y
                    );
                }
            })
        );
    }
}

} // end namespace
