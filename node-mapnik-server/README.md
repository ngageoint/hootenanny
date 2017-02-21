### Background
The node-mapnik-server is a Node.js app that uses node-mapnik to render tiles on-demand for Hoot datasets exported to PostGIS render db format.  It leverages sample code found in [node-mapnik-sample-code](https://github.com/mapnik/node-mapnik-sample-code).

### Datasource
The render databases are built for each Hoot dataset and follow the naming convention 'renderdb_*layerid*'.  A separate PostGIS enabled database is created for each Hoot dataset with the script found [here](https://github.com/ngageoint/hootenanny/blob/mapnik/scripts/services/exportrenderdb.sh).  The map dataset features are exported from the OSM nodes, ways, and relations in the hoot db to PostGIS layers using the following hoot command:

`hoot osm2ogr -D api.db.email=test@test.com translations/RenderDb.js "postgresql://hoot:hoottest@localhost:5432/hoot/example" "PG:dbname='renderdb_example' host='localhost' port='5432' user='hoot' password='hoottest'"`

The features in the render database are not updated after hoot dataset creation, but their purpose is only to provide a small-scale rendering of the data when the limit of loading editable vector data into the UI is reached.  These render databases are deleted when the Hoot dataset tables for the layername are deleted from the hoot api db.

### Service
The node-mapnik-server provides a service endpoint in the format:

'http://192.168.33.11:8000/?z=6&x=19&y=35&color=%23ff5599&name=PortAuPrinceOsmPoiRoadBuilding`

where `z, x, y` parameters follow the tile index from the TMS spec, the `name` parameter is used to identify the render database to access and the `color` is used to set the stroke and fill color for the mapnik style.

### Installing
```
cd node-mapnik-server
npm install
```
### Running
To run:
```
cd hoot
source SetupEnv.sh
source conf/database/DatabaseConfig.sh
cd node-mapnik-server
npm start
```

There is also an init.d script provided that can be modified and used to automate the start/stop of this service.

```
sudo cp init.d/node-mapnik-server /etc/init.d
sudo service node-mapnik-server [start|stop|status|restart]
# optionally start/stop on boot/shutdown
sudo update-rc.d node-mapnik-server defaults
# to remove
sudo update-rc.d -f node-mapnik-server remove
```

*Note:*  Adding the init.d script to rc.d will not work for Vagrant vms, due to the late mounting of the shared folder that contains the node-mapnik-server source code.  The Vagrantfile works around this by adding a 'run always' inline command to start the service.

### Mapnik Style
The mapnik style is generated from a template to replace entities that are not supported by mapnik.  Make edits to hoot_style_template.xml, then run this command:

`xmllint --noent hoot_style_template.xml > hoot_style.xml`
