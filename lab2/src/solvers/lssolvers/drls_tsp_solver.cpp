#include <cstdlib>
#include <ctime>
#include <random>

#include "../../types.h"
#include "drls_tsp_solver.h"

typedef pair<TPaths, TPathCost> TSolution;

using namespace std;

TPaths DRLSSolver::solve()
{
    printf("Solving Destroy Repair Large Scale Local Search\n");

    // this->init_sol_gen->set_initial_solution(this->paths);
    // this->init_sol_gen->set_initial_cost(this->path_cost);
    // this->paths = this->init_sol_gen->solve();

    // this->path_cost = this->init_sol_gen->get_cost();

    this->path_length.first = N / 2;
    this->path_length.second = N / 2;

    for (int j = 0; j < this->iterations; j++)
    {
        TPaths destroyed_cycles = this->destroy();
        TSolution repaired_solution = this->repair(destroyed_cycles);

        if (repaired_solution.second.first + repaired_solution.second.second < this->path_cost.first + this->path_cost.second)
        {
            this->paths = repaired_solution.first;
            this->path_cost = repaired_solution.second;

            // printf("Result improved to: (%d, %d)\n", this->path_cost.first, this->path_cost.second);
        }
    }

    return this->paths;
}

TPaths DRLSSolver::destroy()
{
    int vertices_to_remove = 0.2 * N;
    int vertices_removed = 0;

    TPaths cycles = this->paths;

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist1(0, 1);
    uniform_int_distribution<int> dist2(0, N / 2 - 1);
    uniform_int_distribution<int> dist3(5, 20);

    while (vertices_removed < vertices_to_remove)
    {
        vertices_removed = 0;
        int path_idx = dist1(rng);
        int vertex_idx = dist2(rng);
        int path_length = dist3(rng);

        if (path_idx == 0 && cycles.first[vertex_idx] == -1)
            continue;

        if (path_idx == 1 && cycles.second[vertex_idx] == -1)
            continue;

        // printf("Removing from path %d, starting from %d, length %d\n", path_idx, vertex_idx, path_length);
        // printf("Before removal:\n");
        // printf("path 0:");
        // for (int i = 0; i < cycles.first.size(); i++)
        //     printf("%d ", cycles.first[i]);
        // printf("\n");
        // printf("path 1:");
        // for (int i = 0; i < cycles.second.size(); i++)
        //     printf("%d ", cycles.second[i]);
        // printf("\n");

        cycles = this->remove_path_from_cycle(cycles, path_idx, vertex_idx, path_length);

        // printf("After removal:\n");
        // printf("path 0: ");
        // for (int i = 0; i < cycles.first.size(); i++)
        //     printf("%d ", cycles.first[i]);
        // printf("\n");
        // printf("path 1: ");
        // for (int i = 0; i < cycles.second.size(); i++)
        //     printf("%d ", cycles.second[i]);
        // printf("\n");

        for (int i = 0; i < N / 2; i++)
        {
            vertices_removed += (cycles.first[i] == -1) ? 1 : 0;
            vertices_removed += (cycles.second[i] == -1) ? 1 : 0;
        }

        // printf("Vertices to remove: %d, vertices_removed: %d\n", vertices_to_remove, vertices_removed);
    }

    // printf("Closing solution\n");
    cycles = this->close_cycles(cycles);

    return cycles;
}

TSolution DRLSSolver::repair(TPaths cycles)
{
    // printf("-=Solution passed to GC=-\n");
    // printf("path 0: ");
    // for (int i = 0; i < cycles.first.size(); i++)
    //     printf("%d ", cycles.first[i]);
    // printf("\n");
    // printf("path 1: ");
    // for (int i = 0; i < cycles.second.size(); i++)
    //     printf("%d ", cycles.second[i]);
    // printf("\n");
    // printf("Path lengths before GC: %d, %d\n", cycles.first.size(), cycles.second.size());
    this->constructive_solver->set_initial_solution(cycles);
    this->constructive_solver->reset_used_vertices();
    this->constructive_solver->reset_path_lengths();
    this->constructive_solver->reset_path_cost();

    TPaths solution = this->constructive_solver->solve();
    TPathCost cost = this->constructive_solver->get_cost();

    // printf("-=Solution after GC=-\n");
    // printf("path 0: ");
    // for (int i = 0; i < solution.first.size(); i++)
    //     printf("%d ", solution.first[i]);
    // printf("\n");
    // printf("path 1: ");
    // for (int i = 0; i < solution.second.size(); i++)
    //     printf("%d ", solution.second[i]);
    // printf("\n");
    // printf("Cost: %d, %d\n", cost.first, cost.second);
    // printf("Path lengths after GC: %d, %d\n", solution.first.size(), solution.second.size());

    // this->local_search_solver->set_initial_solution(solution);
    // this->local_search_solver->reset_path_lengths();
    // this->local_search_solver->reset_path_cost();

    // solution = this->local_search_solver->solve();
    // cost = this->local_search_solver->get_cost();

    // printf("-=Solution after LS=-\n");
    // printf("path 0: ");
    // for (int i = 0; i < solution.first.size(); i++)
    //     printf("%d ", solution.first[i]);
    // printf("\n");
    // printf("path 1: ");
    // for (int i = 0; i < solution.second.size(); i++)
    //     printf("%d ", solution.second[i]);
    // printf("\n");
    // printf("Cost: %d, %d\n", cost.first, cost.second);
    // printf("Path lengths after LS: %d, %d\n", solution.first.size(), solution.second.size());

    return make_pair(solution, cost);
}

TPaths DRLSSolver::remove_path_from_cycle(TPaths cycles, int path_idx, int first_vertex_idx, int length)
{
    TPath cycle;
    int c_idx = first_vertex_idx;
    int n_idx;

    if (path_idx == 0)
    {
        cycle = cycles.first;
    }
    else
    {
        cycle = cycles.second;
    }

    for (int i = 0; i < length - 1; i++)
    {
        n_idx = (c_idx + 1) % (N / 2);

        if (cycle[n_idx] == -1)
        {
            break;
        }

        cycle[c_idx] = -1;
        c_idx = (c_idx + 1) % (N / 2);
    }

    if (path_idx == 0)
    {
        cycles.first = cycle;
    }
    else
    {
        cycles.second = cycle;
    }

    return cycles;
}

TPaths DRLSSolver::close_cycles(TPaths cycles)
{
    TPaths closed_cycles;

    for (int i = 0; i < cycles.first.size(); i++)
    {
        if (cycles.first[i] != -1)
        {
            closed_cycles.first.push_back(cycles.first[i]);
        }
    }

    for (int i = 0; i < cycles.second.size(); i++)
    {
        if (cycles.second[i] != -1)
        {
            closed_cycles.second.push_back(cycles.second[i]);
        }
    }

    return closed_cycles;
}
