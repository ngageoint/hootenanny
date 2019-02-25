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
echo "Listing an output format..."
echo ""
hoot info --formats --output | grep ".shp"
echo ""

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

# MATCHERS
echo "Listing a match creator..."
echo ""
hoot info --matchers | grep "HighwayMatchCreator"
echo ""
echo "Listing a script match creator..."
echo ""
hoot info --matchers | grep "ScriptMatchCreator,PoiGeneric.js"
echo ""

# MERGERS
echo "Listing a merger..."
echo ""
hoot info --mergers | grep "PoiPolygonMergerCreator"
echo ""
echo "Listing a script merger..."
echo ""
hoot info --mergers | grep "ScriptMergerCreator"
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
