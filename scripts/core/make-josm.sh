#!/bin/bash
set -e

cd /ramdisk/hoot

rsync -rzuv ~/hoot/conf .
rsync -rzuv ~/hoot/hoot-core .
rsync -rzuv ~/hoot/hoot-rnd .
rsync -rzuv ~/hoot/hoot-services .
rsync -rzuv ~/hoot/test-files .
make -j12

echo "Building hoot-josm jar..."
rm -f /home/vagrant/hoot/tmp/hoot-josm.jar

# This is kludgy, but the best that can be done for now, since I'm having trouble getting classpaths to play nicely with Maven and JNI.

# copy me jar out of m2 into tmp as our starting point and rename to be hoot josm jar
# TODO: make the version a wildcard
cp /home/vagrant/.m2/repository/org/openstreetmap/josm/me-josm/4.4.4/me-josm-4.4.4.jar /home/vagrant/hoot/tmp/hoot-josm.jar

# add in external dependency classes
# TODO: make the version a wildcard
# 1) guava
cp hoot-services/target/hoot-services-vagrant/WEB-INF/lib/guava-20.0.jar /home/vagrant/hoot/tmp
cd /home/vagrant/hoot/tmp
jar xf guava-20.0.jar com/google/common
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar com/google/common
rm -rf com
rm -f guava-20.0.jar

# add our hoot josm validator classes
cd /ramdisk/hoot/hoot-services/target/classes
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar hoot/services/validation

chmod -R 777 /home/vagrant/hoot/tmp

cd /ramdisk/hoot
