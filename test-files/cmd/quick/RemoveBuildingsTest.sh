#!/bin/bash

mkdir -p test-output/cmd/quick/
hoot convert -D convert.ops="hoot::RemoveElementsVisitor" -D remove.elements.visitor.element.criterion="hoot::BuildingCriterion" test-files/conflate/PoiBuildingA.osm test-output/cmd/quick/RemoveBuildingsTest.osm
# The last few lines of the stats aren't reliable since there are no ways or relations, so only use the first 7 lines with head
hoot stats --brief test-output/cmd/quick/RemoveBuildingsTest.osm | head -n 7
