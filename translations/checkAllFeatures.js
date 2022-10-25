#!/usr/bin/node

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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

//
// Script to check translations
// checkAllFeatures & checkSingleFeature are basically the same. Having both makes testing easier
//

var HOOT_HOME = process.env.HOOT_HOME;

// Library functions
transTest = require(HOOT_HOME + '/translations/checkTranslations.js');

// Skip the TransportationGroundCrv type layers
hoot.Settings.set({"ogr.thematic.structure":"false"});

// LOTS of debug output
// hoot.Settings.set({"ogr.debug.dumptags":"true"});
hoot.Settings.set({"ogr.debug.dumpvalidate":"true"});


// ####################################################################################
// The map of schema to test
var schemaMap = {
  // DNC: require(HOOT_HOME + '/translations/dnc_schema.js'),
  // ENCv311: require(HOOT_HOME + '/translations/encv311_schema.js'),
  MGCP: require(HOOT_HOME + '/translations/mgcp_schema.js'),
  // GGDMv30: require(HOOT_HOME + '/translations/ggdm30_full_schema.js'),
  // TDSv40: require(HOOT_HOME + '/translations/tds40_full_schema.js'),
  // TDSv61: require(HOOT_HOME + '/translations/tds61_full_schema.js'),
  // TDSv70: require(HOOT_HOME + '/translations/tds70_full_schema.js'),
  // TDSv71: require(HOOT_HOME + '/translations/tds71_full_schema.js')
};

// Test a single schema
// schemaMap = {
//   TDSv70: require(HOOT_HOME + '/translations/tds70_full_schema.js'),
// }

// Test every fcode in a schema
// transTest.testSchema(schemaMap);

// ####################################################################################
// Dump values from a schema
// This dumps out what F_CODE has an attribute and if it is an enumeration, the consolidated list
// transTest.dumpValues(schemaMap.MGCP,'FFN');
// transTest.dumpValues(schemaMap.MGCP,'WID');


// getTranslations, getCapabilities,
var tList = transTest.getThing('translations');
console.log('\nTransList: ' + JSON.stringify(tList));


tList = transTest.getThing('capabilities');
console.log('\nCapabilities: ' + JSON.stringify(tList));


tList = transTest.getThing('version');
console.log('\nVersion: ' + JSON.stringify(tList));

process.exit();

// ####################################################################################
// List of geometries to test
var geomList = ['Point','Line','Area'];

// All of the schema to test
var schemaList = ['MGCP','DNC','ENCv311','TDSv40','TDSv61','TDSv70','TDSv71','GGDMv30'];

// List of F_CODES to test
// NOTE: "smurf" should send back an error
var fCodeList = [
// 'AA012',
  'smurf',
  'BB082',
  'AL015',
  'AL013',
  // 'FA015',
  // 'BH051',
  // 'BH020',
  // 'AL260',
  // 'AJ051',
  // 'GB055',
  // 'AK130',
  // 'GB050',
  // 'DB160',
  // 'AT005','AT060',
  // 'AQ125',
  // 'AM030',
  // 'AL208',
  // 'AL200',
  // 'AL105',
  // 'AL012',
  // 'AD010'
  ];

// Test a single F_CODE
transTest.testF_CODE(fCodeList,schemaList,geomList);

// End
