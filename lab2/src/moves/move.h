#ifndef MOVE_SOLVER_H
#define MOVE_SOLVER_H

#include "../types.h"

using namespace std;

class Move
{
public:
    virtual ~Move() {}
    virtual void apply(TPaths* paths) = 0;
    virtual TPathCost get_cost_delta(TPaths paths) = 0;
    virtual void print() = 0;
};

#endif
