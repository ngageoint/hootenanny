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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

var __extension__ = "";

var hoot = {};

hoot.require = function(param)
{
}

function require(param)
{	

}

function __setupPackage__(param)
{
}

function getHootConfig(param)
{
  return false;
}

function createUuid()
{// temporary should be replaced by java code
  /*  var s = [];
    var hexDigits = "0123456789ABCDEF";
    for (var i = 0; i < 32; i++) {
        s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
    }
   
    // bits 12-15 of the time_hi_and_version field to 0010
    s[12] = "4"; 
    // bits 6-7 of the clock_seq_hi_and_reserved to 01
    s[16] = hexDigits.substr((s[16] & 0x3) | 0x8, 1); 
    var uuid = s.join("");
    return uuid;*/
  return "" + java.util.UUID.randomUUID().toString();
}

function logError(msg)
{
}

function logWarn(msg)
{
}
