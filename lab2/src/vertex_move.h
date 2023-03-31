#ifndef VERTEX_SOLVER_H
#define VERTEX_SOLVER_H

#include "move.h"

using namespace std;

class VertexMove : public Move
{
public:
    VertexMove(pair<int, int> path_ids, pair<int, int> vertex_ids);
    void apply(TPaths* paths) = 0;
    pair<int, int> get_cost_delta(TPaths paths) = 0;
private:
    pair<int, int> path_ids;
    pair<int, int> vertex_ids;
};

#endif
