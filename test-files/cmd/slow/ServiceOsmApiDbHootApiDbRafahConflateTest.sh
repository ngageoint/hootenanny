#!/bin/bash

#hoot crop-map tmp/rafah.osm tmp/rafah-cropped.osm "34.046,31.178,34.057,31.184"

scripts/core/ServiceOsmApiDbHootApiDbConflate.sh test-files/cmd/slow/ServiceOsmApiDbHootApiDbRafahConflateTest/rafah-cropped.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbRafahConflateTest/secondary.osm 34.04725915004996,31.17907522629906,34.05654176863703,31.18329523832873 ServiceOsmApiDbHootApiDbRafahConflateTest
