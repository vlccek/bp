
#include <iostream>

#include "voro++.hh"
#include "../src/randompoint.h"
#include "../src/hastree.h"

/*
 * gnuplot
splot "all_cells.gnu" with lines, "ghost_cells.gnu" with lines
 */




template<int FROM, int TO>
void spliting_test(int numberofpoints) {
    int id = 0;

    voro::pre_container pre_con(FROM, TO, FROM, TO, FROM, TO, false, false, false);

    pre_con.put(numberofpoints + 1, 1, 1, 1);
    for (int i = 0; i < numberofpoints; i++) {
        RandomPoint p(genNumber<FROM, TO>);
        pre_con.put(id, p.x, p.y, p.z);
    }

    voro::container con(FROM, TO, FROM, TO, FROM, TO, 1, 1, 1, false, false, false, 1);
    pre_con.setup(con);

    voro::voronoicell c;

    con.compute_all_cells();

    FILE *f_cells = fopen("all_cells.gnu", "w");
    FILE *f_ghost_cell = fopen("ghost_cells.gnu", "w");

    voro::voronoicell_neighbor vc;
    bool a = con.compute_ghost_cell(vc, 1, 1, 1);


    vc.draw_gnuplot(1, 1, 1, f_ghost_cell);
    con.draw_cells_gnuplot(f_cells);
}


int main() {
    for (int i: {10})
        spliting_test<0, 10>(i);
}