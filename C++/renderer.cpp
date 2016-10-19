#include "renderer.h"
#include <iostream>

#define SHADOW_SAMPLES 4
#define ENVIRO_SAMPLES 4

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

// Trace from camera
Color trace_from_camera(
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
    // Deviation
    double deviation_x = random_double() / 2;
    double deviation_y = random_double() / 2;

    // Ray Direction
    double direction_x = (2 * (x + deviation_x) / (double)camera->render_width - 1) * scale;
    double direction_y = (1 - 2 * (y + deviation_y) / (double)camera->render_height) * scale / aspect_ratio;
    Vector3 direction(direction_x, direction_y, -1);
    direction = direction.as_direction_multiplied(camera->camera_to_world);
    direction.normalize();

    // Actually trace
    return trace(
        background_color,
        traceables,
        origin,
        direction
    );
}


// Trace a pixel
Color trace(
        const Color &background_color,
        const std::vector<std::shared_ptr<Traceable>> &traceables,
        const Vector3 &origin,
        const Vector3 &direction,
        const int recursion_depth
    )
{
    // Pixel
    Color pixel;

    // Recursion Limit
    if (recursion_depth > MAX_RECURSION) return pixel;


    // Hit
    Ray primary_ray(origin, direction);
    RayHit primary_hit = ray_cast(primary_ray, traceables);

    // Primary hit miss, exit
    if (primary_hit.object_index == -1)
    {
        pixel.red = background_color.red;
        pixel.green = background_color.green;
        pixel.blue = background_color.blue;
        return pixel;
    }

    // Hit Traceable
    std::shared_ptr<Traceable> primary = traceables.at(primary_hit.object_index);

    // Surface attributes
    SurfaceAttributes primary_surface = primary->surface_attributes_at(primary_hit.position);

    // Emissive Color
    pixel.red += primary_surface.emission_color.red;
    pixel.green += primary_surface.emission_color.green;
    pixel.blue += primary_surface.emission_color.blue;

    // Incidence
    double incidence = 1.0 - direction.dot_product(primary_surface.normal);

    // Reflection
    if (primary_surface.reflection_amount > 0)
    {
        // Prepare direction
        Vector3 reflection_direction = primary_surface.normal;

        // If blurry
        if (primary_surface.reflection_blur > 0)
        {
            reflection_direction = reflection_direction + (random_direction() * primary_surface.reflection_blur);
            reflection_direction.normalize();
        }

        // Recurse
        Color reflected_color = trace(
            background_color,
            traceables,
            primary_hit.position,
            reflection_direction,
            recursion_depth + 1
        );

        // Add
        pixel.red += reflected_color.red * primary_surface.reflection_amount * incidence;
        pixel.green += reflected_color.green * primary_surface.reflection_amount * incidence;
        pixel.blue += reflected_color.blue * primary_surface.reflection_amount * incidence;
    }

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
                pixel.red += emittor_surface.emission_color.red * dot_product * primary->diffuse_color.red / SHADOW_SAMPLES / (emittor_hit.distance + 1);
                pixel.green += emittor_surface.emission_color.green * dot_product * primary->diffuse_color.green / SHADOW_SAMPLES / (emittor_hit.distance + 1);
                pixel.blue += emittor_surface.emission_color.blue * dot_product * primary->diffuse_color.blue / SHADOW_SAMPLES / (emittor_hit.distance + 1);
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
            pixel.red += primary->diffuse_color.red * background_color.red * factor / ENVIRO_SAMPLES;
            pixel.green += primary->diffuse_color.green * background_color.green * factor / ENVIRO_SAMPLES;
            pixel.blue += primary->diffuse_color.blue * background_color.blue * factor / ENVIRO_SAMPLES;
        }
    }
    return pixel;
}


// Given a Scene and a Buffer, trace all pixels
void render(const std::shared_ptr<Scene> &scene, std::shared_ptr<Buffer> &buffer, const std::size_t core_count)
{
    // Core count
    std::size_t cores = core_count;
    // Atomic
    std::vector<std::future<void>> futures;

    // Each Core
    while (cores--)
    {
        // Add to pool of threads
        futures.emplace_back(
            // Async call to loop
            std::async([=, &buffer, &scene]()
            {
                while (true)
                {
                    double camera_scale = tan(deg_to_rad(scene->cameras[0]->fov * 0.5));
                    double camera_aspect_ratio = scene->cameras[0]->render_width / (float)scene->cameras[0]->render_height;

                    Vector3 camera_origin(0, 0, 0);
                    camera_origin = camera_origin.as_point_multiplied(scene->cameras[0]->camera_to_world);

                    // Get Pixel to render
                    std::size_t pixel_index = buffer->pixel_to_render();

                    // Get x, y coord of pixel
                    std::size_t x = pixel_index % buffer->width;
                    std::size_t y = pixel_index / buffer->width;
                    // Trace and write to buffer memory
                    buffer->contribute_to_pixel(
                        pixel_index,
                        trace_from_camera(
                            scene->background,
                            scene->traceables,
                            camera_origin,
                            camera_scale,
                            camera_aspect_ratio,
                            scene->camera_at(0),
                            x, y
                        )
                    );
                }
            })
        );
    }
}

} // end namespace
