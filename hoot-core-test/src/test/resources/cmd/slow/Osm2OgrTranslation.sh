#!/bin/bash

OUTPUT=test-output/cmd/slow/Osm2OgrTranslation

function getLayerNames() {
    result=`ogrinfo -q $1 2>&1 | sed "s/[0-9]*: //;s/ \(.*\)//g" | sort`
}

# Using this function instead of "ogrinfo -al" gives more consistent output between
# version 1.9 and 1.10 of GDAL. If this is failing it may be due to a formatting
# change in GDAL
function printLayerInfo() {
    path=$1
    getLayerNames $path
    layerNames=$result
    for l in $layerNames
    do
        echo $l
        ogrinfo -q -geom=SUMMARY $path $l | sed "s/  */ /g"
    done
}

rm -rf $OUTPUT
mkdir -p $OUTPUT

echo "#### Shapefile Output ####"
hoot --osm2ogr test-files/io/SampleTranslation.js test-files/io/SampleTranslation.osm $OUTPUT/output.shp
printLayerInfo $OUTPUT/output/

echo "#### FileGDB Output ####"
hoot --osm2ogr test-files/io/SampleTranslation.js test-files/io/SampleTranslation.osm $OUTPUT/output.gdb
printLayerInfo $OUTPUT/output.gdb/

echo "#### Test prepending layer names and lazy layer creation ####"
hoot --osm2ogr -D ogr.writer.pre.layer.name=bar_ test-files/io/SampleTranslation.js test-files/io/SampleTranslation.osm $OUTPUT/options.shp
hoot --osm2ogr -D ogr.writer.pre.layer.name=foo_ -D ogr.writer.create.all.layers=true test-files/io/SampleTranslation.js test-files/io/SampleTranslation.osm $OUTPUT/options.shp
printLayerInfo $OUTPUT/options/

