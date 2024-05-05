#include "OctreeNode.h"
#include "hastree.h"
#include "randompoint.h"
#include <benchmark/benchmark.h>
#include <iostream>

int lastMmax = 0;
int lastPoints = 0;
HashOctree *treeP = nullptr;

// https://github.com/google/benchmark/issues/1217
static void basicknn(benchmark::State &state) {
  constexpr int from = -1000000;
  constexpr int to = 1000000;

  int Mmax = state.range(1);
  int points = state.range(0);
  int k = state.range(2);

  if (lastMmax != Mmax || lastPoints != points) {
    lastMmax = Mmax;
    lastPoints = points;
    std::cerr << "Generating points" << std::endl;

    auto p = genPoints<from, to>(points);
    delete treeP;
    treeP = new HashOctree(p, from, to, omp_get_max_threads(), Mmax);
  }

  HashOctree &tree = *treeP;

  auto test = RandomPoint(&genNumber<from, to>);

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  std::vector<Point *> foundedpoints;
  for (auto _ : state) {
    foundedpoints = tree.knn(test, k);
    benchmark::DoNotOptimize(foundedpoints);
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
      for (int knn_size : {8, 16, 32, 64, 96, 128, 256, 512, 1000}) {
        if (Mmax == 8 && points >= 10000)
          continue;
        b->Args({points, Mmax, knn_size});
      }
    }
}

BENCHMARK(basicknn)->Apply(CustomArguments_withoutthr)->Complexity();

BENCHMARK_MAIN();