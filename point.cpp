#include "point.h"
#include <cmath>
#include <cassert>

double Distance(const Point &a, const Point &b) {
    assert(a.size() == b.size());
    double squareDist = 0;
    for (size_t i = 0; i < a.size(); i++) {
        squareDist += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return (squareDist >= 0) ? std::sqrt(squareDist) : 0;
}
