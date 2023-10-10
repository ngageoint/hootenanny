#!/bin/bash
# This file is part of Hootenanny
# This file is responsible for executing hoot commands across containers

# Commands can be added here
# TODO: create functions for different commands when they are added and add flag to specify which command to run

# Match conflate different inputs
run-conflate()
{
    hoot conflate -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D conflate.match.only=true -D writer.include.conflate.score.tags=true -D match.creators="BuildingMatchCreator;HighwayMatchCreator" -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D conflate.post.ops-="WayJoinerOp" -D conflate.tag.disable.value.truncation=true -D building.match.threshold=$HOOT_MATCH_THRESH -D building.review.threshold=$HOOT_REVIEW_THRESH $HOOT_REF_FILE_NAME $HOOT_TARGET_FILE_NAME $HOOT_MATCH_FILE_NAME
}

# Generate a task grid
run-task-grid()
{
    hoot task-grid $FILE_NAME $OUTPUT_GEOJSON --maxNodesPerCell 800000 --node-density
}

# Split a file
run-split()
{
    hoot split $OUTPUT_GEOJSON $FILE_NAME $OUTPUT_SPLIT_OSM
}

# Translate a directory
run-translate-directory()
{
    if [ $AFTER_SPLIT == 0 ] 
    then
        hoot convert -D schema.translation.script=$TRANSLATION_SCRIPT ${DIR_PATH} translated.osm --recursive "*.shp" --separate-output
    else  
        hoot convert -D schema.translation.script=$TRANSLATION_SCRIPT ${DIR_PATH} translated.osm --recursive "*.osm" --separate-output
    fi
}

# Translate a file
run-translate()
{
    hoot convert -D schema.translation.script=$TRANSLATION_SCRIPT $FILE_NAME $OUTPUT_FILE
}

# Import data from overpass instance using specific bounding box coordinates
run-overpass()
{
    hoot convert -D bounds=$MIN_LON,$MIN_LAT,$MAX_LON,$MAX_LAT -D overpass.api.query.path=$OQF -D schema.translation.script=$TRANSLATE_SCRIPT \
        -D hoot.pkcs12.key.path=$CERT_PATH -D hoot.pkcs12.key.phrase=$CERT_PW -D overpass.api.host=$OVERPASS_API_HOST \
        "${OP_URL}" ${OUTPUT_GPKG} 

    hoot convert -D bounds=$MIN_LON,$MIN_LAT,$MAX_LON,$MAX_LAT -D overpass.api.query.path=$OQF \
        -D hoot.pkcs12.key.path=$CERT_PATH -D hoot.pkcs12.key.phrase=$CERT_PW -D overpass.api.host=$OVERPASS_API_HOST \
        "${OP_URL}" $OUTPUT_OSM
}

# Import data from overpass instance using a bounding box file
run-overpass-bounds()
{
    hoot convert -D bounds.input.file=$BOUNDING_BOX -D overpass.api.query.path=$OQF \
        -D hoot.pkcs12.key.path=$CERT_PATH -D hoot.pkcs12.key.phrase=$CERT_PW -D overpass.api.host=$OVERPASS_API_HOST \
        "${OP_URL}" $OUTPUT_OSM
}

# Export data from the database as an OSM file
run-export-database-osm()
{
    hoot convert -D schema.translation.script=$TRANSLATE_SCRIPT PG:"dbname=$PGDATABASE host=$PGHOST port=5432 user=$PGUSER schemas=$SCHEMAS" $OUTPUT_OSM
}

case "$1" in
    -c)
        HOOT_REF_FILE_NAME=$2
        HOOT_TARGET_FILE_NAME=$3
        HOOT_MATCH_FILE_NAME=$4
        HOOT_MATCH_THRESH=$5
        HOOT_REVIEW_THRESH=$6
        run-conflate ;;
    -t)
        FILE_NAME=$2
        OUTPUT_GEOJSON=$3
        run-task-grid ;;
    -s)
        FILE_NAME=$2
        OUTPUT_GEOJSON=$3
        OUTPUT_SPLIT_OSM=$4
        run-split ;;
    -d)
        AFTER_SPLIT=$2
        TRANSLATION_SCRIPT=$3
        DIR_PATH=$4
        run-translate-directory ;;
    -r)
        TRANSLATION_SCRIPT=$2
        FILE_NAME=$3
        OUTPUT_FILE=$4
        run-translate ;;
    -o)
        MIN_LON=$2
        MIN_LAT=$3
        MAX_LON=$4
        MAX_LAT=$5
        OQF=$6
        TRANSLATE_SCRIPT=$7
        CERT_PATH=$8
        CERT_PW=$9
        OP_URL=${10}
        OUTPUT_GPKG=${11}
        OUTPUT_OSM=${12}
        OVERPASS_API_HOST=${13}
        run-overpass ;;
    -b)
        BOUNDING_BOX=$2
        OQF=$3
        TRANSLATE_SCRIPT=$4
        CERT_PATH=$5
        CERT_PW=$6
        OP_URL=$7
        OUTPUT_GPKG=$8
        OUTPUT_OSM=$9
        OVERPASS_API_HOST=${10}
        run-overpass-bounds ;;
    -e)
        TRANSLATE_SCRIPT=$2
        PGDATABASE=$3
        PGHOST=$4
        PGUSER=$5
        SCHEMAS=$6
        OUTPUT_OSM=$7
        run-export-database-osm ;;
esac
