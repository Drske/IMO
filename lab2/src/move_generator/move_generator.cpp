#include "../moves/edge_move.h"
#include "../moves/vertex_move.h"
#include "move_generator.h"
#include <queue>

pair<int, int> MoveGenerator::get_adjacent_vertex_idxs(TPath path, int vertex_idx) {
    int pred_idx = (vertex_idx == 0) ? (path.size() - 1) : (vertex_idx - 1); // predecessor
    int next_idx = (vertex_idx == (path.size() - 1)) ? 0 : (vertex_idx + 1); // succesor

    pair<int, int> adjacent_vertex_idxs(pred_idx, next_idx);
    
    return adjacent_vertex_idxs;
}

pair<int, int> MoveGenerator::get_adjacent_vertex_ids(TPath path, int vertex_idx) {
    pair<int, int> adjacent_vertex_idxs = get_adjacent_vertex_idxs(path, vertex_idx);
    pair<int, int> adjacent_vertex_ids(
        path[adjacent_vertex_idxs.first], 
        path[adjacent_vertex_idxs.second]
    );
    
    return adjacent_vertex_ids;
}

vector<Move *> MoveGenerator::get_first_neighbourhood_moves(TPaths paths)
{
    // Wymiana wierzchołków wewnątrz + wymiana wierzchołków pomiędzy cyklami
    vector<Move *> moves;

    add_vertex_moves_from_path(paths.first, 0, moves);
    add_vertex_moves_from_path(paths.second, 1, moves);
    add_vertex_moves_from_paths(paths, moves);

    return moves;
}

vector<Move *> MoveGenerator::get_second_neighbourhood_moves(TPaths paths)
{
    // Wymiana krawędzi wewnątrz + wymiana wierzchołków pomiędzy cyklami
    vector<Move *> moves;

    add_edge_moves_from_path(paths.first, 0, moves);
    // printf("Second neighbourhood after adding edge moves (1):\t%lu\n", moves.size());
    add_edge_moves_from_path(paths.second, 1, moves);
    // printf("Second neighbourhood after adding edge moves (2):\t%lu\n", moves.size());
    add_vertex_moves_from_paths(paths, moves);
    // printf("Second neighbourhood after adding vertex moves (1):\t%lu\n", moves.size());

    return moves;
}

vector<Move *> MoveGenerator::get_second_neighbourhood_candidate_moves(TPaths paths, int distance_matrix[][N], int max_candidates)
{
    vector<Move *> moves;

    add_candidate_edge_moves_from_path(paths.first, 0, moves, distance_matrix, max_candidates);
    // printf("Candidate second neighbourhood after adding edge moves (1):\t%lu\n", moves.size());
    add_candidate_edge_moves_from_path(paths.second, 1, moves, distance_matrix, max_candidates);
    // printf("Candidate second neighbourhood after adding edge moves (2):\t%lu\n", moves.size());
    add_candidate_vertex_moves_from_paths(paths, moves, distance_matrix, max_candidates);
    // printf("Candidate second neighbourhood after adding vertex moves (1):\t%lu\n", moves.size());

    return moves;
}

void MoveGenerator::add_vertex_moves_from_path(TPath path, int path_id, vector<Move *> &moves)
{
    pair<int, int> path_ids(path_id, path_id);
    for (int i = 0; i < path.size() - 1; i++)
    {
        for (int j = i + 1; j < path.size(); j++)
        {
            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(path[i], path[j]);

            pair<pair<int, int>, pair<int, int>> adjacent_vertex_ids;
            adjacent_vertex_ids.first = get_adjacent_vertex_ids(path, vertex_idxs.first);
            adjacent_vertex_ids.second = get_adjacent_vertex_ids(path, vertex_idxs.second);

            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids, adjacent_vertex_ids));
        }
    }
}

void MoveGenerator::add_edge_moves_from_path(TPath path, int path_id, vector<Move *> &moves)
{
    for (int i = 0; i < path.size() - 2; i++)
    {
        TEdge edge1_idxs(i, i + 1);
        TEdge edge1_ids(path[i], path[i + 1]);

        for (int j = i + 2; j < path.size(); j++)
        {
            TEdge edge2_idxs;
            TEdge edge2_ids;

            if (j == (path.size() - 1))
                edge2_idxs = make_pair(j, 0);
            else
                edge2_idxs = make_pair(j, j + 1);

            edge2_ids = make_pair(path[edge2_idxs.first], path[edge2_idxs.second]);

            TEdges edge_idxs(edge1_idxs, edge2_idxs);
            TEdges edge_ids(edge1_ids, edge2_ids);

            moves.push_back(new EdgeMove(path_id, edge_idxs, edge_ids));
        }
    }
}

