#!/bin/sh
set -eu
/usr/bin/pg_isready -h "${PGHOST:-0.0.0.0}"
