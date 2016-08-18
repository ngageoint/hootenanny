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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// The NodeJs version of TDS61 Conversion - With "English" output
// This is for testing only at the moment.
//


// NodeJs Function
global.toEnglish = function(tags, geometryType)
{
    if (typeof SchemaTools === 'undefined') hoot.require('SchemaTools');
    if (typeof tds61 === 'undefined') hoot.require('tds61');
    if (typeof tds61_schema === 'undefined') hoot.require('tds61_schema');
    if (typeof tds61_rules === 'undefined') hoot.require('tds61_rules');
    if (typeof fcode_common === 'undefined')hoot.require('fcode_common');
    if (typeof tds61_schema === 'undefined')hoot.require('tds61_schema');
    if (typeof etds61 === 'undefined') hoot.require('etds61');
    if (typeof etds61_rules === 'undefined') hoot.require('etds61_rules');
    if (typeof config === 'undefined') hoot.require('config');
    if (typeof translate === 'undefined') hoot.require('translate');

    // Turn off the TDS structure so we just get the raw feature
    hoot.Settings.set({"ogr.tds.structure":"false"});

    // Turn on partial translations
    hoot.Settings.set({"ogr.partial.translate":"true"});

    // Turn off the tds extra function
    hoot.Settings.set({"ogr.tds.extra":"false"});


    return etds61.toEnglish(tags, '', geometryType)
} // End of toEnglish


