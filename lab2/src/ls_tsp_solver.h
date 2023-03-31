#ifndef LS_TSP_SOLVER_H
#define LS_TSP_SOLVER_H

#include <string>

#include "tsp_solver.h"

using namespace std;

class LSSolver : public TSPSolver
{
public:
    virtual TPaths solve() = 0;
};

#endif