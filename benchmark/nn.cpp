#include <benchmark/benchmark.h>
#include <iostream>
#include "OctrerNodeBuilder.h"
#include "randompoint.h"
#include "hastree.h"

static void basicnn(benchmark::State &state) {
    std::vector<Point> p{
            Point(0, 0, .95)
    };


    for (int i = 0; i < state.range(0); i++) {
        p.push_back(RandomPoint(&genNumber<0, 1>));
        // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() - 1].y, p[p.size() - 1].z) << std::endl;
    }

    HashOctree tree(p, {0, 0, 0}, {1, 1, 1});

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