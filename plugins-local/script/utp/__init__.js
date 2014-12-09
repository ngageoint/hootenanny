/*
    UTP conversion script
        UTP -> OSM, and
        OSM -> UTP

    ##### This has been chopped down from the bigger NFDD script. #####

    See the big__init__.js file for the rest of the values
    Based on nfdd/__init__.js script
*/

// __setupPackage__(__extension__);

require('translate')

utp = {
    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName) 
    {
        // Convert all of the Attrs to uppercase - if needed - makes life easier later
        for (var col in attrs)
        {
            var c = col.toUpperCase();
            if (c !== col)
            {
                attrs[c] = attrs[col];
                delete attrs[col];
            }
        }

        /* 
           Now fix up the crazyness with RST, SURFACE, SMC, MATERIAL and MCC.
           MCC & MATERIAL _really_ should be just for vertical construction (building, tower etc) but some of the 
           _wonderful_ folks who make UTP havw a habit of mixing RST with SURFACE with SMC with MATERIAL with MCC. 
           Some files have SURFACE = RST, others have SURFACE = SMC. Also some have MATERIAL = MCC, others have 
           MATERIAL = RST.   AAARRRGGGHHH!!!
        */

        rstList = ['paved', 'loose', 'grass', 'sod'];

        if (attrs.SURFACE)
        {
            var attrValue = attrs['SURFACE'].toString().toLowerCase();

            for (var i = 0, len = rstList.length; i < len; i++)
            {
                if (attrValue.indexOf(rstList[i]) != -1)
                {
                    attrs.RST = attrs.SURFACE;
                    delete attrs.SURFACE;
                    break;
                }
            }
        }

        if (attrs.MATERIAL && !(attrs.SURFACE))
        {
            attrs.SMC = attrs.MATERIAL;
            delete attrs.MATERIAL;
        }

        // This is a handy loop. We use it to:
        // 1) Remove all of the "No Information" and -999999 fields
        // 2) Fix the renameing of some fields

        // List of data values to drop/ignore. Love the spelling of some of these
        var ignoreList = { 'unknown':1, 'Unknown (0)':1, 'Unknown (0) (0)':1, 'unk':1, 
                           '<Null>':1, 'Null (-32768)':1, 'Null (-32768) (-32768)':1,
                           'Not Applicable (8)':1,
                           'Not Applicable (997)':1,
                           'Not Applicable (998)':1, 'Not Applicable (998) (998)':1,
                           'Not Applicabe':1, 'Not Appliccable':1, 'Not Appliicable':1,
                           'notapplicable':1, 'notdefined':1
                         };

        // Some of the attributes are repeated or have funky spelling etc. 
        // Time to go back through the list and clean-up to make it easier for the one2one rules
        // This list assumes that if "SMC" is in the file, it WILL NOT have a "MATERIAL" attribute etc
        var swapList = { 
            'BFC':'FUNCTION', 
            'BLDG_SHAPE':'BSS', 
            'BRIDG_TYPE':'BSC',
            'BRIDGETYPE':'BSC',
            'BSM':'SPAN_TYPE',
            'COMPOSED':'MCC', 
            'EXS':'EXISTENCE', 
            'GAP_WIDTH_M':'GAP_WIDTH',
            'GW1':'GAP_WIDTH',
            'HWT':'WORSHIP',
            'HYDROLOGY':'HYC', 
            'INTERCHNGE':'RIT', 
            'INTRCHANGE':'RIT', 
            'LOC':'LOCATION', 
            'MATERIAL':'MCC', 
            'PERMANENCE':'HYC', 
            'POWER':'PPC', 
            'POWER_PLNT':'PPC', 
            'POWERPLANT':'PPC', 
            'PRO':'PRODUCT', 
            'RDRNWYSURF':'RST',
            'QUALITY':'SSC',
            'RELEASABLE':'QLE',
            'SEC':'SEC_CLASS', 
            'SECURITY':'SEC_CLASS', 
            'SPRWLLCHAR':'SCC', 
            'SURFACE':'SMC',
            'SSR':'ROOF_SHAPE', 
            'TRANSPTUSE':'TUC', 
            'TRANS_USE':'TUC', 
            'TTC':'TOWER_TYPE', 
            'USE_':'USAGE', 
            'USE':'USAGE', 
            'USAGE_':'USAGE', 
            'WEATHER':'WTC',
            'WEATHERTYP':'WTC',
            'WORSHIPTYP':'WORSHIP',
        };

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

            // Now clean up the funky variations in the attribute names
            if (col in swapList)  
            {
                attrs[swapList[col]] = attrs[col];
                delete attrs[col]; // debug: Comment this out to leave all of the junk in for testing
            }

        } // End in attrs loop


        // Need to fix the RST/SMC mix - This is done AFTER the initial cleanup and attribute swap
        // RST = paved, unpaved or grass
        // SMC = asphalt, concrete, metal etc
        // We are going to go with the more detailed surface info (Asphalt) instead of the generic "Hard Paved"
        if (attrs.RST)
        {
            // If we don't have a Surface Material, use the Road Surface Type
            if (!(attrs.SMC)) attrs.SMC = attrs.RST;

            delete attrs.RST;
        }

        // Clean up Width, Length etc. Remove units: '10 meters' -> '10'
        if (attrs.WIDTH) attrs.WIDTH = attrs.WIDTH.replace(' meters','');
        if (attrs.LENGTH) attrs.LENGTH = attrs.LENGTH.replace(' meters','');
        if (attrs.HEIGHT) attrs.HEIGHT = attrs.HEIGHT.replace(' meters','');
        if (attrs.ELEVATION) attrs.ELEVATION = attrs.ELEVATION.replace(' meters','');

        // Now find an FCODE.
        if (attrs.F_CODE)
        {
            // Nothing to do :-)
        }
        if (attrs.FAC_CODE) // Part of the sample files
        {
            attrs.F_CODE = attrs.FAC_CODE;
            delete attrs.FAC_CODE;
        }
        else if (attrs.FCODE)
        {
             attrs.F_CODE = attrs.FCODE;
             delete attrs.FCODE;
        }

    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName)
    {
        // Built-up-areas & Settlements vs Buildings
        // If we have a BUA, change the building tags to settlement:type
        if ((attrs.F_CODE == 'AL020' || attrs.F_CODE == 'AL105') && tags.building)
        {
            tags['settlement:type'] = tags.building;
            delete tags.building;
        }

        // School vs University vs Academy
        if (attrs.ED_BLDG == 'University') tags.building = 'university'

        // Add extra tags for factories etc
        if (tags.industrial)
        {
            tags.building = 'industrial';
            tags.use = tags.industrial;
            delete tags.industrial;
        }

        // Alt Names: There could be three of these
        if (attrs.ALT_NAME_1) tags.alt_name = attrs.ALT_NAME_1;

        if (attrs.ALT_NAME_2)
        {
            if (tags.alt_name)
            {
                tags.alt_name = tags.alt_name + ';' + attrs.ALT_NAME_2;
            }
            else
            {
                tags.alt_name = attrs.ALT_NAME_2;
            }
        }

        if (attrs.ALT_NAME_3)
        {
            if (tags.alt_name)
            {
                tags.alt_name = tags.alt_name + ';' + attrs.ALT_NAME_3;
            }
            else
            {
                tags.alt_name = attrs.ALT_NAME_3;
            }
        }

        // AQ060 Airport Control Tower needs some extra tags, post processing adds another tag
        // if (tags.aeroway == 'tower') tags['tower:type'] = 'observation';
        if (tags.man_made == 'control_tower') tags.use = 'air_traffic_control';

        // AT030 Powerline -> AT005 in TDS. 
        if (tags['cable:type'] == 'power' && tags.pylon !== 'yes') tags.cable = 'yes';

        // Gardens
        if (tags['garden:type']) tags.leisure = 'garden';

      // Should be disabled until polygons are fixed....
        translate.fixConstruction(tags, 'highway');
        translate.fixConstruction(tags, 'railway');

        // Fix up the 'surface' values for buildings
        if (attrs.F_CODE == 'AL015' && tags.surface == 'unknown')  
        {
            delete tags.surface;
        }

        // Add a building tag to buildings if we don't have one.
        // for: amenity = XXX etc
        if (attrs.F_CODE == 'AL015' && !(tags.building))  tags.building = 'yes';

        // Add a building tag to "Communications Buildings"
        if (attrs.F_CODE == 'AT050' && !(tags.building))  tags.building = 'yes';

        // temp hack from the osm rules replacing type='yes' with type='building'
        // Not sure why.
        // if (attrs.F_CODE == 'AL015' && !(tags.building))  tags.building = 'building';

        // If we have a Tower, Add a man_made tag
        if (tags['tower:type']) tags.man_made = 'tower';

        // Add a UUID
        tags.source = 'utp';
        tags.uuid = createUuid();

        // Fix up the "social_facility" tags
        // Need to expand this to handle other types
        if (tags.social_facility)
        {
            // print('## Social_facility = ' + tags.social_facility);
            tags.amenity = "social_facility";
            tags['social_facility:for'] = tags.social_facility;
            delete tags.social_facility;
        }


    }, // End of applyToOsmPostProcessing
  
    // ##### End of the xxToOsmxx Block #####

    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert TO OSM
    toOsm : function(attrs, layerName)
    { 
        tags = {};  // The final output Tag list

        if (utp.lookup == undefined)
        {
            // Setup lookup tables to make translation easier. I'm aumeing that since this is not set, the 
            // other tables are not set either.

            // Add the FCODE rules to the one2one rules.
            utp.rules.one2one.push.apply(utp.rules.one2one,fcodeCommon.one2one);
            utp.rules.one2one.push.apply(utp.rules.one2one,utp.rules.fcodeOne2oneIn);
            
            utp.lookup = translate.createLookup(utp.rules.one2one);

            // Build an Object with both the SimpleText & SimpleNum lists
            utp.biasedList = translate.joinList(utp.rules.numBiased, utp.rules.txtBiased);
            
            // Add features to ignore
            utp.biasedList.F_CODE = '';
        }

        // pre processing
        utp.applyToOsmPreProcessing(attrs, layerName);

        // one 2 one
        translate.applyOne2One(attrs, tags, utp.lookup, {'k':'v'}, utp.biasedList);
        
        // apply the simple number and text biased rules
        translate.applySimpleNumBiased(attrs, tags, utp.rules.numBiased, 'forward');
        translate.applySimpleTxtBiased(attrs, tags, utp.rules.txtBiased, 'forward');

        // post processing
        utp.applyToOsmPostProcessing(attrs, tags, layerName);

        if (getHootConfig('ogr.debug.dumpattrs') == 'true') for (var i in attrs) print('In Attrs:' + i + ': :' + attrs[i] + ':');
        if (getHootConfig('ogr.debug.dumptags') == 'true') for (var i in tags) print('Out Tags: ' + i + ': :' + tags[i] + ':');

        // debug: Add the FCODE to the tags
        if (getHootConfig('ogr.debug.addfcode') == 'true') tags['raw:debugFcode'] = attrs.F_CODE;

        return tags;
    }, // End of toOsm

} // End of utp
