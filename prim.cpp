#include <algorithm>
#include <float.h>
#include <utility>
#include "prim.h"

void UpdateClosest(size_t p, std::vector<std::pair<double, size_t>>& closest,
                   const std::vector<bool>& marked, const std::vector<Point>& points) {
    size_t n = closest.size();
    for (size_t i = 0; i < n; i++) {
        if (marked[i]) {
            continue;
        }
        closest[i] = std::min(closest[i], {DistanceSquared(points[p], points[i]), p});
    }
}

size_t SelectClosest(const std::vector<std::pair<double, size_t>>& closest,
                     const std::vector<bool>& marked, const std::vector<Point>& points) {
    size_t n = closest.size();
    std::pair<double, size_t> optimal = {DBL_MAX, n};
    for (size_t i = 0; i < n; i++) {
        if (marked[i]) {
            continue;
        }
        optimal = std::min(optimal, {closest[i].first, i});
    }
    return optimal.second;
}

Graph MinimalSpanningTree(const std::vector<Point>& points) {
    size_t n = points.size();
    std::vector<std::vector<size_t>> graph(n);
    std::vector<std::pair<double, size_t>> closest(n);
    std::vector<bool> marked(n);
    marked[0] = true;
    for (size_t i = 1; i < n; i++) {
        closest[i] = std::make_pair(DistanceSquared(points[0], points[i]), 0);
    }
    for (size_t iteration = 1; iteration < n; iteration++) {
        size_t u = SelectClosest(closest, marked, points);
        size_t v = closest[u].second;
        marked[u] = true;
        AddEdge(v, u, graph);
        UpdateClosest(u, closest, marked, points);
    }
    return graph;
}
