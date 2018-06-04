#!/usr/bin/env bash
set -e

# set HOOT_HOME if not set
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi

# generate gcov files from gcda files
cd $HOOT_HOME
gcov ./hoot-core/tmp/debug/*.gcda
gcov ./tbs/tmp/debug/*.gcda
gcov ./hoot-rnd/tmp/debug/*.gcda