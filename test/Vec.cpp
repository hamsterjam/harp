#include <iostream>
#include <cassert>
#include <cmath>

#include <harpMath.h>

using namespace std;

int main(int argc, char** argv) {
    Vec<3, int> testVec1;
    testVec1[0] = 3;
    testVec1[1] = 4;
    testVec1[2] = 5;

    Vec<3, int> testVec2;
    testVec2[0] = 2;
    testVec2[1] = 1;
    testVec2[2] = 0;

    auto testVec3 = testVec1 + testVec2;

    assert(testVec3[0] == 5);
    assert(testVec3[1] == 5);
    assert(testVec3[2] == 5);

    auto testVec4 = testVec1 - testVec2;

    assert(testVec4[0] == 1);
    assert(testVec4[1] == 3);
    assert(testVec4[2] == 5);

    assert(dot(testVec1,testVec2) == 10);

    Vec<3, float> testFloatVec;
    testFloatVec[0] = 3;
    testFloatVec[1] = 4;
    testFloatVec[2] = 5;

    // Direct comparison of floats doesn't really work, so use a delta
    float delta = 0.000001;
    assert(abs(norm(testFloatVec) - sqrt(2)*5) < delta);

    auto normalized = normalize(testFloatVec);
    assert(abs(normalized[0] - 3./(sqrt(2) * 5)) < delta);
    assert(abs(normalized[1] - 2.*sqrt(2)/5) < delta);
    assert(abs(normalized[2] - 1./sqrt(2)) < delta);

    assert(abs(norm(normalized) - 1.0) < delta);

    Vec<3, int> x;
    x[0] = 1;
    x[1] = 0;
    x[2] = 0;

    Vec<3, int> y;
    y[0] = 0;
    y[1] = 1;
    y[2] = 0;

    Vec<3, int> z;
    z[0] = 0;
    z[1] = 0;
    z[2] = 1;

    assert(cross(x, y) ==  z);
    assert(cross(y, x) == -z);

    {
        Vec<3, int> test1;
        test1[0] = 1;
        test1[1] = 2;
        test1[2] = 3;

        Vec<4, int> test2 = zeroVec<4, int>() + test1;

        assert(test2[0] == 1 && test2[1] == 2 && test2[2] == 3 && test2[3] == 0);
    }

    {
        int aData[] = {2, 3, 0, 0};
        Vec<4, int> a(aData);

        int bData[] = {0, 5, 7, 0};
        Vec<4, int> b(bData);

        int cData[] = {0, 0, 11, 13};
        Vec<4, int> c(cData);

        Vec<4, int> vecs[] = {a, b, c};
        Vec<4, int> d = wedge(vecs);

        assert(dot(a, d) == 0 && dot(b, d) == 0 && dot(c, d) == 0);
        assert(d[0] == 273 && d[1] == -182 && d[2] == 130 && d[3] == -110);
    }

    cout << "All systems nominal" << endl;

    return 0;
}
