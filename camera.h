#ifndef OBJECT_H
#define OBJECT_H
#include "vector"
#include "matrix.h"

namespace raytracer {

class Camera {
    public:
        // Members
        Matrix44 camera_to_world;
        Matrix44 world_to_camera;
        float fov;
        int render_width;
        int render_height;
        float exposure;

        // Constructor
        Camera(Matrix44 camera_to_world_, float fov_, int render_width_, int render_height_, float exposure_) :
            camera_to_world(camera_to_world_),
            world_to_camera(camera_to_world_.inverted()),
            fov(fov_),
            render_width(render_width_),
            render_height(render_height_),
            exposure(exposure_)
        {}
};


} // end namespace

#endif // OBJECT_H
