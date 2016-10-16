#ifndef BUFFER_H
#define BUFFER_H

#define PIXEL_DIVISION 16

#include <mutex>
#include <vector>
#include "color.h"

namespace raytracer {

class Buffer {
    public:
        int width;
        int height;
        int pixel_count;
        std::vector<std::unique_ptr<Color>> pixels;
        std::mutex render_pixel_mutex;
        std::size_t render_pixel_index;
        int render_iteration;

        Buffer(const int width_, const int height_);

        void clear();
        std::size_t pixel_to_render();
        void reset_render();
        void contribute_to_pixel(const std::size_t pixel_index, const Color &color);
};

} // end namespace

#endif // BUFFER_H
