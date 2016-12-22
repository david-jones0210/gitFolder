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

if os.path.exists(unique_name):	#Prüfe, ob Ordner zum speichern vorhanden ist, falls nicht, erstelle ihn
  pass
else:
  os.mkdir(unique_name)


#--------------- original getValues ---------------------

species_read = open(data_folder + "/mm_species.out",'r')	#laden der Input-Dateien
get_max_time = open(data_folder + "/mm_spn_nps.out", 'r')

max_time = float(get_max_time.readlines()[-1].split('\t')[0])	#Feststellen der gesamten Laufzeit (Simulationsintern, Ereignisse)
time_steps = 25000						#Zahl der eingegebenen Laufzeit in der Kommandozeile

species_count = 40.*40.						#Zahl der Patches
time_list = [0.]
species_count_list = [species_count]
species_lines = species_read.readlines()
patch_introduction_list = []
patch_death_list = []


#------------ für unterschiedliche Startspezies----------------

for i in range(0,len(species_lines)-1):
  
  data_raw = species_lines[i].strip('\n').split("\t")
  data1 = [float(column) for column in data_raw]
  
  #for j in range(0, (len(data1)-3)/3):
  if int(data1[4]) == 0:
    for k in range(0, (len(data1)-3)/3):
      patch_introduction_list.append(float(data1[4+k*3]))
      patch_death_list.append(float(data1[5+k*3]))

patch_introduction_list = sorted(patch_introduction_list)
patch_death_list = sorted(patch_death_list)

i = 0
j = 0

while i <= len(patch_introduction_list) and j < len(patch_death_list):
  
  if i < len(patch_introduction_list) and j < len(patch_death_list):
    
    if patch_introduction_list[i] == 0:
      i += 1
    
    elif patch_introduction_list[i] < patch_death_list[j]:
      species_count += 1
      species_count_list.append(species_count)
      time_list.append(patch_introduction_list[i]* 25000/max_time)
      i += 1
		      
    elif patch_introduction_list[i] > patch_death_list[j]:
      species_count -= 1
      species_count_list.append(species_count)
      time_list.append(patch_introduction_list[i]* 25000/max_time)
      j += 1
    
    elif patch_introduction_list[i] == patch_death_list[j]:
      i += 1
      j += 1
      
    else:
      print 'something unforeseen has happened!'
      break
      
  elif i == len(patch_introduction_list):
    species_count -= 1
    species_count_list.append(species_count)
    time_list.append(patch_death_list[j] * 25000/max_time)
    j+=1
    
  else:
    print 'somehing else unforeseen happened!'
    break
      
#--------------------------------------------------------------

species_read.close()
print species_count_list[-1]

plt.title('number of initial species remaining over time (' + unique_name + ')')
plt.ylabel('species')
plt.xlabel('number of time steps (out of ' + str(time_steps) + ')')
plt.semilogy(time_list, species_count_list, 'b.')
plt.savefig('./' + unique_name + '/' + unique_name + '_plot_die_out_of_start_species.png')

print "Done!" 
