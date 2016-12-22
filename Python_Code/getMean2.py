#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib.pyplot as plt
import datetime
import math
import os
import sys

print "Getting mean values"

cut_off_num = int(sys.argv[3])	 #Falls eine bestimmte Zahl der Werte (cut_off_num) von der Aufbauphase nicht Berücksichtigt werden sollen
unique_name = sys.argv[1] #"2016_11_17_Het1_mig9_"
data_folder = sys.argv[2] #"./Running_Het1_mig9/OutputSD"


#--------------- original getValues ---------------------

f_write = open("./" + unique_name + "_spn_nps_conn_cutoff_" + str(cut_off_num) + ".txt", 'w')
f_write.close()
os.remove("./" + unique_name + "_spn_nps_conn_cutoff_" + str(cut_off_num) + ".txt")

f_write = open("./" + unique_name + "_spn_nps_conn_cutoff_" + str(cut_off_num) + ".txt", 'a')
spn_nps_read = open(data_folder + "/mm_spn_nps.out",'r')
mc_md_read = open(data_folder + "/mm_mc_md.out", 'r')

spn_lines = spn_nps_read.readlines()
mc_lines = mc_md_read.readlines()

for i in xrange(0, len(spn_lines)):
  data1 = spn_lines[i].split("\t")
  time = data1[0]
  val1 = data1[1]
  val2 = data1[2].strip("\n")
  data2 = mc_lines[i].split("\t")
  val3 = data2[1].strip("\n")
  f_write.write(time + '\t' + val1+"\t"+val2+"\t"+val3+"\n")


f_write.close()
spn_nps_read.close()
mc_md_read.close()

#--------------  original getMeanValues ------------------


f_read = open("./" + unique_name + "_spn_nps_conn_cutoff_" + str(cut_off_num) + ".txt",'r')
f_write = open("./" + unique_name + "_spn_nps_conn_cutoff_" + str(cut_off_num) + ".txt", 'a')

datalines = f_read.readlines()

mean_spn = 0.
mean_nps = 0.
mean_conn = 0.

nps_var = 0.
spn_var=0.
conn_var=0.

time_list = []
spn_list = []
nps_list = []
conn_list = []

if cut_off_num==0:

  for i in range(0, len(datalines)):
    data = datalines[i].strip("\n").split("\t")
    
    mean_spn += float(data[1])/(len(datalines))
    mean_nps += float(data[2])/(len(datalines))
    mean_conn += float(data[3])/(len(datalines))
    
    spn_list.append(float(data[1]))
    nps_list.append(float(data[2]))
    conn_list.append(float(data[3]))
    
    time_list.append(float(data[0]))
    
else:
  for i in range(cut_off_num, len(datalines)):
    data = datalines[i].strip("\n").split("\t")
    
    mean_spn += float(data[1])/(len(datalines)-cut_off_num)
    mean_nps += float(data[2])/(len(datalines)-cut_off_num)
    mean_conn += float(data[3])/(len(datalines)-cut_off_num)
    
    spn_list.append(float(data[1]))
    nps_list.append(float(data[2]))
    conn_list.append(float(data[3]))
    
    time_list.append(float(data[0]))
      
spn_var = np.std(spn_list)
nps_var = np.std(nps_list)
conn_var = np.std(conn_list)

f_write.write("\n\nmean spn +/- std dev \t mean nps +/- std dev \t mean_conn \n" + str(mean_spn) + " +/- " + str(spn_var) + "\t" + str(mean_nps) + " +/- " + str(nps_var) + "\t" + str(mean_conn) + " +/- " + str(conn_var))

f_read.close()
f_write.close()

plt.figure(1)
plt.subplot(311)
plt.title('spn')
plt.plot(time_list, spn_list, 'bo-')
plt.plot([0, time_list[len(time_list)-1]], [mean_spn, mean_spn], color = '0.75', linestyle = '--' )
plt.errorbar(time_list[len(time_list)-1]/2, mean_spn, yerr=spn_var, fmt='o', color = '0.75')

plt.subplot(312)
plt.title('nps')
plt.plot(time_list, nps_list, 'ro-')
plt.plot([0, time_list[len(time_list)-1]], [mean_nps, mean_nps], color = '0.75', linestyle = '--' )
plt.errorbar(time_list[len(time_list)-1]/2, mean_nps, yerr=nps_var, fmt='o', color = '0.75')

plt.subplot(313)
plt.title('conn')
plt.plot(time_list, conn_list, 'go-')
plt.plot([0, time_list[len(time_list)-1]], [mean_conn, mean_conn], color = '0.75', linestyle = '--' )
plt.errorbar(time_list[len(time_list)-1]/2, mean_conn, yerr=conn_var, fmt='o', color = '0.75')

plt.tight_layout()
plt.savefig(unique_name + '_plots_spn_nps_conn_cutoff_' + str(cut_off_num) + '.png')


print "Done!" 
