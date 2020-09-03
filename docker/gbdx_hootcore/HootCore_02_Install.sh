#!/usr/bin/env bash

set -e

#####
# NOTE: This is a VERY cut down version of Hootenanny for a single purpose
#
# It runs on Centos7
#
# It DOES NOT include a database or a UI
#
#
# MattJ


###################################################
# VERY IMPORTANT: Set the $HOOT_HOME environment  #
# variable prior to running this script if ~/hoot #
# isn't the correct location for HOOT_HOME        #
###################################################
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
echo HOOT_HOME: $HOOT_HOME
#################################################


#####
cd $HOOT_HOME

echo "Copying GBDX files"
cp docker/hootcore/*.py /


echo "Installing Hoot"
make -s install

# Get Hoot into the default path
echo "export HOOT_HOME=/var/lib/hootenanny" > /etc/profile.d/hootenanny.sh

chmod 755 /etc/profile.d/hootenanny.sh
cp -R test-files /var/lib/hootenanny/
ln -s /usr/lib64 /var/lib/hootenanny/lib
rm -f /usr/bin/HootEnv.sh

# This allows all the tests to run.
mkdir -p /var/lib/hootenanny/hoot-core-test/src/test/
ln -s /var/lib/hootenanny/test-files/ /var/lib/hootenanny/hoot-core-test/src/test/resources

# This makes it so HootEnv.sh resolves hoot home properly.
ln -s /var/lib/hootenanny/bin/HootEnv.sh /usr/bin/HootEnv.sh

##########################################
