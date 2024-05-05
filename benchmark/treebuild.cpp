#include "OctreeNode.h"
#include "hastree.h"
#include "randompoint.h"
#include <benchmark/benchmark.h>
#include <iostream>

static void treebuild_randompoints(benchmark::State &state) {
  std::vector<Point> p;
  constexpr float from = 0;
  constexpr float to = 100;

  int Mmax = state.range(1);
  int points = state.range(0);

  for (int i = 0; i < points; i++) {
    p.push_back(RandomPoint(&genNumber<from, to>));
    // std::cout << std::format("({},{},{})", p[p.size() - 1].x, p[p.size() -
    // 1].y, p[p.size() - 1].z) << std::endl;
  }

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  HashOctree *tree;
  for (auto _ : state) {
    tree = new HashOctree(p, from, to, omp_get_max_threads(), Mmax);
    benchmark::DoNotOptimize(tree);
    state.PauseTiming();

    state.counters["Time_voro(ns)"] =
        duration_cast<chrono::nanoseconds>(tree->voroBuild - tree->start)
            .count();
    state.counters["Time_tree(ns)"] =
        duration_cast<chrono::nanoseconds>(tree->treeBuild - tree->voroBuild)
            .count();
    state.counters["Time_hashtable(ns)"] =
        duration_cast<chrono::nanoseconds>(tree->htBuild - tree->treeBuild)
            .count();

    state.counters["MaxTreeLevel"] = tree->maxLevel;
    state.counters["nodes"] = tree->hashTable.size();

    delete tree;

    state.ResumeTiming();
  }

  // delete tree;

  state.SetComplexityN(points);
}

static void CustomArguments(benchmark::internal::Benchmark *b) {
  int threadMax = omp_get_max_threads();
  int MmaxMax = 30;
  int pointsMax = 10000000;

  for (int threads = 1; threads <= threadMax; threads *= 2)
    for ( auto& Mmax : {8, 16, 32, 64, 96})
      for (int points = 100; points <= pointsMax; points *= 10)
        b->Args({points, threads, Mmax});
}

static void CustomArguments_withoutthr(benchmark::internal::Benchmark *b) {

  int pointsMax = 1000000;

  for (int points = 100; points <= pointsMax; points *= 10)
    for (int Mmax : {8, 16, 32, 64, 96, 128, 256}) {
      if (Mmax == 8 && points == 1000000)
        continue;
      b->Args({points, Mmax});
    }
}



BENCHMARK(treebuild_randompoints)
    ->Apply(CustomArguments_withoutthr)
    // ->ThreadRange(1, 24)
    ->Complexity(benchmark::oAuto);

BENCHMARK_MAIN();