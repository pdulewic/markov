#!/usr/bin/env python
import json
from random import randint
from random import uniform

# nazwa pliku ze swiatem, przygotowanego przez graficzny edytor map
FILENAME = '2.json'
NUMBER_OF_ACTIONS = 5;
ITERATIONS = 10000;
EPSILON = 0.05

json_data = open(FILENAME)
data = json.load(json_data)

width = data["width"]
height = data["height"]
gamma = data["discountingFactor"]
defReward = data["defaultReward"]
p1 = data["p1"]
p2 = data["p2"]
p3 = data["p3"]

map = [[0 for x in range(height)] for y in range(width)] 

forbidden = data["forbidden"]
for i in range(len(forbidden)):
    map[forbidden[i][0]][forbidden[i][1]] = 1

rewardDict = {}
special = data["special"]
for i in range(len(special)):
    map[special[i]["position"][0]][special[i]["position"][1]] = 2
    rewardDict[(special[i]["position"][0],special[i]["position"][1])] = special[i]["reward"]

terminal = data["terminal"]
for i in range(len(terminal)):
    map[terminal[i]["position"][0]][terminal[i]["position"][1]] = 3
    rewardDict[(terminal[i]["position"][0],terminal[i]["position"][1])] = terminal[i]["reward"]


N = [[0 for x in range(NUMBER_OF_ACTIONS)] for y in range(width*height)] 
Q = [[0 for x in range(NUMBER_OF_ACTIONS)] for y in range(width*height)]

def act(position, action):
    roll = uniform(0.0, 1.0)
    if roll < p1:
        pass
    elif roll < p1+p2:
        action += 3
    elif roll < p1+p2+p3:
        action += 1
    else:
        action += 2
    action = action % 4

    newPos = position[:]

    if action == 0:
        newPos[1] += 1
    elif action == 1:
        newPos[0] += 1
    elif action == 2:
        newPos[1] -= 1
    else:
        newPos[0] -= 1
    if newPos[0] == width or newPos[0] < 0 or newPos[1] == height or newPos[1] < 0:
        newPos = position[:]
    if map[newPos[0]][newPos[1]] == 1:
        newPos = position[:]
    reward = 0
    if map[newPos[0]][newPos[1]] == 2 or map[newPos[0]][newPos[1]] == 3:
        reward = rewardDict[(newPos[0],newPos[1])]
    else:
        reward = defReward
    
    return newPos, reward

def maxQ(pos):
    max = -1000000
    for i in range(5):
        if Q[pos[0]*height + pos[1]][i] > max:
            max = Q[pos[0]*height + pos[1]][i]
    return max

def nextAction(pos):
    if uniform(0.0, 1.0) < EPSILON:
        return randint(0,3)
    max = -1000000
    action = 0
    for i in range(4):
        if Q[pos[0]*height + pos[1]][i] > max:
            max = Q[pos[0]*height + pos[1]][i]
            action = i
    return action
    
    

for t in range(ITERATIONS):
    pos = [0,0]  # startowa pozycja
    reward = 0
    action = randint(0,3)
    while map[pos[0]][pos[1]] != 3:
        newPos, rewardChange = act(pos, action)
        N[pos[0]*height + pos[1]][action] += 1
        Q[pos[0]*height + pos[1]][action] += (reward + gamma*maxQ(newPos) - Q[pos[0]*height + pos[1]][action])/N[pos[0]*height + pos[1]][action]
        pos = newPos[:]
        reward = rewardChange
        action = nextAction(pos)
    Q[pos[0]*height + pos[1]][4] = rewardChange

# wartosci do oceny jakosci, obliczone przez algorytm iteracji wartosci
realU = [[77.4526, 79.5936, 81.7354, 81.9384], [78.2495, 80.5997, 84.2724, 84.2609], [0, 70.467, 87.0596, 86.586], [100, 94.5352, 91.5547, 88.8827]]

err = 0
U = [[0 for x in range(height)] for y in range(width)]
for i in range(width):
    for j in range(height):
        U[i][j] = maxQ([i,j])
        err += abs(U[i][j] - realU[i][j])
        print(err)

err = err / (width*height)
print("sredni blad uzytecznosci stanow: ",err)

print(map)
print("---------------------")
print(realU)
print("---------------------")
print(U)
print("---------------------")
print(Q)






