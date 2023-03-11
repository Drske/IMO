#include "nn_tsp_solver.h"

TPaths NNSolver::solve(int start_vertex){
    printf("Solving nearest neighbour\n");

    int start_vertex2 = this->find_furthest_vertex(start_vertex);
    
    this->add_vertex_to_path(1, start_vertex);
    this->add_vertex_to_path(2, start_vertex2);

    int path_costs[2] = {0, 0};

    while (this->path_length.first + this->path_length.second < 100) {
        for (int i = 0; i < 2; i++) {
            int* path = (i == 0) ? this->paths.first : this->paths.second;
            int cost = path_costs[i];
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

            // Add new vertex to the best placement
            if (best_placement == path_length)
                this->add_vertex_to_path(i + 1, new_vertex);
            else
                // TODO

            path_costs[i] = best_cost;
        }
    }

    return this->paths;
}
