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
   PrintOsmDocs.js

   This file "hijacks" either the "initialize" or the "translateToOsm" function 
   to print out documentation.

   This is the default translation file for hoot --print-docs
*/

// Set up stubs for the namespaces.
// If we don't do this, v8 complains about "xx.rules"
tds = {}
tds61 = {}
mgcp = {}
ufd = {}
dnc = {}
utp = {}
geonames = {}
navteq = {}


// Grab all of the init and rules files so we have somthing to work with
hoot.require('tds40_rules')
hoot.require('SchemaTools');
hoot.require('tds61_rules')
hoot.require('mgcp_rules')
hoot.require('ufd_rules')
hoot.require('utp_rules')
hoot.require('geonames_rules')
hoot.require('navteq_rules')
hoot.require('dnc_rules')
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
    createLookup(tagList, tds.rules.one2one, 'TDS40');
    createLookup(tagList, tds.rules.one2oneIn, 'TDS40');
    createLookup(tagList, tds61.rules.one2one, 'TDS61');
    createLookup(tagList, tds61.rules.one2oneIn, 'TDS61');
    createLookup(tagList, mgcp.rules.one2one, 'MGCP');
    createLookup(tagList, mgcp.rules.one2oneIn, 'MGCP');
    createLookup(tagList, ufd.rules.one2one, 'UFD');
    createLookup(tagList, utp.rules.one2one, 'UTP');
    createLookup(tagList, dnc.rules.one2one, 'DNC');
    // createLookup(tagList, navteq.rules.one2one, 'NAVTEQ');
    // createLookup(tagList, geonames.rules.one2one, 'GeoNames');

    // Add in the Biased rules
    createBiasedLookup(tagList, tds.rules.txtBiased, 'TDS40');
    createBiasedLookup(tagList, tds.rules.numBiased, 'TDS40');
    createBiasedLookup(tagList, tds61.rules.txtBiased, 'TDS61');
    createBiasedLookup(tagList, tds61.rules.numBiased, 'TDS61');
    createBiasedLookup(tagList, mgcp.rules.txtBiased, 'MGCP');
    createBiasedLookup(tagList, mgcp.rules.numBiased, 'MGCP');
    createBiasedLookup(tagList, ufd.rules.numBiased, 'UFD');
    createBiasedLookup(tagList, ufd.rules.txtBiased, 'UFD');
    createBiasedLookup(tagList, utp.rules.numBiased, 'UTP');
    createBiasedLookup(tagList, utp.rules.txtBiased, 'UTP');
    createBiasedLookup(tagList, dnc.rules.numBiased, 'DNC');
    createBiasedLookup(tagList, dnc.rules.txtBiased, 'DNC');
    // createBiasedLookup(tagList, NAVTEQ.rules.txtBiased, 'NAVTEQ');

    // F_CODE one2one rules
    createLookup(tagList, fcodeCommon.one2one, 'NFDD');
    createLookup(tagList, tds.rules.fcodeOne2oneIn, 'TDS40');
    createLookup(tagList, tds61.rules.fcodeOne2oneIn, 'TDS61');
    createLookup(tagList, mgcp.rules.fcodeOne2oneIn, 'MGCP');
    createLookup(tagList, ufd.rules.fcodeOne2oneIn, 'UFD');
    createLookup(tagList, utp.rules.fcodeOne2oneIn, 'UTP');
    createLookup(tagList, dnc.rules.fcodeOne2oneIn, 'DNC');

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
            if (!desc) desc = 'Text or number value';

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

    print('"Tag","Value","Tag_Description"');

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
            
            print('"' + tag + '","' + value + '","' + desc + '"');
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
    'ACS':'Accessibility Status',
    'ADI':'Administrative Division',
    'ADR':'Address',
    'AFA':'Available Small Craft Service/Available Vessel Service',
    'AFC':'Agricultural Facility Type',
    'AHA':'Absolute Horizontal Accuracy (90%)',
    'ALE':'Absolute Linear Error/Absolute Vertical Accuracy',
    'ALE_EVAL':'Absolute Vertical Accuracy Evaluation Method',
    'ALE_EVAL_METHOD_CD':'Absolute Linear Error Evaluation Method',
    'AMA':'Amusement Attraction Type',
    'AMB':'Special Administrative Unit',
    'AOO':'Angle of Orientation',
    'APT2':'Airfield Use [2]',
    'APT3':'Airfield Use [3]',
    'APT':'Airfield Use',
    'APU2':'Apron Usage [2]',
    'APU3':'Apron Usage [3]',
    'APU':'Apron Usage',
    'APY':'Apron Type',
    'AQN':'Aquifer Name',
    'AQO1':'Aquifer Overburden [1]',
    'AQO2':'Aquifer Overburden [2]',
    'AQO3':'Aquifer Overburden [3]',
    'AQO':'Aquifer Overburden',
    'AQP2':'Aquifer Composition [2]',
    'AQP3':'Aquifer Composition [3]',
    'AQP':'Aquifer Composition',
    'AQT':'Aquifer Thickness',
    'AQTC':'Aquifer Thickness <interval closure>',
    'AQTL':'Aquifer Thickness <lower value>',
    'AQTU':'Aquifer Thickness <upper value>',
    'ARA':'Area',
    'ART':'Aeronautical Route Category',
    'ASC':'Man-made',
    'ASU1':'Aerodrome Movement Area Surface Composition [1]',
    'ASU2':'Aerodrome Movement Area Surface Composition [2]',
    'ASU3':'Aerodrome Movement Area Surface Composition [3]',
    'ASU':'Aerodrome Movement Area Surface Composition',
    'ASY':'Airfield Symbol Type',
    'AT005_CAB1':'Cable : Cable Type [1]',
    'AT005_CAB2':'Cable : Cable Type [2]',
    'AT005_CAB3':'Cable : Cable Type [3]',
    'AT005_CAB':'Cable : Cable Type',
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
    'BAC':'Built-up Area Density Category',
    'BAL':'BGN Administrative Level',
    'BAT':'Barrier Top Type',
    'BC034_MRT':'Maritime Radiobeacon : Maritime Radiobeacon Type',
    'BC040_EOL':'Maritime Navigation Light : Nominal Range',
    'BC040_LCN':'Maritime Navigation Light : Light Characteristic Number',
    'BC040_LVN':'Maritime Navigation Light : Light Elevation',
    'BC040_PER':'Maritime Navigation Light : Period of Light',
    'BC101_SST':'Fog Signal : Fog Signal Type',
    'BCC':'Bypass Condition',
    'BCC':'Bypass Condition Category/Bypass Condition',
    'BEL':'Base Elevation',
    'BEN':'Basic Encyclopedia (BE) Number',
    'BE_NUMBER':'Basic Encyclopedia Number',
    'BFC':'Building Function Category',
    'BGT':'Basin Gate Type',
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
    'BMC1':'Bottom Material Type [1]',
    'BMC2':'Bottom Material Type [2]',
    'BMC3':'Bottom Material Type [3]',
    'BMC':'Bottom Material Type',
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
    'BRF':'Broadcast Frequency',
    'BRF2':'Broadcast Frequency 2',
    'BRG':'Bearing of Object',
    'BRN':'Bridge Reference Number',
    'BRR':'Bearing and Reciprocal Category',
    'BRS':'Bearing from Seaward',
    'BSC1':'Bridge Structure Type [1]',
    'BSC2':'Bridge Structure Type [2]',
    'BSC3':'Bridge Structure Type [3]',
    'BSC':'Bridge/Bridge Superstructure Category',
    'BSM':'Bridge Span Mobility',
    'BSS':'Building Superstructure Type',
    'BST':'Boundary Status/Boundary Status Type',
    'BSU':'Building Superstructure Type',
    'BUD':'Brush/Undergrowth Density Code',
    'BWVCAC':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (first bank) <interval closure>',
    'BWVCAL':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (first bank) <lower value>',
    'BWVCAU':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (first bank) <upper value>',
    'BWVCBC':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (second bank) <interval closure>',
    'BWVCBL':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (second bank) <lower value>',
    'BWVCBU':'Inland Waterbody Bank : Waterbody Bank Vegetation Cover (second bank) <upper value>',
    'CAA':'Controlling Authority',
    'CAB1':'Cable Type [1]',
    'CAB2':'Cable Type [2]',
    'CAB3':'Cable Type [3]',
    'CAB':'Cable Classification/Cable Type',
    'CAM':'Conservation Area Management Category',
    'CAT':'Cableway Type',
    'CBP':'Containment Berm Present',
    'CCG':'Military Environmental Hazard Category',
    'CCN':'Commercial Copyright Notice',
    'CCT':'Cover Closure Type',
    'CDA':'Covered Drain',
    'CDL':'Covered Drain Length',
    'CDM':'Cadastral Source Measurement',
    'CDR':'Commercial Distribution Restriction',
    'CDY':'Cadastral Source Type',
    'CEF':'Cultural Facility Type',
    'CET':'Cut/Embankment Type Category/Grading Type',
    'CFC':'Corrections Facility Type',
    'CID':'Cell Identifier',
    'CIT':'Commercial Facility Type',
    'CITY':'City Name',
    'CLI':'Isolated Communication Line',
    'CNF':'Confined',
    'CNS':'Controlled',
    'CNTRY_FULL':'Country Name',
    'COC':'Conspicuous Category',
    'COD':'Certainty of Delineation/Delineation Known',
    'COE':'Certainty of Existence',
    'COF':'Complex Outline Function',
    'COL':'Character of Light',
    'COMM_CPYRT_NOTICE':'Commercial Copyright Notice',
    'COMMENTS':'Comments',
    'COMM_LIC_TIER_NOTICE':'Distribution of Commercial Data',
    'COMPLETENESS_CODE':'Completeness Code',
    'CON':'Route Width Constriction Type',
    'COS':'Facility Operational Status',
    'CPA':'Survey Control Point Type',
    'CPS':'Cell Partition Scheme',
    'CPYRT_NOTE':'Commercial Copyright Notice',
    'CRA':'Crane Type Category/Crane Type',
    'CREATION_DATE':' Creation Date',
    'CRM':'Crane Mobility Type',
    'CRN':'Current Rate Minimum',
    'CRX':'Current Rate Maximum',
    'CSC1':'Intersection Control Type [1]',
    'CSC2':'Intersection Control Type [2]',
    'CSC3':'Intersection Control Type [3]',
    'CSC':'Intersection Control Type',
    'CSI':'Cadastral Source Identifier',
    'CSO':'Contaminant Source',
    'CSP':'Crop Species',
    'CST':'Contained within Service Tunnel',
    'CSY':'Cadastral Source Identifier Type',
    'CTC':'Culvert Type',
    'CTL':'Cumulative Track Length',
    'CUD':'Currency Date and Time',
    'CUL':'Cultural Context Type',
    'CUS':'Communication Facility Type',
    'CVH':'Depth Curve or Contour Value - High',
    'CVL':'Depth Curve or Contour Value - Low',
    'CVT2':'Conveyor Type [2]',
    'CVT3':'Conveyor Type [3]',
    'CVT':'Conveyor Type',
    'CWT':'Contained within Tunnel',
    'DAN':'Description of Aids to Navigation',
    'DDC':'Dwelling Type',
    'DEP':'Depth Below Surface Level',
    'DEV':'Deck Level',
    'DFT':'Dam Face Type',
    'DGC':'Transportation Block Type',
    'DIM':'Diameter',
    'DMBC':'Undergrowth Density <interval closure>',
    'DMB':'Density Measure (Brush/Undergrowth)/Undergrowth Density',
    'DMBL':'Undergrowth Density <lower value>',
    'DMBU':'Undergrowth Density <upper value>',
    'DMD':'Dammed',
    'DMF':'Feature Count',
    'DMT':'Canopy Cover',
    'DMT':'Canopy Cover/Density Measure (% of Tree/Canopy Cover)',
    'DOF':'Direction of Flow',
    'DPA':'Aquifer Depth',
    'DPAC':'Aquifer Depth <interval closure>',
    'DPAL':'Aquifer Depth <lower value>',
    'DPAU':'Aquifer Depth <upper value>',
    'DQS':'Data Quality Statement',
    'DRP':'Description of Reference Point',
    'DSP':'Boundary Dispute Type',
    'DWT':'Dam Type',
    'DZC':'Deck Count',
    'DZP':'Deepest Depth Below Surface Level',
    'EBT':'Educational Building Type/Educational Facility Type',
    'EET':'Engineered Earthwork Type',
    'ELA':'Elevation Accuracy',
    'ELE':'Elevation',
    'EOL':'Elevation of Light',
    'EPW':'Electrical Power Generation Capacity',
    'EQC':'Equivalent Scale Category',
    'ESC':'Elevation Surface Category',
    'ETS':'Extraction Specification',
    'ETY':'Engine Test Cell Type',
    'ETZ':'Extraction Specification Version',
    'EVA':'Elevation Vertical Accuracy (90%)',
    'EXISTENCE':'Existence Category',
    'EXP':'Route Width Expansion Type',
    'EXS':'Existence Category',
    'FAC':'Solid Maritime Construction',
    'FCL':'Ferry Crossing Distance',
    'FCL':'Ferry Crossing Distance/Ferry Crossing Length',
    'FCM':'Feature Construction Material',
    'F_CODE':'Feature Code',
    'FCO':'Feature Configuration',
    'FCS':'Flood Control Structure Type',
    'FER':'Ferry Crossing Type/Ferry Type',
    'FFN1':'Feature Function [1]',
    'FFN2':'Feature Function [2]',
    'FFN3':'Feature Function [3]',
    'FFN':'Feature Function',
    'FFP':'Farming Pattern',
    'FHC1':'Harbour Facility Function [1]',
    'FHC2':'Harbour Facility Function [2]',
    'FHC3':'Harbour Facility Function [3]',
    'FHC':'Harbour Facility Function',
    'FIC':'Embankment Type',
    'FLO':'Floating',
    'FMM':'Farming Method',
    'FPT':'Airfield Type/Facility Priority Type',
    'FRT1':'Weapons Range Type [1]',
    'FRT2':'Weapons Range Type [2]',
    'FRT3':'Weapons Range Type [3]',
    'FRT':'Firing Range Type/Weapons Range Type',
    'FSC':'Flight Strip Capable',
    'FTC':'Farming Type Category',
    'FTI':'Fence Type Indicator/Fence Type',
    'FTP':'Fabrication Type',
    'FUC':'Functional Use',
    'FUN':'Condition of Facility',
    'FUNCTION':'Feature Function',
    'FZR':'Fortified Building Type',
    'GAP_WIDTH':'Terrain Gap Width',
    'GAW':'Railway Gauge',
    'GB052_RIDH':'Runway Direction : Runway Designator (high end)',
    'GB052_RIDL':'Runway Direction : Runway Designator (low end)',
    'GEC':'Geopolitical Entity Type',
    'GFC':'Government Facility Type',
    'GFID':'Global Feature ID',
    'GFT':'Geologic Fault Trace Visible',
    'GNC':'Water Gate Type',
    'GOT':'Geothermal Outlet Type',
    'GSGCHC':'Runway Direction : Surface Slope (high end) <interval closure>',
    'GSGCHL':'Runway Direction : Surface Slope (high end) <lower value>',
    'GSGCHU':'Runway Direction : Surface Slope (high end) <upper value>',
    'GSGCLC':'Runway Direction : Surface Slope (low end) <interval closure>',
    'GSGCLL':'Runway Direction : Surface Slope (low end) <lower value>',
    'GSGCLU':'Runway Direction : Surface Slope (low end) <upper value>',
    'GTC':'Gate Type Category/Gate Use',
    'GUG':'Guyed or Unguyed Category',
    'HAF':'Helipad Associated Facility',
    'HCA':'Horizontal Clearance',
    'HCT':'Thoroughfare Class',
    'HDH':'Hydrographic Drying Height',
    'HDP':'Hydrographic Depth',
    'HEI':'Height of Object',
    'HFC':'Hydrological Form Category',
    'HGS':'Spillway Height',
    'HGT':'Height Above Surface Level',
    'HHD':'Homogenous Habitation Distribution',
    'HQC':'Hypsography Portrayal Type',
    'HSB':'Height above sea bottom',
    'HSS':'Historic Significance',
    'HST2':'Hazard Shelter Intended Use [2]',
    'HST3':'Hazard Shelter Intended Use [3]',
    'HST':'Hazard Shelter Intended Use',
    'HTP':'Hangar Type Category',
    'HVA':'Height Vertical Accuracy (90%)',
    'HWT':'House of Worship Type',
    'HYC':'Hydrological Category',
    'HYP':'Hydrologic Persistence',
    'HZD':'Geodetic Datum',
    'ICF':'Manufacturing Facility Type',
    'IKO':'ICAO Designator/ICAO Identifier/ICAO Location Indicator',
    'IKO':'ICAO Location Indicator',
    'INU':'Inundation Type',
    'IRG':'Irrigation Method',
    'ISO_3116_2':'ISO 3166-1 Alpha-2 Country Code',
    'ISO_3116_3':'ISO 3166-1 Alpha-3 Country Code',
    'ISS':'Isolated',
    'IWO':'Inland Water Obstruction',
    'IWT':'Inland Water Type',
    'KVA':'Kilovolt Capacity Attribute/Power Line Maximum Voltage',
    'LBV':'Left Bank Vegetation Cover',
    'LC1':'Load Class Type 1',
    'LC2':'Load Class Type 2',
    'LC3':'Load Class Type 3',
    'LC4':'Load Class Type 4',
    'LCA':'Lifting Capacity',
    'LDC':'Dam Crest Length',
    'LEA':'Least Depth Below Surface Level',
    'LEG':'Length/Diameter in Meters With Greater Precision',
    'LEN':'Length/Diameter',
    'LFA':'Aeronautical Light Function',
    'LINK_ID':'Link Id',
    'LMC':'Landmark Category/Navigation Landmark',
    'LND':'Land Morphology',
    'LNU':'Usable Length',
    'LOCATION':'Location Category/Vertical Relative Location',
    'LOC':'Location Category/Vertical Relative Location',
    'LOG':'Gradient Length',
    'LSA':'Light Sector Angle',
    'LSP':'Geopolitical Line Type',
    'LTN':'Track or Lane Count',
    'LUN':'Located Underground',
    'LVN':'Light Range Nominal',
    'LZN':'Length',
    'MAC':'Maritime Area Category',
    'MAG':'Magnetic Variation',
    'MAN':'Maritime Navigation Marked',
    'MCC1':'Structural Material Type [1]',
    'MCC2':'Structural Material Type [2]',
    'MCC3':'Structural Material Type [3]',
    'MCC':'Material Composition Category/Structural Material Type',
    'MCS':'Material Composition Secondary',
    'MCX':'Motorized Crossing',
    'MDE':'Maintenance Date and Time',
    'MED':'Median Category',
    'MEM':'Memorandum',
    'MES':'Median Present',
    'MFB':'Manufactured Building',
    'MFC':'Military Facility Type',
    'MGL1':'Maritime Geopolitical Limit Type [1]',
    'MGL2':'Maritime Geopolitical Limit Type [2]',
    'MGL3':'Maritime Geopolitical Limit Type [3]',
    'MINE':'Mining Category',
    'MIN':'Extraction Mine Type/Mining Category',
    'MLR':'Multiple Light Ranges',
    'MNS':'Man-made Shoreline',
    'MOH':'Maximum Obstacle Height',
    'MST2':'Missile Site Type [2]',
    'MST3':'Missile Site Type [3]',
    'MST':'Missile Site Type',
    'MUB':'Multi-unit Building',
    'MVC':'Maximum Vertical Clearance',
    'MWD':'Maximum Design Water Depth',
    'MWG':'Centerline Spacing',
    'MZN':'Extraction Mine Type',
    'NA2':'Touristic Name',
    'NA3':'Classification Name',
    'NA8':'Aerodrome Official Name',
    'NAM':'Name Category/Name',
    'NAO':'Installation Operator',
    'NFI':'Named Feature Identifier',
    'NFN':'Name Identifier',
    'NLT':'Named Location Type',
    'NM3':'Name 3 (name of the political entity on one side of a boundary).',
    'NM4':'Name 4 (name of the political entity on the other side of the boundary).',
    'NOS':'Span Count',
    'NPL':'Number of Parallel Lines',
    'NST':'Navigation System Type',
    'NVS':'Navigability Information',
    'OCC':'Overhead Clearance Category Code',
    'OCS':'Offshore Construction Primary Structure',
    'OHB':'Overall Bridge Height/Overhead Height of Bridge',
    'OHC':'Overhead Clearance Category/Overhead Clearance',
    'OHD':'Derived Obstacle Height/Depth Category',
    'ONE':'One-way',
    'OOC':'Overhead Obstruction Type',
    'OPC':'Offshore Platform Type',
    'OPERATOR':'Operator ID',
    'OPT':'Operating Cycle',
    'OR2':'Operating Range Category',
    'ORC':'Operating Range Category 2',
    'ORD':'Ordinal Category/Relative Importance',
    'ORIG':'Feature Metadata : Process Step Information : Resource Content Originator',
    'ORIGINATING_SOURCE':'Originating Source Name',
    'ORIGINATOR':'Feature Metadata : Process Step Information : Resource Content Originator',
    'ORIG_SOURCE_DATE':'Originating Source Date',
    'ORIG_SOURCE_INFO':'Originating Source Information',
    'OTH':'Specified Domain Value(s)/Specified Enumerant(s)',
    'OWO':'Waterbody Overhead Obstruction',
    'PAF':'Public Accommodation Facility',
    'PBY1':'By-product [1]',
    'PBY2':'By-product [2]',
    'PBY3':'By-product [3]',
    'PBY':'By-product',
    'PCF':'Physical Condition',
    'PDA':'Property DFDD-compatible Accessor',
    'PEC':'Port of Entry',
    'PFD':'Predominant Feature Depth',
    'PFG':'Predominant Feature Height',
    'PFH':'Predominant Feature Height',
    'PHT':'Predominant Height',
    'PLT2':'Pipeline Type [2]',
    'PLT3':'Pipeline Type [3]',
    'PLT':'Pipeline Type',
    'POS2':'Power Source [2]',
    'POS3':'Power Source [3]',
    'POS':'Power Source',
    'PPC':'Power Plant Category/Power Plant Type',
    'PPO1':'Product [1]',
    'PPO2':'Product [2]',
    'PPO3':'Product [3]',
    'PPO':'Product',
    'PPT':'Populated Place Type',
    'PRM':'Permanency/Permanent',
    'PRODUCT':'Product Category',
    'PRO':'Product Category',
    'PRW1':'Raw Material [1]',
    'PRW2':'Raw Material [2]',
    'PRW3':'Raw Material [3]',
    'PRW':'Raw Material',
    'PSE':'Periodic Restriction Month Interval',
    'PSF':'Public Service Facility Type',
    'PVH':'Predominant Vegetation Height',
    'PWAC':'Predominant Water Depth <interval closure>',
    'PWAL':'Predominant Water Depth <lower value>',
    'PWA':'Predominant Water Depth',
    'PWAU':'Predominant Water Depth <upper value>',
    'PWC':'Shoreline Construction Type',
    'PYC':'Pylon Configuration',
    'PYM':'Pylon Material',
    'QLE':'Restrictions on Releasibility',
    'RAC':'Radar Antenna Configuration',
    'RAD':'Curve Radius',
    'RAS':'Radar Station Function',
    'RAY':'Racing Type',
    'RBV':'Right Bank Vegetation Cover',
    'RCG':'Resource Content Originator',
    'REL':'Religious Designation',
    'RES':'Research Facility Type',
    'RFC':'Religious Facility Type',
    'RFD':'Roofed',
    'RGC':'Railroad Gauge Category/Railway Gauge Classification',
    'RIN_ROI2':'Route Identification [2] <route designation type>',
    'RIN_ROI3':'Route Identification [3] <route designation type>',
    'RIN_ROI':'Route Identification <route designation type>',
    'RIN_RTN2':'Route Identification [2] <route designation>',
    'RIN_RTN3':'Route Identification [3] <route designation>',
    'RIN_RTN':'Route Identification <route designation>',
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
    'RPC':'Boundary Representation Policy',
    'RRA':'Railroad Power Source',
    'RRC1':'Railway Use [1]',
    'RRC2':'Railway Use [2]',
    'RRC3':'Railway Use [3]',
    'RRC':'Railroad Categories/Railway Use',
    'RSA':'Branch Railway Type/Railroad Siding Attribute',
    'RST':'Load-bearing Surface Type',
    'RSTRN_DCLS_XMPT_CD':'Restriction Declassification Exemptions',
    'RSTRN_DECLASS':'Restriction Declassification',
    'RSTRN_DSEM_CTRL_IC':'Restrictions on Dissemination Control IC',
    'RSTRN_DSEM_CTRL_NIC':'Restrictions on Dissemination Control Non-IC',
    'RSTRN_FOREIGN_GOV':'Restrctions on Foreign Government Access',
    'RSTRN_RELEASIBILITY':'Restrictions on Releasibility',
    'RST':'Road/Runway Surface Type',
    'RTA':'Linear Feature Arrangement',
    'RTN2':'Route Designation [2]',
    'RTN3':'Route Designation [3]',
    'RTN_ROI2':'Route Designation [2] (route designation type)',
    'RTN_ROI3':'Route Designation [3] (route designation type)',
    'RTN_ROI':'Route Designation (route designation type)',
    'RTN':'Route Designation/Route Number',
    'RTP':'Reservoir Type',
    'RTT':'Route Intended Use',
    'RTY':'Roadway Type',
    'RWC':'Railway Class',
    'SAD':'Sand Dune Stabilized',
    'SBB':'Supported by Bridge Span',
    'SBC':'Shelter Belt',
    'SBT1':'Substation Type [1]',
    'SBT2':'Substation Type [2]',
    'SBT3':'Substation Type [3]',
    'SBT':'Substation Type',
    'SCALE':'Feature Scale',
    'SCAMIN':'Feature Metadata : Cartographic Usability Range <lower value>',
    'SCAMAX':'Feature Metadata : Cartographic Usability Range <upper value>',
    'SCB':'Pavement Condition/Surface Condition Attribute',
    'SCC':'Spring/Well Characteristic Category/Water Type',
    'SDCC':'Soil Depth <interval closure>',
    'SDCL':'Soil Depth <lower value>',
    'SDCU':'Soil Depth <upper value>',
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
    'SFS':'Aerodrome Pavement Functional',
    'SFY':'Security Facility Type',
    'SGCC':'Surface Slope <interval closure>',
    'SGC':'Gradient/Slope',
    'SGCL':'Surface Slope <lower value>',
    'SGCU':'Surface Slope <upper value>',
    'SHC':'Safe Horizontal Clearance',
    'SHD':'Shoreline Delineated',
    'SHL':'Left Bank Shoreline Type',
    'SHP':'Shape of Beacon',
    'SHR':'Right Bank Shoreline Type',
    'SHT':'Shipping Container Type',
    'SIC':'Frozen Cover Type',
    'SLR':'Shoreline Ramp Type',
    'SLT':'Shoreline Type Category/Shoreline Type',
    'SMC':'Surface Material Category/Surface Material Type',
    'SOH':'Severity of Hazard',
    'SOURCE':'Originating Source Information',
    'SPAN_TYPE':'Bridge Opening Type',
    'SPEC_ID':'Extraction Specification ID',
    'SPM':'Speed Limit (KPH)',
    'SPT':'Supported',
    'SRC_DATE':'Source Date and Time',
    'SRC_INFO':'Source Description',
    'SRC_NAME':'Source Type',
    'SRC_RELSE':'Restrictions on Releasibility of the source data',
    'SRD':'Terrain Morphology',
    'SRE':'Traffic Restriction Type',
    'SRL1':'Location Referenced to Shoreline [1]',
    'SRL2':'Location Referenced to Shoreline [2]',
    'SRL3':'Location Referenced to Shoreline [3]',
    'SRL':'Location Referenced to Shoreline',
    'SRT':'Source Type',
    'SSC':'Structure Shape Category/Structure Shape',
    'SSE':'Seasonal Ice Limit',
    'SSG':'Street Sign Type',
    'SSO':'Shaft Slope Orientation',
    'SSR1':'Roof Shape [1]',
    'SSR2':'Roof Shape [2]',
    'SSR3':'Roof Shape [3]',
    'SSR':'Roof Shape/Structure Shape of Roof',
    'STA':'Maritime Station Type',
    'STL1':'Nomadic Seasonal Location [1]',
    'STL2':'Nomadic Seasonal Location [2]',
    'STL3':'Nomadic Seasonal Location [3]',
    'STL':'Nomadic Seasonal Location',
    'STP':'Soil Type',
    'SUC':'Shed Type',
    'SUY':'Survey Point Type',
    'SWC':'Soil Wetness Condition',
    'SWT':'Natural Pool Type/Well or Spring Feature Type',
    'SWW':'Static Water Level',
    'TCS':'Cross-sectional Profile',
    'TEL':'Telescope Type',
    'TFC':'Transportation Facility Type',
    'THI':'Thickness',
    'THR':'Through Route',
    'TID':'Tidal/Non-Tidal Category/Tide Influenced',
    'TIER_NOTE':'Commercial Distribution Restriction',
    'TOS':'Tower Shape',
    'TOWER_TYPE':'Tower Type Category/Tower Type',
    'TRA':'Pedestrian Traversable/Traversability',
    'TRE':'Foliage Type/Tree Type Category',
    'TRF':'Traffic Flow',
    'TRP':'Transportation Route Protection Structure Type',
    'TRS1':'Transportation System Type [1]',
    'TRS2':'Transportation System Type [2]',
    'TRS3':'Transportation System Type [3]',
    'TRS':'Transportation System Type',
    'TSCC':'Tree Spacing <interval closure>',
    'TSCL':'Tree Spacing <lower value>',
    'TSCU':'Tree Spacing <upper value>',
    'TSM1':'Terrain Surface Material [1]',
    'TSM2':'Terrain Surface Material [2]',
    'TSM3':'Terrain Surface Material [3]',
    'TSM':'Terrain Surface Material',
    'TST':'Cable Suspended Shape/Power Line Shape/Transmission Suspension Type',
    'TTC1':'Tower Type [1]',
    'TTC2':'Tower Type [2]',
    'TTC3':'Tower Type [3]',
    'TTC':'Tower Type Category/Tower Type',
    'TTY':'Tomb Type',
    'TUC':'Transportation Use Category',
    'TXP':'Taxiway Type',
    'TXT':'Associated Text',
    'TYP':'Thoroughfare Type',
    'UBC':'Underbridge Clearance',
    'UFI':'Unique Entity Identifier',
    'UHS':'Uncovering Height Known',
    'UID':'MGCP Feature universally unique identifier',
    'UMA':'Underground Mine Access',
    'UPDATE_SOURCE_DATE':'Update/Review Source Date',
    'UPDATE_SOURCE_INFO':'Update/Review Source Information',
    'UPDATE_SOURCE':'Update/Review Source Name',
    'UPD_DATE':'Review Source Date and Time',
    'UPD_INFO':'Review Source Description',
    'UPD_NAME':'Review Source Type',
    'URI':'Unique Resource Identifier',
    'USAGE':'Usage Category',
    'USE_':'Usage2',
    'USG':'Usage Category',
    'UTY1':'Accessible Utility Type [1]',
    'UTY2':'Accessible Utility Type [2]',
    'UTY3':'Accessible Utility Type [3]',
    'UTY':'Accessible Utility Type',
    'UUC':'Utility Facility Type',
    'VAV':'Variation Anomaly Value',
    'VAL':'Value - A Generic Value',
    'VCA1':'Void Collection Reason [1]',
    'VCA2':'Void Collection Reason [2]',
    'VCA3':'Void Collection Reason [3]',
    'VCA':'Void Collection Attribute/Void Collection Reason',
    'VCM1':'Vertical Construction Material [1]',
    'VCM2':'Vertical Construction Material [2]',
    'VCM3':'Vertical Construction Material [3]',
    'VCM':'Vertical Construction Material',
    'VCS_VCD':'Vertical Clearance, Safe <reference level>',
    'VCS_VCR':'Vertical Clearance, Safe <reference level remark>',
    'VCS':'Vertical Clearance, Safe',
    'VCT1':'Void Collection Type [1]',
    'VCT2':'Void Collection Type [2]',
    'VCT3':'Void Collection Type [3]',
    'VCT':'Void Collection Type',
    'VDC':'Sounding Datum/Vertical Datum Category',
    'VDR':'Vertical Datum Record',
    'VDT':'Vertical Datum',
    'VEG':'Vegetation Characteristic',
    'VET':'Vehicle Type',
    'VGT':'Volcanic Geologic Type/Volcano Shape',
    'VH3':'Predominant Vegetation Height',
    'VLM':'Volume',
    'VOA':'Volcanic Activity',
    'VOI':'Vertical Obstruction Identifier',
    'VOL':'Volume',
    'VRR':'Vertical Reference Category',
    'VSP1':'Vegetation Species [1]',
    'VSP2':'Vegetation Species [2]',
    'VSP3':'Vegetation Species [3]',
    'VSP':'Vegetation Species',
    'VST':'Vehicle Scale Count',
    'VTI':'Vegetation Trafficability Impact',
    'WAD':'Water Well Construction Description',
    'WBD':'Waterbody Depth',
    'WCC':'Watercourse Channel Type',
    'WD1':'Route Minimum Travelled Way Width',
    'WD2':'Route Total Usable Width',
    'WD3':'Terrain Gap Width',
    'WD5':'Width at Top',
    'WDAC':'Average Water Depth <interval closure>',
    'WDAL':'Average Water Depth <lower value>',
    'WDAU':'Average Water Depth <upper value>',
    'WDH':'Predominant Maximum Water Depth',
    'WDL':'Predominant Minimum Water Depth',
    'WDU':'Usable Width',
    'WEATHER':'Road Weather Restriction/Weather Type Category',
    'WEQ1':'Well Equipment [1]',
    'WEQ2':'Well Equipment [2]',
    'WEQ3':'Well Equipment [3]',
    'WEQ':'Well Equipment',
    'WFT':'Well Feature Type/Well Type',
    'WGP':'Width with greater than 1 meter resolution',
    'WID':'Width',
    'WLE':'Water Level Effect',
    'WMT':'Watercourse Morphology',
    'WOC':'Width of Crest',
    'WORSHIP':'House of Worship Type',
    'WPI':'World Port Index Identifier',
    'WRT':'Water Race Type',
    'WST':'Watercourse Sink Type',
    'WT2':'Width of Second Travelled Way',
    'WTC':'Road Weather Restriction/Weather Type Category',
    'WTI':'Wall Type',
    'WVA':'Predominant Average Water Velocity',
    'WVH':'Predominant Maximum Water Velocity',
    'WVL':'Predominant Minimum Water Velocity',
    'YSU':'Military Service Branch',
    'YWQ':'Water Potability',
    'ZI001_NSD':'Source Information : Non-spatial Source Date and Time',
    'ZI001_NSN':'Source Information : Non-spatial Source Description',
    'ZI001_SDP':'Source Information : Source Description',
    'ZI001_SDV':'Source Information : Source Date and Time',
    'ZI001_SPS':'Source Information : Spatial Source',
    'ZI001_SRT':'Source Information : Source Type',
    'ZI001_SSD':'Source Information : Spatial Source Date and Time',
    'ZI001_SSN':'Source Information : Spatial Source Description',
    'ZI001_VSC':'Source Information : Vertical Source Category',
    'ZI001_VSD':'Source Information : Vertical Source Date and Time',
    'ZI001_VSN':'Source Information : Vertical Source Description',
    'ZI002_CCN':'Commercial Copyright Notice',
    'ZI002_UFI':'Restriction Information : Unique Entity Identifier',
    'ZI004_PRE':'Process Step Information : Process Step Description',
    'ZI004_RCG':'Feature Metadata : Process Step Information : Resource Content Originator',
    'ZI005_FNA1':'Geographic Name Information : (first) Full Name',
    'ZI005_FNA2':'Geographic Name Information : Full Name (second)',
    'ZI005_FNA3':'Geographic Name Information : Full Name (third)',
    'ZI005_FNAA':'Geopolitical Entity : Geographic Name Information (1) : Full Name (first side)',
    'ZI005_FNAB':'Geopolitical Entity : Geographic Name Information (2) : Full Name (second side)',
    'ZI005_FNA':'Geographic Name Information : Full Name',
    'ZI005_NFN1':'Geographic Name Information : (first) Name Identifier',
    'ZI005_NFN2':'Geographic Name Information : Name Identifier (second)',
    'ZI005_NFN3':'Geographic Name Information : Name Identifier (third)',
    'ZI005_NFNA':'Geopolitical Entity : Geographic Name Information (1) : Name Identifier (first side)',
    'ZI005_NFNB':'Geopolitical Entity : Geographic Name Information (2) : Name Identifier (second side)',
    'ZI005_NFN':'Geographic Name Information : Name Identifier',
    'ZI006_MEM':'Note : Memorandum',
    'ZI013_CSP1':'Crop Information : Crop Species [1]',
    'ZI013_CSP2':'Crop Information : Crop Species [2]',
    'ZI013_CSP3':'Crop Information : Crop Species [3]',
    'ZI013_CSP':'Crop Information : Crop Species',
    'ZI013_FFP1':'Crop Information : Farming Pattern [1]',
    'ZI013_FFP2':'Crop Information : Farming Pattern [2]',
    'ZI013_FFP3':'Crop Information : Farming Pattern [3]',
    'ZI013_FFP':'Crop Information : Farming Pattern',
    'ZI013_FMM1':'Crop Information : Farming Method [1]',
    'ZI013_FMM2':'Crop Information : Farming Method [2]',
    'ZI013_FMM3':'Crop Information : Farming Method [3]',
    'ZI013_FMM':'Crop Information : Farming Method',
    'ZI013_IRG':'Crop Information : Irrigation Method',
    'ZI013_PIG':'Crop Information : Permanent Irrigation',
    'ZI014_PBY1':'Manufacturing Information : By-product [1]',
    'ZI014_PBY2':'Manufacturing Information : By-product [2]',
    'ZI014_PBY3':'Manufacturing Information : By-product [3]',
    'ZI014_PBY':'Manufacturing Information : By-product',
    'ZI014_PPO1':'Manufacturing Information : Product [1]',
    'ZI014_PPO2':'Manufacturing Information : Product [2]',
    'ZI014_PPO3':'Manufacturing Information : Product [3]',
    'ZI014_PPO':'Manufacturing Information : Product',
    'ZI014_PRW1':'Manufacturing Information : Raw Material [1]',
    'ZI014_PRW2':'Manufacturing Information : Raw Material [2]',
    'ZI014_PRW3':'Manufacturing Information : Raw Material [3]',
    'ZI014_PRW':'Manufacturing Information : Raw Material',
    'ZI016_ROC':'Route Pavement Information : Route Surface Composition',
    'ZI016_WD1':'Route Pavement Information : Route Minimum Travelled Way Width',
    'ZI016_WTC':'Route Pavement Information : Road Weather Restriction',
    'ZI017_GAW':'Track Information : Railway Gauge',
    'ZI017_RGC':'Track Information : Railway Gauge Classification',
    'ZI017_RIR':'Track Information : Railway in Road',
    'ZI017_RRA':'Track Information : Railway Power Method',
    'ZI017_TRT':'Track Information : Track Type',
    'ZI018_WIT':'Wireless Telecommunication Information : Wireless Telecommunication Type',
    'ZI019_ASP1':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [1]',
    'ZI019_ASP2':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [2]',
    'ZI019_ASP3':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [3]',
    'ZI019_ASP':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method',
    'ZI019_ASU1':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [1]',
    'ZI019_ASU2':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [2]',
    'ZI019_ASU3':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [3]',
    'ZI019_ASU':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition',
    'ZI019_ASX':'Aerodrome Pavement Information : Aerodrome Movement Area Surface Category',
    'ZI019_SFS':'Aerodrome Pavement Information : Aerodrome Pavement Functional Status',
    'ZI020_ANM':'Geopolitical Entity Designation : Administrative Name',
    'ZI020_FI1':'Geopolitical Entity Designation : FIPS 10-4 Country Code (two character)',
    'ZI020_FI2A':'Geopolitical Entity : Designation (1) : FIPS 10-4 Country Code (first side)',
    'ZI020_FI2B':'Geopolitical Entity : Designation (2) : FIPS 10-4 Country Code (second side)',
    'ZI020_FI2':'Geopolitical Entity Designation : FIPS 10-4 Country Name',
    'ZI020_GE42':'(Location Country) Designation : GENC Short URN-based Identifier (second)',
    'ZI020_GE43':'(Location Country) Designation : GENC Short URN-based Identifier (third)',
    'ZI020_GE44':'(Location Country) Designation : GENC Short URN-based Identifier (fourth)',
    'ZI020_GE4':'(Location Country) Designation : GENC Short URN-based Identifier',
    'ZI020_IC2A':'Geopolitical Entity : Designation (1) : ISO 3166-1 Alpha-3 Code (first side)',
    'ZI020_IC2B':'Geopolitical Entity : Designation (2) : ISO 3166-1 Alpha-3 Code (second side)',
    'ZI020_IC2':'Geopolitical Entity Designation : ISO 3166-1 Alpha-3 Code',
    'ZI020_IC4':'Geopolitical Entity Designation : ISO 3166-1 English Name',
    'ZI021_ANM':'Administrative Division Designation : Administrative Name',
    'ZI021_FI5':'Administrative Division Designation : FIPS 10-4 Principal Administrative Division Code',
    'ZI024_ASE':'Water Resource Information : Arsenic Concentration',
    'ZI024_CFR':'Water Resource Information : Coliform Concentration',
    'ZI024_CLO':'Water Resource Information : Chloride Concentration',
    'ZI024_CYN':'Water Resource Information : Cyanide Concentration',
    'ZI024_HAR':'Water Resource Information : Water Hardness',
    'ZI024_HYP':'Water Resource Information : Hydrologic Persistence',
    'ZI024_MGN':'Water Resource Information : Magnesium Concentration',
    'ZI024_PHW':'Water Resource Information : pH',
    'ZI024_SCC':'Water Resource Information : Water Type',
    'ZI024_SUL':'Water Resource Information : Sulfate Concentration',
    'ZI024_TDS':'Water Resource Information : Total Dissolved Solids',
    'ZI024_TEP':'Water Resource Information : Temperature',
    'ZI024_TUR':'Water Resource Information : Nephelometric Turbidity',
    'ZI024_WAC':'Water Resource Information : Water Conductivity',
    'ZI024_YWQ':'Water Resource Information : Water Potability',
    'ZI025_MAN':'Hydrographic Vertical Positioning Information : Maritime Navigation Marked',
    'ZI025_WLE':'Hydrographic Vertical Positioning Information : Water Level Effect',
    'ZI026_CTUC':'Feature Metadata : Cartographic Usability Range <interval closure>',
    'ZI026_CTUL':'Feature Metadata : Cartographic Usability Range <lower value>',
    'ZI026_CTUU':'Feature Metadata : Cartographic Usability Range <upper value>',
    'ZI026_SUR':'Feature Metadata : Survey Coverage Category',
    'ZI032_GUG':'Pylon Information : Guyed',
    'ZI032_PYC':'Pylon Information : Pylon Configuration',
    'ZI032_PYM':'Pylon Information : Pylon Material',
    'ZI032_TOS':'Pylon Information : Tower Shape',
    'ZI037_REL':'Religious Information : Religious Designation',
    'ZI037_RFA':'Religious Information : Religious Facility Type',
    'ZI071_CTX':'Cave Information : Cave Type',
    'ZI071_FFN1':'Cave Information : Feature Function [1]',
    'ZI071_FFN2':'Cave Information : Feature Function [2]',
    'ZI071_FFN3':'Cave Information : Feature Function [3]',
    'ZI071_FFN':'Cave Information : Feature Function',
    'ZI071_UAO':'Cave Information : Underground Access Orientation',
    'ZSAX_RS0':'Restriction Information : Security Attributes Group <resource classification>',
    'ZSAX_RX0':'Restriction Information : Security Attributes Group <resource dissemination controls>',
    'ZSAX_RX3':'Restriction Information : Security Attributes Group <resource non-intelligence community markings>',
    'ZSAX_RX4':'Restriction Information : Security Attributes Group <resource owner-producer>',
    'ZV2':'Highest Z-Value',
    'ZV3':'Airfield/Aerodrome Elevation',
    'ZVA':'Aerodrome Elevation',
    'ZVAL_TYPE':'Vertical Source Category',
    'Z_VALUE_TYPE':'Z-Coordinate Type',
    'ZVH_AVA':'Highest Elevation <absolute vertical accuracy>',
    'ZVH':'Highest Elevation',
    'ZVH_VDT':'Highest Elevation <vertical datum>'
    }
}

