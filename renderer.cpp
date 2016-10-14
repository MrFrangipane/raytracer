#include "renderer.h"


namespace raytracer {

// Given a Scene, trace a pixel
Color trace(const std::shared_ptr<Scene> scene, const std::size_t x, const std::size_t y)
{
    // New Pixel
    Color pixel;

    // teste un truc
    pixel = scene.get()->traceables[0].get()->diffuse_color;

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
                    buffer_pixel[pixel_index] = trace(scene, x, y);
                }
            })
        );
    }
}

} // end namespace
