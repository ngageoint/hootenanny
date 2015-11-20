#!/usr/bin/env node

var mapnik = require('mapnik')
  , mercator = require('./utils/sphericalmercator.js')
  , mappool = require('./utils/pool.js')
  , osm_geojson = require('osm-and-geojson')
  , DOMParser = require('xmldom').DOMParser
  , fs = require('fs')
  , http = require('http')
  , url = require('url');

if (mapnik.register_default_input_plugins) mapnik.register_default_input_plugins();

var TMS_SCHEME = true;

// create a pool of 5 maps to manage concurrency under load
var maps = mappool.create_pool(5);

var usage = 'usage: app.js <port>\ndemo:  app.js 8000';

// map with just a style
var s = fs.readFileSync('hoot_style.xml', 'utf8');

var port = process.argv[3];

if (!port) {
   console.log(usage);
   process.exit(1);
}

var acquire = function(id, mapid, color, stylesheet, options, callback) {
    methods = {
        create: function(cb) {
                stylesheet = stylesheet.replace(/\${COLOR}/g, color || 'rgb(255, 85, 153)');
                var obj = new mapnik.Map(options.width || 256, options.height || 256);
                obj.fromString(stylesheet, {strict: true}, function(err, obj) {
                    if (options.bufferSize) {
                        obj.bufferSize = options.bufferSize;
                    }
                    (['area', 'line', 'point']).forEach(function(d) {
                        // construct a mapnik layer dynamically
                        var l = new mapnik.Layer(d);
                        l.srs = '+init=epsg:4326';
                        l.styles = [d];

                        // change this to fit your db connection and settings
                        var postgis_settings = {
                          dbname: 'testhoot',
                          table: d,
                          user: 'hoot',
                          password: 'hoottest',
                          host: 'localhost',
                          type: 'postgis',
                          srid: '3857',
                          extent: '-180,-90,180,90'
                        };

                        var ds = new mapnik.Datasource(postgis_settings);
                        // add our custom datasource
                        l.datasource = ds;
                        // add this layer to the map
                        obj.add_layer(l);
                    });

                    cb(err, obj);
                });
            },
            destroy: function(obj) {
                delete obj;
            }
    };
    maps.acquire(id, methods, function(err, obj) {
      callback(err, obj);
    });
};

http.createServer(function(req, res) {
    var query = url.parse(req.url.toLowerCase(), true).query;
    if (query
        && query.x !== undefined
        && query.y !== undefined
        && query.z !== undefined
        && query.color !== undefined
        && query.mapid !== undefined
    ) {
      var id = query.mapid + '_' + query.color;
      acquire(id, query.mapid, query.color, s, {bufferSize: 128}, function(err, map) {
          if (err) {
              process.nextTick(function() {
                  maps.release(id, map);
              });
              res.writeHead(500, {
                'Content-Type': 'text/plain'
              });
              res.end(err.message);
          } else {
              // bbox for x,y,z
              var bbox = mercator.xyz_to_envelope(parseInt(query.x), parseInt(query.y), parseInt(query.z), TMS_SCHEME);
              map.extent = bbox;
              var im = new mapnik.Image(map.width, map.height);
              map.render(im, function(err, im) {
                  process.nextTick(function() {
                      maps.release(id, map);
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
        res.end('no x,y,z,color,mapid provided!');
    }
}).listen(port);

console.log('node-mapnik server listening on port %d', port);