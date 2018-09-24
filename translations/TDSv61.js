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
// Convert TDSv61 to/from OSM+
//
// NOTE: This file wil go away when the Hoot services get refactored to use 
//       the XXX_to_OSM and XXX_to_OGR translation scripts 
//


// Create the output Schema
function getDbSchema()
{
	print('tds61 getDbSchema');
    if (typeof buildTds61Schema == 'undefined')
    {
		print('Require: tds61 Schema');
        hoot.require('TDSv61_dbSchema')
    }
	print('tds61 Back from Require Schema');

    return buildTds61Schema();
}


// IMPORT
// translateToOsm - takes 'attrs' and returns OSM 'tags'
function translateToOsm(attrs, layerName, geometryType)
{
    if (typeof tds61 == 'undefined')
    {
        hoot.require('TDSv61_to_OSM')
    }
 
    return tds61.toOsm(attrs, layerName, geometryType);
} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
	print('translateToOgr');
	// NOTE: getDbSchema() will init am empty tds61 object
    if (tds61.toOgr == undefined)
    {
	print('Require: translateToOgr');
        hoot.require('TDSv61_to_OGR')
    }

    return tds61.toOgr(tags, elementType, geometryType)
} // End of translateToOgr

