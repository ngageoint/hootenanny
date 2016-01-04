#!/bin/bash
set -e

export TA_FILE=cali-test
export TA_IN=test-files/translation_assistant
export TA_OUT=test-output/cmd/translation_assistant_ogr2osm
mkdir -p $TA_OUT

schemas=( "tdsv61" "tdsv40" )
for schema in "${schemas[@]}"
do
    # Debug
    #echo "Out: " $TA_OUT/$TA_FILE-$schema.osm "  In: " $TA_IN/$TA_FILE-$schema.osm
    hoot --ogr2osm $TA_IN/cali-translation-$schema.js $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE.shp
    hoot --is-match --ignore-uuid $TA_OUT/$TA_FILE-$schema.osm $TA_IN/$TA_FILE-$schema.osm
done

#Test Feature Code mapping
hoot --ogr2osm $TA_IN/cali-translation-tdsv61-fcode.js $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE.shp
hoot --is-match --ignore-uuid $TA_OUT/$TA_FILE-tdsv61-fcode.osm $TA_IN/$TA_FILE-tdsv61.osm

