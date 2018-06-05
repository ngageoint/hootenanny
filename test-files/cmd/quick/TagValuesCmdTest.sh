#!/bin/bash
set -e

inputfile=$HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot tag-values --warn $inputfile
