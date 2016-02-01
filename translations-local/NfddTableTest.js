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

/*
    Testing creation of the DBase schema.
    Doesn't do a lot but it is a good test file
*/

// Build a schema
function getDbSchema()
{
    // Attribute Definitions - Taken from TDS EC v5
    // ZI005_FNA - Geographic Name Information: Full Name
    var zi005_fna = { 
        'name':'ZI005_FNA',
        'type':'String'
    } // End zi005_fna

    // ARA - Area
    var ara = {
        'name':'ARA',
        'type':'double',
    }; // End ara

    // PCF - Physical Condition
    var pcf = {
        'name':'PCF',
        'defValue':-999999, // No Information
        'type':'enumeration',
        'enumerations':[
            { 'name':'No Information', 'value':-999999 },
            { 'name':'Raised', 'value':1 },
            { 'name':'Level', 'value':2 },
            { 'name':'Depressed', 'value':3 },
            { 'name':'Not Applicable', 'value':998 },
        ], // End Enumeration
    }; // End pcf

    // RTY - Roadway Type
    var rty = {
        'name':'RTY',
        'defValue':-999999, // No Information
        'type':'enumeration',
        'enumerations':[
            { 'name':'No Information', 'value':-999999 },
            { 'name':'Motorway', 'value':1 },
            { 'name':'Limited Access Motorway', 'value':2 },
            { 'name':'Road', 'value':3 },
            { 'name':'Street', 'value':4 },
            { 'name':'Other', 'value':998 },
        ], // End Enumeration
    }; // End pcf

    // LTN - Track or Lane Count
    var ltn = {
        'name':'LTN',
        'type':'Long Integer',
        'defValue':1,
        'minimum':1,
        'maximum':99
    };

    // Now we add columns to a feature
    // AP030 - Roads
    var lap030 = { 
        'name':'LAP030',
        'geom':'Line',
        'columns': [zi005_fna,ara,pcf,rty,ltn]
    }; // End AP030

    // AP010 - Cart Track
    var lap010 = { 
        'name':'LAP010',
        'geom':'Line',
        'columns': [zi005_fna,ara,pcf]
    }; // End AP010
    
    // Now we add columns to a schema
//     schema.tables.push(lap030);
//     schema.tables.push(lap010);
    schema.push(lap030);
    schema.push(lap010);

    // Do we need this??
    return schema;
}

// an optional initialize function that gets called once before any 
// translateAttribute calls.
function initialize()
{
    // The print method simply prints the string representation to stdout
    print('Initializing.')
}

// an optional finalize function that gets called once after all
// translateAttribute calls.
function finalize()
{
    // the debug method prints to stdout when --debug has been specified on
    // the hoot command line. (DEBUG log level)
    debug('Finalizing.');
}

// ##########
// A translateAttributes method that is very similar to the python translate 
// attributes

// Set up an empty schema. This will go somewhere else later
schema = [];

function translateAttributes(attrs, layerName, geometryType)
{ 
    // this gets called from Hoot
    tags = {}; 

    // Go build a schema
    if (schema[0] == undefined)
    {
        print('Building Schema');
        getDbSchema();
    }

    // Now try to dump out the schema
    for (var i in schema)
    {
        print('Feature Name: ' + schema[i].name);
        for (var j in schema[i].columns) // 
        {
            print('Column Name: ' + schema[i].columns[j].name);
            print('Column Type: ' + schema[i].columns[j].type);
            if (schema[i].columns[j].enumerations)
            {
                for (k in schema[i].columns[j].enumerations)
                {
                    print('Enumeration: Name: ' + schema[i].columns[j].enumerations[k].name + ' Value: ' +schema[i].columns[j].enumerations[k].value);
                }
            }
        }
    }


    return tags;
}

