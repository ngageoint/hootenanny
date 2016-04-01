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

//
// If you have additional columns you want in the output just add them here.
//
possibleTags = ['name', 'alt_name', 'amenity', 'poi', 'place', 'waterway', 'natural', 'man_made', 'landuse', 
  'leisure', 'highway', 'building', 'office', 'bridge', 'shop', 'mountain_pass', 'slope', 'source', 'uid'];

//
// Exports the tags that are in the list above.
//
function translateToOgr(tags, elementType, geometryType) 
{ 
    var attrs = {};

    if (elementType != 'Node')
    {
        return null;
    }

    for (var i = 0; i < possibleTags.length; i++)
    {
        var k = possibleTags[i];
        if (k in tags) 
        {
            attrs[k] = tags[k];
        }
    }

    return { attrs: attrs, tableName: "poi" };
}

//
// Keeps all columns in the CSV file.
//
function translateToOsm(attrs, layerName, geometryType)
{ 
    return attrs;
}

//
// This returns a schema for a subset of the NFDDv4 LAP030 (road) columns.
//
function getDbSchema()
{
    var columns = [];
    for (var i = 0; i < possibleTags.length; i++)
    {
        var k = possibleTags[i];
        var c = {}
        c.name = k;
        c.type = "String";
        c.defValue = '';
        columns.push(c);
    }

    var schema = [
        poi = {
            name: 'poi',
            geom: 'Point',
            columns: columns
        }
    ];

    return schema;
}
