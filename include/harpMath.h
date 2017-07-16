/*
 * HARP
 * harpMath.h
 *
 * This defines 3 useful linear algebra template classes as well as the various
 * arithemetic operators and standard functions associated with them.
 *
 * The classes defined are:
 *
 * Vec<N, T>
 *     a vector (as in a math vector) of dimension N that stores type T. This
 *     is memory equivalent to an array T[N]
 *
 * Matrix<M, N, T>
 *     a matrix of dimension M*N that stores type T. This is memory equivalent
 *     to an array in row major order. This is actually equivalent to a
 *     Vec<M, Vec<N, T>> so it inherits arithematic from Vec
 *
 * Quaternion<T>
 *     a quaternion of type T. This is just a Vec<4, T> with a special
 *     quaternion multiplication defined. The component order is
 *     {i, j, k, r} to make it easy to convert between aa Vec<3, T> and the
 *     quaternion that it represents.
 *
 * It's probably worth mentioning that I had to call it Vec instead of Vector
 * to avoid the name collision with std::vector. God dammit Alex Stepanov, I
 * blame you for this! Why std::vector is nothing like a vector?!
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_MATH_H
#define HARP_MATH_H

#include <cmath>
#include <cstring> // memcpy

template <unsigned int N, typename T>
class Vec {
    public:
        T data[N];

        Vec<N, T>() {}

        Vec<N, T>(T val[N]) {
            for (int i = 0; i < N; ++i) {
                data[i] = val[i];
            }
        }

        T& operator[](unsigned int index) {
            return data[index];
        }

        template<typename T2>
        Vec<N, T>& operator*=(T2 scalar) {
            for (int i = 0; i < N; ++i) {
                data[i] *= scalar;
            }
            return *this;
        }

        template<typename T2>
        Vec<N, T>& operator/=(T2 scalar) {
            for (int i = 0; i < N; ++i) {
                data[i] /= scalar;
            }
            return *this;
        }

        template<unsigned int N2, typename T2>
        Vec<N, T>& operator+=(Vec<N2, T2> rhs) {
            for (int i = 0; i < N && i < N2; ++i) {
                data[i] += rhs[i];
            }
            return *this;
        }

        template<unsigned int N2, typename T2>
        Vec<N, T>& operator-=(Vec<N2, T2> rhs) {
            for (int i = 0; i < N && i < N2; ++i) {
                data[i] -= rhs[i];
            }
            return *this;
        }
};

template<unsigned int M, unsigned int N, typename T>
class Matrix : public Vec<M, Vec<N, T> > {
    public:
        Matrix<M, N, T>() {}

        Matrix<M, N, T>(T val[M*N]) {
            for (int r = 0; r < M; ++r) {
                for (int c = 0; c < N; ++c) {
                    (*this)[r][c] = val[r + c*M];
                }
            }
        }

        Matrix<M, N, T>(Vec<M, Vec<N, T> > clone) {
            std::memcpy((void*) &(this->data), (void*) &(clone.data), M * sizeof(Vec<N, T>));
        }

        // This uses the binary * which is defined later
        Matrix<M, N, T>& operator*=(Matrix<N, N, T> rhs) {
            *this = *this * rhs;
            return *this;
        }
};

template<typename T>
class Quaternion : public Vec<4, T> {
    public:
        Quaternion<T>() {}

        Quaternion<T>(T val[4]) : Vec<4, T>(val) {}

        Quaternion<T>(Vec<4, T> clone) {
            std::memcpy((void*) &(this->data), (void*) &(clone.data), sizeof(T) * 4);
        }

        Quaternion<T>& operator*=(Quaternion<T> rhs) {
            T ai = (*this)[0];
            T aj = (*this)[1];
            T ak = (*this)[2];
            T ar = (*this)[3];

            T bi = rhs[0];
            T bj = rhs[1];
            T bk = rhs[2];
            T br = rhs[3];

            (*this)[0] = ar*bi + ai*br + aj*bk - ak*bj;
            (*this)[1] = ar*bj + aj*br + ak*bi - ai*bk;
            (*this)[2] = ar*bk + ak*br + ai*bj - aj*bi;
            (*this)[3] = ar*br - ai*bi - aj*bj - ak*bk;

            return *this;
        }
};

/********************
 * Vec Functions *
 ********************/

//
// Builder Functions
//

template<unsigned int N, typename T>
Vec<N, T> onesVec() {
    Vec<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = 1;
    }
    return ret;
}

template<unsigned int N, typename T>
Vec<N, T> zeroVec() {
    Vec<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = 0;
    }
    return ret;
}

