#!/bin/bash
set -e

# CONFIG OPTIONS
# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/ConfigOptionsCmdTest/ConfigOptions.asciidoc

CONFIG="-C Testing.conf"

echo "Listing all option names..."
echo ""
hoot info $CONFIG -D config.options.file=$CONFIG_FILE --config-options --option-names
echo ""

echo "Listing all option names and descriptions..."
echo ""
hoot info $CONFIG -D config.options.file=$CONFIG_FILE --config-options --option-details
echo ""

echo "Listing all option names containing 'poi.polygon'..."
echo ""
hoot info $CONFIG -D config.options.file=$CONFIG_FILE --config-options --option-names poi.polygon
echo ""

echo "Listing all option names containing 'poi.polygon' and their descriptions..."
echo ""
hoot info $CONFIG -D config.options.file=$CONFIG_FILE --config-options --option-details poi.polygon

# Since the list of a lot of these will constantly be changing, just going to grep out ones that are unlikely to change any time soon.

# FEATURE EXTRACTORS
echo "Listing all option names..."
echo ""
hoot info $CONFIG --feature-extractors | grep "SampledAngleHistogramExtractor"
echo ""

# FORMATS
echo "Listing a format that is both input and output..."
echo ""
hoot info $CONFIG --formats | grep "hootapidb://"
echo ""
echo "Listing an input format..."
echo ""
hoot info $CONFIG --formats --input | grep ".osm.bz2"
echo ""
echo "Listing an input format supporting streamable reads..."
echo ""
hoot info $CONFIG --formats --input-streamable | grep ".osm"
echo ""
echo "Listing an input format supporting bounded reads..."
echo ""
hoot info $CONFIG --formats --input-bounded | grep "hootapidb://"
echo ""
echo "Listing an output format..."
echo ""
hoot info $CONFIG --formats --output | grep ".shp"
echo ""
echo "Listing an output format supporting streamable reads..."
echo ""
hoot info $CONFIG --formats --output-streamable | grep ".osm"
echo ""
echo "Listing a format used with OGR..."
echo ""
hoot info --formats --ogr | grep ".shp"
echo ""
echo "Listing an input format read by OGR..."
echo ""
hoot info $CONFIG --formats --input --ogr | grep ".shp"
echo ""
echo "Listing an output format written by OGR..."
echo ""
hoot info $CONFIG --formats --output --ogr | grep ".shp"
echo ""
echo "Listing a streamable input format read by OGR..."
echo ""
hoot info $CONFIG --formats --input-streamable --ogr | grep ".shp"
echo ""
echo "Listing a streamable output format written by OGR..."
echo ""
hoot info $CONFIG --formats --input-streamable --ogr | grep ".sql"
echo ""
echo "Listing input formats supported bounded reads that are read by OGR..."
echo ""
hoot info $CONFIG --formats --input-bounded --ogr # none currently exist

# MATCH CREATORS
echo "Listing a match creator..."
echo ""
hoot info $CONFIG --match-creators | grep "HighwayMatchCreator"
echo ""
echo "Listing a script match creator..."
echo ""
hoot info $CONFIG --match-creators | grep "ScriptMatchCreator,Poi.js"
echo ""

# MERGER CREATORS
echo "Listing a merger creator..."
echo ""
hoot info $CONFIG --merger-creators | grep "PoiPolygonMergerCreator"
echo ""
echo "Listing a script merger creator..."
echo ""
hoot info $CONFIG --merger-creators | grep "ScriptMergerCreator"
echo ""

# MATCHERS
echo "Listing a matcher..."
echo ""
hoot info $CONFIG --matchers | grep "HighwayMatch"
echo ""
echo "Listing a script matcher..."
echo ""
hoot info $CONFIG --matchers | grep "ScriptMatch"
echo ""

# MERGERS
echo "Listing a merger..."
echo ""
hoot info $CONFIG --mergers | grep "PoiPolygonMerger"
echo ""
echo "Listing a script merger..."
echo ""
hoot info $CONFIG --mergers | grep "ScriptMerger"
echo ""

# OPERATORS
echo "Listing an op..."
echo ""
hoot info $CONFIG --operators | grep "DuplicateNameRemover"
echo ""
echo "Listing a visitor..."
echo ""
hoot info $CONFIG --operators | grep "RemoveEmptyAreasVisitor"
echo ""
echo "Listing a criterion..."
echo ""
hoot info $CONFIG --operators | grep "NonBuildingAreaCriterion"
echo ""

# TAG MERGERS
echo "Listing a tag merger..."
echo ""
hoot info $CONFIG --tag-mergers | grep "AverageTagMerger"
echo ""

# CLEANING OPS
echo "Listing cleaning operations..."
echo ""
hoot info $CONFIG --cleaning-operations | grep "DuplicateWayRemover"
echo ""

# SUBLINE MATCHERS
echo "Listing subline matchers..."
echo ""
hoot info $CONFIG --subline-matchers | grep "FrechetSublineMatcher"
echo ""

# SUBLINE STRING MATCHERS
echo "Listing subline string matchers..."
echo ""
hoot info $CONFIG --subline-string-matchers | grep "MaximalSublineStringMatcher"
echo ""

# CONFLATE PRE OPS
echo "Listing pre-conflation operations..."
echo ""
hoot info $CONFIG --conflate-pre-operations | grep "MapCleaner"
echo ""

# CONFLATE POST OPS
echo "Listing post-conflation operations..."
echo ""
hoot info $CONFIG --conflate-post-operations | grep "SuperfluousNodeRemover"
echo ""

# WAY JOINERS
echo "Listing way joiners..."
echo ""
hoot info $CONFIG --way-joiners | grep "WayJoinerAdvanced"
echo ""

# WAY SNAP CRITERIA
echo "Listing way snap criteria..."
echo ""
# Check against the default conflate config.
hoot info $CONFIG --way-snap-criteria
# Check against a custom conflate config. Note that the config option goes before --way-snap-criteria.
hoot info $CONFIG -D match.creators="HighwayMatchCreator;ScriptMatchCreator,River.js" \
  -D merger.creators="HighwayMergerCreator;ScriptMergerCreator" --way-snap-criteria
echo ""

# CONFLATABLE CRITERIA
echo "Listing conflatable criteria..."
echo ""
hoot info $CONFIG --conflatable-criteria | grep "BuildingCriterion"
echo ""

# FILTERS (aka ElementCriterion)
echo "Listing filters..."
echo ""
hoot info $CONFIG --filters | grep "NonBuildingAreaCriterion"
echo ""

# CRITERION CONSUMERS
echo "Listing criterion consumers..."
echo ""
hoot info $CONFIG --criterion-consumers | grep "CopyMapSubsetOp"
echo ""

# STRING COMPARATORS
echo "Listing string comparators..."
echo ""
hoot info $CONFIG --string-comparators | grep "ExactStringDistance"
echo ""

# GEOMETRY TYPE CRITERIA
echo "Listing geometry type criteria..."
echo ""
hoot info $CONFIG --geometry-type-criteria | grep "TunnelCriterion"
echo ""

# VALUE AGGREGATORS
echo "Listing value aggregators..."
echo ""
hoot info $CONFIG --value-aggregators | grep "MinAggregator"
echo ""

# VALIDATORS
echo "Listing validators..."
echo ""
hoot info $CONFIG --validators | grep "RoadCrossingPolyMarker"
echo ""
