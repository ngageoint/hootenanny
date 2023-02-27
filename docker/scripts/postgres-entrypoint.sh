#!/bin/bash
set -eu

update_postgres_setting () {
    local pg_setting="$1"
    local pg_value="$2"
    if grep -q -e "^${pg_setting}\s\+\?=" "${PGDATA}/postgresql.conf"; then
        # Modify existing setting.
        sed -i -e "s#^${pg_setting}\s\+\?=\s\+\?.\+#${pg_setting} = ${pg_value}#g" \
            "${PGDATA}/postgresql.conf"
    else
        # New setting.
        echo "${pg_setting} = ${pg_value}" >> "${PGDATA}/postgresql.conf"
    fi
}

# Customize any PostgreSQL settings from POSTGRES_SETTINGS environment.
if [ -n "${POSTGRES_SETTINGS:-}" ]; then
    read -d '#' -a POSTGRES_SETTINGS_ARR <<< "$POSTGRES_SETTINGS#"
    for pg_setting_pair in "${POSTGRES_SETTINGS_ARR[@]}"; do
        pg_setting="$(echo "${pg_setting_pair}" | awk -F= '{ print $1 }')"
        pg_value="$(echo "${pg_setting_pair}" | awk -F= '{ print $2 }')"
        update_postgres_setting "${pg_setting}" "${pg_value}"
    done
fi

# ensure first argument is `postgres`.
if [ "${1#-}" != "$1" ]; then
        set -- postgres "$@"
fi

exec "$@"
