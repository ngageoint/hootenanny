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
// GAIT Exported Shapefile Conversion
//
// Very basic....
//


// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
// function translateAttributes(attrs, layerName)
function translateToOsm(attrs, layerName, geometryType)
{
    tags = {};

    tags['gait:error_type'] = attrs.ERRTYPE;
    tags['gait:magnitude'] = attrs.MAGNITUDE;
    tags['gait:instance'] = attrs.INSTANCE;
    tags['gait:condition_number'] = attrs.COND_NUM;
    tags['gait:fcode1'] = attrs.CODE1;
    tags['gait:fcode2'] = attrs.CODE2;
    tags['gait:label1'] = attrs.LABEL1;
    tags['gait:label2'] = attrs.LABEL2;
    tags['gait:annotation'] = attrs.ANNOTATION;
    tags['gait:geometry_type'] = attrs.ORIG_GEOM;
    tags['gait:uuid'] = attrs.UFI;
    tags['gait:schema'] = attrs.ATTRSCHEMA;
    tags['gait:source_table'] = attrs.ORIG_TABLE;

    return tags;
} // End of Translate Attributes


