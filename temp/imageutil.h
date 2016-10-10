#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include <vector>
#include <memory>
#include "rgbtype.h"
#include "qbytearray.h"
#include "qimage.h"
#include <iostream>

std::vector<int> to_uchar_buffer(const int width, const int height, const std::shared_ptr<RGBType> buffer){

    int pixel_count = width * height;
    std::vector<int> image_buffer;

    for (int pixel_index = 0; pixel_index < pixel_count; pixel_index++) {
        // Float to 8bit
        RGBType pixel = buffer.get()[pixel_index];
        double red = pixel.r * 255;
        double green = pixel.g * 255;
        double blue = pixel.b * 255;
        double alpha = pixel.a * 255;

        image_buffer.push_back(qRgba((int)red, (int)green, (int)blue, (int)alpha));
    }

    return image_buffer;
}


void renderr(std::shared_ptr<Renderable> renderable) {
    while (true) {
        // Next Pixel to render
        int pixel_index = renderable.get()->next_pixel_index();
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

#endif // IMAGEUTIL_H
