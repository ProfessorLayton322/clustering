#include "root_forest.h"

class GathGevaCalculator {
public:
    
    GathGevaCalculator(size_t size, size_t clusters, size_t dim, const RootForest& forest);

    GathGevaCalculator(size_t size, size_t clusters, size_t dim, const std::vector<Matrix>& points, const std::vector<std::vector<int>>& clustering);

    void Iterate(float exponent);

    size_t Size() const;

    size_t Clusters() const;

    const Matrix& U() const;

    const Matrix& Points() const;

    std::vector<int> GetClustering() const;
private:

    Matrix U_;
    Matrix points_;

    size_t n_;
    size_t c_;
};
