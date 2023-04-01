#include <numeric>
#include <algorithm>
#include <random>

#include "rls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

void RLSSolver::generate_fully_random_solution(default_random_engine rnd_e)
{
    vector<int> vertices(N);
    iota(vertices.begin(), vertices.end(), 0);
    shuffle(vertices.begin(), vertices.end(), rnd_e);

    this->add_vertex_to_path(1, vertices[0]);
    this->add_vertex_to_path(2, vertices[0 + N / 2]);

    for (int i = 1; i < N / 2; i++)
    {
        this->add_vertex_to_path(1, vertices[i]);
        this->add_vertex_to_path(2, vertices[i + N / 2]);
        this->path_cost.first += distance_matrix[vertices[i - 1]][vertices[i]];
        this->path_cost.second += distance_matrix[vertices[i - 1 + N / 2]][vertices[i + N / 2]];
    }

    this->path_cost.first += distance_matrix[this->paths.first[0]][this->paths.first[N / 2 - 1]];
    this->path_cost.first += distance_matrix[this->paths.first[N / 2]][this->paths.first[N - 1]];
}

TPaths RLSSolver::solve()
{
    printf("Solving Random Walk Local Search\n");

    vector<Move *> (*get_moves)(TPaths) = nullptr;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rnd_e(seed);

    if (this->path_cost.first == 0 && this->path_cost.second == 0)
    {
        this->generate_fully_random_solution(rnd_e);
    }

    if (this->neighbourhood == "N1")
    {
        get_moves = MoveGenerator::get_first_neighbourhood_moves;
    }
    else if (this->neighbourhood == "N2")
    {
        get_moves = MoveGenerator::get_second_neighbourhood_moves;
    }

    TPaths best_solution = this->paths;
    TPathCost best_cost = this->path_cost;

    for (int i = 1; i < this->iterations; i++)
    {
        vector<Move *> moves = get_moves(this->paths);
        shuffle(moves.begin(), moves.end(), rnd_e);

        TPathCost delta = moves[0]->get_cost_delta(this->paths, this->distance_matrix);
        moves[0]->apply(this->paths);
        this->path_cost.first += delta.first;
        this->path_cost.second += delta.second;

        if (this->path_cost.first + this->path_cost.second < best_cost.first + best_cost.second)
        {
            best_solution = this->paths;
            best_cost = this->path_cost;
        }
    }

    this->paths = best_solution;
    this->path_cost = best_cost;

    return this->paths;
}