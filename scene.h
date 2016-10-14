#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <memory>
#include "camera.h"
#include "traceable.h"

namespace raytracer {

class Scene {
    public:
        // Members
        std::vector<std::unique_ptr<Camera>> cameras;
        std::vector<std::unique_ptr<Traceable>> traceables;

        // Constructor
        Scene() {}
};


} // end namespace

#endif // SCENE_H
