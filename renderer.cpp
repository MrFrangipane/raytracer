#include "renderer.h"

namespace raytracer {


void render(std::shared_ptr<Buffer> buffer) {
    // Loop
    while (true) {
        // Next Pixel to render
        int pixel_index = buffer.get();
        // If valid
        if (pixel_index != -1) {
            // New Pixel
            RGBType pixel;
            pixel.r = 0.0;
            pixel.g = 1.0;
            pixel.b = 0.5;
            pixel.a = 1.0;
            // Update Renderable
            renderable.get()->write_pixel(pixel_index, pixel);
            // Sleep
            if (pixel_index % 30 == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        } else {
            // Break
            break;
        }
    }
}

}
