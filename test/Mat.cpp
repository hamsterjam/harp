#include <iostream>
#include <cassert>

#include <math/Mat.h>
#include <math/Vec.h>

#include <cmath>

using namespace std;

int main(int argc, char** argv) {
    {
        int data1[] = {
            1, 4, 7,
            2, 5, 8,
            3, 6, 9
        };

        Mat<3, 3, int> testMat1;
        testMat1.set(data1);

        assert(testMat1[0][0] == 1);
        assert(testMat1[1][0] == 4);
        assert(testMat1[1][2] == 6);

        Mat<3, 3, int> transMat = trans(testMat1);

        assert(transMat[0][0] == 1);
        assert(transMat[1][0] == 2);
        assert(transMat[1][2] == 8);
    }

    {
        int data1[] = {
            1, 3, 5,
            2, 4, 6
        };
        int data2[] = {
            7, 10,
            8, 11,
            9, 12
        };
        Mat<3, 2, int> testMat1;
        Mat<2, 3, int> testMat2;
        testMat1.set(data1);
        testMat2.set(data2);

        Mat<3, 3, int> resMat1 = testMat1 * testMat2;
        Mat<2, 2, int> resMat2 = testMat2 * testMat1;

        assert(resMat1[0][0] == 27 && resMat1[0][1] ==  30 && resMat1[0][2] ==  33);
        assert(resMat1[1][0] == 61 && resMat1[1][1] ==  68 && resMat1[1][2] ==  75);
        assert(resMat1[2][0] == 95 && resMat1[2][1] == 106 && resMat1[2][2] == 117);

        assert(resMat2[0][0] ==  76 && resMat2[0][1] == 100);
        assert(resMat2[1][0] == 103 && resMat2[1][1] == 136);

        Vec<2, int> testVec;
        testVec[0] = 7;
        testVec[1] = 8;

        Vec<3, int> resVec = testMat1 * testVec;

        assert(resVec[0] == 23 && resVec[1] == 53 && resVec[2] == 83);
    }

    {
        int data[] = {
            1, 4, 7,
            2, 4, 8,
            3, 6, 9
        };
        Mat<3, 3, int> test1;
        test1.set(data);

        Mat<2, 2, int> test2 = minorMatrix(test1, 1, 1);
        assert(test2[0][0] == 1 && test2[0][1] == 3);
        assert(test2[1][0] == 7 && test2[1][1] == 9);

        assert(det(test1) == 12);
    }

    {
        double data[] = {
            2.,  7., 17.,
            3., 11., 19.,
            5., 13., 23.
        };
        Mat<3, 3, double> mat;
        mat.set(data);

        assert(det(mat) == -78);

        Mat<3, 3, double> inv = inverse(mat);

        Mat<3, 3, double> test1 = inv * mat;
        Mat<3, 3, double> test2 = mat * inv;

        double delta = 0.00000000001;
        for (int r=0; r<3; ++r) for (int c=0; c<3; ++c) {
            if (r == c) {
                assert(abs(test1[r][c] - 1) < delta);
                assert(abs(test2[r][c] - 1) < delta);
            }
            else {
                assert(abs(test1[r][c]) < delta);
                assert(abs(test2[r][c]) < delta);
            }
        }
    }

    cout << "All systems nominal" << endl;

    return 0;
}
