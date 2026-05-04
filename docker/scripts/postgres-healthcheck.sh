#!/bin/sh
set -eu
pg_isready -h "${PGHOST:-127.0.0.1}" -U "${POSTGRES_USER:-postgres}"
