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

        // Add the empty Review layers
        rawSchema = translate.addReviewFeature(rawSchema);

        hgis20.layersInSchema = {};
        hgis20.attrLookup = {};

        // Quick loop to build lookup tables
        for (var i=0, slen = rawSchema.length; i < slen; i++)
        {
            // Build a layername lookup list to validate against
            hgis20.layersInSchema[rawSchema[i].name + ':' + rawSchema[i].geom] = 1;

            // Build a lookup table of attributes for validation
            var attrArray = [];
            for (var j=0, cLen = rawSchema[i].columns.length; j < cLen; j++)
            {
                attrArray.push(rawSchema[i].columns[j].name);
            }
            // Add the attrArray to the list as <geom><NAME>:[array]
            // Eg[L,A,P]Airfields:[array]
            hgis20.attrLookup[rawSchema[i].geom.charAt(0) + rawSchema[i].name] = attrArray;

        } // End For rawSchema.length

        // Debug:
        // translate.dumpSchema(rawSchema);

        return rawSchema;

    }, // End getDbSchema


    // validateAttrs: Clean up the supplied attr list by dropping anything that should not be part of the
    //                feature.
    validateAttrs: function(geometryType,tableName,attrs) {

        // First, use the lookup table to quickly drop all attributes that are not part of the feature.
        // This is quicker than going through the Schema due to the way the Schema is arranged
        var attrList = hgis20.attrLookup[geometryType.toString().charAt(0) + tableName];

        if (attrList != undefined)
        {
            // The code is duplicated but it is quicker than doing the "if" on each iteration
            if (config.getOgrDebugDumpvalidate() == 'true')
            {
                for (var val in attrs)
                {
                    if (attrList.indexOf(val) == -1)
                    {
                        logWarn('Validate: Dropping ' + val + '  from ' + tableName);
                        delete attrs[val];
                    }
                }
            }
            else
            {
                for (var val in attrs)
                {
                    if (attrList.indexOf(val) == -1) delete attrs[val];
                }
            }
        }
        else
        {
            logWarn('Validate: No attrList for ' + tableName + ' ' + geometryType);
        } // End Drop attrs

        // No quick and easy way to do this unless we build yet another lookup table
        var feature = {};

        for (var i=0, sLen = rawSchema.length; i < sLen; i++)
        {
            if (rawSchema[i].name == tableName && rawSchema[i].geom == geometryType)
            {
                feature = rawSchema[i];
                break;
            }
        }
    }, // End validateAttrs

    // Sort out if we need to return two features or one.
    // This is generally for Roads/Railways & bridges but can also be for other features.
    twoFeatures: function(geometryType, layerName, tags, attrs)
    {
        var newAttrs = {};

//         // Sort out Roads, Railways and Bridges.
//         if (geometryType == 'Line' && tags.bridge && (tags.highway || tags.railway))
//         {
//             if (layerName !== 'Bridges_Tunnels') // Not a Bridge
//             {
//                 newAttrs.XtableName = 'Bridges_Tunnels';
//
//                 if (tags.highway)
//                 {
//                     newAttrs.TYPE2 = 'Road'; // Road
//                 }
//                 else if (tags.railway)
//                 {
//                     newAttrs.TYPE2 = 'Rail'; // Railway
//                 }
//             }
//             else // A Bridge
//             {
//                 if (tags.railway)
//                 {
//                     newAttrs.XtableName = 'Railways'; // Railway
//                 }
//                 else
//                 {
//                     newAttrs.XtableName = 'Roads';
//                     // Put in switch statement on the highway tag
//                 }
//             }
//
//             // Remove the uuid from the tag list so we get a new one for the second feature
//             delete tags.uuid;
//         } // End sort out Road, Railway & Bridge


        // Add features to a second layer.
        // Note: The geometry MUST be the same

        if (layerName == 'Marine_POI' && attrs.TYPE == 'Beach')
        {
            newAttrs.XtableName = 'Natural_POI';
            newAttrs.TYPE = attrs.TYPE;
            newAttrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'Duplicate from Marine_POI',';');
        }

        if (layerName == 'Cultural_POI')
        {
            if (attrs.TYPE == 'Memorial' || attrs.TYPE == 'Monument' || attrs.TYPE == 'Museum' || attrs.TYPE == 'Theatre')
            {
                newAttrs.XtableName = 'Tourist_Locations_POI';
                newAttrs.TYPE = attrs.TYPE;
                newAttrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'Duplicate from Cultural_POI',';');
            }
        }

        if (layerName == 'Medical_Facilities' && attrs.TYPE2 == 'Spa')
        {
            newAttrs.XtableName = 'Tourist_Locations_POI';
            newAttrs.TYPE = 'Spa';
            newAttrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'Duplicate from Medical_Facilities',';');
        }

        if (layerName == 'Natural_Resources' && attrs.OPER_TYPE == 'Well')
        {
            newAttrs.XtableName = 'Hydrology_POI';
            newAttrs.TYPE = 'Well';
            newAttrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'Duplicate from Natural Resources',';');
        }



        // If we are making a second feature, process it.
        if (newAttrs.XtableName)
        {
            // Now go make a second feature
            // pre processing
            hgis20.applyToHgisPreProcessing(tags, newAttrs, geometryType);

            // one 2 one - we call the version that knows about OTH fields
            translate.applyOne2One(tags, newAttrs, hgis20.lookup, hgis20.layerLookup, hgis20.ignoreList);

            // apply the simple number and text biased rules
            // Note: These are BACKWARD, not forward!
            translate.applySimpleNumBiased(newAttrs, tags, hgis20.rules.numBiased, 'backward');
            translate.applySimpleTxtBiased(newAttrs, tags, hgis20.rules.txtBiased, 'backward');

            // post processing
            hgis20.applyToHgisPostProcessing(tags, newAttrs, geometryType);
        }

        // Debug:
        // for (var i in newAttrs) print('twoFeatures: New Attrs:' + i + ': :' + newAttrs[i] + ':');

        // Return the new attributes
        return newAttrs;
    }, // End twoFeatures

    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName) 
    {
        // This is a handy loop. We use it to:
        // 1) Remove all of the "No Information" and -999999 fields
        // 2) Convert all of the Attrs to uppercase - if needed

        // List of data values to drop/ignore
        var ignoreList = { '-999999.0':1, '-999999':1, '999999':1 };

        for (var col in attrs)
        {
            // Wipe out the useless values
            if (attrs[col] == '' || attrs[col] in ignoreList)
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

        // Slam some of the attributes to lower case to amke matching easier. Yes, this is Ugly
        if (attrs.CONF_IMAGE)
        {
            var tmp = attrs.CONF_IMAGE;
            attrs.CONF_IMAGE = tmp.toLowerCase();
        }

        if (attrs.EVENT_TYPE)
        {
            var tmp = attrs.EVENT_TYPE;
            attrs.EVENT_TYPE = tmp.toLowerCase();
        }

        if (attrs.DISEASE)
        {
            var tmp = attrs.DISEASE;
            attrs.DISEASE = tmp.toLowerCase();
        }


        // Sort out uglyness. We have seen: '1', 'High', '1 - High', 'high' etc
        if (attrs.SPA_ACC)
        {
            var acc_val = attrs.SPA_ACC;
            if (acc_val.indexOf('1' > -1 || acc_val.indexOf('igh') > -1)) // High
            {
                attrs.SPA_ACC = '1 - High';
            }
            else if (acc_val.indexOf('2' > -1 || acc_val.indexOf('edium') > -1)) // Medium
            {
                attrs.SPA_ACC = '2 - Medium';
            }
            else if (acc_val.indexOf('3' > -1 || acc_val.indexOf('ow') > -1)) // Low
            {
                attrs.SPA_ACC = '3 - Low';
            }
      } // End SPA_ACC

        //Sort out TYPE, TYPE1 & TYPE2. This is to make life easier in the one2one rules
        if (hgis20.rules.layerNames[layerName] !== undefined)
        {
            var tName = hgis20.rules.layerNames[layerName];

            if (attrs.TYPE)
            {
                attrs[tName + '$TYPE'] = attrs.TYPE;
                delete attrs.TYPE;
            }

            if (attrs.TYPE1)
            {
                attrs[tName + '$TYPE1'] = attrs.TYPE1;
                delete attrs.TYPE1;
            }

            if (attrs.TYPE2)
            {
                attrs[tName + '$TYPE2'] = attrs.TYPE2;
                delete attrs.TYPE2;
            }
        }
        else
        {
            logError('Layer: ' + layerName + ' not found in layer names list.');
        }


    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName)
    {
        // If we have a UFI, store it. Some of the MAAX data has a LINK_ID instead of a UFI
        if (!(tags.source)) tags.source = 'hgisv20';
        tags.uuid = createUuid();

        // Refugee Camps
        if (tags.idp == 'yes' || tags.refugee == 'yes') tags.tourism = 'camp_site';

        // Ford can be in Roads and in Natural POI
        if (tags.nford)
        {
            tags.ford = 'yes';
            delete tags.nford;
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


        if (hgis20.hgisPreRules == undefined)
        {
        // See TDSv61 ToOsmPostProcessing for more details about rulesList.
            var rulesList = [
//             ["t.amenity == 'bus_station'","t.public_transport = 'station'; t['transport:type'] == 'bus'"],
//             ["t.amenity == 'marketplace'","t.facility = 'yes'"],
            ["t.building == 'school' && !(t.amenity)","t.amenity = 'school'; delete t.building"],
            ["t.construction && t.highway","t.highway = t.construction; t.condition = 'construction'; delete t.construction"],
            ["t.construction && t.railway","t.railway = t.construction; t.condition = 'construction'; delete t.construction"],
//             ["t.control_tower && t.man_made == 'tower'","delete t.man_made"],
            ["t.crossing == 'traffic_signals' && t.highway == 'crossing'","delete t.crossing"], // Crosswalk
//             ["t.crossing == 'tank' && t.highway == 'crossing'","delete t.highway"],
//             ["t.dock && t.waterway == 'dock'","delete t.waterway"],
//             ["t.golf == 'driving_range' && t.leisure == 'golf_course'","delete t.leisure"],
//             ["t.highway == 'bus_stop'","t['transport:type'] = 'bus'"],
//             ["t.highway == 'crossing'","t['transport:type'] = 'road';a.F_CODE = 'AQ062'; delete t.highway"],
            ["t.highway == 'mini_roundabout'","t.junction = 'roundabout'"],
//             ["t.historic == 'castle' && t.building","delete t.building"],
//             ["t.historic == 'castle' && t.ruins == 'yes'","t.condition = 'destroyed'; delete t.ruins"],
//             ["t.landcover == 'snowfield' || t.landcover == 'ice-field'","a.F_CODE = 'BJ100'"],
//             ["t.landuse == 'allotments'","t.landuse = 'farmland'"],
//             ["t.landuse == 'brownfield'","t.landuse = 'built_up_area'; t.condition = 'destroyed'"],
//             ["t.landuse == 'construction'","t.landuse = 'built_up_area'; t.condition = 'construction'"],
//             ["t.landuse == 'farm'","t.landuse = 'farmland'"],
//             ["t.landuse == 'farmland' && t.crop == 'fruit_tree'","t.landuse = 'orchard'"],
//             ["t.landuse == 'farmyard'","t.facility = 'yes'; t.use = 'agriculture'; delete t.landuse"],
//             ["t.landuse == 'grass'","t.natural = 'grassland'; t['grassland:type'] = 'grassland';"],
//             ["t.landuse == 'meadow'","t.natural = 'grassland'; t['grassland:type'] = 'meadow';"],
//             ["t.landuse == 'military'","t.military = 'installation'; delete t.landuse"],
//             ["t.leisure == 'recreation_ground'","t.landuse = 'recreation_ground'; delete t.leisure"],
//             ["t.landuse == 'reservoir'","t.water = 'reservoir'; delete t.landuse"],
//             ["t.landuse == 'retail'","t.landuse = 'built_up_area'; t.use = 'commercial'"],
//             ["t.landuse == 'scrub'","t.natural = 'scrub'; delete t.landuse"],
//             ["t.landuse == 'grass'","a.F_CODE = 'EB010'; t['grassland:type'] = 'grassland';"],
//             ["t.landuse == 'meadow'","a.F_CODE = 'EB010'; t['grassland:type'] = 'meadow';"],
//             ["t.leisure == 'sports_centre'","t.facility = 'yes'; t.use = 'recreation'; delete t.leisure"],
//             ["t.leisure == 'stadium' && t.building","delete t.building"],
//             ["t.median == 'yes'","t.divider = 'yes'"],
//             ["t.natural == 'desert' && t.surface","t.desert_surface = t.surface; delete t.surface"],
//             ["t.natural == 'wood'","t.landuse = 'forest'; delete t.natural"],
//             ["t.power == 'pole'","t['cable:type'] = 'power'; t['tower:shape'] = 'pole'"],
//             ["t.power == 'tower'","t['cable:type'] = 'power'"],
//             ["t.power == 'line'","t['cable:type'] = 'power'; t.cable = 'yes'"],
//             ["t.power == 'generator'","t.use = 'power_generation'; a.F_CODE = 'AL013'"],
            ["t.rapids == 'yes'","t.waterway = 'rapids'; delete t.rapids"],
//             ["t.railway == 'station'","t.public_transport = 'station';  t['transport:type'] = 'railway'"],
//             ["t.railway == 'level_crossing'","t['transport:type'] = 'railway';t['transport:type2'] = 'road'; a.F_CODE = 'AQ062'; delete t.railway"],
//             ["t.railway == 'crossing'","t['transport:type'] = 'railway'; a.F_CODE = 'AQ062'; delete t.railway"],
//             ["t.resource","t.raw_material = t.resource; delete t.resource"],
            ["t.route == 'road' && !(t.highway)","t.highway = 'road'; delete t.route"],
//             ["(t.shop || t.office) &&  !(t.building)","a.F_CODE = 'AL013'"],
//             ["t.social_facility == 'shelter'","t.social_facility = t['social_facility:for']; delete t.amenity; delete t['social_facility:for']"],
            ["!(t.water) && t.natural == 'water'","t.water = 'lake'"],
//             ["t.use == 'islamic_prayer_hall' && t.amenity == 'place_of_worship'","delete t.amenity"],
//             ["t.wetland && t.natural == 'wetland'","delete t.natural"],
            ["t.water == 'river'","t.waterway = 'river'"],
            ["t.waterway == 'riverbank'","t.waterway = 'river'"]
            ];

            hgis20.hgisPreRules = translate.buildComplexRules(rulesList);
        }

        // Apply the rulesList.
        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,hgis20.hgisPreRules);

        // Cleanup wetlands
        if (tags.wetland && tags.natural == 'wetland')
        {
            if (tags.wetland == 'tidal_flat')
            {
                delete tags.natural;
            }
            else
            {
                attrs.COMMENTS = translate.appendValue(attrs.COMMENTS,tags.wetland,';');
                delete tags.wetland;
            }
        }

        // Sort out Fords.
        // * Point = Natural_POI
        // * Line = Roads
        if (tags.ford == 'yes' && geometryType == 'Point')
        {
            tags.nford = 'yes';
            delete tags.ford;
        }

        // Sort out Lagoons.
        // * Point = Natural_POI
        // * Area = Hydrology_Polygons
        if (tags.water == 'lagoon' && geometryType == 'Point')
        {
            tags.nlagoon = 'yes';
            delete tags.water;
        }

        // Sort out Wetlands.
        // * Point = Natural_POI
        // * Area = Hydrology_Polygons
        if (tags.natural == 'wetland' && geometryType == 'Point')
        {
            tags.nwetland = 'yes';
            delete tags.natural;
        }

        // Country, State etc
        if (tags['addr:country'])
        {
            tags['is_in:country_code'] = tags['addr:country'];
            delete tags['addr:country'];
        }

        if (tags['is_in:state'])
        {
            tags['admin_level1:name'] = tags['is_in:state'];
            delete tags['is_in:state'];
        }

        // City - Possibly map to admin level as well???
        if (tags['addr:city'])
        {
            tags['is_in:city'] = tags['addr:city'];
            delete tags['addr:city'];
        }


        // Transportation features
        // Bus POI's
        if (tags.highway == 'bus_stop')
        {
            tags.bus = 'yes';
            tags.public_transport = 'stop_position';
            delete tags.highway;
        }

        if (tags.amenity == 'bus_station')
        {
            tags.bus = 'yes'
            tags.public_transport = 'station';
            delete tags.amenity;
        }

        // Railway POI's
        if (tags.railway)
        {
            switch (tags.railway)
            {
                case 'station':
                case 'yard':
                    tags.train = 'yes';
                    tags.public_transport = tags.railway;
                    break;

                case 'tram_stop':
                    tags.tram = 'yes';
                    tags.public_transport = 'stop_position';
                    delete tags.railway;
                    break;

                case 'stop':
                    tags.train = 'yes';
                    tags.public_transport = 'stop_position';
                    break;
            } // End switch
        } // End Railway

        if (tags.amenity == 'ferry_terminal')
        {
            tags.ferry = 'yes'
            tags.public_transport = 'terminal';
            delete tags.amenity;
        }

        if (tags.tunnel == 'yes')
        {
            if (tags.highway)
            {
                tags['transport:type2'] = 'tunnel';
                tags.car = 'yes';
            }
            else if (tags.railway == 'subway')
            {
                tags['transport:type2'] = 'tunnel';
                tags.subway = 'yes';
            }
            else if (tags.railway == 'tram')
            {
                tags['transport:type2'] = 'tunnel';
                tags.tram = 'yes';
            }
            else if (tags.railway)
            {
                tags['transport:type2'] = 'tunnel';
                tags.train = 'yes';
            }
        }

        // Houses
        if (tags.building == 'house') tags.building = 'residential';
    }, // End applyToHgisPreProcessing

    applyToHgisPostProcessing : function (tags, attrs, geometryType)
    {
        // Start looking for a Table Name
        // Next step is to make this a list: we might get more than one match...
//         if (!(attrs.XtableName))
//         {
        for (var val in attrs)
        {
        // Unsplit the layer and the type
            if (val.indexOf('$') !== -1)
            {
                var tList = val.split('$');
                attrs.XtableName = hgis20.layerList[tList[0]];

                // Debug
                // print('Post: Loop XtableName: ' + attrs.XtableName);

                attrs[tList[1]] = attrs[val];
                delete attrs[val];
            }
        }
//         }

        // Now run through the list of layer specific tags. This is after the one2one list so we can correct some of the
        // previous rules
        // E.g. Religion vs Religious_Institutions
//         if (!(attrs.XtableName) && hgis20.layerLookup)
        if (hgis20.layerLookup)
        {
            for (var col in tags)
            {
                var value = tags[col];
                if (col in hgis20.layerLookup)
                {
                    if (value in hgis20.layerLookup[col])
                    {
                        var row = hgis20.layerLookup[col][value];
                        attrs.XtableName = row[1];

                        // Debug
                        // print('Post: layerName: Got ' + attrs.XtableName);
                    }
                }
            }
        } // End find layerName

        // Debug
        // print('Post: XtableName: ' + attrs.XtableName);

        // If we still don't have a tablename, try looking for unique attributes
        if (!(attrs.XtableName) && hgis20.rules.uniqList)
        {
            for (var val in attrs)
            {
                if (val in hgis20.rules.uniqList)
                {
                    attrs.XtableName = hgis20.rules.uniqList[val];

                    // Debug
                    // print('Unique: ' + attrs.XtableName);

                    break;
                }
            } // End attrs loop
        } // End find XtableName


        // Fix up Hydro Lines vs Areas. Just for what is in the spec, everything else should throw an error
        if (attrs.XtableName == 'Hydrology_Polygons')
        {
            if (geometryType == 'Line')
            {
                switch (attrs.TYPE)
                {
                    case 'Lake':
                    case 'Inland Water':
                    case 'Land Subject to Inundation':
                        attrs.XtableName = 'Hydrology_Polylines';
                        break;

                    case 'River':
                        attrs.TYPE = 'River/Stream';
                        attrs.XtableName = 'Hydrology_Polylines';
                        break;
                } // End Switch
            }
        } // End Hydrology_Polygons

        // Fix Railway POI vs lines
        if (attrs.XtableName == 'Railways' || attrs.XtableName == 'Railways_POI')
        {
            if (geometryType == 'Line')
            {
                attrs.XtableName = 'Railways';
                if (attrs.TYPEX)
                {
                    attrs.TYPE2 = attrs.TYPEX;
//                     delete attrs.TYPEX;
                }
            }
            if (geometryType == 'Point')
            {
                attrs.XtableName = 'Railways_POI';
                if (attrs.TYPEX)
                {
                    attrs.TYPE1 = attrs.TYPEX;
                    delete attrs.TYPEX;
                }
            }
        }

        // Free Trade Zones
        if (attrs.XtableName == 'Free_Trade_Zones' && geometryType == 'Area')
        {
            attrs.XtableName == 'Free_Trade_Zones_Polygons';
        }

        // Sort out STATUS vs OP_STATUS
        if (attrs.XtableName == 'Power_Plants' && attrs.STATUS)
        {
            attrs.OP_STATUS = attrs.STATUS;
            delete attrs.STATUS;
        }

        // Assign a default Spatial Accuracy if we don't have one
        if (!(attrs.SPA_ACC) && tags.source)
        {
            if (tags['source'].indexOf('osm') > -1)
            {
                attrs.SPA_ACC = '1 - High';
            }
            else if (tags['source'].indexOf('wikimapia') > -1)
            {
                attrs.SPA_ACC = '1 - High';
            }
            else if (tags['source'].indexOf('hoteldb') > -1)
            {
                attrs.SPA_ACC = '3 - Low';
            }
            else if (tags['source'].indexOf('geonames') > -1)
            {
                attrs.SPA_ACC = '3 - Low';
            }
        } // End SPA_ACC

        // Easy Stuff:
        // Jam shops into Commercial_POI if they haven't been categorised
        if (!(attrs.XtableName) && tags.shop)
        {
            attrs.XtableName = 'Commercial_POI';
            attrs.TYPE1 = 'Retail';
            attrs.TYPE2 = 'Other';
            attrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'shop:' + tags.shop,';');
        }

        // Do the same with Offices
        if (!(attrs.XtableName) && tags.office)
        {
            attrs.XtableName = 'Commercial_POI';
            attrs.TYPE1 = 'Service';
            attrs.TYPE2 = 'Office';
            attrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'office:' + tags.office,';');
        }

        // Post process the Commercial POI into TYPE1 & TYPE2
        if (attrs.XtableName == 'Commercial_POI' && !(attrs.TYPE1))
        {
            var comTypeList = {
                'Airline':'Service',
                'ATM':'Financial',
                'Auto Repair':'Automotive',
                'Bakery':'Retail',
                'Bank':'Financial',
                'Bar':'Retail',
                'Beauty Salon':'Retail',
                'Brewery':'Manufacturing',
                'Butcher':'Retail',
                'Car Dealership':'Automotive',
                'Car Rental':'Automotive',
                'Car Wash':'Automotive',
                'Computer':'Retail',
                'Construction':'Industrial',
                'Day Care':'Service',
                'Delicatessen':'Retail',
                'Electronics':'Retail',
                'Entertainment':'Service',
                'Factory':'Industrial',
                'Film':'Service',
                'Freight - Shipping':'Service',
                'Funeral Home':'Service',
                'Fuel':'Retail',
                'Gas':'Retail',
                'Grocery':'Retail',
                'Heavy Machinery':'Industrial',
                'Legal':'Service',
                'Market':'Retail',
                'Medical':'Service',
                'Mining':'Industrial',
                'Nursery':'Retail',
                'Office':'Service',
                'Other':'Other',
                'Research':'Service',
                'Restaurant':'Retail',
                'Shipping':'Service',
                'Store':'Retail',
                'Telecommunications':'Telecommunications',
                'Television':'Telecommunications',
                'Tourist':'Service',
                'Tower':'Telecommunications',
                'Veterinarian':'Service',
            }; // End comTypeList

            if (attrs.TYPE2 in comTypeList)
            {
                attrs.TYPE1 = comTypeList[attrs.TYPE2];
            }
            else
            {
                attrs.TYPE1 = 'Other';
            }
        } // End post process Commercial POI

        // Clean up the Religion polygon layer
        // * This layer is for religious demographics, not buildings, cemetery's etc
        if (attrs.XtableName == 'Religion')
        {
            if (tags.building || tags.landuse || tags.amenity || tags.historic)
            {
                delete attrs.XtableName;
            }

        }

    }, // End applyToHgisPostProcessing

    // ##### End of the xxToHgisxx Block #####

    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert _TO_ OSM
    toOsm : function(attrs, layerName)
    {
        tags = {};  // The final output Tag list

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        if (hgis20.layerLookup == undefined)
        {
            // Build the lookup table for the additional tags to add based on the table name
            hgis20.rules.layerIn.push.apply(hgis20.rules.layerIn,hgis20.rules.layerCommon);
            hgis20.layerLookup = translate.createLookup(hgis20.rules.layerIn);
            // translate.dumpOne2OneLookup(hgis20.layerLookup);
        }

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
            hgis20.ignoreList.SHAPE_LENGTH = '';
            hgis20.ignoreList.SHAPE_LENG = '';
            hgis20.ignoreList.SHAPE_AREA = '';
            hgis20.ignoreList.IMGNAME = '';

            // Drop GEONAMES stuff
            hgis20.ignoreList.DSG = '';
        }

        // pre processing
        hgis20.applyToOsmPreProcessing(attrs, layerName);

        // Use the layerName to add some tags.
        var ftag = hgis20.layerLookup['XtableName'][layerName];
        if (ftag)
        {
            tags[ftag[0]] = ftag[1];
        }
