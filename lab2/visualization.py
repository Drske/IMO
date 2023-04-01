import sys
import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def plot_results(X1, Y1, X2, Y2, title, fig_path):
    fig, ax = plt.subplots()

    ax.scatter(X1, Y1, color='blue', s=100)
    ax.scatter(X2, Y2, color='red', s=100)

    ax.plot(X1, Y1, color='blue')
    ax.plot(X2, Y2, color='red')

    ax.tick_params(left=False, labelleft=False, labelbottom=False, bottom=False)

    ax.set_title(title, wrap='true', fontsize=12)

    ax.set_xlabel('X')
    ax.set_ylabel('Y')

    for i, id in enumerate(ids):
        ax.annotate(id-1, (X[i], Y[i]), fontsize=6, ha='center', va='center')

    fig.tight_layout()
    plt.savefig(fig_path)

if __name__ == '__main__':
    # Load TSP instance
    df = pd.read_csv(sys.argv[1])

    ids = df['Node'].values
    X = df['X'].values
    Y = df['Y'].values

    # Load JSON data
    result = json.load(open(sys.argv[2]))

    costs = (result['cost']['first'], result['cost']['second'])
    duration = result['duration']
    initial_costs = (result['initial-cost']['first'], result['initial-cost']['second'])

    initital_solver = result['initial-solver']
    instance = result['instance']
    neighbourhood = result['neighbourhood']
    solver = result['solver']
    start_vertex = result['start-vertex']

    initial_paths = (result['initial-path']['first'], result['initial-path']['second'])
    paths = (result['path']['first'], result['path']['second'])

    # Plot results
    X1 = np.append(X[paths[0]], X[paths[0]][0])
    Y1 = np.append(Y[paths[0]], Y[paths[0]][0])
    X2 = np.append(X[paths[1]], X[paths[1]][0])
    Y2 = np.append(Y[paths[1]], Y[paths[1]][0])

    result_title = f"""
            solver = {solver}, initial solver = {initital_solver}
            costs = ({costs[0]}, {costs[1]})
            instance = {instance}, neighbourhood = {neighbourhood}
            duration = {duration} ms"""

    plot_results(X1, Y1, X2, Y2, result_title, sys.argv[3])

    # Plot initial solution
    X1_initial = np.append(X[initial_paths[0]], X[initial_paths[0]][0])
    Y1_initial = np.append(Y[initial_paths[0]], Y[initial_paths[0]][0])
    X2_initial = np.append(X[initial_paths[1]], X[initial_paths[1]][0])
    Y2_initial = np.append(Y[initial_paths[1]], Y[initial_paths[1]][0])

    initial_result_title = f"""
            solver = {initital_solver}
            costs = ({initial_costs[0]}, {initial_costs[1]})
            instance = {instance}
            start_vertex = {start_vertex}"""

    plot_results(X1_initial, Y1_initial, X2_initial, Y2_initial, initial_result_title, sys.argv[4])
