#!/bin/bash

#hoot convert tmp/haiti-and-domrep-latest.osm.pbf tmp/haiti-and-domrep-latest.osm
#hoot crop-map tmp/haiti-and-domrep-latest.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbHaitiDrConflateTest/haiti-and-domrep-latest-cropped.osm "-72.48643,18.52015,-72.46768,18.54206"
#hoot crop-map tmp/PapMgcpClip.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbHaitiDrConflateTest/PapMgcpClip-cropped.osm "-72.48643,18.52015,-72.46768,18.54206"

scripts/core/ServiceOsmApiDbHootApiDbConflate.sh test-files/cmd/slow/ServiceOsmApiDbHootApiDbHaitiDrConflateTest/haiti-and-domrep-latest-cropped.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbHaitiDrConflateTest/PapMgcpClip-cropped.osm -72.4822997917,18.5264918926,-72.4768066276,18.531700324028872 ServiceOsmApiDbHootApiDbHaitiDrConflateTest
