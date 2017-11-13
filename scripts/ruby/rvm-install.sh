#!/usr/bin/env bash
set -e
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

RVM_INSTALLER_DIR="${RVM_INSTALLER_DIR:-${HOME}/rvm-installer}"
RVM_TAR="rvm-${RVM_VERSION}.tar.gz"
RVM_TAR_URL="${RVM_TAR_URL:-${RVM_BASE_URL}/archive/${RVM_VERSION}.tar.gz}"
RVM_SIG_URL="${RVM_SIG_URL:-${RVM_BASE_URL}/releases/download/${RVM_VERSION}/${RVM_VERSION}.tar.gz.asc}"

# Assume that Ruby is installed with packages instead.
if [ "${WITH_RVM}" != "yes" ]; then
    exit 0
fi

if ! $RVM_HOME/bin/rvm --version | grep -q "^rvm ${RVM_VERSION//\./\\.}"; then
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

    # Setting the home directory the place to look for preseed archives.
    if [ ! -f $HOME/.rvmrc ]; then
        echo "rvm_archives_path=${HOME}" >> ~/.rvmrc
    fi

    # Download RVM installation tarball signature.
    if [ ! -f $HOME/$RVM_TAR.asc ]; then
        curl -sSL -o $HOME/$RVM_TAR.asc $RVM_SIG_URL
    fi

    if [ ! -f $HOME/$RVM_TAR ]; then
        # Download to temporary location, move into place when GPG verified.
        curl -sSL -o /tmp/$RVM_TAR $RVM_TAR_URL
        gpg --verify --quiet $HOME/$RVM_TAR.asc /tmp/$RVM_TAR
        mv /tmp/$RVM_TAR $HOME/$RVM_TAR

        # Extract RVM installer tarball.
        mkdir -p $RVM_INSTALLER_DIR
        tar -C $RVM_INSTALLER_DIR --strip-components=1 -xzf $HOME/$RVM_TAR
        echo 'Downloaded, verified, and extracted RVM Installer!'
    fi

    # Install RVM.
    cd $RVM_INSTALLER_DIR
    ./install --auto-dotfiles
    cd $HOME

    # Point to our pre-built binary packages.
    mkdir -p $RVM_HOME/user
    cat > $RVM_HOME/user/db <<EOF
rvm_remote_server_url=${RVM_BINARIES_URL}
EOF
    # SHA-512 checksums for the binary releases.
    cat > $RVM_HOME/user/sha512 <<EOF
${RVM_BINARIES_URL}/centos/7/x86_64/ruby-2.3.4.tar.bz2=a84cf1788a35d0be80cee161cc91015052c8bc7b4a8c5883b5ec47c39902be42aedd907a52c11c5bb929b4b37ac14ea4ae26dcd9fb48f9606c85c8af2824bd4a
${RVM_BINARIES_URL}/centos/7/x86_64/ruby-2.4.2.tar.bz2=f1dba0895b13e363cd8065b09df56acbd403c15fcc20bf896c826b77c25042cad3e1c4419bcb69f8fb926712830f2066a1d46ac484f994f1d4bdddec068ed925
${RVM_BINARIES_URL}/ubuntu/14.04/x86_64/ruby-2.3.4.tar.bz2=ae2a7f880cf07778714cd63d79bdd6a5c420bd391af75e381656b9ae9251f3db41c11f6ab3a0ea07f7dcad3f43849e37f54c964c4b442f4cb2ad5dd5e8e7e651
${RVM_BINARIES_URL}/ubuntu/14.04/x86_64/ruby-2.4.2.tar.bz2=a5ab53e7f4f94f9296a70580e938287ccd1f72a67552359522e8fb005902b816f88840ec3bc5ccc95bc70e09b3bc9ac8dd5fdca1ecfa4ace09a3b38d02f11af7
${RVM_BINARIES_URL}/ubuntu/16.04/x86_64/ruby-2.3.4.tar.bz2=d4b9064c8e9039c1c5b9fc38969493a12030948c702a5d4f2ffaba8d0ac1fef4162c7a96810ccb89489621f31ee17a03a3f4fc5968a2ac712ffc371737592c02
${RVM_BINARIES_URL}/ubuntu/16.04/x86_64/ruby-2.4.2.tar.bz2=30c37483456405c34eb5f4824451fc5b93c4bf172df2bc48d68834bb4355f2be11f2619861a31fec7a5c12a0ef716d868175c6d6a114bdcfa03250eb460cd0de
EOF
    echo 'Installed RVM!'
fi

# Setup environment to use RVM.
source $RVM_HOME/scripts/rvm

# Install desired Ruby version.
if ! ruby -v | grep -q "${RUBY_VERSION//\./\\.}"; then
    rvm install ruby-$RUBY_VERSION --quiet-curl
    rvm use $RUBY_VERSION --default
fi
