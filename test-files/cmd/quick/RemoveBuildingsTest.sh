#!/bin/bash

mkdir -p test-output/cmd/quick/

CONFIG="-C Testing.conf"

hoot convert $CONFIG -D convert.ops="hoot::RemoveElementsVisitor" -D remove.elements.visitor.element.criteria="hoot::BuildingCriterion" test-files/conflate/poi-polygon/PoiBuildingA.osm test-output/cmd/quick/RemoveBuildingsTest.osm
# The last few lines of the stats aren't reliable since there are no ways or relations, so only use the first n lines with head
hoot stats $CONFIG --brief test-output/cmd/quick/RemoveBuildingsTest.osm | head -n 21
