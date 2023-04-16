import subprocess
import os

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")

# Running instances
for filename in os.listdir(DIR_DATA):
    if filename.endswith("200.tsp"):
        input_file_path = os.path.join(DIR_DATA, filename)
    else:
        continue

    for ls_solver in ["greedy-cycle", "steepest-ls", "queue-ls", "candidate-ls", "qc-ls"]:
        for init_sol_gen in ["random-walk"]:
            for neighbourhood in ["N2"]:
                for start_vertex in range(1, 11):
                    start_vertex = start_vertex * 2
                    instance = filename.split(".")[0]
                    output_file_path = os.path.join(DIR_RESULTS, "{}-{}-{}.json".format(
                        instance, ls_solver, start_vertex))
                    
                    print("Running:", instance, ls_solver, init_sol_gen, neighbourhood, start_vertex)
                    
                    subprocess.run([
                        os.path.join("..", "main.out"),
                        "-solver", ls_solver,
                        "-init-sol-gen", init_sol_gen,
                        "-neigh", neighbourhood,
                        "-start-vertex", str(start_vertex),
                        "-max-candidates", str(15),
                        "-in", input_file_path,
                        "-out", output_file_path])