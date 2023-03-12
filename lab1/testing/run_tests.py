import subprocess
import os
import pandas as pd
import json

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")

for filename in os.listdir(DIR_DATA):
    if filename.endswith("100.tsp"):
        input_file_path = os.path.join(DIR_DATA, filename)
    else:
        continue

    for start_vertex in range(1, 101):
        for solver in ["nearest-neighbour", "greedy-cycle", "regrets"]:
            instance = filename.split(".")[0]
            output_file_path = os.path.join(
                DIR_RESULTS, "{}-{}-{}.json".format(instance, solver, start_vertex))
            subprocess.run([os.path.join("..", "main"), "-solver", solver, "-in",
                           input_file_path, "-start-vertex", str(start_vertex), "-out", output_file_path])

results = pd.DataFrame({
    "instance": [],
    "solver": [],
    "start_vertex": [],
    "cost_first": [],
    "cost_second": [],
    "total_cost": []
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
        "start_vertex": [jresults["start-vertex"]],
        "cost_first": [jresults["cost"]["first"]],
        "cost_second": [jresults["cost"]["second"]],
        "total_cost": [jresults["cost"]["first"] + jresults["cost"]["second"]]
    })

    results = pd.concat([results, results_row], axis=0)

results.to_csv(os.path.join("test_results.csv"), index=False)
