import subprocess
import os

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")
DIR_PLOTS = os.path.join("..", "plots")

# Running instances
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