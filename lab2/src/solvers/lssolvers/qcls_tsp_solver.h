#ifndef QCLS_TSP_SOLVER_H
#define QCLS_TSP_SOLVER_H

#include "ls_tsp_solver.h"

class QCLSSolver : public LSSolver
{
    TPaths solve();
};

#endif