#ifndef PIXEL_H
#define PIXEL_H

#include "common/typedefs.h"
#include "math/vector.h"
#include "image/color.h"


namespace frangiray {

class Pixel
{
public:
    // Members
    size_t object_id = 0;
    f_real depth = F_INFINITY;  // Infinity means no Object
    Vector world_normal;
    RealColor emission;
    RealColor albedo;
    RealColor direct_lighting;
    RealColor indirect_lighting;
    RealColor reflection;
    RealColor beauty;

    // Constructors
    Pixel() {}

    // Methods
    void compute_beauty();
};

}

#endif // PIXEL_H
