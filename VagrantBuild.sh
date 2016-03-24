#!/usr/bin/env bash

# We assume that $HOOT_HOME has been set in ~/.profile.....
source ~/.profile
cd $HOOT_HOME
source ./SetupEnv.sh

aclocal && autoconf && autoheader && automake && ./configure --quiet --with-rnd --with-services --with-uitests
if [ ! -f LocalConfig.pri ] && ! grep --quiet QMAKE_CXX LocalConfig.pri; then
    echo 'Customizing LocalConfig.pri...'
    cp LocalConfig.pri.orig LocalConfig.pri
    # ccache is not enabled by default
    sed -i s/"QMAKE_CXX=g++"/"#QMAKE_CXX=g++"/g LocalConfig.pri
    sed -i s/"#QMAKE_CXX=ccache g++"/"QMAKE_CXX=ccache g++"/g LocalConfig.pri
fi
echo "### Building Hoot... "
echo "Will take several extra minutes to build the training data the initial time Hootenanny is installed only."
make -sj$(nproc)
make -sj$(nproc) docs
sudo -u tomcat6 scripts/CopyWebAppsToTomcat.sh

echo "### Hootenanny successfully installed to virtual machine:"
hoot version
echo "See VAGRANT.md for additional configuration instructions and then access the web application from the host machine at: http://localhost:8888/hootenanny-id"
echo "Run 'vagrant ssh' at any time from the host machine to log into the Hootenanny virtual machine."
echo "See $HOOT_HOME/docs for Hootenanny documentation files."
echo "See VAGRANT.md for instructions on running the diagnostic tests."

