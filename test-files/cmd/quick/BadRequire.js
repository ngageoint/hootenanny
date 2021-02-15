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
 * @copyright Copyright (C) 2014, 2015 Maxar (http://www.maxar.com/)
 */
//
// Test Script
//

// hoot.require('tds');
hoot.require('BadSyntaxTest');

// an optional initialize function that gets called once before any
// translateToOsm calls.
function initialize()
{
    print("Initialize works!");
    // log("TestAdd: " + testAdd(2,5));
    // print("TimeNow = " + timeNow());
    // print("HootConfig = " + getHootConfig("smurf"));
    // print("HootConfig = " + getHootConfig("ogr.debug.dumptags"));
    // print("GetTest = " + hoot.get("ogr.debug.dumptags"));

    // hoot.debug("Debug.");
    // hoot.logDebug("LogDebug.");
    // hoot.logInfo("LogInfo.");
    // hoot.logWarn("LogWarn.");
    // hoot.logError("LogError.");
    // hoot.logFatal("LogFatal.");

    /*
    // Script test
    print('t1: ' + smurf());;
    print('t1: ' + rules.smurf2("Yippee"));;
    print('t2: ' + rules.fcode1);;
    print('t2: ' + rules.fcode1[2]);;
    print('t3: ' + rules.tList['name']);;
    */

    // Translate Test
    var t1 = 'papa';
    if (translate !== undefined)
    {
        print('appendValue: ' + translate.appendValue(t1,'smurf',';'));
    }
}
