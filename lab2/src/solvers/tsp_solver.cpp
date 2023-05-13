#include <string>
#include "tsp_solver.h"
using namespace std;

void TSPSolver::load_data(int distance_matrix[][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            this->distance_matrix[i][j] = distance_matrix[i][j];
        }
    }
}

TPathCost TSPSolver::get_cost(){
    return this->path_cost;
}

void TSPSolver::set_iterations(int iterations){
    this->iterations = iterations;
}

void TSPSolver::set_neighbourhood(string neighbourhood){
    this->neighbourhood = neighbourhood;
}

void TSPSolver::set_start_vertex(int start_vertex){
    this->start_vertex = start_vertex;
}

void TSPSolver::set_initial_solution(TPaths initial_solution){
    this->paths = initial_solution;
}

void TSPSolver::set_initial_cost(TPathCost intitial_solution_cost){
    this->path_cost = intitial_solution_cost;
}

void TSPSolver::set_max_candidates(int no){
    this->max_candidates = no;
}

void TSPSolver::set_local_search_solver(TSPSolver* solver){
    this->local_search_solver = solver;
}

void TSPSolver::set_init_sol_gen(TSPSolver* solver){
    this->init_sol_gen = solver;
}

void TSPSolver::set_constructive_solver(TSPSolver* solver){
    this->constructive_solver = solver;
}

void TSPSolver::add_vertex_to_path(int path_no, int vertex_id, int index){
    if (path_no == 1) {
        if ((index == -1) || index == this->path_length.first)
            this->paths.first.push_back(vertex_id);
        else
            this->paths.first.insert(this->paths.first.begin() + index, vertex_id);

        this->path_length.first += 1;
    }
    if (path_no == 2){
        if ((index == -1) || index == this->path_length.second)
            this->paths.second.push_back(vertex_id);
        else
            this->paths.second.insert(this->paths.second.begin() + index, vertex_id);

        this->path_length.second += 1;
    }

    this->used_vertices[vertex_id] = true;
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

    return nearest_vertex_id;
}
