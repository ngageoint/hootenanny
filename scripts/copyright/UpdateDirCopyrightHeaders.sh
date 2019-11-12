#!/bin/bash
set -e
updateMode=0
logFilename=""
searchDir=""
LICENSE_TEMPLATE=$HOOT_HOME/scripts/copyright/LicenseTemplate.txt

case $# in
    0|1)
        exit 2 # Need at least two args log filename and search directory
        ;;
    2|3)
        if [ ! -d $1 ]; then
            exit 12 # Search directory not found
        fi
        searchDir=$1
        if [[ $2 != *".log" ]]; then
            exit 3  # Unrecognized .log filename
        fi
        logFilename=$2
        if [ $# -eq 3 ]; then
            if [ $3 != '--update' ] && [ $3 != '-u' ]; then
                exit 4  # Unrecognized update argument
            else
                updateMode=1
            fi
        fi
        ;;
    *)
        exit 5 # Too many arguments
        ;;
esac

cd $searchDir
find . -type f -and \( \( -name \*.js -or -name \*.java -or -name \*.cpp -or -name \*.h \) -and -not \( -wholename \*/db2/\* -or -wholename \*/target/\* -or -wholename \*/tmp/\* -or -name \*.pb.h -or -wholename \*/schema/__init__.js -or -wholename \*/etds\*/__init__.js \) \) -exec $HOOT_HOME/scripts/copyright/UpdateCopyrightHeader.py --copyright-header=$LICENSE_TEMPLATE --file={} --update-mode=$updateMode \; >> $logFilename
cd ..

echo "Complete: $searchDir"

exit $exitCode
