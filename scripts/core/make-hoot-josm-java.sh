#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm
mkdir -p bin
rm -f bin/*.jar

# This can be changed downstream to package with other JOSM versions with a file text replacement.
rm -f $HOOT_HOME/bin/hoot-josm.jar
JOSM_JAR=josm-snapshot-15553.jar
if test -f "bin/$JOSM_JAR"; then
  echo $JOSM_JAR" already downloaded to bin/josm.jar."
else
  echo "Downloading "$JOSM_JAR" to bin/josm.jar..."
  wget --quiet https://josm.openstreetmap.de/download/josm-snapshot-15553.jar
fi
cp $JOSM_JAR bin/josm.jar
chmod 777 bin/josm.jar

echo "Building hoot-josm jar..."
#rm -f $HOOT_HOME/bin/josm.jar
mkdir -p target/classes
rm -rf target/classes/*
# The main downside to this is it will always build, regardless of whether the file has been modified. Will worry about that later and maybe
# get Maven in the mix.
find -name "*.java" > sources
javac -d target/classes -cp bin/josm.jar @sources
cd target/classes
jar cf $HOOT_HOME/hoot-josm/bin/hoot-josm.jar hoot
cd $HOOT_HOME/hoot-josm
chmod 777 bin/hoot-josm.jar

cp bin/*.jar $HOOT_HOME/bin

cd $HOOT_HOME
