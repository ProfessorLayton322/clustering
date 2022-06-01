#include <algorithm>
#include <atomic>
#include <float.h>
#include <utility>
#include <thread>
#include <functional>
#include "prim_thread.h"

MSTCalculator::MSTCalculator(const std::vector<Point>& points)
    : verts_(points.size()), points_(points), closest_(points.size()), marked_(points.size()) {
    marked_[0] = true;
    for (size_t i = 1; i < verts_; i++) {
        closest_[i] = std::make_pair(DistanceSquared(points_[0], points_[i]), 0);
    }
}

MSTCalculator::MSTCalculator(std::vector<Point>&& points)
    : verts_(points.size()), points_(std::move(points)), closest_(points.size()), marked_(points.size()) {
    marked_[0] = true;
    for (size_t i = 1; i < verts_; i++) {
        closest_[i] = std::make_pair(DistanceSquared(points_[0], points_[i]), 0);
    }
}

Graph MSTCalculator::Calculate(int threadNum) {
    Graph graph(points_);
    std::vector<std::thread> threads;
    threads.reserve(threadNum);
    std::vector<size_t> threadResult(threadNum);
    size_t sectorSize = (verts_ + threadNum - 1) / threadNum;

    for (size_t iteration = 1; iteration < verts_; iteration++) {
        for (size_t i = 0; i < threadNum; i++) {
            size_t first = i * sectorSize, last = std::min(verts_, first + sectorSize);
            threads.emplace_back(&MSTCalculator::SelectClosestRange, this, first, last,
                                 std::ref(threadResult[i]));
        }
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();
        auto optimal = std::make_pair(DBL_MAX, verts_);
        for (size_t i : threadResult) {
            if (i == verts_) {
                continue;
            }
            optimal = std::min({closest_[i].first, i}, optimal);
        }
        size_t u = optimal.second;
        size_t v = closest_[u].second;
        marked_[u] = true;
        graph.AddEdge(v, u);
        for (size_t i = 0; i < threadNum; i++) {
            size_t first = i * sectorSize, last = std::min(verts_, first + sectorSize);
            threads.emplace_back(&MSTCalculator::UpdateClosestRange, this, first, last, u);
        }
        for (auto& thread : threads) {
            thread.join();
        }
        threads.clear();
    }
    return graph;
}

void MSTCalculator::UpdateClosestRange(size_t first, size_t last, size_t p) {
    for (size_t i = first; i < last; i++) {
        if (marked_[i]) {
            continue;
        }
        closest_[i] = std::min(closest_[i], {DistanceSquared(points_[p], points_[i]), p});
    }
}

void MSTCalculator::SelectClosestRange(size_t first, size_t last, size_t& result) {
    std::pair<double, size_t> optimal = {DBL_MAX, verts_};
    for (size_t i = first; i < last; i++) {
        if (marked_[i]) {
            continue;
        }
        optimal = std::min(optimal, {closest_[i].first, i});
    }
    result = optimal.second;
}
