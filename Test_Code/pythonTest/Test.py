#!/usr/bin/python
# -*- coding: utf-8 -*-

#import matplotlib
#matplotlib.use('pdf')
import matplotlib.pyplot as plt
import time
import math
import os
import sys
import ModulesAreaLaws
import resource

print 'program started...'

f_read = open("testfile.txt")
f_lines = []
time_stamp = 20

ModulesAreaLaws.filter_list_for_timestamp(time_stamp, f_read, f_lines)

print f_lines
print resource.getrusage(resource.RUSAGE_SELF).ru_maxrss

print 'done!'