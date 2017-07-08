#ifndef HARP_MATRIX_H
#define HARP_MATRIX_H

#include <cstring> // memcpy

#include <math/Vector.h>

template<unsigned int M, unsigned int N, typename T>
class Matrix;

template<unsigned int M, unsigned int N, unsigned int K, typename T>
Matrix<M, K, T> operator*(Matrix<M, N, T> lhs, Matrix<N, K, T> rhs);

template<unsigned int M, unsigned int N, typename T>
class Matrix : public Vector<M, Vector<N, T> > {
    public:
        Matrix<M, N, T>() {}

        Matrix<M, N, T>(T val[M*N]) {
            for (int r = 0; r < M; ++r) {
                for (int c = 0; c < N; ++c) {
                    (*this)[r][c] = val[r + c*M];
                }
            }
        }

        Matrix<M, N, T>(Vector<M, Vector<N, T> > clone) {
            std::memcpy((void*) &(this->data), (void*) &(clone.data), M * sizeof(Vector<N, T>));
        }

        Matrix<M, N, T>& operator*=(Matrix<N, N, T> rhs) {
            *this = *this * rhs;
            return *this;
        }
};

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
Matrix<3, 3, T> conjugationTransformation(Vector<4, T> q) {
    // q is a Vec<4, T> instead of a Quaternion<T> because dependancy
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

    // Row 2
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
Vector<M, T> operator*(Matrix<M, N, T> mat, Vector<N, T> vec) {
    Vector<M, T> ret;
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

#endif
