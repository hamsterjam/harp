#include <cassert>
#include <cmath>

#include <iostream>

#include <harpMath.h>

using namespace std;

int main(int argc, char** argv) {
    {
        Quaternion<int> i = zeroVector<4, int>();
        i[0] = 1;

        Quaternion<int> j = zeroVector<4, int>();
        j[1] = 1;

        Quaternion<int> k = zeroVector<4, int>();
        k[2] = 1;

        Quaternion<int> res1 = i*i;
        Quaternion<int> res2 = j*j;
        Quaternion<int> res3 = k*k;
        Quaternion<int> res4 = i*j*k;

        assert(res1[0] == 0 && res1[1] == 0 && res1[2] == 0 && res1[3] == -1);
        assert(res2[0] == 0 && res2[1] == 0 && res2[2] == 0 && res2[3] == -1);
        assert(res3[0] == 0 && res3[1] == 0 && res3[2] == 0 && res3[3] == -1);
        assert(res4[0] == 0 && res4[1] == 0 && res4[2] == 0 && res4[3] == -1);
    }

    {
        double data[] = {1, 1, 1, 1};
        Quaternion<double> quat(data);
        quat = normalize(quat);

        assert(quat[0] == 0.5 && quat[1] == 0.5 && quat[2] == 0.5 && quat[3] == 0.5);
    }

    {
        double data[] = {1, 2, 3, 4};
        Quaternion<double> q1(data);
        Quaternion<double> q2 = q1*inverse(q1);

        assert(q2[0] == 0 && q2[1] == 0 && q2[2] == 0 && q2[3] == 1);
    }

    {
        double data[] = {5, 7, 11, 0};
        Quaternion<double> pQuat(data);
        Vector<3, double>  pVec = zeroVector<3, double>() + pQuat;

        double axisData[] = {1, -2, 3};
        Vector<3, double> axis(axisData);
        axis = normalize(axis);

        Quaternion<double>   rQuat = rotationQuaternion(M_PI*7/5, axis);
        Matrix<3, 3, double> rMat  = conjugationTransformation(rQuat);

        Quaternion<double> resQuat = rQuat * pQuat * inverse(rQuat);
        Vector<3, double>  resVec  = rMat * pVec;

        double delta = 0.000000000001;
        for (int i=0; i<3; ++i) {
            assert(abs(resQuat[i] - resVec[i]) < delta);
        }
    }

    cout << "All systems nominal" << endl;
    return 0;
}
