#!/bin/bash
set -e

# This technically isn't a test for a hoot command...its a test for a method in the hoot js interface.  Put it here b/c I wanted to take
# advantage of the command line testing framework.  The argument can be made to move it somewhere else more fitting, though. - BDW

# way as building input
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-1.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-1.osm'

# relation as building input
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-2.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-2.osm'

# wrong number of args
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-1.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-3.osm'

# missing poi input
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-3.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-4.osm'

# missing building input
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-4.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-5.osm'

# bad poi id
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-1.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-6.osm'

# bad building id
PoiBuildingMergeTest/PoiBuildingMerge.js 'PoiBuildingMergeTest/input-1.osm' -1, -1 'test-output/cmd/PoiBuildingMergeTest/output-7.osm'
