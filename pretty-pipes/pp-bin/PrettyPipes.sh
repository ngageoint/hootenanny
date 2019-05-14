#!/bin/bash

set -e

export PRETTY_PIPES_HOME=$HOME/geoeye/src/hootenanny/pretty-pipes/

#cp $HOOT_HOME/lib/* $HADOOP_HOME/lib/native/Linux-amd64-64/

#zip -1 ../lib/archive.zip ../lib/*.so ../lib/*.so.1 ../lib/*.so.0 &

hadoop fs -ls /pretty-pipes &> /dev/null || hadoop fs -mkdir /pretty-pipes

# Don't care if it fails
hadoop fs -rmr /pretty-pipes/PrettyPipes &> /dev/null || true

hadoop fs -put $PRETTY_PIPES_HOME/bin/PrettyPipes /pretty-pipes/

wait

FILES=`ls ../lib/* | sed s/$/,/g | tr -d '\n'`
FILES=$FILES`ls ../../lib/* | sed s/$/,/g | tr -d '\n'`
echo $FILES
time hadoop pipes -D "CLASSPATH=$CLASSPATH" -files $FILES $*
