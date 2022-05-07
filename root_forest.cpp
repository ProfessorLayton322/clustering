#include <deque>
#include <cmath>
#include <stdexcept>
#include <utility>
#include "root_forest.h"

RootForest::RootForest(const Graph& other): verts_(other.Points().size()), parent_(verts_, -1), graph_(other.GetGraph()), points_(verts_), sumDP_(verts_), transposeDP_(verts_) {
    size_t dim = other.Points()[0].size();
    for (int i = 0; i < verts_; i++) {
        points_[i] = Matrix(dim, 1);
        for (int j = 0; j < dim; j++) {
            points_[i](j, 0) = other.Points()[i][j];
        }
    }
    std::vector<bool> used(verts_, false);
    for (int i = 0; i < verts_; i++) {
        if (used[i]) {
            continue;
        }
        InitialDfs(i, -1, used);
        UpdateDP(i);
        float volume = GetClusterVolume(i);
        clustersQueue_.insert({volume, i});
    }
}

void RootForest::InitialDfs(int v, int prev, std::vector<bool> &used) {
    used[v] = true;
    parent_[v] = prev;
    for (int i = 0; i < graph_[v].size(); i++) {
        int u = graph_[v][i];
        if (u == prev) {
            std::swap(graph_[v][i], graph_[v].back());
            graph_[v].pop_back();
            i--;
            continue;
        }
        InitialDfs(u, v, used);
    }
}

void RootForest::UpdateDP(int clusterRoot) {
    auto& vec = points_[clusterRoot];
    sumDP_[clusterRoot] = vec;
    transposeDP_[clusterRoot] = vec * vec.transpose();
    sizeDP_[clusterRoot] = 1;
    for (int u : graph_[clusterRoot]) {
        UpdateDP(u);
        sumDP_[clusterRoot] += sumDP_[u];
        transposeDP_[clusterRoot] += transposeDP_[u];
        sizeDP_[clusterRoot] += sizeDP_[u];
    }
}

float RootForest::CalculateVolume(const Matrix& transposedSum, const Matrix& plainSum, int size) const {
    float invSize = 1.0f / (float)size;
    auto clusterCenter = plainSum * invSize;
    auto covarianceMatrix = transposedSum * invSize - clusterCenter * clusterCenter.transpose();
    float det = covarianceMatrix.determinant();
    return (det >= 0.f)? std::sqrt(det) : 0.0f;
}

float RootForest::GetClusterVolume(int clusterRoot) const {
    if (parent_[clusterRoot] != -1) {
        throw std::invalid_argument("Given vertex is not a root of a cluster");
    }
    return CalculateVolume(transposeDP_[clusterRoot], sumDP_[clusterRoot], sizeDP_[clusterRoot]);
}

const std::vector<Matrix>& RootForest::Points() const {
    return points_;
}
