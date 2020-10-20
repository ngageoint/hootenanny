#!/bin/bash

CONFIG="-C Testing.conf"

# This test is sensitive to schema changes and that is ok.

# exact match
hoot type-similarity $CONFIG amenity=school amenity=school

# no similarity
hoot type-similarity $CONFIG amenity=school landuse=residential

# partial match
hoot type-similarity $CONFIG highway=footway highway=service
