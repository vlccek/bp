#include "OctreeNode.h"
#include "hastree.h"
#include "randompoint.h"
#include <benchmark/benchmark.h>
#include <iostream>

// dont judge me for this
int lastMmax = 0;
int lastPoints = 0;
HashOctree *treeP = nullptr;

std::vector<Point> readPointsFromFile(std::string filename) {
  std::vector<Point> rest;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "File not found: " << filename << std::endl;
    exit(1);
  }
  std::string str;
  while (std::getline(file, str)) {
    std::istringstream iss(str);
    float x, y, z, normalx, normaly, normalz;
    iss >> x >> y >> z >> normalx >> normaly >> normalz;
    rest.emplace_back(x, y, z, normalx, normaly, normalz);
  }

  return rest;
}

static void nnNormalFirst(benchmark::State &state) {

  constexpr int from = -1000000;
  constexpr int to = 1000000;

  int Mmax = state.range(1);
  int points = state.range(0);

  if (lastMmax != Mmax || lastPoints != points) {
    lastMmax = Mmax;
    lastPoints = points;
    std::cerr << "Generating points" << std::endl;

    auto p = readPointsFromFile(
        std::format("points_3d_normal_plane_{}.txt", points));
    delete treeP;
    treeP = new HashOctree(p, from, to, omp_get_max_threads(), Mmax);
  }

  HashOctree &tree = *treeP;

  std::vector<Point> testedpoints =
      readPointsFromFile(std::format("points_3d_testedpoints_{}.txt", 0));

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  auto test = testedpoints[0];

  Point foundedpoint(-1, -1, -1);
  for (auto _ : state) {
    foundedpoint = tree.nnFirstNormal(test);
    benchmark::DoNotOptimize(foundedpoint);
    state.PauseTiming();

    int randomIndex = genNumber<0, 99>();
    test = testedpoints[randomIndex];
    state.ResumeTiming();
  }

  state.SetComplexityN(state.range(0));
}

static void nnNormalLeaf(benchmark::State &state) {

  constexpr int from = -1000000;
  constexpr int to = 1000000;

  int Mmax = state.range(1);
  int points = state.range(0);

  if (lastMmax != Mmax || lastPoints != points) {
    lastMmax = Mmax;
    lastPoints = points;
    std::cerr << "Generating points" << std::endl;

    auto p = readPointsFromFile(
        std::format("points_3d_normal_plane_{}.txt", points));

    delete treeP;
    treeP = new HashOctree(p, from, to, omp_get_max_threads(), Mmax);
  }

  HashOctree &tree = *treeP;

  std::vector<Point> testedpoints =
      readPointsFromFile(std::format("points_3d_testedpoints_{}.txt", 0));

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  auto test = testedpoints[0];

  Point foundedpoint(-1, -1, -1);
  for (auto _ : state) {
    foundedpoint = tree.nnBestNormalLeaf(test, 0);
    benchmark::DoNotOptimize(foundedpoint);
    state.PauseTiming();

    int randomIndex = genNumber<0, 99>();
    test = testedpoints[randomIndex];
    state.ResumeTiming();
  }

  state.SetComplexityN(state.range(0));
}

static void nnNormalIterator(benchmark::State &state) {

  constexpr int from = -1000000;
  constexpr int to = 1000000;

  int Mmax = state.range(1);
  int points = state.range(0);

  if (lastMmax != Mmax || lastPoints != points) {
    lastMmax = Mmax;
    lastPoints = points;
    std::cerr << "Generating points" << std::endl;

    auto p = readPointsFromFile(
        std::format("points_3d_normal_plane_{}.txt", points));
    delete treeP;
    treeP = new HashOctree(p, from, to, omp_get_max_threads(), Mmax);
  }

  HashOctree &tree = *treeP;

  std::vector<Point> testedpoints =
      readPointsFromFile(std::format("points_3d_testedpoints_{}.txt", 0));

  // std::cerr << "Running benchmark with " << state.range() << "points. \n" <<
  // std::endl;
  auto test = testedpoints[0];

  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  auto it = tree.nnNormalSearch(test, 100000);
  std::chrono::high_resolution_clock::time_point stop =
      std::chrono::high_resolution_clock::now();

  state.counters["interator build(ns)"] =
      duration_cast<chrono::nanoseconds>(stop - start).count();

  Point *foundedpoint;
  for (auto _ : state) {
    foundedpoint = it.next();
    benchmark::DoNotOptimize(foundedpoint);
    state.PauseTiming();
    int randomIndex = genNumber<0, 99>();
    test = testedpoints[randomIndex];
    it = tree.nnNormalSearch(test, 100000);
    state.ResumeTiming();
  }

  state.SetComplexityN(state.range(0));
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

BENCHMARK(nnNormalFirst)->Apply(CustomArguments_withoutthr)->Complexity();
BENCHMARK(nnNormalLeaf)->Apply(CustomArguments_withoutthr)->Complexity();
BENCHMARK(nnNormalIterator)->Apply(CustomArguments_withoutthr)->Complexity();

BENCHMARK_MAIN();