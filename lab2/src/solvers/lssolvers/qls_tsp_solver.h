#ifndef QLS_TSP_SOLVER_H
#define QLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class QLSSolver : public LSSolver
{
    TPaths solve();
};

#endif