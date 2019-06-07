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
// GGDMv30 Conversion - With "English" output
//

// For the OSM+ to NFDD translation
hoot.require('SchemaTools')
hoot.require('ggdm30')
hoot.require('ggdm30_schema')
hoot.require('ggdm30_rules')
hoot.require('fcode_common')

// The main translation functions
hoot.require('eggdm30')
hoot.require('eggdm30_rules')

hoot.require('config')
hoot.require('translate')

// Create the output Schema - This is not used here. We _do_ use it in the etds functions but
// we don't need to expose it to the main Hoot program
function getDbSchema()
{
     return ggdm30.getDbSchema();
}

// Get raw schema directly from translations/schema
function getRawDbSchema()
{
     return ggdm30.schema.getDbSchema();
}


function initialize()
{
    // Turn off the TDS structure so we just get the raw feature
    hoot.Settings.set({"ogr.thematic.structure":"false"});

        // Turn off the tds extra function
    hoot.Settings.set({"ogr.note.extra":"none"});

    // Turn off the ESRI FCSUBTYPE
    hoot.Settings.set({"ogr.esri.fcsubtype":"false"});

    // Throw errors instead of returning partial translations/o2s_X features
    hoot.Settings.set({"ogr.throw.error":"true"});

    // Set the schema type for the export
    hoot.Settings.set({"osm.map.writer.schema":"GGDMv30"});

}


// IMPORT
// translateAttributes - Normally takes 'attrs' and returns OSM 'tags'.  This version
//    converts OSM+ tags to NFDD "English" Attributes
//
// This can be called via the following for testing:
// hoot convert -D "convert.ops=hoot::TranslationOp"  \
//      -D schema.translation.script=$HOOT_HOME/translations/OSM_to_englishTDS.js <input>.osm <output>.osm
//
function translateAttributes(attrs, layerName, geometryType)
{
    // We use the temp var because etds.toEnglish returns "attrs" and "tableName"
    var output = eggdm30.toEnglish(attrs,layerName,geometryType);

    // Make sure the returned value isn't NULL. This does occur
    if (output)
    {
        return output.attrs;
    }
    else
    {
        return null;
    }
} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
//    This version converts OSM+ tags to NFDD "English" attributes
function translateToOgr(tags, elementType, geometryType)
{
        return eggdm30.toEnglish(tags, elementType, geometryType)
} // End of translateToOgr





