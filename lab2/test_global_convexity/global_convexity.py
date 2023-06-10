from __future__ import annotations
import os
import json
import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple

INPUT_LIST = ['kroA200', 'kroB200']
RESULTS_PATH = './results'
PLOTS_PATH = './plots'
N = 200


class TSPSolution:
    def __init__(self, cost: Tuple[int, int], paths: Tuple[np.ndarray, np.ndarray]):
        self.cost = cost
        self.paths = paths
        self.edges = np.sort(
            np.append(
                np.column_stack((paths[0], np.roll(paths[0], -1))),
                np.column_stack((paths[1], np.roll(paths[1], -1))),
                axis=0,
            )
        )

    def get_sim_vertices(self, other: TSPSolution) -> float:
        n_common1 = len(np.intersect1d(self.paths[0], other.paths[0]))
        n_common2 = len(np.intersect1d(self.paths[1], other.paths[0]))

        if n_common1 > n_common2:
            return (n_common1 + len(np.intersect1d(self.paths[1], other.paths[1]))) / N

        return (n_common2 + len(np.intersect1d(self.paths[0], other.paths[1]))) / N

    def get_sim_edges(self, other: TSPSolution) -> int:
        common_edges = np.intersect1d(
            self.edges.view(
                dtype=(np.void, self.edges.dtype.itemsize * self.edges.shape[1])
            ),
            other.edges.view(
                dtype=(np.void, other.edges.dtype.itemsize * other.edges.shape[1])
            ),
        )

        return len(common_edges) / N


def load_json(path: str) -> TSPSolution:
    result = json.load(open(path))

    cost = (result['cost']['first'], result['cost']['second'])
    paths = (np.array(result['path']['first']), np.array(result['path']['second']))

    return TSPSolution(cost, paths)


def save_plot(costs: np.ndarray, sims: np.ndarray, title: str, path: str):
    fig, ax = plt.subplots()

    ax.scatter(costs, sims)
    ax.set_xlabel('Wartość funkcji celu')
    ax.set_ylabel('Podobieństwo')
    ax.set_title(title, wrap='true', fontsize=12)

    fig.tight_layout()
    plt.savefig(path)


for instance_name in INPUT_LIST:
    sols = [load_json(f'./results/{instance_name}/output-{i}.json') for i in range(1000)]

    best_sol = min(sols, key=lambda x: sum(x.cost))
    sim_vertices = np.array(
        [sol.get_sim_vertices(best_sol) for sol in sols if sol != best_sol]
    )
    avg_sim_vertices = np.array(
        [
            np.mean([sol1.get_sim_vertices(sol2) for sol2 in sols if sol2 != sol1])
            for sol1 in sols
        ]
    )
    sim_edges = np.array(
        [sol.get_sim_edges(best_sol) for sol in sols if sol != best_sol]
    )
    avg_sim_edges = np.array(
        [
            np.mean([sol1.get_sim_edges(sol2) for sol2 in sols if sol2 != sol1])
            for sol1 in sols
        ]
    )
    costs = np.array([sum(sol.cost) for sol in sols])
    costs_wo_best = np.array([sum(sol.cost) for sol in sols if sol != best_sol])

    corr_sim_vertices = np.corrcoef(costs_wo_best, sim_vertices)[0, 1]
    corr_sim_edges = np.corrcoef(costs_wo_best, sim_edges)[0, 1]
    corr_avg_sim_vertices = np.corrcoef(costs, avg_sim_vertices)[0, 1]
    corr_avg_sim_edges = np.corrcoef(costs, avg_sim_edges)[0, 1]

    save_plot(
        costs_wo_best,
        sim_vertices,
        f'{instance_name}, l. wierzchołków (do najlepszego), korelacja = {corr_sim_vertices:.3f}',
        os.path.join(PLOTS_PATH, f'{instance_name}-opt-vert.pdf'),
    )
    save_plot(
        costs_wo_best,
        sim_edges,
        f'{instance_name}, l. krawędzi (do najlepszego), korelacja = {corr_sim_edges:.3f}',
        os.path.join(PLOTS_PATH, f'{instance_name}-opt-edges.pdf'),
    )
    save_plot(
        costs,
        avg_sim_vertices,
        f'{instance_name}, l. wierzchołków (średnia), korelacja = {corr_avg_sim_vertices:.3f}',
        os.path.join(PLOTS_PATH, f'{instance_name}-mean-vert.pdf'),
    )
    save_plot(
        costs,
        avg_sim_edges,
        f'{instance_name}, l. krawędzi (średnia), korelacja = {corr_avg_sim_edges:.3f}',
        os.path.join(PLOTS_PATH, f'{instance_name}-mean-edges.pdf'),
    )
