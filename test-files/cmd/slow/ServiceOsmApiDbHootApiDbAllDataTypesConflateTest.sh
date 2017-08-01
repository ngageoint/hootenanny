#!/bin/bash

scripts/core/ServiceOsmApiDbHootApiDbConflate.sh test-files/cmd/slow/ServiceOsmApiDbHootApiDbAllDataTypesConflateTest/AllDataTypesA-with-divided-highway.osm test-files/cmd/slow/ServiceOsmApiDbHootApiDbAllDataTypesConflateTest/AllDataTypesB-with-divided-highway.osm -104.9132,38.8242,-104.6926,38.9181 "unifying" ServiceOsmApiDbHootApiDbAllDataTypesConflateTest false -1
