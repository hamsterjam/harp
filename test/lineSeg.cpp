#include <iostream>
#include <cassert>
#include <cmath>

#include <harpMath.h>

using namespace std;

int main(int argc, char** argv) {
    Vec<2, double> p0, p1, q0, q1;
    p0[0] = 0;      p0[1] = 0;
    p1[0] = 1;      p1[1] = 1;
    q0[0] = 1;      q0[1] = 0;
    q1[0] = 0;      q1[1] = 1;

    double s = 1;
    double t = 1;

    assert(lineSegmentsIntersect(p0, p1, q0, q1, s, t));
    assert(s == 0.5);
    assert(t == 0.5);

    s = 1;

    assert(lineSegmentsIntersect(p0, p1, q0, q1, s));
    assert(s == 0.5);

    assert(lineSegmentsIntersect(p0, p1, q0, q1));

    cout << "All systems nominal!" << endl;
    return 0;
}
