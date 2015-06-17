#!/usr/bin/python

import matplotlib as mpl
# don't use x server
mpl.use('Agg')
import matplotlib.pyplot as plt
import numpy
import os
import sys

times = {}
fontSize = 10

def loadFiles(dir, files):
    errorCount = 0
    times = {}
    errors = {}
    for file in files:
        fp = open(dir + '/' + file, 'r')
        if (file.endswith('.log') == False):
            continue

        for line in fp:
            cols = line.strip().split('\t')
            try:
                if (line.strip() != '' and cols[3] not in times):
                    times[cols[3]] = []
                if (line.strip() != '' and cols[3] not in errors):
                    errors[cols[3]] = []
                if (cols[4] == 'success'):
                    times[cols[3]].append(int(cols[2]))
                elif (cols[4] == 'error'):
                    errors[cols[3]].append(int(cols[2]))
            except:
                errorCount = errorCount + 1
                if (errorCount == 10):
                    print "More than 10 errors. Silencing."
                elif (errorCount > 10):
                    pass
                else:
                    print "Error parsing line in: " + dir + '/' + file
                    print 'line: ' + line

    return (times, errors)

#fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(6,6))

def genBoxPlot(plot, stats, op, lim = False):
    keys = sorted(stats.keys())
    print keys
    values = []
    maxy = 0
    for k in keys:
        if op in stats[k]:
            values.append(stats[k][op])
            if (len(stats[k][op]) > 0):
                maxy = max(maxy, max(stats[k][op]))
        else:
            values.append([])
    if (len(values) == 0):
        values = [0]
    print len(values)
    plot.set_title(op, fontsize=fontSize)
    plot.set_xticklabels(keys, rotation=90)
    plot.set_ylabel('milliseconds', fontsize=fontSize - 1)
    plot.set_xlabel('Simulated Users', fontsize=fontSize - 1)
    if lim:
        if lim >= 1000:
            plot.set_ylim([lim[0] / 1000, lim[1] / 1000])
            for v in values:
                for i in range(len(v)):
                    v[i] = v[i] / 1000.0
            plot.set_ylabel('seconds')
        else:
            plot.set_ylim(lim)
    elif maxy >= 1000:
        for v in values:
            for i in range(len(v)):
                v[i] = v[i] / 1000.0
        plot.set_ylabel('seconds')
    plot.boxplot(values, sym='')
    #plt.show()

offset = 0
legend = []
legendRects = []
def genCountBar(plot, stats, op, log):
    global offset
    global legend
    colors = ['r', 'g', 'b', 'y', 'purple', 'orange', 'gray']
    keys = sorted(stats.keys())
    print keys
    values = []
    maxy = 0
    for k in keys:
        v = 0
        if op in stats[k]:
            v = len(stats[k][op])
        if (log and v == 0):
            v = 1e-1
        values.append(v)

    plot.set_ylabel('count', fontsize=fontSize - 1)
    plot.set_xlabel('Simulated Users', fontsize=fontSize - 1)
    print op
    print values
    rects = plot.bar(numpy.arange(len(keys)) - .4 + offset / 8.0, 
        values, 1.0 / 8.0, color=colors[offset], log=log)
    print plot.xaxis.get_ticklocs()
    plot.set_xticklabels(keys, rotation=90)
    plot.set_xticks(numpy.arange(len(keys)))
    plot.set_xlim([-1, len(keys)])
    legendRects.append(rects)
    legend.append(op)
    print offset
    offset = offset + 1
    #plt.show()

stats = {}
errors = {}

for dir in os.listdir(sys.argv[1]):
    print dir
    (times, errs) = loadFiles(sys.argv[1] + '/' + dir, os.listdir(sys.argv[1] + '/' + dir))
    stats[int(dir)] = times
    errors[int(dir)] = errs
    print 'dir: ' + dir
    for k, v in stats[int(dir)].iteritems():
        print k
        print len(v)

fig, axes = plt.subplots(nrows=3, ncols=3, figsize=(12,12))
#genBoxPlot(stats, 'getBounds', [0, 200])
genBoxPlot(axes[0, 0], stats, 'getTile', [0, 1000])
genBoxPlot(axes[0, 1], stats, 'getMaps', [0, 1000])
genBoxPlot(axes[0, 2], stats, 'getVersion', [0, 1000])
genBoxPlot(axes[2, 1], stats, 'exportMap')
genBoxPlot(axes[2, 2], stats, 'translateElement', [0, 1000])
genBoxPlot(axes[1, 0], stats, 'ingestZip')
genBoxPlot(axes[1, 1], stats, 'conflate')

genCountBar(axes[1, 2], stats, 'conflate', False)
genCountBar(axes[1, 2], stats, 'ingestZip', False)
genCountBar(axes[1, 2], stats, 'exportMap', False)
axes[1, 2].legend(legendRects, legend, fontsize=fontSize, framealpha=0.5)
axes[1, 2].set_title('Operation Counts', fontsize=fontSize)

legendRects = []
legend = []
offset = 0
# add error/success rate
axes[2, 0].set_yscale('log')
genCountBar(axes[2, 0], errors, 'conflate', True)
genCountBar(axes[2, 0], errors, 'ingestZip', True)
genCountBar(axes[2, 0], errors, 'getTile', True)
genCountBar(axes[2, 0], errors, 'getMaps', True)
genCountBar(axes[2, 0], errors, 'getVersion', True)
genCountBar(axes[2, 0], errors, 'exportMap', True)
genCountBar(axes[2, 0], errors, 'translateElement', True)
axes[2, 0].legend(legendRects, legend, fontsize=fontSize, loc='upper left', framealpha=0.5)
axes[2, 0].set_title('Error Counts', fontsize=fontSize)

fig.tight_layout()
plt.savefig(sys.argv[2], dpi=92)
#plt.show()
