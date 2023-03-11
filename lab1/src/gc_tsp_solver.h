#ifndef GC_TSP_SOLVER
#define GC_TSP_SOLVER

#include "tsp_solver.h"

class GCSolver : public TSPSolver
{
public:
    TPaths solve();
};

#endif
