#!/usr/bin/python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib
matplotlib.use('pdf')
import matplotlib.pyplot as plt
import os
import sys
import HistModule
import time
import math

start_time = time.time()

print 'getting species histogram: \n'

unique_name = sys.argv[1]

datafolders = []
for i in range(2, len(sys.argv)):
  datafolders.append(sys.argv[i])

hist_array = HistModule.GrowingList()

for i in range(0, len(datafolders)):
  time_file = open(datafolders[i] + '/mm_spn_nps.out', 'r') #argv[0] -> Outputfolder (output SD)
  species_file_path = datafolders[i] + '/mm_species.out'

  time_array = [float(line.strip('\n').split('\t')[0]) for line in time_file]

  HistModule.addLivingSpeciesHistList(species_file_path, hist_array, time_array)

print 'finished raw histogram list, '
print 'getting partially binned histogram data'

i = 1
binsize = 20
bin_switch = False

print 'binsize = ' + str(binsize)

while i <= len(hist_array):
  try:
    if hist_array[i] != 0 and bin_switch == False:
	i+=1
	
    elif hist_array[i] == 0 and bin_switch == False:
      i = binsize * int(i/binsize)
      bin_switch = True
      
    elif bin_switch == True:
      bin_val = 0
      
      for j in range(0, binsize):
	bin_val += hist_array[i+j]
	hist_array[i+j] = 0
      
      hist_array[int(round(math.sqrt(i*(i+binsize-1))))] = bin_val/binsize
      i+=binsize  
      
    else:
      print "I didn't do anything!"
  except IndexError:
    break

f_write = open(unique_name + '_plot_data.txt', 'w')
f_write.write('hist_array: %s' % hist_array)

plt.loglog(range(0, len(hist_array)), hist_array, 'b.')
plt.savefig('./Plotfolder/' + unique_name + '_binsize_' + str(binsize) +'.png')

print 'finished! That took %s seconds.' % (time.time()-start_time)
print 'plot saved to: ./' + unique_name