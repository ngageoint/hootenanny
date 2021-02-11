#!/usr/bin/env bash
set -e

# In order to run correctly in sonar, pre-compiled headers need to be turned off

#hoot-cmd/Makefile
# none
#hoot-core/Makefile
sed -i "s|(CXX) -c -include tmp/release/HootCore|(CXX) -c -include src/main/cpp/hoot/core/HootCoreStable.h|g" hoot-core/Makefile
#hoot-core/Makefile.qmake
#sed -i "s|(CXX) -c -include tmp/release/HootCore|(CXX) -c -include src/main/cpp/hoot/core/HootCoreStable.h|g" hoot-core/Makefile.qmake
#hoot-core-test/Makefile
sed -i "s|(CXX) -c -include tmp/release/HootCoreTest|(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g" hoot-core-test/Makefile
#hoot-josm/Makefile
sed -i "s|(CXX) -c -include tmp/release/HootJosm|(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g" hoot-josm/Makefile
#hoot-js/Makefile
sed -i "s|(CXX) -c -include tmp/release/HootJs|(CXX) -c -include src/main/cpp/hoot/js/HootJsStable.h|g" hoot-js/Makefile
#hoot-rnd/Makefile
sed -i "s|(CXX) -c -include tmp/release/HootRnd|(CXX) -c -include ../hoot-core/src/main/cpp/hoot/core/HootCoreStable.h|g" hoot-rnd/Makefile
#hoot-test/Makefile
# none
#tbs/Makefile
# none
#tgs/Makefile
# none

