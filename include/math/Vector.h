#ifndef HARP_VECTOR_H
#define HARP_VECTOR_H

#include <cmath>

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

        Vec<N, T>& operator+=(Vec<N, T> rhs) {
            for (int i = 0; i < N; ++i) {
                data[i] += rhs[i];
            }
            return *this;
        }

        Vec<N, T>& operator-=(Vec<N, T> rhs) {
            for (int i = 0; i < N; ++i) {
                data[i] -= rhs[i];
            }
            return *this;
        }
};

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
Vec<N, T> zerosVec() {
    Vec<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = 0;
    }
    return ret;
}

template<unsigned int M, unsigned int N, typename T>
Vec<M, T> resize(Vec<N, T> op) {
    Vec<M, T> ret;
    for (int i = 0; i < N && i < M; ++i) {
        ret[i] = op[i];
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
// Vector Addition
//

template<unsigned int N, typename T>
Vec<N, T> operator+(Vec<N, T> lhs, Vec<N, T> rhs) {
    lhs += rhs;
    return lhs;
}

template<unsigned int N, typename T>
Vec<N, T> operator-(Vec<N, T> lhs, Vec<N, T> rhs) {
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
    ret[1] = lhs[3]*rhs[0] - lhs[0]*rhs[3];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

    return ret;
}

//TODO// A general wedge product

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

#endif
