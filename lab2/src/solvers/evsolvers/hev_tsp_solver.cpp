#include <cstdlib>
#include <ctime>
#include <random>

#include "../../types.h"
#include "hev_tsp_solver.h"

using namespace std;

TPaths HEVSolver::solve()
{
    printf("Solving Hybrid Evolutionary Algorithm\n");
    this->population_max_size = 20;
    this->initialise_population();

    for (int k = 0; k < this->iterations; k++)
    {
        printf("-= GENERATION NO %d=-\n", k);
        TSolutions parents = this->select_parents(2, 5);                           // Tournament selection
        TSolutions kids = this->recombine(parents);                                 // Crossover
        kids = this->mutate(kids, 0.5);                                             // Mutation of new solutions
        this->population = this->select_next_generation(this->population, kids);
    }

    int best_cost = INT_MAX;

    for (int i = 0; i < this->population_max_size; i++)
    {
        if (this->population[i].second.first + this->population[i].second.second < best_cost)
        {
            this->paths = population[i].first;
            this->path_cost = population[i].second;
            best_cost = this->population[i].second.first + this->population[i].second.second;
        }
    }

    return this->paths;
}

void HEVSolver::initialise_population()
{
    this->population.clear();
    int sv = this->start_vertex;

    while (this->population.size() < this->population_max_size)
    {
        this->init_sol_gen->clear_paths();
        this->init_sol_gen->set_path_length(make_pair(0, 0));
        this->init_sol_gen->set_initial_cost(make_pair(0, 0));
        this->init_sol_gen->set_start_vertex(sv);

        // Generate Random Solution
        TPaths initial_solution = this->init_sol_gen->solve();
        TPathCost initial_cost = this->init_sol_gen->get_cost();

        // Solve with LS
        this->local_search_solver->set_initial_solution(initial_solution);
        this->local_search_solver->set_initial_cost(initial_cost);

        TPaths local_search_solution = this->local_search_solver->solve();
        TPathCost local_search_cost = this->local_search_solver->get_cost();

        bool sol_in_population = false;

        for (int i = 0; i < this->population.size(); i++)
        {
            if (this->population[i].second.first + this->population[i].second.second == local_search_cost.first + local_search_cost.second)
            {
                sol_in_population = true;
            }
        }

        if (!sol_in_population)
        {
            printf("[INIT] (%d): Adding a solution: %d\n", this->population.size(), local_search_cost.first + local_search_cost.second);
            this->population.push_back(make_pair(local_search_solution, local_search_cost));
        }

        sv += 5;
    }
}

TSolutions HEVSolver::select_parents(int k, int n)
{
    vector<int> indices;
    set<int> parent_indices;

    for (int i = 0; i < this->population_max_size; ++i)
    {
        indices.push_back(i);
    }

    random_device rd;
    mt19937 generator(rd());

    while (parent_indices.size() < n)
    {
        shuffle(indices.begin(), indices.end(), generator);
        vector<int> selected_indices(indices.begin(), indices.begin() + k);

        int best_cost = INT_MAX;
        int best_idx = -1;

        for (int i = 0; i < k; i++)
        {
            int idx = selected_indices[i];
            if (this->population[idx].second.first + this->population[idx].second.second < best_cost)
            {
                best_idx = idx;
                best_cost = this->population[idx].second.first + this->population[idx].second.second;
            }
        }
        parent_indices.insert(best_idx);
    }

    TSolutions parents;

    for (auto idx : parent_indices)
    {
        parents.push_back(this->population[idx]);

        // printf("Adding parent: %d\n", idx);
    }

    return parents;
}

TSolutions HEVSolver::recombine(TSolutions parents)
{
    TSolutions kids;
    TSolution input, output;

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, 1);

    for (int i = 0; i < parents.size(); i++)
    {
        for (int j = i + 1; j < parents.size(); j++)
        {
            if (dist(rng) == 0)
            {
                input = parents[i];
                output = parents[j];
            }
            else
            {
                input = parents[j];
                output = parents[i];
            }
            // printf("-= RECOMBINATION OF %d, %d =-\n", i, j);
            kids.push_back(recombine_one(input, output));
        }
    }

    return kids;
}

