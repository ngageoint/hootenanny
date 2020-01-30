#!/bin/bash
set -e

# CONFIG OPTIONS
# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/ConfigOptionsCmdTest/ConfigOptions.asciidoc

# JOSM MAP VALIDATORS
echo "Listing JOSM validators..."
echo ""
hoot info --josm-validators | grep "DuplicatedWayNodes"
echo ""
