#include "point.h"
#include "randompoint.h"

#include "hastree.h"
#include "voro++.hh"
#include <format>
#include <iostream>
#include <random>
#include "omp.h"

using namespace voro;

const int ten_milion = 10000000;
const int milion = 1000000;

int main() {


    auto p = genPoints<0, 100>(ten_milion);

    HashOctree tree(p, 0,100, omp_get_max_threads());

    return 0;
}
