#include "sphere.h"

namespace frangiray {

f_real Sphere::intersection_distance(const Ray &ray) const
{
    // Init
    f_real tca, thc, d2, t0, t1;

    // Compute
    Vector L = position() - ray.origin;
    tca = L.dot_product(ray.direction);

    // Exit
    if (tca < 0) return -1;

    d2 = L.dot_product(L) - (tca * tca);
    if (d2 > _radius2) return -1;

    thc = sqrt(_radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1;

        // Exit
        if (t0 < 0) return -1;
    }

    // Return
    return t0;
}

}
