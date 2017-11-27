#!/usr/bin/env bash
set -e
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

sudo cp $SCRIPT_HOME/RPM-GPG-KEY-Hoot /etc/pki/rpm-gpg
sudo chown root:root /etc/pki/rpm-gpg/RPM-GPG-KEY-Hoot
sudo chmod 0644 /etc/pki/rpm-gpg/RPM-GPG-KEY-Hoot

cat > /tmp/hoot.repo <<EOF
[hoot]
name = Hootenanny Enterprise Linux
baseurl = https://s3.amazonaws.com/hoot-repo/el7/\$basearch
enable = 1
gpgcheck = 1
repo_gpgcheck = 1
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-Hoot
EOF

sudo mv /tmp/hoot.repo /etc/yum.repos.d
sudo chown root:root /etc/yum.repos.d/hoot.repo
sudo chmod 0644 /etc/yum.repos.d/hoot.repo
