#!/usr/bin/python
# -*- coding: utf-8 -*-

import array
import numpy as np
import matplotlib.pyplot as plt
import datetime


print "Programm started"

x = [1, 2, 3, 4, 5, 6, 7, 6, 7, 8, 1, 3, 3, 5, 10, 15]

plt.hist(x, bins=np.arange(max(x)+2), align='left')
plt.savefig('pic.pdf')
plt.show()


print "Programm ended"



