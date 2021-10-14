#!/bin/bash
set -e

# Note that not all options support the JSON format, so we only test those that do. Also, the JSON
# itself isn't being validated since a single line is pulled out of it.

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
echo "Listing feature extractors..."
echo ""
hoot info $CONFIG --feature-extractors | grep "SampledAngleHistogramExtractor"
echo ""
echo "Listing feature extractors as json..."
echo ""
hoot info $CONFIG --feature-extractors --json | grep "SampledAngleHistogramExtractor"
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
echo "Listing a matcher as json..."
echo ""
hoot info $CONFIG --matchers --json | grep "HighwayMatch"
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
echo "Listing a merger as json..."
echo ""
hoot info $CONFIG --mergers --json | grep "PoiPolygonMerger"
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
echo "Listing a tag merger as json..."
echo ""
hoot info $CONFIG --tag-mergers --json | grep "AverageTagMerger"
echo ""

# CLEANING OPS
echo "Listing a cleaning operation..."
echo ""
hoot info $CONFIG --cleaning-operations | grep "DuplicateWayRemover"
echo ""

# SUBLINE MATCHERS
echo "Listing a subline matcher..."
echo ""
hoot info $CONFIG --subline-matchers | grep "FrechetSublineMatcher"
echo ""
echo "Listing a subline matcher as json..."
echo ""
hoot info $CONFIG --subline-matchers --json | grep "FrechetSublineMatcher"
echo ""

# SUBLINE STRING MATCHERS
echo "Listing a subline string matcher..."
echo ""
hoot info $CONFIG --subline-string-matchers | grep "MaximalSublineStringMatcher"
echo ""
echo "Listing a subline string matcher as json..."
echo ""
hoot info $CONFIG --subline-string-matchers --json | grep "MaximalSublineStringMatcher"
echo ""

# CONFLATE PRE OPS
echo "Listing a pre-conflation operation..."
echo ""
hoot info $CONFIG --conflate-pre-operations | grep "MapCleaner"
echo ""

# CONFLATE POST OPS
echo "Listing a post-conflation operation..."
echo ""
hoot info $CONFIG --conflate-post-operations | grep "SuperfluousNodeRemover"
echo ""

# WAY JOINERS
echo "Listing a way joiner..."
echo ""
hoot info $CONFIG --way-joiners | grep "WayJoinerAdvanced"
echo ""
echo "Listing a way joiner as json..."
echo ""
hoot info $CONFIG --way-joiners --json | grep "WayJoinerAdvanced"
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
echo "Listing a conflatable criterion..."
echo ""
hoot info $CONFIG --conflatable-criteria | grep "BuildingCriterion"
echo ""
echo "Listing a conflatable criterion as json..."
echo ""
hoot info $CONFIG --conflatable-criteria --json | grep "BuildingCriterion"
echo ""

# FILTERS (aka ElementCriterion)
echo "Listing a filter..."
echo ""
hoot info $CONFIG --filters | grep "NonBuildingAreaCriterion"
echo ""
echo "Listing a filter as json..."
echo ""
hoot info $CONFIG --filters --json | grep "NonBuildingAreaCriterion"
echo ""

# CRITERION CONSUMERS
echo "Listing a criterion consumer..."
echo ""
hoot info $CONFIG --criterion-consumers | grep "CopyMapSubsetOp"
echo ""

# STRING COMPARATORS
echo "Listing a string comparator..."
echo ""
hoot info $CONFIG --string-comparators | grep "ExactStringDistance"
echo ""
echo "Listing a string comparator as json..."
echo ""
hoot info $CONFIG --string-comparators --json | grep "ExactStringDistance"
echo ""

# GEOMETRY TYPE CRITERIA
echo "Listing a geometry type criterion..."
echo ""
hoot info $CONFIG --geometry-type-criteria | grep "TunnelCriterion"
echo ""
echo "Listing a geometry type criterion as json..."
echo ""
hoot info $CONFIG --geometry-type-criteria --json | grep "TunnelCriterion"
echo ""

# VALUE AGGREGATORS
echo "Listing a value aggregator..."
echo ""
hoot info $CONFIG --value-aggregators | grep "MinAggregator"
echo ""
echo "Listing a value aggregator as json..."
echo ""
hoot info $CONFIG --value-aggregators --json | grep "MinAggregator"
echo ""

# VALIDATORS
echo "Listing a validator..."
echo ""
hoot info $CONFIG --validators | grep "RoadCrossingPolyMarker"
echo ""
echo "Listing a validator as json..."
echo ""
hoot info $CONFIG --validators --json | grep "RoadCrossingPolyMarker"
echo ""

# LANGUAGES
echo "Listing detectable languages..."
echo ""
hoot info $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --detectable
echo ""
echo "Listing translatable languages..."
echo ""
hoot info $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --translatable
echo ""
echo "Listing available language detectors..."
echo ""
hoot info $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --detectors
echo ""
echo "Listing available to English language translators..."
echo ""
hoot info $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --translators
echo ""
