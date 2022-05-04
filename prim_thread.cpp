#include <algorithm>
#include <atomic>
#include <float.h>
#include <utility>
#include <thread>
#include <functional>
#include "prim_thread.h"

MSTCalculator::MSTCalculator(const std::vector<Point>& points)
    : n(points.size()), points(points), closest(n), marked(n) {
    marked[0] = true;
    for (size_t i = 1; i < n; i++) {
        closest[i] = std::make_pair(DistanceSquared(points[0], points[i]), 0);
    }
}

Graph MSTCalculator::Calculate(int threadNum) {
    Graph graph(n);
    std::vector<std::thread> threads;
    threads.reserve(threadNum);
    std::vector<size_t> threadResult(threadNum);
    size_t sectorSize = (n + threadNum - 1) / threadNum;

    for (size_t iteration = 1; iteration < n; iteration++) {
        for (size_t i = 0; i < threadNum; i++) {
            size_t first = i * sectorSize, last = std::min(n, first + sectorSize);
            threads.emplace_back(&MSTCalculator::SelectClosestRange, this, first, last,
                                 std::ref(threadResult[i]));
        }
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();
        auto optimal = std::make_pair(DBL_MAX, n);
        for (size_t i : threadResult) {
            if (i == n) {
                continue;
            }
            optimal = std::min({closest[i].first, i}, optimal);
        }
        size_t u = optimal.second;
        size_t v = closest[u].second;
        marked[u] = true;
        graph.AddEdge(v, u);
        for (size_t i = 0; i < threadNum; i++) {
            size_t first = i * sectorSize, last = std::min(n, first + sectorSize);
            threads.emplace_back(&MSTCalculator::UpdateClosestRange, this, first, last, u);
        }
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();
    }
    return graph;
}

void UpdateClosestRange(size_t first, size_t last, size_t p,
                        std::vector<std::pair<double, size_t>>& closest,
                        const std::vector<bool>& marked, const std::vector<Point>& points) {
    size_t n = closest.size();
    for (size_t i = first; i < last; i++) {
        if (marked[i]) {
            continue;
        }
        closest[i] = std::min(closest[i], {DistanceSquared(points[p], points[i]), p});
    }
}

void MSTCalculator::UpdateClosestRange(size_t first, size_t last, size_t p) {
    size_t n = closest.size();
    for (size_t i = first; i < last; i++) {
        if (marked[i]) {
            continue;
        }
        closest[i] = std::min(closest[i], {DistanceSquared(points[p], points[i]), p});
    }
}

void SelectClosestRange(size_t first, size_t last,
                        const std::vector<std::pair<double, size_t>>& closest,
                        const std::vector<bool>& marked, const std::vector<Point>& points,
                        size_t& result) {
    size_t n = closest.size();
    std::pair<double, size_t> optimal = {DBL_MAX, n};
    for (size_t i = first; i < last; i++) {
        if (marked[i]) {
            continue;
        }
        optimal = std::min(optimal, {closest[i].first, i});
    }
    result = optimal.second;
}

void MSTCalculator::SelectClosestRange(size_t first, size_t last, size_t& result) {
    size_t n = closest.size();
    std::pair<double, size_t> optimal = {DBL_MAX, n};
    for (size_t i = first; i < last; i++) {
        if (marked[i]) {
            continue;
        }
        optimal = std::min(optimal, {closest[i].first, i});
    }
    result = optimal.second;
}
