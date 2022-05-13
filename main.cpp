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

void PrintClusters(const std::vector<std::vector<int>>& clustering) {
    size_t cluster_num = clustering.size();
    cout << cluster_num << endl;
    for (int i = 0; i < cluster_num; i++) {
        cout << clustering[i].size() << endl;
    }
    for (int i = 0; i < cluster_num; i++) {
        for (int v : clustering[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {

    Matrix m(3, 3);
    m <<
    1, 2, 3,
    4, 5, 6,
    7, 8, 10;

    cout << m << endl << endl;
    cout << m.row(1) << endl << endl;
    
    cout << m.row(1).rows() << endl;
    
    Matrix res = m.array().pow(2.0f);
    cout << res << endl << endl;

    cout << m.inverse() << endl;
    return 0;

    freopen(argv[1], "r", stdin);
    float ratio = atof(argv[2]);

    auto graph = ReadGraph();
    
    RootForest forest(graph);

    int cutByTreshold = 0, cutByVolume = 0;

    while (forest.LargestCluster().first >= 3e5) {
        int toBeCut = forest.GetBiggestCluster(100000);
        if (toBeCut == -1) {
            break;
        }
        if (forest.SeparateByRatio(toBeCut, ratio, 10000)) {
            cutByTreshold++;
            continue;
        }
        if (!forest.SeparateByVolume(toBeCut, 10000)) {
            cout << "Cant split by volume" << endl;
            return 1;
        }
        cutByVolume++;
    }

    cout << cutByTreshold << " " << cutByVolume << endl;

    auto result = forest.GetClustering();
    PrintClusters(result);

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
    cout << new_points.size() << " " << 16 << endl;
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
