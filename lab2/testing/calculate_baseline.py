import subprocess
import os

DIR_DATA = os.path.join("..", "data")
DIR_BASELINES = os.path.join("..", "baselines")
DIR_PLOTS = os.path.join("..", "plots")
ITERATIONS = 1000

# Running instances
for filename in os.listdir(DIR_DATA):
    if filename.endswith("100.tsp"):
        input_file_path = os.path.join(DIR_DATA, filename)
    else:
        continue

    for neighbourhood in ["N1", "N2"]:
        instance = filename.split(".")[0]
        output_file_path = os.path.join(
            DIR_BASELINES, "bas-{}-{}.json".format(instance, neighbourhood))

        subprocess.run([
            os.path.join("..", "main.out"),
            "-solver", "random-walk",
            "-init-sol-gen", "greedy-cycle",
            "-neigh", neighbourhood,
            "-start-vertex", '1',
            "-iterations", str(ITERATIONS),
            "-in", input_file_path,
            "-out", output_file_path])
