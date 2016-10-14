#ifndef TRACEABLE_H
#define TRACEABLE_H
#include "matrix.h"
#include "color.h"
#include "vector.h"


namespace raytracer {

struct SurfaceAttributes {
    Color diffuse_color;
    Color emission_color;
    Vector3 normal;
};

class Traceable {
    public:
        // Members
        Matrix44 object_to_world;
        Matrix44 world_to_object;
        Color diffuse_color;
        Color emission_color;

        // Constructors
        Traceable(const Matrix44 object_to_world_, const Color diffuse_color_, const Color emission_color_) :
            object_to_world(object_to_world_),
            world_to_object(object_to_world_.inverted()),
            diffuse_color(diffuse_color_),
            emission_color(emission_color_)
        {}

        virtual ~Traceable() {}
        virtual double hit_distance(const Vector3 &, const Vector3 &) const = 0;
        virtual SurfaceAttributes surface_attributes_at(const Vector3 &) const = 0;
};


} // end namespace

#endif // TRACEABLE_H
