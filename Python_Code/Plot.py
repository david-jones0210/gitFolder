#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib.pyplot as plt

def loaddata(folder, filename, maxlen, start = 0, end = -1):
  filename = "./" + folder + "/" + filename
  f = open(filename, "r")
      
  S = []
  time = []
  patch = []
  values = []

  for line in f:
    data = line.split("\t")
    ss = int(data[0])
    S.append(ss)
    time.append(data[1])
    patch.append(data[2])
    varr = []
    for i in xrange(3, len(data)):
      varr.append(data[i])
    values.append(varr)
  f.close()
  
  del S[end:]
  del time[end:]
  del patch[end:]
  del values[end:]
  
  del S[:start]
  del time[:start]
  del patch[:start]
  del values[:start]
  
  while len(time) > maxlen:
    del S[::2]
    del time[::2]
    del patch[::2]
    del values[::2]
    
  return (S, time, patch, values)
  
# -------------------- mm_bm.out data (dim, t, patch, bm) --------------------

f = "Output"
dim, t, patch, bm = loaddata(f, "mm_bm.out", 100000, 0, 100000)

  #print "loading done"

  #plt.plot(t, dim, "b.")

  #plt.title("dim ueber t")
  #plt.savefig("t_dim"+f+".pdf");
  #plt.savefig("t_dim"+f+".png");
  #plt.show()


  #parameter (z.B. x) ist in der Ausgabedatei durchgefahren und wird geplottet.
  #for i in xrange(30):
  #plt.plot(t[1000*i:1000*(i+1)], dim[1000*i:1000*(i+1)], color = (i/30.0,0.2,1.0-i/30.0))


for i in xrange(max(dim)):
  plt.plot(t, [bm[j][i] if len(bm[j]) > i else 0 for j in xrange(len(bm))], "b,")

plt.title("bm ueber t")
plt.savefig("Output2/bm_ueber_t_"+f+".pdf")
plt.savefig("Output2/bm_ueber_t_"+f+".png")
plt.show()
  
# -------------------- mm_tropos.out data (dim, t, patch, tl) --------------------


  #for i in xrange(max(dim)):
    #plt.plot(t, [tl[j][i] if len(tl[j]) > i else 0 for j in xrange(len(tl))], "r.")

  #plt.title("tl ueber t")
  #plt.savefig("TestFolder/tl_ueber_t_"+f+".pdf")
  #plt.savefig("TestFolder/tl_ueber_t_"+f+".png")
  #plt.show()
  
# -------------------- mm_trophdistri.out data (dim, t, patch, tl) --------------------

  #f = "TestFolder"
  #dim, t, patch, td = loaddata(f, "mm_trophdistri.out", 10000, 0, 10000)

  #for i in range(0,len(td)): <--- funktioniert nur für td[i][0,1,2] bzw immer dann, wenn von anfang an das array der trophischen level gefüllt ist, muss noch bearbeitet werden.
    #plt.plot(t[i], [td[i][2]], "r.")

  #plt.title("td ueber t")
  #plt.savefig("TestFolder/td_ueber_t_"+f+".pdf")
  #plt.show()
