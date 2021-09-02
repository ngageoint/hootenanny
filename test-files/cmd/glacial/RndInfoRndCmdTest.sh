#!/bin/bash
set -e

CONFIG="-C Testing.conf"

# LANGUAGES
echo "Listing detectable languages..."
echo ""
hoot info-rnd $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --detectable
echo ""
echo "Listing translatable languages..."
echo ""
hoot info-rnd $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --translatable
echo ""
echo "Listing available language detectors..."
echo ""
hoot info-rnd $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --detectors
echo ""
echo "Listing available to English language translators..."
echo ""
hoot info-rnd $CONFIG -D language.info.provider=HootServicesLanguageInfoMockClient --languages --translators
echo ""
