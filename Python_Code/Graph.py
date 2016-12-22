#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import math
import graph_tool.all as gr
import pylab
import matplotlib.pyplot as plt

def loaddata(filename, time):
  filename = "./Output2/" + filename
  f = open(filename, "r")
  
  S = 0
  patch = 0
  values = []

  for line in f:
    data = line.split("\t")
    if int(data[1]) == time:
      S = int(data[0])
      patch = int(data[2])
      for i in xrange(3, len(data)):
	values.append(float(data[i]))
      break
  f.close()
    
  return (S, time, patch, values)
  
    
def draw_web(dim, alpha, tl, time): 
  # Skalierungen fur schone Darstellung
  scaleVOffset = 1.0 / min([x for x in alpha if x > 0]) + 10000.0
  scaleV = 1.5
  def myLog(x):  
      if x > 0:
	  return math.log(x * scaleVOffset) 
      else:
	  return 0

  # LOS: initialisiere einen Graph mit "maxLevel" trophischen Ebenen
  maxLevel = int(max(tl)+1)			
  g = gr.Graph()

  # Definiere Eigenschaften: Populationsgrosse, Linkstarke, trophische Position
  e_size = g.new_edge_property("double")
  v_size = g.new_vertex_property("double")
  inmigrant = g.new_vertex_property("double")
  position = g.new_vertex_property("vector<double>")

  # Kreiere Knoten und weise Populationsgrossen zu
  for i in xrange(dim):
      v = g.add_vertex()
      v_size[v] = 30
      inmigrant[v] = 0
  inmigrant[g.vertex(0)] = 0

  # Kreiere Kanten und weise Linkstarken zu, Durchzahlen
  for i in xrange(dim):
    v_target = g.vertex(i)
    for j in xrange(dim):
      link = alpha[i*dim + j]
      if (link > 0 and v_size[g.vertex(i)] > 0 and v_size[g.vertex(j)] > 0):
	v_source = g.vertex(j)
	e = g.add_edge(v_source, v_target)
	e_size[e] = myLog(alpha[i*dim + j])*scaleV

  # Auslesen und Zuweisung der trophische Positionen
  for i in xrange(dim):
    if (v_size[g.vertex(i)] > 0):
      position[g.vertex(i)].append(i/dim)   	# dummy-x-Wert
      position[g.vertex(i)].append(1.0 - tl[i] / maxLevel)
    else:
      position[g.vertex(i)].append(0.5)   		# Relikte ohne Biomass
      position[g.vertex(i)].append(1.0) 


  # Durchzahlen, wie viele Species auf welchem Level existieren
  NumSTL = pylab.zeros(maxLevel, int)   
  for i in xrange(dim):
    for j in xrange(maxLevel - 1): 
      if (position[g.vertex(i)][1] > (1 - (j + 1.5) / maxLevel)) and (position[g.vertex(i)][1] < (1 - (j + 0.5) / maxLevel)):
	NumSTL[j] = NumSTL[j] + 1

  # dummyposition nach korpergrosse auffullen
  idxTL = pylab.zeros(maxLevel, int)
  for i in xrange(dim):
    for j in xrange(maxLevel - 1): 
      if (position[g.vertex(i)][1] > (1 - (j + 1.5) / maxLevel)) and (position[g.vertex(i)][1] < (1 - (j + 0.5) / maxLevel)):
	idxTL[j] = idxTL[j] + 1
	position[g.vertex(i)][0] = float(idxTL[j]) / float(NumSTL[j]) - 1 / (2 * float(NumSTL[j]))
  #Resource in die Mitte:
  position[g.vertex(0)][0] = 0.5


  # Graph mit Grossensortierung:
  gr.graph_draw(g, pos=position, vertex_size=v_size, vertex_fill_color=inmigrant, edge_pen_width=e_size, output="Output2/graph_" + str(time) + ".png")
  
def trophic_level(dim, sizes, N):
  #Berechnet die trophischen Level auf allen patches zu einem Zeitpunkt
  tl = [list(N[i]) for i in xrange(len(N))]
  for p in xrange(patches):
    tl[0][p] = 0.0
    for i in xrange(1,S+1):
      tl[i][p] = 0.0
      norm = 0.0
      #if N[i][p] > 0:
      for j in xrange(i):
	tl[i][p] += N[j][p] * tl[j][p] * math.exp(-4.0*(sizes[i]-sizes[j]-2)**2) #hier steckt dist=2 und sdev= 0.5 drin
	norm += N[j][p] * math.exp(-4.0*(sizes[i]-sizes[j]-2)**2)
      tl[i][p] /= norm
      tl[i][p] += 1
  return tl
  
for time in [1000, 5000, 10000, 25000, 50000, 75000, 100000, 150000, 200000, 250000, 300000, 350000, 400000, 450000, 500000]:
  dim, t, patch, alpha = loaddata("mm_alpha.out", time)
  dim, t, patch, bm = loaddata("mm_bm.out", time)

  #print dim, t, patch, alpha, bm

  draw_web(dim, alpha, bm, t)