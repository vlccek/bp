#include <iostream>

#include "voro++.hh"
#include "../src/randompoint.h"
#include "../src/hastree.h"

/*
 * gnuplot
splot "loops_test.gnu" with lines, "loops_test_points.gnu" u 2:3:4 w p  t "points"


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

    voro::c_loop_subset cls(con);
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
    FILE *f1 = fopen("loops_test.gnu", "w");
    FILE *f2 = fopen("loops_test_points.gnu", "w");
    con.draw_particles(f2);
    con.draw_cells_gnuplot("random_points_v.gnu");

    std::cout << "Test with " << numberofpoints << " points " << std::endl;

    std::cout << std::format("{:^5}|{:^15}|{:^15}|{:^15}|{:^15}\n",
                             "ID", "Cells count", "cells volume", "NOT cells", "% diff in volume");

    for (auto i: splitboxby8({FROM, FROM, FROM}, {TO, TO, TO})) {
        cls.setup_box(i.min.x, i.max.x, i.min.y,
                      i.max.y, i.min.z, i.max.z,
                      false);

        sum_volume_subbox = 0;
        sum_voronoi = 0;
        sum_not_voronoi = 0;


        if (cls.start())
            do {
                if (con.compute_cell(c, cls)) {
                    sum_volume_subbox += c.volume();
                    sum_voronoi++;
                    if (i.level_order != 4) { continue; }
                    cls.pos(x, y, z);
                    c.draw_gnuplot(x, y, z, f1);
                } else {
                    sum_not_voronoi++;
                }

            } while (cls.inc());


        std::cout << std::format("{:^5}{:^15}{:^15.5}{:^15}{:^15.3}%\n",
                                 i.level_order, sum_voronoi, sum_volume_subbox, sum_not_voronoi,
                                 sum_volume_subbox / (pow(TO - FROM, 3) / 8));
        sum_volume_all += sum_volume_subbox;
        sum_voronoi_all += sum_voronoi;
        sum_not_voronoi_all += sum_not_voronoi;
    }
    std::cout << std::format("{:^5}{:^15}{:^15.5}{:^15}{:^15.3}%\n",
                             "-S-", sum_voronoi_all, sum_volume_all, sum_not_voronoi_all,
                             sum_volume_all / (pow(TO - FROM, 3)));


    std::cout << std::format("SUMMARY:\n"
                             "Count of cells: {:^8}\n"
                             "Volume of cells count: {:^15.5}\n"
                             "Count of cells without cells: {:^15}\n"
                             "diff of volume: {:^15.3}%\n\n\n",

                             sum_voronoi_all, sum_volume_all, sum_not_voronoi_all,
                             sum_volume_all / (pow(TO - FROM, 3)));
    fclose(f1);
}


int main() {
    for (int i: {30})
        spliting_test<0, 10>(i);
}