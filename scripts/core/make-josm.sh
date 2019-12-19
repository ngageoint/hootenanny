#!/bin/bash
set -e

HOME=/ramdisk/hoot
#HOME=/home/vagrant/hoot
cd $HOME

rsync -rzuv /home/vagrant/hoot/conf .
rsync -rzuv /home/vagrant/hoot/hoot-core .
rsync -rzuv /home/vagrant/hoot/hoot-core-test .
rsync -rzuv /home/vagrant/hoot/hoot-services .
rsync -rzuv /home/vagrant/hoot/test-files .
make -j12

echo "Building hoot-josm jar..."
rm -f /home/vagrant/hoot/tmp/hoot-josm.jar

# This is kludgy, but the best that can be done for now, since I'm having trouble getting classpaths to play nicely with Maven and JNI.

# copy josm jar out of m2 into tmp as our starting point and rename to be hoot josm jar
cp /home/vagrant/.m2/repository/org/openstreetmap/josm/me-josm/4.4.6/me-josm-4.4.6.jar /home/vagrant/hoot/tmp/hoot-josm.jar
#cp /home/vagrant/.m2/repository/org/openstreetmap/josm/josm-tested/15553/josm-tested-15553.jar /home/vagrant/hoot/tmp/hoot-josm.jar

# add our hoot josm validator classes
echo "Packaging hoot josm classes..."
cd $HOME/hoot-services/target/classes
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar hoot/services/josm

chmod -R 777 /home/vagrant/hoot/tmp

cd $HOME
