#ifndef NN_TSP_SOLVER
#define NN_TSP_SOLVER

#include "tsp_solver.h"

class NNSolver : public TSPSolver
{
public:
    TPaths solve();
};

#endif
