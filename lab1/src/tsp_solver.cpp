#include "tsp_solver.h"

void TSPSolver::load_data(int distance_matrix[][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            this->distance_matrix[i][j] = distance_matrix[i][j];
        }
    }
}

void TSPSolver::add_vertex_to_path(int path_no, int vertex_id){
    if (path_no == 1){
        this->paths.first[path_length.first] = vertex_id;
        this->path_length.first += 1;
    }
    if (path_no == 2){
        this->paths.second[path_length.second] = vertex_id;
        this->path_length.second += 1;
    }
}

int TSPSolver::find_furthest_vertex(int vertex_id) {
    int furthest_vertex_id = -1;
    int max_distance = -1;

    for (int i = 0; i < N; i++) {
        if (this->used_vertices[i])
            continue;

        int distance = this->distance_matrix[vertex_id][i];

        if (distance > max_distance) {
            furthest_vertex_id = i;
            max_distance = distance;
        }
    }

    this->used_vertices[furthest_vertex_id] = true;

    return furthest_vertex_id;
}

int TSPSolver::find_nearest_vertex(int vertex_id) {
    int nearest_vertex_id = -1;
    int min_distance = INT_MAX;

    for (int i = 0; i < N; i++) {
        if (this->used_vertices[i])
            continue;

        int distance = this->distance_matrix[vertex_id][i];

        if (distance < min_distance) {
            nearest_vertex_id = i;
            min_distance = distance;
        }
    }

    this->used_vertices[nearest_vertex_id] = true;

    return nearest_vertex_id;
}
