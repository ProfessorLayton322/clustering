#include <random>
#include "point.h"

std::vector<Point> GenerateTests(size_t n, size_t dim) {
    std::vector<Point> points(n, Point(dim));
    double lower_bound = -500, upper_bound = 500;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < dim; j++) {
            points[i][j] = unif(re);
        }
    }
    return points;
}
