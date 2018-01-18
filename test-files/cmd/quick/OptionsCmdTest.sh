#!/bin/bash
set -e

# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/OptionsCmdTest/ConfigOptions.asciidoc

hoot options -D $CONFIG_FILE

hoot options -D $CONFIG_FILE --detail

hoot options -D $CONFIG_FILE poi.polygon

hoot options -D $CONFIG_FILE poi.polygon --detail
