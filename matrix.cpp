#include "matrix.h"

Matrix GetClusterCenter(const std::vector<Matrix>& points, const std::vector<int>& clusterIndexes) {
    size_t n = clusterIndexes.size();
    Matrix center = points[clusterIndexes[0]];
    for (int i = 1; i < n; i++) {
        center += points[clusterIndexes[i]];
    }
    float inv_size = 1.0f / (float)n;
    center *= inv_size;
    return center;
}

Matrix PointToMatrix(const Point& point) {
    size_t dim = point.size();
    Matrix answer(dim, 1);
    for (int i = 0; i < dim; i++) {
        answer(i, 0) = point[i];
    }
    return answer;
}
