#ifndef EDGE_SOLVER_H
#define EDGE_SOLVER_H

#include "move.h"

using namespace std;

class EdgeMove : public Move
{
public:
    EdgeMove(int path_id, TEdges edge_idxs, TEdges edge_ids);
    void apply(TPaths &paths);
    void print();
    TPathCost get_cost_delta(TPaths paths, int distance_matrix[][N]);
    MoveState checkMoveState(TPaths paths);
    TEdges get_edge_idxs();
    int get_path_id();
private:
    int path_id;
    TEdges edge_idxs;
    TEdges edge_ids;
};

#endif
