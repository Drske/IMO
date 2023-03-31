#include "rls_tsp_solver.h"

TPaths RLSSolver::solve() {
    // TODO
    // No initial solution
    // Solve method is empty
    // First iteration generates fully random solution
    return this->paths;
}

void RLSSolver::set_iterations(int iterations){
    this->iterations = iterations;
}