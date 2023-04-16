#include <iostream>
#include <stdlib.h>
#include <map>
#include <string>
#include <fstream>
#include <math.h>
#include <random>
#include <chrono>
#include <nlohmann/json.hpp>

#include "solvers/tsp_solver.h"
#include "solvers/gc_tsp_solver.h"

#include "solvers/lssolvers/ls_tsp_solver.h"
#include "solvers/lssolvers/rls_tsp_solver.h"
#include "solvers/lssolvers/gls_tsp_solver.h"
#include "solvers/lssolvers/sls_tsp_solver.h"
#include "solvers/lssolvers/cls_tsp_solver.h"
#include "solvers/lssolvers/qls_tsp_solver.h"
#include "solvers/lssolvers/qcls_tsp_solver.h"

#include "move_generator/move_generator.h"

using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

bool cmd_option_provided(string option, int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i] == option)
        {
            return true;
        }
    }
    return false;
}

string get_cmd_option(string option, int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i] == option)
        {
            return argv[i + 1];
        }
    }
    return "";
}

int euclidean_distance(TVertex A, TVertex B)
{
    double u = A.first - B.first;
    double v = A.second - B.second;

    double dist = pow(u, 2) + pow(v, 2);
    dist = sqrt(dist);

    return round(dist);
}

void save_results_to_json(string data_path,
                          string output_path,
                          string solver_name,
                          string init_sol_gen_name,
                          string neighbourhood,
                          int start_vertex,
                          TPaths paths,
                          TPaths initial_solution,
                          TPathCost cost,
                          TPathCost initial_cost,
                          double duration,
                          int max_candidates)

{
    size_t pos = data_path.find_last_of("/");
    string instance = data_path.substr(pos + 1);

    json j;
    j["instance"] = instance;
    j["solver"] = solver_name;
    j["initial-solver"] = init_sol_gen_name;
    j["neighbourhood"] = neighbourhood;
    j["start-vertex"] = start_vertex;
    j["duration"] = duration;
    j["max-candidates"] = max_candidates;
    j["cost"] = {
        {"first", cost.first},
        {"second", cost.second}};
    j["initial-cost"] = {
        {"first", initial_cost.first},
        {"second", initial_cost.second}};
    j["path"] = {
        {"first", paths.first},
        {"second", paths.second}};
    j["initial-path"] = {
        {"first", initial_solution.first},
        {"second", initial_solution.second}};

    ofstream ofs(output_path);
    ofs << j.dump(4);
    ofs.close();
}

