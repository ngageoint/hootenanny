#!/bin/bash
set -e
exitCode=0

if [ -a LicenseTemplate.txt ] ; then
    LICENSE_TEMPLATE=LicenseTemplate.txt
elif [ -a ../LicenseTemplate.txt ] ; then
    LICENSE_TEMPLATE=../LicenseTemplate.txt
else
    echo "Error: LicenseTemplate.txt not found"
    exit 1
fi
echo $LICENSE_TEMPLATE

if [ $# -eq 0 ]; then
    # Need at least one arg which would be the log filename.
    exitCode=2
else
    # ONE argument - should be the log file name.
    if [ $# -eq 1 ]; then
        if [[ $1 == *".log" ]]; then
            # CHECKS all the copyright headers in the current directory recursively noting all files that have copyright headers that need updating.
            find . -type f -and \( \( -name \*.js -or -name \*.java -or -name \*.cpp -or -name \*.h \) -and -not \( -wholename \*/db2/\* -or -wholename \*/target/\* -or -wholename \*/tmp/\* -or -name \*.pb.h -or -wholename \*/schema/__init__.js -or -wholename \*/etds\*/__init__.js \) \) -exec $HOOT_HOME/scripts/copyright/UpdateCopyrightHeader.py --copyright-header=$LICENSE_TEMPLATE --file={} --update-mode=0 \; >> $1
        else
            # Unrecognized .log file name.
            exitCode=3
        fi
    else
        # TWO arguments - should be the log file name and the update switch.
        if [ $# -eq 2 ]; then
            if [[ $1 != *".log" ]]; then
                # Unrecognized .log file name.
                exitCode=3
            else
                if [ $2 == '--update' ] || [ $2 == '-u' ]; then
                    # UPDATES all the copyright headers in the current directory recursively.
                    #echo 'Mode: ' $1
                    find . -type f -and \( \( -name \*.js -or -name \*.java -or -name \*.cpp -or -name \*.h \) -and -not \( -wholename \*/db2/\* -or -wholename \*/target/\* -or -wholename \*/tmp/\* -or -name \*.pb.h -or -wholename \*/schema/__init__.js -or -wholename \*/etds\*/__init__.js \) \) -exec $HOOT_HOME/scripts/copyright/UpdateCopyrightHeader.py --copyright-header=$LICENSE_TEMPLATE --file={} --update-mode=1 \; >> $1
                else
                    # Unrecognized 2nd argument.
                    exitCode=4
                fi
            fi
        else
            # Too many arguments.
            exitCode=5
        fi
    fi
fi

exit $exitCode
