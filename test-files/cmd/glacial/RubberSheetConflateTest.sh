#!/bin/bash
set -e

# See ticket #6208 for an explanation

mkdir -p test-output/cmd/RubberSheetConflateTest

# Broke this into seperate lines to make reading easier.
hoot conflate -C core/RemoveReview2Pre.conf -D uuid.helper.repeatable=true -D convert.ops=hoot::DecomposeBuildingRelationsVisitor \
 -D writer.include.conflate.score.tags=true -D hootapi.db.writer.overwrite.map=true -D hootapi.db.writer.create.user=true -D api.db.email=test@test.com \
 -D "map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualWaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallWayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover;hoot::RubberSheet" \
 -D "duplicate.name.case.sensitive=true" -D "small.way.merger.threshold=15" -D "rubber.sheet.ref=false" -D "rubber.sheet.minimum.ties=10" -D "unify.optimizer.time.limit=30" -D "ogr.split.o2s=false" -D "ogr.esri.fcsubtype=true" \
 -D "ogr.thematic.structure=true" -D "conflate.enable.old.roads=true" -D "conflate.match.highway.classifier=hoot::HighwayExpertClassifier" -D "way.subline.matcher=hoot::MaximalSublineMatcher" \
 -D "match.creators=hoot::BuildingMatchCreator;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js" \
 -D "merger.creators=hoot::BuildingMergerCreator;hoot::BuildingMergerCreator;hoot::ScriptMergerCreator" -D "search.radius.highway=-1" -D "way.matcher.heading.delta=5.0" -D "way.matcher.max.angle=60" \
 -D "way.merger.min.split.size=5" -D "way.angle.sample.distance=10.0" -D "poi.polygon.name.score.threshold=0.8" -D "poi.polygon.review.distance.threshold=0" -D "autocorrect.options=false" \
 test-files/DcGisRoads.osm test-files/DcTigerRoads.osm test-output/cmd/RubberSheetConflateTest/output.osm

hoot map-diff test-output/cmd/RubberSheetConflateTest/output.osm test-files/cmd/glacial/RubberSheetConflateTest/output.osm ||
(echo "You should run: diff test-output/cmd/RubberSheetConflateTest/output.osm test-files/cmd/glacial/RubberSheetConflateTest/output.osm" ;\
echo "If it looks good then copy a new baseline: cp test-output/cmd/RubberSheetConflateTest/output.osm test-files/cmd/glacial/RubberSheetConflateTest/output.osm"; exit -1)

