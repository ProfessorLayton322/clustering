#include <iostream>
#include <algorithm>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>

#include <prim_thread.h>
#include <root_forest.h>
#include <gath_geva_calculator.h>

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
    //beware of the pipeline
    //Usage: ./pipeline input output dim threads clusters
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w+", stdout);
    int dim = atoi(argv[3]);
    int threads = atoi(argv[4]);
    int maxClusters = atoi(argv[5]);

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
    auto graph = mstCalculator.Calculate(threads);

    RootForest forest(graph);
    const int minimalClusterSize = dim + 10;

    for (int i = 2; i <= maxClusters; i++) {
        int toBeCut = forest.GetBiggestCluster(minimalClusterSize * 2);
        if (toBeCut == -1) {
            break;
        }
        if (!forest.SeparateByVolume(toBeCut, minimalClusterSize)) {
            cout << "Cant split by volume" << endl;
            return 1;
        }
    }

    auto clustering = forest.GetClustering();

    double exponent = 2;
    GathGevaCalculator gg(new_points.size(), clustering.size(), dim, forest.Points(), clustering);

    gg.Recluster(exponent, 0.0001);

    auto finalClustering = gg.GetClustering();
    PrintClusters(finalClustering);

    return 0;
}
