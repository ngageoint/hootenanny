#!/bin/bash
set -e

source conf/database/DatabaseConfig.sh
export HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"

OUTPUT_DIR=test-output/cmd/slow/ServiceHootApiDbReaderTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

# initial data load
hoot convert --error -D hootapi.db.writer.create.user=true -D api.db.email=ServiceHootApiDbReaderTest@hoottestcpp.org -D hootapi.db.writer.overwrite.map=true test-files/DcGisRoads.osm "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderTest"

# unbounded query
hoot convert --error -D api.db.email=ServiceHootApiDbReaderTest@hoottestcpp.org -D convert.ops="hoot::RemoveTagVisitor;hoot::RemoveAttributeVisitor" -D remove.tag.visitor.keys="source:datetime" -D remove.attribute.visitor.types="changeset;timestamp" "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderTest" $OUTPUT_DIR/output1.osm
hoot is-match --error test-files/cmd/slow/ServiceHootApiDbReaderTest/output1.osm $OUTPUT_DIR/output1.osm

# bounded query
hoot convert --error -D api.db.email=ServiceHootApiDbReaderTest@hoottestcpp.org -D convert.ops="hoot::RemoveTagVisitor;hoot::RemoveAttributeVisitor" -D remove.tag.visitor.keys="source:datetime" -D remove.attribute.visitor.types="changeset;timestamp" -D convert.bounding.box=-77.04,38.8916,-77.03324,38.8958 "$HOOT_DB_URL/DcGisRoads-ServiceHootApiDbReaderTest" $OUTPUT_DIR/output2.osm
hoot is-match --error test-files/cmd/slow/ServiceHootApiDbReaderTest/output1.osm $OUTPUT_DIR/output1.osm
