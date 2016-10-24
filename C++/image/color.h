#ifndef COLOR_H
#define COLOR_H

#include "common/typedefs.h"


namespace frangiray {

struct RealColor {
    f_real r = 0.0;
    f_real g = 0.0;
    f_real b = 0.0;
    f_real a = 0.0;

    // Operators
    RealColor operator * (const f_real &factor) const
    {
        RealColor result;

        result.r = r * factor;
        result.g = g * factor;
        result.b = b * factor;
        result.a = a * factor;

        return result;
    }

    RealColor operator + (const RealColor &rc) const
    {
        RealColor result;

        result.r = r + rc.r;
        result.g = g + rc.g;
        result.b = b + rc.b;
        result.a = a + rc.a;

        return result;
    }
};

struct IntColor {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0;
};

}

#endif // COLOR_H
