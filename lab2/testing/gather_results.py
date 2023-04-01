import os
import pandas as pd
import json
import numpy as np
from copy import deepcopy

DIR_RESULTS = os.path.join("..", "results")

# Gathering results
results = pd.DataFrame({
    "instance": [],
    "solver": [],
    "init_solver": [],
    "neighbourhood": [],
    "start_vertex": [],
    "cost_first": [],
    "cost_second": [],
    "total_cost": [],
    "duration": []
})

for filename in os.listdir(DIR_RESULTS):
    if not filename.endswith(".json"):
        continue

    results_file_path = os.path.join(DIR_RESULTS, filename)

    with open(results_file_path) as file:
        jresults = json.load(file)

    results_row = pd.DataFrame({
        "instance": [jresults["instance"]],
        "solver": [jresults["solver"]],
        "init_solver": [jresults["initial-solver"]],
        "neighbourhood": [jresults["neighbourhood"]],
        "start_vertex": [jresults["start-vertex"]],
        "cost_first": [jresults["cost"]["first"]],
        "cost_second": [jresults["cost"]["second"]],
        "total_cost": [jresults["cost"]["first"] + jresults["cost"]["second"]],
        "duration": [jresults["duration"]]
    })
    results = pd.concat([results, results_row], axis=0)

results["start_vertex"] = results["start_vertex"].astype(int)
results["cost_first"] = results["cost_first"].astype(int)
results["cost_second"] = results["cost_second"].astype(int)
results["duration"] = results["duration"].astype(int)
results["total_cost"] = results["total_cost"].astype(int)

results.to_csv(os.path.join("test_results.csv"), index=False)

gathered_results = pd.DataFrame({
    "instance": [],
    "solver": [],
    "initial_solver": [],
    "neighbourhood": [],
    "avg_cost": [],
    "min_cost": [],
    "max_cost": [],
    "avg_duration [ms]": [],
    "min_duration [ms]": [],
    "max_duration [ms]": [],
})

for instance in ["kroA100.tsp", "kroB100.tsp"]:
    for ls_solver in ["greedy-ls", "steepest-ls"]:
        for init_sol_gen in ["greedy-cycle", "random-walk"]:
            for neighbourhood in ["N1", "N2"]:
                tmp = deepcopy(results[results["instance"] == instance])
                tmp = tmp[tmp["solver"] == ls_solver]
                tmp = tmp[tmp["init_solver"] == init_sol_gen]
                tmp = tmp[tmp["neighbourhood"] == neighbourhood]
                
                avg_cost = tmp.total_cost.mean();
                min_cost = tmp[tmp.total_cost == tmp.total_cost.min()]["total_cost"].values[0]
                min_cost_sv = tmp[tmp.total_cost == tmp.total_cost.min()]["start_vertex"].values[0] + 1
                max_cost = tmp[tmp.total_cost == tmp.total_cost.max()]["total_cost"].values[0]
                max_cost_sv = tmp[tmp.total_cost == tmp.total_cost.max()]["start_vertex"].values[0] + 1
                
                avg_duration = tmp.duration.mean();
                min_duration = tmp[tmp.duration == tmp.duration.min()]["duration"].values[0]
                min_duration_sv = tmp[tmp.duration == tmp.duration.min()]["start_vertex"].values[0] + 1
                max_duration = tmp[tmp.duration == tmp.duration.max()]["duration"].values[0]
                max_duration_sv = tmp[tmp.duration == tmp.duration.max()]["start_vertex"].values[0] + 1
            
                gathered_results_row = pd.DataFrame({
                    "instance": [instance],
                    "solver": [ls_solver],
                    "initial_solver": [init_sol_gen],
                    "neighbourhood": [neighbourhood],
                    "avg_cost": [avg_cost],
                    "min_cost": ["{} ({})".format(min_cost, min_cost_sv)],
                    "max_cost": ["{} ({})".format(max_cost, max_cost_sv)],
                    "avg_duration [ms]": [avg_duration],
                    "min_duration [ms]": ["{} ({})".format(min_duration, min_duration_sv)],
                    "max_duration [ms]": ["{} ({})".format(max_duration, max_duration_sv)],
                })
                gathered_results = pd.concat([gathered_results, gathered_results_row], axis=0)
                
gathered_results.to_csv(os.path.join("gathered_results.csv"), index=False)