#ifndef NODE_H
#define NODE_H

#include <string>
#include "math/vector.h"
#include "math/matrix.h"
#include "math/ray.h"
#include "math/rayhit.h"


namespace frangiray {

class AbstractNode
{
public:
    // Members
    Matrix transform;
    std::string name;
    const static bool traceable = false;

    // Constructors
    AbstractNode(const std::string name_)
        : name(name_) {}

    AbstractNode(const std::string name_, const Matrix transform_)
        : name(name_), transform(transform_) {}

    // Methods
    Vector position() const;
    virtual f_real intersection_distance(const Ray &ray) const = 0;
};

}

#endif // NODE_H
