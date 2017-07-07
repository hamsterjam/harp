#include<cassert>

#include<iostream>

#include<math/Quaternion.h>
#include<math/Vec.h>

using namespace std;

int main(int argc, char** argv) {
    {
        int iData[] = {1, 0, 0, 0};
        Quaternion<int> i(iData);

        int jData[] = {0, 1, 0, 0};
        Quaternion<int> j(jData);

        int kData[] = {0, 0, 1, 0};
        Quaternion<int> k(kData);

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

    cout << "All systems nominal" << endl;
    return 0;
}
