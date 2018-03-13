#!/bin/bash
set -euo pipefail

# Don't install documentation for gems
if [ ! -f $HOME/.gemrc ]; then
    cat > $HOME/.gemrc <<EOT
install: --no-document
update: --no-document
EOT
fi

# Install bundler.
if ! gem list --silent --installed bundler --version $BUNDLER_VERSION; then
    gem install --version $BUNDLER_VERSION bundler
fi
