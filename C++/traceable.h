#ifndef TRACEABLE_H
#define TRACEABLE_H

#include "utils.h"
#include "matrix.h"
#include "color.h"
#include "vector.h"
#include "ray.h"


namespace raytracer {

struct SurfaceAttributes {
    Color diffuse_color;
    Color emission_color;
    double reflection_amount;
    double reflection_blur;
    Vector3 normal;
};

class Traceable {
    public:
        // Members
        Matrix44 object_to_world;
        Matrix44 world_to_object;
        Vector3 position;
        Color diffuse_color;
        Color emission_color;
        double reflection_amount;
        double reflection_blur;

        // Constructors
        Traceable(const Matrix44 object_to_world_, const Color diffuse_color_, const Color emission_color_, const double reflection_amount_, const double reflection_blur_) :
            object_to_world(object_to_world_),
            world_to_object(object_to_world_.inverted()),
            diffuse_color(diffuse_color_),
            emission_color(emission_color_),
            reflection_amount(reflection_amount_),
            reflection_blur(reflection_blur_)
        {
            Vector3 world(0);
            position = world.as_point_multiplied(object_to_world);
        }

        virtual ~Traceable() {}
        virtual double hit_distance(const Ray &ray) const = 0;
        virtual SurfaceAttributes surface_attributes_at(const Vector3 &position) const = 0;
        virtual Vector3 random_position() const = 0;
};


} // end namespace

#endif // TRACEABLE_H
