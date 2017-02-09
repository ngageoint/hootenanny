#!/bin/bash
set -e

if [ -a LicenseTemplate.txt ] ; then
    LICENSE_TEMPLATE=LicenseTemplate.txt
elif [ -a ../LicenseTemplate.txt ] ; then
    LICENSE_TEMPLATE=../LicenseTemplate.txt
else
    echo Error locating LicenseTemplate.txt
    exit -1
fi
echo $LICENSE_TEMPLATE

if [ $# -eq 0 ]; then
    # CHECKS all the copyright headers in the current directory recursively noting all files that have copyright headers than need updating.
    find . -type f -and \( \( -name \*.js -or -name \*.java -or -name \*.cpp -or -name \*.h \) -and -not \( -wholename \*/db2/\* -or -wholename \*/target/\* -or -wholename \*/tmp/\* -or -name \*.pb.h -or -wholename \*/schema/__init__.js -or -wholename \*/etds\*/__init__.js \) \) -exec $HOOT_HOME/scripts/UpdateCopyrightHeader.py --copyright-header=$LICENSE_TEMPLATE --file={} --update-mode=0 \; #>> ~/Documents/ucrh_all_update-mode-0_results.txt
else
    if [ $1 == '--update' ] || [ $1 == '-u' ]; then
        # UPDATES all the copyright headers in the current directory recursively.
        #echo 'Mode: ' $1
        find . -type f -and \( \( -name \*.js -or -name \*.java -or -name \*.cpp -or -name \*.h \) -and -not \( -wholename \*/db2/\* -or -wholename \*/target/\* -or -wholename \*/tmp/\* -or -name \*.pb.h -or -wholename \*/schema/__init__.js -or -wholename \*/etds\*/__init__.js \) \) -exec $HOOT_HOME/scripts/UpdateCopyrightHeader.py --copyright-header=$LICENSE_TEMPLATE --file={} --update-mode=1 \; #>> ~/Documents/ucrh_all_update-mode-1_results.txt
    fi
fi

