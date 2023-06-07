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
    "iterations": [],
    "cost_first": [],
    "cost_second": [],
    "total_cost": [],
    "duration": [],
    "start_vertex": [],
    "run_no": [],
})

for filename in os.listdir(DIR_RESULTS):
    if not filename.endswith(".json"):
        continue
    
    run_no = filename.split('.')[0][-1]

    results_file_path = os.path.join(DIR_RESULTS, filename)

    with open(results_file_path) as file:
        jresults = json.load(file)
        
    solver = jresults["solver"]
    if jresults["use-ls-to-repair"]:
        solver += "-a"

    results_row = pd.DataFrame({
        "instance": [jresults["instance"]],
        "solver": [solver],
        "start_vertex": [jresults["start-vertex"]],
        "run_no": [run_no],
        "iterations": [jresults["iterations"]],
        "cost_first": [jresults["cost"]["first"]],
        "cost_second": [jresults["cost"]["second"]],
        "total_cost": [jresults["cost"]["first"] + jresults["cost"]["second"]],
        "duration": [jresults["duration"]],
    })
    results = pd.concat([results, results_row], axis=0)

results["start_vertex"] = results["start_vertex"].astype(int)
results["run_no"] = results["run_no"].astype(int)
results["iterations"] = results["iterations"].astype(int)
results["cost_first"] = results["cost_first"].astype(int)
results["cost_second"] = results["cost_second"].astype(int)
results["duration"] = results["duration"].astype(int)
results["total_cost"] = results["total_cost"].astype(int)

results.to_csv(os.path.join("test_results.csv"), index=False)

gathered_results = pd.DataFrame({
    "instance": [],
    "solver": [],
    "iterations": [],
    "avg_cost": [],
    "min_cost": [],
    "max_cost": [],
    "avg_duration [ms]": [],
    "min_duration [ms]": [],
    "max_duration [ms]": [],
})

for instance in ["kroA200.tsp", "kroB200.tsp"]:
    for solver in ["shev", "shev-a"]:
        tmp = deepcopy(results[results["instance"] == instance])
        tmp = tmp[tmp["solver"] == solver]
        
        avg_cost = tmp.total_cost.mean()
        min_cost = tmp[tmp.total_cost == tmp.total_cost.min()]["total_cost"].values[0]
        min_cost_sv = tmp[tmp.total_cost == tmp.total_cost.min()]["start_vertex"].values[0]
        min_cost_rn = tmp[tmp.total_cost == tmp.total_cost.min()]["run_no"].values[0]
        max_cost = tmp[tmp.total_cost == tmp.total_cost.max()]["total_cost"].values[0]
        max_cost_sv = tmp[tmp.total_cost == tmp.total_cost.max()]["start_vertex"].values[0]
        max_cost_rn = tmp[tmp.total_cost == tmp.total_cost.max()]["run_no"].values[0]
        
        avg_duration = tmp.duration.mean()
        min_duration = tmp[tmp.duration == tmp.duration.min()]["duration"].values[0]
        min_duration_sv = tmp[tmp.duration == tmp.duration.min()]["start_vertex"].values[0]
        min_duration_rn = tmp[tmp.duration == tmp.duration.min()]["run_no"].values[0]
        max_duration = tmp[tmp.duration == tmp.duration.max()]["duration"].values[0]
        max_duration_sv = tmp[tmp.duration == tmp.duration.max()]["start_vertex"].values[0]
        max_duration_rn = tmp[tmp.duration == tmp.duration.max()]["run_no"].values[0]
        
        avg_iterations = int(tmp.iterations.mean())
        
        gathered_results_row = pd.DataFrame({
            "instance": [instance],
            "solver": [solver],
            "iterations": [avg_iterations],
            "avg_cost": [avg_cost],
            "min_cost": ["{} ({}) [{}]".format(min_cost, min_cost_sv, min_cost_rn)],
            "max_cost": ["{} ({}) [{}]".format(max_cost, max_cost_sv, max_cost_rn)],
            "avg_duration [ms]": [avg_duration],
            "min_duration [ms]": ["{} ({}) [{}]".format(min_duration, min_duration_sv, min_duration_rn)],
            "max_duration [ms]": ["{} ({}) [{}]".format(max_duration, max_duration_sv, max_duration_rn)],
        })
        gathered_results = pd.concat([gathered_results, gathered_results_row], axis=0)
        
gathered_results["iterations"] = gathered_results["iterations"].astype(int)
gathered_results.to_csv(os.path.join("gathered_results.csv"), index=False)