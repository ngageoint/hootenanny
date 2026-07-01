#!/usr/bin/env node

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
 */

//
// Script to check one-off OSM tag pairing translations.
//
// Add short-lived/manual test cases below. Each test runs all geometries by
// default, matching checkSingleFeature.js behavior.
//

var tagPairing = require('./checkTagPairing.js');

function testOSM(translation, tags, geometries, options)
{
  console.log('\n' + translation + ' ' + JSON.stringify(tags));
  tagPairing.runCase({
    translation: translation,
    tags: tags,
    geometries: geometries,
    options: options
  });
}

testOSM('TDSv71', {'highway':'road','bridge':'yes'});
testOSM('TDSv71', {'highway':'road','ford':'yes'});
testOSM('TDSv71', {'highway':'road','tunnel':'yes'});
testOSM('TDSv71', {'highway':'track','bridge':'yes'});
testOSM('TDSv71', {'highway':'track','ford':'yes'});
