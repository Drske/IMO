#ifndef EV_TSP_SOLVER_H
#define EV_TSP_SOLVER_H

#include <string>

#include "../tsp_solver.h"

using namespace std;

class EVSolver : public TSPSolver
{
public:
    virtual TPaths solve() = 0;
protected:
    int population_max_size;
    TSolutions population;
};

#endif