#ifndef RE_TSP_SOLVER
#define RE_TSP_SOLVER

#include "tsp_solver.h"

class RESolver : public TSPSolver
{
public:
    TPaths solve();
};

#endif
