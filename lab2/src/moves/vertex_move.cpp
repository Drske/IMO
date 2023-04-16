#include "vertex_move.h"

VertexMove::VertexMove(pair<int, int> path_ids, pair<int, int> vertex_idxs, pair<int, int> vertex_ids, pair<pair<int, int>, pair<int, int>> adjacent_vertex_ids)
{
    this->path_ids = path_ids;
    this->vertex_idxs = vertex_idxs;
    this->vertex_ids = vertex_ids;
    this->adjacent_vertex_ids = adjacent_vertex_ids;
}

void VertexMove::apply(TPaths &paths)
{
    int p1 = this->path_ids.first, p2 = this->path_ids.second;
    int v1 = this->vertex_idxs.first, v2 = this->vertex_idxs.second;

    if (vertex_ids.first == 30)
    {
        printf("APPLY\n");
        int pred_v1, succ_v1, pred_v2, succ_v2;

        for (int i = 0; i < paths.first.size(); i++)
        {
            if (paths.first[i] == vertex_ids.first)
            {
                i == 0 ? pred_v1 = paths.first.size() - 1 : pred_v1 = i - 1;
                i == paths.first.size() - 1 ? succ_v1 = 0 : succ_v1 = i + 1;
            }
        }

        for (int i = 0; i < paths.second.size(); i++)
        {
            if (paths.second[i] == vertex_ids.second)
            {
                i == 0 ? pred_v2 = paths.second.size() - 1 : pred_v2 = i - 1;
                i == paths.second.size() - 1 ? succ_v2 = 0 : succ_v2 = i + 1;
            }
        }

        printf("OTOCZENIE V1: %d %d %d\n", paths.first[pred_v1], vertex_ids.first, paths.first[succ_v1]);
        printf("OTOCZENIE V2: %d %d %d\n", paths.second[pred_v2], vertex_ids.second, paths.second[succ_v2]);
    }

    if (p1 == p2 && p2 == 0)
        swap(paths.first[v1], paths.first[v2]);
    else if (p1 == p2 && p2 == 1)
        swap(paths.second[v1], paths.second[v2]);
    else if (p1 == 0 && p2 == 1)
        swap(paths.first[v1], paths.second[v2]);
}

void VertexMove::print()
{
    printf("v1 = %d, v2 = %d, v1_idx = %d, v2_idx = %d, p1 = %d, p2 = %d\n", this->vertex_ids.first, this->vertex_ids.second, this->vertex_idxs.first, this->vertex_idxs.second, this->path_ids.first, this->path_ids.second);
}

int VertexMove::get_cost(TPath path, int vertex_idx, int distance_matrix[][N])
{
    int prev_idx = (vertex_idx == 0) ? path.size() - 1 : (vertex_idx - 1);
    int next_idx = (vertex_idx == path.size() - 1) ? 0 : (vertex_idx + 1);

    int v1 = path[prev_idx], v2 = path[vertex_idx], v3 = path[next_idx];

    return distance_matrix[v1][v2] + distance_matrix[v2][v3];
}

int VertexMove::get_new_cost(TPath path, pair<int, int> vertex_idxs, int distance_matrix[][N])
{
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

TPathCost VertexMove::get_new_cost(TPath path1, TPath path2, pair<int, int> vertex_idxs, int distance_matrix[][N])
{
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
        distance_matrix[v2_prev][v1] + distance_matrix[v1][v2_next]);

    return new_cost;
}

