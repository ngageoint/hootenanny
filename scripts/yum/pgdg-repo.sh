#!/bin/bash
# Copyright (C) 2019 Radiant Solutions (http://www.radiantsolutions.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
set -euo pipefail

PG_VERSION="$1"
PG_DOTLESS="$(echo "$PG_VERSION" | awk '{ gsub(/\./, ""); print substr($0, 1, 2) }')"
PG_MAJOR_VERSION="$(echo "$PG_VERSION" | awk -F. '{ if ($1 >= 10) print $1; else print $0 }')"
PGDG_KEY="${PGDG_KEY:-/etc/pki/rpm-gpg/RPM-GPG-KEY-PGDG-$PG_DOTLESS}"
PGDG_REPO="${PGDG_REPO:-/etc/yum.repos.d/pgdg-$PG_DOTLESS-centos.repo}"

# Clean up any PGDG repository RPM.
if rpm -q pgdg-redhat-repo &> /dev/null; then
    yum remove -y -q pgdg-redhat-repo
fi

# Clean up existing repo files and keys, in case we're
# upgrading versions.
find /etc/pki/rpm-gpg -type f -name RPM-GPG-KEY-PGDG-\* -delete
find /etc/yum.repos.d -type f -name pgdg-\*-centos.repo -delete

cat > "${PGDG_KEY}" <<EOF
-----BEGIN PGP PUBLIC KEY BLOCK-----
Version: GnuPG v2.0.22 (GNU/Linux)

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
=fPP0
-----END PGP PUBLIC KEY BLOCK-----
EOF

cat > "$PGDG_REPO" <<EOF
[pgdg$PG_DOTLESS]
name=PostgreSQL $PG_MAJOR_VERSION \$releasever - \$basearch
baseurl=https://download.postgresql.org/pub/repos/yum/$PG_MAJOR_VERSION/redhat/rhel-\$releasever-\$basearch
enabled=1
exclude=CGAL* geos* gdal* ogdi* ogr* osm* postgis* proj* SFCGAL*
gpgcheck=1
gpgkey=file://$PGDG_KEY

[pgdg$PG_DOTLESS-source]
name=PostgreSQL $PG_MAJOR_VERSION \$releasever - \$basearch - Source
failovermethod=priority
baseurl=https://download.postgresql.org/pub/repos/yum/srpms/$PG_MAJOR_VERSION/redhat/rhel-\$releasever-\$basearch
enabled=0
gpgcheck=1
gpgkey=file://$PGDG_KEY
EOF
