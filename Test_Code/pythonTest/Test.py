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

f_read = open("/local.work/jones/testfile.txt")
f_lines = f_read.readlines()

print f_lines
print resource.getrusage(resource.RUSAGE_SELF).ru_maxrss

print 'done!'