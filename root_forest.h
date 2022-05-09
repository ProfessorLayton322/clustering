#pragma once

#include <set>
#include <algorithm>
#include <set>
#include "graph.h"
#include "Eigen/Dense"

using Matrix = Eigen::MatrixXf;

class RootForest {
public:

    //Should always be constructed from a set of points and a forest graph
    RootForest(const Graph& other);

    //Returns cluster with the biggest fuzzy volume, -1 if all clusters are less than minimalSize
    int GetBiggestCluster(int minimalSize) const;

    //Cut the cluster by Criterion 1 with given treshold
    bool SeparateByTreshold(int clusterRoot, float treshold);

    //Cut the cluster by Criterion 2 with given raio
    bool SeparateByRatio(int clusterRoot, float ratio);

    //Cut the cluster by criterion  3 (minimizing total fuzzy volume)
    bool SeparateByVolume(int clusterRoot);

    //Return clusters fuzzy volume
    float GetClusterVolume(int clusterRoot) const;

    //Returns the final partition into clusters
    std::vector<std::vector<int>> GetClustering() const;

    //Read only points
    const std::vector<Matrix>& Points() const;

    //Cut out a subtree
    void SeparateSubtree(int oldRoot, int newRoot);

private:

    void UpdateDP(int clusterRoot);

    void InitialDfs(int v, int prev, std::vector<bool> &used);

    template<typename Callback>
    void Dfs(int v, const Callback& edgeCallback) const;

    float CalculateVolume(const Matrix& transposedSum, const Matrix& plainSum, int size) const;

    void FetchCluster(int clusterRoot);
    
    size_t verts_;

    std::set<std::pair<float, int>> clustersQueue_;

    std::vector<int> parent_;
    std::vector<std::vector<int>> graph_;

    std::vector<Matrix> points_;
    std::vector<Matrix> sumDP_;
    std::vector<Matrix> transposeDP_;
    std::vector<int> sizeDP_;
};
