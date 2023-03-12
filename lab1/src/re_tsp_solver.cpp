#include "re_tsp_solver.h"

TPaths RESolver::solve(int start_vertex){
    printf("Solving with regrets\n");

    int start_vertex2 = this->find_furthest_vertex(start_vertex);
    
    this->add_vertex_to_path(1, start_vertex);
    this->add_vertex_to_path(2, start_vertex2);

    this->path_cost.first = 0;
    this->path_cost.second = 0;

    return this->paths;
}
