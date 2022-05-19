#pragma once

#include <vector>
#include <utility>

#include "matrix.h"

class MetricCalculator {
public:
    MetricCalculator(size_t n, size_t dim, const std::vector<Matrix>& points, const std::vector<std::vector<int>>& clustering);

    const std::vector<Matrix>& Points() const;

    const std::vector<std::vector<int>>& Clustering() const;

    size_t N() const;

    size_t Dim() const;

    size_t C() const;

    double RS() const;

private:

    size_t n_, dim_, c_;

    std::vector<Matrix> points_;
    std::vector<Matrix> centers_;

    std::vector<std::vector<int>> clustering_;
};
