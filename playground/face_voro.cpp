#include <iostream>

#include "voro++.hh"
#include "../src/randompoint.h"
#include "../src/hastree.h"

/*
 * gnuplot
 splot "loops_every_point.gnu" with lines, "loops_every_point-box.gnu" with lines lw 3, "loops_every_point-points.gnu" lw 3, "





 */


template<int FROM, int TO>
void spliting_test(int numberofpoints) {
    int id = 0;

    voro::pre_container pre_con(FROM, TO, FROM, TO, FROM, TO, false, false, false);

    for (int i = 0; i < numberofpoints; i++) {
        RandomPoint p(genNumber<FROM, TO>);
        pre_con.put(id, p.x, p.y, p.z);
    }

    voro::container con(FROM, TO, FROM, TO, FROM, TO, 1, 1, 1, false, false, false, 1);
    pre_con.setup(con);

    voro::voronoicell c;

    con.compute_all_cells();

    //Stats:
    double sum_volume_subbox = 0;
    double sum_volume_all = 0;

    int sum_voronoi = 0;
    int sum_voronoi_all = 0;

    int sum_not_voronoi = 0;
    int sum_not_voronoi_all = 0;

    int l;
    double x, y, z, r, rx, ry, rz;


    std::cout << "Test with " << numberofpoints << " points " << std::endl;

    std::cout << std::format("{:^15}|{:^15}|{:^15}|{:^15}\n",
                             "Counter", "cells volume", "NOT cells", "% diff in volume");

    voro::c_loop_all cls(con);

    FILE *f1 = fopen("loops_every_point.gnu", "w"); //
  


    auto boxs = splitboxby8({FROM, FROM, FROM}, {TO, TO, TO});

    if (cls.start())
        do {
            if (con.compute_cell(c, cls)) {
                std::vector<int> a;
                cls.pos(x, y, z);
                c.face_vertices(a);
                c.draw_gnuplot(x, y, z, f1);
            }

        } while (cls.inc());

}


int main() {
    for (int i: {10})
        spliting_test<0, 10>(i);
}