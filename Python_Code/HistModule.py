#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib.pyplot as plt
import datetime
import os
import sys

class GrowingList(list):
    def __setitem__(self, index, value):
        if index >= len(self):
            self.extend([0]*(index + 1 - len(self)))
        list.__setitem__(self, index, value)



def getLivingSpeciesLineList(species_file, eval_time):	#species_file -> mm_species.out ; eval_time -> sim-interne Zeit (einzelne Ereignisse)
  
  f_read = open(species_file, 'r')
  line_list = []
    
  for line in f_read:
    data = line.strip('\n').split('\t')
    data = [float(column) for column in data] 		#get floats for all values
    for i in range(0, (len(data)-3)/3):
      if float(data[4+i*3]) <= eval_time and float(data[5+i*3]) >= eval_time and data[0] != "0":
	line_list.append(data)

  f_read.close()
  return line_list



def addLivingSpeciesHistList(species_file_path, hist_array, eval_time_list):	#species_file -> mm_species.out ; hist_array -> array für histogramm (GrowingList) 
										  #eval_time -> sim-interne Zeit (einzelne Ereignisse)
  print ('running "addLivingSpeciesHistList" for %s points in time' % len(eval_time_list))
  species_file = open(species_file_path, 'r')
  for line in species_file:
    
    data = line.strip('\n').split('\t')
    data = [float(column) for column in data]
    
    for i in range(0, len(eval_time_list)):
      count = 0      
      for j in range(0, (len(data)-3)/3):
	if float(data[4+j*3]) <= eval_time_list[i] and float(data[5+j*3]) >= eval_time_list[i] and data[0] != 0.:
	  count+=1
      if count == 0:
	pass
      else:
	try:
	  hist_array[count] += 1
	except IndexError:
	  hist_array[count] = 1
	
  species_file.close()
  
#def getStartDieList