import subprocess
import os
import pandas as pd

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")
DIR_BASELINES = os.path.join("..", "baselines")
DIR_PLOTS = os.path.join("..", "plots")

gathered_results = pd.read_csv("gathered_results.csv")

for i, row in gathered_results.iterrows():
    instance = row["instance"].split(".")[0]
    instance_csv = os.path.join(DIR_DATA, instance + ".csv")
    solver = row["solver"]
    initial_solver = row["initial_solver"]
    neighbourhood = row["neighbourhood"]
    
    min_cost_sv = row["min_cost"].split("(")[1].split(")")[0]
    max_cost_sv = row["max_cost"].split("(")[1].split(")")[0]
    
    min_duration_sv = row["min_duration [ms]"].split("(")[1].split(")")[0]
    max_duration_sv = row["max_duration [ms]"].split("(")[1].split(")")[0]
    
    result_files = [
        "{}-{}-{}-{}-{}".format(instance, solver, initial_solver, neighbourhood, min_cost_sv),
        "{}-{}-{}-{}-{}".format(instance, solver, initial_solver, neighbourhood, max_cost_sv),
        "{}-{}-{}-{}-{}".format(instance, solver, initial_solver, neighbourhood, min_duration_sv),
        "{}-{}-{}-{}-{}".format(instance, solver, initial_solver, neighbourhood, max_duration_sv),
    ]
    
    for result_file in result_files:
        json_path = os.path.join(DIR_RESULTS, result_file + ".json")
        solution_plot = os.path.join(DIR_PLOTS, result_file + "-sol.pdf")
        init_sol_plot = os.path.join(DIR_PLOTS, result_file + "-ini.pdf")
        subprocess.run([
            "python3", os.path.join("..", "visualization.py"),
            instance_csv, json_path, solution_plot, init_sol_plot
        ])

baselines = [
    "bas-kroA100-N1",
    "bas-kroA100-N2",
    "bas-kroB100-N1",
    "bas-kroB100-N2",
]

instances = [
    "kroA100",
    "kroA100",
    "kroB100",
    "kroB100"
]

for instance, baseline in zip(instances, baselines):
    instance_csv = os.path.join(DIR_DATA, instance + ".csv")
    json_path = os.path.join(DIR_BASELINES, baseline + ".json")
    solution_plot = os.path.join(DIR_PLOTS, baseline + "-sol.pdf")
    init_sol_plot = os.path.join(DIR_PLOTS, baseline + "-ini.pdf")
    
    subprocess.run([
        "python3", os.path.join("..", "visualization.py"),
        instance_csv, json_path, solution_plot, init_sol_plot
    ])