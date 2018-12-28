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
 * @copyright Copyright (C) 2014, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
   PrintOsmDocs.js

   This file "hijacks" either the "initialize" or the "translateToOsm" function 
   to print out documentation.

   This is the default translation file for hoot print-docs
*/

// Set up stubs for the namespaces.
// If we don't do this, v8 complains about "xx.rules"
tds40 = {}
tds61 = {}
mgcp = {}
ggdm30 = {}
geonames = {}
// ufd = {}
// dnc = {}
// utp = {}


// Grab all of the init and rules files so we have somthing to work with
hoot.require('tds40_rules')
hoot.require('SchemaTools');
hoot.require('tds61_rules')
hoot.require('mgcp_rules')
hoot.require('ggdm30_rules')
// hoot.require('ufd_rules')
hoot.require('geonames_rules')
hoot.require('fcode_common')
hoot.require('config')


// The initialize function gets called once BEFORE the translateToXXX functions
// We are hijacking this function to print out documentation
function initialize()
{
  /*
    All of the lookup tables:
     mgcp.rules.one2one
     mgcp.rules.one2oneIn
     tds40.rules.one2one
     tds40.rules.one2oneIn
     tds61.rules.one2one
     tds61.rules.one2oneIn
     ufd.rules.one2one
     utp.rules.one2one
     dnc.rules.one2one
     navteq.rules.one2one
     geonames.rules.one2one <- Non standard format

   Biased:
     tds40.rules.txtBiased
     tds40.rules.numBiased
     tds61.rules.txtBiased
     tds61.rules.numBiased
     mgcp.rules.txtBiased
     mgcp.rules.numBiased
     ufd.rules.txtBiased
     ufd.rules.numBiased
     utp.rules.txtBiased
     utp.rules.numBiased
     dnc.rules.txtBiased
     dnc.rules.numBiased
     navteq.rules.txtBiased

   FCODE Lookup Tables:
     fcodeCommon.one2one
     mgcp.rules.fcodeOne2oneIn
     ufd.rules.fcodeOne2oneIn
     utp.rules.fcodeOne2oneIn
     tds40.rules.fcodeOne2oneIn
     tds40.rules.fcodeOne2oneOut
     tds61.rules.fcodeOne2oneIn
     tds61.rules.fcodeOne2oneIn
     dnc.rules.fcodeOne2oneOut
  */

    var tagList = {} // Final list of tags

    // Standard one2one rules
    createLookup(tagList, ggdm30.rules.one2one, 'GGDM30');
    createLookup(tagList, ggdm30.rules.one2oneIn, 'GGDM30');
    createLookup(tagList, tds.rules.one2one, 'TDS40');
    createLookup(tagList, tds.rules.one2oneIn, 'TDS40');
    createLookup(tagList, tds61.rules.one2one, 'TDS61');
    createLookup(tagList, tds61.rules.one2oneIn, 'TDS61');
    createLookup(tagList, mgcp.rules.one2one, 'MGCP');
    createLookup(tagList, mgcp.rules.one2oneIn, 'MGCP');
    createLookup(tagList, geonames.rules.one2one, 'GeoNames');

    // createLookup(tagList, ufd.rules.one2one, 'UFD');
    // createLookup(tagList, utp.rules.one2one, 'UTP');
    // createLookup(tagList, dnc.rules.one2one, 'DNC');
    // createLookup(tagList, navteq.rules.one2one, 'NAVTEQ');

    // Add in the Biased rules
    createBiasedLookup(tagList, ggdm30.rules.txtBiased, 'GGDM30');
    createBiasedLookup(tagList, ggdm30.rules.numBiased, 'GGDM30');
    createBiasedLookup(tagList, tds.rules.txtBiased, 'TDS40');
    createBiasedLookup(tagList, tds.rules.numBiased, 'TDS40');
    createBiasedLookup(tagList, tds61.rules.txtBiased, 'TDS61');
    createBiasedLookup(tagList, tds61.rules.numBiased, 'TDS61');
    createBiasedLookup(tagList, mgcp.rules.txtBiased, 'MGCP');
    createBiasedLookup(tagList, mgcp.rules.numBiased, 'MGCP');
    // createBiasedLookup(tagList, ufd.rules.numBiased, 'UFD');
    // createBiasedLookup(tagList, ufd.rules.txtBiased, 'UFD');
    // createBiasedLookup(tagList, utp.rules.numBiased, 'UTP');
    // createBiasedLookup(tagList, utp.rules.txtBiased, 'UTP');
    // createBiasedLookup(tagList, dnc.rules.numBiased, 'DNC');
    // createBiasedLookup(tagList, dnc.rules.txtBiased, 'DNC');
    // createBiasedLookup(tagList, NAVTEQ.rules.txtBiased, 'NAVTEQ');

    // F_CODE one2one rules
    createLookup(tagList, fcodeCommon.one2one, 'NFDD');
    createLookup(tagList, ggdm30.rules.fcodeOne2oneIn, 'GGDM30');
    createLookup(tagList, tds.rules.fcodeOne2oneIn, 'TDS40');
    createLookup(tagList, tds61.rules.fcodeOne2oneIn, 'TDS61');
    createLookup(tagList, mgcp.rules.fcodeOne2oneIn, 'MGCP');
    // createLookup(tagList, ufd.rules.fcodeOne2oneIn, 'UFD');
    // createLookup(tagList, utp.rules.fcodeOne2oneIn, 'UTP');
    // createLookup(tagList, dnc.rules.fcodeOne2oneIn, 'DNC');

    // Print the tagList
    switch (config.getTagPrintingFormat())
    {
        case 'html':
            dumpHtmlTags(tagList);
            break;

        case 'json':
            dumpJsonTags(tagList);
            break;

        case 'csv':
            dumpCsvTags(tagList);
            break;

        case 'redmine':
            dumpRedmineTags(tagList);
            break;

        case 'asciidoc':
        default:
            dumpAsciidocTags(tagList);
            break;
    }

    return null;
} // end Initialize

// The finalize function gets called once AFTER the translateToXXX functions
function finalize()
{
    // print('In the finalize function');
}


// We are not using this any more.
function translateToOsm(attrs, layerName, geometryType)
{ 
    return null;
}


// Adds values to the tagList lookup table
function createLookup(tagList, one2one, source)
{
    var nlist = [];
    var tagShort = '';
    var hValue = '';
    var desc = '';
    var tsrc = '';

    // Grab the lookup table for the attr values
    var attrList = attrLookupTable();

    for (var r in one2one)
    {
        var row = one2one[r];
        // row[0] = Attribute
        // row[1] = Attribute value
        // row[2] = OSM+ tag
        // row[3] = OSM+ Tag value

        if (row[2]) // Make sure it isn't 'undefined'
        {
            nList = row[2].split(':');
            tagShort = nList[0];

            // This should make a unique Hash Value: Tag Name + Tag value
            hValue = row[2] + '_' + row[3];

            desc = attrList[row[0]];
            if (row[0] == 'F_CODE') desc = 'Feature Code ' + row[1];
            if (!desc) desc = '##### Text or number value';

            // New tag value
            if (!(tagShort in tagList))
            {
                tagList[tagShort] = {};
                tsrc = row[0] + ':' + source;
                if (row[0] == 'F_CODE') tsrc = row[1] + ':' + source;
                // tagList[tagShort][hValue] = { 'tag': row[2], 'value':row[3], 'attr':row[0], 'desc':desc, 'src':tsrc }
                tagList[tagShort][hValue] = { 'tag': row[2], 'value':row[3], 'desc':desc, 'src':tsrc }
                continue;
            }

            // We have the shortTag _and_ a value for it
            if (tagList[tagShort][hValue])
            {
                // Same tag and attribute so update the source of the tag
                if (tagList[tagShort][hValue]['src'].indexOf(source) == -1 || tagList[tagShort][hValue]['src'].indexOf(row[0]) == -1)  
                {
                    tsrc = tagList[tagShort][hValue]['src'] + ', ' + row[1] + ':' + source;

                    if (row[0] !== 'F_CODE') 
                    {
                        tsrc = tagList[tagShort][hValue]['src'] + ', ' + row[0] + ':' + source;
                        tagList[tagShort][hValue]['desc'] = desc;
                    }

                    tagList[tagShort][hValue]['src'] = tsrc;
                }
                continue;
            }

            // We have a shortTag but havent seen _this_ value for it.
            tsrc = row[0] + ':' + source;
            if (row[0] == 'F_CODE') tsrc = row[1] + ':' + source;
            // tagList[tagShort][hValue] = { 'tag':row[2], 'value':row[3], 'attr':row[0], 'desc':desc, 'src':tsrc }
            tagList[tagShort][hValue] = { 'tag':row[2], 'value':row[3], 'desc':desc, 'src':tsrc }
        }
    }
} // End createLookup


// Add Text and Number tags to the tagList lookup table
// Same code as the "standard" createLookup() function
function createBiasedLookup(tagList, one2one, source)
{
    var desc = '';
    var hValue = '';
    var nList = [];
    var tag = '';
    var tagShort = '';
    var tsrc = '';

    // Grab the lookup table for the attr values
    var attrList = attrLookupTable();

    for (var attr in one2one)
    {
        tag = one2one[attr];

        nList = tag.split(':');
        tagShort = nList[0];

        desc = attrList[attr];
        if (!desc) desc = '';

        // Not great but OK hash value. We are not expecting any clashes.....
        hValue = tag + '_';

        if (!(tagShort in tagList))
        {
            tagList[tagShort] = {};
            tsrc = attr + ':' + source;
            tagList[tagShort][hValue] = { 'tag': tag, 'value':'Text or number value', 'desc':desc, 'src':tsrc }
            continue;
        }

        if (tagList[tagShort][hValue])
        {
            if (tagList[tagShort][hValue]['src'].indexOf(source) == -1 || tagList[tagShort][hValue]['src'].indexOf(attr) == -1)  
            {
                tsrc = tagList[tagShort][hValue]['src'] + ', ' + attr + ':' + source;
                tagList[tagShort][hValue]['src'] = tsrc;
            }
            continue;
        }

        tsrc = attr + ':' + source;
        tagList[tagShort][hValue] = { 'tag': tag, 'value':'Text or number value', 'desc':desc, 'src':tsrc }
    }
} // End createBiasedLookup

