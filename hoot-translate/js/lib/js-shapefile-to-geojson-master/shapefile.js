(function(window,undefined){

    if(window.document && window.Worker){
        var worker = null;

        var Shapefile = function(o, callback){
            var
                t = this,
                o = typeof o == "string" ? {shp: o} : o

            if (!worker) {
                var path = (o.jsRoot || "") + "shapefile.js"
                var w = worker = this.worker = new Worker(path)
            } else {
                var w = worker
            }

            w.onmessage = function(e){
                t.data = e.date
                if(callback) callback(e.data)
            }

            w.postMessage(["Load", o])

            if(o.dbf) this.dbf = new DBF(o.dbf,function(data){
                w.postMessage(["Add DBF Attributes", data])
            })
        }

        window["Shapefile"] = Shapefile
        return
    }

    var IN_WORKER = !window.document
    if (IN_WORKER) {
        importScripts('stream.js')
        onmessage = function(e){
            switch (e.data[0]) {
                case "Load":
                    window.shapefile = new Shapefile(e.data[1])
                    break
                case "Add DBF Attributes":
                    window.shapefile.addDBFDataToGeoJSON(e.data[1])
                    window.shapefile._postMessage()
                    break
                default:
            }
        };
    }

    var SHAPE_TYPES = {
        "0": "Null Shape",
        "1": "Point", // standard shapes
        "3": "PolyLine",
        "5": "Polygon",
        "8": "MultiPoint",
        "11": "PointZ", // 3d shapes
        "13": "PolyLineZ",
        "15": "PolygonZ",
        "18": "MultiPointZ",
        "21": "PointM", // user-defined measurement shapes
        "23": "PolyLineM",
        "25": "PolygonM",
        "28": "MultiPointM",
        "31": "MultiPatch"
    }

    var Shapefile = function(o,callback){
        var o = typeof o == "string" ? {shp: o} : o
        this.callback = callback

        if (!!o.shp.lastModifiedDate)
            this.handleFile(o);
        else
            this.handleUri(o);
    }

    Shapefile.prototype = {
        constructor: Shapefile,
        handleUri: function(o) {
            var xhr = new XMLHttpRequest(),
                that = this
            
            xhr.open("GET", o.shp, false)
            xhr.overrideMimeType("text/plain; charset=x-user-defined")
            xhr.send()

            if(200 != xhr.status)
                throw "Unable to load " + o.shp + " status: " + xhr.status

            this.url = o.shp
            this.stream = new Gordon.Stream(xhr.responseText)

            this.readFileHeader()
            this.readRecords()
            this.formatIntoGeoJson()

            if(o.dbf) this.dbf = IN_WORKER ?
                null :
                new DBF(o.dbf,function(data){
                    that.addDBFDataToGeoJSON(data)
                    that._postMessage()
                })
            else this._postMessage()

        },
        handleFile: function(o) {
            this.options = o
            if (!!window.FileReader) {
                var reader = new FileReader();
            } else {
                var reader = new FileReaderSync();
            }

            reader.onload = (function(that){
                return function(e){
                    that.onFileLoad(e.target.result)
                }
            })(this);

            if (!!window.FileReader) {
                reader.readAsBinaryString(o.shp);
            } else {
                this.onFileLoad(reader.readAsBinaryString(o.shp));   
            }
        },
        onFileLoad: function(data) {
            this.stream = new Gordon.Stream(data)

            this.readFileHeader()
            this.readRecords()
            this.formatIntoGeoJson()            

            if(this.options.dbf) this.dbf = IN_WORKER ?
                null :
                new DBF(this.options.dbf,function(data){
                    that.addDBFDataToGeoJSON(data)
                    that._postMessage()
                })
            else this._postMessage()
        },
        _postMessage: function() {
            var data = {
                    header: this.header,
                    records: this.records,
                    dbf: this.dbf,
                    geojson: this.geojson
                }
            if (IN_WORKER) postMessage(data)
            else if (this.callback) this.callback(data)
        },
        readFileHeader: function(){
            var s = this.stream,
                header = this.header = {}

            // The main file header is fixed at 100 bytes in length
            if(s < 100) throw "Invalid Header Length"

            // File code (always hex value 0x0000270a)
            header.fileCode = s.readSI32(true)

            if(header.fileCode != parseInt(0x0000270a))
                throw "Invalid File Code"

            // Unused; five uint32
            s.offset += 4 * 5

            // File length (in 16-bit words, including the header)
            header.fileLength = s.readSI32(true) * 2

            header.version = s.readSI32()

            header.shapeType = SHAPE_TYPES[s.readSI32()]

            // Minimum bounding rectangle (MBR) of all shapes contained within the shapefile; four doubles in the following order: min X, min Y, max X, max Y
            this._readBounds(header)

            // Z axis range
            header.rangeZ = {
                min: s.readDouble(),
                max: s.readDouble()
            }

            // User defined measurement range
            header.rangeM = {
                min: s.readDouble(),
                max: s.readDouble()
            }

        },
        readRecords: function(){
            var s = this.stream,
                records = this.records = [],
                record

            do {
                record = {}

                // Record number (1-based)
                record.id = s.readSI32(true)

                if(record.id == 0) break //no more records

                // Record length (in 16-bit words)
                record.length = s.readSI32(true) * 2

                record.shapeType = SHAPE_TYPES[s.readSI32()]

                // Read specific shape
                this["_read" + record.shapeType](record);

                records.push(record);

            } while(true);

        },
        _readBounds: function(object){
            var s = this.stream

            object.bounds = {
                left: s.readDouble(),
                bottom: s.readDouble(),
                right: s.readDouble(),
                top: s.readDouble()
            }

            return object
        },
        _readParts: function(record){
            var s = this.stream,
                nparts,
                parts = []

            nparts = record.numParts = s.readSI32()

            // since number of points always proceeds number of parts, capture it now
            record.numPoints = s.readSI32()

            // parts array indicates at which index the next part starts at
            while(nparts--) parts.push(s.readSI32())

            record.parts = parts

            return record
        },
        _readPoint: function(record){
            var s = this.stream

            record.x = s.readDouble()
            record.y = s.readDouble()

            return record
        },
        _readPoints: function(record){
            var s = this.stream,
                points = [],
                npoints = record.numPoints || (record.numPoints = s.readSI32())

            while(npoints--)
                points.push({
                    x: s.readDouble(),
                    y: s.readDouble()
                })

            record.points = points

            return record
        },
        _readMultiPoint: function(record){
            var s = this.stream

            this._readBounds(record)
            this._readPoints(record)

            return record
        },
        _readPolygon: function(record){
            var s = this.stream

            this._readBounds(record)
            this._readParts(record)
            this._readPoints(record)

            return record
        },
        _readPolyLine: function(record){
            return this._readPolygon(record);
        },
        formatIntoGeoJson: function(){
            var bounds = this.header.bounds,
                records = this.records,
                features = [],
                feature, geometry, points, fbounds, gcoords, parts, point,
                geojson = {}

            geojson.type = "FeatureCollection"
            geojson.bbox = [
                    bounds.left,
                    bounds.bottom,
                    bounds.right,
                    bounds.top
                ]
            geojson.features = features

            for (var r = 0, record; record = records[r]; r++){
                feature = {}, fbounds = record.bounds, points = record.points, parts = record.parts
                feature.type = "Feature"
                if (record.shapeType !== 'Point') {
                    feature.bbox = [
                        fbounds.left,
                        fbounds.bottom,
                        fbounds.right,
                        fbounds.top
                    ]                    
                }
                geometry = feature.geometry = {}

                switch (record.shapeType) {
                    case "Point":
                        geometry.type = "Point"
                        geometry.coordinates = [
                            record.x,
                            record.y ]
                        break
                    case "MultiPoint":
                    case "PolyLine":
                        geometry.type = (record.shapeType == "PolyLine" ? "LineString" : "MultiPoint")
                        gcoords = geometry.coordinates = []

                        for (var p = 0; p < points.length; p++){
                            var point = points[p]
                            gcoords.push([point.x,point.y])
                        }
                        break
                    case "Polygon":
                        geometry.type = "Polygon"
                        gcoords = geometry.coordinates = []

                        for (var pt = 0; pt < parts.length; pt++){
                            var partIndex = parts[pt],
                                part = [],
                                point

                            // partIndex 0 == main poly, partIndex > 0 == holes in poly
                            for (var p = partIndex; p < (parts[pt+1] || points.length); p++){
                                point = points[p]
                                part.push([point.x,point.y])
                            }
                            gcoords.push(part)
                        }
                        break
                    default:
                }
                features.push(feature)
            }
            this.geojson = geojson

            if(this._addDataAfterLoad) this.addDBFDataToGeoJSON(this._addDataAfterLoad);
        },
        addDBFDataToGeoJSON: function(dbfData){
            if(!this.geojson) return (this._addDataAfterLoad = dbfData)

            this.dbf = dbfData

            var features = this.geojson.features,
                len = features.length,
                records = dbfData.records

            while(len--) features[len].properties = records[len]
        }
    }

    window["Shapefile"] = Shapefile;
})(self);

