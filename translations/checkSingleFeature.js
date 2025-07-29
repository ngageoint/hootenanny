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
// Script to check translations for a single F_CODE and attributes or translating OSM to a schema
//

var HOOT_HOME = process.env.HOOT_HOME;

transTest = require(HOOT_HOME + '/translations/checkTranslations.js');

// Set the logging level for the output
// error, warn, info, debug, trace
hoot.Log.setLogLevel("warn");

// Skip the TransportationGroundCrv type layers
hoot.Settings.set({"writer.thematic.structure":"true"});

//hoot.Settings.set({"ogr.debug.dumptags":"true"});

// LOTS of debug output
// hoot.Settings.set({"ogr.debug.dumptags":"true"});

// Set this to false to  keep  default/usless values
hoot.Settings.set({"reader.drop.defaults":"true"});

// ####################################################################################

// testTranslated:  schema, F_CODE, {attribute:value}. ['Point','Line','Area']
// testOSM:  schema, {tag:value}, ['Point','Line','Area']

// NOTE: if the geometry is not specified, the default is to try all geometries

// Schema List:
// TDSv40, TDSv61, TDSv70, TDSv71, MGCP, GGDMv30


//console.log('Just the F_CODE');
//transTest.testTranslated('TDSv71','AL013');

//console.log('Checking soil surface features');
//transTest.testTranslated('TDSv71','DA010',{'TSM':'13'});

//console.log('Checking waterway=boatyard features OSM to TDS');
//transTest.testOSM('TDSv71',{'landuse':'brownfield'});

//console.log('\nF_CODE with attributes');
//transTest.testTranslated('MGCP','AQ040',{'FUN':'6','NOS':'2','SDP':'DigitalGLobe','OSMTAGS':'{\"security:classification\":\"UNCLASSIFIED\"}'});

//console.log('\nF_CODE with default attributes');
//transTest.testTranslated('MGCP','AQ040',{'VOI':'N_A','OHB':'-32767.0','FUN':'0','NOS':'2','SDP':'DigitalGLobe','OSMTAGS':'{\"security:classification\":\"UNCLASSIFIED\"}'});

//console.log('\nF_CODE with attributes');
//transTest.testTranslated('MGCP','AL015',{'HWT':'20','FFN':'850'},['Point']);

//console.log('\nEB010 Area with names and unknown grass type');
//transTest.testTranslated('MGCP','EB010',{'NAM':'Feature Name','NFI':'NFI String','NFN':'NFN String','VEG':'0'},['Area']);

//console.log('\nEB010 Area with names and grass');
//transTest.testTranslated('MGCP','EB010',{'NAM':'Feature Name','NFI':'NFI String','NFN':'NFN String','VEG':'8'},['Area']);

//console.log('\nOSM to TDS');
//transTest.testOSM('TDSv71',{'bridge':'yes'});

console.log('\nOSM to MGCP');
transTest.testOSM('MGCP',{'aeroway':'launchpad'});
//transTest.testOSM('MGCP',{'water':'moat','natural':'water'});
//transTest.testOSM('MGCP',{'natural':'water'});

//console.log('\nOSM to GGDMv30');
//transTest.testOSM('GGDMv30',{'highway':'yes','width':'10'},['Line']);

// End
