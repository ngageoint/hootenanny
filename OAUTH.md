## OAuth Configuration

Hoot services are now authenticated with OAuth1 which adds a config element for hoot-services.conf that will need to be tweaked per environment.

This file is found in the hoot-services.war file under the path `WEB-INF/classes/conf/hoot-services.conf` with the pertinent section below.

```
# oAuth Redirect URL
# [!] Externally facing URL [!]
# This will be used to redirect the user back to the site;
# and it should land the user at index.html to complete the oAuth process.
oauthRedirectURL=http://localhost:8080/
```

The default value works for the UI running separately with node.
The proper value for the UI deployed by tomcat on vagrant is `http://localhost:8888/hootenanny-id`.

There is also a chunk of config to change the OAuth provider from public openstreetmap to another private instance.
This will require registering a new client application with the rails app and then updating this section in hoot-services.conf, as well as the key, secret and url values in referenced `WEB-INF/classes/security-applicationContext.xml` file.

```
# oAuth Provider
# Also see hoot-services/src/main/resources/security-applicationContext.xml
oauthProviderURL=https://api.openstreetmap.org
```

While all services are now wrapped by authentication, none of the service endpoints have yet been migrated to be user aware.
The plan is for new datasets to be private to the user by default.  Existing datasets owned by the dummy test user will be public.
We will add a method for making private datasets and their review bookmarks public.

:tada: Thanks to great work by @sumarlidason and @jbronn
