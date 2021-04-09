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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014 Maxar (http://www.maxar.com/)
 */

/*
   PrintOsmTds.js

   This file "hijacks" either the "initialize" or the "translateToOsm" function
   to print print out the OSM<>TDS tanslation forach TDS feature
   The next step with this would be to expand this to all of the other translations.
*/

hoot.require('SchemaTools');
hoot.require('tds70');
hoot.require('tds70_full_schema');
hoot.require('tds70_rules');
hoot.require('config');
hoot.require('translate');
hoot.require('fcode_common');


// The initialize function gets called once BEFORE the translateToXXX functions
// We are hijacking this function to print out documentation
function initialize()
{
  var tagList = {}; // Final list of tags

  // Change the Schema as well
  // var schema = tds40.schema.getDbSchema();
  var schema = tds70.schema.getDbSchema();

  // Setup translation lookup table
  fcodeCommon.one2one.push.apply(fcodeCommon.one2one,tds70.rules.fcodeOne2oneIn);
  tds70.fcodeLookup = translate.createLookup(fcodeCommon.one2one);

  // hoot.print("#####");
  // translate.dumpOne2OneLookup(tds70.fcodeLookup);
  // hoot.print("#####");

  tds70.rules.one2one.push.apply(tds70.rules.one2one,tds70.rules.one2oneIn);
  tds70.lookup = translate.createLookup(tds70.rules.one2one);

  // translate.dumpOne2OneLookup(tds70.lookup);
  // hoot.print("#####");

  // Print the tagList
  switch (config.getTagPrintingFormat())
  {
  //         case 'html':
  //             dumpHtmlTags(tagList);
  //             break;
  //
  //         case 'asciidoc':
  //             dumpAsciidocTags(tagList);
  //             break;
  //
  //         case 'json':
  //             dumpJsonTags(tagList);
  //             break;
  //
  //         case 'redmine':
  //             dumpRedmineTags(tagList);
  //             break;
  //
  case 'csv':
  default:
    dumpCsvTags(schema);
    break;
  }

  return null;
} // end Initialize

// The finalize function gets called once AFTER the translateToXXX functions
function finalize()
{
  // hoot.print('In the finalize function');
}


// We are not using this any more.
function translateToOsm(attrs, layerName, geometryType)
{
  return null;
}


// Adds values to the tagList lookup table
function createLookup(tagList, schema)
{
} // End createLookup

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
  hoot.print('<html>');
  hoot.print('<head>');
  hoot.print('<title>Hootennanny OSM+ Tags</title>');
  hoot.print('</head>');
  hoot.print('<style>');
  hoot.print('table,th,td { border:1px solid black; border-collapse:collapse; padding-left:5px; padding-right:5px; }');
  hoot.print('caption { text-align:left; font-weight:bold;}');
  hoot.print('</style>');
  hoot.print('<body>');
  hoot.print('<h2 align=center>Hootennanny OSM+ Tags</h2>');
  hoot.print('<p>Generated: ' + Date() + '</p>');

  for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
  {
    shortTag = shortKeys[i];

    hoot.print('<table>');
    hoot.print('<caption>' + shortTag + '<caption>');
    hoot.print('<tr>');
    hoot.print('<th>Key</th><th>Value</th><th>Comment</th><th>Source</th>');
    hoot.print('</tr>');

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

      hoot.print('<tr>');
      hoot.print('<td>' + tag + '</td><td>' + value + '</td><td>' + desc + '</td><td>' + source + '</td>');
      hoot.print('</tr>');
    }
    hoot.print('</table>');
    hoot.print('<br>');
    hoot.print('<br>');
  }

  // End the HTML document
  hoot.print('</body>');
  hoot.print('</html>');

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

  hoot.print('Hootennanny OSM+ Tags');
  hoot.print('=====================');
  hoot.print('Generated: ' + Date());
  hoot.print('');

  for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
  {
    shortTag = shortKeys[i];

    // hoot.print('.An ' + shortTag);
    hoot.print('.' + shortTag);
    hoot.print('[options="header"]');
    hoot.print('|==========');
    hoot.print('| Key | Value | Comment | Source');

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

      hoot.print('| ' + tag + ' | ' + value + ' | ' + desc + ' | ' + source);
    }

    hoot.print('|==========');
    hoot.print('');
    hoot.print('');
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

  hoot.print('h1. Hootennanny OSM+ Tags');
  hoot.print('');
  hoot.print('Generated: ' + Date());
  hoot.print('');
  hoot.print('----');
  hoot.print('');

  for (var i = 0, tlen = shortKeys.length; i < tlen; i++)
  {
    shortTag = shortKeys[i];

    hoot.print('h2. ' + shortTag);
    hoot.print('');
    hoot.print('|_.Key| _.Value|_.Comment|_.Source|');

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

      hoot.print('| ' + tag + ' | ' + value + ' | ' + desc + ' | ' + source + ' |');
    }

    hoot.print('');
    hoot.print('----');
    hoot.print('');
  }
} // End dumpRedmineTags


