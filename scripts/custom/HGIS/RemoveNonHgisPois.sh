#!/bin/bash

export HOOT_OPTIONS="-D api.db.email=test@test.com"
$HOOT_HOME/scripts/RemoveNonHgisPois.js $1 $2

