#!/bin/bash


make -f $HOOT_HOME/scripts/services/submakebasemaprastertotiles INPUT="$1" INPUT_NAME="$2" RASTER_OUTPUT_DIR="$3" PROJECTION="$4" JOB_PROCESSOR_DIR="$5" jobid="$6"
if [ $? -eq 0 ]; then
  mv -f "$5/$2.processing" "$5/$2.disabled"
  exit 0
else
  mv -f "$5/$2.processing" "$5/$2.failed"
  exit 10
fi
