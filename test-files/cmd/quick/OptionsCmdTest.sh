#!/bin/bash
set -e

# use a static config file, so this is not susceptible to config file changes
CONFIG_FILE=test-files/cmd/quick/OptionsCmdTest/ConfigOptions.asciidoc

hoot options -D config.options.file=$CONFIG_FILE

hoot options -D config.options.file=$CONFIG_FILE --detail

hoot options -D config.options.file=$CONFIG_FILE poi.polygon

hoot options -D config.options.file=$CONFIG_FILE poi.polygon --detail
