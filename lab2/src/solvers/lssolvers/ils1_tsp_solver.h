#ifndef ILS1_TSP_SOLVER_H
#define ILS1_TSP_SOLVER_H

#include <random>
#include "ls_tsp_solver.h"

class ILS1Solver : public LSSolver
{
public:
    TPaths solve();
protected:
    TPathCost perturbance(TPaths &paths);
    pair<int, int> get_random_vertex_idxs(pair<int, int> path_ids, TPaths paths, mt19937 gen, uniform_int_distribution<> distr);
    TEdges get_random_edge_idxs(int path_id, TPaths paths, mt19937 gen, uniform_int_distribution<> distr);
    TPathCost random_vertex_swap(pair<int, int> path_ids, TPaths &paths, mt19937 gen, uniform_int_distribution<> distr);
    TPathCost random_edge_swap(int path_id, TPaths &paths, mt19937 gen, uniform_int_distribution<> distr);
};

#endif