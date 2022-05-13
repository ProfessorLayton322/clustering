#include "gath_geva_calculator.h"

GathGevaCalculator::GathGevaCalculator(size_t size, size_t clusters, size_t dim, 
                                       const std::vector<Matrix>& points,
                                       const std::vector<std::vector<int>>& clustering):
                                       n_(size), c_(clusters), U_(clusters, size), points_(size, dim) {
    for (int i = 0; i < c_; i++) {
        for (int j = 0; j < n_; j++) {
            U_(i, j) = 0;
        }
        for (int v : clustering[i]) {
            U_(i, v) = 1;
        }
    }
    for (int i = 0; i < n_; i++) {
        for (int j = 0; j < dim; j++) {
            points_(i, j) = points[i](j, 0);
        }
    }
}

GathGevaCalculator::GathGevaCalculator(size_t size, size_t clusters, size_t dim,
                                       const RootForest& forest):
                                       n_(size), c_(clusters), U_(clusters, size), points_(size, dim) {
    auto& points = forest.Points();
    auto clustering = forest.GetClustering();
    for (int i = 0; i < c_; i++) {
        for (int j = 0; j < n_; j++) {
            U_(i, j) = 0;
        }
        for (int v : clustering[i]) {
            U_(i, v) = 1;
        }
    }
    for (int i = 0; i < n_; i++) {
        for (int j = 0; j < dim; j++) {
            points_(i, j) = points[i](j, 0);
        }
    }
}

size_t GathGevaCalculator::Size() const {
    return n_;
}

size_t GathGevaCalculator::Clusters() const {
    return c_;
}

const Matrix& GathGevaCalculator::U() const {
    return U_;
}

const Matrix& GathGevaCalculator::Points() const {
    return points_;
}

void GathGevaCalculator::Iterate(float exponent) {
    /*
    auto weightedPartition = U_.array().pow(exponent);
    auto centers = weightedPartition * points_;
    for (int i = 0; i < c_; i++) {
        float coef = weightedPartition.row(i).sum();
        if (std::abs(coef) <= 1e-4) {
            continue;
        }

    }
    */
    //TODO
}
