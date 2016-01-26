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
    FACC conversion script
        FACC -> OSM

    Based on nfdd/__init__.js script

    Possible attribute values are taken from somewhere....

    MattJ, May 14
*/

hoot.require('translate');

facc = {
    // ##### Start of the common block #####

    // applySimpleNumBiased - Apply 0ne2one rules for Number Attributes
    // The "direction is either "forward" or "backward" - convert to or from
    applySimpleNumBiased : function(attrs, tags, direction)
    {
     var numBiased = {  

         // Nothing to do here

        }; // End numBiased

     // Convert Attrs to Tags
     for (var i in numBiased)
     {
         if (i in attrs)
         {
             // Sanity checking :-)
             if (translate.isOK(attrs[i])) tags[numBiased[i]] = attrs[i];
         }
     }
    }, // End applySimpleNumBiased

    // applySimpleTxtBiased - Apply 0ne2one rules for Text Attributes
    // The "direction is either "forward" or "backward" - convert to or from
    applySimpleTxtBiased : function(attrs, tags, direction)
    {
     var txtBiased = {  
            'ACT_ADDR':'addr:full', // In NAVTEQ, this is the _actual_ address not just the street
            'ACT_LANGCD':'language', // The language the Railway name is in
            'ACT_POSTAL':'addr:postcode', // The Actual postcode
            'ACT_ST_NUM':'addr:housenumber', // Street Number
            'ACT_ST_NAM':'addr:street', // Street Name ** Will de-conflict this in Custom Rules
            'HIGHWAY_NM':'name', // Major Highways
            'LANG_CODE':'language', // The language the Railway name is in
            'NM_LANGCD':'language', // The language the name is in
            'PH_NUMBER':'phone', // Phone Number
            'POLYGON_NM':'name', // Waterways etc
            'POI_LANGCD':'language', // The language the POIN name is in
            'POI_NAME':'name', // POI Name
            'POI_ST_NUM':'addr:housenumber', // Street Number
            'ST_LANGCD':'addr:language', // The language street name is in
            'ST_NAME':'addr:street', // Street Name ** Will de-conflict this in Custom Rules
        }; // End txtBiased

     // Convert Attrs to Tags
     for (var i in txtBiased)
     {
         if (i in attrs)
         {
             // Sanity checking :-)
             if (!(translate.isUnknown(attrs[i]))) tags[txtBiased[i]] = attrs[i];
         }
     }
    }, // End applySimpleTxtBiased

    // ##### End of common block #####

    // ##### Start of the xxToOsmxx Block #####
    applyToOsmCustomRules : function(attrs, tags, layerName)
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
        tags.source = 'DNC'; 
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

    }, // End of applyToOsmCustomRules

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

        // Nothing to do yet

    }, // End of applyToOsmPostProcessing
  
    // ##### End of the xxToOsmxx Block #####

    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert TO OSM
    toOsm : function(attrs, layerName)
    { 
        tags = {};  // The final output Tag list

        if (getHootConfig('ogr.debug.dumptags') == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');

        // Very simple dump
        for (var i in attrs) tags[i] = attrs[i];

/*
        if (facc.lookup == undefined)
        {
            facc.lookup = translate.createLookup(facc.rules.one2one);
        }

        // pre processing
        facc.applyToOsmPreProcessing(attrs, layerName);

        // one 2 one
        translate.applyOne2One(attrs, tags, facc.lookup);

        // apply the simple number and text biased rules
        // facc.applySimpleNumBiased(attrs, tags);
        facc.applySimpleTxtBiased(attrs, tags);

        // custom rules
        facc.applyToOsmCustomRules(attrs, tags, layerName);

        // post processing
        facc.applyToOsmPostProcessing(attrs, tags, layerName);
*/

        if (getHootConfig('ogr.debug.dumptags') == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

        return tags;
    } // End of toOsm

} // End of facc
