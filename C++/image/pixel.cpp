#include "pixel.h"

namespace frangiray {

// Compute Beauty
void Pixel::compute_beauty()
{
    beauty.r = (albedo.r * (direct_lighting.r + ambient.r) + reflection.r) * 255;
    beauty.g = (albedo.g * (direct_lighting.g + ambient.g) + reflection.g) * 255;
    beauty.b = (albedo.b * (direct_lighting.b + ambient.b) + reflection.b) * 255;
}

}
