#pragma once

#include "point.h"
#include "Eigen/Dense"

using Matrix = Eigen::MatrixXf;

Matrix GetClusterCenter(const std::vector<Matrix>& points, const std::vector<int>& cluserINdexes);

Matrix PointToMatrix(const Point& point);
