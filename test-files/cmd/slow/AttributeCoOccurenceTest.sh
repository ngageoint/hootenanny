#!/bin/bash
set -e

# We just run the command and check stderr & stdout
# for correctitude
hoot attr-co-occurence $HOOT_HOME/test-files/ToyTestA.osm $HOOT_HOME/test-files/ToyTestB.osm

