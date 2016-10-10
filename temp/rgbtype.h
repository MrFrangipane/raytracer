#ifndef RGBTYPE_H
#define RGBTYPE_H

#include <mutex>
#include <iostream>


struct RGBType {
    double r;
    double g;
    double b;
    double a;
    bool rendered=false;
    int index;
};


struct Renderable {
    std::mutex mutex;
    std::shared_ptr<RGBType> buffer;
    int request_number = 0;
    int pixel_to_render_1 = 0;
    int pixel_to_render_2 = 230400;
    int pixel_to_render_3 = 460800;
    int pixel_to_render_4 = 691200;

    int next_pixel_index() {
        // Mutex
        std::lock_guard<std::mutex> lock(mutex);
        // Next Pixel
        int request_index = request_number % 4;
        int pixel_index = 0;

        if (request_index == 0) {
            pixel_index = pixel_to_render_1;
            pixel_to_render_1++;
        }
        else if (request_index == 1) {
            pixel_index = pixel_to_render_2;
            pixel_to_render_2++;
        }
        else if (request_index == 2) {
            pixel_index = pixel_to_render_3;
            pixel_to_render_3++;
        }
        else {
            pixel_index = pixel_to_render_4;
            pixel_to_render_4++;
        }
        request_number++;

        if (pixel_index <= 1280 * 720) {
            return pixel_index;
        }
        else {
            return -1;
        }

    }

    void write_pixel(const int pixel_index, const RGBType pixel) {
        // Mutex
        std::lock_guard<std::mutex> lock(mutex);
        // New Pixel
        RGBType new_pixel;
        new_pixel.r = pixel.r;
        new_pixel.g = pixel.g;
        new_pixel.b = pixel.b;
        new_pixel.a = pixel.a;
        new_pixel.index = pixel_index;
        new_pixel.rendered = true;
        // Write
        buffer.get()[pixel_index] = new_pixel;
    }

    std::shared_ptr<RGBType> get_buffer(){
        // Mutex
        std::lock_guard<std::mutex> lock(mutex);
        // Return
        return buffer;
    }
};

#endif // RGBTYPE_H
