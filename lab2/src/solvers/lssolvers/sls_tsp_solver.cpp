#include <algorithm>
#include <random>

#include "sls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

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
    // double gen_mov_dur = 0;
    // double eval_mov_dur = 0;
    // double apply_mov_dur = 0;

    int move_count = 0;
    
    while (applied)
    {
        applied = false;
        best_delta = make_pair(INT_MAX / 2, INT_MAX / 2);
        
        // high_resolution_clock::time_point start = high_resolution_clock::now();
        vector<Move *> moves = get_moves(this->paths);
        // high_resolution_clock::time_point stop = high_resolution_clock::now();
        // gen_mov_dur += duration_cast<microseconds>(stop - start).count();

        // printf("Generated moves (steepest without candidates): %lu\n", moves.size());
        
        for (vector<Move *>::iterator it = moves.begin(); it != moves.end(); it++)
        {
            // high_resolution_clock::time_point start = high_resolution_clock::now();
            TPathCost delta = (*it)->get_cost_delta(this->paths, this->distance_matrix);
            // high_resolution_clock::time_point stop = high_resolution_clock::now();
            // eval_mov_dur += duration_cast<microseconds>(stop - start).count();
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

            printf("Best delta: %d, %d\n", best_delta.first, best_delta.second);
            printf("Best move: ");
            best_move->print();
            printf("Current cost: %d, %d\n", path_cost.first, path_cost.second);
            printf("-------- %d --------\n", move_count);
            move_count++;

            printf("Path 1: ");
            for (auto v : paths.first){
                printf("%d ", v);
            }
            printf("\nPath 2: ");
            for (auto v : paths.second){
                printf("%d ", v);
            }
            printf("\n");
        }
    }

    return this->paths;
}