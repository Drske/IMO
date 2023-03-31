#ifndef SLS_TSP_SOLVER_H
#define SLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class SLSSolver : public LSSolver
{
    TPaths solve();
};

#endif