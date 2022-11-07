import time
import subprocess
import distinctipy
import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.animation as animation


# Generating graph
subprocess.call([
	'g++', 'gg/generate_graph.cpp', '-o', 'generate_graph'
])
subprocess.run('generate_graph.exe 18 21')


# Solving for isomorphism
subprocess.call([
	'gcc', 'ic/isomorphic_check.c',
	'ic/naugraph.c', 'ic/naurng.c', 'ic/nautil.c',
	'ic/nautinv.c', 'ic/nauty.c', 'ic/schreier.c',
	'-o', 'isomorphic_check'
])
subprocess.run('isomorphic_check.exe')


# Reading input graphs
g1 = nx.Graph()
g2 = nx.Graph()
g1_colors = []
g2_colors = []
g1_pos = []
g2_pos = []
g2_g1 = []
is_iso = False

with open('data.in') as file:
	n, e = [int(x) for x in file.readline().split()]

	g1.add_nodes_from([i for i in range(n)])
	g2.add_nodes_from([i for i in range(n)])
	g1_colors = distinctipy.get_colors(n)
	g2_colors = [(0, 0, 0)] * n
	g1_pos = [0] * n
	g2_pos = [0] * n
	g2_g1 = [0] * n

	for i in range(e):
		g1.add_edge(*[int(x) for x in file.readline().split()])

	for i in range(e):
		g2.add_edge(*[int(x) for x in file.readline().split()])


# Reading isomorphic matchings
with open('data.out') as file:
	is_iso = (file.readline() == 'Isomorphic\n')

	if is_iso:
		for i in range(n):
			line = file.readline().split()

			if len(line):
				g2_colors[int(line[1])] = g1_colors[int(line[0])]
				g2_g1[int(line[1])] = int(line[0])


# Displaying input graphs
fig = plt.figure()
g1_pos = nx.spring_layout(g1)
g2_pos = nx.spring_layout(g1)
g2_cur_pos = g2_pos.copy()


def update(i):
	fig.clear()

	if n > 20:
		g2_cur_pos[i] = g2_pos[i] + (g1_pos[g2_g1[i]] - g2_pos[i])
	else:
		h = i
		i //= 10
		g2_cur_pos[i] = g2_pos[i] + (g1_pos[g2_g1[i]] - g2_pos[i]) * (h % 10 + 1) / 10

	plt.subplot(1, 2, 1)
	nx.draw(g1, with_labels=True, node_color=g1_colors, pos=g1_pos)
	plt.subplot(1, 2, 2)
	nx.draw(g2, with_labels=True, node_color=g2_colors, pos=g2_cur_pos)


update(0)

plt.pause(5)

if is_iso:
	if n > 20:
		ani = animation.FuncAnimation(fig, update, frames=n, interval=10, repeat=False)
	else:
		ani = animation.FuncAnimation(fig, update, frames=10*n, interval=10, repeat=False)

plt.show()