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
// Convert MapEdit RenderDb to TDSv61
//

hoot.require('tds61');
hoot.require('tds61_schema');
hoot.require('tds61_rules');
hoot.require('config');
hoot.require('translate');
hoot.require('fcode_common');

// Layer name filter - Filter out all layers that match this regexp
function layerNameFilter()
{
    // Drop all of the "SRC_*" and "o2s_*" layers
    return "^(?!SRC_|o2s_)";
}


// Create the output Schema
function getDbSchema()
{
    return tds61.getDbSchema();
}


// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
// function translateAttributes(attrs, layerName)
function translateToOsm(attrs, layerName)
{
    return tds61.toOsm(attrs, layerName);

} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
    // print('Going to OGR: eType:' + elementType + '  gType:' + geometryType);

    if (tags.tags)
    {
        // The "tags" value is a string with this structure: "cables"=>"3", "voltage"=>"230000"
        // Trying to split on ", instead of just , due to having comma's inside the data values. Arrrggghhh!
        var tList = tags['tags'].split('\",');

        for (var i = 0, tLen = tList.length; i < tLen; i++)
        {
            var rTag = tList[i].replace(/\"/g,'').split('=>');
            tags[rTag[0].trim()] = rTag[1].trim();
        }

        delete tags.tags;
    }

    return tds61.toNfdd(tags, elementType, geometryType)

} // End of translateToOgr

