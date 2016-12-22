#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib.pyplot as plt
import datetime

print "Started line-plotter.py ..."

currTime = datetime.datetime.now().strftime("%s")
vals=[]
f = open("./OutputSD/mm_die.out",'r')

c='b'

for line in f:

  data = line.split("\t")
  
  if data[0] == '2':
    vals.append((data[1], data[2]))
    vals.append((data[3], data[3]))
    vals.append(c)


#fig = plt.figure() 
#ax1=fig.add_subplot(111)
#ax1.errorbar(x1, avgdim1, yerr=errbardim1, fmt='o', c="r")
#ax1.errorbar(x2, avgdim2, yerr=errbardim2, fmt='o', c="b")

plt.plot(*vals)
plt.savefig("./Plots/"+currTime+"bm_ueber_t.png")

print "Lines plotted!"


