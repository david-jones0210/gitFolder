#!/usr/bin/python
# -*- coding: utf-8 -*-

#import matplotlib
#matplotlib.use('pdf')
import matplotlib.pyplot as plt
import time
import math
import os
import sys
import ModulesAreaLawsTest

print 'program started...'

data_list = []

y_list = [0, 86399, 38662, 21413, 13321, 8887, 6102, 4201, 3022, 2314, 1736, 1285, 1052, 782, 642, 540, 429, 381, 314, 270, 236, 207, 155, 122, 156, 129, 106, 97, 102, 76, 77, 70, 62, 63, 60, 57, 59, 57, 25, 43, 44, 41, 41, 37, 39, 31, 40, 27, 41, 26, 15, 20, 27, 25, 22, 20, 13, 18, 17, 19, 17, 27, 15, 8, 15, 14, 17, 11, 14, 11, 13, 18, 15, 9, 12, 13, 9, 17, 13, 10, 8, 9, 3, 11, 11, 3, 9, 6, 6, 9, 10, 11, 8, 6, 7, 3, 12, 3, 11, 6, 9, 6, 3, 8, 4, 2, 7, 5, 4, 3, 9, 4, 3, 3, 9, 4, 5, 6, 5, 3, 1, 8, 8, 3, 1, 3, 7, 3, 5, 3, 7, 5, 1, 2, 3, 4, 2, 4, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
x_list = range(0, len(y_list))

data = [[1, 25], [5, 74], [13, 145], [25, 220], [41, 326], [61, 435], [85, 575], [113, 721], [145, 899], [181, 1052], [221, 1246], [265, 1444], [313, 1694], [365, 1941], [421, 2185], [481, 2472], [545, 2787], [613, 3082], [685, 3408], [761, 3713], [839, 4060], [915, 4335], [987, 4616], [1055, 4874], [1119, 5143], [1179, 5397], [1235, 5618], [1287, 5818], [1335, 6003], [1379, 6165], [1419, 6324], [1455, 6440], [1487, 6549], [1515, 6648], [1539, 6735], [1559, 6808], [1575, 6865], [1587, 6906], [1595, 6932], [1599, 6940], [1600, 6941]]

#for i in range(0, len(y_list)):
  #if x_list[i] != 0 and y_list[i] != 0:
    #data_list.append([math.log(x_list[i]), math.log(y_list[i])])

x_data = []
y_data = []

for i in range(0, len(data)):
  x_data.append(data[i][0])
  y_data.append(data[i][1])

plt.plot(x_data, y_data, 'bo-')
plt.show()

#f_write = open('hist_data_Hom1_mig4.txt', 'w')
#f_write.write('%s' % data_list)


print 'done!'