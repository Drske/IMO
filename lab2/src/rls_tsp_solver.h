#ifndef RLS_TSP_SOLVER_H
#define RLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class RLSSolver : public LSSolver
{
    TPaths solve(TPaths start_solution);
};

#endif