//
// Comparison
//

template<unsigned int N, typename T>
bool operator==(Vec<N, T> lhs, Vec<N, T> rhs) {
    for (int i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template<unsigned int N, typename T>
bool operator!=(Vec<N, T> lhs, Vec<N, T> rhs) {
    return !(lhs == rhs);
}

//
// Scalar Multiplication
//

template<unsigned int N, typename T, typename T2>
Vec<N, T> operator*(T2 scalar, Vec<N, T> vec) {
    vec *= scalar;
    return vec;
}

template<unsigned int N, typename T, typename T2>
Vec<N, T> operator*(Vec<N, T> vec, T2 scalar) {
    return scalar*vec;
}

template<unsigned int N, typename T, typename T2>
Vec<N, T> operator/(Vec<N, T> vec, T2 scalar) {
    vec /= scalar;
    return vec;
}

template<unsigned int N, typename T>
Vec<N, T> operator-(Vec<N, T> op) {
    return (T) -1 * op;
}

//
// Vec Addition
//

template<unsigned int N, unsigned int N2, typename T, typename T2>
Vec<N, T> operator+(Vec<N, T> lhs, Vec<N2, T2> rhs) {
    lhs += rhs;
    return lhs;
}

template<unsigned int N, unsigned int N2, typename T, typename T2>
Vec<N, T> operator-(Vec<N, T> lhs, Vec<N2, T2> rhs) {
    lhs -= rhs;
    return lhs;
}

//
// Special Operations
//

template<unsigned int N, typename T>
T dot(Vec<N, T> lhs, Vec<N, T> rhs) {
    T ret = lhs[0] * rhs[0];
    for (int i = 1; i < N; ++i) {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

template<typename T>
Vec<2, T> perp(Vec<2, T> op) {
    Vec<2, T> ret;
    ret[0] = -op[1];
    ret[1] =  op[0];

    return ret;
}

template<typename T>
Vec<3, T> cross(Vec<3, T> lhs, Vec<3, T> rhs) {
    Vec<3, T> ret;
    ret[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    ret[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

    return ret;
}

// This uses det and minorMatrix which are defined later
template<unsigned int N, typename T>
Vec<N, T> wedge(Vec<N, T> args[N-1]) {
    // You should look this up if your confused.
    // You build up a matrix of all the vectors, the components of the new vector
    // are minors of this matrix

    Matrix<N, N, T> vecMatrix;
    for (int i = 0; i < N-1; ++i) {
        vecMatrix[i+1] = args[i];
    }
    // row 0 is always going to be deleted by minorMatrix so its all good

    Vec<N, T> ret;
    for (int i = 0; i < N; ++i) {
        T sign = (i % 2 == 0) ? (T) 1 : (T) -1;

        ret[i] = sign * det(minorMatrix(vecMatrix, 0, i));
    }

    return ret;
}

template<unsigned int N, typename T>
T norm(Vec<N, T> op) {
    return sqrt(dot(op,op));
}

template<unsigned int N, typename T>
Vec<N, T> normalize(Vec<N, T> op) {
    return op/norm(op);
}

template<unsigned int N, typename T>
Vec<N, T> proj(Vec<N, T> lhs, Vec<N, T> rhs) {
    return dot(lhs,rhs)*normalize(rhs);
}

/********************
 * Matrix Functions *
 ********************/

//
// Builder Functions
//

template<unsigned int N, typename T>
Matrix<N, N, T> identityMatrix() {
    Matrix<N, N, T> ret;
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            ret[row][col] = (row == col) ? 1 : 0;
        }
    }
    return ret;
}

template<unsigned int M, unsigned int N, typename T>
Matrix<M, N, T> zeroMatrix() {
    Matrix<M, N, T> ret;
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < N; ++col) {
            ret[row][col] = 0;
        }
    }
    return ret;
}

template<unsigned int M, unsigned int N, typename T>
Matrix<M, N, T> onesMatrix() {
    Matrix<M, N, T> ret;
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < N; ++col) {
            ret[row][col] = 1;
        }
    }
    return ret;
}

