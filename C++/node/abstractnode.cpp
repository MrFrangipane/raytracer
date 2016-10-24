#include "abstractnode.h"

namespace frangiray {

Vector AbstractNode::position() const
{
    return Vector(
        local_to_world[3][0],
        local_to_world[3][1],
        local_to_world[3][2]
    );
}

}
