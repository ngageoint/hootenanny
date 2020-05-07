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

/*
    GIFD D4 to OSM+ conversion script
*/

gifd = {
  // ##### Start of the xxToOsmxx Block #####
  applyToOsmPreProcessing: function(attrs, layerName, geometryType)
  {
    // #####
    // This is from MGCP & some TDS
    // #####

    // Drop the FCSUBTYPE since we don't use it
    if (attrs.FCSUBTYPE) delete attrs.FCSUBTYPE;

    // List of data values to drop/ignore
    var ignoreList = { '-32765.0':1, '-32767':1, 'noinformation':1, 'unk':1, 'unknown':1, '997':1 };

    // This is a handy loop. We use it to:
    // 1) Remove all of the "No Information" and -999999 fields
    // 2) Convert all of the Attrs to uppercase - if needed
    for (var col in attrs)
    {
      // slightly ugly but we would like to account for: 'No Information', 'noInformation' etc
      // First, push to lowercase
      var attrValue = attrs[col].toString().toLowerCase();

      // Get rid of the spaces in the text
      attrValue = attrValue.replace(/\s/g, '');

      // Wipe out the useless values
      if (attrs[col] == '' || attrValue in ignoreList || attrs[col] in ignoreList)
      {
        delete attrs[col]; // debug: Comment this out to leave all of the No Info stuff in for testing
        continue;
      }

      // Push the attribute to upper case - if needed
      var c = col.toUpperCase();
      if (c !== col)
      {
        attrs[c] = attrs[col];
        delete attrs[col];
        col = c; // For the rest of this loop iteration
      }

    } // End in attrs loop

    // Now find an F_CODE
    if (attrs.F_CODE)
    {
      // Drop the the "Not Found" F_CODE. This is from the UI
      if (attrs.F_CODE == 'Not found') delete attrs.F_CODE;
    }
    else if (attrs.FCODE)
    {
      attrs.F_CODE = attrs.FCODE;
      delete attrs.FCODE;
    }
    else
    {
      // Time to find an FCODE based on the filename
      var fCodeMap = [
        ['AF010', ['af010','smokestack_p']], // Smokestack
        ['AH025', ['ah025','engineered_earthwork_s','engineered_earthwork_p']], // Engineered Earthwork
        ['AH060', ['ah060','underground_bunker_s','underground_bunker_p']], // Underground Bunker
        ['AL010', ['al010','facility_s','facility_p']], // Facility
        ['AL013', ['al013','building_s','building_p']], // Building
        ['AL018', ['al018','building_superstructure_s','building_superstructure_c','building_superstructure_p']], // Building Superstructure
        ['AL020', ['al020','built-up_area_s','built-up_area_p']], // Built up area
        ['AL030', ['al030','cemetery_s','cemetery_p']], // Cemetary
        ['AL070', ['al070','fence_c']], // Fence
        ['AL099', ['al099','hut_p']], // Hut
        ['AL105', ['al105','settlement_s','settlement_p']], // Settlement
        ['AL130', ['al130','memorial_monument_s','memorial_monument_p']], // Memorial Monument
        ['AL200', ['al200','ruins_s','ruins_p']], // Ruins
        ['AL208', ['al208','shanty_town_s','shanty_town_p']], // Shanty Town
        ['AL241', ['al241','tower_s','tower_p']], // Tower
        ['AL260', ['al260','wall_c']], // Wall
        ['AM080', ['am080','water_tower_p','water_tower_s']], // Water Tower
        ['AN010', ['an010','railway_c']], // Railway
        ['AN050', ['an050','railway_sidetrack_c']], // Railway Sidetrack
        ['AN060', ['an060','railway_yard_s']], // Railway Yard
        ['AN075', ['an075','railway_turntable_p','railway_turntable_p']], // Railway Turntable
        ['AN076', ['an076','roundhouse_s','roundhouse_p']], // Roundhouse
        ['AP010', ['ap010','cart_track_c']], // Cart Track
        ['AP020', ['ap020','road_interchange_p']], // Interchange
        ['AP030', ['ap030','road_c']], // Road
        ['AP040', ['ap040','gate_c','gate_p']], // Gate
        ['AP041', ['ap041','vehicle_barrier_c','vehicle_barrier_p']], // Vehicle Barrier
        ['AP050', ['ap050','trail_c']], // Trail
        ['AQ040', ['aq040','bridge_c','bridge_p']], // Bridge
        ['AQ045', ['aq045','bridge_span_c','bridge_span_p']], // Bridge Span
        ['AQ065', ['aq065','culvert_c','culvert_p']], // Culvert
        ['AQ070', ['aq070','ferry_crossing_c']], // Ferry Crossing
        ['AQ095', ['aq095','tunnel_mouth_p']], // Tunnel Mouth
        ['AQ113', ['aq113','pipeline_c']], // Pipeline
        ['AQ125', ['aq125','transportation_station_s','transportation_station_p']], // Transportation Station
        ['AQ130', ['aq130','tunnel_c']], // Tunnel
        ['AQ140', ['aq140','vehicle_lot_s']], // Vehicle Lot
        ['AQ141', ['aq141','parking_garage_s','parking_garage_p']], // Parking Garage
        ['AQ170', ['aq170','motor_vehicle_station_s','motor_vehicle_station_p']], // Motor Vehicle Station
        ['AT010', ['at010','dish_aerial_p']], // Dish Aerial
        ['AT042', ['at042','pylon_p']], // Pylon
        ['BH010', ['bh010','aqueduct_s','aqueduct_c']], // Aqueduct
        ['BH020', ['bh020','canal_s','canal_c']], // Canal
        ['BH030', ['bh030','ditch_s','ditch_c']], // Ditch
        ['BH070', ['bh070','ford_c','ford_p']], // Ford
        ['BH082', ['bh082','inland_waterbody_s','inland_waterbody_p']], // Inland Waterbody
        ['BH140', ['bh140', 'river_s','river_c']], // River
        ['BH170', ['bh170','natural_pool_p']], // Natural Pool
        ['BH230', ['bh230', 'water_well_p','water_well_s']], // Water Well
        ['BI010', ['bi010', 'cistern_p']], // Cistern
        ['DB070', ['db070','cut_c']], // Cut
        ['DB150', ['db150','mountain_pass_p']], // Mountain Pass
        ['GB050', ['gb050','aircraft_revetment_c']], // Aircraft Revetment
        ['ZD040', ['zd040','named_location_s','named_location_c','named_location_p']], // Named Location
        ['ZD045', ['zd045','annotated_location_s','annotated_location_c','annotated_location_p']], // Named Location
      ];

      // Funky but it makes life easier
      var llayerName = layerName.toString().toLowerCase();

      for (var row in fCodeMap)
      {
        for (var val in fCodeMap[row][1])
        {
          if (llayerName == fCodeMap[row][1][val])
          {
            attrs.F_CODE = fCodeMap[row][0];
            break;
          }
        }
      }
    } // End of Find an FCode

    // The FCODE for Buildings is different. TDS uses AL013
    if (attrs.F_CODE == 'AL015') attrs.F_CODE = 'AL013';

  }, // End of applyToOsmPreProcessing


  // #####################################################################################################
  applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
  {
    // #####
    // This is from MGCP & some TDS
    // #####
    if (attrs.HWT && attrs.HWT !== '0')
    {
      tags.amenity = 'place_of_worship';

      if (tags.building)
      {
        switch (tags.building)
        {
        case 'cathedral':
        case 'chapel':
        case 'church':
          tags.religion = 'christian';
          break;

        case 'marabout':
        case 'mosque':
          tags.religion = 'muslim';
          break;

        case 'synagogue':
          tags.religion = 'jewish';
          break;

        case 'stupa':
          religion = 'buddhist';
          break;

                    // In the spec, these don't specify a religion.
                    // case 'religious_community':
                    // case 'pagoda':
                    // case 'shrine':
                    // case 'tabernacle':
                    // case 'temple':
        } // End switch
      }

      if (tags['tower:type'] == 'minaret')
      {
        tags.religion = 'muslim';
      }
    } // End HWT

    // Add the LayerName to the source
    if ((! tags.source) && layerName !== '') tags.source = 'gifd_d4:' + layerName.toLowerCase();

    if (attrs.UID)
    {
      tags.uuid = '{' + attrs['UID'].toString().toLowerCase() + '}';
    }
    else
    {
      if (gifd.configIn.OgrAddUuid == 'true') tags.uuid = createUuid();
    }


    if (gifd.osmPostRules == undefined)
    {
      // ##############
      // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
      //
      // Rules format:  ["test expression","output result"];
      // Note: t = tags, a = attrs and attrs can only be on the RHS
      var rulesList = [
        ['t[\'bridge:movable\'] && t[\'bridge:movable\'] !== \'no\' && t[\'bridge:movable\'] !== \'unknown\'','t.bridge = \'movable\''],
        ['t.navigationaid && !(t.aeroway)','t.aeroway = \'navigationaid\''],
        ['t.amenity == \'stop\' && t[\'transport:type\'] == \'bus\'','t.highway = \'bus_stop\''],
        ['t.diplomatic && !(t.amenity)','t.amenity = \'embassy\''],
        ['t.boundary == \'protected_area\' && !(t.protect_class)','t.protect_class = \'4\''],
        ['t.bunker_type && !(t.military)','t.military = \'bunker\''],
        ['t.control_tower == \'yes\' && t.use == \'air_traffic_control\'','t[\'tower:type\'] = \'observation\''],
        ['t.crossing == \'tank\'','t.highway = \'crossing\''],
        ['t.desert_surface','t.surface = t.desert_surface; delete t.desert_surface'],
        ['t.dock && !(t.waterway)','t.waterway = \'dock\''],
        ['t.drive_in == \'yes\'','t.amenity = \'cinema\''],
        ['t[\'generator:source\']','t.power = \'generator\''],
        ['t[\'glacier:type\'] == \'icecap\' && t.natural == \'glacier\'','delete t.natural'],
        ['t.golf == \'driving_range\' && !(t.leisure)','t.leisure = \'golf_course\''],
        ['t.historic == \'castle\' && !(t.ruins) && !(t.building)','t.building = \'yes\''],
        ['t.in_tunnel == \'yes\' && !(t.tunnel)','t.tunnel = \'yes\'; delete t.in_tunnel'],
        ['t.industrial && !(t.landuse)','t.landuse = \'industrial\''],
        ['(t.landuse == \'built_up_area\' || t.place == \'settlement\') && t.building','t[\'settlement:type\'] = t.building; delete t.building'],
        ['t.leisure == \'stadium\'','t.building = \'yes\''],
        ['t[\'material:vertical\']','t.material = t[\'material:vertical\']; delete t[\'material:vertical\']'],
        ['t[\'monitoring:weather\'] == \'yes\'','t.man_made = \'monitoring_station\''],
        ['t.on_bridge == \'yes\' && !(t.bridge)','t.bridge = \'yes\'; delete t.on_bridge'],
        ['t.public_transport == \'station\' && t[\'transport:type\'] == \'railway\'','t.railway = \'station\''],
        ['t.public_transport == \'station\' && t[\'transport:type\'] == \'bus\'','t.bus = \'yes\''],
        ['t.protect_class && !(t.boundary)','t.boundary = \'protected_area\''],
        ['t.pylon ==\'yes\' && t[\'cable:type\'] == \'cableway\'',' t.aerialway = \'pylon\''],
        ['t.pylon ==\'yes\' && t[\'cable:type\'] == \'power\'',' t.power = \'tower\''],
        ['t.sidetrack && !(t.railway)','t.railway = \'rail\''],
        ['t.sidetrack && !(t.service)','t.service = \'siding\''],
        ['t.social_facility','t.amenity = \'social_facility\'; t[\'social_facility:for\'] = t.social_facility; t.social_facility = \'shelter\''],
        ['t[\'theatre:type\'] && !(t.amenity)','t.amenity = \'theatre\''],
        ['t[\'tower:material\']','t.material = t[\'tower:material\']; delete t[\'tower:material\']'],
        ['t[\'tower:type\'] && !(t.man_made)','t.man_made = \'tower\''],
        ['t.use == \'islamic_prayer_hall\' && !(t.amenity)','t.amenity = \'place_of_worship\''],
        ['t.water || t.landuse == \'basin\'','t.natural = \'water\''],
        ['t.waterway == \'flow_control\'','t.flow_control = \'sluice_gate\''],
        ['t.wetland && !(t.natural)','t.natural = \'wetland\''],
        ['t[\'width:minimum_traveled_way\'] && !(t.width)','t.width = t[\'width:minimum_traveled_way\']']
      ];

      gifd.osmPostRules = translate.buildComplexRules(rulesList);
    }

    // translate.applyComplexRules(tags,attrs,rulesList);
    translate.applyComplexRules(tags,attrs,gifd.osmPostRules);

    // ##############
    // Lifecycle tags
    if (tags.condition)
    {
      if (tags.condition == 'construction')
      {
        //                 if (tags.highway && attrs.F_CODE == 'AP030')
        if (tags.highway)
        {
          tags.construction = tags.highway;
          tags.highway = 'construction';
          delete tags.condition;
        }
        else if (tags.railway)
        {
          tags.construction = tags.railway;
          tags.railway = 'construction';
          delete tags.condition;
        }
      } // End Construction

    } // End Condition tags

    // // Add 'building = yes' to amenities if we don't already have one
    // if (tags.amenity && !tags.building)
    // {
    //     // Debug:
    //     // print('Added building');
    //     // Don't add building=yes to built up areas!
    //     if (!tags.place) tags.building = 'yes';
    // }

    // Add 'building = yes' to military if it isn't a range
    if (tags.military && !tags.building)
    {
      // Debug:
      // print('Added building to military');
      if (tags.military !== 'range' && tags.military !== 'installation' ) tags.building = 'yes';
    }

    // if (tags.building == 'train_station' && !tags.railway) tags.railway = 'station';
    // if ('ford' in tags && !tags.highway) tags.highway = 'road';

    // Some FCODE specific rules
    switch (attrs.F_CODE)
    {
    case undefined: // Break early if no value
      break;

    case 'AF030': // Cooling Tower
      if (!tags['tower:type']) tags['tower:type'] = 'cooling';
      break;

    case 'AL013': // Building  NOTE this is the TDS F_CODE for Building. This was swapped during pre-processing
      if (tags.surface == 'unknown') delete tags.surface;
      break;

    case 'AL020': // AL020 (Built-up Area) should become a Place. NOTE: This is a bit vague...
      tags.place = 'yes'; // Catch All

      switch (tags['place:importance'])
      {
      case undefined: // Break early if no value
        break;

      case 'first':
        tags.place = 'city';
        tags.capital = 'yes';
        break;

      case 'second':
        tags.place = 'city';
        break;

      case 'third':
      case 'fourth':
        tags.place = 'town';
        break;

      case 'fifth':
        tags.place = 'village';
        break;

      case 'sixth':
        tags.place = 'hamlet';
        break;
      } // End switch

      switch (tags.use) // Fixup the landuse tags
      {
      case undefined: // Break early if no value
        break;

      case 'industrial':
        tags.landuse = 'industrial';
        delete tags.use;
        break;

      case 'commercial':
        tags.landuse = 'commercial';
        delete tags.use;
        break;

      case 'residential':
        tags.landuse = 'residential';
        delete tags.use;
        break;
      } // End switch

      break;

    case 'AL045': // Complex Outline. Going with landuse for this
      if (tags.building = 'residential') delete tags.building; // Goes with landuse == residential

      break;

    case 'BH070': // Ford
      // Fords are also supposed to be roads.
      if (geometryType == 'Line' && !tags.highway) tags.highway = 'road';
      break;

    } // End switch FCODE

    // Sort out TRS (Transport Type)
    switch (attrs.TRS)
    {
    case undefined:
      break;

    case '9': // Pedestrian
      // NOTE: This _might_ be a path: AP050 (Trail)
      if (!tags.highway) tags.highway = 'track';
      break;

    case '12': // Railway
      if (!tags.railway) tags.railway = 'rail';
      break;

    case '3': // TRD3 'Automotive'
    case '4': // Bus
    case '13': // Road
      if (!tags.highway) tags.highway = 'road';
      break;

    case '14': // Road and Railway. This might be ugly....
      if (!tags.highway) tags.highway = 'road';
      if (!tags.railway) tags.railway = 'rail';
      break;
    } // End switch TRS

    // AC000 (Processing Facility) vs AL010 (Facility)
    // In TDS, this is just AL010. Therefore, make it AL010 and use a custom rule if we are exporting
    // We are assumeing that it should produce something.
    if (tags.facility == 'processing')
    {
      if (! tags.product) tags.product = 'unknown';
      tags.facility = 'yes';
    }

  }, // End of applyToOsmPostProcessing

  // ##### End of the xxToOsmxx Block #####

  // #####################################################################################################

  // toOsm - Translate Attrs to Tags
  // This is the main routine to convert _TO_ OSM
  toOsm : function(attrs, layerName, geometryType)
  {
    tags = {};  // The final output Tag list

    // Setup config variables. We could do this in initialize() but some things don't call it :-(
    // Doing this so we don't have to keep calling into Hoot core
    if (gifd.configIn == undefined)
    {
      gifd.configIn = {};
      gifd.configIn.OgrDebugAddfcode = config.getOgrDebugAddfcode();
      gifd.configIn.OgrDebugDumptags = config.getOgrDebugDumptags();
      gifd.configIn.OgrAddUuid = config.getOgrAddUuid();

      // Get any changes
      gifd.toChange = hoot.Settings.get('schema.translation.override');
    }

    // Debug:
    if (gifd.configIn.OgrDebugDumptags == 'true') translate.debugOutput(attrs,layerName,geometryType,'','In attrs: ');

    // Set up the fcode translation rules. We need this due to clashes between the one2one and
    // the fcode one2one rules
    if (gifd.fcodeLookup == undefined)
    {
      // Add the FCODE rules for Import
      fcodeCommon.one2one.push.apply(fcodeCommon.one2one,gifd.rules.fcodeOne2oneIn);

      gifd.fcodeLookup = translate.createLookup(fcodeCommon.one2one);
      // translate.dumpOne2OneLookup(gifd.fcodeLookup);
    }

    if (gifd.lookup == undefined)
    {
      // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the
      // other tables are not set either.

      // Support Import Only attributes
      gifd.rules.one2one.push.apply(gifd.rules.one2one,gifd.rules.one2oneIn);

      gifd.lookup = translate.createLookup(gifd.rules.one2one);
    }

    // pre processing
    gifd.applyToOsmPreProcessing(attrs, layerName, geometryType);

    // Use the FCODE to add some tags.
    if (attrs.F_CODE)
    {
      var ftag = gifd.fcodeLookup['F_CODE'][attrs.F_CODE];
      if (ftag)
      {
        tags[ftag[0]] = ftag[1];
        // Debug: Dump out the tags from the FCODE
        // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
      }
      else
      {
        hoot.logVerbose('Translation for F_CODE ' + attrs.F_CODE + ' not found');
      }
    }

    // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
    // isn't used in the translation - this should end up empty.
    // not in v8 yet: // var tTags = Object.assign({},tags);
    var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));
    delete notUsedAttrs.F_CODE;

    // apply the simple number and text biased rules
    // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
    translate.applySimpleNumBiased(notUsedAttrs, tags, gifd.rules.numBiased, 'forward',[]);
    translate.applySimpleTxtBiased(notUsedAttrs, tags, gifd.rules.txtBiased, 'forward');

    // one 2 one
    translate.applyOne2One(notUsedAttrs, tags, gifd.lookup, {'k':'v'});

    // Crack open the OTH field and populate the appropriate attributes
    if (attrs.OTH) translate.processOTH(attrs, tags, gifd.lookup);

    // post processing
    gifd.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

    // Debug
    for (var i in notUsedAttrs) print('NotUsed: ' + i + ': :' + notUsedAttrs[i] + ':');

    // Debug: Add the FCODE to the tags
    if (gifd.configIn.OgrDebugAddfcode == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

    // Override tag values if appropriate
    translate.overrideValues(tags,gifd.toChange);

    // Debug:
    if (gifd.configIn.OgrDebugDumptags == 'true')
    {
      translate.debugOutput(tags,layerName,geometryType,'','Out tags: ');
      print('');
    }

    return tags;
  }, // End of toOsm

}; // End of gifd
