#!/bin/bash

set -x
set -e

cd $HOOT_HOME

make -sj`nproc`

hoot version --debug

export HOOT_TEST_DIFF=--diff
make -sj`nproc` test
