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

// A non-standard extension to include additional js files within the same dir
// sub-tree.
hoot.require("example")

// an optional initialize function that gets called once before any 
// translateAttribute calls.
function initialize()
{
    // The print method simply prints the string representation to stdout
    //print("Initializing.")
}

// an optional finalize function that gets called once after all
// translateAttribute calls.
function finalize()
{
    // the debug method prints to stdout when --debug has been specified on
    // the hoot command line. (DEBUG log level)
    debug("Finalizing.");
}

// A translateAttributes method that is very similar to the python translate 
// attributes
function translateAttributes(attrs, layerName, geometryType) 
{ 
    tags = {};
    //print(layerName);
    for (var key in attrs)
    {
        k = key.toLowerCase()
        //print(k + ": " + attrs[key]);
        tags[k] = attrs[key]
    }
    return tags;
}

