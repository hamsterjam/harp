#ifndef HARP_QUATERNION_H
#define HARP_QUATERNION_H

#include <cmath>
#include <cstring> // memcpy

#include <math/Vector.h>

template<typename T>
class Quaternion : public Vector<4, T> {
    public:
        Quaternion<T>() {}

        Quaternion<T>(T val[4]) : Vector<4, T>(val) {}

        Quaternion<T>(Vector<4, T> clone) {
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

//
// Builder Functions
//

template<typename T>
Quaternion<T> rotationQuaternion(T angle, Vector<3, T> axis) {
    axis *= sin(angle/2);
    Quaternion<T> ret = zeroVector<4, T>() + axis;
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
