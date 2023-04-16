import subprocess
import os
import pandas as pd
from tqdm import tqdm

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")
DIR_BASELINES = os.path.join("..", "baselines")
DIR_PLOTS = os.path.join("..", "plots")

gathered_results = pd.read_csv("test_results.csv")

for i, row in tqdm(gathered_results.iterrows()):
    instance = row["instance"].split(".")[0]
    instance_csv = os.path.join(DIR_DATA, instance + ".csv")
    solver = row["solver"]
    start_vertex = row["start_vertex"]
    
    result_files = [
        "{}-{}-{}".format(instance, solver, start_vertex),
        "{}-{}-{}".format(instance, solver, start_vertex),
        "{}-{}-{}".format(instance, solver, start_vertex),
        "{}-{}-{}".format(instance, solver, start_vertex),
    ]
    
    for result_file in result_files:
        json_path = os.path.join(DIR_RESULTS, result_file + ".json")
        solution_plot = os.path.join(DIR_PLOTS, result_file + "-sol.pdf")
        init_sol_plot = os.path.join(DIR_PLOTS, result_file + "-ini.pdf")
        subprocess.run([
            "python3", os.path.join("..", "visualization.py"),
            instance_csv, json_path, solution_plot, init_sol_plot
        ])