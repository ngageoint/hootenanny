#!/bin/bash

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off ServiceChangesetReplacementBuildingStrictTest test-files/BostonSubsetRoadBuilding_FromOsm.osm "" -71.4698,42.4866,-71.4657,42.4902 true hoot::BuildingCriterion "" "db;xml;json"
