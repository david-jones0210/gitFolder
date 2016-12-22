#!/usr/bin/python
# -*- coding: utf-8 -*-

import matplotlib
matplotlib.use('pdf')
import matplotlib.pyplot as plt
import time
import math
import os
import sys
import ModulesAreaLaws



f_read = open('./Het1_mig9/OutputSD/mm_species.out', 'r')
f_lines = f_read.readlines()
start_time = time.time()

#start_patch = i

patchcount = 100
time_stamp = float(sys.argv[1])
area_counted = 0

species_counter = [0]*patchcount
max_conn = int(math.sqrt(patchcount)-patchcount % 2)
species_counted = 0
species_in_area = []
areas = []
start_patch = 0
#------------ get species_count

ModulesAreaLaws.filter_list_for_timestamp(time_stamp, f_lines)

for conn in range(0, max_conn+1):
  patches_in_area = ModulesAreaLaws.get_patches_in_area(conn, start_patch, patchcount)
  if patches_in_area == []:
    patches_in_area = [start_patch]
    
  areas.append(patches_in_area)
  area_counted += len(patches_in_area)
  
  ModulesAreaLaws.count_del_species(patches_in_area, time_stamp, f_lines, species_counter)
  
  for j in range(0, len(patches_in_area)):
    species_counted += species_counter[patches_in_area[j]]
  species_in_area.append([area_counted, species_counted])

  
f_write = open('datafile_SAL_' + str(time_stamp), 'w')

f_write.write(str(species_in_area))


print 'total number of species counted for start_patch %s: %s' % (start_patch, sum(species_counter))
#print species_counter
print species_in_area
#print areas
print 'finished! That took %s seconds.' % (time.time()-start_time)
