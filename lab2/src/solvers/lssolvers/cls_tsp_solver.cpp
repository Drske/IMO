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

    double gen_mov_dur = 0;
    double eval_mov_dur = 0;
    double apply_mov_dur = 0;

    while (applied)
    {
        applied = false;
        best_delta = make_pair(INT_MAX / 2, INT_MAX / 2);

        high_resolution_clock::time_point start = high_resolution_clock::now();
        vector<Move *> moves = get_moves(this->paths, this->distance_matrix, this->max_candidates);
        high_resolution_clock::time_point stop = high_resolution_clock::now();
        gen_mov_dur += duration_cast<microseconds>(stop - start).count();

        // printf("Generated moves (steepest without candidates): %lu\n", moves.size());

        for (vector<Move *>::iterator it = moves.begin(); it != moves.end(); it++)
        {
            high_resolution_clock::time_point start = high_resolution_clock::now();
            TPathCost delta = (*it)->get_cost_delta(this->paths, this->distance_matrix);
            high_resolution_clock::time_point stop = high_resolution_clock::now();
            eval_mov_dur += duration_cast<microseconds>(stop - start).count();
            if (delta.first + delta.second < best_delta.first + best_delta.second)
            {
                best_delta = delta;
                best_move = (*it);
            }
        }

        if (best_delta.first + best_delta.second < 0)
        {
            high_resolution_clock::time_point start = high_resolution_clock::now();
            best_move->apply(this->paths);
            high_resolution_clock::time_point stop = high_resolution_clock::now();
            apply_mov_dur += duration_cast<microseconds>(stop - start).count();

            this->path_cost.first += best_delta.first;
            this->path_cost.second += best_delta.second;
            applied = true;
        }
        
        // printf("BEST MOVE:\n");
        // best_move->print();
        // printf("Cost1: %d, Cost2: %d\n", this->path_cost.first, this->path_cost.second);
    }

    printf("Gen: %f\nEval: %f\nApply: %f\n", gen_mov_dur, eval_mov_dur, apply_mov_dur);
    return this->paths;
}