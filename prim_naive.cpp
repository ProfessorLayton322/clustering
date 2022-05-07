#include "prim_naive.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>

struct Edge {
    float weight;
    int from;
    int to;

    Edge(float weight, int from, int to) : weight(weight), from(from), to(to) {}
};

struct EdgeCmp {
    bool operator()(const Edge& e1, const Edge& e2) { return e1.weight < e2.weight; }
};

Graph PrimNaive(const std::vector<Point>& points) {
    size_t n = points.size();
    size_t dim = points[0].size();
    std::vector<Edge> edges;
    edges.reserve(n * n);

    Graph graph(points);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            float distance = std::sqrt(DistanceSquared(points[i], points[j]));
            edges.emplace_back(distance, i, j);
        }
    }
    std::sort(edges.begin(), edges.end(), EdgeCmp());

    std::vector<int> sign(n);
    for (int i = 0; i < n; i++) {
        sign[i] = i;
    }
    // The most naive O(n^2) algorithm
    for (const Edge& e : edges) {
        if (sign[e.from] == sign[e.to]) {
            continue;
        }
        graph.AddEdge(e.from, e.to);
        int eliminate = sign[e.to];
        for (int i = 0; i < n; i++) {
            if (sign[i] == eliminate) {
                sign[i] = sign[e.from];
            }
        }
    }
    return graph;
}
