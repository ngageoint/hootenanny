var tags = {};
var eTDSfcodes = {};
var eTDSschemas = {};

d3.json("data/osm-taginfo.json", function(error, json) {
    if (error) return console.warn(error);
    tags['OSM'] = d3.map(json).entries().map(function(d) {
        return {key: d.key,
                rank: d.value.count_nodes,
                value: d.value.values.map(function(v) { return v.value; })
        };
    });
    console.log("osmTagInfo keys = " + tags['OSM'].length);
    //console.log(JSON.stringify(tags['OSM'][0]));
});

d3.csv("data/osm-plus-taginfo.csv", function(error, csv) {
    if (error) return console.warn(error);
    tags['OSM+'] = d3.nest()
        .key(function(d) { return d.Tag; })
        .entries(csv)
        .map(function(d) {
            return {key: d.key,
                    rank: 0,
                    value: d.values.map(function(v) { return v.Value; })
        };
    });
    console.log("osmPlus keys = " + tags['OSM+'].length);
    //console.log(JSON.stringify(tags['OSM+'][0]));
});

tags['TDSv40'] = [];
tags['TDSv61'] = [];

//d3.csv("data/TDSv40_FCODE_english.csv", function(error, csv) {
//    if (error) return console.warn(error);
//    eTDSschemas.TDSv40 = d3.nest()
//        .key(function(d) { return d.Feature; })
//        .key(function(d) { return d.Name; })
//        .entries(csv)
//        .map(function(d) {
//            return {key: d.key,
//                    values: d.values.map(function(k) {
//                        return {
//                            key: k.key,
//                            rank: 0,
//                            value: k.values.map(function(v) { return v.Value; })
//                        };
//                    })
//                };
//        });
//    console.log("TDSv40 FCODE keys = " + eTDSschemas.TDSv40.length);
//    var output = JSON.stringify(eTDSschemas.TDSv40);
//    var blob = new Blob([output], {type: "text/plain;charset=utf-8"});
//    saveAs(blob, 'TDSv40_tag_schema.json');
//});
//
////d3.csv("data/TDSv40_FCODE_List.csv", function(error, csv) {
////    if (error) return console.warn(error);
////    eTDSfcodes.TDSv40 = {}
////    csv.forEach(function(d) {
////        eTDSfcodes.TDSv40[d.Name] = d.FCODE;
////    });
////    console.log("TDSv40 English to FCODE keys = " + d3.keys(eTDSfcodes.TDSv40).length);
////    //console.log(JSON.stringify(eTDSfcodes.TDSv40));
////});
//
//d3.csv("data/TDSv61_FCODE_english.csv", function(error, csv) {
//    if (error) return console.warn(error);
//    eTDSschemas.TDSv61 = d3.nest()
//        .key(function(d) { return d.Feature; })
//        .key(function(d) { return d.Name; })
//        .entries(csv)
//        .map(function(d) {
//            return {key: d.key,
//                    values: d.values.map(function(k) {
//                        return {
//                            key: k.key,
//                            rank: 0,
//                            value: k.values.map(function(v) { return v.Value; })
//                        };
//                    })
//        };
//    });
//    console.log("TDSv61 FCODE keys = " + eTDSschemas.TDSv61.length);
//    var output = JSON.stringify(eTDSschemas.TDSv61);
//    var blob = new Blob([output], {type: "text/plain;charset=utf-8"});
//    saveAs(blob, 'TDSv61_tag_schema.json');
//});
//
//d3.csv("data/TDSv61_FCODE_List.csv", function(error, csv) {
//    if (error) return console.warn(error);
//    eTDSfcodes.TDSv61 = {}
//    csv.forEach(function(d) {
//        eTDSfcodes.TDSv61[d.Name] = d.FCODE;
//    });
//    console.log("TDSv61 English to FCODE keys = " + d3.keys(eTDSfcodes.TDSv61).length);
//    var output = JSON.stringify(eTDSfcodes.TDSv61);
//    var blob = new Blob([output], {type: "text/plain;charset=utf-8"});
//    saveAs(blob, 'TDSv61_fcodes.json');
//
//    eTDSfcodes.TDSv40 = eTDSfcodes.TDSv61;
//});

d3.json("data/TDSv61_fcodes.json", function(error, json) {
    if (error) return console.warn(error);
    eTDSfcodes.TDSv61 = json;
    eTDSfcodes.TDSv40 = eTDSfcodes.TDSv61;
    console.log("TDSv61 English to FCODE keys = " + d3.keys(eTDSfcodes.TDSv61).length);
});

d3.json("data/TDSv61_tag_schema.json", function(error, json) {
    if (error) return console.warn(error);
    eTDSschemas.TDSv61 = json;
    console.log("TDSv61 FCODE keys = " + eTDSschemas.TDSv61.length);
});

d3.json("data/TDSv40_tag_schema.json", function(error, json) {
    if (error) return console.warn(error);
    eTDSschemas.TDSv40 = json;
    console.log("TDSv40 FCODE keys = " + eTDSschemas.TDSv40.length);
});
