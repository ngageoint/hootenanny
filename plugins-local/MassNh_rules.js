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

/*
        MassNh rules
*/

MassNh.rules = {
    // ##### Start of One2One Rules #####

    // One2one rules for Text Fields
    txtBiased : {
     'ADDRESS':'addr:full', // Address
     'NAME':'name',
     'STREET_NAME':'name',
     'STREET':'name',
     'STATION':'name',
     'TRAILNAME':'name',
     }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
     'NUMBEROFTR':'lanes', // Track or Lane Count
     'NUM_LANES':'lanes', // Track or Lane Count
     }, // End numBiased
    

    // Common one2one rules. Used for both import and export
    one2one : [
     // Road class
     ['CLASS','1','highway','motorway'],
     ['CLASS','2','highway','trunk'],
     ['CLASS','3','highway','primary'],
     ['CLASS','4','highway','Secondary'],
     ['CLASS','5','highway','tertiary'],
     ['CLASS','6','highway','unclassified'],
     ['CLASS','7','highway','track'],
     ['CLASS','8','highway','path'],

     ['FUNCT_CL_1','Urban - Principal Arterial -- Other Freeways and Expressways','highway','motorway'],
     ['FUNCT_CL_1','Urban - Principal Arterial -- Other','highway','primary'],
     ['FUNCT_CL_1','Urban - Principal Arterial -- Interstate','highway','motorway'],
     ['FUNCT_CL_1','Urban - Minor Arterial','highway','secondary'],
     ['FUNCT_CL_1','Urban - Local -- Public','highway','residential'],
     ['FUNCT_CL_1','Urban - Collector','highway','tertiary'],

     ['FUNCT_CL_1','Rural - Major Collector','highway','secondary'],
     ['FUNCT_CL_1','Rural - Minor Collector','highway','residential'],
     ['FUNCT_CL_1','Rural - Minor Arterial','highway','primary'],
     ['FUNCT_CL_1','Rural - Local - Public','highway','residential'],
     ['FUNCT_CL_1','Rural - Principal Arterial -- Interstate','highway','motorway'],
     ['FUNCT_CL_1','Rural - Principal Arterial -- Other','highway','primary'],

     ['FUNCT_CL_1','Non-Public Roads','highway','residential'],

     // Road type from Mass
     ['RDTYPE','7','link_road','yes'], // Ramp
     ['RDTYPE','8','tunnel','yes'], // Tunnel
     ['RDTYPE','9','tunnel','yes'], // Tunnel
     ['RDTYPE','10','tunnel','yes'], // Tunnel

     // Railway Use
     ['RRA','1','railway','rail'], // Railroad
     ['RRA','2','railway','rail'], // Cog
     ['RRA','5','railway','abandoned'], // Abandoned
     ['RRA','6','railway','Other'], // Other

     // TYPE gets used in a few files
     // Trains
     ['TYPE','1','railway','rail'],
     ['TYPE','2','railway','rail'],
     ['TYPE','3','railway','abandoned'],
     ['TYPE','4','railway','abandoned'],
     ['TYPE','5','railway','yes'],
     ['TYPE','6','railway','disused'],
     ['TYPE','7','railway','abandoned'],
     ['TYPE','8','railway','abandoned'],
     // ['TYPE','9','railway','rapid_transit'], // Covered by RT_CLASS

     // RT_CLASS - Rapid Transit Class
     ['RT_CLASS','1','railway','rapid_transit'],
     ['RT_CLASS','2','railway','light_rail'],
     ['RT_CLASS','3','railway','light_rail'], // RIR
     ['RT_CLASS','4','railway','rapid_transit'], // Elevated
     ['RT_CLASS','6','railway','rapid_transit'], // Open Cut
     ['RT_CLASS','7','railway','subway'],

     // One-way
     ['STREETOPR','1','oneway','yes'],
     ['STREETOPR','2','oneway','no'],
     ['DIRECTION','One way','oneway','yes'],
     ['DIRECTION','Two way','oneway','no'],

     // Toll Road
     ['TOLLROAD','1','toll','yes'],
     ['TOLLROAD','2','toll','yes'],
     ['IS_TOLL','YES','toll','yes'],

     // SurfaceType -> ZI016_ROC - Route Pavement Information : Route Surface Composition
     ['SURFACETYP','1','surface','ground'], // Unimproved
     ['SURFACETYP','2','surface','gravel'], // Aggregate
     ['SURFACETYP','4','surface','cobblestone'], // Cobble-stone
     ['SURFACETYP','5','surface','asphalt'], // Asphalt
     ['SURFACETYP','6','surface','asphalt_over_concrete'], // Asphalt over Concrete
     ['SURFACETYP','7','surface','concrete'], // Concrete
     ['SURFACETYP','8','surface','flexible_pavement'], // Flexible Pavement
     ['SURFACETYP','9','surface','ridgid_pavement'], // Rigid Pavement
     ['SURFACETYP','10','surface','other'], // Other - in data but not in spec
     ['SURF_TYPE','Paved','surface','asphalt'], // Asphalt
     ['SURF_TYPE','Unpaved','surface','unpaved'], // Stabilized

     // STRUCTURAL -> PCF - Physical Condition
     ['STRUCTURAL','1','condition','functional'], // Good
     ['STRUCTURAL','2','condition','functional'], // Fair
     ['STRUCTURAL','3','condition','damaged'], // Deficient
     ['STRUCTURAL','4','condition','damaged'], // Intolerable ?????

     // Religious Denomination
     ['DENOM','ABC','denomination','baptist'],
     ['DENOM','BAPT','denomination','baptist'],
     ['DENOM','CHRISTIAN','denomination','christian'],
     ['DENOM','CRC','denomination','reformed'],
     ['DENOM','EPIS','denomination','episcopal'],
     // ['DENOM','LIBERAL','denomination','liberal'],
     // ['DENOM','NON_SECTARIAN','denomination',''],
     ['DENOM','PCA','denomination','damaged'],
     ['DENOM','REFORM','denomination','reformed'],
     ['DENOM','UCC','denomination','united_church_of_christ'],
     // ['DENOM','ZEN BUDDHIST MASTER THICH NHAT HAHN','denomination','damaged'],
     ['CATH','Y','denomination','catholic'],


    ], // End one2one

    // ##### End of One2One Rules #####

} // End of MassNh.rules
