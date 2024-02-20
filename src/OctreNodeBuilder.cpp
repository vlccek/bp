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
    if (border.isTooSmall()) {
        std::cout << "Unable to build tree becouse the region is too small" << std::endl;

        border.writeGnuFormat();
        // todo remove this
        exit(10);
    }
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
#if 1
            // check by bounding box
            auto boudingboxVertexs = ph->boudingBox.allVertex();
            //check if bounding box interferes to the box
            if (box.isInside(boudingboxVertexs)) {
                // use gjk algorithm to detect intersection
#if 1
                alocateIfNeccesary(l, box);
                childs[l]->addVoroCell(ph);
                continue;
#else
                if (gjk(box, ph)) {
                    alocateIfNeccesary(l, box);
                    childs[l]->addVoroCell(ph);
                    continue;
                }
#endif
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

void OctrerNodeBuilder::printVoronoiCells(std::ofstream &file) {
    for (auto &vc: voronoiCells) {
        // file << vc->p.operator std::string() << std::endl;
        for (auto &p: vc->vertexPoints) {
            file << p.operator std::string() << std::endl;
        }
        file << std::endl;  // empty line
    }
}

void OctrerNodeBuilder::getAllNodes(std::set<OctrerNodeBuilder *> &allNodes) {
    for (auto child: childs) {
        if (child == nullptr) {
            continue;
        }
        allNodes.insert(child);
        // todo add comdiotn if in allNodes is already all nodes dont need to iterate through all nodes
        child->getAllNodes(allNodes);
    }
}


