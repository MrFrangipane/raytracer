#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <vector>
#include "buffer.h"

namespace raytracer {


class Renderer {
    private:
        vector<int> pixel_indexes;

    public:
        std::shared_ptr<Buffer> buffer;

        Renderer(std::shared_ptr<Buffer> buffer_);

        void render();
};


}

#endif // RENDERER_H
