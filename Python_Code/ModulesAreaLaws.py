#!/usr/bin/python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import os
import random
import math

def moveUP(curr_pos, patches):
  target_pos = (curr_pos + patches - math.sqrt(patches)) % patches
  return target_pos


def moveLEFT(curr_pos, patches):
  if curr_pos % math.sqrt(patches) == 0:
    target_pos = curr_pos + math.sqrt(patches) - 1
  else:
    target_pos = curr_pos - 1
  return target_pos


def moveDOWN(curr_pos, patches):
  target_pos = (curr_pos + math.sqrt(patches)) % patches
  return target_pos


def moveRIGHT(curr_pos, patches):
  if (curr_pos + 1) % math.sqrt(patches) == 0:
    target_pos = curr_pos - math.sqrt(patches) + 1
  else:
    target_pos = curr_pos + 1
  return target_pos



def filter_list_for_timestamp(time_stamp, line_list):
  i = 0
  species_alive = False
  while i < len(line_list):
    data_raw = line_list[i].strip('\n').split("\t")
    data = [float(column) for column in data_raw]
   
    for j in range(3, len(data), 3):				#Für jeden Eintrag prüfe ob zu bestimmtem Zeitpunkt spezies gelebt hat, wenn nicht, lösche Species aus Liste
      if data[j+1] <= time_stamp and data[j+2] >= time_stamp:
	species_alive = True
	break
    
    if species_alive == False:
      del line_list[i]
    else:
      species_alive = False
      i += 1
      

def count_del_species(patches_in_area, time_stamp, line_list, species_count):
  i = 0
  line_found = False
  while i < len(line_list):	#Für jede Zeile wandle sie in Liste von floats um	
    data_raw = line_list[i].strip('\n').split("\t")
    data = [float(column) for column in data_raw]
      
    for j in range(3, len(data), 3):	#Für jeden Eintrag prüfe ob zu bestimmtem Zeitpunkt spezies auf gesuchtem patch gelebt hat, wenn ja, zähle auf patch +1 und lösche Spezies aus Liste
      if data[j] in patches_in_area and data[j+1] <= time_stamp and data[j+2] >= time_stamp:
	species_count[int(data[j])] += 1
	del line_list[i]
	line_found = True
	break
    
    if line_found == False:
      i += 1
    else:
      line_found = False



# go through grid starting from starting point to all points reachable with given number of connectors counter clockwise
# returns list of patch indexes that are num_connectors away from the starting point (moving outward)
    
def get_patches_in_area(num_connectors, start_patch, patches):
    
  patches_in_run = []
  curr_pos = start_patch

  for UL_i in range(max(0, num_connectors - ((patches+1) % 2) - int((math.sqrt(patches)-1)/2)), min(num_connectors, int((math.sqrt(patches)-1)/2)+ 1 + ((patches+1) % 2))):			#Move through upper left quadrant (full up -to- 1 up, num_connectorsector-1 left)
    for up_times in range(0, num_connectors-UL_i):
      curr_pos = moveUP(curr_pos, patches)
    for left_time in range(0, UL_i):
      curr_pos = moveLEFT(curr_pos, patches)
    patches_in_run.append(int(curr_pos))
    curr_pos = start_patch
    
  for BL_i in range(max(0, num_connectors - ((patches+1) % 2) - int((math.sqrt(patches)-1)/2)), min(num_connectors, int((math.sqrt(patches)-1)/2)+1)):				#Move through bottom left quadrant (full left -to- 1 left, num_connectorsector-1 down)
    for up_times in range(0, num_connectors-BL_i):
      curr_pos = moveLEFT(curr_pos, patches)
    for left_time in range(0, BL_i):
      curr_pos = moveDOWN(curr_pos, patches)
    patches_in_run.append(int(curr_pos))
    curr_pos = start_patch
    
  for BR_i in range(max(0, num_connectors - int((math.sqrt(patches)-1)/2)), min(num_connectors, int((math.sqrt(patches)-1)/2)+1)):				#Move through bottom right quadrant (full down -to- 1 down, num_connectorsector-1 right)
    for down_times in range(0, num_connectors-BR_i):
      curr_pos = moveDOWN(curr_pos, patches)
    for right_time in range(0, BR_i):
      curr_pos = moveRIGHT(curr_pos, patches)
    patches_in_run.append(int(curr_pos))
    curr_pos = start_patch
    
  for RU_i in range(max(0, num_connectors - int((math.sqrt(patches)-1)/2)), min(num_connectors, int((math.sqrt(patches)-1)/2)+1+((patches+1) % 2))):				#Move through top right quadrant (full right -to- 1 right, num_connectorsector-1 up)
    for right_time in range(0, num_connectors-RU_i):
      curr_pos = moveRIGHT(curr_pos, patches)
    for up_times in range(0, RU_i):
      curr_pos = moveUP(curr_pos, patches)
    patches_in_run.append(int(curr_pos))
    curr_pos = start_patch
 
  return patches_in_run

# ----------------- TESTAREA -----------------

#starting_point = 4
#grid_size = 25

#read_pos = starting_point
#read_pos = moveUP(read_pos, grid_size)

#print 'starting point %s and 1 up: %s' % (starting_point, read_pos)

#read_pos = moveLEFT(read_pos, grid_size)

#print 'after 1 left: %s' % read_pos

#read_pos = moveDOWN(read_pos, grid_size)

#print 'after 1 down: %s' % read_pos

#read_pos = moveRIGHT(read_pos, grid_size)

#print 'and this should be the starting point: %s' % read_pos
