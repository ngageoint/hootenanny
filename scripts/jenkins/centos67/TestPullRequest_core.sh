#!/bin/bash

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

export HOOT_TEST_DIFF=--diff
make -s -f Makefile.hoot services-test-all
HootTest --exclude=.*ConflateAverageTest.sh \
         --exclude=.*ExactMatchInputsTest.sh \
         --exclude=.*RafahConflateTest.sh \
         --exclude=.*HaitiDrConflateTest.sh \
         --exclude=.*DbDcStreetsNetworkConflateTest.sh \
         --exclude=.*NetworkConflateCmd.* \
         --exclude=.*OsmApiDbHootApiDb.* \
         --exclude=.*MultiaryIngestCmd.* \
         --glacial \
         --parallel \
         --diff
