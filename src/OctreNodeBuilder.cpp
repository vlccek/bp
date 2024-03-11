//
// Created by jvlk on 18.10.23.
//

#include "OctrerNodeBuilder.h"


OctrerNodeBuilder::OctrerNodeBuilder(int level, int *maxLevel, voro::container_3d *con,
                                     std::vector<Polyhedron> *allVoronoiCells) {
    this->maxLevel = maxLevel;
    this->level = level;

    if (level > *maxLevel) {
        *maxLevel = level;
    }
    voronoiCells = {};
    this->con = con;
    this->allVoronoiCells = allVoronoiCells;
}


void OctrerNodeBuilder::buildTree() {
    if (border.isTooSmall()) {
        std::cout << "Unable to build tree becouse the region is too small" << std::endl;
        exit(10);
    }

    std::vector<Box> splitedBox = border.splitBoxBy8();
    for (int l = 0; l < splitedBox.size(); ++l) {

        auto box = splitedBox[l];
        double x = 0, y = 0, z = 0;
        int pid;
        con->find_voronoi_cell(box.min.x, box.min.y, box.min.z, x, y, z, pid);


        std::set<int> visitedCells = {pid};
        std::set<int> toVisit = {};
        std::set<int> totoVisit = {};

        Polyhedron &first = allVoronoiCells->at(pid);
        toVisit.insert(first.neighbors.begin(), first.neighbors.end());

        while (!toVisit.empty()) {
            for (auto &idVc: toVisit) {
                if (idVc < 0) {
                    continue;
                }
                Polyhedron &voronoiCell = allVoronoiCells->at(idVc);
                Polyhedron *ph = &voronoiCell;

                if (intersect(splitedBox[l], voronoiCell)) {
                    alocateIfNeccesary(l, box);
                    childs[l]->addVoroCell(ph);

                    // add neighbors of neighbor to totoVisit
                    totoVisit.insert(voronoiCell.neighbors.begin(), voronoiCell.neighbors.end());
                }
            }


            visitedCells.insert(toVisit.begin(), toVisit.end());
            toVisit.clear();
            std::set_difference(totoVisit.begin(), totoVisit.end(), visitedCells.begin(), visitedCells.end(),
                                std::inserter(toVisit, toVisit.begin())
            );

            totoVisit.clear();

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
        childs[index] = new OctrerNodeBuilder(level + 1, maxLevel, this->con);
        childs[index]->border = b;
        childs[index]->addAllVoroCell(allVoronoiCells);
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

#pragma omp parallel
    {
#pragma omp for
        for (auto child: childs) {
            if (child == nullptr) {
                continue;
            }
#pragma omp critical
            allNodes.insert(child);
            // todo add comdiotn if in allNodes is already all nodes dont need to iterate through all nodes
            child->getAllNodes(allNodes);
        }
    }

}

void OctrerNodeBuilder::addAllVoroCell(std::vector<Polyhedron> *vcVector) {
    allVoronoiCells = vcVector;
}

bool OctrerNodeBuilder::intersect(Box &b, Polyhedron &vc) {
    Polyhedron *ph = &vc;
    Point particle = vc.p;
    std::vector<Point> *vp = &vc.vertexPoints; // vertex point of the voronoi cell

    // center point of voronoi cell
    auto box = b;
    // is particle in BOX ?
    if (box.isInside(particle)) {
        return true;
    }


    // is one of vertex of voronoi cell in box ?
    if (box.isInside(vp)) {
        return true;
    }
    // check by bounding box
    auto boudingboxVertexs = ph->boudingBox.allVertex();
    //check if bounding box interferes to the box
    if (box.isInside(boudingboxVertexs)) {
        // use gjk algorithm to detect intersection
        //if 1 skips gjk algorithm and just return true (if bounding box intersects)
#if 1
        return true;
#else
        if (gjk(box, ph)) {
                return true;
        }
#endif
    }
    return false;
}



