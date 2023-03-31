#ifndef EDGE_SOLVER_H
#define EDGE_SOLVER_H

#include "move.h"

using namespace std;

class EdgeMove : public Move
{
public:
    EdgeMove(int path_id, TEdges edge_ids);
    void apply(TPaths* paths);
    void print();
    TPathCost get_cost_delta(TPaths paths);
private:
    int path_id;
    TEdges edge_ids;
};

#endif
