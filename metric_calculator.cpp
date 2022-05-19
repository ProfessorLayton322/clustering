#include <cmath>
#include "metric_calculator.h"

MetricCalculator::MetricCalculator(size_t n, size_t dim, 
                                   const std::vector<Matrix>& points, 
                                   const std::vector<std::vector<int>>& clustering):
                                   n_(n), dim_(dim), c_(clustering.size()), 
                                   points_(points), centers_(c_, Matrix(dim, 1)) {
    for (int i = 0; i < c_; i++) {
        centers_[i] *= 0;
        for (int v : clustering_[i]) {
            centers_[i] += points_[v];
        }
        centers_[i] /= 1.0f / (double)clustering_[i].size();
    }
}

const std::vector<Matrix>& MetricCalculator::Points() const {
    return points_;
}

const std::vector<std::vector<int>>& MetricCalculator::Clustering() const {
    return clustering_;
}

size_t MetricCalculator::N() const {
    return n_;
}

size_t MetricCalculator::Dim() const {
    return dim_;
}

size_t MetricCalculator::C() const {
    return c_;
}

double MetricCalculator::RS() const {
    double answer = 0;
    for (int i = 0; i < c_; i++) {
        for (int v : clustering_[i]) {
            Matrix diff = points_[v] - centers_[i];
            answer += diff.squaredNorm();
        }
    }
    answer /= (double)(n_ - c_);
    return std::sqrt(answer);
}
