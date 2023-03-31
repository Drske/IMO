#ifndef LS_TSP_SOLVER_H
#define LS_TSP_SOLVER_H

#include "tsp_solver.h"

class LSSolver : public TSPSolver
{
    TPaths solve_greedy(TPaths start_solution);
    TPaths solve_steepest(TPaths start_solution);
};

#endif