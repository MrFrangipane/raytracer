#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include "pixel.h"

namespace raytracer {

class Buffer {
    public:
        int width;
        int height;
        int pixel_count;
        std::vector<Pixel> pixels;

        Buffer(const int width_, const int height_);


};

}

#endif // BUFFER_H
