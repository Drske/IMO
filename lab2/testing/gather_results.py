import os
import pandas as pd
import json

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