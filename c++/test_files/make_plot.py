import numpy as np
import matplotlib.pyplot as plt

def read(filename):
    duration_tmp = []
    size = []
    with open(filename) as f:
        for line in f:
            temp_list = line.split(' ')
            size.append(float(temp_list[0]))
            duration_tmp.append(float(temp_list[1]))

    for i in range(len(duration_tmp)):
            duration_tmp[i]/=size[i]

    return duration_tmp

################################################

size = []
duration = []
with open('speed_unbalanced') as f:
    for line in f:
        temp_list = line.split(' ')
        size.append(float(temp_list[0]))
        duration.append([float(i) for i in temp_list[1:10]])

for i in range(len(duration)):
    for j in range(len(duration[i])):
        duration[i][j]/=size[i]

ave_unbalanced = [np.mean(duration[i]) for i in range(len(duration))]
balanced = read('speed_balanced')
std = read('speed_std')

for i in range(len(duration)):
    for j in range(len(duration[i])):
        plt.plot(size[i], duration[i][j], marker = '.', color = 'blue')
plt.plot(size, ave_unbalanced)
plt.plot(size, balanced)
plt.plot(size, std)
plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
plt.xscale('log')


plt.savefig('Search_cost.pdf')