// Dump the tagList out in HTML format.
function dumpHtmlTags(tagList)
{
    var desc = '';
    var shortTag = '';
    var source = '';
    var tag = '';
    var val = '';
    var value = '';

    var shortKeys = Object.keys(tagList);
    shortKeys.sort();

    // Start the HTML document
    print('<html>');
    print('<head>');
    print('<title>Hootennanny OSM+ Tags</title>');
    print('</head>');
    print('<style>');
    print('table,th,td { border:1px solid black; border-collapse:collapse; padding-left:5px; padding-right:5px; }');
    print('caption { text-align:left; font-weight:bold;}');
    print('</style>');
    print('<body>');
    print('<h2 align=center>Hootennanny OSM+ Tags</h2>');
    print('<p>Generated: ' + Date() + '</p>');

    for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
    {
        shortTag = shortKeys[i];

        print('<table>');
        print('<caption>' + shortTag + '<caption>');
        print('<tr>');
        print('<th>Key</th><th>Value</th><th>Comment</th><th>Source</th>');
        print('</tr>');

        var extraKeys = Object.keys(tagList[shortTag]);
        extraKeys.sort();

        for (var j = 0, elen = extraKeys.length; j < elen; j++)
        {
            val = extraKeys[j];

            // Just to make things a bit clearer while building the print statement
            desc = tagList[shortTag][val]['desc'];
            source = tagList[shortTag][val]['src'];
            tag = tagList[shortTag][val]['tag'];
            value = tagList[shortTag][val]['value'];

            print('<tr>');
            print('<td>' + tag + '</td><td>' + value + '</td><td>' + desc + '</td><td>' + source + '</td>');
            print('</tr>');
        }
        print('</table>');
        print('<br>');
        print('<br>');
    }

    // End the HTML document
    print('</body>');
    print('</html>');

} // End dumpHtmlTags


// Dump the tagList out in asciidoc format.
function dumpAsciidocTags(tagList)
{
    var desc = '';
    var shortTag = '';
    var source = '';
    var tag = '';
    var val = '';
    var value = '';

    var shortKeys = Object.keys(tagList);
    shortKeys.sort();

    print('Hootennanny OSM+ Tags');
    print('=====================');
    print('Generated: ' + Date());
    print('');

    for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
    {
        shortTag = shortKeys[i];

        // print('.An ' + shortTag);
        print('.' + shortTag);
        print('[options="header"]');
        print('|==========');
        print('| Key | Value | Comment | Source');

        var extraKeys = Object.keys(tagList[shortTag]);
        extraKeys.sort();

        for (var j = 0, elen = extraKeys.length; j < elen; j++)
        {
            val = extraKeys[j];

            // Just to make things a bit clearer while building the print statement
            desc = tagList[shortTag][val]['desc'];
            source = tagList[shortTag][val]['src'];
            tag = tagList[shortTag][val]['tag'];
            value = tagList[shortTag][val]['value'];

            print('| ' + tag + ' | ' + value + ' | ' + desc + ' | ' + source);
        }

        print('|==========');
        print('');
        print('');
    }

} // End dumpAsciidocTags


// Dump the tagList out in Redmine format.
function dumpRedmineTags(tagList)
{
    var desc = '';
    var shortTag = '';
    var source = '';
    var tag = '';
    var val = '';
    var value = '';

    var shortKeys = Object.keys(tagList);
    shortKeys.sort();

    print('h1. Hootennanny OSM+ Tags');
    print('');
    print('Generated: ' + Date());
    print('');
    print('----');
    print('');

    for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
    {
        shortTag = shortKeys[i];

        print('h2. ' + shortTag);
        print('');
        print('|_.Key| _.Value|_.Comment|_.Source|');

        var extraKeys = Object.keys(tagList[shortTag]);
        extraKeys.sort();

        for (var j = 0, elen = extraKeys.length; j < elen; j++)
        {
            val = extraKeys[j];

            // Just to make things a bit clearer while building the print statement
            desc = tagList[shortTag][val]['desc'];
            source = tagList[shortTag][val]['src'];
            tag = tagList[shortTag][val]['tag'];
            value = tagList[shortTag][val]['value'];

            print('| ' + tag + ' | ' + value + ' | ' + desc + ' | ' + source + ' |');
        }

        print('');
        print('----');
        print('');
    }
} // End dumpRedmineTags


// Dump the tagList out as a Large Ugly JSON Blob.
function dumpJsonTags(tagList)
{
    print('JSON format is not working yet');
    print(tagList.toString());
} // End dumpJsonTags


// Dump the tagList out in CSV format.
function dumpCsvTags(tagList)
{
    var desc = '';
    var shortTag = '';
    var tag = '';
    var val = '';
    var value = '';

    var shortKeys = Object.keys(tagList);
    shortKeys.sort();

    print('"Tag","Value","Tag_Description","Tag_Source"');

    for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
    {
        shortTag = shortKeys[i];

        var extraKeys = Object.keys(tagList[shortTag]);
        extraKeys.sort();

        for (var j = 0, elen = extraKeys.length; j < elen; j++)
        {
            val = extraKeys[j];

            // Just to make things a bit clearer while building the print statement
            tag = tagList[shortTag][val]['tag'];
            value = tagList[shortTag][val]['value'];
            desc = tagList[shortTag][val]['desc'];
            source = tagList[shortTag][val]['src'].replace(/,/g,";");
            
            print('"' + tag + '","' + value + '","' + desc + '","' + source + '"');
        }
    }
} // End dumpCsvTags



