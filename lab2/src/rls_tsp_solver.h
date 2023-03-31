#ifndef RLS_TSP_SOLVER_H
#define RLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class RLSSolver : public LSSolver
{
    public:
    TPaths solve();
    void set_iterations(int);

    private:
    int iterations;
};

#endif