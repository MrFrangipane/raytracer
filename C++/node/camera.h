#ifndef CAMERA_H
#define CAMERA_H

#include "math/matrix.h"
#include "node/abstractnode.h"


namespace frangiray {

class Camera : public AbstractNode
{
public:
    // Members
    f_real fov = 45.0;
    f_real exposure = 1.0;
    const static bool traceable = false;

    // Constructors
    Camera(const std::string name_)
        : AbstractNode(name_) {}

    Camera(const std::string name_, const Matrix &transform_)
        : AbstractNode(name_, transform_) {}

    Camera(const std::string name_, const Matrix &transform_, const f_real fov_)
        : AbstractNode(name_, transform_), fov(fov_) {}

    Camera(const std::string name_, const Matrix &transform_, const f_real fov_, const f_real exposure_)
        : AbstractNode(name_, transform_), fov(fov_), exposure(exposure_) {}

    // Methods
    f_real intersection_distance(const Ray &ray) const;

};

}

#endif // CAMERA_H
