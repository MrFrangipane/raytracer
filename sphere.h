#ifndef SPHERE_H
#define SPHERE_H

#include "traceable.h"

namespace raytracer {


class Sphere : public Traceable {
    public:
        // Members
        Vector3 position;
        float radius;
        float radius2;

        // Constructors
        Sphere(const Matrix44 object_to_world_, const Color diffuse_color_, const Color emission_color_,
               const float radius_) :
            radius(radius_),
            Traceable(object_to_world_, diffuse_color_, emission_color_)
        {
            Vector3 world(0);
            position = world.as_point_multiplied(object_to_world);
            radius2 = radius * radius;
        }

        double hit_distance(const Vector3 &origin, const Vector3 &direction) const
        {
            Vector3 L = position - origin;
            double tca = L.dot_product(direction);

            if (tca < 0) return -1;

            double d2 = L.dot_product(L) - (tca * tca);
            if (d2 > radius2) return -1;

            double thc = sqrt(radius2 - d2);
            double t0 = tca - thc;
            double t1 = tca + thc;

            if (t0 > t1) std::swap(t0, t1);

            if (t0 < 0) {
                t0 = t1;
                if (t0 < 0) return -1;
            }

            return t0;
        }

        // Surface Information
        SurfaceAttributes surface_attributes_at(const Vector3 &position) const
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
