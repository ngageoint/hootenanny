#!/bin/bash

export HOOT_OPTIONS="-D api.db.email=test@test.com"
$HOOT_HOME/scripts/PrepareForValidation.js $1 $2
