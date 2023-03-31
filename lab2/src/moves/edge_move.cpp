#include "edge_move.h"

EdgeMove::EdgeMove(int path_id, TEdges edge_ids) {
    this->path_id = path_id;
    this->edge_ids = edge_ids;
}

void EdgeMove::apply(TPaths* paths) {
    // TODO
}

void EdgeMove::print() {
    printf("e11 = %d, e12 = %d, e21 = %d, e22 = %d, p = %d\n", this->edge_ids.first.first, this->edge_ids.first.second, this->edge_ids.second.first, this->edge_ids.second.second, this->path_id);
}

TPathCost EdgeMove::get_cost_delta(TPaths paths) {
    // TODO
    TPathCost costs = make_pair(0, 0);

    return costs;
}