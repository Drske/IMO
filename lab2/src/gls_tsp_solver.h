#ifndef GLS_TSP_SOLVER_H
#define GLS_TSP_SOLVER_H

#include "tsp_solver.h"

class GLSSolver : public TSPSolver
{
    TPaths solve(TPaths start_solution);
};

#endif