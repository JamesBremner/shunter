// Dewclare the problem instance
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

/// Declare functions to generate problem instances

void generateLinks1();
void generate1();
void generate2();

/// @brief  read input file, one train per line
/// @param fname 
void readtrainfile( const std::string& fname );

/// @brief read input file, one station per line
/// @param fname 
void readstationfile( const std::string& fname );

// declare shunting functions

std::vector<int> MoveStationBlocked(int train);

void MoveStationBlockedOnebyOne();
