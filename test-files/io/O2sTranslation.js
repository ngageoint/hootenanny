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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// This translation file converts out to Out of spec layers.
//

// Layer filter test - Test passing a string from Javascript back to Hoot
function layerNameFilter()
{
    return "Papa Smurf";
}

function translateToOgr(tags, elementType, geometryType) 
{ 
    var attrs = {};
    var tableName;

    attrs = {};
    // These times are irrelevant to us and constantly changing.
    delete tags['source:ingest:datetime'];
    if (geometryType == 'Point') {
        tableName = 'o2s_p';
        attrs.tags = JSON.stringify(tags);
        hoot.log(tags);
    } else if (geometryType == 'Line') {
        tableName = 'o2s_l';
        attrs.tags = JSON.stringify(tags);
        hoot.log(tags);
    } else if (geometryType == 'Area') {
        tableName = 'o2s_a';
        attrs.tags = JSON.stringify(tags);
        hoot.log(tags);
    } else {
        throw "Unexpected geometry type or bad table name.";
    }

    return { attrs: attrs, tableName: tableName };
}

function translateToOsm() { return {}; }
function getDbSchema()
{
    var schema = [];

    var tags = {
        'name':'tags',
        'type':'String',
        'length':'254',
    };

    var o2s_p = {
        name: 'o2s_p',
        geom: 'point',
        columns: [tags]
    };

    var o2s_l = {
        name: 'o2s_l',
        geom: 'line',
        columns: [tags]
    };

    var o2s_a = {
        name: 'o2s_a',
        geom: 'area',
        columns: [tags]
    };

    schema.push(o2s_p);
    schema.push(o2s_l);
    schema.push(o2s_a);

    return schema;
}
