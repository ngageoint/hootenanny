#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm

JOSM_ARTIFACT_ID=`awk -F '[<>]' '/<josm.artifactId>/{print $3}' pom.xml`
JOSM_VERSION=`awk -F '[<>]' '/<josm.version>/{print $3}' pom.xml`
JOSM_JAR=$JOSM_ARTIFACT_ID-$JOSM_VERSION.jar

# This needs to be done first time for each new version of a local jar only (not in a public repo).
# wget --quiet http://josm-jar-url # replace this URL
# mvn install:install-file -Dfile=/home/vagrant/hoot/tmp/$JOSM_JAR -DgroupId=org.openstreetmap.josm -DartifactId=$JOSM_ARTIFACT_ID -Dversion=$JOSM_VERSION -Dpackaging=jar -DgeneratePom=true

mvn -q package

# This prevents from having to change the default value of jni.class.path in ConfigOptions.asciidoc.
DEP_DIR=target/dependency-jars
cp $DEP_DIR/$JOSM_JAR $DEP_DIR/hoot-josm.jar

cd $HOOT_HOME
