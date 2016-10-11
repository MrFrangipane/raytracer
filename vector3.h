#ifndef VECTOR3_H
#define VECTOR3_H
#include "stdint.h"
#include "math.h"

namespace raytracer {

class Vector3 {
    public:
        // Members
        double x, y, z;

        // Constructors
        Vector3() : x(double(0)), y(double(0)), z(double(0)) {}
        Vector3(double x_) : x(x_), y(x_), z(x_) {}
        Vector3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

        // +
        Vector3 operator + (const Vector3 &v) const
        { return Vector3(x + v.x, y + v.y, z + v.z); }

        // -
        Vector3 operator - (const Vector3 &v) const
        { return Vector3(x - v.x, y - v.y, z - v.z); }

        Vector3 operator - () const
        { return Vector3(-x, -y, -z); }

        // *
        Vector3 operator * (const float &factor) const
        { return Vector3(x * factor, y * factor, z * factor); }

        Vector3 operator * (const Vector3 &v) const
        { return Vector3(x * v.x, y * v.y, z * v.z); }

        // *=
        Vector3& operator *= (const double factor)
        { x *= factor, y *= factor, z *= factor; return *this; }

        // /=
        Vector3& operator /= (const double factor)
        { x /= factor, y /= factor, z /= factor; return *this; }

        // dot_product
        double dot_product(const Vector3 &v) const
        { return x * v.x + y * v.y + z * v.z; }

        // cross_product
        Vector3 cross_product(const Vector3 &v) const
        { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

        // norm
        double norm() const
        { return x * x + y * y + z * z; }

        // length
        double length() const
        { return sqrt(norm()); }

        // normalize
        Vector3& normalize()
        {
            double n = norm();
            if (n > 0) {
                double factor = 1 / sqrt(n);
                x *= factor, y *= factor, z *= factor;
            }

            return *this;
        }

        // []
        const double& operator [] (uint8_t i) const
        { return (&x)[i]; }

        double& operator [] (uint8_t i)
        { return (&x)[i]; }

};

} // end namespace

#endif // VECTOR3_H
