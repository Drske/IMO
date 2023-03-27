import sys
import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

result = json.load(open(sys.argv[2]))

cost1 = result['cost']['first']
cost2 = result['cost']['second']
instance = result['instance']
solver = result['solver']
start_vertex = result['start-vertex']

path1 = result['path']['first']
path2 = result['path']['second']

df = pd.read_csv(sys.argv[1])

ids = df['Node'].values
X = df['X'].values
Y = df['Y'].values

fig, ax = plt.subplots()

X1 = np.append(X[path1], X[path1][0])
Y1 = np.append(Y[path1], Y[path1][0])
X2 = np.append(X[path2], X[path2][0])
Y2 = np.append(Y[path2], Y[path2][0])

ax.scatter(X1, Y1, color='blue', s=100)
ax.scatter(X2, Y2, color='red', s=100)

ax.plot(X1, Y1, color='blue')
ax.plot(X2, Y2, color='red')

ax.tick_params(left=False, labelleft=False, labelbottom=False, bottom=False)

ax.set_title(f'solver = {solver} \n cost1 = {cost1} \n cost2 = {cost2} \n instance = {instance} \n start_vertex = {start_vertex}', wrap='true')

ax.set_xlabel('X')
ax.set_ylabel('Y')

for i, id in enumerate(ids):
    ax.annotate(id-1, (X[i], Y[i]), fontsize=6, ha='center', va='center')

fig.tight_layout()

plt.savefig(sys.argv[3])
# plt.show()

