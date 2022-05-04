#include <algorithm>
#include <utility>
#include "graph.h"

Graph::Graph(const std::vector<std::vector<size_t>>& graph): graph_(graph), verts_(graph.size()) {}

Graph::Graph(size_t n): graph_(n), verts_(n) {}

Graph::Graph(const Graph& other): graph_(other.graph_), verts_(other.verts_) {}

Graph::Graph(Graph&& other): graph_(std::move(other.graph_)) {}

float Graph::GetWeight(const std::vector<Point>& points) const {
    float answer = 0;
    for (size_t v = 0; v < verts_; v++) {
        for (size_t u : graph_[v]) {
            if (u < v) {
                continue;
            }
            answer += Distance(points[v], points[u]);
        }
    }
    return answer;
}

void Graph::AddEdge(size_t v, size_t u) {
    graph_[v].push_back(u);
    graph_[u].push_back(v);
}

std::vector<size_t>& Graph::operator[](size_t v) {
    return graph_[v];
}

const std::vector<size_t>& Graph::operator[](size_t v) const {
    return graph_[v];
}

bool Graph::Compare(Graph& other) {
    SortLists();
    other.SortLists();
    return graph_ == other.graph_;
}

void Graph::SortLists() {
    for (size_t v = 0; v < verts_; v++) {
        std::sort(graph_[v].begin(), graph_[v].end());
    }
}