void MoveGenerator::add_vertex_moves_from_paths(TPaths paths, vector<Move *> &moves)
{
    pair<int, int> path_ids(0, 1);
    for (int i = 0; i < paths.first.size(); i++)
    {
        for (int j = 0; j < paths.second.size(); j++)
        {
            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(paths.first[i], paths.second[j]);
            pair<pair<int, int>, pair<int, int>> adjacent_vertex_ids;
            adjacent_vertex_ids.first = get_adjacent_vertex_ids(paths.first, vertex_idxs.first);
            adjacent_vertex_ids.second = get_adjacent_vertex_ids(paths.second, vertex_idxs.second);

            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids, adjacent_vertex_ids));
        }
    }
}

struct neigh_comp
{
    bool operator()(
        pair<int, int> const &a,
        pair<int, int> const &b)
    {
        return a.second > b.second;
    }
};

void MoveGenerator::add_candidate_edge_moves_from_path(TPath path, int path_id, vector<Move *> &moves, int distance_matrix[][N], int max_candidates)
{
    int candidates_no;
    priority_queue<pair<int, int>, vector<pair<int, int>>, neigh_comp> nearest_neighbours;

    for (int i = 0; i < path.size(); i++)
    {
        while (!nearest_neighbours.empty())
        {
            nearest_neighbours.pop();
        }
        candidates_no = 0;

        for (int j = i + 1; j < path.size(); j++)
        {
            // Save neighbour as idx in path
            nearest_neighbours.push(make_pair(j, distance_matrix[path[i]][path[j]]));
        }

        while (!nearest_neighbours.empty() && candidates_no < max_candidates)
        {
            int j = nearest_neighbours.top().first;
            nearest_neighbours.pop();

            candidates_no += 1;

            int p = path[i]; // p is an id of idx i
            int q = path[j]; // q is an id of idx j

            int succ_i = i != path.size() - 1 ? i + 1 : 0;
            int succ_p = path[succ_i];
            int succ_j = j != path.size() - 1 ? j + 1 : 0;
            int succ_q = path[succ_j];

            int pred_i = i != 0 ? i - 1 : path.size() - 1;
            int pred_p = path[pred_i];
            int pred_j = j != 0 ? j - 1 : path.size() - 1;
            int pred_q = path[pred_j];

            TEdges from_succ_idxs(make_pair(i, succ_i), make_pair(j, succ_j));
            TEdges from_succ_ids(make_pair(p, succ_p), make_pair(q, succ_q));

            TEdges from_pred_idxs(make_pair(pred_i, i), make_pair(pred_j, j));
            TEdges from_pred_ids(make_pair(pred_p, p), make_pair(pred_q, q));

            moves.push_back(new EdgeMove(path_id, from_succ_idxs, from_succ_ids));
            moves.push_back(new EdgeMove(path_id, from_pred_idxs, from_pred_ids));
        }
    }
}

void MoveGenerator::add_candidate_vertex_moves_from_paths(TPaths paths, vector<Move *> &moves, int distance_matrix[][N], int max_candidates)
{
    pair<int, int> path_ids(0, 1);

    priority_queue<pair<int, int>, vector<pair<int, int>>, neigh_comp> nearest_neighbours;
    int candidates_no;

    for (int i = 0; i < paths.first.size(); i++)
    {
        while (!nearest_neighbours.empty())
        {
            nearest_neighbours.pop();
        }

        candidates_no = 0;

        for (int j = 0; j < paths.second.size(); j++)
        {
            // Save neighbour as idx in path
            nearest_neighbours.push(make_pair(j, distance_matrix[paths.first[i]][paths.second[j]]));
        }

        while (!nearest_neighbours.empty() && candidates_no < max_candidates)
        {
            int j = nearest_neighbours.top().first;
            nearest_neighbours.pop();
            candidates_no += 1;

            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(paths.first[i], paths.second[j]);
            pair<pair<int, int>, pair<int, int>> adjacent_vertex_ids;
            adjacent_vertex_ids.first = get_adjacent_vertex_ids(paths.first, vertex_idxs.first);
            adjacent_vertex_ids.second = get_adjacent_vertex_ids(paths.second, vertex_idxs.second);

            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids, adjacent_vertex_ids));
        }
    }
}

