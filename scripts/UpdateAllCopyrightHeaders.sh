#!/bin/bash
set -e

# Updates all the copyright headers in all source directories
for i in $HOOT_HOME/hoot-core/ $HOOT_HOME/hoot-cmd/ $HOOT_HOME/hoot-test $HOOT_HOME/hoot-js $HOOT_HOME/hoot-rnd $HOOT_HOME/hoot-services $HOOT_HOME/tgs $HOOT_HOME/pretty-pipes $HOOT_HOME/tbs
do
    echo $i
    cd $i
    if [ $# -eq 0 ]; then
        $HOOT_HOME/scripts/UpdateDirCopyrightHeaders.sh
    else
        if [ $1 == '--update' ] || [ $1 == '-u' ]; then
            $HOOT_HOME/scripts/UpdateDirCopyrightHeaders.sh $1
        else
            echo 'Unrecognized parameter'
        fi
    fi
done

