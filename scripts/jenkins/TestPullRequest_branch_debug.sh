#!/bin/bash
# This is a custom script for debugging tests which pass locally but hang remotely when run on Jenkins PR branches.  Customize it to 
# whatever you need, and then you can run it from the "branch_debug" Jenkins job (or clone your own version of the job).

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

#test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.sh
export HOOT_TEST_DIFF=--diff
bin/HootTest $HOOT_TEST_DIFF --glacial -D script.test.max.exec.time=30 --include=".*ServiceDeriveChangesetCmdXmlJobIdTest.*"
#cat test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.stdout.failed
#cat test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.stderr.failed
