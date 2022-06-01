#include <cmath>
#include "metric_calculator.h"

MetricCalculator::MetricCalculator(size_t n, size_t dim, 
                                   const std::vector<Matrix>& points, 
                                   const std::vector<std::vector<int>>& clustering):
                                   n_(n), dim_(dim), c_(clustering.size()), 
                                   points_(points), clustering_(clustering),
                                   centers_(c_, Matrix(dim, 1)),
                                   globalCenter_(dim, 1) {
    for (int i = 0; i < c_; i++) {
        centers_[i] *= 0;
        for (int v : clustering_[i]) {
            centers_[i] += points_[v];
        }
        centers_[i] /= (double)clustering_[i].size();
    }
    globalCenter_ *= 0;
    for (int i = 0; i < n_; i++) {
        globalCenter_ += points_[i];
    }
    globalCenter_ /= (double)n_;
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
    double allSquared = 0;
    for (const Matrix& point : points_) {
        Matrix diff = point - globalCenter_;
        allSquared += diff.squaredNorm();
    }
    double clusterSquared = 0;
    for (int i = 0; i < c_; i++) {
        for (int v : clustering_[i]) {
            Matrix diff = centers_[i] - points_[v];
            clusterSquared += diff.squaredNorm();
        }
    }
    return (allSquared - clusterSquared) / allSquared;
}
