#include "edge_move.h"
#include <cmath>

EdgeMove::EdgeMove(int path_id, TEdges edge_idxs) {
    this->path_id = path_id;
    this->edge_idxs = edge_idxs;
}

void EdgeMove::apply(TPaths &paths) {
    int v11 = this->edge_idxs.first.first, v12 = this->edge_idxs.first.second;
    int v21 = this->edge_idxs.second.first, v22 = this->edge_idxs.second.second;

    int n = round((float)(v21 - v12) / 2.0);

    if (this->path_id == 0)
        for (int i = 0; i < n; i++)
            swap(paths.first[v12 + i], paths.first[v21 - i]);

    if (this->path_id == 1)
        for (int i = 0; i < n; i++)
            swap(paths.second[v12 + i], paths.second[v21 - i]);
}

void EdgeMove::print() {
    printf("v11 = %d, v12 = %d, v21 = %d, v22 = %d, p = %d\n", this->edge_idxs.first.first, this->edge_idxs.first.second, this->edge_idxs.second.first, this->edge_idxs.second.second, this->path_id);
}

TPathCost EdgeMove::get_cost_delta(TPaths paths, int distance_matrix[][N]) {
    int v11 = this->edge_idxs.first.first, v12 = this->edge_idxs.first.second;
    int v21 = this->edge_idxs.second.first, v22 = this->edge_idxs.second.second;

    int current_cost = distance_matrix[v11][v12] + distance_matrix[v21][v22];
    int new_cost = distance_matrix[v11][v21] + distance_matrix[v12][v22];
    
    TPathCost delta_cost(0, 0);

    if (this->path_id == 0)
        delta_cost.first = new_cost - current_cost;
    else
        delta_cost.second = new_cost - current_cost;

    return delta_cost;
}