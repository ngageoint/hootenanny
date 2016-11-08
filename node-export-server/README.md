### Background
The node export server is a node js service that provides a web interface to leverage Hootenanny to export by bounding box, data from a number of datasources to several supported combinations of translated tag schema and file format.

### Installation
This service depends on the Hootenanny environment being configured, so

    cd hoot
    source SetupEnv.sh
should be run before starting the server.
The node export server is built with Node.js.  Node and the node package manager are required.  To install dependencies, run

    cd node-export-server
    npm install
To start the service run

    npm start

There is also an init.d script provided that can be used to automate the start/stop of this service.

```
sudo cp init.d/node-export-server /etc/init.d
sudo service node-export-server [start|stop|status|restart]
# optionally start/stop on boot/shutdown
sudo update-rc.d node-export-server defaults
# to remove
sudo update-rc.d -f node-export-server remove
```

*Note:*  Adding the init.d script to rc.d will not work for Vagrant vms, due to the late mounting of the shared folder that contains the node-mapnik-server source code.  The Vagrantfile works around this by adding a 'run always' inline command to start the service.

### Configuration
The service uses a json configuration file, `config.json`.  Any valid Hootenanny input source string (*i.e.* OGR datasource) can be configured as an export datasource.  Initally this was a PostGIS render db generated with `osm2pgsql` (*Note: this is the only datasource that will export with the schema/format combination of OSM/Shapefile and OSM/FileGeodatabase.*), but now includes connections to osm api db and osm api web service.  OSM and PBF files could be used, but I do not believe the hoot convert.bounding.box param works with them.  In the future the overpass api service could be added.

```
{
  "datasources": {
    "RenderDb": {
      "conn": "PG:dbname='osmsyria' host='192.168.33.12' port='5432' user='vagrant' password=''"
    },
    "API": {
      "conn": "http://192.168.33.12:3000/api/0.6/map"
    },
    "APIDB": {
      "conn": "osmapidb://vagrant:password@192.168.33.12:5432/openstreetmap"
    }
  },
  "formats": {
    "OSM XML": ".osm",
    "Shapefile": ".shp",
    "File Geodatabase": ".gdb"
  },
  "schemas": {
    "OSM": "translations/RenderDb
    "TDSv61": "translations/TDSv61.js",
    "MGCP": "translations/MGCP_TRD4.js"
  },
  "settings": {
    "cleanupDelay": 30000,
    "port": 8101
  }
}
```

The formats and schemas available are dependent on the version of Hootenanny installed (*and should only be edited by Hoot developers*), but are current as of Hootenanny release 0.2.19.

This is the table of supported schema/format combinations:

|      | Shapefile | File Geodatabase | OSM XML |
| ---- |:---------:|:----------------:|:-------:|
| **OSM**| &#x2713;*| &#x2713;*| &#x2713;|
| **TDSv40**| &#x2713;| &#x2713;| &#x2713;|
| **TDSv61**| &#x2713;| &#x2713;| &#x2713;|
| **MGCP**| &#x2713;| &#x2713;| &#x2713;|
*`osm2pgsql` PostGIS render database only

The remaining setting, `cleanupDelay`, is how long the exported files should remain on the server after initial download.  The default is 30 seconds, but can be configured to be longer, say if the same url is sent to someone else so they can download the export too.

All export files are removed when the node-export-server process is shutdown.

### Service
The node-export-server provides a service with the following API:

___
## GET /options

### URL
http://localhost:8108/options

### Response
```
{
  "Datasource": [
    "RenderDb",
    "API",
    "APIDB"
  ],
  "Schema": [
    "OSM",
    "TDSv40",
    "TDSv61",
    "MGCP"
  ],
  "Format": [
    "OSM XML",
    "Shapefile",
    "File Geodatabase"
  ]
}
```
___

## GET /export

### URL
http://host:port/export/[datasource]/[schema]/[format]?bbox=[minx,miny,maxx,maxy]

e.g. http://localhost:8101/export/API/MGCP/File%20Geodatabase?bbox=-69.43883,19.19755,-69.42268,19.20439
### Parameters
___
datasource | The datasource from which to export.
--- | ---
_required_ | **Example: API**
___

schema | The translation tag schema to use.
--- | ---
_required_ | **Example: MGCP**
___

format | The output file format.
--- | ---
_required_ | **Example: File%20Geodatabase**
___

bbox | The geographic bounds to export.
--- | ---
_required_ | **Example: -69.43883,19.19755,-69.42268,19.20439**
___

### Response
A zip file containing the exported output.
___
