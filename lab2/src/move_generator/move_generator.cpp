#include "../moves/edge_move.h"
#include "../moves/vertex_move.h"
#include "move_generator.h"

vector<Move*> MoveGenerator::get_first_neighbourhood_moves(TPaths paths) {
    // Wymiana wierzchołków wewnątrz + wymiana wierzchołków pomiędzy cyklami
    vector<Move*> moves;

    add_vertex_moves_from_path(paths.first, 0, moves);
    add_vertex_moves_from_path(paths.second, 1, moves);
    add_vertex_moves_from_paths(paths, moves);

    return moves;
}

vector<Move*> MoveGenerator::get_second_neighbourhood_moves(TPaths paths) {
    // Wymiana krawędzi wewnątrz + wymiana wierzchołków pomiędzy cyklami
    vector<Move*> moves;

    add_edge_moves_from_path(paths.first, 0, moves);
    add_edge_moves_from_path(paths.second, 1, moves);
    add_vertex_moves_from_paths(paths, moves);

    return moves;
}

void MoveGenerator::add_vertex_moves_from_path(TPath path, int path_id, vector<Move*>& moves) {
    pair<int, int> path_ids(path_id, path_id);
    for (int i = 0; i < path.size() - 1; i++) {
        for (int j = i + 1; j < path.size(); j++) {
            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(path[i], path[j]);
            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
        }
    }
}

void MoveGenerator::add_edge_moves_from_path(TPath path, int path_id, vector<Move*>& moves) {
    for (int i = 0; i < path.size() - 2; i++) {
        TEdge edge1_idxs(i, i + 1);
        TEdge edge1_ids(path[i], path[i + 1]);

        for (int j = i + 2; j < path.size(); j++) {
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

void MoveGenerator::add_vertex_moves_from_paths(TPaths paths, vector<Move*>& moves) {
    pair<int, int> path_ids(0, 1);
    for (int i = 0; i < paths.first.size(); i++) {
        for (int j = 0; j < paths.second.size(); j++) {
            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(paths.first[i], paths.second[j]);
            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
        }
    }
}

vector<Move*> MoveGenerator::get_new_moves_after_move(TPaths paths, VertexMove* vertexMove) {
    pair<int, int> path_ids = vertexMove->get_path_ids();
    pair<int, int> vertex_idxs = vertexMove->get_vertex_idxs();

    pair<vector<int>, vector<int>> paths_modified_idxs;
    vector<Move*> moves;

    if (path_ids.first == path_ids.second && path_ids.second == 0) {
        paths_modified_idxs.first.push_back(vertex_idxs.first);
        paths_modified_idxs.first.push_back(vertex_idxs.second);

        add_edge_moves_after_move(paths.first, path_ids.first, paths_modified_idxs.first, moves);
    } else if (path_ids.first == path_ids.second && path_ids.second == 1) {
        paths_modified_idxs.second.push_back(vertex_idxs.first);
        paths_modified_idxs.second.push_back(vertex_idxs.second);

        add_edge_moves_after_move(paths.second, path_ids.second, paths_modified_idxs.second, moves);
    } else if (path_ids.first == 0 && path_ids.second == 1) {
        paths_modified_idxs.first.push_back(vertex_idxs.first);
        paths_modified_idxs.second.push_back(vertex_idxs.second);

        add_edge_moves_after_move(paths.first, path_ids.first, paths_modified_idxs.first, moves);
        add_edge_moves_after_move(paths.second, path_ids.second, paths_modified_idxs.second, moves);
    }

    add_vertex_moves_after_move(paths, paths_modified_idxs, moves);

    return moves;
}

vector<Move*> MoveGenerator::get_new_moves_after_move(TPaths paths, EdgeMove* edgeMove) {
    int path_id = edgeMove->get_path_id();
    TPath path = (path_id == 0) ? paths.first : paths.second;
    TEdges edge_idxs = edgeMove->get_edge_idxs();
    vector<int> path_modified_idxs = {edge_idxs.first.first, edge_idxs.first.second,
                                      edge_idxs.second.first, edge_idxs.second.second};
    pair<vector<int>, vector<int>> paths_modified_idxs;

    if (path_id == 0)
        paths_modified_idxs.first = path_modified_idxs;
    else
        paths_modified_idxs.second = path_modified_idxs;

    vector<Move*> moves;

    // 2nd neighbourhood
    add_edge_moves_after_move(path, path_id, path_modified_idxs, moves);
    add_vertex_moves_after_move(paths, paths_modified_idxs, moves);

    return moves;
}

void MoveGenerator::add_vertex_moves_after_move(TPaths paths, pair<vector<int>, vector<int>> paths_modified_idxs, vector<Move*>& moves) {
    pair<int, int> path_ids(0, 1);

    for (int i : paths_modified_idxs.first) {
        for (int j = 0; j < paths.second.size(); j++) {
            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(paths.first[i], paths.second[j]);

            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
        }
    }

    for (int i = 0; i < paths.first.size(); i++) {
        for (int j : paths_modified_idxs.second) {
            pair<int, int> vertex_idxs(i, j);
            pair<int, int> vertex_ids(paths.first[i], paths.second[j]);

            moves.push_back(new VertexMove(path_ids, vertex_idxs, vertex_ids));
        }
    }
}

void MoveGenerator::add_edge_moves_after_move(TPath path, int path_id, vector<int> path_modified_idxs, vector<Move*>& moves) {
    for (int i : path_modified_idxs) {
        TEdge edge1_prev_idxs(i - 1, i);
        TEdge edge1_next_idxs(i, i + 1);

        if (i == 0)
            edge1_prev_idxs = make_pair(path.size() - 1, i);
        else if (i == (path.size() - 1))
            edge1_next_idxs = make_pair(i, 0);
        
        vector<TEdge> edges1_idxs = {edge1_next_idxs, edge1_next_idxs};

        for (TEdge edge1_idxs : edges1_idxs) {
            TEdge edge1_ids(path[edge1_idxs.first], path[edge1_idxs.second]);

            for (int j = 0; j < path.size(); j++) {
                TEdge edge2_idxs;
                TEdge edge2_ids;

                if (j == (path.size() - 1))
                    edge2_idxs = make_pair(j, 0);
                else
                    edge2_idxs = make_pair(j, j + 1);

                int start_idxs_diff = edge1_idxs.first - edge2_idxs.first;

                if (abs(start_idxs_diff) < 2)
                    continue;

                edge2_ids = make_pair(path[edge2_idxs.first], path[edge2_idxs.second]);

                TEdges edge_idxs;
                TEdges edge_ids;

                if (start_idxs_diff < 0) {
                    edge_idxs = make_pair(edge1_idxs, edge2_idxs);
                    edge_ids = make_pair(edge1_ids, edge2_ids);
                } else {
                    edge_idxs = make_pair(edge2_idxs, edge1_idxs);
                    edge_ids = make_pair(edge2_ids, edge1_ids);
                }

                moves.push_back(new EdgeMove(path_id, edge_idxs, edge_ids));
            }
        }
    }
}