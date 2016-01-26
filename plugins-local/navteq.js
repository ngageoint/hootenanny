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
    NAVTEQ conversion script
        NAVTEQ -> OSM

    Based on nfdd/__init__.js script

    MattJ, Mar 14
*/


navteq = {
    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName) 
    {
        // List of data values to drop/ignore
        var ignoreList = { 'NULL':1, '-999999':1, 'noinformation':1 };

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
            // if (attrs[col] == '-999999' || attrValue == 'noinformation' || attrs[col] == '')  
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

        // Clean out all of the spaces in the ACT_ADDR field
        if (attrs.ACT_ADDR) attrs.ACT_ADDR = attrs.ACT_ADDR.toString().replace(/\s+/g,' ');

        // Wipe out worldwide names for state,country, district etc. We use the FEAT_COD instead
        // of the FEAT_TYPE to decide the admin_level.  If we don't get rid of these, we either have
        // heaps of "Lookup value not found..." errors OR lots more one2one rules
        var adminList = ['907196','900170','900101','900156','909996'];

        if (attrs.FEAT_COD && (adminList.indexOf(attrs.FEAT_COD) !== -1)) delete attrs.FEAT_TYPE;

    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName)
    {
        // The is the problem layer
        if (layerName == 'RailRds' && !(tags.railway)) tags.railway = 'rail';

  /*
     Still thinking about this

        // Check for empty features and add a tag depending on the layername
        if (!(tags.highway) && !(tags.amenity) && !(tags.shop) && !(tags.building) && !(tags.leisure) && !(tags.tourism) && !(tags.natural) && !(tags.place) && !(tags.barrier) && !(tags.office))
        {
            // The defaults are not great
            var layerList = {
                'AutoSvc':['building','yes'],       'BordCross':['barrier','border_control'],
                'Business':['building','commercial'], 'CommSvc':['building','commercial'],
                'EduInsts':['building','school'],   'Entertn':['building','yes'],
                'FinInsts':['building','yes'],      'Hamlet':['place','hamlet'],
                'Hospital':['building','hospital'], 'Islands':['place','island'],
                'Landmark':['poi','yes'],           'LandUseA':['poi','yes'],
                'LandUseB':['poi','yes'],           'MajHwys':['highway','road'],
                'NamedPlc':['poi','yes'],           'Parking':['amenity','parking'],
                'ParkRec':['leisure','park'],       'PostOffice':['amenity','post_office'],
                'RailRds':['railway','rail'],        'Restrnts':['amenity','restaurant'],
                'SecHwys':['highway','road'],       'Shopping':['building','commercial'],
                'Streets':['highway','road'],       'TransHubs':['building','transportation_hub'],
                'TravDest':['poi','yes'],           'WaterPoly':['waterway','river'],
                'WaterSeg':['waterway','river']
                };

            // Make sure we have a default...
            if (layerName in layerList) 
            {
                tags[layerList[layerName][0]] = layerList[layerName][1];
                print('LayerName: ' + layerName);
                for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

            }
        }
    */

        // If we have a UFI, store it
        tags.source = 'navteq'; 
        if (attrs.LINK_ID || attrs.POLYGON_ID)
        {
            if (attrs.LINK_ID)
            {
                tags.uuid = '{' + attrs.LINK_ID + '}';
            }
            else
            {
                tags.uuid = '{' + attrs.POLYGON_ID + '}';
            }
        }
        else
        {
            tags.uuid = createUuid(); 
        }

        var rampList = {'1':'motorway_link','2':'primary_link','3':'secondary_link'};

        // Sort out ramps
        if (attrs.RAMP == 'Y' && attrs.FUNC_CLASS in rampList) tags.highway = rampList[attrs.FUNC_CLASS];

        // Move Primary to Motorway if it is Controlled Access
        if (attrs.CONTRACC == 'Y' && attrs.FUNC_CLASS == '2') tags.highway = 'motorway';

        // Legal not physical divider i.e Yellow Lines
        if (attrs.DIVIDERLEG == 'Y') tags.divider = 'no';

        // Inland water features - Lake etc
        if (tags.water) tags.natural = 'water';

        // Parking 
        if (tags.park_ride || tags.building == 'garage') tags.amenity = 'parking';

        // Roads inside a parking lot
        if (attrs.PLOT_ROAD == 'yes') tags.highway = 'service';

        // Physical number of lanes - If populated, use it instead of LANE_CAT
        if (attrs.PHYS_LANES && attrs.PHYS_LANES !== '0') tags.lanes = attrs.PHYS_LANES;

        // Admin boundaries need another attribute
        if (tags.admin_level) tags.boundary = 'administrative';

        // Street Naming.
        // - POI have "addr:street"
        // - Roads have "name"
        if (tags.highway && tags['addr:street'])
        {
            tags.name = tags['addr:street'];
            delete tags['addr:street'];
        }

        // Fix up clothing stores
        if (tags.clothes) tags.shop = 'clothes'

    }, // End of applyToOsmPostProcessing
  
    // ##### End of the xxToOsmxx Block #####

    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert TO OSM
    toOsm : function(attrs, layerName)
    { 
        tags = {};  // The final output Tag list

        if (config.getOgrDebugDumptags() == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        if (navteq.lookup == undefined)
        {
            navteq.lookup = translate.createLookup(navteq.rules.one2one);
        }

        // pre processing
        navteq.applyToOsmPreProcessing(attrs, layerName);

        // one 2 one
        translate.applyOne2One(attrs, tags, navteq.lookup, {'k':'v'}, navteq.rules.txtBiased);

        // apply the simple number and text biased rules
        translate.applySimpleTxtBiased(attrs, tags, navteq.rules.txtBiased, 'forward');

        // post processing
        navteq.applyToOsmPostProcessing(attrs, tags, layerName);

        if (config.getOgrDebugDumptags() == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

        return tags;
    } // End of toOsm

} // End of nfdd
