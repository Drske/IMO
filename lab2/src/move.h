#ifndef MOVE_SOLVER_H
#define MOVE_SOLVER_H

#include "types.h"

using namespace std;

class Move
{
public:
    virtual void apply(TPaths* paths) = 0;
    virtual pair<int, int> get_cost_delta(TPaths paths) = 0;
};

#endif
