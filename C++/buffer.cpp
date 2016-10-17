#include <iostream>
#include <algorithm>
#include "buffer.h"


namespace raytracer {

Buffer::Buffer(const int width_, const int height_)
{
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;
    render_pixel_index = 0;
    render_iteration = 0;

    // Init Pixels
    pixels = std::vector<std::unique_ptr<Color>>(pixel_count);
    for (std::size_t pixel_index = 0; pixel_index < pixel_count; pixel_index++)
    {
        pixels[pixel_index] = std::unique_ptr<Color>(new Color);
    }
}

void Buffer::reset_render()
{
    clear();

    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->render_pixel_mutex);

    render_pixel_index = 0;
    render_iteration = 0;
}

std::size_t Buffer::pixel_to_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->render_pixel_mutex);

    // Go to next Pixel
    render_pixel_index = render_pixel_index + PIXEL_DIVISION;

    // Iteration done
    if (render_pixel_index >= pixel_count)
    {
        render_iteration++;
        render_pixel_index = render_iteration;
    }

    // Return
    return render_pixel_index;
}

void Buffer::contribute_to_pixel(const std::size_t pixel_index, const Color &color)
{
    if (render_iteration >= PIXEL_DIVISION)
    {
        pixels[pixel_index]->red = (color.red + pixels[pixel_index]->red) / 2.0;
        pixels[pixel_index]->green = (color.green + pixels[pixel_index]->green) / 2.0;
        pixels[pixel_index]->blue = (color.blue + pixels[pixel_index]->blue) / 2.0;
    }
    else
    {
        for (int i=0; i < PIXEL_DIVISION - render_iteration; i++)
        {
            int index = pixel_index + i;
            if (index >= pixel_count) continue;
            pixels[index]->red = color.red;
            pixels[index]->green = color.green;
            pixels[index]->blue = color.blue;
        }
    }
}

void Buffer::clear()
{
    // Empty
    for (std::size_t pixel_index = 0; pixel_index < pixel_count; pixel_index++)
    {
        pixels[pixel_index]->red = 0;
        pixels[pixel_index]->green = 0;
        pixels[pixel_index]->blue = 0;
        pixels[pixel_index]->alpha = 1;
    }
}

} // end namespace
