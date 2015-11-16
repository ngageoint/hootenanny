#!/usr/bin/env node

var mapnik = require('mapnik')
  , mercator = require('./utils/sphericalmercator.js')
  , mappool = require('./utils/pool.js')
  , http = require('http')
  , url = require('url');

// register shapefile plugin
if (mapnik.register_default_input_plugins) mapnik.register_default_input_plugins();

var TMS_SCHEME = true;

// create a pool of 5 maps to manage concurrency under load
var maps = mappool.create_pool(20);

var usage = 'usage: app.js <port>\ndemo:  app.js 8000';

var prj = new mapnik.Projection(mercator.proj4);

// map with just a style
// eventually the api will support adding styles in javascript
var s = '<Map background-color="#00000000" srs="' + mercator.proj4 + '">';
//var s = '<Map background-color="#00000000" srs="+proj=latlong +datum=WGS84">';
s += '    <Style name="hoot">';
s += '        <Rule>';
s += '            <Filter>[building] &lt;&gt; \'\' or [name] &lt;&gt; \'\'</Filter>';
s += '            <LineSymbolizer stroke="${COLOR}" stroke-width="2" stroke-linejoin="miter"';
s += '                stroke-linecap="square" />';
s += '            <PolygonSymbolizer fill="${COLOR}" fill-opacity="0.3" />';
s += '        </Rule>';
s += '        <Rule>';
s += '            <Filter>[highway] &lt;&gt; \'\' or [name] &lt;&gt; \'\'</Filter>';
s += '            <LineSymbolizer stroke="${COLOR}" stroke-width="2" stroke-linejoin="round"';
s += '                stroke-linecap="round" />';
s += '        </Rule>';
s += '        <Rule>';
s += '            <Filter>[amenity] &lt;&gt; \'\' or [name] &lt;&gt; \'\'</Filter>';
s += '            <MarkersSymbolizer file="poi.svg" fill="${COLOR}" transform="translate(8, 23)" />';
s += '        </Rule>';
s += '    </Style>';
s += '</Map>';

var port = process.argv[3];

if (!port) {
   console.log(usage);
   process.exit(1);
}

var aquire = function(id,options,callback) {
    methods = {
        create: function(cb) {
                var obj = new mapnik.Map(options.width || 256, options.height || 256);
                obj.fromString(id, {strict: true},function(err,obj) {
                    if (options.bufferSize) {
                        obj.bufferSize = options.bufferSize;
                    }
                    cb(err,obj);
                });
            },
            destroy: function(obj) {
                delete obj;
            }
    };
    maps.acquire(id,methods,function(err,obj) {
      callback(err, obj);
    });
};

// var client = require('http').createClient(8080, 'localhost');

// function search(mapid, bbox, callback) {
//     var headers = {
//         'Content-Length': el_query.length,
//         'charset': 'UTF-8',
//         'Content-Type': 'application/xml'
//     };
//     var request = client.request('GET', '/hoot-services/osm/api/0.6/map?mapId=', headers);
//     request.write(el_query, 'utf8');
//     request.on('response', function(response) {
//         var body = '';
//         response.on('data', function(data) {
//             body += data;
//         });
//         response.on('end', function() {
//             callback(JSON.parse(body));
//         });
//     });
//     request.end();
// }

http.createServer(function(req, res) {

            var query = url.parse(req.url.toLowerCase(), true).query;
            var stylesheet = s.replace(/\${COLOR}/g, query.color || 'rgb(255, 85, 153)');
            aquire(stylesheet, {}, function(err, map) {
                if (err) {
                    process.nextTick(function() {
                        maps.release(stylesheet, map);
                    });
                    res.writeHead(500, {
                      'Content-Type': 'text/plain'
                    });
                    res.end(err.message);
                } else {
                    var map = new mapnik.Map(256, 256);
                    map.fromStringSync(stylesheet);
                    // bbox for x,y,z
                    var bbox = mercator.xyz_to_envelope(parseInt(query.x), parseInt(query.y), parseInt(query.z), TMS_SCHEME);
                    //map.extent = bbox;
                    map.extent = prj.forward(bbox);
                    var im = new mapnik.Image(map.width, map.height);

// construct a mapnik layer dynamically
var l = new mapnik.Layer('hoot');
//l.srs = map.srs;
l.srs = '+init=epsg:4326';
//l.srs = '+proj=latlong +datum=WGS84';
l.styles = ['hoot'];

var ds = new mapnik.Datasource({
    type: 'osm',
    //url: 'http://localhost:8080/hoot-services/osm/api/0.6/map?mapId=' + query.mapid,
    //bbox: bbox,
    file: 'PortAuPrinceOsmPoiRoadBuilding.osm'
});

// add our custom datasource
l.datasource = ds;

// add this layer to the map
map.add_layer(l);

                    map.render(im, function(err, im) {
                        process.nextTick(function() {
                            maps.release(stylesheet, map);
                        });
                        if (err) {
                            res.writeHead(500, {
                              'Content-Type': 'text/plain'
                            });
                            res.end(err.message);
                        } else {
                            res.writeHead(200, {
                              'Content-Type': 'image/png'
                            });
                            res.end(im.encodeSync('png'));
                        }
                    });
                 }
             });

}).listen(port);


console.log('Test server listening on port %d', port);