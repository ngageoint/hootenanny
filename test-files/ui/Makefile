
# If the silent flag is passed to make then make hoot quiet too.
ifneq (,$(findstring s,$(MAKEFLAGS)))
  HOOT_OPTS=--warn
  TIME=
  ECHO=true
endif

$(shell $(HOOT_HOME)/scripts/ReplaceEnvironmentVariables.sh DbSettings.inc.in DbSettings.inc)
-include DbSettings.inc

HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
HOOT_OPTS+= -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true
HOOT_OPTS+= -D hootapi.db.writer.email=test@test.com -D hootapi.db.reader.email=test@test.com

test: load-data
	xvfb-run --server-args="-screen 0, 1024x768x24" cucumber

clean:
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -t -A -c "SELECT display_name FROM maps WHERE display_name like '%Cucumber%'" \
	| while read mapname; do \
 	   echo hoot delete-map $(HOOT_OPTS) $(DB_URL)/$$mapname | bash ; \
	done; \
	true
load-data:
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcGisRoads.osm $(DB_URL)/DcGisRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcTigerRoads.osm $(DB_URL)/DcTigerRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesA.osm $(DB_URL)/AllDataTypesACucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesB.osm $(DB_URL)/AllDataTypesBCucumber
