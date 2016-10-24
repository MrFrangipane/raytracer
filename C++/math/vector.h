#ifndef VECTOR_H
#define VECTOR_H

#include "common/typedefs.h"
#include "math/matrix.h"

namespace frangiray {

class Vector
{
public:
    // Members
    f_real x, y, z;

    // Constructors
    Vector() : x(f_real(0)), y(f_real(0)), z(f_real(0)) {}
    Vector(f_real x_) : x(x_), y(x_), z(x_) {}
    Vector(f_real x_, f_real y_, f_real z_) : x(x_), y(y_), z(z_) {}

    // +
    Vector operator + (const Vector &v) const;

    // -
    Vector operator - (const Vector &v) const;

    Vector operator - () const;

    // *
    Vector operator * (const f_real &factor) const;

    Vector operator * (const Vector &v) const;

    // *=
    Vector& operator *= (const f_real factor);

    // /=
    Vector& operator /= (const f_real factor);

    // dot_product
    f_real dot_product(const Vector &v) const;

    // cross_product
    Vector cross_product(const Vector &v) const;

    // norm
    f_real norm() const;

    // length
    f_real length() const;

    // normalize
    Vector& normalize();

    // Vector as point matrix multiplication
    Vector as_point_multiplied(const Matrix &m) const;

    // Vector as direction matrix multiplication
    Vector as_direction_multiplied(const Matrix &m) const;

    // []
    const f_real& operator [] (uint8_t i) const;

    f_real& operator [] (uint8_t i);

    // double / Vector3
    friend Vector operator / (const f_real &factor, const Vector &v);

    // Ostream
    friend std::ostream& operator << (std::ostream &s, const Vector &v);
};


inline
Vector random_direction()
{
    return Vector(
        random_real(),
        random_real(),
        random_real()
    ).normalize();
}


inline
Vector reflect(const Vector &incident, const Vector &normal)
{
    // Reflect
    return incident - (normal * (2 * incident.dot_product(normal)));
}

}

#endif // VECTOR_H
