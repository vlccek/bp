#include "OctreeNode.h"
#include "hastree.h"
#include "randompoint.h"
#include <benchmark/benchmark.h>
#include <iostream>

// https://github.com/google/benchmark/issues/1217
static void basicnn(benchmark::State &state) {
  std::vector<Point> p{Point(0, 0, .95)};
  constexpr int from = -1000000;
  constexpr int to = 1000000;

  int Mmax = state.range(1);
  int points = state.range(0);

  for (int i = 0; i < state.range(0); i++) {
    p.push_back(RandomPoint(&genNumber<from, to>));
    // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() -
    // 1].y, p[p.size() - 1].z) << std::endl;
  }

  HashOctree tree(p, from, to, omp_get_max_threads(), Mmax);

  int counter = 0;

  auto test = RandomPoint(&genNumber<from, to>);

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  Point foundedpoint(-1, -1, -1);
  for (auto _ : state) {
    foundedpoint = tree.nn(test);
    state.PauseTiming();
    test = RandomPoint(&genNumber<from, to>);
    state.ResumeTiming();
  }
  state.SetComplexityN(state.range(0));
}

static void CustomArguments_withoutthr(benchmark::internal::Benchmark *b) {

  int MmaxMax = 30;
  int pointsMax = 100000000;

  for (int points = 10000000; points <= pointsMax; points *= 10)
    for (int Mmax : {8, 16, 32, 64, 96}) {
      if ((Mmax ==  8 || Mmax == 16) && points > 1000000) {
        continue;
      }
      b->Args({points, Mmax});
    }
}

BENCHMARK(basicnn)->Apply(CustomArguments_withoutthr)->Complexity();

BENCHMARK_MAIN();