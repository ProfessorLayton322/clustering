#include <iostream>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include "prim.h"
#include "prim_thread.h"
#include "generate_tests.h"

using std::cout;
using std::endl;

void  MeasureTime(size_t n, size_t dim, int threads) {
    auto points = GenerateTests(n, dim);
    MSTCalculator mstCalculator(points);
    time_t seconds = time(NULL);
    auto graph_threaded = mstCalculator.Calculate(threads);
    seconds = time(NULL) - seconds;
    printf("Takes %ld seconds for %d nodes %d dim and %d threads\n", seconds, n, dim, threads);
}

int main(int argc, char* argv[]) {
    const size_t n = atoi(argv[1]);
    size_t dim = atoi(argv[2]);
    int threads = atoi(argv[3]);
    MeasureTime(n, dim, threads); 
    return 0;
}
