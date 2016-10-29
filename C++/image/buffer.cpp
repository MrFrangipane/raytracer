#include "buffer.h"

namespace frangiray {

Buffer::Buffer() {}

Buffer::Buffer(const std::size_t width_, const std::size_t height_)
{
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;

    // Fill Pixels
    pixels.reserve(pixel_count);
    integers.reserve(pixel_count);
    for (std::size_t pixel_index = 0; pixel_index < pixel_count; pixel_index++)
    {
        pixels.emplace_back(std::make_shared<Pixel>());
        integers.emplace_back(std::make_shared<int>(0));
    }
}

}
