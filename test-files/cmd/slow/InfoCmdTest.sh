#!/bin/bash
set -e

# CONFIG OPTIONS
# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/ConfigOptionsCmdTest/ConfigOptions.asciidoc

echo "Listing all option names..."
echo ""
hoot info --config-options --option-names -D config.options.file=$CONFIG_FILE
echo ""

echo "Listing all option names and descriptions..."
echo ""
hoot info --config-options --option-details -D config.options.file=$CONFIG_FILE
echo ""

echo "Listing all option names containing 'poi.polygon'..."
echo ""
hoot info --config-options --option-names -D config.options.file=$CONFIG_FILE poi.polygon
echo ""

echo "Listing all option names containing 'poi.polygon' and their descriptions..."
echo ""
hoot info --config-options --option-details -D config.options.file=$CONFIG_FILE poi.polygon

# Since the list of a lot of these will constantly be changing, just going to grep out ones that are unlikely to change any time soon.

# FEATURE EXTRACTORS
hoot info --feature-extractors | grep "SampledAngleHistogramExtractor"

# FORMATS
hoot info --formats | grep "hootapidb://"
hoot info --formats --input | grep ".osm.bz2"
hoot info --formats --output | grep ".shp"

# MATCHERS
hoot info --matchers | grep "HighwayMatchCreator"
hoot info --matchers | grep "ScriptMatchCreator"

# MERGERS
hoot info --mergers | grep "PoiPolygonMergerCreator"
hoot info --mergers | grep "ScriptMergerCreator"

# OPERATORS
hoot info --operators | grep "MapCleaner"
hoot info --operators | grep "ImplicitPoiTypeTagger"
hoot info --operators | grep "RemoveEmptyAreasVisitor"
hoot info --operators | grep "NonBuildingAreaCriterion"

# TAG MERGERS
hoot info --tag-mergers | grep "AverageTagMerger"
