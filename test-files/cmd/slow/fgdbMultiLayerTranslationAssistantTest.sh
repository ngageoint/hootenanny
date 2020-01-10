#!/bin/bash
set -e

export TA_FILE=caligdb
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/slow/translation_assistant_ogr2osm
mkdir -p $TA_OUT

rm -rf $TA_OUT/caligdb.gdb
ogr2ogr -f FileGDB -mapFieldType Integer64=Integer $TA_OUT/caligdb.gdb $TA_IN/calizip/cali-test.shp
ogr2ogr -f FileGDB -mapFieldType Integer64=Integer -append $TA_OUT/caligdb.gdb $TA_IN/calizip/cali-fake-points.shp

hoot convert --warn -D duplicate.node.remover.distance.threshold=1.0 -D schema.translation.script=$TA_IN/$TA_FILE-translation.js $TA_OUT/$TA_FILE.gdb $TA_OUT/$TA_FILE.osm
hoot diff --ignore-uuid $TA_OUT/$TA_FILE.osm $TA_IN/$TA_FILE.osm

