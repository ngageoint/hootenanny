#!/bin/bash

export HOOT_OPTIONS="-D hootapi.db.writer.email=test@test.com -D hootapi.db.reader.email=test@test.com"
$HOOT_HOME/scripts/RemoveNonHgisPois.js $1 $2

