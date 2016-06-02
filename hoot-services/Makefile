
####
# This makefile is here to give a consistent interface to all the projects.
# In other words, I don't like remembering cryptic maven commands. -JRS
####

SHELL=/bin/bash

# If the silent flag is passed to make then make Maven quiet too.
ifneq (,$(findstring s,$(MAKEFLAGS)))
    MVN_QUIET="-q"
endif

default: build

./src/main/resources/db/liquibase.properties: ./src/main/resources/db/liquibase.properties.in ../conf/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh ./src/main/resources/db/liquibase.properties.in ./src/main/resources/db/liquibase.properties

src/main/resources/db/spring-database.xml: src/main/resources/db/spring-database.xml.in ../conf/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh src/main/resources/db/spring-database.xml.in src/main/resources/db/spring-database.xml

src/main/webapp/WEB-INF/workspace/jdbc/WFS_Connection.xml: src/main/webapp/WEB-INF/workspace/jdbc/WFS_Connection.xml.in ../conf/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh src/main/webapp/WEB-INF/workspace/jdbc/WFS_Connection.xml.in src/main/webapp/WEB-INF/workspace/jdbc/WFS_Connection.xml

db.properties: db.properties.in ../conf/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh db.properties.in db.properties

src/main/resources/log4j.xml: src/main/resources/log4j.xml.in ../conf/DatabaseConfig.sh 
	../scripts/ReplaceEnvironmentVariables.sh src/main/resources/log4j.xml.in src/main/resources/log4j.xml

src/main/resources/conf/hoot-services.conf: src/main/resources/conf/hoot-services.conf.in ../conf/DatabaseConfig.sh
	../scripts/ReplaceEnvironmentVariables.sh src/main/resources/conf/hoot-services.conf.in src/main/resources/conf/hoot-services.conf

build-info:
	../scripts/GenerateBuildInfoFiles.sh

config: ./src/main/resources/db/liquibase.properties src/main/resources/db/spring-database.xml src/main/webapp/WEB-INF/workspace/jdbc/WFS_Connection.xml db.properties src/main/resources/conf/hoot-services.conf src/main/resources/log4j.xml

#add -X param temporarily to mvn-build command to debug mvn execution
build: build-info config
	# If there isn't a local copy of the properties, copy the global version.
	# [ -f src/main/resources/conf/hoot-services.conf ] || cp src/main/resources/conf/hoot-services.conf.orig src/main/resources/conf/hoot-services.conf
	rm -rf ./src/main/java/hoot/services/db2
	../scripts/mvn-build $(MVN_QUIET) build
	
	cp target/hoot-services-*.war ../bin

docs: config
	../scripts/mvn-build $(MVN_QUIET) docs

clean: src/main/resources/log4j.xml config clean-coverage
	../scripts/mvn-build $(MVN_QUIET) clean
	rm -f db.properties
	rm -f src/main/resources/log4j.xml
	rm -rf target/site/apidocs

# Both commands now do the same thing.
clean-db: clean-db-force

clean-db-force:
	../scripts/DeleteAllTables.sh
	rm -rf ./src/main/java/hoot/services/db2

coverage:
	echo "Generating services code coverage report (runs services tests)..."
	mkdir -p ../coverage
	# Run the integration tests for the code coverage, so we get a report covering all of the code.
	../scripts/mvn-build $(MVN_QUIET) coverage
	rm -rf ../coverage/services
	cp -R target/site/cobertura ../coverage
	mv ../coverage/cobertura ../coverage/services

clean-coverage:
	#../scripts/mvn-build $(MVN_QUIET) cobertura:clean
	rm -rf target/site/cobertura
	rm -rf target/cobertura
	rm -rf ../coverage/services

test: config
	echo Running services tests...
	rm -rf ./src/main/java/hoot/services/db2
ifneq ($(MVN_QUIET),)
	../scripts/mvn-build $(MVN_QUIET) test 2> >( $(FILTER) >&2 )
else
	../scripts/mvn-build $(MVN_QUIET) test
endif

test-all: config
	echo Running services tests...
	rm -rf ./src/main/java/hoot/services/db2
ifneq ($(MVN_QUIET),)
	../scripts/mvn-build $(MVN_QUIET) verify 2> >( $(FILTER) >&2 )
else
	../scripts/mvn-build $(MVN_QUIET) verify
endif

