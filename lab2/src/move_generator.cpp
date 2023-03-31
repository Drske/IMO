#include "edge_move.h"
#include "vertex_move.h"
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

void MoveGenerator::add_vertex_moves_from_path(vector<int> path, int path_id, vector<Move*>& moves) {
    pair<int, int> path_ids(path_id, path_id);
    for (int i = 0; i < path.size() - 1; i++) {
        for (int j = i + 1; j < path.size(); j++) {
            pair<int, int> vertex_ids(i, j);
            moves.push_back(new VertexMove(path_ids, vertex_ids));
        }
    }
}

void MoveGenerator::add_edge_moves_from_path(vector<int> path, int path_id, vector<Move*>& moves) {
    for (int i = 0; i < path.size() - 1; i++) {
        TEdge edge1_ids(i, i + 1);
        
        for (int j = i; j < path.size(); j++) {
            TEdge edge2_ids;

            if (j == (path.size() - 1))
                edge2_ids = make_pair(j, 0);
            else
                edge2_ids = make_pair(j, j + 1);

            TEdges edge_ids(edge1_ids, edge2_ids);

            moves.push_back(new EdgeMove(path_id, edge_ids));
        }
    }
}

void MoveGenerator::add_vertex_moves_from_paths(TPaths paths, vector<Move*>& moves) {
    pair<int, int> path_ids(0, 1);
    for (int i = 0; i < paths.first.size(); i++) {
        for (int j = 0; j < paths.second.size(); j++) {
            pair<int, int> vertex_ids(i, j);
            moves.push_back(new VertexMove(path_ids, vertex_ids));
        }
    }
}
