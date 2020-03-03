#!/bin/bash

input_file=$HOOT_HOME/test-files/io/OsmChangesetElementTest/ToyTestA.osc
input_directory_success=$HOOT_HOME/test-files/cmd/quick/DiffCmdChangesetTest/Success/
input_directory_failure=$HOOT_HOME/test-files/cmd/quick/DiffCmdChangesetTest/Failure/

CONFIG="-C Testing.conf"

# run the changeset diff on a complete directory
hoot diff $CONFIG $input_file $input_directory_success

# run the changeset diff on an incomplete directory
hoot diff $CONFIG $input_file $input_directory_failure

