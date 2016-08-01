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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert TDSv61 to/from OSM+
//

hoot.require('SchemaTools');
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
// function translateAttributes(attrs, layerName, geometryType)
function translateToOsm(attrs, layerName, geometryType)
{
    return tds61.toOsm(attrs, layerName, geometryType);

} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
    return tds61.toNfdd(tags, elementType, geometryType)

} // End of translateToOgr

