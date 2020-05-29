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

// Shorten some of the names when exporting to shapefile
var shpSwapList = {
  'construction':'constructn',
  'denomination':'denominatn',
  'description':'descript',
  'generator:source':'gen_source',
  'intermittent':'intermit',
  'power_source':'pwr_source',
  'public_transport':'public_tpt',
  'tower:type':'tower_type',
  'addr:housename':'house_name',
  'addr:housenumber':'house_num',
  'addr:interpolation':'addr_intrp',
  'admin_level':'admin_lvl',
  'source:imagery:datetime':'imagery_dt',
  'source:imagery:id':'imagery_id',
};

var gdbSwapList = {
  'generator:source':'generator_source',
  'tower:type':'tower_type',
  'addr:housename':'addr_housename',
  'addr:housenumber':'addr_housenumber',
  'addr:interpolation':'addr_interpolation',
  'source:imagery:datetime':'source_imagery_datetime',
  'source:imagery:id':'source_imagery_id',
};

var inputSwapList = Object.keys(shpSwapList).reduce(function(map, key) {
  map[shpSwapList[key]] = key;
  return map;
}, {});

inputSwapList = Object.keys(gdbSwapList).reduce(function(map, key) {
  map[gdbSwapList[key]] = key;
  return map;
}, inputSwapList);


