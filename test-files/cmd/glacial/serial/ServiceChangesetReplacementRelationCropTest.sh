#!/bin/bash
set -e

# Tests to make sure cropping of relations doesn't leave behind missing relation member references

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRelationCropTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRelationCropTest/Input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRelationCropTest/Input2.osm" "7.910156,5.003394,7.998047,5.090946" "-180,-90,180,90" "false" "false" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "false"
