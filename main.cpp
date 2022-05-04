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

void MeasureTime(size_t n, size_t dim, int threads) {
    auto points = GenerateTests(n, dim);
    MSTCalculator mstCalculator(points);
    time_t seconds = time(NULL);
    auto graph_threaded = mstCalculator.Calculate(threads);
    seconds = time(NULL) - seconds;
    printf("Takes %ld seconds for %d nodes %d dim and %d threads\n", seconds, n, dim, threads);
}

bool dfs(int v, int prev, const Graph& g, std::vector<bool>& used) {
    used[v] = true;
    for (size_t u : g[v]) {
        if (u == prev) {
            continue;
        }
        if (used[u]) {
            return false;
        }
        if (!dfs(u, v, g, used)) {
            return false;
        }
    }
    return true;
}

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {

    const size_t n = atoi(argv[1]);
    const size_t dim = atoi(argv[2]);
    const int threads = atoi(argv[3]);

    time_t start = time(NULL);
    const time_t secondsInDay = 24 * 60 * 60;

    while (time(NULL) <= start + secondsInDay) {
        auto points = GenerateTests(n, dim);
        MSTCalculator mstCalculator(points);
        auto graphThreaded = mstCalculator.Calculate(threads);
        std::vector<bool> used(n);

        if (!dfs(0, -1, graphThreaded, used)) {
            cout << "Graph has a loop" << endl;
            cout << points << endl;
            return 0;
        }

        for (size_t i = 0; i < n; i++) {
            if (!used[i]) {
                cout << "Graph not connected" << endl;
                cout << points << endl;
                return 0;
            }
        }

        auto realGraph = PrimNaive(points);
        if (realGraph.Compare(graphThreaded)) {
            continue;
        }

        float totalWeight = graphThreaded.GetWeight(points);
        float realWeight = realGraph.GetWeight(points);
        if (std::abs(totalWeight - realWeight) > 1e-2) {
            cout << "Weights do not match" << endl;
            cout << totalWeight << endl;
            cout << realWeight << endl;
            cout << points << endl;
            return 0;
        }
    }
    /*
    const size_t n = atoi(argv[1]);
    size_t dim = atoi(argv[2]);
    int threads = atoi(argv[3]);
    MeasureTime(n, dim, threads);
    */

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
