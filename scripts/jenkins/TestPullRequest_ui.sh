#!/bin/bash
# Runs all tests that are appropriate for a pull request

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

# activate EGD plugin so it's available for cucumber tests
[ -f ~/ActivateEGDplugin.sh ] && sudo -u tomcat6 ~/ActivateEGDplugin.sh /var/lib/tomcat6

time -p make -s ui-test
