#!/bin/bash
set -e

# Since the formats will changing from time to time, just going to grep out some that are unlikely to change any time soon.

hoot formats | grep "hootapidb://"
hoot formats --input | grep ".osm.bz2"
hoot formats --output | grep ".shp"
