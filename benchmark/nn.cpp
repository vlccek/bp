#include "OctreeNode.h"
#include "hastree.h"
#include "randompoint.h"
#include <benchmark/benchmark.h>
#include <iostream>


// https://github.com/google/benchmark/issues/1217
static void basicnn(benchmark::State &state) {
  constexpr int from = -1000000;
  constexpr int to = 1000000;

  int Mmax = state.range(1);
  int points = state.range(0);

  auto p = genPoints<from, to>(points);

  HashOctree tree(p, from, to, omp_get_max_threads(), Mmax);

  int counter = 0;

  auto test = RandomPoint(&genNumber<from, to>);

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  Point foundedpoint(-1, -1, -1);
  for (auto _ : state) {
    foundedpoint = tree.nn(test);
    benchmark::DoNotOptimize(foundedpoint);
    state.PauseTiming();
    test = RandomPoint(&genNumber<from, to>);
    state.ResumeTiming();
  }
  state.SetComplexityN(state.range(0));
}

static void CustomArguments_withoutthr(benchmark::internal::Benchmark *b) {

  int pointsMax = 1000000;

  for (int points = 1000000; points <= pointsMax; points *= 10)
    for (int Mmax : {8, 16, 32, 64, 96, 128, 256}) {
      if (Mmax == 8 && points == 1000000)
        continue;
      b->Args({points, Mmax});
    }
}

BENCHMARK(basicnn)->Apply(CustomArguments_withoutthr)->Complexity();

BENCHMARK_MAIN();