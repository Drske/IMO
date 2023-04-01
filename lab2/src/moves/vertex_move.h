#ifndef VERTEX_SOLVER_H
#define VERTEX_SOLVER_H

#include "move.h"

using namespace std;

class VertexMove : public Move
{
public:
    VertexMove(pair<int, int> path_ids, pair<int, int> vertex_ids);
    void apply(TPaths* paths);
    void print();
    TPathCost get_cost_delta(TPaths paths);
private:
    pair<int, int> path_ids;
    pair<int, int> vertex_ids;
};

#endif
