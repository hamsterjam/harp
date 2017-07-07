#ifndef HARP_QUATERNION_H
#define HARP_QUATERNION_H

#include <cmath>
#include <cstring> // memcpy

#include <math/Vec.h>

template<typename T>
class Quaternion : public Vec<4, T> {
    public:
        Quaternion<T>() {}

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

template<typename T>
Quaternion<T> operator*(Quaternion<T> lhs, Quaternion<T> rhs) {
    lhs *= rhs;
    return lhs;
}

#endif
