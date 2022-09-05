# REQUIRED LIBRARIES-----------------------------------------------------------------------
import networkx as nx
from matplotlib import pyplot as plt
import math
import sys
import random
import os

# AUXILIARY FUNCTIONS---------------------------------------------------------------------
sys.setrecursionlimit(5000)  # new max depth for interpreter stack


def get_euclidean_distance(graph, node1, node2):
    (x1, y1) = nx.get_node_attributes(graph, 'pos')[node1]
    (x2, y2) = nx.get_node_attributes(graph, 'pos')[node2]
    return (((x2 - x1) ** 2 + (y2 - y1) ** 2) ** 0.5)


# MAIN FUNCTIONS--------------------------------------------------------------------------
#   Graph Creation++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
def initGrafo(G, num, radiusForConnection):
    #nodes creation
    for i in range(0,num):
        xValue = random.uniform(0.0, 100.0)
        yValue = random.uniform(0.0, 100.0)
        G.add_node(i, pos = (xValue, yValue))
        
    #edges creation
    for i in range(0,num):
        for j in range(i+1, num):
            if (get_euclidean_distance(G, i, j) <= radiusForConnection):
                G.add_edge(i, j, peso = get_euclidean_distance(G, i, j))


#   Brute force algorithms++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   DFS
def dfs(start, target, graph):
    path = [[start]]
    steps = 0
    while path:
        steps = steps + 1
        index = -1
        s_path = path.pop(index)
        l_node = s_path[-1]  # the last node is our target, it's done

        if l_node == target:
            print("Cantidad de Pasos -> {}".format(steps))
            return s_path
        else:
            for node in graph[l_node]:
                if node not in s_path:
                    new_path = s_path + [node]
                    path.append(new_path)
    print('El camino no existe' % (start, target))


#   BFS
def bfs1(start, target, graph):
    path = [[start]]
    steps = 0
    while path:
        steps = steps + 1
        index = 0
        s_path = path.pop(index)
        l_node = s_path[-1]  # the last node is our target, it's done

        if l_node == target:
            print("Cantidad de Pasos -> {}".format(steps))
            return s_path
        else:
            for node in graph[l_node]:
                if node not in s_path:
                    new_path = s_path + [node]
                    path.append(new_path)

    print("El camino no existe {} {}".format(start, target))


def delNodes(G, porcentaje, tam, ini, fin):
    porcentajeInt = math.floor((tam * tam) * porcentaje / 100)
    for i in random.sample(range(0, tam * tam), porcentajeInt):
        if (i != ini and i != fin):
            G.remove_node(i)


#   Heuristics algorithms++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   HILL CLIMBING


#   A STAR
def a_star_search(graph, initialNode, finalNode, steps=0):
    path = []
    path.append(initialNode)
    # close condition: destination node reached
    while (initialNode != finalNode):
        # ask for the neighbors
        minWeight = 1000000
        minNeighbor = initialNode
        for neighbor in graph.neighbors(initialNode):
            localWeight = graph[initialNode][neighbor]["peso"]  # calculate G(n)
            localWeight += get_euclidean_distance(graph, neighbor, finalNode)  # calculate H(n)
            if (localWeight < minWeight):
                minWeight = localWeight
                minNeighbor = neighbor
        steps += 1
        path.append(minNeighbor)
        initialNode = minNeighbor

    print("Number of nodes visited was {}".format(steps))
    return path


# MAIN PROGRAM--------------------------------------------------------------------------
G = nx.Graph()
print("NUMBER OF NODES: ")
tam = int(input())
print("RADIUS FOR CONNECT THE NODES: ")
radiusForConnection = int(input())
print("Escribe en que nodo inicia la busqueda del 0 al {}".format(tam))
inicio = int(input())
print("Escribe en que nodo termina la busqueda del 0 al {}".format(tam))
final = int(input())

## RECIBE EL GRAFO Y N PARA CREAR UN GRAFO DE N * N
initGrafo(G, tam, radiusForConnection)



res = []

while (True):

    print("Que busqueda deseas realizar: \n")
    print("1. BFS \n")
    print("2. DFS \n")
    print("3. HILL CLIMBING \n")
    print("4. A* \n")
    print("5. Salir")

    option = int(input("-> "))
    if (option == 1):
        res = bfs1(inicio, final, G)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'), node_color=color_map, with_labels=False)
        plt.show()

    if (option == 2):
        res = dfs(inicio, final, G)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'), node_color=color_map, with_labels=True)
        plt.show()

    # HILL CLIMBING
    if (option == 3):
        # HILL CLIMBING algorithm here
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print("Cantidad de Pasos -> {}".format(len(res)))
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'), node_color=color_map, with_labels=True)
        plt.show()

    # A*
    if (option == 4):
        res = a_star_search(G, inicio, final)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'),node_color = color_map, with_labels=True)
        plt.show()

    if (option == 5):
        break
    os.system('cls' if os.name == 'nt' else 'clear')
