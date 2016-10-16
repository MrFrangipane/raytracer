#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include "color.h"

namespace raytracer {

class Buffer {
    public:
        int width;
        int height;
        int pixel_count;
        std::vector<Color> pixels;

        Buffer(const int width_, const int height_);

        void clear();
};

} // end namespace

#endif // BUFFER_H
