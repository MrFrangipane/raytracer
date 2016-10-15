#include "qtutils.h"
#include <qrgb.h>


std::vector<int> buffer_to_raw(const std::shared_ptr<raytracer::Buffer> &buffer, const double &factor){
    // Init Return
    std::vector<int> int_buffer;

    for (int pixel_index = 0; pixel_index < buffer->pixel_count; pixel_index++) {
        // Float to 8bit
        raytracer::Color pixel = buffer->pixels[pixel_index];
        double red = (1 - exp(-pixel.red * factor)) * 255;
        double green = (1 - exp(-pixel.green * factor)) * 255;
        double blue = (1 - exp(-pixel.blue * factor)) * 255;
        double alpha = pixel.alpha * 255;

        int_buffer.push_back(qRgba((int)red, (int)green, (int)blue, (int)alpha));
    }

    return int_buffer;
}
