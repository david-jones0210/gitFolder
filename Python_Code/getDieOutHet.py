#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib
matplotlib.use('pdf')
import matplotlib.pyplot as plt
import datetime
import math
import os
import sys

print "Getting start species info"

unique_name = sys.argv[1]	#Name der Datei (z.B 2016_11_22_Het_40x40)
data_folder = sys.argv[2] 	#Ordner in dem die Simulationsdateien (mm_species.out...) gespeichert sind


#--------------- original getValues ---------------------

species_read = open(data_folder + "/mm_species.out",'r')	#laden der Input-Dateien
get_max_time = open(data_folder + "/mm_spn_nps.out", 'r')

max_time = float(get_max_time.readlines()[-1].split('\t')[0])	#Feststellen der gesamten Laufzeit (Simulationsintern, Ereignisse)
time_steps = 25000						#Zahl der eingegebenen Laufzeit in der Kommandozeile

species_count = 40.*40.						#Zahl der Patches
time_list = [0.]
species_count_list = [species_count]
species_lines = species_read.readlines()

#------------ für unterschiedliche Startspezies----------------

for i in range(0,len(species_lines)-1):
  
  data_raw = species_lines[i].strip('\n').split("\t")
  data1 = [float(column) for column in data_raw]
  
  for j in range(0, (len(data1)-3)/3):
    if int(data1[4+j*3]) == 0:
      species_count = species_count-1
      time_list.append(time_steps*float(max(data1))/max_time)
      species_count_list.append(species_count)
      
#--------------------------------------------------------------

species_read.close()
print species_count_list[-1]

plt.title('number of initial species remaining over time (' + unique_name + ')')
plt.ylabel('species')
plt.xlabel('number of time steps (out of ' + str(time_steps) + ')')
plt.semilogy(sorted(time_list), species_count_list, 'b.')
plt.savefig('./' + unique_name + '_plot_die_out_of_start_species.png')

print "Done!" 
