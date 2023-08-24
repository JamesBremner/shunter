
// declare the application gui 
#include "cGUI.h"

// the graph theory library declarations
// used for breadth first searching 
// https://github.com/JamesBremner/PathFinder
#include "GraphTheory.h"

// shunter application declarations
#include "shunter.h"

// construct global problem instance
sProblemInstance gPI;

main()
{
    generate1();
    MoveStationBlockedOnebyOne();

    cGUI theGUI;
    return 0;
}
