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
// MGCP Conversion
//

hoot.require('config')
hoot.require('mgcp')
hoot.require('mgcp_schema')
hoot.require('mgcp_rules')
hoot.require('fcode_common')
hoot.require('translate');


// Set up the Schema for export
function getDbSchema()
{
    // return mgcp.schema.getDbSchema();
    return mgcp.getDbSchema();
}


// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateAttributes(attrs, layerName, geometryType)
{
    return mgcp.toOsm(attrs, layerName, geometryType);
} // End of TranslateAttributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + layername
function translateToOgr(tags, elementType, geometryType)
{
    return mgcp.toMgcp(tags, elementType, geometryType)

} // End of translateToOgr


