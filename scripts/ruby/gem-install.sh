#!/usr/bin/env bash
set -e

# Don't install documentation for gems
if [ ! -f $HOME/.gemrc ]; then
    cat > $HOME/.gemrc <<EOT
install: --no-document
update: --no-document
EOT
fi

if [ "${WITH_RVM}" == "yes" ]; then
    source $RVM_HOME/scripts/rvm
    GEM_CACHE="$(rvm gemdir)/cache"
else
    GEM_CACHE="${HOME}/.gem/ruby/cache"
fi

# Seed the cache.
if [ -f $HOME/hoot-gems.tar ]; then
    # TODO: GPG verify the gem cache tarball.
    curl -sSL -o $HOME/hoot-gems.tar $RUBY_GEMS_URL/hoot-gems.tar
    tar -C $GEM_CACHE -xf $HOME/hoot-gems.tar
fi

# Install gems.
for hoot_gem in mime-types cucumber capybara-webkit selenium-webdriver rspec capybara-screenshot selenium-cucumber
do
    if ! gem list --local | grep -q $hoot_gem; then
        gem install $hoot_gem
    fi
done
