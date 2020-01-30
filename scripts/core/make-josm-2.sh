#!/bin/bash
set -e

echo "Building hoot-josm jar..."

cd $HOOT_HOME/hoot-josm
mkdir -p bin
mkdir -p target/classes

rm -f $HOOT_HOME/bin/josm.jar
rm -f $HOOT_HOME/bin/hoot-josm.jar
rm -f bin/*.jar

# This can be changed downstream to package with other JOSM versions with a file text replacement.
JOSM_JAR=bin/josm-snapshot-15553.jar
if test -f "$JOSM_JAR"; then
else
  wget --quiet https://josm.openstreetmap.de/download/josm-snapshot-15553.jar
fi
cp $JOSM_JAR bin/josm.jar
chmod 777 bin/josm.jar

#rm -f sources
find -name "*.java" > sources
javac -d target/classes -cp bin/josm.jar @sources
jar cf bin/hoot-josm.jar target/classes
chmod 777 bin/hoot-josm.jar

cp bin/*.jar $HOOT_HOME/bin
