#ifndef RLS_TSP_SOLVER_H
#define RLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class RLSSolver : public LSSolver
{
public:
    TPaths solve();
private:
    void generate_fully_random_solution(default_random_engine);
};

#endif