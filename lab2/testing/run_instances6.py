import subprocess
import os

DIR_DATA = os.path.join("..", "data")
DIR_RESULTS = os.path.join("..", "results")

# Running instances
for filename in os.listdir(DIR_DATA):
    if filename.endswith("B200.tsp"):
        input_file_path = os.path.join(DIR_DATA, filename)
    else:
        continue

    for solver_variance, iterations in zip(["shev", "shev-a", "hev", "hev-a"], [150_000, 1000, 1000, 150]):
        solver = solver_variance
        init_sol_gen = "random-walk"
        ls_solver = "queue-ls"
        con_solver = "greedy-cycle"
        
        neighbourhood = "N2"
        use_ls_to_repair = "false"
        
        if solver_variance == "shev-a":
            use_ls_to_repair = "true"
            solver = "shev"
            
        if solver_variance == "hev-a":
            use_ls_to_repair = "true"
            solver = "hev"
            
        for run in range(10):
            start_vertex = run * 100
            
            instance = filename.split(".")[0]
            output_file_path = os.path.join(DIR_RESULTS, "{}-{}-{}.json".format(
                instance, solver_variance, run))
            
            print("Running:", instance, solver_variance, run, use_ls_to_repair)
            
            subprocess.run([
                os.path.join("..", "main.out"),
                "-solver", solver,
                "-ls-solver", ls_solver,
                "-con-solver", con_solver,
                "-iterations", str(iterations),
                "-start-vertex", str(start_vertex),
                "-use-ls-to-repair", use_ls_to_repair,
                "-in", input_file_path,
                "-out", output_file_path])