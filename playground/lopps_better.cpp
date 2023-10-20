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
    FILE *f2 = fopen("loops_every_point-points.gnu", "w"); // edge points
    FILE *f4 = fopen("loops_every_point-points-set.gnu", "w"); // voronoi cell's points
    FILE *f3 = fopen("loops_every_point-box.gnu", "w");
    fprintf(f3, "%s",
            "0 0 0\n"
            "5 0 0\n"
            "5 5 0\n"
            "0 5 0\n"
            "0 0 0\n"
            "0 0 5\n"
            "0 5 5\n"
            "0 5 0\n"
            "\n\n"
            "5 5 5\n"
            "5 5 0\n"
            "5 0 0\n"
            "5 0 5\n"
            "0 0 5\n"
            "0 5 5\n"
            "5 5 5\n"
            "5 0 5");


    auto boxs = splitboxby8({FROM, FROM, FROM}, {TO, TO, TO});

    if (cls.start())
        do {
            if (con.compute_cell(c, cls)) {
                std::vector<double> a;
                cls.pos(x, y, z);
                c.vertices(x, y, z, a);

                if (boxs[0].isInside({x, y, z})) {
                    c.draw_gnuplot(x, y, z, f1);
                    fprintf(f4, "%g %g %g\n", x, y, z);
                    continue;
                }


                for (int j = 0; j < a.size(); j += 3) {
                    if (boxs[0].isInside({a[j], a[j + 1], a[j + 2]})) {
                        // boxs.erase(boxs.begin());

                        cls.pos(x, y, z);
                        c.draw_gnuplot(x, y, z, f1);

                        sum_volume_subbox += c.volume();
                        sum_voronoi++;

                        fprintf(f2, "%g %g %g\n", a[j], a[j + 1], a[j + 2]);

                        std::cout << std::format("{:^15}{:^15.5}{:^15}{:^15.3}%\n",
                                                 sum_voronoi, sum_volume_subbox, sum_not_voronoi,
                                                 sum_volume_subbox / (pow(TO - FROM, 3) / 8));
                        sum_volume_all += sum_volume_subbox;
                        sum_voronoi_all += sum_voronoi;
                        sum_not_voronoi_all += sum_not_voronoi;

                        sum_volume_subbox = 0;
                        sum_not_voronoi = 0;
                        break;
                    }
                }
            }

        } while (cls.inc());

    std::cout << std::format("{:^5}{:^15}{:^15.5}{:^15}{:^15.3}%\n",
                             "-S-", sum_voronoi_all, sum_volume_all, sum_not_voronoi_all,
                             sum_volume_all / (pow(TO - FROM, 3)));


}


int main() {
    for (int i: {10})
        spliting_test<0, 10>(i);
}