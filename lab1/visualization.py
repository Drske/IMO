import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

result = json.load(open('res.json'))

cost1 = result['cost']['first']
cost2 = result['cost']['second']
instance = result['instance']
solver = result['solver']
start_vertex = result['start-vertex']

path1 = result['path']['first']
path2 = result['path']['second']

df = pd.read_csv('data/kroB100.csv')

ids = df['Node'].values
X = df['X'].values
Y = df['Y'].values

fig, ax = plt.subplots()

ax.scatter(X[path1], Y[path1], color='blue', s=100)
ax.scatter(X[path2], Y[path2], color='red', s=100)

ax.plot(X[path1], Y[path1], color='blue')
ax.plot(X[path2], Y[path2], color='red')

ax.tick_params(left=False, labelleft=False, labelbottom = False, bottom = False)

ax.set_title(f'solver = {solver} \n cost1 = {cost1} \n cost2 = {cost2} \n instance = {instance} \n start_vertex = {start_vertex}', wrap='true')

ax.set_xlabel('X')
ax.set_ylabel('Y')

for i, id in enumerate(ids):
    ax.annotate(id-1, (X[i], Y[i]), fontsize=6, ha='center', va='center')

fig.tight_layout()

plt.savefig('foo.pdf')
plt.show()

