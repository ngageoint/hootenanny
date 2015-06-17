#!/usr/bin/python

import os

for i in range(12001, 12133, 2):
    cmd = "wget ftp://ftp2.census.gov/geo/tiger/TIGER2010/ROADS/tl_2010_%d_roads.zip" % i
    os.system(cmd)
