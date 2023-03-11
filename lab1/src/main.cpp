#include <iostream>
#include <stdlib.h>
#include <map>
#include <string>
#include <fstream>
#include <math.h>

#include "tsp_solver.h"
#include "nn_tsp_solver.h"
#include "gc_tsp_solver.h"
#include "re_tsp_solver.h"

#define N 100

using namespace std;

bool cmd_option_provided(string option, int argc, char **argv)
{
    for (int i = 1; i <= argc; i++)
    {
        if (argv[i] == option)
        {
            return true;
        }
    }
    return false;
}

string get_cmd_otpion(string option, int argc, char **argv)
{
    for (int i = 1; i <= argc; i++)
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

int main(int argc, char **argv)
{
    map<string, TSPSolver*> solvers;

    solvers["nearest-neighbour"] = new NNSolver();
    solvers["greedy-cycle"] = new GCSolver();
    solvers["regrets"] = new RESolver();

    TSPSolver *solver;
    string data_path;
    string output_path;

    if (cmd_option_provided("-solver", argc, argv))
    {
        solver = solvers[get_cmd_otpion("-solver", argc, argv)];
    }
    else
    {
        cout << "No solver provided";
        return 1;
    }

    if (cmd_option_provided("-in", argc, argv))
    {
        data_path = get_cmd_otpion("-in", argc, argv);
    }
    else
    {
        cout << "No input provided";
        return 1;
    }

    if (cmd_option_provided("-out", argc, argv))
    {
        output_path = get_cmd_otpion("-out", argc, argv);
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

    int start_vertex = 0;

    (*solver).load_data(distance_matrix);
    (*solver).solve(start_vertex);
    return 0;
}