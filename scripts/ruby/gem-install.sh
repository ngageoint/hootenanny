#!/usr/bin/env bash
set -e

# Don't install documentation for gems
if [ ! -f $HOME/.gemrc ]; then
    cat > $HOME/.gemrc <<EOT
install: --no-document --no-ri
update: --no-document --no-ri
EOT
fi

if [ "${WITH_RVM}" == "yes" ]; then
    source $RVM_HOME/scripts/rvm
fi

# Seed the cache.
if [ -f $HOME/hoot-gems.tar ]; then
    curl -sSL -o $HOME/hoot-gems.tar $RUBY_GEMS_URL/hoot-gems.tar
    # TODO: Extract into non-RVM cache directory.
    tar -C "$(rvm gemdir)/cache" -xf $HOME/hoot-gems.tar
fi

# gem installs are *very* slow, hence all the checks in place here to facilitate debugging
if ! gem list --local | grep -q mime-types; then
   gem install mime-types
fi

if ! gem list --local | grep -q cucumber; then
   gem install cucumber
fi

if ! gem list --local | grep -q capybara-webkit; then
   gem install capybara-webkit
fi

if ! gem list --local | grep -q selenium-webdriver; then
   gem install selenium-webdriver
fi

if ! gem list --local | grep -q rspec; then
   gem install rspec
fi

if ! gem list --local | grep -q capybara-screenshot; then
   gem install capybara-screenshot
fi

if ! gem list --local | grep -q selenium-cucumber; then
   gem install selenium-cucumber
fi