//         else
//         {
//             hoot.logWarn('Default Translation for ' + layerName + ' not found');
//         }

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

        // Check if we have a schema. This is a quick way to workout if various lookup tables have been built
        if (hgis20.rawSchema == undefined)
        {
            var tmp_schema = hgis20.getDbSchema();
        }

        // Start processing here
        // Debug:
        if (config.getOgrDebugDumptags() == 'true')
        {
            print('');
            print('Element: ' + elementType + '  Geom: ' + geometryType);
            for (var i in tags) print('In Tags: ' + i + ': :' + tags[i] + ':');
        }

        // The Nuke Option: If we have a relation, drop the feature and carry on
        if (tags['building:part']) return null;

        // The Nuke Option: "Collections" are groups of different feature types: Point, Area and Line.  
        // There is no way we can translate these to a single feature.
        if (geometryType == 'Collection') return null;

        // Set up the tableName translation rules. We need this due to clashes between the one2one and
        // the tableName one2one rules
        if (hgis20.layerLookup == undefined)
        {
            // Build the TableName lookup list
            hgis20.rules.layerOut.push.apply(hgis20.rules.layerOut,hgis20.rules.layerCommon);
            hgis20.layerLookup = translate.createBackwardsLookup(hgis20.rules.layerOut);
            // translate.dumpOne2OneLookup(hgis20.layerLookup);
        }


        // We assume that if one table is undefined, the rest are.
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
            // hgis20.ignoreList.source = '';
            hgis20.ignoreList.area = '';
            hgis20.ignoreList['note:extra'] = '';
            hgis20.ignoreList['hoot:status'] = '';
            hgis20.ignoreList['hoot:score:match'] = '';
            hgis20.ignoreList['hoot:score:miss'] = '';
            hgis20.ignoreList['hoot:score:review'] = '';
            hgis20.ignoreList['hoot:score:uuid'] = '';
            hgis20.ignoreList['error:circular'] = '';
            hgis20.ignoreList['source:ingest:datetime'] = '';

            // translate.dumpLookup(hgis20.ignoreList);

            // List of layers & short versions
            hgis20.layerList = translate.flipList(hgis20.rules.layerNames);
        }

        // pre processing
        hgis20.applyToHgisPreProcessing(tags, attrs, geometryType);

        // one 2 one Rules
        translate.applyOne2One(tags, attrs, hgis20.lookup,hgis20.layerLookup, hgis20.ignoreList);

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
        if (attrs.XtableName && hgis20.layersInSchema[attrs.XtableName + ':' + geometryType])
        {
            tableName = attrs.XtableName;
            delete attrs.XtableName;

            // Check if we need to make two features
            attrs2 = hgis20.twoFeatures(geometryType,tableName,tags,attrs);

            if (attrs2.XtableName)
            {
                tableName2 = attrs2.XtableName;
                delete attrs2.XtableName;

                hgis20.validateAttrs(geometryType,tableName2,attrs2);

            } // End two features

            // Validate attrs: remove all that are not supposed to be part of a feature
            hgis20.validateAttrs(geometryType,tableName,attrs);

        } // End We have a feature
        else // Start o2s
        {
            tableName = 'o2s_' + geometryType.toString().charAt(0);

            if (attrs.XtableName)
            {
                logError('Layer: ' + attrs.XtableName + ' with Geometry: ' + geometryType + ' is not in the schema');
            }
            else
            {
                logError('Unable to determine an appropriate layer for output. Adding to ' + tableName + ' layer');
            }

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
//             if (str.length < 255 || config.getOgrSplitO2s() == 'false')
            if (str.length < 255)
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
        } // End o2s

        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true')
        {
            print('TableName: ' + tableName + '  Geom: ' + geometryType);
            for (var i in attrs) print('Out Attrs:' + i + ': :' + attrs[i] + ':');
            if (tableName2 !== '') for (var i in attrs2) print('2Out Attrs:' + i + ': :' + attrs2[i] + ':');
            print('');
        }

        var returnData = [{attrs:attrs, tableName: tableName}];

        if (tableName2 !== '')
        {
            returnData.push({attrs: attrs2, tableName: tableName2});
        }

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
