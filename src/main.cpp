#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cGUI.h"

// the graph theory library used for breadth first searching https://github.com/JamesBremner/PathFinder
#include "GraphTheory.h"

#include "shunter.h"

// construct global problem instance
sProblemInstance gPI;

// generate problem 1 ( based on example in https://stackoverflow.com/q/76954728/16582 )

void generateLinks1()
{
    gPI.gd.g.clear();
    gPI.gd.g.add("0", "1");
    gPI.gd.g.add("1", "2");
    gPI.gd.g.add("2", "3");
    gPI.gd.g.add("3", "4");
    gPI.gd.g.add("2", "5");
    gPI.gd.g.add("2", "6");
    gPI.gd.g.add("2", "7");
    gPI.gd.g.add("1", "8");
    gPI.gd.g.add("4", "6");
}

void generate1()
{
    // rail network
    generateLinks1();

    // init_positions[N] == M specifies trainN in stationM
    std::vector<int> init_positions{
        0, 0, 2};
    gPI.init_positions = init_positions;

    // init_positions[N] == M specifies trainN moves to statitionM
    std::vector<int> dest_positions{
        1, 4, 5};
    gPI.dest_positions = dest_positions;
}

void generate2()
{
    // rail network - same as for problem1
    generateLinks1();

        // init_positions[N] == M specifies trainN in stationM
    std::vector<int> init_positions{
        0};
    gPI.init_positions = init_positions;

    // init_positions[N] == M specifies trainN moves to statitionM
    std::vector<int> dest_positions{
        3};
    gPI.dest_positions = dest_positions;
}

// Find train paths from initial to destination positions, ignoring blocked station
void MoveNoStationsBlocked()
{
    gPI.paths.clear();

    // loop over trains in problem instance
    for (int train = 0; train < gPI.init_positions.size(); train++)
    {
        // find shortest path using breadth first search
        gPI.gd.startName = gPI.gd.g.userName(gPI.init_positions[train]);
        gPI.gd.endName = gPI.gd.g.userName(gPI.dest_positions[train]);
        gPI.paths.push_back(bfsPath(gPI.gd));
    }
}

/// @brief Move train from initial to destination position, if possible
/// @param train
/// @return vector of station indices on path, empty if destination unreachabe

std::vector<int> MoveStationBlocked(int train)
{
    // construct subgraph without blocked stations
    raven::graph::sGraphData sub;
    sub = gPI.gd;

    // loop over stationary trains
    for (int blockTrain = 0; blockTrain < gPI.init_positions.size(); blockTrain++)
    {
        // check if train is moving
        if (blockTrain == train)
            continue;

        // check if station occupied by train is initial or destination of moving train
        int blockedStation = gPI.curr_positions[blockTrain];
        if (blockedStation == gPI.init_positions[train] ||
            blockedStation == gPI.dest_positions[train])
            continue;

        // remove links to blocked station
        auto vl = sub.g.edgeList();
        for (auto &l : vl)
        {
            if (l.first == blockedStation || l.second == blockedStation)
                sub.g.remove(l.first, l.second);
        }
    }

    // find path for train from start to destination in graph without blocked stations
    sub.startName = sub.g.userName(gPI.init_positions[train]);
    sub.endName = sub.g.userName(gPI.dest_positions[train]);
    return bfsPath(sub);
}

void MoveStationBlockedOnebyOne()
{
    gPI.paths.clear();
    gPI.paths.resize(gPI.init_positions.size());
    gPI.order.clear();

    // trains start at initial positions
    gPI.curr_positions = gPI.init_positions;

    // loop until either all trains shunted or all unshunted trains are blocked

    int waiting = gPI.init_positions.size(); // trains waiting to be shunted
    bool success = true;
    while (waiting > 0 && success)
    {
        success = false;

        // loop over trains in problem instance
        for (int train = 0; train < gPI.init_positions.size(); train++)
        {
            // check if train has been shunted
            if (gPI.paths[train].size())
                continue;

            // check if train can be shunted from start to destination
            auto path = MoveStationBlocked(train);
            if (!path.size())
                continue;

            // train succesfully shunted to destination
            gPI.paths[train] = path;
            gPI.curr_positions[train] = gPI.dest_positions[train];
            gPI.order.push_back(train);
            success = true;
            waiting--;
        }
    }
}

main()
{
    generate1();
    MoveStationBlockedOnebyOne();

    cGUI theGUI;
    return 0;
}
