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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

//
// Convert TDSv70 to/from OSM+
//

// include HootJs if loaded from mocha test
if (typeof hoot === 'undefined') {
  var hoot = require(process.env.HOOT_HOME + '/lib/HootJs');
}

hoot.require('SchemaTools');
hoot.require('tds70');
hoot.require('tds70_schema');
hoot.require('tds70_rules');
hoot.require('translate');
hoot.require('fcode_common');

function initialize()
{
  // Set the schema type for the export
  hoot.Settings.set({'map.writer.schema':'TDSv70'});
}

// Layer name filter - Filter out all layers that match this regexp
function layerNameFilter()
{
  // Drop all of the "SRC_*", "o2s_*" and "extra_*" layers
  return '^(?!SRC_|extra_)';
}


// Create the output Schema
function getDbSchema()
{
  return tds70.getDbSchema();
}


// IMPORT
// translateToOsm - takes 'attrs' and returns OSM 'tags'
function translateToOsm(attrs, layerName, geometryType)
{
  return tds70.toOsm(attrs, layerName, geometryType);

} // End of translateToOsm


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
  return tds70.toOgr(tags, elementType, geometryType);

} // End of translateToOgr


// module method for use in Translation Server
if (typeof exports !== 'undefined') {
  exports.toOsm = tds70.toOsm;
}
