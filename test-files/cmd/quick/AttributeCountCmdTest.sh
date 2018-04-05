#!/bin/bash
set -e

inputfile=$HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot attribute-count --warn $inputfile
