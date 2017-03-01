import matplotlib.pyplot as plt
import math
import numpy as np

f1 = open("result.txt", "r")
f2 = open("result2.txt", "r")
x1 = []
y1 = []
for line in f1.readlines():
    l = line.strip().split(',')
    x1.append(math.log(int(l[0])))
    if int(l[1]) > 0:
        y1.append(math.log(int(l[1])))
    else:
        y1.append(0.0)
        
x2 = []
y2 = []
for line in f2.readlines():
    l = line.strip().split(',')
    x2.append(math.log(int(l[0])))
    if int(l[1]) > 0:
        y2.append(math.log(int(l[1])))
    else:
        y2.append(0.0)
        

plt.scatter(x1, y1, s=1.0, c='r')
plt.plot(np.unique(y1), np.poly1d(np.polyfit(x1, y1, 1))(np.unique(y1)))
plt.axis([0, math.ceil(max(x1)), 0, math.ceil(max(y1))])
plt.show()