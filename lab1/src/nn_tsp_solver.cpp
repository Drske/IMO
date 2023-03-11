#include "nn_tsp_solver.h"

TPaths NNSolver::solve(int start_vertex){
    printf("Solving nearest neighbour\n");

    int start_vertex2 = this->find_furthest_vertex(start_vertex);
    
    this->add_vertex_to_path(1, start_vertex);
    this->add_vertex_to_path(2, start_vertex2);

    this->path_cost.first = 0;
    this->path_cost.second = 0;

    while (this->path_length.first + this->path_length.second < 100) {
        for (int i = 0; i < 2; i++) {
            vector<int> path = (i == 0) ? this->paths.first : this->paths.second;
            int cost = (i == 0) ? this->path_cost.first : this->path_cost.second;
            int path_length = (i == 0) ? this->path_length.first : this->path_length.second;

            // Find nearest vertex
            int new_vertex = this->find_nearest_vertex(path[path_length-1]);
            
            // Find best placement for new vertex
            int best_cost = cost + this->distance_matrix[path[path_length-1]][new_vertex];
            int best_placement = path_length;

            for (int j = 1; j < path_length; j++) {
                int v1 = path[j-1];
                int v2 = path[j];
                int new_cost = cost - this->distance_matrix[v1][v2] + this->distance_matrix[v1][new_vertex] + this->distance_matrix[new_vertex][v2];

                if (new_cost < best_cost) {
                    best_cost = new_cost;
                    best_placement = j;
                }
            }

            this->add_vertex_to_path(i + 1, new_vertex, best_placement);

            if (i == 0)
                this->path_cost.first = best_cost;
            else
                this->path_cost.second = best_cost;
        }
    }

    this->path_cost.first += this->distance_matrix[this->paths.first.back()][this->paths.first.front()];
    this->path_cost.second += this->distance_matrix[this->paths.second.back()][this->paths.second.front()];

    return this->paths;
}
