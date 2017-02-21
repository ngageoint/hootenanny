#!/bin/bash

# slightly larger aoi for ref dataset testing: -72.509,18.517,-72.463,18.558
# haiti-and-domrep-latest-cropped.osm
# haiti-and-domrep-latest.osm.pbf
# old aoi: -72.4987792839,18.5317003042,-72.4932861198,18.5369085571
# new aoi: -72.4822997917,18.5264918926,-72.4768066276,18.531700324028872

#hoot convert tmp/haiti-and-domrep-latest.osm.pbf test-files/cmd/slow/haiti-and-domrep-latest.osm
#hoot crop-map tmp/haiti-and-domrep-latest.osm test-files/cmd/slow/haiti-and-domrep-latest-cropped.osm "-72.509,18.517,-72.463,18.558"
scripts/core/ServiceOsmApiDbHootApiDbConflate.sh tmp/haiti-and-domrep-latest-cropped.osm tmp/PapMgcpClip.osm -72.4822997917,18.5264918926,-72.4768066276,18.531700324028872 ServiceOsmApiDbHootApiDbHaitiDrConflateTest
