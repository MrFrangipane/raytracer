#include "abstractnode.h"

namespace frangiray {

Vector AbstractNode::position() const
{
    return Vector(transform[3][0], transform[3][1], transform[3][2]);
}

}
