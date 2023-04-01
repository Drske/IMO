#include <algorithm>
#include <random>

#include "gls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

TPaths GLSSolver::solve()
{
    printf("Solving Greedy Local Search\n");

    vector<Move *> (*get_moves)(TPaths) = nullptr;
    // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
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

    while (applied)
    {
        applied = false;

        vector<Move *> moves = get_moves(this->paths);
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
    }

    return this->paths;
}