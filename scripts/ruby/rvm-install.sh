#!/usr/bin/env bash
set -e
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

RVM_INSTALLER_DIR="${RVM_INSTALLER_DIR:-${HOME}/rvm-installer}"
RVM_TAR="rvm-${RVM_VERSION}.tar.gz"
RVM_TAR_URL="${RVM_TAR_URL:-${RVM_BASE_URL}/archive/${RVM_VERSION}.tar.gz}"
RVM_SIG_URL="${RVM_SIG_URL:-${RVM_BASE_URL}/releases/download/${RVM_VERSION}/${RVM_VERSION}.tar.gz.asc}"

if [ ! -f $RVM_HOME/bin/rvm ] || ! $RVM_HOME/bin/rvm --version | grep -q "^rvm ${RVM_VERSION//\./\\.}"; then
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

    echo 'Installed RVM!'
fi

# Setup environment to use RVM.
source $RVM_HOME/scripts/rvm

# Install desired Ruby version.
if ! hash ruby >& /dev/null || ! ruby -v | grep -q "${RUBY_VERSION_HOOT//\./\\.}"; then

    # SHA-512 checksums for the binary releases; always update when installing
    # new rubies as it may have been updated.
    cat > $RVM_HOME/user/sha512 <<EOF
${RVM_BINARIES_URL}/centos/7/x86_64/ruby-2.3.4.tar.bz2=a84cf1788a35d0be80cee161cc91015052c8bc7b4a8c5883b5ec47c39902be42aedd907a52c11c5bb929b4b37ac14ea4ae26dcd9fb48f9606c85c8af2824bd4a
${RVM_BINARIES_URL}/centos/7/x86_64/ruby-2.3.6.tar.bz2=0292d05b72894266e7884e0d7513529cce66c49c889775252b2c949f3154a9701893d8fd2a46a53f33602e535705ba083b628bdbf39a8476745d0e943e50f03a
${RVM_BINARIES_URL}/centos/7/x86_64/ruby-2.4.3.tar.bz2=1eba19ffb3693365baa0a71760ee3610e41c1ad913dbc9fa85f787a1926bc6be9343ccdecddd2c6edfef241c231c78de990e32ef3c67cb61d9fb67f7d0ee330b
${RVM_BINARIES_URL}/centos/7/x86_64/ruby-2.5.0.tar.bz2=69c4faa0a4ede05ba89ecc76564e2b334d95b4aa6a78accf402bfec34967262f42500a81bcfd18247b12ed03d606daee6dd25350998f1c655552750ab9ea940e
${RVM_BINARIES_URL}/ubuntu/16.04/x86_64/ruby-2.3.4.tar.bz2=d4b9064c8e9039c1c5b9fc38969493a12030948c702a5d4f2ffaba8d0ac1fef4162c7a96810ccb89489621f31ee17a03a3f4fc5968a2ac712ffc371737592c02
${RVM_BINARIES_URL}/ubuntu/16.04/x86_64/ruby-2.3.6.tar.bz2=4b895e2c2e391266f07778a17d7e238996152d7567d5609f015e464a46c77eb5b02b36fd1f663df12d20da3af733e2239d8cf19084c77ccccf686c1d8ab16033
${RVM_BINARIES_URL}/ubuntu/16.04/x86_64/ruby-2.4.3.tar.bz2=abf8430d6405a6431d830516665880d350c76cc86e7b9c266a23de58e719e2240d4ce90616194fd58a2d2604b28792476e4a54794c7c0875853a8e5c11f10044
${RVM_BINARIES_URL}/ubuntu/16.04/x86_64/ruby-2.5.0.tar.bz2=9343d2a1e7b8786dfcf9335079dd444754dd51cd025357b754613cbc8337ce676d289cd1e10a11609186f29335ac8efa7fd47bf612d778a57773f038efe6a3e2
EOF

    rvm install ruby-$RUBY_VERSION_HOOT --quiet-curl
    rvm use $RUBY_VERSION_HOOT --default
fi
