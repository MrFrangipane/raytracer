#include "pixel.h"

namespace frangiray {

// Compute Beauty
void Pixel::compute_beauty()
{
    beauty.r = emission.r + albedo.r * (direct_lighting.r + indirect_lighting.r) + reflection.r;
    beauty.g = emission.g + albedo.g * (direct_lighting.g + indirect_lighting.g) + reflection.g;
    beauty.b = emission.b + albedo.b * (direct_lighting.b + indirect_lighting.b) + reflection.b;
}

}
