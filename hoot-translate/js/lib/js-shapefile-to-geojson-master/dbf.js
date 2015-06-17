(function(window,undefined){

    if(window.document && window.Worker){
        var worker = new Worker("js/lib/js-shapefile-to-geojson-master/dbf.js")

        var DBF = function(url, callback){
            var
                w = this._worker = worker,
                t = this

            w.onmessage = function(e){
                t.data = e.data
                if (callback) callback(e.data);
            };

            w.postMessage(url)
        }

        window["DBF"] = DBF
        return
    }

    var IN_WORKER = !window.document
    if (IN_WORKER) {
        importScripts('stream.js')
        onmessage = function(e){
            new DBF(e.data);
        };
    }

    var
        DBASE_LEVEL = {
            "3": "dBASE Level 5",
            "4": "dBase Level 7"
        },
        DBASE_FIELD_TYPE = {
            "N": "Number",
            "C": "Character", // binary
            "L": "Logical",
            "D": "Date",
            "M": "Memo", // binary
            "F": "Floating point",
            "B": "Binary",
            "G": "General",
            "P": "Picture",
            "Y": "Currency",
            "T": "DateTime",
            "I": "Integer",
            "V": "VariField",
            "X": "Variant",
            "@": "Timestamp",
            "O": "Double",
            "+": "Autoincrement", // (dBase Level 7)
            "O": "Double", // (dBase Level 7)
            "@": "Timestamp" // (dBase Level 7)
        }

    var DBF = function(url, callback){
        if (!!url.lastModifiedDate)
            this.handleFile(url, callback);
        else
            this.handleUri(url, callback);
    }

    DBF.prototype = {
        constructor: DBF,
        handleFile: function(file, callback) {
            this.callback = callback;

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
                reader.readAsBinaryString(file);
            } else {
                this.onFileLoad(reader.readAsBinaryString(file));
            }
        },
        onFileLoad: function(data) {
            this.stream = new Gordon.Stream(data)

            this.readFileHeader()
            this.readFieldDescriptions()
            this.readRecords()

            this._postMessage()
        },
        handleUri: function(url, callback) {
            var xhr = new XMLHttpRequest();

            xhr.open("GET", url, false)
            xhr.overrideMimeType("text/plain; charset=x-user-defined")
            xhr.send()

            if(200 != xhr.status)
                throw "Unable to load " + url + " status: " + xhr.status

            this.stream = new Gordon.Stream(xhr.responseText)
            this.callback = callback

            this.readFileHeader()
            this.readFieldDescriptions()
            this.readRecords()

            this._postMessage()
        },
        _postMessage: function() {
            var data = {
                    header: this.header,
                    fields: this.fields,
                    records: this.records
                }
            if (IN_WORKER) postMessage(data)
            else if (this.callback) this.callback(data)
        },
        readFileHeader: function(){
            var s = this.stream,
                header = this.header = {},
                date = new Date;

            header.version = DBASE_LEVEL[s.readSI8()]

            // Date of last update; in YYMMDD format.  Each byte contains the number as a binary.  YY is added to a base of 1900 decimal to determine the actual year. Therefore, YY has possible values from 0x00-0xFF, which allows for a range from 1900-2155.
            date.setUTCFullYear(1900 + s.readSI8())
            date.setUTCMonth(s.readSI8())
            date.setUTCDate(s.readSI8())

            header.lastUpdated = date

            // Number of records in file
            header.numRecords = s.readSI32()

            // Position of first data record
            header.firstRecordPosition = s.readSI16()

            // Length of one data record, including delete flag
            header.recordLength = s.readSI16()

            // Reserved; filled with zeros
            s.offset += 16

            /*
            Table flags:
            0x01   file has a structural .cdx
            0x02   file has a Memo field
            0x04   file is a database (.dbc)
            This byte can contain the sum of any of the above values. For example, the value 0x03 indicates the table has a structural .cdx and a Memo field.
            */
            header.flags = s.readSI8()

            // Code page mark
            header.codePageMark = s.readSI8()

            // Reserved; filled with zeros.
            s.offset += 2

        },
        readFieldDescriptions: function(){
            var s = this.stream,
                fields = [],
                field

            while (s.readSI8() != 0x0D) {
                s.offset--
                field = {}

                // Field name with a maximum of 10 characters. If less than 10, it is padded with null characters (0x00).
                field.name = s.readString(11).replace(/\u0000/g,"")

                field.type = DBASE_FIELD_TYPE[s.readString(1)]

                // Displacement of field in record
                field.fieldDisplacement = s.readSI32()

                // Length of field (in bytes)
                field.fieldLength = s.readUI8()

                // Number of decimal places
                field.decimals = s.readSI8()

                /*
                Field flags:
                0x01   System Column (not visible to user)
                0x02   Column can store null values
                0x04   Binary column (for CHAR and MEMO only)
                0x06   (0x02+0x04) When a field is NULL and binary (Integer, Currency, and Character/Memo fields)
                0x0C   Column is autoincrementing
                */
                field.flags = s.readSI8()

                // Value of autoincrement Next value
                field.autoincrementNextValue = s.readSI32()

                // Value of autoincrement Step value
                field.autoincrementStepValue = s.readSI8()

                // Reserved
                s.offset += 8

                fields.push(field)
            }

            this.fields = fields

        },
        readRecords: function(){
            var s = this.stream,
                numRecords = this.header.numRecords,
                recordsOffset = this.header.firstRecordPosition,
                recordSize = this.header.recordLength,
                fields = this.fields,
                numFields = fields.length,
                records = [],
                field, record

            for (var index = 0; index < numRecords; index++) {
                s.offset = recordsOffset + index * recordSize

                record = {}

                // Data records begin with a delete flag byte. If this byte is an ASCII space (0x20), the record is not deleted. If the first byte is an asterisk (0x2A), the record is deleted
                record._isDeleted = s.readSI8() == 42

                for(var i = 0; i < numFields; i++){
                    field = fields[i]
                    record[field.name] = s.readString(field.fieldLength).trim();
                }

                records.push(record);
            }

            this.records = records

        }
    }

    window["DBF"] = DBF;

})(self);

