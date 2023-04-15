#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "../types.h"
#include "../moves/move.h"

class MoveGenerator
{
    public:
        static vector<Move*> get_first_neighbourhood_moves(TPaths paths);
        static vector<Move*> get_second_neighbourhood_moves(TPaths paths);
        static vector<Move*> get_second_neighbourhood_candidate_moves(TPaths paths, int distance_matrix[][N], int max_candidates);
    private:
        static void add_vertex_moves_from_path(TPath path, int path_id, vector<Move*>& moves);
        static void add_edge_moves_from_path(TPath path, int path_id, vector<Move*>& moves);
        static void add_vertex_moves_from_paths(TPaths paths, vector<Move*>& moves);

        static void add_candidate_edge_moves_from_path(TPath path, int path_id, vector<Move*>& moves, int distance_matrix[][N], int max_candidates);
        static void add_candidate_vertex_moves_from_paths(TPaths paths, vector<Move*>& moves, int distance_matrix[][N], int max_candidates);
};

#endif