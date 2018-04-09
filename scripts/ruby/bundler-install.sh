#!/bin/bash
set -euo pipefail

# Don't install documentation for gems
if [ ! -f $HOME/.gemrc ]; then
    cat > $HOME/.gemrc <<EOT
install: --no-document
update: --no-document
EOT
fi

RVM_GEM="${RVM_HOME}/bin/rvm ${RUBY_VERSION_HOOT} do gem"

# Install bundler.
if ! $RVM_GEM list --silent --installed bundler --version $BUNDLER_VERSION; then
    $RVM_GEM install --version $BUNDLER_VERSION bundler
fi
