#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "types.h"

using namespace std;

class TSPSolver
{
public:
    virtual TPaths solve() {return this->paths;}
    virtual TPaths solve(int start_vertex) {return this->paths;}
    virtual TPaths solve(TPaths start_solution) {return this->paths;}
    
    void load_data(int [][N]);
    void add_vertex_to_path(int path_no, int vertex_id, int index = -1);
    int find_furthest_vertex(int vertex_id);
    int find_nearest_vertex(int vertex_id);
    TPathCost get_cost();
    void set_iterations(int);

protected:
    int iterations;
    int distance_matrix[N][N];
    bool used_vertices[N] = { false };
    TPaths paths;
    TPathLength path_length;
    TPathCost path_cost;
};

#endif
