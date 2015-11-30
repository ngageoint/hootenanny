The node-mapnik-server is a Node.js app that uses node-mapnik to render tiles on-demand for Hoot datasets in PostGIS render db format.  It leverages sample code found in [node-mapnik-sample-code](https://github.com/mapnik/node-mapnik-sample-code).

The render databases are built for each Hoot dataset and follow the naming convention 'renderdb_*layername*'.  A separate PostGIS enabled database is created for each Hoot dataset with the script found [here](https://github.com/ngageoint/hootenanny/blob/mapnik/scripts/exportrenderdb.sh).  The map dataset features are exported from the OSM nodes, ways, and relations in the hoot db to PostGIS layers using the following hoot command:

`hoot osm2ogr -D services.db.reader.email=test@test.com translations/RenderDb_to_ESRI.js "postgresql://hoot:hoottest@localhost:5432/hoot/example" "PG:dbname='renderdb_example' host='localhost' port='5432' user='hoot' password='hoottest'"`

The features in the render database are not updated after hoot dataset creation, but their purpose is only to provide a small-scale rendering of the data when the limit of loading editiable vector data into the UI is reached.  These render databases are deleted when the Hoot dataset tables for the layername are deleted from the hoot api db.


The node-mapnik-server provides a service endpoint in the format:

'http://192.168.33.11:8000/?z=6&x=19&y=35&color=%23ff5599&name=PortAuPrinceOsmPoiRoadBuilding`

where `z, x, y` parameters follow the tile index from the TMS spec, the `name` parameter is used to identify the render database to access and the `color` is used to set stroke and fill color for the mapnik style.

To run:
```
cd hoot
source SetupEnv.sh
source conf/DatabaseConfig.sh
cd node-mapnik-server
npm start
```
