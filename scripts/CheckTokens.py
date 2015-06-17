#!/usr/bin/python
#################################################################################################
# CheckTokens - A utility to check the tokens in the format of:
#   <attribute name1>,<attribute name2>,...,<attribute name_n>
#   e.g. Attribute Score, Total Features, Number Conflated
# 
# The command line looks like:
#   CheckTokens.py file "<attribute>" "<attribute2>" ..."<attribute_n>"
#   CheckTokens.py input.txt "Node Count" "Vertex Count" "Total Features" 

import re
import sys

scoreFn = sys.argv[1]
fp = open(scoreFn)

d = []

for line in fp:
    # make sure its a line with a token and values after
    if "\t" not in line:
        continue;

    # split the line to get first entry (the token)
    v = re.split("\t", line)
    try:
        if v[0] == '':
            continue;
        
        d.append(v[0])

    except ValueError:
        pass

result = 0

#print "list of tokens is %s" % d

# Go through all the tokens provided and check for all 
for a in sys.argv[2:]:
    if (a not in d):
        continue; 

    else:
        result = result + 1

print "number of tokens = %g" % result

exit(result)
