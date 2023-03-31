#ifndef SLS_TSP_SOLVER_H
#define SLS_TSP_SOLVER_H

#include "tsp_solver.h"

class SLSSolver : public TSPSolver
{
    TPaths solve(TPaths start_solution);
};

#endif