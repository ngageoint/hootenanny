#!/bin/bash
set -e

# We just run the command and check stderr & stdout
# for correctitude
inputfile1=$HOOT_HOME/test-files/cmd/slow/csr/csr.osm
inputfile2=$HOOT_HOME/test-files/cmd/slow/csr/csr.osm
hoot attr-co-occurence $inputfile1 $inputfile2

