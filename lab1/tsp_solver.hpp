#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include <utility>;

using namespace std;

#define N 100

typedef pair<int[N/2], int[N/2]> TPaths;

class TSPSolver
{
public:
    TSPSolver();
    virtual TPaths solve() = 0;
private:
};

#endif