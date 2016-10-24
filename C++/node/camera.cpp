#include "camera.h"

namespace frangiray {

// Pseudo Static Member
bool Camera::traceable() const
{
    return Camera::_traceable;
}


// Intersection Distance
f_real Camera::intersection_distance(const Ray &ray) const
{
    return -1;
}


// Surface Attributes
SurfaceAttributes Camera::surface_attributes_at(const Vector &position_) const
{
    return SurfaceAttributes();
}

}
