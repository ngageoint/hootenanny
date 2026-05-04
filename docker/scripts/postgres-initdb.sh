#!/bin/bash
set -euo pipefail

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<'SQL'
CREATE EXTENSION IF NOT EXISTS hstore;
ALTER USER hoot WITH SUPERUSER;
ALTER SYSTEM SET listen_addresses = '*';
ALTER SYSTEM SET max_connections = '1000';
ALTER SYSTEM SET shared_buffers = '1024MB';
ALTER SYSTEM SET max_files_per_process = '1000';
ALTER SYSTEM SET work_mem = '16MB';
ALTER SYSTEM SET maintenance_work_mem = '256MB';
ALTER SYSTEM SET autovacuum = 'off';
SQL
