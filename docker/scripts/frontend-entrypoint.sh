#!/bin/bash
set -euo pipefail

if [ "${HOOT_NPM_INSTALL:-0}" == "1" ] || [ ! -d node_modules ]; then
    npm install && npm run all
fi

npm start
