#include <random>

#include "gls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

TPaths GLSSolver::solve() {
    printf("Solving Greedy Local Search\n");

    vector<Move*> (*get_moves)(TPaths) = nullptr;

    if (this->neighbourhood == "N1"){
        get_moves = MoveGenerator::get_first_neighbourhood_moves;
    }
    else if (this->neighbourhood == "N2"){
        get_moves = MoveGenerator::get_second_neighbourhood_moves;
    }

    while (true){
        vector<Move*> moves = get_moves(this->paths);
        random_shuffle(moves.begin(), moves.end());
    }
    
    return this->paths;
}