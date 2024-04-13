#include "OctreeNode.h"
#include "hastree.h"
#include "randompoint.h"
#include <benchmark/benchmark.h>
#include <iostream>

// https://github.com/google/benchmark/issues/1217
static void treebuild_randompoints(benchmark::State &state) {
    std::vector<Point> p;
    constexpr int from = 0;
    constexpr int to = 100;


    for (int i = 0; i < state.range(0); i++) {
        p.push_back(RandomPoint(&genNumber<from, to>));
        // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() - 1].y, p[p.size() - 1].z) << std::endl;
    }

    std::cerr << "Running benchmark with " << state.range() << "points. \n" << std::endl;
    for (auto _: state)
        HashOctree tree(p, from, to);

    state.SetComplexityN(state.range(0));
}

BENCHMARK(treebuild_randompoints)
->RangeMultiplier(10)
->Range(10,1000000)
->Complexity(benchmark::oAuto);

BENCHMARK_MAIN();