TPathCost VertexMove::get_cost_delta(TPaths paths, int distance_matrix[][N])
{
    TPathCost current_cost(0, 0), new_cost(0, 0);

    int p1 = this->path_ids.first;
    int p2 = this->path_ids.second;

    int v1_idx = this->vertex_idxs.first;
    int v2_idx = this->vertex_idxs.second;

    if (p1 == p2 && p2 == 0)
    {
        current_cost.first = this->get_cost(paths.first, v1_idx, distance_matrix) + this->get_cost(paths.first, v2_idx, distance_matrix);

        int v1 = paths.first[v1_idx];
        int v2 = paths.first[v2_idx];

        if (abs(v2_idx - v1_idx) == 1 || (v1_idx == 0 && v2_idx == (paths.first.size() - 1)))
            current_cost.first -= distance_matrix[v1][v2];

        new_cost.first = this->get_new_cost(paths.first, this->vertex_idxs, distance_matrix);
    }
    else if (p1 == p2 && p2 == 1)
    {
        current_cost.second = this->get_cost(paths.second, v1_idx, distance_matrix) + this->get_cost(paths.second, v2_idx, distance_matrix);

        int v1 = paths.second[v1_idx];
        int v2 = paths.second[v2_idx];

        if (abs(v2_idx - v1_idx) == 1 || (v1_idx == 0 && v2_idx == (paths.second.size() - 1)))
            current_cost.second -= distance_matrix[v1][v2];

        new_cost.second = this->get_new_cost(paths.second, this->vertex_idxs, distance_matrix);
    }
    else if (p1 == 0 && p2 == 1)
    {
        current_cost.first = this->get_cost(paths.first, v1_idx, distance_matrix);
        current_cost.second = this->get_cost(paths.second, v2_idx, distance_matrix);

        new_cost = this->get_new_cost(paths.first, paths.second, this->vertex_idxs, distance_matrix);
    }

    return make_pair(new_cost.first - current_cost.first, new_cost.second - current_cost.second);
}

