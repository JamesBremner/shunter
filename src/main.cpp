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

// The problem instance
// stores the specifications and the solution for the current problem
struct sProblemInstance
{
    // specs
    raven::graph::sGraphData gd;     // rail network
    std::vector<int> init_positions; // initial train positions
    std::vector<int> curr_positions; // current train positions
    std::vector<int> dest_positions; // destination train positions

    // results
    std::vector<std::vector<int>> paths; // optimum path for each train
    std::vector<int> order;              // order trains are to be shunted
} gPI;

// generate problem 1 ( based on example in https://stackoverflow.com/q/76954728/16582 )

void generate1()
{
    // rail network
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

    // init_positions[N] == M specifies trainN in stationM
    std::vector<int> init_positions{
        0, 0, 2};
    gPI.init_positions = init_positions;

    // init_positions[N] == M specifies trainN moves to statitionM
    std::vector<int> dest_positions{
        1, 4, 5};
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

/// @brief Move train from initial to destionation position, if possible
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

cGUI::cGUI()
    : cStarterGUI(
          "Shunter",
          {50, 50, 1000, 500})
{
    fm.events().draw(
        [&](PAINTSTRUCT &ps)
        {
            wex::shapes S(ps);
            displayTrainPaths(S);
        });

    show();
    run();
}

void cGUI::displayTrainPaths(wex::shapes &S)
{
    int row = 0;
    for (int train : gPI.order)
    {
        S.text("train" + std::to_string(train) + ":",
               {20, 50 + 50 * row, 50, 25});
        std::string line;
        for (int station : gPI.paths[train])
        {
            line += gPI.gd.g.userName(station) + " ";
        }
        S.text(line, {80, 50 + 50 * row, 200, 25});
        row++;
    }
    for (int train = 0; train < gPI.init_positions.size(); train++)
        if (!gPI.paths[train].size())
        {
            S.text("train" + std::to_string(train) + ": blocked",
                   {20, 50 + 50 * row, 200, 25});
            row++;
        }
}

main()
{
    generate1();
    // MoveNoStationsBlocked();
    MoveStationBlockedOnebyOne();

    cGUI theGUI;
    return 0;
}
