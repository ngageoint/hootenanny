#!/bin/bash

export HOOT_OPTIONS="-D services.db.writer.email=test@test.com -D services.db.reader.email=test@test.com" 
$HOOT_HOME/scripts/PrepareForValidation.js $1 $2
