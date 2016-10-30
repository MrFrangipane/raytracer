#include "tracer.h"

namespace frangiray {

// Update Buffer Int
void Tracer::update_buffer_int(const BufferType buffer_type)
{
    // Camera
    std::shared_ptr<Camera> camera = scene->current_camera();

    // Cast to int
    for(std::size_t pixel_index=0; pixel_index < pixel_count; pixel_index++)
    {
        *buffer.integers[pixel_index] = r_g_b_a(
            std::min(255, (int)(buffer.pixels[pixel_index]->beauty.r * camera->exposure * 255)),
            std::min(255, (int)(buffer.pixels[pixel_index]->beauty.g * camera->exposure * 255)),
            std::min(255, (int)(buffer.pixels[pixel_index]->beauty.b * camera->exposure * 255)),
            std::min(255, (int)(buffer.pixels[pixel_index]->beauty.a * 255))
        );
    }
}


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
            // Higher Resolution
            _render_resolution_divider--;
            _pixel_size = (std::size_t)pow(2, _render_resolution_divider);

            // Stop over rendering if division by0
            if (_render_resolution_divider == 0) is_over_rendering = false;

            // Reset Pixel index
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
        // World Normal
        (*buffer.pixels[pixel_index]).world_normal[0] = (buffer.pixels[pixel_index]->world_normal[0] * _render_iteration + pixel.world_normal[0]) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).world_normal[1] = (buffer.pixels[pixel_index]->world_normal[1] * _render_iteration + pixel.world_normal[1]) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).world_normal[2] = (buffer.pixels[pixel_index]->world_normal[2] * _render_iteration + pixel.world_normal[2]) / (_render_iteration + 1);

        // Emission
        (*buffer.pixels[pixel_index]).emission.r = (buffer.pixels[pixel_index]->emission.r * _render_iteration + pixel.emission.r) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).emission.g = (buffer.pixels[pixel_index]->emission.g * _render_iteration + pixel.emission.g) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).emission.b = (buffer.pixels[pixel_index]->emission.b * _render_iteration + pixel.emission.b) / (_render_iteration + 1);

        // Albedo
        (*buffer.pixels[pixel_index]).albedo.r = (buffer.pixels[pixel_index]->albedo.r * _render_iteration + pixel.albedo.r) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).albedo.g = (buffer.pixels[pixel_index]->albedo.g * _render_iteration + pixel.albedo.g) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).albedo.b = (buffer.pixels[pixel_index]->albedo.b * _render_iteration + pixel.albedo.b) / (_render_iteration + 1);

        // Direct Lighting
        (*buffer.pixels[pixel_index]).direct_lighting.r = (buffer.pixels[pixel_index]->direct_lighting.r * _render_iteration + pixel.direct_lighting.r) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).direct_lighting.g = (buffer.pixels[pixel_index]->direct_lighting.g * _render_iteration + pixel.direct_lighting.g) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).direct_lighting.b = (buffer.pixels[pixel_index]->direct_lighting.b * _render_iteration + pixel.direct_lighting.b) / (_render_iteration + 1);

        // Indirect Lighting
        (*buffer.pixels[pixel_index]).indirect_lighting.r = (buffer.pixels[pixel_index]->indirect_lighting.r * _render_iteration + pixel.indirect_lighting.r) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).indirect_lighting.g = (buffer.pixels[pixel_index]->indirect_lighting.g * _render_iteration + pixel.indirect_lighting.g) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).indirect_lighting.b = (buffer.pixels[pixel_index]->indirect_lighting.b * _render_iteration + pixel.indirect_lighting.b) / (_render_iteration + 1);

        // Reflection
        (*buffer.pixels[pixel_index]).reflection.r = (buffer.pixels[pixel_index]->reflection.r * _render_iteration + pixel.reflection.r) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).reflection.g = (buffer.pixels[pixel_index]->reflection.g * _render_iteration + pixel.reflection.g) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).reflection.b = (buffer.pixels[pixel_index]->reflection.b * _render_iteration + pixel.reflection.b) / (_render_iteration + 1);

        // Beauty
        (*buffer.pixels[pixel_index]).beauty.r = (buffer.pixels[pixel_index]->beauty.r * _render_iteration + pixel.beauty.r) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).beauty.g = (buffer.pixels[pixel_index]->beauty.g * _render_iteration + pixel.beauty.g) / (_render_iteration + 1);
        (*buffer.pixels[pixel_index]).beauty.b = (buffer.pixels[pixel_index]->beauty.b * _render_iteration + pixel.beauty.b) / (_render_iteration + 1);
    }

    // If First Pixel
    if (pixel_index == 0) update_buffer_int(BufferType::Beauty);
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
    std::shared_ptr<Camera> camera = scene->current_camera();
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
void Tracer::trace(const Ray &primary_ray, Pixel &target_pixel, const int recursion_depth)
{
    // Recursion Limit
    if (recursion_depth > MAX_RECURSION)
    {
        // Exit Color
        target_pixel.emission.r = 0.0;
        target_pixel.emission.g = 0.0;
        target_pixel.emission.b = 0.0;
        // Compute for Gi and reflection
        target_pixel.compute_beauty();
        return;
    }

    // Primary Ray Cast
    RayHit primary_hit;
    ray_cast(primary_ray, primary_hit);

    // Exit If no hit
    if (primary_hit.distance <= 0 || primary_hit.distance >= F_INFINITY)
    {
        // Backdrop Color
        target_pixel.emission.r = 0.4;
        target_pixel.emission.g = 0.4;
        target_pixel.emission.b = 0.4;
        // Compute for Gi and reflection
        target_pixel.compute_beauty();
        return;
    }

    // Get Hit object
    std::shared_ptr<AbstractNode> primary_hit_node = scene->node_at(primary_hit.node_index);

    // Surface Attributes
    SurfaceAttributes primary_hit_surface = primary_hit_node->surface_attributes_at(primary_hit.position);
    f_real incidence = 1.0 - primary_ray.direction.dot_product(-primary_hit_surface.normal);

    // Store Object ID and Depth
    target_pixel.object_id = primary_hit.node_index;
    target_pixel.depth = primary_hit.distance;

    // Store World Normal
    target_pixel.world_normal = primary_hit_surface.normal;

    // Store Emissive
    target_pixel.emission.r = primary_hit_surface.emission_color.r;
    target_pixel.emission.g = primary_hit_surface.emission_color.g;
    target_pixel.emission.b = primary_hit_surface.emission_color.b;

    // Store Albedo
    target_pixel.albedo.r = primary_hit_surface.diffuse_color.r;
    target_pixel.albedo.g = primary_hit_surface.diffuse_color.g;
    target_pixel.albedo.b = primary_hit_surface.diffuse_color.b;

    // Reflection
    if (primary_hit_surface.reflection_amount > 0)
    {
        // Prepare direction
        Vector reflection_direction = reflect(primary_ray.direction, primary_hit_surface.normal);

        // If blurry
        if (primary_hit_surface.reflection_roughness > 0)
        {
            reflection_direction = reflection_direction + (random_direction() * primary_hit_surface.reflection_roughness);
            reflection_direction.normalize();
        }

        // Recurse
        Ray reflection_ray(primary_hit.position, reflection_direction);
        Pixel reflected_pixel;
        trace(reflection_ray, reflected_pixel, recursion_depth + 1);

        // Store Reflection
        target_pixel.reflection.r = reflected_pixel.beauty.r * (0.2 + incidence * 0.8);
        target_pixel.reflection.g = reflected_pixel.beauty.g * (0.2 + incidence * 0.8);
        target_pixel.reflection.b = reflected_pixel.beauty.b * (0.2 + incidence * 0.8);
    }

    // Direct Lighting
    f_real attenuation;
    int direct_ray_count;
    if (recursion_depth == 0) direct_ray_count = DIRECT_LIGHTING_RAYS;
    else direct_ray_count = 1;
    for (int i = 0; i < direct_ray_count; i++)
    {
        for (int emittor_index = 0; emittor_index < scene->node_count(); emittor_index++)
        {
            // Skip self
            if (emittor_index == primary_hit.node_index) continue;

            // Node
            std::shared_ptr<AbstractNode> emittor_node = scene->node_at(emittor_index);

            // Skip if not Emission
            if (
                emittor_node->emission_color.r == 0 &&
                emittor_node->emission_color.g == 0 &&
                emittor_node->emission_color.b == 0
            ) continue;

            // Ray Direction
            Vector shadow_ray_direction = emittor_node->random_position() - primary_hit.position;
            shadow_ray_direction.normalize();

            // Skip if backface
            f_real shadow_incidence = primary_hit_surface.normal.dot_product(shadow_ray_direction);
            if (shadow_incidence < 0) continue;

            // Ray Cast
            Ray shadow_ray(primary_hit.position, shadow_ray_direction);
            RayHit emittor_hit;
            ray_cast(shadow_ray, emittor_hit);

            // Emittor Hit
            if (emittor_hit.node_index == emittor_index)
            {
                // Surface attributes
                SurfaceAttributes emittor_hit_surface = emittor_node->surface_attributes_at(emittor_hit.position);

                // Basic Shading
                attenuation = 1.0 / ((emittor_hit.distance / LIGHT_ATTENUATION_SCALE) + 1);
                target_pixel.direct_lighting.r += emittor_hit_surface.emission_color.r * shadow_incidence * attenuation / direct_ray_count;
                target_pixel.direct_lighting.g += emittor_hit_surface.emission_color.g * shadow_incidence * attenuation / direct_ray_count;
                target_pixel.direct_lighting.b += emittor_hit_surface.emission_color.b * shadow_incidence * attenuation / direct_ray_count;
            }
        }
    }

    // Indirect Lighting
    int indirect_ray_count;
    if (recursion_depth == 0) indirect_ray_count = INDIRECT_LIGHTING_RAYS;
    else indirect_ray_count = 1;
    for (int i=0; i < indirect_ray_count; i++)
    {
        Vector indirect_ray_direction = primary_hit_surface.normal + random_direction();
        f_real indirect_incidence = primary_hit_surface.normal.dot_product(indirect_ray_direction);

        if (indirect_incidence > 0)
        {
            // Indirect Ray
            indirect_ray_direction.normalize();
            Ray indirect_ray(primary_hit.position, indirect_ray_direction);

            // Trace
            Pixel indirect_pixel;
            trace(indirect_ray, indirect_pixel, recursion_depth + MAX_RECURSION - INDIRECT_LIGHTING_BOUNCES);

            // Store Indirect Lighting
            target_pixel.indirect_lighting.r += target_pixel.albedo.r * (indirect_pixel.beauty.r * indirect_incidence) / indirect_ray_count;
            target_pixel.indirect_lighting.g += target_pixel.albedo.g * (indirect_pixel.beauty.g * indirect_incidence) / indirect_ray_count;
            target_pixel.indirect_lighting.b += target_pixel.albedo.b * (indirect_pixel.beauty.b * indirect_incidence) / indirect_ray_count;
        }
    }

    // Compute Beauty
    target_pixel.compute_beauty();
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
