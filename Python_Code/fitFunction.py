import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

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
print xdata
print ydata

def function(x, a, b):
    return a * x + b

xFitData = xdata[0:8]
yFitData = ydata[0:8]

popt, pcov = curve_fit(function, xFitData, yFitData)
y = []
for x in xFitData:
    y.append(function(x, popt[0], popt[1]))

plt.plot(xdata[0:8], ydata[0:8], 'bo')
plt.plot(xFitData, y, 'r')
plt.show()

print xdata[1:8]
print popt[0], popt[1]
print pcov