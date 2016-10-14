#ifndef OBJECT_H
#define OBJECT_H
#include "vector"
#include "matrix.h"

namespace raytracer {

class Camera {
    public:
        // Members
        Matrix44 transform;
        float fov;
        int render_width;
        int render_height;
        float exposure;

        // Constructor
        Camera(Matrix44 transform_, float fov_, int render_width_, int render_height_, float exposure_) :
            transform(transform_),
            fov(fov_),
            render_width(render_width_),
            render_height(render_height_),
            exposure(exposure_)
        {}
};


} // end namespace

#endif // OBJECT_H
