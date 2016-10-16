#include "qtutils.h"
#include <qrgb.h>


std::vector<int> buffer_to_raw(const std::shared_ptr<raytracer::Buffer> &buffer, const double &factor){
    // Init Return
    std::vector<int> int_buffer(buffer->pixel_count);

    for (int pixel_index = 0; pixel_index < buffer->pixel_count; pixel_index++) {
        // Float to 8bit
        double red = (1 - exp(-buffer->pixels[pixel_index].red * factor)) * 255;
        double green = (1 - exp(-buffer->pixels[pixel_index].green * factor)) * 255;
        double blue = (1 - exp(-buffer->pixels[pixel_index].blue * factor)) * 255;
        double alpha = buffer->pixels[pixel_index].alpha * 255;

        int_buffer[pixel_index] =  qRgba((int)red, (int)green, (int)blue, (int)alpha);
    }

    return int_buffer;
}
