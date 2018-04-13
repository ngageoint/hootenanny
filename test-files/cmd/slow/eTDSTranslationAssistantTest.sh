#!/bin/bash
set -e

export TA_FILE=cali-test
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/translation_assistant_convert-ogr2osm
mkdir -p $TA_OUT

schemas=( "tdsv61" "tdsv40" )
for schema in "${schemas[@]}"
do
    # Debug
    #echo "Out: " $TA_OUT/$TA_FILE-$schema.osm "  In: " $TA_IN/$TA_FILE-$schema.osm
    hoot convert-ogr2osm --debug $TA_IN/cali-translation-$schema.js $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE.shp

    # Uncomment this line to update the input files when there is a schema change
    #cp $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE-$schema.osm

    hoot map-diff --ignore-uuid $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE-$schema.osm
done

#Test Feature Code mapping
hoot convert-ogr2osm --debug $TA_IN/cali-translation-tdsv61-fcode.js $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE.shp

# Uncomment this line to update the input files when there is a schema change
#cp $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE-tdsv61.osm

hoot map-diff --ignore-uuid $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE-tdsv61.osm


