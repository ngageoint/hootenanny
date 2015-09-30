The MapEdit export server is a node js service that can be configured to provide the MapEdit OpenStreetMap website app
the capability to leverage Hootenanny to export data from any number of datasources to a number of vector schemas and file formats.

This service depends on the Hootenanny environment being configured, so

    cd hoot
    source SetupEnv.sh
should be run before starting the server.
The MapEdit export server is built with Node.js.  Node and the node package manager are required.  To install dependencies, run

    cd mapedit-export-server
    npm install
To start the service run

    npm start
The service uses a json configuration file, `config.json`.  Any valid Hootenanny input source string (*i.e.* OGR datasource) can be configured as an export datasource.  Most commonly this will be a PostGIS render db generated with `osm2pgsql`.  *Note: this is the only datasource that will export with the schema/format combination of OSM/Shapefile and OSM/FileGeodatabase.*

An optional `epsg` property has been added to datasource definitions to support export from render databases in the web mercator coordinate system (*epsg:3785*, the osm2pgsql default and *epsg:3857*, with the `--proj 3857` param).  In the future the datasource srid may be detected by hoot and the input bounding box projected as necessary, eliminating the need to specify this epsg datasource metadata.
```
{
  "datasources": {
    "OpenStreetMap": {
      "conn": "PG:dbname='osmsyria' host='192.168.33.12' port='5432' user='vagrant' password=''"
    },
    "3785": {
      "conn": "PG:dbname='osmsyria_3785' host='192.168.33.12' port='5432' user='vagrant' password=''",
      "epsg": "3785"
    },
    "MapEdit": {
      "conn": "PG:dbname='ogrsyria' host='192.168.33.12' port='5432' user='vagrant' password=''"
    },
    "XML": {
      "conn": "syriasample.osm"
    }
  },
  "formats": {
    "OSM XML": ".osm",
    "Shapefile": ".shp",
    "File Geodatabase": ".gdb"
  },
  "schemas": {
    "OSM": "translations/RenderDb_to_ESRI.js",
    "TDSv40": "translations/TDSv40.js",
    "TDSv61": "translations/TDSv61.js",
    "MGCP": "translations/MGCP_TRD4.js"
  },
  "settings": {
    "cleanupDelay": 30000
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

All export files are removed when the mapedit-export-server process is shutdown.
