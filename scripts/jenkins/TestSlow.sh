#!/bin/bash

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

export HOOT_TEST_DIFF=--diff
make -s -f Makefile.old services-test-all
HootTest --exclude=.*ConflateAverageTest.sh \
         --exclude=.*RubberSheetConflateTest.sh \
         --exclude=.*ExactMatchInputsTest.sh \
         --exclude=.*RafahConflateTest.sh \
         --exclude=.*HaitiDrConflateTest.sh \
         --slow

