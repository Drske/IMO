#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "types.h"
#include "move.h"

class MoveGenerator
{
    public:
        static vector<Move*> get_first_neighbourhood_moves(TPaths paths);
        static vector<Move*> get_second_neighbourhood_moves(TPaths paths);
};

#endif