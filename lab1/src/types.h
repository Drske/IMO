#ifndef TYPES_H
#define TYPES_H

#include <utility>
#include <vector>

#define N 100

using namespace std;

typedef pair<vector<int>, vector<int>> TPaths;
typedef pair<int,int> TPathLength;
typedef pair<int,int> TPathCost;

typedef pair<int,int> TVertex;
typedef TVertex TVertices[N];

#endif