#!/bin/bash

set -x
set -e

cd $HOOT_HOME

hoot version --debug

export HOOT_TEST_DIFF=--diff
make -s -f Makefile.hoot services-test-all
HootTest --quick --parallel