void HEVSolver::gather_edges(TPath cycle, vector<pair<int, int>> &edges)
{
    for (int i = 0; i < cycle.size(); i++)
    {
        int idx = i;
        int p_idx = (i + 1) % (N / 2);

        edges.push_back(make_pair(cycle[idx], cycle[p_idx]));
    }
}

TSolution HEVSolver::recombine_one(TSolution input, TSolution output)
{
    // printf("OLD RECOMBINATION COST: %d, %d\n", input.second.first + input.second.second, output.second.first + output.second.second);
    vector<pair<int, int>> input_edges;
    vector<pair<int, int>> output_edges;
    vector<pair<int, int>> matched_edges;
    set<pair<int, int>> cycles_edges;

    gather_edges(input.first.first, input_edges);
    gather_edges(input.first.second, input_edges);
    gather_edges(output.first.first, output_edges);
    gather_edges(output.first.second, output_edges);

    for (const auto &output_edge : output_edges)
    {
        bool found = false;
        for (const auto &input_edge : input_edges)
        {
            if ((output_edge.first == input_edge.first && output_edge.second == input_edge.second) ||
                (output_edge.first == input_edge.second && output_edge.second == input_edge.first))
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            matched_edges.push_back(output_edge);
        }
    }

    // Add edges from first cycle to the set
    for (size_t i = 0; i < output.first.first.size() - 1; ++i)
    {
        cycles_edges.insert({output.first.first[i], output.first.first[i + 1]});
        cycles_edges.insert({output.first.first[i + 1], output.first.first[i]});
    }

    // Add edges from second cycle to the set
    for (size_t i = 0; i < output.first.second.size() - 1; ++i)
    {
        cycles_edges.insert({output.first.second[i], output.first.second[i + 1]});
        cycles_edges.insert({output.first.second[i + 1], output.first.second[i]});
    }

    // Vector to store the vertices to be removed
    set<int> vertices_to_remove;

    // Iterate over each vertex in the cycle edges
    for (const auto &edge : cycles_edges)
    {
        int vertex = edge.first;

        // Check if the vertex should be removed
        if (none_of(matched_edges.begin(), matched_edges.end(), [vertex](const pair<int, int> &matched_edge)
                    { return matched_edge.first == vertex || matched_edge.second == vertex; }))
        {
            vertices_to_remove.insert(vertex);
        }
    }

    output.first.first.erase(
        remove_if(output.first.first.begin(), output.first.first.end(), [&vertices_to_remove](int vertex)
                  { return vertices_to_remove.count(vertex) > 0; }),
        output.first.first.end());

    output.first.second.erase(
        remove_if(output.first.second.begin(), output.first.second.end(), [&vertices_to_remove](int vertex)
                  { return vertices_to_remove.count(vertex) > 0; }),
        output.first.second.end());

    this->constructive_solver->set_initial_solution(output.first);
    this->constructive_solver->reset_used_vertices();
    this->constructive_solver->reset_path_lengths();
    this->constructive_solver->reset_path_cost();

    TPaths cycles = this->constructive_solver->solve();
    TPathCost cost = this->constructive_solver->get_cost();

    if (this->use_ls_to_repair)
    {
        this->local_search_solver->set_initial_solution(cycles);
        this->local_search_solver->set_initial_cost(cost);

        cycles = this->local_search_solver->solve();
        cost = this->local_search_solver->get_cost();
    }

    // printf("NEW RECOMBINATION COST: %d\n", cost.first + cost.second);
    return make_pair(cycles, cost);
}

TSolutions HEVSolver::mutate(TSolutions solutions, float prob)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0; i < solutions.size(); i++)
    {
        if (dist(gen) < prob)
        {
            solutions[i] = this->mutate_one(solutions[i]);
        }
    }

    return solutions;
}

TSolution HEVSolver::swap_random_vertices(TSolution solution) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> dis(0, N/2 - 1);

    int index1 = dis(gen);
    int index2 = dis(gen);

    swap(solution.first.first[index1], solution.first.second[index2]);

    return solution;
}

