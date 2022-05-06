#include <algorithm>
#include <utility>
#include "graph.h"

Graph::Graph(const std::vector<Point>& points): points_(points), verts_(points.size()), graph_(points.size()) {}

Graph::Graph(std::vector<Point>&& points): points_(std::move(points)), verts_(points.size()), graph_(points.size()) {}

Graph::Graph(const Graph& other): graph_(other.graph_), verts_(other.verts_), points_(other.points_)  {}

Graph::Graph(Graph&& other): graph_(std::move(other.graph_)), verts_(other.verts_), points_(std::move(other.points_)) {}

float Graph::GetWeight() const {
    float answer = 0;
    for (size_t v = 0; v < verts_; v++) {
        for (size_t u : graph_[v]) {
            if (u < v) {
                continue;
            }
            answer += Distance(points_[v], points_[u]);
        }
    }
    return answer;
}

void Graph::AddEdge(size_t v, size_t u) {
    graph_[v].push_back(u);
    graph_[u].push_back(v);
}

void Graph::RemoveEdge(size_t v, size_t u) {
    for (size_t i = 0; i < graph_[v].size(); i++) {
        if (graph_[v][i] == u) {
            std::swap(graph_[v][i], graph_[v].back());
            graph_[v].pop_back();
            break;
        }
    }
    for (size_t i = 0; i < graph_[u].size(); i++) {
        if (graph_[u][i] == v) {
            std::swap(graph_[u][i], graph_[u].back());
            graph_[u].pop_back();
            break;
        }
    }
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

int Graph::CountEdges() const {
    int answer = 0;
    for (size_t v = 0; v < verts_; v++) {
        answer += graph_[v].size();
    }
    return answer / 2;
}

void Graph::RemoveByTreshold(float treshold) {
    std::vector<std::vector<size_t> > newGraph(verts_);
    for (size_t v = 0; v < verts_; v++) {
        for (size_t u : graph_[v]) {
            if (u > v) {
                continue;
            }
            float distance = Distance(points_[v], points_[u]);
            if (distance <= treshold) {
                newGraph[v].push_back(u);
                newGraph[u].push_back(v);
            }
        }
    }
    graph_ = std::move(newGraph);
}

const std::vector<Point>& Graph::Points() const {
    return points_;
}
