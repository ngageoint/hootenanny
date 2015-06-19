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

//
// Basic NFDD Testing File
//

hoot.require('nfdd')
hoot.require('nfdd.schema')

function getDbSchema()
{
    return nfdd.getDbSchema();
}

// EXPORT
// translateToOgr - takes OSM tags + geometry and returns attributes + layername
function translateToOgr(tags, elementType, geometryType)
{
    var attrs = {}, tableName = '';

    attrs.FCODE = tags.FCODE;
    for (var i=0; i < schema.length; i++)
    {
        if (schema[i].fcode == attrs.FCODE && schema[i].geom == geometryType)
        {
            tableName = schema[i].name
        }
    }

    print('TableName: ' + tableName);

    return {attrs: attrs, tableName: tableName};
    // return nfdd.toNfdd(tags, elementType, geometryType)

} // End of translateToOgr


// IMPORT
// TranslateAttributes converts from Attributes to OSM tags
function translateAttributes(attrs, layerName)
{
    var tags = {};

    tags.FCODE = attrs.F_CODE;

    return tags;

    // return nfdd.toOsm(attrs, layerName);

} // End of translateAttributes


// ##### Not Used #####
// If we call nfdd.toNfdd directly, do we really need this?
function translateTags(tags, elementType)
{
    return nfdd.toNfdd(tags, elementType)

} // End of translateTags

