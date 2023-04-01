#include "edge_move.h"

EdgeMove::EdgeMove(int path_id, TEdges edge_idxs) {
    this->path_id = path_id;
    this->edge_idxs = edge_idxs;
}

void EdgeMove::apply(TPaths &paths) {
    // TODO
}

void EdgeMove::print() {
    printf("e11 = %d, e12 = %d, e21 = %d, e22 = %d, p = %d\n", this->edge_idxs.first.first, this->edge_idxs.first.second, this->edge_idxs.second.first, this->edge_idxs.second.second, this->path_id);
}

TPathCost EdgeMove::get_cost_delta(TPaths paths, int distance_matrix[][N]) {
    // TODO
    TPathCost costs = make_pair(0, 0);

    return costs;
}