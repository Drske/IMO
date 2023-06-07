#ifndef DRLS_TSP_SOLVER_H
#define DRLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class DRLSSolver : public LSSolver
{
public:
    TPaths solve();

private:
    TPaths destroy();
    TSolution repair(TPaths);
    TPaths remove_path_from_cycle(TPaths, int, int, int);
    TPaths close_cycles(TPaths);
};

#endif