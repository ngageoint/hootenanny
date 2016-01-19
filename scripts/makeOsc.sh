#!/bin/bash

# test for at least 1 arg
if [ $# -lt 1 ];
then
  echo "makeOsc <input_osm_file>"
  echo ""
  exit 1
fi

# vars
BLANKFILE="$HOOT_HOME/tmp/blank.osm"
LOG="$HOOT_HOME/tmp/makeOscLog.txt"
INPUT=`echo $1|sed s/.osm//`

# create log
mkdir -p $HOOT_HOME/tmp
echo "Creating log for makeOsc" >& $LOG

# test if blank.osm file exists
if [ ! -f $BLANKFILE ];
then
  # create a blank osm file 
  echo "File $BLANKFILE does not exist so creating with osmosis."
  echo "Creating blank.osm file" >> $LOG 2>&1
  osmosis -v 9 --read-empty --write-xml file=$BLANKFILE >> $LOG 2>&1
fi

# data needs to be sorted in order to upload properly
echo "Sorting the osm file" >> $LOG 2>&1
osmosis --read-xml file=$INPUT.osm --sort type="TypeThenId" --write-xml file=$INPUT"_sorted.osm"

# create the change file
echo "Creating the change file" >> $LOG 2>&1
osmosis --read-xml file=$INPUT"_sorted.osm" --read-xml file=$BLANKFILE --derive-change --write-xml-change file=$INPUT.osc
