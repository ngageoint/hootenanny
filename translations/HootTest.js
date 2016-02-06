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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    HootTest.js

    Basic translation script for unit testing.
*/

// ##########
// Translate OSM to a cut-down version of the NFDD TDSv4 translator
//
//  90% of this code is taken from the standard NFDD translation script.
//

function translateAttributes(attrs, layerName, geometryType)
{
        return attrs; // Do Nothing

} // End of Translate Attributes

function translateToOgr(tags, elementType, geometryType)
{ 
    var attrs = {};   // The Output
    var tableName = ''; // The final Table Name
    
    var attrs2 = {};   // The Second Output
    var tableName2 = ''; // The Second Table Name

    // The Nuke Option: If we have a relation, drop the feature and carry on
    if (tags['building:part']) return null;

    // Pre Process :-)
    if (tags.amenity && !(tags.building)) tags.building = 'yes';


    // Basic translations
    if (tags.name) attrs.ZI005_FNA = tags.name;
    if (tags.note) attrs.ZI006_MEM = tags.note;

    // Alt Name - gets added to the standard name
    if (tags.alt_name)
    {
        if (attrs.ZI005_FNA)
        {
            attrs.ZI005_FNA = attrs.ZI005_FNA + ';' + tags.alt_name;
        }
        else
        {
            attrs.ZI005_FNA = tags.alt_name;
        } 
    }

    // Buildings
    if (tags.building == 'restaurant') attrs.FFN = '572';

    if (tags.amenity) attrs.OTH = '(FFN:' + tags.amenity + ')';

    // Road rules 
    if (tags.highway == 'road') 
    {
        attrs.RTN_ROI = '5'; // Making them local
        attrs.TYP = '1'; // Making them a road
    }
    else if (tags.highway == 'secondary')
    {
        attrs.RTN_ROI = '4'; // Making them secondary
        attrs.TYP = '1'; // Making them a road
    }
     else if (tags.highway == 'unclassified')
    {
        attrs.RTN_ROI = '5'; // Making them Local
        attrs.TYP = '1'; // Making them a road
    }

    // Add a UFI
    if (tags.uuid)
    {
        var str = tags['uuid'].split(';');
        attrs.UFI = str[0].replace('{','').replace('}','');
    }
    else
    {
        attrs.UFI = createUuid().replace('{','').replace('}','');
    }

    // Set the F_CODE
    // Note: This is not great.  If we have a bridge _and_ a road/railway we are depending
    // on the order of the tests to determine the FCODE.
    if (tags.bridge) attrs.F_CODE = 'AQ040';
    if (tags.poi) attrs.F_CODE = 'ZD040';
    if (tags.highway) attrs.F_CODE = 'AP030';
    if (tags.highway == 'track') attrs.F_CODE = 'AP010';
    if (tags.building) attrs.F_CODE = 'AL013';
    if (tags.railway) attrs.F_CODE = 'AN010';

    // if (tags.leisure == 'park') attrs.F_CODE = 'AK120'; // Not in our test spec

    // Now for the new stuff: Build a bridge from a Road or a Railway
    if (geometryType == 'Line' && tags.bridge && (tags.highway || tags.railway))
    {
        tableName2 = 'BRIDGE_C'; // Hardcoded for testing
        attrs2.F_CODE = 'AQ040';
        attrs2.UFI = attrs.UFI;
        if (attrs.ZI005_FNA) attrs2.ZI005_FNA = attrs.ZI005_FNA;
        if (attrs.ZI006_MEM) attrs2.ZI006_MEM = attrs.ZI006_MEM;
    } // End Make a Bridge
    
    // Build a Table Name
    for (var i=0, sLen = schema.length; i < sLen; i++)
    {
        if (schema[i].fcode == attrs.F_CODE && schema[i].geom == geometryType)
        {
            tableName = schema[i].name; 
            break;
        }
    }

    // No tableName = o2s layer
    if (!tableName)
    {
        if (geometryType == 'Point')
        {
            tableName = 'o2s_P'; // o2s == Out Of Spec :-)
        }
        else if (geometryType == 'Line')
        {
            tableName = 'o2s_L';
        }
        // else if (geometryType == 'Area') // Could probably remove this if statement
        else
        {
            tableName = 'o2s_A';
        }
        // Convert all of the Tags to a string so we can jam it into an attribute
        var str = JSON.stringify(tags);

        return {attrs:{tag1:str}, tableName: tableName};
    }
    else
    {
        if (attrs2.F_CODE)
        {
            // print ('Returning two features');
            // for (var i in attrs) print('Attrs1: ' + i + ': :' + attrs[i] + ':');
            // for (var i in attrs2) print('Attrs2: ' + i + ': :' + attrs2[i] + ':');
            return([{attrs: attrs, tableName: tableName},{attrs: attrs2, tableName: tableName2}]);
        }
        else
        {
            // for (var i in attrs) print('Attrs1: ' + i + ': :' + attrs[i] + ':');
            return {attrs: attrs, tableName: tableName};
        }
    }

}

