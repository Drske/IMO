#ifndef RLS_TSP_SOLVER_H
#define RLS_TSP_SOLVER_H

#include "tsp_solver.h"

class RLSSolver : public TSPSolver
{
    TPaths solve(TPaths start_solution);
};

#endif