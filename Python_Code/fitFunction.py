import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

name = "SAL_Het1_Run4_58533500"

arr = []
string = " "
switch = 0
fread = open('C:\Users\David\Documents\gitFolder\Python_Code\SAL_Het1_Run4_58522500.txt', 'r')
for c in fread.readline():
    try:
        char = int(c)
        switch = 1
        string = string + c
    except:
        if switch == 1:
            arr.append(int(string))
            string = " "
            switch = 0
        else:
            pass
count = 0
xdata = []
ydata= []
for val in arr:
    if count == 0:
        xdata.append(np.log10(val))
        count = 1
    else:
        ydata.append(np.log10(val))
        count = 0        

def function(x, a, b):
    return a * x + b

xFitData = xdata
yFitData = ydata

popt, pcov = curve_fit(function, xFitData, yFitData)
y = []
for x in xFitData:
    y.append(function(x, popt[0], popt[1]))

plotData, = plt.plot(xdata, ydata, 'bo')
plotFit, = plt.plot(xFitData, y, 'r', label = 'fit function: f(x) = %s * x + %s' %(popt[0], popt[1]))
plt.legend(handles = [plotFit], loc = 'bottom', fontsize = 'small')
plt.savefig(name + '_fitPlot.png')

fwrite = open(name + "_fit_params.txt", 'a')
fwrite.write("fit function: f(x) = %s * x + %s\n" %(popt[0], popt[1]))
fwrite.write("kovarianzmatrix:\n %s" %pcov)
fwrite.close()
