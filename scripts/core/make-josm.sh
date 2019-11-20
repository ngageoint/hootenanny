#!/bin/bash
set -e

# probably a temp solution until this can all be made to work within maven

cd /ramdisk/hoot

make -j12

echo "Building hoot-josm jar..."
rm -f /home/vagrant/hoot/tmp/hoot-josm.jar

mkdir -p tmp/hoot-josm/hoot/services/validation
cp -R hoot-services/target/classes/hoot/services/validation/*.class tmp/hoot-josm/hoot/services/validation
cd tmp/hoot-josm
jar cf /home/vagrant/hoot/tmp/hoot-josm.jar .
rm -rf tmp/hoot-josm

chmod -R 777 /home/vagrant/hoot/tmp

cd /ramdisk/hoot
