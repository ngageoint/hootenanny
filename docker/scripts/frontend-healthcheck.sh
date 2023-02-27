#!/bin/sh
set -eu
/usr/bin/curl \
    --insecure \
    --fail \
    --output /dev/null \
    --silent \
    --user-agent docker-healthcheck \
    "https://0.0.0.0:8080/globals.js" # use /globals.js to curcumvent the 304 behavior of non-authorized requests to /
