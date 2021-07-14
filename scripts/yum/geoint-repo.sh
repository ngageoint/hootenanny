#!/bin/bash
# Copyright (C) 2021 Maxar Technologies (https://www.maxar.com)
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

GEOINT_DEPS_CHANNEL="${GEOINT_DEPS_CHANNEL:-stable}"
GEOINT_DEPS_BASEURL="${GEOINT_DEPS_BASEURL:-https://geoint-deps.s3.amazonaws.com/el7/${GEOINT_DEPS_CHANNEL}}"
GEOINT_DEPS_KEY="${GEOINT_DEPS_KEY:-/etc/pki/rpm-gpg/RPM-GPG-KEY-GEOINT}"
GEOINT_DEPS_REPO="${GEOINT_DEPS_REPO:-/etc/yum.repos.d/geoint-deps.repo}"

cat > "${GEOINT_DEPS_KEY}" <<EOF
-----BEGIN PGP PUBLIC KEY BLOCK-----

mQINBGBBFwcBEADg0shAu374kNPaui8s3aS9qPnW6KPuyj1a73Ok6Xox75AVrE31
fRrD0dILowJNtjq4ml3Y71GkvQS7tRuwYS+hvgN2Ex8iKALsKA1FQ6/EgnWi8dFV
CqmwLfI8D9FPhcvzrx5fv6kFM3zzdr6arSTj2FZWoSSye4i5raQLhw1J/r39d6yC
ohdmnQ5b6NWPJCLTrhKD0p47hYVP7R+cDIsZFAUm4IugiLagvobTZ/2RETgTXkSr
OlUqxsOnqyis5yBTwhlXDtFwF0lEBcH66V9lgjygVruSL0/gh9yGvl7p8+CtrJos
sIA74F2J6OUY79Im33EnIXKpLchkDuPRS5aB0LIqN3iwwCN74c3n9J0FLzAojbun
8jq61vRocLecePxonHYVLBUBT49JaUZtei+NwjYAYuSO9D13KZnfJO8uRGJmmsgj
hzWcK9DPeEkYDZxRWgniEVjtJlwbjNODBH+LC+J8XfmiDmaHwIdTK80o9VPY8Qwn
YcJHmejQWXeFmxVu1eD/egWetMHD198qRIgOYcmsd7ZSLzykQPbr+YVqi1Hv2zpf
4TjiRtYkXFFZnxHQIsFcqW6WU0xsq/3+cFMcdGiqlBcQ2k9aknBfAu0q763lSTWJ
DE0pH9T93qO42PmonRacuqJBTXPYmLM8wIFO+lWDej9HFome/Zg1c/ZIrQARAQAB
tEFGb3VuZGF0aW9uR0VPSU5UIFBhY2thZ2luZyA8Zm91bmRhdGlvbmdlb2ludC1w
YWNrYWdpbmdAbWF4YXIuY29tPokCNQQTAQoAHwUCYEEXBwIbAwQLCQgHBBUKCQgF
FgIDAQACHgECF4AACgkQwINrjOIZXyM+cxAAyAMaMAcu8Vrv1zRqQho2EMGlwIUb
sJZruF5qcE49qAuILwBxBd69k7UFI4bhVggPDWXUadm93eUu31C3FgNRuCn4upML
QBhiOQ62hVs8YDTyo3s4ktiOQ6U4Os6FVcEUVBekmQ1lLNwXO4zdcnBJAuQPoNxS
HpSZcsMx75aCsmpVgJIc3e3ttQwcRyh7fJ5KzaEY45nZMjNurauKKVfqJm/cGVSS
ovRfnqkg3A9DmkwgZ6Ro7eEDSMLHZuIyI8EaiCaXqFvU+wP3iMrxzo6C7+6e868m
MhPqT9nv+c0V0Cm/6LPLvcNHS0yAOUxv3gFa0NYprQVuJB2MunvyZnWHkweCci9P
lknFqP7h+E5uh5InA6voZVzm6WpagkUzItAb0ikogm/J/0HTsjtc/pcRLJUa4pA+
FHyCyfENPUPZkuvJZ3y1yr7u+OPc+ycy1MhM3lbQedUaIleTuwe69LChs3sKLs2c
VRFbuhjoWRUrvccpHGuYdWDL+FzLghB+q85VDX+zSQoqAga3M0b7l0zpWblV67Of
GxmI3xI8uxQW0+ByqZ0gAYJHEkzXXUm9vYg6Hjn1hWhECgvMc3shpI4H1HAsG1Eg
BKvRcPtz0vU2ZA+JjgCXqpu0sLpMY/CGlgn9m4XlZYQ8ok/fe1no7k6cJat1EHLH
0WxnS20NH2GMJss=
=9QoT
-----END PGP PUBLIC KEY BLOCK-----
EOF

cat > "${GEOINT_DEPS_REPO}" <<EOF
[geoint-deps]
name = GEOINT Dependencies
baseurl = ${GEOINT_DEPS_BASEURL}
enable = 1
gpgcheck = 1
gpgkey=file://${GEOINT_DEPS_KEY}
repo_gpgcheck = 1
EOF
