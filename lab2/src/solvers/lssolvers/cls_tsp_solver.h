#ifndef CLS_TSP_SOLVER_H
#define CLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class CLSSolver : public LSSolver
{
    TPaths solve();
};

#endif