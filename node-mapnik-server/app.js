#!/usr/bin/env node

var mapnik = require('mapnik')
  , mercator = require('./utils/sphericalmercator.js')
  , mappool = require('./utils/pool.js')
  , osm_geojson = require('osm-and-geojson')
  , DOMParser = require('xmldom').DOMParser
  , http = require('http')
  , url = require('url');

if (mapnik.register_default_input_plugins) mapnik.register_default_input_plugins();

var TMS_SCHEME = true;

// create a pool of 5 maps to manage concurrency under load
var maps = mappool.create_pool(5);

var usage = 'usage: app.js <port>\ndemo:  app.js 8000';

var prj = new mapnik.Projection(mercator.proj4);

// map with just a style
var s = '<Map background-color="#00000000" srs="' + mercator.proj4 + '">';
s += '    <Style name="hoot">';
s += '        <Rule>';
s += '            <Filter>[building] &lt;&gt; \'\'</Filter>';
s += '            <LineSymbolizer stroke="${COLOR}" stroke-width="2" stroke-linejoin="miter"';
s += '                stroke-linecap="square" />';
s += '            <PolygonSymbolizer fill="${COLOR}" fill-opacity="0.3" />';
s += '        </Rule>';
s += '        <Rule>';
s += '            <Filter>[highway] &lt;&gt; \'\'</Filter>';
s += '            <LineSymbolizer stroke="${COLOR}" stroke-width="2" stroke-linejoin="round"';
s += '                stroke-linecap="round" />';
s += '        </Rule>';
s += '        <Rule>';
s += '            <Filter>[amenity] &lt;&gt; \'\'</Filter>';
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


function search(mapid, bbox, callback) {
    var options = {
      host: 'localhost',
      port: 8080,
      path: '/hoot-services/osm/api/0.6/map?mapId=' + mapid + '&bbox=' + bbox
    };

    http.get(options, function(response){
        var body = '';
        response.on('data', function(data) {
            body += data;
        });
        response.on('end', function() {
            callback(body);
        });
    }).on('error', function(e) {
        console.error(e.message);
    });
}

http.createServer(function(req, res) {
    var query = url.parse(req.url.toLowerCase(), true).query;
    if (query
        && query.x !== undefined
        && query.y !== undefined
        && query.z !== undefined
        && query.color !== undefined
        && query.mapid !== undefined
    ) {
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
            // bbox for x,y,z
            var bbox = mercator.xyz_to_envelope(parseInt(query.x), parseInt(query.y), parseInt(query.z), TMS_SCHEME);
            map.extent = prj.forward(bbox);
            map.bufferSize = 128;
            var im = new mapnik.Image(map.width, map.height);

            (['polygon', 'line', 'point']).forEach(function(d) {
                // change this to fit your db connection and settings
                var postgis_settings = {
                  dbname: query.mapid,
                  table: 'planet_osm_' + d,
                  user: 'hoot',
                  password: 'hoottest',
                  host: 'localhost',
                  type: 'postgis',
                  srid: '3857',
                  extent: '-8052518,2105784.46,-8041386.05,2115534.51'
                };

                // construct a mapnik layer dynamically
                var l = new mapnik.Layer(d);
                l.srs = map.srs;
                l.styles = ['hoot'];

                var ds = new mapnik.Datasource(postgis_settings);
                // add our custom datasource
                l.datasource = ds;
                // add this layer to the map
                map.add_layer(l);
            });
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
    } else {
        res.writeHead(500, {
          'Content-Type': 'text/plain'
        });
        res.end('no x,y,z,color,mapid provided');
    }
}).listen(port);

console.log('Test server listening on port %d', port);