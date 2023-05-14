#include "ils1_tsp_solver.h"
#include "../../types.h"
#include "../../moves/vertex_move.h"
#include "../../moves/edge_move.h"
#include <vector>
using namespace std;

pair<int, int> ILS1Solver::get_random_vertex_idxs(pair<int, int> path_ids, TPaths paths, mt19937 gen, uniform_int_distribution<> distr) {
    int v1 = distr(gen);
    int v2 = distr(gen);

    if (v1 > v2)
        swap(v1, v2);

    if ((path_ids.first == path_ids.second) && (v2 - v1 < 2)) {
        v2 = (v2 + 2) % paths.first.size();
    }

    return make_pair(v1, v2);
}

TEdges ILS1Solver::get_random_edge_idxs(int path_id, TPaths paths, mt19937 gen, uniform_int_distribution<> distr) {
    int v11 = distr(gen);
    int v21 = distr(gen);

    if (v11 > v21)
        swap(v11, v21);

    if (v21 - v11 < 2) {
        v21 = (v21 + 3) % paths.first.size();
    }

    int v12 = (v11 == (int)paths.first.size() - 1) ? 0 : (v11 + 1);
    int v22 = (v21 == (int)paths.first.size() - 1) ? 0 : (v21 + 1);

    return make_pair(make_pair(v11, v12), make_pair(v21, v22));
}

TPathCost ILS1Solver::random_vertex_swap(pair<int, int> path_ids, TPaths &paths, mt19937 gen, uniform_int_distribution<> distr) {
    pair<int, int> rand_idxs = this->get_random_vertex_idxs(path_ids, paths, gen, distr);
    VertexMove move = VertexMove(path_ids, rand_idxs, make_pair(0, 0), make_pair(make_pair(0, 0), make_pair(0, 0)));
    TPathCost cost_delta = move.get_cost_delta(paths, this->distance_matrix);

    move.apply(paths);

    return cost_delta;
}

TPathCost ILS1Solver::random_edge_swap(int path_id, TPaths &paths, mt19937 gen, uniform_int_distribution<> distr) {
    TEdges rand_idxs = this->get_random_edge_idxs(path_id, paths, gen, distr);
    EdgeMove move = EdgeMove(path_id, rand_idxs, rand_idxs);
    TPathCost cost_delta = move.get_cost_delta(paths, this->distance_matrix);

    move.apply(paths);

    return cost_delta;
}

TPathCost ILS1Solver::perturbance(TPaths &paths) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, N / 2 - 1);

    TPathCost cost_delta = make_pair(0, 0);
    vector<TPathCost> perturbance_costs;

    for (int i = 0; i < 5; i++) {
        perturbance_costs.push_back(this->random_edge_swap(0, paths, gen, distr));
        perturbance_costs.push_back(this->random_edge_swap(1, paths, gen, distr));
    }

    for (int i = 0; i < 5; i++) {
        perturbance_costs.push_back(this->random_vertex_swap(make_pair(0, 1), paths, gen, distr));
    }

    for (TPathCost cost : perturbance_costs) {
        cost_delta.first += cost.first;
        cost_delta.second += cost.second;
    }

    return cost_delta;
}

TPaths ILS1Solver::solve()
{
    printf("Solving Iterated Local Search 1\n");
    this->paths = this->local_search_solver->solve();
    this->path_cost = this->local_search_solver->get_cost();

    // printf("Initital cost: %d %d\n", this->path_cost.first, this->path_cost.second);

    for (int i = 0; i < this->iterations; i++) {
        TPaths base_paths = this->paths;
        TPathCost base_cost = this->path_cost;
        TPathCost perturbance_cost = this->perturbance(base_paths);
        base_cost.first += perturbance_cost.first;
        base_cost.second += perturbance_cost.second;

        this->local_search_solver->set_initial_solution(base_paths);
        this->local_search_solver->set_initial_cost(base_cost);

        TPaths new_paths = this->local_search_solver->solve();
        TPathCost new_cost = this->local_search_solver->get_cost();

        if (new_cost.first + new_cost.second < this->path_cost.first + this->path_cost.second) {
            this->paths = new_paths;
            this->path_cost = new_cost;
        }
    }

    return this->paths;
}