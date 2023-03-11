#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include <vector>

#include "types.h"

using namespace std;

class TSPSolver
{
public:
    virtual TPaths solve(int start_vertex) = 0;
    void load_data(int [][N]);
    void add_vertex_to_path(int, int);
    int find_furthest_vertex(int vertex_id);
    int find_nearest_vertex(int vertex_id);

protected:
    int distance_matrix[N][N];
    bool used_vertices[N] = { false };
    TPaths paths;
    TPathLength path_length;
};

#endif
