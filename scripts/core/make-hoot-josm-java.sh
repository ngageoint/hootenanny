#!/bin/bash
set -e

cd $HOOT_HOME/hoot-josm
mkdir -p bin

mkdir -p tmp
# These params could be changed downstream to package other JOSM versions with string replace of this var. Since we don't have any compile-time
# dep checks when using JNI against JOSM, don't blindly update to the latest version without checking tests first.
JOSM_PATH=https://josm.openstreetmap.de/download # can either be a url or a dir
JOSM_JAR=josm-snapshot-15796.jar
#JOSM_PATH=/home/vagrant/hoot/tmp
#JOSM_JAR=me-josm-4.4.9.jar
if test -f "tmp/$JOSM_JAR"; then
  echo $JOSM_JAR" already downloaded."
#elif test -z "$JOSM_DOWNLOAD_URL"; then
elif [[ $JOSM_PATH == http* ]]; then
  echo "Downloading "$JOSM_JAR" to bin/josm.jar..."
  wget --quiet $JOSM_DOWNLOAD_URL/$JOSM_JAR
  mv $JOSM_JAR tmp
else
  echo "Copying "$JOSM_JAR" to bin/josm.jar..."
  cp $JOSM_PATH/$JOSM_JAR tmp
fi
cp tmp/$JOSM_JAR bin/josm.jar
rm -f *.jar.1 # failed download
chmod 777 bin/josm.jar

echo "Building hoot-josm jar..."
mkdir -p target/classes
# The main downside to this is it will always build, regardless of whether the file has been modified. Will worry about that later and maybe
# get Maven in the mix or check file timestamps.
rm -f sources
find -name "*.java" > sources
javac -d target/classes -cp bin/josm.jar @sources
cd target/classes
jar cf $HOOT_HOME/hoot-josm/bin/hoot-josm.jar hoot
cd $HOOT_HOME/hoot-josm
chmod 777 bin/hoot-josm.jar

cp bin/*.jar $HOOT_HOME/bin

cd $HOOT_HOME
