#!/bin/bash

set -x
set -e

make -sj`nproc`

hoot --version --debug

export HOOT_TEST_DIFF=--diff
make -sj`nproc` test-all
