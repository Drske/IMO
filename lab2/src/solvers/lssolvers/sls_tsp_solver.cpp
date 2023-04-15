#include <algorithm>
#include <random>

#include "sls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

TPaths SLSSolver::solve()
{
    printf("Solving Steepest Local Search\n");

    vector<Move *> (*get_moves)(TPaths) = nullptr;

    if (this->neighbourhood == "N1")
    {
        get_moves = MoveGenerator::get_first_neighbourhood_moves;
    }
    else if (this->neighbourhood == "N2")
    {
        get_moves = MoveGenerator::get_second_neighbourhood_moves;
    }

    bool applied = true;
    Move *best_move;
    TPathCost best_delta;
    
    while (applied)
    {
        applied = false;
        best_delta = make_pair(INT_MAX / 2, INT_MAX / 2);

        vector<Move *> moves = get_moves(this->paths);
        
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
            applied=true;
        }
    }

    return this->paths;
}