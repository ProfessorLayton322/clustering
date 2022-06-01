#include <generate_tests.h>

int main(int argc, char* argv[]) {
    size_t n = atoi(argv[1]);
    size_t dim = atoi(argv[2]);
    int threads = atoi(argv[3]);
    MeasureTime(n, dim, threads);
    return 0;
}
