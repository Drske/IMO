#ifndef RE_TSP_SOLVER_H
#define RE_TSP_SOLVER_H

#include "tsp_solver.h"

class RESolver : public TSPSolver
{
public:
    TPaths solve(int start_vertex);
private:
    TInsertionCosts calculate_insertion_costs(vector<int>, int);
};

#endif
