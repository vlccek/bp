
#include <benchmark/benchmark.h>
#include <iostream>
#include "OctrerNodeBuilder.h"
#include "randompoint.h"

static void vorocell_computation_pre(benchmark::State &state) {
    voro::container_3d con(-10, 10, -10, 10, -10, 10, 26, 26, 26, false, false, false, 8);

    for (int i = 0; i < state.range(0); i++) {
        RandomPoint p(genNumber<-10, 10>);
        con.put(i, p.x, p.y, p.z);
    }


    voro::voronoicell_3d c;


    for (auto _: state)
        con.compute_all_cells();

    state.SetComplexityN(state.range(0));
}
// Register the function as a benchmark
BENCHMARK(vorocell_computation_pre)
        ->RangeMultiplier(2)
        ->Range(1 << 10, 1 << 15)
        ->Complexity(benchmark::oAuto);


// create a simular benchmark for computing vorocell with using just container class
static void vorocell_computation(benchmark::State &state) {
    voro::container_3d con(-10, 10, -10, 10, -10, 10, 26, 26, 26, false, false, false, 8);


    for (int i = 0; i < state.range(0); i++) {
        RandomPoint p(genNumber<-10, 10>);
        con.put(i, p.x, p.y, p.z);
    }


    voro::voronoicell_3d c;


    for (auto _: state)
        con.compute_all_cells();

    state.SetComplexityN(state.range(0));
}

BENCHMARK(vorocell_computation)
        ->RangeMultiplier(2)
        ->Range(1 << 10, 1 << 15)
        ->Complexity(benchmark::oAuto);


BENCHMARK_MAIN();