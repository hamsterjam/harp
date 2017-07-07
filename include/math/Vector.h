#ifndef HARP_VECTOR_H
#define HARP_VECTOR_H

#include <cmath>

template <unsigned int N, typename T>
class Vector {
    public:
        T data[N];

        Vector<N, T>() {}

        Vector<N, T>(T val[N]) {
            for (int i = 0; i < N; ++i) {
                data[i] = val[i];
            }
        }

        T& operator[](unsigned int index) {
            return data[index];
        }

        template<typename T2>
        Vector<N, T>& operator*=(T2 scalar) {
            for (int i = 0; i < N; ++i) {
                data[i] *= scalar;
            }
            return *this;
        }

        template<typename T2>
        Vector<N, T>& operator/=(T2 scalar) {
            for (int i = 0; i < N; ++i) {
                data[i] /= scalar;
            }
            return *this;
        }

        Vector<N, T>& operator+=(Vector<N, T> rhs) {
            for (int i = 0; i < N; ++i) {
                data[i] += rhs[i];
            }
            return *this;
        }

        Vector<N, T>& operator-=(Vector<N, T> rhs) {
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
Vector<N, T> onesVector() {
    Vector<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = 1;
    }
    return ret;
}

template<unsigned int N, typename T>
Vector<N, T> zerosVector() {
    Vector<N, T> ret;
    for (int i = 0; i < N; ++i) {
        ret[i] = 0;
    }
    return ret;
}

template<unsigned int M, unsigned int N, typename T>
Vector<M, T> resize(Vector<N, T> op) {
    Vector<M, T> ret;
    for (int i = 0; i < N && i < M; ++i) {
        ret[i] = op[i];
    }
    return ret;
}

//
// Comparison
//

template<unsigned int N, typename T>
bool operator==(Vector<N, T> lhs, Vector<N, T> rhs) {
    for (int i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template<unsigned int N, typename T>
bool operator!=(Vector<N, T> lhs, Vector<N, T> rhs) {
    return !(lhs == rhs);
}

//
// Scalar Multiplication
//

template<unsigned int N, typename T, typename T2>
Vector<N, T> operator*(T2 scalar, Vector<N, T> vec) {
    vec *= scalar;
    return vec;
}

template<unsigned int N, typename T, typename T2>
Vector<N, T> operator*(Vector<N, T> vec, T2 scalar) {
    return scalar*vec;
}

template<unsigned int N, typename T, typename T2>
Vector<N, T> operator/(Vector<N, T> vec, T2 scalar) {
    vec /= scalar;
    return vec;
}

template<unsigned int N, typename T>
Vector<N, T> operator-(Vector<N, T> op) {
    return (T) -1 * op;
}

//
// Vector Addition
//

template<unsigned int N, typename T>
Vector<N, T> operator+(Vector<N, T> lhs, Vector<N, T> rhs) {
    lhs += rhs;
    return lhs;
}

template<unsigned int N, typename T>
Vector<N, T> operator-(Vector<N, T> lhs, Vector<N, T> rhs) {
    lhs -= rhs;
    return lhs;
}

//
// Special Operations
//

template<unsigned int N, typename T>
T dot(Vector<N, T> lhs, Vector<N, T> rhs) {
    T ret = lhs[0] * rhs[0];
    for (int i = 1; i < N; ++i) {
        ret += lhs[i] * rhs[i];
    }
    return ret;
}

template<typename T>
Vector<2, T> perp(Vector<2, T> op) {
    Vector<2, T> ret;
    ret[0] = -op[1];
    ret[1] =  op[0];

    return ret;
}

template<typename T>
Vector<3, T> cross(Vector<3, T> lhs, Vector<3, T> rhs) {
    Vector<3, T> ret;
    ret[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    ret[1] = lhs[3]*rhs[0] - lhs[0]*rhs[3];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];

    return ret;
}

//TODO// A general wedge product

template<unsigned int N, typename T>
T norm(Vector<N, T> op) {
    return sqrt(dot(op,op));
}

template<unsigned int N, typename T>
Vector<N, T> normalize(Vector<N, T> op) {
    return op/norm(op);
}

template<unsigned int N, typename T>
Vector<N, T> proj(Vector<N, T> lhs, Vector<N, T> rhs) {
    return dot(lhs,rhs)*normalize(rhs);
}

#endif
