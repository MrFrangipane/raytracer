#include "tracer.h"

namespace frangiray {

// Reset Render
void Tracer::reset_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->_render_pixel_mutex);

    // Reset Values
    _render_pixel_x = 0;
    _render_pixel_y = 0;
    _render_call_counter = 0;
    _render_iteration = 0;
    _render_resolution_divider = OVERSCAN_STEP;
    _pixel_size = (std::size_t)pow(2, _render_resolution_divider);
    is_over_rendering = true;
}

// Render
void Tracer::render()
{
    // Core count
    std::size_t core_count = std::thread::hardware_concurrency() - 2;
    // Atomic
    std::vector<std::future<void>> futures;

    // Each Core
    while (core_count--)
    {
        // Add to pool of threads
        futures.emplace_back(
            // Async call to loop
            std::async([=]()
            {
                while(true)
                {
                    trace_from_camera();
                }
            })
        );
    }
}


// Pixel to render
std::size_t Tracer::pixel_to_render()
{
    // Mutex Lock
    std::lock_guard<std::mutex> guard(this->_render_pixel_mutex);

    if (is_over_rendering)
    {
        _render_pixel_x = (_render_call_counter * _pixel_size) % width;
        _render_pixel_y = ((_render_call_counter * _pixel_size) / width) * _pixel_size;

        std::size_t render_pixel_index = (_render_pixel_x + _render_pixel_y * width);

        if (render_pixel_index >= pixel_count)
        {
            _render_resolution_divider--;
            _pixel_size = (std::size_t)pow(2, _render_resolution_divider);
            if (_render_resolution_divider == 0) is_over_rendering = false;
            render_pixel_index = 0;
            _render_pixel_x = 0;
            _render_pixel_y = 0;
            _render_call_counter = 0;
        }

        _render_call_counter++;

        return render_pixel_index;
    }
    else
    {
        if (_render_call_counter >= pixel_count)
        {
            _render_iteration++;
            _render_call_counter = 0;
        }
        return _render_call_counter++;
    }
}


// Contribute to Pixel
void Tracer::contribute_to_pixel(const std::size_t pixel_index, const Pixel &pixel)
{
    // Mutex Lock
    //std::lock_guard<std::mutex> guard(this->_render_pixel_mutex);

    if (is_over_rendering)
    {
        std::size_t offset;
        for (int x = 0; x <= _pixel_size; x++) {
        for (int y = 0; y <= _pixel_size; y++) {
            // Offset
            offset = pixel_index + x + y * width;
            // Safely write pixel
            if (offset < pixel_count) *buffer.pixels[offset] = pixel;
        }}
    }
    else
    {
        *buffer.pixels[pixel_index] = pixel;
        // buffer.pixels[pixel_index] * _render_iteration + pixel) / (_render_iteration + 1)
    }
}


// Trace from camera
void Tracer::trace_from_camera()
{
    // Pixel to render
    Pixel pixel;
    std::size_t render_pixel = pixel_to_render();

    // Simulate Load
    for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
        std::size_t k = i * j;
    }}

    // Dummy values to test interactions with GUI thread    
    pixel.beauty.r = (std::uint8_t)(((f_real)render_pixel / pixel_count) * 255);
    //pixel.beauty.g = (std::uint8_t)((1 - exp(scene->node_at(0)->position()[2])) * 255);

    // Write buffer
    contribute_to_pixel(render_pixel, pixel);
}

}
