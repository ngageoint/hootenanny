#!/usr/bin/env bash
set -e
RVM_HOME="${RVM_HOME:-${HOME}/.rvm}"

# TODO: Download a cache and extract it.
source $RVM_HOME/scripts/rvm

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
