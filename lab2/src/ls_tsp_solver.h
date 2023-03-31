#ifndef LS_TSP_SOLVER_H
#define LS_TSP_SOLVER_H

#include <string>

#include "tsp_solver.h"

using namespace std;

class LSSolver : public TSPSolver
{
public:
    virtual TPaths solve(TPaths start_solution) = 0;
    void set_neighbourhood(string);
    
protected:
    string neighbourhood;
};

#endif