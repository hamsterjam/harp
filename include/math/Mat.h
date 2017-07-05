#ifndef HARP_MATRIX_H
#define HARP_MATRIX_H

#include <math/Vec.h>

template<unsigned int M, unsigned int N, typename T>
class Mat : public Vec<M, Vec<N, T> > {
    public:
        void set(T val[N*M]) {
            for (int row = 0; row < M; ++row) {
                for (int col = 0; col < N; ++col) {
                    (*this)[row][col] = val[row + col*M];
                }
            }
        }
};

//
// Unary Operations
//

template<unsigned int M, unsigned int N, typename T>
Mat<N, M, T> trans(Mat<M, N, T> op) {
    Mat<N, M, T> ret;
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
Mat<M, K, T> operator*(Mat<M, N, T> lhs, Mat<N, K, T> rhs) {
    Mat<K, N, T> rhsCols = trans(rhs);

    Mat<M, K, T> ret;
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < K; ++col) {
            ret[row][col] = dot(lhs[row], rhsCols[col]);
        }
    }
    return ret;
}

template<unsigned int M, unsigned int N, typename T>
Vec<M, T> operator*(Mat<M, N, T> mat, Vec<N, T> vec) {
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
Mat<N-1, N-1, T> minorMatrix(Mat<N, N, T> op, unsigned int rTarget, unsigned int cTarget) {
    Mat<N-1, N-1, T> ret;
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
T det(Mat<2, 2, T> op) {
    return op[0][0]*op[1][1] - op[0][1]*op[1][0];
}

template<unsigned int N, typename T>
T det(Mat<N, N, T> op) {
    // Expand on the first row and recurse
    T ret = (T) 0;
    for (int col = 0; col < N; ++col) {
        T sign = (col%2 == 0) ? (T) 1 : (T) -1;
        Mat<N-1, N-1, T> min = minorMatrix(op, 0, col);
        ret += sign * op[0][col] * det(min);
    }
    return ret;
}

#endif
