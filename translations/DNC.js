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
 * @copyright Copyright (C) 2018,2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert DNC to/from OSM+
//

hoot.require('dnc_core');
hoot.require('dnc_rules');
hoot.require('dnc_schema');
hoot.require('config');
hoot.require('translate');
hoot.require('fcode_common');

function initialize()
{
  // Set the schema type for the export
  hoot.Settings.set({'osm.map.writer.schema':'DNC'});
  hoot.Settings.set({'ogr.esri.fdname':''});

  // We need this to avoid errors with int & LongLong
  // It really needs to be set on the commandline not here
  hoot.Settings.set({'ogr.strict.checking':'off'});
}

// Layer name filter - Filter out all layers that match this regexp
function layerNameFilter()
{
  // Drop all of the tileref and libref layers
  return '^(?!tileref_|libref_)';
}

// Create the output Schema
function getDbSchema()
{
  return dnc.getDbSchema();
}

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
// function translateAttributes(attrs, layerName)
function translateToOsm(attrs, layerName, geometryType)
{
  return dnc.toOsm(attrs, layerName, geometryType);

} // End of Translate Attributes

// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
  return dnc.toOgr(tags, elementType, geometryType);

} // End of translateToOgr
