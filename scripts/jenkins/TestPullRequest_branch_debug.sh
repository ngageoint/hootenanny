#!/bin/bash
# 

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

export HOOT_TEST_DIFF=--diff
bin/HootTest $HOOT_TEST_DIFF --glacial -D script.test.max.exec.time=30 --include=".*ServiceDeriveChangesetCmdXmlJobIdTest.*"
cat test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.stdout.failed
cat test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.stderr.failed
# TODO: make this smart enough to pass in custom test commands
#test-files/cmd/slow/ServiceDeriveChangesetCmdXmlJobIdTest.sh
