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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

//
// Script to check translations for a single F_CODE and attributes or translating OSM to a schema
//

var HOOT_HOME = process.env.HOOT_HOME;

transTest = require(HOOT_HOME + '/translations/checkTranslations.js');

// Skip the TransportationGroundCrv type layers
hoot.Settings.set({"ogr.thematic.structure":"true"});

// LOTS of debug output
hoot.Settings.set({"ogr.debug.dumptags":"true"});

// Debug messages from feature validation
hoot.Settings.set({"ogr.debug.dumpvalidate":"true"});

// Set this to false to  keep  default/usless values
hoot.Settings.set({"reader.drop.defaults":"false"});


// ####################################################################################

// Parameters for the call:  schema, F_CODE, {attribute:value}. ['Point','Line','Area']
// NOTE: if the geometry is not specified, the default is to try all geometries

// console.log('Just the F_CODE');
// transTest.testTranslated('TDSv71','AL013',{});

// console.log('\nF_CODE with attributes');
// transTest.testTranslated('MGCP','AQ040',{'FUN':'6','NOS':'2','SDP':'DigitalGLobe','OSMTAGS':'{\"security:classification\":\"UNCLASSIFIED\"}'});

// console.log('\nF_CODE with default attributes');
// transTest.testTranslated('MGCP','AQ040',{'VOI':'N_A','OHB':'-32767.0','FUN':'0','NOS':'2','SDP':'DigitalGLobe','OSMTAGS':'{\"security:classification\":\"UNCLASSIFIED\"}'});

// console.log('\nF_CODE with attributes');
// transTest.testTranslated('MGCP','AL015',{'HWT':'20','FFN':'850'},['Point']);

console.log('\nF_CODE with attributes');
transTest.testTranslated('GGDMv30','BA010',{'SLT':'6'},['Line']);

// transTest.testTranslated('MGCP','AL015',{'HWT':'20','FFN':'850'});

// Parameters for the call:  schema, {tag:value}
// console.log('\nOSM Tags');
// transTest.testOSM('MGCP',{'poi':'yes','amenity':'cafe','uuid':'{4632d15b-7c44-4ba1-a0c4-8cfbb30e39d4}',});

// End

