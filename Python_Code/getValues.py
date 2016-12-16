#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib.pyplot as plt
import datetime


print "Programm started"

f_write = open("../Quellcode/Minimodell_Migration_Heterogen/Analysis/Values_spn_nps_conn", 'a')
spn_nps_read = open("../Quellcode/Minimodell_Migration_Heterogen/OutputSD/mm_spn_nps.out",'r')
mc_md_read = open("../Quellcode/Minimodell_Migration_Heterogen/OutputSD/mm_mc_md.out", 'r')

spn_lines = spn_nps_read.readlines()
mc_lines = mc_md_read.readlines()

for i in xrange(0, 1):
  data1 = spn_lines[0].split("\t")
  val1 = data1[1]
  val2 = data1[2].strip("\n")
  data2 = mc_lines[0].split("\t")
  val3 = data2[1].strip("\n")
  f_write.write(val1+"\t"+val2+"\t"+val3+"\n")
  
f_write.close()
spn_nps_read.close()
mc_md_read.close()

print "Programm ended"



