#!/bin/bash
set -euo pipefail

source $HOOT_HOME/scripts/sonar/sonar-config.sh

# Defaults.
USAGE=no
DEBUG_MODE=""
SONAR_BRANCH=""
SONAR_HOST_URL=$SONAR_PROJECT_HOST_URL
SONAR_LOGIN=""
SONAR_ORG=$SONAR_PROJECT_ORG
SONAR_PROJECT=$SONAR_PROJECT_SERVICES

# Getting parameters from the command line.
while getopts ":b:d:j:l:o:" opt; do
    case "$opt" in
        # Required parameters.
        b)
            SONAR_BRANCH="$OPTARG"
            ;;
        d)
            DEBUG_MODE="-X"
            ;;
        j)
            SONAR_PROJECT="$OPTARG"
            ;;
        l)
            SONAR_LOGIN="$OPTARG"
            ;;
        o)
            SONAR_ORG="$OPTARG"
            ;;
        *)
            USAGE=yes
            ;;
    esac
done
shift $((OPTIND-1))

# Setting up.
function usage() {
    echo "sonar-scan.sh:"
    echo "  -l <Sonar Login token>"
    echo " [-a <Sonar GitHub OAuth>]"
    echo " [-b <Sonar Branch Name>]"
    echo " [-d]  # Debug mode"
    exit 1
}

# Abort if invalid options.
if [[ "$USAGE" = "yes" || -z "$SONAR_ORG" || -z "$SONAR_PROJECT" ]]; then
    usage
fi

# Build out the scan command options.
OPTIONS=(
    "-Dsonar.host.url=$SONAR_HOST_URL"
    "-Dsonar.organization=$SONAR_ORG"
    "-Dsonar.projectKey=$SONAR_PROJECT"
)

# Optional scan parameters based off parameters passed into script
if [ -n "$SONAR_LOGIN" ]; then
    # This is the hoot sonarcloud project key, requried to pass scan results to
    # server for analysis and display
    OPTIONS=(
        "${OPTIONS[@]}"
        "-Dsonar.token=$SONAR_LOGIN"
    )
fi

if [ -n "$SONAR_BRANCH" ]; then
    OPTIONS=(
        "${OPTIONS[@]}"
        "-Dsonar.branch.name=$SONAR_BRANCH"
    )
fi

# run the actual maven scanner
JAVA_HOME=$SONAR_JDK_PATH mvn sonar:sonar $DEBUG_MODE "${OPTIONS[@]}"

