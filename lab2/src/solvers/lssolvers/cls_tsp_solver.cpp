#include <algorithm>
#include <random>
#include <chrono>

#include "cls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;
using namespace std::chrono;

TPaths CLSSolver::solve()
{
    printf("Solving Steepest Local Search with Candidate Moves\n");

    vector<Move *> (*get_moves)(TPaths, int[][N], int) = nullptr;
    get_moves = MoveGenerator::get_second_neighbourhood_candidate_moves;

    bool applied = true;
    Move *best_move;
    TPathCost best_delta;

    while (applied)
    {
        applied = false;
        best_delta = make_pair(INT_MAX / 2, INT_MAX / 2);

        vector<Move *> moves = get_moves(this->paths, this->distance_matrix, this->max_candidates);

        for (vector<Move *>::iterator it = moves.begin(); it != moves.end(); it++)
        {
            TPathCost delta = (*it)->get_cost_delta(this->paths, this->distance_matrix);
            if (delta.first + delta.second < best_delta.first + best_delta.second)
            {
                best_delta = delta;
                best_move = (*it);
            }
        }

        if (best_delta.first + best_delta.second < 0)
        {
            best_move->apply(this->paths);
            this->path_cost.first += best_delta.first;
            this->path_cost.second += best_delta.second;
            applied = true;
        }
    }

    return this->paths;
}