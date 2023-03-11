#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include <vector>

#include "types.h"

using namespace std;

class TSPSolver
{
public:
    virtual TPaths solve() = 0;
    void load_data(int [][N]);
    void add_vertex_to_path(int, int);

protected:
    int distance_matrix[N][N];
    TPaths paths;
    TPathLength path_length;
};

#endif
