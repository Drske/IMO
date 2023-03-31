#include "vertex_move.h"

VertexMove::VertexMove(pair<int, int> path_ids, pair<int, int> vertex_ids) {
    this->path_ids = path_ids;
    this->vertex_ids = vertex_ids;
}

void VertexMove::apply(TPaths* paths) {
    // TODO
}

TPathCost VertexMove::get_cost_delta(TPaths paths) {
    // TODO
    TPathCost costs = make_pair(0, 0);

    return costs;
}
