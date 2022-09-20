from ast import If
import string
import numpy as np
import random
import operator
import pandas as pd
import matplotlib.pyplot as plt
import networkx as nx
import math
from random import randint, choice

class City:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def distance(self, city):
        xDis = abs(self.x - city.x)
        yDis = abs(self.y - city.y)
        distance = np.sqrt((xDis ** 2) + (yDis ** 2))
        return distance
    
    def __repr__(self):
        return "(" + str(self.x) + "," + str(self.y) + ")"

class Fitness:
    def __init__(self, route):
        self.route = route
        self.distance = 0
        self.fitness= 0.0
    
    def routeDistance(self):
        if self.distance ==0:
            pathDistance = 0
            for i in range(0, len(self.route)):
                fromCity = self.route[i]
                toCity = None
                if i + 1 < len(self.route):
                    toCity = self.route[i + 1]
                else:
                    toCity = self.route[0]
                pathDistance += fromCity.distance(toCity)
            self.distance = pathDistance
        return self.distance

    def routeFitness(self):
        if self.fitness == 0:
            self.fitness = 1/float(self.routeDistance())
        return self.fitness

def createRoute(cityList):
    route = random.sample(cityList, len(cityList))
    #print("Rutita: ",route)
    return route

def initialPopulation(popSize, cityList):
    population = []
    for i in range(0, popSize):
        population.append(createRoute(cityList))
    global firstEverRoute
    firstEverRoute=population[popSize-1]
    return population

def routeRanking(population):
    fitnessResults = {}
    for i in range(0,len(population)):
        fitnessResults[i] = Fitness(population[i]).routeFitness()
    return sorted(fitnessResults.items(), key = operator.itemgetter(1), reverse = True)

def selection(popRanked):
    selectionResults = []
    df = pd.DataFrame(np.array(popRanked), columns=["Idx","Fitness"])
    df['cum_sum'] = df.Fitness.cumsum()
    df['cum_perc'] = 100*df.cum_sum/df.Fitness.sum()
    for i in range(0, 10):
        selectionResults.append(popRanked[i][0])
    for i in range(0, len(popRanked) - 10):
        pick = 100*random.random()
        for i in range(0, len(popRanked)):
            if pick <= df.iat[i,3]:
                selectionResults.append(popRanked[i][0])
                break
    return selectionResults

def finalSelection(population, selectionResults):
    matingpool = []
    for i in range(0, len(selectionResults)):
        index = selectionResults[i]
        matingpool.append(population[index])
    return matingpool

def crossing(parent1, parent2):
    '''
    child = []
    childP1 = []
    binary = []
    if (parent1 ==parent2):
        return parent1
    binary.append(0)
    for i in range (1,len(parent1)):
        binary.append(round(random.random()))
    for i in range (len(binary)):
        if(binary[i]==1):
            childP1.append(parent2[i])
    temp = 0
    temp1 = 0
    for i in range (len(parent1)):
        if(binary[i]==1):
            child.append(childP1[temp])
            temp = temp+1
        else:
            ingresado = False
            while ((temp1<len(parent1)) and (not ingresado)):
                if(parent1[temp1] not in childP1):
                    child.append(parent1[temp1])
                    ingresado = True
                else:
                    temp1 = temp1 + 1
            if (ingresado == False):
                child.append(City(x=int(random.random()*((parent1[i].x+parent2[i].x))), y=int(random.random()*((parent2[i].y+parent1[i].y)))))
    return child
    
    '''
    child = []
    childP1 = []
    childP2 = []
    partA = int(random.random() * len(parent1))
    partB = int(random.random() * len(parent1))
    ini = min(partA, partB)
    end = max(partA, partB)
    binary = []
    for i in range (0, len(parent1)):
        binary.append(randint(0,1))

    for i in range(ini, end):
        childP1.append(parent1[i])
    childP2 = [item for item in parent2 if item not in childP1]
    child = childP1 + childP2
    return child
    

def crossingPopulation(matingpool):
    children = []
    length = len(matingpool) - 10
    pool = random.sample(matingpool, len(matingpool))
    for i in range(0,10):
        children.append(matingpool[i])
    for i in range(0, length):
        child = crossing(pool[i], pool[len(matingpool)-i-1])
        children.append(child)
    return children

