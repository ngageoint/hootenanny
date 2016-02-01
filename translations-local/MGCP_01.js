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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Initial MGCP -> OSM conversion script
//
// Based on TableExample.js script
//
// Possible attribute values are taken from GAIT20 feature types and are a combination
// of MGCP3 and MGCP4

include("Tran.js")

// create a table of nfdd biased rules.
var biased = [
    { con:"attrs['HCT'] == '-32768'", res:"tags['highway'] = 'road'" },
    { con:"attrs['MES'] == '1001'", res:"tags['divider'] = 'median'" },
    { con:"attrs['MES'] == '1001' && attrs['SEP'] == '1001'", res:"print('conflict: SEP & MES')" },
    { con:"attrs['SEP'] == '1000'", res:"tags['divider'] = 'no'" },
    { con:"attrs['SEP'] == '1001' && attrs['MES'] != '1001'", res:"tags['divider'] = 'yes'" },
    { con:"attrs['LTN'] != ''", res:"tags['lanes'] = attrs['LTN']" }
];

// build a one to one translation table.
var one2one = [
    // AP030 Roads
    ['CON', '35',    'X', 'Road Interchange'], //Road Width Constriction Type
    ['CON', '998',    'X', 'Not applicable'],
    ['FUN', '0',    'status', ''], // Condition of Facility
    ['FUN', '1',    'status', 'construction'],
    ['FUN', '2',    'status', 'abandoned'], 
    ['FUN', '3',    'status', 'destroyed'],
    ['FUN', '4',    'status', 'dismantled'],
    ['FUN', '6',    'status', ''], // Fully functional - the default
    ['FUN', '13',   'status', 'damaged'],
    ['HCT', '0',    'highway', 'road'], // Thoroughfare Class
    ['HCT', '1',    'highway', 'primary'], 
    ['HCT', '2',    'highway', 'secondary'], 
    ['HCT', '3',    'highway', 'trunk'], // National route might be Motorway
    ['HCT', '4',    'highway', 'unclassified'], 
    ['LOC', '0',    'location', ''], // Vertical Relative Location
    ['LOC', '17',    'location', 'underwater'],
    ['LOC', '40',    'location', 'underground'],
    ['LOC', '44',    'location', 'surface'], // The default is on the surface
    ['LOC', '45',    'location', 'overground'],
    ['RST', '0',    'surface', ''], // Load Bearing Surface Type
    ['RST', '1',    'surface', 'paved'], 
    ['RST', '2',    'surface', 'unpaved'], 
    ['RST', '3',    'surface', 'gravel'], // DIGEST FACC from Data: Loose/Light
    ['RST', '5',    'surface', 'grass'], // DIGEST FACC from Data: Grass/Sod
    ['RST', '6',    'surface', 'ground'], // DIGEST FACC from Data: Natural
    ['RST', '8',    'surface', ''], // Temporary
    ['WTC', '0',    'seasonal', ''], // Road Weather Restriction
    ['WTC', '1',    'seasonal', 'no'], // Road Weather Restriction
    ['WTC', '2',    'seasonal', 'fair'], // possibly seasonal=
    ['WTC', '3',    'seasonal', 'winter'],
    ['WTC', '5',    'seasonal', 'not_winter'],
    // AP010 Cart Track - Has:NAM,NFI,NFN,WD1 and WTC 
    // AP050 Trail - Has:NAM,NFI,NFN and WTC 
    // AN010 Railway - Has FUN,LOC,LTN,NAM,NFI,NFN,
    ['RGC', '0',    'railway', ''], // Railway Gauge Classification
    ['RGC', '1',    'railway', 'broad'], 
    ['RGC', '2',    'railway', 'narrow_gauge'],
    ['RGC', '3',    'railway', 'rail'],
    ['RGC', '6',    'railway', 'monorail'],
    ['RIR', '0',    'X', 'Unknown'], // Railway in Road
    ['RIR', '1000',    'X', 'Separated from road'],
    ['RIR', '1001',    'X', 'In Road'],
    ['RRA', '0',    'electrified', ''], // Railway Power Method
    ['RRA', '1',    'electrified', 'rail'],
    ['RRA', '3',    'electrified', 'contact_line'],
    ['RRA', '4',    'electrified', 'no'],
    ['RRC', '-32768',    'railway', ''], // Railway Use
    ['RRC', '0',    'railway', 'yes'],
    ['RRC', '2',    'railway', 'carline'],
    ['RRC', '6',    'railway', 'subway'],
    ['RRC', '8',    'railway', 'logging'],
    ['RRC', '14',    'railway', 'tram'],
    ['RRC', '15',    'railway', 'funicular'],
    ['RRC', '32',    'railway', 'Automated Transit System'],
    ['RRC', '33',    'railway', 'longhaul'],
    ['RRC', '999',    'railway', 'Other'],
    ['RTA', '-32768',    'X', 'Null'], // Linear Feature Arangement
    ['RTA', '0',    'X', 'Unknown'],
    ['RTA', '1',    'X', 'Single Arangement'],
    ['RTA', '2',    'X', 'Double Arangement'],
    ['RTA', '3',    'X', 'Multiple Arangements'],
    ['RTA', '4',    'X', 'Juxtaposition'],
    ['RWC', '1',    'usage', 'High Speed Rail'], //Railway Class
    ['RWC', '2',    'usage', 'main'], 
    ['RWC', '3',    'usage', 'branch'], 
    // AN050 Railway Side Track - Has FUN,GAW,NAM,NFI,NFN,RGC,RRA,
    ['RSA', '0',    'service', ''], // Branch Railway Type
    ['RTA', '1',    'service', 'spur'],
    ['RTA', '2',    'service', 'siding'],
    ['RTA', '3',    'service', 'passing'],
    // BH140 River - Has NAM,NFI,NFN
    ['CDA', '-32768',    'X', 'Null'], //Covered Drain
    ['CDA', '0',    'X', 'Unknown'], //Covered Drain
    ['CDA', '1000',    'waterway', 'drain'],
    ['CDA', '1001',    'waterway', 'covered_drain'],
    ['HYP', '0',    'intermittent', ''], // Hydrologic Persistance
    ['HYP', '1',    'intermittent', 'no'], // Perennial
    ['HYP', '2',    'intermittent', 'yes'],
    ['HYP', '4',    'intermittent', 'dry'],
    ['SHL', '-32768',    'l_bank_type', 'Null'], // Left Bank Shoreline Type
    ['SHL', '0',    'l_bank_type', ''], //Unknown
    ['SHL', '6',    'l_bank_type', 'mangrove'],
    ['SHL', '8',    'l_bank_type', 'marshy'],
    ['SHL', '10',    'l_bank_type', 'stony'],
    ['SHL', '11',    'l_bank_type', 'rubble'],
    ['SHL', '13',    'l_bank_type', 'sandy'],
    ['SHL', '14',    'l_bank_type', 'shingly'],
    ['SHR', '-32768',    'r_bank_type', 'Null'], // Right Bank Shoreline Type
    ['SHR', '0',    'r_bank_type', ''], //Unknown
    ['SHR', '6',    'r_bank_type', 'mangrove'],
    ['SHR', '8',    'r_bank_type', 'marshy'],
    ['SHR', '10',    'r_bank_type', 'stony'],
    ['SHR', '11',    'r_bank_type', 'rubble'],
    ['SHR', '13',    'r_bank_type', 'sandy'],
    ['SHR', '14',    'r_bank_type', 'shingly'],
    ['TID', '-32768',    'tidal', 'Null'], // Tide Influenced
    ['TID', '0',    'tidal', ''],
    ['TID', '1000',    'tidal', 'no'],
    ['TID', '1001',    'tidal', 'yes'],
    ['WCC', '0',    'waterway', 'yes'], // Watercourse Channel Type
    ['WCC', '1',    'waterway', 'stream'],
    ['WCC', '2',    'waterway', 'braided_Stream'],
    ['WCC', '3',    'waterway', 'gorge'],
    ['WCC', '4',    'waterway', 'wadi'],
    ['WCC', '7',    'waterway', 'river'],
    ['WID', 'X',    'X', 'X'], // Width
    ['WST', '-32768',    'X', 'Null'], // Watercourse Sink Type
    ['WST', '0',    'X', 'Unknown'],
    ['WST', '1',    'X', 'Dissipating'],
    ['WST', '2',    'X', 'Disappearing'],
    ['WST', '998',    'X', 'Not Applicable'],
    ['WST', '999',    'X', 'Other']
]; // End of one2one

