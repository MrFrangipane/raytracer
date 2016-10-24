#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>
#include <iostream>
#include <iomanip>
#include "common/typedefs.h"


namespace frangiray {

class Matrix
{
public:
    // Members
    f_real x[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    // Constructors
    Matrix();

    Matrix (
        f_real a, f_real b, f_real c, f_real d,
        f_real e, f_real f, f_real g, f_real h,
        f_real i, f_real j, f_real k, f_real l,
        f_real m, f_real n, f_real o, f_real p
    );

    // Multiplied
    Matrix multiplied(const Matrix &right) const;

    // Transposed
    Matrix transposed() const;

    // Transpose (on place)
    Matrix& transpose();

    // Inverted
    Matrix inverted() const;

    // Invert (on place)
    Matrix& invert();

    // []
    const f_real* operator [] (uint8_t index) const;

    f_real* operator [] (uint8_t index);

    // current * given
    Matrix operator * (const Matrix &right) const;

    // Ostream
    friend std::ostream& operator << (std::ostream &s, const Matrix &m);
};

}

#endif // MATRIX_H
