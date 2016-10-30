#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include "common/typedefs.h"
#include "math/ray.h"
#include "math/rayhit.h"
#include "node/abstractnode.h"
#include "node/surfaceattributes.h"


namespace frangiray {

class Sphere: public AbstractNode
{
public:
    // Members
    f_real radius = 1.0;

    // Constructors
    Sphere(const std::string name_)
        : AbstractNode(name_) {}

    Sphere(const std::string name_, const f_real radius_)
        : AbstractNode(name_),
          radius(radius_), _radius2(radius_ * radius_) {}

    Sphere(const std::string name_, const Matrix &transform_)
        : AbstractNode(name_, transform_) {}

    Sphere(const std::string name_, const Matrix &local_to_world_, const f_real radius_)
        : AbstractNode(name_, local_to_world_),
          radius(radius_), _radius2(radius_ * radius_) {}

    Sphere(
        const std::string name_,
        const Matrix local_to_world_,
        const RealColor diffuse_color_,
        const RealColor emission_color_,
        const f_real reflection_amount_,
        const f_real reflection_roughness_,
        const f_real radius_
    )
        :   AbstractNode(name_, local_to_world_, diffuse_color_, emission_color_, reflection_amount_, reflection_roughness_),
            radius(radius_), _radius2(radius_ * radius_) {}

    // Methods
    void set_radius(f_real radius_);
    bool traceable() const;
    f_real intersection_distance(const Ray &ray) const;
    SurfaceAttributes surface_attributes_at(const Vector &position_) const;
    Vector random_position() const;

private:
    // Members
    const static bool _traceable = true;
    f_real _radius2 = 1.0;
};

}

#endif // SPHERE_H
