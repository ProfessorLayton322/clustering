#include <vector>
#include <iostream>

typedef std::vector<float> Point;

float DistanceSquared(const Point& a, const Point& b);

std::ostream& operator << (std::ostream& out, const Point& point);

std::ostream& operator << (std::ostream& out, const std::vector<Point>& points);
