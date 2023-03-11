#ifndef NN_TSP_SOLVER_H
#define NN_TSP_SOLVER_H

#include "tsp_solver.h"

class NNSolver : public TSPSolver
{
public:
    TPaths solve();
};

#endif
