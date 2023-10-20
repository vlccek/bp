//
// Created by jvlk on 18.10.23.
//

#include "OctrerNodeBuilder.h"


OctrerNodeBuilder::OctrerNodeBuilder() {
    voronoiCells = {};
}

void OctrerNodeBuilder::addVoroCell(Polyhedron *vc) {
    voronoiCells.push_back(vc);
}

void OctrerNodeBuilder::buildTree() {
    std::vector<Box> splitedBox = border.splitBoxBy8();
    int l;
    for (l = 0; l < splitedBox.size(); ++l) {
        for (auto &voronoiCell: voronoiCells) {
            Polyhedron *ph = voronoiCell;
            Point particle = voronoiCell->p;
            std::vector<Point> *vp = &voronoiCell->vertexPoints; // vertex point of the voronoi cell

            // center point of voronoi cell
            auto box = splitedBox[l];

            // is particle in BOX ?
            if (box.isInside(particle)) {
                alocateIfNeccesary(l, box);
                childs[l]->addVoroCell(ph);
                continue;
            }


            // is one of vertex of voronoi cell in box ?
            if (box.isInside(vp)) {
                alocateIfNeccesary(l, box);
                childs[l]->addVoroCell(ph);
                continue;
            }

        }
        if (childs[l] != nullptr && childs[l]->voronoiCells.size() >= 5) // todo remove magic constant
        {
            childs[l]->buildTree();
        }

    }


}


void OctrerNodeBuilder::alocateIfNeccesary(int index, Box &b) {
    if (childs[index] == nullptr) {
        childs[index] = new OctrerNodeBuilder();
        childs[index]->border = b;
    }

}

