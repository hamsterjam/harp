#ifndef HARP_ECS_H
#define HARP_ECS_H

#include <cmath>

template <unsigned int N, typename T>
class Vec {
    private:
        T data[N];

    public:
        T& operator[](unsigned int index) {
            return data[index];
        }
};

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

//
// Scalar Multiplication
//

template<unsigned int N, typename T>
Vec<N, T> operator*(T scalarOp, Vec<N, T> vecOp) {
    Vec<N, T> ret = vecOp;
    for (int i = 0; i < N; ++i) {
        ret[i] *= scalarOp;
    }
    return ret;
}

template<unsigned int N, typename T>
Vec<N, T> operator*(Vec<N, T> vecOp, T scalarOp) {
    return scalarOp*vecOp;
}

template<unsigned int N, typename T>
Vec<N, T> operator/(Vec<N, T> vecOp, T scalarOp) {
    Vec<N, T> ret = vecOp;
    for (int i = 0; i < N; ++i) {
        ret[i] /= scalarOp;
    }
    return ret;
}

template<unsigned int N, typename T>
Vec<N, T> operator-(Vec<N, T> op) {
    return -1*op;
}

//
// Vector Addition
//

template<unsigned int N, typename T>
Vec<N, T> operator+(Vec<N, T> lhs, Vec<N, T> rhs) {
    Vec<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = lhs[i] + rhs[i];
    }
    return ret;
}

// Do it like this instead of just multiplying by -1 so unsigned types work
template<unsigned int N, typename T>
Vec<N, T> operator-(Vec<N, T> lhs, Vec<N, T> rhs) {
    Vec<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = lhs[i] - rhs[i];
    }
    return ret;
}

//
// Special Operations
//

template<unsigned int N, typename T>
T operator*(Vec<N, T> lhs, Vec<N, T> rhs) {
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
    ret[1] = lhs[3]*rhs[0] - lhs[0]*rhs[3];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

    return ret;
}

//TODO// A general wedge product

template<unsigned int N, typename T>
T norm(Vec<N, T> op) {
    return sqrt(op*op);
}

template<unsigned int N, typename T>
Vec<N, T> normalize(Vec<N, T> op) {
    return op/norm(op);
}

#endif
