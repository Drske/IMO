#ifndef MSLS_TSP_SOLVER_H
#define MSLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class MSLSSolver : public LSSolver
{
public:
    TPaths solve();
};

#endif