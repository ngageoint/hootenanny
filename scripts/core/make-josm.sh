#!/bin/bash
set -e

# probably a temp solution until this can all be made to work within maven

cd /ramdisk/hoot

rsync -rzuv ~/hoot/hoot-rnd .
rsync -rzuv ~/hoot/hoot-services .
make -j12

echo "Building hoot-josm jar..."
rm -f /home/vagrant/hoot/tmp/hoot-josm.jar

#mkdir -p tmp/hoot-josm/hoot/services/validation
#cp hoot-services/target/classes/hoot/services/validation/*.class tmp/hoot-josm/hoot/services/validation
#cd tmp/hoot-josm
#jar cf /home/vagrant/hoot/tmp/hoot-josm.jar .
#rm -rf tmp/hoot-josm

# copy me jar out of m2 into tmp and rename
cp /home/vagrant/.m2/repository/org/openstreetmap/josm/me-josm/4.4.4/me-josm-4.4.4.jar /home/vagrant/hoot/tmp/hoot-josm.jar
# use jar to add the validator class to it
cd hoot-services/target/classes
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar hoot/services/validation/JosmValidator.class

chmod -R 777 /home/vagrant/hoot/tmp

cd /ramdisk/hoot
