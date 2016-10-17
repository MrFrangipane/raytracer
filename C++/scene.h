#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "color.h"
#include "camera.h"
#include "traceable.h"

namespace raytracer {

class Scene {
    public:
        // Members
        Color background;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Traceable>> traceables;

        // Constructor
        Scene() {}
};


} // end namespace

#endif // SCENE_H
