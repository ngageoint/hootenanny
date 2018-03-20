#!/bin/bash
set -e

# We just run the command and check stderr & stdout
# for correctitude
inputfile1=$HOOT_HOME/test-files/cmd/glacial/csr/csr.osm
inputfile2=$HOOT_HOME/test-files/cmd/glacial/csr/csr.osm
hoot attribute-co-occurrence $inputfile1 $inputfile2

