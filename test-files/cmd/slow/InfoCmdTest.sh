#!/bin/bash
set -e

# CONFIG OPTIONS
# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/ConfigOptionsCmdTest/ConfigOptions.asciidoc

echo "Listing all option names..."
echo ""
hoot info -D config.options.file=$CONFIG_FILE --config-options --option-names
echo ""

echo "Listing all option names and descriptions..."
echo ""
hoot info -D config.options.file=$CONFIG_FILE --config-options --option-details
echo ""

echo "Listing all option names containing 'poi.polygon'..."
echo ""
hoot info -D config.options.file=$CONFIG_FILE --config-options --option-names poi.polygon
echo ""

echo "Listing all option names containing 'poi.polygon' and their descriptions..."
echo ""
hoot info -D config.options.file=$CONFIG_FILE --config-options --option-details poi.polygon

# Since the list of a lot of these will constantly be changing, just going to grep out ones that are unlikely to change any time soon.

# FEATURE EXTRACTORS
echo "Listing all option names..."
echo ""
hoot info --feature-extractors | grep "SampledAngleHistogramExtractor"
echo ""

# FORMATS
echo "Listing a format that is both input and output..."
echo ""
hoot info --formats | grep "hootapidb://"
echo ""
echo "Listing an input format..."
echo ""
hoot info --formats --input | grep ".osm.bz2"
echo ""
echo "Listing an input format supporting streamable reads..."
echo ""
hoot info --formats --input-streamable | grep ".osm"
echo ""
echo "Listing an input format supporting bounded reads..."
echo ""
hoot info --formats --input-bounded | grep "hootapidb://"
echo ""
echo "Listing an output format..."
echo ""
hoot info --formats --output | grep ".shp"
echo ""
echo "Listing an output format supporting streamable reads..."
echo ""
hoot info --formats --output-streamable | grep ".osm"
echo ""
echo "Listing a format used with OGR..."
echo ""
hoot info --formats --ogr | grep ".shp"
echo ""
echo "Listing an input format read by OGR..."
echo ""
hoot info --formats --input --ogr | grep ".shp"
echo ""
echo "Listing an output format written by OGR..."
echo ""
hoot info --formats --output --ogr | grep ".shp"
echo ""
echo "Listing a streamable input format read by OGR..."
echo ""
hoot info --formats --input-streamable --ogr | grep ".shp"
echo ""
echo "Listing a streamable output format written by OGR..."
echo ""
hoot info --formats --input-streamable --ogr | grep ".sql"
echo ""
echo "Listing input formats supported bounded reads that are read by OGR..."
echo ""
hoot info --formats --input-bounded --ogr # none currently exist

# LANGUAGES
echo "Listing detectable languages..."
echo ""
hoot info -D language.info.provider=hoot::HootServicesLanguageInfoMockClient --languages --detectable
echo ""
echo "Listing translatable languages..."
echo ""
hoot info -D language.info.provider=hoot::HootServicesLanguageInfoMockClient --languages --translatable
echo ""
echo "Listing available language detectors..."
echo ""
hoot info -D language.info.provider=hoot::HootServicesLanguageInfoMockClient --languages --detectors
echo ""
echo "Listing available to English language translators..."
echo ""
hoot info -D language.info.provider=hoot::HootServicesLanguageInfoMockClient --languages --translators
echo ""

# MATCH CREATORS
echo "Listing a match creator..."
echo ""
hoot info --match-creators | grep "HighwayMatchCreator"
echo ""
echo "Listing a script match creator..."
echo ""
hoot info --match-creators | grep "ScriptMatchCreator,Poi.js"
echo ""

# MERGER CREATORS
echo "Listing a merger creator..."
echo ""
hoot info --merger-creators | grep "PoiPolygonMergerCreator"
echo ""
echo "Listing a script merger creator..."
echo ""
hoot info --merger-creators | grep "ScriptMergerCreator"
echo ""

# MATCHERS
echo "Listing a matcher..."
echo ""
hoot info --matchers | grep "HighwayMatch"
echo ""
echo "Listing a script matcher..."
echo ""
hoot info --matchers | grep "ScriptMatch"
echo ""

# MERGERS
echo "Listing a merger..."
echo ""
hoot info --mergers | grep "PoiPolygonMerger"
echo ""
echo "Listing a script merger..."
echo ""
hoot info --mergers | grep "ScriptMerger"
echo ""

# OPERATORS
echo "Listing an op..."
echo ""
hoot info --operators | grep "MapCleaner"
echo ""
echo "Listing a visitor..."
echo ""
hoot info --operators | grep "RemoveEmptyAreasVisitor"
echo ""
echo "Listing a criterion..."
echo ""
hoot info --operators | grep "NonBuildingAreaCriterion"
echo ""

# TAG MERGERS
echo "Listing a tag merger..."
echo ""
hoot info --tag-mergers | grep "AverageTagMerger"
echo ""

# CLEANING OPS
echo "Listing cleaning operations..."
echo ""
hoot info --cleaning-operations | grep "DuplicateWayRemover"
echo ""

# SUBLINE MATCHERS
echo "Listing subline matchers..."
echo ""
hoot info --subline-matchers | grep "FrechetSublineMatcher"
echo ""

# SUBLINE STRING MATCHERS
echo "Listing subline string matchers..."
echo ""
hoot info --subline-string-matchers | grep "MaximalSublineStringMatcher"
echo ""

# CONFLATE PRE OPS
echo "Listing pre-conflation operations..."
echo ""
hoot info --conflate-pre-operations | grep "MapCleaner"
echo ""

# CONFLATE POST OPS
echo "Listing post-conflation operations..."
echo ""
hoot info --conflate-post-operations | grep "SuperfluousNodeRemover"
echo ""

# WAY JOINERS
echo "Listing way joiners..."
echo ""
hoot info --way-joiners | grep "WayJoinerAdvanced"
echo ""

# CONFLATABLE CRITERIA
echo "Listing conflatable criteria..."
echo ""
hoot info --conflatable-criteria | grep "BuildingCriterion"
echo ""

# FILTERS (aka ElementCriterion)
echo "Listing filters..."
echo ""
hoot info --filters | grep "NonBuildingAreaCriterion"
echo ""
