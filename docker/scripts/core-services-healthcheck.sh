#!/bin/sh
set -eu

if [ -f core-services-building.txt ]; then
    sleep 5
else
    . ./SetupEnv.sh && hoot version && \
    /usr/bin/curl \
        --fail \
        --output /dev/null \
        --silent \
        --user-agent docker-healthcheck \
        "http://0.0.0.0:8080/hootenanny-id/" && \
    /usr/bin/curl \
        --request OPTIONS \
        --fail \
        --output /dev/null \
        --silent \
        --user-agent docker-healthcheck \
        "http://0.0.0.0:8096/" && \
    /usr/bin/curl \
        --fail \
        --output /dev/null \
        --silent \
        --user-agent docker-healthcheck \
        "http://0.0.0.0:8094/translations" && \
    /usr/bin/curl \
        --fail \
        --output /dev/null \
        --silent \
        --user-agent docker-healthcheck \
        "http://0.0.0.0:8101/options"
fi