#include <vector>
#include "point.h"
#include "graph.h"

Graph MinimalSpanningTreeThreads(const std::vector<Point>& points,
                                                             size_t threadNum = 8);
