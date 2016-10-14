#include "renderer.h"


namespace raytracer {

// Given a Scene, trace a pixel
Pixel trace(const std::size_t x, const std::size_t y) {
    // New opaque Pixel
    Pixel pixel;
    pixel.alpha = 1.0;

    // Simulate load
    for (int i = 0; i < 100; i++) {
        for (int j=0; j < 1; j++)
        {
            // teste un truc
            Vector3 vec_1((double)x, (double)y, rand());
            Vector3 vec_2(rand() * 0.1, rand() * 0.1, rand() * 0.1);

            Ray le_rayon(Vector3(0, 0, 0), Vector3(0, 1, 0));
            Matrix44 patrice;
            patrice.invert();
            le_rayon.direction = vec_1.as_direction_multiplied(patrice);

            Vector3 vec_3 = vec_1.cross_product(vec_2);

            pixel.red = vec_3.x;
            pixel.green = vec_3.y;
            pixel.blue = vec_3.z;
        }
    }

    return pixel;
}


// Given a Scene and a Buffer, trace all pixels
void render(std::shared_ptr<Buffer> buffer, const std::size_t core_count)
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

                    // Exit if all pixel rendered
                    if (pixel_index >= buffer.get()->pixel_count)
                        break;

                    // Get x, y coord of pixel
                    std::size_t x = pixel_index % buffer.get()->width;
                    std::size_t y = pixel_index / buffer.get()->width;
                    // Trace and write to buffer memory
                    buffer_pixel[pixel_index] = trace(x, y);
                }
            })
        );
    }
}

} // end namespace
