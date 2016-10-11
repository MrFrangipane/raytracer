#include "renderer.h"
#include "pixel.h"

namespace raytracer {

// Given a Scene, trace a pixel
Pixel trace() {
    raytracer::Pixel pixel;
    pixel.red = 1.0;
    pixel.green = 0;
    pixel.blue = 0;
    pixel.alpha = 1.0;

    for (int i = 0; i < 100; i++)
    {
        for (int j=0; j < 100; j++)
        {
            int k = i * j;
        }
    }

    return pixel;
}


// Given a Scene and a Buffer, trace all pixels
void Render(std::shared_ptr<Buffer> buffer, const std::size_t core_count)
{
    // Core count
    std::size_t cores = core_count;
    // Atomic
    volatile std::atomic<std::size_t> atomic_pixel_index(0);
    std::vector<std::future<void>> futures;

    // Pointer to first pixel of buffer
    Pixel* buffer_pixel = &buffer.get()->pixels[0];

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
                    // Exit all pixel rendered
                    if (pixel_index >= buffer.get()->pixel_count)
                        break;
                    // Trace and write to buffer memory
                    buffer_pixel[pixel_index] = trace();
                }
            })
        );
    }
}

} // end namespace
