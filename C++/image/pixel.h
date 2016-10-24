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
    size_t object_id = 0;  // Internally, 0 means no Object
    f_real depth = F_INFINITY;
    Vector world_normal;
    RealColor albedo;
    RealColor direct_lighting;
    RealColor reflection;
    RealColor ambient;
    IntColor beauty;

    // Constructors
    Pixel() {}

    // Methods
    void compute_beauty();
};

}

#endif // PIXEL_H
