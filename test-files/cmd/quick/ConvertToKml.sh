#!/bin/bash
set -e

rm -rf test-output/cmd/quick/ConvertToKml
mkdir -p test-output/cmd/quick/ConvertToKml

hoot convert -C Testing.conf -D uuid.helper.repeatable=true -D schema.translation.script=translations/HootTest.js test-files/ToyTestA.osm test-output/cmd/quick/ConvertToKml/Output.kml
diff test-output/cmd/quick/ConvertToKml/Output.kml test-files/cmd/quick/ConvertToKml/Expected.kml
