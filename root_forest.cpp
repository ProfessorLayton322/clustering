#include <deque>
#include <float.h>
#include <cmath>
#include <stdexcept>
#include <utility>
#include "root_forest.h"

float VolumeNaive(const std::vector<Matrix>& points, const std::vector<int>& clusterIndexes) {
    if (clusterIndexes.empty()) {
        return 0;
    }
    size_t n = clusterIndexes.size();
    Matrix center = points[clusterIndexes[0]];
    for (int i = 1; i < n; i++) {
        center += points[clusterIndexes[i]];
    }
    float inv_size = 1.0f / (float)n;
    center *= inv_size;
    Matrix diff = points[clusterIndexes[0]] - center;
    Matrix covariance = diff * diff.transpose();
    for (int i = 1; i < n; i++) {
        diff = points[clusterIndexes[i]] - center;
        covariance += diff * diff.transpose();
    }
    covariance *= inv_size;
    float det = covariance.determinant();
    return (det >= 0)? std::sqrt(det) : 0;
}

RootForest::RootForest(const Graph& other): verts_(other.Points().size()), parent_(verts_, -1), graph_(other.GetGraph()), points_(verts_), sumDP_(verts_), transposeDP_(verts_), sizeDP_(verts_) {
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
        FetchCluster(i);
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

int RootForest::GetBiggestCluster(int minimalSize) const {
    for (auto it = clustersQueue_.rbegin(); it != clustersQueue_.rend(); ++it) {
        int cluster = it->second;
        if (sizeDP_[cluster] >= minimalSize) {
            return cluster;
        }
    }
    return -1;
}

void RootForest::FetchCluster(int clusterRoot) {
    UpdateDP(clusterRoot);
    float volume = GetClusterVolume(clusterRoot);
    clustersQueue_.insert({volume, clusterRoot});
}

void RootForest::SeparateSubtree(int oldRoot, int newRoot) {
    for (auto it = clustersQueue_.begin(); it != clustersQueue_.end(); ++it) {
        if (it->second == oldRoot) {
            clustersQueue_.erase(it);
            break;
        }
    }
    int v = parent_[newRoot];
    for (int i = 0; i < graph_[v].size(); i++) {
        if (graph_[v][i] == newRoot) {
            std::swap(graph_[v][i], graph_[v].back());
            graph_[v].pop_back();
        }
    }
    parent_[newRoot] = -1;
    FetchCluster(oldRoot);
    FetchCluster(newRoot);
}

template<typename Callback>
void RootForest::Dfs(int v, const Callback& edgeCallback) const {
    edgeCallback(v);
    for (int u : graph_[v]) {
        Dfs(u, edgeCallback);
    }
}

float Distance(const Matrix& A, const Matrix& B) {
    auto diff = A - B;
    return diff.norm();
}

bool RootForest::SeparateByTreshold(int clusterRoot, float treshold, int minimalSize) {
    int optimalCut = -1;
    float maxWeight = 0;
    auto callback = [&optimalCut, &maxWeight, clusterRoot, minimalSize, treshold, this](int v) {
        int prev = parent_[v];
        if (prev == -1) {
            return;
        }
        if (sizeDP_[v] < minimalSize || sizeDP_[clusterRoot] - sizeDP_[v] < minimalSize) {
            return;
        }
        float distance = Distance(points_[v], points_[prev]);
        if (distance >= treshold && distance > maxWeight) {
            optimalCut = v;
            maxWeight = distance;
        }
    };
    Dfs(clusterRoot, callback);
    if (optimalCut == -1) {
        return false;
    }
    SeparateSubtree(clusterRoot, optimalCut);
    return true;
}

bool RootForest::SeparateByRatio(int clusterRoot, float ratio, int minimalSize) {
    int optimalCut = -1;
    float maxWeight = 0;
    auto callback = [&optimalCut, &maxWeight, clusterRoot, minimalSize, ratio, this](int v) {
        int prev = parent_[v];
        if (prev == -1) {
            return;
        }
        if (sizeDP_[v] < minimalSize || sizeDP_[clusterRoot] - sizeDP_[v] < minimalSize) {
            return;
        }
        float distance = Distance(points_[v], points_[prev]);
        float neighbourSum = 0;
        int neighbourNumber = 0;
        if (parent_[prev] != -1) {
            neighbourSum += Distance(points_[prev], points_[parent_[prev]]);
            neighbourNumber++;
        }
        for (int u : graph_[prev]) {
            neighbourSum += Distance(points_[prev], points_[u]);
            neighbourNumber++;
        }
        for (int u : graph_[v]) {
            neighbourSum += Distance(points_[v], points_[u]);
            neighbourNumber++;
        }
        float avgNeighbour = neighbourSum / (float)neighbourNumber * ratio;
        if (distance >= avgNeighbour && distance > maxWeight) {
            optimalCut = v;
            maxWeight = distance;
        }
    };
    Dfs(clusterRoot, callback);
    if (optimalCut == -1) {
        return false;
    }
    SeparateSubtree(clusterRoot, optimalCut);
    return true;
}

#include <iostream>    
using std::cout;
using std::endl;

bool RootForest::SeparateByVolume(int clusterRoot, int minimalSize) {    
//    cout << sizeDP_[clusterRoot] << endl;
    int optimalCut = -1;
    float optimalVolume = FLT_MAX;
    auto callback = [&optimalCut, &optimalVolume, clusterRoot, minimalSize, this](int v) {
        if (v == clusterRoot) {
            return;
        }
        if (sizeDP_[v] < minimalSize || sizeDP_[clusterRoot] - sizeDP_[v] < minimalSize) {
            return;
        }
        auto transposedSum = transposeDP_[clusterRoot] - transposeDP_[v];
        auto plainSum = sumDP_[clusterRoot] - sumDP_[v];
        int size = sizeDP_[clusterRoot] - sizeDP_[v];
        float firstVolume = CalculateVolume(transposedSum, plainSum, size);
        float secondVolume = CalculateVolume(transposeDP_[v], sumDP_[v], sizeDP_[v]);
        if (firstVolume + secondVolume <= optimalVolume) {
            optimalCut = v;
            optimalVolume = firstVolume + secondVolume;
        }
    };
    Dfs(clusterRoot, callback);
    if (optimalCut == -1) {
        return false;
    }
    SeparateSubtree(clusterRoot, optimalCut);
    return true;
}

std::vector<std::vector<int>> RootForest::GetClustering() const {
    std::vector<std::vector<int>> answer;
    auto callback = [&answer](int v) {
        answer.back().push_back(v);
    };
    for (int v = 0; v < verts_; v++) {
        if (parent_[v] == -1) {
            answer.push_back(std::vector<int>());
            Dfs(v, callback);
        }
    };
    return answer;
}

bool RootForest::CheckVolumeValidity() const {
    auto clustering = GetClustering();
    for (auto& cluster : clustering) {
        float correctVolume = VolumeNaive(points_, cluster);
        int root = -1;
        for (int v : cluster) {
            if (parent_[v] == -1) {
                root = v;
                break;
            }
        }
        float assumedVolume = GetClusterVolume(root);
        if (std::abs(assumedVolume - correctVolume) > 1e-2) {
            cout << "Something is wrong" << endl;
            cout << correctVolume << endl << assumedVolume << endl;
            return false;
        }
    }
    return true;
}

std::pair<int, int> RootForest::LargestCluster() const {
    std::pair<int, int> answer = {0, -1};
    for (auto it = clustersQueue_.begin(); it != clustersQueue_.end(); ++it) {
        int root = it->second;
        answer = std::max(answer, {sizeDP_[root], root});
    }
    return answer;
}