TSolution HEVSolver::mutate_one(TSolution solution)
{
    // printf("[MUTA] OLD COST: %d\n", solution.second.first + solution.second.second);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, N / 2 - 2);

    int l = dist(gen);
    int k = dist(gen);

    if (l > k)
    {
        swap(l, k);
    }

    // printf("[MUTA] Ereasing from %d to %d\n", l, k);
    solution.first.first.erase(solution.first.first.begin() + l, solution.first.first.begin() + k + 1);

    l = dist(gen);
    k = dist(gen);

    if (l > k)
    {
        swap(l, k);
    }

    // printf("[MUTA] Ereasing from %d to %d\n", l, k);
    solution.first.second.erase(solution.first.second.begin() + l, solution.first.second.begin() + k + 1);

    // if (this->use_ls_to_repair)
    // {
    //     for (int i = 0; i < 10; i++)
    //     {
    //         solution = swap_random_vertices(solution);
    //     }
    // }

    this->constructive_solver->set_initial_solution(solution.first);
    this->constructive_solver->reset_used_vertices();
    this->constructive_solver->reset_path_lengths();
    this->constructive_solver->reset_path_cost();

    TPaths cycles = this->constructive_solver->solve();
    TPathCost cost = this->constructive_solver->get_cost();

    if (this->use_ls_to_repair)
    {
        this->local_search_solver->set_initial_solution(cycles);
        this->local_search_solver->set_initial_cost(cost);

        cycles = this->local_search_solver->solve();
        cost = this->local_search_solver->get_cost();
    }

    // printf("[MUTA] NEW COST: %d\n", cost.first + cost.second);

    return make_pair(cycles, cost);
}

TSolutions HEVSolver::remove_duplicates(TSolutions population)
{
    // printf("POPULATION SIZE BEFORE RM DUP: %d\n", population.size());
    // Function to calculate the sum of TPathCost
    auto sum_path_cost = [](const TSolution &solution)
    {
        return solution.second.first + solution.second.second;
    };

    // Sort the population based on the sum of TPathCost
    sort(population.begin(), population.end(),
         [sum_path_cost](const TSolution &a, const TSolution &b)
         {
             return sum_path_cost(a) < sum_path_cost(b);
         });

    // Remove duplicates
    population.erase(unique(population.begin(), population.end(),
                            [sum_path_cost](const TSolution &a, const TSolution &b)
                            {
                                return sum_path_cost(a) == sum_path_cost(b);
                            }),
                     population.end());

    // printf("POPULATION SIZE AFTER RM DUP: %d\n", population.size());
    return population;
}

TSolutions HEVSolver::pass_best_solution(TSolutions population, TSolution kid)
{
    int worst_cost = population[0].second.first + population[0].second.second;
    int worst_idx = 0;
    bool kid_is_duplicate = false;

    // Get worst solution and check if kid is a duplicate
    for (int i = 0; i < population.size(); i++)
    {
        if (population[i].second.first + population[i].second.second == kid.second.first + kid.second.second)
        {
            kid_is_duplicate = true;
            break;
        }

        if (population[i].second.first + population[i].second.second > worst_cost)
        {
            worst_cost = population[i].second.first + population[i].second.second;
            worst_idx = i;
        }
    }

    // Do not pass kid as it is a duplicate
    if (kid_is_duplicate)
    {
        printf("[PASS] Ommiting pass as kid is the duplicate\n");
        return population;
    }

    // Do not pass kid if it is worse than worst solution
    if (kid.second.first + kid.second.second > worst_cost)
    {
        printf("[PASS] Kid gave improvement worse improvement than worst solution\n");
        return population;
    }

    // Pass kid in place of worst solution
    printf("[PASS] Replacing vertex %d (%d) with kid (%d)\n", worst_idx, population[worst_idx].second.first + population[worst_idx].second.second, kid.second.first + kid.second.second);
    population[worst_idx] = kid;

    return population;
}



TSolutions HEVSolver::select_next_generation(TSolutions old_population, TSolutions kids)
{
    for (int i = 0; i < kids.size(); i++){
        old_population = pass_best_solution(old_population, kids[i]);
    }

    for (int i = 0; i < old_population.size(); i++)
    {
        printf("(NP %d) %d\n", i, old_population[i].second.first + old_population[i].second.second);
    }

    return old_population;
}