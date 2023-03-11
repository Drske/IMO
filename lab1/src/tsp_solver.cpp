#include "tsp_solver.h"

void TSPSolver::load_data(int distance_matrix[][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            this->distance_matrix[i][j] = distance_matrix[i][j];
        }
    }
}

void TSPSolver::add_indice_to_path(int path_no, int indice_id){
    if (path_no == 1){
        this->paths.first[path_length.first] = indice_id;
        this->path_length.first += 1;
    }
    if (path_no == 2){
        this->paths.second[path_length.second] = indice_id;
        this->path_length.second += 1;
    }
}
