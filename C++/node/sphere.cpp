#include "sphere.h"

namespace frangiray {

// Pseudo Static Member
bool Sphere::traceable() const
{
    return Sphere::_traceable;
}


// Set Radius
void Sphere::set_radius(f_real radius_)
{
    radius = radius_;
    _radius2 = radius_ * radius_;
}


// Intersection Distance
f_real Sphere::intersection_distance(const Ray &ray) const
{
    // Init
    f_real tca, thc, d2, t0, t1;

    // Compute
    Vector L = position() - ray.origin;
    tca = L.dot_product(ray.direction);

    // Exit
    if (tca < 0) return -1;

    d2 = L.dot_product(L) - (tca * tca);
    if (d2 > _radius2) return -1;

    thc = sqrt(_radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1;

        // Exit
        if (t0 < 0) return -1;
    }

    // Return
    return t0;
}


// Surface Attributes
SurfaceAttributes Sphere::surface_attributes_at(const Vector &position_) const
{
    SurfaceAttributes attributes;

    attributes.diffuse_color = diffuse_color;
    attributes.emission_color = emission_color;
    attributes.reflection_amount = reflection_amount;
    attributes.reflection_roughness = reflection_roughness;
    attributes.normal = (position_ - position()).normalize();

    return attributes;
}


Vector Sphere::random_position() const
{
    Vector random_pos = random_direction();
    random_pos *= radius;
    return position() + random_pos;
}

}
