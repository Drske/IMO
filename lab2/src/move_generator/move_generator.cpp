#include "../moves/edge_move.h"
#include "../moves/vertex_move.h"
#include "move_generator.h"
#include <queue>

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
            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
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

            edge2_ids = make_pair(path[edge2_idxs.first], path[edge1_idxs.second]);

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
            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
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

        for (int j = 0; j < path.size(); j++)
        {
            if (i == j)
            {
                continue;
            }

            // Save neighbour as idx in path
            nearest_neighbours.push(make_pair(j, distance_matrix[path[i]][path[j]]));
        }

        while (!nearest_neighbours.empty() && candidates_no < max_candidates)
        {
            int j = nearest_neighbours.top().first;
            nearest_neighbours.pop();

            if (i > j)
            {
                continue;
            }

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

            TEdge edge1_idxs(i, succ_i);
            TEdge edge1_ids(p, succ_p);
            TEdge edge2_idxs(j, succ_j);
            TEdge edge2_ids(q, succ_q);

            TEdge edge3_idxs(pred_i, i);
            TEdge edge3_ids(pred_p, p);
            TEdge edge4_idxs(pred_j, j);
            TEdge edge4_ids(pred_q, q);

            TEdges from_succ_idxs(edge1_idxs, edge2_idxs);
            TEdges from_succ_ids(edge1_ids, edge2_ids);

            TEdges from_pred_idxs(edge3_idxs, edge4_idxs);
            TEdges from_pred_ids(edge3_ids, edge4_ids);

            moves.push_back(new EdgeMove(path_id, from_succ_idxs, from_succ_ids));
            moves.push_back(new EdgeMove(path_id, from_pred_idxs, from_pred_ids));
        }
    }
}

void MoveGenerator::add_candidate_vertex_moves_from_paths(TPaths paths, vector<Move *> &moves, int distance_matrix[][N], int max_candidates)
{
    // add_vertex_moves_from_paths(paths, moves);

    pair<int, int> path_ids(0, 1);

    // for (int i = 0; i < paths.first.size(); i++)
    // {
    //     for (int j = 0; j < paths.second.size(); j++)
    //     {
    //         pair<int, int> vertex_idxs(i, j);
    //         pair<int, int> vertex_ids(paths.first[i], paths.second[j]);
    //         moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
    //     }
    // }

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
            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
        }
    }
}