template<typename T>
Matrix<3, 3, T> conjugationTransformation(Quaternion<T> q) {
    T qi = q[0];
    T qj = q[1];
    T qk = q[2];
    T qr = q[3];

    T s = (T) 1 / dot(q, q);

    Matrix<3, 3, T> ret;

    // Row 1
    ret[0][0] = 1 - 2*s*(qj*qj +qk*qk);
    ret[0][1] = 2*s*(qi*qj - qk*qr);
    ret[0][2] = 2*s*(qi*qk + qj*qr);

    // Row 2
    ret[1][0] = 2*s*(qi*qj + qk*qr);
    ret[1][1] = 1 - 2*s*(qi*qi + qk*qk);
    ret[1][2] = 2*s*(qj*qk - qi*qr);

    // Row 3
    ret[2][0] = 2*s*(qi*qk - qj*qr);
    ret[2][1] = 2*s*(qj*qk + qi*qr);
    ret[2][2] = 1 - 2*s*(qi*qi + qj*qj);

    return ret;
}

//
// Unary Operations
//

template<unsigned int M, unsigned int N, typename T>
Matrix<N, M, T> trans(Matrix<M, N, T> op) {
    Matrix<N, M, T> ret;
    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < M; ++col) {
            ret[row][col] = op[col][row];
        }
    }
    return ret;
}

//
// Matrix Multiplication
//

template<unsigned int M, unsigned int N, unsigned int K, typename T>
Matrix<M, K, T> operator*(Matrix<M, N, T> lhs, Matrix<N, K, T> rhs) {
    Matrix<K, N, T> rhsCols = trans(rhs);

    Matrix<M, K, T> ret;
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < K; ++col) {
            ret[row][col] = dot(lhs[row], rhsCols[col]);
        }
    }
    return ret;
}

template<unsigned int M, unsigned int N, typename T>
Vec<M, T> operator*(Matrix<M, N, T> mat, Vec<N, T> vec) {
    Vec<M, T> ret;
    for (int i = 0; i < M; ++i) {
        ret[i] = dot(vec, mat[i]);
    }
    return ret;
}

//
// Other stuff
//

template<unsigned int N, typename T>
Matrix<N-1, N-1, T> minorMatrix(Matrix<N, N, T> op, unsigned int rTarget, unsigned int cTarget) {
    Matrix<N-1, N-1, T> ret;
    for (int row = 0; row < N-1; ++row) {
        int rOff = (row < rTarget) ? 0 : 1;
        for (int col = 0; col < N-1; ++col) {
            int cOff = (col < cTarget) ? 0 : 1;
            ret[row][col] = op[row+rOff][col+cOff];
        }
    }
    return ret;
}

template<typename T>
T det(Matrix<2, 2, T> op) {
    return op[0][0]*op[1][1] - op[0][1]*op[1][0];
}

template<unsigned int N, typename T>
T det(Matrix<N, N, T> op) {
    // Expand on the first row and recurse
    T ret = (T) 0;
    for (int col = 0; col < N; ++col) {
        T sign = (col%2 == 0) ? (T) 1 : (T) -1;
        Matrix<N-1, N-1, T> min = minorMatrix(op, 0, col);
        ret += sign * op[0][col] * det(min);
    }
    return ret;
}

template<unsigned int N, typename T>
Matrix<N, N, T> inverse(Matrix<N, N, T> op) {
    Matrix<N, N, T> id = identityMatrix<N, T>();
    // This is row reduction, the rule is, whatever we do to op, we do to id
    for (int r = 0; r < N; ++r) {
        // Keep track of the value on the diagonal, we are going to pretend
        // to divide every value on the row by this (but do it later)
        T pivot = op[r][r];

        // For each other row, add the right amount to make the coloumn all zeros
        for (int rr = 0; rr < N; ++rr) {
            if (rr == r) continue;
            T target = op[rr][r];
            op[rr] -= op[r] * target / pivot;
            id[rr] -= id[r] * target / pivot;
        }

        // Now set the pivot to one
        op[r] /= pivot;
        id[r] /= pivot;
    }

    return id;
}

/************************
 * Quaternion Functions *
 ************************/

//
// Builder Functions
//

template<typename T>
Quaternion<T> rotationQuaternion(T angle, Vec<3, T> axis) {
    axis *= sin(angle/2);
    Quaternion<T> ret = zeroVec<4, T>() + axis;
    ret[3] = cos(angle/2);

    return ret;
}

//
// Arithmetic
//

template<typename T>
Quaternion<T> operator*(Quaternion<T> lhs, Quaternion<T> rhs) {
    lhs *= rhs;
    return lhs;
}

//
// Standard Operations
//

template<typename T>
Quaternion<T> conj(Quaternion<T> op) {
    for (int i = 0; i < 3; ++i) {
        op[i] = -op[i];
    }
    return op;
}

template<typename T>
Quaternion<T> inverse(Quaternion<T> op) {
    return conj(op)/dot(op, op);
}

#endif
