#!/bin/bash

IN_DIR=test-files/cmd/quick/RemoveBuildingsTest
OUT_DIR=test-output/cmd/quick/RemoveBuildingsTest
mkdir -p OUT_DIR

CONFIG="-C Testing.conf"
LOG_LEVEL=--warn

hoot convert $LOG_LEVEL $CONFIG -D convert.ops="hoot::RemoveElementsVisitor" -D remove.elements.visitor.element.criteria="hoot::BuildingCriterion" test-files/conflate/poi-polygon/PoiBuildingA.osm $OUT_DIR/output.osm
hoot diff $LOG_LEVEL $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm
