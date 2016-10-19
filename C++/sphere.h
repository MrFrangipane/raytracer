#ifndef SPHERE_H
#define SPHERE_H

#include "utils.h"
#include "traceable.h"

namespace raytracer {

class Sphere : public Traceable {
    public:
        // Members
        float radius;
        float radius2;

        // Constructors
        Sphere(const Matrix44 object_to_world_, const Color diffuse_color_, const Color emission_color_, const double reflection_amount_, const double reflection_blur_,
               const float radius_) :
            radius(radius_),
            Traceable(object_to_world_, diffuse_color_, emission_color_, reflection_amount_, reflection_blur_)
        {
            radius2 = radius * radius;
        }

        double hit_distance(const Ray &ray) const
        {
            Vector3 L = position - ray.origin;
            double tca = L.dot_product(ray.direction);

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
        SurfaceAttributes surface_attributes_at(const Vector3 &at) const
        {
            Vector3 normal;
            normal = at - position;
            normal.normalize();
            
            SurfaceAttributes attributes;
            attributes.diffuse_color = diffuse_color;
            attributes.emission_color.red = emission_color.red * radius;
            attributes.emission_color.green = emission_color.green * radius;
            attributes.emission_color.blue = emission_color.blue * radius;
            attributes.reflection_amount = reflection_amount;
            attributes.reflection_blur = reflection_blur;
            attributes.normal = normal;

            return attributes;
        }

        //Random Position
        Vector3 random_position() const
        {
            Vector3 random_pos = random_direction();
            random_pos *= radius;
            return position + random_pos;
        }
};

} // end namespace

#endif // SPHERE_H
