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

## Public vs Private Visibility
All services are now wrapped by authentication and many of the service endpoints have been migrated to be user aware.

>The plan is for new datasets to be private to the user by default.  Existing datasets owned by the dummy test user will be public.  We will add a method for making private datasets and their review bookmarks public.

* During implementation, it was decided that datasets would not have an inherent visibility.  Instead, folders have visibility and datasets inherit visibility from their containing folder.  The root folder is always public.  Data and folders that are public are read-only by non-owners with respect to modification (removing, moving, renaming, writing output to etc.)
* Public datasets can be clipped, conflated, and exported by non-owning users, but the destination output folder must be owned by the user.
* The data within a public dataset, accessed through the Hoot Map API, is editable by any authenticated user.  This allows multiple users to work through conflation reviews.
* There is a python [script](https://github.com/ngageoint/hootenanny/blob/develop/scripts/database/UpdateFolderOwnerVisibility.py) for migrating existing folders and maps from the dummy user to a real user (once that user has logged in).  
* Public folders and their contents are shaded blue.  Folders and sub-folders cannot mix visibility.  A change to one affects the entire tree.

:tada: Thanks to great work by @sumarlidason and @jbronn
