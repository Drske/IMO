#ifndef GLS_TSP_SOLVER_H
#define GLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class GLSSolver : public LSSolver
{
    TPaths solve();
};

#endif