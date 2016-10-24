#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include "common/typedefs.h"
#include "math/ray.h"
#include "math/rayhit.h"
#include "node/abstractnode.h"


namespace frangiray {

class Sphere: public AbstractNode
{
public:
    // Members
    f_real radius = 1.0;
    const static bool traceable = true;

    // Constructors
    Sphere(const std::string name_)
        : AbstractNode(name_) {}

    Sphere(const std::string name_, const f_real radius_)
        : AbstractNode(name_), radius(radius_), _radius2(radius_ * radius_) {}

    Sphere(const std::string name_, const Matrix &transform_)
        : AbstractNode(name_, transform_) {}

    Sphere(const std::string name_, const Matrix &transform_, const f_real radius_)
        : AbstractNode(name_, transform_), radius(radius_), _radius2(radius_ * radius_) {}

    // Methods
    f_real intersection_distance(const Ray &ray) const;


private:
    // Members
    f_real _radius2 = 1.0;

};

}

#endif // SPHERE_H
