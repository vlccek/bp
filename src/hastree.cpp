//
// Created by jvlk on 27.9.23.
//

#include "hastree.h"

HashOctree::HashOctree(std::vector<Point> &p, const Point &min, const Point &max, int threads)
        : min(min), max(max),
          con(min.x, max.x, min.y, max.y, min.z, max.z, 160, 160, 160, false, false, false, 8, threads) {
    root = new OctrerNodeBuilder(0, &maxLevel);


    std::cout << std::format("HashOctree with {}", threads) << std::endl;
    pointCount = (p.size());

    int id = 0;
    for (auto &i: p) {
        con.put(id++, i.x, i.y, i.z);
    }
        voro::container_3d::iterator cli;

    start = chrono::high_resolution_clock::now();

    voronoiCells.resize(pointCount);
#pragma omp parallel
    {
        voro::voronoicell_3d c(con);
        double x, y, z;
#pragma omp for
        for (cli = con.begin(); cli < con.end(); cli++) {
            if (con.compute_cell(c, cli)) {
                con.pos(cli, x, y, z);
                Point po(x, y, z);
                auto poly = Polyhedron(c, po);


                int index = cli-con.begin();
                voronoiCells[index] = poly;

            }
        }
    }

    voroBuild = chrono::high_resolution_clock::now();
    auto duration = duration_cast<chrono::milliseconds>(voroBuild - start);
    cout << "Voronoi finish: " << duration.count() << "ms" << endl;

    // saveVoroCellToFile(con);
    initTree();

    buildTree();

    treeBuild = chrono::high_resolution_clock::now();
    duration = duration_cast<chrono::milliseconds>(treeBuild - voroBuild);
    cout << "tree finish: " << duration.count() << "ms" << endl;


    buildHashTable();


    htBuild = chrono::high_resolution_clock::now();
    duration = duration_cast<chrono::milliseconds>(htBuild - treeBuild);
    cout << "HT finish: " << duration.count() << "ms" << endl;
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
    std::set<OctrerNodeBuilder *> allNodes = {root};
    root->getAllNodes(allNodes);
    hashTable.rehash(allNodes.size());

    std::vector<OctrerNodeBuilder *> nodes(allNodes.begin(), allNodes.end());
    std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctrerNodeBuilder *> hashTableThreads[omp_get_max_threads()];
#pragma omp parallel
    {
        std::tuple<int, int, int> p;

        std::unordered_map<std::pair<int, std::tuple<int, int, int>>, OctrerNodeBuilder *> &hashTableThread = hashTableThreads[omp_get_thread_num()];
        //hashTableThread.reserve(allNodes.size());
        // https://en.cppreference.com/w/cpp/container/unordered_map/load_factor
        // zkusit realoakace

#pragma omp for
        for (OctrerNodeBuilder* i: nodes) {
            p = findBellogingIntervalsByLevel(i->border.center(), i->level);
            auto pair = std::make_pair(i->level, p);

            // hashTable[std::make_pair(i->level, p)] = i;
            hashTableThread.emplace(pair, i);
        }
#pragma omp critical
        {
            hashTable.merge(hashTableThread);
        }
    }
}


Point HashOctree::nn(Point &p) {
#if 0
    if (maxLevel < 2) { // todo add connection to max number of points in node
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
    float smallestDistance = std::numeric_limits<float>::max();
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

void HashOctree::saveVoroCellToFile(voro::container_3d &con) {
    // Output the particle positions in gnuplot format
    con.draw_particles("random_points_p.gnu");

    // Output the Voronoi cells in gnuplot format
    con.draw_cells_gnuplot("random_points_v.gnu");

    //splot 'random_points_p.gnu' u 2:3:4 with points, 'random_points_v.gnu' with lines

}












