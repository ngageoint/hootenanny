#!/bin/bash

SCRIPT_FILE=`readlink -f ${BASH_SOURCE[0]}`
SCRIPT_PATH="$( cd "$( dirname "$SCRIPT_FILE" )" && pwd )"

source $SCRIPT_PATH/HootEnv.sh $SCRIPT_PATH/hoot.bin "$@"
