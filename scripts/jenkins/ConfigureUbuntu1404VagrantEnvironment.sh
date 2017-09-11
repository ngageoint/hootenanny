#!/bin/bash
#
# * Assumes the parent directory is the workspace. E.g.
#   * .../[jenkins workspaces]/[this project workspace]/hootenanny
# * Configure LocalConfig.pri for tests
# * Destroy vagrant instance if necessary
# * Build hoot with most things enabled
#
set -e
set -x

cd $HOOT_HOME

# Just wipe out the files. Db cleaning comes later
scripts/jenkins/VeryClean.sh

# Recreate this folder that Tomcat requires
if [ ! -d "$HOOT_HOME/userfiles/ingest/processed" ]; then
  mkdir -p $HOOT_HOME/userfiles/ingest/processed
fi

# Maintain vagrant state in the parent directory so very clean will still work.
mkdir -p ../vagrant-hootenanny
ln -s ../vagrant-hootenanny .vagrant

# Update hoot-ui
git submodule update --init

# Copy words1.sqlite Db so we don't have to download it again
( [ -e $WORDS_HOME/words1.sqlite ] &&  cp $WORDS_HOME/words1.sqlite conf )

# Grab the latest version of the software that the VagrantProvision script will try to download
cp -R ../../software.ubuntu1404 software

# Error out on any warnings. This only applies to Ubuntu 14.04, not CentOS (yet)
# See: https://github.com/ngageoint/hootenanny/issues/348
cp LocalConfig.pri.orig LocalConfig.pri
echo "QMAKE_CXXFLAGS += -Werror" >> LocalConfig.pri
sed -i s/"QMAKE_CXX=g++"/"#QMAKE_CXX=g++"/g LocalConfig.pri                 
sed -i s/"#QMAKE_CXX=ccache g++"/"QMAKE_CXX=ccache g++"/g LocalConfig.pri   

# Make sure we are not running
vagrant halt

REBUILD_VAGRANT=false

[ -f Vagrant.marker ] && [ Vagrant.marker -ot VagrantProvision.sh ] && REBUILD_VAGRANT=true

# On the first build of the day, rebuild everything
if [ "`date +%F`" != "`test -e ../BuildDate.txt && cat ../BuildDate.txt`" ]; then
    REBUILD_VAGRANT=true
fi

# Disable VM reuse option.  Not working with AWS provider.
#if [ $REBUILD_VAGRANT == 'true' ]; then
    vagrant destroy -f
    time -p vagrant up --provider aws
#else
#    time -p vagrant up --provision-with build,EGD,tomcat,mapnik,hadoop --provider aws
#fi

# Disableing this until it gets moved earlier into the build.
# Clean out the Database
#vagrant ssh -c "cd hoot; source ./SetupEnv.sh; cd hoot-services; make clean-db &> /dev/null"

date +%F > ../BuildDate.txt
