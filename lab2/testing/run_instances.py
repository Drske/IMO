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
        
    for ls_solver in ["greedy-ls", "steepest-ls"]:
        for neighbourhood in ["N1", "N2"]:
            for init_sol_gen in ["greedy-cycle", "random-walk"]:
                for start_vertex in range(1, 101):
                    instance = filename.split(".")[0]
                    output_file_path = os.path.join(DIR_RESULTS, "{}-{}-{}-{}-{}.json".format(instance, ls_solver, neighbourhood, init_sol_gen, start_vertex))
                    subprocess.run([
                        os.path.join("..", "main"),
                        "-solver", ls_solver,
                        "-init-sol-gen", init_sol_gen,
                        "-neigh", neighbourhood,
                        "-start-vertex", str(start_vertex),
                        "-in", input_file_path, 
                        "-out", output_file_path])