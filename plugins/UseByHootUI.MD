# Hootenanny UI use of Translation and P2PMerge NodeJs Servers

___
## GET /version
Unused at this point, but would provide clients way to know what API version they are hitting
___
## GET /capabilities
Used to populate the available translation schemas in schema switcher
___
## POST /osmtotds
Used by schema switcher to translate an OSM feature to the selected translation schema
___
## GET /osmtotds
Used by schema switcher after an OSM feature is translated to retrieve schema entry for translated feature and populate list of all available attribute fields in English (as opposed to field codes) 
___
## GET /tdstoosm
Used when a new feature is created in a translated schema to translate an fcode value to a set of equivalent OSM tags
___
## POST /tdstoosm
Used when changes are made to feature fields or raw tags when using translated schema to convert tags back to OSM
___
## GET /schema
Used when searching for available translated schema feature types after creating a new feature
___
## GET /taginfo/keys/all
Used by raw tag editor to populate combobox values with available tag key codes in translated schema
___
## GET /taginfo/key/values
Used by raw tag editor to populate combobox values with available tag values in English (as opposed to field codes) for selected key
___
## POST /p2pmerge
Used during conflation review to merge two POIs
___
