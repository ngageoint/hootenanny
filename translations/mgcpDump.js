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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

/*
   mgcpDump.js

   This file "hijacks" either the "initialize" function to print out
   MGCP translations as a csv file.

    Usage:
    hoot convert --error \
        -D "schema.translation.script=./mgcpCheck.js"  \
        input.osm \
        output.osm > mgcp_dump.csv

    input.osm can be anything. It doesn't get translated since all of the work
    is done in the "initialize" function which is run once when the translation
    is loaded.
*/

mgcp = {}
hoot.require('mgcp_schema')
hoot.require('mgcp_rules')
hoot.require('fcode_common')
hoot.require('translate')


// The initialize function gets called once BEFORE the translateToXXX functions
// We are hijacking this function to print out documentation
function initialize()
{
    var nameList = {}
    var schema = mgcp.schema.getDbSchema();

    // Make a name lookup list
    schema.forEach( function(item) {
        if (!nameList[item.fcode]) nameList[item.fcode] = item.desc;

        item.columns.forEach( function (column) {
            if (!nameList[column.name]) nameList[column.name] = column.desc;

            if (column.type == 'enumeration') {
                column.enumerations.forEach( function (eValue) {
                    if (!nameList[column.name + ':' + eValue.value]) nameList[column.name + ':' + eValue.value] = eValue.name;
                })
            }
        });
    });

    // Debug
    // for (var i in nameList) print('nameList: ' + i + ' = ' + nameList[i]);
    // print('#####');

    if (mgcp.fcodeLookup == undefined)
    {
        // Order is important:
        // First the MGCPv3 & 4 FCODES, then the common ones. This ensures that the common ones don't
        // stomp on the other ones
        // mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,mgcp.rules.fcodeOne2oneInV3);

        // NOTE: This is ONLY the TRD4 stuff
        mgcp.rules.fcodeOne2oneV4.push.apply(mgcp.rules.fcodeOne2oneV4,fcodeCommon.one2one);

        mgcp.fcodeLookup = translate.createLookup(mgcp.rules.fcodeOne2oneV4);

        // Grungy.....
        mgcp.fcodeLookup.FCODE = mgcp.fcodeLookup.F_CODE;
        delete mgcp.fcodeLookup.F_CODE;

        // Drop the things that are not part of the TRD4 spec
        for (var i in mgcp.fcodeLookup.FCODE)
        {
            if (!nameList[i]) delete mgcp.fcodeLookup.FCODE[i];
        }

        // Debug:
        // translate.dumpOne2OneLookup(mgcp.fcodeLookup);
    }

    if (mgcp.lookup == undefined)
    {
        // NOTE: This is ONLY the TRD4 stuff
        for (var i in mgcp.rules.txtBiased) mgcp.rules.one2one.push([i,'',mgcp.rules.txtBiased[i],'']);
        for (var i in mgcp.rules.numBiased) mgcp.rules.one2one.push([i,'',mgcp.rules.numBiased[i],'']);

        mgcp.lookup = translate.createLookup(mgcp.rules.one2one);
    }

    // Now do the actual printing
    dumpCsvHeader();
    dumpCsvTags(nameList,mgcp.fcodeLookup);
    dumpCsvTags(nameList,mgcp.lookup);

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
    // Return `null` so we don't get any output
    return null;
}


function dumpCsvHeader()
{
    print('"Attribute","AttributeDescription","AttributeValue","AttributeValueDescription","OSMTag","OSMValue"');
}


function dumpCsvTags(nameList,lookup)
{
    var iList = Object.keys(lookup).sort()

    if (iList.length > 0)
    {
        for (var i = 0, fLen = iList.length; i < fLen; i++)
        {
            // print(iList[i] + ':');
            var iDesc = nameList[iList[i]];
            if (iDesc == undefined)
            {
                // print('## Skipping: ' + iList[i]);
                continue
            }

            // Funky sort function from StackOverflow that handles numbers and text
            var jList = Object.keys(lookup[iList[i]]).sort(function (a, b) {
                var reA = /[^a-zA-Z]/g;
                var reN = /[^0-9]/g;
                var aA = a.replace(reA, "");
                var bA = b.replace(reA, "");
                if(aA === bA) {
                  var aN = parseInt(a.replace(reN, ""), 10);
                  var bN = parseInt(b.replace(reN, ""), 10);
                  if(isNaN(bN) || isNaN(bN)){
                    return  a > b ? 1 : -1;
                  }
                  return aN === bN ? 0 : aN > bN ? 1 : -1;
                } else {
                 return aA > bA ? 1 : -1;
                }
             })

            if (jList.length > 0)
            {
                if (jList.length == 1)
                {
                    // print('YY' + iList[i]);
                    if (nameList[iList[i]])
                    {
                        print(iList[i] + ',' + iDesc + ',,,' + lookup[iList[i]][jList[0]]);
                    }

                    continue;
                }

                for (var j = 0, gLen = jList.length; j < gLen; j++)
                {
                    if (nameList[jList[j]])
                    {
                        print(iList[i] + ',' + iDesc + ',' + jList[j] + ',' + nameList[jList[j]] + ',' + lookup[iList[i]][jList[j]]);
                    }
                    else
                    {
                        jDesc = nameList[iList[i] + ':' + jList[j]];
                        if (jDesc == undefined)
                        {
                            // print('## Missing jDesc for ' + jList[j]);
                            continue;
                        }

                        print(iList[i] + ',' + iDesc + ',' + jList[j] + ',' + jDesc + ',' + lookup[iList[i]][jList[j]]);
                    }
                }
            }
        }
    }
} // End dumpCsvTags
