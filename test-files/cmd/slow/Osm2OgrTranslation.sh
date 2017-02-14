#!/bin/bash

OUTPUT=test-output/cmd/slow/Osm2OgrTranslation

function getLayerNames() {
    # For LC_ALL explanation see:
    # https://github.com/ngageoint/hootenanny-rpms/issues/41
    result=`ogrinfo -q $1 2>&1 | sed "s/[0-9]*: //;s/ \(.*\)//g" | LC_ALL=C sort`
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
        ogrinfo -q -geom=SUMMARY $path $l | sed "s/  */ /g" \
            | sed "s/DBF_DATE_LAST_UPDATE=.*/DBF_DATE_LAST_UPDATE/"
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

echo "#### Test to make sure all layers are read from a data source. ####"
rm -rf test-output/cmd/slow/delaware-tds test-output/cmd/slow/delaware-tds.shp
mkdir -p test-output/cmd/slow
export SHAPE_ENCODING=UTF-8
hoot osm2ogr test-files/io/O2sTranslation.js test-files/cmd/slow/delaware.shp test-output/cmd/slow/delaware-tds.shp
mv test-output/cmd/slow/delaware-tds test-output/cmd/slow/delaware-tds.shp
hoot stats --brief test-output/cmd/slow/delaware-tds.shp

