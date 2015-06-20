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

/*
    HGISv20 conversion script
        HGISv20 -> OSM+, and
        OSM+ -> HGISv20

    Based on the TDSv61.js script
*/

hgis20 = {
    // getDbSchema - Load the standard schema.
    getDbSchema: function() {

        // Warning: This is <GLOBAL> so we can get access to it from other functions
        rawSchema = hgis20.schema.getDbSchema();

        // Now add an o2s[A,L,P] feature to the rawSchema
        // We can drop features but this is a nice way to see what we would drop
        rawSchema = translate.addEmptyFeature(rawSchema);

        // Debug:
        // translate.dumpSchema(rawSchema);

        return rawSchema;

    }, // End getDbSchema


    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName) 
    {
        if (hgis20.osmPostRules == undefined)

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

//             // Wipe out the useless values
//             if (attrs[col] == '' || attrValue in ignoreList || attrs[col] in ignoreList)
//             {
//                 delete attrs[col]; // debug: Comment this out to leave all of the No Info stuff in for testing
//                 continue;
//             }

            // Push the attribute to upper case - if needed
            var c = col.toUpperCase();
            if (c !== col)
            {
                attrs[c] = attrs[col];
                delete attrs[col];
                col = c; // For the rest of this loop iteration
            }

        } // End in attrs loop

        //Sort out TYPE, TYPE1 & TYPE2.
        if (hgis20.rules.layerNames[layerName] !== undefined)
        {
            var tName = hgis20.rules.layerNames[layerName];

            if (attrs.TYPE)
            {
                attrs[tName + '$TYPE'] = attrs.TYPE;
                delete attrs.TYPE;
            }
            else if (attrs.TYPE1)
            {
                attrs[tName + '$TYPE1'] = attrs.TYPE1;
                delete attrs.TYPE1;

                attrs[tName + '$TYPE2'] = attrs.TYPE2;
                delete attrs.TYPE2;
            }
        }


    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName)
    {

        // New TDSv61 Attribute - ROR (Road Interchange Ramp)
        if (tags.highway && tags.link_road == 'yes')
        {
            var roadList = ['motorway','trunk','primary','secondary','tertiary'];
            if (roadList.indexOf(tags.highway) !== -1) tags.highway = tags.highway + '_link';
        }

        // If we have a UFI, store it. Some of the MAAX data has a LINK_ID instead of a UFI
        tags.source = 'hgisv20';
        tags.uuid = '{' + attrs['UFI'].toString().toLowerCase() + '}';


        if (hgis20.osmPostRules == undefined)
        {
            // ##############
            // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
            //
            // Rules format:  ["test expression","output result"];
            // Note: t = tags, a = attrs and attrs can only be on the RHS
            var rulesList = [
            ["t.amenity == 'stop' && t['transport:type'] == 'bus'","t.highway = 'bus_stop';"],
            ["t.boundary == 'protected_area' && !(t.protect_class)","t.protect_class = '4';"],
            ["t.control_tower == 'yes' && t.use == 'air_traffic_control'","t['tower:type'] = 'observation'"],
            ["t['generator:source'] == 'wind'","t.power = 'generator'"],
            ["t.historic == 'castle' && !(t.ruins) && !(t.building)","t.building = 'yes'"],
            ["(t.landuse == 'built_up_area' || t.place == 'settlement') && t.building","t['settlement:type'] = t.building; delete t.building"],
            ["t.leisure == 'stadium'","t.building = 'yes'"],
            ["t['material:vertical']","t.material = t['material:vertical']; delete t['material:vertical']"],
            ["t['monitoring:weather'] == 'yes'","t.man_made = 'monitoring_station'"],
            ["t.public_transport == 'station' && t['transport:type'] == 'railway'","t.railway = 'station'"],
            ["t.public_transport == 'station' && t['transport:type'] == 'bus'","t.bus = 'yes'"],
            ["t.pylon =='yes' && t['cable:type'] == 'cableway'"," t.aerialway = 'pylon'"],
            ["t.pylon =='yes' && t['cable:type'] == 'power'"," t.power = 'tower'"],
            ["t.service == 'yard'","t.railway = 'yes'"],
            ["t.social_facility","t.amenity = 'social_facility'; t['social_facility:for'] = t.social_facility; t.social_facility = 'shelter'"],
            ["t['tower:material']","t.material = t['tower:material']; delete t['tower:material']"],
            ["t['tower:type'] && !(t.man_made)","t.man_made = 'tower'"],
            ["t.use == 'islamic_prayer_hall' && !(t.amenity)","t.amenity = 'place_of_worship'"],
            ["t.water || t.landuse == 'basin'","t.natural = 'water'"],
            ["t.waterway == 'flow_control'","t.flow_control = 'sluice_gate'"],
            ["t.wetland && !(t.natural)","t.natural = 'wetland'"]
            ];

            hgis20.osmPostRules = translate.buildComplexRules(rulesList);
        }

        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,hgis20.osmPostRules);

        // ##############

        // Road & Railway Crossings
        // Road/Rail = crossing
        // Road + Rail = level_crossing
        if (tags.crossing_point)
        {
            if (tags['transport:type'] == 'railway')
            {
                tags.railway = 'crossing';

                if (tags['transport:type2'] == 'road') tags.railway = 'level_crossing';
            }
            else if (tags['transport:type'] == 'road')
            {
                if (tags['transport:type2'] == 'railway')
                {
                    tags.railway = 'level_crossing';
                }
                else
                {
                    tags.highway = 'crossing';
                }
            }
        } // End crossing_point


        // Add a building tag to buildings if we don't have one
        // We can't do this in the funky rules function as it uses "attrs" _and_ "tags"
        if (attrs.F_CODE == 'AL013' && !(tags.building)) tags.building = 'yes';

        // facility list is used for translating between buildings and facilities based on use
        // format:  "use":"building or amenity"
        // var facilityList = {'education':'school', 'healthcare':'hospital', 'university':'university'};

        // Fix the building 'use' tag. If the building has a 'use' and no specific building tag. Give it one
        if (tags.building == 'yes' && tags.use)
        {
            if ((tags.use.indexOf('manufacturing') > -1) || (tags.use.indexOf('processing') > -1))
            {
                tags.building = 'industrial';
            }
      /*
            else if (tags.use in facilityList)
            {
                tags.building = facilityList[tags.use];
                // delete tags.use;
            }
       */
        }

        // A facility is an area. Therefore "use" becomes "amenity". "Building" becomes "landuse"
        if (tags.facility && tags.use)
        {
            if ((tags.use.indexOf('manufacturing') > -1) || (tags.use.indexOf('processing') > -1))
            {
                tags.man_made = 'works';
            }
       /*
            else if (tags.use in facilityList) 
            {
                tags.amenity = facilityList[tags.use];
            }
        */
        }

    }, // End of applyToOsmPostProcessing
  
    // ##### End of the xxToOsmxx Block #####

    // ##### Start of the xxToHgisxx Block #####

    applyToHgisPreProcessing: function(tags, attrs, geometryType)
    {
        // initial cleanup
        for (var val in tags)
        {
            // Remove empty tags
            if (tags[val] == '')
            {
                delete tags[val];
                continue;
            }

            // Convert "abandoned:XXX" features
            if (val.indexOf('abandoned:') !== -1)
            {
                // Hopeing there is only one ':' in the tag name...
                var tList = val.split(':');
                tags[tList[1]] = tags[val];
                tags.condition = 'abandoned';
                delete tags[val];
            }
        }

        if (hgis20.nfddPreRules == undefined)
        {
        // See ToOsmPostProcessing for more details about rulesList.
            var rulesList = [
            ["t.amenity == 'bus_station'","t.public_transport = 'station'; t['transport:type'] == 'bus'"],
            ["t.amenity == 'marketplace'","t.facility = 'yes'"],
            ["t.construction && t.highway","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.construction && t.railway","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.control_tower && t.man_made == 'tower'","delete t.man_made"],
            ["t.highway == 'bus_stop'","t['transport:type'] = 'bus'"],
            ["t.highway == 'crossing'","t['transport:type'] = 'road';a.F_CODE = 'AQ062'; delete t.highway"],
            ["t.highway == 'mini_roundabout'","t.junction = 'roundabout'"],
            ["t.historic == 'castle' && t.building","delete t.building"],
            ["t.historic == 'castle' && t.ruins == 'yes'","t.condition = 'destroyed'; delete t.ruins"],
            ["t.landcover == 'snowfield' || t.landcover == 'ice-field'","a.F_CODE = 'BJ100'"],
            ["t.landuse == 'allotments'","t.landuse = 'farmland'"],
            ["t.landuse == 'brownfield'","t.landuse = 'built_up_area'; t.condition = 'destroyed'"],
            ["t.landuse == 'construction'","t.landuse = 'built_up_area'; t.condition = 'construction'"],
            ["t.landuse == 'farm'","t.landuse = 'farmland'"],
            ["t.landuse == 'farmland' && t.crop == 'fruit_tree'","t.landuse = 'orchard'"],
            ["t.landuse == 'farmyard'","t.facility = 'yes'; t.use = 'agriculture'; delete t.landuse"],
            ["t.landuse == 'grass'","t.natural = 'grassland'; t['grassland:type'] = 'grassland';"],
            ["t.landuse == 'meadow'","t.natural = 'grassland'; t['grassland:type'] = 'meadow';"],
            ["t.landuse == 'military'","t.military = 'installation'; delete t.landuse"],
            ["t.leisure == 'recreation_ground'","t.landuse = 'recreation_ground'; delete t.leisure"],
            ["t.landuse == 'reservoir'","t.water = 'reservoir'; delete t.landuse"],
            ["t.landuse == 'retail'","t.landuse = 'built_up_area'; t.use = 'commercial'"],
            ["t.landuse == 'scrub'","t.natural = 'scrub'; delete t.landuse"],
            // ["t.landuse == 'grass'","a.F_CODE = 'EB010'; t['grassland:type'] = 'grassland';"],
            // ["t.landuse == 'meadow'","a.F_CODE = 'EB010'; t['grassland:type'] = 'meadow';"],
            ["t.leisure == 'sports_centre'","t.facility = 'yes'; t.use = 'recreation'; delete t.leisure"],
            ["t.leisure == 'stadium' && t.building","delete t.building"],
            ["t.median == 'yes'","t.divider = 'yes'"],
            ["t.natural == 'wood'","t.landuse = 'forest'; delete t.natural"],
            ["t.power == 'pole'","t['cable:type'] = 'power'; t['tower:shape'] = 'pole'"],
            ["t.power == 'tower'","t['cable:type'] = 'power'"],
            ["t.power == 'line'","t['cable:type'] = 'power'; t.cable = 'yes'"],
            ["t.power == 'generator'","t.use = 'power_generation'; a.F_CODE = 'AL013'"],
            ["t.rapids == 'yes'","t.waterway = 'rapids'; delete t.rapids"],
            ["t.railway == 'station'","t.public_transport = 'station';  t['transport:type'] = 'railway'"],
            ["t.railway == 'level_crossing'","t['transport:type'] = 'railway';t['transport:type2'] = 'road'; a.F_CODE = 'AQ062'; delete t.railway"],
            ["t.railway == 'crossing'","t['transport:type'] = 'railway'; a.F_CODE = 'AQ062'; delete t.railway"],
            ["t.resource","t.raw_material = t.resource; delete t.resource"],
            ["t.route == 'road' && !(t.highway)","t.highway = 'road'; delete t.route"],
            ["(t.shop || t.office) &&  !(t.building)","a.F_CODE = 'AL013'"],
            ["t.social_facility == 'shelter'","t.social_facility = t['social_facility:for']; delete t.amenity; delete t['social_facility:for']"],
            ["!(t.water) && t.natural == 'water'","t.water = 'lake'"],
            ["t.use == 'islamic_prayer_hall' && t.amenity == 'place_of_worship'","delete t.amenity"],
            ["t.wetland && t.natural == 'wetland'","delete t.natural"],
            ["t.water == 'river'","t.waterway = 'river'"],
            ["t.waterway == 'riverbank'","t.waterway = 'river'"]
            ];

            hgis20.nfddPreRules = translate.buildComplexRules(rulesList);
        }

        // Apply the rulesList.
        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,hgis20.nfddPreRules);

        // Fix up OSM 'walls' around facilities
        if (tags.barrier == 'wall' && geometryType == 'Area')
        {
            attrs.F_CODE = 'AL010'; // Facility
            delete tags.barrier; // Take away the walls...
        }

        // Some tags imply that they are buildings but don't actually say so.
        // Most of these are taken from raw OSM and the OSM Wiki
        // Not sure if the list of amenities that ARE buildings is shorter than the list of ones that 
        // are not buildings.
        // Taking "place_of_worship" out of this and making it a building
        var notBuildingList = [
            'bbq','biergarten','drinking_water','bicycle_parking','bicycle_rental','boat_sharing',
            'car_sharing','charging_station','grit_bin','parking','parking_entrance','parking_space',
            'taxi','atm','fountain','bench','clock','hunting_stand','marketplace','post_box',
            'recycling', 'vending_machine','waste_disposal','watering_place','water_point',
            'waste_basket','drinking_water','swimming_pool','fire_hydrant','emergency_phone','yes',
            'compressed_air','water','nameplate','picnic_table','life_ring','grass_strip','dog_bin',
            'artwork','dog_waste_bin','street_light','park','hydrant','tricycle_station','loading_dock',
            'trailer_park','game_feeding'
            ]; // End notBuildingList

        if (tags.amenity && !(tags.building) && (notBuildingList.indexOf(tags.amenity) == -1)) attrs.F_CODE = 'AL013';

        // going out on a limb and processing OSM specific tags:
        // - Building == a thing,
        // - Amenity == The area around a thing
        // Note: amenity=place_of_worship is a special case. It _should_ have an associated building tag
        var facilityList = {
            'school':'850', 'university':'855', 'college':'857', 'hospital':'860'
            };

        if (tags.amenity in facilityList)
        {
            if (geometryType == 'Area')
            {
                attrs.F_CODE = 'AL010'; // Facility
            }
            else
            {
                attrs.F_CODE = 'AL013'; // Building
            }

            // If we don't have a Feature Function then assign one.
            if (!(attrs.FFN)) attrs.FFN = facilityList[tags.amenity];
        }

        // Fix up water features from OSM
        if (tags.natural == 'water' && !(tags.water)) 
        {
            if (geometryType =='Line')
            {
                tags.waterway = 'river';
                attrs.F_CODE = 'BH140';
            }
            else
            {
                tags.water = 'lake';
                attrs.F_CODE = 'BH082';
            }
        }

        // Cutlines/Cleared Ways & Highways
        // When we can output two features, this will be split
        if (tags.man_made == 'cutline' && tags.highway)
        {
            if (geometryType == 'Area')
            {
                // Keep the cutline/cleared way
                attrs.F_CODE = 'EC040';

                if (tags.memo)
                {
                    tags.memo = tags.memo + ';highway:' + tags.highway;
                }
                else
                {
                    tags.memo = 'highway:' + tags.highway;
                }

                delete tags.highway;
            }
            else
            { 
                // Drop the cutline/cleared way
                delete tags.man_made; 

                if (tags.memo)
                {
                    tags.memo = tags.memo + ';cleared_way:yes';
                }
                else
                {
                    tags.memo = 'cleared_way:yes';
                }
            }
        }

        // Places, localities and regions
        if (tags.place)
        {
            switch (tags.place)
            {
                case 'city':
                case 'town':
                case 'suburb':
                case 'neighbourhood':
                case 'quarter':
                case 'village':
                    attrs.F_CODE = 'AL020'; // Built Up Area
                    delete tags.place;
                    break;

                case 'hamlet':
                case 'isolated_dwelling':
                    attrs.F_CODE = 'AL105'; // Settlement
                    delete tags.place;
                    break;

                case 'populated':
                case 'state':
                case 'county':
                case 'region':
                case 'locality':
                case 'municipality':
                case 'borough':
                case 'unknown':
                    attrs.F_CODE = 'ZD045'; // Annotated Location
                    if (tags.memo)
                    {
                        tags.memo = tags.memo + ';annotation:' + tags.place;
                    }
                    else
                    {
                        tags.memo = 'annotation:' + tags.place;
                    }
                    delete tags.place;
                    break;

            } // End switch
        }

        // Bridges & Roads.  If we have an area or a line everything is fine.
        // If we have a point, we need to make sure that it becomes a bridge, not a road.
        if (tags.bridge && geometryType =='Point') attrs.F_CODE = 'AQ040';

        // Sort out PYM vs ZI032_PYM vs MCC vs VCM - Ugly
        var pymList = [ 'AL110','AL241','AQ055','AQ110','AT042'];

        var vcmList = [ 'AA040', 'AC020', 'AD010', 'AD025', 'AD030', 'AD041', 'AD050', 'AF010',
                        'AF020', 'AF021', 'AF030', 'AF040', 'AF070', 'AH055', 'AJ050', 'AJ051',
                        'AJ080', 'AJ085', 'AL010', 'AL013', 'AL019', 'AL080', 'AM011', 'AM020',
                        'AM030', 'AM070', 'AN076', 'AQ040', 'AQ045', 'AQ060', 'AQ116', 'BC050',
                        'BD115', 'BI010', 'BI050', 'GB230' ];

        if (tags.material)
        {
            if (pymList.indexOf(attrs.F_CODE) !== -1) 
            {
                tags['tower:material'] = tags.material;
                delete tags.material;
            }
            else if (vcmList.indexOf(attrs.F_CODE) !== -1) 
            {
                tags['material:vertical'] = tags.material;
                delete tags.material;
            }
        }

       // Protected areas have two attributes that need sorting out
       if (tags.protection_object == 'habitat' || tags.protection_object == 'breeding_ground')
       {
           if (tags.protect_class) delete tags.protect_class;
       }

       // Split link roads. TDSv61 now has an attribute for this
       if (tags.highway && (tags['highway'].indexOf('_link') !== -1))
       {
           tags.highway = tags['highway'].replace('_link','');
           tags.road_ramp = 'yes';
       }


    }, // End applyToHgisPreProcessing

    applyToHgisPostProcessing : function (tags, attrs, geometryType)
    {
    }, // End applyToHgisPostProcessing

    // ##### End of the xxToHgisxx Block #####

    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert _TO_ OSM
    toOsm : function(attrs, layerName)
    {
        tags = {};  // The final output Tag list

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        if (hgis20.lookup == undefined)
        {
            // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the 
            // other tables are not set either.
            
            // Support Import Only attributes
            hgis20.rules.one2one.push.apply(hgis20.rules.one2one,hgis20.rules.one2oneIn);
            
            hgis20.lookup = translate.createLookup(hgis20.rules.one2one);

            // Build an Object with both the SimpleText & SimpleNum lists
            hgis20.ignoreList = translate.joinList(hgis20.rules.numBiased, hgis20.rules.txtBiased);
            
            // Add features to ignore
            hgis20.ignoreList.FCSUBTYPE = '';
            hgis20.ignoreList.UFI = '';
        }

        // pre processing
        hgis20.applyToOsmPreProcessing(attrs, layerName);

        // one 2 one
        translate.applyOne2One(attrs, tags, hgis20.lookup, {'k':'v'}, hgis20.ignoreList);

        
        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, tags, hgis20.rules.numBiased, 'forward');
        translate.applySimpleTxtBiased(attrs, tags, hgis20.rules.txtBiased, 'forward');

        // post processing
        hgis20.applyToOsmPostProcessing(attrs, tags, layerName);
        
        // Debug:
        if (config.getOgrDebugDumptags() == 'true') 
        {
            for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');
            print('');
        }

        return tags;
    }, // End of toOsm


    // This gets called by translateToOGR and is where the main work gets done
    // We get Tags and return Attrs and a tableName
    // This is the main routine to convert _TO_ NFDD
    toHgis : function(tags, elementType, geometryType)
    {
        var tableName = ''; // The final table name
        attrs = {}; // The output
        
        var tableName2 = ''; // The second table name - will populate if appropriate
        var attrs2 = {}; // The second feature - will populate if appropriate

        // Start processing here
        // Debug:
        if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('In Tags: ' + i + ': :' + tags[i] + ':');

        // The Nuke Option: If we have a relation, drop the feature and carry on
        if (tags['building:part']) return null;

        // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.  
        // There is no way we can translate these to a single TDS feature.
        if (geometryType == 'Collection') return null;

        if (hgis20.lookup == undefined)
        {
            // Add "other" rules to the one2one
            hgis20.rules.one2one.push.apply(hgis20.rules.one2one,hgis20.rules.one2oneOut);

            hgis20.lookup = translate.createBackwardsLookup(hgis20.rules.one2one);
            // translate.dumpOne2OneLookup(hgis20.lookup);
            
            // Build a list of things to ignore and flip is backwards
            hgis20.ignoreList = translate.flipList(translate.joinList(hgis20.rules.numBiased, hgis20.rules.txtBiased));
            
            // Add some more features to ignore
            hgis20.ignoreList.uuid = '';
            hgis20.ignoreList.source = '';
            hgis20.ignoreList.area = '';
            hgis20.ignoreList['note:extra'] = '';
            hgis20.ignoreList['hoot:status'] = '';
            hgis20.ignoreList['error:circular'] = '';
        }

        // pre processing
        hgis20.applyToHgisPreProcessing(tags, attrs, geometryType);

        // one 2 one - we call the version that knows about OTH fields
        translate.applyOne2One(tags, attrs, hgis20.lookup, {'k':'v'}, hgis20.ignoreList);

        // apply the simple number and text biased rules
        // Note: These are BACKWARD, not forward!
        translate.applySimpleNumBiased(attrs, tags, hgis20.rules.numBiased, 'backward');
        translate.applySimpleTxtBiased(attrs, tags, hgis20.rules.txtBiased, 'backward');

        // post processing
        // hgis20.applyToHgisPostProcessing(attrs, tableName, geometryType);
        hgis20.applyToHgisPostProcessing(tags, attrs, geometryType);

        // Now check for invalid feature geometry
        // E.g. If the spec says a runway is a polygon and we have a line, throw error and 
        // push the feature to o2s layer
        var gFcode = geometryType.toString().charAt(0) + attrs.F_CODE;



        if (!(hgisTableNameLookup[gFcode]))
        {
            logError('Layer and Geometry: ' + gFcode + ' is not in the schema');

            tableName = 'o2s_' + geometryType.toString().charAt(0);

            // Debug:
            // Dump out what attributes we have converted before they get wiped out
            if (config.getOgrDebugDumpattrs() == 'true') for (var i in attrs) print('Converted Attrs:' + i + ': :' + attrs[i] + ':');

            for (var i in tags)
            {
                // Clean out all of the source: hoot: and error: tags to save space
                // if (i.indexOf('source:') !== -1) delete tags[i];
                if (i.indexOf('hoot:') !== -1) delete tags[i];
                if (i.indexOf('error:') !== -1) delete tags[i];
            }

            // Convert all of the Tags to a string so we can jam it into an attribute
            var str = JSON.stringify(tags);

            // If the tags are > 254 char, split into pieces. Not pretty but stops errors.
            // A nicer thing would be to arrange the tags until they fit neatly
            // Apparently Shape & FGDB can't handle fields > 254 chars. 
            if (str.length < 255 || config.getOgrSplitO2s() == 'false') 
            {
                // return {attrs:{tag1:str}, tableName: tableName};
                attrs = {tag1:str};
            }
            else
            {
                // Not good. Will fix with the rewrite of the tag splitting code
                if (str.length > 1012)
                {
                    logError('o2s tags truncated to fit in available space.');
                    str = str.substring(0,1012);
                }

                // return {attrs:{tag1:str.substring(0,253), tag2:str.substring(253)}, tableName: tableName};
                attrs = {tag1:str.substring(0,253), 
                         tag2:str.substring(253,506),
                         tag3:str.substring(506,759),
                         tag4:str.substring(759,1012)};
            }
        }
        else // We have a feature
        {
            // Validate attrs: remove all that are not supposed to be part of a feature
            hgis20.validateAttrs(geometryType,attrs);

            // Now set the FCSubtype. 
            // NOTE: If we export to shapefile, GAIT _will_ complain about this
            if (config.getOgrTdsAddFcsubtype() == 'true') attrs.FCSUBTYPE = hgis20.rules.subtypeList[attrs.F_CODE];
            tableName = layerNameLookup[gFcode.toUpperCase()];
        } // End We have a feature

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true' || config.getOgrDebugDumptags() == 'true')
        {
            print('TableName: ' + tableName + '  FCode: ' + attrs.F_CODE + '  Geom: ' + geometryType);
        }

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true')
        {
            for (var i in attrs) print('Out Attrs:' + i + ': :' + attrs[i] + ':');
            print('');
        }

        var returnData = [{attrs:attrs, tableName: tableName}];


        // Look for Review tags and push them to a review layer if found
        if (tags['hoot:review:needs'] == 'yes')
        {
            var reviewAttrs = {};

            // Note: Some of these may be "undefined"
            reviewAttrs.note = tags['hoot:review:note'];
            reviewAttrs.score = tags['hoot:review:score'];
            reviewAttrs.uuid = tags.uuid;
            reviewAttrs.source = tags['hoot:review:source'];

            var reviewTable = 'review_' + geometryType.toString().charAt(0);
            returnData.push({attrs: reviewAttrs, tableName: reviewTable});
        }

        return returnData;

    } // End of toHgis

} // End of hgis20
