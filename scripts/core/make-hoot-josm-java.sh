#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm

# Get the josm jar info from the pom.
JOSM_ARTIFACT_ID=`awk -F '[<>]' '/<josm.artifactId>/{print $3}' pom.xml`
JOSM_VERSION=`awk -F '[<>]' '/<josm.version>/{print $3}' pom.xml`
JOSM_JAR=$JOSM_ARTIFACT_ID-$JOSM_VERSION.jar

# Install a local josm jar if its not available from a repo. This needs to be done first time for 
# each new version of a local jar only.
# wget --quiet http://josm-jar-url # replace this URL
# mvn install:install-file -Dfile=/home/vagrant/hoot/tmp/$JOSM_JAR -DgroupId=org.openstreetmap.josm -DartifactId=$JOSM_ARTIFACT_ID -Dversion=$JOSM_VERSION -Dpackaging=jar -DgeneratePom=true

# Build the hoot josm integration jar and copy over the josm jar.
#make clean
mvn -q package

# Rename the josm jar to match what's in the hoot config.
DEP_DIR=target/dependency-jars
mv $DEP_DIR/$JOSM_JAR $DEP_DIR/josm.jar

cd $HOOT_HOME
