
####
# This makefile is here to give a consistent interface to all the projects.
# In other words, I don't like remembering cryptic maven commands. -JRS
####

SHELL=/bin/bash

# If the silent flag is passed to make then make Maven quiet too.
ifneq (,$(findstring s,$(MAKEFLAGS)))
    MVN_QUIET="-q"
    SCRIPT_QUIET=> /dev/null 2>&1
endif

default: build

src/main/resources/db/db.properties: src/main/resources/db/db.properties.in ../conf/database/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh src/main/resources/db/db.properties.in src/main/resources/db/db.properties

src/main/resources/conf/hoot-services.conf: src/main/resources/conf/hoot-services.conf.in ../conf/database/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh src/main/resources/conf/hoot-services.conf.in src/main/resources/conf/hoot-services.conf

src/main/webapp/META-INF/context.xml: src/main/webapp/META-INF/context.xml.in
	../scripts/ReplaceEnvironmentVariables.sh src/main/webapp/META-INF/context.xml.in src/main/webapp/META-INF/context.xml

build-info:
	../scripts/GenerateBuildInfoFiles.sh

config: src/main/resources/db/db.properties src/main/resources/conf/hoot-services.conf src/main/webapp/META-INF/context.xml

run-liquibase:
	rm -f ./src/main/resources/db/liquibase.properties
	../scripts/ReplaceEnvironmentVariables.sh ../scripts/database/blank_osmapidb.sql.in ../scripts/database/blank_osmapidb.sql
	../scripts/ReplaceEnvironmentVariables.sh ./src/main/resources/db/liquibase.properties.in ./src/main/resources/db/liquibase.properties
	mvn $(MVN_QUIET) liquibase:update

#add -X param temporarily to mvn-build command to debug mvn execution
build: build-info config run-liquibase
	# If there isn't a local copy of the properties, copy the global version.
	# [ -f src/main/resources/conf/hoot-services.conf ] || cp src/main/resources/conf/hoot-services.conf.orig src/main/resources/conf/hoot-services.conf
	#../scripts/maven/mvn-build $(MVN_QUIET) build
	#cp target/hoot-services-*.war ../bin

docs: config
	../scripts/maven/mvn-build $(MVN_QUIET) docs

clean: config clean-coverage
	../scripts/maven/mvn-build $(MVN_QUIET) clean
	rm -rf ./db.properties
	rm -rf ./src/main/java/hoot/services/db2
	rm -rf ./src/main/resources/db/liquibase.properties
	rm -rf ./src/main/resources/conf/hoot-services.conf
	rm -rf ./src/main/resources/db/db.properties
	rm -rf ./src/main/webapp/META-INF/context.xml
	rm -rf ./target/site/apidocs

# Both commands now do the same thing.
clean-db: clean-db-force

clean-db-force:
	../scripts/database/DeleteAllTables.sh $(SCRIPT_QUIET)

coverage:
	echo "Generating services code coverage report (runs services tests)..."
	mkdir -p ../coverage
	# Run the integration tests for the code coverage, so we get a report covering all of the code.
	../scripts/maven/mvn-build $(MVN_QUIET) coverage
	rm -rf ../coverage/services
	cp -R target/site/jacoco ../coverage
	mv ../coverage/jacoco ../coverage/services

clean-coverage:
	#../scripts/maven/mvn-build $(MVN_QUIET) jacoco:restore-instrumented-classes
	rm -rf target/site/jacoco
	rm -rf target/jacoco
	rm -rf ../coverage/services

test: config
	echo Running services tests...
ifneq ($(MVN_QUIET),)
	../scripts/maven/mvn-build $(MVN_QUIET) test 2> >( $(FILTER) >&2 )
else
	../scripts/maven/mvn-build $(MVN_QUIET) test
endif

test-all: config
	echo Running services tests...
ifneq ($(MVN_QUIET),)
	../scripts/maven/mvn-build $(MVN_QUIET) verify 2> >( $(FILTER) >&2 )
else
	../scripts/maven/mvn-build $(MVN_QUIET) verify
endif

generate-querydsl-entities: run-liquibase
	rm -rf ./src/main/java/hoot/services/db2
	rm -rf ./src/main/java/hoot/services/models/db
	mvn querydsl:export
