#include "point.h"
#include "randompoint.h"

#include "hastree.h"
#include "voro++.hh"
#include <format>
#include <iostream>
#include <random>
#include "omp.h"

using namespace voro;


int main() {


    auto p = genPoints<0, 100>(100000);
    omp_set_num_threads(1);

    HashOctree tree(p, 0,100);

    return 0;
}
