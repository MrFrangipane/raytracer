#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <vector>
#include "image/pixel.h"

namespace frangiray {

class Buffer
{
public:
    // Members
    std::size_t width = 0;
    std::size_t height = 0;
    std::size_t pixel_count = 0;
    std::vector<std::shared_ptr<Pixel>> pixels;

    // Constructors
    Buffer();
    Buffer(const std::size_t width_, const std::size_t height_);
};

}

#endif // BUFFER_H
