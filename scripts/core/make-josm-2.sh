#!/bin/bash
set -e

echo "Building hoot-josm jar..."

cd $HOOT_HOME/hoot-josm
mkdir -p bin
mkdir -p target/classes

rm -f $HOOT_HOME/bin/josm.jar
rm -f $HOOT_HOME/bin/hoot-josm.jar
rm -f bin/*.jar

#cd $HOME/hoot-services/target/classes
#jar uf /home/vagrant/hoot/tmp/hoot-josm.jar hoot/services/josm

#chmod -R 777 /home/vagrant/hoot/tmp

wget --quiet https://josm.openstreetmap.de/download/josm-snapshot-15553.jar bin/josm.jar

#rm -f sources
find -name "*.java" > sources
javac -d target/classes -cp bin/josm.jar @sources

cp bin/*.jar ../bin
