#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm

# TODO: these are replicated in pom.xml
JOSM_ARTIFACT_ID=josm
JOSM_VERSION=15628
#JOSM_ARTIFACT_ID=me-josm
#JOSM_VERSION=4.4.9
JOSM_JAR=$JOSM_ARTIFACT_ID-$JOSM_VERSION.jar

# needs to be done first time for the me jar only
# wget --quiet http://me-josm-url
# mvn install:install-file -Dfile=/home/vagrant/hoot/tmp/$JOSM_JAR -DgroupId=org.openstreetmap.josm -DartifactId=$JOSM_ARTIFACT_ID -Dversion=$JOSM_VERSION -Dpackaging=jar -DgeneratePom=true

mvn package

# This prevents from having to change the default value of jni.class.path in ConfigOptions.asciidoc.
DEP_DIR=target/dependency-jars
cp $DEP_DIR/$JOSM_JAR $DEP_DIR/hoot-josm.jar
