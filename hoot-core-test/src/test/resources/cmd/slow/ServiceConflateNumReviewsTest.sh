#!/bin/bash
set -e

mkdir -p test-output/cmd/ConflateNumReviewsTest
FILE_OUT=test-output/cmd/ConflateNumReviewsTest/dc-roads-conflated-from-file-input
CONFLATE_DB_OUT=test-output/cmd/ConflateNumReviewsTest/dc-roads-conflated-from-db-input

#conflat two dc osm files
hoot conflate -C RemoveReview2Pre.conf -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor -D conflate.add.score.tags=yes test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $FILE_OUT.osm --stats > $FILE_OUT

#convert dc gis data to the database
hoot convert -D services.db.writer.overwrite.map=true -D services.db.writer.create.user=true -D services.db.writer.email=test@test.com -D services.db.reader.email=test@test.com test-files/DcGisRoads.osm postgresql://hoot:hoottest@localhost:5432/hoot/dcgisroads_fornumreviewstest

#convert dc tiger data to the database
hoot convert -D services.db.writer.overwrite.map=true -D services.db.writer.create.user=true -D services.db.writer.email=test@test.com -D services.db.reader.email=test@test.com test-files/DcTigerRoads.osm postgresql://hoot:hoottest@localhost:5432/hoot/dctigerroads_fornumreviewstest

#conflate two set of data from the database
hoot conflate -C RemoveReview2Pre.conf -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor -D conflate.add.score.tags=yes -D services.db.writer.overwrite.map=true -D services.db.writer.create.user=true -D services.db.writer.email=test@test.com -D services.db.reader.email=test@test.com postgresql://hoot:hoottest@localhost:5432/hoot/dcgisroads_fornumreviewstest postgresql://hoot:hoottest@localhost:5432/hoot/dctigerroads_fornumreviewstest postgresql://hoot:hoottest@localhost:5432/hoot/dcroadsconflated_fornumreviewstest --stats > $CONFLATE_DB_OUT

#convert conflated data from the database to osm
hoot convert -D services.db.writer.overwrite.map=true -D services.db.writer.create.user=true -D services.db.writer.email=test@test.com -D services.db.reader.email=test@test.com postgresql://hoot:hoottest@localhost:5432/hoot/dcroadsconflated_fornumreviewstest $CONFLATE_DB_OUT.osm

#grep number of reviews from two output osm files
file_reviews_count=`cat $FILE_OUT.osm | grep "hoot:review:needs" | wc -l`
db_reviews_count=`cat $CONFLATE_DB_OUT.osm | grep "hoot:review:needs" | wc -l`

#print out the counts
echo $file_reviews_count
echo $db_reviews_count
