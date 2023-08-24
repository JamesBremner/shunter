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
};

extern sProblemInstance gPI;