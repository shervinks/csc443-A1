import matplotlib.pyplot as plt
import math
import xlsxwriter

f1 = open("result.txt", "r")
f2 = open("result2.txt", "r")
x1 = []
y1 = []
x1_real = []
y1_real = []
for line in f1.readlines():
    l = line.strip().split(',')
    x1.append(math.log(int(l[0])))
    x1_real.append(int(l[0]))
    if int(l[1]) > 0:
        y1.append(math.log(int(l[1])))
        y1_real.append(int(l[1]))
    else:
        y1.append(0.0)
        y1_real.append(0)
x2 = []
y2 = []
x2_real = []
y2_real = []
for line in f2.readlines():
    l = line.strip().split(',')
    x2.append(math.log(int(l[0])))
    x2_real.append(int(l[0]))
    if int(l[1]) > 0:
        y2.append(math.log(int(l[1])))
        y2_real.append(int(l[1]))
    else:
        y2.append(0.0)
        y2_real.append(0)

#'''Scatter plot out-degree'''
#plt.scatter(x1, y1, s=1.0, c='r')
#plt.axis([0, math.ceil(max(x1)), 0, math.ceil(max(y1) + 1)])
#plt.xlabel('log k')
#plt.ylabel('log f(k)')
#plt.plot([0, 8.2], [15.5, 0], linewidth=0.5)
#plt.title("Out-Degree Distibution")
#plt.show()

#'''Scatter plot in-degree'''
#plt.scatter(x2, y2, s=1.0, c='r')
#plt.axis([0, math.ceil(max(x1)), 0, math.ceil(max(y1) + 1)])
#plt.xlabel('log k')
#plt.ylabel('log f(k)')
#plt.plot([0, 7.95], [15.8, 0], linewidth=0.5)
#plt.title("In-Degree Distibution")
#plt.show()

#"""Write histogram in excel file"""
#workbook = xlsxwriter.Workbook('histogram.xlsx')
#page = workbook.add_worksheet()

#for i in range(0,214381):
    #page.write(i, 0, x1_real[i])
    #page.write(i, 1, y1_real[i])
    #page.write(i, 3, x2_real[i])
    #page.write(i, 4, y2_real[i])    
    
#workbook.close()