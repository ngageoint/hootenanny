#!/usr/bin/python

import re
import sys

errorOnHigh = False
argi = 1

# If the user specifies --error-on-high then a high value will cause an error
# rather than a warning.
if sys.argv[argi] == "--error-on-high":
    errorOnHigh = True
    argi = argi + 1

scoreFn = sys.argv[argi]
argi = argi + 1
fp = open(scoreFn)

d = {}

# Parse the values in the score file. We're expecting a format similar to:
#
# Attribute Score 1: 879 +/-15
# Attribute Score 2: 928 +/-17
# Attribute Score: 903 +/-16 (887 to 919)
# Raster Score 1: 967
# Raster Score 2: 931
# Raster Score: 949
# Graph Score 1: 894 +/-23 (871 to 917)
# Graph Score 2: 803 +/-41 (762 to 844)
# Graph Score: 848 +/-32 (816 to 880)
# Overall: 900 +/-16 (884 to 916)
# 
for line in fp:
    v = re.split("\t|: ", line)
    try:
        if (len(v) >= 2):
            n = float(v[1].split()[0])
            d[v[0]] = n
    except ValueError:
        pass

result = 0



# Go through all the score argument and check for a legit range.
for a in sys.argv[argi:]:
    v = a.split(",")
    key = v[0]
    lower = float(v[1])
    upper = sys.float_info.max
    if (len(v) > 2):
        upper = float(v[2])
    if (lower > upper):
        print "Bad argument (%s); expected lower (%g) to be less than upper (%g)" % (key, lower, upper)
        result = -1
    elif (key not in d):
        result = -1
        print "Could not find key (" + key + ") in score file (" + scoreFn + ")"
    else:
        n = d[key]
        if (n > upper):
            if (errorOnHigh):
                print "%s is %g, expected a value between %g and %g" % (key, n, lower, upper)
                result = -1
            else:
                print "%s is %g, This is better than expected. Expected a value between %g and %g" % (key, n, lower, upper)
        elif (n < lower):
            print "%s is %g, expected a value between %g and %g" % (key, n, lower, upper)
            result = -1

exit(result)
