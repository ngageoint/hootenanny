#!/bin/bash
#
# * Assumes the parent directory is the workspace. E.g.
#   * .../[jenkins workspaces]/[this project workspace]/hootenanny
# * Configure LocalConfig.pri for tests
# * Destroy vagrant instance if necessary
# * Get Hoot read to run the configure tests
#
set -e
set -x

cd $HOOT_HOME

scripts/jenkins/VeryClean.sh

# Maintain vagrant state in the parent directory so very clean will still work.
mkdir -p ../vagrant-hootenanny
ln -s ../vagrant-hootenanny .vagrant

# Update hoot-ui
git submodule update --init

# Jenkins Vagrant setup
ln -s ../../vagrant/VSphereDummy.box VSphereDummy.box
ln -s ../../vagrant/VagrantfileLocal.ubuntu1404 VagrantfileLocal

# Copy words1.sqlite Db so we don't have to download it again
( [ -e $WORDS_HOME/words1.sqlite ] &&  cp $WORDS_HOME/words1.sqlite conf )

# Grab the latest version of the software that the VagrantProvision script will try to download
cp -R ../../software.ubuntu1404 software

# Turning off ccache for the configure tests
cp LocalConfig.pri.orig LocalConfig.pri
#sed -i s/"QMAKE_CXX=g++"/"#QMAKE_CXX=g++"/g LocalConfig.pri                 
#sed -i s/"#QMAKE_CXX=ccache g++"/"QMAKE_CXX=ccache g++"/g LocalConfig.pri   


# Setup the database config. We need to do this since we are not running the VagrantBuild.sh script
cp conf/DatabaseConfig.sh.orig conf/DatabaseConfig.sh

# Make sure we are not running
vagrant halt

# # This causes grief....
#touch Vagrant.marker

REBUILD_VAGRANT=false

[ -f Vagrant.marker ] && [ Vagrant.marker -ot VagrantProvision.sh ] && REBUILD_VAGRANT=true

# On the first build of the day, rebuild everything
if [ "`date +%F`" != "`test -e ../BuildDate.txt && cat ../BuildDate.txt`" ]; then
    REBUILD_VAGRANT=true
fi

# We are going to run a stack of configure tests, we don't neem Tomcat, Mapnik or Hadoop. 
# Just the basic nfs and software dependencies
if [ $REBUILD_VAGRANT == 'true' ]; then
    vagrant destroy -f
    time -p vagrant up --provision-with nfs,hoot,hadoop --provider vsphere
else
    time -p vagrant up --provision-with nfs,hadoop --provider vsphere
fi

date +%F > ../BuildDate.txt

