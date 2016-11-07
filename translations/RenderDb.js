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
 * ' * @copyright Copyright ...'
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert RenderDb to FGDB & Shapefile
//

hoot.require('config');
hoot.require('translate');

function initialize()
{
    var _global = (0, eval)('this');

    // If it exists, we assume we are good.....
    if (!_global.renderDb)
    {
        _global.renderDb = {}

        var schema = getDbSchema();

        // Loop through the schema and make a list of the field names
        for (var i = 0, schemaLen = schema.length; i < schemaLen; i++)
        {
            var fName = schema[i].name;

            _global.renderDb[fName] = [];

            for (var j = 0, columnLen = schema[i].columns.length; j < columnLen; j++)
            {
                _global.renderDb[fName].push(schema[i].columns[j].name);
            } // End for j
        } // End for i

    } // End !_global

} // End Initialize

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateToOsm(attrs, layerName, geometryType)
{
    return attrs;
} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
    // Debug:
    if (config.getOgrDebugDumptags() == 'true')
    {
        print('In Geometry: ' + geometryType + '  In Element Type: ' + elementType);
        for (var i in tags) print('In Tags :' + i + ': :' + tags[i] + ':');
        print('');
    }

    // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.
    // There is no way we can translate these to a single geometry.
    if (geometryType == 'Collection') return null;

    // Drop non usefull hoot tags.
    if (tags['hoot:status'] == 'invalid')
    {
        delete tags['hoot:status'];
    }


    // Clean out the Tags
    for (var i in tags)
    {
        // Drop Hoot specific stuff
        // if (i.indexOf('hoot:') !== -1 || i.indexOf('error:') !== -1)
        if (i.indexOf('error:') !== -1)
        {
            delete tags[i];
            continue;
        }


        // Look for the tag in the schema lookup tables. We assume that the initialize function has been run.
        if (renderDb[geometryType].indexOf(i) == -1)
        {
            // Add the value to the "tags" tags with this format:
            // "operator_type"=>"private_religious", "error:circular"=>"15", "school:first_cycle"=>"yes", "school:preschool_cycle"=>"yes"
            if (tags.tags)
            {
                tags.tags = tags.tags + ',\"' + i + '\"=>\"' + tags[i] + '\"';
            }
            else
            {
                tags.tags = '\"' + i + '\"=>\"' + tags[i] + '\"';
            }

            delete tags[i];
        }
    }

    // Debug:
    if (config.getOgrDebugDumptags() == 'true')
    {
        for (var i in tags) print('Out Tags :' + i + ': :' + tags[i] + ':');
        print('');
    }

    return [{attrs:tags, tableName: geometryType}];

} // End of translateToOgr


