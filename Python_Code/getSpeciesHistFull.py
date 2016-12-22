#!/usr/bin/python
# -*- coding: utf-8 -*-

# Extract lines from mm_species.out corresponding to species that existed to a certain point in time

import array
import numpy as np
import matplotlib.pyplot as plt
import datetime
import os
import sys

print "Programm started"


unique_name = sys.argv[1] #"2016_11_17_Het1_mig4"
data_folder = sys.argv[2] #"./Running_Het1_mig4/OutputSD"

if os.path.exists(unique_name):
  pass
else:
  os.mkdir(unique_name)


t_file=open(data_folder + "/mm_spn_nps.out", 'r')
t_file_lines=t_file.readlines()
num_of_outputs = len(t_file_lines)
input_runtime = 25000.

class GrowingList(list):
    def __setitem__(self, index, value):
        if index >= len(self):
            self.extend([0]*(index + 1 - len(self)))
        list.__setitem__(self, index, value)

hist_array = GrowingList()

for i in range(0, len(t_file_lines)):
    
  f_write = open("./" + unique_name + "Data.txt", 'a')
  f_write.close()
  os.remove("./" + unique_name + "Data.txt")

  f_write = open("./" + unique_name + "Data.txt", 'a')
  f_read = open(data_folder + "/mm_species.out",'r')
  
  time_line=t_file_lines[i].split('\t')
  time= input_runtime * (i+1)/num_of_outputs
    
  pit_val=float(time_line[0]) #"point-in-time value" at which species data is to be analyzed

  # get time value of end of simulation

  #for line in f_read:              5791       all p32_4100  priscel  R   17:04:13      1 columbina

    #data = line.split('\t')
    #for i in range(0, (len(data)-3)/3):
      #if float(data[5+i*3]) > pit_val:
	#pit_val = float(data[5+i*3])

  #print "pit_val = " + str(pit_val)



  # go through file and find lines, where all die-out times correspoond to pit_val time

  f_read.close()
  f_read = open(data_folder + "/mm_species.out",'r')

  for line in f_read:
    data = line.split('\t')
    for i in range(0, (len(data)-3)/3):
      if float(data[4+i*3]) <= pit_val and float(data[5+i*3]) >= pit_val and data[0] != "0":
	f_write.write(line)
	break
    else:
      pass
      

  f_write.close()
  f_read.close()


  f_write = open("./" + unique_name + "Hist_Data.txt", 'a')
  f_write.close()
  os.remove("./" + unique_name + "Hist_Data.txt")

  f_write = open("./" + unique_name + "Hist_Data.txt", 'a')
  f_read = open("./" + unique_name + "Data.txt",'r')

  for line in f_read:
    count = 0
    data = line.split('\t')
    for j in range(0, (len(data)-3)/3):
      if float(data[4+j*3]) <= pit_val and float(data[5+j*3]) >= pit_val and data[0] != "0":
	count += 1
    try:
      hist_array[count] += 1
    except IndexError:
      hist_array[count] = 1
    
    f_write.write(str(count)+"\n")
  
  f_write.close()
  f_read.close()

plt.loglog(range(0, len(hist_array)), hist_array, 'b.')
plt.xlabel('Zahl der bevoelkerten Patches')
plt.ylabel('Spezies')
plt.title('Zeitpunkt: ' + str(time))
plt.savefig('./' + unique_name + '/' + unique_name + "_FullHist_" + str(time) + ".png")
plt.close()

print "plot done!"
    
print "Programm ended"