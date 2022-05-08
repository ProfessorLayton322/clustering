#include <iostream>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>

#include "prim_thread.h"
#include "generate_tests.h"
#include "prim_naive.h"
#include "root_forest.h"

/*
void MeasureTime(size_t n, size_t dim, int threads) {
    auto points = GenerateTests(n, dim);
    MSTCalculator mstCalculator(points);
    time_t seconds = time(NULL);
    auto graph_threaded = mstCalculator.Calculate(threads);
    seconds = time(NULL) - seconds;
    printf("Takes %ld seconds for %d nodes %d dim and %d threads\n", seconds, n, dim, threads);
}
*/

using std::cout;
using std::cin;
using std::endl;


int main(int argc, char* argv[]) {
    freopen(argv[1], "r", stdin);
    
    size_t n, dim;
    cin >> n >> dim;
    std::vector<Point> points(n, Point(dim));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < dim; j++) {
            cin >> points[i][j];
        }
    }

    Graph graph(points);

    for (size_t i = 0; i < n; i++) {
        size_t k;
        cin >> k;
        for (size_t j = 0; j < k; j++) {
            size_t u;
            cin >> u;
            if (i > u) {
                graph.AddEdge(i, u);
            }
        }
    }

    cout << graph.Points() << endl << endl;

    RootForest forest(graph);

    int root = forest.GetBiggestCluster(0);
    cout << root << endl;
    cout << forest.GetClusterVolume(root) << endl;

    /*
    freopen(argv[1], "r", stdin);
    std::vector<Point> points;
    Point p;
    p.reserve(16);
    float f;
    while (std::cin >> f) {
        p.push_back(f);
        if (p.size() == 16) {
            points.push_back(p);
            p.clear();
        }
    }
    std::sort(points.begin(), points.end());
    std::vector<Point> new_points;
    const float eps = 1e-8;
    for (size_t i = 0; i < points.size(); i++) {
        bool flag = true;
        for (size_t j = 1; j <= i && j < 6; j++) {
            if (DistanceSquared(points[i - j], points[i]) <= eps) {
                flag = false;
                break;
            }
        }
        if (flag) {
            new_points.push_back(points[i]);
        }
    }
    cout << new_points.size() << endl;
    for (size_t i = 0; i < new_points.size(); i++) {
        for (float f : new_points[i]) {
            cout << f << " ";
        }
        cout << endl;
    }
    MSTCalculator mstCalculator(new_points);
    auto graph = mstCalculator.Calculate(40);
    for (size_t i = 0; i < graph.size(); i++) {
        cout << graph[i].size() << endl;
        for (size_t u : graph[i]) {
            cout << u << " ";
        }
        cout << endl;
    }
    */
    return 0;
}
