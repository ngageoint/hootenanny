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
 * @copyright Copyright (C) 2013, 2014 Maxar (http://www.maxar.com/)
 */

/*
    MGCP Rules
*/

mgcp.rules = {
    // ##### Start of One2One Rules #####
    // FCODE rules for Import
    fcodeOne2oneInV3 : [
    // MGCP TRDv3 Only Fcodes
    ['F_CODE','AL100','building','hut'], // Hut - NFDD AL099
    ['F_CODE','AL240','man_made','tower'], // Non-communication Tower - NFDD AL241
    ['F_CODE','AQ064','man_made','causeway'], // Causeway - NFDD AQ063
    ['F_CODE','AT080','tower:type','communication'], // Communications Tower - NFDD AL241
    ['F_CODE','DB030','natural','cave_entrance'], // Cave - FCODE Retired, moveing to DB029
    ['F_CODE','GA035','aeroway','navigationaid'], // Aeronautical NAVAID - FCODE Retired
    ['F_CODE','GB010','man_made','beacon'], // Airport Navigation Lights - FCODE Retired
    ], // End fcodeOne2oneInV3

    fcodeOne2oneV4 : [
    // MGCP TRDv4 Only FCODES
    // These are used for both import and export
    ['F_CODE','AA012','landuse','quarry'], // Quarry - NFDD AA010
    ['F_CODE','AA050','man_made','well'], // Well - NFDD BH230 or AA054 based on Product type
    ['F_CODE','AC000','facility','processing'], // Processing Facility - NFDD AL010
    ['F_CODE','AL010','landuse','religious'], // Processing Facility - NFDD AL010
    // ['F_CODE','AH050','site:type','fortification'], // Fortification - FCODE Retired
    ['F_CODE','AH050','military','bunker'], // Fortification - FCODE Retired
    //['F_CODE','AJ010','farm:irrigation','centre_pivot'], // Circular Irrigation System - FCODE Retired
    ['F_CODE','AK190','man_made','recreational_pier'], // Recreational Pier - NFDD BB081
    ['F_CODE','AL015','building','yes'], // Building - changed to AL013
    ['F_CODE','AL210','route:protection','yes'], // Protection Shed - NFDD AL211
    ['F_CODE','AT030','power','line'], // Power Line - NFDD AT005
    ['F_CODE','AT050','use','communication'], // Communication Station - No replacement code
    ['F_CODE','AT060','communication','line'], // Communication Line - NFDD AT005
    ['F_CODE','BB041','man_made','breakwater'], // Breakwater - NFDD BB081
    ['F_CODE','BB043','man_made','groyne'], // Groin - NFDD BB081
    ['F_CODE','BB140','man_made','training_wall'], // Training Wall - NFDD BB081
    // ['F_CODE','BB190','man_made','berthing_structure'], // Berthing Structure - NFDD BB081
    ['F_CODE','BB190','man_made','shoreline_construction'], // Shoreline Construction
    ['F_CODE','BB190','seamark:type','shoreline_construction'], // Shoreline Construction
    ['F_CODE','BB230','wall','seawall'], // Seawall - NFDD BB081
    ['F_CODE','BB240','leisure','slipway'], // Slipway - NFDD BB082
    ['F_CODE','BD110','man_made','offshore_construction'], // Offshore Platform - NFDD BD115
    ['F_CODE','BH060','water:race','flume'], // Flume - NFDD BH065
    ['F_CODE','BH080','water','lake'], // Lake - NFDD BH082
    ['F_CODE','BH130','water','reservoir'], // Reservoir - NFDD BH082
    ['F_CODE','BI041','man_made','basin_gate'], // Water Gate - NFDD BI045
    ['F_CODE','CA035','geological','water_elevation'], // Inland Water Elevation
    ['F_CODE','DB200','natural','gully'], // Gully - FCODE Retired
    ['F_CODE','EC030','landuse','forest'], // Wood - NFDD EC015
    ['F_CODE','ED030','wetland','mangrove'], // Mangrove Swamp - NFDD ED020
    ['F_CODE','FA090','man_made','geophysical_prospecting_grid'], // Geophysical Prospecting Grid - No replacement code
    ['F_CODE','GA034','airmark','beacon'], // Aeronautical Radio Navigation Service - TDS GA033
    ['F_CODE','GB220','aeroway','obstacle'], // Aeronautical Obstacle - No replacement code
    ['F_CODE','GB485','aeroway','navigationaid'], // Approach Lighting System - Not in TDS
    ], // End fcodeOne2oneV4

    // These are for finding an FCODE for export.
    // Note: These get swapped around before being used
    fcodeOne2oneOut : [
    ['F_CODE','AA050','man_made','well'], // Well - NFDD BH230 or AA054 based on product
    ['F_CODE','AA050','man_made','water_well'], // Well - NFDD BH230
    ['F_CODE','AA050','man_made','petroleum_well'], // Well - NFDD AA054
    ['F_CODE','AD010','power','station'], // Electric Power Station, Power Plant
    ['F_CODE','AG030',undefined,undefined], // Shopping Complex - Not valid MGCP
    ['F_CODE','AH050','historic','castle'], // Fortification - Not great
    ['F_CODE','AJ110','building','greenhouse_horticulture'], // Greenhouse
    ['F_CODE','AK030','leisure','water_park'], // Amusement Park
    ['F_CODE','AK040','sport','soccer'], // From OSM
    ['F_CODE','AK120','landuse','recreation_ground'], // Park
    ['F_CODE','AK120','landuse','village_green'], // Park
    // ['F_CODE','AK120','leisure','nature_reserve'], // Park (commenting out because it was translating over other features)
    ['F_CODE','AK120','leisure','playground'], // Park
    ['F_CODE','AK170','amenity','swimming_pool'], // Pool
    ['F_CODE','AL010','diplomatic','embassy'], // Embassy
    ['F_CODE','AL015','building','yes'], // To fix amenity etc
    ['F_CODE','AL015','information','office'], // Tourism Office
    ['F_CODE','AL015','man_made','depot'],
    ['F_CODE','AL015','tourism','hotel'], // To fix amenity etc
    // ['F_CODE','AL105','place','hamlet'], // From OSM
    // ['F_CODE','AL105','place','isolated_dwelling'], // From OSM
    // ['F_CODE','AL105','place','town'], // From OSM
    // ['F_CODE','AL105','place','village'], // From OSM
    ['F_CODE','AL019','building','other_shed'], //Ugly and from TRDv3
    ['F_CODE','AL030','amenity','grave_yard'], // From OSM Data
    ['F_CODE','AL105','tourism','caravan_site'], // From OSM data, not a building so...
    ['F_CODE','AL130','historic','memorial'],
    //['F_CODE','AL130','tourism','attraction'], // From OSM data, not great
    ['F_CODE','AL200','ruins','yes'], // Ruins
    ['F_CODE','AL241','man_made','mast'], // MGCP does not have AT011 (Aerial)
    ['F_CODE','AL241','man_made','lighthouse'], // Lighthouse
    ['F_CODE','AL260','barrier','city_wall'], // Wall
    ['F_CODE','AP020',undefined,undefined], // Road Interchange -Not valid MGCP
    // ['F_CODE','AP030','highway','living_street'],
    ['F_CODE','AP030','highway','construction'], // Lifecycle
    ['F_CODE','AP030','highway','motorway'], // Limited Access Motorway
    ['F_CODE','AP030','highway','motorway_link'], // Limited Access Motorway
    ['F_CODE','AP030','highway','primary'],
    ['F_CODE','AP030','highway','primary_link'],
    ['F_CODE','AP030','highway','residential'],
    ['F_CODE','AP030','highway','secondary'],
    ['F_CODE','AP030','highway','secondary_link'],
    ['F_CODE','AP030','highway','tertiary'],
    ['F_CODE','AP030','highway','tertiary_link'],
    ['F_CODE','AP030','highway','trunk'], // Motorway
    ['F_CODE','AP030','highway','trunk_link'], // Motorway
    ['F_CODE','AP030','highway','unclassified'],
    ['F_CODE','AP030','highway','road'], // Just in case...
    ['F_CODE','AP030','highway','yes'], // Just in case...
    // ['F_CODE','AP030','highway','living_street'],
    // ['F_CODE','AP030','highway','pedestrian'], // It is still a road
    // ['F_CODE','AP030','highway','service'],
    // ['F_CODE','AP050','highway','bridleway'],
    // ['F_CODE','AP050','highway','cycleway'],
    // ['F_CODE','AP050','highway','footway'],
    ['F_CODE','AQ040','man_made','bridge'], // Bridge
    ['F_CODE','AQ125','amenity','bus_station'],
    ['F_CODE','AT030','power','minor_line'], // Power Line - NFDD AT005
    ['F_CODE','BA010','natural','land_water_boundary'], // Land/Water Boundary - old translation
    ['F_CODE','BA030','place','islet'], // From OSM
    ['F_CODE','BA040','water','tidal'], // Tidal Water
    ['F_CODE','BB190','man_made','pier'], // From OSM
    ['F_CODE','BB190','man_made','quay'], // Berthing Structure - NFDD BB081
    ['F_CODE','BB190','waterway','dock'], // Berthing Structure - NFDD BB081
    ['F_CODE','BH030','barrier','ditch'], // Ditch. Not great: Dry vs wet
    ['F_CODE','BH080','water','basin'], // From OSM - Lake is close but not great
    ['F_CODE','BH080','landuse','basin'], // From OSM - Lake is close but not great
    ['F_CODE','BH130','landuse','reservoir'], // From OSM
    ['F_CODE','BH130','man_made','reservoir'], // Reservoir - NFDD BH082
    ['F_CODE','BH135','landuse','rice_field'], // Rice Field - tag change
    ['F_CODE','BH155','man_made','salt_pond'], // Due to the default translation changing
    ['F_CODE','DA010','landuse','brownfield'], // area planned for development - Soil Surface Region
    ['F_CODE','DA010','natural','desert'], // EE030 Desert - mapped to DA010 Soil Surface Region
    ['F_CODE','DB090','embankment','berm'],
    ['F_CODE','DB090','embankment','divider'],
    ['F_CODE','DB090','embankment','fill'],
    ['F_CODE','DB090','embankment','levee'],
    ['F_CODE','DB090','embankment','mound'], // Embankment
    ['F_CODE','DB090','man_made','dyke'],
    ['F_CODE','EB020','natural','heath'], // Thicket
    ['F_CODE','EC030','natural','wood'], // Trees
    ['F_CODE','ED010','natural','wetland'], // Marsh
    ['F_CODE','ED010','wetland','reedbed'], // Marsh
    ['F_CODE','FA015','man_made','shooting_range'], // Firing Range
    ['F_CODE','GB485','aeroway','approach_light'], // From OSM
    ['F_CODE','GB485','aeroway','landing_light'], // From OSM
    ['F_CODE','GB485','aeroway','navigation_aid'], // From OSM
    ['F_CODE','GB485','aeroway','papi'], // From OSM
    ['F_CODE','GB485','aeroway','vasi'], // From OSM
    ['F_CODE','GB050','barrier','berm'], // Aircraft Revetment
    ['F_CODE','ZD040','natural','bay'], // From OSM
    ['F_CODE','ZD040','natural','cape'], // From OSM
    ['F_CODE','ZD040','natural','hill'], // From OSM
    ['F_CODE','ZD040','natural','peak'], // From OSM
    ['F_CODE','ZD040','natural','valley'], // From GeoNames
    // ['F_CODE','ZD040','place','county'], // From OSM
    // ['F_CODE','ZD040','place','neighbourhood'], // From OSM
    // ['F_CODE','ZD040','place','quarter'], // From OSM
    // ['F_CODE','ZD040','place','state'], // From OSM
    // ['F_CODE','ZD040','place','suburb'], // From OSM
    // ['F_CODE','ZD040','place','unknown'], // From OSM
    ], // End fcodeOne2oneOut

    // One2one rules for Text attributes
    txtBiasedV4 : {
    'CCN':'source:copyright', // Commercial Copyright Notice
    'IKO':'icao',  // ICAO Identifier
    'NA2':'alt_name',  // Touristic Name
    'NAM':'name', // Name
    'NFI':'gndb_id:2', // Named Feature Identifier. Not sure how this is different to NFN
    'NFN':'gndb_id', // Name Identifier
    //'SDP':'source:description', // Source Description
    'SDP':'source', // Source Description
    'SDV':'source:datetime', // Source Date and Time
    'TXT':'note', // Associated Text
    'UID':'uuid', // Universally Unique Identifier
    'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
    'NM3':'name:three', // Name 3
    'NM4':'name:four', // Name 4
    }, // End txtBiased

    // Txt  rules for importing TRD3
    txtBiasedV3 : {
    'NA3':'classification_name',  // Classification Name
    'RTN':'ref',  // Route Designation
    'TIER_NOTE':'source:commercial_distribution_restriction', // Commercial Distribution Restriction
    'UPD_DATE':'source:update:datetime', // Review Source Date and Time
    'UPD_INFO':'source:update:description', // Review Source Description
    },


    // One2one rules for Number attributes
    numBiasedV4 : {
    'AOO':'angle',  // Angle of Orientation
    'BEL':'ele:base', // Base Elevation
    'DMB':'undergrowth:density',  // Undergrowth Density
    'DMT':'canopy_cover',  // Canopy Cover
    'GAW':'gauge',  // Railway Gauge
    'HGT':'height',  // Height Above Surface Level
    'LEN':'length', // Length or Diameter
    'LTN':'lanes', // Track or Lane Count
    'NOS':'bridge:span_count', // Span Count
    // 'OHB':'height', // Overall Bridge Height - copy of HGT
    'SDO':'dune:orientation',  // Sand Dune Orientation
    'WD1':'width:minimum_traveled_way', // Route Minimum Travelled Way
    'WID':'width', // Width
    'ZVA':'ele', // Aerodrome Elevation
    'ZVH':'ele:max' // Highest Elevation
    }, // End numBiased


    // Txt rules for importing TRD3
    numBiasedV3 : {
    'ACE':'source:accuracy:horizontal', // Absolute Horizontal Accuracy
    'ALE':'source:accuracy:vertical', // Absolute Vertical Accuracy
    'ARA':'feature_area', // Area
    'BRF':'navigation_aid:broadcast_freq', // Broadcast Frequency
    'CTL':'railway:track:length', // Cumulative Track Length
    'DMF':'feature_count',  // Feature Count
    'DMT':'canopy_cover',  // Canopy Cover
    'FCL':'ferry:crossing_distance',  // Ferry Crossing Distance
    'HCA':'bridge:horizontal_clearance', // Horizontal Clearance
    'KVA':'voltage',  // Power Line Maximum Voltage
    'LBV':'waterway:bank2:cover', // Left Bank Vegetation Cover
    'LC1':'mlc:wheeled_oneway', // Load Class Type 1
    'LC2':'mlc:wheeled', // Load Class Type 2
    'LC3':'mlc:tracked_oneway', // Load Class Type 3
    'LC4':'mlc:tracked', // Load Class Type 4
    'LNU':'length:interior_useable',  // Usable Length
    'MOH':'obstacle:height:max',  // Maximum Obstacle Height
    'MVC':'max_clearance', // Maximum Vertical Clearance
    'NPL':'cables',  // Parallel Line Count
    'PFH':'height:predominant', // Predominant Feature Height
    'RBV':'waterway:bank1:cover', // Right Bank Vegetation Cover
    },


    // Common one2one rules. Used for both import and export
    one2one : [
    // ACC - Horizontal Accuracy Category
    ['ACC','1','source:accuracy:horizontal:category','accurate'],
    ['ACC','2','source:accuracy:horizontal:category','approximate'],

    // AFA - Available Vessel Service
    // ['AFA','0',undefined,undefined], // Unknown
    ['AFA','0','available_service','unknown'], // Unknown
    ['AFA','19','available_service','helipad'], // Helipad
    ['AFA','41','available_service','none'], // None Available
    ['AFA','999','available_service','other'], // Other

    // ATC - Aqueduct Type
    // ['ATC','0',undefined,undefined], // Unknown
    ['ATC','0','aqueduct:type','unknown'], // Unknown
    ['ATC','1','aqueduct:type','qanat'], // Qanat Shaft

    // AZC - Man-made
    // ['AZC','0',undefined,undefined], // Unknown
    ['AZC','0','artificial','unknown'], // Unknown
    ['AZC','1000','artificial','no'], // Natural
    ['AZC','1001','artificial','yes'], // Man-made

    // BAC - Built-up Area Density
    // ['BAC','0',undefined,undefined],
    ['BAC','0','building:density','unknown'],
    ['BAC','1','building:density','sparse'],
    ['BAC','2','building:density','dense'],

    // BAL - BGN Administrative Level
    ['BAL','1','administrative_subdivision','first-order'], // First-order
    ['BAL','2','administrative_subdivision','second-order'], // Second-order
    ['BAL','3','administrative_subdivision','third-order'], // Third-order
    ['BAL','4','administrative_subdivision','fourth-order'], // Fourth-order
    ['BAL','5','administrative_subdivision','undifferentiated'], // Undifferentiated
    ['BAL','999','administrative_subdivision','other'], // Other

    // BOC - Bog Type
    // ['BOC','0',undefined,undefined], // Unknown
    ['BOC','0','bog:type','unknown'], // Unknown
    ['BOC','14','bog:type','peat'], // Peat Bog
    ['BOC','7','bog:type','cranberry'], // Cranberry Bog
    ['BOC','999','bog:type','other'], // Other

    // BOT - Bridge Opening Type
    // ['BOT','0',undefined,undefined], // Unk
    ['BOT','0','bridge:movable','unknown'], // Unk
    ['BOT','4','bridge:movable','bascule'],
    ['BOT','10','bridge:movable','swing'],
    ['BOT','11','bridge:movable','lift'],
    ['BOT','12','bridge:movable','retractable'],
    ['BOT','13',undefined,undefined], // In data but not in any spec!
    ['BOT','17','bridge:movable','no'],
    ['BOT','998','bridge:movable','not_applicable'],

    // BSC - Bridge Structure
    // ['BSC','0',undefined,undefined], // Unknown
    ['BSC','0','bridge:structure','unknown'],
    ['BSC','1','bridge:structure','open_spandrel_arch'],
    ['BSC','2','bridge:structure','cantilever'],
    ['BSC','3','bridge:structure','deck'],
    ['BSC','5','bridge:structure','floating'],
    ['BSC','6','bridge:structure','girder'],
    ['BSC','7','bridge:structure','tower_suspension'],
    ['BSC','8','bridge:structure','truss'],
    ['BSC','9','bridge:structure','suspension'],
    ['BSC','12','bridge:structure','transporter'],
    ['BSC','15','bridge:structure','slab'],
    ['BSC','16','bridge:structure','stringer_beam'],
    ['BSC','19','bridge:structure','bowstring'],
    ['BSC','26','bridge:structure','closed_spandrel_arch'],
    ['BSC','27','bridge:structure','cable_stayed'],
    ['BSC','999','bridge:structure','other'],

    // BST - Boundary Status
    ['BST','0','boundary:status','unknown'], // Unknown
    ['BST','1','boundary:status','definite'], // Definite
    ['BST','2','boundary:status','indefinite'], // Indefinite
    ['BST','3','boundary:status','in_dispute'], // In Dispute
    ['BST','4','boundary:status','no_defined_boundary'], // No Defined Boundary
    ['BST','999','boundary:status','other'], // Other

    // CAA - Controlling Authority
    // ['CAA','0',undefined,undefined],
    ['CAA','0','operator','unknown'],
    ['CAA','3','operator','private'],
    ['CAA','5','operator','military'],
    ['CAA','7','operator','joint_military_civilian'],
    ['CAA','16','operator','civilian'],
    ['CAA','999','operator','other'], // Other

    // CAB - Cable Type
    // ['CAB','0',undefined,undefined], // Unknown
    ['CAB','0','cable:type','unknown'], // Unknown
    ['CAB','2','cable:type','power'], // Power Line
    ['CAB','6','cable:type','transmission'], // Transmission Line
    ['CAB','8','cable:type','communication'], // Communication Line
    ['CAB','998','cable:type','not_applicable'],
    ['CAB','999','cable:type','other'], // Other

    // CAT - Cableway Type
    // ['CAT','0',undefined,undefined], // Unknown
    ['CAT','0','cableway:type','unknown'], // Unknown
    ['CAT','2','cableway:type','chair-lift'], // Chair-lift
    ['CAT','5','cableway:type','aerial_tramway'], // Aerial Tramway
    ['CAT','6','cableway:type','gondola_lift'], // Gondola Lift
    ['CAT','8','cableway:type','industrial_ropeway'], // Industrial Ropeway
    ['CAT','9','cableway:type','material_tramway'], // Material Tramway
    ['CAT','998','cableway:type','not_applicable'],
    ['CAT','999','cableway:type','other'], // Other

    // CCT - Cover Closure Type
    // ['CCT','0',undefined,undefined], // Unknown
    ['CCT','0','cover:type','unknown'], // Unknown
    ['CCT','1','cover:type','open'], // Open
    ['CCT','2','cover:type','partial'], // Partial
    ['CCT','3','cover:type','moveable'], // Moveable
    ['CCT','4','cover:type','complete'], // Complete
    ['CCT','999','cover:type','other'], // Complete

    // CDA - Covered Drain
    // ['CDA','0',undefined,undefined],
    ['CDA','0','covered','unknown'],
    ['CDA','1000','covered','no'],
    ['CDA','1001','covered','yes'],

    // CFT - Boundary Determination Method
    // ['CFT','0','seamark:boundary:determination','unknown'], // Unknown
    ['CFT','7','seamark:boundary:determination','elevation_contour'], // Elevation contour
    ['CFT','8','seamark:boundary:determination','drainage_limit'], // Drainage Limit
    ['CFT','10','seamark:boundary:determination','ridgeline'], // Ridgeline
    ['CFT','11','seamark:boundary:determination','drainage_line'], // Drainage Line
    ['CFT','12','seamark:boundary:determination','left_bank'], // Left Bank
    ['CFT','13','seamark:boundary:determination','right_bank'], // Right Bank
    ['CFT','14','seamark:boundary:determination','shoreline'], // Shoreline
    ['CFT','15','seamark:boundary:determination','lake_transec'], // Lake Transec
    ['CFT','16','seamark:boundary:determination','meridian'], // Meridian
    ['CFT','17','seamark:boundary:determination','median'], // Median
    ['CFT','18','seamark:boundary:determination','parallel'], // Parallel
    ['CFT','19','seamark:boundary:determination','straight_segmented'], // Straight Segmented
    ['CFT','20','seamark:boundary:determination','straight'], // Straight
    ['CFT','33','seamark:boundary:determination','anthropogeographic'], // Anthropogeographic

    // CNS - Controlled
    // ['CNS','0',undefined,undefined], // Unknown
    ['CNS','0','controlled','unknown'], // Unknown
    ['CNS','1000','controlled','no'], // Uncontrolled
    ['CNS','1001','controlled','yes'], // Controlled

    // COD - Delineation Known
    // ['COD','1000','raw:COD','limits_and_information_unknown'], // Limits and Information Unknown
    ['COD','1000','delineation','unknown'], // Limits and Information Unknown
    ['COD','1001','delineation','known'], // Limits and Information Known

    //CON - Road Width Constriction Type
    ['CON','35','junction','yes'],
    ['CON','998','route_constriction','not_applicable'],

    // CRA - Crane Type
    ['CRA','0','crane:type','unknown'], // Unknown
    ['CRA','2','crane:type','bridge'], // Bridge Crane
    ['CRA','3','crane:type','rotating'], // Rotating Crane
    ['CRA','7','crane:type','tower'], // Tower Crane
    ['CRA','99','crane:type','container'], // Container Crane
    ['CRA','999','crane:type','other'], // Other

    // CRM - Crane Mobility Type
    ['CRM','0','crane:mobile','unknown'], // Unknown
    ['CRM','1','crane:mobile','fixed'], // Fixed
    ['CRM','2','crane:mobile','travelling'], // Travelling
    ['CRM','3','crane:mobile','floating'], // Floating

    // CSP - Crop Species
    // ['CSP','0',undefined,undefined], // Unknown
    ['CSP','0','crop','unknown'], // Unknown
    ['CSP','113','crop','nut'], // Nut
    ['CSP','13','crop','dry_crop'], // Dry Crop
    ['CSP','15','trees','fruit_tree'], // Fruit Tree
    ['CSP','29','trees','rubber_tree'], // Rubber
    ['CSP','34','trees','tea_plants'], // Tea
    ['CSP','37','crop','vegetable'], // Vegetable Crop
    ['CSP','41','trees','banana_plants'], // Banana
    ['CSP','45','crop','cotton'], // Cotton
    ['CSP','157','crop','palm'], // Palm
    ['CSP','999','crop','other'], // Other

    // EET - Engineered Earthwork Type
    // ['EET','0',undefined,undefined], // Unknown
    ['EET','0','trench','unknown'], // Unknown
    ['EET','1','trench','battery'], // Battery
    ['EET','2','trench','parapet'], // Military Parapet
    ['EET','3','military','trench'], // Military Trench
    ['EET','4','trench','rampart'], // Rampart
    ['EET','5','trench','redoubt'], // Redoubt
    ['EET','999','trench','other'], // Other

    // ELA - Elevation Accuracy Category
    // ['ELA','0','source:accuracy:vertical:category','unknown'], // Unknown
    ['ELA','1','source:accuracy:vertical:category','accurate'], // Accurate
    ['ELA','2','source:accuracy:vertical:category','approximate'], // Approximate

    // ESC - Elevation Surface Category
    // ['ESC','0','source:accuracy:vertical:category','unknown'], // Unknown
    ['ESC','1','source:accuracy:vertical:category','land'], // Land
    ['ESC','2','source:accuracy:vertical:category','snow_field_and/or_ice-field'], // Snow Field and/or Ice-field
    ['ESC','4','source:accuracy:vertical:category','vegetation'], // Vegetation
    ['ESC','5','source:accuracy:vertical:category','inland_water'], // Inland Water
    ['ESC','6','source:accuracy:vertical:category','tidal_water'], // Tidal Water

    // FAC - Solid Maritime Construction
    // ['FAC','0',undefined,undefined], // Unknown
    ['FAC','0','solid_construction','unknown'], // Unknown
    ['FAC','1000','solid_construction','no'], // Open
    ['FAC','1001','solid_construction','yes'], // Solid Face

    // FFN - Feature Function from MGCP v4
    // ['FFN','0',undefined,undefined], // Good default?
    ['FFN','0','feature_function','unknown'], // Unknown
    ['FFN','2','use','agriculture'], // Farm?
    ['FFN','99','industrial','manufacturing'],
    ['FFN','105','use','oil-mill'],
    ['FFN','107','use','grain-milling'],
    ['FFN','161','craft','sawmilling'],
    ['FFN','171','industrial','paper_mill'],
    ['FFN','192','industrial','petroleum_refining'],
    ['FFN','241','industrial','steel'],
    ['FFN','290','industrial','motor_vehicleis'],
    ['FFN','330','repair','yes'],
    ['FFN','341','repair','aircraft'],
    ['FFN','343','shop','car_repair'],
    ['FFN','350','building','utilities'],
    ['FFN','351','use','power_generation'],
    ['FFN','355','use','cooling'],
    ['FFN','362','man_made','wastewater_plant'],
    ['FFN','440','building','commercial'],
    ['FFN','470','amenity','fuel'],
    ['FFN','475','amenity','marketplace'],
    ['FFN','480','building','transportation'],
    ['FFN','481','building','terminal'],
    ['FFN','486','building','signalling'],
    ['FFN','501','building','pumping'],
    ['FFN','511','building','maritime_pilotage'],
    ['FFN','513','building','harbour_control'],
    ['FFN','530','building','warehouse'],
    ['FFN','535','building','parking'],
    ['FFN','540','amenity','post_office'],
    ['FFN','550','building','accomodation'],
    ['FFN','563','building','residence'],
    ['FFN','572','amenity','restaurant'],
    ['FFN','582','building','print_publishing'],
    ['FFN','601','building','radio_broadcasting'],
    ['FFN','604','building','television_broadcasting'],
    ['FFN','610','building','telecommunications'],
    ['FFN','626','man_made','ground_station'],
    ['FFN','643','amenity','bank'],
    ['FFN','720','amenity','research_institute'],
    ['FFN','721','man_made','measurement_station'],
    ['FFN','722','man_made','weather_station'],
    ['FFN','730','building','wind_tunnel'],
    ['FFN','781','building','guard'],
    ['FFN','810','office','administrative'],
    ['FFN','811','office','government'],
    ['FFN','815','building','palace'],
    ['FFN','825','amenity','diplomacy'],
    ['FFN','827','amenity','embassy'],
    ['FFN','835','building','military'],
    ['FFN','841','amenity','police'],
    ['FFN','843','amenity','prison'],
    ['FFN','845','amenity','fire_station'],
    ['FFN','850','building','school'],
    ['FFN','855','building','university'],
    ['FFN','859','building','institute'],
    ['FFN','860','building','healthcare'],
    ['FFN','861','building','in-patient_care'],
    ['FFN','887','building','social_work'],
    ['FFN','891','amenity','theatre'],
    ['FFN','892','amenity','auditorium'],
    ['FFN','893','amenity','community_centre'],
    ['FFN','894','tourism','opera_house'],
    ['FFN','902','amenity','library'],
    ['FFN','905','tourism','museum'],
    ['FFN','907','use','botanical_and/or_zoological_reserve_activities'],
    ['FFN','912','leisure','sports_centre'],
    ['FFN','919','amenity','animal_boarding'],
    ['FFN','930','amenity','religious_activities'],
    ['FFN','931','amenity','place_of_worship'], // Needs extra tags
    ['FFN','955','leisure','yacht_club'],
    ['FFN','999','building','other'],

    // FFP - Farming Pattern
    // ['FFP','0',undefined,undefined], // Unknown
    ['FFP','0','farm:pattern','unknown'], // Unknown
    ['FFP','2','farm:pattern','regular'], // Regular
    ['FFP','3','farm:pattern','terraced'], // Terraced
    ['FFP','5','farm:pattern','intermingled_trees'], // Intermingled Trees
    ['FFP','6','farm:pattern','treeless'], // Treeless
    ['FFP','999','farm:pattern','other'], // Other

    // FHC - Harbour Facility Function
    // ['FHC','0',undefined,undefined], // Unknown
    ['FHC','0','harbour:use','unknown'], // Unknown
    ['FHC','10','harbour:use','container_terminal'], // Container Terminal
    ['FHC','3','harbour:use','ferry_terminal'], // Ferry Terminal
    ['FHC','4','harbour:use','fishing_harbour'], // Fishing Harbour
    ['FHC','5','harbour:use','marina'], // Marina
    ['FHC','6','harbour:use','naval_base'], // Naval Base
    ['FHC','7','harbour:use','tanker_terminal'], // Tanker Terminal
    ['FHC','999','harbour:use','other'], // Other

    // FIC - Embankment Type
    // ['FIC','0',undefined,undefined], // Unknown
    ['FIC','0','embankment:type','unknown'], // Unknown
    ['FIC','1','embankment','mound'], // Mound
    ['FIC','2','embankment','fill'], // Fill
    ['FIC','3','man-made','dyke'], // Dyke
    ['FIC','999','embankment:type','other'], // Other

    // FMM - Farming Method
    // ['FMM','0',undefined,undefined], // Unknown
    ['FMM','0','farm:method','unknown'], // Unknown
    ['FMM','1','farm:method','fallow'], // Fallow
    ['FMM','3','farm:method','permanent'], // Permanent
    ['FMM','4','farm:method','slash_and_burn'], // Slash and Burn
    ['FMM','5','farm:method','permanent_irrigation'], // Permanent Irrigation
    ['FMM','999','farm:method','other'], // Other

    // FPT - Airfield Type
    // ['FPT','0',undefined,undefined], // Unknown
    ['FPT','0','aeroway:airfield:type','unknown'], // Unknown
    ['FPT','1','aeroway:airfield:type','major'], // Major
    ['FPT','2','aeroway:airfield:type','minor_and_hard'], // Minor and Hard
    ['FPT','3','aeroway:airfield:type','minor_and_soft'], // Minor and Soft
    ['FPT','999','aeroway:airfield:type','other'], // Other

    // FRT - Weapons Range Type
    // ['FRT','0',undefined,undefined], // Unknown
    ['FRT','0','range:type','unknown'], // Unknown
    ['FRT','1','range:type','small_arms'], // Small Arms
    ['FRT','2','range:type','tank'], // Tank
    ['FRT','3','range:type','artillery'], // Field Artillery
    ['FRT','4','range:type','grenade'], // Grenade
    ['FRT','5','range:type','demolition'], // Demolition Area
    ['FRT','6','range:type','impact_area'], // Impact Area
    ['FRT','999','range:type','other'], // Other

    // FUC - Functional Use
    // ['FUC','0',undefined,undefined],
    ['FUC','0','use','unknown'],
    ['FUC','1','use','industrial'],
    ['FUC','2','use','commercial'],
    ['FUC','4','use','residential'],
    ['FUC','8','use','refugee'], // not great....
    ['FUC','13','use','recreation'],
    ['FUC','19','use','urbanised'], // Multi Use
    ['FUC','20','use','transportation'],
    ['FUC','999','use','other'],


    // FUN - Condition of Facility
    // ['FUN','0',undefined,undefined], // Null
    ['FUN','0','condition','unknown'],
    ['FUN','1','condition','construction'],
    ['FUN','2','condition','abandoned'],
    ['FUN','3','condition','destroyed'],
    ['FUN','4','condition','dismantled'],
    ['FUN','6','condition','functional'], //Fully Functional - default
    ['FUN','13','condition','damaged'],

    // GEC - Geopolitical Entity Type
    ['GEC','5','geopolitical_entity','area_of_no_sovereignty'], // Area of No Sovereignty
    ['GEC','7','geopolitical_entity','demilitarized_zone_(dmz)'], // Demilitarized Zone (DMZ)
    ['GEC','8','geopolitical_entity','zone_of_occupation'], // Zone of Occupation
    ['GEC','9','geopolitical_entity','leased_area'], // Leased Area
    ['GEC','10','geopolitical_entity','political_entity'], // Political Entity
    ['GEC','11','geopolitical_entity','dependent_political_entity'], // Dependent Political Entity
    ['GEC','12','geopolitical_entity','freely_associated_state'], // Freely Associated State
    ['GEC','13','geopolitical_entity','independent_political_entity'], // Independent Political Entity
    ['GEC','14','geopolitical_entity','semi-independent_political_entity'], // Semi-independent Political Entity
    ['GEC','15','geopolitical_entity','economic_region'], // Economic Region
    ['GEC','16','geopolitical_entity','territory'], // Territory
    ['GEC','17','geopolitical_entity','buffer_zone'], // Buffer Zone

    // GNC - Water Gate Type
    // ['GNC','0',undefined,undefined], // Unknown
    ['GNC','0','flood_control:type','unknown'], // Unknown
    ['GNC','2','flood_control:type','flood_gate'], // Flood Barrage Gate
    ['GNC','4','flood_control:type','lock_gate'], // Lock Gate
    ['GNC','5','flood_control:type','dyke_gate'], // Dyke Gate
    ['GNC','999','flood_control:type','other'], // Other

    // HAF - Helipad Accociated Facilities
    // ['HAF','0',undefined,undefined],
    ['HAF','0','helipad:facility','unknown'],
    ['HAF','1','helipad:facility','heliport'],
    ['HAF','2','helipad:facility','land_aerodrome'],
    ['HAF','3','helipad:facility','hospital'],
    ['HAF','4','helipad:facility','non-hospital_building'],
    ['HAF','6','helipad:facility','rig'],
    ['HAF','9','helipad:facility','offshore_construction'],
    ['HAF','999','helipad:facility','other'],

    // HCT - Thoroughfare Class
    ['HCT','0','highway_class','unknown'],
    ['HCT','1','highway','primary'], // Primary
    ['HCT','2','highway','secondary'], // Secondary
    ['HCT','3','highway','motorway'], // National Motorway
    ['HCT','4','highway','tertiary'], // Local

    // HQC - Hypsography Portrayal Type
    // ['HQC','0','contour:type','unknown'], // Unknown
    ['HQC','1','contour:type','index_contour'], // Index Contour
    ['HQC','2','contour:type','intermediate_contour'], // Intermediate Contour
    ['HQC','3','contour:type','half_auxiliary_contour'], // Half Auxiliary Contour
    ['HQC','5','contour:type','depression_index_contour'], // Depression Index Contour
    ['HQC','6','contour:type','depression_intermediate_contour'], // Depression Intermediate Contour
    ['HQC','8','contour:type','mound_index_contour'], // Mound Index Contour
    ['HQC','9','contour:type','mound_intermediate_contour'], // Mound Intermediate Contour
    ['HQC','14','contour:type','quarter_auxiliary_contour'], // Quarter Auxiliary Contour
    ['HQC','19','contour:type','intermediate_carrying_contour'], // Intermediate Carrying Contour
    ['HQC','20','contour:type','auxiliary_carrying_contour'], // Auxiliary Carrying Contour
    ['HQC','21','contour:type','index_carrying_contour'], // Index Carrying Contour
    ['HQC','22','contour:type','depression_auxiliary_contour'], // Depression Auxiliary Contour
    ['HQC','999','contour:type','other'], // Other

    // HWT - House of Worship Type
    // ['HWT','0',undefined,undefined],
    ['HWT','0','house_of_worship','unknown'],
    ['HWT','2','building','cathedral'],
    ['HWT','3','building','chapel'],
    ['HWT','4','building','church'],
    ['HWT','5','building','marabout'],
    ['HWT','6','with_minaret','yes'], // Fixed in pre/post processing
    ['HWT','7','building','religious_community'],
    ['HWT','9','building','mosque'],
    ['HWT','11','building','pagoda'],
    ['HWT','14','building','shrine'],
    ['HWT','15','building','tabernacle'],
    ['HWT','16','building','temple'],
    ['HWT','20','building','synagogue'],
    ['HWT','21','building','stupa'],
    ['HWT','998','building:religious','not_applicable'],
    ['HWT','999','building:religious','other'], // To avoid conflicts

    // HYP - Hydrologic Persistance
    // ['HYP','0',undefined,undefined],
    ['HYP','0','intermittent','unknown'],
    ['HYP','1','intermittent','no'], // Perennial
    ['HYP','2','intermittent','yes'],
    ['HYP','4','intermittent','dry'],
    ['HYP','998','intermittent','not_applicable'],

    // LFA - Aeronautical Light Function
    // ['LFA','0',undefined,undefined], // Unknown
    ['LFA','0','aeroway:light','unknown'], // Unknown
    ['LFA','15','aeroway:light','alsf-i'], // ALSF-I
    ['LFA','16','aeroway:light','alsf-ii'], // ALSF-II
    ['LFA','17','aeroway:light','ssalf'], // SSALF
    ['LFA','18','aeroway:light','ssalr'], // SSALR
    ['LFA','19','aeroway:light','malsf'], // MALSF
    ['LFA','20','aeroway:light','malsr'], // MALSR
    ['LFA','999','aeroway:light','other'], // Other

    // LOC - Vertical Relative Location
    // ['LOC','0',undefined,undefined],
    ['LOC','0','location','unknown'],
    // ['LOC','4','location','underwater'], // From DIGEST FACC - Below Water Surface
    ['LOC','17','location','on_waterbody_bottom'], // No OSM
    ['LOC','23','location','below_waterbody_bottom'], // No OSM
    // ['LOC','25','location','overground'], // From DIGEST FACC - Elevated above ground or water surface
    ['LOC','40','location','underground'],
    ['LOC','44','location','surface'], // The default is on the surface
    ['LOC','45','location','overground'], // Above ground
    ['LOC','46','location','underwater'], // Above waterbody bottom

    // LSP - Geopolitical Line Type
    ['LSP','1','geopolitical_line','administrative'], // Generic Administrative Boundary
    ['LSP','2','geopolitical_line','provisional_administrative'], // Provisional Administrative Line
    ['LSP','3','geopolitical_line','armistice'], // Armistice Line
    ['LSP','4','geopolitical_line','line_of_control'], // Line of Control
    ['LSP','5','geopolitical_line','demarcation'], // Demarcation Line
    ['LSP','6','geopolitical_line','line_of_convenience'], // Line of Convenience
    ['LSP','7','geopolitical_line','cease_fire'], // Cease Fire Line
    ['LSP','8','geopolitical_line','convention'], // Convention Line
    ['LSP','9','geopolitical_line','claim'], // Claim Line
    ['LSP','10','geopolitical_line','intercolonial'], // Intercolonial Line
    ['LSP','11','geopolitical_line','interentity'], // Interentity Line
    ['LSP','12','geopolitical_line','line_of_adjacency'], // Line of Adjacency
    ['LSP','13','geopolitical_line','line_of_withdrawal'], // Line of Withdrawal
    ['LSP','14','geopolitical_line','military_disengagement'], // Military Disengagement Line
    ['LSP','15','geopolitical_line','treaty'], // Treaty Line
    ['LSP','16','geopolitical_line','unclos_claim_boundary'], // UNCLOS Claim Boundary
    ['LSP','17','geopolitical_line','international_boundary'], // Generic International Boundary
    ['LSP','999','geopolitical_line','other'], // Other

    // MCC - Structural Material Type
    // ['MCC','0',undefined,undefined],
    ['MCC','0','material','unknown'],
    ['MCC','5','material','asphalt'],
    ['MCC','9','material','brick'],
    ['MCC','8','material','boulders'], // From SMC
    ['MCC','14','material','cinders'], // From SMC
    ['MCC','16','material','clay'],
    ['MCC','20','material','composition'],
    ['MCC','21','material','concrete'],
    ['MCC','22','material','conglomerate'],
    ['MCC','46','material','gravel'],
    ['MCC','52','material','lava'], // From SMC
    ['MCC','55','material','loess'], // From SMC
    ['MCC','57','material','macadam'],
    ['MCC','62','material','masonry'],
    ['MCC','64','material','metal'],
    ['MCC','65','material','mud'], // From SMC
    ['MCC','67','material','oil'], // From SMC
    ['MCC','72','material','part_metal'],
    ['MCC','73','material','pebbles'],
    ['MCC','77','material','prestressed_concrete'],
    ['MCC','83','material','reinforced_concrete'],
    ['MCC','84','material','rock'],
    ['MCC','85','material','rubber'], // From SMC
    ['MCC','86','material','rubble'],
    ['MCC','87','material','salt'], // From SMC
    ['MCC','88','material','sand'],
    ['MCC','103','material','frozen_water'], // From SMC
    ['MCC','104','material','soil'],
    ['MCC','108','material','stone'],
    ['MCC','117','material','wood'],
    ['MCC','120','material','sand_and_gravel'], // From SMC
    ['MCC','122','material','evaporite'], // From SMC
    ['MCC','124','material','sand_and_boulders'], // From SMC
    ['MCC','126','material','sand_and_mud'], // From SMC
    ['MCC','257','material','ice'], // From SMC
    ['MCC','258','material','snow'], // From SMC
    ['MCC','269','material','limestone'], // From SMC
    ['MCC','274','material','sod'], // From SMC
    ['MCC','998','material','not_applicable'],
    ['MCC','999','material','other'],

    // MES - Median Present
    // ['MES','0',undefined,undefined],
    ['MES','0','median','unknown'],
    ['MES','1000','median','no'],
    ['MES','1001','median','yes'],

    // MZN - Extraction Mine type TRDv4, was MIN in TRDv3
    // ['MZN','0',undefined,undefined],
    ['MZN','0','mine:type','unknown'],
    ['MZN','3','mine:type','opencast'],
    ['MZN','4','mine:type','placer'],
    ['MZN','5','mine:type','prospect'],
    ['MZN','6','mine:type','strip-mine'],
    ['MZN','8','mine:type','peatery'],
    ['MZN','9','mine:type','below_surface'],

    // NST - Navigation System Type
    // ['NST','0',undefined,undefined],
    ['NST','0','beacon:type','unknown'],
    ['NST','12','beacon:type','radio'],
    ['NST','13','beacon:type','radio_telephone'],
    ['NST','15','beacon:type','television'],
    ['NST','16','beacon:type','microwave'],
    ['NST','17','beacon:type','ndb'], // non-directional_radio_beacon
    ['NST','18','beacon:type','ndb-dme'], // non-directional_radio_beacon/distance_measuring_equipment
    ['NST','20','beacon:type','vor'], // vhf_omni_directional_radio_range
    ['NST','21','beacon:type','vor-dme'], // vhf_omni_directional_radio_range/distance_measuring_equipment
    ['NST','22','beacon:type','vortac'], // vhf_omni_directional_radio_range_and_tacan
    ['NST','23','beacon:type','tacan'], // tactical_air_navigation_equipment tacan
    ['NST','24','beacon:type','ils'], // instrument_landing_system
    ['NST','25','beacon:type','ils-dme'], // instrument_landing_system/distance_measuring_equipment
    ['NST','26','beacon:type','localizer'],
    ['NST','27','beacon:type','localizer/distance_measuring_equipment'],
    ['NST','30','beacon:type','microwave_landing_system'],
    ['NST','33','beacon:type','radio_telegraph'],
    ['NST','35','beacon:type','radar_antenna'],
    ['NST','37','beacon:type','precision_approach_radar'],
    ['NST','38','beacon:type','aeronautical_radio'],
    ['NST','58','beacon:type','dme'], // distance_measuring_equipment
    ['NST','74','beacon:type','ils_back_course'],
    ['NST','75','beacon:type','loc_back_course'],
    ['NST','76','beacon:type','marker'],
    ['NST','78','beacon:type','outer_marker'],
    ['NST','79','beacon:type','middle_marker'],
    ['NST','80','beacon:type','locator_outer_marker'],
    ['NST','81','beacon:type','locator_middle_marker'],
    ['NST','82','beacon:type','rotating_beacon_light'],
    ['NST','999','beacon:type','other'],

    // OPC - Offshore Platform Type
    // ['OPC','0',undefined,undefined], // Unknown
    ['OPC','0','offshore_construction','unknown'], // Unknown
    ['OPC','2','offshore_construction','production_platform'], // Production Platform
    ['OPC','3','offshore_construction','observation_platform'], // Observation Platform
    ['OPC','4','offshore_construction','articulated_loading_platform_(alp)'], // Articulated Loading Platform (ALP)
    ['OPC','9','offshore_construction','accommodation_platform'], // Accommodation Platform
    ['OPC','10','offshore_construction','navigation_aid_support'], // Navigation Aid Support
    ['OPC','999','offshore_construction','other'], // Other

    // ORD - Relative Importance
    // ['ORD','0',undefined,undefined],
    ['ORD','0','place:importance','unknown'],
    ['ORD','1','place:importance','first'],
    ['ORD','2','place:importance','second'],
    ['ORD','3','place:importance','third'],
    ['ORD','4','place:importance','fourth'],
    ['ORD','5','place:importance','fifth'],
    ['ORD','6','place:importance','sixth'], // From data, not in spec
    ['ORD','999','place:importance','other'],

    // PBY - By-product
    // ['PBY','0',undefined,undefined], // Unknown
    ['PBY','0','by_product','unknown'], // Unknown
    ['PBY','8','by_product','refuse'], // Refuse
    ['PBY','15','by_product','slag'], // Slag
    ['PBY','17','by_product','spoil'], // Spoil
    ['PBY','999','by_product','other'], // Other

    // PPC - Power Station Type
    // ['PPC','0',undefined,undefined], // Unknown
    ['PPC','0','plant:source','unknown'], // Unknown
    ['PPC','1','plant:source','hydro'], // Hydro-electric
    ['PPC','2','plant:source','nuclear'], // Nuclear
    ['PPC','3','plant:source','solar'], // Solar
    ['PPC','4','plant:source','geothermal'], // Geothermal
    ['PPC','5','plant:source','wind'], // Wind
    ['PPC','6','plant:source','tidal'], // Tidal
    ['PPC','7','plant:source','thermal'], // Thermal
    ['PPC','999','plant:source','other'], // Other

    // PPO Product
    // ['PPO','0',undefined,undefined],
    ['PPO','0','product','unknown'],
    ['PPO','1','product','aircraft'],
    ['PPO','2','product','aluminium'],
    ['PPO','3','product','ammunition'],
    ['PPO','4','product','asphalt'],
    ['PPO','9','product','bauxite'],
    ['PPO','13','product','brick'],
    ['PPO','15','product','cement'],
    ['PPO','16','product','chemical'],
    ['PPO','17','product','clay'],
    ['PPO','18','product','coal'],
    ['PPO','21','product','coke'],
    ['PPO','23','product','concrete'],
    ['PPO','26','product','copper'],
    ['PPO','33','product','diamond'],
    ['PPO','35','product','dolomite'],
    ['PPO','38','product','explosive'],
    ['PPO','39','product','fish'],
    ['PPO','41','product','food'],
    ['PPO','45','product','gas'],
    ['PPO','46','product','petrol'],
    ['PPO','47','product','glass'],
    ['PPO','48','product','gold'],
    ['PPO','50','product','granite'],
    ['PPO','53','product','gravel'],
    ['PPO','57','product','ice'],
    ['PPO','58','product','iron'],
    ['PPO','59','product','lead'],
    ['PPO','63','product','lumber'],
    ['PPO','65','product','manganese'],
    ['PPO','66','product','marble'],
    ['PPO','69','product','metal'],
    ['PPO','75','product','oil'],
    ['PPO','79','product','palm'],
    ['PPO','80','product','paper'],
    ['PPO','83','product','petroleum'],
    ['PPO','84','product','plastic'],
    ['PPO','89','product','quartz'],
    ['PPO','90','product','radioactive_material'],
    ['PPO','93','product','rock'],
    ['PPO','94','product','rubber'],
    ['PPO','95','product','salt'],
    ['PPO','96','product','sand'],
    ['PPO','101','product','sewage'],
    ['PPO','105','product','silver'],
    ['PPO','106','product','snow'],
    ['PPO','109','product','steel'],
    ['PPO','110','product','stone'],
    ['PPO','111','product','sugar'],
    ['PPO','114','product','textile'],
    ['PPO','117','product','tobacco'],
    ['PPO','120','product','uranium'],
    ['PPO','121','product','vegetation_product'],
    ['PPO','122','product','water'],
    ['PPO','124','product','wood'],
    ['PPO','126','product','zinc'],
    ['PPO','129','product','madrepore'],
    ['PPO','136','product','biochemical'],
    ['PPO','137','product','petrochemical'],
    ['PPO','996','product','multiple'],
    ['PPO','999','product','other'],

    // PRM - Permanent
    // ['PRM','0',undefined,undefined], // Unknown
    ['PRM','0','permanent','unknown'], // Unknown
    ['PRM','1000','permanent','no'], // Temporary
    ['PRM','1001','permanent','yes'], // Permanent

    // PWC - Shoreline Construction Type
    //['PWC','0','raw:PWC','unknown'], // Unknown
    ['PWC','0','shoreline_construction:type','unknown'], // Unknown
    ['PWC','1','man_made','pier'], // Pier
    ['PWC','2','waterway','dock'], // Wharf
    ['PWC','3','man_made','quay'], // Quay

    // RAC - Radar Antenna Configuration
    // ['RAC','0',undefined,undefined], // Unknown
    ['RAC','0','radar:configuration','unknown'], // Unknown
    ['RAC','1','radar:configuration','dome_enclosed'], // Dome Enclosed
    ['RAC','2','radar:configuration','mast_mounted'], // Mast Mounted
    ['RAC','4','radar:configuration','radome_on_tower'], // Radome on Tower
    ['RAC','5','radar:configuration','scanner'], // Scanner
    ['RAC','6','radar:configuration','tower_mounted'], // Tower Mounted
    ['RAC','999','radar:configuration','other'], // Other

    // RAY - Racing Type
    // ['RAY','0',undefined,undefined], // Unknown
    ['RAY','0','race:type','unknown'], // Unknown
    ['RAY','1','race:type','bicycle'], // Bicycle
    ['RAY','2','race:type','camel'], // Camel
    ['RAY','3','race:type','greyhound'], // Greyhound
    ['RAY','4','race:type','harness'], // Harness
    ['RAY','5','race:type','horse'], // Horse
    ['RAY','6','race:type','ice_skate'], // Ice Skate
    ['RAY','7','race:type','motor_vehicle'], // Motor Vehicle
    ['RAY','8','race:type','roller_skate'], // Roller Skate
    ['RAY','9','race:type','track_and_field'], // Track and Field
    ['RAY','999','race:type','other'], // Other

    // REL - Religious Designation
    // ['REL','0',undefined,undefined],
    ['REL','0','religion','unknown'],
    ['REL','1','religion','buddhist'],
    ['REL','2','religion','muslim'],
    ['REL','4','religion','christian'],
    ['REL','5','religion','jewish'],
    ['REL','999','religion','other'],

    // RGC - Railway Gauge Classification
    // ['RGC','0',undefined,undefined],
    ['RGC','0','gauge:type','unknown'],
    ['RGC','1','gauge:type','broad'],
    ['RGC','2','gauge:type','narrow'],
    ['RGC','3','gauge:type','standard'],
    // ['RGC','6','railway','monorail'], // Gauge = 0.5?
    ['RGC','6','railway:track','monorail'], // moved to "railway=monorail" in pre/post processing

    // RIR - Railway in Road
    // ['RIR','0',undefined,undefined],
    ['RIR','0','railway:in_road','unknown'],
    ['RIR','1000','railway:in_road','separated'],
    ['RIR','1001','railway:in_road','yes'],

    // RKF - Rock Formation Structure
    // ['RKF','0',undefined,undefined], // Unknown
    ['RKF','0','rock_formation','unknown'], // Unknown
    ['RKF','1','rock_formation','columnar'], // Columnar
    ['RKF','3','rock_formation','pinnacle'], // Pinnacle
    ['RKF','999','rock_formation','other'], // Other

    // RRA - Railway Power Method
    // ['RRA','0',undefined,undefined],
    ['RRA','0','electrified','unknown'],
    ['RRA','1','electrified','rail'], // Electrified Track
    ['RRA','3','electrified','contact_line'], // Overhead Electrified
    ['RRA','4','electrified','no'],

    // RRC - Railway Use
    ['RRC','0','railway','rail'],
    ['RRC','2','railway','carline'],
    ['RRC','6','railway','subway'],
    ['RRC','8','railway','logging'],
    ['RRC','14','railway','tram'],
    ['RRC','15','railway','funicular'],
    ['RRC','32','automated_transit_system','yes'],
    ['RRC','33','railway','longhaul'],
    ['RRC','999','railway','other'],

    // RSA - Branch Railway Type
    // ['RSA','0',undefined,undefined],
    ['RSA','0','service','unknown'],
    ['RSA','1','service','spur'],
    ['RSA','2','service','siding'],
    ['RSA','3','service','passing'],

    // RST - Load Bearing Surface Type
    // ['RST','0',undefined,undefined],
    ['RST','0','surface','unknown'],
    ['RST','1','surface','paved'],
    ['RST','2','surface','unpaved'],
    ['RST','5','surface','grass'], // DIGEST FACC from Data: Grass/Sod
    ['RST','6','surface','ground'], // DIGEST FACC from Data: Natural
    ['RST','8','surface','earth'], // Temporary - same as ground?
    ['RST','999','surface','other'],

    // RTA - Linear Feature Arrangement
    // ['RTA','0',undefined,undefined], // This also gets used by pipelines
    ['RTA','0','arrangement','unknown'], // Unknown
    ['RTA','1','arrangement','single'],
    ['RTA','2','arrangement','double'],
    ['RTA','3','arrangement','multiple'],
    ['RTA','4','arrangement','juxtaposition'],

    // RWC - Railway Class
    ['RWC','1','usage','high_speed_rail'],
    ['RWC','2','usage','main'],
    ['RWC','3','usage','branch'],

    // SCC - Water Type
    // ['SCC','0',undefined,undefined],
    ['SCC','0','water:type','unknown'],
    ['SCC','1','water:type','alkaline'],
    ['SCC','4','water:type','mineral'],
    ['SCC','9','water:type','potable'],
    ['SCC','10','water:type','salt'],
    ['SCC','11','water:type','fresh'], // Fresh
    ['SCC','12','water:type','brackish'], // Brackish
    ['SCC','998','water:type','not_applicable'],
    ['SCC','999','water:type','other'],

    // SDT - Sand Dune Type
    // ['SDT','0',undefined,undefined], // Unknown
    ['SDT','0','dune:type','unknown'], // Unknown
    ['SDT','1','dune:type','crescent'], // Crescent
    ['SDT','2','dune:type','dome'], // Dome
    ['SDT','3','dune:type','transverse'], // Transverse
    ['SDT','4','dune:type','linear'], // Linear
    ['SDT','5','dune:type','parabolic'], // Parabolic
    ['SDT','6','dune:type','ripple'], // Ripple
    ['SDT','7','dune:type','star'], // Star
    ['SDT','8','dune:type','dome_and_transverse'], // Dome and Transverse

    // SEP - Divided
    // ['SEP','0',undefined,undefined],
    ['SEP','0','divider','unknown'],
    ['SEP','1000','is_divided','no'],
    ['SEP','1001','is_divided','yes'],

    // SFS - Aerodrome Pavement Functional
    // ['SFS','0',undefined,undefined], // Unknown
    ['SFS','0','aeroway:pavement:status','unknown'], // Unknown
    ['SFS','1','aeroway:pavement:status','fair'], // Fair
    ['SFS','3','aeroway:pavement:status','good'], // Good
    ['SFS','5','aeroway:pavement:status','poor'], // Poor
    ['SFS','998','aeroway:pavement:status','not_applicable'],

    // SLT - Shoreline Type
    // ['SLT','0',undefined,undefined], // Unknown
    ['SLT','0','shoreline:type','unknown'], // Unknown
    ['SLT','6','shoreline:type','mangrove'], // Mangrove
    ['SLT','8','shoreline:type','marshy'], // Marshy
    ['SLT','10','shoreline:type','stony'], // Stony
    ['SLT','11','shoreline:type','building_rubble'], // Building Rubble
    ['SLT','13','shoreline:type','sandy'], // Sandy
    ['SLT','14','shoreline:type','shingly'], // Shingly
    ['SLT','999','shoreline:type','other'], // Other


    // SRT - Source Type (MGCP v4)
    // ['SRT','0',undefined,undefined],
    ['SRT','0','source:name','unknown'],
    ['SRT','1','source:name','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG)
    ['SRT','2','source:name','automated_aeronautical_facilities_information_file_(aafif)'], //  Automated Aeronautical Facilities Information File (AAFIF)
    ['SRT','3','source:name','chart_update_manual_(chum)'], // CHUM
    ['SRT','4','source:name','city_graphic'], // City Graphic
    ['SRT','5','source:name','combat_chart'], // Combat Chart
    ['SRT','6','source:name','compressed_arc_digitized_raster_graphic_(cadrg)'], // Compressed Arc Digitized Raster Graphic (CADRG)
    ['SRT','7','source:name','controlled_imagery_base_1_(cib1)'], // Controlled Imagery Base 1 (CIB1)
    ['SRT','8','source:name','controlled_imagery_base_5_(cib5)'], // Controlled Imagery Base 5 (CIB5)
    ['SRT','10','source:name','digital_nautical_chart_(dnc)'], // Digital Nautical Chart (DNC)
    ['SRT','11','source:name','digital_print_file_(dpf)'], // Digital Print File (DPF)
    ['SRT','16','source:name','digital_topographic_data_1_(dtop1)'], // Digital Topographic Data 1 (DTOP1)
    ['SRT','17','source:name','digital_topographic_data_2_(dtop2)'], // Digital Topographic Data 2 (DTOP2)
    ['SRT','18','source:name','digital_topographic_data_3_(dtop3)'], // Digital Topographic Data 3 (DTOP3)
    ['SRT','19','source:name','digital_topographic_data_4_(dtop4)'], // Digital Topographic Data 4 (DTOP4)
    ['SRT','20','source:name','digital_topographic_data_5_(dtop5)'], // Digital Topographic Data 5 (DTOP5)
    ['SRT','21','source:name','digital_vertical_obstruction_file_(dvof)'], // Digital Vertical Obstruction File (DVOF)
    ['SRT','22','source:name','foundation_feature_data_(ffd)'], // Foundation Feature Data (FFD)
    ['SRT','24','source:name','geocover/landcover'], // GeoCover/LandCover
    ['SRT','25','source:name','geonames'], // GeoNames
    ['SRT','26','source:name','gps_field_collected_open_source'], // GPS Field Collected Open Source
    ['SRT','27','source:name','image_city_map_(icm)'], // Image City Map (ICM)
    ['SRT','29','source:name','ntm_imagery'], // Imagery (NTM)
    ['SRT','30','source:name','imagery'], // Imagery (Other)
    ['SRT','31','source:name','interim_terrain_data_(itd)'], // Interim Terrain Data (ITD)
    ['SRT','32','source:name','interim_vector_data_(ivd)'], // Interim Vector Data (IVD)
    ['SRT','34','source:name','joint_operational_graphic_(jog)'], // Joint Operational Graphic (JOG)
    ['SRT','33','source:name','international_boundaries'],
    ['SRT','36','source:name','littoral_warfare_data_(lwd)'], // Littoral Warfare Data (LWD)
    ['SRT','37','source:name','mapping_charting_and_geodetic_(mc&g)'], // Mapping Charting and Geodetic (MC&G)
    ['SRT','38','source:name','mission_specific_data_1_(msd1)'], // Mission Specific Data 1 (MSD1)
    ['SRT','39','source:name','mission_specific_data_2_(msd2)'], // Mission Specific Data 2 (MSD2)
    ['SRT','40','source:name','mission_specific_data_3_(msd3)'], // Mission Specific Data 3 (MSD3)
    ['SRT','41','source:name','mission_specific_data_4_(msd4)'], // Mission Specific Data 4 (MSD4)
    ['SRT','42','source:name','mission_specific_data_5_(msd5)'], // Mission Specific Data 5 (MSD5)
    ['SRT','43','source:name','modernized_integrated_data_base_(midb)'], // Modernized Integrated Data Base (MIDB)
    ['SRT','44','source:name','native_data_source'], // Native Data Source
    ['SRT','45','source:name','native_map_source'], // Native Map Source
    ['SRT','46','source:name','medical_facilities'],
    ['SRT','47','source:name','nominally_attributed_topographic_evaluation_map_(nate_map)'], //  Nominally Attributed Topographic Evaluation Map (NATE Map)
    ['SRT','48','source:name','planning_graphic'], // Planning Graphic
    ['SRT','50','source:name','shuttle_radar_topographic_mission_(srtm)'], // Shuttle Radar Topographic Mission (SRTM)
    ['SRT','51','source:name','site_map/nuclear_facility'], // Site Map/Nuclear Facility
    ['SRT','52','source:name','tactical_oceanographic_data_lv_0_(tod0)'], // Tactical Oceanographic Data Lv 0 (TOD0)
    ['SRT','53','source:name','tactical_oceanographic_data_lv_1_(tod1)'], // Tactical Oceanographic Data Lv 1 (TOD1)
    ['SRT','54','source:name','tactical_oceanographic_data_lv_2_(tod2)'], // Tactical Oceanographic Data Lv 2 (TOD2)
    ['SRT','55','source:name','tactical_oceanographic_data_lv_3_(tod3)'], // Tactical Oceanographic Data Lv 3 (TOD3)
    ['SRT','56','source:name','tactical_oceanographic_data_lv_4_(tod4)'], // Tactical Oceanographic Data Lv 4 (TOD4)
    ['SRT','57','source:name','tactical_oceanographic_data_lv_5_(tod5)'], // Tactical Oceanographic Data Lv 5 (TOD5)
    ['SRT','59','source:name','united_nations_data'], // United Nations Data
    ['SRT','60','source:name','urban_vector_map_(uvmap)'], // Urban Vector Map (UVMap)
    ['SRT','61','source:name','vector_interim_terrain_data_(vitd)'], // Vector Interim Terrain Data (VITD)
    ['SRT','62','source:name','vector_map_lv0_(vmap0)'], // Vector Map Lv0 (VMap0)
    ['SRT','63','source:name','vector_map_lv1_(vmap1)'], // Vector Map Lv1 (VMap1)
    ['SRT','64','source:name','vector_map_lv2_(vmap2)'], // Vector Map Lv2 (VMap2)
    ['SRT','65','source:name','world_vector_shoreline_plus_(wvsplus)'], // World Vector Shoreline Plus (WVSPlus)
    ['SRT','85','source:name','spot_hrg_digital_terrain_model'],
    ['SRT','92','source:name','vector_data'],
    ['SRT','93','source:name','1:25k_vector_data'],
    ['SRT','94','source:name','1:50k_vector_data'],
    ['SRT','95','source:name','1:100k_vector_data'],
    ['SRT','110','source:name','very_high_resolution_commercial_monoscopic_imagery'],
    ['SRT','111','source:name','very_high_resolution_commercial_stereoscopic_imagery'],
    ['SRT','112','source:name','high_resolution_commercial_monoscopic_imagery'],
    ['SRT','113','source:name','high_resolution_commercial_stereoscopic_imagery'],
    ['SRT','114','source:name','medium_resolution_commercial_monoscopic_imagery'],
    ['SRT','115','source:name','medium_resolution_commercial_stereoscopic_imagery'],
    ['SRT','116','source:name','low_resolution_commercial_monoscopic_imagery'],
    ['SRT','117','source:name','low_resolution_commercial_stereoscopic_imagery'],
    ['SRT','118','source:name','map_1:25k'],
    ['SRT','119','source:name','map_1:50k'],
    ['SRT','120','source:name','map_1:100k'],
    ['SRT','121','source:name','routing_data'],
    ['SRT','996','source:name','multiple'], // Multiple
    ['SRT','997','source:name','unpopulated'],
    ['SRT','998','source:name','not_applicable'],
    ['SRT','999','source:name','other'],

    // SSC - Structure Shape
    // ['SSC','0',undefined,undefined], // Unknown
    ['SSC','0','shape','unknown'], // Unknown
    ['SSC','2','shape','horizontal_capped_cylindrical'], // Horizontal Capped Cylindrical
    ['SSC','4','shape','vertical_capped_cylindrical'], // Vertical Capped Cylindrical
    ['SSC','12','shape','pyramidal'], // Pyramidal
    ['SSC','17','shape','spherical'], // Spherical
    ['SSC','59','shape','cylindrical_with_framework'], // Cylindrical with Framework
    ['SSC','65','shape','cylindrical_with_flat_top'], // Cylindrical with Flat Top
    ['SSC','77','shape','arched'], // Arched
    ['SSC','88','shape','spherical_on_column'], // Spherical on Column
    ['SSC','97','shape','statue'], // Statue
    ['SSC','109','shape','obelisk'], // Obelisk
    ['SSC','999','shape','other'], // Other

    // STA - Maritime Station Type
    // ['STA','0',undefined,undefined], // Unknown
    ['STA','0','seamark:signal_station','unknown'], // Unknown
    ['STA','1','seamark:signal_station','coast_guard'], // Coast Guard Station
    ['STA','11','seamark:signal_station','pilot'], // Pilot Station
    ['STA','13','seamark:signal_station','signal'], // Signal Station
    ['STA','15','seamark:signal_station','storm_signal'], // Storm Signal Station
    ['STA','17','seamark:signal_station','tide'], // Tide Station
    ['STA','19','seamark:signal_station','time_signal'], // Time Signal Station
    ['STA','2','seamark:signal_station','fireboat'], // Fireboat Station
    ['STA','21','seamark:signal_station','weather_signal'], // Weather Signal Station
    ['STA','27','seamark:signal_station','tidal_stream'], // Tidal Stream Station
    ['STA','28','seamark:signal_station','traffic_signal'], // Traffic Signal Station
    ['STA','29','seamark:signal_station','bridge_signal'], // Bridge Signal Station
    ['STA','3','seamark:signal_station','water-police'], // Water-police Station
    ['STA','30','seamark:signal_station','lock_signal'], // Lock Signal Station
    ['STA','32','seamark:signal_station','international_port_signals'], // International Port Signals Station
    ['STA','33','seamark:signal_station','military_practice_signal'], // Military Practice Signal Station
    ['STA','35','seamark:signal_station','warning_signal'], // Warning Signal Station
    ['STA','36','seamark:signal_station','radar_surveillance'], // Radar Surveillance Station
    ['STA','37','seamark:signal_station','pilot_lookout'], // Pilot Lookout Station
    ['STA','4','seamark:signal_station','ice_signal'], // Ice Signal Station
    ['STA','5','seamark:signal_station','rescue'], // Rescue Station
    ['STA','6','seamark:signal_station','port_control'], // Port Control Station
    ['STA','999','seamark:signal_station','other'], // Other

    // SUC - Shed Type
    // ['SUC','0',undefined,undefined],
    ['SUC','0','shed_type','unknown'],
    ['SUC','1','man_made','snow_fence'],
    ['SUC','2','man_made','rock_shed'],
    ['SUC','4','man_made','rock_and_snow_fence'],
    ['SUC','999','protection:type','other'],

    // SUY - Survey Point Type
    // ['SUY','0','survey_point:type','unknown'], // Unknown
    ['SUY','1','survey_point:type','astronomic_position'], // Astronomic Position
    ['SUY','2','survey_point:type','benchmark'], // Benchmark
    ['SUY','3','survey_point:type','cadastral_control_point'], // Cadastral Control Point
    ['SUY','4','survey_point:type','camera_station'], // Camera Station
    ['SUY','5','survey_point:type','geodetic_point'], // Geodetic Point
    ['SUY','6','survey_point:type','gravity_point'], // Gravity Point
    ['SUY','7','survey_point:type','magnetic_station'], // Magnetic Station
    ['SUY','8','survey_point:type','theodolite_station'], // Theodolite Station
    ['SUY','9','survey_point:type','tidal_benchmark'], // Tidal Benchmark
    ['SUY','999','survey_point:type','other'], // Other

    // SWT - Natural Pool Type
    // ['SWT','0',undefined,undefined], // Unknown
    ['SWT','0','spring:type','unknown'], // Unknown
    ['SWT','4','spring:type','spring'], // Spring - Will remove this in post processing
    ['SWT','5','water','water-hole'], // Water-hole
    ['SWT','999','spring:type','other'], // Other

    // THC - Thematic Classification
    // ['THC','0','thematic_classification','unknown'], // Unknown
    ['THC','1','thematic_classification','aeronautical'], // Aeronautical
    ['THC','2','thematic_classification','vegetation'], // Vegetation
    ['THC','3','thematic_classification','utilities'], // Utilities
    ['THC','4','thematic_classification','population'], // Population
    ['THC','5','thematic_classification','physiography'], // Physiography
    ['THC','6','thematic_classification','industry'], // Industry
    ['THC','7','thematic_classification','ground_transportation'], // Ground Transportation
    ['THC','8','thematic_classification','elevation'], // Elevation
    ['THC','9','thematic_classification','boundaries'], // Boundaries
    ['THC','10','thematic_classification','waterbodies'], // Waterbodies
    ['THC','11','thematic_classification','maritime'], // Maritime

    // TID - Tide Influenced
    // ['TID','0',undefined,undefined],
    ['TID','0','tidal','unknown'],
    ['TID','1000','tidal','no'],
    ['TID','1001','tidal','yes'],

    // TOS - Tower Shape
    // ['TOS','0',undefined,undefined], // Unknown
    ['TOS','0','tower:shape','unknown'], // Unknown
    ['TOS','1','tower:shape','a_frame'], // 'A' Frame
    ['TOS','2','tower:shape','h_frame'], // 'H' Frame
    ['TOS','3','tower:shape','i_frame'], // 'I' Frame
    ['TOS','4','tower:shape','t_frame'], // 'I' Frame
    ['TOS','5','tower:shape','y_frame'], // 'Y' Frame
    ['TOS','6','tower:shape','mast'], // Mast
    ['TOS','999','tower:shape','other'], // Other

    // TRE - Foliage Type
    // ['TRE','0',undefined,undefined], // Unknown
    ['TRE','0','leaf_cycle','unknown'], // Unknown
    ['TRE','1','leaf_cycle','deciduous'], // Deciduous
    ['TRE','2','leaf_cycle','evergreen'], // Evergreen
    ['TRE','3','leaf_cycle','mixed'], // Mixed

    // TRS - Transportation System Type: See BH070 Ford
    // ['TRS','0',undefined,undefined],
    ['TRS','0','transport:type','unknown'],
    ['TRS','1','transport:type','air'],
    ['TRS','2','transport:type','aqueduct'],
    ['TRS','4','transport:type','bus'],
    ['TRS','5','transport:type','canal'],
    ['TRS','7','transport:type','maritime'],
    ['TRS','9','transport:type','pedestrian'],
    ['TRS','10','transport:type','pipeline'],
    ['TRS','12','transport:type','railway'],
    ['TRS','13','transport:type','road'],
    ['TRS','998','transport:type','not_applicable'],
    ['TRS','999','transport:type','other'],

    // TTC - Tower Type
    // ['TTC','0',undefined,undefined],
    ['TTC','0','tower:type','unknown'],
    ['TTC','1','tower:type','bridge'], // Bridge Tower
    ['TTC','2','tower:type','observation'], // Observation Tower
    ['TTC','5','tower:type','light'], // Light Tower
    // ['TTC','6','tower:type','water'], // Water Tower
    ['TTC','6','man_made','water_tower'], // Water Tower
    ['TTC','8','tower:type','cooling'], // Cooling Tower
    ['TTC','10','tower:type','lookout'], // Lookout Tower
    ['TTC','12','tower:type','fire'], // Fire Tower
    // ['TTC','16','tower:type','control'], // Control Tower
    ['TTC','16','service','aircraft_control'], // Control Tower
    ['TTC','18','tower:type','communication'], // Telecommunication Tower
    ['TTC','19','tower:type','cell'], // Cell Tower
    ['TTC','21','tower:type','guard'], // Guard Tower
    ['TTC','22','tower:type','industrial'], // Industrial Tower
    ['TTC','23','tower:type','drop'], // Drop Tower
    ['TTC','26','tower:type','mine_shaft_superstructure'], // Mine Shaft Superstructure Tower
    ['TTC','999','tower:type','other'], // Other

    // TUC - Transportation Use
    // ['TUC','0',undefined,undefined],
    ['TUC','0','transport:use','unknown'],
    ['TUC','25','transport:use','cargo'],
    ['TUC','26','transport:use','passenger'],
    ['TUC','45','transport:use','general'],
    ['TUC','999','transport:use','other'],

    // USE_ - Usage
    // ['USE_','0','use','unknown'], // Unknown
    ['USE_','7','use','tribal'], // Tribal
    ['USE_','16','use','city'], // City
    ['USE_','23','use','international'], // International
    ['USE_','26','use','primary/1st_order'], // Primary/1st Order
    ['USE_','30','use','secondary/2nd_order'], // Secondary/2nd Order
    ['USE_','31','use','tertiary/3rd_order'], // Tertiary/3rd Order
    ['USE_','32','use','insular'], // Insular
    ['USE_','70','use','reserve/reservation'], // Reserve/Reservation
    ['USE_','141','use','forest_preserve'], // Forest Preserve
    ['USE_','155','use','prohibited_area'], // Prohibited Area
    ['USE_','999','use','other'], // Other

    // VEG - Vegetation Characteristic
    // ['VEG','0',undefined,undefined], // Unknown
    ['VEG','0','vegetation:characteristic','unknown'], // Unknown
    ['VEG','50','swamp:vegetation:type','wood'], // Wood  - In MGCP Spec but no details about it
    ['VEG','51','swamp:vegetation:type','herb_and_brush'], // Herb and Brush
    ['VEG','75','swamp:vegetation:type','brush'], // Brush
    ['VEG','8','grassland:type','grassland'], // Grassland
    ['VEG','9','grassland:type','grassland_with_trees'], // Grassland with Trees
    ['VEG','999','vegetation:characteristic','other'], // Other

    // VGT - Volcano Shape
    // ['VGT','0',undefined,undefined], // Unknown
    ['VGT','0','volcano:type','unknown'], // Unknown
    ['VGT','1','volcano:type','cone'], // Cone
    ['VGT','2','volcano:type','cinder_cone'], // Cinder Cone
    ['VGT','3','volcano:type','shield'], // Shield
    ['VGT','4','volcano:type','caldera'], // Caldera
    ['VGT','5','volcano:type','composite'], // Composite
    ['VGT','999','volcano:type','other'], // Other

    // VSP - Vegetation Species
    // ['VSP','0',undefined,undefined], // Unknown
    ['VSP','0','vegetation:type','unknown'], // Unknown
    ['VSP','10','vegetation:type','eucalyptus'], // Eucalyptus
    ['VSP','11','vegetation:type','filao'], // Filao
    ['VSP','22','vegetation:type','nipa'], // Nipa
    ['VSP','25','vegetation:type','palm'], // Palm
    ['VSP','4','vegetation:type','casuarina'], // Casuarina
    ['VSP','50','vegetation:type','umbrella_thorn'], // Umbrella Thorn
    ['VSP','6','vegetation:type','conifer'], // Conifer
    ['VSP','8','vegetation:type','cypress'], // Cypress
    ['VSP','999','vegetation:type','other'], // Other

    // WCC - Watercourse Channel Type
    ['WCC','0','channel:type','unknown'],
    ['WCC','1','channel:type','channelized_stream'],
    ['WCC','2','channel:type','braided_stream'],
    ['WCC','3','channel:type','gorge'],
    ['WCC','4','wadi','yes'],
    ['WCC','7','channel:type','normal'],

    // WFT - Well Type
    // ['WFT','0',undefined,undefined],
    ['WFT','0','water_well:type','unknown'],
    ['WFT','2','water_well:type','walled-in'],
    ['WFT','3','water_well:type','artesian'],
    ['WFT','6','water_well:type','dug'],
    ['WFT','7','water_well:type','drilled'],
    ['WFT','999','water_well:type','other'],

    // WLE - Water Level Effect
    // ['WLE','0',undefined,undefined], // Unknown
    ['WLE','0','hydrographic_vertical_position','unknown'], // Unknown
    ['WLE','1','hydrographic_vertical_position','partly_submerged'], // Partly Submerged
    ['WLE','2','hydrographic_vertical_position','always_dry'], // Always Dry
    ['WLE','3','hydrographic_vertical_position','always_submerged'], // Always Submerged
    ['WLE','4','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
    ['WLE','8','hydrographic_vertical_position','floating'], // Floating
    ['WLE','998','hydrographic_vertical_position','not_applicable'], // Other
    ['WLE','999','hydrographic_vertical_position','other'], // Other

    // WST - Watercourse Sink Type: No OSM even close
    // ['WST','0',undefined,undefined],
    ['WST','0','water:sink:type','unknown'],
    ['WST','1','water:sink:type','dissipating'],
    ['WST','2','water:sink:type','disappearing'],
    ['WST','998','water:sink:type','not_applicable'],
    ['WST','999','water:sink:type','other'],

    // WTC - Road Weather Restriction
    // ['WTC','0',undefined,undefined],
    ['WTC','0','seasonal','unknown'],
    ['WTC','1','seasonal','no'],
    ['WTC','2','seasonal','fair'], // possibly seasonal=
    ['WTC','3','seasonal','winter'],
    ['WTC','5','seasonal','not_winter'],

    ], // End one2one

    // one-to-one translation table for MGCP TRD 3.0 attributes. This is for import only
    one2oneIn : [
    // ACE_EVAL Absolute Horizontal Accuracy Evaluation Method
    // ['ACE_EVAL','0',undefined,undefined],
    ['ACE_EVAL','0','source:accuracy:horizontal:evaluation','unknown'],
    ['ACE_EVAL','1','source:accuracy:horizontal:evaluation','aca_geodetic_survey_adequate'],
    ['ACE_EVAL','2','source:accuracy:horizontal:evaluation','acs_geodetic_survey_small'],
    ['ACE_EVAL','4','source:accuracy:horizontal:evaluation','ana_photogrammetric_gdas_adequate'],
    ['ACE_EVAL','5','source:accuracy:horizontal:evaluation','ans_photogrammetric_gdas_small'],
    ['ACE_EVAL','15','source:accuracy:horizontal:evaluation','emc_product_specification'],
    ['ACE_EVAL','21','source:accuracy:horizontal:evaluation','fzd_evaluation_deferred'],
    ['ACE_EVAL','999','source:accuracy:horizontal:evaluation','other'],

    // ACS - Accessibility Status
    // ['ACS','0','access','unknown'], // Unknown
    ['ACS','0',undefined,undefined], // Unknown
    ['ACS','1','access','locked'], // Locked
    ['ACS','2','access','yes'], // Open
    ['ACS','5','access','restricted'], // Restricted
    ['ACS','6','access','limited'], // Limited
    ['ACS','7','access','no'], // Closed

    // AFC - Agricultural Facility Type
    ['AFC','0',undefined,undefined], // Unknown
    ['AFC','1','building','farm'], // Farm Building
    ['AFC','2','building','greenhouse'], // Greenhouse
    ['AFC','4','building','farm_auxiliary'], // Outbuilding
    ['AFC','5','building','barn'], // Barn
    ['AFC','999','building','other'], // Other

    // ALE_EVAL Absolute Vertical Accuracy Evaluation Method
    // ['ALE_EVAL','0','source:accuracy:vertical:evaluation','unknown'],
    ['ALE_EVAL','0',undefined,undefined],
    ['ALE_EVAL','1','source:accuracy:vertical:evaluation','aca_geodetic_survey_adequate'],
    ['ALE_EVAL','2','source:accuracy:vertical:evaluation','acs_geodetic_survey_small'],
    ['ALE_EVAL','4','source:accuracy:vertical:evaluation','ana_photogrammetric_gdas_adequate'],
    ['ALE_EVAL','5','source:accuracy:vertical:evaluation','ans_photogrammetric_gdas_small'],
    ['ALE_EVAL','15','source:accuracy:vertical:evaluation','emc_product_specification'],
    ['ALE_EVAL','21','source:accuracy:vertical:evaluation','fzd_evaluation_deferred'],

    // ART - Aeronautical Route Category
    // ['ART','0','aero_route:type','unknown'],
    ['ART','0',undefined,undefined],
    ['ART','1','aero_route:type','low'],
    ['ART','2','aero_route:type','high'],
    ['ART','3','aero_route:type','both'],

    // ASC - Man-made
    // ['ASC','0','artificial','unknown'], // Unknown
    ['ASC','0',undefined,undefined], // Unknown
    ['ASC','1000','artificial','no'], // Natural
    ['ASC','1001','artificial','yes'], // Man-made

    // AWP - Aeronautical Obstacle Light Present
    // ['AWP','0','light:obstacle','unknown'],
    ['AWP','0',undefined,undefined],
    ['AWP','1000','light:obstacle','no'],
    ['AWP','1001','light:obstacle','yes'],

    // BSM - Mobile Bridge Span
    // ['BSM','0','bridge:mobile_span','unknown'], // Unknown
    ['BSM','0',undefined,undefined], // Unknown
    ['BSM','1000','bridge:mobile_span','no'], // Fixed Span
    ['BSM','1001','bridge:mobile_span','yes'], // Moveable Span

    // CAA - Controlling Authority - This does not play nicely in OSM
    // There is no nice way to include this in other tags:
    ['CAA','1','operator','national'],
    ['CAA','2','operator','state'],
    ['CAA','4','operator','tribal'], // From NFDD
    ['CAA','6','operator','municipal'], // From NFDD
    ['CAA','8','operator','international'],
    ['CAA','11','operator','province'], // From NFDD
    ['CAA','13','operator','regional'], // From NFDD
    ['CAA','17','operator','public'], // From NFDD

    // CAB - Cable Type
    ['CAB','3','cable:type','telephone'], // Telephone
    ['CAB','4','cable:type','telegraph'], // Telegraph
    ['CAB','7','cable:type','digital_communication'], // Digital Communication Line

    // CEF - Cultural Facility Type
    ['CEF','0',undefined,undefined],
    ['CEF','1','historic','castle'],
    ['CEF','2','tourism','museum'],
    ['CEF','6','amenity','auditorium'],
    ['CEF','7','tourism','opera_house'],
    ['CEF','9','leisure','sports_centre'],
    ['CEF','10','amenity','community_centre'],
    ['CEF','11','amenity','theatre'],
    ['CEF','12','amenity','library'],
    ['CEF','13','building','lyceum'], // Type of school
    ['CEF','999','building','cultural'], // Not good but better than 'other'

    // CFC - Corrections Facility Type
    ['CFC','0',undefined,undefined],
    ['CFC','1','amenity','prison'],
    ['CFC','2','amenity','jail'], // Valid on wiki but maps to prison
    ['CFC','3','amenity','reformatory'],
    ['CFC','4','amenity','correctional_institute'],
    ['CFC','999','building','correctional'], // Not good but better than 'other'

    // CIT - Commercial Facility Type
    ['CIT','0',undefined,undefined],
    ['CIT','1','amenity','bank'],
    ['CIT','5','amenity','animal_boarding'],
    ['CIT','6','amenity','marketplace'],
    ['CIT','7','amenity','newspaper_plant'], // ??
    ['CIT','8','amenity','parking'],
    ['CIT','10','shop','mall'],
    ['CIT','11','building','yes'], // Significant Non-Government Building ??
    ['CIT','12','amenity','trading_post'],
    ['CIT','999','building','commercial'],

    // CLI - Isolated Communication Line
    // ['CLI','0','raw:CLI','unknown'], // Unknown
    ['CLI','0',undefined,undefined], // Unknown
    ['CLI','1000','cable:isolated','no'], // Not isolated
    ['CLI','1001','cable:isolated','yes'], // Isolated

    // COS - Facility Operational Status
    // ['COS','0','operational_status','unknown'], // Unknown
    ['COS','0',undefined,undefined], // Unknown
    ['COS','1','operating_cycle','continuous'], // Continuous
    ['COS','2','operating_cycle','intermittent'], // Intermittent
    ['COS','3','operational_status','scheduled'], // Scheduled
    ['COS','4','operational_status','non-operational'], // Non-operational
    ['COS','5','operational_status','temporary'], // Temporary
    ['COS','999','operational_status','other'], // Other

    // CUS - Communications Facility Type
    ['CUS','0',undefined,undefined],
    ['CUS','2',undefined,undefined], // In data but not in Spec
    ['CUS','4','building','ground_station'],
    ['CUS','999','building','communications'],

    // DDC - Dwelling Type
    ['DDC','0',undefined,undefined],
    ['DDC','1','building','residential'],
    ['DDC','2','building','apartments'],
    ['DDC','3','building','caravan'], // not good
    ['DDC','4','social_facility','senior'],
    ['DDC','5','building','dormitory'],
    ['DDC','999','building','dwelling'], // grungy

    // DFT - Dam Face Type
    // ['DFT','0','dam:face:type','unknown'],
    ['DFT','0',undefined,undefined],
    ['DFT','1','dam:face:type','vertical'],
    ['DFT','2','dam:face:type','slope'],

    // EBT - Educational Facility Type
    ['EBT','0',undefined,undefined],
    ['EBT','1','building','academy'],
    ['EBT','2','building','college'],
    ['EBT','3','building','educational_centre'],
    ['EBT','5','building','university'],
    ['EBT','6','building','seminary'],
    ['EBT','7','building','collegiate'],
    ['EBT','9','building','school'],
    ['EBT','999','use','education'], // Catch all?

    // FUC - Functional Use
    ['FUC','3','use','institute'],
    ['FUC','9','use','correctional'],  // Prison?
    ['FUC','23','use','healthcare'], // Dont know if hospital, clinic etc
    ['FUC','26','use','education'], // Don't know if school, college, university

    // FTI -  Fence Type
    // ['FTI','0','fence:type','unknown'],
    ['FTI','0',undefined,undefined],
    ['FTI','1','fence:type','metal'],
    ['FTI','2','fence:type','wood'],
    ['FTI','5','fence:type','barbed_wire'],
    ['FTI','6','fence:type','chain_link'],
    ['FTI','999','fence:type','other'],

    // GFC - Government Facility Type - This does not play nicely in OSM
    ['GFC','0',undefined,undefined],
    ['GFC','1','office','government'],
    ['GFC','2','amenity','legislative_building'],
    ['GFC','3','amenity','government_administration_building'],
    ['GFC','4','building','palace'],
    ['GFC','5','office','administrative'],
    ['GFC','6','amenity','customs_house'],
    ['GFC','8','amenity','courthouse'],
    ['GFC','9','amenity','legation'],
    ['GFC','10','amenity','chancery'],
    ['GFC','11','amenity','ambassadorial_residence' ],
    ['GFC','12','amenity','embassy'],
    ['GFC','13','amenity','consulate'],
    ['GFC','15','amenity','diplomatic_building'],
    ['GFC','17','amenity','municipal_hall'],
    ['GFC','999','amenity','government'],

    // HAF - Helipad Accociated Facilities
    ['HAF','5','helipad:facility','offshore_construction'], // Seems to have changed to '9' in TRD4

    // ICF - Manufacturing Facility Type - the rest of these are in biased rules
    ['ICF','0',undefined,undefined],
    ['ICF','1','industrial','manufacturing'],
    ['ICF','3','product','grist'],
    ['ICF','4','building','wind_tunnel'],
    ['ICF','5','building','warehouse'],
    ['ICF','10','product','oil'],
    ['ICF','11','building','aeration'],
    ['ICF','12','product','woodwork'],
    ['ICF','13','product','lumber'],
    ['ICF','14','man_made','kiln'],
    ['ICF','18','building','processing'],
    ['ICF','19','building','pumping'],
    // ['ICF','21','product','petroleum'],
    ['ICF','21','industrial','petroleum_refining'],
    ['ICF','22','product','paper'],
    ['ICF','23','amenity','shipyard'],
    ['ICF','24','product','steel'],
    ['ICF','26','product','motor_vehicle'],
    ['ICF','28','repair','yes'],
    ['ICF','35','building','manufacturing_outbuilding'],
    ['ICF','999','building','manufacturing'],

    // IRG - Irrigation Method
    // ['IRG','0','farm:irrigation','unknown'], // Unknown
    ['IRG','0',undefined,undefined], // Unknown
    ['IRG','1','farm:irrigation','furrow'], // Furrow
    ['IRG','2','farm:irrigation','overhead'], // Overhead
    ['IRG','999','farm:irrigation','other'], // Other

    // ISS - Isolated
    // ['ISS','0','isolated','unknown'], // Unknown
    ['ISS','0',undefined,undefined], // Unknown
    ['ISS','1000','isolated','no'], // Not Isolated
    ['ISS','1001','isolated','yes'], // Isolated

    // LFA - Aeronautical Light Function
    ['LFA','1','aeroway:light','airport_terminal_lights'], // Airport Terminal Lights
    ['LFA','2','aeroway:light','apron_flood'], // Apron Flood
    ['LFA','3','aeroway:light','boundary'], // Boundary
    ['LFA','4','aeroway:light','runway_centreline'], // Runway Centre-line
    ['LFA','5','aeroway:light','runway_end_identification_lighting'], // REIL
    ['LFA','6','aeroway:light','runway_edge'], // Runway Edge
    ['LFA','7','aeroway:light','sequenced_strobe'], // Sequenced Strobe
    ['LFA','8','aeroway:light','taxiway_lighting'], // Taxiway Lighting
    ['LFA','9','aeroway:light','visual_approach_slope_indicator'], // VASI
    ['LFA','10','aeroway:light','rotating_beacon'], // Rotating Beacon
    ['LFA','11','aeroway:light','obstruction_lighting'], // Obstruction Marker
    ['LFA','12','aeroway:light','threshold'], // Threshold
    ['LFA','13','aeroway:light','touchdown_zone'], // Touchdown Zone
    ['LFA','14','aeroway:light','other_airport_lighting'], // Other Airport Lighting
    ['LFA','21','aeroway:light','landing_direction_indicator'], // LDIN
    ['LFA','22','aeroway:light','runway_alignment_indicator_lights'], // RAIL
    ['LFA','23','aeroway:light','omni_directional_approach_landing_system'], // ODALS
    ['LFA','24','aeroway:light','other_approach_lighting'], // Other Approach Lighting
    ['LFA','25','aeroway:light','precision_approach_path_indicator'], // PAPI
    ['LFA','26','aeroway:light','strobe'], // Strobe
    ['LFA','27','aeroway:light','runway_flood'], // Runway Flood
    ['LFA','28','aeroway:light','variable_intensity_runway_lights'], // Variable Intensity Runway Lights
    ['LFA','29','aeroway:light','portable_runway_lights'], // Portable Runway Lights
    ['LFA','30','aeroway:light','flares'], // Flares
    ['LFA','31','aeroway:light','wind_indicator_lights'], // Wind Indicator Lights
    ['LFA','32','aeroway:light','visual_approach_slope_indicator_3_bar'], // VASI 3-bar
    ['LFA','33','aeroway:light','optical_landing_system'], // Optical Landing System
    ['LFA','73','aeroway:light','none_available'], // None available
    ['LFA','100','aeroway:light','overrun_centreline'], // Overrun Centre-line
    ['LFA','101','aeroway:light','centreline_and_bar'], // Centre-line and Bar
    ['LFA','102','aeroway:light','u.s._configuration_(b)'], // U.S. Configuration (B)
    ['LFA','103','aeroway:light','hong_kong_curve'], // Hong Kong Curve
    ['LFA','104','aeroway:light','left_single_row'], // Left Single Row
    ['LFA','105','aeroway:light','centre_row'], // Centre Row
    ['LFA','106','aeroway:light','former_nato_standard'], // Former NATO Standard
    ['LFA','107','aeroway:light','nato_standard'], // NATO Standard
    ['LFA','108','aeroway:light','centre_and_double_row'], // Centre and Double Row
    ['LFA','109','aeroway:light','portable_approach'], // Portable Approach
    ['LFA','110','aeroway:light','centre_row_category_2'], // Centre Row Category 2
    ['LFA','111','aeroway:light','centre_row_category_1'], // Centre Row Category 1
    ['LFA','112','aeroway:light','navy_parallel_row_and_crossbar'], // Navy Parallel Rows and Crossbar
    ['LFA','113','aeroway:light','two_parallel_row'], // Two Parallel Rows
    ['LFA','114','aeroway:light','left_row_high_intensity'], // Left Row High
    ['LFA','115','aeroway:light','air_force_overrun'], // Air Force Overrun
    ['LFA','116','aeroway:light','calvert'], // Calvert
    ['LFA','117','aeroway:light','single_row_centreline'], // Single Row Centre-line
    ['LFA','118','aeroway:light','narrow_multi_cross'], // Narrow Multi-cross
    ['LFA','119','aeroway:light','centreline_high_intensity_approach_lights'], // Centre-line High Intensity Approach Lights
    ['LFA','120','aeroway:light','alternate_centreline_and_bar_approach_lights'], // Alternate Centre-line and Bar Approach Lights
    ['LFA','121','aeroway:light','cross'], // Cross
    ['LFA','122','aeroway:light','neon_ladder'], // Neon Ladder
    ['LFA','123','aeroway:light','singapore_centreline_approach_lights'], // Singapore Centre-line
    ['LFA','124','aeroway:light','centreline_2_crossbars_approach_lights'], // Centre-line 2 Crossbars Approach Lights
    ['LFA','125','aeroway:light','t-vasi'], // T-VASI
    ['LFA','126','aeroway:light','pvasi'], // PVASI
    ['LFA','127','aeroway:light','apap_alignment'], // APAP Alignment
    ['LFA','128','aeroway:light','vasi_with_threshold_indicator'], // VASI with Threshold Indicator
    ['LFA','129','aeroway:light','lcvasi'], // LCVASI
    ['LFA','130','aeroway:light','high_intensity_runway_lights'], // HIRL
    ['LFA','131','aeroway:light','medium_intensity_runway_lights'], // MIRL

    // LMC - Navigation Landmark
    // ['LMC','0','navigation_landmark','unknown'], // Unknown
    ['LMC','0',undefined,undefined], // Unknown
    ['LMC','1000','navigation:landmark','no'], // Not a landmark
    ['LMC','1001','navigation:landmark','yes'], // Landmark

    // MFC - Military Facility Type
    ['MFC','0',undefined,undefined],
    ['MFC','1','military','administration'],
    ['MFC','2','military','range'],
    ['MFC','3','military','dormitory'],
    ['MFC','4','military','dependent_housing'],
    ['MFC','5','military','battery'],
    ['MFC','6','military','armory'],
    ['MFC','12','military','barracks'],
    ['MFC','13','military','military_outbuilding'],
    ['MFC','999','military','other'],

    // MIN - Extraction Mine type
    // ['MIN','0','mine:type','unknown'],
    ['MIN','0',undefined,undefined],
    ['MIN','3','mine:type','opencast'],
    ['MIN','4','mine:type','placer'],
    ['MIN','5','mine:type','prospect'],
    ['MIN','6','mine:type','strip-mine'],
    ['MIN','8','mine:type','peatery'],
    ['MIN','9','mine:type','below_surface'],

    // NST - Navigation System Type
    ['NST','2','beacon:type','consol'],
    ['NST','3','beacon:type','decca'],
    ['NST','7','beacon:type','loran'], // long_range_air_navigation_system
    ['NST','8','beacon:type','omega'],
    ['NST','11','beacon:type','radar'],

    // NVS - Navigability Information
    // ['NVS','0','navigation','unknown'],
    ['NVS','0',undefined,undefined],
    ['NVS','3','navigation','yes'],
    ['NVS','999','navigation','other'],

    // OWO - Waterbody Overhead Obstruction
    // ['OWO','0','pipeline:obstruction','unknown'],
    ['OWO','0',undefined,undefined],
    ['OWO','2',undefined,undefined], // In data but not in spec!
    ['OWO','1000','waterway:overhead_obstruction','no'], // Feature does not cross navigable water
    ['OWO','1001','waterway:overhead_obstruction','yes'],

    // PAF - Public Accomodation Facility Type
    ['PAF','0',undefined,undefined],
    ['PAF','1','tourism','inn'],
    ['PAF','2','amenity','restaurant'],
    ['PAF','3','tourism','hotel'],
    ['PAF','4','tourism','hostel'],
    ['PAF','5','tourism','motel'],
    ['PAF','999','tourism','public_accomodation'],

    // PPO Product
    ['PPO','6','product','bamboo'],
    ['PPO','7','product','banana'],
    ['PPO','20','product','coffee'],
    ['PPO','28','product','cotton'],
    ['PPO','32','product','desalinated_water'],
    ['PPO','37','product','electric_power'],
    ['PPO','44','product','fruit'],
    ['PPO','56','product','heat'],
    ['PPO','64','product','macadam'],
    ['PPO','78','product','palmetto'],
    ['PPO','118','product','travertine'],
    ['PPO','130','product','chalk'],

    // PSF - Public Service Facility Type
    ['PSF','0',undefined,undefined],
    ['PSF','1','building','hospital'],
    ['PSF','2','amenity','police'],
    ['PSF','3','amenity','ranger_station'],
    ['PSF','4','amenity','health_office'],
    ['PSF','5','amenity','post_office'],
    ['PSF','6','amenity','fire_station'],
    ['PSF','8','amenity','motor_vehicle_station'],
    ['PSF','9','amenity','water_police'],
    ['PSF','10','amenity','rescue_station'],
    ['PSF','12','amenity','clinic'],
    ['PSF','13','amenity','sanatorium'],
    ['PSF','16','amenity','fire_and_police'],
    ['PSF','19','amenity','public_service_outbuilding'],
    ['PSF','999','amenity','public_service'],

    // RES - Research Facility Type
    ['RES','0',undefined,undefined],
    ['RES','1','amenity','astronomical_station'], // ??
    ['RES','2','man_made','observatory'],
    ['RES','3','building','research_centre'],
    ['RES','4','monitoring:weather','yes'],
    ['RES','999','building','research'],

    // RFC - Religious Facility Type
    ['RFC','0',undefined,undefined],
    ['RFC','1','amenity','place_of_worship'],
    ['RFC','2','building','cemetery_building'],
    ['RFC','4','amenity','mission'],
    ['RFC','999','amenity','religion'],

    // RIT - Road Interchange Type
    ['RIT','0','junction','yes'],
    ['RIT','1','junction','cloverleaf'],
    ['RIT','2','junction','diamond'],
    ['RIT','3','junction','fork'],
    ['RIT','4','junction','rotary'],
    ['RIT','5','junction','staggered_ramps'],
    ['RIT','6','junction','standard_ramps'],
    ['RIT','7','junction','symmetrical_ramps'],
    ['RIT','8','junction','trumpet'],
    ['RIT','9','junction','turban'],
    ['RIT','10','junction','wye'],
    ['RIT','999','junction','other'],

    // SCB - Pavement Condition
    // ['SCB','0','surface:condition','unknown'], // Unknown
    ['SCB','0',undefined,undefined], // Unknown
    ['SCB','1','surface:condition','good'], // Good
    ['SCB','2','surface:condition','fair'], // Fair
    ['SCB','3','surface:condition','poor'], // Poor

    // SFY - Security Facility Type
    ['SFY','0',undefined,undefined],
    ['SFY','1','building','guardhouse'],
    ['SFY','2','building','guardroom'],
    ['SFY','999','building','security'],

    // SHL - Left Bank Shoreline Type: No OSM even close
    ['SHL','0',undefined,undefined], //Unknown
    ['SHL','6','waterway:bank2:type','mangrove'],
    ['SHL','8','waterway:bank2:type','marshy'],
    ['SHL','10','waterway:bank2:type','stony'],
    ['SHL','11','waterway:bank2:type','rubble'],
    ['SHL','13','waterway:bank2:type','sandy'],
    ['SHL','14','waterway:bank2:type','shingly'],

    // SHR - Right Bank Shoreline Type: No OSM even close
    ['SHR','0',undefined,undefined], //Unknown
    ['SHR','6','waterway:bank1:type','mangrove'],
    ['SHR','8','waterway:bank1:type','marshy'],
    ['SHR','10','waterway:bank1:type','stony'],
    ['SHR','11','waterway:bank1:type','rubble'],
    ['SHR','13','waterway:bank1:type','sandy'],
    ['SHR','14','waterway:bank1:type','shingly'],

    // SRC_NAME - Source Name
    // ['SRC_NAME','0','source:name','unknown'],
    ['SRC_NAME','0',undefined,undefined],
    ['SRC_NAME','1','source:name','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG)
    ['SRC_NAME','2','source:name','automated_aeronautical_facilities_information_file_(aafif)'], //  Automated Aeronautical Facilities Information File (AAFIF)
    ['SRC_NAME','3','source:name','chart_update_manual_(chum)'], // CHUM
    ['SRC_NAME','4','source:name','city_graphic'], // City Graphic
    ['SRC_NAME','5','source:name','combat_chart'], // Combat Chart
    ['SRC_NAME','6','source:name','compressed_arc_digitized_raster_graphic_(cadrg)'], // Compressed Arc Digitized Raster Graphic (CADRG)
    ['SRC_NAME','7','source:name','controlled_imagery_base_1_(cib1)'], // Controlled Imagery Base 1 (CIB1)
    ['SRC_NAME','8','source:name','controlled_imagery_base_5_(cib5)'], // Controlled Imagery Base 5 (CIB5)
    ['SRC_NAME','9','source:name','quickbird_imagery'], // From DFDD
    ['SRC_NAME','10','source:name','digital_nautical_chart_(dnc)'], // Digital Nautical Chart (DNC)
    ['SRC_NAME','11','source:name','digital_print_file_(dpf)'], // Digital Print File (DPF)
    ['SRC_NAME','12','source:name','digital_terrain_elevation_data_1_(dted1)'], // Digital Terrain Elevation Data 1 (DTED1)
    ['SRC_NAME','13','source:name','digital_terrain_elevation_data_2_(dted2)'], // Digital Terrain Elevation Data 2 (DTED2)
    ['SRC_NAME','14','source:name','digital_terrain_elevation_data_3_(dted3)'], // Digital Terrain Elevation Data 3 (DTED3)
    ['SRC_NAME','15','source:name','digital_terrain_elevation_data_4_(dted4)'], // Digital Terrain Elevation Data 4 (DTED4)
    ['SRC_NAME','16','source:name','digital_topographic_data_1_(dtop1)'], // Digital Topographic Data 1 (DTOP1)
    ['SRC_NAME','17','source:name','digital_topographic_data_2_(dtop2)'], // Digital Topographic Data 2 (DTOP2)
    ['SRC_NAME','18','source:name','digital_topographic_data_3_(dtop3)'], // Digital Topographic Data 3 (DTOP3)
    ['SRC_NAME','19','source:name','digital_topographic_data_4_(dtop4)'], // Digital Topographic Data 4 (DTOP4)
    ['SRC_NAME','20','source:name','digital_topographic_data_5_(dtop5)'], // Digital Topographic Data 5 (DTOP5)
    ['SRC_NAME','21','source:name','digital_vertical_obstruction_file_(dvof)'], // Digital Vertical Obstruction File (DVOF)
    ['SRC_NAME','22','source:name','foundation_feature_data_(ffd)'], // Foundation Feature Data (FFD)
    ['SRC_NAME','24','source:name','geocover/landcover'], // GeoCover/LandCover
    ['SRC_NAME','25','source:name','geonames'], // GeoNames
    ['SRC_NAME','26','source:name','gps_field_collected_open_source'], // GPS Field Collected Open Source
    ['SRC_NAME','27','source:name','image_city_map_(icm)'], // Image City Map (ICM)
    ['SRC_NAME','28','source:name','ikonos_imagery'], // Imagery (IKONOS)
    ['SRC_NAME','29','source:name','ntm_imagery'], // Imagery (NTM)
    ['SRC_NAME','30','source:name','imagery'], // Imagery (Other)
    ['SRC_NAME','31','source:name','interim_terrain_data_(itd)'], // Interim Terrain Data (ITD)
    ['SRC_NAME','32','source:name','interim_vector_data_(ivd)'], // Interim Vector Data (IVD)
    ['SRC_NAME','33','source:name','international_boundaries'],
    ['SRC_NAME','34','source:name','joint_operational_graphic_(jog)'], // Joint Operational Graphic (JOG)
    ['SRC_NAME','36','source:name','littoral_warfare_data_(lwd)'], // Littoral Warfare Data (LWD)
    ['SRC_NAME','37','source:name','mapping_charting_and_geodetic_(mc&g)'], // Mapping Charting and Geodetic (MC&G)
    ['SRC_NAME','38','source:name','mission_specific_data_1_(msd1)'], // Mission Specific Data 1 (MSD1)
    ['SRC_NAME','39','source:name','mission_specific_data_2_(msd2)'], // Mission Specific Data 2 (MSD2)
    ['SRC_NAME','40','source:name','mission_specific_data_3_(msd3)'], // Mission Specific Data 3 (MSD3)
    ['SRC_NAME','41','source:name','mission_specific_data_4_(msd4)'], // Mission Specific Data 4 (MSD4)
    ['SRC_NAME','42','source:name','mission_specific_data_5_(msd5)'], // Mission Specific Data 5 (MSD5)
    ['SRC_NAME','43','source:name','modernized_integrated_data_base_(midb)'], // Modernized Integrated Data Base (MIDB)
    ['SRC_NAME','44','source:name','native_data_source'], // Native Data Source
    ['SRC_NAME','45','source:name','native_map_source'], // Native Map Source
    ['SRC_NAME','46','source:name','medical_facilities'],
    ['SRC_NAME','47','source:name','nominally_attributed_topographic_evaluation_map_(nate_map)'], //  Nominally Attributed Topographic Evaluation Map (NATE Map)
    ['SRC_NAME','48','source:name','planning_graphic'], // Planning Graphic
    ['SRC_NAME','50','source:name','shuttle_radar_topographic_mission_(srtm)'], // Shuttle Radar Topographic Mission (SRTM)
    ['SRC_NAME','51','source:name','site_map/nuclear_facility'], // Site Map/Nuclear Facility
    ['SRC_NAME','52','source:name','tactical_oceanographic_data_lv_0_(tod0)'], // Tactical Oceanographic Data Lv 0 (TOD0)
    ['SRC_NAME','53','source:name','tactical_oceanographic_data_lv_1_(tod1)'], // Tactical Oceanographic Data Lv 1 (TOD1)
    ['SRC_NAME','54','source:name','tactical_oceanographic_data_lv_2_(tod2)'], // Tactical Oceanographic Data Lv 2 (TOD2)
    ['SRC_NAME','55','source:name','tactical_oceanographic_data_lv_3_(tod3)'], // Tactical Oceanographic Data Lv 3 (TOD3)
    ['SRC_NAME','56','source:name','tactical_oceanographic_data_lv_4_(tod4)'], // Tactical Oceanographic Data Lv 4 (TOD4)
    ['SRC_NAME','57','source:name','tactical_oceanographic_data_lv_5_(tod5)'], // Tactical Oceanographic Data Lv 5 (TOD5)
    ['SRC_NAME','58','source:name','topographic_line_map_(tlm)'], // Topographic Line Map (TLM)
    ['SRC_NAME','59','source:name','united_nations_data'], // United Nations Data
    ['SRC_NAME','60','source:name','urban_vector_map_(uvmap)'], // Urban Vector Map (UVMap)
    ['SRC_NAME','61','source:name','vector_interim_terrain_data_(vitd)'], // Vector Interim Terrain Data (VITD)
    ['SRC_NAME','62','source:name','vector_map_lv0_(vmap0)'], // Vector Map Lv0 (VMap0)
    ['SRC_NAME','63','source:name','vector_map_lv1_(vmap1)'], // Vector Map Lv1 (VMap1)
    ['SRC_NAME','64','source:name','vector_map_lv2_(vmap2)'], // Vector Map Lv2 (VMap2)
    ['SRC_NAME','65','source:name','world_vector_shoreline_plus_(wvsplus)'], // World Vector Shoreline Plus (WVSPlus)
    ['SRC_NAME','66','source:name','quickbird_multispectral_monoscopic_imagery'], // DFDD
    ['SRC_NAME','67','source:name','quickbird_multispectral_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','68','source:name','quickbird_panchromatic_monoscopic_imagery'], // DFDD
    ['SRC_NAME','69','source:name','quickbird_panchromatic_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','70','source:name','ikonos_multispectral_monoscopic_imagery'], // DFDD
    ['SRC_NAME','71','source:name','ikonos_multispectral_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','72','source:name','ikonos_panchromatic_monoscopic_imagery'], // DFDD
    ['SRC_NAME','73','source:name','ikonos_panchromatic_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','74','source:name','spot_10m_imagery'], // DFDD
    ['SRC_NAME','75','source:name','spot_5m_imagery'], // DFDD
    ['SRC_NAME','76','source:name','spot_5m_multispectral_monoscopic_imagery'], // DFDD
    ['SRC_NAME','77','source:name','spot_5m_multispectral_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','78','source:name','spot_5m_panchromatic_monoscopic_imagery'], // DFDD
    ['SRC_NAME','79','source:name','spot_5m_panchromatic_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','80','source:name','spot_2.5m_panchromatic_monoscopic_imagery'], // DFDD
    ['SRC_NAME','81','source:name','spot_2.5m_multispectral_monoscopic_imagery'], // DFDD
    ['SRC_NAME','82','source:name','spot_5m_monoscopic_imagery'], // DFDD
    ['SRC_NAME','83','source:name','spot_5m_stereoscopic_imagery'], // DFDD
    ['SRC_NAME','84','source:name','spot_10m_multispectral_monoscopic_imagery'], // DFDD
    ['SRC_NAME','85','source:name','spot_hrg_digital_terrain_model'],
    ['SRC_NAME','92','source:name','vector_data'],
    ['SRC_NAME','93','source:name','1:25k_vector_data'],
    ['SRC_NAME','94','source:name','1:50k_vector_data'],
    ['SRC_NAME','95','source:name','1:100k_vector_data'],
    ['SRC_NAME','110','source:name','very_high_resolution_commercial_monoscopic_imagery'],
    ['SRC_NAME','111','source:name','very_high_resolution_commercial_stereoscopic_imagery'],
    ['SRC_NAME','112','source:name','high_resolution_commercial_monoscopic_imagery'],
    ['SRC_NAME','113','source:name','high_resolution_commercial_stereoscopic_imagery'],
    ['SRC_NAME','114','source:name','medium_resolution_commercial_monoscopic_imagery'],
    ['SRC_NAME','115','source:name','medium_resolution_commercial_stereoscopic_imagery'],
    ['SRC_NAME','116','source:name','low_resolution_commercial_monoscopic_imagery'],
    ['SRC_NAME','117','source:name','low_resolution_commercial_stereoscopic_imagery'],
    ['SRC_NAME','118','source:name','map_1:25k'],
    ['SRC_NAME','119','source:name','map_1:50k'],
    ['SRC_NAME','120','source:name','map_1:100k'],
    ['SRC_NAME','121','source:name','routing_data'],
    ['SRC_NAME','996','source:name','multiple'], // Multiple
    ['SRC_NAME','997','source:name','unpopulated'],
    ['SRC_NAME','999','source:name','other'],

    // SRL - Location Referenced to Shoreline
    // ['SRL','0','location','unknown'], // Unknown
    ['SRL','0',undefined,undefined], // Unknown
    ['SRL','1','location','offshore'], // Offshore
    ['SRL','2','location','inland'], // Inland
    ['SRL','3','location','at_shoreline'], // At Shoreline
    ['SRL','999','location','other'], // Other ### Need to look at this

    // SSO - Shaft Surface Orientation
    // ['SSO','0','mine:shaft:orientation','unknown'],
    ['SSO','0',undefined,undefined],
    ['SSO','1','mine:shaft:orientation','horizontal'],
    ['SSO','2','mine:shaft:orientation','vertical'],
    ['SSO','999','mine:shaft:orientation','other'],

    // SSR - Roof Shape
    // ['SSR','0','roof:shape','unknown'], // Unknown
    ['SSR','0',undefined,undefined], // Unknown
    ['SSR','40','roof:shape','domed'], // Domed
    ['SSR','999','roof:shape','other'], // Other

    // SUC - Shed Type
    // NOTE: Retamed to avoid confusion with the TRD4 SUC (Shed type)
    ['xSUC','3','building','shed'],
    ['xSUC','999','building','other_shed'], // This is nuts

    // TFC - Transportation Facility Type
    ['TFC','0',undefined,undefined],
    ['TFC','1','railway','roundhouse'],
    ['TFC','2','repair','rail_car'],
    ['TFC','3','man_made','depot'], // not a good fit for depot terminal
    ['TFC','4','repair','aircraft'],
    ['TFC','5','building','hangar'],
    ['TFC','8','leisure','yacht_club'],
    ['TFC','9','man_made','measurement_station'],
    ['TFC','10','signal_station','railway'], // not a good fit for railway signal
    ['TFC','11','building','harbour_master'],
    ['TFC','12','building','port_control'],
    ['TFC','15','railway','station'],
    ['TFC','16','amenity','weighbridge'],
    ['TFC','17','aeroway','terminal'],
    ['TFC','18','public_transport','station'],
    ['TFC','19','building','pilot_office'],
    ['TFC','20','building','pilot_lookout_station'],
    ['TFC','21','building','transportation_outbuilding'],
    ['TFC','999','building','transportation'],

    // TOS - Tower Shape
    ['TOS','12','tower:shape','truss'], // Truss
    ['TOS','13','tower:shape','tubular'], // Tubular

    // TRS - Transportation System Type: See BH070 Ford
    ['TRS','3','transport:type','automotive'], // From TRD3.0 to stop warnings
    ['TRS','14','transport:type','road_and_railway'],

    // TST - Power Line Shape
    // ['TST','0','cable:shape','unknown'], // Unknown
    ['TST','0',undefined,undefined], // Unknown
    ['TST','1','cable:shape','symmetric_catenary'], // Symmetric Catenary
    ['TST','2','cable:shape','mountain_catenary'], // Mountain Catenary
    ['TST','3','cable:shape','overwater_catenary'], // Overwater Catenary

    // TXP - Taxiway Type
    // ['TXP','0','taxiway:type','unknown'], // Unknown
    ['TXP','0',undefined,undefined], // Unknown
    ['TXP','4','taxiway:type','dispersal'], // Dispersal
    ['TXP','5','taxiway:type','link'], // Link
    ['TXP','6','taxiway:type','loop'], // Loop
    ['TXP','7','taxiway:type','parallel'], // Parallel
    ['TXP','8','taxiway:type','perimeter'], // Perimeter

    // UHS - Uncovering Height Known
    // ['UHS','1000','raw:UHS','uncovering_height_unknown'], // Uncovering Height Unknown
    ['UHS','1000','hydrographic_depth_information','uncovering_height_unknown'], // Uncovering Height Unknown
    ['UHS','1001','hydrographic_depth_information','uncovering_height_known'], // Uncovering Height Known

    // UPD_NAME - Review Source Type
    // ['UPD_NAME','0','source:review_source:type','unknown'],
    ['UPD_NAME','0',undefined,undefined],
    ['UPD_NAME','1','source:review_source:type','arc_digitized_raster_graphic_(adrg)'], // Arc Digitized Raster Graphic (ADRG)
    ['UPD_NAME','2','source:review_source:type','automated_aeronautical_facilities_information_file_(aafif)'], //  Automated Aeronautical Facilities Information File (AAFIF)
    ['UPD_NAME','3','source:review_source:type','chart_update_manual_(chum)'], // CHUM
    ['UPD_NAME','4','source:review_source:type','city_graphic'], // City Graphic
    ['UPD_NAME','5','source:review_source:type','combat_chart'], // Combat Chart
    ['UPD_NAME','6','source:review_source:type','compressed_arc_digitized_raster_graphic_(cadrg)'], // Compressed Arc Digitized Raster Graphic (CADRG)
    ['UPD_NAME','7','source:review_source:type','controlled_imagery_base_1_(cib1)'], // Controlled Imagery Base 1 (CIB1)
    ['UPD_NAME','8','source:review_source:type','controlled_imagery_base_5_(cib5)'], // Controlled Imagery Base 5 (CIB5)
    ['UPD_NAME','9','source:review_source:type','quickbird_imagery'], // From DFDD
    ['UPD_NAME','10','source:review_source:type','digital_nautical_chart_(dnc)'], // Digital Nautical Chart (DNC)
    ['UPD_NAME','11','source:review_source:type','digital_print_file_(dpf)'], // Digital Print File (DPF)
    ['UPD_NAME','12','source:review_source:type','digital_terrain_elevation_data_1_(dted1)'], // Digital Terrain Elevation Data 1 (DTED1)
    ['UPD_NAME','13','source:review_source:type','digital_terrain_elevation_data_2_(dted2)'], // Digital Terrain Elevation Data 2 (DTED2)
    ['UPD_NAME','14','source:review_source:type','digital_terrain_elevation_data_3_(dted3)'], // Digital Terrain Elevation Data 3 (DTED3)
    ['UPD_NAME','15','source:review_source:type','digital_terrain_elevation_data_4_(dted4)'], // Digital Terrain Elevation Data 4 (DTED4)
    ['UPD_NAME','16','source:review_source:type','digital_topographic_data_1_(dtop1)'], // Digital Topographic Data 1 (DTOP1)
    ['UPD_NAME','17','source:review_source:type','digital_topographic_data_2_(dtop2)'], // Digital Topographic Data 2 (DTOP2)
    ['UPD_NAME','18','source:review_source:type','digital_topographic_data_3_(dtop3)'], // Digital Topographic Data 3 (DTOP3)
    ['UPD_NAME','19','source:review_source:type','digital_topographic_data_4_(dtop4)'], // Digital Topographic Data 4 (DTOP4)
    ['UPD_NAME','20','source:review_source:type','digital_topographic_data_5_(dtop5)'], // Digital Topographic Data 5 (DTOP5)
    ['UPD_NAME','21','source:review_source:type','digital_vertical_obstruction_file_(dvof)'], // Digital Vertical Obstruction File (DVOF)
    ['UPD_NAME','22','source:review_source:type','foundation_feature_data_(ffd)'], // Foundation Feature Data (FFD)
    ['UPD_NAME','24','source:review_source:type','geocover/landcover'], // GeoCover/LandCover
    ['UPD_NAME','25','source:review_source:type','geonames'], // GeoNames
    ['UPD_NAME','26','source:review_source:type','gps_field_collected_open_source'], // GPS Field Collected Open Source
    ['UPD_NAME','27','source:review_source:type','image_city_map_(icm)'], // Image City Map (ICM)
    ['UPD_NAME','28','source:review_source:type','ikonos_imagery'], // Imagery (IKONOS)
    ['UPD_NAME','29','source:review_source:type','ntm_imagery'], // Imagery (NTM)
    ['UPD_NAME','30','source:review_source:type','imagery'], // Imagery (Other)
    ['UPD_NAME','31','source:review_source:type','interim_terrain_data_(itd)'], // Interim Terrain Data (ITD)
    ['UPD_NAME','32','source:review_source:type','interim_vector_data_(ivd)'], // Interim Vector Data (IVD)
    ['UPD_NAME','33','source:review_source:type','international_boundaries'],
    ['UPD_NAME','34','source:review_source:type','joint_operational_graphic_(jog)'], // Joint Operational Graphic (JOG)
    ['UPD_NAME','36','source:review_source:type','littoral_warfare_data_(lwd)'], // Littoral Warfare Data (LWD)
    ['UPD_NAME','37','source:review_source:type','mapping_charting_and_geodetic_(mc&g)'], // Mapping Charting and Geodetic (MC&G)
    ['UPD_NAME','38','source:review_source:type','mission_specific_data_1_(msd1)'], // Mission Specific Data 1 (MSD1)
    ['UPD_NAME','39','source:review_source:type','mission_specific_data_2_(msd2)'], // Mission Specific Data 2 (MSD2)
    ['UPD_NAME','40','source:review_source:type','mission_specific_data_3_(msd3)'], // Mission Specific Data 3 (MSD3)
    ['UPD_NAME','41','source:review_source:type','mission_specific_data_4_(msd4)'], // Mission Specific Data 4 (MSD4)
    ['UPD_NAME','42','source:review_source:type','mission_specific_data_5_(msd5)'], // Mission Specific Data 5 (MSD5)
    ['UPD_NAME','43','source:review_source:type','modernized_integrated_data_base_(midb)'], // Modernized Integrated Data Base (MIDB)
    ['UPD_NAME','44','source:review_source:type','native_data_source'], // Native Data Source
    ['UPD_NAME','45','source:review_source:type','native_map_source'], // Native Map Source
    ['UPD_NAME','46','source:review_source:type','medical_facilities'],
    ['UPD_NAME','47','source:review_source:type','nominally_attributed_topographic_evaluation_map_(nate_map)'], //  Nominally Attributed Topographic Evaluation Map (NATE Map)
    ['UPD_NAME','48','source:review_source:type','planning_graphic'], // Planning Graphic
    ['UPD_NAME','50','source:review_source:type','shuttle_radar_topographic_mission_(srtm)'], // Shuttle Radar Topographic Mission (SRTM)
    ['UPD_NAME','51','source:review_source:type','site_map/nuclear_facility'], // Site Map/Nuclear Facility
    ['UPD_NAME','52','source:review_source:type','tactical_oceanographic_data_lv_0_(tod0)'], // Tactical Oceanographic Data Lv 0 (TOD0)
    ['UPD_NAME','53','source:review_source:type','tactical_oceanographic_data_lv_1_(tod1)'], // Tactical Oceanographic Data Lv 1 (TOD1)
    ['UPD_NAME','54','source:review_source:type','tactical_oceanographic_data_lv_2_(tod2)'], // Tactical Oceanographic Data Lv 2 (TOD2)
    ['UPD_NAME','55','source:review_source:type','tactical_oceanographic_data_lv_3_(tod3)'], // Tactical Oceanographic Data Lv 3 (TOD3)
    ['UPD_NAME','56','source:review_source:type','tactical_oceanographic_data_lv_4_(tod4)'], // Tactical Oceanographic Data Lv 4 (TOD4)
    ['UPD_NAME','57','source:review_source:type','tactical_oceanographic_data_lv_5_(tod5)'], // Tactical Oceanographic Data Lv 5 (TOD5)
    ['UPD_NAME','58','source:review_source:type','topographic_line_map_(tlm)'], // Topographic Line Map (TLM)
    ['UPD_NAME','59','source:review_source:type','united_nations_data'], // United Nations Data
    ['UPD_NAME','60','source:review_source:type','urban_vector_map_(uvmap)'], // Urban Vector Map (UVMap)
    ['UPD_NAME','61','source:review_source:type','vector_interim_terrain_data_(vitd)'], // Vector Interim Terrain Data (VITD)
    ['UPD_NAME','62','source:review_source:type','vector_map_lv0_(vmap0)'], // Vector Map Lv0 (VMap0)
    ['UPD_NAME','63','source:review_source:type','vector_map_lv1_(vmap1)'], // Vector Map Lv1 (VMap1)
    ['UPD_NAME','64','source:review_source:type','vector_map_lv2_(vmap2)'], // Vector Map Lv2 (VMap2)
    ['UPD_NAME','65','source:review_source:type','world_vector_shoreline_plus_(wvsplus)'], // World Vector Shoreline Plus (WVSPlus)
    ['UPD_NAME','66','source:review_source:type','quickbird_multispectral_monoscopic_imagery'], // DFDD
    ['UPD_NAME','67','source:review_source:type','quickbird_multispectral_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','68','source:review_source:type','quickbird_panchromatic_monoscopic_imagery'], // DFDD
    ['UPD_NAME','69','source:review_source:type','quickbird_panchromatic_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','70','source:review_source:type','ikonos_multispectral_monoscopic_imagery'], // DFDD
    ['UPD_NAME','71','source:review_source:type','ikonos_multispectral_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','72','source:review_source:type','ikonos_panchromatic_monoscopic_imagery'], // DFDD
    ['UPD_NAME','73','source:review_source:type','ikonos_panchromatic_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','74','source:review_source:type','spot_10m_imagery'], // DFDD
    ['UPD_NAME','75','source:review_source:type','spot_5m_imagery'], // DFDD
    ['UPD_NAME','76','source:review_source:type','spot_5m_multispectral_monoscopic_imagery'], // DFDD
    ['UPD_NAME','77','source:review_source:type','spot_5m_multispectral_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','78','source:review_source:type','spot_5m_panchromatic_monoscopic_imagery'], // DFDD
    ['UPD_NAME','79','source:review_source:type','spot_5m_panchromatic_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','80','source:review_source:type','spot_2.5m_panchromatic_monoscopic_imagery'], // DFDD
    ['UPD_NAME','81','source:review_source:type','spot_2.5m_multispectral_monoscopic_imagery'], // DFDD
    ['UPD_NAME','82','source:review_source:type','spot_5m_monoscopic_imagery'], // DFDD
    ['UPD_NAME','83','source:review_source:type','spot_5m_stereoscopic_imagery'], // DFDD
    ['UPD_NAME','84','source:review_source:type','spot_10m_multispectral_monoscopic_imagery'], // DFDD
    ['UPD_NAME','85','source:review_source:type','spot_hrg_digital_terrain_model'],
//         ['UPD_NAME','92','source:review_source:type','vector_data'],
    ['UPD_NAME','93','source:review_source:type','1:25k_vector_data'],
    ['UPD_NAME','94','source:review_source:type','1:50k_vector_data'],
    ['UPD_NAME','95','source:review_source:type','1:100k_vector_data'],
    ['UPD_NAME','110','source:review_source:type','very_high_resolution_commercial_monoscopic_imagery'],
    ['UPD_NAME','111','source:review_source:type','very_high_resolution_commercial_stereoscopic_imagery'],
    ['UPD_NAME','112','source:review_source:type','high_resolution_commercial_monoscopic_imagery'],
    ['UPD_NAME','113','source:review_source:type','high_resolution_commercial_stereoscopic_imagery'],
    ['UPD_NAME','114','source:review_source:type','medium_resolution_commercial_monoscopic_imagery'],
    ['UPD_NAME','115','source:review_source:type','medium_resolution_commercial_stereoscopic_imagery'],
    ['UPD_NAME','116','source:review_source:type','low_resolution_commercial_monoscopic_imagery'],
    ['UPD_NAME','117','source:review_source:type','low_resolution_commercial_stereoscopic_imagery'],
    ['UPD_NAME','118','source:review_source:type','map_1:25k'],
    ['UPD_NAME','119','source:review_source:type','map_1:50k'],
    ['UPD_NAME','120','source:review_source:type','map_1:100k'],
    ['UPD_NAME','121','source:review_source:type','routing_data'],
    ['UPD_NAME','996','source:review_source:type','multiple'], // Multiple
    ['UPD_NAME','997','source:review_source:type','unpopulated'],
    ['UPD_NAME','999','source:review_source:type','other'],

    // UUC - Utility Facility Type
    ['UUC','0',undefined,undefined],
    ['UUC','10','use','power_generation'],
    ['UUC','11','use','filtration'],
    ['UUC','12','use','cooling'],
    ['UUC','999','building','utility'],

    // VDC - Sounding Datum
    // ['VDC','0','raw:VDC','unknown'], // Unknown
    ['VDC','0',undefined,undefined], // Unknown
    ['VDC','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
    ['VDC','999','source:datum:sounding','other'], // Other

    // VSP - Vegetation Species
    ['VSP','19','vegetation:type','mangrove'], // Mangrove

    // ZVAL_TYPE - Vertical Source Category
    // ['ZVAL_TYPE','0','source:vertical_source:type','unknown'],
    ['ZVAL_TYPE','0',undefined,undefined],
    ['ZVAL_TYPE','1','source:vertical_source:type','dted1'],
    ['ZVAL_TYPE','2','source:vertical_source:type','dted2'],
    // ['ZVAL_TYPE','3','source:vertical_source:type','no_elevations'],
    ['ZVAL_TYPE','3',undefined,undefined],
    ['ZVAL_TYPE','4','source:vertical_source:type','reflective_surface'],
    ['ZVAL_TYPE','5','source:vertical_source:type','stereoscopic_imagery'],
    ['ZVAL_TYPE','6','source:vertical_source:type','tin_data'],
    ['ZVAL_TYPE','999','source:vertical_source:type','other'],
    ], // End one2oneIn

    // One2one translation table for converting MGCP TRD 3.0 to TRD v4.0 attributes.
    // This is for Export only
    one2oneOut : [
    [undefined,undefined,'landuse','railway'], // To stop warnings. Fixed in Custom Rules
    [undefined,undefined,'amenity','social_facility'], // To stop warnings. Fixed in biased rules
    [undefined,undefined,'amenity','hospital'], // To stop warnings. Fixed in biased rules
    [undefined,undefined,'natural','water'], // To stop warnings. Fixed in biased rules

    // CAA - Controlling Authority - TRD3/NFDD values. Convert to 'civilian'
    ['CAA','16','operator','national'],
    ['CAA','16','operator','state'],
    ['CAA','16','operator','tribal'], // From NFDD
    ['CAA','16','operator','municipal'], // From NFDD
    ['CAA','16','operator','international'],
    ['CAA','16','operator','province'], // From NFDD
    ['CAA','16','operator','regional'], // From NFDD
    ['CAA','16','operator','public'], // From NFDD

    // AP020 - Interchange
    ['CON','35','junction','cloverleaf'],
    ['CON','35','junction','diamond'],
    ['CON','35','junction','fork'],
    ['CON','35','junction','roundabout'],
    ['CON','35','junction','staggered_ramps'],
    ['CON','35','junction','standard_ramps'],
    ['CON','35','junction','symetrical_ramps'],
    ['CON','35','junction','trumpet'],
    ['CON','35','junction','turban'],
    ['CON','35','junction','wye'],
    ['CON','35','junction','other'],

    ['COS','4','operational_status','non_operational'], // Non-operational

    ['FIC','1','embankment:type','mound'], // Mound
    ['FIC','1','embankment:type','berm'],
    ['FIC','1','embankment','berm'],
    ['FIC','2','embankment:type','fill'], // Fill
    ['FIC','3','embankment:type','dyke'], // Dyke
    ['FIC','3','embankment','dyke'], // Dyke
    ['FIC','999','embankment','levee'], // TDS etc
    ['FIC','999','embankment','divider'], // TDS etc
    ['FIC','999','embankment:type','levee'], // TDS etc
    ['FIC','999','embankment:type','divider'], // TDS etc
    // ['FIC','999','embankment:type','other'], // Other

    // AFC - Agricultural Facility Type
    ['FFN','2','building','farm'],
    // ['FFN','2','building','greenhouse'],
    ['FFN','2','building','farm_auxiliary'],
    ['FFN','2','building','barn'],
    ['FFN','999','building','agricultural'],

    // CEF - Cultural Facility Type
    ['FFN','999','historic','castle'], // Why can't this have it's own code? :-)
    ['FFN','850','building','lyceum'],
    ['FFN','999','building','cultural'],

    // CFC - Corrections Facility Type
    ['FFN','843','amenity','jail'], // Valid on wiki but maps to prison
    ['FFN','843','amenity','reformatory'],
    ['FFN','843','amenity','correctional_institute'],
    ['FFN','843','building','correctional'],

    // CIT - Commercial Facility Type
    ['FFN','582','amenity','newspaper_plant'], // ??
    ['FFN','475','shop','mall'],
    ['FFN','475','amenity','trading_post'],

    // CUS - Communications Facility Type
    ['FFN','626','building','ground_station'],
    ['FFN','610','building','communications'],

    // DDC - Dwelling Type
    ['FFN','550','social_facility','senior'], // Not good
    ['FFN','563','building','residential'],
    ['FFN','563','building','apartments'],

    ['FFN','563','building','caravan'], // not good
    ['FFN','563','building','dormitory'],
    ['FFN','999','building','dwelling'],

    // EBT - Educational Facility Type
    ['FFN','855','building','academy'],
    ['FFN','855','building','college'],
    ['FFN','855','building','educational_centre'],
    ['FFN','855','building','seminary'],
    ['FFN','855','building','collegiate'],

    // GFC - Government Facility Type - This does not play nicely in OSM
    ['FFN','811','amenity','legislative_building'],
    ['FFN','810','amenity','government_administration_building'],
    ['FFN','811','amenity','customs_house'],
    ['FFN','841','amenity','courthouse'],
    ['FFN','825','amenity','legation'],
    ['FFN','825','amenity','chancery'],
    ['FFN','825','amenity','ambassadorial_residence' ],
    ['FFN','825','amenity','consulate'],
    ['FFN','825','amenity','diplomatic_building'],
    ['FFN','810','amenity','municipal_hall'],
    ['FFN','811','amenity','government'],

    // MFC - Military Facility Type
    ['FFN','835','military','administration'],
    // ['FFN','835','military','range'],
    ['FFN','835','military','dormitory'],
    ['FFN','835','military','dependent_housing'],
    ['FFN','835','military','battery'],
    ['FFN','835','military','armory'],
    ['FFN','835','military','barracks'],
    ['FFN','835','military','military_outbuilding'],
    ['FFN','835','military','other'],

    // PAF - Public Accomodation Facility Type
    ['FFN','550','tourism','inn'],
    ['FFN','550','tourism','hostel'],
    ['FFN','550','tourism','motel'],
    ['FFN','550','tourism','public_accomodation'],

    // PSF - Public Service Facility Type
    ['FFN','860','building','hospital'],
    ['FFN','841','amenity','ranger_station'],
    ['FFN','860','amenity','health_office'],
    ['FFN','343','amenity','motor_vehicle_station'],
    ['FFN','841','amenity','water_police'],
    ['FFN','841','amenity','rescue_station'],
    ['FFN','860','amenity','clinic'],
    ['FFN','860','amenity','sanatorium'],
    ['FFN','841','amenity','fire_and_police'],
    ['FFN','811','amenity','public_service_outbuilding'],
    ['FFN','999','amenity','public_service'],

    // RES - Research Facility Type
    ['FFN','721','amenity','astronomical_station'], // ??
    ['FFN','721','man_made','observatory'],
    ['FFN','720','building','research_centre'],
    ['FFN','720','building','research'],

    // RFC - Religious Facility Type
    ['FFN','930','building','cemetery_building'],
    ['FFN','930','amenity','mission'],
    ['FFN','930','amenity','religion'],

    // SFY - Security Facility Type
    ['FFN','781','building','guardhouse'],
    ['FFN','781','building','guardroom'],
    ['FFN','781','building','security'],

    // SUC - Shed Type
    // ['FFN','530','building','shed'],
    // ['FFN','530','building','other_shed'], //Ugly

    // TFC - Transportation Facility Type
    // ['FFN','999','railway','roundhouse'],
    ['FFN','330','repair','rail_car'],
    ['FFN','481','man_made','depot'], // not a good fit for depot terminal
    ['FFN','486','signal_station','railway'], // not a good fit for railway signal
    ['FFN','513','building','harbour_master'],
    ['FFN','513','building','port_control'],
    ['FFN','480','railway','station'],
    ['FFN','480','amenity','weighbridge'],
    ['FFN','481','aeroway','terminal'],
    ['FFN','511','building','pilot_office'],
    ['FFN','511','building','pilot_lookout_station'],
    ['FFN','480','building','transportation_outbuilding'],

    // UUC - Utility Facility Type
    ['FFN','362','man_made','water_works'],
    ['FFN','999','plant','other'],
    ['FFN','350','building','utility'],

    // ICF - Manufacturing Facility Type - the rest of these are in biased rules
    ['FFN','999','building','aeration'], // ???
    ['FFN','999','building','processing'],
    ['FFN','999','amenity','shipyard'], // No good match
    ['FFN','99','man_made','kiln'], //Not a good match for kiln
    ['FFN','99','building','manufacturing_outbuilding'],
    ['FFN','99','building','manufacturing'],

    // Building
    ['FFN','563','building','detached'],
    ['FFN','563','building','terrace'],
    ['FFN','440','building','retail'],
    ['FFN','999','building','civic'],
    ['FFN','999','building','public'],
    ['FFN','999','building','bridge'],
    ['FFN','835','building','bunker'], // Possibly another FCODE AM060
    ['FFN','563','building','cabin'],
    ['FFN','535','building','garage'],
    ['FFN','535','building','garages'],
    ['FFN','999','building','roof'],
    ['FFN','919','building','stable'],
    ['FFN','811','amenity','public_building'], // Not good - Government
    ['FFN','481','amenity','bus_station'], // Not good - terminal
    ['FFN','999','amenity','arts_centre'], // Nothing close to this, going with Other

    // Office: Things that are not "commerce"
    ['FFN','720','office','research'], // Scientific R&D
    ['FFN','810','office','association'], // Administrative
    ['FFN','810','office','educational_institution'], // Administrative
    ['FFN','810','office','religion'], // Administrative
    ['FFN','810','office','foundation'], // Administrative
    ['FFN','825','office','diplomatic'], // Diplomacy
    ['FFN','841','office','police'], // Law Enforcement
    ['FFN','859','office','ngo'], // Institution
    ['FFN','859','office','charity'], // Institution
    ['FFN','860','office','physician'], // Human Health
    ['FFN','860','office','therapist'], // Human Health
    ['FFN','860','office','medical'], // Human Health
    ['FFN','440','information','office'], // Commerce - Tourist information as a "service"
    ['FFN','192','industrial','oil_gas_facility'],

    // Tags from "native" OSM start here - for export
    ['FFN','550','tourism','hotel'], // Accomodation
    ['FFN','550','tourism','guest_house'], // Accomodation
    ['FFN','550','amenity','doctors'], // Healthcare
    ['FFN','572','amenity','cafe'],
    ['FFN','850','use','education'],
    ['FFN','850','amenity','school'],
    ['FFN','850','amenity','kindergarten'],
    ['FFN','850','amenity','college'],
    ['FFN','850','amenity','university'],

    ['FUN','6','condition','intact'],
    ['FUN','6','condition','not_isolated'], // Changed to Fully Functional

    // Highway
    ['HCT','1','highway','trunk'],
    ['HCT','1','highway','trunk_link'],
    ['HCT','1','highway','primary_link'],
    ['HCT','2','highway','secondary_link'],
    ['HCT','3','highway','motorway_link'],
    ['HCT','4','highway','tertiary_link'],
    ['HCT','4','highway','living_street'],
    ['HCT','4','highway','residential'],
    ['HCT','4','highway','service'],
    [undefined,undefined,'highway','traffic_signals'], // Drop
    [undefined,undefined,'highway','residential;unclassified'], // Drop
    [undefined,undefined,'highway','tertiary;track'], // Drop

    // Surface
    ['RST','1','surface','asphalt'],
    ['RST','1','surface','cobblestone'],
    ['RST','1','surface','concrete'],
    ['RST','1','surface','paving_stones'],
    ['RST','1','surface','compacted'],
    ['RST','1','surface','metal'],
    ['RST','1','surface','pebblestone'],
    ['RST','1','surface','wood'],
    ['RST','2','surface','sand'],
    ['RST','2','surface','dirt'],
    ['RST','2','surface','fine_gravel'],
    ['RST','2','surface','gravel'], // DIGEST FACC from Data: Loose/Light
    ['RST','2','surface','mud'],
    ['RST','5','surface','grass_paver'],
    ['RST','999','surface','ice'],
    ['RST','999','surface','snow'],

    // Source stuff from UFD
    ['SRT','116','source:name','landsat'], // Landsat
    ['SRT','999','source:name','foundation_feature_data/relocatable_target_data_(ffd/rtad)'], //  Foundation Feature Data/Relocatable Target Data (FFD/RTAD)
    ['SRT','999','source:name','nima_gis_medical_facilities_database'], // NIMA GIS Medical Facilities Database
    ['SRT','999','source:name','reference_map'], // Reference Map
    ['SRT','999','source:name','digital_globe'], // Digital Globe
    ['SRT','999','source:name','boundaries_(international_boundaries_database)'], // Boundaries (International Boundaries Database)

    // TRE - Foliage Type
    ['TRE','1','wood','deciduous'], // Deciduous
    ['TRE','2','wood','evergreen'], // Evergreen
    ['TRE','3','wood','mixed'], // Mixed

    ], // End one2oneOut
    // ##### End of One2One Rules #####

    // Stuff to be ignored or that gets swapped later
    // This is taken from OSM pre processing and a few things added.
    ignoreList : ['FCODE','error_circ','CPYRT_NOTE','SRC_INFO','SRC_DATE','SMC'],
    // ##### End of ignoreList #####

    // Attribute values that are to be dropped since they are defaults or don't add value.
    dropList : { '-32765':1,'-32767':1,'-32768':1,
                 '-32765.0':1,'-32767.0':1,'-32768.0':1,
                 '998':1,'n_a':1,'n/a':1,'unknown':1,'unk':1,'fcsubtype':1 },
    // ##### End of dropList #####

    // Attribute names to swap on input. These are to avoid having copies of the same translation.
    swapListIn : {'OHB':'HGT','CPYRT_NOTE':'CCN','SRC_INFO':'SDP','SRC_DATE':'SDV','SMC':'MCC','LEN_':'LEN'},

    // Swap attribute values on export.
    // Format is: <FCODE>:{<from>:<to>}
    swapListOut : {
      'AQ040':{'HGT':'OHB'},
      'AK040':{'MCC':'SMC'},
      'BA050':{'MCC':'SMC'},
      'DA010':{'MCC':'SMC'},
      'DB010':{'MCC':'SMC'},
      'DB070':{'MCC':'SMC'},
    },

    // ##### Start of intList #####
    // This list is for validateing the integer attributes prior to export
    intList : ['LTN','NOS'],
    // ##### End of intList#####


    // Static lookup tables.  If the schema changes, rebuild these in getDbSchema()

    // List of all of the valid MGCP FCODEs and their engilsh name
    fcodeNameLookup : {"AA010":"Extraction Mine","AA012":"Quarry","AA052":"Hydrocarbons Field","AB000":"Disposal Site","AB010":"Recycling Site","AC000":"Processing Facility","AC030":"Settling Pond","AD010":"Electric Power Station","AD030":"Power Substation","AD050":"Heating Facility","AH050":"Fortification","AI030":"Camp","AJ010":"Circular Irrigation System","AJ030":"Holding Pen","AJ110":"Greenhouse","AK030":"Amusement Park","AK040":"Sports Ground","AK060":"Camp-site","AK090":"Fairground","AK100":"Golf Course","AK120":"Park","AK160":"Stadium","AK170":"Swimming Pool","AK180":"Zoo","AK190":"Recreational Pier","AL010":"Facility","AL012":"Archeological Site","AL015":"General Building","AL019":"Shed","AL020":"Built-Up Area","AL030":"Cemetery","AL060":"Dragon's Teeth","AL105":"Settlement","AL140":"Particle Accelerator","AL170":"Public Square","AL200":"Ruins","AL208":"Shanty Town","AM010":"Storage Depot","AM020":"Grain Storage Structure","AM030":"Grain Elevator","AM040":"Mineral Pile","AM060":"Surface Bunker","AM070":"Storage Tank","AN060":"Railway Yard","AN076":"Roundhouse","AQ040":"Bridge","AQ116":"Pumping Station","AQ125":"Transportation Station","AQ130":"Tunnel","AQ135":"Roadside Rest Area","AQ140":"Vehicle Lot","AT050":"Communication Station","BA030":"Island","BA040":"Tidal Water","BA050":"Beach","BB005":"Harbour","BB041":"Breakwater","BB043":"Groin","BB090":"Dry Dock","BB140":"Training Wall","BB190":"Berthing Structure","BB240":"Slipway","BD100":"Structural Pile","BD120":"Reef","BH010":"Aqueduct","BH015":"Bog","BH020":"Canal","BH030":"Ditch","BH040":"Water Treatment Bed","BH050":"Mariculture Site","BH051":"Fish Farm Facility","BH080":"Lake","BH090":"Land Subject to Inundation","BH120":"Rapids","BH130":"Reservoir","BH135":"Rice Field","BH140":"River","BH150":"Salt Flat","BH155":"Salt Evaporator","BH160":"Sabkha","BH165":"Spillway","BI020":"Dam","BI030":"Lock","BJ020":"Moraine","BJ030":"Glacier","BJ031":"Crevasse","BJ100":"Snow Field and/or Ice-field","BJ110":"Tundra","DA010":"Soil Surface Region","DB061":"Crevice","DB090":"Embankment","DB115":"Geothermal Outlet","DB160":"Rock Formation","DB170":"Sand Dunes","DB180":"Volcano","DB200":"Gully","EA010":"Crop Land","EA040":"Orchard","EA050":"Vineyard","EA055":"Hop Field","EB010":"Grassland","EB020":"Thicket","EC010":"Cane","EC020":"Oasis","EC030":"Wood","EC040":"Cleared Way","EC060":"Forest Clearing","ED010":"Marsh","ED020":"Swamp","ED030":"Mangrove Swamp","FA002":"Geopolitical Entity","FA003":"Administrative Division","FA015":"Firing Range","FA100":"Test Site","GB005":"Land Aerodrome","GB015":"Apron","GB035":"Heliport","GB045":"Stopway","GB055":"Runway","GB065":"Water Aerodrome","GB075":"Taxiway","GB230":"Aircraft Hangar","SU001":"Military Installation","ZD020":"Void Collection Area","AF020":"Conveyor","AH025":"Engineered Earthwork","AK130":"Racetrack","AK150":"Ski-jump","AL070":"Fence","AL210":"Protection Shed","AL260":"Wall","AN010":"Railway","AN050":"Railway Sidetrack","AP010":"Cart Track","AP030":"Road","AP050":"Trail","AQ063":"Causeway Structure","AQ070":"Ferry Crossing","AQ075":"Ice Route","AQ113":"Pipeline","AT030":"Power Line","AT041":"Cableway","AT060":"Communication Line","BA010":"Land Water Boundary","BB230":"Seawall","BH060":"Flume","BH070":"Ford","BH110":"Penstock","BH180":"Waterfall","BI040":"Sluice Gate","BI041":"Water Gate","BJ040":"Ice Cliff","CA010":"Elevation Contour","DB010":"Steep Terrain Face","DB070":"Cut","DB071":"Cut Line","DB100":"Esker","DB110":"Geologic Fault","EA020":"Hedgerow","FA090":"Geophysical Prospecting Grid","FA110":"International Date Line","FC021":"Maritime Limit Boundary","GB050":"Aircraft Revetment","AA040":"Rig","AA050":"Well","AC020":"Catalytic Cracker","AD020":"Solar Panel","AF010":"Smokestack","AF040":"Crane","AF070":"Flare Pipe","AH070":"Checkpoint","AJ050":"Windmill","AJ051":"Wind Turbine","AL025":"Cairn","AL099":"Hut","AL130":"Memorial Monument","AL241":"Tower","AN075":"Railway Turntable","AQ065":"Culvert","AQ090":"Entrance and/or Exit","AQ110":"Mooring Mast","AT010":"Dish Aerial","AT042":"Pylon","AT045":"Radar Station","BB155":"Maritime Signal Station","BD110":"Offshore Platform","BD130":"Hazardous Rock","BD180":"Wreck","BH145":"Vanishing Point","BH170":"Natural Pool","BI010":"Cistern","BI050":"Water Intake Tower","BJ060":"Ice Peak","CA030":"Spot Elevation","CA035":"Inland Water Elevation","DB029":"Cave Mouth","DB150":"Mountain Pass","GA034":"Aeronautical Radio Navigation Service","GB030":"Helipad","GB040":"Launch Pad","GB220":"Aeronautical Obstacle","GB485":"Approach Lighting System","ZB050":"Survey","ZD040":"Named Location","ZD045":"Annotated Location"},

    // layerNameLookup - List of all of the valid geometry & F_CODES.
    layerNameLookup : ["AAA010","AAA012","AAA052","AAB000","AAB010","AAC000","AAC030","AAD010","AAD030","AAD050","AAH050","AAI030","AAJ010","AAJ030","AAJ110","AAK030","AAK040","AAK060","AAK090","AAK100","AAK120","AAK160","AAK170","AAK180","AAK190","AAL010","AAL012","AAL015","AAL019","AAL020","AAL030","AAL060","AAL105","AAL140","AAL170","AAL200","AAL208","AAM010","AAM020","AAM030","AAM040","AAM060","AAM070","AAN060","AAN076","AAQ040","AAQ116","AAQ125","AAQ130","AAQ135","AAQ140","AAT050","ABA030","ABA040","ABA050","ABB005","ABB041","ABB043","ABB090","ABB140","ABB190","ABB240","ABD100","ABD120","ABH010","ABH015","ABH020","ABH030","ABH040","ABH050","ABH051","ABH080","ABH090","ABH120","ABH130","ABH135","ABH140","ABH150","ABH155","ABH160","ABH165","ABI020","ABI030","ABJ020","ABJ030","ABJ031","ABJ100","ABJ110","ADA010","ADB061","ADB090","ADB115","ADB160","ADB170","ADB180","ADB200","AEA010","AEA040","AEA050","AEA055","AEB010","AEB020","AEC010","AEC020","AEC030","AEC040","AEC060","AED010","AED020","AED030","AFA002","AFA003","AFA015","AFA100","AGB005","AGB015","AGB035","AGB045","AGB055","AGB065","AGB075","AGB230","ASU001","AZD020","LAF020","LAH025","LAK130","LAK150","LAK190","LAL060","LAL070","LAL210","LAL260","LAN010","LAN050","LAP010","LAP030","LAP050","LAQ040","LAQ063","LAQ070","LAQ075","LAQ113","LAQ130","LAT030","LAT041","LAT060","LBA010","LBB041","LBB043","LBB140","LBB190","LBB230","LBD120","LBH010","LBH020","LBH030","LBH060","LBH070","LBH110","LBH120","LBH140","LBH165","LBH180","LBI020","LBI030","LBI040","LBI041","LBJ031","LBJ040","LCA010","LDB010","LDB061","LDB070","LDB071","LDB090","LDB100","LDB110","LDB160","LDB200","LEA020","LEC030","LFA000","LFA090","LFA110","LFC021","LGB050","PAA010","PAA012","PAA040","PAA050","PAC000","PAC020","PAC030","PAD010","PAD020","PAD030","PAD050","PAF010","PAF040","PAF070","PAH050","PAH070","PAJ030","PAJ050","PAJ051","PAJ110","PAK040","PAK150","PAK160","PAK170","PAL012","PAL015","PAL019","PAL020","PAL025","PAL030","PAL099","PAL105","PAL130","PAL210","PAL241","PAM020","PAM030","PAM040","PAM060","PAM070","PAN075","PAN076","PAQ065","PAQ070","PAQ090","PAQ110","PAQ116","PAQ125","PAT010","PAT042","PAT045","PBA050","PBB155","PBD100","PBD110","PBD130","PBD180","PBH010","PBH070","PBH120","PBH145","PBH170","PBH180","PBI010","PBI020","PBI030","PBI040","PBI041","PBI050","PBJ060","PCA030","PCA035","PDB029","PDB115","PDB150","PDB160","PDB180","PEC030","PGA034","PGB030","PGB040","PGB050","PGB065","PGB220","PGB230","PGB485","PSU001","PZB050","PZD040","PZD045"],

    // Mapping between geometry + FCODE and the Thematic layer name
    thematicGroupLookup : {"AAA010":"ExtractA","AAA012":"ExtractA","AAA052":"PumpingA","AAB000":"DisposeA","AAB010":"DisposeA","AAC000":"ProcessA","AAC030":"TreatA","AAD010":"PowerA","AAD030":"SubstatA","AAD050":"PowerA","AAH050":"FortA","AAI030":"LandmrkA","AAJ010":"LandmrkA","AAJ030":"TreatA","AAJ110":"MiscpopA","AAK030":"LandmrkA","AAK040":"LandmrkA","AAK060":"LandmrkA","AAK090":"SportA","AAK100":"SportA","AAK120":"LandmrkA","AAK160":"LandmrkA","AAK170":"LandmrkA","AAK180":"LandmrkA","AAK190":"PierA","AAL010":"MiscpopA","AAL012":"MiscpopA","AAL015":"BuildA","AAL019":"MiscpopA","AAL020":"BuiltupA","AAL030":"LandmrkA","AAL060":"TransA","AAL105":"MiscpopA","AAL140":"NuclearA","AAL170":"PlazaA","AAL200":"RuinsA","AAL208":"BuiltupA","AAM010":"StorageA","AAM020":"AgristrA","AAM030":"AgristrA","AAM040":"StorageA","AAM060":"StorageA","AAM070":"StorageA","AAN060":"RryardA","AAN076":"TransA","AAQ040":"BridgeA","AAQ116":"PumpingA","AAQ125":"TransA","AAQ130":"TunnelA","AAQ135":"TransA","AAQ140":"TransA","AAT050":"CommA","ABA030":"CoastA","ABA040":"CoastA","ABA050":"CoastA","ABB005":"HarborA","ABB041":"SeastrtA","ABB043":"SeastrtA","ABB090":"HarborA","ABB140":"SeastrtA","ABB190":"PierA","ABB240":"RampA","ABD100":"DangerA","ABD120":"DangerA","ABH010":"AquedctA","ABH015":"SwampA","ABH020":"WatrcrsA","ABH030":"WatrcrsA","ABH040":"TreatA","ABH050":"TreatA","ABH051":"TreatA","ABH080":"LakeresA","ABH090":"InundA","ABH120":"RapidsA","ABH130":"LakeresA","ABH135":"CropA","ABH140":"WatrcrsA","ABH150":"ExtractA","ABH155":"ExtractA","ABH160":"Landfrm1A","ABH165":"DamA","ABI020":"DamA","ABI030":"LockA","ABJ020":"Landfrm2A","ABJ030":"LandIceA","ABJ031":"LandfrmA","ABJ100":"LandIceA","ABJ110":"TundraA","ADA010":"GroundA","ADB061":"LandfrmA","ADB090":"EmbankA","ADB115":"ThermalA","ADB160":"Landfrm2A","ADB170":"Landfrm1A","ADB180":"PhysA","ADB200":"LandfrmA","AEA010":"CropA","AEA040":"CropA","AEA050":"CropA","AEA055":"CropA","AEB010":"GrassA","AEB020":"GrassA","AEC010":"GrassA","AEC020":"OasisA","AEC030":"TreesA","AEC040":"FirebrkA","AEC060":"FirebrkA","AED010":"SwampA","AED020":"SwampA","AED030":"SwampA","AFA002":"PolbndA","AFA003":"PolbndA","AFA015":"LandmrkA","AFA100":"TestA","AGB005":"AerofacA","AGB015":"RunwayA","AGB035":"AerofacA","AGB045":"RunwayA","AGB055":"RunwayA","AGB065":"AerofacA","AGB075":"RunwayA","AGB230":"AerofacA","ASU001":"MilA","AZD020":"VoidA","LAF020":"IndL","LAH025":"MilL","LAK130":"LandmrkL","LAK150":"LandmrkL","LAK190":"PierL","LAL060":"TransL","LAL070":"BarrierL","LAL210":"ShedL","LAL260":"BarrierL","LAN010":"RailrdL","LAN050":"RailrdL","LAP010":"TrackL","LAP030":"RoadL","LAP050":"TrailL","LAQ040":"BridgeL","LAQ063":"EmbankL","LAQ070":"FerryL","LAQ075":"LandmrkL","LAQ113":"PipeL","LAQ130":"TunnelL","LAT030":"PowerL","LAT041":"PowerL","LAT060":"TeleL","LBA010":"CoastL","LBB041":"SeastrtL","LBB043":"SeastrtL","LBB140":"SeastrtL","LBB190":"PierL","LBB230":"SeastrtL","LBD120":"DangerL","LBH010":"AquedctL","LBH020":"WatrcrsL","LBH030":"WatrcrsL","LBH060":"IndL","LBH070":"FordL","LBH110":"MiscL","LBH120":"RapidsL","LBH140":"WatrcrsL","LBH165":"DamL","LBH180":"RapidsL","LBI020":"DamL","LBI030":"LockL","LBI040":"LockL","LBI041":"MiscL","LBJ031":"LandfrmL","LBJ040":"LandfrmL","LCA010":"ContourL","LDB010":"BluffL","LDB061":"LandfrmL","LDB070":"EmbankL","LDB071":"EmbankL","LDB090":"EmbankL","LDB100":"LandfrmL","LDB110":"LandfrmL","LDB160":"LandfrmL","LDB200":"LandfrmL","LEA020":"TreesL","LEC030":"TreesL","LFA000":"PolbndL","LFA090":"IndL","LFA110":"PolbndL","LFC021":"PolbndL","LGB050":"RunwayL","PAA010":"ExtractP","PAA012":"ExtractP","PAA040":"RigwellP","PAA050":"WellsprP","PAC000":"ProcessP","PAC020":"RigwellP","PAC030":"TreatP","PAD010":"PowerP","PAD020":"PowerP","PAD030":"SubstatP","PAD050":"PowerP","PAF010":"ObstrP","PAF040":"ObstrP","PAF070":"ObstrP","PAH050":"FortP","PAH070":"LandmrkP","PAJ030":"TreatP","PAJ050":"ObstrP","PAJ051":"UtilP","PAJ110":"MiscpopP","PAK040":"LandmrkP","PAK150":"LandmrkP","PAK160":"LandmrkP","PAK170":"LandmrkP","PAL012":"MiscpopP","PAL015":"BuildP","PAL019":"MiscpopP","PAL020":"BuiltupP","PAL025":"MarkersP","PAL030":"LandmrkP","PAL099":"MiscpopP","PAL105":"MiscpopP","PAL130":"LandmrkP","PAL210":"ShedP","PAL241":"TowerP","PAM020":"AgristrP","PAM030":"AgristrP","PAM040":"StorageP","PAM060":"StorageP","PAM070":"StorageP","PAN075":"RrturnP","PAN076":"TransP","PAQ065":"TransP","PAQ070":"FerryP","PAQ090":"TransP","PAQ110":"MiscaeroP","PAQ116":"PumpingP","PAQ125":"TransP","PAT010":"CommP","PAT042":"UtilP","PAT045":"CommP","PBA050":"CoastP","PBB155":"HarborP","PBD100":"DangerP","PBD110":"DangerP","PBD130":"DangerP","PBD180":"DangerP","PBH010":"AquedctP","PBH070":"FordP","PBH120":"RapidsP","PBH145":"RapidsP","PBH170":"WellsprP","PBH180":"RapidsP","PBI010":"CisternP","PBI020":"DamP","PBI030":"LockP","PBI040":"LockP","PBI041":"MiscP","PBI050":"MiscP","PBJ060":"LandfrmP","PCA030":"ElevP","PCA035":"ElevP","PDB029":"MtnP","PDB115":"ThermalP","PDB150":"MtnP","PDB160":"LandfrmP","PDB180":"ThermalP","PEC030":"TreesP","PGA034":"MiscaeroP","PGB030":"AerofacP","PGB040":"MiscaeroP","PGB050":"RunwayP","PGB065":"AerofacP","PGB220":"MiscaeroP","PGB230":"AerofacP","PGB485":"MiscaeroP","PSU001":"MilP","PZB050":"ElevP","PZD040":"TextP","PZD045":"AnnoP"},


    // fCodeList - This is a list of all of the valid MGCP F_CODES in TRD4.6
    // fcodeList : ["AA010","AA012","AA040","AA050","AA052","AB000","AB010","AC000","AC020","AC030","AD010","AD020","AD030","AD050","AF010","AF020","AF040","AF070","AH025","AH050","AH070","AI030","AJ010","AJ030","AJ050","AJ051","AJ110","AK030","AK040","AK060","AK090","AK100","AK120","AK130","AK150","AK160","AK170","AK180","AK190","AL010","AL012","AL015","AL019","AL020","AL025","AL030","AL060","AL070","AL099","AL105","AL130","AL140","AL170","AL200","AL208","AL210","AL241","AL260","AM010","AM020","AM030","AM040","AM060","AM070","AN010","AN050","AN060","AN075","AN076","AP010","AP030","AP050","AQ040","AQ063","AQ065","AQ070","AQ075","AQ090","AQ110","AQ113","AQ116","AQ125","AQ130","AQ135","AQ140","AT010","AT030","AT041","AT042","AT045","AT050","AT060","BA010","BA030","BA040","BA050","BB005","BB041","BB043","BB090","BB140","BB155","BB190","BB230","BB240","BD100","BD110","BD120","BD130","BD180","BH010","BH015","BH020","BH030","BH040","BH050","BH051","BH060","BH070","BH080","BH090","BH110","BH120","BH130","BH135","BH140","BH145","BH150","BH155","BH160","BH165","BH170","BH180","BI010","BI020","BI030","BI040","BI041","BI050","BJ020","BJ030","BJ031","BJ040","BJ060","BJ100","BJ110","DA010","DB010","DB029","DB061","DB070","DB071","DB090","DB100","DB110","DB115","DB150","DB160","DB170","DB180","DB200","EA010","EA020","EA040","EA050","EA055","EB010","EB020","EC010","EC020","EC030","EC040","EC060","ED010","ED020","ED030","FA015","FA090","FA100","GA034","GB005","GB015","GB030","GB035","GB040","GB045","GB050","GB055","GB065","GB075","GB220","GB230","GB485","SU001","ZD020","ZD040"],


} // End of mgcp.rules
