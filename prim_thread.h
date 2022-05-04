#pragma once

#include <vector>
#include <utility>
#include "point.h"
#include "graph.h"

class MSTCalculator {
public:
    MSTCalculator(const std::vector<Point>& points);

    Graph Calculate(int threadNum);

private:
    void UpdateClosestRange(size_t first, size_t second, size_t p);

    void SelectClosestRange(size_t first, size_t second, size_t& result);

    size_t n;
    const std::vector<Point>& points;
    std::vector<std::pair<double, size_t>> closest;
    std::vector<bool> marked;
};
