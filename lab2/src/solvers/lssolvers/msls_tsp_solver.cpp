#include <algorithm>
#include <random>

#include "../../types.h"
#include "msls_tsp_solver.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

TPaths MSLSSolver::solve()
{
    printf("Solving Multiple Start Local Search\n");

    this->path_cost = make_pair(INT_MAX / 2, INT_MAX / 2);

    for (int j = 0; j < this->iterations; j++) {
        this->init_sol_gen->clear_paths();
        this->init_sol_gen->set_path_length(make_pair(0, 0));
        this->init_sol_gen->set_initial_cost(make_pair(0, 0));
        this->init_sol_gen->set_start_vertex(this->start_vertex + j);

        TPaths initial_solution = this->init_sol_gen->solve();
        TPathCost initial_cost = this->init_sol_gen->get_cost();
        TPathLength initial_length = this->init_sol_gen->get_path_length();

        this->local_search_solver->set_initial_solution(initial_solution);
        this->local_search_solver->set_initial_cost(initial_cost);
        
        this->local_search_solver->set_path_length(initial_length);

        TPaths local_search_solution = this->local_search_solver->solve();
        TPathCost local_search_cost = this->local_search_solver->get_cost();

        if (local_search_cost.first + local_search_cost.second < this->path_cost.first + this->path_cost.second) {
            this->paths.first = local_search_solution.first;
            this->paths.second = local_search_solution.second;

            this->path_cost.first = local_search_cost.first;
            this->path_cost.second = local_search_cost.second;
        }
    }

    return this->paths;
}