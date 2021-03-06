#pragma once

#include "point.h"
#include <Dense>

using Matrix = Eigen::MatrixXd;

Matrix GetClusterCenter(const std::vector<Matrix>& points, const std::vector<int>& cluserIndexes);

Matrix PointToMatrix(const Point& point);
