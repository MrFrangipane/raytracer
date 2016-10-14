#ifndef SPHERE_H
#define SPHERE_H
#include "traceable.h"

namespace raytracer {


class Sphere : public Traceable {
    public:
        // Members
        float radius;

        // Constructors
        Sphere(const Matrix44 transform_, const Vector3 diffuse_color_, const Vector3 emission_color_,
               const float radius_) :
            radius(radius_),
            Traceable(transform_, diffuse_color_, emission_color_) {}

        // Hit Distance
        double hit_distance(const Vector3 &, const Vector3 &) const
        {
            return -1;
        }

        // Surface Information
        SurfaceInformation information_at(const Vector3 &) const
        {
            SurfaceInformation infos;
            infos.diffuse_color = Vector3(1, 1, 0);
            infos.emission_color = Vector3(5, 5, 5);
            infos.normal = Vector3(1, 0, 0);

            return infos;
        }
};


} // end namespace

#endif // SPHERE_H
