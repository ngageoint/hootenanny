#hoot services variables

export DB_NAME=hoot
export DB_USER=hoot
export DB_PASSWORD=hoottest
export DB_HOST=localhost
export DB_PORT=5432

export DB_NAME_OSMAPI=osmapi_test
export DB_USER_OSMAPI=hoot
export DB_PASSWORD_OSMAPI=hoottest
export DB_HOST_OSMAPI=localhost
export DB_PORT_OSMAPI=5432

# The base URL for pulling raw OSM data
# export OSM_API_URL=https://api.openstreetmap.org/api/0.6
export OSM_API_URL=https://api.openstreetmap.org
export OSM_API_VERSION=0.6

# The main internet Overpas URL
export OVERPASS_API_URL=https://overpass-api.de/api/interpreter

# Setup access to an OSM API Rails Port.
# Override this in the Local config
# NOTE: Put "XXX" for port if you dont need to specify one
export RAILS_PROTOCOL=http
export RAILS_HOST=localhost
export RAILS_PORT=3000
export RAILS_API=/api/0.6/map
