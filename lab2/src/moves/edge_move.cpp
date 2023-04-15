#include "edge_move.h"
#include <cmath>
#include <set>

EdgeMove::EdgeMove(int path_id, TEdges edge_idxs, TEdges edge_ids) {
    this->path_id = path_id;
    this->edge_idxs = edge_idxs;
    this->edge_ids = edge_ids;
}

void EdgeMove::apply(TPaths &paths) {
    int v12 = this->edge_idxs.first.second;
    int v21 = this->edge_idxs.second.first;

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
    TPath path = (this->path_id == 0) ? paths.first : paths.second;

    int v11 = path[this->edge_idxs.first.first], v12 = path[this->edge_idxs.first.second];
    int v21 = path[this->edge_idxs.second.first], v22 = path[this->edge_idxs.second.second];

    int current_cost = distance_matrix[v11][v12] + distance_matrix[v21][v22];
    int new_cost = distance_matrix[v11][v21] + distance_matrix[v12][v22];
    
    TPathCost delta_cost(0, 0);

    if (this->path_id == 0)
        delta_cost.first = new_cost - current_cost;
    else
        delta_cost.second = new_cost - current_cost;

    return delta_cost;
}

MoveState EdgeMove::checkMoveState(TPaths paths) {
    TPath path = (this->path_id == 0) ? paths.first : paths.second;
    TEdges new_edge_ids = make_pair(
        make_pair(path[this->edge_idxs.first.first], path[this->edge_idxs.first.second]),
        make_pair(path[this->edge_idxs.second.first], path[this->edge_idxs.second.second])
    );

    // Exactly same order of ids - applicable
    if (this->edge_ids.first.first == new_edge_ids.first.first &&
        this->edge_ids.first.second == new_edge_ids.first.second && 
        this->edge_ids.second.first == new_edge_ids.second.first && 
        this->edge_ids.second.second == new_edge_ids.second.second) 
        return MoveState::APPLICABLE;

    // Both edges reversed - applicable
    if (this->edge_ids.first.first == new_edge_ids.first.second &&
        this->edge_ids.first.second == new_edge_ids.first.first && 
        this->edge_ids.second.first == new_edge_ids.second.second && 
        this->edge_ids.second.second == new_edge_ids.second.first) 
        return MoveState::APPLICABLE;

    // Same ids, different order - future applicable
    set<int> set_edge_ids = {this->edge_ids.first.first, this->edge_ids.first.second,
                             this->edge_ids.second.first, this->edge_ids.second.second};
    set<int> set_new_edge_ids = {new_edge_ids.first.first, new_edge_ids.first.second,
                                 new_edge_ids.second.first, new_edge_ids.second.second};

    if (set_edge_ids == set_new_edge_ids)
        return MoveState::FUTURE_APPLICABLE;

    // Otherwise - not applicable
    return MoveState::NOT_APPLICABLE;
}

TEdges EdgeMove::get_edge_idxs() {
    return this->edge_idxs;
}

int EdgeMove::get_path_id() {
    return this->path_id;
}