#!/bin/bash

# vars for /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/conf/hoot-services.conf
export OAUTH_REDIRECT_URL='oauthRedirectURL=https:\/\/testing.vgihub.geointservices.io\/hootenanny-nightly\/hootenanny-id\/'
export OAUTH_PROVIDER_URL='oauthProviderURL=http:\/\/10.0.1.176'


# vars for /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/security-applicationContext.xml
export APPCONTEXT_KEY='key=\"xYADAL3B3XKsUp7Fz8nP1OWvGXPIBXghda5KBs9c\"'
export DEFAULT_APPCONTEXT_KEY='key=\"4aiqf3gHIWVcKDu0V8Xx6h6I2QYfJ7ClCK8xhj2Z\"'
export APPCONTEXT_SECRET='secret=\"RmmCmp7qmj3kOkHAD7ZaZbUj4HwLOzuFof5MpH5Q\"'
export DEFAULT_APPCONTEXT_SECRET='secret=\"glOzMrdYh86WbLbMsPP6uXC4gWR4ccDrDdY5LBwj\"'
export APPCONTEXT_REQ_TOKEN_URL='request-token-url=\"http:\/\/10.0.1.176\/oauth\/request_token\"'
export APPCONTEXT_USER_TOKEN_URL='user-authorization-url=\"https:\/\/testing.vgihub.geointservices.io\/oauth\/authorize\"'
export APPCONTEXT_ACCESS_TOKEN_URL='access-token-url=\"http:\/\/10.0.1.176\/oauth\/access_token\"'


# vars for /var/lib/tomcat8/webapps/hootenanny-id-legacy/iD.js
export LEGACY_TRANSLATION_PORT='\"translationServerPort\":\"hootenanny-nightly\/switcher\"'
export LEGACY_ELEMENTMERGE_PORT='\"elementMergeServerPort\":\"hootenanny-nightly\/p2p"'


# vars for /var/lib/tomcat8/webapps/hootenanny-id/iD.min.js 
export TRANSLATION_PORT='\"translationServerPort\":\"hootenanny-nightly\/switcher\"'
export MERGE_PORT='\"mergeServerPort\":\"hootenanny-nightly\/p2p\"'


# vars for /var/lib/tomcat8/webapps/hoot-services/WEB-INF/classes/db/db.properties
export RAILSPORT_HOST='RAILSPORT_HOST = 10.0.1.176'
export RAILSPORT_PORT='RAILSPORT_PORT = XXX'
