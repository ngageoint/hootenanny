#!/bin/bash
# This is a custom script for debugging tests which pass locally but hang remotely when run on Jenkins PR branches.  Customize it to 
# whatever you need, and then you can run it from a cloned Jenkins test job.

set -x
set -e

cd $HOOT_HOME

make -sj4

hoot --version --debug

#test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.sh
export HOOT_TEST_DIFF=--diff
bin/HootTest $HOOT_TEST_DIFF --glacial -D script.test.max.exec.time=30 --include=".*ServiceDeriveChangesetCmdXmlJobIdTest.*"
