#include <benchmark/benchmark.h>
#include <iostream>
#include "OctrerNodeBuilder.h"
#include "randompoint.h"
#include "hastree.h"


// https://github.com/google/benchmark/issues/1217
static void basicnn(benchmark::State &state) {
    std::vector<Point> p{
            Point(0, 0, .95)
    };
    constexpr int from = 0;
    constexpr int to = 100;


    for (int i = 0; i < state.range(0); i++) {
        p.push_back(RandomPoint(&genNumber<from, to>));
        // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() - 1].y, p[p.size() - 1].z) << std::endl;
    }

    HashOctree tree(p, from, to);

    auto point = Point(0, 0, .99);

    std::cerr << "Running benchmark with " << state.range() << "points. \n" << std::endl;
    Point findedpoint(-1, -1, -1);
    for (auto _: state)
        findedpoint = tree.nn(point);


    state.SetComplexityN(state.range(0));
}

BENCHMARK(basicnn)->RangeMultiplier(2)
        ->Range(1 << 10, 1 << 20)
        ->Complexity(benchmark::o1);

BENCHMARK_MAIN();