// Create the output Schema
function getDbSchema()
{
    var common_list = [
        { name:'access',desc:'access',type:'String',defValue:'' },
        { name:'addr:housename',desc:'addr:housename',type:'String',defValue:'' },
        { name:'addr:housenumber',desc:'addr:housenumber',type:'String',defValue:'' },
        { name:'addr:interpolation',desc:'addr:interpolation',type:'String',defValue:'' },
        { name:'admin_level',desc:'admin_level',type:'String',defValue:'' },
        { name:'aerialway',desc:'aerialway',type:'String',defValue:'' },
        { name:'aeroway',desc:'aeroway',type:'String',defValue:'' },
        { name:'amenity',desc:'amenity',type:'String',defValue:'' },
        { name:'area',desc:'area',type:'String',defValue:'' },
        { name:'barrier',desc:'barrier',type:'String',defValue:'' },
        { name:'bicycle',desc:'bicycle',type:'String',defValue:'' },
        { name:'boundary',desc:'boundary',type:'String',defValue:'' },
        { name:'brand',desc:'brand',type:'String',defValue:'' },
        { name:'bridge',desc:'bridge',type:'String',defValue:'' },
        { name:'building',desc:'building',type:'String',defValue:'' },
        { name:'construction',desc:'construction',type:'String',defValue:'' },
        { name:'covered',desc:'covered',type:'String',defValue:'' },
        { name:'culvert',desc:'culvert',type:'String',defValue:'' },
        { name:'cutting',desc:'cutting',type:'String',defValue:'' },
        { name:'denomination',desc:'denomination',type:'String',defValue:'' },
        { name:'disused',desc:'disused',type:'String',defValue:'' },
        { name:'embankment',desc:'embankment',type:'String',defValue:'' },
        { name:'foot',desc:'foot',type:'String',defValue:'' },
        { name:'generator:source',desc:'generator:source',type:'String',defValue:'' },
        { name:'harbour',desc:'harbour',type:'String',defValue:'' },
        { name:'highway',desc:'highway',type:'String',defValue:'' },
        { name:'historic',desc:'historic',type:'String',defValue:'' },
        { name:'horse',desc:'horse',type:'String',defValue:'' },
        { name:'intermittent',desc:'intermittent',type:'String',defValue:'' },
        { name:'junction',desc:'junction',type:'String',defValue:'' },
        { name:'landuse',desc:'landuse',type:'String',defValue:'' },
        { name:'layer',desc:'layer',type:'String',defValue:'' },
        { name:'leisure',desc:'leisure',type:'String',defValue:'' },
        { name:'lock',desc:'lock',type:'String',defValue:'' },
        { name:'man_made',desc:'man_made',type:'String',defValue:'' },
        { name:'military',desc:'military',type:'String',defValue:'' },
        { name:'motorcar',desc:'motorcar',type:'String',defValue:'' },
        { name:'name',desc:'name',type:'String',defValue:'' },
        { name:'natural',desc:'natural',type:'String',defValue:'' },
        { name:'office',desc:'office',type:'String',defValue:'' },
        { name:'oneway',desc:'oneway',type:'String',defValue:'' },
        { name:'operator',desc:'operator',type:'String',defValue:'' },
        { name:'osm_id',desc:'osm_id',type:'Integer',defValue:'-999999' },
        { name:'place',desc:'place',type:'String',defValue:'' },
        { name:'population',desc:'population',type:'String',defValue:'' },
        { name:'power',desc:'power',type:'String',defValue:'' },
        { name:'power_source',desc:'power_source',type:'String',defValue:'' },
        { name:'public_transport',desc:'public_transport',type:'String',defValue:'' },
        { name:'railway',desc:'railway',type:'String',defValue:'' },
        { name:'ref',desc:'ref',type:'String',defValue:'' },
        { name:'religion',desc:'religion',type:'String',defValue:'' },
        { name:'route',desc:'route',type:'String',defValue:'' },
        { name:'service',desc:'service',type:'String',defValue:'' },
        { name:'shop',desc:'shop',type:'String',defValue:'' },
        { name:'sport',desc:'sport',type:'String',defValue:'' },
        { name:'surface',desc:'surface',type:'String',defValue:'' },
        { name:'tags',desc:'tags',type:'String',defValue:'' },
        { name:'toll',desc:'toll',type:'String',defValue:'' },
        { name:'tourism',desc:'tourism',type:'String',defValue:'' },
        { name:'tower:type',desc:'tower:type',type:'String',defValue:'' },
        { name:'tunnel',desc:'tunnel',type:'String',defValue:'' },
        { name:'water',desc:'water',type:'String',defValue:'' },
        { name:'waterway',desc:'waterway',type:'String',defValue:'' },
        { name:'wetland',desc:'wetland',type:'String',defValue:'' },
        { name:'width',desc:'width',type:'String',defValue:'' },
        { name:'wood',desc:'wood',type:'String',defValue:'' },
        { name:'z_order',desc:'z_order',type:'Integer',defValue:'-999999' }
    ];

    var extraOtherList = [
        { name:'tracktype',desc:'tracktype',type:'String',defValue:'' },
        { name:'way_area',desc:'way_area',type:'Real',defValue:'-999999.0' },
        ];

    var extraPointList = [
        { name:'capital',desc:'capital',type:'String',defValue:'' },
        { name:'ele',desc:'ele',type:'String',defValue:'' },
        { name:'poi',desc:'poi',type:'String',defValue:'' }
        ];

    var schema = [];

    // Add the Line & Polygon specific features
    var tList = [];
    tList.push.apply(tList,common_list);
    tList.push.apply(tList,extraOtherList);

    // Line Layer
    schema.push({ name: 'Line',
                  desc: 'Line',
                  geom: 'Line',
                  columns:tList
                });

    // Polygon Layer
    schema.push({ name: 'Area',
                  desc: 'Area',
                  geom: 'Area',
                  columns:tList
                });

    // Point layer
    // Add the Point layer specific features
    var pList = [];
    pList.push.apply(pList,common_list);
    pList.push.apply(pList,extraPointList);

    schema.push({ name: 'Point',
                  desc: 'Point',
                  geom: 'Point',
                  columns:pList
                });

    // Debug
    // translate.dumpSchema(schema);

    return schema;

} // End of GetDBSchema