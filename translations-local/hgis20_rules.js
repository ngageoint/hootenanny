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
    HGISv20 One2one rules
*/

hgis20.rules = {
    // ##### Start of One2One Rules #####

    // One2one rules for Text Fields
    txtBiased : {
     'NAME':'name', // Geographic Name Information : Full Name

     'COMMENTS':'note', // Note : Memorandum
     'SOURCE':'source:description', // Source Information : Source Description
     'SOURCE2':'source:description:2', // Source Information : Source Description
     'SOURCE_DT':'source:datetime', // Source Information : Source Date and Time
     'SOURCE2_DT':'source:datetime:2', // Source Information : Source Date and Time
     }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
     }, // End numBiased
    

    // Common one2one rules. Used for both import and export
    // taken from the HGISv20 spec.
    one2one : [
     // DSG - From Geonames
     ['DSG','ADM1','geonames:dsg','a'], // Country
     ['DSG','ADM2','geonames:dsg','a'], // Country
     ['DSG','PPL','geonames:dsg','a'], // Country
     ['DSG','PPLA','geonames:dsg','a'], // Country
     ['DSG','PPLA2','geonames:dsg','a'], // Country
     ['DSG','PPLC','geonames:dsg','a'], // Country
     ['DSG','PPLL','geonames:dsg','a'], // Country
     ['DSG','PPLW','geonames:dsg','a'], // Country
     ['DSG','PPLX','geonames:dsg','a'], // Country

     // FC - From Geonames
     ['FC','A','geonames:feature_class','a'], // Country, state, region
     ['FC','H','geonames:feature_class','h'], // stream, lake
     ['FC','L','geonames:feature_class','l'], // parks, area
     ['FC','P','geonames:feature_class','p'], // City, village
     ['FC','R','geonames:feature_class','r'], // Road, railway
     ['FC','S','geonames:feature_class','s'], // spot, building, farm
     ['FC','T','geonames:feature_class','t'], // mountain, hill, rock
     ['FC','U','geonames:feature_class','u'], // Undersea
     ['FC','V','geonames:feature_class','v'], // forest, heath



    // TYPE is shared by a lot of features
     ['TYPE','999999','raw:TYPE','999999'],
     ['TYPE','Administrative Facility','raw:TYPE','Administrative Facility'],
     ['TYPE','Air Force','raw:TYPE','Air Force'],
     ['TYPE','Armed Robbery','raw:TYPE','Armed Robbery'],
     ['TYPE','Army','raw:TYPE','Army'],
     ['TYPE','Assault','raw:TYPE','Assault'],
     ['TYPE','Beach','raw:TYPE','Beach'],
     ['TYPE','Botanic Garden','raw:TYPE','Botanic Garden'],
     ['TYPE','Bridleway','raw:TYPE','Bridleway'],
     ['TYPE','Burial Site','raw:TYPE','Burial Site'],
     ['TYPE','Bus Guideway','raw:TYPE','Bus Guideway'],
     ['TYPE','Casino','raw:TYPE','Casino'],
     ['TYPE','Church','raw:TYPE','Church'],
     ['TYPE','Coal','raw:TYPE','Coal'],
     ['TYPE','Coast Guard','raw:TYPE','Coast Guard'],
     ['TYPE','Combustion','raw:TYPE','Combustion'],
     ['TYPE','Construction','raw:TYPE','Construction'],
     ['TYPE','Consulate','raw:TYPE','Consulate'],
     ['TYPE','Corruption','raw:TYPE','Corruption'],
     ['TYPE','Cultural','raw:TYPE','Cultural'],
     ['TYPE','Cultural Center','raw:TYPE','Cultural Center'],
     ['TYPE','Customs','raw:TYPE','Customs'],
     ['TYPE','Cycleway','raw:TYPE','Cycleway'],
     ['TYPE','Desert','raw:TYPE','Desert'],
     ['TYPE','Diesel','raw:TYPE','Diesel'],
     ['TYPE','Docking','raw:TYPE','Docking'],
     ['TYPE','Drug Smuggling','raw:TYPE','Drug Smuggling'],
     ['TYPE','Electric','raw:TYPE','Electric'],
     ['TYPE','Electricity','raw:TYPE','Electricity'],
     ['TYPE','Electricity, Water Supply','raw:TYPE','Electricity, Water Supply'],
     ['TYPE','Embassy','raw:TYPE','Embassy'],
     ['TYPE','Extortion','raw:TYPE','Extortion'],
     ['TYPE','Fire Station','raw:TYPE','Fire Station'],
     ['TYPE','Footway','raw:TYPE','Footway'],
     ['TYPE','Ford','raw:TYPE','Ford'],
     ['TYPE','Foreign Mission','raw:TYPE','Foreign Mission'],
     ['TYPE','Fuel Smuggling','raw:TYPE','Fuel Smuggling'],
     ['TYPE','Gallery','raw:TYPE','Gallery'],
     ['TYPE','Game Reserve','raw:TYPE','Game Reserve'],
     ['TYPE','Gas','raw:TYPE','Gas'],
     ['TYPE','General','raw:TYPE','General'],
     ['TYPE','Generator','raw:TYPE','Generator'],
     ['TYPE','Geothermal','raw:TYPE','Geothermal'],
     ['TYPE','Goods','raw:TYPE','Goods'],
     ['TYPE','Gorge','raw:TYPE','Gorge'],
     ['TYPE','Harbor','raw:TYPE','Harbor'],
     ['TYPE','Hijack','raw:TYPE','Hijack'],
     ['TYPE','Historical','raw:TYPE','Historical'],
     ['TYPE','Human','raw:TYPE','Human'],
     ['TYPE','Hydroelectricity','raw:TYPE','Hydroelectricity'],
     ['TYPE','IDP Camp','raw:TYPE','IDP Camp'],
     ['TYPE','IED','raw:TYPE','IED'],
     ['TYPE','Inland Water','raw:TYPE','Inland Water'],
     ['TYPE','Irrigated Area','raw:TYPE','Irrigated Area'],
     ['TYPE','Irrigation','raw:TYPE','Irrigation'],
     ['TYPE','Irrigation, Electricity, Water Supply','raw:TYPE','Irrigation, Electricity, Water Supply'],
     ['TYPE','Irrigation, Water Supply','raw:TYPE','Irrigation, Water Supply'],
     ['TYPE','Jetty','raw:TYPE','Jetty'],
     ['TYPE','Juvenile','raw:TYPE','Juvenile'],
     ['TYPE','Kidnapping','raw:TYPE','Kidnapping'],
     ['TYPE','Lagoon','raw:TYPE','Lagoon'],
     ['TYPE','Lake','raw:TYPE','Lake'],
     ['TYPE','Landmark','raw:TYPE','Landmark'],
     ['TYPE','Land Subject to Inundation','raw:TYPE','Land Subject to Inundation'],
     ['TYPE','Levee','raw:TYPE','Levee'],
     ['TYPE','Library','raw:TYPE','Library'],
     ['TYPE','Lighthouses & Beacons','raw:TYPE','Lighthouses & Beacons'],
     ['TYPE','Living Street','raw:TYPE','Living Street'],
     ['TYPE','Lock','raw:TYPE','Lock'],
     ['TYPE','Magazine','raw:TYPE','Magazine'],
     ['TYPE','Marina','raw:TYPE','Marina'],
     ['TYPE','Marines','raw:TYPE','Marines'],
     ['TYPE','Memorial','raw:TYPE','Memorial'],
     ['TYPE','Mission','raw:TYPE','Mission'],
     ['TYPE','Monastery','raw:TYPE','Monastery'],
     ['TYPE','Monument','raw:TYPE','Monument'],
     ['TYPE','Mosque','raw:TYPE','Mosque'],
     ['TYPE','Motorway','raw:TYPE','Motorway'],
     ['TYPE','Motorway Link','raw:TYPE','Motorway Link'],
     ['TYPE','Mountain','raw:TYPE','Mountain'],
     ['TYPE','Murder','raw:TYPE','Murder'],
     ['TYPE','Museum','raw:TYPE','Museum'],
     ['TYPE','Narcotics/Drugs','raw:TYPE','Narcotics/Drugs'],
     ['TYPE','National','raw:TYPE','National'],
     ['TYPE','National Park','raw:TYPE','National Park'],
     ['TYPE','Navy','raw:TYPE','Navy'],
     ['TYPE','Newspaper','raw:TYPE','Newspaper'],
     ['TYPE','Nuclear','raw:TYPE','Nuclear'],
     ['TYPE','Oil','raw:TYPE','Oil'],
     ['TYPE','Oil/Fuel','raw:TYPE','Oil/Fuel'],
     ['TYPE','Other','raw:TYPE','Other'],
     ['TYPE','Outdoor/Adventure','raw:TYPE','Outdoor/Adventure'],
     ['TYPE','Park','raw:TYPE','Park'],
     ['TYPE','Path','raw:TYPE','Path'],
     ['TYPE','Paved','raw:TYPE','Paved'],
     ['TYPE','Peak','raw:TYPE','Peak'],
     ['TYPE','Pedestrian','raw:TYPE','Pedestrian'],
     ['TYPE','Pier','raw:TYPE','Pier'],
     ['TYPE','Piracy','raw:TYPE','Piracy'],
     ['TYPE','Poaching','raw:TYPE','Poaching'],
     ['TYPE','Police Station','raw:TYPE','Police Station'],
     ['TYPE','Ponds','raw:TYPE','Ponds'],
     ['TYPE','Pool','raw:TYPE','Pool'],
     ['TYPE','Precious Minerals','raw:TYPE','Precious Minerals'],
     ['TYPE','Preserve/Refuge','raw:TYPE','Preserve/Refuge'],
     ['TYPE','Primary','raw:TYPE','Primary'],
     ['TYPE','Primary Link','raw:TYPE','Primary Link'],
     ['TYPE','Raceway','raw:TYPE','Raceway'],
     ['TYPE','Radio','raw:TYPE','Radio'],
     ['TYPE','Refugee Camp','raw:TYPE','Refugee Camp'],
     ['TYPE','Refugee Location','raw:TYPE','Refugee Location'],
     ['TYPE','Refugee Return','raw:TYPE','Refugee Return'],
     ['TYPE','Refugee Settlement','raw:TYPE','Refugee Settlement'],
     ['TYPE','Regional','raw:TYPE','Regional'],
     ['TYPE','Reservoir','raw:TYPE','Reservoir'],
     ['TYPE','Residential','raw:TYPE','Residential'],
     ['TYPE','Rest Area','raw:TYPE','Rest Area'],
     ['TYPE','River','raw:TYPE','River'],
     ['TYPE','Robbery','raw:TYPE','Robbery'],
     ['TYPE','Sculpture','raw:TYPE','Sculpture'],
     ['TYPE','Secondary','raw:TYPE','Secondary'],
     ['TYPE','Secondary Link','raw:TYPE','Secondary Link'],
     ['TYPE','Security Force','raw:TYPE','Security Force'],
     ['TYPE','Security Station','raw:TYPE','Security Station'],
     ['TYPE','Service','raw:TYPE','Service'],
     ['TYPE','Ship Yard','raw:TYPE','Ship Yard'],
     ['TYPE','Shrine','raw:TYPE','Shrine'],
     ['TYPE','Smuggling Hub','raw:TYPE','Smuggling Hub'],
     ['TYPE','Solar Panel','raw:TYPE','Solar Panel'],
     ['TYPE','Solitary','raw:TYPE','Solitary'],
     ['TYPE','Spa','raw:TYPE','Spa'],
     ['TYPE','Special Operations Force','raw:TYPE','Special Operations Force'],
     ['TYPE','Sports Facility','raw:TYPE','Sports Facility'],
     ['TYPE','Statue','raw:TYPE','Statue'],
     ['TYPE','Steps','raw:TYPE','Steps'],
     ['TYPE','Substation','raw:TYPE','Substation'],
     ['TYPE','Television','raw:TYPE','Television'],
     ['TYPE','Temple','raw:TYPE','Temple'],
     ['TYPE','Tertiary','raw:TYPE','Tertiary'],
     ['TYPE','Tertiary Link','raw:TYPE','Tertiary Link'],
     ['TYPE','Theater','raw:TYPE','Theater'],
     ['TYPE','Theatre','raw:TYPE','Theatre'],
     ['TYPE','Theft','raw:TYPE','Theft'],
     ['TYPE','Theme Park','raw:TYPE','Theme Park'],
     ['TYPE','Track','raw:TYPE','Track'],
     ['TYPE','Transformer','raw:TYPE','Transformer'],
     ['TYPE','Transmission Pole','raw:TYPE','Transmission Pole'],
     ['TYPE','Transmission Tower','raw:TYPE','Transmission Tower'],
     ['TYPE','Trunk','raw:TYPE','Trunk'],
     ['TYPE','Trunk Link','raw:TYPE','Trunk Link'],
     ['TYPE','Unclassified','raw:TYPE','Unclassified'],
     ['TYPE','United Nations','raw:TYPE','United Nations'],
     ['TYPE','Unpaved','raw:TYPE','Unpaved'],
     ['TYPE','Urban Refugee Area','raw:TYPE','Urban Refugee Area'],
     ['TYPE','Viewpoint','raw:TYPE','Viewpoint'],
     ['TYPE','Village of Interest','raw:TYPE','Village of Interest'],
     ['TYPE','Violence','raw:TYPE','Violence'],
     ['TYPE','Volcano','raw:TYPE','Volcano'],
     ['TYPE','Waterfall','raw:TYPE','Waterfall'],
     ['TYPE','Water Park','raw:TYPE','Water Park'],
     ['TYPE','Water Supply','raw:TYPE','Water Supply'],
     ['TYPE','Weapons','raw:TYPE','Weapons'],
     ['TYPE','Weapons Smuggling','raw:TYPE','Weapons Smuggling'],
     ['TYPE','Wetlands','raw:TYPE','Wetlands'],
     ['TYPE','Wind','raw:TYPE','Wind'],
     ['TYPE','Yearbook','raw:TYPE','Yearbook'],
     ['TYPE','Youth Center','raw:TYPE','Youth Center'],
     ['TYPE','Zoo','raw:TYPE','Zoo'],


     // SPA_ACC - Spatial Accuracy
     ['SPA_ACC','1 - High','source:accuracy','high'],
     ['SPA_ACC','2 - Medium','source:accuracy','medium'],
     ['SPA_ACC','3 - Low','source:accuracy','low'],
     ['SPA_ACC','4 - Regional','source:accuracy','regional'],
     ['SPA_ACC','5 - Country','source:accuracy','country'],

     ], // End one2one

     // Input Translation Rules:
     // These are only used for IMPORT and get added to the standard one2one rules
     // This is mainly for TDS 3.0 attributes but we have added some "funky" TDSv40 attributes as well.
     one2oneIn : [
     // To account for an "error" in the TDSv61 sample GDB
     ['ZI001_VSC','Stereoscopic Imagery','source:vertical_source:type','stereoscopic_imagery'],
     ['ZI001_SRT','NTM Imagery','source:non_spatial_source:type','ntm_imagery'],
    ], // End one2oneIn

    // One2one translation table for converting "Other" OSM attributes to NFDD
    // This is for Export only. The values are swapped before use
    one2oneOut : [

   ], // End one2oneOut

    // ##### End of One2One Rules #####

} // End of hgis20.rules
