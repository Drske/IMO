#include "tsp_solver.h"

TPaths TSPSolver::solve(){
    return this->paths;
}

void TSPSolver::load_data(int distance_matrix[][N]){
    for (int i = 0; i <= N; i++){
        for (int j = 0; j <= N; j++){
            this->distance_matrix[i][j] = distance_matrix[i][j];
        }
    }
}

void TSPSolver::add_path(int from, int to){
    this->paths.push_back(make_pair(from, to));
    this->no_of_paths += 1;
}
