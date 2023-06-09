#ifndef HEV_TSP_SOLVER_H
#define HEV_TSP_SOLVER_H

typedef pair<TPaths, TPathCost> TSolution;

#include "ev_tsp_solver.h"

class HEVSolver : public EVSolver
{
public:
    TPaths solve();

private:
    float pop_mutation_prob;
    TSolutions archive;

    void initialise_population();
    TSolutions select_parents(TSolutions, int, int);
    TSolutions recombine(TSolutions);
    TSolution recombine_one(TSolution, TSolution);
    void gather_edges(TPath, vector<pair<int, int>> &);
    TSolutions mutate(TSolutions, float);
    TSolution mutate_one(TSolution);
    // TSolution swap_random_vertices(TSolution);
    // TSolutions remove_duplicates(TSolutions);
    TSolutions pass_best_solution(TSolutions, TSolution);
    TSolutions select_next_generation(TSolutions, TSolutions, int);
};

#endif