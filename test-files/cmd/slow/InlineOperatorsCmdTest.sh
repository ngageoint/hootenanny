#!/bin/bash
set -e

# Since the operators will constantly be changing, just going to grep out some that are unlikely to change any time soon.

hoot inline-operators | grep "Map Cleaner"
hoot inline-operators | grep "Implicit POI Tagger"
hoot inline-operators | grep "Remove Empty Areas"
hoot inline-operators | grep "Non-Building Area"
