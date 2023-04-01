#include <algorithm>
#include <random>

#include "sls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

TPaths SLSSolver::solve() {
    printf("Solving Steepest Local Search\n");

    vector<Move*> (*get_moves)(TPaths) = nullptr;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rnd_e(seed);

    if (this->neighbourhood == "N1"){
        get_moves = MoveGenerator::get_first_neighbourhood_moves;
    }
    else if (this->neighbourhood == "N2"){
        get_moves = MoveGenerator::get_second_neighbourhood_moves;
    }

    bool applied = true;
    Move* best_move;
    TPathCost best_delta = make_pair(INT_MAX, INT_MAX);

    while (applied){
        applied = false;
        best_delta = make_pair(INT_MAX, INT_MAX);

        vector<Move*> moves = get_moves(this->paths);
        shuffle(moves.begin(), moves.end(), rnd_e);

        for (vector<Move*>::iterator it = moves.begin(); it != moves.end(); it++){
            TPathCost delta = (*it)->get_cost_delta(this->paths, this->distance_matrix);
            if (delta.first + delta.second < best_delta.first + best_delta.second){
                best_delta = delta;
                best_move = (*it);
            }
        }

        best_move->apply(this->paths);
        this->path_cost.first += best_delta.first;
        this->path_cost.second += best_delta.second;
    }

    return this->paths;
}