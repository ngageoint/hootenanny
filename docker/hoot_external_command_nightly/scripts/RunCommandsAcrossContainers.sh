#!/bin/bash
# This file is part of Hootenanny
# This file is responsible for executing hoot commands across containers

# Commands can be added here
# TODO: create functions for different commands when they are added and add flag to specify which command to run


run-conflate()
{
    hoot conflate -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D conflate.match.only=true -D writer.include.conflate.score.tags=true -D match.creators="BuildingMatchCreator;HighwayMatchCreator" -D conflate.use.data.source.ids.2=true -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D conflate.post.ops-="WayJoinerOp" -D building.match.threshold=$HOOT_MATCH_THRESH -D building.review.threshold=$HOOT_REVIEW_THRESH $HOOT_REF_FILE_NAME $HOOT_TARGET_FILE_NAME $HOOT_MATCH_FILE_NAME
}

run-task-grid()
{
    hoot task-grid $FILE_NAME $OUTPUT_GEOJSON --maxNodesPerCell 800000 --node-density
}

run-split()
{
    hoot split $OUTPUT_GEOJSON $FILE_NAME $OUTPUT_SPLIT_OSM
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
esac

