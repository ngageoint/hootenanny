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

// create a table of nfdd biased rules.
var nfddBiased = [
    { condition:"attrs['SBB'] == '995'", consequence:"tags['bridge'] = 'yes'" }
];

// build a one to one translation table.
var one2one = [
    ['ROC', '1',    'surface',  'ground'],
    ['ROC', '2',    'surface',  'unimproved'],
    ['WTC', '1',    'all_weather', 'yes'],
    ['WTC', '2',    'all_weather', 'fair']
];

// build a more efficient lookup
var lookup = {}
for (var r in one2one)
{
    var row = one2one[r];
    if (!(row[0] in lookup))
    {
        lookup[row[0]] = {}
    }

    lookup[row[0]][row[1]] = [row[2], row[3]];
}

// A translateAttributes method that is very similar to the python translate 
// attributes
function translateAttributes(attrs, layerName, geometryType)
{ 
    var tags = {};

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
        else
        {
            for (var bi in nfddBiased)
            {
                print(attrs['SBB']);
                print(nfddBiased[bi].condition);
                print(eval(nfddBiased[bi].condition));
                print(nfddBiased[bi].consequence);
                if (eval(nfddBiased[bi].condition))
                {
                    print("Condition true.");
                    eval(nfddBiased[bi].consequence);
                }
            }
        }
    }
    return tags;
}