MoveState VertexMove::checkMoveState(TPaths paths)
{
    for (int i = 0; i < paths.first.size(); i++)
    {
        if (paths.first[i] == vertex_ids.first)
        {
            vertex_idxs.first = i;
            break;
        }
    }

    for (int i = 0; i < paths.second.size(); i++)
    {
        if (paths.second[i] == vertex_ids.second)
        {
            vertex_idxs.second = i;
        }
    }
    bool verbose = false;

    // Only works for changing vertices between paths 0 and 1

    if ((vertex_ids.first == 30 && vertex_ids.second == 50) || (vertex_ids.first == 50 && vertex_ids.second == 50))
    {
        verbose = true;
        printf("CHECK VERTEX MOVE ");
        this->print();
        printf("CHECK: %d == %d, %d == %d\n", this->vertex_ids.first, paths.first[this->vertex_idxs.first], this->vertex_ids.second, paths.second[this->vertex_idxs.second]);
        int pred_v1, succ_v1, pred_v2, succ_v2;

        vertex_idxs.first == 0 ? pred_v1 = paths.first.size() - 1 : pred_v1 = vertex_idxs.first - 1;
        vertex_idxs.first == paths.first.size() - 1 ? succ_v1 = 0 : succ_v1 = vertex_idxs.first + 1;

        vertex_idxs.second == 0 ? pred_v2 = paths.second.size() - 1 : pred_v2 = vertex_idxs.second - 1;
        vertex_idxs.second == paths.second.size() - 1 ? succ_v2 = 0 : succ_v2 = vertex_idxs.second + 1;

        printf("OTOCZENIE V1: %d %d %d\n", paths.first[pred_v1], vertex_ids.first, paths.first[succ_v1]);
        printf("OTOCZENIE V2: %d %d %d\n", paths.second[pred_v2], vertex_ids.second, paths.second[succ_v2]);
    }

    int v1_idx = this->vertex_idxs.first;
    int v1_id = this->vertex_ids.first;

    // Look for new position of v1
    for (int i = 0; i < paths.first.size(); i++) {
        if (paths.first[i] == v1_id) {
            v1_idx = i;
            this->vertex_idxs.first = i;
            break;
        }
    }

    int v1_pred_idx = (v1_idx == 0) ? (paths.first.size() - 1) : (v1_idx - 1);
    int v1_pred_id = this->adjacent_vertex_ids.first.first;
    int v1_next_idx = (v1_idx == (paths.first.size() - 1)) ? 0 : (v1_idx + 1);
    int v1_next_id = this->adjacent_vertex_ids.first.second;

    int v2_idx = -1;
    int v2_id = this->vertex_ids.second;

    for (int i = 0; i < paths.second.size(); i++) {
        if (paths.second[i] == v2_id) {
            v2_idx = i;
            this->vertex_idxs.second = i;
            break;
        }
    }

    int v2_pred_idx = (v2_idx == 0) ? (paths.second.size() - 1) : (v2_idx - 1);
    int v2_pred_id = this->adjacent_vertex_ids.second.first;
    int v2_next_idx = (v2_idx == (paths.second.size() - 1)) ? 0 : (v2_idx + 1);
    int v2_next_id = this->adjacent_vertex_ids.second.second;

    if (verbose){
    printf("OTOCZENIE V1 (MD): %d %d %d\n", v1_pred_id, v1_id, v1_next_id);
    printf("OTOCZENIE V2 (MD): %d %d %d\n", v2_pred_id, v2_id, v2_next_id);
    }

    if (v1_id != paths.first[v1_idx] || v2_id != paths.second[v2_idx])
        return MoveState::NOT_APPLICABLE;

    if (!(v1_pred_id == paths.first[v1_pred_idx] && v1_next_id == paths.first[v1_next_idx]) &&
        !(v1_next_id == paths.first[v1_pred_idx] && v1_pred_id == paths.first[v1_next_idx]))
        return MoveState::NOT_APPLICABLE;

    if (!(v2_pred_id == paths.second[v2_pred_idx] && v2_next_id == paths.second[v2_next_idx]) &&
        !(v2_next_id == paths.second[v2_pred_idx] && v2_pred_id == paths.second[v2_next_idx]))
        return MoveState::NOT_APPLICABLE;

    return MoveState::APPLICABLE;

    // if (((v1_id == paths.first[v1_idx]) && (v1_pred_id == paths.first[v1_pred_idx]) && (v1_next_id == paths.first[v1_next_idx])) &&
    //     ((v2_id == paths.second[v2_idx]) && (v2_pred_id == paths.second[v2_pred_idx]) && (v2_next_id == paths.second[v2_next_idx])))
    // {
    //     if (verbose)
    //         printf("APLICABLE 1\n");
    //     return MoveState::APPLICABLE;
    // }

    // if (((v1_id == paths.first[v1_idx]) && (v1_pred_id == paths.first[v1_next_idx]) && (v1_next_id == paths.first[v1_pred_idx])) &&
    //     ((v2_id == paths.second[v2_idx]) && (v2_pred_id == paths.second[v2_next_idx]) && (v2_next_id == paths.second[v2_pred_idx])))
    // {
    //     if (verbose)
    //         printf("APLICABLE 2\n");
    //     return MoveState::APPLICABLE;
    // }

    // if (((v2_id == paths.first[v1_idx]) && (v2_pred_id == paths.first[v1_pred_idx]) && (v2_next_id == paths.first[v1_next_idx])) &&
    //     ((v1_id == paths.second[v2_idx]) && (v1_pred_id == paths.second[v2_pred_idx]) && (v1_next_id == paths.second[v2_next_idx])))
    // {
    //     if (verbose)
    //         printf("APLICABLE 3\n");
    //     return MoveState::APPLICABLE;
    // }

    // if (((v2_id == paths.first[v1_idx]) && (v2_pred_id == paths.first[v1_next_idx]) && (v2_next_id == paths.first[v1_pred_idx])) &&
    //     ((v1_id == paths.second[v2_idx]) && (v1_pred_id == paths.second[v2_next_idx]) && (v1_next_id == paths.second[v2_pred_idx])))
    // {
    //     if (verbose)
    //         printf("APLICABLE 4\n");
    //     return MoveState::APPLICABLE;
    // }
    // if (verbose)
    // {
    //     printf("NOT APPLICABLE\n");
    // }
    // return MoveState::NOT_APPLICABLE;
}

pair<int, int> VertexMove::get_path_ids()
{
    return this->path_ids;
}

pair<int, int> VertexMove::get_vertex_idxs()
{
    return this->vertex_idxs;
}