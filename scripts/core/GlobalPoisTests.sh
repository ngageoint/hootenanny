#!/bin/bash
set -e

# Disable the tests that aren't running on Ubuntu 16.04. They aren't relevant to global POIs.
#hoot --version --debug; HootTest --slow --exclude=.*PrintNamesCmdTest.sh --exclude=.*ConflateCmdStatsGenericRiversTest.sh --exclude=.*PertyScoreCmdTest.sh --exclude=.*ScoreCmdTest.sh --exclude=.*StatsFileTest.sh --exclude=.*PertyMatchScorerTest.* --exclude=.*BigPertyOpTest.* --exclude=.*TDistributionTest.* --exclude=.*ExpectationIntersectionTest.* --exclude=.*PertyDuplicatePoiOpTest.* --exclude=hoot::PertyOpTest.* --parallel `nproc`

#make plugins-test

echo false error
false
