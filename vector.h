#ifndef VECTOR3_H
#define VECTOR3_H
#include <stdint.h>
#include <iostream>
#include <math.h>
#include "matrix.h"

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

        // Vector as point matrix multiplication
        Vector3 as_point_multiplied(const Matrix44 &m) const
        {
            Vector3 result;
            double a, b, c, w;
            // Compute
            a = x * m[0][0] + y * m[1][0] + z * m[2][0] + m[3][0];
            b = x * m[0][1] + y * m[1][1] + z * m[2][1] + m[3][1];
            c = x * m[0][2] + y * m[1][2] + z * m[2][2] + m[3][2];
            w = x * m[0][3] + y * m[1][3] + z * m[2][3] + m[3][3];
            // Assign
            result.x = a / w;
            result.y = b / w;
            result.z = c / w;
            // Return
            return result;
        }

        // Vector as direction matrix multiplication
        Vector3 as_direction_multiplied(const Matrix44 &m) const
        {
            Vector3 result;
            double a, b, c;
            // Compute
            a = x * m[0][0] + y * m[1][0] + z * m[2][0];
            b = x * m[0][1] + y * m[1][1] + z * m[2][1];
            c = x * m[0][2] + y * m[1][2] + z * m[2][2];
            // Assign
            result.x = a;
            result.y = b;
            result.z = c;
            // Return
            return result;
        }

        // []
        const double& operator [] (uint8_t i) const
        { return (&x)[i]; }

        double& operator [] (uint8_t i)
        { return (&x)[i]; }

        // double / Vector3
        friend Vector3 operator / (const double &factor, const Vector3 &v)
        { return Vector3(factor / v.x, factor / v.y, factor / v.z); }

        // Ostream
        friend std::ostream& operator << (std::ostream &s, const Vector3 &v)
        {
            return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
        }

};

} // end namespace

#endif // VECTOR3_H
