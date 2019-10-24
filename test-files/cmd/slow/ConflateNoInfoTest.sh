#!/bin/bash
set -e

export OUTPUT_DIR=$HOOT_HOME/test-output/cmd/slow/ConflateNoInfoTest
export DATA_DIR=$HOOT_HOME/test-files
export INPUT1=$DATA_DIR/NoInfoInput1.osm
export INPUT2=$DATA_DIR/NoInfoInput2.osm
export KNOWN_GOOD_OUTPUT=$DATA_DIR/cmd/slow/ConflateNoInfoTest/output.osm
export TEST_OUTPUT=$OUTPUT_DIR/ConflateNoInfoOutput.osm

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

hoot conflate -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D "map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualWaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallWayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover" -D "small.way.merger.threshold=15" -D "unify.optimizer.time.limit=60" -D "ogr.split.o2s=false" -D "ogr.esri.fcsubtype=true" -D "ogr.thematic.structure=true" -D "duplicate.name.case.sensitive=true" -D "element.cache.size.node=2000000" -D "element.cache.size.relation=200000" -D "element.cache.size.way=200000" -D "conflate.match.highway.classifier=hoot::HighwayRfClassifier" -D "match.creators=hoot::HighwayMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::ScriptMatchCreator,LinearWaterway.js" -D "merger.creators=hoot::HighwayMergerCreator;hoot::BuildingMergerCreator;hoot::ScriptMergerCreator;hoot::ScriptMergerCreator" -D "search.radius.highway=-1" -D "highway.matcher.heading.delta=5.0" -D "highway.matcher.max.angle=60" -D "way.merger.min.split.size=5" -D "waterway.subline.matcher=hoot::MaximalSublineMatcher" -D "waterway.angle.sample.distance=20.0" -D "waterway.matcher.heading.delta=150.0" -D "waterway.auto.calc.search.radius=true" -D "search.radius.waterway=-1" -D "waterway.rubber.sheet.minimum.ties=5" -D "waterway.rubber.sheet.ref=true" $INPUT1 $INPUT2 $TEST_OUTPUT

hoot diff $KNOWN_GOOD_OUTPUT $TEST_OUTPUT && echo "Test Complete"
