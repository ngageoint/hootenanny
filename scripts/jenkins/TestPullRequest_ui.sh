#!/bin/bash
# Runs all tests that are appropriate for a pull request

set -x
set -e

cd $HOOT_HOME

aclocal
autoconf
autoheader
automake --add-missing
./configure --with-rnd --with-services --with-uitests

make -sj`nproc`

cd $HOOT_HOME/docs
make -sj`nproc`

cd $HOOT_HOME

hoot --version --debug

# activate EGD plugin so it's available for cucumber tests
[ -f ~/ActivateEGDplugin.sh ] && sudo -u tomcat8 ~/ActivateEGDplugin.sh /var/lib/tomcat8

time -p make -s ui-test
