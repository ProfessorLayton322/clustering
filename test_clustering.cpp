#include <iostream>
#include <cassert>
#include <ctime>
#include <algorithm>
#include "prim.h"
#include "prim_thread.h"
#include "generate_tests.h"

using std::cout;
using std::endl;

int main() {
    const size_t n = 50000, dim = 30;
    auto points = GenerateTests(n, dim);
    time_t seconds;
    for (int threads = 8; threads > 0; threads--) {
        MSTCalculator mstCalculator(points);
        seconds = time(NULL);
        auto graph_threaded = mstCalculator.Calculate(threads);
        seconds = time(NULL) - seconds;
        printf("Took %ld seconds to build with %d threads\n", seconds, threads);
    }
    seconds = time(NULL);
    auto graph = MinimalSpanningTree(points);
    seconds = time(NULL) - seconds;
    printf("Took %ld seconds to build without threads\n", seconds);
    return 0;
}
