#include <exception>
#include <utility>
#include <algorithm>
#include <math.h>
#include <cmath>
#include "gath_geva_calculator.h"

#include <iostream>
using std::cout;
using std::endl;

GathGevaCalculator::GathGevaCalculator(size_t size, size_t clusters, size_t dim, 
                                       const std::vector<Matrix>& points,
                                       const std::vector<std::vector<int>>& clustering):
                                       n_(size), c_(clusters), dim_(dim), U_(clusters, size), points_(size, dim) {
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
                                       n_(size), c_(clusters), dim_(dim_), U_(clusters, size), points_(size, dim) {
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

double GathGevaCalculator::Iterate(double exponent) {
    if (exponent <= 1.0f) {
        throw std::invalid_argument("Exponent should not be 1 or less");
    }
    Matrix weightedPartition = U_.array().pow(exponent);
    std::vector<double> weight(c_);
    std::vector<bool> empty(c_);
    for (int i = 0; i < c_; i++) {
        weight[i] = weightedPartition.row(i).sum();
        empty[i] = (std::abs(weight[i]) <= 1e-5);
    }

    Matrix centers = weightedPartition * points_;
    for (int i = 0; i < c_; i++) {
        if (empty[i]) {
            continue;
        } else {
            for (int j = 0; j < dim_; j++) {
                centers(i, j) /= weight[i];
            }
        }
    }
    std::vector<Matrix> covariance;
    covariance.reserve(c_);
    int tmp = 0;
    for (int i = 0; i < c_; i++) {
        covariance.emplace_back(dim_, dim_);
        covariance[i] *= 0.0f;
        if (empty[i]) {
            continue;
        }
        for (int j = 0; j < n_; j++) {
            Matrix diff = points_.row(j) - centers.row(i);
            covariance[i] += weightedPartition(i, j) * (diff.transpose() * diff);
        }
        cout << "Previous det is " << covariance[i].determinant() << endl;
        covariance[i] /= weight[i];
        double det = covariance[i].determinant();
        if (det < -1e-3) {
            throw std::invalid_argument("Matrix has a negative det");
        }
        if (std::abs(det) < 1e-3) {
            tmp++;
            cout << det << endl;
            /*
            cout << covariance[i] << endl << endl;
            throw std::invalid_argument("Matrix has zero det");
            */
        }
    }
    cout << tmp << endl;
    if (tmp) {
        throw std::invalid_argument("Matrix has zero det");
    }
    auto ClusterDistance = [&](int cluster, int point) -> double {
        if (empty[cluster]) {
            return 0;
        }
        double coef = std::sqrt(covariance[cluster].determinant()) / weight[cluster];
        Matrix diff = points_.row(point) - centers.row(cluster);
        Matrix product = diff * covariance[cluster].inverse() * diff.transpose();
        return coef * exp(product(0, 0) / 2.0f);
    };
    Matrix old(U_);
    double power = 1.0f / (exponent - 1.0f);
    for (int i = 0; i < n_; i++) {
        std::vector<double> temp(c_);
        double denominator = 0;
        for (int j = 0; j < c_; j++) {
            temp[j] = 1.0f / pow(ClusterDistance(j, i), power);
            denominator += temp[j];
        }
        for (int j = 0; j < c_; j++) {
            U_(j, i) = temp[j] / denominator;
        }
    }
    old -= U_;
    return old.norm();
}

int GathGevaCalculator::Recluster(double exponent, double tolerance, int maxIterations) {
    int iteration;
    for (iteration = 1; iteration <= maxIterations; iteration++) {
        if (Iterate(exponent) <= tolerance) {
            break;
        }
    }
    return iteration;
}

std::vector<std::vector<int>> GathGevaCalculator::GetClustering() const {
    std::vector<std::vector<int>> answer(c_);
    for (int i = 0; i < n_; i++) {
        auto best = std::make_pair(U_(0, i), 0);
        for (int j = 1; j < c_; j++) {
            best = std::max(best, std::make_pair(U_(j, i), j));
        }
        answer[best.second].push_back(i);
    }
    return answer;
}
