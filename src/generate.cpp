#include <fstream>
#include "GraphTheory.h"
#include "shunter.h"

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

// generate problem 1 ( based on example in https://stackoverflow.com/q/76954728/16582 )
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

void readtrainfile(const std::string &fname)
{
    // rail network - same as for problem1
    generateLinks1();

    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open " + fname);
    int start, end;

    ifs >> start >> end;
    while (ifs.good())
    {
        gPI.init_positions.push_back(start);
        gPI.dest_positions.push_back(end);
        ifs >> start >> end;
    }
}
void readstationfile( const std::string& fname )
{
    // rail network - same as for problem1
    generateLinks1();

    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open " + fname);
    int start, end;

    int station = -1;
    ifs >> start >> end;
    while (ifs.good())
    {
        station++;

        if( start == -1 && end == -1 ) {
            // ignore stations that are empty at start and beginning
            continue;
        }
        if( start > -1)
        {
            if( start > gPI.init_positions.size()-1)
                 gPI.init_positions.resize(start,-1);
            gPI.init_positions[start] = station;
        }
        if( end > -1 )
        {
            if( start > gPI.dest_positions.size()-1)
                 gPI.dest_positions.resize(end,-1);
            gPI.dest_positions[start] = station;
        } 
    }
}