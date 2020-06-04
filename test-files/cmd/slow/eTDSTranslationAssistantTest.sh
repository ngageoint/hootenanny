#!/bin/bash
set -e

export TA_FILE=cali-test
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/slow/translation_assistant_convert-ogr2osm
mkdir -p $TA_OUT

# Minimal output
HOOT_OPT="--debug -C Testing.conf"

# Lots and lots of debug output
#HOOT_OPT="--debug -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

schemas=( "tdsv61" "tdsv40" "mgcp" "tdsv70")

for schema in "${schemas[@]}"
do
    # Debug
    # echo "##### Start ${schema} #####"
    # echo "Out: " $TA_OUT/$TA_FILE-$schema.osm "  In: " $TA_IN/$TA_FILE-$schema.osm

    hoot convert $HOOT_OPT -D duplicate.node.remover.distance.threshold=1.0 -D schema.translation.script=$TA_IN/cali-translation-$schema.js $TA_IN/$TA_FILE.shp $TA_OUT/$TA_FILE-$schema.osm

    # Uncomment this line to update the input files when there is a schema change
    # cp $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE-$schema.osm

    hoot diff --ignore-uuid $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE-$schema.osm

    # Debug
    # echo "##### End ${schema} #####"
done

#Test Feature Code mapping
hoot convert $HOOT_OPT  -D duplicate.node.remover.distance.threshold=1.0 -D schema.translation.script=$TA_IN/cali-translation-tdsv61-fcode.js $TA_IN/$TA_FILE.shp $TA_OUT/$TA_FILE-tdsv61-fcode.osm

# Uncomment this line to update the input files when there is a schema change
#cp $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE-tdsv61.osm

hoot diff $HOOT_OPT --ignore-uuid $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE-tdsv61.osm