// Dump the tagList out as a Large Ugly JSON Blob.
function dumpJsonTags(tagList)
{
  hoot.print('JSON format is not working yet');
  hoot.print(tagList.toString());
} // End dumpJsonTags


// Dump the schema out in CSV format.
function dumpCsvTags(schema)
{
  hoot.print('"layerName","F_CODE",attributeName","attributeDescription","attributeValue","attributeValueDescription","osmTag","osmValue"');

  for (var i = 0, iLen = schema.length; i < iLen; i++)
  {
    var featureName = schema[i].name;
    var F_CODE = schema[i].fcode;
    var tName = '';

    hoot.print('"' + featureName + '","' + F_CODE + '","","","","' + tds70.fcodeLookup['F_CODE'][F_CODE][0] + '","' + tds70.fcodeLookup['F_CODE'][F_CODE][1]+ '"');

    for (var j = 0, jLen = schema[i].columns.length; j < jLen; j++)
    {
      var columnName = schema[i].columns[j].name;
      var columnDesc = schema[i].columns[j].desc;
      var columnType = schema[i].columns[j].type;

      if (columnDesc == 'Feature Code') continue; // Skip the F_CODE

      var tColumnName = columnName;

      // We swap some names on import to avoid code duplication
      if (tds70.rules.swapListIn[columnName]) tColumnName = tds70.rules.swapListIn[columnName];

      if (columnType !== 'enumeration')
      {
        if (columnType == 'String')
        {
          tName = tds70.rules.txtBiased[tColumnName];
        }
        else
        {
          tName = tds70.rules.numBiased[tColumnName];
        }
        hoot.print('"' + featureName + '","' + F_CODE + '","' + columnName + '","' +columnDesc + '","<value>","' + tName + '","<value>"');

        continue;
      }

      for (var k = 0, kLen = schema[i].columns[j].enumerations.length; k < kLen; k++)
      {
        var enumName = schema[i].columns[j].enumerations[k].name;
        var enumValue = schema[i].columns[j].enumerations[k].value;
        var osmTag = '<undefined>';
        var osmValue =  '<undefined>';

        if (enumValue != '-999999' && enumValue != 'noInformation')
        {
          // NOTE: Not using tColumnName here since it probably got chopped.
          if (columnName == 'ZI020_GE4')
          {
            osmTag = 'addr:country';
            osmValue = translate.convertCountryCode('urn','c2',enumValue);
          }
          else if (columnName == 'ZI020_GE42')
          {
            osmTag = 'addr:country:second';
            osmValue = translate.convertCountryCode('urn','c2',enumValue);
          }
          else if (columnName == 'ZI020_GE43')
          {
            osmTag = 'addr:country:third';
            osmValue = translate.convertCountryCode('urn','c2',enumValue);
          }
          else if (columnName == 'ZI020_GE44')
          {
            osmTag = 'addr:country:fourth';
            osmValue = translate.convertCountryCode('urn','c2',enumValue);
          }
          else if (['2','3','4','5'].indexOf(tColumnName.charAt(tColumnName.length -1)) > -1)
          {
            // Handle XXX, XXX2, XXX3 etc attributes
            tColumnName = tColumnName.slice(0,-1);
            // hoot.print('Sliced: ' + tColumnName + '  Var: ' + tds70.lookup[tColumnName][enumValue]);

            // Sanity Check
            if (! tds70.lookup[tColumnName])
            {
              hoot.print('### Missing attributes for ' + tColumnName);
            }
            osmTag = tds70.lookup[tColumnName][enumValue][0];
            osmValue = tds70.lookup[tColumnName][enumValue][1];
          }
          else
          {
            // NOTE: Using the swapped column name if applicable.
            osmTag = tds70.lookup[tColumnName][enumValue][0];
            osmValue = tds70.lookup[tColumnName][enumValue][1];
          }
        }

        hoot.print('"' + featureName + '","' + F_CODE + '","' + columnName + '","' +columnDesc + '","' + enumValue + '","' + enumName + '","' + osmTag + '","' + osmValue + '"');
      }

    } // End for j
  } // End for i

} // End dumpCsvTags



