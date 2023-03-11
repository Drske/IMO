#include "gc_tsp_solver.h"

TPaths GCSolver::solve(int start_vertex){
    printf("Solving greedy cycle\n");

    int start_vertex2 = this->find_furthest_vertex(start_vertex);
    
    // Create cycles
    this->add_vertex_to_path(1, start_vertex);
    this->add_vertex_to_path(1, this->find_nearest_vertex(start_vertex));
    this->add_vertex_to_path(2, start_vertex2);
    this->add_vertex_to_path(2, this->find_nearest_vertex(start_vertex2));

    int path_costs[2] = {
        this->distance_matrix[this->paths.first[0]][this->paths.first[1]],
        this->distance_matrix[this->paths.second[0]][this->paths.second[1]]
    };

    while (this->path_length.first + this->path_length.second < 100) {
        for (int i = 0; i < 2; i++) {
            int* path = (i == 0) ? this->paths.first : this->paths.second;
            int cost = path_costs[i];
            int path_length = (i == 0) ? this->path_length.first : this->path_length.second;

            int best_vertex = -1;
            int best_cost = INT_MAX;
            int best_placement = -1;

            for (int j = 0; j < N; j++) {
                if (this->used_vertices[j])
                    continue;

                for (int k = 1; k <= path_length; k++) {
                    int v1, v2, new_cost;

                    if (k == path_length) {
                        v1 = path[path_length-1];
                        v2 = path[0];

                        if (cost == 0)
                            new_cost = cost + this->distance_matrix[v1][j] + this->distance_matrix[j][v2];
                        else
                            new_cost = cost - this->distance_matrix[v1][v2] + this->distance_matrix[v1][j] + this->distance_matrix[j][v2];
                    } else {
                        v1 = path[k-1];
                        v2 = path[k];
                        new_cost = cost - this->distance_matrix[v1][v2] + this->distance_matrix[v1][j] + this->distance_matrix[j][v2];
                    }

                    if (new_cost < best_cost) {
                        best_vertex = j;
                        best_cost = new_cost;
                        best_placement = k;
                    }
                }
            }

            if (best_placement == path_length)
                this->add_vertex_to_path(i + 1, best_vertex);
            else
                // TODO

            path_costs[i] = best_cost;
        }
    }

    return this->paths;
}
