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
 * @copyright Copyright (C) 2013, 2014, 2021 Maxar (http://www.maxar.com/)
 */

//
// MUVD Conversion
//

// include HootJs if loaded from mocha test
if (typeof hoot === 'undefined') {
    var hoot = require(process.env.HOOT_HOME + '/lib/HootJs');
}

// hoot.require('config')
hoot.require('muvd')
hoot.require('muvd_schema')
hoot.require('muvd_rules')
hoot.require('fcode_common')
hoot.require('translate');

function initialize()
{
    // Set the schema type for the export
    hoot.Settings.set({"map.writer.schema":"MUVD"});
}

// Set up the Schema for export
function getDbSchema()
{
    // return muvd.schema.getDbSchema();
    return muvd.getDbSchema();
}

// IMPORT
// translateToOsm - takes 'attrs' and returns OSM 'tags'
function translateToOsm(attrs, layerName, geometryType)
{
    return muvd.toOsm(attrs, layerName, geometryType);
} // End of translateToOsm

// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + layername
function translateToOgr(tags, elementType, geometryType)
{
    return muvd.toOgr(tags, elementType, geometryType)
} // End of translateToOgr

// module method for use in Translation Server
if (typeof exports !== 'undefined') {
    exports.toOsm = muvd.toOsm;
}