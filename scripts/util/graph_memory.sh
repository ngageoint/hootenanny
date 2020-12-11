#!/bin/env bash

# https://stackoverflow.com/questions/7998302/graphing-a-processs-memory-usage
# https://gist.github.com/nicolasazrak/32d68ed6c845a095f75f037ecc2f0436

if [ $# -lt 1 ] || [ $# -gt 2 ]
then
  echo "graph_memory.sh requires 1 or 2 command line arguments"
  echo "Usage:"
  echo "     graph_memory.sh <process name> [PNG image path]"
  exit -1
fi

LOG=$(mktemp)
SCRIPT=$(mktemp)
if [ $# -eq 1 ]
then
  IMAGE=$(mktemp)
else
  IMAGE=$2
fi

echo "Output to LOG=$LOG and SCRIPT=$SCRIPT and IMAGE=$IMAGE"

cat >$SCRIPT <<EOL
set term png small size 1200,900
set output "$IMAGE"
set key left box
set ylabel "Memory (MB)"
set xlabel "Seconds"

set grid ytics lc rgb "#bbbbbb" lw 1 lt 0
set grid xtics lc rgb "#bbbbbb" lw 1 lt 0

set ytics nomirror
set yrange [0:*]

plot "$LOG" using (\$2/1024) with filledcurves title "Virtual Memory" lc rgb "#00bf9b0b", "$LOG" using (\$3/1024) with filledcurves title "Real Memory" lc rgb "#004d950f"

EOL

echo "Waiting for $1 to start..."

PID=`pidof $1`
while [ -z $PID ]
do
  sleep 0.1
  PID=`pidof $1`
done

echo "Logging $1 info..."

SAVE_PID=$PID
#echo "$SAVE_PID 0 0" >> $LOG
echo "$SAVE_PID 0 0" >> $LOG

while true
do
  # Drop out when the process doesn't exist anymore
  if [ -z $PID ]
  then
    echo "Rendering graph..."
    echo "$SAVE_PID 0 0" >> $LOG
    gnuplot $SCRIPT > /dev/null
    xdg-open $IMAGE > /dev/null &
    exit 0
  fi
  # Log the process information
  ps -p $PID -o pid= -o vsz= -o rss= >> $LOG
  sleep 1
  PID=`pidof $1`
done

