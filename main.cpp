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

#include "Eigen/Dense"
using Matrix = Eigen::MatrixXf;


int main(int argc, char* argv[]) {
    Matrix m(3, 3);
    m << 1, 2, 3,
         4, 5, 6,
         7, 8, 10;

    cout << m << endl;
    cout << m / 2.0f << endl;
    cout << m.determinant() << endl;
    /*
    freopen(argv[1], "r", stdin);
    
    size_t dim = atoi(argv[2]);

    size_t n;
    cin >> n;
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

    float totalWeight = graph.GetWeight();
    for (float alpha = 0.9f; alpha >= 0.1f; alpha -= 0.1f) {
        float lambda = alpha * (totalWeight / (float)(n - 1));
        graph.RemoveByTreshold(lambda);
        int clusters = n - graph.CountEdges();
        cout << clusters << " clusters for alpha = " << alpha << endl;
    }
    */

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