vector<Move *> MoveGenerator::get_new_moves_after_move(TPaths paths, VertexMove *vertexMove)
{
    vector<Move *> moves;
    pair<int, int> path_ids = vertexMove->get_path_ids();
    pair<int, int> vertex_idxs = vertexMove->get_vertex_idxs();
    pair<vector<int>, vector<int>> paths_modified_idxs;
    pair<int, int> v1_adjacent_idxs = get_adjacent_vertex_idxs(paths.first, vertex_idxs.first);
    pair<int, int> v2_adjacent_idxs = get_adjacent_vertex_idxs(paths.second, vertex_idxs.second);

    paths_modified_idxs.first.push_back(vertex_idxs.first);
    paths_modified_idxs.first.push_back(v1_adjacent_idxs.first);
    paths_modified_idxs.first.push_back(v1_adjacent_idxs.second);

    paths_modified_idxs.second.push_back(vertex_idxs.second);
    paths_modified_idxs.second.push_back(v2_adjacent_idxs.first);
    paths_modified_idxs.second.push_back(v2_adjacent_idxs.second);

    add_edge_moves_after_move(paths.first, path_ids.first, paths_modified_idxs.first, moves);
    add_edge_moves_after_move(paths.second, path_ids.second, paths_modified_idxs.second, moves);

    add_vertex_moves_after_move(paths, paths_modified_idxs, moves);

    return moves;
}

vector<Move *> MoveGenerator::get_new_moves_after_move(TPaths paths, EdgeMove *edgeMove)
{
    int path_id = edgeMove->get_path_id();
    TPath path = (path_id == 0) ? paths.first : paths.second;
    TEdges edge_idxs = edgeMove->get_edge_idxs();
    pair<vector<int>, vector<int>> paths_modified_idxs;
    vector<int> path_modified_idxs = {edge_idxs.first.first, edge_idxs.first.second,
                                      edge_idxs.second.first, edge_idxs.second.second};

    if (path_id == 0)
        paths_modified_idxs.first = path_modified_idxs;
    else
        paths_modified_idxs.second = path_modified_idxs;

    vector<Move *> moves;

    // 2nd neighbourhood
    add_edge_moves_after_move(path, path_id, path_modified_idxs, moves);
    add_vertex_moves_after_move(paths, paths_modified_idxs, moves);

    return moves;
}

void MoveGenerator::add_vertex_move(TPaths paths, int i, int j, vector<Move *> &moves) 
{
    pair<int, int> path_ids(0, 1);
    pair<int, int> vertex_idxs(i, j);
    pair<int, int> vertex_ids(paths.first[i], paths.second[j]);

    pair<pair<int, int>, pair<int, int>> adjacent_vertex_ids;
    adjacent_vertex_ids.first = get_adjacent_vertex_ids(paths.first, vertex_idxs.first);
    adjacent_vertex_ids.second = get_adjacent_vertex_ids(paths.second, vertex_idxs.second);

    moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids, adjacent_vertex_ids));
}

void MoveGenerator::add_vertex_moves_after_move(TPaths paths, pair<vector<int>, vector<int>> paths_modified_idxs, vector<Move *> &moves)
{
    for (int i : paths_modified_idxs.first)
        for (int j = 0; j < paths.second.size(); j++)
            add_vertex_move(paths, i, j, moves);

    for (int i = 0; i < paths.first.size(); i++)
        for (int j : paths_modified_idxs.second)
            add_vertex_move(paths, i, j, moves);
}

void MoveGenerator::add_edge_moves_after_move(TPath path, int path_id, vector<int> path_modified_idxs, vector<Move *> &moves)
{
    for (int i : path_modified_idxs)
    {        
        TEdge edge1_prev_idxs = (i == 0) ? make_pair((int)path.size() - 1, i) : make_pair(i - 1, i);
        TEdge edge1_next_idxs = (i == ((int)path.size() - 1)) ? make_pair(i, 0) : make_pair(i, i + 1);

        vector<TEdge> edges1_idxs = {edge1_prev_idxs, edge1_next_idxs};

        for (TEdge edge1_idxs : edges1_idxs)
        {
            TEdge edge1_ids(path[edge1_idxs.first], path[edge1_idxs.second]);

            for (int j = 0; j < path.size(); j++)
            {
                TEdge edge2_idxs = (j == (path.size() - 1)) ? make_pair(j, 0) : make_pair(j, j + 1);
                TEdge edge2_ids;

                int start_idxs_diff = edge1_idxs.first - edge2_idxs.first;

                if (abs(start_idxs_diff) < 2)
                    continue;

                edge2_ids = make_pair(path[edge2_idxs.first], path[edge2_idxs.second]);

                TEdges edge_idxs;
                TEdges edge_ids;

                if (start_idxs_diff < 0)
                {
                    edge_idxs = make_pair(edge1_idxs, edge2_idxs);
                    edge_ids = make_pair(edge1_ids, edge2_ids);
                }
                else
                {
                    edge_idxs = make_pair(edge2_idxs, edge1_idxs);
                    edge_ids = make_pair(edge2_ids, edge1_ids);
                }

                moves.push_back(new EdgeMove(path_id, edge_idxs, edge_ids));
            }
        }
    }
}