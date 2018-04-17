#!/bin/bash
set -e

# Since the operators will constantly be changing, just going to grep out some that are unlikely to change any time soon.

hoot operators | grep "MapCleaner"
hoot operators | grep "ImplicitPoiTypeTagger"
hoot operators | grep "RemoveEmptyAreasVisitor"
hoot operators | grep "NonBuildingAreaCriterion"
