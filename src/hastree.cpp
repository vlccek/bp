//
// Created by jvlk on 27.9.23.
//

#include "hastree.h"


HashOctree::HashOctree(std::vector<Point> &p, const Point &min,
                       const Point &max) : min(min), max(max) {

    voro::pre_container pre_con(min.x, max.x, min.y, max.y, min.z, max.z, false, false, false);


    int id = 0;
    for (auto i: p) {
        pre_con.put(id++, i.x, i.y, i.z);
    }
    std::cout << "Points inserted \n";


    voro::container con(min.x, max.x, min.y, max.y, min.z, max.z, 6, 6, 6, false, false,
                        false, 8);

    pre_con.setup(con);

    voro::c_loop_all cls(con);

    voro::voronoicell c;

    std::vector<double> vertex;
    double x, y, z;

    if (cls.start()) {
        do {
            con.compute_cell(c, cls);
            cls.pos(x, y, z);
            Point po(x, y, z);
            std::vector<int> a;
            c.face_vertices(a);


            voronoiCells.push_back(Polyhedron(c, po));
        } while (cls.inc());

        initTree();
        root->buildTree();
    }
}


void HashOctree::initTree() {
    for (int i = 0; i < voronoiCells.size(); ++i) {
        root->addVoroCell(&(this->voronoiCells[i]));
    }
    root->setBox(min, max);
}

void HashOctree::buildTree() {
    root->buildTree();
}







