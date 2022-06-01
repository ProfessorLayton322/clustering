#include <iostream>
#include <cstdlib>
#include <string>

#include <root_forest.h>

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
    //Usage ./cut_by_ratio input.txt output.txt max_clusters ratio
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
    int maxClusters = atoi(argv[3]);
    double ratio = atof(argv[4]);

    auto graph = ReadGraph();

    const std::vector<Point>& points = graph.Points();
    size_t n = points.size();
    size_t dim = points[0].size();
    
    RootForest forest(graph);

    for (int i = 2; i <= maxClusters; i++) {
        int toBeCut = forest.GetBiggestCluster(1000);
        if (toBeCut == -1) {
            break;
        }
        if (forest.SeparateByRatio(toBeCut, ratio, 100)) {
            continue;
        }
        if (!forest.SeparateByVolume(toBeCut, 100)) {
            cout << "Cant split" << endl;
            return 1;
        }
    }

    auto result = forest.GetClustering();
    PrintClusters(result);
}