function attrLookupTable()
{
    // This is a list of the text descriptions for each of the Attribute values
    return {
'ACC':'Accuracy Category/Horizontal Accuracy Category',
'ACE':'Absolute Circular Error/Absolute Horizontal Accuracy',
'ACE_EVAL':'Absolute Horizontal Accuracy Evaluation Method',
'ACE_EVAL_METHOD_CD':'Absolute Circular Error Evaluation Method',
'ACH':'Anchorage Type',
'ACS':'Accessibility Status',
'ADI':'Administrative Division',
'ADR':'Address',
'ADUR':'Average Water Supply Duration',
'AFA':'Available Small Craft Service/Available Vessel Service',
'AFC':'Agricultural Facility Type',
'AHA':'Absolute Horizontal Accuracy (90%)',
'AHC':'Associated Hydrographic Category',
'ALE':'Absolute Linear Error/Absolute Vertical Accuracy',
'ALE_EVAL':'Absolute Vertical Accuracy Evaluation Method',
'ALE_EVAL_METHOD_CD':'Absolute Linear Error Evaluation Method',
'AMA':'Amusement Attraction Type',
'AMB':'Special Administrative Unit',
'ANFL':'Annual Flow Rate',
'AOO':'Angle of Orientation',
'APT':'Airfield Type / Airfield Use',
'APT2':'Airfield Use [2]',
'APT3':'Airfield Use [3]',
'APU':'Apron Usage',
'APU2':'Apron Usage [2]',
'APU3':'Apron Usage [3]',
'APY':'Apron Type',
'AQF':'Aquaculture Facility Type',
'AQN':'Aquifer Name',
'AQO':'Aquifer Overburden',
'AQO1':'Aquifer Overburden [1]',
'AQO2':'Aquifer Overburden [2]',
'AQO3':'Aquifer Overburden [3]',
'AQP':'Aquifer Composition',
'AQP2':'Aquifer Composition [2]',
'AQP3':'Aquifer Composition [3]',
'AQT':'Aquifer Thickness',
'AQTC':'Aquifer Thickness <interval closure>',
'AQTL':'Aquifer Thickness <lower value>',
'AQTU':'Aquifer Thickness <upper value>',
'ARA':'Area',
'ART':'Aeronautical Route Category',
'ASC':'Man-made',
'ASO':'Aeronautical Service Operational Status',
'ASU':'Aerodrome Movement Area Surface Composition',
'ASU1':'Aerodrome Movement Area Surface Composition [1]',
'ASU2':'Aerodrome Movement Area Surface Composition [2]',
'ASU3':'Aerodrome Movement Area Surface Composition [3]',
'ASY':'Airfield Symbol Type',
'AT005_CAB':'Cable : Cable Type',
'AT005_CAB1':'Cable : Cable Type [1]',
'AT005_CAB2':'Cable : Cable Type [2]',
'AT005_CAB3':'Cable : Cable Type [3]',
'ATB':'Attached Building',
'ATC':'Aqueduct Type',
'ATN':'Aids to Navigation',
'AVA':'Absolute Vertical Accuracy (90%)',
'AWP':'Aeronautical Obstacle Light Present',
'AXS':'Aerodrome Surface Status',
'AYR':'Aquifer Yield Rating',
'AYRC':'Aquifer Yield Rating <interval closure>',
'AYRL':'Aquifer Yield Rating <lower value>',
'AYRU':'Aquifer Yield Rating <upper value>',
'AZC':'Man-made',
'BA000_VDC':'Water Line : Sounding Datum',
'BA000_VDR':'Water Line : Sounding Datum Name',
'BAC':'Built-Up Area Classification/ Built-up Area Density Category',
'BAL':'BGN Administrative Level',
'BAT':'Barrier Top Type',
'BBMCA':'Wtrbdy Bank (1) : Bottom Material Type (first bank)',
'BBMCA2':'Wtrbdy Bank (1) : Bottom Material Type (first bank) [2]',
'BBMCA3':'Wtrbdy Bank (1) : Bottom Material Type (first bank) [3]',
'BBMCB':'Wtrbdy Bank (2) : Bottom Material Type (second bank)',
'BBMCB2':'Wtrbdy Bank (2) : Bottom Material Type (second bank) [2]',
'BBMCB3':'Wtrbdy Bank (2) : Bottom Material Type (second bank) [3]',
'BC010_REF':'Maritime Navigation Beacon : Radar Reflector Present',
'BC010_TZP':'Maritime Navigation Beacon : Topmark Shape',
'BC034_BRF':'Maritime Radiobeacon : Broadcast Frequency',
'BC034_BRF2':'Maritime Radiobeacon : Broadcast Frequency [2]',
'BC034_MRT':'Maritime Radiobeacon : Maritime Radiobeacon Type',
'BC034_MRT2':'Maritime Radiobeacon : Maritime Radiobeacon Type [2]',
'BC034_MRT3':'Maritime Radiobeacon : Maritime Radiobeacon Type [3]',
'BC040_CAA':'Maritime Navigation Light : Controlling Authority',
'BC040_COL':'Maritime Navigation Light : Character of Light',
'BC040_EOL':'Maritime Navigation Light : Light Elevation',
'BC040_LCN':'Maritime Navigation Light : Light Characteristic Number',
'BC040_LVN':'Maritime Navigation Light : Nominal Range',
'BC040_MLR':'Mar Nav Lt : Multiple Light Ranges',
'BC040_PER':'Maritime Navigation Light : Period of Light',
'BC040_ZVH':'Maritime Navigation Light : Highest Elevation',
'BC101_SST':'Fog Signal : Fog Signal Type',
'BCC':'Bypass Condition Category/Bypass Condition',
'BDS':'Bidirectional',
'BE_NUMBER':'Basic Encyclopedia Number',
'BEL':'Base Elevation',
'BEN':'Basic Encyclopedia (BE) Number',
'BER':'Berth Identifier',
'BET':'Maritime Navigation Beacon Type',
'BFC':'Building Function Category',
'BGT':'Basin Gate Type',
'BH010_CAA':'Aqueduct : Controlling Authority',
'BH141_AWBA':'Inland Waterbody Bank : Above Water Bank Slope (first bank)',
'BH141_AWBB':'Inland Waterbody Bank : Above Water Bank Slope (second bank)',
'BH141_IBOA':'Inland Waterbody Bank : Bank Orientation (first bank)',
'BH141_IBOB':'Inland Waterbody Bank : Bank Orientation (second bank)',
'BH141_SHDA':'Inland Waterbody Bank : Shoreline Delineated (first bank)',
'BH141_SHDB':'Inland Waterbody Bank : Shoreline Delineated (second bank)',
'BH141_SLTA':'Inland Waterbody Bank : Shoreline Type (first bank)',
'BH141_SLTB':'Inland Waterbody Bank : Shoreline Type (second bank)',
'BH141_WBHA':'Inland Waterbody Bank : Waterbody Bank Height (first bank)',
'BH141_WBHB':'Inland Waterbody Bank : Waterbody Bank Height (second bank)',
'BMC':'Bottom Material Type',
'BMC1':'Bottom Material Type [1]',
'BMC2':'Bottom Material Type [2]',
'BMC3':'Bottom Material Type [3]',
'BNF':'Floor Count',
'BOC':'Bog Type',
'BOT':'Bridge Opening Type',
'BPWHAC':'Inland Waterbody Bank : Predominant Waterbody Bank Height (first bank) <interval closure>',
'BPWHAL':'Inland Waterbody Bank : Predominant Waterbody Bank Height (first bank) <lower value>',
'BPWHAU':'Inland Waterbody Bank : Predominant Waterbody Bank Height (first bank) <upper value>',
'BPWHBC':'Inland Waterbody Bank : Predominant Waterbody Bank Height (second bank) <interval closure>',
'BPWHBL':'Inland Waterbody Bank : Predominant Waterbody Bank Height (second bank) <lower value>',
'BPWHBU':'Inland Waterbody Bank : Predominant Waterbody Bank Height (second bank) <upper value>',
'BPWSAC':'Inland Waterbody Bank : Predominant Waterbody Bank Slope (first bank) <interval closure>',
'BPWSAL':'Inland Waterbody Bank : Predominant Waterbody Bank Slope (first bank) <lower value>',
'BPWSAU':'Inland Waterbody Bank : Predominant Waterbody Bank Slope (first bank) <upper value>',
'BPWSBC':'Inland Waterbody Bank : Predominant Waterbody Bank Slope (second bank) <interval closure>',
'BPWSBL':'Inland Waterbody Bank : Predominant Waterbody Bank Slope (second bank) <lower value>',
'BPWSBU':'Inland Waterbody Bank : Predominant Waterbody Bank Slope (second bank) <upper value>',
'BR2':'Broadcast Frequency 2',
'BRF':'Broadcast Frequency',
'BRF2':'Broadcast Frequency [2]',
'BRG':'Bearing of Object',
'BRN':'Bridge Reference Number',
'BRR':'Bearing and Reciprocal Category',
'BRS':'Bearing from Seaward',
'BSC':'Bridge/Bridge Superstructure Category',
'BSC1':'Bridge Structure Type [1]',
'BSC2':'Bridge Structure Type [2]',
'BSC3':'Bridge Structure Type [3]',
'BSM':'Bridge Span Mobility Type',
'BSS':'Building Superstructure Type',
'BST':'Boundary Status/Boundary Status Type',
'BSU':'Building Superstructure Type',
'BTC':'Buoy Type Category',
'BUD':'Brush/Undergrowth Density Code',
'BUS':'Buoy Shape',
'BUT':'Buoy Type',
'BWVCAC':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (first bank) <interval closure>',
'BWVCAL':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (first bank) <lower value>',
'BWVCAU':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (first bank) <upper value>',
'BWVCBC':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (second bank) <interval closure>',
'BWVCBL':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (second bank) <lower value>',
'BWVCBU':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (second bank) <upper value>',
'C35':'Tidal Current Speed 5 Hours Before',
'C36':'Tidal Current Speed 4 Hours Before',
'C37':'Tidal Current Speed 3 Hours Before',
'C38':'Tidal Current Speed 2 Hours Before',
'C39':'Tidal Current Speed 1 Hour Before',
'C40':'Tidal Current Speed',
'C41':'Tidal Current Speed 1 Hour After',
'C42':'Tidal Current Speed 2 Hours After',
'C43':'Tidal Current Speed 3 Hours After',
'C44':'Tidal Current Speed 4 Hours After',
'C45':'Tidal Current Speed 5 Hours After',
'C46':'Tidal Current Speed 6 Hours After',
'C80':'Rate of Current',
'C81':'Rate of Current',
'C82':'Rate of Current',
'C83':'Rate of Current',
'C84':'Rate of Current',
'C85':'Rate of Current',
'C86':'Rate of Current',
'C87':'Rate of Current',
'C88':'Rate of Current',
'C89':'Rate of Current',
'C90':'Rate of Current',
'C91':'Rate of Current',
'CAA':'Controlling Authority',
'CAB':'Cable Classification/Cable Type',
'CAB1':'Cable Type [1]',
'CAB2':'Cable Type [2]',
'CAB3':'Cable Type [3]',
'CAM':'Conservation Area Management Category',
'CAT':'Cableway Type',
'CBP':'Containment Berm Present',
'CCC':'Color Code Category',
'CCG':'Military Environmental Hazard Category',
'CCN':'Commercial Copyright Notice',
'CCR':'Colour of Navigation Mark Description',
'CCT':'Cover Closure Type',
'CDA':'Covered Drain',
'CDI':'Casing Diameter',
'CDL':'Covered Drain Length',
'CDM':'Cadastral Source Measurement',
'CDR':'Commercial Distribution Restriction',
'CDY':'Cadastral Source Type',
'CEF':'Cultural Facility Type',
'CET':'Cut/Embankment Type Category/Grading Type',
'CFC':'Corrections Facility Type',
'CFT':'Boundary Determination Method',
'CHA':'Light Characteristic Category / Navigation Light Characteristic',
'CID':'Cell Identifier',
'CIT':'Commercial Facility Type',
'CITY':'City Name',
'CIW':'Closed in Winter',
'CLI':'Isolated Communication Line',
'CMS':'Commissioned Status',
'CNCP':'Construction Completion Date',
'CNF':'Confined',
'CNS':'Controlled',
'CNTRY_FULL':'Country Name',
'COA':'Conspicuous Air Category',
'COC':'Conspicuous Category',
'COD':'Certainty of Delineation/Delineation Known',
'COE':'Certainty of Existence',
'COF':'Complex Outline Function',
'COG':'Conspicuous Ground Category',
'COL':'Character of Light',
'COMM_CPYRT_NOTICE':'Commercial Copyright Notice',
'COMM_LIC_TIER_NOTICE':'Distribution of Commercial Data',
'COMMENTS':'Comments',
'COMPLETENESS_CODE':'Completeness Code',
'CON':'Route Width Constriction Type',
'COS':'Facility Operational Status',
'CPA':'Survey Control Point Type',
'CPS':'Cell Partition Scheme',
'CPT':'Colour Pattern',
'CPYRT_NOTE':'Commercial Copyright Notice',
'CRA':'Crane Type Category/Crane Type',
'CREATION_DATE':' Creation Date',
'CRM':'Crane Mobility Type',
'CRN':'Current Rate Minimum',
'CRS':'Current Rate (Speed)',
'CRV':'Depth Curve or Contour Value',
'CRX':'Current Rate Maximum',
'CSC':'Intersection Control Type',
'CSC1':'Intersection Control Type [1]',
'CSC2':'Intersection Control Type [2]',
'CSC3':'Intersection Control Type [3]',
'CSI':'Cadastral Source Identifier',
'CSM':'Secondary Material Characteristics',
'CSO':'Contaminant Source',
'CSP':'Crop Species',
'CSS':'Cross-sectional Shape',
'CST':'Contained within Service Tunnel',
'CSY':'Cadastral Source Identifier Type',
'CTC':'Culvert Type',
'CTL':'Cumulative Track Length',
'CUD':'Currency Date and Time',
'CUL':'Cultural Context Type',
'CUR':'Current Type Category',
'CUS':'Communication Facility Type',
'CVH':'Depth Curve or Contour Value - High',
'CVL':'Depth Curve or Contour Value - Low',
'CVT':'Conveyor Type',
'CVT2':'Conveyor Type [2]',
'CVT3':'Conveyor Type [3]',
'CWT':'Contained within Tunnel',
'D35':'Tidal Current Direction 5 Hours Before',
'D36':'Tidal Current Direction 4 Hours Before',
'D37':'Tidal Current Direction 3 Hours Before',
'D38':'Tidal Current Direction 2 Hours Before',
'D39':'Tidal Current Direction 1 Hour Before',
'D40':'Tidal Current Direction',
'D41':'Tidal Current Direction 1 Hour After',
'D42':'Tidal Current Direction 2 Hours After',
'D43':'Tidal Current Direction 3 Hours After',
'D44':'Tidal Current Direction 4 Hours After',
'D45':'Tidal Current Direction 5 Hours After',
'D46':'Tidal Current Direction 6 Hours After',
'D80':'Direction of Current',
'D81':'Direction of Current',
'D82':'Direction of Current',
'D83':'Direction of Current',
'D84':'Direction of Current',
'D85':'Direction of Current',
'D86':'Direction of Current',
'D87':'Direction of Current',
'D88':'Direction of Current',
'D89':'Direction of Current',
'D90':'Direction of Current',
'D91':'Direction of Current',
'DAN':'Description of Aids to Navigation',
'DAT':'Date',
'DCAP':'Daily Capacity',
'DDC':'Dwelling Type',
'DEP':'Depth / Depth Below Surface Level',
'DEV':'Deck Level',
'DF1':'Direction of Traffic - 1',
'DF2':'Direction of Traffic - 2',
'DF3':'Direction of Traffic - 3',
'DF4':'Direction of Traffic - 4',
'DFT':'Dam Face Type',
'DFU':'Dam Function',
'DFU2':'Dam Function [2]',
'DFU3':'Dam Function [3]',
'DGC':'Transportation Block Type',
'DIA':'DIAM Functional Classification',
'DIM':'Diameter',
'DINF':'Water Disinfection Method',
'DIR':'Directivity',
'DIT':'Ditch Function',
'DKC':'Bathymetric Measurement Quality Category',
'DKC2':'Bathymetric Measurement Quality Category [2]',
'DKC3':'Bathymetric Measurement Quality Category [3]',
'DKS':'Daymark Shape',
'DMB':'Density Measure (Brush/Undergrowth)/Undergrowth Density',
'DMBC':'Undergrowth Density <interval closure>',
'DMBL':'Undergrowth Density <lower value>',
'DMBU':'Undergrowth Density <upper value>',
'DMC':'Boundary Demarcated',
'DMD':'Dammed',
'DMF':'Feature Count',
'DMT':'Canopy Cover/Density Measure (% of Tree/Canopy Cover)',
'DNE':'Human Development Index',
'DOF':'Direction of Flow',
'DOXY':'Dissolved Oxygen',
'DPA':'Aquifer Depth',
'DPAC':'Aquifer Depth <interval closure>',
'DPAL':'Aquifer Depth <lower value>',
'DPAU':'Aquifer Depth <upper value>',
'DPG':'Dumping Ground Type',
'DPG2':'Dumping Ground Type [2]',
'DPG3':'Dumping Ground Type [3]',
'DQS':'Data Quality Statement',
'DRP':'Description of Reference Point',
'DSD':'Dam Structural Design Type',
'DSP':'Boundary Dispute Type',
'DTN':'Dangerous to Navigation',
'DWL':'Dynamic Water Level',
'DWT':'Dam Type',
'DZC':'Deck Count',
'DZP':'Deepest Depth Below Surface Level',
'EBT':'Educational Building Type/Educational Facility Type',
'EET':'Engineered Earthwork Type',
'ELA':'Elevation Accuracy / Elevation Accuracy Category',
'ELE':'Elevation',
'EOL':'Elevation of Light',
'EPW':'Electrical Power Generation Capacity',
'EQC':'Equivalent Scale Category',
'EQS':'Density Equivalent Scale',
'ESC':'Elevation Surface Category',
'ETS':'Extraction Specification',
'ETY':'Engine Test Cell Type',
'ETZ':'Extraction Specification Version',
'EVA':'Elevation Vertical Accuracy (90%)',
'EXISTENCE':'Existence Category',
'EXP':'Route Width Expansion Type',
'EXS':'Existence Category',
'F_CODE':'Feature Code',
'FCODE':'Feature Code',
'FA003_BAL':'Administrative Subdivision : BGN Administrative Level',
'FAC':'Solid Maritime Construction',
'FCL':'Ferry Crossing Distance/Ferry Crossing Length',
'FCM':'Feature Construction Material',
'FCO':'Feature Configuration',
'FCS':'Flood Control Structure Type',
'FDI':'Geologic Dip',
'FEF':'Fence Function',
'FEO':'Feature Element Orientation',
'FER':'Ferry Crossing Type/Ferry Type',
'FFN':'Feature Function',
'FFN1':'Feature Function [1]',
'FFN2':'Feature Function [2]',
'FFN3':'Feature Function [3]',
'FFP':'Farming Pattern',
'FHC':'Harbour Facility Function',
'FHC1':'Harbour Facility Function [1]',
'FHC2':'Harbour Facility Function [2]',
'FHC3':'Harbour Facility Function [3]',
'FIC':'Embankment Type',
'FLO':'Floating',
'FLT':'Floodlit',
'FMM':'Farming Method',
'FPT':'Airfield Type/Facility Priority Type',
'FRT':'Firing Range Type/Weapons Range Type',
'FRT1':'Weapons Range Type [1]',
'FRT2':'Weapons Range Type [2]',
'FRT3':'Weapons Range Type [3]',
'FRZ':'Seasonally Frozen',
'FSC':'Flight Strip Capable',
'FTC':'Farming Type Category',
'FTI':'Fence Type Indicator/Fence Type',
'FTP':'Fabrication Type',
'FUC':'Functional Use',
'FUFI':'Feature Unique Identifier',
'FUN':'Condition of Facility',
'FUNCTION':'Feature Function',
'FURL':'External References',
'FZR':'Fortified Building Type',
'GA032_NSX':'Aero Rad Nav Inst : Aeronautical Radio Navigation Service Name',
'GAP_WIDTH':'Terrain Gap Width',
'GAW':'Railway Gauge',
'GB001_AID':'Aerodrome : Aerodrome Identifier',
'GB052_RIDH':'Runway Direction : Runway Designator (high end)',
'GB052_RIDL':'Runway Direction : Runway Designator (low end)',
'GB052_TRHH':'Runway Direction (high end) : Runway True Heading (high)',
'GB052_TRHL':'Runway Direction (low end) : Runway True Heading (low)',
'GDBV':'Geodatabase Schema Version',
'GEC':'Geopolitical Entity Type',
'GFC':'Government Facility Type',
'GFID':'Global Feature ID',
'GFT':'Geologic Fault Trace Visible',
'GFY':'Geologic Fault Type',
'GNC':'Water Gate Type',
'GOT':'Geothermal Outlet Type',
'GRD':'Guarded',
'GSGCHC':'Runway Direction : Surface Slope (high end) <interval closure>',
'GSGCHL':'Runway Direction : Surface Slope (high end) <lower value>',
'GSGCHU':'Runway Direction : Surface Slope (high end) <upper value>',
'GSGCLC':'Runway Direction : Surface Slope (low end) <interval closure>',
'GSGCLL':'Runway Direction : Surface Slope (low end) <lower value>',
'GSGCLU':'Runway Direction : Surface Slope (low end) <upper value>',
'GTC':'Gate Type Category/Gate Use',
'GUG':'Guyed or Unguyed Category',
'GWS':'Groundwater Potential Score',
'GWTR':'Groundwater Transmissivity',
'HAF':'Helipad Associated Facility',
'HBH':'Hydrographic Base Height',
'HCA':'Horizontal Clearance',
'HCT':'Thoroughfare Class',
'HDH':'Hydrographic Drying Height',
'HDI':'Hydrographic Depth/Height Information ',
'HDP':'Hydrographic Depth',
'HEI':'Height of Object',
'HFC':'Hydrological Form Category',
'HGS':'Spillway Height',
'HGT':'Height Above Surface Level',
'HHD':'Homogenous Habitation Distribution',
'HLT':'Hydrographic Light Type',
'HQC':'Hypsography Portrayal Type',
'HS1':'Current Information (1)',
'HS2':'Current Information (2)',
'HSB':'Height above sea bottom',
'HSC':'Hospital Number of Beds',
'HSCD':'Hose Connection Diameter',
'HSE':'Periodic Marine Current Month Interval',
'HSS':'Historic Significance',
'HST':'Hazard Shelter Intended Use',
'HST2':'Hazard Shelter Intended Use [2]',
'HST3':'Hazard Shelter Intended Use [3]',
'HTP':'Hangar Type Category',
'HVA':'Height Vertical Accuracy (90%)',
'HWT':'House of Worship Type',
'HYC':'Hydrologic Category',
'HYCD':'Hydraulic Conductivity',
'HYP':'Hydrologic Persistence',
'HZD':'Geodetic Datum',
'IAA':'IALA A or B Adopted',
'IAC':'IALA Aid Category',
'IAS':'IMO Adopted / IMO Approval Status',
'IC2':'Geopolitical Entity Designation : ISO 3166-1 Alpha-3 Code',
'ICF':'Manufacturing Facility Type',
'IKO':'ICAO Designator/ICAO Identifier/ICAO Location Indicator',
'INU':'Inundation Type',
'IRG':'Irrigation Method',
'ISO_3116_2':'ISO 3166-1 Alpha-2 Country Code',
'ISO_3116_3':'ISO 3166-1 Alpha-3 Country Code',
'ISS':'Isolated',
'IWO':'Inland Water Obstruction',
'IWT':'Inland Water Type',
'KVA':'Kilovolt Capacity Attribute/Power Line Maximum Voltage',
'LAF':'Line Associated Features / Maritime Navigation Line Features',
'LASZ':'Largest Aircraft',
'LBV':'Left Bank Vegetation Cover',
'LC1':'Load Class Type 1',
'LC2':'Load Class Type 2',
'LC3':'Load Class Type 3',
'LC4':'Load Class Type 4',
'LCA':'Lifting Capacity',
'LCN':'Light Characteristic Number',
'LDC':'Dam Crest Length',
'LEA':'Least Depth Below Surface Level',
'LEG':'Length/Diameter in Meters With Greater Precision',
'LEN':'Length/Diameter',
'LFA':'Aeronautical Light Function',
'LFC':'Maritime Light Type',
'LFC2':'Maritime Light Type [2]',
'LFC3':'Maritime Light Type [3]',
'LINK_ID':'Link Id',
'LMC':'Landmark Category/Navigation Landmark',
'LND':'Land Morphology',
'LNU':'Usable Length',
'LOC':'Location Category/Vertical Relative Location',
'LOCATION':'Location Category/Vertical Relative Location',
'LOG':'Gradient Length',
'LOR':'Length of Range',
'LSA':'Light Sector Angle',
'LSI':'Light Sector Initial Limit',
'LSP':'Geopolitical Line Type',
'LST':'Light Sector Terminal Limit',
'LTN':'Track or Lane Count',
'LUN':'Located Underground',
'LVN':'Light Range Nominal / Maritime Navigation Nominal Range',
'LZN':'Length',
'MAC':'Maritime Area Category',
'MAG':'Magnetic Variation',
'MAN':'Maritime Navigation Marked',
'MAS':'Maintenance Status',
'MBC_MCC1':'Bot Char (First Material Type)',
'MBC_MCC2':'Bot Char (Second Material Type)',
'MBC_PSC1':'Bot Char (first material quality)',
'MBC_PSC2':'Bot Char (second material quality)',
'MBC_SCO1':'Bot Char (First Sediment Colour)',
'MBC_SCO2':'Bot Char (Second Sediment Colour)',
'MCA':'Morse Code Attribute / Characters Emitted',
'MCC':'Material Composition Category/Structural Material Type',
'MCC1':'Structural Material Type [1]',
'MCC2':'Structural Material Type [2]',
'MCC3':'Structural Material Type [3]',
'MCS':'Material Composition Secondary',
'MCU':'Material Composition Underlying',
'MCX':'Motorized Crossing',
'MCY':'Maritime Caution Type',
'MCY2':'Maritime Caution Type [2]',
'MCY3':'Maritime Caution Type [3]',
'MDE':'Maintenance Date and Time',
'MED':'Median Category',
'MEL':'Medical Service',
'MEM':'Memorandum',
'MES':'Median Present',
'MFB':'Manufactured Building',
'MFC':'Military Facility Type',
'MFE':'Marine Farm Enclosure Method',
'MFY':'Medical Facility Type',
'MGL':'Maritime Geopolitical Limit Type',
'MGL1':'Maritime Geopolitical Limit Type [1]',
'MGL2':'Maritime Geopolitical Limit Type [2]',
'MGL3':'Maritime Geopolitical Limit Type [3]',
'MIN':'Extraction Mine Type/Mining Category',
'MINE':'Mining Category',
'MKSZ':'Market Size',
'MLO':'Limit(s) Physically Based',
'MLOP':'Military Operations Type',
'MLR':'Multiple Light Ranges',
'MLUS':'Military Use Type',
'MNR':'Maritime Navigation Restriction',
'MNR2':'Maritime Navigation Restriction [2]',
'MNR3':'Maritime Navigation Restriction [3]',
'MNS':'Man-made Shoreline',
'MOH':'Maximum Obstacle Height',
'MRP':'Maritime Radiobeacon Present',
'MRR':'Maritime Area Restriction',
'MRR2':'Maritime Area Restriction [2]',
'MRR3':'Maritime Area Restriction [3]',
'MRT':'Maritime Radiobeacon Type',
'MST':'Missile Site Type',
'MST2':'Missile Site Type [2]',
'MST3':'Missile Site Type [3]',
'MUB':'Multi-unit Building',
'MVC':'Maximum Vertical Clearance',
'MWD':'Maximum Design Water Depth',
'MWG':'Centerline Spacing',
'MZN':'Extraction Mine Type',
'NA2':'Touristic Name',
'NA3':'Classification Name',
'NA8':'Aerodrome Official Name',
'NAM':'Name Category/Name / Geographic Name Information : Full Name',
'NAO':'Installation Operator',
'NDX':'Neighbourhood-defining Characteristic',
'NDX2':'Neighbourhood-defining Characteristic [2]',
'NDX3':'Neighbourhood-defining Characteristic [3]',
'NFI':'Named Feature Identifier',
'NFN':'Name Identifier',
'NFP':'Naval Firing and/or Practice Type',
'NFP2':'Naval Firing and/or Practice Type [2]',
'NFP3':'Naval Firing and/or Practice Type [3]',
'NLT':'Named Location Type',
'NM3':'Name 3 (name of the political entity on one side of a boundary).',
'NM4':'Name 4 (name of the political entity on the other side of the boundary).',
'NMC':'Navigation Mark Colour',
'NMC2':'Navigation Mark Colour [2]',
'NMC3':'Navigation Mark Colour [3]',
'NOA':'Naval Operations Type',
'NOA2':'Naval Operations Type [2]',
'NOA3':'Naval Operations Type [3]',
'NOR':'Number of Rooms',
'NOS':'Span Count',
'NPL':'Parallel Line Count',
'NS2':'Navigation System Types (2)',
'NSC':'Hydrographic NAVAID System',
'NST':'Navigation System Type',
'NVS':'Navigability Information',
'OCC':'Overhead Clearance Category Code',
'OCS':'Offshore Construction Primary Structure',
'OHB':'Overall Bridge Height/Overhead Height of Bridge',
'OHC':'Overhead Clearance Category/Overhead Clearance',
'OHD':'Derived Obstacle Height/Depth Category',
'OLQ':'Obstruction Light Quality',
'ONE':'One-way',
'OOC':'Overhead Obstruction Type',
'OPC':'Offshore Platform Type',
'OPERATOR':'Operator ID',
'OPS':'Operational Status',
'OPT':'Operating Cycle',
'OR2':'Operating Range Category (2)',
'ORC':'Operating Range Category',
'ORD':'Ordinal Category/Relative Importance',
'ORIG':'Feature Metadata : Process Step Information : Resource Content Originator',
'ORIG_SOURCE_DATE':'Originating Source Date',
'ORIG_SOURCE_INFO':'Originating Source Information',
'ORIGINATING_SOURCE':'Originating Source Name',
'ORIGINATOR':'Feature Metadata : Process Step Information : Resource Content Originator',
'ORS':'Operating Restriction',
'ORS2':'Operating Restriction [2]',
'ORS3':'Operating Restriction [3]',
'OTH':'Specified Domain Value(s)/Specified Enumerant(s)',
'OWO':'Over Water Obstruction / Waterbody Overhead Obstruction',
'PAF':'Public Accommodation Facility',
'PBV':'Pilot Boarding Vehicle',
'PBY':'By-product',
'PBY1':'By-product [1]',
'PBY2':'By-product [2]',
'PBY3':'By-product [3]',
'PCF':'Physical Condition',
'PDA':'Property DFDD-compatible Accessor',
'PEC':'Port of Entry',
'PER':'Period of Light',
'PFD':'Predominant Feature Depth',
'PFG':'Predominant Feature Height',
'PFH':'Predominant Feature Height',
'PFHC':'Predominant Feature Height <interval closure>',
'PFHL':'Predominant Feature Height <lower value>',
'PFHU':'Predominant Feature Height <upper value>',
'PHT':'Predominant Height',
'PLC':'Pile Type',
'PLT':'Pipeline Type',
'PLT2':'Pipeline Type [2]',
'PLT3':'Pipeline Type [3]',
'PMPR':'Pump Present',
'POS':'Power Source',
'POS2':'Power Source [2]',
'POS3':'Power Source [3]',
'PPC':'Power Plant Category/Power Plant Type',
'PPL':'Population Count',
'PPO':'Product',
'PPO1':'Product [1]',
'PPO2':'Product [2]',
'PPO3':'Product [3]',
'PPT':'Populated Place Type',
'PRM':'Permanency/Permanent',
'PRO':'Product Category',
'PRO':'Product',
'PRODUCT':'Product Category',
'PRVR':'Primary River',
'PRW':'Raw Material',
'PRW1':'Raw Material [1]',
'PRW2':'Raw Material [2]',
'PRW3':'Raw Material [3]',
'PSC':'Physical Surface Characteristics',
'PSE':'Periodic Restriction Month Interval',
'PSF':'Public Service Facility Type',
'PSN':'Path Sinuosity',
'PTG':'Portage Use',
'PUD':'Pump Depth',
'PVH':'Predominant Vegetation Height',
'PWA':'Predominant Water Depth',
'PWAC':'Predominant Water Depth <interval closure>',
'PWAL':'Predominant Water Depth <lower value>',
'PWAU':'Predominant Water Depth <upper value>',
'PWC':'Shoreline Construction Type',
'PWR':'NAVAID Power',
'PYC':'Pylon Configuration',
'PYM':'Pylon Material',
'QLE':'Restrictions on Releasibility',
'RAC':'Radar Antenna Configuration',
'RAD':'Curve Radius',
'RAS':'Radar Station Function',
'RAY':'Racing Type',
'RBV':'Right Bank Vegetation Cover',
'RCG':'Resource Content Originator',
'REF':'Radar Reflector Present / Radar Reflector Attribute',
'REL':'Religious Designation',
'RES':'Research Facility Type',
'REW':'Reference Water Level',
'RFC':'Religious Facility Type',
'RFD':'Roofed',
'RGC':'Railroad Gauge Category/Railway Gauge Classification',
'RIN_ROI':'Route Identification <route designation type>',
'RIN_ROI2':'Route Identification [2] <route designation type>',
'RIN_ROI3':'Route Identification [3] <route designation type>',
'RIN_RTN':'Route Identification <route designation>',
'RIN_RTN2':'Route Identification [2] <route designation>',
'RIN_RTN3':'Route Identification [3] <route designation>',
'RIP':'Rig Present',
'RIR':'Railway in Road',
'RIT':'Road Interchange Type',
'RKF':'Rock Formation Structure',
'RLE':'Relative Level',
'RMWC':'Route Median Width <interval closure>',
'RMWL':'Route Median Width <lower value>',
'RMWU':'Route Median Width <upper value>',
'ROOF_SHAPE':'Roof Shape/Structure Shape of Roof',
'ROR':'Road Interchange Ramp',
'RPA':'Required Port Access',
'RPC':'Boundary Representation Policy',
'RRA':'Railroad Power Source',
'RRC':'Railroad Categories/Railway Use',
'RRC1':'Railway Use [1]',
'RRC2':'Railway Use [2]',
'RRC3':'Railway Use [3]',
'RSA':'Branch Railway Type/Railroad Siding Attribute',
'RSI':'Radar Significance',
'RST':'Road/Runway Surface Type / Load-bearing Surface Type',
'RSTRN_DCLS_XMPT_CD':'Restriction Declassification Exemptions',
'RSTRN_DECLASS':'Restriction Declassification',
'RSTRN_DSEM_CTRL_IC':'Restrictions on Dissemination Control IC',
'RSTRN_DSEM_CTRL_NIC':'Restrictions on Dissemination Control Non-IC',
'RSTRN_FOREIGN_GOV':'Restrctions on Foreign Government Access',
'RSTRN_RELEASIBILITY':'Restrictions on Releasibility',
'RTA':'Linear Feature Arrangement',
'RTL':'Resource Title',
'RTN':'Route Designation/Route Number',
'RTN2':'Route Designation [2]',
'RTN3':'Route Designation [3]',
'RTN_ROI':'Route Designation (route designation type)',
'RTN_ROI2':'Route Designation [2] (route designation type)',
'RTN_ROI3':'Route Designation [3] (route designation type)',
'RTP':'Reservoir Type',
'RTT':'Route Intended Use',
'RTY':'Roadway Type',
'RWC':'Railway Class',
'SAX_RS6':'Security Attributes Group <resource declassification exception>',
'SAX_RX8':'Security Attributes Group <resource type of exempted source>',
'SAD':'Sand Dune Stabilized',
'SBB':'Supported by Bridge Span',
'SBC':'Shelter Belt',
'SBT':'Substation Type',
'SBT1':'Substation Type [1]',
'SBT2':'Substation Type [2]',
'SBT3':'Substation Type [3]',
'SCALE':'Feature Scale',
'SCAMAX':'Feature Metadata : Cartographic Usability Range <upper value>',
'SCAMIN':'Feature Metadata : Cartographic Usability Range <lower value>',
'SCB':'Pavement Condition/Surface Condition Attribute',
'SCC':'Spring/Well Characteristic Category/Water Type',
'SCOG':'Sample Collector Organization',
'SCVN':'Schema Version',
'SDCC':'Soil Depth <interval closure>',
'SDCL':'Soil Depth <lower value>',
'SDCU':'Soil Depth <upper value>',
'SDI':'Geologic Strike Direction',
'SDO':'Sand Dune Orientation',
'SDP':'Source Description',
'SDQ':'Structurally-dissected',
'SDSC':'Stem Diameter <interval closure>',
'SDSL':'Stem Diameter <lower value>',
'SDSU':'Stem Diameter <upper value>',
'SDT':'Sand Dune Type',
'SDV':'Source Date and Time',
'SEC_CD_CNTRL':'Security Codeword Control',
'SEC_CLASS':'Security Classification',
'SEC_CLASS_SYS_ID':'Security Classification System Identifier',
'SEC_CLS_AUTH_DESC':'Non-standard Classification Authority',
'SEC_CLS_AUTH_MULT':'Multiple Classification Authority Sources',
'SEC_CLS_AUTHORITY':'Classification Authority',
'SEP':'Divided',
'SERP':'Surface Permanence',
'SFC':'Sea Floor Category',
'SFS':'Aerodrome Pavement Functional',
'SFY':'Security Facility Type',
'SGC':'Gradient/Slope',
'SGCC':'Surface Slope <interval closure>',
'SGCL':'Surface Slope <lower value>',
'SGCU':'Surface Slope <upper value>',
'SHC':'Safe Horizontal Clearance',
'SHD':'Shoreline Delineated',
'SHL':'Left Bank Shoreline Type',
'SHO':'Coast Type',
'SHP':'Shape of Beacon',
'SHR':'Right Bank Shoreline Type',
'SHT':'Shipping Container Type',
'SIC':'Frozen Cover Type / Snow / Ice Category',
'SLR':'Shoreline Ramp Type',
'SLT':'Shoreline Type Category/Shoreline Type',
'SMC':'Surface Material Category/Surface Material Type',
'SND':'Sounding Category',
'SNDC':'Snow or Ice Depth <interval closure>',
'SNDL':'Snow or Ice Depth <lower value>',
'SNDU':'Snow or Ice Depth <upper value>',
'SOH':'Severity of Hazard',
'SOURCE':'Originating Source Information',
'SPAN_TYPE':'Bridge Opening Type',
'SPD':'Speed Limit (MPH)',
'SPEC_ID':'Extraction Specification ID',
'SPM':'Speed Limit (KPH)',
'SPT':'Supported',
'SRC_DATE':'Source Date and Time',
'SRC_INFO':'Source Description',
'SRC_NAME':'Source Type',
'SRC_RELSE':'Restrictions on Releasibility of the source data',
'SRD':'Terrain Morphology',
'SRE':'Traffic Restriction Type',
'SRL':'Location Referenced to Shoreline',
'SRL1':'Location Referenced to Shoreline [1]',
'SRL2':'Location Referenced to Shoreline [2]',
'SRL3':'Location Referenced to Shoreline [3]',
'SRT':'Source Type',
'SSC':'Structure Shape Category/Structure Shape',
'SSE':'Seasonal Ice Limit',
'SSG':'Street Sign Type',
'SSO':'Shaft Slope Orientation',
'SSR':'Roof Shape/Structure Shape of Roof',
'SSR1':'Roof Shape [1]',
'SSR2':'Roof Shape [2]',
'SSR3':'Roof Shape [3]',
'SST':'Sound Signal Type',
'STA':'Maritime Station Type / Station Type Category (Maritime)',
'STA2':'Maritime Station Type [2]',
'STA3':'Maritime Station Type [3]',
'STAF':'Total Staff',
'STL':'Nomadic Seasonal Location',
'STL1':'Nomadic Seasonal Location [1]',
'STL2':'Nomadic Seasonal Location [2]',
'STL3':'Nomadic Seasonal Location [3]',
'STNB':'Strahler Number',
'STP':'Soil Type',
'STR':'Summer Canopy Cover',
'SUC':'Shed Type',
'SUY':'Survey Point Type',
'SVC':'Sounding Velocity',
'SWC':'Soil Wetness Condition',
'SWCP':'Specific Water Capacity',
'SWT':'Natural Pool Type/Well or Spring Feature Type',
'SWW':'Static Water Level',
'TAN':'TSS-associated Aids to Navigation',
'TCS':'Cross-sectional Profile',
'TEL':'Telescope Type',
'TFC':'Transportation Facility Type',
'THI':'Thickness',
'THR':'Through Route',
'TID':'Tidal/Non-Tidal Category/Tide Influenced',
'TIER_NOTE':'Commercial Distribution Restriction',
'TIM':'Time Attribute',
'TMC':'Top Mark Characteristic',
'TOS':'Tower Shape',
'TOWER_TYPE':'Tower Type Category/Tower Type',
'TRA':'Pedestrian Traversable/Traversability',
'TRE':'Foliage Type/Tree Type Category',
'TRE':'Tree Category',
'TRF':'Traffic Flow',
'TRP':'Transportation Route Protection Structure Type',
'TRS':'Transportation System Type',
'TRS1':'Transportation System Type [1]',
'TRS2':'Transportation System Type [2]',
'TRS3':'Transportation System Type [3]',
'TSCC':'Tree Spacing <interval closure>',
'TSCL':'Tree Spacing <lower value>',
'TSCU':'Tree Spacing <upper value>',
'TSM':'Terrain Surface Material',
'TSM1':'Terrain Surface Material [1]',
'TSM2':'Terrain Surface Material [2]',
'TSM3':'Terrain Surface Material [3]',
'TSP':'Traffic Scheme Part / Maritime Traffic Separation Scheme Component',
'TSSM':'Terrain Subsurface Material',
'TSSM2':'Terrain Subsurface Material [2]',
'TSSM3':'Terrain Subsurface Material [3]',
'TST':'Cable Suspended Shape/Power Line Shape/Transmission Suspension Type',
'TSTE':'Water Taste',
'TTC':'Tower Type Category/Tower Type',
'TTC1':'Tower Type [1]',
'TTC2':'Tower Type [2]',
'TTC3':'Tower Type [3]',
'TTY':'Tomb Type',
'TUC':'Transportation Use Category',
'TWS':'Trees Widely Scattered',
'TXP':'Taxiway Type',
'TXT':'Associated Text',
'TYP':'Thoroughfare Type',
'TZP':'Topmark Shape',
'UBC':'Underbridge Clearance',
'UDQ':'Underwater Delineation Quality',
'UFI':'Unique Entity Identifier',
'UHS':'Uncovering Height Known',
'UID':'MGCP Feature universally unique identifier',
'UMA':'Underground Mine Access',
'UMC':'Underlying Material Characteristics',
'UPD_DATE':'Review Source Date and Time',
'UPD_INFO':'Review Source Description',
'UPD_NAME':'Review Source Type',
'UPDATE_SOURCE':'Update/Review Source Name',
'UPDATE_SOURCE_DATE':'Update/Review Source Date',
'UPDATE_SOURCE_INFO':'Update/Review Source Information',
'URI':'Unique Resource Identifier',
'USAGE':'Usage Category',
'USE':'Usage',
'USE_':'Usage2',
'USG':'Usage Category',
'UTY':'Accessible Utility Type',
'UTY1':'Accessible Utility Type [1]',
'UTY2':'Accessible Utility Type [2]',
'UTY3':'Accessible Utility Type [3]',
'UUC':'Utility Facility Type',
'VAL':'Value - A Generic Value',
'VAV':'Variation Anomaly Value',
'VAVC':'Magnetic Variation Anomaly <interval closure>',
'VAVL':'Magnetic Variation Anomaly <lower value>',
'VAVU':'Magnetic Variation Anomaly <upper value>',
'VCA':'Void Collection Attribute/Void Collection Reason',
'VCA1':'Void Collection Reason [1]',
'VCA2':'Void Collection Reason [2]',
'VCA3':'Void Collection Reason [3]',
'VCM':'Vertical Construction Material',
'VCM1':'Vertical Construction Material [1]',
'VCM2':'Vertical Construction Material [2]',
'VCM3':'Vertical Construction Material [3]',
'VCO':'Vertical Clearance, Open',
'VCS':'Vertical Clearance, Safe',
'VCS_VCD':'Vertical Clearance, Safe <reference level>',
'VCS_VCR':'Vertical Clearance, Safe <reference level remark>',
'VCT':'Void Collection Type',
'VCT1':'Void Collection Type [1]',
'VCT2':'Void Collection Type [2]',
'VCT3':'Void Collection Type [3]',
'VDC':'Sounding Datum/Vertical Datum Category',
'VDR':'Vertical Datum Record',
'VDT':'Vertical Datum',
'VEC':'Vehicle Capacity',
'VEG':'Vegetation Characteristic',
'VET':'Vehicle Type',
'VGT':'Volcanic Geologic Type/Volcano Shape',
'VH3':'Predominant Vegetation Height',
'VIS':'Light Visibility',
'VIS2':'Light Visibility [2]',
'VIS3':'Light Visibility [3]',
'VLM':'Volume',
'VOA':'Volcanic Activity',
'VOI':'Vertical Obstruction Identifier',
'VOL':'Volume',
'VRR':'Vertical Reference Category',
'VSP':'Vegetation Species',
'VSP1':'Vegetation Species [1]',
'VSP2':'Vegetation Species [2]',
'VSP3':'Vegetation Species [3]',
'VST':'Vehicle Scale Count',
'VTI':'Vegetation Trafficability Impact',
'WAD':'Water Well Construction Description',
'WBD':'Waterbody Depth',
'WCAC':'Alkalinity',
'WCC':'Watercourse Channel Type',
'WCL':'Winter Canopy Cover',
'WD1':'Route Minimum Travelled Way Width',
'WD2':'Route Total Usable Width',
'WD3':'Terrain Gap Width',
'WD5':'Width at Top',
'WDAC':'Average Water Depth <interval closure>',
'WDAL':'Average Water Depth <lower value>',
'WDAU':'Average Water Depth <upper value>',
'WDH':'Predominant Maximum Water Depth',
'WDL':'Predominant Minimum Water Depth',
'WDST':'Water Distribution System Type',
'WDU':'Usable Width',
'WEATHER':'Road Weather Restriction/Weather Type Category',
'WEQ':'Well Equipment',
'WEQ1':'Well Equipment [1]',
'WEQ2':'Well Equipment [2]',
'WEQ3':'Well Equipment [3]',
'WFT':'Well Feature Type/Well Type',
'WGP':'Width with greater than 1 meter resolution',
'WHCO':'Bicarbonate',
'WID':'Width',
'WLE':'Water Level Effect',
'WLO':'Wreck or Hulk Exposure',
'WLSS':'Water Loss',
'WMT':'Watercourse Morphology',
'WOC':'Dam Crest Width / Width of Crest',
'WODR':'Water Odour',
'WORSHIP':'House of Worship Type',
'WPC':'Coastal Work Type / Work in Progress Category',
'WPC2':'Coastal Work Type [2]',
'WPC3':'Coastal Work Type [3]',
'WPI':'World Port Index Identifier',
'WPST':'Windows Per Story',
'WRCC':'Water Recharge Rate <interval closure>',
'WRCL':'Water Recharge Rate <lower value>',
'WRCU':'Water Recharge Rate <upper value>',
'WRT':'Water Race Type',
'WSRC':'Source of Water',
'WST':'Watercourse Sink Type',
'WT2':'Width of Second Travelled Way',
'WTBB':'Boron',
'WTC':'Road Weather Restriction/Weather Type Category',
'WTCA':'Calcium',
'WTCL':'Water Colour',
'WTFE':'Total Iron',
'WTI':'Wall Type',
'WTKK':'Potassium',
'WTMN':'Manganese',
'WTNA':'Sodium',
'WTNO':'Nitrate',
'WTPO':'Phosphate',
'WTSI':'Silicon',
'WTST':'Water Test Type',
'WTT':'Water Turbulence Type',
'WTYP':'Window Type',
'WVA':'Predominant Average Water Velocity',
'WVH':'Predominant Maximum Water Velocity',
'WVL':'Predominant Minimum Water Velocity',
'WWM':'Sub-Surface Stratum Type',
'WWM2':'Sub-Surface Stratum Type [2]',
'WWM3':'Sub-Surface Stratum Type [3]',
'WWRC':'Water Withdrawal Rate <interval closure>',
'WWRL':'Water Withdrawal Rate <lower value>',
'WWRU':'Water Withdrawal Rate <upper value>',
'YSU':'Military Service Branch',
'YWQ':'Water Potability',
'Z_VALUE_TYPE':'Z-Coordinate Type',
'ZHBH_DCR':'Base Height : Sounding Metadata : Vessel Draft Correction',
'ZHBH_DKC':'Base Height : Sounding Metadata : Bathymetric Measurement Quality Category',
'ZHBH_DKC2':'Base Height : Sounding Metadata : Bathymetric Measurement Quality Category [2] ',
'ZHBH_DKC3':'Base Height : Sounding Metadata : Bathymetric Measurement Quality Category [3]',
'ZHBH_SVC':'Base Height : Sounding Metadata : Sounding Velocity Correction Method',
'ZHBH_TEC':'Base Height : Sounding Metadata : Bathymetric Measurement Technique',
'ZHBH_TEC2':'Base Height : Sounding Metadata : Bathymetric Measurement Technique [2]',
'ZHBH_TEC3':'Base Height : Sounding Metadata : Bathymetric Measurement Technique [3]',
'ZHBH_VDC':'Base Height : Sounding Metadata : Sounding Datum',
'ZHBH_VDR':'Base Height : Sounding Metadata : Sounding Datum Name',
'ZHDP_DCR':'Hydrographic Depth : Sounding Metadata : Vessel Draft Correction',
'ZHDP_DKC':'Hydrographic Depth : Sounding Metadata : Bathymetric Measurement Quality Category',
'ZHDP_DKC2':'Hydrographic Depth : Sounding Metadata : Bathymetric Measurement Quality Category [2]',
'ZHDP_DKC3':'Hydrographic Depth : Sounding Metadata : Bathymetric Measurement Quality Category [3]',
'ZHDP_SVC':'Hydrographic Depth : Sounding Metadata : Sounding Velocity Correction Method',
'ZHDP_TEC':'Hydrographic Depth : Sounding Metadata : Bathymetric Measurement Technique',
'ZHDP_TEC2':'Hydrographic Depth : Sounding Metadata : Bathymetric Measurement Technique [2]',
'ZHDP_TEC3':'Hydrographic Depth : Sounding Metadata : Bathymetric Measurement Technique [3]',
'ZHDP_VDC':'Hydrographic Depth : Sounding Metadata : Sounding Datum',
'ZHDP_VDR':'Hydrographic Depth : Sounding Metadata : Sounding Datum Name',
'ZI001_NSD':'Source Information : Non-spatial Source Date and Time',
'ZI001_NSN':'Source Information : Non-spatial Source Description',
'ZI001_NSP':'Source Information : Non-spatial Source Type',
'ZI001_SDP':'Source Information : Source Description',
'ZI001_SDV':'Source Information : Source Date and Time',
'ZI001_SPS':'Source Information : Spatial Source',
'ZI001_SRT':'Source Information : Source Type',
'ZI001_SSD':'Source Information : Spatial Source Date and Time',
'ZI001_SSN':'Source Information : Spatial Source Description',
'ZI001_SSY':'Source Information : Spatial Source Type',
'ZI001_VSC':'Source Information : Vertical Source Category',
'ZI001_VSD':'Source Information : Vertical Source Date and Time',
'ZI001_VSN':'Source Information : Vertical Source Description',
'ZI002_CCN':'Commercial Copyright Notice',
'ZI002_UFI':'Restriction Information : Unique Entity Identifier',
'ZI004_PRE':'Process Step Information : Process Step Description',
'ZI004_RCG':'Feature Metadata : Process Step Information : Resource Content Originator',
'ZI005_FNA':'Geographic Name Information : Full Name',
'ZI005_FNA1':'Geographic Name Information : (first) Full Name',
'ZI005_FNA2':'Geographic Name Information : Full Name (second)',
'ZI005_FNA3':'Geographic Name Information : Full Name (third)',
'ZI005_FNAA':'Geopolitical Entity : Geographic Name Information (1) : Full Name (first side)',
'ZI005_FNAB':'Geopolitical Entity : Geographic Name Information (2) : Full Name (second side)',
'ZI005_GNR':'Geographic Name Information : Geographic Name Rank',
'ZI005_GNR2':'Geographic Name Information : Geographic Name Rank [2]',
'ZI005_GNR3':'Geographic Name Information : Geographic Name Rank [3]',
'ZI005_GNT':'Geographic Name Information : Geographic Name Type',
'ZI005_GNT2':'Geographic Name Information : Geographic Name Type [2]',
'ZI005_GNT3':'Geographic Name Information : Geographic Name Type [3]',
'ZI005_GNTA':'Geographic Name Information (1) : Geographic Name Type (first side)',
'ZI005_GNTB':'Geographic Name Information (2) : Geographic Name Type (second side)',
'ZI005_NFN':'Geographic Name Information : Name Identifier',
'ZI005_NFN1':'Geographic Name Information : (first) Name Identifier',
'ZI005_NFN2':'Geographic Name Information : Name Identifier (second)',
'ZI005_NFN3':'Geographic Name Information : Name Identifier (third)',
'ZI005_NFNA':'Geopolitical Entity : Geographic Name Information (1) : Name Identifier (first side)',
'ZI005_NFNB':'Geopolitical Entity : Geographic Name Information (2) : Name Identifier (second side)',
'ZI006_MEM':'Note : Memorandum',
'ZI012_DCR':'Depth Curve : Sounding Metadata : Vessel Draft Correction',
'ZI012_DKC':'Depth Curve : Sounding Metadata : Bathymetric Measurement Quality Category',
'ZI012_DKC2':'Depth Curve : Sounding Metadata : Bathymetric Measurement Quality Category [2]',
'ZI012_DKC3':'Depth Curve : Sounding Metadata : Bathymetric Measurement Quality Category [3]',
'ZI012_SVC':'Depth Curve : Sounding Metadata : Sounding Velocity Correction Method',
'ZI012_TEC':'Depth Curve : Sounding Metadata : Bathymetric Measurement Technique',
'ZI012_TEC2':'Depth Curve : Sounding Metadata : Bathymetric Measurement Technique [2]',
'ZI012_TEC3':'Depth Curve : Sounding Metadata : Bathymetric Measurement Technique [3]',
'ZI012_VDC':'Depth Curve : Sounding Metadata : Sounding Datum',
'ZI012_VDR':'Depth Curve : Sounding Metadata : Sounding Datum Name',
'ZI013_CSP':'Crop Information : Crop Species',
'ZI013_CSP1':'Crop Information : Crop Species [1]',
'ZI013_CSP2':'Crop Information : Crop Species [2]',
'ZI013_CSP3':'Crop Information : Crop Species [3]',
'ZI013_FFP':'Crop Information : Farming Pattern',
'ZI013_FFP1':'Crop Information : Farming Pattern [1]',
'ZI013_FFP2':'Crop Information : Farming Pattern [2]',
'ZI013_FFP3':'Crop Information : Farming Pattern [3]',
'ZI013_FMM':'Crop Information : Farming Method',
'ZI013_FMM1':'Crop Information : Farming Method [1]',
'ZI013_FMM2':'Crop Information : Farming Method [2]',
'ZI013_FMM3':'Crop Information : Farming Method [3]',
'ZI013_IRG':'Crop Information : Irrigation Method',
'ZI013_PIG':'Crop Information : Permanent Irrigation',
'ZI014_PBY':'Manufacturing Information : By-product',
'ZI014_PBY1':'Manufacturing Information : By-product [1]',
'ZI014_PBY2':'Manufacturing Information : By-product [2]',
'ZI014_PBY3':'Manufacturing Information : By-product [3]',
'ZI014_PPO':'Manufacturing Information : Physical Product',
'ZI014_PPO':'Manufacturing Information : Product',
'ZI014_PPO1':'Manufacturing Information : Product [1]',
'ZI014_PPO2':'Manufacturing Information : Physical Product [2]',
'ZI014_PPO2':'Manufacturing Information : Product [2]',
'ZI014_PPO3':'Manufacturing Information : Physical Product [3]',
'ZI014_PPO3':'Manufacturing Information : Product [3]',
'ZI014_PRW':'Manufacturing Information : Raw Material',
'ZI014_PRW1':'Manufacturing Information : Raw Material [1]',
'ZI014_PRW2':'Manufacturing Information : Raw Material [2]',
'ZI014_PRW3':'Manufacturing Information : Raw Material [3]',
'ZI015_GCUC':'Geo Name Collection : Geographic Name Cartographic Usability Range <interval closure>',
'ZI015_GCUL':'Geo Name Collection : Geographic Name Cartographic Usability Range <lower value>',
'ZI015_GCUU':'Geo Name Collection : Geographic Name Cartographic Usability Range <upper value>',
'ZI016_ROC':'Route Pavement Information : Route Surface Composition #####',
'ZI016_ROC':'Route Pavement Information : Route Surface Composition',
'ZI016_WD1':'Route Pavement Information : Route Minimum Travelled Way Width',
'ZI016_WTC':'Route Pavement Information : Road Weather Restriction',
'ZI017_GAW':'Track Information : Railway Gauge',
'ZI017_RGC':'Track Information : Railway Gauge Classification',
'ZI017_RIR':'Track Information : Railway in Road',
'ZI017_RRA':'Track Information : Railway Power Method',
'ZI017_TRT':'Track Information : Track Type',
'ZI018_BRF':'Wireless : Broadcast Frequency',
'ZI018_WIT':'Wireless Telecommunication Information : Wireless Telecommunication Type',
'ZI019_ASP':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method',
'ZI019_ASP1':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [1]',
'ZI019_ASP2':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [2]',
'ZI019_ASP3':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [3]',
'ZI019_ASU':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition',
'ZI019_ASU1':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [1]',
'ZI019_ASU2':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [2]',
'ZI019_ASU3':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [3]',
'ZI019_ASX':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Category',
'ZI019_SFS':'Aerodrome Pavement Information : Aerodrome Pavement Functional Status',
'ZI020_ANM':'Geopolitical Entity Designation : Administrative Name',
'ZI020_FI1':'Geopolitical Entity Designation : FIPS 10-4 Country Code (two character)',
'ZI020_FI2':'Geopolitical Entity Designation : FIPS 10-4 Country Name',
'ZI020_FI2A':'Geopolitical Entity : Designation (1) : FIPS 10-4 Country Code (first side)',
'ZI020_FI2B':'Geopolitical Entity : Designation (2) : FIPS 10-4 Country Code (second side)',
'ZI020_GE4':'(Location Country) Designation : GENC Short URN-based Identifier',
'ZI020_GE42':'(Location Country) Designation : GENC Short URN-based Identifier (second)',
'ZI020_GE43':'(Location Country) Designation : GENC Short URN-based Identifier (third)',
'ZI020_GE44':'(Location Country) Designation : GENC Short URN-based Identifier (fourth)',
'ZI020_GE4A':'Designation : GENC Short URN-based Identifier',
'ZI020_GE4B':'Designation : GENC Short URN-based Identifier',
'ZI020_IC2':'Geopolitical Entity Designation : ISO 3166-1 Alpha-3 Code',
'ZI020_IC2A':'Geopolitical Entity : Designation (1) : ISO 3166-1 Alpha-3 Code (first side)',
'ZI020_IC2B':'Geopolitical Entity : Designation (2) : ISO 3166-1 Alpha-3 Code (second side)',
'ZI020_IC4':'Geopolitical Entity Designation : ISO 3166-1 English Name',
'ZI021_ANM':'Administrative Division Designation : Administrative Name',
'ZI021_FI5':'Administrative Division Designation : FIPS 10-4 Principal Administrative Division Code',
'ZI024_ASE':'Water Resource Information : Arsenic Concentration',
'ZI024_AWO':'Water : Predominant Water Flow',
'ZI024_AWOI':'Water : Predominant Water Flow - Maximum',
'ZI024_AWOX':'Water : Predominant Water Flow - Minimum',
'ZI024_CFR':'Water Resource Information : Coliform Concentration',
'ZI024_CLO':'Water Resource Information : Chloride Concentration',
'ZI024_CYN':'Water Resource Information : Cyanide Concentration',
'ZI024_DRL':'Water : Device Relative Location',
'ZI024_DVPT':'Water : Device Power Type',
'ZI024_FMR':'Water : Freshwater Production Rate / Maximum Rate',
'ZI024_HAR':'Water Resource Information : Water Hardness',
'ZI024_HYP':'Water Resource Information : Hydrologic Persistence',
'ZI024_MGN':'Water Resource Information : Magnesium Concentration',
'ZI024_PHW':'Water Resource Information : pH',
'ZI024_PMP':'Water : Pump Type',
'ZI024_PUR':'Water : Purification Process',
'ZI024_PUR2':'Water : Purification Process [2]',
'ZI024_PUR3':'Water : Purification Process [3]',
'ZI024_PUR4':'Water : Purification Process [4]',
'ZI024_PUR5':'Water : Purification Process [5]',
'ZI024_RTP':'Water : Reservoir Type',
'ZI024_SCC':'Water Resource Information : Water Type',
'ZI024_SQN':'Water : Water Flow Rate Category',
'ZI024_SUL':'Water Resource Information : Sulfate Concentration',
'ZI024_TDS':'Water Resource Information : Total Dissolved Solids',
'ZI024_TEP':'Water Resource Information : Temperature',
'ZI024_TUR':'Water Resource Information : Nephelometric Turbidity',
'ZI024_WAC':'Water Resource Information : Water Conductivity',
'ZI024_WPAC':'Water : Water Pump Accessibility',
'ZI024_WSG':'Water : Water Stage',
'ZI024_WTBB':'Water : Boron',
'ZI024_WTCA':'Water : Calcium',
'ZI024_WTFE':'Water : Total Iron',
'ZI024_WTKK':'Water : Potassium',
'ZI024_WTMN':'Water : Manganese',
'ZI024_WTNA':'Water : Sodium',
'ZI024_WTNO':'Water : Nitrate',
'ZI024_WTPO':'Water : Phosphate',
'ZI024_WTSI':'Water : Silicon',
'ZI024_WUR':'Water : Water Use',
'ZI024_WUR2':'Water : Water Use [2]',
'ZI024_WUR3':'Water : Water Use [3]',
'ZI024_WUR4':'Water : Water Use [4]',
'ZI024_YWQ':'Water Resource Information : Water Potability',
'ZI025_HDP':'Vartical Positioning : Hydrographic Depth',
'ZI025_MAN':'Hydrographic Vertical Positioning Information : Maritime Navigation Marked',
'ZI025_UHS':'Vertical Positioning : Uncovering Height Known',
'ZI025_WLE':'Hydrographic Vertical Positioning Information : Water Level Effect',
'ZI026_CTUC':'Feature Metadata : Cartographic Usability Range <interval closure>',
'ZI026_CTUL':'Feature Metadata : Cartographic Usability Range <lower value>',
'ZI026_CTUU':'Feature Metadata : Cartographic Usability Range <upper value>',
'ZI026_SUR':'Feature Metadata : Survey Coverage Category',
'ZI031S_URI':'Dataset (Surface) : Unique Resource Identifier',
'ZI032_GUG':'Pylon Information : Guyed',
'ZI032_PYC':'Pylon Information : Pylon Configuration',
'ZI032_PYM':'Pylon Information : Pylon Material',
'ZI032_TOS':'Pylon Information : Tower Shape',
'ZI037_REL':'Religious Information : Religious Designation',
'ZI037_RFA':'Religious Information : Religious Facility Type / Site Type',
'ZI039S_UFI':'Entity Collection Metadata (Surface) : Unique Entity Identifier',
'ZI071_CTX':'Cave Information : Cave Type',
'ZI071_FFN':'Cave Information : Feature Function',
'ZI071_FFN1':'Cave Information : Feature Function [1]',
'ZI071_FFN2':'Cave Information : Feature Function [2]',
'ZI071_FFN3':'Cave Information : Feature Function [3]',
'ZI071_UAO':'Cave Information : Underground Access Orientation',
'ZI103_MVD':'Magnetic Variation Information : Magnetic Variation Date',
'ZSAX_RS0':'Restriction Information : Security Attributes Group <resource classification>',
'ZSAX_RX0':'Restriction Information : Security Attributes Group <resource dissemination controls>',
'ZSAX_RX3':'Restriction Information : Security Attributes Group <resource non-intelligence community markings>',
'ZSAX_RX4':'Restriction Information : Security Attributes Group <resource owner-producer>',
'ZV2':'Highest Z-Value',
'ZV3':'Airfield/Aerodrome Elevation',
'ZVA':'Aerodrome Elevation',
'ZVAL_TYPE':'Vertical Source Category',
'ZVH':'Highest Elevation',
'ZVH_AVA':'Highest Elevation <absolute vertical accuracy>',
'ZVH_VDT':'Highest Elevation <vertical datum>'
    }
}

