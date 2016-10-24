#ifndef NODE_H
#define NODE_H

#include <string>
#include "math/vector.h"
#include "math/matrix.h"
#include "math/ray.h"
#include "math/rayhit.h"
#include "node/surfaceattributes.h"


namespace frangiray {

class AbstractNode
{
public:
    // Members
    Matrix local_to_world;
    Matrix world_to_local;
    RealColor diffuse_color;
    RealColor emission_color;
    f_real reflection_amount;
    f_real reflection_roughness;
    std::string name;

    // Constructors
    AbstractNode(const std::string name_)
        : name(name_) {}

    AbstractNode(const std::string name_, const Matrix local_to_world_)
        : name(name_), local_to_world(local_to_world_), world_to_local(local_to_world_.inverted()) {}

    AbstractNode(
        const std::string name_,
        const Matrix local_to_world_,
        const RealColor diffuse_color_,
        const RealColor emission_color_,
        const f_real reflection_amount_,
        const f_real reflection_roughness_
    )
        :   name(name_),
            local_to_world(local_to_world_),
            world_to_local(local_to_world_.inverted()),
            diffuse_color(diffuse_color_),
            emission_color(emission_color_),
            reflection_amount(reflection_amount_),
            reflection_roughness(reflection_roughness_) {}

    // Methods
    Vector position() const;
    virtual bool traceable() const = 0;
    virtual f_real intersection_distance(const Ray &ray) const = 0;
    virtual SurfaceAttributes surface_attributes_at(const Vector &position_) const = 0;
    virtual Vector random_position() const = 0;

private:
    const static bool _traceable;
};

}

#endif // NODE_H
