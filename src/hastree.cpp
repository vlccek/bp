//
// Created by jvlk on 27.9.23.
//

#include "hastree.h"


HashOctree::HashOctree(std::vector<Point> &p, const Point &min,
                       const Point &max) : min(min), max(max) {

    voro::pre_container pre_con(min.x, max.x, min.y, max.y, min.z, max.z, false, false, false);
    pointCount = (p.size());

    int id = 0;
    for (auto i: p) {
        pre_con.put(id++, i.x, i.y, i.z);
    }

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

        buildTree();

        buildHashTable();
    }
}

HashOctree::HashOctree(std::vector<Point> &p, const double min, const double max) : HashOctree(p, Point(min, min, min),
                                                                                               Point(max, max, max)) {
//empty
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

void HashOctree::buildHashTable() {
    std::set<OctrerNodeBuilder *> allNodes;
    root->getAllNodes(allNodes);

    for (auto i: allNodes) {
        auto p = findBellogingIntervalsByLevel(i->border.center(), i->level);
        hashTable[std::make_pair(i->level, p)] = i;
    }
}


Point HashOctree::nn(Point &p) {
#if 0
    if (pointCount < (8 * 4) + 1) { // todo add connection to max number of points in node
        return findClosesPointInNode(p, root);
    }
#endif

    int lmin = 0; // minimal leaf node level
    int lmax = maxLevel;
    int lc;

    std::tuple<int, int, int> idx = {0, 0, 0};


    while (lmax - lmin > 1) {
        lc = (lmax + lmin) / 2;
        idx = findBellogingIntervalsByLevel(p, lc);
        auto iterator = hashTable.find(std::make_pair(lc, idx));
        if (iterator != hashTable.end()) { // node does exists
            lmin = lc;
            // change range to [lc, lmax]
        } else {// node does not exists
            lmax = lc - 1;
            // change range to [lmin, lc - 1]

        }
    }

    lc = (lmax + lmin) / 2;
    idx = findBellogingIntervalsByLevel(p, lc);
    auto iterator = hashTable.find(std::make_pair(lc, idx));
#if 0
    if (iterator == hashTable.end()) {

            std::cerr << std::format("not found: ({},{},{})", p.x, p.y, p.z) << std::endl;
            return {0, 0, 0};
        }
#endif

    // printNodePoints(iterator->second);
    return findClosesPointInNode(p, iterator->second); // find closes point in selected mode
}

Point HashOctree::findClosesPointInNode(Point &p, const OctrerNodeBuilder *node) {
    double smallestDistance = std::numeric_limits<double>::max();
    Point *closesPoint;
    for (auto &i: node->voronoiCells) {
        auto distance = i->p.distance(p);
        if (smallestDistance > distance) {
            smallestDistance = distance;
            closesPoint = &i->p;
        }
    }
    // std::cerr << std::format("for ({},{},{}), found: ({},{},{}), distance: {}", p.x,p.y,p.z, closesPoint->x, closesPoint->y, closesPoint->z,smallestDistance) << std::endl;
    return *closesPoint;
}


void HashOctree::printHashTable() {

    for (auto &[key, value]: hashTable) {
        std::cout
                << std::format("({},({},{},{})) ---> ( \n", key.first, std::get<0>(key.second), std::get<1>(key.second),
                               std::get<2>(key.second));
        printNodePoints(value);
        //  std::cout << std::format("{:2f},{:2f},{:2f}", value->border.min.x, value->border.min.y, value->border.min.z);
        std::cout << " )" << std::endl;
    }

}

void HashOctree::printNodePoints(OctrerNodeBuilder *value) {
    for (auto j: value->voronoiCells) {
        std::cout << std::format("      ({:.2f},{:.2f},{:.2f})", j->p.x, j->p.y, j->p.z);
        std::cout << "\n";


    }
}

std::vector<OctrerNodeBuilder *> HashOctree::getLeafs() {
    std::vector<OctrerNodeBuilder *> leafs;
    root->getLeafs(leafs);
    return leafs;
}

std::set<OctrerNodeBuilder *> HashOctree::getAllNodes() {
    std::set<OctrerNodeBuilder *> allNodes;
    root->getAllNodes(allNodes);

    return allNodes;
}












