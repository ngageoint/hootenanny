
SHELL=/bin/bash

# If the silent flag is passed to make then make hoot quiet too.
ifneq (,$(findstring s,$(MAKEFLAGS)))
  HOOT_OPTS=--warn
  TIME=
  ECHO=true
endif

# Check if we are using a different Tomcat6 port
ifeq ($(TOMCAT_PORT),)
  TOMCAT_PORT=8080
endif

CWD=$(shell pwd)

$(shell $(HOOT_HOME)/scripts/ReplaceEnvironmentVariables.sh DbSettings.inc.in DbSettings.inc)
-include DbSettings.inc

HOOT_OPTS+= -D osm2ogr.ops=hoot::DecomposeBuildingRelationsVisitor
HOOT_OPTS+= -D hootapi.db.writer.create.user=true -D hootapi.db.writer.overwrite.map=true
HOOT_OPTS+= -D hootapi.db.writer.email=test@test.com -D hootapi.db.reader.email=test@test.com

test: load-data
ifneq (,$(findstring s,$(MAKEFLAGS)))
	$(HOOT_HOME)/scripts/VerifyTomcatStarted.sh $(TOMCAT_PORT) --quiet 2> >( $(FILTER) >&2 )
else
	$(HOOT_HOME)/scripts/VerifyTomcatStarted.sh $(TOMCAT_PORT)
endif
	echo Running user interface tests...
ifneq (,$(findstring s,$(MAKEFLAGS)))
	TOMCAT_PORT=$(TOMCAT_PORT) xvfb-run --server-args="-screen 0, 1024x768x24" cucumber --format progress --strict
else
	TOMCAT_PORT=$(TOMCAT_PORT) xvfb-run --server-args="-screen 0, 1024x768x24" cucumber --format pretty --strict
endif
	echo ""

	#Remove test datasets from db
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -t -A -c "SELECT display_name FROM maps WHERE display_name like '%Cucumber%'" \
	| while read mapname; do \
 	   echo hoot delete-map $(HOOT_OPTS) $(DB_URL)/$$mapname | bash ; \
	done; \
	true

clean:
	#Remove test datasets from db
	PGPASSWORD=$(PGPASSWORD) psql $(AUTH) -t -A -c "SELECT display_name FROM maps WHERE display_name like '%Cucumber%'" \
	| while read mapname; do \
 	   echo hoot delete-map $(HOOT_OPTS) $(DB_URL)/$$mapname | bash ; \
	done; \
	true

coverage:
	echo Running user interface coverage report...
ifneq (,$(findstring s,$(MAKEFLAGS)))
	$(HOOT_HOME)/scripts/CoverHootUI.sh &> coverage.log
else
	$(HOOT_HOME)/scripts/CoverHootUI.sh
endif
	echo ""

clean-coverage:
	rm -f coverage.log
	rm -f $(HOOT_HOME)/hoot-ui/test/istanbul_index.html
	rm -rf $(HOOT_HOME)/hoot-ui/istanbul
	rm -rf $(HOOT_HOME)/hoot-ui/mocha-coverage
	rm -rf $(HOOT_HOME)/hoot-ui/cucumber-coverage
	rm -rf $(HOOT_HOME)/hoot-ui/combined-coverage
load-data:
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcGisRoads.osm $(DB_URL)/DcGisRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/DcTigerRoads.osm $(DB_URL)/DcTigerRoadsCucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesA.osm $(DB_URL)/AllDataTypesACucumber
	hoot convert $(HOOT_OPTS) $(HOOT_HOME)/test-files/conflate/unified/AllDataTypesB.osm $(DB_URL)/AllDataTypesBCucumber


