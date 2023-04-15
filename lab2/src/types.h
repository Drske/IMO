#ifndef TYPES_H
#define TYPES_H

#include <utility>
#include <vector>

#define N 200

using namespace std;

typedef vector<int> TPath;
typedef pair<TPath, TPath> TPaths;
typedef pair<int,int> TPathLength;
typedef pair<int,int> TPathCost;

typedef pair<int,int> TVertex;
typedef TVertex TVertices[N];

typedef pair<int,int> TInsertionCost;
typedef vector<TInsertionCost> TInsertionCosts;

typedef pair<int,int> TEdge;
typedef pair<TEdge, TEdge> TEdges;

enum MoveState {
    APPLICABLE,
    NOT_APPLICABLE,
    FUTURE_APPLICABLE
};



#endif