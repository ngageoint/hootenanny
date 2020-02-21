#!/bin/bash
set -e

source conf/database/DatabaseConfig.sh
export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
export EMAIL=ServiceHootApiDbReaderCmdTest.sh@hoottestcpp.org

OUTPUT_DIR=test-output/cmd/slow/ServiceHootApiDbReaderCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf"

echo "initial data load"
hoot convert --warn $CONFIG -D hootapi.db.writer.create.user=true -D api.db.email=$EMAIL -D hootapi.db.writer.overwrite.map=true test-files/DcGisRoads.osm "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderCmdTest"

echo "unbounded query"
hoot convert --warn $CONFIG -D api.db.email=$EMAIL -D convert.ops="hoot::RemoveTagsVisitor;hoot::RemoveAttributesVisitor" -D tag.filter.keys="source:datetime" -D remove.attributes.visitor.types="changeset;timestamp" "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderCmdTest" $OUTPUT_DIR/output1.osm
hoot diff --warn $CONFIG test-files/cmd/slow/ServiceHootApiDbReaderTest/output1.osm $OUTPUT_DIR/output1.osm

echo "bounded query"
hoot convert --warn $CONFIG -D api.db.email=$EMAIL -D convert.ops="hoot::RemoveTagsVisitor;hoot::RemoveAttributesVisitor" -D tag.filter.keys="source:datetime" -D remove.attributes.visitor.types="changeset;timestamp" -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderCmdTest" $OUTPUT_DIR/output2.osm
hoot diff $CONFIG test-files/cmd/slow/ServiceHootApiDbReaderTest/output2.osm $OUTPUT_DIR/output2.osm

echo "clean up database"
hoot db-delete-map --warn $CONFIG -D api.db.email=$EMAIL "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderCmdTest"