// Schema built from bits of conf/nfdd/LTDSv40.csv.gz
// getDbSchema: function()
function getDbSchema ()
{
    // Schema is usually not global. This is a small example so it doesn't matter
    schema = [
        { name:"ROAD_C",
          fcode:"AP030",
          desc:"Road",
          geom:"Line",
          columns:[
                   { name:"F_CODE",
                     desc:"Feature Code",
                     type:"String",
                     optional:"R",
                     defValue:"AP030",
                     length:"5"
                   },
                   { name:"ZI005_FNA",
                     desc:"Geographic Name Information : Full Name",
                     optional:"R",
                     type:"String",
                     length:"200",
                     defValue:"No Information",
                   },
                   { name:"ZI006_MEM",
                     desc:"Note : Memorandum",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   },
                   { name:"UFI",
                     desc:"Unique Entity Identifier",
                     optional:"R",
                     type:"String",
                     length:"254",
                     defValue:"No Information",
                   },
                   { name:"TYP",
                     desc:"Thoroughfare Type",
                     optional:"R",
                     defValue:"-999999",
                     type:"enumeration",
                     enumerations:[
                         { name:"No Information", value:"-999999" },
                         { name:"Road", value:"1" },
                         { name:"Street", value:"33" },
                         { name:"Motorway", value:"41" },
                         { name:"Other", value:"999" },
                         ] // End of Enumerations 
                   },
                   { name:"RTN_ROI",
                     desc:"Route Designation (route designation type)",
                     optional:"R",
                     defValue:"-999999",
                     type:"enumeration",
                     enumerations:[
                         { name:"No Information", value:"-999999" },
                         { name:"International", value:"1" },
                         { name:"National Motorway", value:"2" },
                         { name:"National", value:"3" },
                         { name:"Secondary", value:"4" },
                         { name:"Local", value:"5" },
                         { name:"Other", value:"999" },
                     ] // End of Enumerations 
                   }
                  ] // End of Columns
        }, // End of Feature

        { name:"CART_TRACK_C",
          fcode:"AP010",
          desc:"Cart Track",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code",
                       type:"String",
                       optional:"R",
                       defValue:"",
                       length:"5"
                     },
                     { name:"ZI005_FNA",
                       desc:"Geographic Name Information : Full Name",
                       optional:"R",
                       type:"String",
                       length:"200",
                       defValue:"noInformation",
                     },
                     { name:"ZI006_MEM",
                       desc:"Note : Memorandum",
                       optional:"R",
                       type:"String",
                       defValue:"noInformation",
                     },
                     { name:"TRS",
                       desc:"Transportation System Type [3]",
                       optional:"R",
                       defValue:"-999999",
                       type:"enumeration",
                       enumerations:[
                           { name:"No Information", value:"-999999" },
                           { name:"Automotive", value:"3" },
                           { name:"Bus", value:"4" },
                           { name:"Caravan Route", value:"6" },
                           { name:"Portage", value:"11" },
                           { name:"Pipeline Maintenance", value:"21" },
                           { name:"Power Line Maintenance", value:"22" },
                           { name:"Other", value:"999" },
                        ] // End of Enumerations 
                     },
                     { name:"UFI",
                       desc:"Unique Entity Identifier",
                       optional:"R",
                       type:"String",
                       length:"254",
                       defValue:"noInformation",
                     },
                    ] // End of Columns
          }, // End of Feature

        { name:"RAILWAY_C",
          fcode:"AN010",
          desc:"Railway",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code",
                       type:"String",
                       optional:"R",
                       defValue:"",
                       length:"5"
                     },
                     { name:"ZI005_FNA",
                       desc:"Geographic Name Information : Full Name",
                       optional:"R",
                       type:"String",
                       length:"200",
                       defValue:"noInformation",
                     },
                     { name:"ZI006_MEM",
                       desc:"Note : Memorandum",
                       optional:"R",
                       type:"String",
                       defValue:"noInformation",
                     },
                     { name:"ZI017_GAW",
                       desc:"Track Information : Railway Gauge",
                       optional:"R",
                       type:"Real",
                       defValue:"-999999.0"
                     },
                     { name:"UFI",
                       desc:"Unique Entity Identifier",
                       optional:"R",
                       type:"String",
                       length:"254",
                       defValue:"noInformation",
                     },
                    ] // End of Columns
          }, // End of Feature

        { name:"BRIDGE_C",
          fcode:"AQ040",
          desc:"Bridge",
          geom:"Line",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code",
                       type:"String",
                       optional:"R",
                       defValue:"",
                       length:"5"
                     },
                     { name:"ZI005_FNA",
                       desc:"Geographic Name Information : Full Name",
                       optional:"R",
                       type:"String",
                       length:"200",
                       defValue:"noInformation",
                     },
                     { name:"ZI006_MEM",
                       desc:"Note : Memorandum",
                       optional:"R",
                       type:"String",
                       defValue:"noInformation",
                     },
                     { name:"TRS",
                       desc:"Transportation System Type",
                       optional:"R",
                       defValue:"-999999",
                       type:"enumeration",
                       enumerations:[
                           { name:"No Information", value:"-999999" },
                           { name:"Aqueduct", value:"2" },
                           { name:"Canal", value:"5" },
                           { name:"Pedestrian", value:"9" },
                           { name:"Pipeline", value:"10" },
                           { name:"Railway", value:"12" },
                           { name:"Road", value:"13" },
                           { name:"Taxiway", value:"20" },
                           { name:"Runway", value:"25" },
                           { name:"Other", value:"999" },
                        ] // End of Enumerations 
                     },
                     { name:"UFI",
                       desc:"Unique Entity Identifier",
                       optional:"R",
                       type:"String",
                       length:"254",
                       defValue:"noInformation",
                     },
                    ] // End of Columns
          }, // End of Feature

        { name:"BRIDGE_P",
          fcode:"AQ040",
          desc:"Bridge",
          geom:"Point",
          columns:[ 
                     { name:"F_CODE",
                       desc:"Feature Code",
                       type:"String",
                       optional:"R",
                       defValue:"",
                       length:"5"
                     },
                     { name:"ZI005_FNA",
                       desc:"Geographic Name Information : Full Name",
                       optional:"R",
                       type:"String",
                       length:"200",
                       defValue:"noInformation",
                     },
                     { name:"ZI006_MEM",
                       desc:"Note : Memorandum",
                       optional:"R",
                       type:"String",
                       defValue:"noInformation",
                     },
                     { name:"TRS",
                       desc:"Transportation System Type",
                       optional:"R",
                       defValue:"-999999",
                       type:"enumeration",
                       enumerations:[
                           { name:"No Information", value:"-999999" },
                           { name:"Aqueduct", value:"2" },
                           { name:"Canal", value:"5" },
                           { name:"Pedestrian", value:"9" },
                           { name:"Pipeline", value:"10" },
                           { name:"Railway", value:"12" },
                           { name:"Road", value:"13" },
                           { name:"Taxiway", value:"20" },
                           { name:"Runway", value:"25" },
                           { name:"Other", value:"999" },
                        ] // End of Enumerations 
                     },
                     { name:"UFI",
                       desc:"Unique Entity Identifier",
                       optional:"R",
                       type:"String",
                       length:"254",
                       defValue:"noInformation",
                     },
                    ] // End of Columns
          }, // End of Feature
        
        { name:"BUILDING_P",
          fcode:"AL013",
          desc:"Building",
          geom:"Point",
          columns:[
                   { name:"F_CODE",
                     desc:"Feature Code",
                     type:"String",
                     optional:"R",
                     defValue:"",
                     length:"5"
                   },
                   { name:"ZI005_FNA",
                     desc:"Geographic Name Information : Full Name",
                     optional:"R",
                     type:"String",
                     length:"200",
                     defValue:"No Information",
                   },
                   { name:"ZI006_MEM",
                     desc:"Note : Memorandum",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   },
                   { name:"UFI",
                     desc:"Unique Entity Identifier",
                     optional:"R",
                     type:"String",
                     length:"254",
                     defValue:"No Information",
                   },
                   { name:"OTH",
                     desc:"Specified Domain Value(s)",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   },
                   { name:"FFN",
                     desc:"Feature Function",
                     optional:"R",
                     defValue:"-999999",
                     type:"enumeration",
                     enumerations:[
                         { name:"No Information", value:"-999999" },
                         { name:"Restaurant", value:"572" },
                         { name:"Other", value:"999" },
                     ] // End of Enumerations 
                   }
                  ] // End of Columns
        }, // End of Feature

        { name:"BUILDING_S",
          fcode:"AL013",
          desc:"Building",
          geom:"Area",
          columns:[
                   { name:"F_CODE",
                     desc:"Feature Code",
                     type:"String",
                     optional:"R",
                     defValue:"",
                     length:"5"
                   },
                   { name:"ZI005_FNA",
                     desc:"Geographic Name Information : Full Name",
                     optional:"R",
                     type:"String",
                     length:"200",
                     defValue:"No Information",
                   },
                   { name:"ZI006_MEM",
                     desc:"Note : Memorandum",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   },
                   { name:"UFI",
                     desc:"Unique Entity Identifier",
                     optional:"R",
                     type:"String",
                     length:"254",
                     defValue:"No Information",
                   },
                   { name:"OTH",
                     desc:"Specified Domain Value(s)",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   },
                   { name:"FFN",
                     desc:"Feature Function",
                     optional:"R",
                     defValue:"-999999",
                     type:"enumeration",
                     enumerations:[
                         { name:"No Information", value:"-999999" },
                         { name:"Restaurant", value:"572" },
                         { name:"Other", value:"999" },
                     ] // End of Enumerations 
                   },
                  ] // End of Columns
        }, // End of Feature

        { name:"NAMED_LOCATION_P",
          fcode:"ZD040",
          desc:"Named Location",
          geom:"Point",
          columns:[
                   { name:"F_CODE",
                     desc:"Feature Code",
                     type:"String",
                     optional:"R",
                     defValue:"",
                     length:"5"
                   },
                   { name:"ZI005_FNA",
                     desc:"Geographic Name Information : Full Name",
                     optional:"R",
                     type:"String",
                     length:"200",
                     defValue:"No Information",
                   },
                   { name:"ZI006_MEM",
                     desc:"Note : Memorandum",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   },
                   { name:"UFI",
                     desc:"Unique Entity Identifier",
                     optional:"R",
                     type:"String",
                     length:"254",
                     defValue:"No Information",
                   },
                   { name:"OTH",
                     desc:"Specified Domain Value(s)",
                     optional:"R",
                     type:"String",
                     defValue:"No Information",
                   }
                  ] // End of Columns
        }, // End of Feature
        { name: "o2s_P",
          desc: "o2s",
          geom: "Point",
          columns:[ 
                   { name:'tag1',
                     desc:'Tag List',
                     type:'String',
                     defValue: '',
                     length:'254'
                   }
                 ]
        }, // End of Feature
        { name: "o2s_L",
          desc: "o2s",
          geom: "Line",
          columns:[ 
                   { name:'tag1',
                     desc:'Tag List',
                     type:'String',
                     defValue: '',
                     length:'254'
                   }
                 ]
        }, // End of Feature
        { name: "o2s_A",
          desc: "o2s",
          geom: "Area",
          columns:[ 
                   { name:'tag1',
                     desc:'Tag List',
                     type:'String',
                     defValue: '',
                     length:'254'
                   }
                 ]
        }, // End of Feature

      ]; // End of schema

    return schema;

} // End of GetDBSchema

