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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// This translation file shows examples of a lot of a lot of the translation 
// features, but it is a very poor example of how to perform translations.
//

// Layer filter test - Test passing a string from Javascript back to Hoot
function layerNameFilter()
{
    return "Papa Smurf";
}

function translateToOgr(tags, elementType, geometryType) 
{ 
    var attrs = {};
    var tableName;

    if ('name' in tags)
    {
        attrs['NAM'] = tags['name'];
    }

    if (tags['highway'] == 'track')
    {
        tableName = 'LAP010';
        attrs['ARA'] = -999999;
        attrs['PCF'] = 1;
    }
    else if (tags['highway'] == 'road')
    {
        tableName = 'LAP030';
        attrs['PCF'] = 2;
        attrs['LTN'] = 2;
        attrs['ARA'] = -999999;
    }
    else if (tags['building'] == 'yes')
    {
        if (geometryType == 'Point')
        {
            tableName = 'PAL015';
            attrs['ARA'] = -999999;
        }
        else if (geometryType == 'Area')
        {
            tableName = 'AAL015';
            // bogus testing value.
            attrs['ARA'] = 10;
        }
    }
    // drop all primary roads.
    else if (tags['highway'] == 'primary')
    {
        return undefined;
    }
    // lets me test any combination of tags.
    else
    {
        attrs = tags;
        tableName = attrs["tableName"]
        delete attrs["tableName"]
    }

    return { attrs: attrs, tableName: tableName };
}

function translateToOsm() { return {}; }
function getDbSchema()
{
    var schema = [];

    var nam = {
        name:'NAM',
        'type':'String'
    };

    var ara = {
        'name':'ARA',
        'type':'double',
        'minimum':0,
        'enumerations':[
            { 'name':'No Information', 'value':-999999 }
        ]
    };

    var pcf = {
        'name':'PCF',
        'defValue':-999999, // No Information
        'type':'enumeration',
        'enumerations':[
            { 'name':'No Information', 'value':-999999 },
            { 'name':'Raised', 'value':1 },
            { 'name':'Level', 'value':2 }
        ]
    };

    var ltn = {
        'name':'LTN',
        'type':'Long Integer',
        'defValue':1,
        'minimum':1,
        'maximum':99
    };

    var lap010 = {
        'name':'LAP010',
        'geom':'Line',
        'columns': [nam,ara,pcf]
    };

    var lap020 = {
        'name':'LAP020',
        geom:'Line',
        'columns': [nam,ara,pcf,ltn]
    };

    var lap030 = {
        'name':'LAP030',
        geom:'Line',
        'columns': [nam,ara,pcf,ltn]
    };

    var aal015 = {
        name: 'AAL015',
        geom: 'area',
        columns: [nam,ara]
    };

    var pal015 = {
        name: 'PAL015',
        geom: 'point',
        columns: [nam,ara]
    };

    schema.push(aal015);
    schema.push(lap010);
    schema.push(lap020);
    schema.push(lap030);
    schema.push(pal015);

    return schema;
}
