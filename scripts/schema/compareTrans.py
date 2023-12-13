#!/usr/bin/python

 #/*
 #* This file is part of Hootenanny.
 #*
 #* Hootenanny is free software: you can redistribute it and/or modify
 #* it under the terms of the GNU General Public License as published by
 #* the Free Software Foundation, either version 3 of the License, or
 #* (at your option) any later version.
 #*
 #* This program is distributed in the hope that it will be useful,
 #* but WITHOUT ANY WARRANTY; without even the implied warranty of
 #* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #* GNU General Public License for more details.
 #*
 #* You should have received a copy of the GNU General Public License
 #* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 #*
 #* --------------------------------------------------------------------
 #*
 #* The following copyright notices are generated automatically. If you
 #* have a new notice to add, please use the format:
 #* " * @copyright Copyright ..."
 #* This will properly maintain the copyright information. Maxar
 #* copyrights will be updated automatically.
 #*
 #* @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 #*/


# compareTrans.py
#
# Compare records based on UUID
#

import sys,argparse,json

def openFile(path, mode):
    if path.endswith(".gz"):
        return gzip.GzipFile(path, mode)
    else:
        return open(path, mode)


# The main loop to process a file
def processFiles(fileList,features):
    for fName in fileList:
        with openFile(fName, 'rb') as f:
            for line in f:
                if line[0] != '"':
                    continue

                data = json.loads('{' + line + '}')
                item = next(iter(data)) # We only have a single object property

                if item not in features:
                    features[item] = {}

                if data[item]['direction'] not in features[item]:
                    features[item][data[item]['direction']] = {}

                if data[item]['layer'] not in features[item][data[item]['direction']]:
                    features[item][data[item]['direction']][data[item]['layer']] = {}

                features[item][data[item]['direction']][data[item]['layer']] = data[item]['tags']

    return features
# End of processFile


def getLayer(data):
    for i in data:
        if i != 'outputOSM' and i != 'inputOSM':
            return i
# end getData


###########
# Main Starts Here
#
parser = argparse.ArgumentParser(description='Compare translation output')
parser.add_argument('datafiles', help='The roughly JSON structured datafiles', action='store',nargs='+')

args = parser.parse_args()

features = {}
features = processFiles(args.datafiles,features)

# Testing
item = next(iter(features)) # We only have a single object property

for item in features:
    print(item)

    if 'toOSM' not in features[item]:
        print('# Missing toOSM')
        continue

    inLayer = getLayer(features[item]['toOSM'])

    if 'toOgr' not in features[item]:
        print("in: " + inLayer + ' out: Missing')
        continue

    outLayer = getLayer(features[item]['toOgr'])

    print("in: " + inLayer + ' out: ' + outLayer)
    if outLayer != inLayer:
        print("# Different layers: %s vs %s" % (inLayer,outLayer))

    # Look at the translated tags
    inSet = set(features[item]['toOSM'][inLayer])
    outSet = set(features[item]['toOgr'][outLayer])

    print("translated:")
    print("  in - out: " + str(inSet - outSet))
    print("  out - in: " + str(outSet - inSet))


    # Look at the translated tags
    inSet = set(features[item]['toOSM']['outputOSM'])
    outSet = set(features[item]['toOgr']['inputOSM'])

    print("OSM:")
    print("  in - out: " + str(inSet - outSet))
    print("  out - in: " + str(outSet - inSet))

    print

    # Debug
    # for i in features[item]:
    #     print(i)
    #     for j in features[item][i]:
    #         print(j)
    #         print(features[item][i][j])

# End

