#!/bin/bash
set -e

# Since the operators will constantly be changing, just going to grep out some that are unlikely to change any time soon.

hoot operators | grep "Map Cleaner"
hoot operators | grep "Implicit POI Tagger"
hoot operators | grep "Remove Empty Areas"
hoot operators | grep "Non-Building Area"
