#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib
matplotlib.use('pdf')
import matplotlib.pyplot as plt
import datetime
import os
import random

# generiert .txt Datei mit gleicher Struktur wir mm_species.out für Testzwecke

print "Programm started"

try:				#Falls es Datei shon gibt, soll sie ersetzt werden
  os.remove('testfile.txt')
except:
  pass

f_write = open('testfile.txt', 'a')
grid_dim = 25
max_spread = 4
max_time = 100


species_count = random.randrange(10, 11)		# Zufällige Speziesanzahl in einem Intervall (bestimmt die Anzahl der Zeilen)

for i in range(0, species_count):
  line_length = random.randrange(2, max_spread+1)	#Zufällige anzahl Einträge (web id, rein kommen, aussterben)
  for j in range(0, line_length-1):
    web_id = random.randrange(0, grid_dim)
    time_interval = [random.randrange(0, max_time), random.randrange(0, max_time)]
    f_write.write(str(web_id) + '\t' + str(min(time_interval)) + '\t' + str(max(time_interval)) + '\t')
  
  #web_id = random.randrange(0, grid_dim)			# Letzter Eintrag brauch \n statt \t
  #time_interval = [random.randrange(0, max_time), random.randrange(0, max_time)]
  #f_write.write(str(web_id) + '\t' + str(min(time_interval)) + '\t' + str(max(time_interval)) + '\n')
  
  f_write.write(str(22) + '\t' + str(0) + '\t' + str(10) + '\n')
  
  

print "Testfile generated with %s species!" % species_count

print "Programm ended"