// build a more efficient lookup

function createLookup(one2one)
{
    // build a more efficient lookup
    var lookup = {}
//    for (var r in one2one)
//    {
//        var row = one2one[r];
//        if (!(row[0] in lookup))
//        {
//            lookup[row[0]] = {}
//        }
//    
//        lookup[row[0]][row[1]] = [row[2], row[3]];
//    }
    for (var r in one2one)
    {
        var row = one2one[r];
        if (row[0] in lookup)
        {
            if (!lookup[row[0]][row[1]])
            {
                lookup[row[0]][row[1]] = [row[2], row[3]];
                print("Added row: " + row[0] + row[1]);
            }
            else
            {
                var tmp = row[1] + row[2] + row[3]
                print("tmp= " + tmp);
                print("lookup= " + lookup[row[0]][row[1]]);

            }
        }
        else
        {
            lookup[row[0]] = {}
            lookup[row[0]][row[1]] = [row[2], row[3]];
            print("Added row: " + row[0]);
        }
    
    }



    return lookup;
}

// A translateAttributes method that is very similar to the python translate 
// attributes
function applyOne2One(attrs, tags, lookup) 
{ 
    for (var col in attrs)
    {
        var value = attrs[col];
        if (col in lookup)
        {
            if (value in lookup[col])
            {
                row = lookup[col][value];
                tags[row[0]] = row[1];
            }
            else
            {
                throw "Lookup value not found for column. (" + col + "=" + value + ")";
            }
        }
    }
}

