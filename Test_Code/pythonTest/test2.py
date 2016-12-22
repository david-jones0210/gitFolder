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
  if (curr_pos + 1) % patches == 0:
    target_pos = curr_pos - math.sqrt(patches) + 1
  else:
    target_pos = curr_pos + 1
  return target_pos


patches_in_run = [12]
start_patch = 12
patches = 49

curr_pos = start_patch

for num_connectors in range(0, 7):
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
    for up_times in range(0, num_connectors-BR_i):
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
  
print patches_in_run
print sorted(patches_in_run)
print len(patches_in_run)