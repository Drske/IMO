#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "../types.h"
#include <string>

using namespace std;

class TSPSolver
{
public:
    virtual TPaths solve() = 0;
    
    void load_data(int [][N]);
    void add_vertex_to_path(int path_no, int vertex_id, int index = -1);
    int find_furthest_vertex(int vertex_id);
    int find_nearest_vertex(int vertex_id);
    

    void set_iterations(int);
    void set_neighbourhood(string);
    void set_start_vertex(int);
    void set_initial_solution(TPaths);
    void set_initial_cost(TPathCost);
    void set_max_candidates(int);

    void set_local_search_solver(TSPSolver*);
    void set_init_sol_gen(TSPSolver*);
    void set_constructive_solver(TSPSolver*);

    TPathCost get_cost();

protected:
    int iterations;
    string neighbourhood;
    int start_vertex;
    int distance_matrix[N][N];
    bool used_vertices[N] = { false };
    int max_candidates;
    TPaths paths;
    TPathLength path_length;
    TPathCost path_cost;
    TSPSolver* local_search_solver;
    TSPSolver* init_sol_gen;
    TSPSolver* constructive_solver;
};

#endif
