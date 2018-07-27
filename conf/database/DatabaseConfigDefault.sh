#hoot services variables

export DB_NAME=hoot
export DB_USER=hoot
export DB_PASSWORD=hoottest
export DB_HOST=localhost
export DB_PORT=5432

export WFS_DB_NAME=wfsstoredb

export DB_NAME_OSMAPI=osmapi_test
export DB_USER_OSMAPI=hoot
export DB_PASSWORD_OSMAPI=hoottest
export DB_HOST_OSMAPI=localhost
export DB_PORT_OSMAPI=5432

# The base URL for pulling raw OSM data
export OSM_API_URL=https://api.openstreetmap.org/api/0.6

# Setup access to an OSM API Rails Port.
# Override this in the Local config
export RAILS_USER=hoot
export RAILS_PASSWORD=hoottest
export RAILS_HOST=localhost
export RAILS_PORT=3000
