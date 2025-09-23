#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm

# Get the josm jar info from the pom.
JOSM_ARTIFACT_ID=`awk -F '[<>]' '/<josm.artifactId>/{print $3}' pom.xml`
JOSM_VERSION=`awk -F '[<>]' '/<josm.version>/{print $3}' pom.xml`
JOSM_JAR=$JOSM_ARTIFACT_ID-$JOSM_VERSION.jar

# Install a local josm jar if the version you want is not available from a repo. This needs to be
# done first time for each new version of a local jar only. Download the jar and install it with the
# mvn line below. You can check to see if your public josm jar is already installed by looking at
# the contents of ~/.m2/repository/org/openstreetmap/josm/<JOSM_ARTIFACT_ID>/<JOSM_VERSION>.
# mvn install:install-file -Dfile=<path-to-jar> -DgroupId=org.openstreetmap.josm -DartifactId=<ID> -Dversion=<version> -Dpackaging=jar -DgeneratePom=true

# Build the hoot josm integration jar and copy over the josm jar.
mvn -q package


# In order for our monkey patching of the josm jar to work, we need to remove the josm jar signature
# so we don't get a security error.
DEP_DIR=target/dependency-jars
SRC_JAR=$DEP_DIR/$JOSM_JAR
if [ ! -f "$SRC_JAR" ]; then
  # Fall back to the vendored system dependency location
  SRC_JAR=vendor/$JOSM_JAR
fi
if [ ! -f "$SRC_JAR" ]; then
  echo "ERROR: Could not find $JOSM_JAR in $DEP_DIR or vendor/. Make sure vendor/$JOSM_JAR exists for system-scoped builds." >&2
  exit 1
fi

# Remove signature if present; ignore zip's 'Nothing to do' (exit 12)
zip -qd "$SRC_JAR" "META-INF/*.MF" || [ $? -eq 12 ]

# Ensure destination dir exists and copy as josm.jar for hoot config
install -d "$DEP_DIR"
cp -f "$SRC_JAR" "$DEP_DIR/josm.jar"

cd $HOOT_HOME
