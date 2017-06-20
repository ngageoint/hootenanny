#!/bin/bash

#-104.8192,38.8162,-104.6926,38.9181 - subset data
#-105.7758,38.4511,-101.5594,39.9991 - all data

#scripts/core/ServiceOsmApiDbHootApiDbConflate.sh test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm -104.8192,38.8162,-104.6926,38.9181 "unifying" ServiceOsmApiDbHootApiDbAllDataTypesConflateTest
scripts/core/ServiceOsmApiDbHootApiDbConflate.sh test-files/cmd/slow/ServiceOsmApiDbHootApiDbAllDataTypesConflateTest/AllDataTypesA-with-divided-highway.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbAllDataTypesConflateTest/AllDataTypesB-with-divided-highway.osm -104.9132,38.8242,-104.6926,38.9181 "unifying" ServiceOsmApiDbHootApiDbAllDataTypesConflateTest
