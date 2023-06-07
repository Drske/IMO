#ifndef SHEV_TSP_SOLVER_H
#define SHEV_TSP_SOLVER_H

#include "ev_tsp_solver.h"

class SHEVSolver : public EVSolver
{
public:
    TPaths solve();
private:
    void initialise_population();
    TSolutions select_two_random_parents(TSolutions);

    TSolution recombine(TSolution, TSolution);
    void gather_edges(TPath, vector<pair<int, int>> &);

    TSolution mutate(TSolution);
    TSolution swap_random_vertices(TSolution);
    
    TSolutions pass_best_solution(TSolutions, TSolution);
};

#endif