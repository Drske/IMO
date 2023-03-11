#include <iostream>
#include <stdlib.h>
#include <map>

#include "tsp_solver.h"
#include "nn_tsp_solver.h"
#include "gc_tsp_solver.h"
#include "re_tsp_solver.h"

using namespace std;

int main(int argc, char** argv) {

    map<string, TSPSolver> solvers;
    
    solvers["nearest-neighbour"] = NNSolver();
    solvers["greedy-cycle"] = GCSolver();
    solvers["regrets"] = RESolver();

    return 0;
}