def mutate(individual):
    for aux1 in range(len(individual)):        
        aux2 = int(random.random() * len(individual))
        c1 = individual[aux1]
        c2 = individual[aux2]
        individual[aux1] = c1
        individual[aux2] = c2
    return individual

def mutatePopulation(population):
    mutatedPop = []
    for ind in range(0, len(population)):
        mutatedInd = mutate(population[ind])
        mutatedPop.append(mutatedInd)
    return mutatedPop

def nextGeneration(currentGen):
    popRanked = routeRanking(currentGen)
    selectionResults = selection(popRanked)
    matingpool = finalSelection(currentGen, selectionResults)
    children = crossingPopulation(matingpool)
    nextGeneration = mutatePopulation(children)
    return nextGeneration

def GA(population, popSize, generations):
    pop = initialPopulation(popSize, population)
    progress = []
    min_values = []
    bestRoute=[]
    best_distance= math.inf
    progress.append(1 / routeRanking(pop)[0][1])
    min_values.append(1 / routeRanking(pop)[-1][1])
    for i in range(0, generations):
        pop = nextGeneration(pop)
        actual_best_distance = 1 / routeRanking(pop)[0][1]
        progress.append(actual_best_distance)
        min_values.append(1/routeRanking(pop)[-1][1])
        if(best_distance == math.inf or actual_best_distance<best_distance):
            bestRoute = pop[routeRanking(pop)[0][0]]

        G2 = nx.Graph()
        #create nodes
        for i in range (0, 20):
            G2.add_node(i, pos = (bestRoute[i].x, bestRoute[i].y))
        #create edges
        for i in range(0, 20-1):
            G2.add_edge(i, i+1)
        plt.figure(1)
        nx.draw(G2, nx.get_node_attributes(G2, 'pos'))
        plt.show(block = False)
        plt.pause(.1)
        plt.clf()
    #print(progress[0])
    plt.figure(2)
    plt.plot(progress, 'g', label = 'Best case')
    plt.plot(min_values, 'r', label = 'Worst case')
    plt.legend()
    plt.ylabel('Distancia')
    plt.xlabel('Generacion')
    #bestRoute = pop[routeRanking(pop)[0][0]]
    return bestRoute

########################################## main ################################################################
print("--------------------TSP PROBLEM--------------------")
print("Define number of INDIVIDUALS:\t")
individuals = int(input())
print("Define number of ITERATIONS:\t")
iterations = int(input())


cityList = []
edge_colors=[]
positions=[]
nroCities=20

G = nx.Graph()
for i in range(0,nroCities):
    cityList.append(City(x=int(random.random() * 200), y=int(random.random() * 200)))
    x=cityList[i].x
    y=cityList[i].y
    positions.append((x,y))

G.add_nodes_from(cityList)
bestRoute = GA(population=cityList, popSize=individuals, generations=iterations)
bestRoute.append(bestRoute[0])

for i in range(0,nroCities):
    for j in range(0,nroCities):
        edge_colors.append('b')
        if(i != j):
            G.add_edge(cityList[i],cityList[j])
        
weights = nx.get_edge_attributes(G,'weight').values()
plt.figure(3)
nx.draw(G,edge_color=edge_colors)
plt.show()

print("Visualize route optimized by GENETIC ALGORITHM? (0/1)")
condition = int(input())
if (condition):
    for i in range(0,nroCities):
        x=bestRoute[i].x
        y=bestRoute[i].y
        x_2=bestRoute[i+1].x
        y_2=bestRoute[i+1].y
        print("Route: (",x,",",y,")->(",x_2,",",y_2,")")
    G2 = nx.Graph()
    #create nodes
    for i in range (0, nroCities):
        G2.add_node(i, pos = (bestRoute[i].x, bestRoute[i].y))
    #create edges
    for i in range(0, nroCities-1):
        G2.add_edge(i, i+1)
    plt.figure(4)
    nx.draw(G2, nx.get_node_attributes(G2, 'pos'))
    plt.show()
    