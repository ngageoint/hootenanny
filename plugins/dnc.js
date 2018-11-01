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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    DNC conversion script
        DNC -> OSM
        
    Based on tds/__init__.js script
*/

dnc = {
    // ##### Start of the xxToOsmxx Block #####
    applyToOsmPreProcessing: function(attrs, layerName, geometryType)
    {
        // List of data values to drop/ignore
        var ignoreList = { 'UNK':1, 'N/A':1, 'noinformation':1, '-32768':1 };

        // This is a handy loop. We use it to:
        // 1) Remove all of the "No Information" and -999999 fields
        // 2) Convert all of the Attrs to uppercase - if needed
        // 3) Swap some of the funky named attrs around. 
        // 4) Move the appropriate XXX2 and XXX3 attributes to a 'Note'
        for (var col in attrs)
        {
            // Trim off the whitespace
            attrs[col] = attrs[col].trim();
        
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

    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName, geometryType)
    {
        // DNC doesn't have a lot of info for land features
        if (attrs.F_CODE == 'AP020') tags.junction = 'yes';
        if (attrs.F_CODE == 'AP030') tags.highway = 'road';
        if (attrs.F_CODE == 'BH140') tags.waterway = 'river';

        // If we have a UFI, store it.
        tags.source = 'dnc:' + layerName.toLowerCase();;
        if (attrs.UFI)
        {
            tags.uuid = '{' + attrs['UFI'].toString().toLowerCase() + '}';
        }
        else
        {
            tags.uuid = createUuid(); 
        }

        if (dnc.osmPostRules == undefined)
        {
            // ##############
            // A "new" way of specifying rules. Jason came up with this while playing around with NodeJs
            //
            // Rules format:  ["test expression","output result"];
            // Note: t = tags, a = attrs and attrs can only be on the RHS
            var rulesList = [
            ["t.man_made == 'radar_station'","t['radar:use'] = 'early_warning';"],
            ["t['cable:type'] && !(t.cable)","t.cable = 'yes';"],
            ["t['tower:type'] && !(t.man_made)","t.man_made = 'tower'"],
            ["t.foreshore && !(t.tidal)","t.tidal = 'yes'; t.natural = 'water'"],
            ["t.tidal && !(t.water)","t.natural = 'water'"]
            ];

            dnc.osmPostRules = translate.buildComplexRules(rulesList);
        }

        // translate.applyComplexRules(tags,attrs,rulesList);
        translate.applyComplexRules(tags,attrs,dnc.osmPostRules);

    }, // End of applyToOsmPostProcessing
  
    // ##### End of the xxToOsmxx Block #####

    // toOsm - Translate Attrs to Tags
    // This is the main routine to convert _TO_ OSM
    toOsm : function(attrs, layerName, geometryType)
    {
        tags = {};  // The final output Tag list

        // Debug:
        if (config.getOgrDebugDumptags() == 'true')
        {
            print('In Layername: ' + layerName);
            var kList = Object.keys(attrs).sort()
            for (var i = 0, fLen = kList.length; i < fLen; i++) print('In Attrs: ' + kList[i] + ': :' + attrs[kList[i]] + ':');
        }

        // Set up the fcode translation rules. We need this due to clashes between the one2one and
        // the fcode one2one rules
        if (dnc.fcodeLookup == undefined)
        {
            // Add the FCODE rules for Import
            fcodeCommon.one2one.push.apply(fcodeCommon.one2one,dnc.rules.fcodeOne2oneIn);

            dnc.fcodeLookup = translate.createLookup(fcodeCommon.one2one);
            // translate.dumpOne2OneLookup(dnc.fcodeLookup);
        }

        if (dnc.lookup == undefined)
        {
            // Setup lookup tables to make translation easier. I'm assumeing that since this is not set, the 
            // other tables are not set either.

            dnc.lookup = translate.createLookup(dnc.rules.one2one);

            // Build an Object with both the SimpleText & SimpleNum lists
            dnc.ignoreList = translate.joinList(dnc.rules.numBiased, dnc.rules.txtBiased);
            
            // Add features to ignore
            dnc.ignoreList.F_CODE = '';
            dnc.ignoreList.FAC_ID = '';
            dnc.ignoreList.ID = '';
            dnc.ignoreList.TILE_ID = '';
            dnc.ignoreList.EDG_ID = '';
            dnc.ignoreList.END_ID = '';
        }

        // pre processing
        dnc.applyToOsmPreProcessing(attrs, layerName, geometryType);

        // Use the FCODE to add some tags.
        if (attrs.F_CODE)
        {
            var ftag = dnc.fcodeLookup['F_CODE'][attrs.F_CODE];
            if (ftag)
            {
                tags[ftag[0]] = ftag[1];
                // Debug: Dump out the tags from the FCODE
                // print('FCODE: ' + attrs.F_CODE + ' tag=' + ftag[0] + '  value=' + ftag[1]);
            }
            else
            {
                hoot.logError('Translation for FCODE ' + attrs.F_CODE + ' not found');
            }
        }

        // Make a copy of the input attributes so we can remove them as they get translated. Looking at what
        // isn't used in the translation - this should end up empty.
        // not in v8 yet: // var tTags = Object.assign({},tags);
        var notUsedAttrs = (JSON.parse(JSON.stringify(attrs)));

        // apply the simple number and text biased rules
        // NOTE: We are not using the intList paramater for applySimpleNumBiased when going to OSM.
        translate.applySimpleNumBiased(notUsedAttrs, tags, dnc.rules.numBiased, 'forward',[]);
        translate.applySimpleTxtBiased(notUsedAttrs, tags, dnc.rules.txtBiased, 'forward');

        // one 2 one
        translate.applyOne2One(notUsedAttrs, tags, dnc.lookup, {'k':'v'}, dnc.ignoreList);

        // post processing
        dnc.applyToOsmPostProcessing(attrs, tags, layerName, geometryType);

        // Debug
        // for (var i in notUsedAttrs) print('NotUsed: ' + i + ': :' + notUsedAttrs[i] + ':');

        // debug: Add the FCODE to the tags
        if (config.getOgrDebugAddfcode() == 'true') tags['raw:debugFcode'] = attrs.F_CODE;
        
        // Debug:
        if (config.getOgrDebugDumptags() == 'true')
        {
            var kList = Object.keys(tags).sort()
            for (var i = 0, fLen = kList.length; i < fLen; i++) print('Out Tags: ' + kList[i] + ': :' + tags[kList[i]] + ':');
            print('');
        }

        return tags;

    }, // End of toOsm

} // End of dnc