function initialize()
{
  var _global = (0, eval)('this');

  // If it exists, we assume we are good.....
  if (!_global.renderDb)
  {
    _global.renderDb = {};

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

    // Grab the config variables once so we don't have to keep calling into the core
    _global.renderDb.config = {};
    _global.renderDb.config.OgrDebugDumptags = config.getOgrDebugDumptags();
    _global.renderDb.config.outputFormat = config.getOgrOutputFormat();
    _global.renderDb.config.toChange = config.getSchemaTranslationOverride();
  } // End !_global

} // End Initialize

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateToOsm(attrs, layerName, geometryType)
{
  // Debug
  if (renderDb.config.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In Attrs: ');

  // Translation overrides if set
  if (renderDb.config.toChange) attrs = translate.overrideValues(attrs, renderDb.config.toChange);

  for (var i in attrs)
  {
    if (i in inputSwapList)
    {
      attrs[inputSwapList[i]] = attrs[i];
      delete attrs[i];
    }
  }


  // var nameList = ['tags','tags2','tags3','tags4','tags5'];
  // for (var tName of nameList)
  // {
  //   if (attrs[tName])
  //   {
  //     var tList = attrs[tName].split('","');
  //     delete attrs[tName];

  //     for (var val in tList)
  //     {
  //       vList = tList[val].split('"=>"');
  //       attrs[vList[0].toString().replace('"','')] = vList[1].toString().replace('"','');

  //       // Debug
  //       // print('val :' + tList[val] + ':  vList[0] :' + vList[0] + ':  vList[1] :' + vList[1] + ':');
  //     }
  //   }
  // }

  // Now split all of the tagsX tags into separate tags.
  ['tags','tags2','tags3','tags4','tags5'].forEach(function(tName) {
    if (attrs[tName])
    {
      var tList = attrs[tName].split('","');
      delete attrs[tName];

      for (var val in tList)
      {
        vList = tList[val].split('"=>"');
        attrs[vList[0].toString().replace('"','')] = vList[1].toString().replace('"','');

        // Debug
        // print('val :' + tList[val] + ':  vList[0] :' + vList[0] + ':  vList[1] :' + vList[1] + ':');
      }
    }
  });


  // hoot:id vs osm:id is an issue.
  // If we don't preserve id's on import, osm:id could be positive and hoot:id becomes negative.
  // Leaving this commented out at the moment.
  // if (attrs.osm_id && !(attrs['hoot:id']))
  // {
  //   attrs['hoot:id'] = attrs.osm_id;
  //   delete attrs.osm_id;
  // }

  // Debug:
  if (renderDb.config.OgrDebugDumptags == 'true')
  {
    translate.debugOutput(attrs,layerName,geometryType,'','Out tags: ');
    print('');
  }

  return attrs;
} // End of Translate Attributes


// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
  if (renderDb.config.OgrDebugDumptags == 'true') translate.debugOutput(tags,'',geometryType,elementType,'In tags: ');

  // Translation overrides if set
  if (renderDb.config.toChange) tags = translate.overrideValues(tags, renderDb.config.toChange);

  // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.
  // There is no way we can translate these to a single geometry.
  if (geometryType == 'Collection') return null;

  if (tags['hoot:id'] && !(tags.osm_id))
  {
    tags.osm_id = tags['hoot:id'];
    delete tags['hoot:id'];
  }

  // Drop non usefull/metadata hoot tags.
  if (tags['hoot:status'] == 'invalid' || tags['hoot:status'] == 'Invalid') delete tags['hoot:status'];
  if (tags['hoot:layername']) delete tags['hoot:layername'];
  if (tags['source:ingest:datetime']) delete tags['source:ingest:datetime'];

  var tagsList = [];
  // Sort out what tags go into the "tags" attribute and what gets it's own column
  for (var i in tags)
  {
    // Drop Hoot specific stuff
    // if (i.indexOf('hoot:') !== -1 || i.indexOf('error:') !== -1)

    if (i.indexOf('error:') !== -1)
    {
      delete tags[i];
      continue;
    }

    // Swap names if needed
    if (renderDb.config.outputFormat == 'shp' && i in shpSwapList)
    {
      tags[shpSwapList[i]] = tags[i];
      delete tags[i];
      continue;
    }
    // Swap names if needed
    if (renderDb.config.outputFormat == 'gdb' && i in gdbSwapList)
    {
      tags[gdbSwapList[i]] = tags[i];
      delete tags[i];
      continue;
    }

    // Look for the tag in the schema lookup tables. We assume that the initialize function has been run....
    if (renderDb[geometryType].indexOf(i) == -1)
    {
      tagsList.push('\"' + i + '\"=>\"' + tags[i] + '\"');
      delete tags[i];
    }
  }

  // Find how much text we need to split and store
  tagsLength = 0;
  for (var i = tagsList.length - 1; i >= 0; --i)
  {
    // Debug
    // print('len: ' + tagsList[i].length + '  tag: ' + tagsList[i]);
    if (tagsList[i].length > 254)
    {
      hoot.logError('Tag : ' + tagsList[i] + ' : is too long to be stored (' + tagsList[i].length + ' characters). Dropping the tag');
      tagsList.splice(i,1);
      continue
    }

    tagsLength += tagsList[i].length;
  }

  // Set this so it can be used by everything except shapefiles
  // If we export to spapefile, this will get changed.
  tags.tags = tagsList.join();

  // Shapefile slice and dice time
  if (renderDb.config.outputFormat == 'shp' && tagsLength > 254)
  {
    var nameList = ['tags','tags2','tags3','tags4','tags5'];

    for (var nam = 0, nLen = nameList.length; nam < nLen; nam++)
    {
      var tName = nameList[nam];
      // Debug
      // print('tName: ' + tName + '  len:' + tagsList.length);

      if (tagsList.length == 0) break;

      tags[tName] = tagsList.pop();

      // Loop through the list of tags and try to append them
      for (var i = 0; i < tagsList.length; i++)
      {
        if ((1 + tagsList[i].length + tags[tName].length) < 254)
        {
          tags[tName] += ',' + tagsList[i];
          tagsList.splice(i,1);
        }
      }
    } // End tName

  // Sanity check. If there is anything left in the queue print an error
    for (var i = 0; i < tagsList.length; i++) hoot.logError('Unable to store: ' + tagsList[i] + ' : in the tags attribute');

  } // End shp && tagLength

  // Debug:
  if (renderDb.config.OgrDebugDumptags == 'true')
  {
    translate.debugOutput(tags,'',geometryType,elementType,'Out attrs: ');
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
    { name:'description',desc:'description',type:'String',defValue:'' },
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
    { name:'osm_id',desc:'osm_id',type:'Long Integer',defValue:'-999999' },
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
    { name:'source:imagery:datetime',desc:'source:imagery:datetime',type:'String',defValue:'' },
    { name:'source:imagery:id',desc:'source:imagery:id',type:'String',defValue:'' },
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

  // Debug
  // print('OutputFormat = :' + config.getOgrOutputFormat() + ':')

  // Shapefiles can only have 10 character attribute names
  // This is Ugly and is repeated since getDbSchema gets used before initialise() and the translation functions
  if (config.getOgrOutputFormat() == 'shp')
  {

    // This is brute force and ugly but the list is an array of objects
    for (var i = 0, cLen = common_list.length; i < cLen; i++)
    {
      if (common_list[i].name in shpSwapList)
      {
        common_list[i].name = shpSwapList[common_list[i].name]
      }
    }

    // Now add a few more Tags attributes su we can export upto 1K of text
    common_list.push({name:'tags2',desc:'tags2',type:'String',defValue:''});
    common_list.push({name:'tags3',desc:'tags3',type:'String',defValue:''});
    common_list.push({name:'tags4',desc:'tags4',type:'String',defValue:''});
    common_list.push({name:'tags5',desc:'tags4',type:'String',defValue:''});
  }

  // GDB doesn't likr ":" in attribute names
  if (config.getOgrOutputFormat() == 'gdb')
  {

    // This is brute force and ugly but the list is an array of objects
    for (var i = 0, cLen = common_list.length; i < cLen; i++)
    {
      if (common_list[i].name in gdbSwapList)
      {
        common_list[i].name = gdbSwapList[common_list[i].name]
      }
    }
  }

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
