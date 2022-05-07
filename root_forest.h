#pragma once

#include <set>
#include <set>
#include "graph.h"
#include "Eigen/Dense"

using Matrix = Eigen::MatrixXf;

class RootForest {
public:

    //Should always be constructed from a set of points and a forest graph
    RootForest(const Graph& other);

    int GetBiggestCluster(int minimalSize) const;

    bool SeparateByTreshold(int clusterRoot, float treshold);

    bool SeparateByRatio(int clusterRoot, float ratio);

    bool SeparateByVolume(int clusterRoot);

    float GetClusterVolume(int clusterRoot) const;

    std::vector<int> GetClustering() const;

    const std::vector<Matrix>& Points() const;

private:

    void UpdateDP(int clusterRoot);

    void InitialDfs(int v, int prev, std::vector<bool> &used);

    float CalculateVolume(const Matrix& transposedSum, const Matrix& plainSum, int size) const;
    
    size_t verts_;

    std::set<std::pair<float, int>> clustersQueue_;

    std::vector<int> parent_;
    std::vector<std::vector<int>> graph_;

    std::vector<Matrix> points_;
    std::vector<Matrix> sumDP_;
    std::vector<Matrix> transposeDP_;
    std::vector<int> sizeDP_;
};
