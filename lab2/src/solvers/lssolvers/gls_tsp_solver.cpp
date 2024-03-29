#include <algorithm>
#include <random>

#include "gls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

TPaths GLSSolver::solve()
{
    // printf("Solving Greedy Local Search\n");

    vector<Move *> (*get_moves)(TPaths) = nullptr;
    default_random_engine rnd_e(this->start_vertex);

    if (this->neighbourhood == "N1")
    {
        get_moves = MoveGenerator::get_first_neighbourhood_moves;
    }
    else if (this->neighbourhood == "N2")
    {
        get_moves = MoveGenerator::get_second_neighbourhood_moves;
    }

    bool applied = true;

    vector<Move *> moves;

    while (applied)
    {
        applied = false;

        moves = get_moves(this->paths);
        shuffle(moves.begin(), moves.end(), rnd_e);

        for (vector<Move *>::iterator it = moves.begin(); it != moves.end(); it++)
        {
            TPathCost delta = (*it)->get_cost_delta(this->paths, this->distance_matrix);
            if (delta.first + delta.second < 0)
            {
                (*it)->apply(this->paths);
                this->path_cost.first += delta.first;
                this->path_cost.second += delta.second;
                applied = true;
                break;
            }
        }

        // Memory clean
        for (auto move : moves)
        {
            delete move;
        }
        moves.clear();
    }
    get_moves = nullptr;
    // clean end

    return this->paths;
}