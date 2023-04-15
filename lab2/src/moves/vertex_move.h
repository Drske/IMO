#ifndef VERTEX_SOLVER_H
#define VERTEX_SOLVER_H

#include "move.h"

using namespace std;

class VertexMove : public Move
{
public:
    VertexMove(pair<int, int> path_ids, pair<int, int> vertex_idxs, pair<int, int> vertex_ids);
    void apply(TPaths &paths);
    void print();
    TPathCost get_cost_delta(TPaths paths, int distance_matrix[][N]);
    MoveState checkMoveState(TPaths paths);
    pair<int, int> get_path_ids();
    pair<int, int> get_vertex_idxs();
private:
    int get_cost(TPath path, int vertex_idx, int distance_matrix[][N]);
    int get_new_cost(TPath path, pair<int, int> vertex_idxs, int distance_matrix[][N]);
    TPathCost get_new_cost(TPath path1, TPath path2, pair<int, int> vertex_idxs, int distance_matrix[][N]);
    pair<int, int> path_ids;
    pair<int, int> vertex_idxs;
    pair<int, int> vertex_ids;
};

#endif
