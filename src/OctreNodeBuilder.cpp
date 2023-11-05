//
// Created by jvlk on 18.10.23.
//

#include "OctrerNodeBuilder.h"


OctrerNodeBuilder::OctrerNodeBuilder(int level, int *maxLevel) {
    this->maxLevel = maxLevel;
    this->level = level;

    if (level > *maxLevel) {
        *maxLevel = level;
    }
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
#if 1
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
#endif
#if 0
            // check by bounding box
            auto boudingboxVertexs = ph->boudingBox.allVertex();
            //check if bounding box interferes to the box
            if (box.isInside(boudingboxVertexs)) {
                // use gjk algorithm to detect intersection
                if (gjk(box, ph)) {
                    alocateIfNeccesary(l, box);
                    childs[l]->addVoroCell(ph);
                    continue;
                }
            }
#endif

        }
        if (childs[l] != nullptr && childs[l]->voronoiCells.size() >= 5) // todo remove magic constant
        {
            childs[l]->buildTree();
        }

    }


}


void OctrerNodeBuilder::alocateIfNeccesary(int index, Box &b) {
    if (childs[index] == nullptr) {
        childCount++;
        childs[index] = new OctrerNodeBuilder(level + 1, maxLevel);
        childs[index]->border = b;
    }

}

void OctrerNodeBuilder::getLeafs(std::vector<OctrerNodeBuilder *> &leafs) {
    for (auto &child: childs) {
        if (child != nullptr) {
            if (child->childCount == 0) {
                leafs.push_back(child);
                continue;
            } else {
                child->getLeafs(leafs);
            }
        }
    }
}

OctrerNodeBuilder::~OctrerNodeBuilder() {
    for (auto &child: childs) {
        if (child != nullptr) {
            delete child;
        }
    }

}



