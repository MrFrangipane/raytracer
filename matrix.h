#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <iomanip>

namespace raytracer {

class Matrix44 {
    public:
        // Members
        double x[4][4] = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };

        // Constructors
        Matrix44 () {}

        Matrix44 (double a, double b, double c, double d,
                  double e, double f, double g, double h,
                  double i, double j, double k, double l,
                  double m, double n, double o, double p)
        {
            x[0][0] = a; x[0][1] = b; x[0][2] = c; x[0][3] = d;
            x[1][0] = e; x[1][1] = f; x[1][2] = g; x[1][3] = h;
            x[2][0] = i; x[2][1] = j; x[2][2] = k; x[2][3] = l;
            x[3][0] = m; x[3][1] = n; x[3][2] = o; x[3][3] = p;

        }

        // Multiplied
        Matrix44 multiplied(const Matrix44 &right) const
        {
            Matrix44 result;
            // Each element of resulting matrix
            for (uint8_t row = 0; row < 4; row++) {
                for (uint8_t column = 0; column < 4; column++) {
                    // Compute
                    result[row][column] = *this[row][0] * right[0][column]
                                        + *this[row][1] * right[1][column]
                                        + *this[row][2] * right[2][column]
                                        + *this[row][3] * right[3][column];
                }
            }
            // Return
            return result;
        }

        // Transposed
        Matrix44 transposed() const
        {
            Matrix44 result;
            // Each element of resulting matrix
            for (uint8_t row = 0; row < 4; row++) {
                for (uint8_t column = 0; column < 4; column++) {
                    // Compute
                    result[row][column] = x[column][row];
                }
            }
            // Return
            return result;
        }

        // Transpose (on place)
        Matrix44& transpose()
        {
            *this = transposed();
            return *this;
        }

        // Inverted
        Matrix44 inverted() const
        {
            Matrix44 result;
            Matrix44 copy (*this);

            // Forward elimination
            for (int i = 0; i < 3; i++) {
                int pivot = i;

                double pivot_size = copy[i][i];

                if (pivot_size < 0) {
                    pivot_size = -pivot_size;
                }

                for (int j = i + 1; j < 4; j++) {
                    double temp = copy[j][i];

                    if (temp < 0) {
                        temp = -temp;
                    }

                    if (temp > pivot_size) {
                        pivot = j;
                        pivot_size = temp;
                    }
                }

                if (pivot_size == 0) {
                    // Cannot invert singular matrix
                    return Matrix44();
                }

                if (pivot != i) {
                    for (int j = 0; j < 4; j++) {
                        double temp;

                        temp = copy[i][j];
                        copy[i][j] = copy[pivot][j];
                        copy[pivot][j] = temp;

                        temp = result[i][j];
                        result[i][j] = result[pivot][j];
                        result[pivot][j] = temp;
                    }
                }

                for (int j = i + 1; j < 4; j++) {
                    double factor = copy[j][i] / copy[i][i];

                    for (int k = 0; k < 4; k++) {
                        copy[j][k] -= factor * copy[i][k];
                        result[j][k] -= factor * result[i][k];
                    }
                }
            }

            // Backward substitution
            for (int i = 3; i >= 0; --i) {
                double factor;

                if ((factor = copy[i][i]) == 0) {
                    // Cannot invert singular matrix
                    return Matrix44();
                }

                for (int j = 0; j < 4; j++) {
                    copy[i][j] /= factor;
                    result[i][j] /= factor;
                }

                for (int j = 0; j < i; j++) {
                    factor = copy[j][i];

                    for (int k = 0; k < 4; k++) {
                        copy[j][k] -= factor * copy[i][k];
                        result[j][k] -= factor * result[i][k];
                    }
                }
            }

            return result;
        }

        // Invert (on place)
        Matrix44& invert()
        {
            *this = inverted();
            return *this;
        }

        // []
        const double* operator [] (uint8_t index) const
        { return x[index]; }

        double* operator [] (uint8_t index)
        { return x[index]; }

        // current * given
        Matrix44 operator * (const Matrix44 &right) const
        { return this->multiplied(right); }

        // Ostream
        friend std::ostream& operator << (std::ostream &s, const Matrix44 &m)
        {
            std::ios_base::fmtflags oldFlags = s.flags();
            int width = 10; // total with of the displayed number
            s.precision(6); // control the number of displayed decimals
            s.setf (std::ios_base::fixed);

            s << "[" << std::setw (width) << m[0][0] <<
                 " " << std::setw (width) << m[0][1] <<
                 " " << std::setw (width) << m[0][2] <<
                 " " << std::setw (width) << m[0][3] << "\n" <<

                 " " << std::setw (width) << m[1][0] <<
                 " " << std::setw (width) << m[1][1] <<
                 " " << std::setw (width) << m[1][2] <<
                 " " << std::setw (width) << m[1][3] << "\n" <<

                 " " << std::setw (width) << m[2][0] <<
                 " " << std::setw (width) << m[2][1] <<
                 " " << std::setw (width) << m[2][2] <<
                 " " << std::setw (width) << m[2][3] << "\n" <<

                 " " << std::setw (width) << m[3][0] <<
                 " " << std::setw (width) << m[3][1] <<
                 " " << std::setw (width) << m[3][2] <<
                 " " << std::setw (width) << m[3][3] << "]";

            s.flags (oldFlags);
            return s;
        }
};

}


#endif // MATRIX_H
