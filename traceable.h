#ifndef TRACEABLE_H
#define TRACEABLE_H
#include "matrix.h"
#include "vector"


namespace raytracer {

struct SurfaceInformation {
    Vector3 diffuse_color;
    Vector3 emission_color;
    Vector3 normal;
};

class Traceable {
    public:
        // Members
        Matrix44 transform;
        Vector3 diffuse_color;
        Vector3 emission_color;

        // Constructors
        Traceable(const Matrix44 transform_, const Vector3 diffuse_color_, const Vector3 emission_color_) :
            transform(transform_),
            diffuse_color(diffuse_color_),
            emission_color(emission_color_)
        {}

        virtual ~Traceable() {}
        virtual double hit_distance(const Vector3 &, const Vector3 &) const = 0;
        virtual SurfaceInformation information_at(const Vector3 &) const = 0;
};


} // end namespace

#endif // TRACEABLE_H
