#include "qtutils.h"
#include <qrgb.h>


std::vector<int> buffer_to_raw(const std::shared_ptr<raytracer::Buffer> &buffer){
    // Init Return
    std::vector<int> int_buffer;

    for (int pixel_index = 0; pixel_index < buffer->pixel_count; pixel_index++) {
        // Float to 8bit
        raytracer::Color pixel = buffer->pixels[pixel_index];
        double red = pixel.red * 255;
        double green = pixel.green * 255;
        double blue = pixel.blue * 255;
        double alpha = pixel.alpha * 255;

        int_buffer.push_back(qRgba((int)red, (int)green, (int)blue, (int)alpha));
    }

    return int_buffer;
}
