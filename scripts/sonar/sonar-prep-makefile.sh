#!/usr/bin/env bash
set -e

# In order to run correctly in sonar, pre-compiled headers need to be turned off

#hoot-cmd/Makefile
# none
#hoot-core/Makefile
if [ -f $HOOT_HOME/hoot-core/Makefile ]; then
  echo "hoot-core/Makefile"
  sed -i "s|(CXX) -c -include tmp/release/HootCore|(CXX) -c -include src/main/cpp/hoot/core/HootCoreStable.h|g" $HOOT_HOME/hoot-core/Makefile
else
  echo "hoot-core/Makefile doesn't exist yet!"
fi
#hoot-core/Makefile.qmake
#sed -i "s|(CXX) -c -include tmp/release/HootCore|(CXX) -c -include src/main/cpp/hoot/core/HootCoreStable.h|g" $HOOT_HOME/hoot-core/Makefile.qmake
#hoot-core-test/Makefile
if [ -f $HOOT_HOME/hoot-core-test/Makefile ]; then
  echo "hoot-core-test/Makefile"
  sed -i "s|(CXX) -c -include tmp/release/HootCoreTest|(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g" $HOOT_HOME/hoot-core-test/Makefile
else
  echo "hoot-core-test/Makefile doesn't exist yet!"
fi
#hoot-josm/Makefile
if [ -f $HOOT_HOME/hoot-josm/Makefile ]; then
  echo "hoot-josm/Makefile"
  sed -i "s|(CXX) -c -include tmp/release/HootJosm|(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g" $HOOT_HOME/hoot-josm/Makefile
else
  echo "hoot-josm/Makefile doesn't exist yet!"
fi
#hoot-js/Makefile
if [ -f $HOOT_HOME/hoot-js/Makefile ]; then
  echo "hoot-js/Makefile"
  sed -i "s|(CXX) -c -include tmp/release/HootJs|(CXX) -c -include src/main/cpp/hoot/js/HootJsStable.h|g" $HOOT_HOME/hoot-js/Makefile
else
  echo "hoot-js/Makefile doesn't exist yet!"
fi
#hoot-rnd/Makefile
if [ -f $HOOT_HOME/hoot-rnd/Makefile ]; then
  echo "hoot-rnd/Makefile"
  sed -i "s|(CXX) -c -include tmp/release/HootRnd|(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g" $HOOT_HOME/hoot-rnd/Makefile
else
  echo "hoot-rnd/Makefile doesn't exist yet!"
fi
#hoot-test/Makefile
# none
#tbs/Makefile
# none
#tgs/Makefile
# none

