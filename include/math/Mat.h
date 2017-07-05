#ifndef HARP_MATRIX_H
#define HARP_MATRIX_H

#include <math/Vec.h>
#include <iostream>

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

#endif