function applyBiased(attrs, tags, biased, layerName)
{
    for (var bi in biased)
    {
        print(attrs['SBB']);
        print(biased[bi].con);
        print(eval(biased[bi].con));
        print(biased[bi].res);
        if (eval(biased[bi].con))
        {
            print("Condition true.");
            eval(biased[bi].res);
        }
    }
}

function addName(attrs, names, col)
{
    if (attrs[col] != '' && attrs[col] != 'N_A')
    {
        n = attrs[col].replace("\\", "\\\\");
        n = attrs[col].replace(";", "\\;");
        names.push(n)
    }
}

function applyCustomRules(attrs, tags, layerName)
{
    // figure out the names
    names = [];
    addName(attrs, names, 'NAM');
    addName(attrs, names, 'NFI');
    addName(attrs, names, 'NFN');
    if (names.length >= 1)
    {
        tags['name'] = names[0];
        names.splice(0, 1);
    }
    if (names.length >= 1)
    {
        tags['alt_name'] = names.join(';');
    }
//    ['GAW', 'X',    'X', 'Railway Gauge'], // Railway Gauge
//    ['LBV', 'X',    'X', 'X'], // Left Bank Vegetation Cover
//    ['RBV', 'X',    'X', 'X'], // Right Bank Vegetation Cover
//    ['CTL', 'X',    'X', 'X'], // Cumulative Track Length
}

function fixConstruction(tags, key)
{
    if (tags['status'] == 'construction' && tags[key] != '')
    {
        tags['construction'] = tags[key];
        tags[key] = 'construction';
        delete tags['status'];
    }
}

function applyPostProcessing(tags, layerName)
{
    fixConstruction(tags, 'highway');
    fixConstruction(tags, 'railway');
}

var lookup = createLookup(one2one);

// A translateAttributes method that is very similar to the python translate 
// attributes
function translateAttributes(attrs, layerName, geometryType)
{ 
    tags = {};
    // pre processing
    applyBiased(attrs, tags, biased, layerName);
    // custom rules
    applyCustomRules(attrs, tags, layerName);
    // one 2 one
    applyOne2One(attrs, tags, lookup);
    // post processing
    applyPostProcessing(tags, layerName);

    return tags;
}
