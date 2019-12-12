#!/bin/bash
set -e

HOME=/ramdisk/hoot
#HOME=/home/vagrant/hoot
cd $HOME

rsync -rzuv /home/vagrant/hoot/conf .
rsync -rzuv /home/vagrant/hoot/hoot-core .
rsync -rzuv /home/vagrant/hoot/hoot-services .
rsync -rzuv /home/vagrant/hoot/test-files .
make -j12

echo "Building hoot-josm jar..."
rm -f /home/vagrant/hoot/tmp/hoot-josm.jar

# This is kludgy, but the best that can be done for now, since I'm having trouble getting classpaths to play nicely with Maven and JNI.

# copy me jar out of m2 into tmp as our starting point and rename to be hoot josm jar
# TODO: make the version a wildcard
cp /home/vagrant/.m2/repository/org/openstreetmap/josm/me-josm/4.4.4/me-josm-4.4.4.jar /home/vagrant/hoot/tmp/hoot-josm.jar

# add in external dependency classes
# TODO: make the version a wildcard

echo "Packaging guava..."
cd $HOME
cp hoot-services/target/hoot-services-vagrant/WEB-INF/lib/guava-20.0.jar /home/vagrant/hoot/tmp
cd /home/vagrant/hoot/tmp
jar xf guava-20.0.jar com/google/common
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar com/google/common
rm -rf com
rm -f guava-20.0.jar

echo "Packaging apache commons lang..."
cd $HOME
cp hoot-services/target/hoot-services-vagrant/WEB-INF/lib/commons-lang3-3.1.jar /home/vagrant/hoot/tmp
cd /home/vagrant/hoot/tmp
jar xf commons-lang3-3.1.jar org/apache/commons/lang3
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar org/apache/commons/lang3
rm -rf org
rm -f commons-lang3-3.1.jar

echo "Packaging tink..."
cd $HOME
cp hoot-services/target/hoot-services-vagrant/WEB-INF/lib/tink-1.3.0-rc2.jar /home/vagrant/hoot/tmp
cd /home/vagrant/hoot/tmp
jar xf tink-1.3.0-rc2.jar com/google/crypto/tink
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar com/google/crypto/tink
rm -rf com
rm -f tink-1.3.0-rc2.jar

echo "Packaging protobuf..."
cd $HOME
cp hoot-services/target/hoot-services-vagrant/WEB-INF/lib/protobuf-java-3.10.0.jar /home/vagrant/hoot/tmp
cd /home/vagrant/hoot/tmp
jar xf protobuf-java-3.10.0.jar com/google/protobuf
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar com/google/protobuf
rm -rf com
rm -f protobuf-java-3.10.0.jar

# add our hoot josm validator classes
echo "Packaging hoot josm classes..."
cd $HOME/hoot-services/target/classes
jar uf /home/vagrant/hoot/tmp/hoot-josm.jar hoot/services/josm

chmod -R 777 /home/vagrant/hoot/tmp

cd $HOME
