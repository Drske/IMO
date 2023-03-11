#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include <utility>
#include <vector>

#define N 100

using namespace std;

// typedef pair<int[N/2], int[N/2]> TPaths;
typedef vector<pair<int,int> > TPaths;

class TSPSolver
{
public:
    TPaths solve();
    void load_data(int [][N]);
    void add_path(int, int);

protected:
    int distance_matrix[N][N];
    TPaths paths;
    int no_of_paths;
};

#endif