int main(int argc, char **argv)
{
    map<string, TSPSolver *> solvers;
    map<string, TSPSolver *> init_sol_gens;

    solvers["random-walk"] = new RLSSolver();
    solvers["greedy-ls"] = new GLSSolver();
    solvers["steepest-ls"] = new SLSSolver();
    solvers["queue-ls"] = new QLSSolver();
    solvers["candidate-ls"] = new CLSSolver();
    solvers["qc-ls"] = new QCLSSolver();
    solvers["greedy-cycle"] = new GCSolver();

    init_sol_gens["greedy-cycle"] = new GCSolver();
    init_sol_gens["random-walk"] = new RLSSolver();

    TSPSolver *solver, *init_sol_gen;

    string solver_name, init_sol_gen_name;
    string data_path;
    string output_path;
    string neighbourhood;

    int start_vertex, iterations, max_candidates;

    if (cmd_option_provided("-solver", argc, argv))
    {
        solver_name = get_cmd_option("-solver", argc, argv);
        solver = solvers[solver_name];
    }
    else
    {
        cout << "No solver provided";
        return 1;
    }
    if (cmd_option_provided("-init-sol-gen", argc, argv))
    {
        init_sol_gen_name = get_cmd_option("-init-sol-gen", argc, argv);
        init_sol_gen = init_sol_gens[init_sol_gen_name];
    }
    else
    {
        cout << "No initial solution generator provided";
        return 1;
    }
    if (cmd_option_provided("-neigh", argc, argv))
    {
        neighbourhood = get_cmd_option("-neigh", argc, argv);
    }
    else
    {
        cout << "No neighbourhood provided";
        return 1;
    }
    if (cmd_option_provided("-iterations", argc, argv))
    {
        iterations = stoi(get_cmd_option("-iterations", argc, argv));
    }
    else
    {
        iterations = 1;
    }
    if (cmd_option_provided("-start-vertex", argc, argv))
    {
        start_vertex = stoi(get_cmd_option("-start-vertex", argc, argv));
    }
    else
    {
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> uni(1, N);

        start_vertex = uni(rng);
    }
    if (cmd_option_provided("-max-candidates", argc, argv))
    {
        max_candidates = stoi(get_cmd_option("-max-candidates", argc, argv));
    }
    else
    {
        max_candidates = 10;
    }
    if (cmd_option_provided("-in", argc, argv))
    {
        data_path = get_cmd_option("-in", argc, argv);
    }
    else
    {
        cout << "No input provided";
        return 1;
    }

    if (cmd_option_provided("-out", argc, argv))
    {
        output_path = get_cmd_option("-out", argc, argv);
    }
    else
    {
        cout << "No output file provided";
        return 1;
    }

    ifstream file(data_path);
    string line;

    int header_lines = 6;
    int id, x, y;
    TVertices vertices;

    while (std::getline(file, line))
    {
        if (header_lines > 0)
        {
            header_lines--;
            continue;
        }
        if (line == "EOF")
        {
            break;
        }
        id = stoi(line.substr(0, line.find(" ")));
        line.erase(0, line.find(" ") + 1);
        x = stoi(line.substr(0, line.find(" ")));
        line.erase(0, line.find(" ") + 1);
        y = stoi(line);
        vertices[id - 1] = make_pair(x, y);
    }
    file.close();

    int distance_matrix[N][N];

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            distance_matrix[i][j] = euclidean_distance(vertices[i], vertices[j]);
        }
    }

    (*init_sol_gen).load_data(distance_matrix);
    (*init_sol_gen).set_start_vertex(start_vertex - 1);
    (*init_sol_gen).set_iterations(1);

    TPaths initial_solution = (*init_sol_gen).solve();
    TPathCost initial_cost = (*init_sol_gen).get_cost();

    printf("Type %s\n", typeid(solver).name());

    (*solver).load_data(distance_matrix);
    if (solver_name != "greedy-cycle"){
        (*solver).set_initial_solution(initial_solution);
        (*solver).set_initial_cost(initial_cost);
    }
    
    (*solver).set_iterations(iterations);
    (*solver).set_neighbourhood(neighbourhood);
    (*solver).set_start_vertex(start_vertex);
    (*solver).set_max_candidates(max_candidates);

    high_resolution_clock::time_point start = high_resolution_clock::now();
    TPaths paths = (*solver).solve();
    high_resolution_clock::time_point stop = high_resolution_clock::now();
    double duration = duration_cast<milliseconds>(stop - start).count();

    TPathCost cost = (*solver).get_cost();

    save_results_to_json(data_path, output_path, solver_name, init_sol_gen_name, neighbourhood, start_vertex, paths, initial_solution, cost, initial_cost, duration, max_candidates);

    int evaluated_cost = cost.first + cost.second;
    int real_cost = 0;

    for (int i = 0; i < paths.first.size() - 1; i++)
    {
        real_cost += distance_matrix[paths.first[i]][paths.first[i + 1]];
        // printf("(1) Adding cost (%d) of path from %d to %d\n", distance_matrix[paths.first[i]][paths.first[i + 1]], paths.first[i], paths.first[i + 1]);
    }
    real_cost += distance_matrix[paths.first[N / 2 - 1]][paths.first[0]];
    // printf("(1) Adding cost (%d) of path from %d to %d\n", distance_matrix[paths.first[N / 2 - 1]][paths.first[0]], paths.first[N / 2 - 1], paths.first[0]);

    for (int i = 0; i < paths.second.size() - 1; i++)
    {
        real_cost += distance_matrix[paths.second[i]][paths.second[i + 1]];
        // printf("(2) Adding cost (%d) of path from %d to %d\n", distance_matrix[paths.second[i]][paths.second[i + 1]], paths.second[i], paths.second[i + 1]);
    }
    real_cost += distance_matrix[paths.second[N / 2 - 1]][paths.second[0]];
    // printf("(2) Adding cost (%d) of path from %d to %d\n", distance_matrix[paths.second[N / 2 - 1]][paths.second[0]], paths.second[N / 2 - 1], paths.second[0]);

    printf("Evaluated: %d, real: %d\n", evaluated_cost, real_cost);

    return 0;
}