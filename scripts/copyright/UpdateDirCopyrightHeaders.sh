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

# Updates all the copyright headers in the current directory recursively. If the copyright header doesn't change then nothing happens.
find . -type f -and \( \( -name \*.js -or -name \*.java -or -name \*.cpp -or -name \*.h \) -and -not \( -wholename \*/db2/\* -or -wholename \*/target/\* -or -wholename \*/tmp/\* -or -name \*.pb.h -or -wholename \*/schema/__init__.js -or -wholename \*/etds\*/__init__.js \) \) -exec $HOOT_HOME/scripts/license/UpdateCopyrightHeader.py --copyright-header=$LICENSE_TEMPLATE --file={} \;
