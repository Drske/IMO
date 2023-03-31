#include "edge_move.h"

EdgeMove::EdgeMove(int path_id, TEdges edge_ids) {
    this->path_id = path_id;
    this->edge_ids = edge_ids;
}

void EdgeMove::apply(TPaths* paths) {
    // TODO
}

TPathCost EdgeMove::get_cost_delta(TPaths paths) {
    // TODO
    TPathCost costs = make_pair(0, 0);

    return costs;
}