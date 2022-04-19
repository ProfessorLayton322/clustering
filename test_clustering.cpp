#include <iostream>
#include <time.h>
#include <algorithm>
#include "prim.h"
#include "prim_thread.h"
#include "generate_tests.h"

using std::cout;
using std::endl;

int main() {
    const size_t n = 10000, dim = 30;
    auto points = GenerateTests(n, dim);
    clock_t time;
    time = clock();
    auto graph_threaded = MinimalSpanningTreeThreads(points);
    time = clock() - time;
    printf("Took %f seconds to build with 8 threads\n", (float)time / CLOCKS_PER_SEC);
    /*
    time = clock();
    auto graph = MinimalSpanningTree(points);
    time = clock() - time;
    printf("Took %f seconds to build with 1 thread\n", (float)time / CLOCKS_PER_SEC);
    assert(graph == graph_threaded);
    */
    return 0;
}
