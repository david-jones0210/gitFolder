
print "Getting mean values"

cut_off_num = 0 #Falls eine bestimmte Zahl der Werte (cut_off_num) von der Aufbauphase nicht Berücksichtigt werden sollen

#--------------  original getMeanValues ------------------


f_read1 = open("./Data_Het1_16.txt",'r')
f_read2 = open("./Data_Het2_16.txt",'r')
f_read3 = open ("./Data_Hom1_16.txt", 'r')
f_read4 = open ("./Data_Hom2_16.txt", 'r')


datalines = f_read.readlines()
datalines2 = f_read2.readlines()


mean_spn_list = []
mean_nps_list = []
mean_conn_list = []

nps_var_list = []
spn_var_list = []
conn_var_list = []

time_list = []

spn_list = []
nps_list = []
conn_list = []


if cut_off_num==0:

  for i in range(0, len(datalines)):
    data = datalines[i].strip("\n").split("\t")
    data2 = datalines2[i].strip("\n").split("\t")
    
    mean_spn += float(data[1])/(len(datalines))
    mean_nps += float(data[2])/(len(datalines))
    mean_conn += float(data[3])/(len(datalines))
    
    spn_list.append(float(data[1]))
    nps_list.append(float(data[2]))
    conn_list.append(float(data[3]))
    
    
    mean_spn2 += float(data2[1])/(len(datalines2))
    mean_nps2 += float(data2[2])/(len(datalines2))
    mean_conn2 += float(data2[3])/(len(datalines2))
    
    spn_list2.append(float(data2[1]))
    nps_list2.append(float(data2[2]))
    conn_list2.append(float(data2[3]))
    
    time_list.append(float(data[0]))
    
else:
  for i in range(cut_off_num, len(datalines)):
    data = datalines[i].strip("\n").split("\t")
    data2 = datalines2[i].strip("\n").split("\t")
    
    mean_spn += float(data[1])/(len(datalines)-cut_off_num)
    mean_nps += float(data[2])/(len(datalines)-cut_off_num)
    mean_conn += float(data[3])/(len(datalines)-cut_off_num)
    
    spn_list.append(float(data[1]))
    nps_list.append(float(data[2]))
    conn_list.append(float(data[3]))
    
    
    mean_spn2 += float(data2[1])/(len(datalines2)-cut_off_num)
    mean_nps2 += float(data2[2])/(len(datalines2)-cut_off_num)
    mean_conn2 += float(data2[3])/(len(datalines2)-cut_off_num)
    
    spn_list2.append(float(data2[1]))
    nps_list2.append(float(data2[2]))
    conn_list2.append(float(data2[3]))
    
    time_list.append(float(data[0]))
      
spn_var = np.std(spn_list)
spn_var2 = np.std(spn_list2)
nps_var = np.std(nps_list)
nps_var2 = np.std(nps_list2)
conn_var = np.std(conn_list)
conn_var2 = np.std(conn_list2)


f_read.close()
f_read2.close()

plt.figure(1)
plt.subplot(311)
plt.title('spn')
plt.plot(time_list, spn_list, 'bo-', label='Hetereogenous initial species')
plt.plot([0, time_list[len(time_list)-1]], [mean_spn, mean_spn], color = 'blue', linestyle = '--', alpha = 0.5)
plt.errorbar(time_list[len(time_list)-1]/2, mean_spn, yerr=spn_var, fmt='.', color = 'blue', alpha = 0.5)

plt.plot(time_list, spn_list2, 'ro-', label='Identical initial species')
plt.plot([0, time_list[len(time_list)-1]], [mean_spn2, mean_spn2], 'r--', alpha = 0.5)
plt.errorbar(time_list[len(time_list)-1]/2, mean_spn2, yerr=spn_var2, fmt='.', color = 'red', alpha = 0.5)

plt.legend(loc='lower right', fontsize = 'small')


plt.subplot(312)
plt.title('nps')
plt.plot(time_list, nps_list, 'bo-', label='Hetereogenous initial species')
plt.plot([0, time_list[len(time_list)-1]], [mean_nps, mean_nps], color = 'blue', linestyle = '--', alpha = 0.5)
plt.errorbar(time_list[len(time_list)-1]/2, mean_nps, yerr=nps_var, fmt='.', color = 'blue', alpha = 0.5)

plt.plot(time_list, nps_list2, 'ro-')
plt.plot([0, time_list[len(time_list)-1]], [mean_nps2, mean_nps2], 'r--', alpha = 0.5)
plt.errorbar(time_list[len(time_list)-1]/2, mean_nps2, yerr=nps_var2, fmt='.', color = 'red', alpha = 0.5)


plt.subplot(313)
plt.title('conn')
plt.plot(time_list, conn_list, 'bo-')
plt.plot([0, time_list[len(time_list)-1]], [mean_conn, mean_conn], color = 'blue', linestyle = '--', alpha = 0.5)
plt.errorbar(time_list[len(time_list)-1]/2, mean_conn, yerr=conn_var,fmt='.', color = 'blue', alpha = 0.5)

plt.plot(time_list, conn_list2, 'ro-')
plt.plot([0, time_list[len(time_list)-1]], [mean_conn2, mean_conn2], 'r--', alpha = 0.5)
plt.errorbar(time_list[len(time_list)-1]/2, mean_conn2, yerr=conn_var2, fmt='.', color = 'red', alpha = 0.5)


plt.tight_layout()
plt.savefig('Data_Comparison_2016_11_16_Het_Hom_mig08_2.png')
plt.show()

print "Done!"

print "Programm ended"
