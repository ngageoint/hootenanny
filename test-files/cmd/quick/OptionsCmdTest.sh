#!/bin/bash
set -e

# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/OptionsCmdTest/ConfigOptions.asciidoc

echo "Listing all option names..."
echo ""
hoot options -D config.options.file=$CONFIG_FILE --option-names
echo ""

echo "Listing all option names and descriptions..."
echo ""
hoot options -D config.options.file=$CONFIG_FILE --option-details
echo ""

echo "Listing all option names containing 'poi.polygon'..."
echo ""
hoot options -D config.options.file=$CONFIG_FILE poi.polygon --option-names
echo ""

echo "Listing all option names containing 'poi.polygon' and their descriptions..."
echo ""
hoot options -D config.options.file=$CONFIG_FILE poi.polygon --option-details
