#include "prim_naive.h"
#include <algorithm>
#include <cmath>
#include <utility>

struct Edge {
    float weight;
    size_t from;
    size_t to;

    Edge(float weight, size_t from, size_t to): weight(weight), from(from), to(to) {}
};

struct EdgeCmp {
    bool operator()(const Edge& e1, const Edge& e2) {
        return e1.weight < e2.weight;
    }
};

float PrimNaive(const std::vector<Point>& points) {
    size_t n = points.size();
    size_t dim = points[0].size();
    std::vector<Edge> edges;
    edges.reserve(n * n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = i + 1; j < n; j++) {
            float distance = std::sqrt(DistanceSquared(points[i], points[j]));
            edges.emplace_back(distance, i, j);
        }
    }
    std::sort(edges.begin(), edges.end(), EdgeCmp());

    std::vector<size_t> sign(n);
    for (size_t i = 0; i < n; i++) {
        sign[i] = i;
    }
    float answer = 0;
    //The most naive O(n^2) algorithm
    for (const Edge& e : edges) {
        if (sign[e.from] == sign[e.to]) {
            continue;
        }
        answer += e.weight;
        for (size_t i = 0; i < n; i++) {
            if (sign[i] == sign[e.to]) {
                sign[i] = sign[e.from];
            }
        }
    }
    return answer;
}
