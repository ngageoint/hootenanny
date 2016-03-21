/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

hoot.require('SchemaTools');
hoot.require('config');
hoot.require('translate');

var isSimilar = schemaTools.isSimilar;
var isA = schemaTools.isA;
var simple = schemaTools.simple;
var wildcard = schemaTools.wildcard;

// each of these operations assigns a score to a tag and its associated outcome. Only the highest
// scoring tag is maintained in the output table.
var one2one = [
    ["TYP", 50, 'highway=road', wildcard('highway=.*_link', 1)],
    ["TYP", 1, simple('highway=road', 2), isA('highway=road', 0.001)],
    ["TYP", 41, isSimilar('highway=motorway', 0.7, 0.1, 0.5)],
    ["TYP", 33, isSimilar('highway=unclassified', 0.7, 0.1, 0.5)],
    ["FFN", 464, isSimilar('shop=car', 0.7, 0.1, 0.5)],
    ["FFN", 572, isSimilar('amenity=restaurant', 0.7, 0.1, 0.5)],
    ["FFN", 573, isSimilar('amenity=bar', 0.8, 0.1, 0.5)],
    ["FFN", 574, isSimilar('amenity=dining_hall', 0.7, 0.1, 0.5)],
    ["FFN", 578, isSimilar('amenity=banquet_hall', 0.7, 0.1, 0.5)],
    ["FFN", 579, isSimilar('amenity=convention_centre', 0.7, 0.1, 0.5)],
    ["FFN", 594, isSimilar('amenity=cinema', 0.7, 0.1, 0.5)],
    ["FFN", 643, isSimilar('amenity=bank', 0.7, 0.1, 0.5)],
    ["FFN", 752, isSimilar('shop=photo', 0.7, 0.1, 0.5)],
    ["FFN", 775, isSimilar('shop=travel_agency', 0.7, 0.1, 0.5)]
];

var toOsmTable;
var toOgrTable;

function getToOgrTable()
{
    if (!toOgrTable)
    {
        var t = schemaTools.generateToOgrTable(one2one);
        toOgrTable = t;

        for (var k1 in t) {
            for (var v1 in t[k1]) {
                print(JSON.stringify([k1, v1, t[k1][v1][0], t[k1][v1][1], t[k1][v1][2]]));
            }
        }
    }

    return toOgrTable;
}

function getToOsmTable()
{
    if (!toOsmTable)
    {
        print("Ingest:");
        var t = schemaTools.generateToOsmTable(one2one);
        toOsmTable = t;

        for (var k1 in t) {
            for (var v1 in t[k1]) {
                print(JSON.stringify([k1, v1, t[k1][v1][0], t[k1][v1][1]]));
            }
        }
    }

    return toOsmTable;
}

// an optional initialize function that gets called once before any 
// translateAttribute calls.
function initialize()
{
    // The print method simply prints the string representation to stdout
    print("Initializing.");
}

// an optional finalize function that gets called once after all
// translateAttribute calls.
function finalize()
{
    // the debug method prints to stdout when --debug has been specified on
    // the hoot command line. (DEBUG log level)
    debug("Finalizing.");
}

//
// A very simple function for translating NFDDv4's to OSM+:
// - NAM column to OSM+'s name tag
// - TYP column to OSM+'s highway tag
// This is far from complete, but demonstrates the concepts.
//
function translateToOgr(tags, elementType, geometryType) 
{
    var attrs = {};

    if (geometryType != "Line")
    {
        return null;
    }

    translate.applyOne2One(tags, attrs, getToOgrTable(), [], []);

    return { attrs: attrs, tableName: "LAP030" };
}

//
// A very simple function for translating from OSM+'s schema to NFDDv4:
// - name tag to NFDDv4's NAM column
// - highway tag to NFDDv4's TYP column
// This is far from complete, but demonstrates the concepts.
//
function translateToOsm(attrs, layerName, geometryType)
{ 
    tags = {}; 

    translate.applyOne2One(attrs, tags, getToOsmTable(), [], []);

    return tags
}

//
// This returns a schema for a subset of the NFDDv4 LAP030 (road) columns.
//
function getDbSchema()
{
    var schema = [
        lap030 = {
            name: 'LAP030',
            geom: 'Line',
            columns: [
                { name:"TYP",
                  desc:"Thoroughfare Type" ,
                  optional:"O" ,
                  type:"enumeration",
                  enumerations:[
                     { name:"Unknown", value:"0" }, 
                     { name:"Road", value:"1" },
                     { name:"Street", value:"33" },
                     { name:"Motorway", value:"41" },
                     { name:"Link", value:"50" }
                  ] // End of Enumerations
                 }, // End of TYP
                 { name:"FFN",
                   desc:"Feature Function" ,
                   optional:"R" ,
                   type:"enumeration",
                   defValue:"-999999",
                   enumerations:[
                       { name:"No Information", value:"-999999" },
                       { name:"", value:"572" },
                       { name:"", value:"573" },
                       { name:"", value:"574" },
                       { name:"", value:"578" },
                       { name:"", value:"579" },
                       { name:"", value:"594" },
                       { name:"", value:"643" },
                       { name:"", value:"752" },
                       { name:"", value:"775" },
                       { name:"Other", value:"999" },
                    ] // End of Enumerations·
                 } // End of FFN
            ]
        }
    ]
 
    return schema;
}
