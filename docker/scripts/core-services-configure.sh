#!/bin/bash
set -euo pipefail

cp LocalConfig.pri.orig LocalConfig.pri

set +u
source SetupEnv.sh
set -u

source conf/database/DatabaseConfig.sh

echo "JAVA_HOME=$JAVA_HOME"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

aclocal
autoconf
autoheader
automake --add-missing --copy
./configure --with-services
