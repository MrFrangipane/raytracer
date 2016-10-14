#ifndef SPHERE_H
#define SPHERE_H
#include "traceable.h"

namespace raytracer {


class Sphere : public Traceable {
    public:
        // Members
        float radius;

        // Constructors
        Sphere(const Matrix44 object_to_world_, const Color diffuse_color_, const Color emission_color_,
               const float radius_) :
            radius(radius_),
            Traceable(object_to_world_, diffuse_color_, emission_color_) {}

        // Hit Distance
        double hit_distance(const Vector3 &, const Vector3 &) const
        {
            return -1;
        }

        // Surface Information
        SurfaceAttributes surface_attributes_at(const Vector3 &) const
        {
            SurfaceAttributes attributes;
            attributes.diffuse_color = Color();
            attributes.emission_color = Color();
            attributes.normal = Vector3(1, 0, 0);

            return attributes;
        }
};


} // end namespace

#endif // SPHERE_H
