#ifndef PROGRESSIVE_H
#define PROGRESSIVE_H

#define RANGE 100

#include <mutex>

namespace raytracer {

class Progressive {
    public:
        std::mutex mutex;
        int pixel_index;
        int pixel_count;
        int iteration_index;

        Progressive(const int pixel_count_)
        {
            pixel_count = pixel_count_;
            reset();
        }

        void reset()
        {
            mutex.lock();

            pixel_index = 0;
            iteration_index = 0;

            mutex.unlock();
        }

        int pixel_to_render()
        {
            mutex.lock();

            pixel_index = pixel_index + RANGE;

            if (pixel_index > pixel_count)
            {
                iteration_index++;
                pixel_index = iteration_index;
            }

            if (iteration_index >= RANGE)
            {
                pixel_index = 0;
                iteration_index = 0;
            }

            mutex.unlock();
            return pixel_index;
        }
};

} // end namespace

#endif // PROGRESSIVE_H
