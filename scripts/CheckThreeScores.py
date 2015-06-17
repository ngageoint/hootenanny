#!/usr/bin/python
#################################################################################################
# CheckThreeScores - A utility to check the values of data lines in the format of:
#   <attribute name> tab <value1> tab <value2> tab <value3>
#   e.g. Attribute Score 1\t879\t912\t1020 
# 
# The command line looks like:
#   CheckThreeScores.py file "<attribute>,<1stLow>,<1stHigh>,<2ndLow>,<2ndHigh>,<3rdLow>,<3rdHigh>" ...
#   CheckThreeScores.py input.txt "Node Count,20,22,25,27,43,45" "Vertex Count,1,2,1,2,3,4" ... 

import re
import sys

scoreFn = sys.argv[1]
fp = open(scoreFn)

d = {}

for line in fp:
    if "\t" not in line:
        continue;

    v = re.split("\t", line)
    try:
        if v[1] == '':
            continue;
        if v[2] == '':
            continue;
        if v[3] == '':
            continue;

        n1 = float(v[1].split()[0])
        n2 = float(v[2].split()[0])
        n3 = float(v[3].split()[0])
        d[v[0]] = [n1,n2,n3]
    except ValueError:
        pass

result = 0

# Go through all the score argument and check for a legit range.
for a in sys.argv[2:]:
    v = a.split(",")
    key = v[0]
    for b in range(0,3):
      lower = float(v[b*2+1])
      upper = float(v[b*2+2]) 
      if (lower > upper):
          print "Bad argument (%s); expected lower (%g) to be less than upper (%g)" % (key, lower, upper)
          result = -1
      elif (key not in d):
          result = -1
          print "Could not find key (" + key + ") in score file (" + scoreFn + ")"
      else:
          n = d[key][b]
          if (n > upper or n < lower):
              print "%s is %g, expected a value between %g and %g" % (key, n, lower, upper)
              result = -1

exit(result)
