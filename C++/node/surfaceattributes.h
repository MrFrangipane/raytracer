#ifndef SURFACEATTRIBUTES_H
#define SURFACEATTRIBUTES_H

#include "math/vector.h"
#include "image/color.h"


namespace frangiray {

class SurfaceAttributes
{
public:
    // Members
    RealColor diffuse_color;
    RealColor emission_color;
    f_real reflection_amount;
    f_real reflection_roughness;
    Vector normal;

    // Constructors
    SurfaceAttributes() {}
    SurfaceAttributes(
        const RealColor diffuse_color_,
        const RealColor emission_color_,
        const f_real reflection_amount_,
        const f_real reflection_roughness_,
        const Vector normal_
    )
        : diffuse_color(diffuse_color_),
          emission_color(emission_color_),
          reflection_amount(reflection_amount_),
          reflection_roughness(reflection_roughness_),
          normal(normal_) {}
};

}

#endif // SURFACEATTRIBUTES_H
