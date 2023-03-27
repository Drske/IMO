#ifndef GC_TSP_SOLVER_H
#define GC_TSP_SOLVER_H

#include "tsp_solver.h"

class GCSolver : public TSPSolver
{
    TPaths solve(int start_vertex);
};

#endif
