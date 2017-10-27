#!/usr/bin/env bash
set -e
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
RVM_HOME="${RVM_HOME:-${HOME}/.rvm}"

if ! ruby -v | grep -q $RUBY_VERSION; then
    # RVM signing keys obtained via following command:
    #
    #  gpg --keyserver hkp://keyserver.ubuntu.com \
    #      --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3 \
    #                  7D2BAF1CF37B13E2069D6956105BD0E739499BDB
    #
    # And exported to following:
    #
    #  gpg --output rvm-signing.gpg \
    #      --export 409B6B1796C275462A1703113804BB82D39DC0E3 \
    #               7D2BAF1CF37B13E2069D6956105BD0E739499BDB
    #
    if ! (gpg --list-keys --with-colon | grep -q 3804BB82D39DC0E3 && gpg --list-keys --with-colon | grep -q 105BD0E739499BDB); then
        cat $SCRIPT_HOME/rvm-signing.gpg | gpg --import --quiet
    fi

    mkdir -p $RVM_HOME/archives
    if [ ! -f $RVM_HOME/archives/rvm-installer.asc ]; then
        # Download RVM installation script signature.
        curl -sSL \
             -o $RVM_HOME/archives/rvm-installer.asc \
             https://raw.githubusercontent.com/rvm/rvm/master/binscripts/rvm-installer.asc
    fi

    if [ ! -f $RVM_HOME/archives/rvm-installer ]; then
        # Download RVM installation script.
        curl -sSL \
             -o $RVM_HOME/archives/rvm-installer-unverified \
             https://raw.githubusercontent.com/rvm/rvm/master/binscripts/rvm-installer
        # Verify that the script comes from the proper source.
        gpg --verify --quiet \
            $RVM_HOME/archives/rvm-installer.asc \
            $RVM_HOME/archives/rvm-installer-unverified
        mv $RVM_HOME/archives/rvm-installer-unverified \
           $RVM_HOME/archives/rvm-installer
        chmod +x $RVM_HOME/archives/rvm-installer
        echo 'Downloaded and verified RVM Installer!'
    fi

    if [ ! -f $RVM_HOME/scripts/rvm ]; then
        # Install RVM.
        $RVM_HOME/archives/rvm-installer stable

        # Point to our pre-built binary packages, add checksums for known versions.
        mkdir -p $RVM_HOME/user
        cat > $RVM_HOME/user/db <<EOF
rvm_remote_server_url=${RUBY_BINARIES_URL}
EOF
        cat > $RVM_HOME/user/sha512 <<EOF
${RUBY_BINARIES_URL}/centos/7/x86_64/ruby-2.3.4.tar.bz2=b64a05e81b0fc121e8be515a894492ca5ee340ad71a9be63518fea5f4c10fe087e2d2c800882543f09409783d484d09bf99697ddc98c4a5658b9db415696b7d5
${RUBY_BINARIES_URL}/centos/7/x86_64/ruby-2.4.2.tar.bz2=685652d6afcb99647e5a453816d87c157c778e9dedc3a3f883f0a88860261db69804e92bb2250341cc11f1668d93f7f1348f1e881bf13b2cef3b4fe16b99e32d
${RUBY_BINARIES_URL}/ubuntu/14.04/x86_64/ruby-2.3.4.tar.bz2=a1d2335b20e619735775cc3a33d003c5859b87de1ecb86bece9366e42b524e437b8489f32f6842f514006784efcb70504af2a35db5559a5d9c64a27f31be3bf7
${RUBY_BINARIES_URL}/ubuntu/14.04/x86_64/ruby-2.4.2.tar.bz2=d25ca12cd29945bf10435792bd791bc53295673cd8a1923cfaa611923e36a8a37cebd5f92de05570b95e22702556ef4b690d7147e2f5f48770b6845b784b72ae
EOF
        echo 'Installed RVM!'
    fi
    source $RVM_HOME/scripts/rvm

    # Install desired Ruby version.
    rvm install ruby-$RUBY_VERSION --quiet-curl
    rvm use $RUBY_VERSION --default

    # Don't install documentation for gems
    cat > ~/.gemrc <<EOT
install: --no-document
update: --no-document
EOT
fi
