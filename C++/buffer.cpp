#include "buffer.h"


namespace raytracer {


Buffer::Buffer(const int width_, const int height_)
{
    // Init Members
    width = width_;
    height = height_;
    pixel_count = width * height;
    render_call_counter = 0;
    render_pixel_x = 0;
    render_pixel_y = 0;
    render_iteration = 0;
    render_resolution_divider = OVERSCAN_STEP;
    pixel_size = pow(2, render_resolution_divider);
    over_rendering = true;

    // Init Pixels
    pixels = std::vector<std::unique_ptr<Color>>(pixel_count);
    for (std::size_t pixel_index = 0; pixel_index < pixel_count; pixel_index++)
    {
        pixels[pixel_index] = std::unique_ptr<Color>(new Color);
    }
}


void Buffer::reset_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->render_pixel_mutex);

    render_call_counter = 0;
    render_pixel_x = 0;
    render_pixel_y = 0;
    render_iteration = 0;
    render_resolution_divider = OVERSCAN_STEP;
    pixel_size = pow(2, render_resolution_divider);
    over_rendering = true;
}


std::size_t Buffer::pixel_to_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->render_pixel_mutex);

    if (over_rendering)
    {
        render_pixel_x = (render_call_counter * pixel_size) % width;
        render_pixel_y = (render_call_counter / width) * pixel_size;

        std::size_t render_pixel_index = (render_pixel_x + render_pixel_y * width);

        if (render_pixel_index >= pixel_count)
        {
            render_resolution_divider--;
            pixel_size = pow(2, render_resolution_divider);
            if (render_resolution_divider == 0) over_rendering = false;
            render_pixel_index = 0;
            render_pixel_x = 0;
            render_pixel_y = 0;
            render_call_counter = 0;
        }

        render_call_counter++;

        return render_pixel_index;
    }
    else
    {
        if (render_call_counter >= pixel_count)
        {
            render_iteration++;

            render_call_counter = 0;
        }
        return render_call_counter++;
    }
}


void Buffer::contribute_to_pixel(const std::size_t pixel_index, const Color &color)
{
    if (over_rendering)
    {
        std::size_t offset;
        for (int x = 0; x < pixel_size; x++) {
        for (int y = 0; y < pixel_size; y++) {
            offset = pixel_index + x + y * width;
            pixels[offset]->red = color.red;
            pixels[offset]->green = color.green;
            pixels[offset]->blue = color.blue;
        }}
        /*
        pixels[pixel_index]->red = color.red;
        pixels[pixel_index]->green = color.green;
        pixels[pixel_index]->blue = color.blue;
        */
    }
    else
    {
        pixels[pixel_index]->red = (pixels[pixel_index]->red * render_iteration + color.red) / (render_iteration + 1);
        pixels[pixel_index]->green = (pixels[pixel_index]->green * render_iteration + color.green) / (render_iteration + 1);
        pixels[pixel_index]->blue = (pixels[pixel_index]->blue * render_iteration + color.blue) / (render_iteration + 1);
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
