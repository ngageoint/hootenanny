#!/usr/bin/env node

var mapnik = require('mapnik')
  , mercator = require('./utils/sphericalmercator.js')
  , mappool = require('./utils/pool.js')
  , http = require('http')
  , parseXYZ = require('./utils/tile.js').parseXYZ;

// register shapefile plugin
if (mapnik.register_default_input_plugins) mapnik.register_default_input_plugins();

var TMS_SCHEME = true;

// create a pool of 5 maps to manage concurrency under load
var maps = mappool.create_pool(5);

var usage = 'usage: app.js <stylesheet> <port>\ndemo:  app.js hoot-style.xml 8000';

var stylesheet = process.argv[2];

if (!stylesheet) {
   console.log(usage);
   process.exit(1);
}

var port = process.argv[3];

if (!port) {
   console.log(usage);
   process.exit(1);
}

var aquire = function(id,options,callback) {
    methods = {
        create: function(cb) {
                var obj = new mapnik.Map(options.width || 256, options.height || 256);
                obj.load(id, {strict: true},function(err,obj) {
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


http.createServer(function(req, res) {
    parseXYZ(req, TMS_SCHEME, function(err,params) {
        if (err) {
            res.writeHead(500, {
              'Content-Type': 'text/plain'
            });
            res.end(err.message);
        } else {
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
                    var bbox = mercator.xyz_to_envelope(params.x, params.y, params.z, TMS_SCHEME);
                    // var map = new mapnik.Map(256, 256);
                    // map.load(stylesheet, function(err,map) {
                    //       if (err) {
                    //           return res.end(err.message);
                    //       }
                    map.extent = bbox;
                    var im = new mapnik.Image(map.width, map.height);
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
        }
    });

}).listen(port);


console.log('Test server listening on port %d', port);