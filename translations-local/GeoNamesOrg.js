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

// an optional initialize function that gets called once before any 
// translateAttribute calls.
function initialize()
{
    // The print method simply prints the string representation to stdout
    //print("Initializing.")
}

counts = {}

// an optional finalize function that gets called once after all
// translateAttribute calls.
function finalize()
{
    // the debug method prints to stdout when --debug has been specified on
    // the hoot command line. (DEBUG log level)
    debug("Finalizing.");
    /*
    // handy for printing out the most common entries.
    for (k in counts)
    {
        if (counts[k] > 50)
        print("['" + k + "', {'':''}],");
    }*/
}

// This is not a comprehensive set of translations but it covers some of the most common 
// translations.
classRules = [
    ['A', 'ADMD', {'place':'administrative'}],
    ['H', 'RVN', {'waterway':'ravine'}],
    ['H', 'SPNG', {'natural':'spring'}],
    ['H', 'STM', {'waterway':'stream'}],
    ['H', 'STMI', {'waterway':'stream', 'intermittent':'yes'}],
    ['H', 'WAD', {'waterway':'wadi'}],
    ['H', 'WLL', {'man_made':'water_well'}],
    ['L', 'GRAZ', {'landuse':'meadow', 'meadow':'agricultural'}],
    ['L', 'LCTY', {'place':'locality'}],
    ['L', 'PRK', {'leisure':'park'}],
    ['L', 'TRB', {'place':'tribal_area'}],
    ['P', 'PPL', {'place':'populated'}],
    // it seems like PPLX is part of a city, town, etc.
    ['P', 'PPLX', {'place':'neighbourhood'}],
    ['R', 'TRL', {'highway':'path'}],
    ['S', 'ADMF', {'building':'yes','office':'administrative'}],
    ['S', 'BANK', {'amenity':'bank'}],
    ['S', 'BDG', {'bridge':'yes'}],
    ['S', 'BLDG', {'building':'yes'}],
    ['S', 'DIP', {'amenity':'embassy'}],
    ['S', 'HTL', {'building':'hotel'}],
    ['S', 'MALL', {'shop':'mall'}],
    ['S', 'MSQE', {'amenity':'place_of_worship','religion':'muslim'}],
    ['S', 'SCH', {'building':'school'}],
    ['T', 'CLF', {'natural':'cliff'}],
    ['T', 'DPR', {'natural':'sinkhole'}],
    ['T', 'DUNE', {'natural':'dune'}],
    ['T', 'HLL', {'natural':'hill'}],
    ['T', 'ISL', {'place':'island'}],
    ['T', 'MT', {'natural':'mountain'}],
    ['T', 'MTS', {'natural':'mountain_range'}],
    ['T', 'PASS', {'mountain_pass':'yes'}],
    ['T', 'PK', {'natural':'peak'}],
    ['T', 'RDGE', {'natural':'ridge'}],
    ['T', 'RK', {'natural':'rock'}],
    ['T', 'SAND', {'natural':'sand'}],
    ['T', 'SLP', {'slope':'yes'}],
    ['T', 'SPUR', {'natural':'spur'}],
    ['T', 'TRGD', {'natural':'interdune_trough'}],
    ['T', 'UPLD', {'natural':'upland'}],
    ['V', 'FRST', {'landuse':'forest'}],
];

function applyClassRules(attrs, tags)
{
    var foundOne = false;
    for (i in classRules)
    {
        r = classRules[i];
        if (r[0] == attrs['feature_class'] && r[1] == attrs['feature_code'])
        {
            for (k in r[2])
            {
                v = r[2][k];
                tags[k] = v;
            }
            foundOne = true;
        }
    }

    if (foundOne == false)
    {
        tags['poi'] = 'yes';
    }
}

// A translateAttributes method that is very similar to the python translate 
// attributes
function translateAttributes(attrs, layerName, geometryType)
{ 
    tags = {};
    tags['name'] = attrs['name']
    altName = attrs['alternatenames'].replace(';', ';;');
    if (altName !== '')
    {
        // for some reason doing a simple search/replace wasn't working.
        altName = altName.split(',').join(';');
        tags['alt_name'] = altName;
    }

    pop = Number(attrs['population']);
    if (pop > 0)
    {
        tags['population'] = pop;
    }

    tags['uuid'] = "GeoNames.org:" + attrs['geonameid']
    
    applyClassRules(attrs, tags);
    v = attrs['feature_class'] + "', '" + attrs['feature_code'];
    tags['error:circular'] = "1000";
    /*
    // handy for printing out the most common entries.
    if (v in counts)
    {
        counts[v] = counts[v] + 1;
    }
    else
    {
        counts[v] = 1;
    }
    */
    return tags;
}

