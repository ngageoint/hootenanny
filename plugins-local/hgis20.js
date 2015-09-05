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
            hgis20.layersInSchema[rawSchema[i].name] = 1;

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
    //                feature and checking enumerated values.
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

        // Now validate the Enumerated values
        for (var i=0, cLen = feature['columns'].length; i < cLen; i++)
        {
            // Skip non enumeratied attributes
            if (feature.columns[i].type !== 'enumeration') continue;

            var enumName = feature.columns[i].name;

            // Skip stuff that is missing and will end up as a default value
            if (!(attrs[enumName])) continue;

            var attrValue = attrs[enumName];
            var enumList = feature.columns[i].enumerations;
            var enumValueList = [];

            // Pull all of the values out of the enumerated list to make life easier
            for (var j=0, elen = enumList.length; j < elen; j++) enumValueList.push(enumList[j].value);

            // If we DONT have the value in the list, Set it to "other" and jam it into the COMMENTS field.
            // Note: From the spec, if we have an enumerated value, we have a COMMENTS field as well.
            if (enumValueList.indexOf(attrValue) == -1)
            {
                var othVal = enumName + ':' + attrValue;

                // No "Other" value. Push to the Memo field
                if (enumValueList.indexOf('999999') == -1)
                {
                    // Set the offending enumerated value to the default value
                    attrs[enumName] = feature.columns[i].defValue;
                    logWarn('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + '. Setting ' + enumName + ' to its default value (' + feature.columns[i].defValue + ')');
                }
                else // We have an "other" value
                {
                    // Set the offending enumerated value to the "other" value
                    attrs[enumName] = '999999';
                    logWarn('Validate: Enumerated Value: ' + attrValue + ' not found in ' + enumName + '. Setting ' + enumName + ' to Other (999999)');
                }

                // Add the value to the COMMENTS field
                attrs.COMMENTS = translate.appendValue(attrs.COMMENTS,othVal,';');

            } // End attrValue in enumList

        } // End Validate Enumerations

    }, // End validateAttrs


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

        //Sort out TYPE, TYPE1 & TYPE2. This is to make life easier in the one2one rules
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
        else
        {
            logError('Layer: ' + layerName + ' not found in layer names list.');
        }


    }, // End of applyToOsmPreProcessing

    applyToOsmPostProcessing : function (attrs, tags, layerName)
    {

        // If we have a UFI, store it. Some of the MAAX data has a LINK_ID instead of a UFI
        tags.source = 'hgisv20';
        tags.uuid = '{' + attrs['UFI'].toString().toLowerCase() + '}';


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


    }, // End applyToHgisPreProcessing

    applyToHgisPostProcessing : function (tags, attrs, geometryType)
    {
        // Now use the lookup table to find layerName. This is here to stop clashes with the
        // standard one2one rules
        if (!(attrs.XtableName) && hgis20.layerLookup)
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
                        // print('layerName: Got ' + attrs.F_CODE);
                    }
                }
            }
        } // End find layerName

        // Start looking for a Table Name
        // Next step is to make this a list: we might get more than one match...
        for (var val in attrs)
        {
           // Unsplit the layer and the type
            if (val.indexOf('$') !== -1)
            {
               var tList = val.split('$');
               attrs.XtableName = hgis20.layerList[tList[0]];
               attrs[tList[1]] = attrs[val];
               delete attrs[val];
            }
        }

        // ######################
        // Hardcoded Layer selection. Yes, this is ugly
        // Will move this to a custom rules function - soon

        // Educational_Institutions
        if (tags.amenity == 'school' || tags.building == 'school') attrs.XtableName = 'Educational_Institutions';

        // Geonames
        if (attrs.DSG || (attrs.place && attrs.place !== 'farm'))
        {
            attrs.XtableName = 'Geonames';
            if (attrs.NAME)
            {
                attrs.FULL_NAME = attrs.NAME;
                attrs.SORT_NAME = attrs.NAME;
                delete attrs.NAME;
            }
        }

        // Internet_Cafes
        if (tags.amenity == 'internet_cafe') attrs.XtableName = 'Internet_Cafes';

        // Libraries
        if (tags.amenity == 'library') attrs.XtableName = 'Libraries';

        // Power Plants
        if (tags.power == 'plant') attrs.XtableName = 'Power_Plants';

        // Prisons
        if (tags.amenity == 'prison') attrs.XtableName = 'Prisons';

        // Recreation POI
        if (tags.amenity == 'swimming_pool') attrs.XtableName = 'Recreation_POI';
        
        if (tags.leisure || tags.sport)
        {
            // Water park is a Tourist Location POI
            if (!attrs.leisure == 'water_park')
            {
                attrs.XtableName = 'Recreation_POI';
                if (!attrs.TYPE)
                {
                    attrs.TYPE = 'Other';

                    var othVal = 'Type:';
                    (tags.leisure) ? othVal += tags.leisure : othVal += tags.sport ;

                    attrs.COMMENTS = translate.appendValue(attrs.COMMENTS,othVal,';');
                }
            }
        }


        // Religious Institutions
        if (tags.amenity == 'place_of_worship' && geometryType == 'Point') attrs.XtableName = 'Religious_Institutions';

        // Tourist Locations POI
        if (tags.tourism && tags.tourism !== 'information')
        {
            attrs.XtableName = 'Recreation_POI';
            if (!attrs.TYPE)
            {
                attrs.TYPE = 'Other';
                print('RecPOI: Tourism:' + tags.tourism + ' Comment:' + attrs.COMMENTS);

                attrs.COMMENTS = translate.appendValue(attrs.COMMENTS,'Type:' + tags.tourism,';');
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
            hgis20.ignoreList.SHAPE_Length = '';
            hgis20.ignoreList.SHAPE_Area = '';
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
        // There is no way we can translate these to a single feature.
        if (geometryType == 'Collection') return null;


        // We assume that if one table is undefined, the rest are.
        if (hgis20.lookup == undefined)
        {
            // Add "other" rules to the one2one
            hgis20.rules.one2one.push.apply(hgis20.rules.one2one,hgis20.rules.one2oneOut);

            hgis20.lookup = translate.createBackwardsLookup(hgis20.rules.one2one);
            // translate.dumpOne2OneLookup(hgis20.lookup);

            hgis20.layerLookup = translate.createBackwardsLookup(hgis20.rules.layerOut);
            // translate.dumpOne2OneLookup(hgis20.layerLookup);

            // Build a list of things to ignore and flip is backwards
            hgis20.ignoreList = translate.flipList(translate.joinList(hgis20.rules.numBiased, hgis20.rules.txtBiased));
            
            // Add some more features to ignore
            hgis20.ignoreList.uuid = '';
            hgis20.ignoreList.source = '';
            hgis20.ignoreList.area = '';
            hgis20.ignoreList['note:extra'] = '';
            hgis20.ignoreList['hoot:status'] = '';
            hgis20.ignoreList['hoot:score:match'] = '';
            hgis20.ignoreList['hoot:score:miss'] = '';
            hgis20.ignoreList['hoot:score:review'] = '';
            hgis20.ignoreList['hoot:score:uuid'] = '';
            hgis20.ignoreList['error:circular'] = '';

            // translate.dumpLookup(hgis20.ignoreList);

            // List of layers & short versions
            hgis20.layerList = translate.flipList(hgis20.rules.layerNames);
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
        if (attrs.XtableName && hgis20.layersInSchema[attrs.XtableName])
        {
            tableName = attrs.XtableName;
            delete attrs.XtableName;
            
            // Validate attrs: remove all that are not supposed to be part of a feature
            hgis20.validateAttrs(geometryType,tableName,attrs);

        } // End We have a feature
        else
        {
            logError('Layer and Geometry: ' + tableName + ' is not in the schema');

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
        }


        // Debug:
        if (config.getOgrDebugDumpattrs() == 'true' || config.getOgrDebugDumptags() == 'true')
        {
            print('TableName: ' + tableName + '  Geom: ' + geometryType);
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
