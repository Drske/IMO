#include "re_tsp_solver.h"

bool RESolver::compare_costs(const pair<int, int> &a, const pair<int, int> &b)
{
    return a.second < b.second;
}

TInsertionCosts RESolver::calculate_insertion_costs(vector<int> path, int vertex_id)
{
    int cost;
    TInsertionCost insertion_cost;
    TInsertionCosts insertion_costs;

    for (int k = 0; k < path.size(); k++)
    {
        int v1 = path[k];
        int v2 = (k == path.size() - 1) ? path[0] : path[k + 1];

        // printf("Removing: path from %d to %d of cost %d\n", v1, v2, distance_matrix[v1][v2]);
        // printf("Adding paths from %d to %d and from %d to %d of costs: %d and %d\n", v1, vertex_id, vertex_id, v2, distance_matrix[v1][vertex_id], distance_matrix[vertex_id][v2]);

        cost = this->distance_matrix[v1][vertex_id] + this->distance_matrix[vertex_id][v2] - this->distance_matrix[v1][v2];

        insertion_cost = make_pair(k + 1 % path.size(), cost);
        insertion_costs.push_back(insertion_cost);
    }

    sort(insertion_costs.begin(), insertion_costs.end(), compare_costs);
    return insertion_costs;
}

TPaths RESolver::solve(int start_vertex)
{
    printf("Solving with regrets\n");

    // printf("Distance matrix:\n");

    // printf("\t");
    // for (int i = 0; i < N; i++)
    // {
    //     printf("%d\t", i);
    // }
    // printf("\n");

    // for (int i = 0; i < N; i++)
    // {
    //     printf("%d\t", i);
    //     for (int j = 0; j < N; j++)
    //     {
    //         // printf("From %d to %d is %d\n", i, j, distance_matrix[i][j]);
    //         printf("%d\t", distance_matrix[i][j]);
    //     }
    //     printf("\n");
    // }

    int start_vertex2 = this->find_furthest_vertex(start_vertex);

    this->add_vertex_to_path(1, start_vertex);
    this->add_vertex_to_path(1, find_nearest_vertex(start_vertex), 1);
    this->path_cost.first = distance_matrix[start_vertex][find_nearest_vertex(start_vertex)];
    this->add_vertex_to_path(2, start_vertex2);
    this->add_vertex_to_path(2, find_nearest_vertex(start_vertex2), 1);
    this->path_cost.second = distance_matrix[start_vertex2][find_nearest_vertex(start_vertex2)];

    while (this->path_length.first + this->path_length.second < N)
    {
        // printf("Current paths:\n");
        // for (auto p : this->paths.first)
        // {
        //     printf("%d ", p);
        // }
        // printf("cost: %d", this->path_cost.first);
        // printf("\n");
        // for (auto p : this->paths.second)
        // {
        //     printf("%d ", p);
        // }
        // printf("cost: %d", this->path_cost.second);
        // printf("\n");

        for (int i = 0; i < 2; i++)
        {
            vector<int> path = (i == 0) ? this->paths.first : this->paths.second;

            int best_cost = -1;
            int best_vertex = -1;
            int best_placement = -1;
            int biggest_regret = INT_MIN;
            int regret;
            TInsertionCosts insertion_costs;

            for (int j = 0; j < N; j++)
            {
                if (this->used_vertices[j])
                    continue;

                // printf("Checking insrtion of %d to path %d\n", j, i);

                insertion_costs = this->calculate_insertion_costs(path, j);

                // for (auto p : insertion_costs)
                // {
                //     printf("Place: %d, cost: %d\n", p.first, p.second);
                // }

                regret = (insertion_costs.size() == 1) ? insertion_costs[0].second : insertion_costs[1].second - insertion_costs[0].second;

                if ((regret > biggest_regret) || (regret == biggest_regret && insertion_costs[0].second < best_cost))
                {
                    best_vertex = j;
                    best_placement = insertion_costs[0].first;
                    biggest_regret = regret;
                    best_cost = insertion_costs[0].second;
                }
            }

            this->add_vertex_to_path(i + 1, best_vertex, best_placement);

            if (i == 0)
            {
                this->path_cost.first += best_cost;
            }
            else
            {
                this->path_cost.second += best_cost;
            }
        }
    }

    // printf("Adding last costs:\n");
    // printf("Between %d and %d of %d\n", paths.first[0], paths.first[N / 2 - 1], distance_matrix[paths.first[0]][paths.first[N / 2 - 1]]);
    // printf("Between %d and %d of %d\n", paths.second[0], paths.second[N / 2 - 1], distance_matrix[paths.second[0]][paths.second[N / 2 - 1]]);
    this->path_cost.first += distance_matrix[paths.first[0]][paths.first[N / 2 - 1]];
    this->path_cost.second += distance_matrix[paths.second[0]][paths.second[N / 2 - 1]];
    this->add_vertex_to_path(1, paths.first[0]);
    this->add_vertex_to_path(2, paths.second[0]);

    return this->paths;
}
