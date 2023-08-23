#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"

// the graph theory library used for breadth first searching https://github.com/JamesBremner/PathFinder
#include "GraphTheory.h"

// The problem instance
// stores the specifications and the solution for the current problem
struct sProblemInstance
{
    raven::graph::sGraphData gd;         // rail network
    std::vector<int> init_positions;     // initial train positions
    std::vector<int> dest_positions;     // destination train positions
    std::vector<std::vector<int>> paths; // optimum path for each train
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

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Shunter",
              {50, 50, 1000, 500})
    {
        fm.events().draw(
            [&](PAINTSTRUCT &ps)
            {
                // display train paths

                wex::shapes S(ps);
                int row = 0;
                for (auto &path : gPI.paths)
                {
                    S.text( "train"+ std::to_string(row) + ":",
                        { 20,50 + 50 * row,50,25});
                    std::string line;
                    for (int station : path)
                    {
                        line += gPI.gd.g.userName(station) + " ";
                    }
                    S.text(line, {80, 50 + 50 * row, 200, 25});
                    row++;
                }
            });

        show();
        run();
    }

private:

};

main()
{
    generate1();
    MoveNoStationsBlocked();

    cGUI theGUI;
    return 0;
}
