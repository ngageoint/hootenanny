#!/bin/bash
set -e

AOI="-71.4698,42.4866,-71.4657,42.4902"

HOOT_EMAIL=OsmApiDbHootApiDbConflate@hoottestcpp.org
source conf/database/DatabaseConfig.sh
export OSM_API_DB_URL="osmapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME_OSMAPI"
export OSM_API_DB_AUTH="-h $DB_HOST -p $DB_PORT -U $DB_USER"
export PGPASSWORD=$DB_PASSWORD_OSMAPI
HOOT_DB_URL="hootapidb://$DB_USER:$DB_PASSWORD@$DB_HOST:$DB_PORT/$DB_NAME"
HOOT_OPTS="--info -D uuid.helper.repeatable=true -D writer.include.debug.tags=true"

echo ""
echo "Writing the complete reference dataset to an osm api db (buildings to be replaced)..."
echo ""
scripts/database/CleanAndInitializeOsmApiDb.sh
# using perturbed buildings as the original ref data here, bc they're uglier (we'd want to get rid them)
hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D changeset.user.id=1 -D apidb.bulk.inserter.validate.data=true -D osmapidb.bulk.inserter.reserve.record.ids.before.writing.data=true test-files/BostonSubsetRoadBuilding_FromOsm-perturbed.osm $OSM_API_DB_URL

# Commented out here are settings to tweak cookie cutting behavior.
# -D cookie.cutter.alpha=1000 -D cookie.cutter.alpha.shape.buffer=0.0

# Commented out here are other settings that may be needed to tweak changeset output in situations with production data.
# -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D reader.use.file.status=true -D reader.keep.status.tag=true -D changeset.xml.writer.add.timestamp=false -D changeset.allow.deleting.reference.features=true

# TODO: this won't work correctly until bounds is supported for .osm inputs
#echo ""
#echo "Deriving a changeset that completely replaces buildings in the reference dataset within the specified AOI with those from a secondary dataset (osm xml file secondary source)..."
#echo ""
#hoot changeset-derive -D api.db.email=$HOOT_EMAIL -D changeset.user.id=1 -D convert.ops="hoot::RemoveElementsVisitor;hoot::CookieCutterOp" -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true -D convert.bounding.box.osm.api.database=$AOI $OSM_API_DB_URL test-files/BostonSubsetRoadBuilding_FromOsm.osm ~/hoot/tmp/boston-cookie-cut-out-1.osc

echo ""
echo "Writing the complete secondary dataset to a hoot api db (buildings to replace with)..."
echo ""
# using unperturbed buildings as the secondary replacement buildings here, bc they're prettier (we'd want to keep them)
hoot convert $HOOT_OPTS -D reader.add.source.datetime=false -D reader.preserve.all.tags=true -D api.db.email=$HOOT_EMAIL -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true test-files/BostonSubsetRoadBuilding_FromOsm.osm "$HOOT_DB_URL/BostonSubsetRoadBuilding_FromOsm"

echo ""
echo "Deriving a changeset that completely replaces buildings in the reference dataset within the specified AOI with those from a secondary dataset (hoot api db secondary source)..."
echo ""
hoot changeset-derive -D api.db.email=$HOOT_EMAIL -D changeset.user.id=1 -D convert.ops="hoot::RemoveElementsVisitor;hoot::CookieCutterOp" -D convert.bounding.box=$AOI -D remove.elements.visitor.element.criteria=hoot::BuildingCriterion -D remove.elements.visitor.recursive=true -D element.criterion.negate=true $OSM_API_DB_URL "$HOOT_DB_URL/BostonSubsetRoadBuilding_FromOsm" ~/hoot/tmp/boston-cookie-cut-out-2.osc
