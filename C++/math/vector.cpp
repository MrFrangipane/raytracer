#include "vector.h"

namespace frangiray {

// +
Vector Vector::operator + (const Vector &v) const
{ return Vector(x + v.x, y + v.y, z + v.z); }

// -
Vector Vector::operator - (const Vector &v) const
{ return Vector(x - v.x, y - v.y, z - v.z); }

Vector Vector::operator - () const
{ return Vector(-x, -y, -z); }

// *
Vector Vector::operator * (const double &factor) const
{ return Vector(x * factor, y * factor, z * factor); }

Vector Vector::operator * (const Vector &v) const
{ return Vector(x * v.x, y * v.y, z * v.z); }

// *=
Vector& Vector::operator *= (const f_real factor)
{ x *= factor, y *= factor, z *= factor; return *this; }

// /=
Vector& Vector::operator /= (const f_real factor)
{ x /= factor, y /= factor, z /= factor; return *this; }

// dot_product
f_real Vector::dot_product(const Vector &v) const
{ return x * v.x + y * v.y + z * v.z; }

// cross_product
Vector Vector::cross_product(const Vector &v) const
{ return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

// norm
f_real Vector::norm() const
{ return x * x + y * y + z * z; }

// length
f_real Vector::length() const
{ return sqrt(norm()); }

// normalize
Vector& Vector::normalize()
{
    f_real n = norm();
    if (n > 0) {
        f_real factor = 1 / sqrt(n);
        x *= factor, y *= factor, z *= factor;
    }

    return *this;
}

// Vector as point matrix multiplication
Vector Vector::as_point_multiplied(const Matrix &m) const
{
    Vector result;
    f_real a, b, c, w;
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
Vector Vector::as_direction_multiplied(const Matrix &m) const
{
    Vector result;
    f_real a, b, c;
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
const f_real& Vector::operator [] (uint8_t i) const
{ return (&x)[i]; }

f_real& Vector::operator [] (uint8_t i)
{ return (&x)[i]; }

// double / Vector3
Vector operator / (const f_real &factor, const Vector &v)
{ return Vector(factor / v.x, factor / v.y, factor / v.z); }

// Ostream
std::ostream& operator << (std::ostream &s, const Vector &v)
{
    return s << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
}

}
