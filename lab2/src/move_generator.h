#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "types.h"
#include "move.h"

class MoveGenerator
{
    public:
        static vector<Move*> get_first_neighbourhood_moves(TPaths paths);
        static vector<Move*> get_second_neighbourhood_moves(TPaths paths);
    private:
        static void add_vertex_moves_from_path(vector<int> path, int path_id, vector<Move*>& moves);
        static void add_edge_moves_from_path(vector<int> path, int path_id, vector<Move*>& moves);
        static void add_vertex_moves_from_paths(TPaths paths, vector<Move*>& moves);
};

#endif