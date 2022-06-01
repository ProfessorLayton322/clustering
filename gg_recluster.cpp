#include <iostream>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <string>

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
    //Usage: ./gg_recluster points clustering dim

    freopen(argv[1], "r", stdin);
    size_t dim = atoi(argv[3]);

    int n;
    cin >> n;

    std::vector<Matrix> points;
    for (int i = 0; i < n; i++) {
        points.emplace_back(dim, 1);
        for (int j = 0; j < dim; j++) {
            cin >> points[i](j, 0);
        }
    }

    freopen(argv[2], "r", stdin);
    int c;
    cin >> c;

    cout << n << " " << c << " " << dim << endl << endl;
    
    std::vector<std::vector<int> > clustering(c);
    for (int i = 0; i < c; i++) {
        int k;
        cin >> k;
        clustering[i].resize(k);
    }
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < clustering[i].size(); j++) {
            cin >> clustering[i][j];
        }
    }
    double exponent = 2;
    GathGevaCalculator gg(n, c, dim, points, clustering);

    cout << gg.Recluster(2.0f, 0.0001, 1) << " iterations" << endl;
    auto final_clustering = gg.GetClustering();
    PrintClusters(final_clustering);
    return 0;
}
