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
                G.add_edge(i, j, peso = get_euclidean_distance(G, i, j), color = 'b')

#   Brute force algorithms++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   DFS
def dfs_search(start, target, graph):
    path = [[start]]
    steps = 0
    while path:
        steps = steps + 1
        index = -1
        s_path = path.pop(index)
        l_node = s_path[-1]  # the last node is our target, it's done

        if l_node == target:
            print("\t#STEPS -> {}".format(steps))
            return s_path, steps
        else:
            for node in graph[l_node]:
                if node not in s_path:
                    new_path = s_path + [node]
                    path.append(new_path)
    print("\tPATH NOT FOUND between{} {}".format(start, target))
    return [], 0

#   BFS
def bfs_search(start, target, graph):
    path = [[start]]
    steps = 0
    while path:
        steps = steps + 1
        index = 0
        s_path = path.pop(index)
        l_node = s_path[-1]  # the last node is our target, it's done

        if l_node == target:
            print("\t#STEPS -> {}".format(steps))
            return s_path, steps
        else:
            for node in graph[l_node]:
                if node not in s_path:
                    new_path = s_path + [node]
                    path.append(new_path)

    print("\tPATH NOT FOUND between{} {}".format(start, target))
    return [], 0

#   Heuristics algorithms++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   HILL CLIMBING
def hill_climbing_search(graph, initialNode, finalNode, steps=0):
    path = []
    path.append(initialNode)
    # close condition: destination node reached
    while (initialNode != finalNode and steps < graph.number_of_nodes()):
        # ask for the neighbors
        minWeight = 1000000
        minNeighbor = initialNode
        for neighbor in graph.neighbors(initialNode):
            localWeight = get_euclidean_distance(graph, neighbor, finalNode)
            if (localWeight < minWeight):
                minWeight = localWeight
                minNeighbor = neighbor
        steps += 1
        path.append(minNeighbor)
        initialNode = minNeighbor

    if(initialNode == finalNode):
        print("\tNumber of nodes visited was {}".format(steps))
        return path, steps
    else:
        print("\tNo path was found")
        return [], 0

#   A STAR
def a_star_search(graph, initialNode, finalNode, steps=0):
    path = []
    path.append(initialNode)
    # close condition: destination node reached
    while (initialNode != finalNode and steps < graph.number_of_nodes()):
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

    if(initialNode == finalNode):
        print("\tNumber of nodes visited was {}".format(steps))
        return path, steps
    else:
        print("\tNo path was found")
        return [], 0


# MAIN PROGRAM--------------------------------------------------------------------------
G = nx.Graph()
print("NUMBER OF NODES: ")
tam = int(input())
print("RADIUS FOR CONNECT THE NODES: ")
radiusForConnection = int(input())

## RECIBE EL GRAFO Y N PARA CREAR UN GRAFO DE N * N
initGrafo(G, tam, radiusForConnection)
res = []

while (True):
    print("Initial node from 0 to {}".format(tam-1))
    inicio = int(input())
    print("Destination node from 0 to {}".format(tam-1))
    final = int(input())

    print("\t PICK THE SEARCH ALGORITH: \n")
    print("1. BFS \n")
    print("2. DFS \n")
    print("3. HILL CLIMBING \n")
    print("4. A* \n")
    print("5. Comparation\n")
    print("6. QUIT")

    option = int(input("-> "))
    if (option == 1):
        res, numSteps = bfs_search(inicio, final, G)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'), node_color=color_map, with_labels=True)
        plt.show()

    if (option == 2):
        res, numSteps = dfs_search(inicio, final, G)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        #colors = [G[u][v]['color'] for u,v in G.edges()]
        nx.draw(G, nx.get_node_attributes(G, 'pos'), node_color=color_map, with_labels=True)
        plt.show()

    # HILL CLIMBING
    if (option == 3):
        res, numSteps = hill_climbing_search(G, inicio, final)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'), node_color=color_map, with_labels=True)
        plt.show()

    # A*
    if (option == 4):
        res, numSteps = a_star_search(G, inicio, final)
        color_map = []
        for node in G:
            if node in res:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(res)
        nx.draw(G, nx.get_node_attributes(G, 'pos'),node_color = color_map, with_labels=True)
        plt.show()

    #COMPARATION
    if (option == 5):
        res, numSteps = bfs_search(inicio, final, G)
        print("Steps for BFS:\t{}".format(numSteps))
        minRes = res
        minNumSteps = numSteps
        res, numSteps = dfs_search(inicio, final, G)
        print("Steps for DFS:\t{}".format(numSteps))
        minRes = res if res < minRes else minRes
        minNumSteps = numSteps if numSteps < minNumSteps else minNumSteps
        res, numSteps = hill_climbing_search(G, inicio, final)
        print("Steps for Hill-Climbing:\t{}".format(numSteps))
        minRes = res if res < minRes else minRes
        minNumSteps = numSteps if numSteps < minNumSteps else minNumSteps
        res, numSteps = a_star_search(G, inicio, final)
        print("Steps for A*:\t{}".format(numSteps))
        minRes = res if res < minRes else minRes
        minNumSteps = numSteps if numSteps < minNumSteps else minNumSteps

        #set edge colors
        for i in range(0, minNumSteps):
            nx.set_edge_attributes(G, {(minRes[i], minRes[i+1]): {"color": 'r'}})
        colors = nx.get_edge_attributes(G, 'color').values()
        
        #set node colors
        color_map = []
        for node in G:
            if node in minRes:
                color_map.append('red')
            else:
                color_map.append('blue')
        print(minRes)
        nx.draw(G, nx.get_node_attributes(G, 'pos'),node_color = color_map, with_labels=True, edge_color = colors)
        plt.show()

    if (option == 6):
        break
    os.system('cls' if os.name == 'nt' else 'clear')
