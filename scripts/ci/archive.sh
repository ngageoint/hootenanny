#!/bin/bash
#
# This script generates a Hootenanny archive: a tarball of the source code
# and documentation that's used as input to create a RPM package.
#
set -euo pipefail

# Want to execute all commands relative to HOOT_HOME.
HOOT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../.." && pwd )"
pushd "$HOOT_HOME"

cp LocalConfig.pri.orig LocalConfig.pri

# Temporarily allow undefined variables to allow us to source `SetupEnv.sh`.
set +u
source SetupEnv.sh
set -u

# Ensure database variables are in the environment.
source conf/database/DatabaseConfig.sh

# Generate configure script.
aclocal
autoconf
autoheader
automake --add-missing --copy

# Run configure, enable R&D, services, and PostgreSQL.
./configure --quiet --with-rnd --with-services --with-postgresql

# Make the archive.
make -j"$(nproc)" clean
make -j"$(nproc)" archive

# All done.
popd
