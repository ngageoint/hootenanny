#!/bin/bash
set -e

hoot schema -D tag.printing.format=asciidoc > $HOOT_HOME/docs/schema.asciidoc
