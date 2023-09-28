#include "point.h"
#include "randompoint.h"

#include <random>
#include <iostream>
#include "deps/voro/src/voro++.hh"

using namespace voro;


static double genNumber() {
    static std::default_random_engine e1(1);
    static std::uniform_int_distribution<int> uniform_dist(1, 1000);

    return uniform_dist(e1);
}


int main() {
    std::cout << "Hello, World!" << std::endl;

    container con(0, 1000, 0, 1000, 0, 1000, 6, 6, 6,
                  false, false, false, 8);

    for (int i = 0; i < 100; i++) {
        auto point = RandomPoint(&genNumber);
        point.repr();
        con.put(i, point.x, point.y, point.z);
    }

    double vvol = con.sum_cell_volumes();
    printf("Container volume : %g\n"
           "Voronoi volume   : %g\n"
           "Difference       : %g\n", 1000 * 1000 * 1000.0, vvol, vvol - 1000 * 1000 * 1000);

    // Output the particle positions in gnuplot format
    con.draw_particles("random_points_p.gnu");

    // Output the Voronoi cells in gnuplot format
    con.draw_cells_gnuplot("random_points_v.gnu");

    return 0;
}
