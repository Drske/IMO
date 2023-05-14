import subprocess
import os
import np

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")

# Running instances
for filename in os.listdir(DIR_DATA):
    if filename.endswith("200.tsp"):
        input_file_path = os.path.join(DIR_DATA, filename)
    else:
        continue

    for solver, iterations in zip(["ms-ls", "dr-ls", "dr-ls-a"], [100, 200_000, 1500]):
        init_sol_gen = "random-walk"
        ls_solver = "queue=-ls"
        con_solver = "greedy-cycle"
        
        neighbourhood = "N2"
        use_ls_to_repair = "false"
        
        if solver == "dr-ls-a":
            use_ls_to_repair = "true"
            solver = "dr-ls"
            
        for start_vertex in range(1, 200, 2):
            instance = filename.split(".")[0]
            output_file_path = os.path.join(DIR_RESULTS, "{}-{}-{}.json".format(
                instance, ls_solver, start_vertex))
            
            print("Running:", instance, ls_solver, init_sol_gen, neighbourhood, start_vertex)
            
            subprocess.run([
                os.path.join("..", "main.out"),
                "-solver", solver,
                "-init-sol-gen", init_sol_gen,
                "-neigh", neighbourhood,
                "-start-vertex", str(start_vertex),
                "-max-candidates", str(15),
                "-in", input_file_path,
                "-out", output_file_path])