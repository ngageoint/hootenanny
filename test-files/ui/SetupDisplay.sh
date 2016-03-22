#!/bin/bash
#set -x

#
# This script runs a xvfb display (headless) in the background if a display
# isn't already available.
#

LOCK_FILE=/tmp/.X$EUID-lock

if [ "$DISPLAY" == "" ]; then
    [ -f $LOCK_FILE ] && kill `cat $LOCK_FILE`
    sleep 3
    # Run an x session on the display w/ the user id
    Xvfb :$EUID -ac -screen 0 1600x1200x24 &> /dev/null &
    export XVFB_PID=$!
    sleep 3

    export DISPLAY=:$EUID.0
fi

"$@"
RESULT=$?

[ -f $LOCK_FILE ] && kill `cat $LOCK_FILE`
sleep 3

exit $RESULT

