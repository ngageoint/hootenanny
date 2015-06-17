#!/usr/bin/python

# Usage: CompareShapefiles.py <file1.shp> <file2.shp>
#
# Mattj Mar 14
#

import sys,os,argparse,re
from osgeo import ogr

# Set up Args
parser = argparse.ArgumentParser(description='Compare shapefile 1 to shapefile 2 based on feature UUID. Both files MUST have a UUID or LINK_ID field')
#parser.add_argument('-q','--quiet', help="Don't print warning messages.",action='store_true')
parser.add_argument('shapefile1', help='The first file', action='store')
parser.add_argument('shapefile2', help='The second file', action='store')

args = parser.parse_args()

# Setup File1
file1dataSource = ogr.Open(args.shapefile1,0)
if file1dataSource is None:
    print 'Could not open %s' % (args.shapefile1)
    sys.exit()

file1Layer = file1dataSource.GetLayer(0)
file1LayerDef = file1Layer.GetLayerDefn()
file1NumFields = file1LayerDef.GetFieldCount()

file1FieldList = []
for i in range(file1NumFields):
    file1FieldList.append(file1LayerDef.GetFieldDefn(i).GetName())


# Setup File2
file2dataSource = ogr.Open(args.shapefile2,0)

if file2dataSource is None:
    print 'Could not open %s' % (args.shapefile2)
    sys.exit()

file2Layer = file2dataSource.GetLayer(0)
file2LayerDef = file2Layer.GetLayerDefn()
file2NumFields = file2LayerDef.GetFieldCount()

file2FieldList = []
for i in range(file2NumFields):
    file2FieldList.append(file2LayerDef.GetFieldDefn(i).GetName())

#####
# Start comparing things

# Compare the number of features
if file1Layer.GetFeatureCount() <> file2Layer.GetFeatureCount():
    print 'Different number of features: %d %d' % (file1Layer.GetFeatureCount(), file2Layer.GetFeatureCount())

# Compare the number of fields
if file1NumFields <> file2NumFields:
    print 'Different number of Fields: %d %d' % (file1NumFields, file2NumFields)

# Make sure we have UFI or LINK_ID fields in both files
file1IdField = ''
if 'LINK_ID' in file1FieldList:
    file1IdField = 'LINK_ID'
elif 'UFI' in file1FieldList:
    file1IdField = 'UFI'
elif 'UID' in file1FieldList:
    file1IdField = 'UID'
else:
    print 'No UFI, UID or LINK_ID attribute in %s' % (args.shapefile1)
    sys.exit()


file2IdField = ''
if 'LINK_ID' in file2FieldList:
    file2IdField = 'LINK_ID'
elif 'UFI' in file2FieldList:
    file2IdField = 'UFI'
elif 'UID' in file2FieldList:
    file2IdField = 'UID'
else:
    print 'No UFI,UID or LINK_ID attribute in %s' % (args.shapefile2)
    sys.exit()


# Make sure we have "unique" UUID's
uuidList = []
uuidValue = ''
for feature in file1Layer:
    uuidValue = feature.GetField(file1IdField)
    if uuidValue is None:
        continue
    if uuidValue in uuidList:
        print 'More than one feature with UUID %s in %s' % (uuidValue, args.shapefile1)
        sys.exit()
    else:
        uuidList.append(uuidValue)

uuidList = []
uuidValue = ''
for feature in file2Layer:
    uuidValue = feature.GetField(file2IdField)
    if uuidValue is None:
        continue
    if uuidValue in uuidList:
        print 'More than one feature with UUID %s in %s' % (uuidValue, args.shapefile2)
        sys.exit()
    else:
        uuidList.append(uuidValue)

file1Layer.ResetReading()

# Now start comparing each field in Shapefile1 with the corresponding field in Shapefile2
for feature in file1Layer:
    feature1UFI = feature.GetField(file1IdField)

    if feature1UFI is None:
        print 'Feature with empty %s attribute in %s' % (file1IdField,args.shapefile1)
        continue

    select_str = file2IdField + " = \'" + feature1UFI + "\'"

    file2Layer.SetAttributeFilter(select_str)
    feature2 = file2Layer.GetNextFeature()

    if file2Layer.GetFeatureCount() > 1:
        print 'More than one feature with UUID %s in %s' % (feature1UFI, args.shapefile2)
        sys.exit()

    if not feature2:
        print 'No Feature for UID: %s  in %s' % (feature1UFI, args.shapefile2)
    else:
        for i in file1FieldList: 
            if not i in file2FieldList: # Make sure that the field exists in File2
                continue

            if feature.GetField(i) <> feature2.GetField(i): 
                print 'UUID %s: Different value for %s: %s vs %s' % (feature1UFI, i, feature.GetField(i), feature2.GetField(i))

    file2Layer.SetAttributeFilter('')
    file2Layer.ResetReading()
# End
