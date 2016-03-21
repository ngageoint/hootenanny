#!/bin/bash
set -e

NPROC=`eval nproc`
sed -i "s/<forkCount><\/forkCount>/<forkCount>$NPROC<\/forkCount>/g" $1

