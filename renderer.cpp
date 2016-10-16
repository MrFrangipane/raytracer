#include "renderer.h"
#include <iostream>

#define ANTIAL_SAMPLES 1
#define SHADOW_SAMPLES 1
#define ENVIRO_SAMPLES 1

namespace raytracer {

RayHit ray_cast(const Ray &ray, const std::vector<std::shared_ptr<Traceable>> &traceables)
{
    // Hit
    RayHit ray_hit;

    // Each Primitive
    for (int object_index = 0; object_index < traceables.size(); object_index++)
    {
        std::shared_ptr<Traceable> traceable = traceables.at(object_index);
        double hit_distance = traceable->hit_distance(ray);

        if (hit_distance > 0 && hit_distance < ray_hit.distance)
        {
            ray_hit.distance = hit_distance;
            ray_hit.object_index = object_index;
        }
    }

    // Hit Position
    ray_hit.position = ray.origin + ray.direction * ray_hit.distance;

    return ray_hit;
}


// Trace a pixel
Color trace(
        const Color &background_color,
        const std::vector<std::shared_ptr<Traceable>> &traceables,
        const Vector3 &origin,
        const double &scale,
        const double &aspect_ratio,
        const std::shared_ptr<Camera> &camera,
        const std::size_t &x,
        const std::size_t &y
    )
{
    // Pixel
    Color pixel;

    // Each Antialiasing Sample
    for (int aa_sample_x = 0; aa_sample_x < ANTIAL_SAMPLES; aa_sample_x++)
    for (int aa_sample_y = 0; aa_sample_y < ANTIAL_SAMPLES; aa_sample_y++)
    {{
        // Deviation
        double deviation_x = (double)aa_sample_x / (double)ANTIAL_SAMPLES;
        double deviation_y = (double)aa_sample_y / (double)ANTIAL_SAMPLES;
        // Ray Direction
        double direction_x = (2 * (x + deviation_x) / (double)camera->render_width - 1) * scale;
        double direction_y = (1 - 2 * (y + deviation_y) / (double)camera->render_height) * scale / aspect_ratio;
        Vector3 direction(direction_x, direction_y, -1);
        direction = direction.as_direction_multiplied(camera->camera_to_world);
        direction.normalize();

        // Hit
        Ray primary_ray(origin, direction);
        RayHit primary_hit = ray_cast(primary_ray, traceables);

        // Primary hit miss, exit
        if (primary_hit.object_index == -1)
        {
            pixel.red += background_color.red / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
            pixel.green += background_color.green / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
            pixel.blue += background_color.blue / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
            // Next AA Sample
            continue;
        }

        // Hit Traceable
        std::shared_ptr<Traceable> primary = traceables.at(primary_hit.object_index);

        // Surface attributes
        SurfaceAttributes primary_surface = primary->surface_attributes_at(primary_hit.position);

        // Emissive Color
        pixel.red += primary_surface.emission_color.red / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
        pixel.green += primary_surface.emission_color.green / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
        pixel.blue += primary_surface.emission_color.blue / ANTIAL_SAMPLES / ANTIAL_SAMPLES;

        // Each Shadow Sample
        for (int shadow_sample = 0; shadow_sample < SHADOW_SAMPLES; shadow_sample++)
        {
            // Each Traceable
            for (int emittor_index = 0; emittor_index < traceables.size(); emittor_index++)
            {
                // Skip self
                if (emittor_index == primary_hit.object_index) continue;

                // Traceable
                std::shared_ptr<Traceable> emittor = traceables.at(emittor_index);

                // Skip if not Emission
                if (emittor->emission_color.red == 0 && emittor->emission_color.green == 0 && emittor->emission_color.blue == 0) continue;

                // Ray Direction
                Vector3 shadow_ray_direction = emittor->random_position() - primary_hit.position;
                shadow_ray_direction.normalize();

                // Skip if backface
                double dot_product = primary_surface.normal.dot_product(shadow_ray_direction);
                if (dot_product < 0) continue;

                // Ray Cast
                Ray shadow_ray(primary_hit.position, shadow_ray_direction);
                RayHit emittor_hit = ray_cast(shadow_ray, traceables);

                // Emittor Hit
                if (emittor_hit.object_index == emittor_index)
                {
                    // Surface attributes
                    SurfaceAttributes emittor_surface = emittor->surface_attributes_at(emittor_hit.position);

                    // Basic Shading
                    pixel.red += emittor_surface.emission_color.red * dot_product * primary->diffuse_color.red / SHADOW_SAMPLES / (emittor_hit.distance + 1) / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
                    pixel.green += emittor_surface.emission_color.green * dot_product * primary->diffuse_color.green / SHADOW_SAMPLES / (emittor_hit.distance + 1) / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
                    pixel.blue += emittor_surface.emission_color.blue * dot_product * primary->diffuse_color.blue / SHADOW_SAMPLES / (emittor_hit.distance + 1) / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
                }
            }
        }

        // Environment
        for (int environment_sample = 0; environment_sample < ENVIRO_SAMPLES; environment_sample++)
        {
            // Random Ray
            Vector3 environment_ray_directon = primary_surface.normal + random_direction();
            environment_ray_directon.normalize();
            double factor = primary_surface.normal.dot_product(environment_ray_directon);

            // Skip if backface
            if (factor < 0) continue;

            Ray environment_ray(primary_hit.position, environment_ray_directon);
            // Cast
            RayHit environment_hit = ray_cast(environment_ray, traceables);
            // If missed, background color
            if (environment_hit.object_index == -1)
            {
                pixel.red += primary->diffuse_color.red * background_color.red * factor / ENVIRO_SAMPLES / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
                pixel.green += primary->diffuse_color.green * background_color.green * factor / ENVIRO_SAMPLES / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
                pixel.blue += primary->diffuse_color.blue * background_color.blue * factor / ENVIRO_SAMPLES / ANTIAL_SAMPLES / ANTIAL_SAMPLES;
            }
        }
    }}
    return pixel;
}


// Given a Scene and a Buffer, trace all pixels
void render(std::shared_ptr<Progressive> progressive, const std::shared_ptr<Scene> &scene, std::shared_ptr<Buffer> &buffer, const std::size_t core_count)
{
    // Core count
    std::size_t cores = core_count;
    // Atomic
    std::vector<std::future<void>> futures;

    // Pointer to first pixel of buffer
    Color* buffer_pixel = &buffer->pixels[0];

    // Camera
    std::shared_ptr<Camera> camera = scene->cameras[0];

    // Traceables
    std::vector<std::shared_ptr<Traceable>> traceables = scene->traceables;

    // Each Core
    while (cores--)
    {
        // Add to pool of threads
        futures.emplace_back(
            // Async call to loop
            std::async([=, &progressive, &scene]()
            {
                while (true)
                {
                    double camera_scale = tan(deg_to_rad(camera->fov * 0.5));
                    double camera_aspect_ratio = camera->render_width / (float)camera->render_height;

                    Vector3 camera_origin(0, 0, 0);
                    camera_origin = camera_origin.as_point_multiplied(camera->camera_to_world);

                    // Get Pixel to render
                    std::size_t pixel_index = progressive->pixel_to_render();

                    /*
                    // Exit if all pixel rendered
                    if (pixel_index >= buffer->pixel_count)
                        break;
                    */

                    // Get x, y coord of pixel
                    std::size_t x = pixel_index % buffer->width;
                    std::size_t y = pixel_index / buffer->width;
                    // Trace and write to buffer memory
                    buffer_pixel[pixel_index] = trace(
                        scene->background,
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
