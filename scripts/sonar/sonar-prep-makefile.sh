#!/usr/bin/env bash
set -e

# In order to run correctly in sonar, pre-compiled headers need to be substituted for the actual header
#hoot-cmd/Makefile
# none
#hoot-core/Makefile
if [ -f $HOOT_HOME/hoot-core/Makefile ]; then
  sed -i --regexp-extended 's|\$\(CXX\) -c( -include tmp/release/HootCore)?|$(CXX) -c -include src/main/cpp/hoot/core/HootCoreStable.h|g' $HOOT_HOME/hoot-core/Makefile
else
  echo "hoot-core/Makefile doesn't exist yet!"
fi
#hoot-core/Makefile.qmake
if [ -f $HOOT_HOME/hoot-core/Makefile.qmake ]; then
  sed -i --regexp-extended 's|\$\(CXX\) -c( -include tmp/release/HootCore)?|$(CXX) -c -include src/main/cpp/hoot/core/HootCoreStable.h|g' $HOOT_HOME/hoot-core/Makefile.qmake
else
  echo "hoot-core/Makefile.qmake doesn't exist yet!"
fi
#hoot-core-test/Makefile
if [ -f $HOOT_HOME/hoot-core-test/Makefile ]; then
  sed -i --regexp-extended 's|\$\(CXX\) -c( -include tmp/release/HootCoreTest)?|$(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g' $HOOT_HOME/hoot-core-test/Makefile
else
  echo "hoot-core-test/Makefile doesn't exist yet!"
fi
#hoot-josm/Makefile
if [ -f $HOOT_HOME/hoot-josm/Makefile ]; then
  sed -i --regexp-extended 's|\$\(CXX\) -c( -include tmp/release/HootJosm)?|$(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g' $HOOT_HOME/hoot-josm/Makefile
else
  echo "hoot-josm/Makefile doesn't exist yet!"
fi
#hoot-js/Makefile
if [ -f $HOOT_HOME/hoot-js/Makefile ]; then
  sed -i --regexp-extended 's|\$\(CXX\) -c( -include tmp/release/HootJs)?|$(CXX) -c -include src/main/cpp/hoot/js/HootJsStable.h|g' $HOOT_HOME/hoot-js/Makefile
else
  echo "hoot-js/Makefile doesn't exist yet!"
fi
#hoot-test/Makefile
# none
#tbs/Makefile
# none
#tgs/Makefile
# none

