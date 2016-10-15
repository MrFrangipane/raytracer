#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
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

        // Move by
        void move_by(Vector3 movement) {
            camera_to_world[3][0] = camera_to_world[3][0] + movement[0];
            camera_to_world[3][1] = camera_to_world[3][1] + movement[1];
            camera_to_world[3][2] = camera_to_world[3][2] + movement[2];
            world_to_camera = camera_to_world.inverted();
        }

        void set_fov(float new_fov) {
            fov = new_fov;
        }

};


} // end namespace

#endif // CAMERA_H
