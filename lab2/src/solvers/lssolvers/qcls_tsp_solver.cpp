#include <algorithm>
#include <random>
#include <queue>

#include "qcls_tsp_solver.h"
#include "../../moves/move.h"
#include "../../move_generator/move_generator.h"

using namespace std;

struct MoveItem
{
    Move *move;
    TPathCost cost_delta;
};

class MoveItemComparator
{
public:
    bool operator()(MoveItem m1, MoveItem m2)
    {
        if (m1.cost_delta.first + m1.cost_delta.second > m2.cost_delta.first + m2.cost_delta.second)
            return true;
        return false;
    }
};

TPaths QCLSSolver::solve()
{
    printf("Solving Queue Local Search with Candidate Moves\n");

    vector<Move *> (*get_moves)(TPaths, int[][N], int) = nullptr;
    get_moves = MoveGenerator::get_second_neighbourhood_candidate_moves;

    priority_queue<MoveItem, vector<MoveItem>, MoveItemComparator> LM;

    bool applied = true;
    Move *best_move;
    TPathCost best_delta;

    while (applied)
    {
        applied = false;
        best_delta = make_pair(INT_MAX / 2, INT_MAX / 2);
        vector<Move *> moves;
        vector<MoveItem> future_moves;

        if (LM.empty())
            moves = get_moves(this->paths, this->distance_matrix, this->max_candidates);
        else
        {
            VertexMove *vertexMove = dynamic_cast<VertexMove *>(best_move);
            EdgeMove *edgeMove = dynamic_cast<EdgeMove *>(best_move);

            if (vertexMove)
                moves = MoveGenerator::get_new_moves_after_move(this->paths, vertexMove);
            else if (edgeMove)
                moves = MoveGenerator::get_new_moves_after_move(this->paths, edgeMove);
        }

        for (vector<Move *>::iterator it = moves.begin(); it != moves.end(); it++)
        {
            MoveItem move_item;
            move_item.move = (*it);
            move_item.cost_delta = (*it)->get_cost_delta(this->paths, this->distance_matrix);

            if (move_item.cost_delta.first + move_item.cost_delta.second < 0)
                LM.push(move_item);
        }

        while (!LM.empty())
        {
            MoveItem move_item = LM.top();
            LM.pop();
            Move *move = move_item.move;
            MoveState move_state = move->checkMoveState(this->paths);

            if (move_state == MoveState::APPLICABLE)
            {
                best_move = move;
                best_delta = move_item.cost_delta;
                break;
            }
            else if (move_state == MoveState::FUTURE_APPLICABLE)
            {
                future_moves.push_back(move_item);
            }
            else if (move_state == MoveState::NOT_APPLICABLE)
            {
                //
            }
        }

        if (best_delta.first + best_delta.second < 0)
        {
            best_move->apply(this->paths);
            this->path_cost.first += best_delta.first;
            this->path_cost.second += best_delta.second;
            applied = true;

            for (vector<MoveItem>::iterator it = future_moves.begin(); it != future_moves.end(); it++)
            {
                LM.push(*it);
            }
        }
    }
    return this->paths;
}