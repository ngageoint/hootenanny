#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm
mkdir -p bin

# This could be changed downstream to package other JOSM versions with string replace of this var.
JOSM_JAR=josm-snapshot-15553.jar
JOSM_DOWNLOAD_URL=https://josm.openstreetmap.de/download # leave this empty to specify a local file
if test -f "$JOSM_JAR"; then
  echo $JOSM_JAR" already downloaded."
elif test -z "$JOSM_DOWNLOAD_URL"; then
  echo "Copying "$JOSM_JAR" to bin/josm.jar..."
else
  echo "Downloading "$JOSM_JAR" to bin/josm.jar..."
  wget --quiet $JOSM_DOWNLOAD_URL/$JOSM_JAR
fi
cp $JOSM_JAR bin/josm.jar
rm -f *.jar.1
chmod 777 bin/josm.jar

echo "Building hoot-josm jar..."
mkdir -p target/classes
# The main downside to this is it will always build, regardless of whether the file has been modified. Will worry about that later and maybe
# get Maven in the mix or check file timestamps.
find -name "*.java" > sources
javac -d target/classes -cp bin/josm.jar @sources
cd target/classes
jar cf $HOOT_HOME/hoot-josm/bin/hoot-josm.jar hoot
cd $HOOT_HOME/hoot-josm
chmod 777 bin/hoot-josm.jar

cp bin/*.jar $HOOT_HOME/bin

cd $HOOT_HOME
