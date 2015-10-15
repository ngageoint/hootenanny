#!/usr/bin/python

import codecs
import sys

if (len(sys.argv) != 4):
    print "Crops a given input GeoNames.org file to the given bounds."
    print "Usage:"
    print sys.argv[0] + " <input> <output> <minx,miny,maxx,maxy>"
    sys.exit(-1)

inputFn = sys.argv[1]
outputFn = sys.argv[2]
bounds = [float(x) for x in sys.argv[3].split(',')]

LAT=4;
LON=5;

outFp = codecs.open(outputFn, 'w', 'utf-8')
inFp = codecs.open(inputFn, 'r', 'utf-8')

for line in inFp:
    cols = line.split('\t');
    badFloat = False
    try:
        lat = float(cols[LAT]);
        lon = float(cols[LON]);
    except ValueError:
        badFloat = True

    if (len(cols) != 19):
        print "Expected 19 columns, but got " + cols.length
        print line
    elif badFloat:
        print "Error parsing lat or lon from: "
        print line
    # if this is outside our bounds.
    elif lon < bounds[0] or lon > bounds[2] or lat < bounds[1] or lat > bounds[3]:
        pass
    else:
        outFp.write(line);

