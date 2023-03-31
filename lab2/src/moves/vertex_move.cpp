#include "vertex_move.h"

VertexMove::VertexMove(pair<int, int> path_ids, pair<int, int> vertex_ids) {
    this->path_ids = path_ids;
    this->vertex_ids = vertex_ids;
}

void VertexMove::apply(TPaths* paths) {
    // TODO
}

void VertexMove::print() {
    printf("v1 = %d, v2 = %d, p1 = %d, p2 = %d\n", this->vertex_ids.first, this->vertex_ids.second, this->path_ids.first, this->path_ids.second);
}

TPathCost VertexMove::get_cost_delta(TPaths paths) {
    // TODO
    TPathCost costs = make_pair(0, 0);

    return costs;
}
