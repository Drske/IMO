#include "nn_tsp_solver.h"

TPaths NNSolver::solve(int start_vertex){
    printf("Solving nearest neighbour\n");

    int start_vertex2 = this->find_furthest_vertex(start_vertex);
    
    this->add_vertex_to_path(1, start_vertex);
    this->add_vertex_to_path(2, start_vertex2);

    this->path_cost.first = 0;
    this->path_cost.second = 0;

    while (this->path_length.first + this->path_length.second < N) {
        for (int i = 0; i < 2; i++) {
            vector<int> path = (i == 0) ? this->paths.first : this->paths.second;
            int cost = (i == 0) ? this->path_cost.first : this->path_cost.second;
            int path_length = (i == 0) ? this->path_length.first : this->path_length.second;

            int best_vertex = -1;
            int best_cost = INT_MAX;
            int best_placement = -1;

            for (int j = 0; j < N; j++) {
                if (this->used_vertices[j])
                    continue;

                for (int k = 0; k <= path_length; k++) {
                    int new_cost;

                    if (k == 0) {
                        // Cost of adding new vertex at the start of the current path
                        new_cost = this->distance_matrix[j][path[k]] + cost;
                    } else if (k == path_length) {
                        // Cost of adding new vertex at the end of the current path
                        new_cost = cost + this->distance_matrix[j][path[k-1]];
                    } else {
                        // Cost of adding new vertex in the middle of the current path
                        int v1 = path[k-1]; 
                        int v2 = path[k];
                        new_cost = cost - this->distance_matrix[v1][v2] + this->distance_matrix[v1][j] + this->distance_matrix[j][v2];
                    }

                    if (new_cost < best_cost) {
                        best_vertex = j;
                        best_cost = new_cost;
                        best_placement = k;
                    }
                }
            }

            // Add new vertex at the best position found
            this->add_vertex_to_path(i + 1, best_vertex, best_placement);

            // Update cost
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
