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
    UTP One2one rules
*/


utp.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    fcodeOne2oneIn : [
     ['F_CODE','AA012','landuse','quarry'], // Quarry - NFDD AA010
     ['F_CODE','AA050','man_made','water_well'], // Water Well  - NFDD BH230
     ['F_CODE','AC000','facility','yes'], // Processing Facility - NFDD AL010
     ['F_CODE','AH050','site:type','fortification'], // Fortification - FCODE Retired
     ['F_CODE','AL015',undefined,undefined], // Building - NFDD AL013 - gets rid of warnings
     ['F_CODE','AL121','military','anti_aircraft_artillery'], // Anti Aircraft Artillery Site (DFDD) - NFDD ???? 
     ['F_CODE','AL240','man_made','tower'], // Tower - NFDD AL241
     ['F_CODE','AQ064','man_made','causeway'], // Causeway - NFDD AQ063
     ['F_CODE','AT030','power','line'], // Power Line - NFDD AT005
     ['F_CODE','AT040','pylon','yes'], // Pylon - NFDD AT042
     ['F_CODE','AT050','use','communication'], // Communication Building - No code
     ['F_CODE','AT080','tower:type','communication'], // Communication Tower - NFDD AL241
     ['F_CODE','BB190','waterway','dock'], // Pier/Wharf/Quay - NFDD BB081
     ['F_CODE','BB042','man_made','mole'], // Mole - NFDD BB081
     ['F_CODE','BH080','water','lake'], // Lake/Pond - NFDD BH082
     ['F_CODE','BH130','water','reservoir'], // Reservoir - NFDD BH082
     ['F_CODE','EC030','landuse','forest'], // Trees - NFDD EC015
     ['F_CODE','IE020','poi','yes'], // Miscellaneous - No code
     ['F_CODE','SU003','building','port_facility'], // Port Facility - NFDD BB009 Port is not a good fit
     ['F_CODE','ZD019','poi','yes'], // General Miscellaneous Feature - No code
     ], // End fcodeInOne2one

    // One2one rules for Text attributes
    txtBiased : {
     'NAME':'name', // Name
     'NAM':'name', // Name
     'CITY':'is_in:city',
     'CNTRY_FULL':'is_in:country',
     'COMMENTS':'note', // Comments
     'ISO_3116_2':'is_in:country_code',
     'ISO_3116_3':'is_in:country_code3', // Save the three letter country code
     'ROUTE':'ref', // Route Designation
     'ROUTE_NUM':'ref', // Route Number?
     'RTN':'ref', // Route Number?
     'SOURCE':'source:name', // Source
     'PUB_DATE':'source:creation_date', // 
     }, // End txtBiased

    // One2one rules for Number attributes
    numBiased : {
     'BNF':'building:levels', // Floor Count
     'FLOORS':'building:levels', // Floor Count
     'ELEVATION':'height:max', // Highest Elevation
     'HEIGHT':'height', // Height Above Surface Level
     'HGT':'height', // Height Above Surface Level
     'HIGH_Z_M':'height:max', // Height Above Surface Level
     'LANES':'lanes', // Track or Lane Count
     'LEN':'length', // Length
     'LENG':'length', // Length
     'LENGTH':'length', // Length
     'LENGTH_M':'length', // Length
     'LINES':'cable_count', // Number of parallel lines
     'LTN':'lanes', // Track or Lane Count
     'NUMOFLINES':'cable_count', // Number of parallel lines
     'NPL':'cable_count', // Number of parallel lines
     'TRCKLNNUM':'lanes', // Track or Lane Count
     'TRKS_LANES':'lanes', // Track or Lane Count
     'WID':'width', // Width
     'WIDTH':'width', // Width
     'WIDTH_M':'width', // Width ??
     'ZV2':'ele' // Height Above Datum
     }, // End numBiased


    // Common one2one rules. Used for both import and export
    one2one : [
     // SSR - Roof Shape
     // ['SSR','-999999',undefined,undefined], // No Information
     ['ROOF_SHAPE','Conical/Peaked/NUN','roof:shape','conical'],
     ['ROOF_SHAPE','Conical/Peaked/NUN (6)','roof:shape','conical'],
     ['ROOF_SHAPE','Dome','roof:shape','domed'], // Domed
     ['ROOF_SHAPE','Dome (40)','roof:shape','domed'], // Domed
     ['ROOF_SHAPE','Flat','roof:shape','flat'], // Flat
     ['ROOF_SHAPE','Flat (41)','roof:shape','flat'], // Flat
     ['ROOF_SHAPE','Gable (Pitched)','roof:shape','pitched'], // Pitched
     ['ROOF_SHAPE','Gable (Pitched) (42)','roof:shape','pitched'], // Pitched
     ['ROOF_SHAPE','Curved/Round (Quonset)','roof:shape','semi-cylindrical'],
     ['ROOF_SHAPE','Curved/Round (Quonset) (38)','roof:shape','semi-cylindrical'],
     ['ROOF_SHAPE','Sawtooth (47)','roof:shape','sawtoothed'],
     ['ROOF_SHAPE','Other (999)','roof:shape','other'],

     // BSS - Building Superstructure
     ['BSS','Cupola','building:superstructure:type','cupola'],
     ['BSS','Cupola (3)','building:superstructure:type','cupola'],
     ['BSS','Minaret','building:superstructure:type','minaret'], //
     ['BSS','Minaret (6)','building:superstructure:type','minaret'],
     ['BSS','Minaret (6) (6)','building:superstructure:type','minaret'],
     ['BSS','Monitor','building:superstructure:type','monitor'], // Not great 
     ['BSS','Steeple','building:superstructure:type','steeple'],
     ['BSS','Steeple (2)','building:superstructure:type','steeple'],
     ['BSS','Tower','building:superstructure:type','tower'],
     ['BSS','Tower (5)','building:superstructure:type','tower'],
     ['BSS','Turret','building:superstructure:type','turret'],
     ['BSS','Other','building:superstructure:type','other'],
     ['BSS','Other (999)','building:superstructure:type','other'],

     // BSC - Bridge Structure Type
     // ['BSC','-999999',undefined,undefined], // No Information
     ['BSC','Arch (assume open spandrel)','bridge:structure','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC','Arch (assume open spandrel) (1)','bridge:structure','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC','Deck','bridge:structure','deck'],
     ['BSC','Deck (3)','bridge:structure','deck'],
     ['BSC','Truss (8)','bridge:structure','truss'], // Truss
     ['BSC','Suspension (9)','bridge:structure','suspension'], // Suspension
     ['BSC','Unspecified Fixed','bridge:structure','fixed'], // From data
     ['BSC','Unspecified Fixed (14)','bridge:structure','fixed'], // From data
     ['BSC','Other','bridge:structure','other'], // Trestle
     ['BSC','Other (999)','bridge:structure','other'], // Trestle
     // ['BSC','2','bridge:structure','cantilever'], // Cantilever
     // ['BSC','5','bridge:structure','floating'], // Floating
     // ['BSC','6','bridge:structure','girder'], // Girder
     // ['BSC','7','bridge:structure','tower_suspension'], // Tower Suspension
     // ['BSC','12','bridge:structure','transporter'], // Transporter
     // ['BSC','15','bridge:structure','slab'], // Slab
     // ['BSC','16','bridge:structure','stringer_beam'], // Stringer Beam
     // ['BSC','19','bridge:structure','bowstring'], // Bowstring-bridge
     // ['BSC','22','bridge:structure','covered'], // Covered
     // ['BSC','26','bridge:structure','closed_spandrel_arch'], // Closed Spandrel Arch
     // ['BSC','27','bridge:structure','cable_stayed'], // Cable Stayed
     // ['BSC','30','bridge:structure','submersible'], // Submersible
     // ['BSC','31','bridge:structure','arch'], // Arch
     // ['BSC','32','bridge:structure','trestle'], // Trestle

     // BOT - Bridge Opening Type
     // ['BOT','-999999',undefined,undefined], // Null
     ['SPAN_TYPE','Fixed Span','bridge','fixed'],
     ['SPAN_TYPE','Fixed Span (2)','bridge','fixed'],
     // ['SPAN_TYPE','4','bridge','bascule'],
     // ['SPAN_TYPE','10','bridge','swing'],
     // ['SPAN_TYPE','11','bridge','lift'],
     // ['SPAN_TYPE','12','bridge','retractable'],
     // ['SPAN_TYPE','14','bridge','submersible'],
     // ['SPAN_TYPE','15','bridge','drawbridge'],
     // ['SPAN_TYPE','16','bridge','opening'],
     // ['SPAN_TYPE','999','bridge','other'],

     // Existance - Operational Status
     ['EXISTENCE','Abandoned/Disused','condition','abandoned'], // Unmaintained in spec
     ['EXISTENCE','Abandoned/Disused (6)','condition','abandoned'],
     ['EXISTENCE','Controlled','condition','other'], // ???
     ['EXISTENCE','Controlled (48)','condition','other'], // ???
     ['EXISTENCE','Damaged','condition','damaged'],
     ['EXISTENCE','Damaged (66)','condition','damaged'],
     ['EXISTENCE','Destroyed','condition','destroyed'],
     ['EXISTENCE','Destroyed (7)','condition','destroyed'],
     ['EXISTENCE','Dismantled (8)','condition','dismantled'],
     ['EXISTENCE','Natural','condition','other'], // ???
     ['EXISTENCE','Natural (45)','condition','other'], // ???
     ['EXISTENCE','Navigable (32)','navigation','yes'], // Why part of EXISTENCE???
     ['EXISTENCE','Operational','operational_status','operational'], // Operational
     ['EXISTENCE','Operational (28)','operational_status','operational'],
     ['EXISTENCE','Operational (28) (28)','operational_status','operational'],
     ['EXISTENCE','Other','condition','other'], // ???
     ['EXISTENCE','Other (999)','condition','other'], // ???
     ['EXISTENCE','Under Construction','condition','construction'], // Construction
     ['EXISTENCE','Under Construction (5)','condition','construction'], // Construction
     // ['EXISTENCE','4','operational_status','non_operational'], // Non-operational
     // ['EXISTENCE','6','operational_status','planned'], // Planned
     // ['EXISTENCE','8','operational_status','not_in_operation'], // Not in Operation ???
     // ['EXISTENCE','9','operational_status','temp_non_operational'], // Temporarily Non-operational
     // ['EXISTENCE','14','operational_status','partial'], // Partially Operational

     // Gap Width Range - Not pretty
     ['GAP_WIDTH','below 3 (1)','width:gap:range','0:3'],
     ['GAP_WIDTH','less than or equal to 3 meters','width:gap:range','0:3'],
     ['GAP_WIDTH','3 to 18 (2)','width:gap:range','3:18'],
     ['GAP_WIDTH','greater than 3 and less than or equal to 18 meters','width:gap:range','3:18'],
     ['GAP_WIDTH','18 to 25 (3)','width:gap:range','18:25'],
     ['GAP_WIDTH','greater than 18 and less than or equal to 25 meters','width:gap:range','18:25'],
     ['GAP_WIDTH','25 to 50 (4)','width:gap:range','25:50'],
     ['GAP_WIDTH','greater than 25 and less than or equal to 50 meters','width:gap:range','25:50'],
     ['GAP_WIDTH','50 to 75 (5)','width:gap:range','50:75'],
     ['GAP_WIDTH','greater than 50 and less than or equal to 75 meters','width:gap:range','50:75'],
     ['GAP_WIDTH','75 to 100 (6)','width:gap:range','75:100'],
     ['GAP_WIDTH','greater than 75 and less than or equal to 100 meters','width:gap:range','75:100'],
     ['GAP_WIDTH','greater than 75 and less than or equal to 100 me','width:gap:range','75:100'],
     ['GAP_WIDTH','100 to 142 (7)','width:gap:range','100:142'],
     ['GAP_WIDTH','greater than 100 and less than or equal to 142 m','width:gap:range','100:142'],
     ['GAP_WIDTH','above 142 (8)','width:gap:range','142:500'],
     ['GAP_WIDTH','greater than 142 meters','width:gap:range','142:500'],

     // ZI024_HYP - Water Resource Information : Hydrologic Persistence
     // ['ZI024_HYP','-999999',undefined,undefined], // No Information
     ['HYC','Perennial/Permanent','intermittent','no'], // Perennial
     ['HYC','Perennial/Permanent (8)','intermittent','no'], // Perennial
     ['HYC','Non-Perennial/Interm','intermittent','yes'],
     ['HYC','Non-Perennial/Intermittent/Fluctuating','intermittent','yes'],
     ['HYC','Non-Perennial/Intermittent/Fluctuating (6)','intermittent','yes'],
     ['HYC','Dry','intermittent','dry'], // Dry
     ['HYC','Dry (3)','intermittent','dry'], // Dry

     // RIT - Road Interchange Type
     ['RIT','Cloverleaf','junction','cloverleaf'], // Cloverleaf
     ['RIT','Cloverleaf (1)','junction','cloverleaf'], // Cloverleaf
     ['RIT','Diamond','junction','diamond'], // Diamond
     ['RIT','Diamond (2)','junction','diamond'], // Diamond
     ['RIT','Fork','junction','fork'], // Fork
     ['RIT','Fork (3)','junction','fork'], // Fork
     ['RIT','Other','junction','other'], // Other
     ['RIT','Rotary/Traffic Circle/Ro','junction','rotary'], // Rotary
     ['RIT','Rotary/Traffic Circle/Roundabout','junction','rotary'], // Rotary
     ['RIT','Rotary/Traffic Circle/Roundabout (4)','junction','rotary'], // Rotary
     ['RIT','Staggered Ramps','junction','staggered_ramps'], // Staggered Ramps
     ['RIT','Staggered Ramps (5)','junction','staggered_ramps'], // Staggered Ramps
     ['RIT','Standard Ramps','junction','standard_ramps'], // Standard Ramps
     ['RIT','Standard Ramps (6)','junction','standard_ramps'], // Standard Ramps
     ['RIT','Symmetrical Ramps','junction','symmetrical_ramps'], // Symmetrical Ramps
     ['RIT','Symmetrical Ramps (7)','junction','symmetrical_ramps'], // Symmetrical Ramps
     ['RIT','Trumpet','junction','trumpet'], // Trumpet
     ['RIT','Trumpet (8)','junction','trumpet'], // Trumpet
     ['RIT','Turban','junction','turban'], // Turban
     ['RIT','Turban (9)','junction','turban'], // Turban
     ['RIT','Wye','junction','wye'], // Wye
     ['RIT','Wye (10)','junction','wye'], // Wye

     // ZI017_RRA - Track Information : Railway Power Method
     // ['ZI017_RRA','-999999',undefined,undefined], // No Information
     ['RRA','1','electrified','rail'], // Electrified Track
     ['RRA','3','electrified','overhead'], // Overhead Electrified
     ['RRA','4','electrified','no'], // Non-electrified
     ['RRA','999','electrified','other'], // Other

     // RRC - Railway Use
     ['RRC','Main Line/Branch Line (18)','usage','main_or_branch'], // Not great
     ['RRC','Main Line (16)','usage','main'], // Main Line
     ['RRC','Branch Line (17)','usage','branch'], // Branch-line
     ['RRC','Other (999)','usage','other'], // Other

      // RSA - Branch Railway Type
      // ['RSA','-999999',undefined,undefined], // No Information
      ['RSA','Spur (1)','service','spur'], // Spur
      ['RSA','Siding (2)','service','siding'], // Siding
      ['RSA','Passing (3)','service','passing'], // Passing
      ['RSA','Other (999)','service','other'], // Other

      // RTA - Linear Feature Arrangement
      // ['RTA','-999999',undefined,undefined], // No Information
      // ['RTA','0',undefined,undefined], // This also gets used by pipelines
      ['RTA','Single (1)','arrangement','single'], // Single Arrangement
      ['RTA','Double (2)','arrangement','double'], // Double Arrangement
      ['RTA','Multiple (3)','arrangement','multiple'], // Multiple Arrangements
      ['RTA','Juxtaposition (4)','arrangement','juxtaposition'], // Juxtaposition
      ['RTA','Other (999)','arrangement','other'], // Other

     // LOC - Vertical Relative Location
     // ['LOCATION','-999999',undefined,undefined], // No Information
     ['LOCATION','On Ground Surface','location','surface'], // The default is on the surface // On Surface
     ['LOCATION','On Ground Surface (8)','location','surface'], // The default is on the surface // On Surface
     ['LOCATION','Suspended or Elevated Above Ground or Water Surf','location','overground'], // Above Surface
     ['LOCATION','Suspended or Elevated Above Ground or Water Surface','location','overground'], // Above Surface
     ['LOCATION','Suspended or Elevated Above Ground or Water Surface (25)','location','overground'], // Above Surface
     ['LOCATION','Underground (40)','location','underground'], // Below Ground Surface
     // ['LOCATION','17','location','on_waterbody_bottom'], // On Waterbody Bottom
     // ['LOCATION','23','location','below_waterbody_bottom'], // Below Waterbody Bottom
     // ['LOCATION','40','location','underground'], // Below Ground Surface
     // ['LOCATION','45','location','overground'], // Above Surface
     // ['LOCATION','46','location','above_waterbody_bottom'], // Above Waterbody Bottom
     // ['LOCATION','47','location','below_waterbody_surface'], // Below Waterbody Surface

     // MCC - Structural Material Type
     // ['MCC','-999999',undefined,undefined], // No Information
     ['MCC','Composition','material','composition'],
     ['MCC','Composition (20)','material','composition'],
     ['MCC','Concrete','material','concrete'], // Concrete
     ['MCC','Concrete (21)','material','concrete'],
     ['MCC','Earthen','material','soil'], // Soil - Close to Earthen??
     ['MCC','Earthen (30)','material','soil'], // Soil - Close to Earthen??
     ['MCC','Masonry (Brick/Stone)','material','masonry'], // Masonry
     ['MCC','Masonry (Brick/Stone) (62)','material','masonry'], // Masonry
     ['MCC','Metal','material','metal'], // Metal
     ['MCC','Metal (64)','material','metal'],
     ['MCC','Metal (64) (64)','material','metal'],
     ['MCC','Part Metal','material','part_metal'], // Metal
     ['MCC','Part Metal (72)','material','part_metal'], // Metal
     ['MCC','Soil','material','soil'],
     ['MCC','Steel','material','steel'], // Steel
     ['MCC','Steel (107)','material','steel'], // Steel
     ['MCC','Stone','material','stone'], // Stone
     ['MCC','Stone (108)','material','stone'], // Stone
     ['MCC','Other','material','other'], // Other
     ['MCC','Other (999)','material','other'], // Other

     // FUNCTION - Taken from FFN
     ['FUNCTION','Administration Building','office','administrative'], // Administration
     ['FUNCTION','Administration Building (28)','office','administrative'], // Administration
     ['FUNCTION','Aircraft Maintenance Shop','repair','aircraft'],
     ['FUNCTION','Aerator','man_made','aerator'],
     ['FUNCTION','Aerator (72)','man_made','aerator'],
     ['FUNCTION','Airport Terminal','aeroway','terminal'],
     ['FUNCTION','Airport Terminal (129)','aeroway','terminal'],
     ['FUNCTION','Armory','military','armory'],
     ['FUNCTION','Auditorium','amenity','auditorium'],
     ['FUNCTION','Auditorium (87)','amenity','auditorium'],
     ['FUNCTION','Bank','amenity','bank'], // Retail Banking
     ['FUNCTION','Bank (53)','amenity','bank'], // Retail Banking
     ['FUNCTION','Barracks','military','barracks'],
     ['FUNCTION','Barracks (150)','military','barracks'],
     ['FUNCTION','Barracks/Dormitory','military','barracks'],
     ['FUNCTION','Barracks/Dormitory (36)','military','barracks'],
     ['FUNCTION','Bus Station','amenity','bus_station'], // Extra tags in post processing
     ['FUNCTION','Bus Station (130)','amenity','bus_station'], // Extra tags in post processing
     ['FUNCTION','City Hall','amenity','townhall'],
     ['FUNCTION','City Hall (119)','amenity','townhall'],
     ['FUNCTION','Cemetery Building','building','cemetery_building'],
     ['FUNCTION','Cemetery Building (18)','building','cemetery_building'],
     ['FUNCTION','Commercial building','use','commercial'], // Commerce
     ['FUNCTION','Commercial building (133)','use','commercial'], // Commerce
     ['FUNCTION','Consulate','building','consulate'],
     ['FUNCTION','Consulate (67)','building','consulate'],
     ['FUNCTION','Courthouse','amenity','courthouse'],
     ['FUNCTION','Courthouse (61)','amenity','courthouse'],
     ['FUNCTION','Customs House','amenity','customs'],
     ['FUNCTION','Customs House (31)','amenity','customs'],
     ['FUNCTION','Depot Terminal (27)','man_made','depot'], // Not good
     ['FUNCTION','Dependents Housing/Bivouac Area','building','dependents_housing'], // Not good
     ['FUNCTION','Dependents Housing/Bivouac Area (93)','building','dependents_housing'],
     ['FUNCTION','Diplomatic Building','amenity','diplomatic_building'], // Diplomacy
     ['FUNCTION','Diplomatic Building (96)','amenity','diplomatic_building'], // Diplomacy
     ['FUNCTION','Dormatory','building','dormitory'],
     ['FUNCTION','Dormitory (151)','building','dormitory'],
     ['FUNCTION','Embassy (66)','amenity','embassy'],
     ['FUNCTION','Factory','industrial','manufacturing'],
     ['FUNCTION','Factory (116)','industrial','manufacturing'],
     ['FUNCTION','Fire Station','amenity','fire_station'],
     ['FUNCTION','Fire Station (37)','amenity','fire_station'],
     ['FUNCTION','Garage','use','garage'],
     ['FUNCTION','Garage (21)','use','garage'],
     ['FUNCTION','Government Administration Building','amenity','government_administration_building'],
     ['FUNCTION','Government Administration Building (5)','amenity','government_administration_building'],
     ['FUNCTION','Government Building','office','government'], // Government
     ['FUNCTION','Government Building (2)','office','government'], // Government
     ['FUNCTION','Greenhouse','building','greenhouse'],
     ['FUNCTION','Greenhouse (20)','building','greenhouse'],
     ['FUNCTION','Guard House','building','guard'],
     ['FUNCTION','Guard House (68)','building','guard'],
     ['FUNCTION','Guard Shack/Guard Room (69)','building','guardroom'],
     ['FUNCTION','Hangar','building','hangar'],
     ['FUNCTION','Hangar (30)','building','hangar'],
     ['FUNCTION',"Harbor Master's Office (77)",'office','harbor_master'],
     ['FUNCTION','Health Office','building','health_office'], 
     ['FUNCTION','Health Office (33)','building','health_office'], 
     ['FUNCTION','Hospital','building','hospital'], 
     ['FUNCTION','Hospital (6)','building','hospital'],
     ['FUNCTION','Hotel','tourism','hotel'], // Hotel
     ['FUNCTION','Hotel (95)','tourism','hotel'], // Hotel
     ['FUNCTION','House','building','residential'],
     ['FUNCTION','House (16)','building','residential'],
     ['FUNCTION','House of Worship','amenity','place_of_worship'], // Place of Worship
     ['FUNCTION','House of Worship (7)','amenity','place_of_worship'],
     ['FUNCTION','Kiln/Oven','man_made','kiln'], 
     ['FUNCTION','Kiln/Oven (75)','man_made','kiln'], 
     ['FUNCTION','Library','amenity','library'], 
     ['FUNCTION','Library (128)','amenity','library'], 
     ['FUNCTION','Market','amenity','marketplace'], // Market
     ['FUNCTION','Market (51)','amenity','marketplace'],
     ['FUNCTION','Medical Center','building','healthcare'],
     ['FUNCTION','Medical Center (100)','building','healthcare'],
     ['FUNCTION','Military Administration/Operations Building','building','military'], // Defence Activities
     ['FUNCTION','Military Administration/Operations Building (8)','building','military'], // Defence Activities
     ['FUNCTION','Mission','building','mission'],
     ['FUNCTION','Multi Unit Dwelling','building','appartments'],
     ['FUNCTION','Multi Unit Dwelling (17)','building','appartments'],
     ['FUNCTION','Municipal Hall','amenity','community_centre'], // not great
     ['FUNCTION','Museum','tourism','museum'],
     ['FUNCTION','Museum (9)','tourism','museum'],
     ['FUNCTION','Newspaper Plant (85)','amenity','newspaper_plant'],
     ['FUNCTION','Non-Governmental building of Significance','building','yes'], // ????
     ['FUNCTION','Non-Governmental building of Significance (140)','building','yes'], // ????
     ['FUNCTION','Non-Governmental building of Significance. (140)','building','yes'], // ????
     ['FUNCTION','Observatory','building','observatory'],
     ['FUNCTION','Observatory (10)','building','observatory'],
     ['FUNCTION','Oil/Gas Facilities Building','industrial','oil_gas_facility'],
     ['FUNCTION','Oil/Gas Facilities Building (102)','industrial','oil_gas_facility'],
     ['FUNCTION','Other','building','other'],
     ['FUNCTION','Other (999)','building','other'],
     ['FUNCTION','Palace','building','palace'],
     ['FUNCTION','Palace (11)','building','palace'],
     ['FUNCTION','Police Station','amenity','police'], // Law Enforcement
     ['FUNCTION','Police Station (12)','amenity','police'],
     ['FUNCTION','Post Office','amenity','post_office'],
     ['FUNCTION','Post Office (35)','amenity','post_office'],
     ['FUNCTION','Power Generation','use','power_generation'],
     ['FUNCTION','Power Generation (83)','use','power_generation'],
     ['FUNCTION','Prison','amenity','prison'],
     ['FUNCTION','Prison (13)','amenity','prison'],
     ['FUNCTION','Processing/Treatment (89)','industrial','manufacturing'],
     ['FUNCTION','Pumphouse','man_made','pumping_station'],
     ['FUNCTION','Pumphouse (90)','man_made','pumping_station'],
     ['FUNCTION','Railroad Station','railway','station'],
     ['FUNCTION','Railroad Station (94)','railway','station'],
     ['FUNCTION','Railroad Storage/Repair Facility','repair','railroad'],
     ['FUNCTION','Railroad Storage/Repair Facility (26)','repair','railroad'],
     ['FUNCTION','Repair Facility','repair','yes'],
     ['FUNCTION','Repair Facility (124)','repair','yes'],
     ['FUNCTION','Research and Development Lab/Research Facility (59)','building','research'],
     ['FUNCTION','Restaurant','amenity','restaurant'], // Restaurant
     ['FUNCTION','Restaurant (57)','amenity','restaurant'],
     ['FUNCTION','Sanitarium','amenity','sanatorium'], // OSM Spelling
     ['FUNCTION','Sanitarium (106)','amenity','sanatorium'], // OSM Spelling
     ['FUNCTION','School','building','school'], // Education
     ['FUNCTION','School (15)','building','school'],
     ['FUNCTION',"Senior Citizen's Home (109)",'social_facility','senior'], // NOTE QUOTES. Fixed in Post Proc
     ['FUNCTION','Service/Refueling Station','shop','fuel'], // Petrol Sale -- links to AQ170 Motor Vehicle Station
     ['FUNCTION','Service/Refueling Station (54)','shop','fuel'], // Petrol Sale -- links to AQ170 Motor Vehicle Station
     ['FUNCTION','Shipyard','amenity','shipyard'],
     ['FUNCTION','Shipyard (110)','amenity','shipyard'],
     ['FUNCTION','Sportsplex','leisure','sports_centre'],
     ['FUNCTION','Sportsplex (111)','leisure','sports_centre'],
     ['FUNCTION','Telephone Exchange (Main)','building','main_telephone_exchange'], // Main Telephone Exchange
     ['FUNCTION','Telephone Exchange (Main) (86)','building','main_telephone_exchange'], // Main Telephone Exchange
     ['FUNCTION','Telephone Switching Station','building','branch_telephone_exchange'],
     ['FUNCTION','Theater','amenity','theatre'],
     ['FUNCTION','Theater (127)','amenity','theatre'],
     ['FUNCTION','Town Hall','amenity','townhall'],
     ['FUNCTION','Town Hall (52)','amenity','townhall'],
     ['FUNCTION','Warehouse','building','warehouse'], // Warehousing and Storage
     ['FUNCTION','Warehouse (24)','building','warehouse'],
     ['FUNCTION','Weather Station','man_made','weather_station'],
     ['FUNCTION','Weather Station (92)','man_made','weather_station'],

     // POS - Power Source
     // ['POS','-999999',undefined,undefined], // No Information
     ['PPC','1','generator:source','geothermal'], // Geothermal
     ['PPC','Hydro-electric','generator:source','hydro'], // Hydro-electric
     ['PPC',' Hydro-electric (1)','generator:source','hydro'], // Hydro-electric
     ['PPC','3','generator:source','nuclear'], // Nuclear
     ['PPC','Internal Combustion','generator:source','internal_combustion'], // Internal Combustion
     ['PPC','Thermal','generator:source','thermal'], // Thermal
     ['PPC','Thermal (4)','generator:source','thermal'], // Thermal
     ['PPC','Tidal','generator:source','tidal'], // Tidal
     ['PPC','Other','generator:source','other'], // Other
     ['PPC','Other (999)','generator:source','other'], // Other

     // ZI014_PPO - Manufacturing Information : Product
     // ['ZI014_PPO','-999999',undefined,undefined], // No Information
     ['PRODUCT','Ammunition','product','ammunition'],
     ['PRODUCT','Ammunition (3)','product','ammunition'],
     ['PRODUCT','Asphalt (5)','product','asphalt'],
     ['PRODUCT','Brick','product','brick'],
     ['PRODUCT','Brick (9)','product','brick'],
     ['PRODUCT','Coal (17)','product','coal'],
     ['PRODUCT','Coke (19)','product','coke'],
     ['PRODUCT','Common fruit and/or','product','fruit'],
     ['PRODUCT','Common fruit and/or nuts','product','fruit'],
     ['PRODUCT','Common fruit and/or nuts (124)','product','fruit'],
     ['PRODUCT','Concrete','product','concrete'],
     ['PRODUCT','Concrete (21)','product','concrete'],
     ['PRODUCT','Cotton (121)','product','cotton'],
     ['PRODUCT','Electric (31)','product','electric_power'],
     ['PRODUCT','Explosives (33)','product','explosive'],
     ['PRODUCT','Food','product','food'], // Food
     ['PRODUCT','Food (35)','product','food'], // Food
     ['PRODUCT','Gas','product','gas'], 
     ['PRODUCT','Gas (38)','product','gas'], 
     ['PRODUCT','Gasoline','product','petrol'], // Petrol = Gasoline ??
     ['PRODUCT','Gasoline (39)','product','petrol'], // Petrol = Gasoline ??
     ['PRODUCT','Multiple','product','multiple'], 
     ['PRODUCT','Multiple (998)','product','multiple'], 
     ['PRODUCT','Oil','product','oil'], // Petroleum Lubricant 
     ['PRODUCT','Oil (67)','product','oil'], // Petroleum Lubricant Oil
     ['PRODUCT','Other','product','other'], // Other
     ['PRODUCT','Other (999)','product','other'], // Other
     ['PRODUCT','Refuse','product','refuse'],
     ['PRODUCT','Refuse (128)','product','refuse'],
     ['PRODUCT','Sewage (95)','product','sewage'],
     ['PRODUCT','Tobacco (129)','product','tobacco'], // Tobacco
     ['PRODUCT','Textile','product','textile'],
     ['PRODUCT','Textile (135)','product','textile'],
     ['PRODUCT','Water','product','water'], // Water
     ['PRODUCT','Water (116)','product','water'], // Water

     // ZI017_RGC - Track Information : Railway Gauge Classification
     // ['RGC','-999999',undefined,undefined], // No Information
     // ['RGC','0','gauge:type','unknown'],
     // ['RGC','1','gauge:type','broad'], // Broad
     // ['RGC','2','gauge:type','narrow'], // Narrow
     ['RGC','Normal (Country Specific) (3)','gauge:type','standard'], // Standard

     // ZI024_SCC - Water Resource Information : Water Type
     // ['ZI024_SCC','-999999',undefined,undefined], // No Information
     ['SCC','Alkaline (1)','water:type','alkaline'], // Alkaline
     ['SCC','Mineral (4)','water:type','mineral'], // Mineral
     ['SCC','Salt (10)','water:type','saline'], // Saline
     ['SCC','Fresh','water:type','fresh'], // Fresh
     ['SCC','Fresh (11)','water:type','fresh'], // Fresh
     ['SCC','Other (999)','water:type','other'], // Brine

     // ZI016_ROC - Route Pavement Information : Route Surface Composition
     ['SMC','Asphalt','surface','asphalt'],
     ['SMC','Asphalt (5)','surface','asphalt'],
     ['SMC','Bituminous - tar or','surface','bituminous'], // Tar or asphalt mixed in place, oiled 
     ['SMC','Bituminous - tar or asphalt mixed in','surface','bituminous'], // Tar or asphalt mixed in place, oiled 
     ['SMC','Cobble','surface','cobblestone'], // Cobble-stone
     ['SMC','Composition','surface','composition'], // No idea
     ['SMC','Composition (20)','surface','composition'], // No idea
     ['SMC','Concrete','surface','concrete'],
     ['SMC','Concrete (21)','surface','concrete'],
     ['SMC','Earthen','surface','earth'],
     ['SMC','Earthen (30)','surface','earth'],
     ['SMC','Graded or rolled ear','surface','compacted'],
     ['SMC','Graded or rolled earth, grass on gra','surface','compacted'],
     ['SMC','Graded or rolled earth, grass on graded earth (255)','surface','compacted'],
     ['SMC','Grass/Thatch','surface','grass'],
     ['SMC','Grass/Sod (Soft)','surface','grass'],
     ['SMC','Grass/Sod (Soft) (5)','surface','grass'],
     ['SMC','Gravel','surface','gravel'],
     ['SMC','Hardened Surface (252)','surface','compacted'],
     ['SMC','Hard/Paved','surface','paved'], // Asphalt?
     ['SMC','Hard/Paved (1)','surface','paved'], // Asphalt?
     ['SMC','Herbaceous Vegetatio','surface','vegetation'], // ??
     ['SMC','Loose/Unpaved','surface','unpaved'], // Stabilized earth
     ['SMC','Loose/Unpaved (2)','surface','unpaved'], // Stabilized earth
     ['SMC','Masonry (Brick/Stone)','surface','masonry'],
     ['SMC','Masonry (Brick/Stone) (62)','surface','masonry'],
     ['SMC','Metal','surface','metal'], // Metal
     ['SMC','Metal (64)','surface','metal'], // Metal
     ['SMC','Other','surface','other'], // Other
     ['SMC','Other (999)','surface','other'], // Other
     ['SMC','PEM - part concrete,','surface','paved'], // Best guess??
     ['SMC','PEM - part concrete, part asphalt or','surface','paved'], // Best guess??
     ['SMC','PEM - part concrete, part asphalt or part bitumen bound macadam (251)','surface','paved'], // Best guess??
     ['SMC','Permanent - hard surface type unknow','surface','paved'], 
     ['SMC','Part Metal','surface','part_metal'], 
     ['SMC','Part Metal (72)','surface','part_metal'], 
     ['SMC','Rock/Rocky (84)','surface','rock'], // Need to look at this
     ['SMC','Soil','surface','ground'],
     ['SMC','Soil (104)','surface','ground'], //Unimproved
     ['SMC','Water (116)','surface','water'], // ??? Need to look at this
     ['SMC','Wood (117)','surface','wood'], // Wood
     // ['SMC','3','surface','flexible_pavement'], // Flexible Pavement
     // ['SMC','4','surface','gravel'], // Aggregate 
     // ['SMC','5','surface','macadam'], // Macadam
     // ['SMC','6','surface','bound_surface'], // Bound Surface
     // ['SMC','7','surface','ridgid_pavement'], // Rigid Pavement
     // ['SMC','8','surface','concrete'], // Concrete
     // ['SMC','10','surface','asphalt_over_concrete'], // Asphalt over Concrete
     // ['SMC','12','surface','brick'], // Brick
     // ['SMC','15','surface','corduroy'], // rough hewn logs...  // Corduroy
     // ['SMC','16','surface','wood_plank'], // Wood Plank
     // ['SMC','17','surface','ice'], // Ice
     // ['SMC','18','surface','snow'], // Snow
     // ['SMC','999','surface','other'], // Other

     // TTC - Tower Type
     // ['TTC','-999999',undefined,undefined], // No Information
     ['TOWER_TYPE','Control','man_made','control_tower'], // Control Tower
     ['TOWER_TYPE','Control (16)','man_made','control_tower'], // Control Tower
     ['TOWER_TYPE','Control (16) (16)','man_made','control_tower'], // Control Tower
     ['TOWER_TYPE','Light tower','tower:type','light'], // No idea
     ['TOWER_TYPE','Light tower (5)','tower:type','light'], // No idea
     ['TOWER_TYPE','Microwave (17)','tower:type','microwave'], // Microwave
     ['TOWER_TYPE','Observation/Lookout','tower:type','observation'], // Observation Tower
     ['TOWER_TYPE','Observation/Lookout (2)','tower:type','observation'], // Observation Tower
     ['TOWER_TYPE','Other','tower:type','other'], // Other
     ['TOWER_TYPE','Other (999)','tower:type','other'],
     ['TOWER_TYPE','Powerline','cable:type','power'], // Not good - Pylon?
     ['TOWER_TYPE','Powerline (14)','cable:type','power'], // Not good - Pylon?
     ['TOWER_TYPE','Radar tower','tower:type','radar'], // Not sure
     ['TOWER_TYPE','Radio tower','tower:type','communication'], // Radio??
     // ['TOWER_TYPE','Water tower','tower:type','water'], //  Also an FCODE
     // ['TOWER_TYPE','Water tower (6)','tower:type','water'], //  Also an FCODE
     ['TOWER_TYPE','Water tower','man_made','water_tower'], //  Also an FCODE
     ['TOWER_TYPE','Water tower (6)','man_made','water_tower'], //  Also an FCODE

     // ['TRE','-999999',undefined,undefined], // No Information
     // TRE - Foliage Type
     ['TRE','Deciduous','wood','deciduous'], // Deciduous
     ['TRE','Deciduous (1)','wood','deciduous'], // Deciduous
     ['TRE','Evergreen','wood','evergreen'], // Evergreen
     ['TRE','Evergreen (2)','wood','evergreen'], // Evergreen
     ['TRE','Mixed','wood','mixed'], // Mixed
     ['TRE','Mixed (3)','wood','mixed'], // Mixed
     ['TRE','Other','wood','other'], // Other
     ['TRE','Other (999)','wood','other'], // Other

     // TRS - Transportation System Type
     // ['TRS','-999999',undefined,undefined], // No Information
     ['TUC','Pedestrian','transport:type','pedestrian'], // Pedestrian
     ['TUC','Pedestrian (17)','transport:type','pedestrian'], // Pedestrian
     ['TUC','Railroad','transport:type','railway'], // Railway
     ['TUC','Railroad (3)','transport:type','railway'], // Railway
     ['TUC','Road','transport:type','road'], // Road
     ['TUC','Road (4)','transport:type','road'], // Road
     // ['TUC','999', 'transport:type','other'], // Other
     // ['TUC','0','transport:type','unknown'],
     // ['TUC','1','transport:type','aeronautical'],
     // ['TUC','2','transport:type','aqueduct'], // Aqueduct
     // ['TUC','3','transport:type','automotive'], // From TRD3.0 to stop warnings Automotive
     // ['TUC','4','transport:type','bus'], // Bus
     // ['TUC','5','transport:type','canal'], // Canal
     // ['TUC','6','transport:type','caravan_route'], // Caravan Route
     // ['TUC','7','transport:type','maritime'],
     // ['TUC','8',undefined,undefined], // No Transportation System
     // ['TUC','10','transport:type','pipeline'], // Pipeline
     // ['TUC','11','transport:type','portage'], // Portage
     // ['TUC','14','transport:type','road_and_railway'],
     // ['TUC','15','transport:type','truck'], // Truck
     // ['TUC','16','transport:type','underground_railway'], // Underground Railway
     // ['TUC','17','transport:type','inland_waterway'], // Inland Waterway
     // ['TUC','18','transport:type','pack-road'], // Pack-road
     // ['TUC','20','transport:type','taxiway'], // Taxiway
     // ['TUC','21','transport:type','pipeline_maintenance'], // Pipeline Maintenance
     // ['TUC','22','transport:type','powerline_maintenance'], // Power Line Maintenance
     // ['TUC','23','transport:type','drove'], // Drove
     // ['TUC','25','transport:type','runway'], // Runway

     // Usage - A mix of CAA, FUN etc
     ['USAGE','42','use','commercial'], // Not many of these
     ['USAGE','Civilian/Public','controlling_authority','civilian'],
     ['USAGE','Civilian/Public (49)','controlling_authority','civilian'],
     ['USAGE','Commercial','use','commercial'],
     ['USAGE','Commercial (42)','use','commercial'],
     ['USAGE','Industrial','use','manufacturing'],
     ['USAGE','Industrial (41)','use','manufacturing'],
     ['USAGE','Institutional','use','institute'], // From the specs - WTF is this?
     ['USAGE','Institutional (43)','use','institute'],
     ['USAGE','Interstate','highway','motorway'],
     ['USAGE','Interstate (37)','highway','motorway'],
     ['USAGE','Joint Military/Civil','controlling_authority','joint_military_civilian'], // Joint Military and Civilian
     ['USAGE','Joint Military/Civilian (22)','controlling_authority','joint_military_civilian'], // Joint Military and Civilian
     ['USAGE','Levee/Dike','embankment:type','levee'], // Guess ?
     ['USAGE','Military','controlling_authority','military'],
     ['USAGE','Military (8)','controlling_authority','military'],
     ['USAGE','Primary/1st Order','highway','primary'], 
     ['USAGE','Primary/1st Order (26)','highway','primary'], 
     ['USAGE','Residential','use','residential'],
     ['USAGE','Residential (44)','use','residential'],
     ['USAGE','Secondary/2nd Order','highway','secondary'],
     ['USAGE','Secondary/2nd Order (30)','highway','secondary'],

     // VEG - Vegetation Characteristic - This is species, type, land cover etc
     // This needs more work when we get better data
     // ['VEG','-999999',undefined,undefined], // No Information
     ['VEG','Other','vegetation:type','other'], // Other
     ['VEG','Other (999)','vegetation:type','other'], // Other



     // ZI016_WTC - Route Pavement Information : Road Weather Restriction
     // ['ZI016_WTC','-999999',undefined,undefined], // No Information
     ['WTC','All Weather','seasonal','no'], // All-weather
     ['WTC','All Weather (1)','seasonal','no'], // All-weather
     ['WTC','Fair/Dry Weather','seasonal','fair'], // possibly seasonal // Fair-weather
     ['WTC','Fair/Dry Weather (2)','seasonal','fair'], // possibly seasonal // Fair-weather
     // ['WEATHER','3','seasonal','winter'], // Winter Only
     // ['WEATHER','4','seasonal','limited'], // Limited all-weather 
     // ['WEATHER','5','seasonal','not_winter'], // Closed in Winter
     // ['WEATHER','999','seasonal','other'], // Other

     // ZI037_RFA - Religious Information : Religious Facility Type
     // ['ZI037_RFA','-999999',undefined,undefined], // No Information
     ['WORSHIP','Cathedral (2)','building','cathedral'], // Cathedral
     ['WORSHIP','Church','building','church'], // Church
     ['WORSHIP','Church (4)','building','church'], // Church
     ['WORSHIP','Minaret','building','minaret'], // Minaret
     ['WORSHIP','Minaret (6)','building','minaret'], // Minaret
     ['WORSHIP','Monastery, Convent','building','monastery'], // Monastery
     ['WORSHIP','Monastery, Convent (7)','building','monastery'], // Monastery
     ['WORSHIP','Mosque','building','mosque'], // Mosque
     ['WORSHIP','Mosque (9)','building','mosque'], // Mosque
     ['WORSHIP','Shrine','building','shrine'], // Shrine
     ['WORSHIP','Shrine (14)','building','shrine'], // Shrine
     ['WORSHIP','Temple (15)','building','temple'], // Temple
     ['WORSHIP','Temple (16)','building','temple'], // Temple - Diff to 15 ???
     // ['WORSHIP','2','building','chapel'], // Chapel
     // ['WORSHIP','4','building','marabout'], // Marabout
     // ['WORSHIP','6','building','mission'], // Mission
     // ['WORSHIP','8','building','pagoda'], // Pagoda
     // ['WORSHIP','9','building','religious_community'], // Religious Community
     // ['WORSHIP','10','building','seminary'], // Seminary
     // ['WORSHIP','11','building','shrine'], // Shrine
     // ['WORSHIP','12','building','stupa'], // Stupa
     // ['WORSHIP','13','building','synagogue'], // Synagogue
     // ['WORSHIP','14','building','tabernacle'], // Tabernacle
     // ['WORSHIP','15','building','temple'], // Temple
     // ['WORSHIP','18','building','convent'], // Convent
     // ['WORSHIP','19','building','monastery'], // Monastery
     // ['WORSHIP','20','building','noviciate'], // Noviciate
     // ['WORSHIP','21','building','hermitage'], // Hermitage
     // ['WORSHIP','22','building','retreat'], // Retreat
     // ['WORSHIP','23','building','burial_site'], // Burial Site
     // ['WORSHIP','999','building:religious','other'], // Other - to avoid conflicts

     // SEC_CLASS - Security classification
     ['SEC_CLASS','5','security:classification','U'], // Really, this is a first
     ['SEC_CLASS','Unclassified','security:classification','U'],
     ['SEC_CLASS','Unclassified (5)','security:classification','U'],
     ['SEC_CLASS','Unclassified (5) (5)','security:classification','U'],
     ['SEC_CLASS','Restricted','security:classification','R'],
     ['SEC_CLASS','Confidential','security:classification','C'],
     ['SEC_CLASS','Secret','security:classification','S'],
     ['SEC_CLASS','Top Secret','security:classification','TS'],

     // ZSAX_RX3 - Restriction Information : Security Attributes Group (resource non-intelligence community markings)
     ['QLE','LIMDIS','security:dissemination_control:non_ic','DS'],
     ['QLE','Limited Distribution','security:dissemination_control:non_ic','DS'],
     ['QLE','FOUO','security:dissemination_control:ic','FOUO'],
     ['QLE','LIMIDS','security:dissemination_control:non_ic','DS'], // Really ??
     ['QLE','FOULO','security:dissemination_control:ic','FOUO'], // Really ??

     // Modification of ZSAX_RX3 for SOURCE releasability
     // Kind of ugly and will probably cause problems with dirty-word checkers
     ['SRC_RELSE','LIMDIS','source:non_ic_dissemination_control','DS'],

     // ZI004_RCG - Process Step Information : Resource Content Originator
     // This is just plain Ugly
     // Only one of these. I don't think anyone else actually makes UTP
     ['ORIG','US Army Geospatial Center','attribution','u.s._army_geospatial_center_(agc)'], // U.S.ArmyGeospatialCenter(AGC)

     ], // End one2one

     // ##### End of One2One Rules #####

} // End of utp.rules
