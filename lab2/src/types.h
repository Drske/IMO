#ifndef TYPES_H
#define TYPES_H

#include <utility>
#include <vector>

#define N 100

using namespace std;

pair<int,int> operator-(const std::pair<int, int>& x, const std::pair<int, int>& y) {
    return std::make_pair(x.first-y.first, x.second-y.second);
}

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

#endif