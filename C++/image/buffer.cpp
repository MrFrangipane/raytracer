#include "buffer.h"

namespace frangiray {

Buffer::Buffer()
{
    // Fill Pixels
    pixels = std::vector<std::shared_ptr<Pixel>>(0);
}


Buffer::Buffer(const std::size_t width_, const std::size_t height_)
{
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;

    // Fill Pixels
    pixels = std::vector<std::shared_ptr<Pixel>>(pixel_count);
    for (std::size_t pixel_index = 0; pixel_index < pixel_count; pixel_index++)
    {
        pixels[pixel_index] = std::make_unique<Pixel>();
    }
}

}
