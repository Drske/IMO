import subprocess
import os
import json

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")
DIR_PLOTS = os.path.join("..", "plots")

# Saving plots
for filename in os.listdir(DIR_RESULTS):
    if not filename.endswith(".json"):
        continue

    results_file_path = os.path.join(DIR_RESULTS, filename)

    with open(results_file_path) as file:
        jresults = json.load(file)

    data_file_path = os.path.join(
        DIR_DATA, jresults["instance"].split(".")[0] + ".csv")
    plot_file_path = os.path.join(DIR_PLOTS, filename.split(".")[0] + ".pdf")

    subprocess.run(["python3", os.path.join("..", "visualization.py"),
                   data_file_path, results_file_path, plot_file_path])