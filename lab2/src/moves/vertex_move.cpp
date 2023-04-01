#include "vertex_move.h"

VertexMove::VertexMove(pair<int, int> path_ids, pair<int, int> vertex_idxs) {
    this->path_ids = path_ids;
    this->vertex_idxs = vertex_idxs;
}

void VertexMove::apply(TPaths &paths) {
    int p1 = this->path_ids.first, p2 = this->path_ids.second;
    int v1 = this->vertex_idxs.first, v2 = this->vertex_idxs.second;

    if (p1 == p2 && p2 == 0)
        swap(paths.first[v1], paths.first[v2]);
    else if (p1 == p2 && p2 == 1) 
        swap(paths.second[v1], paths.second[v2]);
    else if (p1 == 0 && p2 == 1)
        swap(paths.first[v1], paths.second[v2]);
}

void VertexMove::print() {
    printf("v1_idx = %d, v2_idx = %d, p1 = %d, p2 = %d\n", this->vertex_idxs.first, this->vertex_idxs.second, this->path_ids.first, this->path_ids.second);
}

int VertexMove::get_cost(TPath path, int vertex_idx, int distance_matrix[][N]) {
    int prev_idx = (vertex_idx == 0) ? path.size() - 1 : (vertex_idx - 1);
    int next_idx = (vertex_idx == path.size() - 1) ? 0 : (vertex_idx + 1);

    int v1 = path[prev_idx], v2 = path[vertex_idx], v3 = path[next_idx];

    return distance_matrix[v1][v2] + distance_matrix[v2][v3];
}

int VertexMove::get_new_cost(TPath path, pair<int, int> vertex_idxs, int distance_matrix[][N]) {
    int v1_idx = vertex_idxs.first;
    int v1_prev_idx = (v1_idx == 0) ? path.size() - 1 : (v1_idx - 1);
    int v1_next_idx = (v1_idx == path.size() - 1) ? 0 : (v1_idx + 1);

    int v2_idx = vertex_idxs.second;
    int v2_prev_idx = (v2_idx == 0) ? path.size() - 1 : (v2_idx - 1);
    int v2_next_idx = (v2_idx == path.size() - 1) ? 0 : (v2_idx + 1);

    int v1 = path[v1_idx], v1_prev = path[v1_prev_idx], v1_next = path[v1_next_idx];
    int v2 = path[v2_idx], v2_prev = path[v2_prev_idx], v2_next = path[v2_next_idx];

    if (v2_idx - v1_idx == 1)
        return distance_matrix[v1_prev][v2] + distance_matrix[v2][v1] + distance_matrix[v1][v2_next];

    if (v1_idx == 0 && v2_idx == (path.size() - 1))
        return distance_matrix[v2_prev][v1] + distance_matrix[v1][v2] + distance_matrix[v2][v1_next];

    return distance_matrix[v1_prev][v2] + distance_matrix[v2][v1_next] + 
           distance_matrix[v2_prev][v1] + distance_matrix[v1][v2_next];
}

TPathCost VertexMove::get_new_cost(TPath path1, TPath path2, pair<int, int> vertex_idxs, int distance_matrix[][N]) {
    int v1_idx = vertex_idxs.first;
    int v1_prev_idx = (v1_idx == 0) ? path1.size() - 1 : (v1_idx - 1);
    int v1_next_idx = (v1_idx == path1.size() - 1) ? 0 : (v1_idx + 1);

    int v2_idx = vertex_idxs.second;
    int v2_prev_idx = (v2_idx == 0) ? path2.size() - 1 : (v2_idx - 1);
    int v2_next_idx = (v2_idx == path2.size() - 1) ? 0 : (v2_idx + 1);

    int v1 = path1[v1_idx], v1_prev = path1[v1_prev_idx], v1_next = path1[v1_next_idx];
    int v2 = path2[v2_idx], v2_prev = path2[v2_prev_idx], v2_next = path2[v2_next_idx];

    TPathCost new_cost(
        distance_matrix[v1_prev][v2] + distance_matrix[v2][v1_next],
        distance_matrix[v2_prev][v1] + distance_matrix[v1][v2_next]
    );

    return new_cost;
}

TPathCost VertexMove::get_cost_delta(TPaths paths, int distance_matrix[][N]) {
    TPathCost current_cost(0, 0), new_cost(0, 0);

    int p1 = this->path_ids.first;
    int p2 = this->path_ids.second;

    int v1_idx = this->vertex_idxs.first;
    int v2_idx = this->vertex_idxs.second;

    if (p1 == p2 && p2 == 0) {
        current_cost.first = this->get_cost(paths.first, v1_idx, distance_matrix) + this->get_cost(paths.first, v2_idx, distance_matrix);

        int v1 = paths.first[v1_idx];
        int v2 = paths.first[v2_idx];

        if (abs(v2_idx - v1_idx) == 1 || (v1_idx == 0 && v2_idx == (paths.first.size() - 1)))
            current_cost.first -= distance_matrix[v1][v2];

        new_cost.first = this->get_new_cost(paths.first, this->vertex_idxs, distance_matrix);
    } else if (p1 == p2 && p2 == 1) {
        current_cost.second = this->get_cost(paths.second, v1_idx, distance_matrix) + this->get_cost(paths.second, v2_idx, distance_matrix);

        int v1 = paths.second[v1_idx];
        int v2 = paths.second[v2_idx];

        if (abs(v2_idx - v1_idx) == 1 || (v1_idx == 0 && v2_idx == (paths.second.size() - 1)))
            current_cost.second -= distance_matrix[v1][v2];

        new_cost.second = this->get_new_cost(paths.second, this->vertex_idxs, distance_matrix);
    } else if (p1 == 0 && p2 == 1) {
        current_cost.first = this->get_cost(paths.first, v1_idx, distance_matrix);
        current_cost.second = this->get_cost(paths.second, v2_idx, distance_matrix);

        new_cost = this->get_new_cost(paths.first, paths.second, this->vertex_idxs, distance_matrix);
    }

    return make_pair(new_cost.first - current_cost.first, new_cost.second - current_cost.second);
}
