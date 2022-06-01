#include <iostream>
#include <algorithm>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>

#include <prim_thread.h>

using std::cout;
using std::cin;
using std::endl;


int main(int argc, char* argv[]) {

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w+", stdout);
    int dim = atoi(argv[3]);
    int threads = atoi(argv[4]);

    std::vector<Point> points;
    Point p;
    p.reserve(dim);
    float f;
    while (std::cin >> f) {
        p.push_back(f);
        if (p.size() == dim) {
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
    cout << new_points.size() << " " << dim << endl;
    for (size_t i = 0; i < new_points.size(); i++) {
        for (float f : new_points[i]) {
            cout << f << " ";
        }
        cout << endl;
    }
    MSTCalculator mstCalculator(new_points);
    auto graph = mstCalculator.Calculate(threads).GetGraph();
    cout << graph.size() << endl;
    for (size_t i = 0; i < graph.size(); i++) {
        cout << graph[i].size() << endl;
        for (size_t u : graph[i]) {
            cout << u << " ";
        }
        cout << endl;
    }
    return 0;
}
