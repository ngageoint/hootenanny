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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

/*
    TDSv70 One2one rules
*/

tds70.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    // This is where anything "unique" to TDS goes.
    fcodeOne2oneIn : [
    // ['F_CODE','AP030','highway','road'], // Road
    ], // End fcodeOne2oneIn

    // These are for finding an FCODE for export.
    // Note: These get swapped around before being used
    fcodeOne2oneOut : [
//     ['F_CODE','AN060','railway','yes'], // Rail yards
    // ['F_CODE','EA010','landuse','farm'], // Alt version of "landuse=farmland"
    // ['F_CODE','EC015','natural','wood'], // Alt version of Forest
    ['F_CODE','AA010','landuse','quarry'], // Extraction Mine
    ['F_CODE','AD010','power','station'], // Electric Power Station, Power Plant
    ['F_CODE','AJ110','building','greenhouse_horticulture'], // Greenhouse
    ['F_CODE','AK030','leisure','water_park'], // Amusement Park
    ['F_CODE','AK120','landuse','recreation_ground'], // Park
    ['F_CODE','AK120','landuse','village_green'], // Park
    ['F_CODE','AK120','leisure','common'], // Park - close but not great
    ['F_CODE','AK120','leisure','nature_reserve'], // Park
    ['F_CODE','AK120','leisure','playground'], // Park
    ['F_CODE','AK170','amenity','swimming_pool'], // Pool
    ['F_CODE','AL020','landuse','industrial'], // From OSM - Map to BUA
    ['F_CODE','AL020','landuse','residential'], // From OSM - Map to BUA
    ['F_CODE','AL030','amenity','grave_yard'], // From OSM Data
    ['F_CODE','AL130','historic','memorial'], // From OSM data, not great
    ['F_CODE','AL200','ruins','yes'], // Ruins
    ['F_CODE','AL241','tower','communication'],
    ['F_CODE','AL241','tower','yes'],
    ['F_CODE','AM080','tower:type','water'],
    ['F_CODE','AP030','highway','motorway'], // Limited Access Motorway
    ['F_CODE','AP030','highway','motorway_link'], // Limited Access Motorway
    ['F_CODE','AP030','highway','primary'],
    ['F_CODE','AP030','highway','primary_link'],
    ['F_CODE','AP030','highway','residential'],
    ['F_CODE','AP030','highway','secondary'],
    ['F_CODE','AP030','highway','secondary_link'],
    ['F_CODE','AP030','highway','service'],
    ['F_CODE','AP030','highway','tertiary'],
    ['F_CODE','AP030','highway','tertiary_link'],
    ['F_CODE','AP030','highway','trunk'], // Motorway
    ['F_CODE','AP030','highway','trunk_link'], // Motorway
    ['F_CODE','AP030','highway','unclassified'],
    ['F_CODE','AP030','highway','yes'], // Just in case...
    ['F_CODE','AP040','barrier','lift_gate'], // Gate
    ['F_CODE','AP040','railway','crossing'], // Pedestrian Crossing - See custom rules
    ['F_CODE','AP040','railway','level_crossing'], // Level Crossing - See custom rules
    ['F_CODE','AP050','highway','footway'], // Trail
    ['F_CODE','AQ125','highway','bus_station'], // Transportaion Station
    ['F_CODE','AQ125','highway','bus_stop'], // Transportaion Station
    ['F_CODE','AQ125','public_transport','stop_position'],
    ['F_CODE','AQ125','railway','station'], // Railway Station
    ['F_CODE','AQ141','parking','multi-story'], // Parking Garage
    ['F_CODE','AT042','power','pole'], // OSM
    ['F_CODE','AT042','power','tower'], // OSM
    ['F_CODE','BA010','natural','land_water_boundary'], // Land/Water Boundary - old translation
    ['F_CODE','BA030','place','islet'], // Island - From OSM
    ['F_CODE','BH140','waterway','yes'], // Make unknown waterways into rivers
    ['F_CODE','BH145','natural','sinkhole'],
    ['F_CODE','BH155','man_made','salt_pond'], // Due to the default translation changing
    ['F_CODE','ED010','natural','wetland'], // Marsh
    ['F_CODE','ED010','wetland','reedbed'], // Marsh
    ['F_CODE','FA015','man_made','shooting_range'], // Firing Range
    ['F_CODE','GB005','aeroway','sport_airport'], // Land Aerodrome
    ['F_CODE','ZB050','survey:point:type','benchmark'], // GGDM3 now has this as an FCODE
    ['F_CODE','ZB050','survey:point:type','geodetic'], // GGDM3 now has this as an FCODE
    ['F_CODE','ZD040','natural','bay'], // From OSM
    ['F_CODE','ZD040','natural','cape'], // From OSM
    ['F_CODE','ZD040','natural','hill'], // From GeoNames
    ['F_CODE','ZD040','natural','peak'], // From OSM
    ['F_CODE','ZD040','natural','valley'], // From GeoNames
    ], // End fcodeOne2oneOut

    // One2one rules for Text Fields
    txtBiased : {
    'ADR':'addr:full', // Address
    'BA000_VDR':'source:datum:sounding:name', // Water Line : Sounding Datum Name
    'BRN':'bridge:reference', // Bridge Reference Number
    'CCN':'source:copyright', // Commercial Copyright Notice
    'CDR':'source:commercial_distribution_restriction', // Commercial Distribution Restriction
    'CID':'source:cell_identifier', // Cell Identifier
    'DQS':'source:data_quality', // Data Quality Statement
    'ETZ':'source:extraction_specification:version', // Extraction Specification Version
//     'F_CODE':'raw:F_CODE', // Feature Code
    'GB052_RIDH':'aeroway:runway_designator_high', // Runway Direction (high end) : Runway Designator (high)
    'GB052_RIDL':'aeroway:runway_designator_low', // Runway Direction (low end) : Runway Designator (low)
    'IKO':'icao', // ICAO Location Indicator
    'MDE':'source:update:datetime', // Maintenance Date and Time
    'NA8':'official_name', // Aerodrome Official Name
    'OTH':'note:oth', // Specified Domain Value(s)
    'RIN_RTN':'ref', // Route Identification <route designation>
    'RIN_RTN2':'ref:2', // Route Identification [2] <route designation>
    'RIN_RTN3':'ref:3', // Route Identification [3] <route designation>
    'UFI':'uuid', // Unique Feature Identifier. This gets cleaned up in post processing
    'URI':'source:ref', // Unique Resource Identifier
    'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
    'WPI':'world_port_index', // World Port Index Identifier - This is part of Built Up Area. What??
    //'ZI001_SDP':'source:description', // Source Information : Source Description
    'ZI001_SDP':'source', // Source Information : Source Description
    'ZI001_SDV':'source:datetime', // Source Information : Source Date and Time
    'ZI001_VSD':'source:vertical_source:datetime', // Source Information : Vertical Source Date and Time
    'ZI001_VSN':'source:vertical_source:description', // Source Information : Vertical Source Description
    'ZI004_PRE':'source:processing_information', // Process Step Information : Process Step Description
    'ZI005_FNA':'name', // Geographic Name Information : Full Name
    'ZI005_FNA2':'alt_name', // Geographic Name Information : (second) Full Name
    'ZI005_NFN':'gndb_id', // Geographic Name Information : Name Identifier
    'ZI005_NFN2':'gndb_id:2', // Geographic Name Information : (second) Name Identifier
    'ZI006_MEM':'note', // Note : Memorandum
    // We fix ZI020_GE4X in post processing
    'ZI020_GE4':'is_in:country_code', // (Location Country) Designation : GENC Short URN-based Identifier
    'ZI020_GE42':'country_code:second', // (Location Country) Designation : GENC Short URN-based Identifier (second)
    'ZI020_GE43':'country_code:third', // (Location Country) Designation : GENC Short URN-based Identifier (third)
    'ZI020_GE44':'country_code:fourth', // (Location Country) Designation : GENC Short URN-based Identifier (fourth)
    'ZSAX_RX0':'security:dissemination_control:ic', // Restriction Information : Security Attributes Group <resource dissemination controls>
    'ZSAX_RX3':'security:dissemination_control:non_ic', // Restriction Information : Security Attributes Group <resource non-intelligence community markings>
    'ZSAX_RX4':'security:resource_owner', // Restriction Information : Security Attributes Group <resource owner-producer>
    }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
    'AHA':'source:accuracy:horizontal', // Absolute Horizontal Accuracy (90%)
    'AOO':'angle', // Angle of Orientation
    'ARA':'feature_area', // Area - this has been moved from 'area' due to hoot conflicts
    'AVA':'source:accuracy:vertical', // Absolute Vertical Accuracy (90%)
    'BNF':'building:levels', // Floor Count
    'CTL':'railway:track:length', // Cumulative Track Length
    'DEV':'level', // Deck Level
    'DIM':'diameter', // Diameter
    'DMT':'canopy_cover', // Canopy Cover
    'DOF':'flow_direction', // Direction of Flow
    'DZC':'deck_count', // Deck Count
    'DZP':'depth:maximum_below_surface', // Deepest Depth Below Surface Level
    'EPW':'generator:output:electricity', // Electrical Power Generation Capacity
    'EVA':'source:accuracy:elevation', // Elevation Vertical Accuracy (90%)
//    'FCSUBTYPE':'etds:fcsubtype', // Very ESRI Specific. Ignored for now
    'GSGCHL':'aeroway:runway_highend:slope:high_lowervalue', // Runway Direction (high end) : Surface Slope (high) (lower value)
    'GSGCHU':'aeroway:runway_highend:slope:high_uppervalue', // Runway Direction (high end) : Surface Slope (high) (upper value)
    'GSGCLL':'aeroway:runway_highend:slope:low_lowervalue', // Runway Direction (low end) : Surface Slope (low) (lower value)
    'GSGCLU':'aeroway:runway_highend:slope:low_uppervalue', // Runway Direction (low end) : Surface Slope (low) (upper value)
    'HCA':'bridge:horizontal_clearance', // Horizontal Clearance
    'HEI':'height:object', // Height of Object
    'HGS':'spillway:height', // Spillway Height
    'HGT':'height', // Height Above Surface Level
    'HVA':'source:accuracy:height', // Height Vertical Accuracy (90%)
    'KVA':'voltage', // Power Line Maximum Voltage
    'LC1':'bridge:load_class1', // Load Class Type 1
    'LC2':'bridge:load_class2', // Load Class Type 2
    'LC3':'bridge:load_class3', // Load Class Type 3
    'LC4':'bridge:load_class4', // Load Class Type 4
    'LDC':'length:crest', // Dam Crest Length
    'LEA':'depth:minimum_below_surface', // Least Depth Below Surface Level
    'LTN':'lanes', // Track or Lane Count
    'LZN':'length', // Length
    'MAG':'magnetic_variation', // Magnetic Variation
    'MWG':'divider:width', // Centerline Spacing
    'NOS':'bridge:span_count', // Span Count
    'NPL':'cables', // Parallel Line Count: See AT005
    'OHC':'max_clearance', // Overhead Clearance
    'PFD':'raw:PFD', // Predominant Feature Depth
    'PFH':'height:feature', // Predominant Feature Height
    'PVH':'height:vegetation', // Predominant Vegetation Height
    'PWA':'depth', // Predominant Water Depth
    'RAD':'curve_radius', // Curve Radius
    'RMWL':'median:min_width', // Route Median Width (lower value)
    'RMWU':'median:max_width', // Route Median Width (upper value)
    'SDO':'dune:orientation', // Sand Dune Orientation
    'SGCL':'incline:min', // Surface Slope (lower value)
    'SGCU':'incline:max', // Surface Slope (upper value)
    'SPM':'maxspeed', // Speed Limit (KPH)
    'THI':'thickness', // Thickness
    'UBC':'bridge:under_clearance', // Underbridge Clearance
    'VCS':'safe_clearance:vertical', // Vertical Clearance, Safe
    'VTI':'trafficability_impact', // Vegetation Trafficability Impact
    'WBD':'raw:WBD', // Waterbody Depth
    'WD3':'width:gap', // Terrain Gap Width
    'WDU':'width:interior_useable', // Usable Width
    'WID':'width', // Width
    'WOC':'width:crest', // Dam Crest Width
    'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
    'ZI016_WD2':'raw:ZI016_WD2', // Route Pavement Information : Route Total Usable Width
    'ZI016_WT2':'raw:ZI016_WT2', // Route Pavement Information : Width of Second Travelled Way
    'ZI017_GAW':'raw:ZI017_GAW', // Track Information : Railway Gauge
    'ZI026_CTUL':'cartographic_scale:lower', // Feature Metadata : Cartographic Usability Range <lower value>
    'ZI026_CTUU':'cartographic_scale:upper', // Feature Metadata : Cartographic Usability Range <upper value>
    'ZVA':'ele', // Aerodrome Elevation
    'ZVH':'ele:max' // Highest Elevation
    }, // End numBiased
    

    // Common one2one rules. Used for both import and export
    one2one : [

     // ACC - Horizontal Accuracy Category
     // ['ACC','-999999',undefined,undefined], // noInformation
     ['ACC','1','raw:ACC','accurate'], // Accurate
     ['ACC','2','raw:ACC','approximate'], // Approximate
     ['ACC','3','raw:ACC','doubtful'], // Doubtful

     // AMA - Amusement Attraction Type
     // ['AMA','-999999',undefined,undefined], // noInformation
     ['AMA','1','raw:AMA','artificial_mountain'], // Artificial Mountain
     ['AMA','2','raw:AMA','ferris_wheel'], // Ferris Wheel
     ['AMA','3','raw:AMA','roller-coaster'], // Roller-coaster
     ['AMA','4','raw:AMA','spherical'], // Spherical
     ['AMA','5','raw:AMA','water_attraction'], // Water Attraction
     ['AMA','6','raw:AMA','vertical_ride'], // Vertical Ride
     ['AMA','999','raw:AMA','other'], // Other

     // APT - Airfield Use
     // ['APT','-999999',undefined,undefined], // noInformation
     ['APT','1','raw:APT','major_airfield'], // Major Airfield
     ['APT','2','raw:APT','minor_airfield'], // Minor Airfield
     ['APT','3','raw:APT','general_aviation_aircraft_operating_only'], // General Aviation Aircraft Operating Only
     ['APT','5','raw:APT','glider_site'], // Glider Site
     ['APT','6','raw:APT','ultralight_site'], // Ultralight Site
     ['APT','7','raw:APT','hang_glider_site'], // Hang Glider Site
     ['APT','8','raw:APT','winch_launched_hang_glider_site'], // Winch Launched Hang Glider Site
     ['APT','12','raw:APT','emergency'], // Emergency
     ['APT','13','raw:APT','parascending_site'], // Parascending Site
     ['APT','16','raw:APT','search_and_rescue_airfield'], // Search and Rescue Airfield
     ['APT','999','raw:APT','other'], // Other

     // APT2 - Airfield Use [2]
     // ['APT2','-999999',undefined,undefined], // noInformation
     ['APT2','1','raw:APT2','major_airfield'], // Major Airfield
     ['APT2','2','raw:APT2','minor_airfield'], // Minor Airfield
     ['APT2','3','raw:APT2','general_aviation_aircraft_operating_only'], // General Aviation Aircraft Operating Only
     ['APT2','5','raw:APT2','glider_site'], // Glider Site
     ['APT2','6','raw:APT2','ultralight_site'], // Ultralight Site
     ['APT2','7','raw:APT2','hang_glider_site'], // Hang Glider Site
     ['APT2','8','raw:APT2','winch_launched_hang_glider_site'], // Winch Launched Hang Glider Site
     ['APT2','12','raw:APT2','emergency'], // Emergency
     ['APT2','13','raw:APT2','parascending_site'], // Parascending Site
     ['APT2','16','raw:APT2','search_and_rescue_airfield'], // Search and Rescue Airfield
     ['APT2','999','raw:APT2','other'], // Other

     // APT3 - Airfield Use [3]
     // ['APT3','-999999',undefined,undefined], // noInformation
     ['APT3','1','raw:APT3','major_airfield'], // Major Airfield
     ['APT3','2','raw:APT3','minor_airfield'], // Minor Airfield
     ['APT3','3','raw:APT3','general_aviation_aircraft_operating_only'], // General Aviation Aircraft Operating Only
     ['APT3','5','raw:APT3','glider_site'], // Glider Site
     ['APT3','6','raw:APT3','ultralight_site'], // Ultralight Site
     ['APT3','7','raw:APT3','hang_glider_site'], // Hang Glider Site
     ['APT3','8','raw:APT3','winch_launched_hang_glider_site'], // Winch Launched Hang Glider Site
     ['APT3','12','raw:APT3','emergency'], // Emergency
     ['APT3','13','raw:APT3','parascending_site'], // Parascending Site
     ['APT3','16','raw:APT3','search_and_rescue_airfield'], // Search and Rescue Airfield
     ['APT3','999','raw:APT3','other'], // Other

     // APU - Apron Usage
     // ['APU','-999999',undefined,undefined], // noInformation
     ['APU','1','raw:APU','parking'], // Parking
     ['APU','2','raw:APU','loading_and/or_unloading'], // Loading and/or Unloading
     ['APU','3','raw:APU','fuelling'], // Fuelling
     ['APU','4','raw:APU','transient'], // Transient
     ['APU','5','raw:APU','compass_rose'], // Compass Rose
     ['APU','6','raw:APU','engine_run_up'], // Engine Run Up
     ['APU','7','raw:APU','maintenance'], // Maintenance
     ['APU','8','raw:APU','alert'], // Alert
     ['APU','9','raw:APU','firing-in'], // Firing-In
     ['APU','10','raw:APU','hot_refuelling'], // Hot Refuelling
     ['APU','11','raw:APU','weapon_loading'], // Weapon Loading
     ['APU','12','raw:APU','de-icing_and/or_anti-icing'], // De-icing and/or Anti-icing
     ['APU','13','raw:APU','ins_alignment'], // INS Alignment
     ['APU','14','raw:APU','decontamination'], // Decontamination
     ['APU','999','raw:APU','other'], // Other

     // APU2 - Apron Usage [2]
     // ['APU2','-999999',undefined,undefined], // noInformation
     ['APU2','1','raw:APU2','parking'], // Parking
     ['APU2','2','raw:APU2','loading_and/or_unloading'], // Loading and/or Unloading
     ['APU2','3','raw:APU2','fuelling'], // Fuelling
     ['APU2','4','raw:APU2','transient'], // Transient
     ['APU2','5','raw:APU2','compass_rose'], // Compass Rose
     ['APU2','6','raw:APU2','engine_run_up'], // Engine Run Up
     ['APU2','7','raw:APU2','maintenance'], // Maintenance
     ['APU2','8','raw:APU2','alert'], // Alert
     ['APU2','9','raw:APU2','firing-in'], // Firing-In
     ['APU2','10','raw:APU2','hot_refuelling'], // Hot Refuelling
     ['APU2','11','raw:APU2','weapon_loading'], // Weapon Loading
     ['APU2','12','raw:APU2','de-icing_and/or_anti-icing'], // De-icing and/or Anti-icing
     ['APU2','13','raw:APU2','ins_alignment'], // INS Alignment
     ['APU2','14','raw:APU2','decontamination'], // Decontamination
     ['APU2','999','raw:APU2','other'], // Other

     // APU3 - Apron Usage [3]
     // ['APU3','-999999',undefined,undefined], // noInformation
     ['APU3','1','raw:APU3','parking'], // Parking
     ['APU3','2','raw:APU3','loading_and/or_unloading'], // Loading and/or Unloading
     ['APU3','3','raw:APU3','fuelling'], // Fuelling
     ['APU3','4','raw:APU3','transient'], // Transient
     ['APU3','5','raw:APU3','compass_rose'], // Compass Rose
     ['APU3','6','raw:APU3','engine_run_up'], // Engine Run Up
     ['APU3','7','raw:APU3','maintenance'], // Maintenance
     ['APU3','8','raw:APU3','alert'], // Alert
     ['APU3','9','raw:APU3','firing-in'], // Firing-In
     ['APU3','10','raw:APU3','hot_refuelling'], // Hot Refuelling
     ['APU3','11','raw:APU3','weapon_loading'], // Weapon Loading
     ['APU3','12','raw:APU3','de-icing_and/or_anti-icing'], // De-icing and/or Anti-icing
     ['APU3','13','raw:APU3','ins_alignment'], // INS Alignment
     ['APU3','14','raw:APU3','decontamination'], // Decontamination
     ['APU3','999','raw:APU3','other'], // Other

     // APY - Apron Type
     // ['APY','-999999',undefined,undefined], // noInformation
     ['APY','1','raw:APY','passenger'], // Passenger
     ['APY','2','raw:APY','cargo'], // Cargo
     ['APY','3','raw:APY','remote_parking'], // Remote Parking
     ['APY','4','raw:APY','services_and/or_hangar'], // Services and/or Hangar
     ['APY','5','raw:APY','general_aviation'], // General Aviation
     ['APY','6','raw:APY','military'], // Military
     ['APY','7','raw:APY','dispersal'], // Dispersal
     ['APY','8','raw:APY','holding'], // Holding
     ['APY','999','raw:APY','other'], // Other

     // ASU - Aerodrome Movement Area Surface Composition
     // ['ASU','-999999',undefined,undefined], // noInformation
     ['ASU','1','raw:ASU','asphalt'], // Asphalt
     ['ASU','2','raw:ASU','bituminous_mix'], // Bituminous Mix
     ['ASU','3','raw:ASU','brick'], // Brick
     ['ASU','4','raw:ASU','clay'], // Clay
     ['ASU','5','raw:ASU','concrete'], // Concrete
     ['ASU','6','raw:ASU','coral'], // Coral
     ['ASU','7','raw:ASU','earthen'], // Earthen
     ['ASU','8','raw:ASU','gravel'], // Gravel
     ['ASU','9','raw:ASU','ice'], // Ice
     ['ASU','10','raw:ASU','landing_mat'], // Landing Mat
     ['ASU','11','raw:ASU','laterite'], // Laterite
     ['ASU','12','raw:ASU','macadam'], // Macadam
     ['ASU','13','raw:ASU','membrane'], // Membrane
     ['ASU','14','raw:ASU','non-bituminous_mix'], // Non-bituminous Mix
     ['ASU','15','raw:ASU','pierced_steel_planking'], // Pierced Steel Planking
     ['ASU','16','raw:ASU','sand'], // Sand
     ['ASU','17','raw:ASU','snow'], // Snow
     ['ASU','18','raw:ASU','stone'], // Stone
     ['ASU','19','raw:ASU','water'], // Water
     ['ASU','20','raw:ASU','wood'], // Wood
     ['ASU','21','raw:ASU','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ASU','999','raw:ASU','other'], // Other

     // ASU2 - Aerodrome Movement Area Surface Composition [2]
     // ['ASU2','-999999',undefined,undefined], // noInformation
     ['ASU2','1','raw:ASU2','asphalt'], // Asphalt
     ['ASU2','2','raw:ASU2','bituminous_mix'], // Bituminous Mix
     ['ASU2','3','raw:ASU2','brick'], // Brick
     ['ASU2','4','raw:ASU2','clay'], // Clay
     ['ASU2','5','raw:ASU2','concrete'], // Concrete
     ['ASU2','6','raw:ASU2','coral'], // Coral
     ['ASU2','7','raw:ASU2','earthen'], // Earthen
     ['ASU2','8','raw:ASU2','gravel'], // Gravel
     ['ASU2','9','raw:ASU2','ice'], // Ice
     ['ASU2','10','raw:ASU2','landing_mat'], // Landing Mat
     ['ASU2','11','raw:ASU2','laterite'], // Laterite
     ['ASU2','12','raw:ASU2','macadam'], // Macadam
     ['ASU2','13','raw:ASU2','membrane'], // Membrane
     ['ASU2','14','raw:ASU2','non-bituminous_mix'], // Non-bituminous Mix
     ['ASU2','15','raw:ASU2','pierced_steel_planking'], // Pierced Steel Planking
     ['ASU2','16','raw:ASU2','sand'], // Sand
     ['ASU2','17','raw:ASU2','snow'], // Snow
     ['ASU2','18','raw:ASU2','stone'], // Stone
     ['ASU2','19','raw:ASU2','water'], // Water
     ['ASU2','20','raw:ASU2','wood'], // Wood
     ['ASU2','21','raw:ASU2','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ASU2','999','raw:ASU2','other'], // Other

     // ASU3 - Aerodrome Movement Area Surface Composition [3]
     // ['ASU3','-999999',undefined,undefined], // noInformation
     ['ASU3','1','raw:ASU3','asphalt'], // Asphalt
     ['ASU3','2','raw:ASU3','bituminous_mix'], // Bituminous Mix
     ['ASU3','3','raw:ASU3','brick'], // Brick
     ['ASU3','4','raw:ASU3','clay'], // Clay
     ['ASU3','5','raw:ASU3','concrete'], // Concrete
     ['ASU3','6','raw:ASU3','coral'], // Coral
     ['ASU3','7','raw:ASU3','earthen'], // Earthen
     ['ASU3','8','raw:ASU3','gravel'], // Gravel
     ['ASU3','9','raw:ASU3','ice'], // Ice
     ['ASU3','10','raw:ASU3','landing_mat'], // Landing Mat
     ['ASU3','11','raw:ASU3','laterite'], // Laterite
     ['ASU3','12','raw:ASU3','macadam'], // Macadam
     ['ASU3','13','raw:ASU3','membrane'], // Membrane
     ['ASU3','14','raw:ASU3','non-bituminous_mix'], // Non-bituminous Mix
     ['ASU3','15','raw:ASU3','pierced_steel_planking'], // Pierced Steel Planking
     ['ASU3','16','raw:ASU3','sand'], // Sand
     ['ASU3','17','raw:ASU3','snow'], // Snow
     ['ASU3','18','raw:ASU3','stone'], // Stone
     ['ASU3','19','raw:ASU3','water'], // Water
     ['ASU3','20','raw:ASU3','wood'], // Wood
     ['ASU3','21','raw:ASU3','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ASU3','999','raw:ASU3','other'], // Other

     // ASY - Airfield Symbol Type
     // ['ASY','-999999',undefined,undefined], // noInformation
     ['ASY','1','raw:ASY','a_-_active_civilian'], // A - Active Civilian
     ['ASY','2','raw:ASY','b_-_active_joint_(civilian/military)'], // B - Active Joint (Civilian/Military)
     ['ASY','3','raw:ASY','c_-_active_military'], // C - Active Military
     ['ASY','4','raw:ASY','d_-_active_(with_less_than_the_minimum_facilities)'], // D - Active (with less than the minimum facilities)
     ['ASY','5','raw:ASY','e_-_abandoned_or_closed_(with_usable_runways_or_landing_areas)'], // E - Abandoned or Closed (with usable runways or landing areas)
     ['ASY','6','raw:ASY','f_-_highway_strip'], // F - Highway Strip
     ['ASY','7','raw:ASY','g_-_unusable_for_landing_or_take-off'], // G - Unusable for landing or take-off
     ['ASY','8','raw:ASY','h_-_active_military_heliport'], // H - Active Military Heliport
     ['ASY','9','raw:ASY','j_-_active_civil_heliport'], // J - Active Civil Heliport
     ['ASY','10','raw:ASY','k_-_active_military_heliport_(with_less_than_minimum_facilities)'], // K - Active Military Heliport (with less than minimum facilities)
     ['ASY','11','raw:ASY','l_-_active_civil_heliport_(with_less_than_minimum_facilities)'], // L - Active Civil Heliport (with less than minimum facilities)
     ['ASY','12','raw:ASY','x_-_decoy'], // X - Decoy
     ['ASY','13','raw:ASY','active_joint_(civilian/military)_heliport'], // Active Joint (Civilian/Military) Heliport

     // AT005_CAB - Cable : Cable Type
     // ['AT005_CAB','-999999',undefined,undefined], // noInformation
     ['AT005_CAB','2','raw:AT005_CAB','power_distribution_line'], // Power Distribution Line
     ['AT005_CAB','3','raw:AT005_CAB','telephone'], // Telephone
     ['AT005_CAB','4','raw:AT005_CAB','telegraph'], // Telegraph
     ['AT005_CAB','6','raw:AT005_CAB','power_transmission_line'], // Power Transmission Line
     ['AT005_CAB','7','raw:AT005_CAB','digital_communication_line'], // Digital Communication Line
     ['AT005_CAB','8','raw:AT005_CAB','communication_line'], // Communication Line
     ['AT005_CAB','9','raw:AT005_CAB','fibre-optic'], // Fibre-optic
     ['AT005_CAB','10','raw:AT005_CAB','tethering'], // Tethering
     ['AT005_CAB','11','raw:AT005_CAB','load_bearing'], // Load Bearing
     ['AT005_CAB','12','raw:AT005_CAB','guide'], // Guide
     ['AT005_CAB','13','raw:AT005_CAB','barrier'], // Barrier
     ['AT005_CAB','14','raw:AT005_CAB','restraining'], // Restraining
     ['AT005_CAB','15','raw:AT005_CAB','towing'], // Towing
     ['AT005_CAB','19','raw:AT005_CAB','cableway'], // Cableway
     ['AT005_CAB','999','raw:AT005_CAB','other'], // Other

     // AT005_CAB2 - Cable : Cable Type [2]
     // ['AT005_CAB2','-999999',undefined,undefined], // noInformation
     ['AT005_CAB2','2','raw:AT005_CAB2','power_distribution_line'], // Power Distribution Line
     ['AT005_CAB2','3','raw:AT005_CAB2','telephone'], // Telephone
     ['AT005_CAB2','4','raw:AT005_CAB2','telegraph'], // Telegraph
     ['AT005_CAB2','6','raw:AT005_CAB2','power_transmission_line'], // Power Transmission Line
     ['AT005_CAB2','7','raw:AT005_CAB2','digital_communication_line'], // Digital Communication Line
     ['AT005_CAB2','8','raw:AT005_CAB2','communication_line'], // Communication Line
     ['AT005_CAB2','9','raw:AT005_CAB2','fibre-optic'], // Fibre-optic
     ['AT005_CAB2','10','raw:AT005_CAB2','tethering'], // Tethering
     ['AT005_CAB2','11','raw:AT005_CAB2','load_bearing'], // Load Bearing
     ['AT005_CAB2','12','raw:AT005_CAB2','guide'], // Guide
     ['AT005_CAB2','13','raw:AT005_CAB2','barrier'], // Barrier
     ['AT005_CAB2','14','raw:AT005_CAB2','restraining'], // Restraining
     ['AT005_CAB2','15','raw:AT005_CAB2','towing'], // Towing
     ['AT005_CAB2','19','raw:AT005_CAB2','cableway'], // Cableway
     ['AT005_CAB2','999','raw:AT005_CAB2','other'], // Other

     // AT005_CAB3 - Cable : Cable Type [3]
     // ['AT005_CAB3','-999999',undefined,undefined], // noInformation
     ['AT005_CAB3','2','raw:AT005_CAB3','power_distribution_line'], // Power Distribution Line
     ['AT005_CAB3','3','raw:AT005_CAB3','telephone'], // Telephone
     ['AT005_CAB3','4','raw:AT005_CAB3','telegraph'], // Telegraph
     ['AT005_CAB3','6','raw:AT005_CAB3','power_transmission_line'], // Power Transmission Line
     ['AT005_CAB3','7','raw:AT005_CAB3','digital_communication_line'], // Digital Communication Line
     ['AT005_CAB3','8','raw:AT005_CAB3','communication_line'], // Communication Line
     ['AT005_CAB3','9','raw:AT005_CAB3','fibre-optic'], // Fibre-optic
     ['AT005_CAB3','10','raw:AT005_CAB3','tethering'], // Tethering
     ['AT005_CAB3','11','raw:AT005_CAB3','load_bearing'], // Load Bearing
     ['AT005_CAB3','12','raw:AT005_CAB3','guide'], // Guide
     ['AT005_CAB3','13','raw:AT005_CAB3','barrier'], // Barrier
     ['AT005_CAB3','14','raw:AT005_CAB3','restraining'], // Restraining
     ['AT005_CAB3','15','raw:AT005_CAB3','towing'], // Towing
     ['AT005_CAB3','19','raw:AT005_CAB3','cableway'], // Cableway
     ['AT005_CAB3','999','raw:AT005_CAB3','other'], // Other

     // ATC - Aqueduct Type
     // ['ATC','-999999',undefined,undefined], // noInformation
     ['ATC','3','raw:ATC','underground'], // Underground
     ['ATC','4','raw:ATC','underwater'], // Underwater
     ['ATC','5','raw:ATC','surface'], // Surface
     ['ATC','6','raw:ATC','qanat'], // Qanat

     // AXS - Aerodrome Surface Status
     // ['AXS','-999999',undefined,undefined], // noInformation
     ['AXS','1','raw:AXS','closed'], // Closed
     ['AXS','2','raw:AXS','open'], // Open
     ['AXS','3','raw:AXS','work_in_progress'], // Work in Progress
     ['AXS','4','raw:AXS','parked_or_disabled_aircraft'], // Parked or Disabled Aircraft

     // AZC - Man-made
     // ['AZC','-999999',undefined,undefined], // noInformation
     ['AZC','1000','raw:AZC','no'],
     ['AZC','1001','raw:AZC','yes'],

     // BA000_VDC - Water Line : Sounding Datum
     // ['BA000_VDC','-999999',undefined,undefined], // noInformation
     ['BA000_VDC','4','raw:BA000_VDC','indian_spring_low_water'], // Indian Spring Low Water
     ['BA000_VDC','7','raw:BA000_VDC','mean_high_water'], // Mean High Water
     ['BA000_VDC','9','raw:BA000_VDC','mean_high_water_springs'], // Mean High Water Springs
     ['BA000_VDC','10','raw:BA000_VDC','mean_higher_high_water'], // Mean Higher High Water
     ['BA000_VDC','11','raw:BA000_VDC','mean_low_water'], // Mean Low Water
     ['BA000_VDC','13','raw:BA000_VDC','mean_low_water_springs'], // Mean Low Water Springs
     ['BA000_VDC','14','raw:BA000_VDC','mean_lower_low_water'], // Mean Lower Low Water
     ['BA000_VDC','15','raw:BA000_VDC','mean_sea_level'], // Mean Sea Level
     ['BA000_VDC','19','raw:BA000_VDC','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
     ['BA000_VDC','20','raw:BA000_VDC','lowest_astronomical_tide'], // Lowest Astronomical Tide
     ['BA000_VDC','24','raw:BA000_VDC','mean_higher_high_water_springs'], // Mean Higher High Water Springs
     ['BA000_VDC','28','raw:BA000_VDC','highest_high_water'], // Highest High Water
     ['BA000_VDC','30','raw:BA000_VDC','indian_spring_high_water'], // Indian Spring High Water
     ['BA000_VDC','90','raw:BA000_VDC','lowest_low_water'], // Lowest Low Water
     ['BA000_VDC','999','raw:BA000_VDC','other'], // Other

     // BAC - Built-up Area Density Category
     // ['BAC','-999999',undefined,undefined], // noInformation
     ['BAC','1','raw:BAC','sparse'], // Sparse
     ['BAC','2','raw:BAC','dense'], // Dense
     ['BAC','3','raw:BAC','moderate'], // Moderate

     // BAT - Barrier Top Type
     // ['BAT','-999999',undefined,undefined], // noInformation
     ['BAT','1','raw:BAT','barbed_wire'], // Barbed Wire
     ['BAT','2','raw:BAT','chain-link'], // Chain-link
     ['BAT','3','raw:BAT','concertina_wire'], // Concertina Wire
     ['BAT','4','raw:BAT','electrified_wire'], // Electrified Wire
     ['BAT','5','raw:BAT','spiked'], // Spiked
     ['BAT','999','raw:BAT','other'], // Other

     // BCC - Bypass Condition
     // ['BCC','-999999',undefined,undefined], // noInformation
     ['BCC','1','raw:BCC','easy'], // Easy
     ['BCC','2','raw:BCC','difficult'], // Difficult
     ['BCC','3','raw:BCC','impossible'], // Impossible

     // BGT - Basin Gate Type
     // ['BGT','-999999',undefined,undefined], // noInformation
     ['BGT','1','raw:BGT','caisson'], // Caisson
     ['BGT','2','raw:BGT','lock_gate'], // Lock Gate
     ['BGT','3','raw:BGT','tide_lock'], // Tide Lock
     ['BGT','999','raw:BGT','other'], // Other

     // BH141_SLTA - Inland Waterbody Bank : Shoreline Type (first bank)
     // ['BH141_SLTA','-999999',undefined,undefined], // noInformation
     ['BH141_SLTA','6','raw:BH141_SLTA','mangrove'], // Mangrove
     ['BH141_SLTA','8','raw:BH141_SLTA','marshy'], // Marshy
     ['BH141_SLTA','10','raw:BH141_SLTA','stony'], // Stony
     ['BH141_SLTA','11','raw:BH141_SLTA','building_rubble'], // Building Rubble
     ['BH141_SLTA','12','raw:BH141_SLTA','erosion_rubble'], // Erosion Rubble
     ['BH141_SLTA','13','raw:BH141_SLTA','sandy'], // Sandy
     ['BH141_SLTA','14','raw:BH141_SLTA','shingly'], // Shingly
     ['BH141_SLTA','17','raw:BH141_SLTA','ice'], // Ice
     ['BH141_SLTA','18','raw:BH141_SLTA','mud'], // Mud
     ['BH141_SLTA','999','raw:BH141_SLTA','other'], // Other

     // BH141_SLTB - Inland Waterbody Bank : Shoreline Type (second bank)
     // ['BH141_SLTB','-999999',undefined,undefined], // noInformation
     ['BH141_SLTB','6','raw:BH141_SLTB','mangrove'], // Mangrove
     ['BH141_SLTB','8','raw:BH141_SLTB','marshy'], // Marshy
     ['BH141_SLTB','10','raw:BH141_SLTB','stony'], // Stony
     ['BH141_SLTB','11','raw:BH141_SLTB','building_rubble'], // Building Rubble
     ['BH141_SLTB','12','raw:BH141_SLTB','erosion_rubble'], // Erosion Rubble
     ['BH141_SLTB','13','raw:BH141_SLTB','sandy'], // Sandy
     ['BH141_SLTB','14','raw:BH141_SLTB','shingly'], // Shingly
     ['BH141_SLTB','17','raw:BH141_SLTB','ice'], // Ice
     ['BH141_SLTB','18','raw:BH141_SLTB','mud'], // Mud
     ['BH141_SLTB','999','raw:BH141_SLTB','other'], // Other

     // BMC - Bottom Material Type
     // ['BMC','-999999',undefined,undefined], // noInformation
     ['BMC','1','raw:BMC','clay_and_silt'], // Clay and Silt
     ['BMC','2','raw:BMC','silty_sands'], // Silty Sands
     ['BMC','3','raw:BMC','sand_and_gravel'], // Sand and Gravel
     ['BMC','4','raw:BMC','gravel_and_cobble'], // Gravel and Cobble
     ['BMC','5','raw:BMC','rocks_and_boulders'], // Rocks and Boulders
     ['BMC','6','raw:BMC','bedrock'], // Bedrock
     ['BMC','7','raw:BMC','paved'], // Paved
     ['BMC','8','raw:BMC','peat'], // Peat
     ['BMC','9','raw:BMC','sand_over_mud'], // Sand over Mud
     ['BMC','11','raw:BMC','coral'], // Coral
     ['BMC','14','raw:BMC','sand'], // Sand
     ['BMC','15','raw:BMC','concrete'], // Concrete
     ['BMC','16','raw:BMC','masonry'], // Masonry
     ['BMC','17','raw:BMC','reinforced_concrete'], // Reinforced Concrete
     ['BMC','18','raw:BMC','soil'], // Soil
     ['BMC','999','raw:BMC','other'], // Other

     // BMC2 - Bottom Material Type [2]
     // ['BMC2','-999999',undefined,undefined], // noInformation
     ['BMC2','1','raw:BMC2','clay_and_silt'], // Clay and Silt
     ['BMC2','2','raw:BMC2','silty_sands'], // Silty Sands
     ['BMC2','3','raw:BMC2','sand_and_gravel'], // Sand and Gravel
     ['BMC2','4','raw:BMC2','gravel_and_cobble'], // Gravel and Cobble
     ['BMC2','5','raw:BMC2','rocks_and_boulders'], // Rocks and Boulders
     ['BMC2','6','raw:BMC2','bedrock'], // Bedrock
     ['BMC2','7','raw:BMC2','paved'], // Paved
     ['BMC2','8','raw:BMC2','peat'], // Peat
     ['BMC2','9','raw:BMC2','sand_over_mud'], // Sand over Mud
     ['BMC2','11','raw:BMC2','coral'], // Coral
     ['BMC2','14','raw:BMC2','sand'], // Sand
     ['BMC2','15','raw:BMC2','concrete'], // Concrete
     ['BMC2','16','raw:BMC2','masonry'], // Masonry
     ['BMC2','17','raw:BMC2','reinforced_concrete'], // Reinforced Concrete
     ['BMC2','18','raw:BMC2','soil'], // Soil
     ['BMC2','999','raw:BMC2','other'], // Other

     // BMC3 - Bottom Material Type [3]
     // ['BMC3','-999999',undefined,undefined], // noInformation
     ['BMC3','1','raw:BMC3','clay_and_silt'], // Clay and Silt
     ['BMC3','2','raw:BMC3','silty_sands'], // Silty Sands
     ['BMC3','3','raw:BMC3','sand_and_gravel'], // Sand and Gravel
     ['BMC3','4','raw:BMC3','gravel_and_cobble'], // Gravel and Cobble
     ['BMC3','5','raw:BMC3','rocks_and_boulders'], // Rocks and Boulders
     ['BMC3','6','raw:BMC3','bedrock'], // Bedrock
     ['BMC3','7','raw:BMC3','paved'], // Paved
     ['BMC3','8','raw:BMC3','peat'], // Peat
     ['BMC3','9','raw:BMC3','sand_over_mud'], // Sand over Mud
     ['BMC3','11','raw:BMC3','coral'], // Coral
     ['BMC3','14','raw:BMC3','sand'], // Sand
     ['BMC3','15','raw:BMC3','concrete'], // Concrete
     ['BMC3','16','raw:BMC3','masonry'], // Masonry
     ['BMC3','17','raw:BMC3','reinforced_concrete'], // Reinforced Concrete
     ['BMC3','18','raw:BMC3','soil'], // Soil
     ['BMC3','999','raw:BMC3','other'], // Other

     // BOC - Bog Type
     // ['BOC','-999999',undefined,undefined], // noInformation
     ['BOC','1','raw:BOC','palsa_bog'], // Palsa Bog
     ['BOC','2','raw:BOC','string_bog'], // String Bog
     ['BOC','3','raw:BOC','atlantic_plateau_bog'], // Atlantic Plateau Bog
     ['BOC','4','raw:BOC','basin_bog'], // Basin Bog
     ['BOC','6','raw:BOC','blanket_bog'], // Blanket Bog
     ['BOC','7','raw:BOC','cranberry_bog'], // Cranberry Bog
     ['BOC','8','raw:BOC','domed_bog'], // Domed Bog
     ['BOC','9','raw:BOC','fen'], // Fen
     ['BOC','10','raw:BOC','flat_bog'], // Flat Bog
     ['BOC','11','raw:BOC','horizontal_fen'], // Horizontal Fen
     ['BOC','12','raw:BOC','lowland_polygon_bog'], // Lowland Polygon Bog
     ['BOC','13','raw:BOC','northern_ribbed_fen'], // Northern Ribbed Fen
     ['BOC','14','raw:BOC','peat_bog'], // Peat Bog
     ['BOC','15','raw:BOC','peat_plateau_bog'], // Peat Plateau Bog
     ['BOC','16','raw:BOC','polygonal_peat_plateau_bog'], // Polygonal Peat Plateau Bog
     ['BOC','17','raw:BOC','shore_fen'], // Shore Fen
     ['BOC','18','raw:BOC','slope_bog'], // Slope Bog
     ['BOC','19','raw:BOC','slope_fen'], // Slope Fen
     ['BOC','20','raw:BOC','veneer_bog'], // Veneer Bog
     ['BOC','999','raw:BOC','other'], // Other

     // BOT - Bridge Opening Type
     // ['BOT','-999999',undefined,undefined], // noInformation
     ['BOT','4','raw:BOT','bascule'], // Bascule
     ['BOT','10','raw:BOT','swing-bridge'], // Swing-bridge
     ['BOT','11','raw:BOT','lift-bridge'], // Lift-bridge
     ['BOT','12','raw:BOT','retractable'], // Retractable
     ['BOT','14','raw:BOT','submersible'], // Submersible
     ['BOT','15','raw:BOT','drawbridge'], // Drawbridge
     ['BOT','16','raw:BOT','opening'], // Opening
     ['BOT','17','raw:BOT','fixed'], // Fixed
     ['BOT','999','raw:BOT','other'], // Other

     // BSC - Bridge Structure Type
     // ['BSC','-999999',undefined,undefined], // noInformation
     ['BSC','1','raw:BSC','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC','2','raw:BSC','cantilever'], // Cantilever
     ['BSC','5','raw:BSC','floating'], // Floating
     ['BSC','6','raw:BSC','girder'], // Girder
     ['BSC','7','raw:BSC','tower_suspension'], // Tower Suspension
     ['BSC','8','raw:BSC','truss'], // Truss
     ['BSC','9','raw:BSC','suspension'], // Suspension
     ['BSC','12','raw:BSC','transporter'], // Transporter
     ['BSC','15','raw:BSC','slab'], // Slab
     ['BSC','16','raw:BSC','stringer_beam'], // Stringer Beam
     ['BSC','19','raw:BSC','bowstring-bridge'], // Bowstring-bridge
     ['BSC','22','raw:BSC','covered'], // Covered
     ['BSC','26','raw:BSC','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC','27','raw:BSC','cable_stayed'], // Cable Stayed
     ['BSC','31','raw:BSC','arch'], // Arch
     ['BSC','32','raw:BSC','trestle'], // Trestle
     ['BSC','999','raw:BSC','other'], // Other

     // BSC2 - Bridge Structure Type [2]
     // ['BSC2','-999999',undefined,undefined], // noInformation
     ['BSC2','1','raw:BSC2','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC2','2','raw:BSC2','cantilever'], // Cantilever
     ['BSC2','5','raw:BSC2','floating'], // Floating
     ['BSC2','6','raw:BSC2','girder'], // Girder
     ['BSC2','7','raw:BSC2','tower_suspension'], // Tower Suspension
     ['BSC2','8','raw:BSC2','truss'], // Truss
     ['BSC2','9','raw:BSC2','suspension'], // Suspension
     ['BSC2','12','raw:BSC2','transporter'], // Transporter
     ['BSC2','15','raw:BSC2','slab'], // Slab
     ['BSC2','16','raw:BSC2','stringer_beam'], // Stringer Beam
     ['BSC2','19','raw:BSC2','bowstring-bridge'], // Bowstring-bridge
     ['BSC2','22','raw:BSC2','covered'], // Covered
     ['BSC2','26','raw:BSC2','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC2','27','raw:BSC2','cable_stayed'], // Cable Stayed
     ['BSC2','31','raw:BSC2','arch'], // Arch
     ['BSC2','32','raw:BSC2','trestle'], // Trestle
     ['BSC2','999','raw:BSC2','other'], // Other

     // BSC3 - Bridge Structure Type [3]
     // ['BSC3','-999999',undefined,undefined], // noInformation
     ['BSC3','1','raw:BSC3','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC3','2','raw:BSC3','cantilever'], // Cantilever
     ['BSC3','5','raw:BSC3','floating'], // Floating
     ['BSC3','6','raw:BSC3','girder'], // Girder
     ['BSC3','7','raw:BSC3','tower_suspension'], // Tower Suspension
     ['BSC3','8','raw:BSC3','truss'], // Truss
     ['BSC3','9','raw:BSC3','suspension'], // Suspension
     ['BSC3','12','raw:BSC3','transporter'], // Transporter
     ['BSC3','15','raw:BSC3','slab'], // Slab
     ['BSC3','16','raw:BSC3','stringer_beam'], // Stringer Beam
     ['BSC3','19','raw:BSC3','bowstring-bridge'], // Bowstring-bridge
     ['BSC3','22','raw:BSC3','covered'], // Covered
     ['BSC3','26','raw:BSC3','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC3','27','raw:BSC3','cable_stayed'], // Cable Stayed
     ['BSC3','31','raw:BSC3','arch'], // Arch
     ['BSC3','32','raw:BSC3','trestle'], // Trestle
     ['BSC3','999','raw:BSC3','other'], // Other

     // BSM - Mobile Bridge Span
     // ['BSM','-999999',undefined,undefined], // noInformation
     ['BSM','1000','raw:BSM','no'],
     ['BSM','1001','raw:BSM','yes'],

     // BSU - Building Superstructure Type
     // ['BSU','-999999',undefined,undefined], // noInformation
     ['BSU','1','raw:BSU','chimney'], // Chimney
     ['BSU','2','raw:BSU','elevator_machine_room'], // Elevator Machine Room
     ['BSU','3','raw:BSU','hvac_equipment'], // HVAC Equipment
     ['BSU','4','raw:BSU','lightning_rod'], // Lightning Rod
     ['BSU','5','raw:BSU','minaret'], // Minaret
     ['BSU','6','raw:BSU','ornamental_element'], // Ornamental Element
     ['BSU','7','raw:BSU','parapet'], // Parapet
     ['BSU','8','raw:BSU','roof_deck'], // Roof Deck
     ['BSU','9','raw:BSU','roof_garden'], // Roof Garden
     ['BSU','10','raw:BSU','signage'], // Signage
     ['BSU','11','raw:BSU','skylight'], // Skylight
     ['BSU','13','raw:BSU','tower'], // Tower
     ['BSU','14','raw:BSU','cupola'], // Cupola
     ['BSU','15','raw:BSU','steeple'], // Steeple
     ['BSU','16','raw:BSU','turret'], // Turret
     ['BSU','17','raw:BSU','dome'], // Dome
     ['BSU','18','raw:BSU','solar_panels'], // Solar Panels
     ['BSU','999','raw:BSU','other'], // Other

     // CAA - Controlling Authority
     // ['CAA','-999999',undefined,undefined], // noInformation
     ['CAA','3','raw:CAA','private'], // Private
     ['CAA','4','raw:CAA','tribal'], // Tribal
     ['CAA','5','raw:CAA','military'], // Military
     ['CAA','6','raw:CAA','municipal'], // Municipal
     ['CAA','7','raw:CAA','joint_military_and_civilian'], // Joint Military and Civilian
     ['CAA','16','raw:CAA','civilian'], // Civilian
     ['CAA','17','raw:CAA','public'], // Public
     ['CAA','999','raw:CAA','other'], // Other

     // CAB - Cable Type
     // ['CAB','-999999',undefined,undefined], // noInformation
     ['CAB','2','raw:CAB','power_distribution_line'], // Power Distribution Line
     ['CAB','3','raw:CAB','telephone'], // Telephone
     ['CAB','4','raw:CAB','telegraph'], // Telegraph
     ['CAB','6','raw:CAB','power_transmission_line'], // Power Transmission Line
     ['CAB','7','raw:CAB','digital_communication_line'], // Digital Communication Line
     ['CAB','8','raw:CAB','communication_line'], // Communication Line
     ['CAB','9','raw:CAB','fibre-optic'], // Fibre-optic
     ['CAB','10','raw:CAB','tethering'], // Tethering
     ['CAB','11','raw:CAB','load_bearing'], // Load Bearing
     ['CAB','12','raw:CAB','guide'], // Guide
     ['CAB','13','raw:CAB','barrier'], // Barrier
     ['CAB','14','raw:CAB','restraining'], // Restraining
     ['CAB','15','raw:CAB','towing'], // Towing
     ['CAB','19','raw:CAB','cableway'], // Cableway
     ['CAB','999','raw:CAB','other'], // Other

     // CAB2 - Cable Type [2]
     // ['CAB2','-999999',undefined,undefined], // noInformation
     ['CAB2','2','raw:CAB2','power_distribution_line'], // Power Distribution Line
     ['CAB2','3','raw:CAB2','telephone'], // Telephone
     ['CAB2','4','raw:CAB2','telegraph'], // Telegraph
     ['CAB2','6','raw:CAB2','power_transmission_line'], // Power Transmission Line
     ['CAB2','7','raw:CAB2','digital_communication_line'], // Digital Communication Line
     ['CAB2','8','raw:CAB2','communication_line'], // Communication Line
     ['CAB2','9','raw:CAB2','fibre-optic'], // Fibre-optic
     ['CAB2','10','raw:CAB2','tethering'], // Tethering
     ['CAB2','11','raw:CAB2','load_bearing'], // Load Bearing
     ['CAB2','12','raw:CAB2','guide'], // Guide
     ['CAB2','13','raw:CAB2','barrier'], // Barrier
     ['CAB2','14','raw:CAB2','restraining'], // Restraining
     ['CAB2','15','raw:CAB2','towing'], // Towing
     ['CAB2','19','raw:CAB2','cableway'], // Cableway
     ['CAB2','999','raw:CAB2','other'], // Other

     // CAB3 - Cable Type [3]
     // ['CAB3','-999999',undefined,undefined], // noInformation
     ['CAB3','2','raw:CAB3','power_distribution_line'], // Power Distribution Line
     ['CAB3','3','raw:CAB3','telephone'], // Telephone
     ['CAB3','4','raw:CAB3','telegraph'], // Telegraph
     ['CAB3','6','raw:CAB3','power_transmission_line'], // Power Transmission Line
     ['CAB3','7','raw:CAB3','digital_communication_line'], // Digital Communication Line
     ['CAB3','8','raw:CAB3','communication_line'], // Communication Line
     ['CAB3','9','raw:CAB3','fibre-optic'], // Fibre-optic
     ['CAB3','10','raw:CAB3','tethering'], // Tethering
     ['CAB3','11','raw:CAB3','load_bearing'], // Load Bearing
     ['CAB3','12','raw:CAB3','guide'], // Guide
     ['CAB3','13','raw:CAB3','barrier'], // Barrier
     ['CAB3','14','raw:CAB3','restraining'], // Restraining
     ['CAB3','15','raw:CAB3','towing'], // Towing
     ['CAB3','19','raw:CAB3','cableway'], // Cableway
     ['CAB3','999','raw:CAB3','other'], // Other

     // CAM - Conservation Area Management Category
     // ['CAM','-999999',undefined,undefined], // noInformation
     ['CAM','1','raw:CAM','strict_nature_reserve'], // Strict Nature Reserve
     ['CAM','2','raw:CAM','wilderness_area'], // Wilderness Area
     ['CAM','3','raw:CAM','national_park'], // National Park
     ['CAM','4','raw:CAM','natural_monument'], // Natural Monument
     ['CAM','5','raw:CAM','habitat_and/or_species_management_area'], // Habitat and/or Species Management Area
     ['CAM','6','raw:CAM','habitat_management_area'], // Habitat Management Area
     ['CAM','7','raw:CAM','species_management_area'], // Species Management Area
     ['CAM','8','raw:CAM','breeding_ground'], // Breeding Ground
     ['CAM','9','raw:CAM','protected_landscape_or_seascape'], // Protected Landscape or Seascape
     ['CAM','10','raw:CAM','managed_resource_protected_area'], // Managed Resource Protected Area
     ['CAM','11','raw:CAM','managed_forest_protected_area'], // Managed Forest Protected Area
     ['CAM','999','raw:CAM','other'], // Other

     // CAT - Cableway Type
     // ['CAT','-999999',undefined,undefined], // noInformation
     ['CAT','2','raw:CAT','chair-lift'], // Chair-lift
     ['CAT','3','raw:CAT','ski_tow'], // Ski Tow
     ['CAT','5','raw:CAT','aerial_tramway'], // Aerial Tramway
     ['CAT','6','raw:CAT','gondola_lift'], // Gondola Lift
     ['CAT','7','raw:CAT','t-bar_lift'], // T-bar Lift
     ['CAT','8','raw:CAT','industrial_ropeway'], // Industrial Ropeway
     ['CAT','9','raw:CAT','material_tramway'], // Material Tramway
     ['CAT','999','raw:CAT','other'], // Other

     // CBP - Containment Berm Present
     // ['CBP','-999999',undefined,undefined], // noInformation
     ['CBP','1000','raw:CBP','no'],
     ['CBP','1001','raw:CBP','yes'],

     // CCT - Cover Closure Type
     // ['CCT','-999999',undefined,undefined], // noInformation
     ['CCT','1','raw:CCT','open'], // Open
     ['CCT','2','raw:CCT','partial'], // Partial
     ['CCT','3','raw:CCT','moveable'], // Moveable
     ['CCT','4','raw:CCT','complete'], // Complete

     // CDA - Covered Drain
     // ['CDA','-999999',undefined,undefined], // noInformation
     ['CDA','1000','raw:CDA','no'],
     ['CDA','1001','raw:CDA','yes'],

     // CET - Grading Type
     // ['CET','-999999',undefined,undefined], // noInformation
     ['CET','1','raw:CET','one_side'], // One Side
     ['CET','2','raw:CET','two_sides'], // Two Sides
     ['CET','3','raw:CET','many_sides'], // Many Sides
     ['CET','4','raw:CET','no_sides'], // No Sides
     ['CET','999','raw:CET','other'], // Other

     // CPS - Cell Partition Scheme
     ['CPS','fixed0r25','raw:CPS','fixed_cells,_0.25_arc_degree'], // Fixed Cells, 0.25 Arc Degree
     ['CPS','fixed0r5','raw:CPS','fixed_cells,_0.5_arc_degree'], // Fixed Cells, 0.5 Arc Degree
     ['CPS','fixed1r0','raw:CPS','fixed_cells,_1_arc_degree'], // Fixed Cells, 1 Arc Degree
     ['CPS','fixed5r0','raw:CPS','fixed_cells,_5_arc_degree'], // Fixed Cells, 5 Arc Degree
     ['CPS','noInformation','raw:CPS','noinformation'], // noInformation
     ['CPS','other','raw:CPS','other'], // Other
     ['CPS','variable','raw:CPS','variable_cells'], // Variable Cells

     // CRA - Crane Type
     // ['CRA','-999999',undefined,undefined], // noInformation
     ['CRA','2','raw:CRA','bridge_crane'], // Bridge Crane
     ['CRA','3','raw:CRA','rotating_crane'], // Rotating Crane
     ['CRA','7','raw:CRA','tower_crane'], // Tower Crane
     ['CRA','99','raw:CRA','container_crane'], // Container Crane
     ['CRA','999','raw:CRA','other'], // Other

     // CRM - Crane Mobility Type
     // ['CRM','-999999',undefined,undefined], // noInformation
     ['CRM','1','raw:CRM','fixed'], // Fixed
     ['CRM','2','raw:CRM','travelling'], // Travelling
     ['CRM','3','raw:CRM','floating'], // Floating
     ['CRM','4','raw:CRM','mobile'], // Mobile
     ['CRM','999','raw:CRM','other'], // Other

     // CST - Contained within Service Tunnel
     // ['CST','-999999',undefined,undefined], // noInformation
     ['CST','1000','raw:CST','no'],
     ['CST','1001','raw:CST','yes'],

     // CTC - Culvert Type
     // ['CTC','-999999',undefined,undefined], // noInformation
     ['CTC','1','raw:CTC','regular_with_soil'], // Regular with Soil
     ['CTC','2','raw:CTC','box_with_soil'], // Box with Soil
     ['CTC','3','raw:CTC','box_with_load'], // Box with Load
     ['CTC','999','raw:CTC','other'], // Other

     // CVT - Conveyor Type
     // ['CVT','-999999',undefined,undefined], // noInformation
     ['CVT','1','raw:CVT','not_visible'], // Not Visible
     ['CVT','2','raw:CVT','belt'], // Belt
     ['CVT','3','raw:CVT','rollers'], // Rollers
     ['CVT','4','raw:CVT','bucket'], // Bucket
     ['CVT','5','raw:CVT','screw_auger'], // Screw Auger
     ['CVT','6','raw:CVT','apron'], // Apron
     ['CVT','7','raw:CVT','flight'], // Flight
     ['CVT','999','raw:CVT','other'], // Other

     // CVT2 - Conveyor Type [2]
     // ['CVT2','-999999',undefined,undefined], // noInformation
     ['CVT2','1','raw:CVT2','not_visible'], // Not Visible
     ['CVT2','2','raw:CVT2','belt'], // Belt
     ['CVT2','3','raw:CVT2','rollers'], // Rollers
     ['CVT2','4','raw:CVT2','bucket'], // Bucket
     ['CVT2','5','raw:CVT2','screw_auger'], // Screw Auger
     ['CVT2','6','raw:CVT2','apron'], // Apron
     ['CVT2','7','raw:CVT2','flight'], // Flight
     ['CVT2','999','raw:CVT2','other'], // Other

     // CVT3 - Conveyor Type [3]
     // ['CVT3','-999999',undefined,undefined], // noInformation
     ['CVT3','1','raw:CVT3','not_visible'], // Not Visible
     ['CVT3','2','raw:CVT3','belt'], // Belt
     ['CVT3','3','raw:CVT3','rollers'], // Rollers
     ['CVT3','4','raw:CVT3','bucket'], // Bucket
     ['CVT3','5','raw:CVT3','screw_auger'], // Screw Auger
     ['CVT3','6','raw:CVT3','apron'], // Apron
     ['CVT3','7','raw:CVT3','flight'], // Flight
     ['CVT3','999','raw:CVT3','other'], // Other

     // CWT - Contained within Tunnel
     // ['CWT','-999999',undefined,undefined], // noInformation
     ['CWT','1000','raw:CWT','no'],
     ['CWT','1001','raw:CWT','yes'],

     // DFT - Dam Face Type
     // ['DFT','-999999',undefined,undefined], // noInformation
     ['DFT','1','raw:DFT','vertical'], // Vertical
     ['DFT','2','raw:DFT','slope'], // Slope

     // DFU - Dam Function
     // ['DFU','-999999',undefined,undefined], // noInformation
     ['DFU','1','raw:DFU','impoundment'], // Impoundment
     ['DFU','2','raw:DFU','navigation'], // Navigation
     ['DFU','3','raw:DFU','flood_control'], // Flood Control
     ['DFU','4','raw:DFU','hydroelectric_power_generation'], // Hydroelectric Power Generation
     ['DFU','5','raw:DFU','weir'], // Weir
     ['DFU','6','raw:DFU','irrigation'], // Irrigation
     ['DFU','7','raw:DFU','recreation'], // Recreation
     ['DFU','8','raw:DFU','water_supply'], // Water Supply
     ['DFU','999','raw:DFU','other'], // Other

     // DGC - Transportation Block Type
     // ['DGC','-999999',undefined,undefined], // noInformation
     ['DGC','1','raw:DGC','drop_gate'], // Drop Gate
     ['DGC','2','raw:DGC','rolling_block'], // Rolling Block
     ['DGC','999','raw:DGC','other'], // Other

     // DMD - Dammed
     // ['DMD','-999999',undefined,undefined], // noInformation
     ['DMD','1000','raw:DMD','no'],
     ['DMD','1001','raw:DMD','yes'],

     // EET - Engineered Earthwork Type
     // ['EET','-999999',undefined,undefined], // noInformation
     ['EET','1','raw:EET','battery'], // Battery
     ['EET','2','raw:EET','military_parapet'], // Military Parapet
     ['EET','3','raw:EET','military_trench'], // Military Trench
     ['EET','4','raw:EET','rampart'], // Rampart
     ['EET','5','raw:EET','redoubt'], // Redoubt
     ['EET','999','raw:EET','other'], // Other

     // ETS - Extraction Specification
     ['ETS','atc','raw:ETS','4aa-atc'], // 4AA-ATC
     ['ETS','combat','raw:ETS','2ad-combat'], // 2AD-Combat
     ['ETS','dfad1','raw:ETS','1ce-dfad-1'], // 1CE-DFAD-1
     ['ETS','dfad2','raw:ETS','1cg-dfad-2'], // 1CG-DFAD-2
     ['ETS','digitalFeg','raw:ETS','dfeg'], // DFEG
     ['ETS','dnc','raw:ETS','dnc'], // DNC
     ['ETS','dted1','raw:ETS','1cd-dted-1'], // 1CD-DTED-1
     ['ETS','dted2','raw:ETS','1cf-dted-2'], // 1CF-DTED-2
     ['ETS','dtop','raw:ETS','3kc/001-dtop'], // 3KC/001-DTOP
     ['ETS','ffd','raw:ETS','5ee-ffd'], // 5EE-FFD
     ['ETS','globalTdsEg','raw:ETS','gtds-eg'], // GTDS-EG
     ['ETS','hac1','raw:ETS','2aa/001-hac-1'], // 2AA/001-HAC-1
     ['ETS','hac2','raw:ETS','2aa/002-hac-2'], // 2AA/002-HAC-2
     ['ETS','hac3','raw:ETS','2aa/003-hac-3'], // 2AA/003-HAC-3
     ['ETS','hac4','raw:ETS','2aa/004-hac-4'], // 2AA/004-HAC-4
     ['ETS','hac5','raw:ETS','2aa/005-hac-5'], // 2AA/005-HAC-5
     ['ETS','hac6','raw:ETS','2aa/006-hac-6'], // 2AA/006-HAC-6
     ['ETS','hac7','raw:ETS','2aa/007-hac-7'], // 2AA/007-HAC-7
     ['ETS','hac8','raw:ETS','2aa/008-hac-8'], // 2AA/008-HAC-8
     ['ETS','hac9','raw:ETS','2aa/009-hac-9'], // 2AA/009-HAC-9
     ['ETS','jogAirGround','raw:ETS','1ae-jog-a/g'], // 1AE-JOG-A/G
     ['ETS','jogRadar','raw:ETS','4ac-jog-r'], // 4AC-JOG-R
     ['ETS','localTdsEg','raw:ETS','ltds-eg'], // LTDS-EG
     ['ETS','mgcpTrd','raw:ETS','mgcp_trd'], // MGCP TRD
     ['ETS','msd1','raw:ETS','msd1'], // MSD1
     ['ETS','msd2','raw:ETS','msd2'], // MSD2
     ['ETS','msd3','raw:ETS','msd3'], // MSD3
     ['ETS','msd4','raw:ETS','msd4'], // MSD4
     ['ETS','msd5','raw:ETS','msd5'], // MSD5
     ['ETS','noInformation','raw:ETS','noinformation'], // noInformation
     ['ETS','onc','raw:ETS','1ab-onc'], // 1AB-ONC
     ['ETS','other','raw:ETS','other'], // Other
     ['ETS','regionalTdsEg','raw:ETS','rtds-eg'], // RTDS-EG
     ['ETS','specUrbanTdsEg','raw:ETS','s-utds-eg'], // S-UTDS-EG
     ['ETS','tercomE','raw:ETS','4gf-tercom-e'], // 4GF-TERCOM-E
     ['ETS','tercomL','raw:ETS','4ge-tercom-l'], // 4GE-TERCOM-L
     ['ETS','tercomT','raw:ETS','4gg-tercom-t'], // 4GG-TERCOM-T
     ['ETS','tlm100','raw:ETS','3ag-tlm100'], // 3AG-TLM100
     ['ETS','tlm50','raw:ETS','3aa-tlm50'], // 3AA-TLM50
     ['ETS','tod0','raw:ETS','tod0'], // TOD0
     ['ETS','tod1','raw:ETS','tod1'], // TOD1
     ['ETS','tod2','raw:ETS','tod2'], // TOD2
     ['ETS','tod3','raw:ETS','tod3'], // TOD3
     ['ETS','tod4','raw:ETS','tod4'], // TOD4
     ['ETS','tpc','raw:ETS','1aa-tpc'], // 1AA-TPC
     ['ETS','uvmap','raw:ETS','3ku-uvmap'], // 3KU-UVMap
     ['ETS','vitd','raw:ETS','3ka-vitd'], // 3KA-VITD
     ['ETS','vmap0','raw:ETS','3kl-vmap-0'], // 3KL-VMap-0
     ['ETS','vmap1','raw:ETS','3km-vmap-1'], // 3KM-VMap-1
     ['ETS','vmap2','raw:ETS','3kh-vmap-2'], // 3KH-VMap-2

     // FAC - Solid Maritime Construction
     // ['FAC','-999999',undefined,undefined], // noInformation
     ['FAC','1000','raw:FAC','no'],
     ['FAC','1001','raw:FAC','yes'],

     // FCO - Feature Configuration
     // ['FCO','-999999',undefined,undefined], // noInformation
     ['FCO','5','raw:FCO','divided_same'], // Divided Same
     ['FCO','6','raw:FCO','divided_different'], // Divided Different
     ['FCO','7','raw:FCO','non-divided'], // Non-divided

     // FCS - Flood Control Structure Type
     // ['FCS','-999999',undefined,undefined], // noInformation
     ['FCS','1','raw:FCS','dyke_gate'], // Dyke Gate
     ['FCS','2','raw:FCS','emergency_gate'], // Emergency Gate
     ['FCS','3','raw:FCS','flood_gate'], // Flood Gate
     ['FCS','4','raw:FCS','fixed_barrage'], // Fixed Barrage
     ['FCS','5','raw:FCS','movable_barrage'], // Movable Barrage
     ['FCS','999','raw:FCS','other'], // Other

     // FER - Ferry Crossing Type
     // ['FER','-999999',undefined,undefined], // noInformation
     ['FER','1','raw:FER','cable'], // Cable
     ['FER','2','raw:FER','free-moving'], // Free-moving
     ['FER','3','raw:FER','ice'], // Ice
     ['FER','999','raw:FER','other'], // Other

     // FFN - Feature Function
     // ['FFN','-999999',undefined,undefined], // noInformation
     ['FFN','2','raw:FFN','agriculture'], // Agriculture
     ['FFN','3','raw:FFN','growing_of_crops'], // Growing of Crops
     ['FFN','9','raw:FFN','raising_of_animals'], // Raising of Animals
     ['FFN','14','raw:FFN','grazing'], // Grazing
     ['FFN','15','raw:FFN','mixed_farming'], // Mixed Farming
     ['FFN','19','raw:FFN','hunting'], // Hunting
     ['FFN','20','raw:FFN','forestry_and/or_logging'], // Forestry and/or Logging
     ['FFN','21','raw:FFN','silviculture'], // Silviculture
     ['FFN','27','raw:FFN','forest_warden'], // Forest Warden
     ['FFN','30','raw:FFN','fishing'], // Fishing
     ['FFN','35','raw:FFN','aquaculture'], // Aquaculture
     ['FFN','40','raw:FFN','mining_and_quarrying'], // Mining and Quarrying
     ['FFN','50','raw:FFN','solid_mineral_fuel_mining'], // Solid Mineral Fuel Mining
     ['FFN','60','raw:FFN','petroleum_and/or_gas_extraction'], // Petroleum and/or Gas Extraction
     ['FFN','70','raw:FFN','metal_ore_mining'], // Metal Ore Mining
     ['FFN','83','raw:FFN','chemical_mining'], // Chemical Mining
     ['FFN','84','raw:FFN','peat_extraction'], // Peat Extraction
     ['FFN','85','raw:FFN','salt_extraction'], // Salt Extraction
     ['FFN','87','raw:FFN','mineral_mining'], // Mineral Mining
     ['FFN','91','raw:FFN','gas_oil_separation'], // Gas Oil Separation
     ['FFN','95','raw:FFN','ore_dressing'], // Ore Dressing
     ['FFN','99','raw:FFN','manufacturing'], // Manufacturing
     ['FFN','100','raw:FFN','food_product_manufacture'], // Food Product Manufacture
     ['FFN','101','raw:FFN','food_processing'], // Food Processing
     ['FFN','102','raw:FFN','meat_processing'], // Meat Processing
     ['FFN','103','raw:FFN','seafood_processing'], // Seafood Processing
     ['FFN','104','raw:FFN','fruit_and/or_vegetable_processing'], // Fruit and/or Vegetable Processing
     ['FFN','105','raw:FFN','oil-mill'], // Oil-mill
     ['FFN','106','raw:FFN','dairying'], // Dairying
     ['FFN','107','raw:FFN','grain_milling'], // Grain Milling
     ['FFN','110','raw:FFN','baking'], // Baking
     ['FFN','111','raw:FFN','sugar_manufacture'], // Sugar Manufacture
     ['FFN','112','raw:FFN','sugar_milling'], // Sugar Milling
     ['FFN','113','raw:FFN','sugar_refining'], // Sugar Refining
     ['FFN','114','raw:FFN','confection_manufacture'], // Confection Manufacture
     ['FFN','115','raw:FFN','pasta_manufacture'], // Pasta Manufacture
     ['FFN','116','raw:FFN','prepared_meal_manufacture'], // Prepared Meal Manufacture
     ['FFN','118','raw:FFN','beverage_manufacture'], // Beverage Manufacture
     ['FFN','119','raw:FFN','animal_feed_manufacture'], // Animal Feed Manufacture
     ['FFN','120','raw:FFN','ice_manufacture'], // Ice Manufacture
     ['FFN','121','raw:FFN','spirit_distillery'], // Spirit Distillery
     ['FFN','122','raw:FFN','winery'], // Winery
     ['FFN','123','raw:FFN','brewing'], // Brewing
     ['FFN','124','raw:FFN','soft_drink_manufacture'], // Soft Drink Manufacture
     ['FFN','125','raw:FFN','tobacco_product_manufacture'], // Tobacco Product Manufacture
     ['FFN','129','raw:FFN','textile,_apparel_and_leather_manufacture'], // Textile, Apparel and Leather Manufacture
     ['FFN','130','raw:FFN','textile_manufacture'], // Textile Manufacture
     ['FFN','140','raw:FFN','apparel_manufacture'], // Apparel Manufacture
     ['FFN','150','raw:FFN','leather_product_manufacture'], // Leather Product Manufacture
     ['FFN','155','raw:FFN','footwear_manufacturing'], // Footwear Manufacturing
     ['FFN','160','raw:FFN','wood-based_manufacturing'], // Wood-based Manufacturing
     ['FFN','161','raw:FFN','sawmilling'], // Sawmilling
     ['FFN','165','raw:FFN','wooden_construction_product_manufacture'], // Wooden Construction Product Manufacture
     ['FFN','171','raw:FFN','paper-mill'], // Paper-mill
     ['FFN','181','raw:FFN','printing'], // Printing
     ['FFN','190','raw:FFN','petroleum_and_coal_products_manufacturing'], // Petroleum and Coal Products Manufacturing
     ['FFN','191','raw:FFN','coke_manufacture'], // Coke Manufacture
     ['FFN','192','raw:FFN','petroleum_refining'], // Petroleum Refining
     ['FFN','195','raw:FFN','chemical_manufacture'], // Chemical Manufacture
     ['FFN','210','raw:FFN','medicinal_product_manufacture'], // Medicinal Product Manufacture
     ['FFN','221','raw:FFN','rubber_product_manufacture'], // Rubber Product Manufacture
     ['FFN','225','raw:FFN','plastic_product_manufacture'], // Plastic Product Manufacture
     ['FFN','230','raw:FFN','nonmetallic_mineral_product_manufacturing'], // Nonmetallic Mineral Product Manufacturing
     ['FFN','231','raw:FFN','glass_product_manufacture'], // Glass Product Manufacture
     ['FFN','232','raw:FFN','refractory_product_manufacture'], // Refractory Product Manufacture
     ['FFN','233','raw:FFN','clay_product_manufacture'], // Clay Product Manufacture
     ['FFN','234','raw:FFN','ceramic_product_manufacture'], // Ceramic Product Manufacture
     ['FFN','235','raw:FFN','cement_mill'], // Cement Mill
     ['FFN','236','raw:FFN','cement_product_manufacture'], // Cement Product Manufacture
     ['FFN','237','raw:FFN','stone_product_manufacture'], // Stone Product Manufacture
     ['FFN','240','raw:FFN','primary_metal_manufacturing'], // Primary Metal Manufacturing
     ['FFN','241','raw:FFN','steel_mill'], // Steel Mill
     ['FFN','242','raw:FFN','metal_refining'], // Metal Refining
     ['FFN','243','raw:FFN','foundry'], // Foundry
     ['FFN','250','raw:FFN','metal_product_manufacturing'], // Metal Product Manufacturing
     ['FFN','251','raw:FFN','structural_metal_product_manufacture'], // Structural Metal Product Manufacture
     ['FFN','255','raw:FFN','munitions_manufacture'], // Munitions Manufacture
     ['FFN','257','raw:FFN','fabricated_metal_product_manufacture'], // Fabricated Metal Product Manufacture
     ['FFN','260','raw:FFN','electronic_equipment_manufacture'], // Electronic Equipment Manufacture
     ['FFN','270','raw:FFN','electrical_equipment_manufacture'], // Electrical Equipment Manufacture
     ['FFN','272','raw:FFN','propane_sale'], // Propane Sale
     ['FFN','280','raw:FFN','machinery_manufacture'], // Machinery Manufacture
     ['FFN','289','raw:FFN','transportation_equipment_manufacturing'], // Transportation Equipment Manufacturing
     ['FFN','290','raw:FFN','motor_vehicle_manufacture'], // Motor Vehicle Manufacture
     ['FFN','301','raw:FFN','ship_construction'], // Ship Construction
     ['FFN','304','raw:FFN','railway_vehicle_manufacture'], // Railway Vehicle Manufacture
     ['FFN','305','raw:FFN','aircraft_manufacture'], // Aircraft Manufacture
     ['FFN','306','raw:FFN','military_vehicle_manufacture'], // Military Vehicle Manufacture
     ['FFN','310','raw:FFN','furniture_manufacture'], // Furniture Manufacture
     ['FFN','320','raw:FFN','miscellaneous_manufacturing'], // Miscellaneous Manufacturing
     ['FFN','321','raw:FFN','jewellery_manufacture'], // Jewellery Manufacture
     ['FFN','322','raw:FFN','musical_instrument_manufacture'], // Musical Instrument Manufacture
     ['FFN','323','raw:FFN','sports_goods_manufacture'], // Sports Goods Manufacture
     ['FFN','324','raw:FFN','game_and/or_toy_manufacture'], // Game and/or Toy Manufacture
     ['FFN','325','raw:FFN','medical_and/or_dental_equipment_manufacture'], // Medical and/or Dental Equipment Manufacture
     ['FFN','330','raw:FFN','general_repair'], // General Repair
     ['FFN','331','raw:FFN','fabricated_metal_product_repair'], // Fabricated Metal Product Repair
     ['FFN','332','raw:FFN','electronic_equipment_repair'], // Electronic Equipment Repair
     ['FFN','333','raw:FFN','electrical_equipment_repair'], // Electrical Equipment Repair
     ['FFN','334','raw:FFN','machinery_repair'], // Machinery Repair
     ['FFN','340','raw:FFN','ship_repair'], // Ship Repair
     ['FFN','341','raw:FFN','aircraft_repair'], // Aircraft Repair
     ['FFN','342','raw:FFN','railway_vehicle_repair'], // Railway Vehicle Repair
     ['FFN','343','raw:FFN','motor_vehicle_repair'], // Motor Vehicle Repair
     ['FFN','350','raw:FFN','utilities'], // Utilities
     ['FFN','351','raw:FFN','power_generation'], // Power Generation
     ['FFN','352','raw:FFN','climate_control'], // Climate Control
     ['FFN','355','raw:FFN','cooling'], // Cooling
     ['FFN','356','raw:FFN','heating'], // Heating
     ['FFN','360','raw:FFN','water_supply'], // Water Supply
     ['FFN','361','raw:FFN','water_collection'], // Water Collection
     ['FFN','362','raw:FFN','water_treatment'], // Water Treatment
     ['FFN','363','raw:FFN','water_distribution'], // Water Distribution
     ['FFN','370','raw:FFN','sewerage'], // Sewerage
     ['FFN','372','raw:FFN','sewerage_screening'], // Sewerage Screening
     ['FFN','382','raw:FFN','restroom'], // Restroom
     ['FFN','383','raw:FFN','waste_treatment_and_disposal'], // Waste Treatment and Disposal
     ['FFN','385','raw:FFN','materials_recovery'], // Materials Recovery
     ['FFN','440','raw:FFN','commerce'], // Commerce
     ['FFN','459','raw:FFN','wholesale_merchant'], // Wholesale Merchant
     ['FFN','460','raw:FFN','retail_sale'], // Retail Sale
     ['FFN','464','raw:FFN','specialized_store'], // Specialized Store
     ['FFN','465','raw:FFN','non-specialized_store'], // Non-specialized Store
     ['FFN','466','raw:FFN','convenience_store'], // Convenience Store
     ['FFN','470','raw:FFN','petrol_sale'], // Petrol Sale
     ['FFN','473','raw:FFN','sales_yard'], // Sales Yard
     ['FFN','474','raw:FFN','precious_metal_merchant'], // Precious Metal Merchant
     ['FFN','475','raw:FFN','market'], // Market
     ['FFN','476','raw:FFN','grocery'], // Grocery
     ['FFN','477','raw:FFN','pharmacy'], // Pharmacy
     ['FFN','478','raw:FFN','pet-shop'], // Pet-shop
     ['FFN','480','raw:FFN','transport'], // Transport
     ['FFN','481','raw:FFN','terminal'], // Terminal
     ['FFN','482','raw:FFN','station'], // Station
     ['FFN','483','raw:FFN','stop'], // Stop
     ['FFN','484','raw:FFN','transfer_hub'], // Transfer Hub
     ['FFN','486','raw:FFN','signalling'], // Signalling
     ['FFN','487','raw:FFN','transport_system_maintenance'], // Transport System Maintenance
     ['FFN','488','raw:FFN','navigation'], // Navigation
     ['FFN','489','raw:FFN','transportation_hub'], // Transportation Hub
     ['FFN','490','raw:FFN','railway_transport'], // Railway Transport
     ['FFN','491','raw:FFN','railway_passenger_transport'], // Railway Passenger Transport
     ['FFN','494','raw:FFN','pedestrian_transport'], // Pedestrian Transport
     ['FFN','495','raw:FFN','road_transport'], // Road Transport
     ['FFN','496','raw:FFN','road_passenger_transport'], // Road Passenger Transport
     ['FFN','497','raw:FFN','road_freight_transport'], // Road Freight Transport
     ['FFN','500','raw:FFN','pipeline_transport'], // Pipeline Transport
     ['FFN','501','raw:FFN','pumping'], // Pumping
     ['FFN','505','raw:FFN','water_transport'], // Water Transport
     ['FFN','507','raw:FFN','inland_waters_transport'], // Inland Waters Transport
     ['FFN','508','raw:FFN','canal_transport'], // Canal Transport
     ['FFN','510','raw:FFN','port_control'], // Port Control
     ['FFN','511','raw:FFN','maritime_pilotage'], // Maritime Pilotage
     ['FFN','512','raw:FFN','pilot_station'], // Pilot Station
     ['FFN','513','raw:FFN','harbour_control'], // Harbour Control
     ['FFN','520','raw:FFN','air_transport'], // Air Transport
     ['FFN','525','raw:FFN','air_traffic_control'], // Air Traffic Control
     ['FFN','529','raw:FFN','transportation_support'], // Transportation Support
     ['FFN','530','raw:FFN','warehousing_and_storage'], // Warehousing and Storage
     ['FFN','535','raw:FFN','motor_vehicle_parking'], // Motor Vehicle Parking
     ['FFN','536','raw:FFN','cargo_handling'], // Cargo Handling
     ['FFN','537','raw:FFN','customs_checkpoint'], // Customs Checkpoint
     ['FFN','538','raw:FFN','inspection_station'], // Inspection Station
     ['FFN','539','raw:FFN','inspection'], // Inspection
     ['FFN','540','raw:FFN','postal_activities'], // Postal Activities
     ['FFN','541','raw:FFN','mail_and_package_transport'], // Mail and Package Transport
     ['FFN','545','raw:FFN','courier_activities'], // Courier Activities
     ['FFN','548','raw:FFN','short-term_accommodation'], // Short-term Accommodation
     ['FFN','549','raw:FFN','long-term_accommodation'], // Long-term Accommodation
     ['FFN','550','raw:FFN','accommodation'], // Accommodation
     ['FFN','551','raw:FFN','hotel'], // Hotel
     ['FFN','552','raw:FFN','resort'], // Resort
     ['FFN','553','raw:FFN','motel'], // Motel
     ['FFN','554','raw:FFN','guest-house'], // Guest-house
     ['FFN','555','raw:FFN','hostel'], // Hostel
     ['FFN','556','raw:FFN','dormitory'], // Dormitory
     ['FFN','557','raw:FFN','vacation_cottage'], // Vacation Cottage
     ['FFN','558','raw:FFN','dependents_housing'], // Dependents Housing
     ['FFN','559','raw:FFN','communal_bath'], // Communal Bath
     ['FFN','563','raw:FFN','residence'], // Residence
     ['FFN','570','raw:FFN','food_service'], // Food Service
     ['FFN','572','raw:FFN','restaurant'], // Restaurant
     ['FFN','573','raw:FFN','bar'], // Bar
     ['FFN','574','raw:FFN','dining_hall'], // Dining Hall
     ['FFN','578','raw:FFN','banquet_hall'], // Banquet Hall
     ['FFN','579','raw:FFN','convention_centre'], // Convention Centre
     ['FFN','580','raw:FFN','publishing_and_broadcasting'], // Publishing and Broadcasting
     ['FFN','582','raw:FFN','print_publishing'], // Print Publishing
     ['FFN','594','raw:FFN','cinema'], // Cinema
     ['FFN','601','raw:FFN','radio_broadcasting'], // Radio Broadcasting
     ['FFN','604','raw:FFN','television_broadcasting'], // Television Broadcasting
     ['FFN','610','raw:FFN','telecommunications'], // Telecommunications
     ['FFN','612','raw:FFN','retail_telecommunications'], // Retail Telecommunications
     ['FFN','614','raw:FFN','wired_telecommunications'], // Wired Telecommunications
     ['FFN','615','raw:FFN','main_telephone_exchange'], // Main Telephone Exchange
     ['FFN','616','raw:FFN','branch_telephone_exchange'], // Branch Telephone Exchange
     ['FFN','617','raw:FFN','wired_repeater'], // Wired Repeater
     ['FFN','620','raw:FFN','wireless_telecommunications'], // Wireless Telecommunications
     ['FFN','621','raw:FFN','mobile_phone_service'], // Mobile Phone Service
     ['FFN','622','raw:FFN','wireless_repeater'], // Wireless Repeater
     ['FFN','625','raw:FFN','satellite_telecommunications'], // Satellite Telecommunications
     ['FFN','626','raw:FFN','satellite_ground_control'], // Satellite Ground Control
     ['FFN','632','raw:FFN','information_service'], // Information Service
     ['FFN','633','raw:FFN','public_records'], // Public Records
     ['FFN','640','raw:FFN','financial_services'], // Financial Services
     ['FFN','642','raw:FFN','central_banking'], // Central Banking
     ['FFN','643','raw:FFN','retail_banking'], // Retail Banking
     ['FFN','651','raw:FFN','insurance'], // Insurance
     ['FFN','662','raw:FFN','financial_market_administration'], // Financial Market Administration
     ['FFN','663','raw:FFN','security_brokerage'], // Security Brokerage
     ['FFN','671','raw:FFN','fund_management'], // Fund Management
     ['FFN','680','raw:FFN','real_estate_activities'], // Real Estate Activities
     ['FFN','681','raw:FFN','professional,_scientific_and_technical'], // Professional, Scientific and Technical
     ['FFN','691','raw:FFN','legal_activities'], // Legal Activities
     ['FFN','696','raw:FFN','accounting'], // Accounting
     ['FFN','701','raw:FFN','head_office'], // Head Office
     ['FFN','706','raw:FFN','business_management'], // Business Management
     ['FFN','711','raw:FFN','architecture_consulting'], // Architecture Consulting
     ['FFN','714','raw:FFN','engineering_design'], // Engineering Design
     ['FFN','717','raw:FFN','surveying'], // Surveying
     ['FFN','720','raw:FFN','scientific_research_and_development'], // Scientific Research and Development
     ['FFN','721','raw:FFN','observation_station'], // Observation Station
     ['FFN','722','raw:FFN','weather_station'], // Weather Station
     ['FFN','725','raw:FFN','nuclear_research_centre'], // Nuclear Research Centre
     ['FFN','730','raw:FFN','wind_tunnel'], // Wind Tunnel
     ['FFN','741','raw:FFN','advertising'], // Advertising
     ['FFN','752','raw:FFN','photography'], // Photography
     ['FFN','757','raw:FFN','veterinary'], // Veterinary
     ['FFN','760','raw:FFN','business_and_personal_support_services'], // Business and Personal Support Services
     ['FFN','761','raw:FFN','motor_vehicle_rental'], // Motor Vehicle Rental
     ['FFN','770','raw:FFN','employment_agency'], // Employment Agency
     ['FFN','775','raw:FFN','travel_agency'], // Travel Agency
     ['FFN','780','raw:FFN','security_enforcement'], // Security Enforcement
     ['FFN','781','raw:FFN','guard'], // Guard
     ['FFN','791','raw:FFN','custodial_service'], // Custodial Service
     ['FFN','795','raw:FFN','landscaping_service'], // Landscaping Service
     ['FFN','801','raw:FFN','office_administration'], // Office Administration
     ['FFN','807','raw:FFN','call_centre'], // Call Centre
     ['FFN','808','raw:FFN','public_administration'], // Public Administration
     ['FFN','809','raw:FFN','headquarters'], // Headquarters
     ['FFN','810','raw:FFN','administration'], // Administration
     ['FFN','811','raw:FFN','government'], // Government
     ['FFN','812','raw:FFN','local_government'], // Local Government
     ['FFN','813','raw:FFN','subnational_government'], // Subnational Government
     ['FFN','814','raw:FFN','national_government'], // National Government
     ['FFN','815','raw:FFN','palace'], // Palace
     ['FFN','817','raw:FFN','capitol'], // Capitol
     ['FFN','818','raw:FFN','executive_activities'], // Executive Activities
     ['FFN','819','raw:FFN','legislative_activities'], // Legislative Activities
     ['FFN','821','raw:FFN','polling_station'], // Polling Station
     ['FFN','822','raw:FFN','civil_activities'], // Civil Activities
     ['FFN','825','raw:FFN','diplomacy'], // Diplomacy
     ['FFN','826','raw:FFN','diplomatic_mission'], // Diplomatic Mission
     ['FFN','827','raw:FFN','embassy'], // Embassy
     ['FFN','828','raw:FFN','consul'], // Consul
     ['FFN','829','raw:FFN','maritime_defense'], // Maritime Defense
     ['FFN','830','raw:FFN','public_order,_safety_and_security_services'], // Public Order, Safety and Security Services
     ['FFN','831','raw:FFN','public_order'], // Public Order
     ['FFN','832','raw:FFN','safety'], // Safety
     ['FFN','833','raw:FFN','security_services'], // Security Services
     ['FFN','835','raw:FFN','defence_activities'], // Defence Activities
     ['FFN','836','raw:FFN','armory'], // Armory
     ['FFN','837','raw:FFN','military_reserve_activities'], // Military Reserve Activities
     ['FFN','838','raw:FFN','military_recruitment'], // Military Recruitment
     ['FFN','839','raw:FFN','cbrne_civilian_support'], // CBRNE Civilian Support
     ['FFN','840','raw:FFN','judicial_activities'], // Judicial Activities
     ['FFN','841','raw:FFN','law_enforcement'], // Law Enforcement
     ['FFN','842','raw:FFN','immigration_control'], // Immigration Control
     ['FFN','843','raw:FFN','imprisonment'], // Imprisonment
     ['FFN','844','raw:FFN','juvenile_corrections'], // Juvenile Corrections
     ['FFN','845','raw:FFN','firefighting'], // Firefighting
     ['FFN','846','raw:FFN','rescue_and_paramedical_services'], // Rescue and Paramedical Services
     ['FFN','847','raw:FFN','emergency_operations'], // Emergency Operations
     ['FFN','848','raw:FFN','civil_intelligence'], // Civil Intelligence
     ['FFN','850','raw:FFN','education'], // Education
     ['FFN','851','raw:FFN','primary_education'], // Primary Education
     ['FFN','852','raw:FFN','secondary_education'], // Secondary Education
     ['FFN','855','raw:FFN','higher_education'], // Higher Education
     ['FFN','857','raw:FFN','vocational_education'], // Vocational Education
     ['FFN','860','raw:FFN','human_health_activities'], // Human Health Activities
     ['FFN','861','raw:FFN','in-patient_care'], // In-patient Care
     ['FFN','862','raw:FFN','out-patient_care'], // Out-patient Care
     ['FFN','863','raw:FFN','urgent_medical_care'], // Urgent Medical Care
     ['FFN','864','raw:FFN','human_tissue_repository'], // Human Tissue Repository
     ['FFN','865','raw:FFN','public_health_activities'], // Public Health Activities
     ['FFN','866','raw:FFN','leprosy_care'], // Leprosy Care
     ['FFN','871','raw:FFN','intermediate_care'], // Intermediate Care
     ['FFN','873','raw:FFN','psychiatric_in-patient_care'], // Psychiatric In-patient Care
     ['FFN','875','raw:FFN','residential_care'], // Residential Care
     ['FFN','881','raw:FFN','emergency_shelter'], // Emergency Shelter
     ['FFN','882','raw:FFN','homeless_shelter'], // Homeless Shelter
     ['FFN','883','raw:FFN','refugee_shelter'], // Refugee Shelter
     ['FFN','884','raw:FFN','emergency_youth_shelter'], // Emergency Youth Shelter
     ['FFN','885','raw:FFN','day_care'], // Day Care
     ['FFN','887','raw:FFN','social_work'], // Social Work
     ['FFN','888','raw:FFN','emergency_relief_services'], // Emergency Relief Services
     ['FFN','890','raw:FFN','cultural,_arts_and_entertainment'], // Cultural, Arts and Entertainment
     ['FFN','891','raw:FFN','theatre'], // Theatre
     ['FFN','892','raw:FFN','auditorium'], // Auditorium
     ['FFN','893','raw:FFN','community_centre'], // Community Centre
     ['FFN','894','raw:FFN','opera_house'], // Opera House
     ['FFN','895','raw:FFN','night_club'], // Night Club
     ['FFN','900','raw:FFN','sports,_amusement_and_recreation'], // Sports, Amusement and Recreation
     ['FFN','902','raw:FFN','library'], // Library
     ['FFN','905','raw:FFN','museum'], // Museum
     ['FFN','906','raw:FFN','aquarium'], // Aquarium
     ['FFN','907','raw:FFN','botanical_and/or_zoological_reserve_activities'], // Botanical and/or Zoological Reserve Activities
     ['FFN','909','raw:FFN','gambling'], // Gambling
     ['FFN','912','raw:FFN','sports_centre'], // Sports Centre
     ['FFN','913','raw:FFN','fitness_centre'], // Fitness Centre
     ['FFN','914','raw:FFN','shooting_range'], // Shooting Range
     ['FFN','915','raw:FFN','water_park'], // Water Park
     ['FFN','919','raw:FFN','animal_boarding'], // Animal Boarding
     ['FFN','921','raw:FFN','recreation'], // Recreation
     ['FFN','922','raw:FFN','amusement'], // Amusement
     ['FFN','923','raw:FFN','hobbies_and/or_leisure_activities'], // Hobbies and/or Leisure Activities
     ['FFN','930','raw:FFN','religious_activities'], // Religious Activities
     ['FFN','931','raw:FFN','place_of_worship'], // Place of Worship
     ['FFN','932','raw:FFN','islamic_prayer_hall'], // Islamic Prayer Hall
     ['FFN','950','raw:FFN','membership_organization'], // Membership Organization
     ['FFN','954','raw:FFN','club'], // Club
     ['FFN','955','raw:FFN','yacht-club'], // Yacht-club
     ['FFN','961','raw:FFN','laundry'], // Laundry
     ['FFN','962','raw:FFN','beauty_treatment'], // Beauty Treatment
     ['FFN','963','raw:FFN','funeral_services'], // Funeral Services
     ['FFN','964','raw:FFN','cremation'], // Cremation
     ['FFN','965','raw:FFN','mortuary_services'], // Mortuary Services
     ['FFN','966','raw:FFN','adult_entertainment'], // Adult Entertainment
     ['FFN','967','raw:FFN','storage_of_human_remains'], // Storage of Human Remains
     ['FFN','970','raw:FFN','meeting_place'], // Meeting Place
     ['FFN','980','raw:FFN','death_care_services'], // Death Care Services
     ['FFN','999','raw:FFN','other'], // Other

     // FFN2 - Feature Function [2]
     // ['FFN2','-999999',undefined,undefined], // noInformation
     ['FFN2','2','raw:FFN2','agriculture'], // Agriculture
     ['FFN2','3','raw:FFN2','growing_of_crops'], // Growing of Crops
     ['FFN2','9','raw:FFN2','raising_of_animals'], // Raising of Animals
     ['FFN2','14','raw:FFN2','grazing'], // Grazing
     ['FFN2','15','raw:FFN2','mixed_farming'], // Mixed Farming
     ['FFN2','19','raw:FFN2','hunting'], // Hunting
     ['FFN2','20','raw:FFN2','forestry_and/or_logging'], // Forestry and/or Logging
     ['FFN2','21','raw:FFN2','silviculture'], // Silviculture
     ['FFN2','27','raw:FFN2','forest_warden'], // Forest Warden
     ['FFN2','30','raw:FFN2','fishing'], // Fishing
     ['FFN2','35','raw:FFN2','aquaculture'], // Aquaculture
     ['FFN2','40','raw:FFN2','mining_and_quarrying'], // Mining and Quarrying
     ['FFN2','50','raw:FFN2','solid_mineral_fuel_mining'], // Solid Mineral Fuel Mining
     ['FFN2','60','raw:FFN2','petroleum_and/or_gas_extraction'], // Petroleum and/or Gas Extraction
     ['FFN2','70','raw:FFN2','metal_ore_mining'], // Metal Ore Mining
     ['FFN2','83','raw:FFN2','chemical_mining'], // Chemical Mining
     ['FFN2','84','raw:FFN2','peat_extraction'], // Peat Extraction
     ['FFN2','85','raw:FFN2','salt_extraction'], // Salt Extraction
     ['FFN2','87','raw:FFN2','mineral_mining'], // Mineral Mining
     ['FFN2','91','raw:FFN2','gas_oil_separation'], // Gas Oil Separation
     ['FFN2','95','raw:FFN2','ore_dressing'], // Ore Dressing
     ['FFN2','99','raw:FFN2','manufacturing'], // Manufacturing
     ['FFN2','100','raw:FFN2','food_product_manufacture'], // Food Product Manufacture
     ['FFN2','101','raw:FFN2','food_processing'], // Food Processing
     ['FFN2','102','raw:FFN2','meat_processing'], // Meat Processing
     ['FFN2','103','raw:FFN2','seafood_processing'], // Seafood Processing
     ['FFN2','104','raw:FFN2','fruit_and/or_vegetable_processing'], // Fruit and/or Vegetable Processing
     ['FFN2','105','raw:FFN2','oil-mill'], // Oil-mill
     ['FFN2','106','raw:FFN2','dairying'], // Dairying
     ['FFN2','107','raw:FFN2','grain_milling'], // Grain Milling
     ['FFN2','110','raw:FFN2','baking'], // Baking
     ['FFN2','111','raw:FFN2','sugar_manufacture'], // Sugar Manufacture
     ['FFN2','112','raw:FFN2','sugar_milling'], // Sugar Milling
     ['FFN2','113','raw:FFN2','sugar_refining'], // Sugar Refining
     ['FFN2','114','raw:FFN2','confection_manufacture'], // Confection Manufacture
     ['FFN2','115','raw:FFN2','pasta_manufacture'], // Pasta Manufacture
     ['FFN2','116','raw:FFN2','prepared_meal_manufacture'], // Prepared Meal Manufacture
     ['FFN2','118','raw:FFN2','beverage_manufacture'], // Beverage Manufacture
     ['FFN2','119','raw:FFN2','animal_feed_manufacture'], // Animal Feed Manufacture
     ['FFN2','120','raw:FFN2','ice_manufacture'], // Ice Manufacture
     ['FFN2','121','raw:FFN2','spirit_distillery'], // Spirit Distillery
     ['FFN2','122','raw:FFN2','winery'], // Winery
     ['FFN2','123','raw:FFN2','brewing'], // Brewing
     ['FFN2','124','raw:FFN2','soft_drink_manufacture'], // Soft Drink Manufacture
     ['FFN2','125','raw:FFN2','tobacco_product_manufacture'], // Tobacco Product Manufacture
     ['FFN2','129','raw:FFN2','textile,_apparel_and_leather_manufacture'], // Textile, Apparel and Leather Manufacture
     ['FFN2','130','raw:FFN2','textile_manufacture'], // Textile Manufacture
     ['FFN2','140','raw:FFN2','apparel_manufacture'], // Apparel Manufacture
     ['FFN2','150','raw:FFN2','leather_product_manufacture'], // Leather Product Manufacture
     ['FFN2','155','raw:FFN2','footwear_manufacturing'], // Footwear Manufacturing
     ['FFN2','160','raw:FFN2','wood-based_manufacturing'], // Wood-based Manufacturing
     ['FFN2','161','raw:FFN2','sawmilling'], // Sawmilling
     ['FFN2','165','raw:FFN2','wooden_construction_product_manufacture'], // Wooden Construction Product Manufacture
     ['FFN2','171','raw:FFN2','paper-mill'], // Paper-mill
     ['FFN2','181','raw:FFN2','printing'], // Printing
     ['FFN2','190','raw:FFN2','petroleum_and_coal_products_manufacturing'], // Petroleum and Coal Products Manufacturing
     ['FFN2','191','raw:FFN2','coke_manufacture'], // Coke Manufacture
     ['FFN2','192','raw:FFN2','petroleum_refining'], // Petroleum Refining
     ['FFN2','195','raw:FFN2','chemical_manufacture'], // Chemical Manufacture
     ['FFN2','210','raw:FFN2','medicinal_product_manufacture'], // Medicinal Product Manufacture
     ['FFN2','221','raw:FFN2','rubber_product_manufacture'], // Rubber Product Manufacture
     ['FFN2','225','raw:FFN2','plastic_product_manufacture'], // Plastic Product Manufacture
     ['FFN2','230','raw:FFN2','nonmetallic_mineral_product_manufacturing'], // Nonmetallic Mineral Product Manufacturing
     ['FFN2','231','raw:FFN2','glass_product_manufacture'], // Glass Product Manufacture
     ['FFN2','232','raw:FFN2','refractory_product_manufacture'], // Refractory Product Manufacture
     ['FFN2','233','raw:FFN2','clay_product_manufacture'], // Clay Product Manufacture
     ['FFN2','234','raw:FFN2','ceramic_product_manufacture'], // Ceramic Product Manufacture
     ['FFN2','235','raw:FFN2','cement_mill'], // Cement Mill
     ['FFN2','236','raw:FFN2','cement_product_manufacture'], // Cement Product Manufacture
     ['FFN2','237','raw:FFN2','stone_product_manufacture'], // Stone Product Manufacture
     ['FFN2','240','raw:FFN2','primary_metal_manufacturing'], // Primary Metal Manufacturing
     ['FFN2','241','raw:FFN2','steel_mill'], // Steel Mill
     ['FFN2','242','raw:FFN2','metal_refining'], // Metal Refining
     ['FFN2','243','raw:FFN2','foundry'], // Foundry
     ['FFN2','250','raw:FFN2','metal_product_manufacturing'], // Metal Product Manufacturing
     ['FFN2','251','raw:FFN2','structural_metal_product_manufacture'], // Structural Metal Product Manufacture
     ['FFN2','255','raw:FFN2','munitions_manufacture'], // Munitions Manufacture
     ['FFN2','257','raw:FFN2','fabricated_metal_product_manufacture'], // Fabricated Metal Product Manufacture
     ['FFN2','260','raw:FFN2','electronic_equipment_manufacture'], // Electronic Equipment Manufacture
     ['FFN2','270','raw:FFN2','electrical_equipment_manufacture'], // Electrical Equipment Manufacture
     ['FFN2','272','raw:FFN2','propane_sale'], // Propane Sale
     ['FFN2','280','raw:FFN2','machinery_manufacture'], // Machinery Manufacture
     ['FFN2','289','raw:FFN2','transportation_equipment_manufacturing'], // Transportation Equipment Manufacturing
     ['FFN2','290','raw:FFN2','motor_vehicle_manufacture'], // Motor Vehicle Manufacture
     ['FFN2','301','raw:FFN2','ship_construction'], // Ship Construction
     ['FFN2','304','raw:FFN2','railway_vehicle_manufacture'], // Railway Vehicle Manufacture
     ['FFN2','305','raw:FFN2','aircraft_manufacture'], // Aircraft Manufacture
     ['FFN2','306','raw:FFN2','military_vehicle_manufacture'], // Military Vehicle Manufacture
     ['FFN2','310','raw:FFN2','furniture_manufacture'], // Furniture Manufacture
     ['FFN2','320','raw:FFN2','miscellaneous_manufacturing'], // Miscellaneous Manufacturing
     ['FFN2','321','raw:FFN2','jewellery_manufacture'], // Jewellery Manufacture
     ['FFN2','322','raw:FFN2','musical_instrument_manufacture'], // Musical Instrument Manufacture
     ['FFN2','323','raw:FFN2','sports_goods_manufacture'], // Sports Goods Manufacture
     ['FFN2','324','raw:FFN2','game_and/or_toy_manufacture'], // Game and/or Toy Manufacture
     ['FFN2','325','raw:FFN2','medical_and/or_dental_equipment_manufacture'], // Medical and/or Dental Equipment Manufacture
     ['FFN2','330','raw:FFN2','general_repair'], // General Repair
     ['FFN2','331','raw:FFN2','fabricated_metal_product_repair'], // Fabricated Metal Product Repair
     ['FFN2','332','raw:FFN2','electronic_equipment_repair'], // Electronic Equipment Repair
     ['FFN2','333','raw:FFN2','electrical_equipment_repair'], // Electrical Equipment Repair
     ['FFN2','334','raw:FFN2','machinery_repair'], // Machinery Repair
     ['FFN2','340','raw:FFN2','ship_repair'], // Ship Repair
     ['FFN2','341','raw:FFN2','aircraft_repair'], // Aircraft Repair
     ['FFN2','342','raw:FFN2','railway_vehicle_repair'], // Railway Vehicle Repair
     ['FFN2','343','raw:FFN2','motor_vehicle_repair'], // Motor Vehicle Repair
     ['FFN2','350','raw:FFN2','utilities'], // Utilities
     ['FFN2','351','raw:FFN2','power_generation'], // Power Generation
     ['FFN2','352','raw:FFN2','climate_control'], // Climate Control
     ['FFN2','355','raw:FFN2','cooling'], // Cooling
     ['FFN2','356','raw:FFN2','heating'], // Heating
     ['FFN2','360','raw:FFN2','water_supply'], // Water Supply
     ['FFN2','361','raw:FFN2','water_collection'], // Water Collection
     ['FFN2','362','raw:FFN2','water_treatment'], // Water Treatment
     ['FFN2','363','raw:FFN2','water_distribution'], // Water Distribution
     ['FFN2','370','raw:FFN2','sewerage'], // Sewerage
     ['FFN2','372','raw:FFN2','sewerage_screening'], // Sewerage Screening
     ['FFN2','382','raw:FFN2','restroom'], // Restroom
     ['FFN2','383','raw:FFN2','waste_treatment_and_disposal'], // Waste Treatment and Disposal
     ['FFN2','385','raw:FFN2','materials_recovery'], // Materials Recovery
     ['FFN2','440','raw:FFN2','commerce'], // Commerce
     ['FFN2','459','raw:FFN2','wholesale_merchant'], // Wholesale Merchant
     ['FFN2','460','raw:FFN2','retail_sale'], // Retail Sale
     ['FFN2','464','raw:FFN2','specialized_store'], // Specialized Store
     ['FFN2','465','raw:FFN2','non-specialized_store'], // Non-specialized Store
     ['FFN2','466','raw:FFN2','convenience_store'], // Convenience Store
     ['FFN2','470','raw:FFN2','petrol_sale'], // Petrol Sale
     ['FFN2','473','raw:FFN2','sales_yard'], // Sales Yard
     ['FFN2','474','raw:FFN2','precious_metal_merchant'], // Precious Metal Merchant
     ['FFN2','475','raw:FFN2','market'], // Market
     ['FFN2','476','raw:FFN2','grocery'], // Grocery
     ['FFN2','477','raw:FFN2','pharmacy'], // Pharmacy
     ['FFN2','478','raw:FFN2','pet-shop'], // Pet-shop
     ['FFN2','480','raw:FFN2','transport'], // Transport
     ['FFN2','481','raw:FFN2','terminal'], // Terminal
     ['FFN2','482','raw:FFN2','station'], // Station
     ['FFN2','483','raw:FFN2','stop'], // Stop
     ['FFN2','484','raw:FFN2','transfer_hub'], // Transfer Hub
     ['FFN2','486','raw:FFN2','signalling'], // Signalling
     ['FFN2','487','raw:FFN2','transport_system_maintenance'], // Transport System Maintenance
     ['FFN2','488','raw:FFN2','navigation'], // Navigation
     ['FFN2','489','raw:FFN2','transportation_hub'], // Transportation Hub
     ['FFN2','490','raw:FFN2','railway_transport'], // Railway Transport
     ['FFN2','491','raw:FFN2','railway_passenger_transport'], // Railway Passenger Transport
     ['FFN2','494','raw:FFN2','pedestrian_transport'], // Pedestrian Transport
     ['FFN2','495','raw:FFN2','road_transport'], // Road Transport
     ['FFN2','496','raw:FFN2','road_passenger_transport'], // Road Passenger Transport
     ['FFN2','497','raw:FFN2','road_freight_transport'], // Road Freight Transport
     ['FFN2','500','raw:FFN2','pipeline_transport'], // Pipeline Transport
     ['FFN2','501','raw:FFN2','pumping'], // Pumping
     ['FFN2','505','raw:FFN2','water_transport'], // Water Transport
     ['FFN2','507','raw:FFN2','inland_waters_transport'], // Inland Waters Transport
     ['FFN2','508','raw:FFN2','canal_transport'], // Canal Transport
     ['FFN2','510','raw:FFN2','port_control'], // Port Control
     ['FFN2','511','raw:FFN2','maritime_pilotage'], // Maritime Pilotage
     ['FFN2','512','raw:FFN2','pilot_station'], // Pilot Station
     ['FFN2','513','raw:FFN2','harbour_control'], // Harbour Control
     ['FFN2','520','raw:FFN2','air_transport'], // Air Transport
     ['FFN2','525','raw:FFN2','air_traffic_control'], // Air Traffic Control
     ['FFN2','529','raw:FFN2','transportation_support'], // Transportation Support
     ['FFN2','530','raw:FFN2','warehousing_and_storage'], // Warehousing and Storage
     ['FFN2','535','raw:FFN2','motor_vehicle_parking'], // Motor Vehicle Parking
     ['FFN2','536','raw:FFN2','cargo_handling'], // Cargo Handling
     ['FFN2','537','raw:FFN2','customs_checkpoint'], // Customs Checkpoint
     ['FFN2','538','raw:FFN2','inspection_station'], // Inspection Station
     ['FFN2','539','raw:FFN2','inspection'], // Inspection
     ['FFN2','540','raw:FFN2','postal_activities'], // Postal Activities
     ['FFN2','541','raw:FFN2','mail_and_package_transport'], // Mail and Package Transport
     ['FFN2','545','raw:FFN2','courier_activities'], // Courier Activities
     ['FFN2','548','raw:FFN2','short-term_accommodation'], // Short-term Accommodation
     ['FFN2','549','raw:FFN2','long-term_accommodation'], // Long-term Accommodation
     ['FFN2','550','raw:FFN2','accommodation'], // Accommodation
     ['FFN2','551','raw:FFN2','hotel'], // Hotel
     ['FFN2','552','raw:FFN2','resort'], // Resort
     ['FFN2','553','raw:FFN2','motel'], // Motel
     ['FFN2','554','raw:FFN2','guest-house'], // Guest-house
     ['FFN2','555','raw:FFN2','hostel'], // Hostel
     ['FFN2','556','raw:FFN2','dormitory'], // Dormitory
     ['FFN2','557','raw:FFN2','vacation_cottage'], // Vacation Cottage
     ['FFN2','558','raw:FFN2','dependents_housing'], // Dependents Housing
     ['FFN2','559','raw:FFN2','communal_bath'], // Communal Bath
     ['FFN2','563','raw:FFN2','residence'], // Residence
     ['FFN2','570','raw:FFN2','food_service'], // Food Service
     ['FFN2','572','raw:FFN2','restaurant'], // Restaurant
     ['FFN2','573','raw:FFN2','bar'], // Bar
     ['FFN2','574','raw:FFN2','dining_hall'], // Dining Hall
     ['FFN2','578','raw:FFN2','banquet_hall'], // Banquet Hall
     ['FFN2','579','raw:FFN2','convention_centre'], // Convention Centre
     ['FFN2','580','raw:FFN2','publishing_and_broadcasting'], // Publishing and Broadcasting
     ['FFN2','582','raw:FFN2','print_publishing'], // Print Publishing
     ['FFN2','594','raw:FFN2','cinema'], // Cinema
     ['FFN2','601','raw:FFN2','radio_broadcasting'], // Radio Broadcasting
     ['FFN2','604','raw:FFN2','television_broadcasting'], // Television Broadcasting
     ['FFN2','610','raw:FFN2','telecommunications'], // Telecommunications
     ['FFN2','612','raw:FFN2','retail_telecommunications'], // Retail Telecommunications
     ['FFN2','614','raw:FFN2','wired_telecommunications'], // Wired Telecommunications
     ['FFN2','615','raw:FFN2','main_telephone_exchange'], // Main Telephone Exchange
     ['FFN2','616','raw:FFN2','branch_telephone_exchange'], // Branch Telephone Exchange
     ['FFN2','617','raw:FFN2','wired_repeater'], // Wired Repeater
     ['FFN2','620','raw:FFN2','wireless_telecommunications'], // Wireless Telecommunications
     ['FFN2','621','raw:FFN2','mobile_phone_service'], // Mobile Phone Service
     ['FFN2','622','raw:FFN2','wireless_repeater'], // Wireless Repeater
     ['FFN2','625','raw:FFN2','satellite_telecommunications'], // Satellite Telecommunications
     ['FFN2','626','raw:FFN2','satellite_ground_control'], // Satellite Ground Control
     ['FFN2','632','raw:FFN2','information_service'], // Information Service
     ['FFN2','633','raw:FFN2','public_records'], // Public Records
     ['FFN2','640','raw:FFN2','financial_services'], // Financial Services
     ['FFN2','642','raw:FFN2','central_banking'], // Central Banking
     ['FFN2','643','raw:FFN2','retail_banking'], // Retail Banking
     ['FFN2','651','raw:FFN2','insurance'], // Insurance
     ['FFN2','662','raw:FFN2','financial_market_administration'], // Financial Market Administration
     ['FFN2','663','raw:FFN2','security_brokerage'], // Security Brokerage
     ['FFN2','671','raw:FFN2','fund_management'], // Fund Management
     ['FFN2','680','raw:FFN2','real_estate_activities'], // Real Estate Activities
     ['FFN2','681','raw:FFN2','professional,_scientific_and_technical'], // Professional, Scientific and Technical
     ['FFN2','691','raw:FFN2','legal_activities'], // Legal Activities
     ['FFN2','696','raw:FFN2','accounting'], // Accounting
     ['FFN2','701','raw:FFN2','head_office'], // Head Office
     ['FFN2','706','raw:FFN2','business_management'], // Business Management
     ['FFN2','711','raw:FFN2','architecture_consulting'], // Architecture Consulting
     ['FFN2','714','raw:FFN2','engineering_design'], // Engineering Design
     ['FFN2','717','raw:FFN2','surveying'], // Surveying
     ['FFN2','720','raw:FFN2','scientific_research_and_development'], // Scientific Research and Development
     ['FFN2','721','raw:FFN2','observation_station'], // Observation Station
     ['FFN2','722','raw:FFN2','weather_station'], // Weather Station
     ['FFN2','725','raw:FFN2','nuclear_research_centre'], // Nuclear Research Centre
     ['FFN2','730','raw:FFN2','wind_tunnel'], // Wind Tunnel
     ['FFN2','741','raw:FFN2','advertising'], // Advertising
     ['FFN2','752','raw:FFN2','photography'], // Photography
     ['FFN2','757','raw:FFN2','veterinary'], // Veterinary
     ['FFN2','760','raw:FFN2','business_and_personal_support_services'], // Business and Personal Support Services
     ['FFN2','761','raw:FFN2','motor_vehicle_rental'], // Motor Vehicle Rental
     ['FFN2','770','raw:FFN2','employment_agency'], // Employment Agency
     ['FFN2','775','raw:FFN2','travel_agency'], // Travel Agency
     ['FFN2','780','raw:FFN2','security_enforcement'], // Security Enforcement
     ['FFN2','781','raw:FFN2','guard'], // Guard
     ['FFN2','791','raw:FFN2','custodial_service'], // Custodial Service
     ['FFN2','795','raw:FFN2','landscaping_service'], // Landscaping Service
     ['FFN2','801','raw:FFN2','office_administration'], // Office Administration
     ['FFN2','807','raw:FFN2','call_centre'], // Call Centre
     ['FFN2','808','raw:FFN2','public_administration'], // Public Administration
     ['FFN2','809','raw:FFN2','headquarters'], // Headquarters
     ['FFN2','810','raw:FFN2','administration'], // Administration
     ['FFN2','811','raw:FFN2','government'], // Government
     ['FFN2','812','raw:FFN2','local_government'], // Local Government
     ['FFN2','813','raw:FFN2','subnational_government'], // Subnational Government
     ['FFN2','814','raw:FFN2','national_government'], // National Government
     ['FFN2','815','raw:FFN2','palace'], // Palace
     ['FFN2','817','raw:FFN2','capitol'], // Capitol
     ['FFN2','818','raw:FFN2','executive_activities'], // Executive Activities
     ['FFN2','819','raw:FFN2','legislative_activities'], // Legislative Activities
     ['FFN2','821','raw:FFN2','polling_station'], // Polling Station
     ['FFN2','822','raw:FFN2','civil_activities'], // Civil Activities
     ['FFN2','825','raw:FFN2','diplomacy'], // Diplomacy
     ['FFN2','826','raw:FFN2','diplomatic_mission'], // Diplomatic Mission
     ['FFN2','827','raw:FFN2','embassy'], // Embassy
     ['FFN2','828','raw:FFN2','consul'], // Consul
     ['FFN2','829','raw:FFN2','maritime_defense'], // Maritime Defense
     ['FFN2','830','raw:FFN2','public_order,_safety_and_security_services'], // Public Order, Safety and Security Services
     ['FFN2','831','raw:FFN2','public_order'], // Public Order
     ['FFN2','832','raw:FFN2','safety'], // Safety
     ['FFN2','833','raw:FFN2','security_services'], // Security Services
     ['FFN2','835','raw:FFN2','defence_activities'], // Defence Activities
     ['FFN2','836','raw:FFN2','armory'], // Armory
     ['FFN2','837','raw:FFN2','military_reserve_activities'], // Military Reserve Activities
     ['FFN2','838','raw:FFN2','military_recruitment'], // Military Recruitment
     ['FFN2','839','raw:FFN2','cbrne_civilian_support'], // CBRNE Civilian Support
     ['FFN2','840','raw:FFN2','judicial_activities'], // Judicial Activities
     ['FFN2','841','raw:FFN2','law_enforcement'], // Law Enforcement
     ['FFN2','842','raw:FFN2','immigration_control'], // Immigration Control
     ['FFN2','843','raw:FFN2','imprisonment'], // Imprisonment
     ['FFN2','844','raw:FFN2','juvenile_corrections'], // Juvenile Corrections
     ['FFN2','845','raw:FFN2','firefighting'], // Firefighting
     ['FFN2','846','raw:FFN2','rescue_and_paramedical_services'], // Rescue and Paramedical Services
     ['FFN2','847','raw:FFN2','emergency_operations'], // Emergency Operations
     ['FFN2','848','raw:FFN2','civil_intelligence'], // Civil Intelligence
     ['FFN2','850','raw:FFN2','education'], // Education
     ['FFN2','851','raw:FFN2','primary_education'], // Primary Education
     ['FFN2','852','raw:FFN2','secondary_education'], // Secondary Education
     ['FFN2','855','raw:FFN2','higher_education'], // Higher Education
     ['FFN2','857','raw:FFN2','vocational_education'], // Vocational Education
     ['FFN2','860','raw:FFN2','human_health_activities'], // Human Health Activities
     ['FFN2','861','raw:FFN2','in-patient_care'], // In-patient Care
     ['FFN2','862','raw:FFN2','out-patient_care'], // Out-patient Care
     ['FFN2','863','raw:FFN2','urgent_medical_care'], // Urgent Medical Care
     ['FFN2','864','raw:FFN2','human_tissue_repository'], // Human Tissue Repository
     ['FFN2','865','raw:FFN2','public_health_activities'], // Public Health Activities
     ['FFN2','866','raw:FFN2','leprosy_care'], // Leprosy Care
     ['FFN2','871','raw:FFN2','intermediate_care'], // Intermediate Care
     ['FFN2','873','raw:FFN2','psychiatric_in-patient_care'], // Psychiatric In-patient Care
     ['FFN2','875','raw:FFN2','residential_care'], // Residential Care
     ['FFN2','881','raw:FFN2','emergency_shelter'], // Emergency Shelter
     ['FFN2','882','raw:FFN2','homeless_shelter'], // Homeless Shelter
     ['FFN2','883','raw:FFN2','refugee_shelter'], // Refugee Shelter
     ['FFN2','884','raw:FFN2','emergency_youth_shelter'], // Emergency Youth Shelter
     ['FFN2','885','raw:FFN2','day_care'], // Day Care
     ['FFN2','887','raw:FFN2','social_work'], // Social Work
     ['FFN2','888','raw:FFN2','emergency_relief_services'], // Emergency Relief Services
     ['FFN2','890','raw:FFN2','cultural,_arts_and_entertainment'], // Cultural, Arts and Entertainment
     ['FFN2','891','raw:FFN2','theatre'], // Theatre
     ['FFN2','892','raw:FFN2','auditorium'], // Auditorium
     ['FFN2','893','raw:FFN2','community_centre'], // Community Centre
     ['FFN2','894','raw:FFN2','opera_house'], // Opera House
     ['FFN2','895','raw:FFN2','night_club'], // Night Club
     ['FFN2','900','raw:FFN2','sports,_amusement_and_recreation'], // Sports, Amusement and Recreation
     ['FFN2','902','raw:FFN2','library'], // Library
     ['FFN2','905','raw:FFN2','museum'], // Museum
     ['FFN2','906','raw:FFN2','aquarium'], // Aquarium
     ['FFN2','907','raw:FFN2','botanical_and/or_zoological_reserve_activities'], // Botanical and/or Zoological Reserve Activities
     ['FFN2','909','raw:FFN2','gambling'], // Gambling
     ['FFN2','912','raw:FFN2','sports_centre'], // Sports Centre
     ['FFN2','913','raw:FFN2','fitness_centre'], // Fitness Centre
     ['FFN2','914','raw:FFN2','shooting_range'], // Shooting Range
     ['FFN2','915','raw:FFN2','water_park'], // Water Park
     ['FFN2','919','raw:FFN2','animal_boarding'], // Animal Boarding
     ['FFN2','921','raw:FFN2','recreation'], // Recreation
     ['FFN2','922','raw:FFN2','amusement'], // Amusement
     ['FFN2','923','raw:FFN2','hobbies_and/or_leisure_activities'], // Hobbies and/or Leisure Activities
     ['FFN2','930','raw:FFN2','religious_activities'], // Religious Activities
     ['FFN2','931','raw:FFN2','place_of_worship'], // Place of Worship
     ['FFN2','932','raw:FFN2','islamic_prayer_hall'], // Islamic Prayer Hall
     ['FFN2','950','raw:FFN2','membership_organization'], // Membership Organization
     ['FFN2','954','raw:FFN2','club'], // Club
     ['FFN2','955','raw:FFN2','yacht-club'], // Yacht-club
     ['FFN2','961','raw:FFN2','laundry'], // Laundry
     ['FFN2','962','raw:FFN2','beauty_treatment'], // Beauty Treatment
     ['FFN2','963','raw:FFN2','funeral_services'], // Funeral Services
     ['FFN2','964','raw:FFN2','cremation'], // Cremation
     ['FFN2','965','raw:FFN2','mortuary_services'], // Mortuary Services
     ['FFN2','966','raw:FFN2','adult_entertainment'], // Adult Entertainment
     ['FFN2','967','raw:FFN2','storage_of_human_remains'], // Storage of Human Remains
     ['FFN2','970','raw:FFN2','meeting_place'], // Meeting Place
     ['FFN2','980','raw:FFN2','death_care_services'], // Death Care Services
     ['FFN2','999','raw:FFN2','other'], // Other

     // FFN3 - Feature Function [3]
     // ['FFN3','-999999',undefined,undefined], // noInformation
     ['FFN3','2','raw:FFN3','agriculture'], // Agriculture
     ['FFN3','3','raw:FFN3','growing_of_crops'], // Growing of Crops
     ['FFN3','9','raw:FFN3','raising_of_animals'], // Raising of Animals
     ['FFN3','14','raw:FFN3','grazing'], // Grazing
     ['FFN3','15','raw:FFN3','mixed_farming'], // Mixed Farming
     ['FFN3','19','raw:FFN3','hunting'], // Hunting
     ['FFN3','20','raw:FFN3','forestry_and/or_logging'], // Forestry and/or Logging
     ['FFN3','21','raw:FFN3','silviculture'], // Silviculture
     ['FFN3','27','raw:FFN3','forest_warden'], // Forest Warden
     ['FFN3','30','raw:FFN3','fishing'], // Fishing
     ['FFN3','35','raw:FFN3','aquaculture'], // Aquaculture
     ['FFN3','40','raw:FFN3','mining_and_quarrying'], // Mining and Quarrying
     ['FFN3','50','raw:FFN3','solid_mineral_fuel_mining'], // Solid Mineral Fuel Mining
     ['FFN3','60','raw:FFN3','petroleum_and/or_gas_extraction'], // Petroleum and/or Gas Extraction
     ['FFN3','70','raw:FFN3','metal_ore_mining'], // Metal Ore Mining
     ['FFN3','83','raw:FFN3','chemical_mining'], // Chemical Mining
     ['FFN3','84','raw:FFN3','peat_extraction'], // Peat Extraction
     ['FFN3','85','raw:FFN3','salt_extraction'], // Salt Extraction
     ['FFN3','87','raw:FFN3','mineral_mining'], // Mineral Mining
     ['FFN3','91','raw:FFN3','gas_oil_separation'], // Gas Oil Separation
     ['FFN3','95','raw:FFN3','ore_dressing'], // Ore Dressing
     ['FFN3','99','raw:FFN3','manufacturing'], // Manufacturing
     ['FFN3','100','raw:FFN3','food_product_manufacture'], // Food Product Manufacture
     ['FFN3','101','raw:FFN3','food_processing'], // Food Processing
     ['FFN3','102','raw:FFN3','meat_processing'], // Meat Processing
     ['FFN3','103','raw:FFN3','seafood_processing'], // Seafood Processing
     ['FFN3','104','raw:FFN3','fruit_and/or_vegetable_processing'], // Fruit and/or Vegetable Processing
     ['FFN3','105','raw:FFN3','oil-mill'], // Oil-mill
     ['FFN3','106','raw:FFN3','dairying'], // Dairying
     ['FFN3','107','raw:FFN3','grain_milling'], // Grain Milling
     ['FFN3','110','raw:FFN3','baking'], // Baking
     ['FFN3','111','raw:FFN3','sugar_manufacture'], // Sugar Manufacture
     ['FFN3','112','raw:FFN3','sugar_milling'], // Sugar Milling
     ['FFN3','113','raw:FFN3','sugar_refining'], // Sugar Refining
     ['FFN3','114','raw:FFN3','confection_manufacture'], // Confection Manufacture
     ['FFN3','115','raw:FFN3','pasta_manufacture'], // Pasta Manufacture
     ['FFN3','116','raw:FFN3','prepared_meal_manufacture'], // Prepared Meal Manufacture
     ['FFN3','118','raw:FFN3','beverage_manufacture'], // Beverage Manufacture
     ['FFN3','119','raw:FFN3','animal_feed_manufacture'], // Animal Feed Manufacture
     ['FFN3','120','raw:FFN3','ice_manufacture'], // Ice Manufacture
     ['FFN3','121','raw:FFN3','spirit_distillery'], // Spirit Distillery
     ['FFN3','122','raw:FFN3','winery'], // Winery
     ['FFN3','123','raw:FFN3','brewing'], // Brewing
     ['FFN3','124','raw:FFN3','soft_drink_manufacture'], // Soft Drink Manufacture
     ['FFN3','125','raw:FFN3','tobacco_product_manufacture'], // Tobacco Product Manufacture
     ['FFN3','129','raw:FFN3','textile,_apparel_and_leather_manufacture'], // Textile, Apparel and Leather Manufacture
     ['FFN3','130','raw:FFN3','textile_manufacture'], // Textile Manufacture
     ['FFN3','140','raw:FFN3','apparel_manufacture'], // Apparel Manufacture
     ['FFN3','150','raw:FFN3','leather_product_manufacture'], // Leather Product Manufacture
     ['FFN3','155','raw:FFN3','footwear_manufacturing'], // Footwear Manufacturing
     ['FFN3','160','raw:FFN3','wood-based_manufacturing'], // Wood-based Manufacturing
     ['FFN3','161','raw:FFN3','sawmilling'], // Sawmilling
     ['FFN3','165','raw:FFN3','wooden_construction_product_manufacture'], // Wooden Construction Product Manufacture
     ['FFN3','171','raw:FFN3','paper-mill'], // Paper-mill
     ['FFN3','181','raw:FFN3','printing'], // Printing
     ['FFN3','190','raw:FFN3','petroleum_and_coal_products_manufacturing'], // Petroleum and Coal Products Manufacturing
     ['FFN3','191','raw:FFN3','coke_manufacture'], // Coke Manufacture
     ['FFN3','192','raw:FFN3','petroleum_refining'], // Petroleum Refining
     ['FFN3','195','raw:FFN3','chemical_manufacture'], // Chemical Manufacture
     ['FFN3','210','raw:FFN3','medicinal_product_manufacture'], // Medicinal Product Manufacture
     ['FFN3','221','raw:FFN3','rubber_product_manufacture'], // Rubber Product Manufacture
     ['FFN3','225','raw:FFN3','plastic_product_manufacture'], // Plastic Product Manufacture
     ['FFN3','230','raw:FFN3','nonmetallic_mineral_product_manufacturing'], // Nonmetallic Mineral Product Manufacturing
     ['FFN3','231','raw:FFN3','glass_product_manufacture'], // Glass Product Manufacture
     ['FFN3','232','raw:FFN3','refractory_product_manufacture'], // Refractory Product Manufacture
     ['FFN3','233','raw:FFN3','clay_product_manufacture'], // Clay Product Manufacture
     ['FFN3','234','raw:FFN3','ceramic_product_manufacture'], // Ceramic Product Manufacture
     ['FFN3','235','raw:FFN3','cement_mill'], // Cement Mill
     ['FFN3','236','raw:FFN3','cement_product_manufacture'], // Cement Product Manufacture
     ['FFN3','237','raw:FFN3','stone_product_manufacture'], // Stone Product Manufacture
     ['FFN3','240','raw:FFN3','primary_metal_manufacturing'], // Primary Metal Manufacturing
     ['FFN3','241','raw:FFN3','steel_mill'], // Steel Mill
     ['FFN3','242','raw:FFN3','metal_refining'], // Metal Refining
     ['FFN3','243','raw:FFN3','foundry'], // Foundry
     ['FFN3','250','raw:FFN3','metal_product_manufacturing'], // Metal Product Manufacturing
     ['FFN3','251','raw:FFN3','structural_metal_product_manufacture'], // Structural Metal Product Manufacture
     ['FFN3','255','raw:FFN3','munitions_manufacture'], // Munitions Manufacture
     ['FFN3','257','raw:FFN3','fabricated_metal_product_manufacture'], // Fabricated Metal Product Manufacture
     ['FFN3','260','raw:FFN3','electronic_equipment_manufacture'], // Electronic Equipment Manufacture
     ['FFN3','270','raw:FFN3','electrical_equipment_manufacture'], // Electrical Equipment Manufacture
     ['FFN3','272','raw:FFN3','propane_sale'], // Propane Sale
     ['FFN3','280','raw:FFN3','machinery_manufacture'], // Machinery Manufacture
     ['FFN3','289','raw:FFN3','transportation_equipment_manufacturing'], // Transportation Equipment Manufacturing
     ['FFN3','290','raw:FFN3','motor_vehicle_manufacture'], // Motor Vehicle Manufacture
     ['FFN3','301','raw:FFN3','ship_construction'], // Ship Construction
     ['FFN3','304','raw:FFN3','railway_vehicle_manufacture'], // Railway Vehicle Manufacture
     ['FFN3','305','raw:FFN3','aircraft_manufacture'], // Aircraft Manufacture
     ['FFN3','306','raw:FFN3','military_vehicle_manufacture'], // Military Vehicle Manufacture
     ['FFN3','310','raw:FFN3','furniture_manufacture'], // Furniture Manufacture
     ['FFN3','320','raw:FFN3','miscellaneous_manufacturing'], // Miscellaneous Manufacturing
     ['FFN3','321','raw:FFN3','jewellery_manufacture'], // Jewellery Manufacture
     ['FFN3','322','raw:FFN3','musical_instrument_manufacture'], // Musical Instrument Manufacture
     ['FFN3','323','raw:FFN3','sports_goods_manufacture'], // Sports Goods Manufacture
     ['FFN3','324','raw:FFN3','game_and/or_toy_manufacture'], // Game and/or Toy Manufacture
     ['FFN3','325','raw:FFN3','medical_and/or_dental_equipment_manufacture'], // Medical and/or Dental Equipment Manufacture
     ['FFN3','330','raw:FFN3','general_repair'], // General Repair
     ['FFN3','331','raw:FFN3','fabricated_metal_product_repair'], // Fabricated Metal Product Repair
     ['FFN3','332','raw:FFN3','electronic_equipment_repair'], // Electronic Equipment Repair
     ['FFN3','333','raw:FFN3','electrical_equipment_repair'], // Electrical Equipment Repair
     ['FFN3','334','raw:FFN3','machinery_repair'], // Machinery Repair
     ['FFN3','340','raw:FFN3','ship_repair'], // Ship Repair
     ['FFN3','341','raw:FFN3','aircraft_repair'], // Aircraft Repair
     ['FFN3','342','raw:FFN3','railway_vehicle_repair'], // Railway Vehicle Repair
     ['FFN3','343','raw:FFN3','motor_vehicle_repair'], // Motor Vehicle Repair
     ['FFN3','350','raw:FFN3','utilities'], // Utilities
     ['FFN3','351','raw:FFN3','power_generation'], // Power Generation
     ['FFN3','352','raw:FFN3','climate_control'], // Climate Control
     ['FFN3','355','raw:FFN3','cooling'], // Cooling
     ['FFN3','356','raw:FFN3','heating'], // Heating
     ['FFN3','360','raw:FFN3','water_supply'], // Water Supply
     ['FFN3','361','raw:FFN3','water_collection'], // Water Collection
     ['FFN3','362','raw:FFN3','water_treatment'], // Water Treatment
     ['FFN3','363','raw:FFN3','water_distribution'], // Water Distribution
     ['FFN3','370','raw:FFN3','sewerage'], // Sewerage
     ['FFN3','372','raw:FFN3','sewerage_screening'], // Sewerage Screening
     ['FFN3','382','raw:FFN3','restroom'], // Restroom
     ['FFN3','383','raw:FFN3','waste_treatment_and_disposal'], // Waste Treatment and Disposal
     ['FFN3','385','raw:FFN3','materials_recovery'], // Materials Recovery
     ['FFN3','440','raw:FFN3','commerce'], // Commerce
     ['FFN3','459','raw:FFN3','wholesale_merchant'], // Wholesale Merchant
     ['FFN3','460','raw:FFN3','retail_sale'], // Retail Sale
     ['FFN3','464','raw:FFN3','specialized_store'], // Specialized Store
     ['FFN3','465','raw:FFN3','non-specialized_store'], // Non-specialized Store
     ['FFN3','466','raw:FFN3','convenience_store'], // Convenience Store
     ['FFN3','470','raw:FFN3','petrol_sale'], // Petrol Sale
     ['FFN3','473','raw:FFN3','sales_yard'], // Sales Yard
     ['FFN3','474','raw:FFN3','precious_metal_merchant'], // Precious Metal Merchant
     ['FFN3','475','raw:FFN3','market'], // Market
     ['FFN3','476','raw:FFN3','grocery'], // Grocery
     ['FFN3','477','raw:FFN3','pharmacy'], // Pharmacy
     ['FFN3','478','raw:FFN3','pet-shop'], // Pet-shop
     ['FFN3','480','raw:FFN3','transport'], // Transport
     ['FFN3','481','raw:FFN3','terminal'], // Terminal
     ['FFN3','482','raw:FFN3','station'], // Station
     ['FFN3','483','raw:FFN3','stop'], // Stop
     ['FFN3','484','raw:FFN3','transfer_hub'], // Transfer Hub
     ['FFN3','486','raw:FFN3','signalling'], // Signalling
     ['FFN3','487','raw:FFN3','transport_system_maintenance'], // Transport System Maintenance
     ['FFN3','488','raw:FFN3','navigation'], // Navigation
     ['FFN3','489','raw:FFN3','transportation_hub'], // Transportation Hub
     ['FFN3','490','raw:FFN3','railway_transport'], // Railway Transport
     ['FFN3','491','raw:FFN3','railway_passenger_transport'], // Railway Passenger Transport
     ['FFN3','494','raw:FFN3','pedestrian_transport'], // Pedestrian Transport
     ['FFN3','495','raw:FFN3','road_transport'], // Road Transport
     ['FFN3','496','raw:FFN3','road_passenger_transport'], // Road Passenger Transport
     ['FFN3','497','raw:FFN3','road_freight_transport'], // Road Freight Transport
     ['FFN3','500','raw:FFN3','pipeline_transport'], // Pipeline Transport
     ['FFN3','501','raw:FFN3','pumping'], // Pumping
     ['FFN3','505','raw:FFN3','water_transport'], // Water Transport
     ['FFN3','507','raw:FFN3','inland_waters_transport'], // Inland Waters Transport
     ['FFN3','508','raw:FFN3','canal_transport'], // Canal Transport
     ['FFN3','510','raw:FFN3','port_control'], // Port Control
     ['FFN3','511','raw:FFN3','maritime_pilotage'], // Maritime Pilotage
     ['FFN3','512','raw:FFN3','pilot_station'], // Pilot Station
     ['FFN3','513','raw:FFN3','harbour_control'], // Harbour Control
     ['FFN3','520','raw:FFN3','air_transport'], // Air Transport
     ['FFN3','525','raw:FFN3','air_traffic_control'], // Air Traffic Control
     ['FFN3','529','raw:FFN3','transportation_support'], // Transportation Support
     ['FFN3','530','raw:FFN3','warehousing_and_storage'], // Warehousing and Storage
     ['FFN3','535','raw:FFN3','motor_vehicle_parking'], // Motor Vehicle Parking
     ['FFN3','536','raw:FFN3','cargo_handling'], // Cargo Handling
     ['FFN3','537','raw:FFN3','customs_checkpoint'], // Customs Checkpoint
     ['FFN3','538','raw:FFN3','inspection_station'], // Inspection Station
     ['FFN3','539','raw:FFN3','inspection'], // Inspection
     ['FFN3','540','raw:FFN3','postal_activities'], // Postal Activities
     ['FFN3','541','raw:FFN3','mail_and_package_transport'], // Mail and Package Transport
     ['FFN3','545','raw:FFN3','courier_activities'], // Courier Activities
     ['FFN3','548','raw:FFN3','short-term_accommodation'], // Short-term Accommodation
     ['FFN3','549','raw:FFN3','long-term_accommodation'], // Long-term Accommodation
     ['FFN3','550','raw:FFN3','accommodation'], // Accommodation
     ['FFN3','551','raw:FFN3','hotel'], // Hotel
     ['FFN3','552','raw:FFN3','resort'], // Resort
     ['FFN3','553','raw:FFN3','motel'], // Motel
     ['FFN3','554','raw:FFN3','guest-house'], // Guest-house
     ['FFN3','555','raw:FFN3','hostel'], // Hostel
     ['FFN3','556','raw:FFN3','dormitory'], // Dormitory
     ['FFN3','557','raw:FFN3','vacation_cottage'], // Vacation Cottage
     ['FFN3','558','raw:FFN3','dependents_housing'], // Dependents Housing
     ['FFN3','559','raw:FFN3','communal_bath'], // Communal Bath
     ['FFN3','563','raw:FFN3','residence'], // Residence
     ['FFN3','570','raw:FFN3','food_service'], // Food Service
     ['FFN3','572','raw:FFN3','restaurant'], // Restaurant
     ['FFN3','573','raw:FFN3','bar'], // Bar
     ['FFN3','574','raw:FFN3','dining_hall'], // Dining Hall
     ['FFN3','578','raw:FFN3','banquet_hall'], // Banquet Hall
     ['FFN3','579','raw:FFN3','convention_centre'], // Convention Centre
     ['FFN3','580','raw:FFN3','publishing_and_broadcasting'], // Publishing and Broadcasting
     ['FFN3','582','raw:FFN3','print_publishing'], // Print Publishing
     ['FFN3','594','raw:FFN3','cinema'], // Cinema
     ['FFN3','601','raw:FFN3','radio_broadcasting'], // Radio Broadcasting
     ['FFN3','604','raw:FFN3','television_broadcasting'], // Television Broadcasting
     ['FFN3','610','raw:FFN3','telecommunications'], // Telecommunications
     ['FFN3','612','raw:FFN3','retail_telecommunications'], // Retail Telecommunications
     ['FFN3','614','raw:FFN3','wired_telecommunications'], // Wired Telecommunications
     ['FFN3','615','raw:FFN3','main_telephone_exchange'], // Main Telephone Exchange
     ['FFN3','616','raw:FFN3','branch_telephone_exchange'], // Branch Telephone Exchange
     ['FFN3','617','raw:FFN3','wired_repeater'], // Wired Repeater
     ['FFN3','620','raw:FFN3','wireless_telecommunications'], // Wireless Telecommunications
     ['FFN3','621','raw:FFN3','mobile_phone_service'], // Mobile Phone Service
     ['FFN3','622','raw:FFN3','wireless_repeater'], // Wireless Repeater
     ['FFN3','625','raw:FFN3','satellite_telecommunications'], // Satellite Telecommunications
     ['FFN3','626','raw:FFN3','satellite_ground_control'], // Satellite Ground Control
     ['FFN3','632','raw:FFN3','information_service'], // Information Service
     ['FFN3','633','raw:FFN3','public_records'], // Public Records
     ['FFN3','640','raw:FFN3','financial_services'], // Financial Services
     ['FFN3','642','raw:FFN3','central_banking'], // Central Banking
     ['FFN3','643','raw:FFN3','retail_banking'], // Retail Banking
     ['FFN3','651','raw:FFN3','insurance'], // Insurance
     ['FFN3','662','raw:FFN3','financial_market_administration'], // Financial Market Administration
     ['FFN3','663','raw:FFN3','security_brokerage'], // Security Brokerage
     ['FFN3','671','raw:FFN3','fund_management'], // Fund Management
     ['FFN3','680','raw:FFN3','real_estate_activities'], // Real Estate Activities
     ['FFN3','681','raw:FFN3','professional,_scientific_and_technical'], // Professional, Scientific and Technical
     ['FFN3','691','raw:FFN3','legal_activities'], // Legal Activities
     ['FFN3','696','raw:FFN3','accounting'], // Accounting
     ['FFN3','701','raw:FFN3','head_office'], // Head Office
     ['FFN3','706','raw:FFN3','business_management'], // Business Management
     ['FFN3','711','raw:FFN3','architecture_consulting'], // Architecture Consulting
     ['FFN3','714','raw:FFN3','engineering_design'], // Engineering Design
     ['FFN3','717','raw:FFN3','surveying'], // Surveying
     ['FFN3','720','raw:FFN3','scientific_research_and_development'], // Scientific Research and Development
     ['FFN3','721','raw:FFN3','observation_station'], // Observation Station
     ['FFN3','722','raw:FFN3','weather_station'], // Weather Station
     ['FFN3','725','raw:FFN3','nuclear_research_centre'], // Nuclear Research Centre
     ['FFN3','730','raw:FFN3','wind_tunnel'], // Wind Tunnel
     ['FFN3','741','raw:FFN3','advertising'], // Advertising
     ['FFN3','752','raw:FFN3','photography'], // Photography
     ['FFN3','757','raw:FFN3','veterinary'], // Veterinary
     ['FFN3','760','raw:FFN3','business_and_personal_support_services'], // Business and Personal Support Services
     ['FFN3','761','raw:FFN3','motor_vehicle_rental'], // Motor Vehicle Rental
     ['FFN3','770','raw:FFN3','employment_agency'], // Employment Agency
     ['FFN3','775','raw:FFN3','travel_agency'], // Travel Agency
     ['FFN3','780','raw:FFN3','security_enforcement'], // Security Enforcement
     ['FFN3','781','raw:FFN3','guard'], // Guard
     ['FFN3','791','raw:FFN3','custodial_service'], // Custodial Service
     ['FFN3','795','raw:FFN3','landscaping_service'], // Landscaping Service
     ['FFN3','801','raw:FFN3','office_administration'], // Office Administration
     ['FFN3','807','raw:FFN3','call_centre'], // Call Centre
     ['FFN3','808','raw:FFN3','public_administration'], // Public Administration
     ['FFN3','809','raw:FFN3','headquarters'], // Headquarters
     ['FFN3','810','raw:FFN3','administration'], // Administration
     ['FFN3','811','raw:FFN3','government'], // Government
     ['FFN3','812','raw:FFN3','local_government'], // Local Government
     ['FFN3','813','raw:FFN3','subnational_government'], // Subnational Government
     ['FFN3','814','raw:FFN3','national_government'], // National Government
     ['FFN3','815','raw:FFN3','palace'], // Palace
     ['FFN3','817','raw:FFN3','capitol'], // Capitol
     ['FFN3','818','raw:FFN3','executive_activities'], // Executive Activities
     ['FFN3','819','raw:FFN3','legislative_activities'], // Legislative Activities
     ['FFN3','821','raw:FFN3','polling_station'], // Polling Station
     ['FFN3','822','raw:FFN3','civil_activities'], // Civil Activities
     ['FFN3','825','raw:FFN3','diplomacy'], // Diplomacy
     ['FFN3','826','raw:FFN3','diplomatic_mission'], // Diplomatic Mission
     ['FFN3','827','raw:FFN3','embassy'], // Embassy
     ['FFN3','828','raw:FFN3','consul'], // Consul
     ['FFN3','829','raw:FFN3','maritime_defense'], // Maritime Defense
     ['FFN3','830','raw:FFN3','public_order,_safety_and_security_services'], // Public Order, Safety and Security Services
     ['FFN3','831','raw:FFN3','public_order'], // Public Order
     ['FFN3','832','raw:FFN3','safety'], // Safety
     ['FFN3','833','raw:FFN3','security_services'], // Security Services
     ['FFN3','835','raw:FFN3','defence_activities'], // Defence Activities
     ['FFN3','836','raw:FFN3','armory'], // Armory
     ['FFN3','837','raw:FFN3','military_reserve_activities'], // Military Reserve Activities
     ['FFN3','838','raw:FFN3','military_recruitment'], // Military Recruitment
     ['FFN3','839','raw:FFN3','cbrne_civilian_support'], // CBRNE Civilian Support
     ['FFN3','840','raw:FFN3','judicial_activities'], // Judicial Activities
     ['FFN3','841','raw:FFN3','law_enforcement'], // Law Enforcement
     ['FFN3','842','raw:FFN3','immigration_control'], // Immigration Control
     ['FFN3','843','raw:FFN3','imprisonment'], // Imprisonment
     ['FFN3','844','raw:FFN3','juvenile_corrections'], // Juvenile Corrections
     ['FFN3','845','raw:FFN3','firefighting'], // Firefighting
     ['FFN3','846','raw:FFN3','rescue_and_paramedical_services'], // Rescue and Paramedical Services
     ['FFN3','847','raw:FFN3','emergency_operations'], // Emergency Operations
     ['FFN3','848','raw:FFN3','civil_intelligence'], // Civil Intelligence
     ['FFN3','850','raw:FFN3','education'], // Education
     ['FFN3','851','raw:FFN3','primary_education'], // Primary Education
     ['FFN3','852','raw:FFN3','secondary_education'], // Secondary Education
     ['FFN3','855','raw:FFN3','higher_education'], // Higher Education
     ['FFN3','857','raw:FFN3','vocational_education'], // Vocational Education
     ['FFN3','860','raw:FFN3','human_health_activities'], // Human Health Activities
     ['FFN3','861','raw:FFN3','in-patient_care'], // In-patient Care
     ['FFN3','862','raw:FFN3','out-patient_care'], // Out-patient Care
     ['FFN3','863','raw:FFN3','urgent_medical_care'], // Urgent Medical Care
     ['FFN3','864','raw:FFN3','human_tissue_repository'], // Human Tissue Repository
     ['FFN3','865','raw:FFN3','public_health_activities'], // Public Health Activities
     ['FFN3','866','raw:FFN3','leprosy_care'], // Leprosy Care
     ['FFN3','871','raw:FFN3','intermediate_care'], // Intermediate Care
     ['FFN3','873','raw:FFN3','psychiatric_in-patient_care'], // Psychiatric In-patient Care
     ['FFN3','875','raw:FFN3','residential_care'], // Residential Care
     ['FFN3','881','raw:FFN3','emergency_shelter'], // Emergency Shelter
     ['FFN3','882','raw:FFN3','homeless_shelter'], // Homeless Shelter
     ['FFN3','883','raw:FFN3','refugee_shelter'], // Refugee Shelter
     ['FFN3','884','raw:FFN3','emergency_youth_shelter'], // Emergency Youth Shelter
     ['FFN3','885','raw:FFN3','day_care'], // Day Care
     ['FFN3','887','raw:FFN3','social_work'], // Social Work
     ['FFN3','888','raw:FFN3','emergency_relief_services'], // Emergency Relief Services
     ['FFN3','890','raw:FFN3','cultural,_arts_and_entertainment'], // Cultural, Arts and Entertainment
     ['FFN3','891','raw:FFN3','theatre'], // Theatre
     ['FFN3','892','raw:FFN3','auditorium'], // Auditorium
     ['FFN3','893','raw:FFN3','community_centre'], // Community Centre
     ['FFN3','894','raw:FFN3','opera_house'], // Opera House
     ['FFN3','895','raw:FFN3','night_club'], // Night Club
     ['FFN3','900','raw:FFN3','sports,_amusement_and_recreation'], // Sports, Amusement and Recreation
     ['FFN3','902','raw:FFN3','library'], // Library
     ['FFN3','905','raw:FFN3','museum'], // Museum
     ['FFN3','906','raw:FFN3','aquarium'], // Aquarium
     ['FFN3','907','raw:FFN3','botanical_and/or_zoological_reserve_activities'], // Botanical and/or Zoological Reserve Activities
     ['FFN3','909','raw:FFN3','gambling'], // Gambling
     ['FFN3','912','raw:FFN3','sports_centre'], // Sports Centre
     ['FFN3','913','raw:FFN3','fitness_centre'], // Fitness Centre
     ['FFN3','914','raw:FFN3','shooting_range'], // Shooting Range
     ['FFN3','915','raw:FFN3','water_park'], // Water Park
     ['FFN3','919','raw:FFN3','animal_boarding'], // Animal Boarding
     ['FFN3','921','raw:FFN3','recreation'], // Recreation
     ['FFN3','922','raw:FFN3','amusement'], // Amusement
     ['FFN3','923','raw:FFN3','hobbies_and/or_leisure_activities'], // Hobbies and/or Leisure Activities
     ['FFN3','930','raw:FFN3','religious_activities'], // Religious Activities
     ['FFN3','931','raw:FFN3','place_of_worship'], // Place of Worship
     ['FFN3','932','raw:FFN3','islamic_prayer_hall'], // Islamic Prayer Hall
     ['FFN3','950','raw:FFN3','membership_organization'], // Membership Organization
     ['FFN3','954','raw:FFN3','club'], // Club
     ['FFN3','955','raw:FFN3','yacht-club'], // Yacht-club
     ['FFN3','961','raw:FFN3','laundry'], // Laundry
     ['FFN3','962','raw:FFN3','beauty_treatment'], // Beauty Treatment
     ['FFN3','963','raw:FFN3','funeral_services'], // Funeral Services
     ['FFN3','964','raw:FFN3','cremation'], // Cremation
     ['FFN3','965','raw:FFN3','mortuary_services'], // Mortuary Services
     ['FFN3','966','raw:FFN3','adult_entertainment'], // Adult Entertainment
     ['FFN3','967','raw:FFN3','storage_of_human_remains'], // Storage of Human Remains
     ['FFN3','970','raw:FFN3','meeting_place'], // Meeting Place
     ['FFN3','980','raw:FFN3','death_care_services'], // Death Care Services
     ['FFN3','999','raw:FFN3','other'], // Other

     // FHC - Harbour Facility Function
     // ['FHC','-999999',undefined,undefined], // noInformation
     ['FHC','1','raw:FHC','ro-ro_terminal'], // Ro-Ro Terminal
     ['FHC','2','raw:FHC','timber-yard'], // Timber-yard
     ['FHC','3','raw:FHC','ferry_terminal'], // Ferry Terminal
     ['FHC','4','raw:FHC','fishing_harbour'], // Fishing Harbour
     ['FHC','5','raw:FHC','marina'], // Marina
     ['FHC','6','raw:FHC','naval_base'], // Naval Base
     ['FHC','7','raw:FHC','tanker_terminal'], // Tanker Terminal
     ['FHC','8','raw:FHC','passenger_terminal'], // Passenger Terminal
     ['FHC','9','raw:FHC','shipyard'], // Shipyard
     ['FHC','10','raw:FHC','container_terminal'], // Container Terminal
     ['FHC','11','raw:FHC','bulk_terminal'], // Bulk Terminal
     ['FHC','999','raw:FHC','other'], // Other

     // FHC2 - Harbour Facility Function [2]
     // ['FHC2','-999999',undefined,undefined], // noInformation
     ['FHC2','1','raw:FHC2','ro-ro_terminal'], // Ro-Ro Terminal
     ['FHC2','2','raw:FHC2','timber-yard'], // Timber-yard
     ['FHC2','3','raw:FHC2','ferry_terminal'], // Ferry Terminal
     ['FHC2','4','raw:FHC2','fishing_harbour'], // Fishing Harbour
     ['FHC2','5','raw:FHC2','marina'], // Marina
     ['FHC2','6','raw:FHC2','naval_base'], // Naval Base
     ['FHC2','7','raw:FHC2','tanker_terminal'], // Tanker Terminal
     ['FHC2','8','raw:FHC2','passenger_terminal'], // Passenger Terminal
     ['FHC2','9','raw:FHC2','shipyard'], // Shipyard
     ['FHC2','10','raw:FHC2','container_terminal'], // Container Terminal
     ['FHC2','11','raw:FHC2','bulk_terminal'], // Bulk Terminal
     ['FHC2','999','raw:FHC2','other'], // Other

     // FHC3 - Harbour Facility Function [3]
     // ['FHC3','-999999',undefined,undefined], // noInformation
     ['FHC3','1','raw:FHC3','ro-ro_terminal'], // Ro-Ro Terminal
     ['FHC3','2','raw:FHC3','timber-yard'], // Timber-yard
     ['FHC3','3','raw:FHC3','ferry_terminal'], // Ferry Terminal
     ['FHC3','4','raw:FHC3','fishing_harbour'], // Fishing Harbour
     ['FHC3','5','raw:FHC3','marina'], // Marina
     ['FHC3','6','raw:FHC3','naval_base'], // Naval Base
     ['FHC3','7','raw:FHC3','tanker_terminal'], // Tanker Terminal
     ['FHC3','8','raw:FHC3','passenger_terminal'], // Passenger Terminal
     ['FHC3','9','raw:FHC3','shipyard'], // Shipyard
     ['FHC3','10','raw:FHC3','container_terminal'], // Container Terminal
     ['FHC3','11','raw:FHC3','bulk_terminal'], // Bulk Terminal
     ['FHC3','999','raw:FHC3','other'], // Other

     // FIC - Embankment Type
     // ['FIC','-999999',undefined,undefined], // noInformation
     ['FIC','1','raw:FIC','mound'], // Mound
     ['FIC','2','raw:FIC','fill'], // Fill
     ['FIC','3','raw:FIC','dyke'], // Dyke
     ['FIC','5','raw:FIC','levee'], // Levee
     ['FIC','6','raw:FIC','divider'], // Divider
     ['FIC','999','raw:FIC','other'], // Other

     // FLO - Floating
     // ['FLO','-999999',undefined,undefined], // noInformation
     ['FLO','1000','raw:FLO','no'],
     ['FLO','1001','raw:FLO','yes'],

     // FPT - Airfield Type
     // ['FPT','-999999',undefined,undefined], // noInformation
     ['FPT','1','raw:FPT','major'], // Major
     ['FPT','2','raw:FPT','minor_and_hard'], // Minor and Hard
     ['FPT','3','raw:FPT','minor_and_soft'], // Minor and Soft
     ['FPT','4','raw:FPT','minor'], // Minor
     ['FPT','999','raw:FPT','other'], // Other

     // FRT - Weapons Range Type
     // ['FRT','-999999',undefined,undefined], // noInformation
     ['FRT','1','raw:FRT','small_arms'], // Small Arms
     ['FRT','2','raw:FRT','tank'], // Tank
     ['FRT','3','raw:FRT','field_artillery'], // Field Artillery
     ['FRT','4','raw:FRT','grenade'], // Grenade
     ['FRT','5','raw:FRT','demolition_area'], // Demolition Area
     ['FRT','6','raw:FRT','impact_area'], // Impact Area
     ['FRT','999','raw:FRT','other'], // Other

     // FRT2 - Weapons Range Type [2]
     // ['FRT2','-999999',undefined,undefined], // noInformation
     ['FRT2','1','raw:FRT2','small_arms'], // Small Arms
     ['FRT2','2','raw:FRT2','tank'], // Tank
     ['FRT2','3','raw:FRT2','field_artillery'], // Field Artillery
     ['FRT2','4','raw:FRT2','grenade'], // Grenade
     ['FRT2','5','raw:FRT2','demolition_area'], // Demolition Area
     ['FRT2','6','raw:FRT2','impact_area'], // Impact Area
     ['FRT2','999','raw:FRT2','other'], // Other

     // FRT3 - Weapons Range Type [3]
     // ['FRT3','-999999',undefined,undefined], // noInformation
     ['FRT3','1','raw:FRT3','small_arms'], // Small Arms
     ['FRT3','2','raw:FRT3','tank'], // Tank
     ['FRT3','3','raw:FRT3','field_artillery'], // Field Artillery
     ['FRT3','4','raw:FRT3','grenade'], // Grenade
     ['FRT3','5','raw:FRT3','demolition_area'], // Demolition Area
     ['FRT3','6','raw:FRT3','impact_area'], // Impact Area
     ['FRT3','999','raw:FRT3','other'], // Other

     // FSC - Flight Strip Capable
     // ['FSC','-999999',undefined,undefined], // noInformation
     ['FSC','1000','raw:FSC','no'],
     ['FSC','1001','raw:FSC','yes'],

     // FTI - Fence Type
     // ['FTI','-999999',undefined,undefined], // noInformation
     ['FTI','1','raw:FTI','metal'], // Metal
     ['FTI','2','raw:FTI','wood'], // Wood
     ['FTI','5','raw:FTI','barbed_wire'], // Barbed Wire
     ['FTI','6','raw:FTI','chain-link'], // Chain-link
     ['FTI','7','raw:FTI','electrified_wire'], // Electrified Wire
     ['FTI','8','raw:FTI','geotextile'], // Geotextile
     ['FTI','9','raw:FTI','netting'], // Netting
     ['FTI','999','raw:FTI','other'], // Other

     // FZR - Fortified Building Type
     // ['FZR','-999999',undefined,undefined], // noInformation
     ['FZR','1','raw:FZR','blockhouse'], // Blockhouse
     ['FZR','2','raw:FZR','casement'], // Casement
     ['FZR','3','raw:FZR','keep'], // Keep
     ['FZR','4','raw:FZR','martello_tower'], // Martello Tower
     ['FZR','5','raw:FZR','non-specific_fortified'], // Non-specific Fortified
     ['FZR','6','raw:FZR','pillbox'], // Pillbox
     ['FZR','999','raw:FZR','other'], // Other

     // GFT - Geologic Fault Trace Visible
     // ['GFT','-999999',undefined,undefined], // noInformation
     ['GFT','1000','raw:GFT','no'],
     ['GFT','1001','raw:GFT','yes'],

     // GOT - Geothermal Outlet Type
     // ['GOT','-999999',undefined,undefined], // noInformation
     ['GOT','1','raw:GOT','fissure'], // Fissure
     ['GOT','2','raw:GOT','fumarole'], // Fumarole
     ['GOT','3','raw:GOT','hot_spring'], // Hot Spring
     ['GOT','4','raw:GOT','sulphur_spring'], // Sulphur Spring
     ['GOT','5','raw:GOT','geyser'], // Geyser
     ['GOT','999','raw:GOT','other'], // Other

     // GSGCHC - Runway Direction : Surface Slope (high end) <interval closure>
     ['GSGCHC','2','raw:GSGCHC','open_interval'], // Open Interval
     ['GSGCHC','3','raw:GSGCHC','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['GSGCHC','4','raw:GSGCHC','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['GSGCHC','5','raw:GSGCHC','closed_interval'], // Closed Interval
     ['GSGCHC','6','raw:GSGCHC','greater-than_semi-interval'], // Greater-than Semi-interval
     ['GSGCHC','7','raw:GSGCHC','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['GSGCHC','8','raw:GSGCHC','less-than_semi-interval'], // Less-than Semi-interval
     ['GSGCHC','9','raw:GSGCHC','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // GSGCLC - Runway Direction : Surface Slope (low end) <interval closure>
     ['GSGCLC','2','raw:GSGCLC','open_interval'], // Open Interval
     ['GSGCLC','3','raw:GSGCLC','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['GSGCLC','4','raw:GSGCLC','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['GSGCLC','5','raw:GSGCLC','closed_interval'], // Closed Interval
     ['GSGCLC','6','raw:GSGCLC','greater-than_semi-interval'], // Greater-than Semi-interval
     ['GSGCLC','7','raw:GSGCLC','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['GSGCLC','8','raw:GSGCLC','less-than_semi-interval'], // Less-than Semi-interval
     ['GSGCLC','9','raw:GSGCLC','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // GTC - Gate Use
     // ['GTC','-999999',undefined,undefined], // noInformation
     ['GTC','1','raw:GTC','toll-gate'], // Toll-gate
     ['GTC','2','raw:GTC','crossing'], // Crossing
     ['GTC','3','raw:GTC','entrance'], // Entrance
     ['GTC','4','raw:GTC','border_crossing'], // Border Crossing
     ['GTC','999','raw:GTC','other'], // Other

     // GUG - Guyed
     // ['GUG','-999999',undefined,undefined], // noInformation
     ['GUG','1000','raw:GUG','no'],
     ['GUG','1001','raw:GUG','yes'],

     // HAF - Helipad Associated Facility
     // ['HAF','-999999',undefined,undefined], // noInformation
     ['HAF','1','raw:HAF','heliport'], // Heliport
     ['HAF','2','raw:HAF','land_aerodrome'], // Land Aerodrome
     ['HAF','3','raw:HAF','hospital'], // Hospital
     ['HAF','4','raw:HAF','non-hospital_building'], // Non-hospital Building
     ['HAF','6','raw:HAF','rig'], // Rig
     ['HAF','9','raw:HAF','offshore_construction'], // Offshore Construction
     ['HAF','10','raw:HAF','water_aerodrome'], // Water Aerodrome
     ['HAF','11','raw:HAF','military_installation'], // Military Installation
     ['HAF','999','raw:HAF','other'], // Other

     // HST - Hazard Shelter Intended Use
     // ['HST','-999999',undefined,undefined], // noInformation
     ['HST','1','raw:HST','bomb_shelter'], // Bomb Shelter
     ['HST','2','raw:HST','fallout_shelter'], // Fallout Shelter
     ['HST','3','raw:HST','storm_shelter'], // Storm Shelter
     ['HST','999','raw:HST','other'], // Other

     // HST2 - Hazard Shelter Intended Use [2]
     // ['HST2','-999999',undefined,undefined], // noInformation
     ['HST2','1','raw:HST2','bomb_shelter'], // Bomb Shelter
     ['HST2','2','raw:HST2','fallout_shelter'], // Fallout Shelter
     ['HST2','3','raw:HST2','storm_shelter'], // Storm Shelter
     ['HST2','999','raw:HST2','other'], // Other

     // HST3 - Hazard Shelter Intended Use [3]
     // ['HST3','-999999',undefined,undefined], // noInformation
     ['HST3','1','raw:HST3','bomb_shelter'], // Bomb Shelter
     ['HST3','2','raw:HST3','fallout_shelter'], // Fallout Shelter
     ['HST3','3','raw:HST3','storm_shelter'], // Storm Shelter
     ['HST3','999','raw:HST3','other'], // Other

     // HTP - Hangar Type Category
     // ['HTP','-999999',undefined,undefined], // noInformation
     ['HTP','1','raw:HTP','m_-_multi-bay'], // M - Multi-bay
     ['HTP','2','raw:HTP','o_-_open_end'], // O - Open End
     ['HTP','3','raw:HTP','n_-_nose_in'], // N - Nose In
     ['HTP','4','raw:HTP','g_-_underground'], // G - Underground
     ['HTP','5','raw:HTP','s_-_single_bay'], // S - Single Bay
     ['HTP','6','raw:HTP','d_-_double_bay'], // D - Double Bay
     ['HTP','7','raw:HTP','t_-_t-shaped'], // T - T-Shaped

     // HZD - Geodetic Datum
     ['HZD','adindanBurkinaFaso','raw:HZD','adindan_(burkina_faso)'], // Adindan (Burkina Faso)
     ['HZD','adindanCameroon','raw:HZD','adindan_(cameroon)'], // Adindan (Cameroon)
     ['HZD','adindanEthiopia','raw:HZD','adindan_(ethiopia)'], // Adindan (Ethiopia)
     ['HZD','adindanMali','raw:HZD','adindan_(mali)'], // Adindan (Mali)
     ['HZD','adindanMeanValue','raw:HZD','adindan_(mean_value)'], // Adindan (mean value)
     ['HZD','adindanSenegal','raw:HZD','adindan_(senegal)'], // Adindan (Senegal)
     ['HZD','adindanSudan','raw:HZD','adindan_(sudan)'], // Adindan (Sudan)
     ['HZD','afgooyeSomalia','raw:HZD','afgooye_(somalia)'], // Afgooye (Somalia)
     ['HZD','ainelAbd1970BahrainIsland','raw:HZD','ain_el_abd_1970_(bahrain_island)'], // Ain el Abd 1970 (Bahrain Island)
     ['HZD','ainelAbd1970SaudiArabia','raw:HZD','ain_el_abd_1970_(saudi_arabia)'], // Ain el Abd 1970 (Saudi Arabia)
     ['HZD','americanSamoaDatum1962','raw:HZD','american_samoa_datum_1962'], // American Samoa Datum 1962
     ['HZD','amersfoort1885dash1903','raw:HZD','amersfoort_1885/1903_(netherlands)'], // Amersfoort 1885/1903 (Netherlands)
     ['HZD','anna1Astro1965CocosIslands','raw:HZD','anna_1_astro_1965_(cocos_islands)'], // Anna 1 Astro 1965 (Cocos Islands)
     ['HZD','antiguaIslandAstro1943','raw:HZD','antigua_island_astro_1943'], // Antigua Island Astro 1943
     ['HZD','approximateLuzonDatum','raw:HZD','approximate_luzon_datum_(philippines)'], // Approximate Luzon Datum (Philippines)
     ['HZD','arc1935Africa','raw:HZD','arc_1935_(africa)'], // Arc 1935 (Africa)
     ['HZD','arc1950Botswana','raw:HZD','arc_1950_(botswana)'], // Arc 1950 (Botswana)
     ['HZD','arc1950Burundi','raw:HZD','arc_1950_(burundi)'], // Arc 1950 (Burundi)
     ['HZD','arc1950Lesotho','raw:HZD','arc_1950_(lesotho)'], // Arc 1950 (Lesotho)
     ['HZD','arc1950Malawi','raw:HZD','arc_1950_(malawi)'], // Arc 1950 (Malawi)
     ['HZD','arc1950MeanValue','raw:HZD','arc_1950_(mean_value)'], // Arc 1950 (mean value)
     ['HZD','arc1950Swaziland','raw:HZD','arc_1950_(swaziland)'], // Arc 1950 (Swaziland)
     ['HZD','arc1950Zaire','raw:HZD','arc_1950_(zaire)'], // Arc 1950 (Zaire)
     ['HZD','arc1950Zambia','raw:HZD','arc_1950_(zambia)'], // Arc 1950 (Zambia)
     ['HZD','arc1950Zimbabwe','raw:HZD','arc_1950_(zimbabwe)'], // Arc 1950 (Zimbabwe)
     ['HZD','arc1960Kenya','raw:HZD','arc_1960_(kenya)'], // Arc 1960 (Kenya)
     ['HZD','arc1960MeanValue','raw:HZD','arc_1960_(mean_value)'], // Arc 1960 (mean value)
     ['HZD','arc1960Tanzania','raw:HZD','arc_1960_(tanzania)'], // Arc 1960 (Tanzania)
     ['HZD','ascensionIsland1958','raw:HZD','ascension_island_1958_(ascension_island)'], // Ascension Island 1958 (Ascension Island)
     ['HZD','astroBeaconEIwoJimaIsland','raw:HZD','astro_beacon_'e'_(iwo_jima_island)'], // Astro Beacon 'E' (Iwo Jima Island)
     ['HZD','astroDos71dash4StHelena','raw:HZD','astro_dos_71/4_(st._helena_island)'], // Astro DOS 71/4 (St. Helena Island)
     ['HZD','astroStation1952MarcusIs','raw:HZD','astro_station_1952_(marcus_island)'], // Astro Station 1952 (Marcus Island)
     ['HZD','astroTernIsland1961Hawaii','raw:HZD','astro_tern_island_1961_(tern_island,_hawaii)'], // Astro Tern Island 1961 (Tern Island, Hawaii)
     ['HZD','australianGeodetic1966','raw:HZD','australian_geod._1966_(australia_and_tasmania_island)'], // Australian Geod. 1966 (Australia and Tasmania Island)
     ['HZD','australianGeodetic1984','raw:HZD','australian_geod._1984_(australia_and_tasmania_island)'], // Australian Geod. 1984 (Australia and Tasmania Island)
     ['HZD','averageTerrestrialSys1977','raw:HZD','average_terrestrial_system_1977,_new_brunswick'], // Average Terrestrial System 1977, New Brunswick
     ['HZD','ayabelleLighthouseDjibouti','raw:HZD','ayabelle_lighthouse_(djibouti)'], // Ayabelle Lighthouse (Djibouti)
     ['HZD','bekaaBaseSouthEndLebanon','raw:HZD','bekaa_base_south_end_(lebanon)'], // Bekaa Base South End (Lebanon)
     ['HZD','belgium1950SystemLommelSig','raw:HZD','belgium_1950_system_(lommel_signal,_belgium)'], // Belgium 1950 System (Lommel Signal, Belgium)
     ['HZD','belgium1972Observatoire','raw:HZD','belgium_1972_(observatoire_d'uccle)'], // Belgium 1972 (Observatoire d'Uccle)
     ['HZD','bellevueIgnEfateErromango','raw:HZD','bellevue_(ign)_(efate_and_erromango_islands)'], // Bellevue (IGN) (Efate and Erromango Islands)
     ['HZD','bermuda1957BermudaIslands','raw:HZD','bermuda_1957_(bermuda_islands)'], // Bermuda 1957 (Bermuda Islands)
     ['HZD','bern1898Switzerland','raw:HZD','bern_1898_(switzerland)'], // Bern 1898 (Switzerland)
     ['HZD','bern1898ZeroMeridian','raw:HZD','bern_1898_(switzerland)_with_zero_meridian_bern'], // Bern 1898 (Switzerland) with Zero Meridian Bern
     ['HZD','bissauGuineaBissau','raw:HZD','bissau_(guinea-bissau)'], // Bissau (Guinea-Bissau)
     ['HZD','bjz54A954BeijingCoord','raw:HZD','bjz54_(a954_beijing_coordinates)_(china)'], // BJZ54 (A954 Beijing Coordinates) (China)
     ['HZD','bogotaObsZeroMeridian','raw:HZD','bogota_observatory_(colombia)_with_zero_meridian_bogota'], // Bogota Observatory (Colombia) with Zero Meridian Bogota
     ['HZD','bogotaObservatoryColombia','raw:HZD','bogota_observatory_(colombia)'], // Bogota Observatory (Colombia)
     ['HZD','bukitRimpahIndonesia','raw:HZD','bukit_rimpah_(bangka_and_belitung_islands,_indonesia)'], // Bukit Rimpah (Bangka and Belitung Islands, Indonesia)
     ['HZD','camacupaBaseSwEndAngola','raw:HZD','camacupa_base_sw_end_(campo_de_aviacao,_angola)'], // Camacupa Base SW End (Campo De Aviacao, Angola)
     ['HZD','campAreaAstroAntarctica','raw:HZD','camp_area_astro_(camp_mcmurdo_area,_antarctica)'], // Camp Area Astro (Camp McMurdo Area, Antarctica)
     ['HZD','campoInchauspeArgentina','raw:HZD','campo_inchauspe_(argentina)'], // Campo Inchauspe (Argentina)
     ['HZD','cantonAstro1966PhoenixIs','raw:HZD','canton_astro_1966_(phoenix_islands)'], // Canton Astro 1966 (Phoenix Islands)
     ['HZD','capeCanaveralMeanValue','raw:HZD','cape_canaveral_(mean_value)'], // Cape Canaveral (mean value)
     ['HZD','capeSouthAfrica','raw:HZD','cape_(south_africa)'], // Cape (South Africa)
     ['HZD','carthageTunisia','raw:HZD','carthage_(tunisia)'], // Carthage (Tunisia)
     ['HZD','chatham1971NewZealand','raw:HZD','chatham_1971_(chatham_island,_new_zealand)'], // Chatham 1971 (Chatham Island, New Zealand)
     ['HZD','chuaAstroParaguay','raw:HZD','chua_astro_(paraguay)'], // Chua Astro (Paraguay)
     ['HZD','compensationGeoQuebec1977','raw:HZD','compensation_geodetique_du_quebec_1977'], // Compensation Geodetique du Quebec 1977
     ['HZD','conakryPyramidGuinea','raw:HZD','conakry_pyramid_of_the_service_geographique_(guinea)'], // Conakry Pyramid of the Service Geographique (Guinea)
     ['HZD','corregoAlegreBrazil','raw:HZD','corrego_alegre_(brazil)'], // Corrego Alegre (Brazil)
     ['HZD','dabolaGuinea','raw:HZD','dabola_(guinea)'], // Dabola (Guinea)
     ['HZD','dcs3LighthouseLesserAnt','raw:HZD','dcs-3_lighthouse,_saint_lucia,_lesser_antilles'], // DCS-3 Lighthouse, Saint Lucia, Lesser Antilles
     ['HZD','deceptionIslAntarctica','raw:HZD','deception_island,_antarctica'], // Deception Island, Antarctica
     ['HZD','djakartaBataviaIndonesia','raw:HZD','djakarta_(batavia)_(sumatra_island,_indonesia)'], // Djakarta (Batavia) (Sumatra Island, Indonesia)
     ['HZD','djakartaBataviaZeroMerid','raw:HZD','djakarta_(batavia)_(sumatra_island,_indonesia)_with_zero_meridian_djakarta'], // Djakarta (Batavia) (Sumatra Island, Indonesia) with Zero Meridian Djakarta
     ['HZD','dominicaAstroM12LesserAnt','raw:HZD','dominica_astro_m-12,_dominica,_lesser_antilles'], // Dominica Astro M-12, Dominica, Lesser Antilles
     ['HZD','dos1968GizoNewGeorgiaIs','raw:HZD','dos_1968_(gizo_island,_new_georgia_islands)'], // DOS 1968 (Gizo Island, New Georgia Islands)
     ['HZD','easterIsland1967EasterIs','raw:HZD','easter_island_1967_(easter_island)'], // Easter Island 1967 (Easter Island)
     ['HZD','estonia1937','raw:HZD','co-ordinate_system_1937_of_estonia'], // Co-ordinate System 1937 of Estonia
     ['HZD','etrs1989','raw:HZD','european_terrestrial_reference_system_1989_(etrs89)'], // European Terrestrial Reference System 1989 (ETRS89)
     ['HZD','european1950BritishIsles','raw:HZD','european_1950_(british_isles)'], // European 1950 (British Isles)
     ['HZD','european1950Cyprus','raw:HZD','european_1950_(cyprus)'], // European 1950 (Cyprus)
     ['HZD','european1950Egypt','raw:HZD','european_1950_(egypt)'], // European 1950 (Egypt)
     ['HZD','european1950England','raw:HZD','european_1950_(england,_channel_islands,_scotland,_and_shetland_islands)'], // European 1950 (England, Channel Islands, Scotland, and Shetland Islands)
     ['HZD','european1950Greece','raw:HZD','european_1950_(greece)'], // European 1950 (Greece)
     ['HZD','european1950Iran','raw:HZD','european_1950_(iran)'], // European 1950 (Iran)
     ['HZD','european1950IraqSyria','raw:HZD','european_1950_(iraq,_israel,_jordan,_kuwait,_lebanon,_saudi_arabia,_and_syria)'], // European 1950 (Iraq, Israel, Jordan, Kuwait, Lebanon, Saudi Arabia, and Syria)
     ['HZD','european1950Malta','raw:HZD','european_1950_(malta)'], // European 1950 (Malta)
     ['HZD','european1950MeanValue','raw:HZD','european_1950_(mean_value)'], // European 1950 (mean value)
     ['HZD','european1950NorwayFinland','raw:HZD','european_1950_(norway_and_finland)'], // European 1950 (Norway and Finland)
     ['HZD','european1950PortugalSpain','raw:HZD','european_1950_(portugal_and_spain)'], // European 1950 (Portugal and Spain)
     ['HZD','european1950Sardinia','raw:HZD','european_1950_(sardinia)'], // European 1950 (Sardinia)
     ['HZD','european1950Sicily','raw:HZD','european_1950_(sicily)'], // European 1950 (Sicily)
     ['HZD','european1950Tunisia','raw:HZD','european_1950_(tunisia)'], // European 1950 (Tunisia)
     ['HZD','european1950WesternEurope','raw:HZD','european_1950_(western_europe)'], // European 1950 (Western Europe)
     ['HZD','european1979MeanValue','raw:HZD','european_1979_(mean_value)'], // European 1979 (mean value)
     ['HZD','fortThomas1955LeewardIs','raw:HZD','fort_thomas_1955_(nevis,_st_kitts,_leeward_islands)'], // Fort Thomas 1955 (Nevis, St Kitts, Leeward Islands)
     ['HZD','gan1970AdduAtoll','raw:HZD','gan_1970_(addu_atoll,_republic_of_maldives)'], // Gan 1970 (Addu Atoll, Republic of Maldives)
     ['HZD','gandajikaBaseZaire','raw:HZD','gandajika_base_(zaire)'], // Gandajika Base (Zaire)
     ['HZD','gdz80China','raw:HZD','gdz80_(china)'], // GDZ80 (China)
     ['HZD','geocentricDatumOfAustralia','raw:HZD','geocentric_datum_of_australia_(gda)'], // Geocentric Datum of Australia (GDA)
     ['HZD','geodeticDatum1949Zealand','raw:HZD','geodetic_datum_1949_(new_zealand)'], // Geodetic Datum 1949 (New Zealand)
     ['HZD','graciosaBaseSWFaialAzores','raw:HZD','graciosa_base_sw_(faial,_graciosa,_pico,_sao_jorge,_and_terceira_island,_azores)'], // Graciosa Base SW (Faial, Graciosa, Pico, Sao Jorge, and Terceira Island, Azores)
     ['HZD','greekDatumGreece','raw:HZD','greek_datum,_greece'], // Greek Datum, Greece
     ['HZD','greekGeodeticRefSystem1987','raw:HZD','greek_geodetic_reference_system_1987_(ggrs_87)'], // Greek Geodetic Reference System 1987 (GGRS 87)
     ['HZD','guam1963','raw:HZD','guam_1963'], // Guam 1963
     ['HZD','gunongSegaraKalimantanIs','raw:HZD','gunong_segara_(kalimantan_island,_indonesia)'], // Gunong Segara (Kalimantan Island, Indonesia)
     ['HZD','gunongSerindung','raw:HZD','gunong_serindung'], // Gunong Serindung
     ['HZD','gux1AstroGuadacanalIs','raw:HZD','gux_1_astro_(guadacanal_island)'], // GUX 1 Astro (Guadacanal Island)
     ['HZD','guyanaCSG67','raw:HZD','guyana_csg67'], // Guyana CSG67
     ['HZD','heratNorthAfganistan','raw:HZD','herat_north_(afganistan)'], // Herat North (Afganistan)
     ['HZD','hermannskogel','raw:HZD','hermannskogel'], // Hermannskogel
     ['HZD','hjorsey1955Iceland','raw:HZD','hjorsey_1955_(iceland)'], // Hjorsey 1955 (Iceland)
     ['HZD','hongKong1929','raw:HZD','hong_kong_1929'], // Hong Kong 1929
     ['HZD','hongKong1963HongKong','raw:HZD','hong_kong_1963_(hong_kong)'], // Hong Kong 1963 (Hong Kong)
     ['HZD','huTzuShan','raw:HZD','hu-tzu-shan'], // Hu-Tzu-Shan
     ['HZD','hungarian1972','raw:HZD','hungarian_1972'], // Hungarian 1972
     ['HZD','indian1954Thailand','raw:HZD','indian_1954_(thailand)'], // Indian 1954 (Thailand)
     ['HZD','indian1960ConSonIsland','raw:HZD','indian_1960_(con_son_island_(vietnam))'], // Indian 1960 (Con Son Island (Vietnam))
     ['HZD','indian1960Vietnam','raw:HZD','indian_1960_(vietnam:_near_16_degrees_north)'], // Indian 1960 (Vietnam: near 16 degrees North)
     ['HZD','indian1975Thailand','raw:HZD','indian_1975_(thailand)'], // Indian 1975 (Thailand)
     ['HZD','indian1975ThailandCycle1','raw:HZD','indian_1975_(thailand)_-_cycle_1'], // Indian 1975 (Thailand) - Cycle 1
     ['HZD','indianBangladesh','raw:HZD','indian_(bangladesh)'], // Indian (Bangladesh)
     ['HZD','indianIndiaNepal','raw:HZD','indian_(india_and_nepal)'], // Indian (India and Nepal)
     ['HZD','indianPakistan','raw:HZD','indian_(pakistan)'], // Indian (Pakistan)
     ['HZD','indianThailandVietnam','raw:HZD','indian_(thailand_and_vietnam)'], // Indian (Thailand and Vietnam)
     ['HZD','indonesian1974','raw:HZD','indonesian_1974'], // Indonesian 1974
     ['HZD','ireland1965IrelandNorthern','raw:HZD','ireland_1965_(ireland_and_northern_ireland)'], // Ireland 1965 (Ireland and Northern Ireland)
     ['HZD','ists061Astro1968GeorgiaIs','raw:HZD','ists_061_astro_1968_(south_georgia_islands)'], // ISTS 061 Astro 1968 (South Georgia Islands)
     ['HZD','ists073Astro1969DiegoGar','raw:HZD','ists_073_astro_1969_(diego_garcia)'], // ISTS 073 Astro 1969 (Diego Garcia)
     ['HZD','johnstonIsland1961','raw:HZD','johnston_island_1961_(johnston_island)'], // Johnston Island 1961 (Johnston Island)
     ['HZD','kalianpurIndia','raw:HZD','kalianpur_(india)'], // Kalianpur (India)
     ['HZD','kandawalaSriLanka','raw:HZD','kandawala_(sri_lanka)'], // Kandawala (Sri Lanka)
     ['HZD','kcs2SierraLeone','raw:HZD','kcs_2,_sierra_leone'], // KCS 2, Sierra Leone
     ['HZD','kerguelenIsland1949','raw:HZD','kerguelen_island_1949_(kerguelen_island)'], // Kerguelen Island 1949 (Kerguelen Island)
     ['HZD','kertau1948RevisedMalaysia','raw:HZD','kertau_1948_(or_revised_kertau)_(west_malaysia_and_singapore)'], // Kertau 1948 (or Revised Kertau) (West Malaysia and Singapore)
     ['HZD','kkjFinland','raw:HZD','kkj_(or_kartastokoordinaattijarjestelma),_finland'], // KKJ (or Kartastokoordinaattijarjestelma), Finland
     ['HZD','koreanGeodeticSystem1995','raw:HZD','korean_geodetic_system_1995_(south_korea)'], // Korean Geodetic System 1995 (South Korea)
     ['HZD','kusaieAstro1951','raw:HZD','kusaie_astro_1951'], // Kusaie Astro 1951
     ['HZD','kuwaitOilCompanyK28','raw:HZD','kuwait_oil_company_(k28)'], // Kuwait Oil Company (K28)
     ['HZD','lc5Astro1961CaymanBracIs','raw:HZD','l.c._5_astro_1961_(cayman_brac_island)'], // L.C. 5 Astro 1961 (Cayman Brac Island)
     ['HZD','leigonGhana','raw:HZD','leigon_(ghana)'], // Leigon (Ghana)
     ['HZD','liberia1964Liberia','raw:HZD','liberia_1964_(liberia)'], // Liberia 1964 (Liberia)
     ['HZD','lisbonCastelodiSaoJorge','raw:HZD','lisbon_(castelo_di_sao_jorge),_portugal'], // Lisbon (Castelo di Sao Jorge), Portugal
     ['HZD','localAstro','raw:HZD','local_astro'], // Local Astro
     ['HZD','lomaQuintanaVenezuela','raw:HZD','loma_quintana_(venezuela)'], // Loma Quintana (Venezuela)
     ['HZD','luzonMindanaoIsland','raw:HZD','luzon_(mindanao_island)'], // Luzon (Mindanao Island)
     ['HZD','luzonPhilipinesNotMindanao','raw:HZD','luzon_(philipines_except_mindanao_island)'], // Luzon (Philipines except Mindanao Island)
     ['HZD','mPoralokoGabon','raw:HZD','m'poraloko_(gabon)'], // M'Poraloko (Gabon)
     ['HZD','mahe1971MaheIsland','raw:HZD','mahe_1971_(mahe_island)'], // Mahe 1971 (Mahe Island)
     ['HZD','manokwariWestIrian','raw:HZD','manokwari_(west_irian)'], // Manokwari (West Irian)
     ['HZD','marcoAstroSalvageIslands','raw:HZD','marco_astro_(salvage_islands)'], // Marco Astro (Salvage Islands)
     ['HZD','martiniqueFortDesaix','raw:HZD','martinique_fort-desaix'], // Martinique Fort-Desaix
     ['HZD','massawEritreaEthiopia','raw:HZD','massawa_(eritrea,_ethiopia)'], // Massawa (Eritrea, Ethiopia)
     ['HZD','mayotteCombani','raw:HZD','mayotte_combani'], // Mayotte Combani
     ['HZD','merchichMorocco','raw:HZD','merchich_(morocco)'], // Merchich (Morocco)
     ['HZD','midwayAstro1961MidwayIs','raw:HZD','midway_astro_1961_(midway_island)'], // Midway Astro 1961 (Midway Island)
     ['HZD','minnaCameroon','raw:HZD','minna_(cameroon)'], // Minna (Cameroon)
     ['HZD','minnaNigeria','raw:HZD','minna_(nigeria)'], // Minna (Nigeria)
     ['HZD','modifiedBJZ54China','raw:HZD','modified_bjz54_(china)'], // Modified BJZ54 (China)
     ['HZD','montjongLowe','raw:HZD','montjong_lowe'], // Montjong Lowe
     ['HZD','montserratIslandAstro1958','raw:HZD','montserrat_island_astro_1958'], // Montserrat Island Astro 1958
     ['HZD','mountDillonTobago','raw:HZD','mount_dillon,_tobago'], // Mount Dillon, Tobago
     ['HZD','nahrwanMasirahIslandOman','raw:HZD','nahrwan_(masirah_island,_oman)'], // Nahrwan (Masirah Island, Oman)
     ['HZD','nahrwanSaudiArabia','raw:HZD','nahrwan_(saudi_arabia)'], // Nahrwan (Saudi Arabia)
     ['HZD','nahrwanUnitedArabEmirates','raw:HZD','nahrwan_(united_arab_emirates)'], // Nahrwan (United Arab Emirates)
     ['HZD','naparimaBwiTrinidadTobago','raw:HZD','naparima_(bwi,_trinidad_and_tobago)'], // Naparima (BWI, Trinidad and Tobago)
     ['HZD','newFrenchZeroMeridianParis','raw:HZD','new_french_or_nouvelle_triangulation_francaise_(ntf)_with_zero_meridian_paris'], // New French or Nouvelle Triangulation Francaise (NTF) with Zero Meridian Paris
     ['HZD','noInformation','raw:HZD','noinformation'], // noInformation
     ['HZD','northAm1927Alaska','raw:HZD','north_american_1927_(alaska)'], // North American 1927 (Alaska)
     ['HZD','northAm1927Alberta','raw:HZD','north_american_1927_(alberta_and_british_columbia)'], // North American 1927 (Alberta and British Columbia)
     ['HZD','northAm1927AleutianE','raw:HZD','north_american_1927_(aleutian_islands_east_of_180_degrees_west)'], // North American 1927 (Aleutian Islands East of 180 degrees West)
     ['HZD','northAm1927AleutianW','raw:HZD','north_american_1927_(aleutian_islands_west_of_180_degrees_west)'], // North American 1927 (Aleutian Islands West of 180 degrees West)
     ['HZD','northAm1927Bahamas','raw:HZD','north_american_1927_(bahamas,_except_san_salvador_island)'], // North American 1927 (Bahamas, except San Salvador Island)
     ['HZD','northAm1927CanadMean','raw:HZD','north_american_1927_(canada_mean)'], // North American 1927 (Canada mean)
     ['HZD','northAm1927CanalZone','raw:HZD','north_american_1927_(canal_zone)'], // North American 1927 (Canal Zone)
     ['HZD','northAm1927Caribbean','raw:HZD','north_american_1927_(caribbean)'], // North American 1927 (Caribbean)
     ['HZD','northAm1927CentAmer','raw:HZD','north_american_1927_(central_america)'], // North American 1927 (Central America)
     ['HZD','northAm1927ConusMean','raw:HZD','north_american_1927_(conus_mean)'], // North American 1927 (CONUS mean)
     ['HZD','northAm1927Cuba','raw:HZD','north_american_1927_(cuba)'], // North American 1927 (Cuba)
     ['HZD','northAm1927EasternUs','raw:HZD','north_american_1927_(eastern_us)'], // North American 1927 (Eastern US)
     ['HZD','northAm1927HayesPen','raw:HZD','north_american_1927_(hayes_peninsula,_greenland)'], // North American 1927 (Hayes Peninsula, Greenland)
     ['HZD','northAm1927Manitoba','raw:HZD','north_american_1927_(manitoba_and_ontario)'], // North American 1927 (Manitoba and Ontario)
     ['HZD','northAm1927Mexico','raw:HZD','north_american_1927_(mexico)'], // North American 1927 (Mexico)
     ['HZD','northAm1927Newfound','raw:HZD','north_american_1927_(newfoundland,_new_brunswick,_nova_scotia_and_quebec)'], // North American 1927 (Newfoundland, New Brunswick, Nova Scotia and Quebec)
     ['HZD','northAm1927NwTerSaskat','raw:HZD','north_american_1927_(northwest_territories_and_saskatchewan)'], // North American 1927 (Northwest Territories and Saskatchewan)
     ['HZD','northAm1927Salvador','raw:HZD','north_american_1927_(san_salvador_island)'], // North American 1927 (San Salvador Island)
     ['HZD','northAm1927WesternUs','raw:HZD','north_american_1927_(western_us)'], // North American 1927 (Western US)
     ['HZD','northAm1927Yukon','raw:HZD','north_american_1927_(yukon)'], // North American 1927 (Yukon)
     ['HZD','northAm1983AlaskaExAleut','raw:HZD','north_american_1983_(alaska,_excluding_aleutian_islands)'], // North American 1983 (Alaska, excluding Aleutian Islands)
     ['HZD','northAm1983Aleutian','raw:HZD','north_american_1983_(aleutian_islands)'], // North American 1983 (Aleutian Islands)
     ['HZD','northAm1983Canada','raw:HZD','north_american_1983_(canada)'], // North American 1983 (Canada)
     ['HZD','northAm1983Conus','raw:HZD','north_american_1983_(conus)'], // North American 1983 (CONUS)
     ['HZD','northAm1983Hawaii','raw:HZD','north_american_1983_(hawaii)'], // North American 1983 (Hawaii)
     ['HZD','northAm1983Mexico','raw:HZD','north_american_1983_(mexico_and_central_america))'], // North American 1983 (Mexico and Central America))
     ['HZD','northSahara1959','raw:HZD','north_sahara_1959'], // North Sahara 1959
     ['HZD','observMeteorologico1939','raw:HZD','observatorio_meteorologico_1939_(corvo_and_flores_islands,_azores)'], // Observatorio Meteorologico 1939 (Corvo and Flores Islands, Azores)
     ['HZD','ocotopequeGuatemala','raw:HZD','ocotopeque,_guatemala'], // Ocotopeque, Guatemala
     ['HZD','oldEgyptian','raw:HZD','old_egyptian_(egypt)'], // Old Egyptian (Egypt)
     ['HZD','oldHawaiianHawaiiIsland','raw:HZD','old_hawaiian_(hawaii)'], // Old Hawaiian (Hawaii)
     ['HZD','oldHawaiianKauaiIsland','raw:HZD','old_hawaiian_(kauai)'], // Old Hawaiian (Kauai)
     ['HZD','oldHawaiianMauiIsland','raw:HZD','old_hawaiian_(maui)'], // Old Hawaiian (Maui)
     ['HZD','oldHawaiianMeanValue','raw:HZD','old_hawaiian_(mean_value)'], // Old Hawaiian (mean value)
     ['HZD','oldHawaiianOahuIsland','raw:HZD','old_hawaiian_(oahu)'], // Old Hawaiian (Oahu)
     ['HZD','oman','raw:HZD','oman_(oman)'], // Oman (Oman)
     ['HZD','ordnanceSurvGB1936England','raw:HZD','ordnance_survey_g.b._1936_(england)'], // Ordnance Survey G.B. 1936 (England)
     ['HZD','ordnanceSurvGB1936MeanVal','raw:HZD','ordnance_survey_g.b._1936_(mean_value)'], // Ordnance Survey G.B. 1936 (mean value)
     ['HZD','ordnanceSurvGB1936ScotShet','raw:HZD','ordnance_survey_g.b._1936_(scotland_and_shetland_islands)'], // Ordnance Survey G.B. 1936 (Scotland and Shetland Islands)
     ['HZD','ordnanceSurvGB1936ScotWale','raw:HZD','ordnance_survey_g.b._1936_(england,_isle_of_man,_and_wales)'], // Ordnance Survey G.B. 1936 (England, Isle of Man, and Wales)
     ['HZD','ordnanceSurvGB1936Wales','raw:HZD','ordnance_survey_g.b._1936_(wales)'], // Ordnance Survey G.B. 1936 (Wales)
     ['HZD','osloObservatoryOld','raw:HZD','oslo_observatory_(old),_norway'], // Oslo Observatory (Old), Norway
     ['HZD','other','raw:HZD','other'], // Other
     ['HZD','padangBaseWestEnd','raw:HZD','padang_base_west_end_(sumatra,_indonesia)'], // Padang Base West End (Sumatra, Indonesia)
     ['HZD','padangBaseWestEndZeroMerid','raw:HZD','padang_base_west_end_(sumatra,_indonesia)_with_zero_meridian_djakarta'], // Padang Base West End (Sumatra, Indonesia) with Zero Meridian Djakarta
     ['HZD','palestine1928','raw:HZD','palestine_1928_(israel,_jordan)'], // Palestine 1928 (Israel, Jordan)
     ['HZD','picoDeLasNievesCanaryIs','raw:HZD','pico_de_las_nieves_(canary_islands)'], // Pico de las Nieves (Canary Islands)
     ['HZD','pitcairnAstro1967','raw:HZD','pitcairn_astro_1967_(pitcairn_island)'], // Pitcairn Astro 1967 (Pitcairn Island)
     ['HZD','point58MeanSolution','raw:HZD','point_58_mean_solution_(burkina_faso_and_niger)'], // Point 58 Mean Solution (Burkina Faso and Niger)
     ['HZD','pointeNoire1948','raw:HZD','pointe_noire_1948'], // Pointe Noire 1948
     ['HZD','potsdamHelmertturmGermany','raw:HZD','potsdam_or_helmertturm_(germany)'], // Potsdam or Helmertturm (Germany)
     ['HZD','provSouthAm1956Bolivia','raw:HZD','prov._s._american_1956_(bolivia)'], // Prov. S. American 1956 (Bolivia)
     ['HZD','provSouthAm1956Columbia','raw:HZD','prov._s._american_1956_(columbia)'], // Prov. S. American 1956 (Columbia)
     ['HZD','provSouthAm1956Ecuador','raw:HZD','prov._s._american_1956_(ecuador)'], // Prov. S. American 1956 (Ecuador)
     ['HZD','provSouthAm1956Guyana','raw:HZD','prov._s._american_1956_(guyana)'], // Prov. S. American 1956 (Guyana)
     ['HZD','provSouthAm1956MeanValue','raw:HZD','prov._s._american_1956_(mean_value)'], // Prov. S. American 1956 (mean value)
     ['HZD','provSouthAm1956NChile','raw:HZD','prov._s._american_1956_(northern_chile_near_19_degrees_south)'], // Prov. S. American 1956 (Northern Chile near 19 degrees South)
     ['HZD','provSouthAm1956Peru','raw:HZD','prov._s._american_1956_(peru)'], // Prov. S. American 1956 (Peru)
     ['HZD','provSouthAm1956SChile','raw:HZD','prov._s._american_1956_(southern_chile_near_43_degrees_south)'], // Prov. S. American 1956 (Southern Chile near 43 degrees South)
     ['HZD','provSouthAm1956Venezuela','raw:HZD','prov._s._american_1956_(venezuela)'], // Prov. S. American 1956 (Venezuela)
     ['HZD','provSouthChilean1963','raw:HZD','provisional_south_chilean_1963_(or_hito_xviii_1963)_(s._chile,_53_degrees_south)'], // Provisional South Chilean 1963 (or Hito XVIII 1963) (S. Chile, 53 degrees South)
     ['HZD','puertoRicoVirginIslands','raw:HZD','puerto_rico_(puerto_rico_and_virgin_islands)'], // Puerto Rico (Puerto Rico and Virgin Islands)
     ['HZD','pulkovo1942Albania','raw:HZD','pulkovo_1942_(albania)'], // Pulkovo 1942 (Albania)
     ['HZD','pulkovo1942Czechoslovakia','raw:HZD','pulkovo_1942_(czechoslovakia)'], // Pulkovo 1942 (Czechoslovakia)
     ['HZD','pulkovo1942Hungary','raw:HZD','pulkovo_1942_(hungary)'], // Pulkovo 1942 (Hungary)
     ['HZD','pulkovo1942Kazakhstan','raw:HZD','pulkovo_1942_(kazakhstan)'], // Pulkovo 1942 (Kazakhstan)
     ['HZD','pulkovo1942Latvia','raw:HZD','pulkovo_1942_(latvia)'], // Pulkovo 1942 (Latvia)
     ['HZD','pulkovo1942Poland','raw:HZD','pulkovo_1942_(poland)'], // Pulkovo 1942 (Poland)
     ['HZD','pulkovo1942Romania','raw:HZD','pulkovo_1942_(romania)'], // Pulkovo 1942 (Romania)
     ['HZD','pulkovo1942Russia','raw:HZD','pulkovo_1942_(russia)'], // Pulkovo 1942 (Russia)
     ['HZD','qatarNationalQatar','raw:HZD','qatar_national_(qatar)'], // Qatar National (Qatar)
     ['HZD','qornoqSouthGreenland','raw:HZD','qornoq_(south_greenland)'], // Qornoq (South Greenland)
     ['HZD','rauenbergBerlinGermany','raw:HZD','rauenberg_(berlin,_germany)'], // Rauenberg (Berlin, Germany)
     ['HZD','reconTriangulationMorocco','raw:HZD','reconnaissance_triangulation,_morocco'], // Reconnaissance Triangulation, Morocco
     ['HZD','reunion1947','raw:HZD','reunion_1947'], // Reunion 1947
     ['HZD','revisedNahrwan','raw:HZD','revised_nahrwan'], // Revised Nahrwan
     ['HZD','rome1940','raw:HZD','rome_1940_(or_monte_mario_1940),_italy'], // Rome 1940 (or Monte Mario 1940), Italy
     ['HZD','rome1940ZeroMeridianRome','raw:HZD','rome_1940_(or_monte_mario_1940),_italy,_with_zero_meridian_rome'], // Rome 1940 (or Monte Mario 1940), Italy, with Zero Meridian Rome
     ['HZD','rt90StockholmSweden','raw:HZD','rt90,_stockholm,_sweden'], // RT90, Stockholm, Sweden
     ['HZD','sJtsk','raw:HZD','s-jtsk'], // S-JTSK
     ['HZD','sainteAnneI1984Guadeloupe','raw:HZD','sainte_anne_i_1984_(guadeloupe)'], // Sainte Anne I 1984 (Guadeloupe)
     ['HZD','santoDos1965EspiritoSanto','raw:HZD','santo_(dos)_1965_(espirito_santo_island)'], // Santo (DOS) 1965 (Espirito Santo Island)
     ['HZD','saoBrazSaoMiguelAzores','raw:HZD','sao_braz_(sao_miguel,_santa_maria_islands,_azores)'], // Sao Braz (Sao Miguel, Santa Maria Islands, Azores)
     ['HZD','sapperHill1943EastFalkland','raw:HZD','sapper_hill_1943_(east_falkland_islands)'], // Sapper Hill 1943 (East Falkland Islands)
     ['HZD','schwarzeckNamibia','raw:HZD','schwarzeck_(namibia)'], // Schwarzeck (Namibia)
     ['HZD','seBasePortoSanto','raw:HZD','se_base_(porto_santo)_(porto_santo_and_madeira_islands)'], // SE Base (Porto Santo) (Porto Santo and Madeira Islands)
     ['HZD','selvagemGrande1938Salvage','raw:HZD','selvagem_grande_1938_(salvage_islands)'], // Selvagem Grande 1938 (Salvage Islands)
     ['HZD','sierraLeone1960','raw:HZD','sierra_leone_1960'], // Sierra Leone 1960
     ['HZD','sirgas','raw:HZD','south_american_geocentric_reference_system_(sirgas)'], // South American Geocentric Reference System (SIRGAS)
     ['HZD','southAfricanSouthAfrica','raw:HZD','south_african_(south_africa)'], // South African (South Africa)
     ['HZD','southAmerican1969Argentina','raw:HZD','south_american_1969_(argentina)'], // South American 1969 (Argentina)
     ['HZD','southAmerican1969BaltraIs','raw:HZD','south_american_1969_(baltra,_galapagos_islands)'], // South American 1969 (Baltra, Galapagos Islands)
     ['HZD','southAmerican1969Bolivia','raw:HZD','south_american_1969_(bolivia)'], // South American 1969 (Bolivia)
     ['HZD','southAmerican1969Brazil','raw:HZD','south_american_1969_(brazil)'], // South American 1969 (Brazil)
     ['HZD','southAmerican1969Chile','raw:HZD','south_american_1969_(chile)'], // South American 1969 (Chile)
     ['HZD','southAmerican1969Columbia','raw:HZD','south_american_1969_(columbia)'], // South American 1969 (Columbia)
     ['HZD','southAmerican1969Ecuador','raw:HZD','south_american_1969_(ecuador)'], // South American 1969 (Ecuador)
     ['HZD','southAmerican1969Guyana','raw:HZD','south_american_1969_(guyana)'], // South American 1969 (Guyana)
     ['HZD','southAmerican1969MeanValue','raw:HZD','south_american_1969_(mean_value)'], // South American 1969 (mean value)
     ['HZD','southAmerican1969Paraguay','raw:HZD','south_american_1969_(paraguay)'], // South American 1969 (Paraguay)
     ['HZD','southAmerican1969Peru','raw:HZD','south_american_1969_(peru)'], // South American 1969 (Peru)
     ['HZD','southAmerican1969Trinidad','raw:HZD','south_american_1969_(trinidad_and_tobago)'], // South American 1969 (Trinidad and Tobago)
     ['HZD','southAmerican1969Venezuela','raw:HZD','south_american_1969_(venezuela)'], // South American 1969 (Venezuela)
     ['HZD','southAsiaSingapore','raw:HZD','south_asia_(southeast_asia,_singapore)'], // South Asia (Southeast Asia, Singapore)
     ['HZD','sovietGeodeticSystem1985','raw:HZD','soviet_geodetic_system_1985'], // Soviet Geodetic System 1985
     ['HZD','sovietGeodeticSystem1990','raw:HZD','soviet_geodetic_system_1990'], // Soviet Geodetic System 1990
     ['HZD','stPierreetMiquelon1950','raw:HZD','st._pierre_et_miquelon_1950'], // St. Pierre et Miquelon 1950
     ['HZD','stockholm1938Sweden','raw:HZD','stockholm_1938_(sweden)'], // Stockholm 1938 (Sweden)
     ['HZD','sydneyObservatoryNewSouth','raw:HZD','sydney_observatory,_new_south_wales,_australia'], // Sydney Observatory, New South Wales, Australia
     ['HZD','tananariveObs1925ZerMerPar','raw:HZD','tananarive_observatory_1925,_with_zero_meridian_paris'], // Tananarive Observatory 1925, with Zero Meridian Paris
     ['HZD','tananariveObservatory1925','raw:HZD','tananarive_observatory_1925'], // Tananarive Observatory 1925
     ['HZD','timbalai1948BruneiMalaysia','raw:HZD','timbalai_1948_(brunei_and_east_malaysia_-_sarawak_and_sabah)'], // Timbalai 1948 (Brunei and East Malaysia - Sarawak and Sabah)
     ['HZD','timbalai1968','raw:HZD','timbalai_1968'], // Timbalai 1968
     ['HZD','tokyoJapan','raw:HZD','tokyo_(japan)'], // Tokyo (Japan)
     ['HZD','tokyoKorea','raw:HZD','tokyo_(korea)'], // Tokyo (Korea)
     ['HZD','tokyoKoreaCycle1','raw:HZD','tokyo_(korea)_-_cycle_1'], // Tokyo (Korea) - Cycle 1
     ['HZD','tokyoMeanValue','raw:HZD','tokyo_(mean_value)'], // Tokyo (mean value)
     ['HZD','tokyoOkinawa','raw:HZD','tokyo_(okinawa)'], // Tokyo (Okinawa)
     ['HZD','trinidad1903','raw:HZD','trinidad_1903'], // Trinidad 1903
     ['HZD','tristanAstro1968Cunha','raw:HZD','tristan_astro_1968_(tristan_da_cunha)'], // Tristan Astro 1968 (Tristan da Cunha)
     ['HZD','vitiLevu1916FijiIslands','raw:HZD','viti_levu_1916_(viti_levu_island,_fiji_islands)'], // Viti Levu 1916 (Viti Levu Island, Fiji Islands)
     ['HZD','voirol1875','raw:HZD','voirol_1875'], // Voirol 1875
     ['HZD','voirol1875ZeroMeridParis','raw:HZD','voirol_1875_with_zero_meridian_paris'], // Voirol 1875 with Zero Meridian Paris
     ['HZD','voirol1960Algeria','raw:HZD','voirol_1960,_algeria'], // Voirol 1960, Algeria
     ['HZD','voirol1960ZeroMeridParis','raw:HZD','voirol_1960,_algeria,_with_zero_meridian_paris'], // Voirol 1960, Algeria, with Zero Meridian Paris
     ['HZD','wakeEniwetok1960MarshallIs','raw:HZD','wake-eniwetok_1960_(marshall_islands)'], // Wake-Eniwetok 1960 (Marshall Islands)
     ['HZD','wakeIslandAstro1952','raw:HZD','wake_island_astro_1952'], // Wake Island Astro 1952
     ['HZD','worldGeodeticSystem1960','raw:HZD','world_geodetic_system_1960'], // World Geodetic System 1960
     ['HZD','worldGeodeticSystem1966','raw:HZD','world_geodetic_system_1966'], // World Geodetic System 1966
     ['HZD','worldGeodeticSystem1972','raw:HZD','world_geodetic_system_1972'], // World Geodetic System 1972
     ['HZD','worldGeodeticSystem1984','raw:HZD','world_geodetic_system_1984'], // World Geodetic System 1984
     ['HZD','yacareUruguay','raw:HZD','yacare_(uruguay)'], // Yacare (Uruguay)
     ['HZD','zanderijSurinam','raw:HZD','zanderij_(surinam)'], // Zanderij (Surinam)

     // INU - Inundation Type
     // ['INU','-999999',undefined,undefined], // noInformation
     ['INU','1','raw:INU','controlled'], // Controlled
     ['INU','2','raw:INU','natural'], // Natural

     // IWT - Inland Water Type
     // ['IWT','-999999',undefined,undefined], // noInformation
     ['IWT','1','raw:IWT','lake'], // Lake
     ['IWT','2','raw:IWT','pond'], // Pond
     ['IWT','3','raw:IWT','undifferentiated_water_body'], // Undifferentiated Water Body
     ['IWT','4','raw:IWT','reservoir'], // Reservoir
     ['IWT','5','raw:IWT','basin'], // Basin
     ['IWT','6','raw:IWT','water-hole'], // Water-hole
     ['IWT','7','raw:IWT','landlocked_sea'], // Landlocked Sea
     ['IWT','999','raw:IWT','other'], // Other

     // LMC - Navigation Landmark
     // ['LMC','-999999',undefined,undefined], // noInformation
     ['LMC','1000','raw:LMC','no'],
     ['LMC','1001','raw:LMC','yes'],

     // LOC - Vertical Relative Location
     // ['LOC','-999999',undefined,undefined], // noInformation
     ['LOC','17','raw:LOC','on_waterbody_bottom'], // On Waterbody Bottom
     ['LOC','23','raw:LOC','below_waterbody_bottom'], // Below Waterbody Bottom
     ['LOC','40','raw:LOC','below_ground_surface'], // Below Ground Surface
     ['LOC','44','raw:LOC','on_surface'], // On Surface
     ['LOC','45','raw:LOC','above_surface'], // Above Surface
     ['LOC','46','raw:LOC','above_waterbody_bottom'], // Above Waterbody Bottom
     ['LOC','47','raw:LOC','below_waterbody_surface'], // Below Waterbody Surface

     // LUN - Located Underground
     // ['LUN','-999999',undefined,undefined], // noInformation
     ['LUN','1000','raw:LUN','no'],
     ['LUN','1001','raw:LUN','yes'],

     // MAN - Maritime Navigation Marked
     // ['MAN','-999999',undefined,undefined], // noInformation
     ['MAN','1000','raw:MAN','no'],
     ['MAN','1001','raw:MAN','yes'],

     // MCC - Structural Material Type
     // ['MCC','-999999',undefined,undefined], // noInformation
     ['MCC','5','raw:MCC','asphalt'], // Asphalt
     ['MCC','7','raw:MCC','bedrock'], // Bedrock
     ['MCC','8','raw:MCC','boulders'], // Boulders
     ['MCC','9','raw:MCC','brick'], // Brick
     ['MCC','21','raw:MCC','concrete'], // Concrete
     ['MCC','46','raw:MCC','gravel'], // Gravel
     ['MCC','51','raw:MCC','iron'], // Iron
     ['MCC','52','raw:MCC','lava'], // Lava
     ['MCC','55','raw:MCC','loess'], // Loess
     ['MCC','62','raw:MCC','masonry'], // Masonry
     ['MCC','64','raw:MCC','metal'], // Metal
     ['MCC','65','raw:MCC','mud'], // Mud
     ['MCC','77','raw:MCC','prestressed_concrete'], // Prestressed Concrete
     ['MCC','83','raw:MCC','reinforced_concrete'], // Reinforced Concrete
     ['MCC','84','raw:MCC','rock/rocky'], // Rock/Rocky
     ['MCC','86','raw:MCC','rubble'], // Rubble
     ['MCC','88','raw:MCC','sand'], // Sand
     ['MCC','103','raw:MCC','frozen_water'], // Frozen Water
     ['MCC','104','raw:MCC','soil'], // Soil
     ['MCC','107','raw:MCC','steel'], // Steel
     ['MCC','108','raw:MCC','stone'], // Stone
     ['MCC','117','raw:MCC','wood'], // Wood
     ['MCC','119','raw:MCC','evaporite'], // Evaporite
     ['MCC','150','raw:MCC','treated_timber'], // Treated Timber
     ['MCC','152','raw:MCC','fibreglass'], // Fibreglass
     ['MCC','999','raw:MCC','other'], // Other

     // MCC2 - Structural Material Type [2]
     // ['MCC2','-999999',undefined,undefined], // noInformation
     ['MCC2','5','raw:MCC2','asphalt'], // Asphalt
     ['MCC2','7','raw:MCC2','bedrock'], // Bedrock
     ['MCC2','8','raw:MCC2','boulders'], // Boulders
     ['MCC2','9','raw:MCC2','brick'], // Brick
     ['MCC2','21','raw:MCC2','concrete'], // Concrete
     ['MCC2','46','raw:MCC2','gravel'], // Gravel
     ['MCC2','51','raw:MCC2','iron'], // Iron
     ['MCC2','52','raw:MCC2','lava'], // Lava
     ['MCC2','55','raw:MCC2','loess'], // Loess
     ['MCC2','62','raw:MCC2','masonry'], // Masonry
     ['MCC2','64','raw:MCC2','metal'], // Metal
     ['MCC2','65','raw:MCC2','mud'], // Mud
     ['MCC2','77','raw:MCC2','prestressed_concrete'], // Prestressed Concrete
     ['MCC2','83','raw:MCC2','reinforced_concrete'], // Reinforced Concrete
     ['MCC2','84','raw:MCC2','rock/rocky'], // Rock/Rocky
     ['MCC2','86','raw:MCC2','rubble'], // Rubble
     ['MCC2','88','raw:MCC2','sand'], // Sand
     ['MCC2','103','raw:MCC2','frozen_water'], // Frozen Water
     ['MCC2','104','raw:MCC2','soil'], // Soil
     ['MCC2','107','raw:MCC2','steel'], // Steel
     ['MCC2','108','raw:MCC2','stone'], // Stone
     ['MCC2','117','raw:MCC2','wood'], // Wood
     ['MCC2','119','raw:MCC2','evaporite'], // Evaporite
     ['MCC2','150','raw:MCC2','treated_timber'], // Treated Timber
     ['MCC2','152','raw:MCC2','fibreglass'], // Fibreglass
     ['MCC2','999','raw:MCC2','other'], // Other

     // MCC3 - Structural Material Type [3]
     // ['MCC3','-999999',undefined,undefined], // noInformation
     ['MCC3','5','raw:MCC3','asphalt'], // Asphalt
     ['MCC3','7','raw:MCC3','bedrock'], // Bedrock
     ['MCC3','8','raw:MCC3','boulders'], // Boulders
     ['MCC3','9','raw:MCC3','brick'], // Brick
     ['MCC3','21','raw:MCC3','concrete'], // Concrete
     ['MCC3','46','raw:MCC3','gravel'], // Gravel
     ['MCC3','51','raw:MCC3','iron'], // Iron
     ['MCC3','52','raw:MCC3','lava'], // Lava
     ['MCC3','55','raw:MCC3','loess'], // Loess
     ['MCC3','62','raw:MCC3','masonry'], // Masonry
     ['MCC3','64','raw:MCC3','metal'], // Metal
     ['MCC3','65','raw:MCC3','mud'], // Mud
     ['MCC3','77','raw:MCC3','prestressed_concrete'], // Prestressed Concrete
     ['MCC3','83','raw:MCC3','reinforced_concrete'], // Reinforced Concrete
     ['MCC3','84','raw:MCC3','rock/rocky'], // Rock/Rocky
     ['MCC3','86','raw:MCC3','rubble'], // Rubble
     ['MCC3','88','raw:MCC3','sand'], // Sand
     ['MCC3','103','raw:MCC3','frozen_water'], // Frozen Water
     ['MCC3','104','raw:MCC3','soil'], // Soil
     ['MCC3','107','raw:MCC3','steel'], // Steel
     ['MCC3','108','raw:MCC3','stone'], // Stone
     ['MCC3','117','raw:MCC3','wood'], // Wood
     ['MCC3','119','raw:MCC3','evaporite'], // Evaporite
     ['MCC3','150','raw:MCC3','treated_timber'], // Treated Timber
     ['MCC3','152','raw:MCC3','fibreglass'], // Fibreglass
     ['MCC3','999','raw:MCC3','other'], // Other

     // MCX - Motorized Crossing
     // ['MCX','-999999',undefined,undefined], // noInformation
     ['MCX','1000','raw:MCX','no'],
     ['MCX','1001','raw:MCX','yes'],

     // MES - Median Present
     // ['MES','-999999',undefined,undefined], // noInformation
     ['MES','1000','raw:MES','no'],
     ['MES','1001','raw:MES','yes'],

     // MNS - Man-made Shoreline
     // ['MNS','-999999',undefined,undefined], // noInformation
     ['MNS','1000','raw:MNS','no'],
     ['MNS','1001','raw:MNS','yes'],

     // MZN - Extraction Mine Type
     // ['MZN','-999999',undefined,undefined], // noInformation
     ['MZN','1','raw:MZN','borrow-pit'], // Borrow-pit
     ['MZN','3','raw:MZN','opencast'], // Opencast
     ['MZN','4','raw:MZN','placer'], // Placer
     ['MZN','5','raw:MZN','prospect'], // Prospect
     ['MZN','6','raw:MZN','area_strip-mine'], // Area Strip-mine
     ['MZN','8','raw:MZN','peatery'], // Peatery
     ['MZN','9','raw:MZN','below_surface'], // Below Surface
     ['MZN','10','raw:MZN','quarry'], // Quarry
     ['MZN','11','raw:MZN','contour_strip-mine'], // Contour Strip-mine
     ['MZN','13','raw:MZN','dredge'], // Dredge
     ['MZN','999','raw:MZN','other'], // Other

     // NLT - Named Location Type
     // ['NLT','-999999',undefined,undefined], // noInformation
     ['NLT','1','raw:NLT','area'], // Area
     ['NLT','2','raw:NLT','locality'], // Locality
     ['NLT','3','raw:NLT','region'], // Region
     ['NLT','4','raw:NLT','arctic_land'], // Arctic Land
     ['NLT','5','raw:NLT','populated_locality'], // Populated Locality
     ['NLT','999','raw:NLT','other'], // Other

     // NVS - Navigability Information
     // ['NVS','-999999',undefined,undefined], // noInformation
     ['NVS','1','raw:NVS','navigable_and_operational'], // Navigable and Operational
     ['NVS','2','raw:NVS','navigable_but_abandoned'], // Navigable but Abandoned
     ['NVS','3','raw:NVS','navigable'], // Navigable
     ['NVS','4','raw:NVS','navigable_with_periodic_restrictions'], // Navigable with Periodic Restrictions
     ['NVS','5','raw:NVS','not_navigable'], // Not Navigable
     ['NVS','999','raw:NVS','other'], // Other

     // OCS - Offshore Construction Primary Structure
     // ['OCS','-999999',undefined,undefined], // noInformation
     ['OCS','1','raw:OCS','platform'], // Platform
     ['OCS','2','raw:OCS','terminal_buoy'], // Terminal Buoy
     ['OCS','3','raw:OCS','catenary_turntable'], // Catenary Turntable
     ['OCS','4','raw:OCS','submerged_turret'], // Submerged Turret
     ['OCS','5','raw:OCS','vessel'], // Vessel
     ['OCS','6','raw:OCS','barge'], // Barge
     ['OCS','7','raw:OCS','submerged_platform'], // Submerged Platform
     ['OCS','999','raw:OCS','other'], // Other

     // ONE - One-way
     // ['ONE','-999999',undefined,undefined], // noInformation
     ['ONE','1000','raw:ONE','no'],
     ['ONE','1001','raw:ONE','yes'],

     // OOC - Overhead Obstruction Type
     // ['OOC','-999999',undefined,undefined], // noInformation
     ['OOC','1','raw:OOC','frame_bridge_span'], // Frame Bridge Span
     ['OOC','2','raw:OOC','arch_bridge_span'], // Arch Bridge Span
     ['OOC','3','raw:OOC','roof'], // Roof
     ['OOC','4','raw:OOC','railway_power_line'], // Railway Power Line
     ['OOC','6','raw:OOC','bridge_superstructure'], // Bridge Superstructure
     ['OOC','7','raw:OOC','building'], // Building
     ['OOC','8','raw:OOC','bridge_span'], // Bridge Span
     ['OOC','9','raw:OOC','gantry'], // Gantry
     ['OOC','10','raw:OOC','scaffold'], // Scaffold
     ['OOC','11','raw:OOC','arcade'], // Arcade
     ['OOC','12','raw:OOC','building_overhang'], // Building Overhang
     ['OOC','13','raw:OOC','cable'], // Cable
     ['OOC','14','raw:OOC','cableway'], // Cableway
     ['OOC','15','raw:OOC','conveyor'], // Conveyor
     ['OOC','16','raw:OOC','entrance_and/or_exit'], // Entrance and/or Exit
     ['OOC','17','raw:OOC','memorial_monument'], // Memorial Monument
     ['OOC','18','raw:OOC','non-building_structure'], // Non-building Structure
     ['OOC','19','raw:OOC','overhead_walkway'], // Overhead Walkway
     ['OOC','20','raw:OOC','parking_garage'], // Parking Garage
     ['OOC','21','raw:OOC','pipeline'], // Pipeline
     ['OOC','22','raw:OOC','pipeline_crossing_point'], // Pipeline Crossing Point
     ['OOC','23','raw:OOC','route-related_structure'], // Route-related Structure
     ['OOC','24','raw:OOC','transportation_block'], // Transportation Block
     ['OOC','25','raw:OOC','transportation_route_protection_structure'], // Transportation Route Protection Structure
     ['OOC','26','raw:OOC','tunnel'], // Tunnel
     ['OOC','27','raw:OOC','traffic_sign'], // Traffic Sign
     ['OOC','999','raw:OOC','other'], // Other

     // OPT - Operating Cycle
     // ['OPT','-999999',undefined,undefined], // noInformation
     ['OPT','1','raw:OPT','daytime'], // Daytime
     ['OPT','2','raw:OPT','night-time'], // Night-time
     ['OPT','3','raw:OPT','continuously_operating'], // Continuously Operating
     ['OPT','4','raw:OPT','summer_season'], // Summer Season
     ['OPT','5','raw:OPT','winter_season'], // Winter Season
     ['OPT','6','raw:OPT','restricted'], // Restricted
     ['OPT','7','raw:OPT','never_operating'], // Never Operating
     ['OPT','999','raw:OPT','other'], // Other

     // OWO - Waterbody Overhead Obstruction
     // ['OWO','-999999',undefined,undefined], // noInformation
     ['OWO','1000','raw:OWO','no'],
     ['OWO','1001','raw:OWO','yes'],

     // PBY - By-product
     // ['PBY','-999999',undefined,undefined], // noInformation
     ['PBY','8','raw:PBY','refuse'], // Refuse
     ['PBY','15','raw:PBY','slag'], // Slag
     ['PBY','16','raw:PBY','sludge'], // Sludge
     ['PBY','17','raw:PBY','spoil'], // Spoil
     ['PBY','999','raw:PBY','other'], // Other

     // PBY2 - By-product [2]
     // ['PBY2','-999999',undefined,undefined], // noInformation
     ['PBY2','8','raw:PBY2','refuse'], // Refuse
     ['PBY2','15','raw:PBY2','slag'], // Slag
     ['PBY2','16','raw:PBY2','sludge'], // Sludge
     ['PBY2','17','raw:PBY2','spoil'], // Spoil
     ['PBY2','999','raw:PBY2','other'], // Other

     // PBY3 - By-product [3]
     // ['PBY3','-999999',undefined,undefined], // noInformation
     ['PBY3','8','raw:PBY3','refuse'], // Refuse
     ['PBY3','15','raw:PBY3','slag'], // Slag
     ['PBY3','16','raw:PBY3','sludge'], // Sludge
     ['PBY3','17','raw:PBY3','spoil'], // Spoil
     ['PBY3','999','raw:PBY3','other'], // Other

     // PCF - Physical Condition
     // ['PCF','-999999',undefined,undefined], // noInformation
     ['PCF','1','raw:PCF','construction'], // Construction
     ['PCF','2','raw:PCF','intact'], // Intact
     ['PCF','3','raw:PCF','unmaintained'], // Unmaintained
     ['PCF','4','raw:PCF','damaged'], // Damaged
     ['PCF','5','raw:PCF','dismantled'], // Dismantled
     ['PCF','6','raw:PCF','destroyed'], // Destroyed

     // PEC - Port of Entry
     // ['PEC','-999999',undefined,undefined], // noInformation
     ['PEC','1000','raw:PEC','no'],
     ['PEC','1001','raw:PEC','yes'],

     // PLT - Pipeline Type
     // ['PLT','-999999',undefined,undefined], // noInformation
     ['PLT','1','raw:PLT','transport_pipe'], // Transport Pipe
     ['PLT','2','raw:PLT','outfall_pipe'], // Outfall Pipe
     ['PLT','3','raw:PLT','intake_pipe'], // Intake Pipe
     ['PLT','4','raw:PLT','sewer'], // Sewer
     ['PLT','5','raw:PLT','pipeline_valve'], // Pipeline Valve
     ['PLT','7','raw:PLT','bubbler_system'], // Bubbler System
     ['PLT','999','raw:PLT','other'], // Other

     // PLT2 - Pipeline Type [2]
     // ['PLT2','-999999',undefined,undefined], // noInformation
     ['PLT2','1','raw:PLT2','transport_pipe'], // Transport Pipe
     ['PLT2','2','raw:PLT2','outfall_pipe'], // Outfall Pipe
     ['PLT2','3','raw:PLT2','intake_pipe'], // Intake Pipe
     ['PLT2','4','raw:PLT2','sewer'], // Sewer
     ['PLT2','5','raw:PLT2','pipeline_valve'], // Pipeline Valve
     ['PLT2','7','raw:PLT2','bubbler_system'], // Bubbler System
     ['PLT2','999','raw:PLT2','other'], // Other

     // PLT3 - Pipeline Type [3]
     // ['PLT3','-999999',undefined,undefined], // noInformation
     ['PLT3','1','raw:PLT3','transport_pipe'], // Transport Pipe
     ['PLT3','2','raw:PLT3','outfall_pipe'], // Outfall Pipe
     ['PLT3','3','raw:PLT3','intake_pipe'], // Intake Pipe
     ['PLT3','4','raw:PLT3','sewer'], // Sewer
     ['PLT3','5','raw:PLT3','pipeline_valve'], // Pipeline Valve
     ['PLT3','7','raw:PLT3','bubbler_system'], // Bubbler System
     ['PLT3','999','raw:PLT3','other'], // Other

     // POS - Power Source
     // ['POS','-999999',undefined,undefined], // noInformation
     ['POS','1','raw:POS','geothermal'], // Geothermal
     ['POS','2','raw:POS','hydro-electric'], // Hydro-electric
     ['POS','3','raw:POS','nuclear'], // Nuclear
     ['POS','4','raw:POS','thermal'], // Thermal
     ['POS','5','raw:POS','tidal'], // Tidal
     ['POS','999','raw:POS','other'], // Other

     // POS2 - Power Source [2]
     // ['POS2','-999999',undefined,undefined], // noInformation
     ['POS2','1','raw:POS2','geothermal'], // Geothermal
     ['POS2','2','raw:POS2','hydro-electric'], // Hydro-electric
     ['POS2','3','raw:POS2','nuclear'], // Nuclear
     ['POS2','4','raw:POS2','thermal'], // Thermal
     ['POS2','5','raw:POS2','tidal'], // Tidal
     ['POS2','999','raw:POS2','other'], // Other

     // POS3 - Power Source [3]
     // ['POS3','-999999',undefined,undefined], // noInformation
     ['POS3','1','raw:POS3','geothermal'], // Geothermal
     ['POS3','2','raw:POS3','hydro-electric'], // Hydro-electric
     ['POS3','3','raw:POS3','nuclear'], // Nuclear
     ['POS3','4','raw:POS3','thermal'], // Thermal
     ['POS3','5','raw:POS3','tidal'], // Tidal
     ['POS3','999','raw:POS3','other'], // Other

     // PPO - Product
     // ['PPO','-999999',undefined,undefined], // noInformation
     ['PPO','1','raw:PPO','aircraft'], // Aircraft
     ['PPO','3','raw:PPO','ammunition'], // Ammunition
     ['PPO','8','raw:PPO','basalt'], // Basalt
     ['PPO','9','raw:PPO','bauxite'], // Bauxite
     ['PPO','11','raw:PPO','bivalve_mollusc'], // Bivalve Mollusc
     ['PPO','13','raw:PPO','brick'], // Brick
     ['PPO','15','raw:PPO','cement'], // Cement
     ['PPO','16','raw:PPO','chemical'], // Chemical
     ['PPO','17','raw:PPO','clay'], // Clay
     ['PPO','18','raw:PPO','coal'], // Coal
     ['PPO','19','raw:PPO','cobbles'], // Cobbles
     ['PPO','21','raw:PPO','coke'], // Coke
     ['PPO','26','raw:PPO','copper'], // Copper
     ['PPO','29','raw:PPO','crustacean'], // Crustacean
     ['PPO','30','raw:PPO','cultivated_shellfish'], // Cultivated Shellfish
     ['PPO','33','raw:PPO','diamond'], // Diamond
     ['PPO','34','raw:PPO','diatomaceous_earth'], // Diatomaceous Earth
     ['PPO','35','raw:PPO','dolomite'], // Dolomite
     ['PPO','37','raw:PPO','electric_power'], // Electric Power
     ['PPO','38','raw:PPO','explosive'], // Explosive
     ['PPO','39','raw:PPO','fish'], // Fish
     ['PPO','45','raw:PPO','gas'], // Gas
     ['PPO','46','raw:PPO','petrol'], // Petrol
     ['PPO','48','raw:PPO','gold'], // Gold
     ['PPO','50','raw:PPO','granite'], // Granite
     ['PPO','53','raw:PPO','gravel'], // Gravel
     ['PPO','58','raw:PPO','iron'], // Iron
     ['PPO','59','raw:PPO','lead'], // Lead
     ['PPO','60','raw:PPO','lime'], // Lime
     ['PPO','61','raw:PPO','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['PPO','62','raw:PPO','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['PPO','63','raw:PPO','lumber'], // Lumber
     ['PPO','65','raw:PPO','manganese'], // Manganese
     ['PPO','66','raw:PPO','marble'], // Marble
     ['PPO','72','raw:PPO','mussels'], // Mussels
     ['PPO','77','raw:PPO','oysters'], // Oysters
     ['PPO','83','raw:PPO','petroleum'], // Petroleum
     ['PPO','85','raw:PPO','porphyry'], // Porphyry
     ['PPO','88','raw:PPO','pumice'], // Pumice
     ['PPO','89','raw:PPO','quartz'], // Quartz
     ['PPO','90','raw:PPO','radioactive_material'], // Radioactive Material
     ['PPO','93','raw:PPO','rock'], // Rock
     ['PPO','95','raw:PPO','salt'], // Salt
     ['PPO','96','raw:PPO','sand'], // Sand
     ['PPO','97','raw:PPO','sandstone'], // Sandstone
     ['PPO','101','raw:PPO','sewage'], // Sewage
     ['PPO','105','raw:PPO','silver'], // Silver
     ['PPO','110','raw:PPO','stone'], // Stone
     ['PPO','116','raw:PPO','timber'], // Timber
     ['PPO','117','raw:PPO','tobacco'], // Tobacco
     ['PPO','118','raw:PPO','travertine'], // Travertine
     ['PPO','120','raw:PPO','uranium'], // Uranium
     ['PPO','122','raw:PPO','water'], // Water
     ['PPO','126','raw:PPO','zinc'], // Zinc
     ['PPO','130','raw:PPO','chalk'], // Chalk
     ['PPO','146','raw:PPO','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['PPO','149','raw:PPO','brine'], // Brine
     ['PPO','151','raw:PPO','chromium'], // Chromium
     ['PPO','152','raw:PPO','nickel'], // Nickel
     ['PPO','153','raw:PPO','tin'], // Tin
     ['PPO','158','raw:PPO','pottery'], // Pottery
     ['PPO','159','raw:PPO','charcoal'], // Charcoal
     ['PPO','160','raw:PPO','milled_grain'], // Milled Grain
     ['PPO','161','raw:PPO','coalbed_methane'], // Coalbed Methane
     ['PPO','162','raw:PPO','natural_gas_condensate'], // Natural Gas Condensate
     ['PPO','163','raw:PPO','helium'], // Helium
     ['PPO','164','raw:PPO','hydrothermal_fluid'], // Hydrothermal Fluid
     ['PPO','170','raw:PPO','beverage'], // Beverage
     ['PPO','173','raw:PPO','fluorite'], // Fluorite
     ['PPO','204','raw:PPO','phosphate'], // Phosphate
     ['PPO','214','raw:PPO','biodiesel'], // Biodiesel
     ['PPO','224','raw:PPO','vanadium'], // Vanadium
     ['PPO','283','raw:PPO','mica'], // Mica
     ['PPO','339','raw:PPO','selenium'], // Selenium
     ['PPO','999','raw:PPO','other'], // Other

     // PPO2 - Product [2]
     // ['PPO2','-999999',undefined,undefined], // noInformation
     ['PPO2','1','raw:PPO2','aircraft'], // Aircraft
     ['PPO2','3','raw:PPO2','ammunition'], // Ammunition
     ['PPO2','8','raw:PPO2','basalt'], // Basalt
     ['PPO2','9','raw:PPO2','bauxite'], // Bauxite
     ['PPO2','11','raw:PPO2','bivalve_mollusc'], // Bivalve Mollusc
     ['PPO2','13','raw:PPO2','brick'], // Brick
     ['PPO2','15','raw:PPO2','cement'], // Cement
     ['PPO2','16','raw:PPO2','chemical'], // Chemical
     ['PPO2','17','raw:PPO2','clay'], // Clay
     ['PPO2','18','raw:PPO2','coal'], // Coal
     ['PPO2','19','raw:PPO2','cobbles'], // Cobbles
     ['PPO2','21','raw:PPO2','coke'], // Coke
     ['PPO2','26','raw:PPO2','copper'], // Copper
     ['PPO2','29','raw:PPO2','crustacean'], // Crustacean
     ['PPO2','30','raw:PPO2','cultivated_shellfish'], // Cultivated Shellfish
     ['PPO2','33','raw:PPO2','diamond'], // Diamond
     ['PPO2','34','raw:PPO2','diatomaceous_earth'], // Diatomaceous Earth
     ['PPO2','35','raw:PPO2','dolomite'], // Dolomite
     ['PPO2','37','raw:PPO2','electric_power'], // Electric Power
     ['PPO2','38','raw:PPO2','explosive'], // Explosive
     ['PPO2','39','raw:PPO2','fish'], // Fish
     ['PPO2','45','raw:PPO2','gas'], // Gas
     ['PPO2','46','raw:PPO2','petrol'], // Petrol
     ['PPO2','48','raw:PPO2','gold'], // Gold
     ['PPO2','50','raw:PPO2','granite'], // Granite
     ['PPO2','53','raw:PPO2','gravel'], // Gravel
     ['PPO2','58','raw:PPO2','iron'], // Iron
     ['PPO2','59','raw:PPO2','lead'], // Lead
     ['PPO2','60','raw:PPO2','lime'], // Lime
     ['PPO2','61','raw:PPO2','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['PPO2','62','raw:PPO2','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['PPO2','63','raw:PPO2','lumber'], // Lumber
     ['PPO2','65','raw:PPO2','manganese'], // Manganese
     ['PPO2','66','raw:PPO2','marble'], // Marble
     ['PPO2','72','raw:PPO2','mussels'], // Mussels
     ['PPO2','77','raw:PPO2','oysters'], // Oysters
     ['PPO2','83','raw:PPO2','petroleum'], // Petroleum
     ['PPO2','85','raw:PPO2','porphyry'], // Porphyry
     ['PPO2','88','raw:PPO2','pumice'], // Pumice
     ['PPO2','89','raw:PPO2','quartz'], // Quartz
     ['PPO2','90','raw:PPO2','radioactive_material'], // Radioactive Material
     ['PPO2','93','raw:PPO2','rock'], // Rock
     ['PPO2','95','raw:PPO2','salt'], // Salt
     ['PPO2','96','raw:PPO2','sand'], // Sand
     ['PPO2','97','raw:PPO2','sandstone'], // Sandstone
     ['PPO2','101','raw:PPO2','sewage'], // Sewage
     ['PPO2','105','raw:PPO2','silver'], // Silver
     ['PPO2','110','raw:PPO2','stone'], // Stone
     ['PPO2','116','raw:PPO2','timber'], // Timber
     ['PPO2','117','raw:PPO2','tobacco'], // Tobacco
     ['PPO2','118','raw:PPO2','travertine'], // Travertine
     ['PPO2','120','raw:PPO2','uranium'], // Uranium
     ['PPO2','122','raw:PPO2','water'], // Water
     ['PPO2','126','raw:PPO2','zinc'], // Zinc
     ['PPO2','130','raw:PPO2','chalk'], // Chalk
     ['PPO2','146','raw:PPO2','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['PPO2','149','raw:PPO2','brine'], // Brine
     ['PPO2','151','raw:PPO2','chromium'], // Chromium
     ['PPO2','152','raw:PPO2','nickel'], // Nickel
     ['PPO2','153','raw:PPO2','tin'], // Tin
     ['PPO2','158','raw:PPO2','pottery'], // Pottery
     ['PPO2','159','raw:PPO2','charcoal'], // Charcoal
     ['PPO2','160','raw:PPO2','milled_grain'], // Milled Grain
     ['PPO2','161','raw:PPO2','coalbed_methane'], // Coalbed Methane
     ['PPO2','162','raw:PPO2','natural_gas_condensate'], // Natural Gas Condensate
     ['PPO2','163','raw:PPO2','helium'], // Helium
     ['PPO2','164','raw:PPO2','hydrothermal_fluid'], // Hydrothermal Fluid
     ['PPO2','170','raw:PPO2','beverage'], // Beverage
     ['PPO2','173','raw:PPO2','fluorite'], // Fluorite
     ['PPO2','204','raw:PPO2','phosphate'], // Phosphate
     ['PPO2','214','raw:PPO2','biodiesel'], // Biodiesel
     ['PPO2','224','raw:PPO2','vanadium'], // Vanadium
     ['PPO2','283','raw:PPO2','mica'], // Mica
     ['PPO2','339','raw:PPO2','selenium'], // Selenium
     ['PPO2','999','raw:PPO2','other'], // Other

     // PPO3 - Product [3]
     // ['PPO3','-999999',undefined,undefined], // noInformation
     ['PPO3','1','raw:PPO3','aircraft'], // Aircraft
     ['PPO3','3','raw:PPO3','ammunition'], // Ammunition
     ['PPO3','8','raw:PPO3','basalt'], // Basalt
     ['PPO3','9','raw:PPO3','bauxite'], // Bauxite
     ['PPO3','11','raw:PPO3','bivalve_mollusc'], // Bivalve Mollusc
     ['PPO3','13','raw:PPO3','brick'], // Brick
     ['PPO3','15','raw:PPO3','cement'], // Cement
     ['PPO3','16','raw:PPO3','chemical'], // Chemical
     ['PPO3','17','raw:PPO3','clay'], // Clay
     ['PPO3','18','raw:PPO3','coal'], // Coal
     ['PPO3','19','raw:PPO3','cobbles'], // Cobbles
     ['PPO3','21','raw:PPO3','coke'], // Coke
     ['PPO3','26','raw:PPO3','copper'], // Copper
     ['PPO3','29','raw:PPO3','crustacean'], // Crustacean
     ['PPO3','30','raw:PPO3','cultivated_shellfish'], // Cultivated Shellfish
     ['PPO3','33','raw:PPO3','diamond'], // Diamond
     ['PPO3','34','raw:PPO3','diatomaceous_earth'], // Diatomaceous Earth
     ['PPO3','35','raw:PPO3','dolomite'], // Dolomite
     ['PPO3','37','raw:PPO3','electric_power'], // Electric Power
     ['PPO3','38','raw:PPO3','explosive'], // Explosive
     ['PPO3','39','raw:PPO3','fish'], // Fish
     ['PPO3','45','raw:PPO3','gas'], // Gas
     ['PPO3','46','raw:PPO3','petrol'], // Petrol
     ['PPO3','48','raw:PPO3','gold'], // Gold
     ['PPO3','50','raw:PPO3','granite'], // Granite
     ['PPO3','53','raw:PPO3','gravel'], // Gravel
     ['PPO3','58','raw:PPO3','iron'], // Iron
     ['PPO3','59','raw:PPO3','lead'], // Lead
     ['PPO3','60','raw:PPO3','lime'], // Lime
     ['PPO3','61','raw:PPO3','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['PPO3','62','raw:PPO3','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['PPO3','63','raw:PPO3','lumber'], // Lumber
     ['PPO3','65','raw:PPO3','manganese'], // Manganese
     ['PPO3','66','raw:PPO3','marble'], // Marble
     ['PPO3','72','raw:PPO3','mussels'], // Mussels
     ['PPO3','77','raw:PPO3','oysters'], // Oysters
     ['PPO3','83','raw:PPO3','petroleum'], // Petroleum
     ['PPO3','85','raw:PPO3','porphyry'], // Porphyry
     ['PPO3','88','raw:PPO3','pumice'], // Pumice
     ['PPO3','89','raw:PPO3','quartz'], // Quartz
     ['PPO3','90','raw:PPO3','radioactive_material'], // Radioactive Material
     ['PPO3','93','raw:PPO3','rock'], // Rock
     ['PPO3','95','raw:PPO3','salt'], // Salt
     ['PPO3','96','raw:PPO3','sand'], // Sand
     ['PPO3','97','raw:PPO3','sandstone'], // Sandstone
     ['PPO3','101','raw:PPO3','sewage'], // Sewage
     ['PPO3','105','raw:PPO3','silver'], // Silver
     ['PPO3','110','raw:PPO3','stone'], // Stone
     ['PPO3','116','raw:PPO3','timber'], // Timber
     ['PPO3','117','raw:PPO3','tobacco'], // Tobacco
     ['PPO3','118','raw:PPO3','travertine'], // Travertine
     ['PPO3','120','raw:PPO3','uranium'], // Uranium
     ['PPO3','122','raw:PPO3','water'], // Water
     ['PPO3','126','raw:PPO3','zinc'], // Zinc
     ['PPO3','130','raw:PPO3','chalk'], // Chalk
     ['PPO3','146','raw:PPO3','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['PPO3','149','raw:PPO3','brine'], // Brine
     ['PPO3','151','raw:PPO3','chromium'], // Chromium
     ['PPO3','152','raw:PPO3','nickel'], // Nickel
     ['PPO3','153','raw:PPO3','tin'], // Tin
     ['PPO3','158','raw:PPO3','pottery'], // Pottery
     ['PPO3','159','raw:PPO3','charcoal'], // Charcoal
     ['PPO3','160','raw:PPO3','milled_grain'], // Milled Grain
     ['PPO3','161','raw:PPO3','coalbed_methane'], // Coalbed Methane
     ['PPO3','162','raw:PPO3','natural_gas_condensate'], // Natural Gas Condensate
     ['PPO3','163','raw:PPO3','helium'], // Helium
     ['PPO3','164','raw:PPO3','hydrothermal_fluid'], // Hydrothermal Fluid
     ['PPO3','170','raw:PPO3','beverage'], // Beverage
     ['PPO3','173','raw:PPO3','fluorite'], // Fluorite
     ['PPO3','204','raw:PPO3','phosphate'], // Phosphate
     ['PPO3','214','raw:PPO3','biodiesel'], // Biodiesel
     ['PPO3','224','raw:PPO3','vanadium'], // Vanadium
     ['PPO3','283','raw:PPO3','mica'], // Mica
     ['PPO3','339','raw:PPO3','selenium'], // Selenium
     ['PPO3','999','raw:PPO3','other'], // Other

     // PRM - Permanent
     // ['PRM','-999999',undefined,undefined], // noInformation
     ['PRM','1000','raw:PRM','no'],
     ['PRM','1001','raw:PRM','yes'],

     // PRW - Raw Material
     // ['PRW','-999999',undefined,undefined], // noInformation
     ['PRW','12','raw:PRW','coal'], // Coal
     ['PRW','14','raw:PRW','coke'], // Coke
     ['PRW','34','raw:PRW','gravel'], // Gravel
     ['PRW','54','raw:PRW','ore'], // Ore
     ['PRW','67','raw:PRW','salt'], // Salt
     ['PRW','68','raw:PRW','sand'], // Sand
     ['PRW','84','raw:PRW','stone'], // Stone
     ['PRW','154','raw:PRW','sulphur'], // Sulphur
     ['PRW','999','raw:PRW','other'], // Other

     // PRW2 - Raw Material [2]
     // ['PRW2','-999999',undefined,undefined], // noInformation
     ['PRW2','12','raw:PRW2','coal'], // Coal
     ['PRW2','14','raw:PRW2','coke'], // Coke
     ['PRW2','34','raw:PRW2','gravel'], // Gravel
     ['PRW2','54','raw:PRW2','ore'], // Ore
     ['PRW2','67','raw:PRW2','salt'], // Salt
     ['PRW2','68','raw:PRW2','sand'], // Sand
     ['PRW2','84','raw:PRW2','stone'], // Stone
     ['PRW2','154','raw:PRW2','sulphur'], // Sulphur
     ['PRW2','999','raw:PRW2','other'], // Other

     // PRW3 - Raw Material [3]
     // ['PRW3','-999999',undefined,undefined], // noInformation
     ['PRW3','12','raw:PRW3','coal'], // Coal
     ['PRW3','14','raw:PRW3','coke'], // Coke
     ['PRW3','34','raw:PRW3','gravel'], // Gravel
     ['PRW3','54','raw:PRW3','ore'], // Ore
     ['PRW3','67','raw:PRW3','salt'], // Salt
     ['PRW3','68','raw:PRW3','sand'], // Sand
     ['PRW3','84','raw:PRW3','stone'], // Stone
     ['PRW3','154','raw:PRW3','sulphur'], // Sulphur
     ['PRW3','999','raw:PRW3','other'], // Other

     // PWC - Shoreline Construction Type
     // ['PWC','-999999',undefined,undefined], // noInformation
     ['PWC','1','raw:PWC','pier'], // Pier
     ['PWC','2','raw:PWC','wharf'], // Wharf
     ['PWC','3','raw:PWC','quay'], // Quay
     ['PWC','4','raw:PWC','breakwater'], // Breakwater
     ['PWC','5','raw:PWC','groin'], // Groin
     ['PWC','6','raw:PWC','mole'], // Mole
     ['PWC','7','raw:PWC','recreational_pier'], // Recreational Pier
     ['PWC','8','raw:PWC','training_wall'], // Training Wall
     ['PWC','9','raw:PWC','riprap'], // Riprap
     ['PWC','10','raw:PWC','revetment_(marine)'], // Revetment (Marine)
     ['PWC','11','raw:PWC','seawall'], // Seawall
     ['PWC','18','raw:PWC','promenade'], // Promenade
     ['PWC','999','raw:PWC','other'], // Other

     // PYC - Pylon Configuration
     // ['PYC','-999999',undefined,undefined], // noInformation
     ['PYC','1','raw:PYC',''a''], // 'A'
     ['PYC','2','raw:PYC',''h''], // 'H'
     ['PYC','3','raw:PYC',''i''], // 'I'
     ['PYC','4','raw:PYC',''y''], // 'Y'
     ['PYC','5','raw:PYC',''t''], // 'T'
     ['PYC','999','raw:PYC','other'], // Other

     // PYM - Pylon Material
     // ['PYM','-999999',undefined,undefined], // noInformation
     ['PYM','1','raw:PYM','aluminum'], // Aluminum
     ['PYM','3','raw:PYM','concrete'], // Concrete
     ['PYM','4','raw:PYM','masonry'], // Masonry
     ['PYM','5','raw:PYM','metal'], // Metal
     ['PYM','7','raw:PYM','wood'], // Wood
     ['PYM','8','raw:PYM','steel'], // Steel
     ['PYM','9','raw:PYM','fibreglass'], // Fibreglass
     ['PYM','10','raw:PYM','iron'], // Iron
     ['PYM','999','raw:PYM','other'], // Other

     // RAS - Radar Station Function
     // ['RAS','-999999',undefined,undefined], // noInformation
     ['RAS','1','raw:RAS','general_surveillance'], // General Surveillance
     ['RAS','2','raw:RAS','coastal_radar'], // Coastal Radar
     ['RAS','5','raw:RAS','early_warning'], // Early Warning
     ['RAS','6','raw:RAS','weather'], // Weather
     ['RAS','7','raw:RAS','satellite_tracking'], // Satellite Tracking
     ['RAS','8','raw:RAS','aircraft_flight_tracking'], // Aircraft Flight Tracking
     ['RAS','9','raw:RAS','fire_control_tracking'], // Fire Control Tracking
     ['RAS','10','raw:RAS','launch_control_tracking'], // Launch Control Tracking
     ['RAS','11','raw:RAS','aerodrome_ground_surveillance'], // Aerodrome Ground Surveillance
     ['RAS','12','raw:RAS','precision_approach'], // Precision Approach
     ['RAS','999','raw:RAS','other'], // Other

     // RCG - Resource Content Originator
     // ['RCG','-999999',undefined,undefined], // noInformation
     ['RCG','1','raw:RCG','u.s._army'], // U.S. Army
     ['RCG','2','raw:RCG','u.s._navy'], // U.S. Navy
     ['RCG','3','raw:RCG','u.s._air_force'], // U.S. Air Force
     ['RCG','4','raw:RCG','u.s._marine_corps'], // U.S. Marine Corps
     ['RCG','5','raw:RCG','u.s._coast_guard'], // U.S. Coast Guard
     ['RCG','6','raw:RCG','u.s._africa_command_(usafricom)'], // U.S. Africa Command (USAFRICOM)
     ['RCG','7','raw:RCG','u.s._central_command_(uscentcom)'], // U.S. Central Command (USCENTCOM)
     ['RCG','8','raw:RCG','u.s._european_command_(useucom)'], // U.S. European Command (USEUCOM)
     ['RCG','9','raw:RCG','u.s._joint_forces_command_(usjfcom)'], // U.S. Joint Forces Command (USJFCOM)
     ['RCG','10','raw:RCG','u.s._northern_command_(usnorthcom)'], // U.S. Northern Command (USNORTHCOM)
     ['RCG','11','raw:RCG','u.s._pacific_command_(pacom)'], // U.S. Pacific Command (PACOM)
     ['RCG','12','raw:RCG','u.s._special_operations_command_(ussocom)'], // U.S. Special Operations Command (USSOCOM)
     ['RCG','13','raw:RCG','u.s._southern_command_(ussouthcom)'], // U.S. Southern Command (USSOUTHCOM)
     ['RCG','14','raw:RCG','u.s._strategic_command_(usstratcom)'], // U.S. Strategic Command (USSTRATCOM)
     ['RCG','15','raw:RCG','u.s._transportation_command_(ustranscom)'], // U.S. Transportation Command (USTRANSCOM)
     ['RCG','16','raw:RCG','u.s._central_intelligence_agency_(cia)'], // U.S. Central Intelligence Agency (CIA)
     ['RCG','17','raw:RCG','u.s._defense_intelligence_agency_(dia)'], // U.S. Defense Intelligence Agency (DIA)
     ['RCG','18','raw:RCG','u.s._national_security_agency_(nsa)'], // U.S. National Security Agency (NSA)
     ['RCG','19','raw:RCG','u.s._national_geospatial-intelligence_agency_(nga)'], // U.S. National Geospatial-Intelligence Agency (NGA)
     ['RCG','20','raw:RCG','u.s._national_reconnaissance_office_(nro)'], // U.S. National Reconnaissance Office (NRO)
     ['RCG','21','raw:RCG','u.s._department_of_state'], // U.S. Department of State
     ['RCG','22','raw:RCG','u.s._department_of_homeland_security_(dhs)'], // U.S. Department of Homeland Security (DHS)
     ['RCG','23','raw:RCG','u.s._department_of_energy_(doe)'], // U.S. Department of Energy (DOE)
     ['RCG','24','raw:RCG','u.s._federal_bureau_of_investigation_(fbi)'], // U.S. Federal Bureau of Investigation (FBI)
     ['RCG','25','raw:RCG','u.s._geological_survey_(usgs)'], // U.S. Geological Survey (USGS)
     ['RCG','26','raw:RCG','u.s._national_civil_applications_program_(ncap)'], // U.S. National Civil Applications Program (NCAP)
     ['RCG','27','raw:RCG','u.s._national_oceanic_and_atmospheric_administration'], // U.S. National Oceanic and Atmospheric Administration
     ['RCG','28','raw:RCG','australian_geospatial-intelligence_organization_(australia)'], // Australian Geospatial-Intelligence Organization (Australia)
     ['RCG','29','raw:RCG','geographic_service_(belgium)'], // Geographic Service (Belgium)
     ['RCG','30','raw:RCG','military_topographic_service_(bulgaria)'], // Military Topographic Service (Bulgaria)
     ['RCG','31','raw:RCG','mapping_and_charting_establishment_(canada)'], // Mapping and Charting Establishment (Canada)
     ['RCG','32','raw:RCG','geographic_service_of_the_czech_armed_forces_(czech_republic)'], // Geographic Service of the Czech Armed Forces (Czech Republic)
     ['RCG','33','raw:RCG','defence_acquisition_and_logistics_organization_(denmark)'], // Defence Acquisition and Logistics Organization (Denmark)
     ['RCG','34','raw:RCG','military_geographic_group_(estonia)'], // Military Geographic Group (Estonia)
     ['RCG','35','raw:RCG','topographic_service_(finland)'], // Topographic Service (Finland)
     ['RCG','36','raw:RCG','bureau_geographie,_hydrographie,_oceanographie_et_meteorologie_(france)'], // Bureau Geographie, Hydrographie, Oceanographie et Meteorologie (France)
     ['RCG','37','raw:RCG','bundeswehr_geoinformation_office_(germany)'], // Bundeswehr Geoinformation Office (Germany)
     ['RCG','38','raw:RCG','hellenic_military_geographic_service_(greece)'], // Hellenic Military Geographic Service (Greece)
     ['RCG','39','raw:RCG','geoinformation_service_of_the_hungarian_defence_forces_(hungary)'], // Geoinformation Service of the Hungarian Defence Forces (Hungary)
     ['RCG','40','raw:RCG','defense_information_security_(italy)'], // Defense Information Security (Italy)
     ['RCG','41','raw:RCG','geospatial_information_agency_(latvia)'], // Geospatial Information Agency (Latvia)
     ['RCG','42','raw:RCG','military_mapping_centre_(lithuania)'], // Military Mapping Centre (Lithuania)
     ['RCG','43','raw:RCG','national_army_topographic_service_(moldova)'], // National Army Topographic Service (Moldova)
     ['RCG','44','raw:RCG','army_geographic_agency_(netherlands)'], // Army Geographic Agency (Netherlands)
     ['RCG','45','raw:RCG','geoint_new_zealand_(new_zealand)'], // GEOINT New Zealand (New Zealand)
     ['RCG','46','raw:RCG','military_geographic_service_(norway)'], // Military Geographic Service (Norway)
     ['RCG','47','raw:RCG','military_geography_division_(poland)'], // Military Geography Division (Poland)
     ['RCG','48','raw:RCG','army_geographic_institute_(portugal)'], // Army Geographic Institute (Portugal)
     ['RCG','49','raw:RCG','military_topographic_directorate_(romania)'], // Military Topographic Directorate (Romania)
     ['RCG','50','raw:RCG','topographic_institute_(slovakia)'], // Topographic Institute (Slovakia)
     ['RCG','51','raw:RCG','army_geographic_centre_(spain)'], // Army Geographic Centre (Spain)
     ['RCG','52','raw:RCG','swedish_armed_forces_(sweden)'], // Swedish Armed Forces (Sweden)
     ['RCG','53','raw:RCG','general_command_of_mapping_(turkey)'], // General Command of Mapping (Turkey)
     ['RCG','54','raw:RCG','defence_geographic_centre_intelligence_collection_group_(united_kingdom)'], // Defence Geographic Centre Intelligence Collection Group (United Kingdom)
     ['RCG','55','raw:RCG','u.s._army_geospatial_center_(agc)'], // U.S. Army Geospatial Center (AGC)
     ['RCG','56','raw:RCG','army_(australia)'], // Army (Australia)
     ['RCG','57','raw:RCG','military_geographic_division_(croatia)'], // Military Geographic Division (Croatia)
     ['RCG','58','raw:RCG','directorate_geospatial_information_(south_africa)'], // Directorate Geospatial Information (South Africa)
     ['RCG','59','raw:RCG','korean_defense_intelligence_agency_(south_korea)'], // Korean Defense Intelligence Agency (South Korea)
     ['RCG','60','raw:RCG','national_intelligence_service_(south_korea)'], // National Intelligence Service (South Korea)
     ['RCG','61','raw:RCG','imagery_support_group_(singapore)'], // Imagery Support Group (Singapore)
     ['RCG','62','raw:RCG','national_security_bureau_(taiwan)'], // National Security Bureau (Taiwan)
     ['RCG','63','raw:RCG','materiel_production_center_(taiwan)'], // Materiel Production Center (Taiwan)
     ['RCG','64','raw:RCG','ministry_of_defense_of_japan_(japan)'], // Ministry of Defense of Japan (Japan)
     ['RCG','65','raw:RCG','ministry_of_construction_and_urban_development_(mongolia)'], // Ministry of Construction and Urban Development (Mongolia)
     ['RCG','66','raw:RCG','national_mapping_and_resource_information_authority_(philippines)'], // National Mapping and Resource Information Authority (Philippines)
     ['RCG','67','raw:RCG','royal_jordanian_geographic_centre_(jordan)'], // Royal Jordanian Geographic Centre (Jordan)
     ['RCG','68','raw:RCG','national_survey_authority_(oman)'], // National Survey Authority (Oman)
     ['RCG','69','raw:RCG','armed_forces_general_headquarters_(qatar)'], // Armed Forces General Headquarters (Qatar)
     ['RCG','70','raw:RCG','ministry_of_defense_of_saudi_arabia_(saudi_arabia)'], // Ministry of Defense of Saudi Arabia (Saudi Arabia)
     ['RCG','71','raw:RCG','directorate_of_survey_(kuwait)'], // Directorate of Survey (Kuwait)
     ['RCG','72','raw:RCG','military_survey_department_(united_arab_emirates)'], // Military Survey Department (United Arab Emirates)
     ['RCG','73','raw:RCG','information_network_security_agency_(ethiopia)'], // Information Network Security Agency (Ethiopia)
     ['RCG','74','raw:RCG','ministry_of_state_for_defense_(kenya)'], // Ministry of State for Defense (Kenya)
     ['RCG','75','raw:RCG','el_instituto_nacional_de_estadistica_y_geografia_(mexico)'], // El Instituto Nacional de Estadistica y Geografia (Mexico)
     ['RCG','76','raw:RCG','instituto_geográfico_militar_(chile)'], // Instituto Geográfico Militar (Chile)
     ['RCG','77','raw:RCG','servicio_geográfico_militar_(uruguay)'], // Servicio Geográfico Militar (Uruguay)
     ['RCG','78','raw:RCG','dirección_del_servicio_geográfico_military_(paraguay)'], // Dirección del Servicio Geográfico Military (Paraguay)
     ['RCG','79','raw:RCG','instituto_geográfico_nacional_(peru)'], // Instituto Geográfico Nacional (Peru)
     ['RCG','80','raw:RCG','instituto_geográfico_agustín_codazzi_(colombia)'], // Instituto Geográfico Agustín Codazzi (Colombia)
     ['RCG','81','raw:RCG','instituto_geográfico_y_del_catastro_nacional_(el_salvador)'], // Instituto Geográfico y del Catastro Nacional (El Salvador)
     ['RCG','82','raw:RCG','instituto_geográfico_nacional_(guatemala)'], // Instituto Geográfico Nacional (Guatemala)
     ['RCG','83','raw:RCG','servicio_geográfico_militar_(guatemala)'], // Servicio Geográfico Militar (Guatemala)
     ['RCG','84','raw:RCG','instituto_cartográfico_militar_(dominican_republic)'], // Instituto Cartográfico Militar (Dominican Republic)
     ['RCG','85','raw:RCG','instituto_nicaragüense_de_estudios_terretoriales_(nicaragua)'], // Instituto Nicaragüense de Estudios Terretoriales (Nicaragua)
     ['RCG','86','raw:RCG','dirección_general_de_registros,_catastro,_y_geografía_(honduras)'], // Dirección General de Registros, Catastro, y Geografía (Honduras)
     ['RCG','87','raw:RCG','instituto_geográfico_militar_(ecuador)'], // Instituto Geográfico Militar (Ecuador)
     ['RCG','88','raw:RCG','instituto_geográfico_nacional_"tommy_guardia"_(panama)'], // Instituto Geográfico Nacional "Tommy Guardia" (Panama)
     ['RCG','89','raw:RCG','instituto_geográfico_nacional_(argentina)'], // Instituto Geográfico Nacional (Argentina)
     ['RCG','90','raw:RCG','diretoria_de_serviço_geográfico_(brazil)'], // Diretoria de Serviço Geográfico (Brazil)
     ['RCG','999','raw:RCG','other'], // Other

     // RFD - Roofed
     // ['RFD','-999999',undefined,undefined], // noInformation
     ['RFD','1000','raw:RFD','no'],
     ['RFD','1001','raw:RFD','yes'],

     // RIN_ROI - Route Identification <route designation type>
     // ['RIN_ROI','-999999',undefined,undefined], // noInformation
     ['RIN_ROI','1','raw:RIN_ROI','international'], // International
     ['RIN_ROI','2','raw:RIN_ROI','national_motorway'], // National Motorway
     ['RIN_ROI','3','raw:RIN_ROI','national'], // National
     ['RIN_ROI','4','raw:RIN_ROI','secondary'], // Secondary
     ['RIN_ROI','5','raw:RIN_ROI','local'], // Local
     ['RIN_ROI','999','raw:RIN_ROI','other'], // Other

     // RIN_ROI2 - Route Identification [2] <route designation type>
     // ['RIN_ROI2','-999999',undefined,undefined], // noInformation
     ['RIN_ROI2','1','raw:RIN_ROI2','international'], // International
     ['RIN_ROI2','2','raw:RIN_ROI2','national_motorway'], // National Motorway
     ['RIN_ROI2','3','raw:RIN_ROI2','national'], // National
     ['RIN_ROI2','4','raw:RIN_ROI2','secondary'], // Secondary
     ['RIN_ROI2','5','raw:RIN_ROI2','local'], // Local
     ['RIN_ROI2','999','raw:RIN_ROI2','other'], // Other

     // RIN_ROI3 - Route Identification [3] <route designation type>
     // ['RIN_ROI3','-999999',undefined,undefined], // noInformation
     ['RIN_ROI3','1','raw:RIN_ROI3','international'], // International
     ['RIN_ROI3','2','raw:RIN_ROI3','national_motorway'], // National Motorway
     ['RIN_ROI3','3','raw:RIN_ROI3','national'], // National
     ['RIN_ROI3','4','raw:RIN_ROI3','secondary'], // Secondary
     ['RIN_ROI3','5','raw:RIN_ROI3','local'], // Local
     ['RIN_ROI3','999','raw:RIN_ROI3','other'], // Other

     // RIP - Rig Present
     // ['RIP','-999999',undefined,undefined], // noInformation
     ['RIP','1000','raw:RIP','no'],
     ['RIP','1001','raw:RIP','yes'],

     // RIT - Road Interchange Type
     // ['RIT','-999999',undefined,undefined], // noInformation
     ['RIT','1','raw:RIT','cloverleaf'], // Cloverleaf
     ['RIT','2','raw:RIT','diamond'], // Diamond
     ['RIT','3','raw:RIT','fork'], // Fork
     ['RIT','4','raw:RIT','rotary'], // Rotary
     ['RIT','5','raw:RIT','staggered_ramps'], // Staggered Ramps
     ['RIT','6','raw:RIT','standard_ramps'], // Standard Ramps
     ['RIT','7','raw:RIT','symmetrical_ramps'], // Symmetrical Ramps
     ['RIT','8','raw:RIT','trumpet'], // Trumpet
     ['RIT','9','raw:RIT','turban'], // Turban
     ['RIT','10','raw:RIT','wye'], // Wye
     ['RIT','999','raw:RIT','other'], // Other

     // RKF - Rock Formation Structure
     // ['RKF','-999999',undefined,undefined], // noInformation
     ['RKF','1','raw:RKF','columnar'], // Columnar
     ['RKF','2','raw:RKF','needle'], // Needle
     ['RKF','3','raw:RKF','pinnacle'], // Pinnacle
     ['RKF','4','raw:RKF','fossilized_forest'], // Fossilized Forest
     ['RKF','999','raw:RKF','other'], // Other

     // RLE - Relative Level
     // ['RLE','-999999',undefined,undefined], // noInformation
     ['RLE','1','raw:RLE','raised'], // Raised
     ['RLE','2','raw:RLE','level'], // Level
     ['RLE','3','raw:RLE','depressed'], // Depressed
     ['RLE','998','raw:RLE','not_applicable'], // Not Applicable

     // RMWC - Route Median Width <interval closure>
     ['RMWC','2','raw:RMWC','open_interval'], // Open Interval
     ['RMWC','3','raw:RMWC','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['RMWC','4','raw:RMWC','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['RMWC','5','raw:RMWC','closed_interval'], // Closed Interval
     ['RMWC','6','raw:RMWC','greater-than_semi-interval'], // Greater-than Semi-interval
     ['RMWC','7','raw:RMWC','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['RMWC','8','raw:RMWC','less-than_semi-interval'], // Less-than Semi-interval
     ['RMWC','9','raw:RMWC','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // ROR - Road Interchange Ramp
     // ['ROR','-999999',undefined,undefined], // noInformation
     ['ROR','1000','raw:ROR','no'],
     ['ROR','1001','raw:ROR','yes'],

     // RRC - Railway Use
     // ['RRC','-999999',undefined,undefined], // noInformation
     ['RRC','2','raw:RRC','carline'], // Carline
     ['RRC','6','raw:RRC','underground_railway'], // Underground Railway
     ['RRC','8','raw:RRC','logging'], // Logging
     ['RRC','11','raw:RRC','rail_rapid_transit'], // Rail Rapid Transit
     ['RRC','13','raw:RRC','marine_railway'], // Marine Railway
     ['RRC','14','raw:RRC','tramway'], // Tramway
     ['RRC','15','raw:RRC','funicular'], // Funicular
     ['RRC','24','raw:RRC','museum'], // Museum
     ['RRC','32','raw:RRC','automated_transit_system'], // Automated Transit System
     ['RRC','33','raw:RRC','long-haul'], // Long-haul
     ['RRC','999','raw:RRC','other'], // Other

     // RRC2 - Railway Use [2]
     // ['RRC2','-999999',undefined,undefined], // noInformation
     ['RRC2','2','raw:RRC2','carline'], // Carline
     ['RRC2','6','raw:RRC2','underground_railway'], // Underground Railway
     ['RRC2','8','raw:RRC2','logging'], // Logging
     ['RRC2','11','raw:RRC2','rail_rapid_transit'], // Rail Rapid Transit
     ['RRC2','13','raw:RRC2','marine_railway'], // Marine Railway
     ['RRC2','14','raw:RRC2','tramway'], // Tramway
     ['RRC2','15','raw:RRC2','funicular'], // Funicular
     ['RRC2','24','raw:RRC2','museum'], // Museum
     ['RRC2','32','raw:RRC2','automated_transit_system'], // Automated Transit System
     ['RRC2','33','raw:RRC2','long-haul'], // Long-haul
     ['RRC2','999','raw:RRC2','other'], // Other

     // RRC3 - Railway Use [3]
     // ['RRC3','-999999',undefined,undefined], // noInformation
     ['RRC3','2','raw:RRC3','carline'], // Carline
     ['RRC3','6','raw:RRC3','underground_railway'], // Underground Railway
     ['RRC3','8','raw:RRC3','logging'], // Logging
     ['RRC3','11','raw:RRC3','rail_rapid_transit'], // Rail Rapid Transit
     ['RRC3','13','raw:RRC3','marine_railway'], // Marine Railway
     ['RRC3','14','raw:RRC3','tramway'], // Tramway
     ['RRC3','15','raw:RRC3','funicular'], // Funicular
     ['RRC3','24','raw:RRC3','museum'], // Museum
     ['RRC3','32','raw:RRC3','automated_transit_system'], // Automated Transit System
     ['RRC3','33','raw:RRC3','long-haul'], // Long-haul
     ['RRC3','999','raw:RRC3','other'], // Other

     // RSA - Branch Railway Type
     // ['RSA','-999999',undefined,undefined], // noInformation
     ['RSA','1','raw:RSA','spur'], // Spur
     ['RSA','2','raw:RSA','siding'], // Siding
     ['RSA','3','raw:RSA','passing'], // Passing
     ['RSA','999','raw:RSA','other'], // Other

     // RTA - Linear Feature Arrangement
     // ['RTA','-999999',undefined,undefined], // noInformation
     ['RTA','1','raw:RTA','single_arrangement'], // Single Arrangement
     ['RTA','2','raw:RTA','double_arrangement'], // Double Arrangement
     ['RTA','3','raw:RTA','multiple_arrangements'], // Multiple Arrangements
     ['RTA','999','raw:RTA','other'], // Other

     // RTY - Roadway Type
     // ['RTY','-999999',undefined,undefined], // noInformation
     ['RTY','1','raw:RTY','motorway'], // Motorway
     ['RTY','2','raw:RTY','limited_access_motorway'], // Limited Access Motorway
     ['RTY','3','raw:RTY','road'], // Road
     ['RTY','4','raw:RTY','street'], // Street
     ['RTY','999','raw:RTY','other'], // Other

     // RWC - Railway Class
     // ['RWC','-999999',undefined,undefined], // noInformation
     ['RWC','1','raw:RWC','high_speed_rail'], // High Speed Rail
     ['RWC','2','raw:RWC','main_line'], // Main Line
     ['RWC','3','raw:RWC','branch-line'], // Branch-line
     ['RWC','999','raw:RWC','other'], // Other

     // SAD - Sand Dune Stabilized
     // ['SAD','-999999',undefined,undefined], // noInformation
     ['SAD','1000','raw:SAD','no'],
     ['SAD','1001','raw:SAD','yes'],

     // SBB - Supported by Bridge Span
     // ['SBB','-999999',undefined,undefined], // noInformation
     ['SBB','1000','raw:SBB','no'],
     ['SBB','1001','raw:SBB','yes'],

     // SBC - Shelter Belt
     // ['SBC','-999999',undefined,undefined], // noInformation
     ['SBC','1000','raw:SBC','no'],
     ['SBC','1001','raw:SBC','yes'],

     // SBT - Substation Type
     // ['SBT','-999999',undefined,undefined], // noInformation
     ['SBT','1','raw:SBT','switched_substation'], // Switched Substation
     ['SBT','2','raw:SBT','transformer_substation'], // Transformer Substation
     ['SBT','3','raw:SBT','converter_substation'], // Converter Substation
     ['SBT','999','raw:SBT','other'], // Other

     // SBT2 - Substation Type [2]
     // ['SBT2','-999999',undefined,undefined], // noInformation
     ['SBT2','1','raw:SBT2','switched_substation'], // Switched Substation
     ['SBT2','2','raw:SBT2','transformer_substation'], // Transformer Substation
     ['SBT2','3','raw:SBT2','converter_substation'], // Converter Substation
     ['SBT2','999','raw:SBT2','other'], // Other

     // SBT3 - Substation Type [3]
     // ['SBT3','-999999',undefined,undefined], // noInformation
     ['SBT3','1','raw:SBT3','switched_substation'], // Switched Substation
     ['SBT3','2','raw:SBT3','transformer_substation'], // Transformer Substation
     ['SBT3','3','raw:SBT3','converter_substation'], // Converter Substation
     ['SBT3','999','raw:SBT3','other'], // Other

     // SDT - Sand Dune Type
     // ['SDT','-999999',undefined,undefined], // noInformation
     ['SDT','1','raw:SDT','crescent'], // Crescent
     ['SDT','2','raw:SDT','dome'], // Dome
     ['SDT','3','raw:SDT','transverse'], // Transverse
     ['SDT','4','raw:SDT','linear'], // Linear
     ['SDT','5','raw:SDT','parabolic'], // Parabolic
     ['SDT','6','raw:SDT','ripple'], // Ripple
     ['SDT','7','raw:SDT','star'], // Star
     ['SDT','8','raw:SDT','dome_and_transverse'], // Dome and Transverse
     ['SDT','999','raw:SDT','other'], // Other

     // SEP - Divided
     // ['SEP','-999999',undefined,undefined], // noInformation
     ['SEP','1000','raw:SEP','no'],
     ['SEP','1001','raw:SEP','yes'],

     // SGCC - Surface Slope <interval closure>
     ['SGCC','2','raw:SGCC','open_interval'], // Open Interval
     ['SGCC','3','raw:SGCC','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['SGCC','4','raw:SGCC','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['SGCC','5','raw:SGCC','closed_interval'], // Closed Interval
     ['SGCC','6','raw:SGCC','greater-than_semi-interval'], // Greater-than Semi-interval
     ['SGCC','7','raw:SGCC','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['SGCC','8','raw:SGCC','less-than_semi-interval'], // Less-than Semi-interval
     ['SGCC','9','raw:SGCC','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // SIC - Frozen Cover Type
     // ['SIC','-999999',undefined,undefined], // noInformation
     ['SIC','1','raw:SIC','snowfield'], // Snowfield
     ['SIC','2','raw:SIC','ice-field'], // Ice-field

     // SLR - Shoreline Ramp Type
     // ['SLR','-999999',undefined,undefined], // noInformation
     ['SLR','1','raw:SLR','log_ramp'], // Log Ramp
     ['SLR','2','raw:SLR','marine_ramp'], // Marine Ramp
     ['SLR','3','raw:SLR','slipway'], // Slipway
     ['SLR','999','raw:SLR','other'], // Other

     // SLT - Shoreline Type
     // ['SLT','-999999',undefined,undefined], // noInformation
     ['SLT','6','raw:SLT','mangrove'], // Mangrove
     ['SLT','8','raw:SLT','marshy'], // Marshy
     ['SLT','10','raw:SLT','stony'], // Stony
     ['SLT','11','raw:SLT','building_rubble'], // Building Rubble
     ['SLT','12','raw:SLT','erosion_rubble'], // Erosion Rubble
     ['SLT','13','raw:SLT','sandy'], // Sandy
     ['SLT','14','raw:SLT','shingly'], // Shingly
     ['SLT','16','raw:SLT','coral'], // Coral
     ['SLT','17','raw:SLT','ice'], // Ice
     ['SLT','18','raw:SLT','mud'], // Mud
     ['SLT','998','raw:SLT','not_applicable'], // Not Applicable
     ['SLT','999','raw:SLT','other'], // Other

     // SPT - Supported
     // ['SPT','-999999',undefined,undefined], // noInformation
     ['SPT','1000','raw:SPT','no'],
     ['SPT','1001','raw:SPT','yes'],

     // SRD - Terrain Morphology
     // ['SRD','-999999',undefined,undefined], // noInformation
     ['SRD','1','raw:SRD','no_effect'], // No Effect
     ['SRD','2','raw:SRD','landslide_potential'], // Landslide Potential
     ['SRD','11','raw:SRD','numerous_cobbles_and_boulders'], // Numerous Cobbles and Boulders
     ['SRD','12','raw:SRD','stony_areas'], // Stony Areas
     ['SRD','13','raw:SRD','stony_soil_with_surface_rock'], // Stony Soil with Surface Rock
     ['SRD','14','raw:SRD','stony_soil_with_scattered_boulders'], // Stony Soil with scattered Boulders
     ['SRD','15','raw:SRD','stony_soil_with_numerous_boulders'], // Stony Soil with numerous Boulders
     ['SRD','16','raw:SRD','numerous_boulders'], // Numerous Boulders
     ['SRD','17','raw:SRD','numerous_rock_outcrops'], // Numerous Rock Outcrops
     ['SRD','18','raw:SRD','scattered_boulders'], // Scattered Boulders
     ['SRD','19','raw:SRD','talus'], // Talus
     ['SRD','20','raw:SRD','boulder_field'], // Boulder Field
     ['SRD','31','raw:SRD','highly_fractured_rock'], // Highly Fractured Rock
     ['SRD','32','raw:SRD','weathered_lava'], // Weathered Lava
     ['SRD','33','raw:SRD','unweathered_lava'], // Unweathered Lava
     ['SRD','34','raw:SRD','stony_soil_with_numerous_rock_outcrops'], // Stony Soil with numerous Rock Outcrops
     ['SRD','35','raw:SRD','irregular_with_deep_foliation_fractures'], // Irregular with deep Foliation Fractures
     ['SRD','36','raw:SRD','rugged_with_numerous_rock_outcrops'], // Rugged with numerous Rock Outcrops
     ['SRD','37','raw:SRD','rugged_bedrock'], // Rugged Bedrock
     ['SRD','43','raw:SRD','highly_distorted_with_sharp_rocky_ridges'], // Highly Distorted with sharp Rocky Ridges
     ['SRD','51','raw:SRD','stony_soil_with_numerous_gullies'], // Stony Soil with numerous Gullies
     ['SRD','52','raw:SRD','moderately_dissected'], // Moderately Dissected
     ['SRD','53','raw:SRD','moderately_dissected_with_scattered_rock_outcrops'], // Moderately Dissected with scattered Rock Outcrops
     ['SRD','54','raw:SRD','dissected_floodplain'], // Dissected Floodplain
     ['SRD','55','raw:SRD','highly_dissected'], // Highly Dissected
     ['SRD','56','raw:SRD','deep_erosional_gullies'], // Deep Erosional Gullies
     ['SRD','57','raw:SRD','steep_rugged_dissected_with_narrow_gullies'], // Steep Rugged Dissected with narrow Gullies
     ['SRD','58','raw:SRD','karst_with_numerous_sinkholes_and_solution_valleys'], // Karst with numerous Sinkholes and Solution Valleys
     ['SRD','59','raw:SRD','karst_with_numerous_sinkholes'], // Karst with numerous Sinkholes
     ['SRD','60','raw:SRD','hummocky_karst_with_large_hills'], // Hummocky Karst with Large Hills
     ['SRD','61','raw:SRD','hummocky_karst_with_low_mounds'], // Hummocky Karst with Low Mounds
     ['SRD','63','raw:SRD','playa'], // Playa
     ['SRD','64','raw:SRD','meander_scars_and_lakes'], // Meander Scars and Lakes
     ['SRD','65','raw:SRD','solifluction_lobes_and_frost_scars'], // Solifluction Lobes and Frost Scars
     ['SRD','66','raw:SRD','hummocky_with_frost_heaves'], // Hummocky with Frost Heaves
     ['SRD','67','raw:SRD','frost_polygons'], // Frost Polygons
     ['SRD','69','raw:SRD','numerous_small_lakes_and_ponds'], // Numerous Small Lakes and Ponds
     ['SRD','70','raw:SRD','numerous_crevasses'], // Numerous Crevasses
     ['SRD','81','raw:SRD','numerous_terraces'], // Numerous Terraces
     ['SRD','86','raw:SRD','mine_tailing(s)'], // Mine Tailing(s)
     ['SRD','88','raw:SRD','numerous_dykes'], // Numerous Dykes
     ['SRD','89','raw:SRD','numerous_dyked_fields'], // Numerous Dyked Fields
     ['SRD','90','raw:SRD','numerous_fences'], // Numerous Fences
     ['SRD','91','raw:SRD','numerous_stone_walls'], // Numerous Stone Walls
     ['SRD','92','raw:SRD','numerous_man-made_drainage'], // Numerous Man-made Drainage
     ['SRD','93','raw:SRD','numerous_terraced_fields'], // Numerous Terraced Fields
     ['SRD','94','raw:SRD','parallel_earthen_rows'], // Parallel Earthen Rows
     ['SRD','95','raw:SRD','numerous_hedgerows'], // Numerous Hedgerows
     ['SRD','999','raw:SRD','other'], // Other

     // SRL - Location Referenced to Shoreline
     // ['SRL','-999999',undefined,undefined], // noInformation
     ['SRL','1','raw:SRL','offshore'], // Offshore
     ['SRL','2','raw:SRL','inland'], // Inland
     ['SRL','3','raw:SRL','at_shoreline'], // At Shoreline

     // SSC - Structure Shape
     // ['SSC','-999999',undefined,undefined], // noInformation
     ['SSC','2','raw:SSC','horizontal_capped_cylindrical'], // Horizontal Capped Cylindrical
     ['SSC','4','raw:SSC','vertical_capped_cylindrical'], // Vertical Capped Cylindrical
     ['SSC','12','raw:SSC','pyramidal'], // Pyramidal
     ['SSC','17','raw:SSC','spherical'], // Spherical
     ['SSC','59','raw:SSC','cylindrical_with_framework'], // Cylindrical with Framework
     ['SSC','65','raw:SSC','cylindrical_with_flat_top'], // Cylindrical with Flat Top
     ['SSC','66','raw:SSC','cylindrical_with_domed_top'], // Cylindrical with Domed Top
     ['SSC','71','raw:SSC','cylindrical_with_conical_top'], // Cylindrical with Conical Top
     ['SSC','77','raw:SSC','arched'], // Arched
     ['SSC','78','raw:SSC','multiple_arched'], // Multiple Arched
     ['SSC','87','raw:SSC','domed'], // Domed
     ['SSC','88','raw:SSC','spherical_on_column'], // Spherical on Column
     ['SSC','89','raw:SSC','cylindrical_on_tower'], // Cylindrical on Tower
     ['SSC','91','raw:SSC','vertical_cylindrical'], // Vertical Cylindrical
     ['SSC','92','raw:SSC','cubic'], // Cubic
     ['SSC','94','raw:SSC','board-like_prism'], // Board-like Prism
     ['SSC','95','raw:SSC','columnar'], // Columnar
     ['SSC','96','raw:SSC','plaque'], // Plaque
     ['SSC','97','raw:SSC','statue'], // Statue
     ['SSC','98','raw:SSC','cross'], // Cross
     ['SSC','99','raw:SSC','horizontal_cylindrical'], // Horizontal Cylindrical
     ['SSC','100','raw:SSC','rectangular_prism'], // Rectangular Prism
     ['SSC','101','raw:SSC','square_prism'], // Square Prism
     ['SSC','109','raw:SSC','obelisk'], // Obelisk
     ['SSC','112','raw:SSC','statue_on_pedestal'], // Statue on Pedestal
     ['SSC','999','raw:SSC','other'], // Other

     // SSR - Roof Shape
     // ['SSR','-999999',undefined,undefined], // noInformation
     ['SSR','6','raw:SSR','conical'], // Conical
     ['SSR','7','raw:SSR','pyramidal'], // Pyramidal
     ['SSR','38','raw:SSR','semi-cylindrical'], // Semi-cylindrical
     ['SSR','40','raw:SSR','domed'], // Domed
     ['SSR','41','raw:SSR','flat'], // Flat
     ['SSR','42','raw:SSR','pitched'], // Pitched
     ['SSR','47','raw:SSR','sawtoothed'], // Sawtoothed
     ['SSR','50','raw:SSR','with_clerestory'], // With Clerestory
     ['SSR','55','raw:SSR','flat_with_clerestory'], // Flat with Clerestory
     ['SSR','64','raw:SSR','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR','999','raw:SSR','other'], // Other

     // SSR2 - Roof Shape [2]
     // ['SSR2','-999999',undefined,undefined], // noInformation
     ['SSR2','6','raw:SSR2','conical'], // Conical
     ['SSR2','7','raw:SSR2','pyramidal'], // Pyramidal
     ['SSR2','38','raw:SSR2','semi-cylindrical'], // Semi-cylindrical
     ['SSR2','40','raw:SSR2','domed'], // Domed
     ['SSR2','41','raw:SSR2','flat'], // Flat
     ['SSR2','42','raw:SSR2','pitched'], // Pitched
     ['SSR2','47','raw:SSR2','sawtoothed'], // Sawtoothed
     ['SSR2','50','raw:SSR2','with_clerestory'], // With Clerestory
     ['SSR2','55','raw:SSR2','flat_with_clerestory'], // Flat with Clerestory
     ['SSR2','64','raw:SSR2','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR2','999','raw:SSR2','other'], // Other

     // SSR3 - Roof Shape [3]
     // ['SSR3','-999999',undefined,undefined], // noInformation
     ['SSR3','6','raw:SSR3','conical'], // Conical
     ['SSR3','7','raw:SSR3','pyramidal'], // Pyramidal
     ['SSR3','38','raw:SSR3','semi-cylindrical'], // Semi-cylindrical
     ['SSR3','40','raw:SSR3','domed'], // Domed
     ['SSR3','41','raw:SSR3','flat'], // Flat
     ['SSR3','42','raw:SSR3','pitched'], // Pitched
     ['SSR3','47','raw:SSR3','sawtoothed'], // Sawtoothed
     ['SSR3','50','raw:SSR3','with_clerestory'], // With Clerestory
     ['SSR3','55','raw:SSR3','flat_with_clerestory'], // Flat with Clerestory
     ['SSR3','64','raw:SSR3','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR3','999','raw:SSR3','other'], // Other

     // STP - Soil Type
     // ['STP','-999999',undefined,undefined], // noInformation
     ['STP','1','raw:STP','gw:_well-graded_gravel'], // GW: Well-graded Gravel
     ['STP','2','raw:STP','gp:_poorly-graded_gravel'], // GP: Poorly-graded Gravel
     ['STP','3','raw:STP','gm:_silty_gravel_sand'], // GM: Silty Gravel Sand
     ['STP','4','raw:STP','gc:_clayey_gravel'], // GC: Clayey Gravel
     ['STP','5','raw:STP','sw:_well-graded_sand'], // SW: Well-graded Sand
     ['STP','6','raw:STP','sp:_poorly-graded_sand'], // SP: Poorly-graded Sand
     ['STP','7','raw:STP','sm:_silty_sand'], // SM: Silty Sand
     ['STP','8','raw:STP','sc:_clayey_sand'], // SC: Clayey Sand
     ['STP','9','raw:STP','ml:_silt_and_fine_sand'], // ML: Silt and Fine Sand
     ['STP','10','raw:STP','cl:_lean_clay'], // CL: Lean Clay
     ['STP','11','raw:STP','ol:_organic_silt_and_clay'], // OL: Organic Silt and Clay
     ['STP','12','raw:STP','ch:_fat_clay'], // CH: Fat Clay
     ['STP','13','raw:STP','mh:_micraceous'], // MH: Micraceous
     ['STP','14','raw:STP','oh:_organic_clay'], // OH: Organic Clay
     ['STP','15','raw:STP','pt:_peat'], // PT: Peat
     ['STP','17','raw:STP','ml-cl:_silt,_fine_sand_and_lean_clay'], // ML-CL: Silt, Fine Sand and Lean Clay
     ['STP','18','raw:STP','evaporite'], // Evaporite
     ['STP','99','raw:STP','not_evaluated'], // Not Evaluated
     ['STP','999','raw:STP','other'], // Other

     // SWC - Soil Wetness Condition
     // ['SWC','-999999',undefined,undefined], // noInformation
     ['SWC','1','raw:SWC','normally_dry'], // Normally Dry
     ['SWC','2','raw:SWC','normally_moist'], // Normally Moist
     ['SWC','3','raw:SWC','normally_wet'], // Normally Wet
     ['SWC','4','raw:SWC','normally_frozen'], // Normally Frozen

     // SWT - Natural Pool Type
     // ['SWT','-999999',undefined,undefined], // noInformation
     ['SWT','4','raw:SWT','spring'], // Spring
     ['SWT','6','raw:SWT','walled-in_spring'], // Walled-in Spring
     ['SWT','7','raw:SWT','resurgence'], // Resurgence
     ['SWT','999','raw:SWT','other'], // Other

     // TCS - Cross-sectional Profile
     // ['TCS','-999999',undefined,undefined], // noInformation
     ['TCS','1','raw:TCS','arch'], // Arch
     ['TCS','2','raw:TCS','box'], // Box
     ['TCS','3','raw:TCS','semicircular'], // Semicircular
     ['TCS','999','raw:TCS','other'], // Other

     // TEL - Telescope Type
     // ['TEL','-999999',undefined,undefined], // noInformation
     ['TEL','1','raw:TEL','optical'], // Optical
     ['TEL','2','raw:TEL','parabolic_radio_aerial'], // Parabolic Radio Aerial
     ['TEL','3','raw:TEL','radio_aerial_array'], // Radio Aerial Array
     ['TEL','999','raw:TEL','other'], // Other

     // THR - Through Route
     // ['THR','-999999',undefined,undefined], // noInformation
     ['THR','1000','raw:THR','no'],
     ['THR','1001','raw:THR','yes'],

     // TID - Tide Influenced
     // ['TID','-999999',undefined,undefined], // noInformation
     ['TID','1000','raw:TID','no'],
     ['TID','1001','raw:TID','yes'],

     // TOS - Tower Shape
     // ['TOS','-999999',undefined,undefined], // noInformation
     ['TOS','1','raw:TOS',''a'_frame'], // 'A' Frame
     ['TOS','2','raw:TOS',''h'_frame'], // 'H' Frame
     ['TOS','3','raw:TOS',''i'_frame'], // 'I' Frame
     ['TOS','5','raw:TOS',''y'_frame'], // 'Y' Frame
     ['TOS','6','raw:TOS','mast'], // Mast
     ['TOS','8','raw:TOS','pole'], // Pole
     ['TOS','11','raw:TOS','tripod'], // Tripod
     ['TOS','12','raw:TOS','truss'], // Truss
     ['TOS','13','raw:TOS','tubular'], // Tubular
     ['TOS','14','raw:TOS','tower_mill'], // Tower Mill
     ['TOS','999','raw:TOS','other'], // Other

     // TRA - Pedestrian Traversable
     // ['TRA','-999999',undefined,undefined], // noInformation
     ['TRA','1000','raw:TRA','no'],
     ['TRA','1001','raw:TRA','yes'],

     // TRE - Foliage Type
     // ['TRE','-999999',undefined,undefined], // noInformation
     ['TRE','1','raw:TRE','deciduous'], // Deciduous
     ['TRE','2','raw:TRE','evergreen'], // Evergreen
     ['TRE','3','raw:TRE','mixed'], // Mixed
     ['TRE','999','raw:TRE','other'], // Other

     // TRP - Transportation Route Protection Structure Type
     // ['TRP','-999999',undefined,undefined], // noInformation
     ['TRP','1','raw:TRP','gallery'], // Gallery
     ['TRP','2','raw:TRP','rock_protection_shed'], // Rock Protection Shed
     ['TRP','3','raw:TRP','snow_protection_shed'], // Snow Protection Shed
     ['TRP','4','raw:TRP','protection_shed'], // Protection Shed
     ['TRP','999','raw:TRP','other'], // Other

     // TRS - Transportation System Type
     // ['TRS','-999999',undefined,undefined], // noInformation
     ['TRS','1','raw:TRS','aeronautical'], // Aeronautical
     ['TRS','2','raw:TRS','aqueduct'], // Aqueduct
     ['TRS','3','raw:TRS','automotive'], // Automotive
     ['TRS','4','raw:TRS','bus'], // Bus
     ['TRS','5','raw:TRS','canal'], // Canal
     ['TRS','6','raw:TRS','caravan_route'], // Caravan Route
     ['TRS','7','raw:TRS','maritime'], // Maritime
     ['TRS','8','raw:TRS','no_transportation_system'], // No Transportation System
     ['TRS','9','raw:TRS','pedestrian'], // Pedestrian
     ['TRS','10','raw:TRS','pipeline'], // Pipeline
     ['TRS','11','raw:TRS','portage'], // Portage
     ['TRS','12','raw:TRS','railway'], // Railway
     ['TRS','13','raw:TRS','road'], // Road
     ['TRS','15','raw:TRS','truck'], // Truck
     ['TRS','16','raw:TRS','underground_railway'], // Underground Railway
     ['TRS','17','raw:TRS','inland_waterway'], // Inland Waterway
     ['TRS','18','raw:TRS','pack-road'], // Pack-road
     ['TRS','20','raw:TRS','taxiway'], // Taxiway
     ['TRS','21','raw:TRS','pipeline_maintenance'], // Pipeline Maintenance
     ['TRS','22','raw:TRS','power_line_maintenance'], // Power Line Maintenance
     ['TRS','23','raw:TRS','drove'], // Drove
     ['TRS','25','raw:TRS','runway'], // Runway
     ['TRS','30','raw:TRS','cableway'], // Cableway
     ['TRS','999','raw:TRS','other'], // Other

     // TRS2 - Transportation System Type [2]
     // ['TRS2','-999999',undefined,undefined], // noInformation
     ['TRS2','1','raw:TRS2','aeronautical'], // Aeronautical
     ['TRS2','2','raw:TRS2','aqueduct'], // Aqueduct
     ['TRS2','3','raw:TRS2','automotive'], // Automotive
     ['TRS2','4','raw:TRS2','bus'], // Bus
     ['TRS2','5','raw:TRS2','canal'], // Canal
     ['TRS2','6','raw:TRS2','caravan_route'], // Caravan Route
     ['TRS2','7','raw:TRS2','maritime'], // Maritime
     ['TRS2','8','raw:TRS2','no_transportation_system'], // No Transportation System
     ['TRS2','9','raw:TRS2','pedestrian'], // Pedestrian
     ['TRS2','10','raw:TRS2','pipeline'], // Pipeline
     ['TRS2','11','raw:TRS2','portage'], // Portage
     ['TRS2','12','raw:TRS2','railway'], // Railway
     ['TRS2','13','raw:TRS2','road'], // Road
     ['TRS2','15','raw:TRS2','truck'], // Truck
     ['TRS2','16','raw:TRS2','underground_railway'], // Underground Railway
     ['TRS2','17','raw:TRS2','inland_waterway'], // Inland Waterway
     ['TRS2','18','raw:TRS2','pack-road'], // Pack-road
     ['TRS2','20','raw:TRS2','taxiway'], // Taxiway
     ['TRS2','21','raw:TRS2','pipeline_maintenance'], // Pipeline Maintenance
     ['TRS2','22','raw:TRS2','power_line_maintenance'], // Power Line Maintenance
     ['TRS2','23','raw:TRS2','drove'], // Drove
     ['TRS2','25','raw:TRS2','runway'], // Runway
     ['TRS2','30','raw:TRS2','cableway'], // Cableway
     ['TRS2','999','raw:TRS2','other'], // Other

     // TRS3 - Transportation System Type [3]
     // ['TRS3','-999999',undefined,undefined], // noInformation
     ['TRS3','1','raw:TRS3','aeronautical'], // Aeronautical
     ['TRS3','2','raw:TRS3','aqueduct'], // Aqueduct
     ['TRS3','3','raw:TRS3','automotive'], // Automotive
     ['TRS3','4','raw:TRS3','bus'], // Bus
     ['TRS3','5','raw:TRS3','canal'], // Canal
     ['TRS3','6','raw:TRS3','caravan_route'], // Caravan Route
     ['TRS3','7','raw:TRS3','maritime'], // Maritime
     ['TRS3','8','raw:TRS3','no_transportation_system'], // No Transportation System
     ['TRS3','9','raw:TRS3','pedestrian'], // Pedestrian
     ['TRS3','10','raw:TRS3','pipeline'], // Pipeline
     ['TRS3','11','raw:TRS3','portage'], // Portage
     ['TRS3','12','raw:TRS3','railway'], // Railway
     ['TRS3','13','raw:TRS3','road'], // Road
     ['TRS3','15','raw:TRS3','truck'], // Truck
     ['TRS3','16','raw:TRS3','underground_railway'], // Underground Railway
     ['TRS3','17','raw:TRS3','inland_waterway'], // Inland Waterway
     ['TRS3','18','raw:TRS3','pack-road'], // Pack-road
     ['TRS3','20','raw:TRS3','taxiway'], // Taxiway
     ['TRS3','21','raw:TRS3','pipeline_maintenance'], // Pipeline Maintenance
     ['TRS3','22','raw:TRS3','power_line_maintenance'], // Power Line Maintenance
     ['TRS3','23','raw:TRS3','drove'], // Drove
     ['TRS3','25','raw:TRS3','runway'], // Runway
     ['TRS3','30','raw:TRS3','cableway'], // Cableway
     ['TRS3','999','raw:TRS3','other'], // Other

     // TSM - Terrain Surface Material
     // ['TSM','-999999',undefined,undefined], // noInformation
     ['TSM','1','raw:TSM','asphalt'], // Asphalt
     ['TSM','2','raw:TSM','bedrock'], // Bedrock
     ['TSM','3','raw:TSM','boulders'], // Boulders
     ['TSM','4','raw:TSM','concrete'], // Concrete
     ['TSM','5','raw:TSM','evaporite'], // Evaporite
     ['TSM','6','raw:TSM','frozen_water'], // Frozen Water
     ['TSM','7','raw:TSM','gravel'], // Gravel
     ['TSM','8','raw:TSM','lava_flow'], // Lava Flow
     ['TSM','9','raw:TSM','loess'], // Loess
     ['TSM','10','raw:TSM','mud'], // Mud
     ['TSM','11','raw:TSM','paved'], // Paved
     ['TSM','12','raw:TSM','rock'], // Rock
     ['TSM','13','raw:TSM','sand'], // Sand
     ['TSM','14','raw:TSM','soil'], // Soil
     ['TSM','999','raw:TSM','other'], // Other

     // TSM2 - Terrain Surface Material [2]
     // ['TSM2','-999999',undefined,undefined], // noInformation
     ['TSM2','1','raw:TSM2','asphalt'], // Asphalt
     ['TSM2','2','raw:TSM2','bedrock'], // Bedrock
     ['TSM2','3','raw:TSM2','boulders'], // Boulders
     ['TSM2','4','raw:TSM2','concrete'], // Concrete
     ['TSM2','5','raw:TSM2','evaporite'], // Evaporite
     ['TSM2','6','raw:TSM2','frozen_water'], // Frozen Water
     ['TSM2','7','raw:TSM2','gravel'], // Gravel
     ['TSM2','8','raw:TSM2','lava_flow'], // Lava Flow
     ['TSM2','9','raw:TSM2','loess'], // Loess
     ['TSM2','10','raw:TSM2','mud'], // Mud
     ['TSM2','11','raw:TSM2','paved'], // Paved
     ['TSM2','12','raw:TSM2','rock'], // Rock
     ['TSM2','13','raw:TSM2','sand'], // Sand
     ['TSM2','14','raw:TSM2','soil'], // Soil
     ['TSM2','999','raw:TSM2','other'], // Other

     // TSM3 - Terrain Surface Material [3]
     // ['TSM3','-999999',undefined,undefined], // noInformation
     ['TSM3','1','raw:TSM3','asphalt'], // Asphalt
     ['TSM3','2','raw:TSM3','bedrock'], // Bedrock
     ['TSM3','3','raw:TSM3','boulders'], // Boulders
     ['TSM3','4','raw:TSM3','concrete'], // Concrete
     ['TSM3','5','raw:TSM3','evaporite'], // Evaporite
     ['TSM3','6','raw:TSM3','frozen_water'], // Frozen Water
     ['TSM3','7','raw:TSM3','gravel'], // Gravel
     ['TSM3','8','raw:TSM3','lava_flow'], // Lava Flow
     ['TSM3','9','raw:TSM3','loess'], // Loess
     ['TSM3','10','raw:TSM3','mud'], // Mud
     ['TSM3','11','raw:TSM3','paved'], // Paved
     ['TSM3','12','raw:TSM3','rock'], // Rock
     ['TSM3','13','raw:TSM3','sand'], // Sand
     ['TSM3','14','raw:TSM3','soil'], // Soil
     ['TSM3','999','raw:TSM3','other'], // Other

     // TST - Cable Suspended Shape
     // ['TST','-999999',undefined,undefined], // noInformation
     ['TST','1','raw:TST','symmetric_catenary'], // Symmetric Catenary
     ['TST','2','raw:TST','mountain_catenary'], // Mountain Catenary
     ['TST','3','raw:TST','overwater_catenary'], // Overwater Catenary
     ['TST','999','raw:TST','other'], // Other

     // TTC - Tower Type
     // ['TTC','-999999',undefined,undefined], // noInformation
     ['TTC','2','raw:TTC','observation_tower'], // Observation Tower
     ['TTC','10','raw:TTC','lookout_tower'], // Lookout Tower
     ['TTC','12','raw:TTC','fire_tower'], // Fire Tower
     ['TTC','20','raw:TTC','telecommunication_tower'], // Telecommunication Tower
     ['TTC','21','raw:TTC','guard_tower'], // Guard Tower
     ['TTC','22','raw:TTC','industrial_tower'], // Industrial Tower
     ['TTC','23','raw:TTC','drop_tower'], // Drop Tower
     ['TTC','24','raw:TTC','solar_power_tower'], // Solar Power Tower
     ['TTC','999','raw:TTC','other'], // Other

     // TTC2 - Tower Type [2]
     // ['TTC2','-999999',undefined,undefined], // noInformation
     ['TTC2','2','raw:TTC2','observation_tower'], // Observation Tower
     ['TTC2','10','raw:TTC2','lookout_tower'], // Lookout Tower
     ['TTC2','12','raw:TTC2','fire_tower'], // Fire Tower
     ['TTC2','20','raw:TTC2','telecommunication_tower'], // Telecommunication Tower
     ['TTC2','21','raw:TTC2','guard_tower'], // Guard Tower
     ['TTC2','22','raw:TTC2','industrial_tower'], // Industrial Tower
     ['TTC2','23','raw:TTC2','drop_tower'], // Drop Tower
     ['TTC2','24','raw:TTC2','solar_power_tower'], // Solar Power Tower
     ['TTC2','999','raw:TTC2','other'], // Other

     // TTC3 - Tower Type [3]
     // ['TTC3','-999999',undefined,undefined], // noInformation
     ['TTC3','2','raw:TTC3','observation_tower'], // Observation Tower
     ['TTC3','10','raw:TTC3','lookout_tower'], // Lookout Tower
     ['TTC3','12','raw:TTC3','fire_tower'], // Fire Tower
     ['TTC3','20','raw:TTC3','telecommunication_tower'], // Telecommunication Tower
     ['TTC3','21','raw:TTC3','guard_tower'], // Guard Tower
     ['TTC3','22','raw:TTC3','industrial_tower'], // Industrial Tower
     ['TTC3','23','raw:TTC3','drop_tower'], // Drop Tower
     ['TTC3','24','raw:TTC3','solar_power_tower'], // Solar Power Tower
     ['TTC3','999','raw:TTC3','other'], // Other

     // TTY - Tomb Type
     // ['TTY','-999999',undefined,undefined], // noInformation
     ['TTY','1','raw:TTY','cave'], // Cave
     ['TTY','2','raw:TTY','catacomb'], // Catacomb
     ['TTY','3','raw:TTY','crypt'], // Crypt
     ['TTY','4','raw:TTY','surface_vault'], // Surface Vault
     ['TTY','5','raw:TTY','mausoleum'], // Mausoleum
     ['TTY','6','raw:TTY','burial_mound'], // Burial Mound
     ['TTY','7','raw:TTY','columbarium'], // Columbarium
     ['TTY','999','raw:TTY','other'], // Other

     // TXP - Taxiway Type
     // ['TXP','-999999',undefined,undefined], // noInformation
     ['TXP','1','raw:TXP','parallel_taxiway'], // Parallel Taxiway
     ['TXP','2','raw:TXP','stub_taxiway'], // Stub Taxiway
     ['TXP','3','raw:TXP','rapid_exit_and/or_turnoff_taxiway'], // Rapid Exit and/or Turnoff Taxiway
     ['TXP','4','raw:TXP','turnaround_taxiway'], // Turnaround Taxiway
     ['TXP','5','raw:TXP','dispersal'], // Dispersal
     ['TXP','6','raw:TXP','loop'], // Loop
     ['TXP','7','raw:TXP','perimeter'], // Perimeter
     ['TXP','8','raw:TXP','apron_taxiway'], // Apron Taxiway
     ['TXP','9','raw:TXP','aircraft_stand_taxilane'], // Aircraft Stand Taxilane
     ['TXP','10','raw:TXP','lead-in_taxilane'], // Lead-in Taxilane
     ['TXP','11','raw:TXP','lead-out_taxilane'], // Lead-out Taxilane
     ['TXP','12','raw:TXP','air_taxiway'], // Air Taxiway
     ['TXP','13','raw:TXP','helicopter_ground_taxiway'], // Helicopter Ground Taxiway
     ['TXP','999','raw:TXP','other'], // Other

     // UMA - Underground Mine Access
     // ['UMA','-999999',undefined,undefined], // noInformation
     ['UMA','1','raw:UMA','drift'], // Drift
     ['UMA','2','raw:UMA','slope'], // Slope
     ['UMA','3','raw:UMA','shaft'], // Shaft
     ['UMA','998','raw:UMA','not_applicable'], // Not Applicable

     // VCA - Void Collection Reason
     // ['VCA','-999999',undefined,undefined], // noInformation
     ['VCA','1','raw:VCA','not_requested'], // Not Requested
     ['VCA','2','raw:VCA','inaccessible'], // Inaccessible
     ['VCA','3','raw:VCA','no_available_imagery'], // No Available Imagery
     ['VCA','6','raw:VCA','no_available_map_source'], // No Available Map Source
     ['VCA','7','raw:VCA','no_suitable_imagery'], // No Suitable Imagery
     ['VCA','8','raw:VCA','not_required'], // Not Required
     ['VCA','9','raw:VCA','cloud_cover'], // Cloud Cover
     ['VCA','10','raw:VCA','snow_cover'], // Snow Cover
     ['VCA','11','raw:VCA','dark_shade'], // Dark Shade
     ['VCA','12','raw:VCA','vegetation_cover'], // Vegetation Cover
     ['VCA','13','raw:VCA','flooded'], // Flooded
     ['VCA','16','raw:VCA','no_available_survey'], // No Available Survey
     ['VCA','999','raw:VCA','other'], // Other

     // VCA2 - Void Collection Reason [2]
     // ['VCA2','-999999',undefined,undefined], // noInformation
     ['VCA2','1','raw:VCA2','not_requested'], // Not Requested
     ['VCA2','2','raw:VCA2','inaccessible'], // Inaccessible
     ['VCA2','3','raw:VCA2','no_available_imagery'], // No Available Imagery
     ['VCA2','6','raw:VCA2','no_available_map_source'], // No Available Map Source
     ['VCA2','7','raw:VCA2','no_suitable_imagery'], // No Suitable Imagery
     ['VCA2','8','raw:VCA2','not_required'], // Not Required
     ['VCA2','9','raw:VCA2','cloud_cover'], // Cloud Cover
     ['VCA2','10','raw:VCA2','snow_cover'], // Snow Cover
     ['VCA2','11','raw:VCA2','dark_shade'], // Dark Shade
     ['VCA2','12','raw:VCA2','vegetation_cover'], // Vegetation Cover
     ['VCA2','13','raw:VCA2','flooded'], // Flooded
     ['VCA2','16','raw:VCA2','no_available_survey'], // No Available Survey
     ['VCA2','999','raw:VCA2','other'], // Other

     // VCA3 - Void Collection Reason [3]
     // ['VCA3','-999999',undefined,undefined], // noInformation
     ['VCA3','1','raw:VCA3','not_requested'], // Not Requested
     ['VCA3','2','raw:VCA3','inaccessible'], // Inaccessible
     ['VCA3','3','raw:VCA3','no_available_imagery'], // No Available Imagery
     ['VCA3','6','raw:VCA3','no_available_map_source'], // No Available Map Source
     ['VCA3','7','raw:VCA3','no_suitable_imagery'], // No Suitable Imagery
     ['VCA3','8','raw:VCA3','not_required'], // Not Required
     ['VCA3','9','raw:VCA3','cloud_cover'], // Cloud Cover
     ['VCA3','10','raw:VCA3','snow_cover'], // Snow Cover
     ['VCA3','11','raw:VCA3','dark_shade'], // Dark Shade
     ['VCA3','12','raw:VCA3','vegetation_cover'], // Vegetation Cover
     ['VCA3','13','raw:VCA3','flooded'], // Flooded
     ['VCA3','16','raw:VCA3','no_available_survey'], // No Available Survey
     ['VCA3','999','raw:VCA3','other'], // Other

     // VCM - Vertical Construction Material
     // ['VCM','-999999',undefined,undefined], // noInformation
     ['VCM','1','raw:VCM','adobe_brick'], // Adobe Brick
     ['VCM','2','raw:VCM','aluminum'], // Aluminum
     ['VCM','3','raw:VCM','brick'], // Brick
     ['VCM','4','raw:VCM','concrete'], // Concrete
     ['VCM','5','raw:VCM','fibreglass'], // Fibreglass
     ['VCM','6','raw:VCM','glass'], // Glass
     ['VCM','7','raw:VCM','iron'], // Iron
     ['VCM','8','raw:VCM','masonry'], // Masonry
     ['VCM','9','raw:VCM','metal'], // Metal
     ['VCM','10','raw:VCM','mud-based_construction'], // Mud-based Construction
     ['VCM','11','raw:VCM','plant_material'], // Plant Material
     ['VCM','12','raw:VCM','prestressed_concrete'], // Prestressed Concrete
     ['VCM','13','raw:VCM','reinforced_concrete'], // Reinforced Concrete
     ['VCM','14','raw:VCM','sod'], // Sod
     ['VCM','15','raw:VCM','steel'], // Steel
     ['VCM','16','raw:VCM','stone'], // Stone
     ['VCM','17','raw:VCM','treated_timber'], // Treated Timber
     ['VCM','18','raw:VCM','wood'], // Wood
     ['VCM','999','raw:VCM','other'], // Other

     // VCM2 - Vertical Construction Material [2]
     // ['VCM2','-999999',undefined,undefined], // noInformation
     ['VCM2','1','raw:VCM2','adobe_brick'], // Adobe Brick
     ['VCM2','2','raw:VCM2','aluminum'], // Aluminum
     ['VCM2','3','raw:VCM2','brick'], // Brick
     ['VCM2','4','raw:VCM2','concrete'], // Concrete
     ['VCM2','5','raw:VCM2','fibreglass'], // Fibreglass
     ['VCM2','6','raw:VCM2','glass'], // Glass
     ['VCM2','7','raw:VCM2','iron'], // Iron
     ['VCM2','8','raw:VCM2','masonry'], // Masonry
     ['VCM2','9','raw:VCM2','metal'], // Metal
     ['VCM2','10','raw:VCM2','mud-based_construction'], // Mud-based Construction
     ['VCM2','11','raw:VCM2','plant_material'], // Plant Material
     ['VCM2','12','raw:VCM2','prestressed_concrete'], // Prestressed Concrete
     ['VCM2','13','raw:VCM2','reinforced_concrete'], // Reinforced Concrete
     ['VCM2','14','raw:VCM2','sod'], // Sod
     ['VCM2','15','raw:VCM2','steel'], // Steel
     ['VCM2','16','raw:VCM2','stone'], // Stone
     ['VCM2','17','raw:VCM2','treated_timber'], // Treated Timber
     ['VCM2','18','raw:VCM2','wood'], // Wood
     ['VCM2','999','raw:VCM2','other'], // Other

     // VCM3 - Vertical Construction Material [3]
     // ['VCM3','-999999',undefined,undefined], // noInformation
     ['VCM3','1','raw:VCM3','adobe_brick'], // Adobe Brick
     ['VCM3','2','raw:VCM3','aluminum'], // Aluminum
     ['VCM3','3','raw:VCM3','brick'], // Brick
     ['VCM3','4','raw:VCM3','concrete'], // Concrete
     ['VCM3','5','raw:VCM3','fibreglass'], // Fibreglass
     ['VCM3','6','raw:VCM3','glass'], // Glass
     ['VCM3','7','raw:VCM3','iron'], // Iron
     ['VCM3','8','raw:VCM3','masonry'], // Masonry
     ['VCM3','9','raw:VCM3','metal'], // Metal
     ['VCM3','10','raw:VCM3','mud-based_construction'], // Mud-based Construction
     ['VCM3','11','raw:VCM3','plant_material'], // Plant Material
     ['VCM3','12','raw:VCM3','prestressed_concrete'], // Prestressed Concrete
     ['VCM3','13','raw:VCM3','reinforced_concrete'], // Reinforced Concrete
     ['VCM3','14','raw:VCM3','sod'], // Sod
     ['VCM3','15','raw:VCM3','steel'], // Steel
     ['VCM3','16','raw:VCM3','stone'], // Stone
     ['VCM3','17','raw:VCM3','treated_timber'], // Treated Timber
     ['VCM3','18','raw:VCM3','wood'], // Wood
     ['VCM3','999','raw:VCM3','other'], // Other

     // VCT - Void Collection Type
     // ['VCT','-999999',undefined,undefined], // noInformation
     ['VCT','1','raw:VCT','hypsography'], // Hypsography
     ['VCT','3','raw:VCT','bathymetry'], // Bathymetry
     ['VCT','4','raw:VCT','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT','999','raw:VCT','other'], // Other

     // VCT2 - Void Collection Type [2]
     // ['VCT2','-999999',undefined,undefined], // noInformation
     ['VCT2','1','raw:VCT2','hypsography'], // Hypsography
     ['VCT2','3','raw:VCT2','bathymetry'], // Bathymetry
     ['VCT2','4','raw:VCT2','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT2','999','raw:VCT2','other'], // Other

     // VCT3 - Void Collection Type [3]
     // ['VCT3','-999999',undefined,undefined], // noInformation
     ['VCT3','1','raw:VCT3','hypsography'], // Hypsography
     ['VCT3','3','raw:VCT3','bathymetry'], // Bathymetry
     ['VCT3','4','raw:VCT3','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT3','999','raw:VCT3','other'], // Other

     // VDT - Vertical Datum
     ['VDT','groundLevel','raw:VDT','ground_level'], // Ground Level
     ['VDT','meanSeaLevel','raw:VDT','mean_sea_level_(msl)'], // Mean Sea Level (MSL)
     ['VDT','navd88','raw:VDT','north_american_vertical_datum_(navd)_1988'], // North American Vertical Datum (NAVD) 1988
     ['VDT','ngvd29','raw:VDT','national_geodetic_vertical_datum_(ngvd)_1929'], // National Geodetic Vertical Datum (NGVD) 1929
     ['VDT','noInformation','raw:VDT','noinformation'], // noInformation
     ['VDT','other','raw:VDT','other'], // Other
     ['VDT','wgs84','raw:VDT','wgs_84_ellipsoid'], // WGS 84 Ellipsoid
     ['VDT','wgs84Egm08','raw:VDT','wgs_84_egm08_geoid'], // WGS 84 EGM08 Geoid
     ['VDT','wgs84Egm96','raw:VDT','wgs_84_egm96_geoid'], // WGS 84 EGM96 Geoid

     // VEG - Vegetation Characteristic
     // ['VEG','-999999',undefined,undefined], // noInformation
     ['VEG','8','raw:VEG','grassland'], // Grassland
     ['VEG','9','raw:VEG','grassland_with_trees'], // Grassland with Trees
     ['VEG','10','raw:VEG','tropical_grass'], // Tropical Grass
     ['VEG','56','raw:VEG','without_trees'], // Without Trees
     ['VEG','83','raw:VEG','meadow'], // Meadow
     ['VEG','999','raw:VEG','other'], // Other

     // VET - Vehicle Type
     // ['VET','-999999',undefined,undefined], // noInformation
     ['VET','1','raw:VET','aircraft'], // Aircraft
     ['VET','2','raw:VET','automobile'], // Automobile
     ['VET','3','raw:VET','barge'], // Barge
     ['VET','4','raw:VET','bicycle'], // Bicycle
     ['VET','5','raw:VET','boat'], // Boat
     ['VET','6','raw:VET','bus'], // Bus
     ['VET','7','raw:VET','caravanette'], // Caravanette
     ['VET','8','raw:VET','caravan'], // Caravan
     ['VET','9','raw:VET','motorcycle'], // Motorcycle
     ['VET','10','raw:VET','ship'], // Ship
     ['VET','999','raw:VET','other'], // Other

     // VGT - Volcano Shape
     // ['VGT','-999999',undefined,undefined], // noInformation
     ['VGT','1','raw:VGT','cone'], // Cone
     ['VGT','2','raw:VGT','cinder_cone'], // Cinder Cone
     ['VGT','3','raw:VGT','shield'], // Shield
     ['VGT','4','raw:VGT','caldera'], // Caldera
     ['VGT','5','raw:VGT','composite'], // Composite

     // VOA - Volcanic Activity
     // ['VOA','-999999',undefined,undefined], // noInformation
     ['VOA','1','raw:VOA','active'], // Active
     ['VOA','2','raw:VOA','dormant'], // Dormant
     ['VOA','3','raw:VOA','inactive_or_extinct'], // Inactive or Extinct

     // VSP - Vegetation Species
     // ['VSP','-999999',undefined,undefined], // noInformation
     ['VSP','8','raw:VSP','cypress'], // Cypress
     ['VSP','19','raw:VSP','mangrove'], // Mangrove
     ['VSP','22','raw:VSP','nipa'], // Nipa
     ['VSP','25','raw:VSP','palm'], // Palm
     ['VSP','34','raw:VSP','bamboo'], // Bamboo
     ['VSP','64','raw:VSP','swamp_cypress'], // Swamp Cypress
     ['VSP','999','raw:VSP','other'], // Other

     // VSP2 - Vegetation Species [2]
     // ['VSP2','-999999',undefined,undefined], // noInformation
     ['VSP2','8','raw:VSP2','cypress'], // Cypress
     ['VSP2','19','raw:VSP2','mangrove'], // Mangrove
     ['VSP2','22','raw:VSP2','nipa'], // Nipa
     ['VSP2','25','raw:VSP2','palm'], // Palm
     ['VSP2','34','raw:VSP2','bamboo'], // Bamboo
     ['VSP2','64','raw:VSP2','swamp_cypress'], // Swamp Cypress
     ['VSP2','999','raw:VSP2','other'], // Other

     // VSP3 - Vegetation Species [3]
     // ['VSP3','-999999',undefined,undefined], // noInformation
     ['VSP3','8','raw:VSP3','cypress'], // Cypress
     ['VSP3','19','raw:VSP3','mangrove'], // Mangrove
     ['VSP3','22','raw:VSP3','nipa'], // Nipa
     ['VSP3','25','raw:VSP3','palm'], // Palm
     ['VSP3','34','raw:VSP3','bamboo'], // Bamboo
     ['VSP3','64','raw:VSP3','swamp_cypress'], // Swamp Cypress
     ['VSP3','999','raw:VSP3','other'], // Other

     // WCC - Watercourse Channel Type
     // ['WCC','-999999',undefined,undefined], // noInformation
     ['WCC','1','raw:WCC','channelized_stream'], // Channelized Stream
     ['WCC','2','raw:WCC','braided_stream'], // Braided Stream
     ['WCC','3','raw:WCC','gorge'], // Gorge
     ['WCC','4','raw:WCC','wadi'], // Wadi
     ['WCC','7','raw:WCC','normal_channel'], // Normal Channel
     ['WCC','999','raw:WCC','other'], // Other

     // WEQ - Well Equipment
     // ['WEQ','-999999',undefined,undefined], // noInformation
     ['WEQ','1','raw:WEQ','wellhead'], // Wellhead
     ['WEQ','2','raw:WEQ','christmas_tree'], // Christmas Tree
     ['WEQ','3','raw:WEQ','manifold'], // Manifold
     ['WEQ','4','raw:WEQ','protective_structure'], // Protective Structure
     ['WEQ','5','raw:WEQ','pump'], // Pump
     ['WEQ','6','raw:WEQ','rod_pump'], // Rod Pump
     ['WEQ','7','raw:WEQ','separator'], // Separator
     ['WEQ','8','raw:WEQ','stock_tank'], // Stock Tank
     ['WEQ','9','raw:WEQ','treater'], // Treater
     ['WEQ','10','raw:WEQ','capped'], // Capped
     ['WEQ','999','raw:WEQ','other'], // Other

     // WEQ2 - Well Equipment [2]
     // ['WEQ2','-999999',undefined,undefined], // noInformation
     ['WEQ2','1','raw:WEQ2','wellhead'], // Wellhead
     ['WEQ2','2','raw:WEQ2','christmas_tree'], // Christmas Tree
     ['WEQ2','3','raw:WEQ2','manifold'], // Manifold
     ['WEQ2','4','raw:WEQ2','protective_structure'], // Protective Structure
     ['WEQ2','5','raw:WEQ2','pump'], // Pump
     ['WEQ2','6','raw:WEQ2','rod_pump'], // Rod Pump
     ['WEQ2','7','raw:WEQ2','separator'], // Separator
     ['WEQ2','8','raw:WEQ2','stock_tank'], // Stock Tank
     ['WEQ2','9','raw:WEQ2','treater'], // Treater
     ['WEQ2','10','raw:WEQ2','capped'], // Capped
     ['WEQ2','999','raw:WEQ2','other'], // Other

     // WEQ3 - Well Equipment [3]
     // ['WEQ3','-999999',undefined,undefined], // noInformation
     ['WEQ3','1','raw:WEQ3','wellhead'], // Wellhead
     ['WEQ3','2','raw:WEQ3','christmas_tree'], // Christmas Tree
     ['WEQ3','3','raw:WEQ3','manifold'], // Manifold
     ['WEQ3','4','raw:WEQ3','protective_structure'], // Protective Structure
     ['WEQ3','5','raw:WEQ3','pump'], // Pump
     ['WEQ3','6','raw:WEQ3','rod_pump'], // Rod Pump
     ['WEQ3','7','raw:WEQ3','separator'], // Separator
     ['WEQ3','8','raw:WEQ3','stock_tank'], // Stock Tank
     ['WEQ3','9','raw:WEQ3','treater'], // Treater
     ['WEQ3','10','raw:WEQ3','capped'], // Capped
     ['WEQ3','999','raw:WEQ3','other'], // Other

     // WFT - Well Type
     // ['WFT','-999999',undefined,undefined], // noInformation
     ['WFT','2','raw:WFT','walled-in'], // Walled-in
     ['WFT','3','raw:WFT','artesian'], // Artesian
     ['WFT','5','raw:WFT','dug_or_drilled'], // Dug or Drilled
     ['WFT','6','raw:WFT','dug'], // Dug
     ['WFT','7','raw:WFT','drilled'], // Drilled
     ['WFT','999','raw:WFT','other'], // Other

     // WLE - Water Level Effect
     // ['WLE','-999999',undefined,undefined], // noInformation
     ['WLE','1','raw:WLE','partly_submerged'], // Partly Submerged
     ['WLE','2','raw:WLE','always_dry'], // Always Dry
     ['WLE','3','raw:WLE','always_submerged'], // Always Submerged
     ['WLE','4','raw:WLE','covers_and_uncovers'], // Covers and Uncovers
     ['WLE','5','raw:WLE','awash_at_low_water'], // Awash at Low Water
     ['WLE','9','raw:WLE','awash_at_chart_datum'], // Awash at Chart Datum
     ['WLE','999','raw:WLE','other'], // Other

     // WMT - Watercourse Morphology
     // ['WMT','-999999',undefined,undefined], // noInformation
     ['WMT','1','raw:WMT','mouth'], // Mouth
     ['WMT','2','raw:WMT','pool'], // Pool
     ['WMT','3','raw:WMT','bend'], // Bend
     ['WMT','4','raw:WMT','confluence'], // Confluence
     ['WMT','5','raw:WMT','cut-off'], // Cut-off
     ['WMT','6','raw:WMT','reach'], // Reach
     ['WMT','7','raw:WMT','anabranch'], // Anabranch
     ['WMT','8','raw:WMT','canalized'], // Canalized
     ['WMT','9','raw:WMT','distributary'], // Distributary
     ['WMT','10','raw:WMT','headwaters'], // Headwaters
     ['WMT','11','raw:WMT','abandoned'], // Abandoned
     ['WMT','12','raw:WMT','meander'], // Meander
     ['WMT','999','raw:WMT','other'], // Other

     // WRT - Water Race Type
     // ['WRT','-999999',undefined,undefined], // noInformation
     ['WRT','1','raw:WRT','sluice'], // Sluice
     ['WRT','2','raw:WRT','flume'], // Flume
     ['WRT','3','raw:WRT','headrace'], // Headrace
     ['WRT','4','raw:WRT','tailrace'], // Tailrace

     // WST - Watercourse Sink Type
     // ['WST','-999999',undefined,undefined], // noInformation
     ['WST','1','raw:WST','dissipating'], // Dissipating
     ['WST','2','raw:WST','disappearing'], // Disappearing
     ['WST','3','raw:WST','sinkhole'], // Sinkhole
     ['WST','4','raw:WST','hole'], // Hole
     ['WST','999','raw:WST','other'], // Other

     // WTI - Wall Type
     // ['WTI','-999999',undefined,undefined], // noInformation
     ['WTI','1','raw:WTI','free-standing'], // Free-standing
     ['WTI','2','raw:WTI','retaining'], // Retaining
     ['WTI','999','raw:WTI','other'], // Other

     // YSU - Military Service Branch
     // ['YSU','-999999',undefined,undefined], // noInformation
     ['YSU','1','raw:YSU','air_force'], // Air Force
     ['YSU','2','raw:YSU','army'], // Army
     ['YSU','3','raw:YSU','coastguard'], // Coastguard
     ['YSU','4','raw:YSU','marines'], // Marines
     ['YSU','5','raw:YSU','navy'], // Navy
     ['YSU','7','raw:YSU','joint'], // Joint
     ['YSU','8','raw:YSU','air_national_guard'], // Air National Guard
     ['YSU','9','raw:YSU','army_national_guard'], // Army National Guard
     ['YSU','998','raw:YSU','not_applicable'], // Not Applicable
     ['YSU','999','raw:YSU','other'], // Other

     // YWQ - Water Potability
     // ['YWQ','-999999',undefined,undefined], // noInformation
     ['YWQ','1','raw:YWQ','potable'], // Potable
     ['YWQ','2','raw:YWQ','treatable'], // Treatable
     ['YWQ','3','raw:YWQ','contaminated'], // Contaminated
     ['YWQ','4','raw:YWQ','nonpotable'], // Nonpotable
     ['YWQ','999','raw:YWQ','other'], // Other

     // ZI001_SPS - Source Information : Spatial Source
     // ['ZI001_SPS','-999999',undefined,undefined], // noInformation
     ['ZI001_SPS','1000','raw:ZI001_SPS','no'],
     ['ZI001_SPS','1001','raw:ZI001_SPS','yes'],

     // ZI001_SRT - Source Information : Source Type
     // ['ZI001_SRT','-999999',undefined,undefined], // noInformation
     ['ZI001_SRT','2','raw:ZI001_SRT','aafif'], // AAFIF
     ['ZI001_SRT','7','raw:ZI001_SRT','cib1'], // CIB1
     ['ZI001_SRT','9','raw:ZI001_SRT','quickbird_imagery'], // QuickBird Imagery
     ['ZI001_SRT','10','raw:ZI001_SRT','dnc'], // DNC
     ['ZI001_SRT','21','raw:ZI001_SRT','dvof'], // DVOF
     ['ZI001_SRT','22','raw:ZI001_SRT','ffd'], // FFD
     ['ZI001_SRT','25','raw:ZI001_SRT','geonames'], // GeoNames
     ['ZI001_SRT','26','raw:ZI001_SRT','gps'], // GPS
     ['ZI001_SRT','28','raw:ZI001_SRT','ikonos_imagery'], // Ikonos Imagery
     ['ZI001_SRT','29','raw:ZI001_SRT','ntm_imagery'], // NTM Imagery
     ['ZI001_SRT','30','raw:ZI001_SRT','imagery'], // Imagery
     ['ZI001_SRT','31','raw:ZI001_SRT','itd'], // ITD
     ['ZI001_SRT','32','raw:ZI001_SRT','ivd'], // IVD
     ['ZI001_SRT','43','raw:ZI001_SRT','midb'], // MIDB
     ['ZI001_SRT','60','raw:ZI001_SRT','uvmap'], // UVMap
     ['ZI001_SRT','61','raw:ZI001_SRT','vitd'], // VITD
     ['ZI001_SRT','64','raw:ZI001_SRT','vmap_2'], // VMap 2
     ['ZI001_SRT','108','raw:ZI001_SRT','navteq_data'], // NAVTEQ Data
     ['ZI001_SRT','159','raw:ZI001_SRT','commercial_data'], // Commercial Data
     ['ZI001_SRT','160','raw:ZI001_SRT','open_source_information'], // Open Source Information
     ['ZI001_SRT','165','raw:ZI001_SRT','dafif'], // DAFIF
     ['ZI001_SRT','166','raw:ZI001_SRT','delorme_digital_atlas_of_the_earth_(dae)'], // DeLorme Digital Atlas of the Earth (DAE)
     ['ZI001_SRT','167','raw:ZI001_SRT','military_map_data'], // Military Map Data
     ['ZI001_SRT','168','raw:ZI001_SRT','non-military_map'], // Non-military Map
     ['ZI001_SRT','169','raw:ZI001_SRT','operations_data'], // Operations Data
     ['ZI001_SRT','170','raw:ZI001_SRT','sac'], // SAC
     ['ZI001_SRT','171','raw:ZI001_SRT','tomtom_data'], // TomTom Data
     ['ZI001_SRT','999','raw:ZI001_SRT','other'], // Other

     // ZI001_VSC - Source Information : Vertical Source Category
     // ['ZI001_VSC','-999999',undefined,undefined], // noInformation
     ['ZI001_VSC','1','raw:ZI001_VSC','dted_1'], // DTED 1
     ['ZI001_VSC','2','raw:ZI001_VSC','dted_2'], // DTED 2
     ['ZI001_VSC','3','raw:ZI001_VSC','no_elevations'], // No Elevations
     ['ZI001_VSC','4','raw:ZI001_VSC','reflective_surface'], // Reflective Surface
     ['ZI001_VSC','5','raw:ZI001_VSC','stereoscopic_imagery'], // Stereoscopic Imagery
     ['ZI001_VSC','6','raw:ZI001_VSC','tin_data'], // TIN Data
     ['ZI001_VSC','999','raw:ZI001_VSC','other'], // Other

     // ZI004_RCG - Feature Metadata : Process Step Information : Resource Content Originator
     // ['ZI004_RCG','-999999',undefined,undefined], // noInformation
     ['ZI004_RCG','1','raw:ZI004_RCG','u.s._army'], // U.S. Army
     ['ZI004_RCG','2','raw:ZI004_RCG','u.s._navy'], // U.S. Navy
     ['ZI004_RCG','3','raw:ZI004_RCG','u.s._air_force'], // U.S. Air Force
     ['ZI004_RCG','4','raw:ZI004_RCG','u.s._marine_corps'], // U.S. Marine Corps
     ['ZI004_RCG','5','raw:ZI004_RCG','u.s._coast_guard'], // U.S. Coast Guard
     ['ZI004_RCG','6','raw:ZI004_RCG','u.s._africa_command_(usafricom)'], // U.S. Africa Command (USAFRICOM)
     ['ZI004_RCG','7','raw:ZI004_RCG','u.s._central_command_(uscentcom)'], // U.S. Central Command (USCENTCOM)
     ['ZI004_RCG','8','raw:ZI004_RCG','u.s._european_command_(useucom)'], // U.S. European Command (USEUCOM)
     ['ZI004_RCG','9','raw:ZI004_RCG','u.s._joint_forces_command_(usjfcom)'], // U.S. Joint Forces Command (USJFCOM)
     ['ZI004_RCG','10','raw:ZI004_RCG','u.s._northern_command_(usnorthcom)'], // U.S. Northern Command (USNORTHCOM)
     ['ZI004_RCG','11','raw:ZI004_RCG','u.s._pacific_command_(pacom)'], // U.S. Pacific Command (PACOM)
     ['ZI004_RCG','12','raw:ZI004_RCG','u.s._special_operations_command_(ussocom)'], // U.S. Special Operations Command (USSOCOM)
     ['ZI004_RCG','13','raw:ZI004_RCG','u.s._southern_command_(ussouthcom)'], // U.S. Southern Command (USSOUTHCOM)
     ['ZI004_RCG','14','raw:ZI004_RCG','u.s._strategic_command_(usstratcom)'], // U.S. Strategic Command (USSTRATCOM)
     ['ZI004_RCG','15','raw:ZI004_RCG','u.s._transportation_command_(ustranscom)'], // U.S. Transportation Command (USTRANSCOM)
     ['ZI004_RCG','16','raw:ZI004_RCG','u.s._central_intelligence_agency_(cia)'], // U.S. Central Intelligence Agency (CIA)
     ['ZI004_RCG','17','raw:ZI004_RCG','u.s._defense_intelligence_agency_(dia)'], // U.S. Defense Intelligence Agency (DIA)
     ['ZI004_RCG','18','raw:ZI004_RCG','u.s._national_security_agency_(nsa)'], // U.S. National Security Agency (NSA)
     ['ZI004_RCG','19','raw:ZI004_RCG','u.s._national_geospatial-intelligence_agency_(nga)'], // U.S. National Geospatial-Intelligence Agency (NGA)
     ['ZI004_RCG','20','raw:ZI004_RCG','u.s._national_reconnaissance_office_(nro)'], // U.S. National Reconnaissance Office (NRO)
     ['ZI004_RCG','21','raw:ZI004_RCG','u.s._department_of_state'], // U.S. Department of State
     ['ZI004_RCG','22','raw:ZI004_RCG','u.s._department_of_homeland_security_(dhs)'], // U.S. Department of Homeland Security (DHS)
     ['ZI004_RCG','23','raw:ZI004_RCG','u.s._department_of_energy_(doe)'], // U.S. Department of Energy (DOE)
     ['ZI004_RCG','24','raw:ZI004_RCG','u.s._federal_bureau_of_investigation_(fbi)'], // U.S. Federal Bureau of Investigation (FBI)
     ['ZI004_RCG','25','raw:ZI004_RCG','u.s._geological_survey_(usgs)'], // U.S. Geological Survey (USGS)
     ['ZI004_RCG','26','raw:ZI004_RCG','u.s._national_civil_applications_program_(ncap)'], // U.S. National Civil Applications Program (NCAP)
     ['ZI004_RCG','27','raw:ZI004_RCG','u.s._national_oceanic_and_atmospheric_administration'], // U.S. National Oceanic and Atmospheric Administration
     ['ZI004_RCG','28','raw:ZI004_RCG','australian_geospatial-intelligence_organization_(australia)'], // Australian Geospatial-Intelligence Organization (Australia)
     ['ZI004_RCG','29','raw:ZI004_RCG','geographic_service_(belgium)'], // Geographic Service (Belgium)
     ['ZI004_RCG','30','raw:ZI004_RCG','military_topographic_service_(bulgaria)'], // Military Topographic Service (Bulgaria)
     ['ZI004_RCG','31','raw:ZI004_RCG','mapping_and_charting_establishment_(canada)'], // Mapping and Charting Establishment (Canada)
     ['ZI004_RCG','32','raw:ZI004_RCG','geographic_service_of_the_czech_armed_forces_(czech_republic)'], // Geographic Service of the Czech Armed Forces (Czech Republic)
     ['ZI004_RCG','33','raw:ZI004_RCG','defence_acquisition_and_logistics_organization_(denmark)'], // Defence Acquisition and Logistics Organization (Denmark)
     ['ZI004_RCG','34','raw:ZI004_RCG','military_geographic_group_(estonia)'], // Military Geographic Group (Estonia)
     ['ZI004_RCG','35','raw:ZI004_RCG','topographic_service_(finland)'], // Topographic Service (Finland)
     ['ZI004_RCG','36','raw:ZI004_RCG','bureau_geographie,_hydrographie,_oceanographie_et_meteorologie_(france)'], // Bureau Geographie, Hydrographie, Oceanographie et Meteorologie (France)
     ['ZI004_RCG','37','raw:ZI004_RCG','bundeswehr_geoinformation_office_(germany)'], // Bundeswehr Geoinformation Office (Germany)
     ['ZI004_RCG','38','raw:ZI004_RCG','hellenic_military_geographic_service_(greece)'], // Hellenic Military Geographic Service (Greece)
     ['ZI004_RCG','39','raw:ZI004_RCG','geoinformation_service_of_the_hungarian_defence_forces_(hungary)'], // Geoinformation Service of the Hungarian Defence Forces (Hungary)
     ['ZI004_RCG','40','raw:ZI004_RCG','defense_information_security_(italy)'], // Defense Information Security (Italy)
     ['ZI004_RCG','41','raw:ZI004_RCG','geospatial_information_agency_(latvia)'], // Geospatial Information Agency (Latvia)
     ['ZI004_RCG','42','raw:ZI004_RCG','military_mapping_centre_(lithuania)'], // Military Mapping Centre (Lithuania)
     ['ZI004_RCG','43','raw:ZI004_RCG','national_army_topographic_service_(moldova)'], // National Army Topographic Service (Moldova)
     ['ZI004_RCG','44','raw:ZI004_RCG','army_geographic_agency_(netherlands)'], // Army Geographic Agency (Netherlands)
     ['ZI004_RCG','45','raw:ZI004_RCG','geoint_new_zealand_(new_zealand)'], // GEOINT New Zealand (New Zealand)
     ['ZI004_RCG','46','raw:ZI004_RCG','military_geographic_service_(norway)'], // Military Geographic Service (Norway)
     ['ZI004_RCG','47','raw:ZI004_RCG','military_geography_division_(poland)'], // Military Geography Division (Poland)
     ['ZI004_RCG','48','raw:ZI004_RCG','army_geographic_institute_(portugal)'], // Army Geographic Institute (Portugal)
     ['ZI004_RCG','49','raw:ZI004_RCG','military_topographic_directorate_(romania)'], // Military Topographic Directorate (Romania)
     ['ZI004_RCG','50','raw:ZI004_RCG','topographic_institute_(slovakia)'], // Topographic Institute (Slovakia)
     ['ZI004_RCG','51','raw:ZI004_RCG','army_geographic_centre_(spain)'], // Army Geographic Centre (Spain)
     ['ZI004_RCG','52','raw:ZI004_RCG','swedish_armed_forces_(sweden)'], // Swedish Armed Forces (Sweden)
     ['ZI004_RCG','53','raw:ZI004_RCG','general_command_of_mapping_(turkey)'], // General Command of Mapping (Turkey)
     ['ZI004_RCG','54','raw:ZI004_RCG','defence_geographic_centre_intelligence_collection_group_(united_kingdom)'], // Defence Geographic Centre Intelligence Collection Group (United Kingdom)
     ['ZI004_RCG','55','raw:ZI004_RCG','u.s._army_geospatial_center_(agc)'], // U.S. Army Geospatial Center (AGC)
     ['ZI004_RCG','56','raw:ZI004_RCG','army_(australia)'], // Army (Australia)
     ['ZI004_RCG','57','raw:ZI004_RCG','military_geographic_division_(croatia)'], // Military Geographic Division (Croatia)
     ['ZI004_RCG','58','raw:ZI004_RCG','directorate_geospatial_information_(south_africa)'], // Directorate Geospatial Information (South Africa)
     ['ZI004_RCG','59','raw:ZI004_RCG','korean_defense_intelligence_agency_(south_korea)'], // Korean Defense Intelligence Agency (South Korea)
     ['ZI004_RCG','60','raw:ZI004_RCG','national_intelligence_service_(south_korea)'], // National Intelligence Service (South Korea)
     ['ZI004_RCG','61','raw:ZI004_RCG','imagery_support_group_(singapore)'], // Imagery Support Group (Singapore)
     ['ZI004_RCG','62','raw:ZI004_RCG','national_security_bureau_(taiwan)'], // National Security Bureau (Taiwan)
     ['ZI004_RCG','63','raw:ZI004_RCG','materiel_production_center_(taiwan)'], // Materiel Production Center (Taiwan)
     ['ZI004_RCG','64','raw:ZI004_RCG','ministry_of_defense_of_japan_(japan)'], // Ministry of Defense of Japan (Japan)
     ['ZI004_RCG','65','raw:ZI004_RCG','ministry_of_construction_and_urban_development_(mongolia)'], // Ministry of Construction and Urban Development (Mongolia)
     ['ZI004_RCG','66','raw:ZI004_RCG','national_mapping_and_resource_information_authority_(philippines)'], // National Mapping and Resource Information Authority (Philippines)
     ['ZI004_RCG','67','raw:ZI004_RCG','royal_jordanian_geographic_centre_(jordan)'], // Royal Jordanian Geographic Centre (Jordan)
     ['ZI004_RCG','68','raw:ZI004_RCG','national_survey_authority_(oman)'], // National Survey Authority (Oman)
     ['ZI004_RCG','69','raw:ZI004_RCG','armed_forces_general_headquarters_(qatar)'], // Armed Forces General Headquarters (Qatar)
     ['ZI004_RCG','70','raw:ZI004_RCG','ministry_of_defense_of_saudi_arabia_(saudi_arabia)'], // Ministry of Defense of Saudi Arabia (Saudi Arabia)
     ['ZI004_RCG','71','raw:ZI004_RCG','directorate_of_survey_(kuwait)'], // Directorate of Survey (Kuwait)
     ['ZI004_RCG','72','raw:ZI004_RCG','military_survey_department_(united_arab_emirates)'], // Military Survey Department (United Arab Emirates)
     ['ZI004_RCG','73','raw:ZI004_RCG','information_network_security_agency_(ethiopia)'], // Information Network Security Agency (Ethiopia)
     ['ZI004_RCG','74','raw:ZI004_RCG','ministry_of_state_for_defense_(kenya)'], // Ministry of State for Defense (Kenya)
     ['ZI004_RCG','75','raw:ZI004_RCG','el_instituto_nacional_de_estadistica_y_geografia_(mexico)'], // El Instituto Nacional de Estadistica y Geografia (Mexico)
     ['ZI004_RCG','76','raw:ZI004_RCG','instituto_geográfico_militar_(chile)'], // Instituto Geográfico Militar (Chile)
     ['ZI004_RCG','77','raw:ZI004_RCG','servicio_geográfico_militar_(uruguay)'], // Servicio Geográfico Militar (Uruguay)
     ['ZI004_RCG','78','raw:ZI004_RCG','dirección_del_servicio_geográfico_military_(paraguay)'], // Dirección del Servicio Geográfico Military (Paraguay)
     ['ZI004_RCG','79','raw:ZI004_RCG','instituto_geográfico_nacional_(peru)'], // Instituto Geográfico Nacional (Peru)
     ['ZI004_RCG','80','raw:ZI004_RCG','instituto_geográfico_agustín_codazzi_(colombia)'], // Instituto Geográfico Agustín Codazzi (Colombia)
     ['ZI004_RCG','81','raw:ZI004_RCG','instituto_geográfico_y_del_catastro_nacional_(el_salvador)'], // Instituto Geográfico y del Catastro Nacional (El Salvador)
     ['ZI004_RCG','82','raw:ZI004_RCG','instituto_geográfico_nacional_(guatemala)'], // Instituto Geográfico Nacional (Guatemala)
     ['ZI004_RCG','83','raw:ZI004_RCG','servicio_geográfico_militar_(guatemala)'], // Servicio Geográfico Militar (Guatemala)
     ['ZI004_RCG','84','raw:ZI004_RCG','instituto_cartográfico_militar_(dominican_republic)'], // Instituto Cartográfico Militar (Dominican Republic)
     ['ZI004_RCG','85','raw:ZI004_RCG','instituto_nicaragüense_de_estudios_terretoriales_(nicaragua)'], // Instituto Nicaragüense de Estudios Terretoriales (Nicaragua)
     ['ZI004_RCG','86','raw:ZI004_RCG','dirección_general_de_registros,_catastro,_y_geografía_(honduras)'], // Dirección General de Registros, Catastro, y Geografía (Honduras)
     ['ZI004_RCG','87','raw:ZI004_RCG','instituto_geográfico_militar_(ecuador)'], // Instituto Geográfico Militar (Ecuador)
     ['ZI004_RCG','88','raw:ZI004_RCG','instituto_geográfico_nacional_"tommy_guardia"_(panama)'], // Instituto Geográfico Nacional "Tommy Guardia" (Panama)
     ['ZI004_RCG','89','raw:ZI004_RCG','instituto_geográfico_nacional_(argentina)'], // Instituto Geográfico Nacional (Argentina)
     ['ZI004_RCG','90','raw:ZI004_RCG','diretoria_de_serviço_geográfico_(brazil)'], // Diretoria de Serviço Geográfico (Brazil)
     ['ZI004_RCG','999','raw:ZI004_RCG','other'], // Other

     // ZI013_CSP - Crop Information : Crop Species
     // ['ZI013_CSP','-999999',undefined,undefined], // noInformation
     ['ZI013_CSP','1','raw:ZI013_CSP','almond'], // Almond
     ['ZI013_CSP','2','raw:ZI013_CSP','apple'], // Apple
     ['ZI013_CSP','3','raw:ZI013_CSP','barley'], // Barley
     ['ZI013_CSP','4','raw:ZI013_CSP','berry'], // Berry
     ['ZI013_CSP','5','raw:ZI013_CSP','carob'], // Carob
     ['ZI013_CSP','6','raw:ZI013_CSP','chestnut'], // Chestnut
     ['ZI013_CSP','7','raw:ZI013_CSP','citrus'], // Citrus
     ['ZI013_CSP','8','raw:ZI013_CSP','cork-oak'], // Cork-Oak
     ['ZI013_CSP','9','raw:ZI013_CSP','cacao'], // Cacao
     ['ZI013_CSP','10','raw:ZI013_CSP','coffee'], // Coffee
     ['ZI013_CSP','11','raw:ZI013_CSP','maize'], // Maize
     ['ZI013_CSP','13','raw:ZI013_CSP','dry_crop'], // Dry Crop
     ['ZI013_CSP','14','raw:ZI013_CSP','fibre_crop'], // Fibre Crop
     ['ZI013_CSP','15','raw:ZI013_CSP','fruit_tree'], // Fruit Tree
     ['ZI013_CSP','16','raw:ZI013_CSP','grape'], // Grape
     ['ZI013_CSP','17','raw:ZI013_CSP','hazelnut'], // Hazelnut
     ['ZI013_CSP','18','raw:ZI013_CSP','hop'], // Hop
     ['ZI013_CSP','19','raw:ZI013_CSP','maple'], // Maple
     ['ZI013_CSP','20','raw:ZI013_CSP','millet'], // Millet
     ['ZI013_CSP','21','raw:ZI013_CSP','oat'], // Oat
     ['ZI013_CSP','22','raw:ZI013_CSP','oil_crop'], // Oil Crop
     ['ZI013_CSP','23','raw:ZI013_CSP','olive'], // Olive
     ['ZI013_CSP','24','raw:ZI013_CSP','peach'], // Peach
     ['ZI013_CSP','25','raw:ZI013_CSP','peanut'], // Peanut
     ['ZI013_CSP','26','raw:ZI013_CSP','potato'], // Potato
     ['ZI013_CSP','27','raw:ZI013_CSP','pulse'], // Pulse
     ['ZI013_CSP','28','raw:ZI013_CSP','rice'], // Rice
     ['ZI013_CSP','29','raw:ZI013_CSP','rubber'], // Rubber
     ['ZI013_CSP','30','raw:ZI013_CSP','rye'], // Rye
     ['ZI013_CSP','31','raw:ZI013_CSP','sisal'], // Sisal
     ['ZI013_CSP','32','raw:ZI013_CSP','sorghum'], // Sorghum
     ['ZI013_CSP','33','raw:ZI013_CSP','sugar_crop'], // Sugar Crop
     ['ZI013_CSP','34','raw:ZI013_CSP','tea'], // Tea
     ['ZI013_CSP','35','raw:ZI013_CSP','tobacco'], // Tobacco
     ['ZI013_CSP','36','raw:ZI013_CSP','tuber'], // Tuber
     ['ZI013_CSP','37','raw:ZI013_CSP','vegetable_crop'], // Vegetable Crop
     ['ZI013_CSP','38','raw:ZI013_CSP','walnut'], // Walnut
     ['ZI013_CSP','39','raw:ZI013_CSP','wheat'], // Wheat
     ['ZI013_CSP','40','raw:ZI013_CSP','date'], // Date
     ['ZI013_CSP','41','raw:ZI013_CSP','banana'], // Banana
     ['ZI013_CSP','42','raw:ZI013_CSP','oil_palm'], // Oil Palm
     ['ZI013_CSP','43','raw:ZI013_CSP','coconut'], // Coconut
     ['ZI013_CSP','44','raw:ZI013_CSP','sugar_cane'], // Sugar Cane
     ['ZI013_CSP','45','raw:ZI013_CSP','cotton'], // Cotton
     ['ZI013_CSP','46','raw:ZI013_CSP','bamboo'], // Bamboo
     ['ZI013_CSP','47','raw:ZI013_CSP','palmetto'], // Palmetto
     ['ZI013_CSP','48','raw:ZI013_CSP','timber'], // Timber
     ['ZI013_CSP','117','raw:ZI013_CSP','opium_poppy'], // Opium Poppy
     ['ZI013_CSP','152','raw:ZI013_CSP','cannabis'], // Cannabis
     ['ZI013_CSP','159','raw:ZI013_CSP','ornamental_crop'], // Ornamental Crop
     ['ZI013_CSP','999','raw:ZI013_CSP','other'], // Other

     // ZI013_CSP2 - Crop Information : Crop Species [2]
     // ['ZI013_CSP2','-999999',undefined,undefined], // noInformation
     ['ZI013_CSP2','1','raw:ZI013_CSP2','almond'], // Almond
     ['ZI013_CSP2','2','raw:ZI013_CSP2','apple'], // Apple
     ['ZI013_CSP2','3','raw:ZI013_CSP2','barley'], // Barley
     ['ZI013_CSP2','4','raw:ZI013_CSP2','berry'], // Berry
     ['ZI013_CSP2','5','raw:ZI013_CSP2','carob'], // Carob
     ['ZI013_CSP2','6','raw:ZI013_CSP2','chestnut'], // Chestnut
     ['ZI013_CSP2','7','raw:ZI013_CSP2','citrus'], // Citrus
     ['ZI013_CSP2','8','raw:ZI013_CSP2','cork-oak'], // Cork-Oak
     ['ZI013_CSP2','9','raw:ZI013_CSP2','cacao'], // Cacao
     ['ZI013_CSP2','10','raw:ZI013_CSP2','coffee'], // Coffee
     ['ZI013_CSP2','11','raw:ZI013_CSP2','maize'], // Maize
     ['ZI013_CSP2','13','raw:ZI013_CSP2','dry_crop'], // Dry Crop
     ['ZI013_CSP2','14','raw:ZI013_CSP2','fibre_crop'], // Fibre Crop
     ['ZI013_CSP2','15','raw:ZI013_CSP2','fruit_tree'], // Fruit Tree
     ['ZI013_CSP2','17','raw:ZI013_CSP2','hazelnut'], // Hazelnut
     ['ZI013_CSP2','19','raw:ZI013_CSP2','maple'], // Maple
     ['ZI013_CSP2','20','raw:ZI013_CSP2','millet'], // Millet
     ['ZI013_CSP2','21','raw:ZI013_CSP2','oat'], // Oat
     ['ZI013_CSP2','22','raw:ZI013_CSP2','oil_crop'], // Oil Crop
     ['ZI013_CSP2','23','raw:ZI013_CSP2','olive'], // Olive
     ['ZI013_CSP2','24','raw:ZI013_CSP2','peach'], // Peach
     ['ZI013_CSP2','25','raw:ZI013_CSP2','peanut'], // Peanut
     ['ZI013_CSP2','26','raw:ZI013_CSP2','potato'], // Potato
     ['ZI013_CSP2','27','raw:ZI013_CSP2','pulse'], // Pulse
     ['ZI013_CSP2','29','raw:ZI013_CSP2','rubber'], // Rubber
     ['ZI013_CSP2','30','raw:ZI013_CSP2','rye'], // Rye
     ['ZI013_CSP2','31','raw:ZI013_CSP2','sisal'], // Sisal
     ['ZI013_CSP2','32','raw:ZI013_CSP2','sorghum'], // Sorghum
     ['ZI013_CSP2','33','raw:ZI013_CSP2','sugar_crop'], // Sugar Crop
     ['ZI013_CSP2','34','raw:ZI013_CSP2','tea'], // Tea
     ['ZI013_CSP2','35','raw:ZI013_CSP2','tobacco'], // Tobacco
     ['ZI013_CSP2','36','raw:ZI013_CSP2','tuber'], // Tuber
     ['ZI013_CSP2','37','raw:ZI013_CSP2','vegetable_crop'], // Vegetable Crop
     ['ZI013_CSP2','38','raw:ZI013_CSP2','walnut'], // Walnut
     ['ZI013_CSP2','39','raw:ZI013_CSP2','wheat'], // Wheat
     ['ZI013_CSP2','40','raw:ZI013_CSP2','date'], // Date
     ['ZI013_CSP2','41','raw:ZI013_CSP2','banana'], // Banana
     ['ZI013_CSP2','42','raw:ZI013_CSP2','oil_palm'], // Oil Palm
     ['ZI013_CSP2','43','raw:ZI013_CSP2','coconut'], // Coconut
     ['ZI013_CSP2','44','raw:ZI013_CSP2','sugar_cane'], // Sugar Cane
     ['ZI013_CSP2','45','raw:ZI013_CSP2','cotton'], // Cotton
     ['ZI013_CSP2','46','raw:ZI013_CSP2','bamboo'], // Bamboo
     ['ZI013_CSP2','47','raw:ZI013_CSP2','palmetto'], // Palmetto
     ['ZI013_CSP2','48','raw:ZI013_CSP2','timber'], // Timber
     ['ZI013_CSP2','117','raw:ZI013_CSP2','opium_poppy'], // Opium Poppy
     ['ZI013_CSP2','152','raw:ZI013_CSP2','cannabis'], // Cannabis
     ['ZI013_CSP2','159','raw:ZI013_CSP2','ornamental_crop'], // Ornamental Crop
     ['ZI013_CSP2','999','raw:ZI013_CSP2','other'], // Other

     // ZI013_CSP3 - Crop Information : Crop Species [3]
     // ['ZI013_CSP3','-999999',undefined,undefined], // noInformation
     ['ZI013_CSP3','1','raw:ZI013_CSP3','almond'], // Almond
     ['ZI013_CSP3','2','raw:ZI013_CSP3','apple'], // Apple
     ['ZI013_CSP3','3','raw:ZI013_CSP3','barley'], // Barley
     ['ZI013_CSP3','4','raw:ZI013_CSP3','berry'], // Berry
     ['ZI013_CSP3','5','raw:ZI013_CSP3','carob'], // Carob
     ['ZI013_CSP3','6','raw:ZI013_CSP3','chestnut'], // Chestnut
     ['ZI013_CSP3','7','raw:ZI013_CSP3','citrus'], // Citrus
     ['ZI013_CSP3','8','raw:ZI013_CSP3','cork-oak'], // Cork-Oak
     ['ZI013_CSP3','9','raw:ZI013_CSP3','cacao'], // Cacao
     ['ZI013_CSP3','10','raw:ZI013_CSP3','coffee'], // Coffee
     ['ZI013_CSP3','11','raw:ZI013_CSP3','maize'], // Maize
     ['ZI013_CSP3','13','raw:ZI013_CSP3','dry_crop'], // Dry Crop
     ['ZI013_CSP3','14','raw:ZI013_CSP3','fibre_crop'], // Fibre Crop
     ['ZI013_CSP3','15','raw:ZI013_CSP3','fruit_tree'], // Fruit Tree
     ['ZI013_CSP3','17','raw:ZI013_CSP3','hazelnut'], // Hazelnut
     ['ZI013_CSP3','19','raw:ZI013_CSP3','maple'], // Maple
     ['ZI013_CSP3','20','raw:ZI013_CSP3','millet'], // Millet
     ['ZI013_CSP3','21','raw:ZI013_CSP3','oat'], // Oat
     ['ZI013_CSP3','22','raw:ZI013_CSP3','oil_crop'], // Oil Crop
     ['ZI013_CSP3','23','raw:ZI013_CSP3','olive'], // Olive
     ['ZI013_CSP3','24','raw:ZI013_CSP3','peach'], // Peach
     ['ZI013_CSP3','25','raw:ZI013_CSP3','peanut'], // Peanut
     ['ZI013_CSP3','26','raw:ZI013_CSP3','potato'], // Potato
     ['ZI013_CSP3','27','raw:ZI013_CSP3','pulse'], // Pulse
     ['ZI013_CSP3','29','raw:ZI013_CSP3','rubber'], // Rubber
     ['ZI013_CSP3','30','raw:ZI013_CSP3','rye'], // Rye
     ['ZI013_CSP3','31','raw:ZI013_CSP3','sisal'], // Sisal
     ['ZI013_CSP3','32','raw:ZI013_CSP3','sorghum'], // Sorghum
     ['ZI013_CSP3','33','raw:ZI013_CSP3','sugar_crop'], // Sugar Crop
     ['ZI013_CSP3','34','raw:ZI013_CSP3','tea'], // Tea
     ['ZI013_CSP3','35','raw:ZI013_CSP3','tobacco'], // Tobacco
     ['ZI013_CSP3','36','raw:ZI013_CSP3','tuber'], // Tuber
     ['ZI013_CSP3','37','raw:ZI013_CSP3','vegetable_crop'], // Vegetable Crop
     ['ZI013_CSP3','38','raw:ZI013_CSP3','walnut'], // Walnut
     ['ZI013_CSP3','39','raw:ZI013_CSP3','wheat'], // Wheat
     ['ZI013_CSP3','40','raw:ZI013_CSP3','date'], // Date
     ['ZI013_CSP3','41','raw:ZI013_CSP3','banana'], // Banana
     ['ZI013_CSP3','42','raw:ZI013_CSP3','oil_palm'], // Oil Palm
     ['ZI013_CSP3','43','raw:ZI013_CSP3','coconut'], // Coconut
     ['ZI013_CSP3','44','raw:ZI013_CSP3','sugar_cane'], // Sugar Cane
     ['ZI013_CSP3','45','raw:ZI013_CSP3','cotton'], // Cotton
     ['ZI013_CSP3','46','raw:ZI013_CSP3','bamboo'], // Bamboo
     ['ZI013_CSP3','47','raw:ZI013_CSP3','palmetto'], // Palmetto
     ['ZI013_CSP3','48','raw:ZI013_CSP3','timber'], // Timber
     ['ZI013_CSP3','117','raw:ZI013_CSP3','opium_poppy'], // Opium Poppy
     ['ZI013_CSP3','152','raw:ZI013_CSP3','cannabis'], // Cannabis
     ['ZI013_CSP3','159','raw:ZI013_CSP3','ornamental_crop'], // Ornamental Crop
     ['ZI013_CSP3','999','raw:ZI013_CSP3','other'], // Other

     // ZI013_FFP - Crop Information : Farming Pattern
     // ['ZI013_FFP','-999999',undefined,undefined], // noInformation
     ['ZI013_FFP','1','raw:ZI013_FFP','linear'], // Linear
     ['ZI013_FFP','2','raw:ZI013_FFP','regular'], // Regular
     ['ZI013_FFP','3','raw:ZI013_FFP','terraced'], // Terraced
     ['ZI013_FFP','4','raw:ZI013_FFP','intermingled_woods'], // Intermingled Woods
     ['ZI013_FFP','5','raw:ZI013_FFP','intermingled_trees'], // Intermingled Trees
     ['ZI013_FFP','6','raw:ZI013_FFP','treeless'], // Treeless
     ['ZI013_FFP','7','raw:ZI013_FFP','trellised'], // Trellised
     ['ZI013_FFP','8','raw:ZI013_FFP','irregular'], // Irregular
     ['ZI013_FFP','999','raw:ZI013_FFP','other'], // Other

     // ZI013_FFP2 - Crop Information : Farming Pattern [2]
     // ['ZI013_FFP2','-999999',undefined,undefined], // noInformation
     ['ZI013_FFP2','1','raw:ZI013_FFP2','linear'], // Linear
     ['ZI013_FFP2','2','raw:ZI013_FFP2','regular'], // Regular
     ['ZI013_FFP2','3','raw:ZI013_FFP2','terraced'], // Terraced
     ['ZI013_FFP2','4','raw:ZI013_FFP2','intermingled_woods'], // Intermingled Woods
     ['ZI013_FFP2','5','raw:ZI013_FFP2','intermingled_trees'], // Intermingled Trees
     ['ZI013_FFP2','6','raw:ZI013_FFP2','treeless'], // Treeless
     ['ZI013_FFP2','7','raw:ZI013_FFP2','trellised'], // Trellised
     ['ZI013_FFP2','8','raw:ZI013_FFP2','irregular'], // Irregular
     ['ZI013_FFP2','999','raw:ZI013_FFP2','other'], // Other

     // ZI013_FFP3 - Crop Information : Farming Pattern [3]
     // ['ZI013_FFP3','-999999',undefined,undefined], // noInformation
     ['ZI013_FFP3','1','raw:ZI013_FFP3','linear'], // Linear
     ['ZI013_FFP3','2','raw:ZI013_FFP3','regular'], // Regular
     ['ZI013_FFP3','3','raw:ZI013_FFP3','terraced'], // Terraced
     ['ZI013_FFP3','4','raw:ZI013_FFP3','intermingled_woods'], // Intermingled Woods
     ['ZI013_FFP3','5','raw:ZI013_FFP3','intermingled_trees'], // Intermingled Trees
     ['ZI013_FFP3','6','raw:ZI013_FFP3','treeless'], // Treeless
     ['ZI013_FFP3','7','raw:ZI013_FFP3','trellised'], // Trellised
     ['ZI013_FFP3','8','raw:ZI013_FFP3','irregular'], // Irregular
     ['ZI013_FFP3','999','raw:ZI013_FFP3','other'], // Other

     // ZI013_FMM - Crop Information : Farming Method
     // ['ZI013_FMM','-999999',undefined,undefined], // noInformation
     ['ZI013_FMM','1','raw:ZI013_FMM','fallow'], // Fallow
     ['ZI013_FMM','2','raw:ZI013_FMM','grazing'], // Grazing
     ['ZI013_FMM','3','raw:ZI013_FMM','permanent'], // Permanent
     ['ZI013_FMM','4','raw:ZI013_FMM','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM','999','raw:ZI013_FMM','other'], // Other

     // ZI013_FMM2 - Crop Information : Farming Method [2]
     // ['ZI013_FMM2','-999999',undefined,undefined], // noInformation
     ['ZI013_FMM2','1','raw:ZI013_FMM2','fallow'], // Fallow
     ['ZI013_FMM2','2','raw:ZI013_FMM2','grazing'], // Grazing
     ['ZI013_FMM2','3','raw:ZI013_FMM2','permanent'], // Permanent
     ['ZI013_FMM2','4','raw:ZI013_FMM2','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM2','999','raw:ZI013_FMM2','other'], // Other

     // ZI013_FMM3 - Crop Information : Farming Method [3]
     // ['ZI013_FMM3','-999999',undefined,undefined], // noInformation
     ['ZI013_FMM3','1','raw:ZI013_FMM3','fallow'], // Fallow
     ['ZI013_FMM3','2','raw:ZI013_FMM3','grazing'], // Grazing
     ['ZI013_FMM3','3','raw:ZI013_FMM3','permanent'], // Permanent
     ['ZI013_FMM3','4','raw:ZI013_FMM3','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM3','999','raw:ZI013_FMM3','other'], // Other

     // ZI013_IRG - Crop Information : Irrigation Method
     // ['ZI013_IRG','-999999',undefined,undefined], // noInformation
     ['ZI013_IRG','1','raw:ZI013_IRG','furrow'], // Furrow
     ['ZI013_IRG','2','raw:ZI013_IRG','overhead'], // Overhead
     ['ZI013_IRG','3','raw:ZI013_IRG','center_pivot'], // Center Pivot
     ['ZI013_IRG','4','raw:ZI013_IRG','linear_move'], // Linear Move
     ['ZI013_IRG','5','raw:ZI013_IRG','lateral_move'], // Lateral Move
     ['ZI013_IRG','6','raw:ZI013_IRG','drip'], // Drip
     ['ZI013_IRG','7','raw:ZI013_IRG','subirrigation'], // Subirrigation
     ['ZI013_IRG','8','raw:ZI013_IRG','terrace'], // Terrace
     ['ZI013_IRG','999','raw:ZI013_IRG','other'], // Other

     // ZI013_PIG - Crop Information : Permanent Irrigation
     // ['ZI013_PIG','-999999',undefined,undefined], // noInformation
     ['ZI013_PIG','1000','raw:ZI013_PIG','no'],
     ['ZI013_PIG','1001','raw:ZI013_PIG','yes'],

     // ZI014_PBY - Manufacturing Information : By-product
     // ['ZI014_PBY','-999999',undefined,undefined], // noInformation
     ['ZI014_PBY','1','raw:ZI014_PBY','ash'], // Ash
     ['ZI014_PBY','2','raw:ZI014_PBY','cinders'], // Cinders
     ['ZI014_PBY','6','raw:ZI014_PBY','no_byproduct'], // No Byproduct
     ['ZI014_PBY','7','raw:ZI014_PBY','radioactive_material'], // Radioactive Material
     ['ZI014_PBY','8','raw:ZI014_PBY','refuse'], // Refuse
     ['ZI014_PBY','12','raw:ZI014_PBY','scrap-metal'], // Scrap-metal
     ['ZI014_PBY','13','raw:ZI014_PBY','sewage'], // Sewage
     ['ZI014_PBY','15','raw:ZI014_PBY','slag'], // Slag
     ['ZI014_PBY','999','raw:ZI014_PBY','other'], // Other

     // ZI014_PBY2 - Manufacturing Information : By-product [2]
     // ['ZI014_PBY2','-999999',undefined,undefined], // noInformation
     ['ZI014_PBY2','1','raw:ZI014_PBY2','ash'], // Ash
     ['ZI014_PBY2','2','raw:ZI014_PBY2','cinders'], // Cinders
     ['ZI014_PBY2','6','raw:ZI014_PBY2','no_byproduct'], // No Byproduct
     ['ZI014_PBY2','7','raw:ZI014_PBY2','radioactive_material'], // Radioactive Material
     ['ZI014_PBY2','8','raw:ZI014_PBY2','refuse'], // Refuse
     ['ZI014_PBY2','12','raw:ZI014_PBY2','scrap-metal'], // Scrap-metal
     ['ZI014_PBY2','13','raw:ZI014_PBY2','sewage'], // Sewage
     ['ZI014_PBY2','15','raw:ZI014_PBY2','slag'], // Slag
     ['ZI014_PBY2','999','raw:ZI014_PBY2','other'], // Other

     // ZI014_PBY3 - Manufacturing Information : By-product [3]
     // ['ZI014_PBY3','-999999',undefined,undefined], // noInformation
     ['ZI014_PBY3','1','raw:ZI014_PBY3','ash'], // Ash
     ['ZI014_PBY3','2','raw:ZI014_PBY3','cinders'], // Cinders
     ['ZI014_PBY3','6','raw:ZI014_PBY3','no_byproduct'], // No Byproduct
     ['ZI014_PBY3','7','raw:ZI014_PBY3','radioactive_material'], // Radioactive Material
     ['ZI014_PBY3','8','raw:ZI014_PBY3','refuse'], // Refuse
     ['ZI014_PBY3','12','raw:ZI014_PBY3','scrap-metal'], // Scrap-metal
     ['ZI014_PBY3','13','raw:ZI014_PBY3','sewage'], // Sewage
     ['ZI014_PBY3','15','raw:ZI014_PBY3','slag'], // Slag
     ['ZI014_PBY3','999','raw:ZI014_PBY3','other'], // Other

     // ZI014_PPO - Manufacturing Information : Product
     // ['ZI014_PPO','-999999',undefined,undefined], // noInformation
     ['ZI014_PPO','1','raw:ZI014_PPO','aircraft'], // Aircraft
     ['ZI014_PPO','2','raw:ZI014_PPO','aluminum'], // Aluminum
     ['ZI014_PPO','3','raw:ZI014_PPO','ammunition'], // Ammunition
     ['ZI014_PPO','4','raw:ZI014_PPO','asphalt'], // Asphalt
     ['ZI014_PPO','5','raw:ZI014_PPO','motor_vehicle'], // Motor Vehicle
     ['ZI014_PPO','13','raw:ZI014_PPO','brick'], // Brick
     ['ZI014_PPO','15','raw:ZI014_PPO','cement'], // Cement
     ['ZI014_PPO','16','raw:ZI014_PPO','chemical'], // Chemical
     ['ZI014_PPO','18','raw:ZI014_PPO','coal'], // Coal
     ['ZI014_PPO','20','raw:ZI014_PPO','coffee'], // Coffee
     ['ZI014_PPO','21','raw:ZI014_PPO','coke'], // Coke
     ['ZI014_PPO','23','raw:ZI014_PPO','concrete'], // Concrete
     ['ZI014_PPO','25','raw:ZI014_PPO','consumer_goods'], // Consumer Goods
     ['ZI014_PPO','26','raw:ZI014_PPO','copper'], // Copper
     ['ZI014_PPO','28','raw:ZI014_PPO','cotton'], // Cotton
     ['ZI014_PPO','32','raw:ZI014_PPO','desalinated_water'], // Desalinated Water
     ['ZI014_PPO','37','raw:ZI014_PPO','electric_power'], // Electric Power
     ['ZI014_PPO','38','raw:ZI014_PPO','explosive'], // Explosive
     ['ZI014_PPO','39','raw:ZI014_PPO','fish'], // Fish
     ['ZI014_PPO','41','raw:ZI014_PPO','food'], // Food
     ['ZI014_PPO','43','raw:ZI014_PPO','frozen_water'], // Frozen Water
     ['ZI014_PPO','44','raw:ZI014_PPO','fruit'], // Fruit
     ['ZI014_PPO','45','raw:ZI014_PPO','gas'], // Gas
     ['ZI014_PPO','46','raw:ZI014_PPO','petrol'], // Petrol
     ['ZI014_PPO','47','raw:ZI014_PPO','glass'], // Glass
     ['ZI014_PPO','48','raw:ZI014_PPO','gold'], // Gold
     ['ZI014_PPO','57','raw:ZI014_PPO','ice'], // Ice
     ['ZI014_PPO','58','raw:ZI014_PPO','iron'], // Iron
     ['ZI014_PPO','59','raw:ZI014_PPO','lead'], // Lead
     ['ZI014_PPO','60','raw:ZI014_PPO','lime'], // Lime
     ['ZI014_PPO','61','raw:ZI014_PPO','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['ZI014_PPO','62','raw:ZI014_PPO','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['ZI014_PPO','63','raw:ZI014_PPO','lumber'], // Lumber
     ['ZI014_PPO','65','raw:ZI014_PPO','manganese'], // Manganese
     ['ZI014_PPO','69','raw:ZI014_PPO','metal'], // Metal
     ['ZI014_PPO','70','raw:ZI014_PPO','milk'], // Milk
     ['ZI014_PPO','73','raw:ZI014_PPO','no_product'], // No Product
     ['ZI014_PPO','74','raw:ZI014_PPO','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
     ['ZI014_PPO','80','raw:ZI014_PPO','paper'], // Paper
     ['ZI014_PPO','83','raw:ZI014_PPO','petroleum'], // Petroleum
     ['ZI014_PPO','84','raw:ZI014_PPO','plastic'], // Plastic
     ['ZI014_PPO','87','raw:ZI014_PPO','prestressed_concrete'], // Prestressed Concrete
     ['ZI014_PPO','90','raw:ZI014_PPO','radioactive_material'], // Radioactive Material
     ['ZI014_PPO','92','raw:ZI014_PPO','rice'], // Rice
     ['ZI014_PPO','94','raw:ZI014_PPO','rubber'], // Rubber
     ['ZI014_PPO','95','raw:ZI014_PPO','salt'], // Salt
     ['ZI014_PPO','101','raw:ZI014_PPO','sewage'], // Sewage
     ['ZI014_PPO','105','raw:ZI014_PPO','silver'], // Silver
     ['ZI014_PPO','106','raw:ZI014_PPO','snow'], // Snow
     ['ZI014_PPO','109','raw:ZI014_PPO','steel'], // Steel
     ['ZI014_PPO','111','raw:ZI014_PPO','sugar'], // Sugar
     ['ZI014_PPO','114','raw:ZI014_PPO','textile'], // Textile
     ['ZI014_PPO','117','raw:ZI014_PPO','tobacco'], // Tobacco
     ['ZI014_PPO','120','raw:ZI014_PPO','uranium'], // Uranium
     ['ZI014_PPO','121','raw:ZI014_PPO','vegetation_product'], // Vegetation Product
     ['ZI014_PPO','122','raw:ZI014_PPO','water'], // Water
     ['ZI014_PPO','123','raw:ZI014_PPO','wine'], // Wine
     ['ZI014_PPO','126','raw:ZI014_PPO','zinc'], // Zinc
     ['ZI014_PPO','136','raw:ZI014_PPO','biochemical'], // Biochemical
     ['ZI014_PPO','137','raw:ZI014_PPO','petrochemical'], // Petrochemical
     ['ZI014_PPO','146','raw:ZI014_PPO','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['ZI014_PPO','147','raw:ZI014_PPO','electronic_equipment'], // Electronic Equipment
     ['ZI014_PPO','148','raw:ZI014_PPO','electrical_equipment'], // Electrical Equipment
     ['ZI014_PPO','150','raw:ZI014_PPO','fertilizer'], // Fertilizer
     ['ZI014_PPO','154','raw:ZI014_PPO','munitions'], // Munitions
     ['ZI014_PPO','155','raw:ZI014_PPO','olive_oil'], // Olive Oil
     ['ZI014_PPO','156','raw:ZI014_PPO','whale_products'], // Whale Products
     ['ZI014_PPO','157','raw:ZI014_PPO','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PPO','160','raw:ZI014_PPO','milled_grain'], // Milled Grain
     ['ZI014_PPO','165','raw:ZI014_PPO','clothing'], // Clothing
     ['ZI014_PPO','192','raw:ZI014_PPO','petroleum_lubricant'], // Petroleum Lubricant
     ['ZI014_PPO','214','raw:ZI014_PPO','biodiesel'], // Biodiesel
     ['ZI014_PPO','279','raw:ZI014_PPO','fluorine'], // Fluorine
     ['ZI014_PPO','325','raw:ZI014_PPO','phosphorus'], // Phosphorus
     ['ZI014_PPO','435','raw:ZI014_PPO','nuclear_fuel'], // Nuclear Fuel
     ['ZI014_PPO','999','raw:ZI014_PPO','other'], // Other

     // ZI014_PPO2 - Manufacturing Information : Product [2]
     // ['ZI014_PPO2','-999999',undefined,undefined], // noInformation
     ['ZI014_PPO2','1','raw:ZI014_PPO2','aircraft'], // Aircraft
     ['ZI014_PPO2','2','raw:ZI014_PPO2','aluminum'], // Aluminum
     ['ZI014_PPO2','3','raw:ZI014_PPO2','ammunition'], // Ammunition
     ['ZI014_PPO2','4','raw:ZI014_PPO2','asphalt'], // Asphalt
     ['ZI014_PPO2','5','raw:ZI014_PPO2','motor_vehicle'], // Motor Vehicle
     ['ZI014_PPO2','13','raw:ZI014_PPO2','brick'], // Brick
     ['ZI014_PPO2','15','raw:ZI014_PPO2','cement'], // Cement
     ['ZI014_PPO2','16','raw:ZI014_PPO2','chemical'], // Chemical
     ['ZI014_PPO2','18','raw:ZI014_PPO2','coal'], // Coal
     ['ZI014_PPO2','20','raw:ZI014_PPO2','coffee'], // Coffee
     ['ZI014_PPO2','21','raw:ZI014_PPO2','coke'], // Coke
     ['ZI014_PPO2','23','raw:ZI014_PPO2','concrete'], // Concrete
     ['ZI014_PPO2','25','raw:ZI014_PPO2','consumer_goods'], // Consumer Goods
     ['ZI014_PPO2','26','raw:ZI014_PPO2','copper'], // Copper
     ['ZI014_PPO2','28','raw:ZI014_PPO2','cotton'], // Cotton
     ['ZI014_PPO2','32','raw:ZI014_PPO2','desalinated_water'], // Desalinated Water
     ['ZI014_PPO2','37','raw:ZI014_PPO2','electric_power'], // Electric Power
     ['ZI014_PPO2','38','raw:ZI014_PPO2','explosive'], // Explosive
     ['ZI014_PPO2','39','raw:ZI014_PPO2','fish'], // Fish
     ['ZI014_PPO2','41','raw:ZI014_PPO2','food'], // Food
     ['ZI014_PPO2','43','raw:ZI014_PPO2','frozen_water'], // Frozen Water
     ['ZI014_PPO2','44','raw:ZI014_PPO2','fruit'], // Fruit
     ['ZI014_PPO2','45','raw:ZI014_PPO2','gas'], // Gas
     ['ZI014_PPO2','46','raw:ZI014_PPO2','petrol'], // Petrol
     ['ZI014_PPO2','47','raw:ZI014_PPO2','glass'], // Glass
     ['ZI014_PPO2','48','raw:ZI014_PPO2','gold'], // Gold
     ['ZI014_PPO2','57','raw:ZI014_PPO2','ice'], // Ice
     ['ZI014_PPO2','58','raw:ZI014_PPO2','iron'], // Iron
     ['ZI014_PPO2','59','raw:ZI014_PPO2','lead'], // Lead
     ['ZI014_PPO2','60','raw:ZI014_PPO2','lime'], // Lime
     ['ZI014_PPO2','61','raw:ZI014_PPO2','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['ZI014_PPO2','62','raw:ZI014_PPO2','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['ZI014_PPO2','63','raw:ZI014_PPO2','lumber'], // Lumber
     ['ZI014_PPO2','65','raw:ZI014_PPO2','manganese'], // Manganese
     ['ZI014_PPO2','69','raw:ZI014_PPO2','metal'], // Metal
     ['ZI014_PPO2','70','raw:ZI014_PPO2','milk'], // Milk
     ['ZI014_PPO2','73','raw:ZI014_PPO2','no_product'], // No Product
     ['ZI014_PPO2','74','raw:ZI014_PPO2','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
     ['ZI014_PPO2','80','raw:ZI014_PPO2','paper'], // Paper
     ['ZI014_PPO2','83','raw:ZI014_PPO2','petroleum'], // Petroleum
     ['ZI014_PPO2','84','raw:ZI014_PPO2','plastic'], // Plastic
     ['ZI014_PPO2','87','raw:ZI014_PPO2','prestressed_concrete'], // Prestressed Concrete
     ['ZI014_PPO2','90','raw:ZI014_PPO2','radioactive_material'], // Radioactive Material
     ['ZI014_PPO2','92','raw:ZI014_PPO2','rice'], // Rice
     ['ZI014_PPO2','94','raw:ZI014_PPO2','rubber'], // Rubber
     ['ZI014_PPO2','95','raw:ZI014_PPO2','salt'], // Salt
     ['ZI014_PPO2','101','raw:ZI014_PPO2','sewage'], // Sewage
     ['ZI014_PPO2','105','raw:ZI014_PPO2','silver'], // Silver
     ['ZI014_PPO2','106','raw:ZI014_PPO2','snow'], // Snow
     ['ZI014_PPO2','109','raw:ZI014_PPO2','steel'], // Steel
     ['ZI014_PPO2','111','raw:ZI014_PPO2','sugar'], // Sugar
     ['ZI014_PPO2','114','raw:ZI014_PPO2','textile'], // Textile
     ['ZI014_PPO2','117','raw:ZI014_PPO2','tobacco'], // Tobacco
     ['ZI014_PPO2','120','raw:ZI014_PPO2','uranium'], // Uranium
     ['ZI014_PPO2','121','raw:ZI014_PPO2','vegetation_product'], // Vegetation Product
     ['ZI014_PPO2','122','raw:ZI014_PPO2','water'], // Water
     ['ZI014_PPO2','123','raw:ZI014_PPO2','wine'], // Wine
     ['ZI014_PPO2','126','raw:ZI014_PPO2','zinc'], // Zinc
     ['ZI014_PPO2','136','raw:ZI014_PPO2','biochemical'], // Biochemical
     ['ZI014_PPO2','137','raw:ZI014_PPO2','petrochemical'], // Petrochemical
     ['ZI014_PPO2','146','raw:ZI014_PPO2','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['ZI014_PPO2','147','raw:ZI014_PPO2','electronic_equipment'], // Electronic Equipment
     ['ZI014_PPO2','148','raw:ZI014_PPO2','electrical_equipment'], // Electrical Equipment
     ['ZI014_PPO2','150','raw:ZI014_PPO2','fertilizer'], // Fertilizer
     ['ZI014_PPO2','154','raw:ZI014_PPO2','munitions'], // Munitions
     ['ZI014_PPO2','155','raw:ZI014_PPO2','olive_oil'], // Olive Oil
     ['ZI014_PPO2','156','raw:ZI014_PPO2','whale_products'], // Whale Products
     ['ZI014_PPO2','157','raw:ZI014_PPO2','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PPO2','160','raw:ZI014_PPO2','milled_grain'], // Milled Grain
     ['ZI014_PPO2','165','raw:ZI014_PPO2','clothing'], // Clothing
     ['ZI014_PPO2','192','raw:ZI014_PPO2','petroleum_lubricant'], // Petroleum Lubricant
     ['ZI014_PPO2','214','raw:ZI014_PPO2','biodiesel'], // Biodiesel
     ['ZI014_PPO2','279','raw:ZI014_PPO2','fluorine'], // Fluorine
     ['ZI014_PPO2','325','raw:ZI014_PPO2','phosphorus'], // Phosphorus
     ['ZI014_PPO2','435','raw:ZI014_PPO2','nuclear_fuel'], // Nuclear Fuel
     ['ZI014_PPO2','999','raw:ZI014_PPO2','other'], // Other

     // ZI014_PPO3 - Manufacturing Information : Product [3]
     // ['ZI014_PPO3','-999999',undefined,undefined], // noInformation
     ['ZI014_PPO3','1','raw:ZI014_PPO3','aircraft'], // Aircraft
     ['ZI014_PPO3','2','raw:ZI014_PPO3','aluminum'], // Aluminum
     ['ZI014_PPO3','3','raw:ZI014_PPO3','ammunition'], // Ammunition
     ['ZI014_PPO3','4','raw:ZI014_PPO3','asphalt'], // Asphalt
     ['ZI014_PPO3','5','raw:ZI014_PPO3','motor_vehicle'], // Motor Vehicle
     ['ZI014_PPO3','13','raw:ZI014_PPO3','brick'], // Brick
     ['ZI014_PPO3','15','raw:ZI014_PPO3','cement'], // Cement
     ['ZI014_PPO3','16','raw:ZI014_PPO3','chemical'], // Chemical
     ['ZI014_PPO3','18','raw:ZI014_PPO3','coal'], // Coal
     ['ZI014_PPO3','20','raw:ZI014_PPO3','coffee'], // Coffee
     ['ZI014_PPO3','21','raw:ZI014_PPO3','coke'], // Coke
     ['ZI014_PPO3','23','raw:ZI014_PPO3','concrete'], // Concrete
     ['ZI014_PPO3','25','raw:ZI014_PPO3','consumer_goods'], // Consumer Goods
     ['ZI014_PPO3','26','raw:ZI014_PPO3','copper'], // Copper
     ['ZI014_PPO3','28','raw:ZI014_PPO3','cotton'], // Cotton
     ['ZI014_PPO3','32','raw:ZI014_PPO3','desalinated_water'], // Desalinated Water
     ['ZI014_PPO3','37','raw:ZI014_PPO3','electric_power'], // Electric Power
     ['ZI014_PPO3','38','raw:ZI014_PPO3','explosive'], // Explosive
     ['ZI014_PPO3','39','raw:ZI014_PPO3','fish'], // Fish
     ['ZI014_PPO3','41','raw:ZI014_PPO3','food'], // Food
     ['ZI014_PPO3','43','raw:ZI014_PPO3','frozen_water'], // Frozen Water
     ['ZI014_PPO3','44','raw:ZI014_PPO3','fruit'], // Fruit
     ['ZI014_PPO3','45','raw:ZI014_PPO3','gas'], // Gas
     ['ZI014_PPO3','46','raw:ZI014_PPO3','petrol'], // Petrol
     ['ZI014_PPO3','47','raw:ZI014_PPO3','glass'], // Glass
     ['ZI014_PPO3','48','raw:ZI014_PPO3','gold'], // Gold
     ['ZI014_PPO3','57','raw:ZI014_PPO3','ice'], // Ice
     ['ZI014_PPO3','58','raw:ZI014_PPO3','iron'], // Iron
     ['ZI014_PPO3','59','raw:ZI014_PPO3','lead'], // Lead
     ['ZI014_PPO3','60','raw:ZI014_PPO3','lime'], // Lime
     ['ZI014_PPO3','61','raw:ZI014_PPO3','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['ZI014_PPO3','62','raw:ZI014_PPO3','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['ZI014_PPO3','63','raw:ZI014_PPO3','lumber'], // Lumber
     ['ZI014_PPO3','65','raw:ZI014_PPO3','manganese'], // Manganese
     ['ZI014_PPO3','69','raw:ZI014_PPO3','metal'], // Metal
     ['ZI014_PPO3','70','raw:ZI014_PPO3','milk'], // Milk
     ['ZI014_PPO3','73','raw:ZI014_PPO3','no_product'], // No Product
     ['ZI014_PPO3','74','raw:ZI014_PPO3','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
     ['ZI014_PPO3','80','raw:ZI014_PPO3','paper'], // Paper
     ['ZI014_PPO3','83','raw:ZI014_PPO3','petroleum'], // Petroleum
     ['ZI014_PPO3','84','raw:ZI014_PPO3','plastic'], // Plastic
     ['ZI014_PPO3','87','raw:ZI014_PPO3','prestressed_concrete'], // Prestressed Concrete
     ['ZI014_PPO3','90','raw:ZI014_PPO3','radioactive_material'], // Radioactive Material
     ['ZI014_PPO3','92','raw:ZI014_PPO3','rice'], // Rice
     ['ZI014_PPO3','94','raw:ZI014_PPO3','rubber'], // Rubber
     ['ZI014_PPO3','95','raw:ZI014_PPO3','salt'], // Salt
     ['ZI014_PPO3','101','raw:ZI014_PPO3','sewage'], // Sewage
     ['ZI014_PPO3','105','raw:ZI014_PPO3','silver'], // Silver
     ['ZI014_PPO3','106','raw:ZI014_PPO3','snow'], // Snow
     ['ZI014_PPO3','109','raw:ZI014_PPO3','steel'], // Steel
     ['ZI014_PPO3','111','raw:ZI014_PPO3','sugar'], // Sugar
     ['ZI014_PPO3','114','raw:ZI014_PPO3','textile'], // Textile
     ['ZI014_PPO3','117','raw:ZI014_PPO3','tobacco'], // Tobacco
     ['ZI014_PPO3','120','raw:ZI014_PPO3','uranium'], // Uranium
     ['ZI014_PPO3','121','raw:ZI014_PPO3','vegetation_product'], // Vegetation Product
     ['ZI014_PPO3','122','raw:ZI014_PPO3','water'], // Water
     ['ZI014_PPO3','123','raw:ZI014_PPO3','wine'], // Wine
     ['ZI014_PPO3','126','raw:ZI014_PPO3','zinc'], // Zinc
     ['ZI014_PPO3','136','raw:ZI014_PPO3','biochemical'], // Biochemical
     ['ZI014_PPO3','137','raw:ZI014_PPO3','petrochemical'], // Petrochemical
     ['ZI014_PPO3','146','raw:ZI014_PPO3','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['ZI014_PPO3','147','raw:ZI014_PPO3','electronic_equipment'], // Electronic Equipment
     ['ZI014_PPO3','148','raw:ZI014_PPO3','electrical_equipment'], // Electrical Equipment
     ['ZI014_PPO3','150','raw:ZI014_PPO3','fertilizer'], // Fertilizer
     ['ZI014_PPO3','154','raw:ZI014_PPO3','munitions'], // Munitions
     ['ZI014_PPO3','155','raw:ZI014_PPO3','olive_oil'], // Olive Oil
     ['ZI014_PPO3','156','raw:ZI014_PPO3','whale_products'], // Whale Products
     ['ZI014_PPO3','157','raw:ZI014_PPO3','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PPO3','160','raw:ZI014_PPO3','milled_grain'], // Milled Grain
     ['ZI014_PPO3','165','raw:ZI014_PPO3','clothing'], // Clothing
     ['ZI014_PPO3','192','raw:ZI014_PPO3','petroleum_lubricant'], // Petroleum Lubricant
     ['ZI014_PPO3','214','raw:ZI014_PPO3','biodiesel'], // Biodiesel
     ['ZI014_PPO3','279','raw:ZI014_PPO3','fluorine'], // Fluorine
     ['ZI014_PPO3','325','raw:ZI014_PPO3','phosphorus'], // Phosphorus
     ['ZI014_PPO3','435','raw:ZI014_PPO3','nuclear_fuel'], // Nuclear Fuel
     ['ZI014_PPO3','999','raw:ZI014_PPO3','other'], // Other

     // ZI014_PRW - Manufacturing Information : Raw Material
     // ['ZI014_PRW','-999999',undefined,undefined], // noInformation
     ['ZI014_PRW','1','raw:ZI014_PRW','aluminum'], // Aluminum
     ['ZI014_PRW','2','raw:ZI014_PRW','asphalt'], // Asphalt
     ['ZI014_PRW','5','raw:ZI014_PRW','bauxite'], // Bauxite
     ['ZI014_PRW','9','raw:ZI014_PRW','cement'], // Cement
     ['ZI014_PRW','10','raw:ZI014_PRW','chemical'], // Chemical
     ['ZI014_PRW','11','raw:ZI014_PRW','clay'], // Clay
     ['ZI014_PRW','12','raw:ZI014_PRW','coal'], // Coal
     ['ZI014_PRW','14','raw:ZI014_PRW','coke'], // Coke
     ['ZI014_PRW','16','raw:ZI014_PRW','copper'], // Copper
     ['ZI014_PRW','18','raw:ZI014_PRW','cotton'], // Cotton
     ['ZI014_PRW','27','raw:ZI014_PRW','gas'], // Gas
     ['ZI014_PRW','28','raw:ZI014_PRW','glass'], // Glass
     ['ZI014_PRW','29','raw:ZI014_PRW','gold'], // Gold
     ['ZI014_PRW','33','raw:ZI014_PRW','plant_material'], // Plant Material
     ['ZI014_PRW','38','raw:ZI014_PRW','ice'], // Ice
     ['ZI014_PRW','39','raw:ZI014_PRW','iron'], // Iron
     ['ZI014_PRW','41','raw:ZI014_PRW','lead'], // Lead
     ['ZI014_PRW','45','raw:ZI014_PRW','lumber'], // Lumber
     ['ZI014_PRW','46','raw:ZI014_PRW','manganese'], // Manganese
     ['ZI014_PRW','48','raw:ZI014_PRW','metal'], // Metal
     ['ZI014_PRW','50','raw:ZI014_PRW','no_raw_material'], // No Raw Material
     ['ZI014_PRW','52','raw:ZI014_PRW','oil'], // Oil
     ['ZI014_PRW','54','raw:ZI014_PRW','ore'], // Ore
     ['ZI014_PRW','57','raw:ZI014_PRW','paper'], // Paper
     ['ZI014_PRW','60','raw:ZI014_PRW','plastic'], // Plastic
     ['ZI014_PRW','64','raw:ZI014_PRW','radioactive_material'], // Radioactive Material
     ['ZI014_PRW','66','raw:ZI014_PRW','rubber'], // Rubber
     ['ZI014_PRW','75','raw:ZI014_PRW','sewage'], // Sewage
     ['ZI014_PRW','78','raw:ZI014_PRW','silver'], // Silver
     ['ZI014_PRW','79','raw:ZI014_PRW','snow'], // Snow
     ['ZI014_PRW','83','raw:ZI014_PRW','steel'], // Steel
     ['ZI014_PRW','85','raw:ZI014_PRW','sugar'], // Sugar
     ['ZI014_PRW','87','raw:ZI014_PRW','textile'], // Textile
     ['ZI014_PRW','90','raw:ZI014_PRW','tobacco'], // Tobacco
     ['ZI014_PRW','93','raw:ZI014_PRW','uranium'], // Uranium
     ['ZI014_PRW','94','raw:ZI014_PRW','vegetation'], // Vegetation
     ['ZI014_PRW','96','raw:ZI014_PRW','water'], // Water
     ['ZI014_PRW','97','raw:ZI014_PRW','wood'], // Wood
     ['ZI014_PRW','99','raw:ZI014_PRW','zinc'], // Zinc
     ['ZI014_PRW','118','raw:ZI014_PRW','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PRW','999','raw:ZI014_PRW','other'], // Other

     // ZI014_PRW2 - Manufacturing Information : Raw Material [2]
     // ['ZI014_PRW2','-999999',undefined,undefined], // noInformation
     ['ZI014_PRW2','1','raw:ZI014_PRW2','aluminum'], // Aluminum
     ['ZI014_PRW2','2','raw:ZI014_PRW2','asphalt'], // Asphalt
     ['ZI014_PRW2','5','raw:ZI014_PRW2','bauxite'], // Bauxite
     ['ZI014_PRW2','9','raw:ZI014_PRW2','cement'], // Cement
     ['ZI014_PRW2','10','raw:ZI014_PRW2','chemical'], // Chemical
     ['ZI014_PRW2','11','raw:ZI014_PRW2','clay'], // Clay
     ['ZI014_PRW2','12','raw:ZI014_PRW2','coal'], // Coal
     ['ZI014_PRW2','14','raw:ZI014_PRW2','coke'], // Coke
     ['ZI014_PRW2','16','raw:ZI014_PRW2','copper'], // Copper
     ['ZI014_PRW2','18','raw:ZI014_PRW2','cotton'], // Cotton
     ['ZI014_PRW2','27','raw:ZI014_PRW2','gas'], // Gas
     ['ZI014_PRW2','28','raw:ZI014_PRW2','glass'], // Glass
     ['ZI014_PRW2','29','raw:ZI014_PRW2','gold'], // Gold
     ['ZI014_PRW2','33','raw:ZI014_PRW2','plant_material'], // Plant Material
     ['ZI014_PRW2','38','raw:ZI014_PRW2','ice'], // Ice
     ['ZI014_PRW2','39','raw:ZI014_PRW2','iron'], // Iron
     ['ZI014_PRW2','41','raw:ZI014_PRW2','lead'], // Lead
     ['ZI014_PRW2','45','raw:ZI014_PRW2','lumber'], // Lumber
     ['ZI014_PRW2','46','raw:ZI014_PRW2','manganese'], // Manganese
     ['ZI014_PRW2','48','raw:ZI014_PRW2','metal'], // Metal
     ['ZI014_PRW2','50','raw:ZI014_PRW2','no_raw_material'], // No Raw Material
     ['ZI014_PRW2','52','raw:ZI014_PRW2','oil'], // Oil
     ['ZI014_PRW2','54','raw:ZI014_PRW2','ore'], // Ore
     ['ZI014_PRW2','57','raw:ZI014_PRW2','paper'], // Paper
     ['ZI014_PRW2','60','raw:ZI014_PRW2','plastic'], // Plastic
     ['ZI014_PRW2','64','raw:ZI014_PRW2','radioactive_material'], // Radioactive Material
     ['ZI014_PRW2','66','raw:ZI014_PRW2','rubber'], // Rubber
     ['ZI014_PRW2','75','raw:ZI014_PRW2','sewage'], // Sewage
     ['ZI014_PRW2','78','raw:ZI014_PRW2','silver'], // Silver
     ['ZI014_PRW2','79','raw:ZI014_PRW2','snow'], // Snow
     ['ZI014_PRW2','83','raw:ZI014_PRW2','steel'], // Steel
     ['ZI014_PRW2','85','raw:ZI014_PRW2','sugar'], // Sugar
     ['ZI014_PRW2','87','raw:ZI014_PRW2','textile'], // Textile
     ['ZI014_PRW2','90','raw:ZI014_PRW2','tobacco'], // Tobacco
     ['ZI014_PRW2','93','raw:ZI014_PRW2','uranium'], // Uranium
     ['ZI014_PRW2','94','raw:ZI014_PRW2','vegetation'], // Vegetation
     ['ZI014_PRW2','96','raw:ZI014_PRW2','water'], // Water
     ['ZI014_PRW2','97','raw:ZI014_PRW2','wood'], // Wood
     ['ZI014_PRW2','99','raw:ZI014_PRW2','zinc'], // Zinc
     ['ZI014_PRW2','118','raw:ZI014_PRW2','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PRW2','999','raw:ZI014_PRW2','other'], // Other

     // ZI014_PRW3 - Manufacturing Information : Raw Material [3]
     // ['ZI014_PRW3','-999999',undefined,undefined], // noInformation
     ['ZI014_PRW3','1','raw:ZI014_PRW3','aluminum'], // Aluminum
     ['ZI014_PRW3','2','raw:ZI014_PRW3','asphalt'], // Asphalt
     ['ZI014_PRW3','5','raw:ZI014_PRW3','bauxite'], // Bauxite
     ['ZI014_PRW3','9','raw:ZI014_PRW3','cement'], // Cement
     ['ZI014_PRW3','10','raw:ZI014_PRW3','chemical'], // Chemical
     ['ZI014_PRW3','11','raw:ZI014_PRW3','clay'], // Clay
     ['ZI014_PRW3','12','raw:ZI014_PRW3','coal'], // Coal
     ['ZI014_PRW3','14','raw:ZI014_PRW3','coke'], // Coke
     ['ZI014_PRW3','16','raw:ZI014_PRW3','copper'], // Copper
     ['ZI014_PRW3','18','raw:ZI014_PRW3','cotton'], // Cotton
     ['ZI014_PRW3','27','raw:ZI014_PRW3','gas'], // Gas
     ['ZI014_PRW3','28','raw:ZI014_PRW3','glass'], // Glass
     ['ZI014_PRW3','29','raw:ZI014_PRW3','gold'], // Gold
     ['ZI014_PRW3','33','raw:ZI014_PRW3','plant_material'], // Plant Material
     ['ZI014_PRW3','38','raw:ZI014_PRW3','ice'], // Ice
     ['ZI014_PRW3','39','raw:ZI014_PRW3','iron'], // Iron
     ['ZI014_PRW3','41','raw:ZI014_PRW3','lead'], // Lead
     ['ZI014_PRW3','45','raw:ZI014_PRW3','lumber'], // Lumber
     ['ZI014_PRW3','46','raw:ZI014_PRW3','manganese'], // Manganese
     ['ZI014_PRW3','48','raw:ZI014_PRW3','metal'], // Metal
     ['ZI014_PRW3','50','raw:ZI014_PRW3','no_raw_material'], // No Raw Material
     ['ZI014_PRW3','52','raw:ZI014_PRW3','oil'], // Oil
     ['ZI014_PRW3','54','raw:ZI014_PRW3','ore'], // Ore
     ['ZI014_PRW3','57','raw:ZI014_PRW3','paper'], // Paper
     ['ZI014_PRW3','60','raw:ZI014_PRW3','plastic'], // Plastic
     ['ZI014_PRW3','64','raw:ZI014_PRW3','radioactive_material'], // Radioactive Material
     ['ZI014_PRW3','66','raw:ZI014_PRW3','rubber'], // Rubber
     ['ZI014_PRW3','75','raw:ZI014_PRW3','sewage'], // Sewage
     ['ZI014_PRW3','78','raw:ZI014_PRW3','silver'], // Silver
     ['ZI014_PRW3','79','raw:ZI014_PRW3','snow'], // Snow
     ['ZI014_PRW3','83','raw:ZI014_PRW3','steel'], // Steel
     ['ZI014_PRW3','85','raw:ZI014_PRW3','sugar'], // Sugar
     ['ZI014_PRW3','87','raw:ZI014_PRW3','textile'], // Textile
     ['ZI014_PRW3','90','raw:ZI014_PRW3','tobacco'], // Tobacco
     ['ZI014_PRW3','93','raw:ZI014_PRW3','uranium'], // Uranium
     ['ZI014_PRW3','94','raw:ZI014_PRW3','vegetation'], // Vegetation
     ['ZI014_PRW3','96','raw:ZI014_PRW3','water'], // Water
     ['ZI014_PRW3','97','raw:ZI014_PRW3','wood'], // Wood
     ['ZI014_PRW3','99','raw:ZI014_PRW3','zinc'], // Zinc
     ['ZI014_PRW3','118','raw:ZI014_PRW3','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PRW3','999','raw:ZI014_PRW3','other'], // Other

     // ZI016_ROC - Route Pavement Information : Route Surface Composition
     // ['ZI016_ROC','-999999',undefined,undefined], // noInformation
     ['ZI016_ROC','1','raw:ZI016_ROC','unimproved'], // Unimproved
     ['ZI016_ROC','2','raw:ZI016_ROC','stabilized_earth'], // Stabilized Earth
     ['ZI016_ROC','3','raw:ZI016_ROC','flexible_pavement'], // Flexible Pavement
     ['ZI016_ROC','4','raw:ZI016_ROC','aggregate'], // Aggregate
     ['ZI016_ROC','5','raw:ZI016_ROC','macadam'], // Macadam
     ['ZI016_ROC','6','raw:ZI016_ROC','bound_surface'], // Bound Surface
     ['ZI016_ROC','7','raw:ZI016_ROC','rigid_pavement'], // Rigid Pavement
     ['ZI016_ROC','8','raw:ZI016_ROC','concrete'], // Concrete
     ['ZI016_ROC','9','raw:ZI016_ROC','asphalt'], // Asphalt
     ['ZI016_ROC','10','raw:ZI016_ROC','asphalt_over_concrete'], // Asphalt over Concrete
     ['ZI016_ROC','11','raw:ZI016_ROC','cobble-stone'], // Cobble-stone
     ['ZI016_ROC','12','raw:ZI016_ROC','brick'], // Brick
     ['ZI016_ROC','13','raw:ZI016_ROC','metal'], // Metal
     ['ZI016_ROC','14','raw:ZI016_ROC','wood'], // Wood
     ['ZI016_ROC','15','raw:ZI016_ROC','corduroy'], // Corduroy
     ['ZI016_ROC','16','raw:ZI016_ROC','wood_plank'], // Wood Plank
     ['ZI016_ROC','17','raw:ZI016_ROC','ice'], // Ice
     ['ZI016_ROC','18','raw:ZI016_ROC','snow'], // Snow
     ['ZI016_ROC','999','raw:ZI016_ROC','other'], // Other

     // ZI016_WTC - Route Pavement Information : Road Weather Restriction
     // ['ZI016_WTC','-999999',undefined,undefined], // noInformation
     ['ZI016_WTC','1','raw:ZI016_WTC','all-weather'], // All-weather
     ['ZI016_WTC','2','raw:ZI016_WTC','fair-weather'], // Fair-weather
     ['ZI016_WTC','3','raw:ZI016_WTC','winter_only'], // Winter Only
     ['ZI016_WTC','4','raw:ZI016_WTC','limited_all-weather'], // Limited All-weather
     ['ZI016_WTC','5','raw:ZI016_WTC','closed_in_winter'], // Closed in Winter
     ['ZI016_WTC','999','raw:ZI016_WTC','other'], // Other

     // ZI017_RGC - Track Information : Railway Gauge Classification
     // ['ZI017_RGC','-999999',undefined,undefined], // noInformation
     ['ZI017_RGC','1','raw:ZI017_RGC','broad'], // Broad
     ['ZI017_RGC','2','raw:ZI017_RGC','narrow'], // Narrow
     ['ZI017_RGC','3','raw:ZI017_RGC','standard'], // Standard

     // ZI017_RIR - Track Information : Railway in Road
     // ['ZI017_RIR','-999999',undefined,undefined], // noInformation
     ['ZI017_RIR','1000','raw:ZI017_RIR','no'],
     ['ZI017_RIR','1001','raw:ZI017_RIR','yes'],

     // ZI017_RRA - Track Information : Railway Power Method
     // ['ZI017_RRA','-999999',undefined,undefined], // noInformation
     ['ZI017_RRA','1','raw:ZI017_RRA','electrified_track'], // Electrified Track
     ['ZI017_RRA','3','raw:ZI017_RRA','overhead_electrified'], // Overhead Electrified
     ['ZI017_RRA','4','raw:ZI017_RRA','non-electrified'], // Non-electrified
     ['ZI017_RRA','999','raw:ZI017_RRA','other'], // Other

     // ZI017_TRT - Track Information : Track Type
     // ['ZI017_TRT','-999999',undefined,undefined], // noInformation
     ['ZI017_TRT','1','raw:ZI017_TRT','crane_track'], // Crane Track
     ['ZI017_TRT','2','raw:ZI017_TRT','drill_track'], // Drill Track
     ['ZI017_TRT','3','raw:ZI017_TRT','house_track'], // House Track
     ['ZI017_TRT','4','raw:ZI017_TRT','joint_track'], // Joint Track
     ['ZI017_TRT','5','raw:ZI017_TRT','ladder_track'], // Ladder Track
     ['ZI017_TRT','6','raw:ZI017_TRT','paired_track'], // Paired Track
     ['ZI017_TRT','7','raw:ZI017_TRT','rip_track'], // Rip Track
     ['ZI017_TRT','9','raw:ZI017_TRT','stub_track'], // Stub Track
     ['ZI017_TRT','10','raw:ZI017_TRT','team_track'], // Team Track
     ['ZI017_TRT','12','raw:ZI017_TRT','monorail'], // Monorail
     ['ZI017_TRT','13','raw:ZI017_TRT','maglev'], // Maglev
     ['ZI017_TRT','999','raw:ZI017_TRT','other'], // Other

     // ZI018_WIT - Wireless Telecommunication Information : Wireless Telecommunication Type
     // ['ZI018_WIT','-999999',undefined,undefined], // noInformation
     ['ZI018_WIT','1','raw:ZI018_WIT','cellular_phone'], // Cellular Phone
     ['ZI018_WIT','2','raw:ZI018_WIT','microwave_radio_relay'], // Microwave Radio Relay
     ['ZI018_WIT','3','raw:ZI018_WIT','mobile_phone'], // Mobile Phone
     ['ZI018_WIT','4','raw:ZI018_WIT','radio_broadcast'], // Radio Broadcast
     ['ZI018_WIT','5','raw:ZI018_WIT','radio_telephone'], // Radio Telephone
     ['ZI018_WIT','6','raw:ZI018_WIT','radio-telegraph'], // Radio-telegraph
     ['ZI018_WIT','7','raw:ZI018_WIT','television_(tv)'], // Television (TV)
     ['ZI018_WIT','999','raw:ZI018_WIT','other'], // Other

     // ZI019_ASP - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method
     // ['ZI019_ASP','-999999',undefined,undefined], // noInformation
     ['ZI019_ASP','1','raw:ZI019_ASP','aggregate_seal_coat'], // Aggregate Seal Coat
     ['ZI019_ASP','2','raw:ZI019_ASP','graded'], // Graded
     ['ZI019_ASP','3','raw:ZI019_ASP','grass'], // Grass
     ['ZI019_ASP','4','raw:ZI019_ASP','grooved'], // Grooved
     ['ZI019_ASP','5','raw:ZI019_ASP','oiled'], // Oiled
     ['ZI019_ASP','6','raw:ZI019_ASP','porous_friction_course'], // Porous Friction Course
     ['ZI019_ASP','7','raw:ZI019_ASP','rolled'], // Rolled
     ['ZI019_ASP','8','raw:ZI019_ASP','rubberized_seal_coat'], // Rubberized Seal Coat
     ['ZI019_ASP','999','raw:ZI019_ASP','other'], // Other

     // ZI019_ASP2 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [2]
     // ['ZI019_ASP2','-999999',undefined,undefined], // noInformation
     ['ZI019_ASP2','1','raw:ZI019_ASP2','aggregate_seal_coat'], // Aggregate Seal Coat
     ['ZI019_ASP2','2','raw:ZI019_ASP2','graded'], // Graded
     ['ZI019_ASP2','3','raw:ZI019_ASP2','grass'], // Grass
     ['ZI019_ASP2','4','raw:ZI019_ASP2','grooved'], // Grooved
     ['ZI019_ASP2','5','raw:ZI019_ASP2','oiled'], // Oiled
     ['ZI019_ASP2','6','raw:ZI019_ASP2','porous_friction_course'], // Porous Friction Course
     ['ZI019_ASP2','7','raw:ZI019_ASP2','rolled'], // Rolled
     ['ZI019_ASP2','8','raw:ZI019_ASP2','rubberized_seal_coat'], // Rubberized Seal Coat
     ['ZI019_ASP2','999','raw:ZI019_ASP2','other'], // Other

     // ZI019_ASP3 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [3]
     // ['ZI019_ASP3','-999999',undefined,undefined], // noInformation
     ['ZI019_ASP3','1','raw:ZI019_ASP3','aggregate_seal_coat'], // Aggregate Seal Coat
     ['ZI019_ASP3','2','raw:ZI019_ASP3','graded'], // Graded
     ['ZI019_ASP3','3','raw:ZI019_ASP3','grass'], // Grass
     ['ZI019_ASP3','4','raw:ZI019_ASP3','grooved'], // Grooved
     ['ZI019_ASP3','5','raw:ZI019_ASP3','oiled'], // Oiled
     ['ZI019_ASP3','6','raw:ZI019_ASP3','porous_friction_course'], // Porous Friction Course
     ['ZI019_ASP3','7','raw:ZI019_ASP3','rolled'], // Rolled
     ['ZI019_ASP3','8','raw:ZI019_ASP3','rubberized_seal_coat'], // Rubberized Seal Coat
     ['ZI019_ASP3','999','raw:ZI019_ASP3','other'], // Other

     // ZI019_ASU - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition
     // ['ZI019_ASU','-999999',undefined,undefined], // noInformation
     ['ZI019_ASU','1','raw:ZI019_ASU','asphalt'], // Asphalt
     ['ZI019_ASU','2','raw:ZI019_ASU','bituminous_mix'], // Bituminous Mix
     ['ZI019_ASU','3','raw:ZI019_ASU','brick'], // Brick
     ['ZI019_ASU','4','raw:ZI019_ASU','clay'], // Clay
     ['ZI019_ASU','5','raw:ZI019_ASU','concrete'], // Concrete
     ['ZI019_ASU','6','raw:ZI019_ASU','coral'], // Coral
     ['ZI019_ASU','7','raw:ZI019_ASU','earthen'], // Earthen
     ['ZI019_ASU','8','raw:ZI019_ASU','gravel'], // Gravel
     ['ZI019_ASU','9','raw:ZI019_ASU','ice'], // Ice
     ['ZI019_ASU','10','raw:ZI019_ASU','landing_mat'], // Landing Mat
     ['ZI019_ASU','11','raw:ZI019_ASU','laterite'], // Laterite
     ['ZI019_ASU','12','raw:ZI019_ASU','macadam'], // Macadam
     ['ZI019_ASU','13','raw:ZI019_ASU','membrane'], // Membrane
     ['ZI019_ASU','14','raw:ZI019_ASU','non-bituminous_mix'], // Non-bituminous Mix
     ['ZI019_ASU','15','raw:ZI019_ASU','pierced_steel_planking'], // Pierced Steel Planking
     ['ZI019_ASU','16','raw:ZI019_ASU','sand'], // Sand
     ['ZI019_ASU','17','raw:ZI019_ASU','snow'], // Snow
     ['ZI019_ASU','18','raw:ZI019_ASU','stone'], // Stone
     ['ZI019_ASU','19','raw:ZI019_ASU','water'], // Water
     ['ZI019_ASU','20','raw:ZI019_ASU','wood'], // Wood
     ['ZI019_ASU','21','raw:ZI019_ASU','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ZI019_ASU','999','raw:ZI019_ASU','other'], // Other

     // ZI019_ASU2 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [2]
     // ['ZI019_ASU2','-999999',undefined,undefined], // noInformation
     ['ZI019_ASU2','1','raw:ZI019_ASU2','asphalt'], // Asphalt
     ['ZI019_ASU2','2','raw:ZI019_ASU2','bituminous_mix'], // Bituminous Mix
     ['ZI019_ASU2','3','raw:ZI019_ASU2','brick'], // Brick
     ['ZI019_ASU2','4','raw:ZI019_ASU2','clay'], // Clay
     ['ZI019_ASU2','5','raw:ZI019_ASU2','concrete'], // Concrete
     ['ZI019_ASU2','6','raw:ZI019_ASU2','coral'], // Coral
     ['ZI019_ASU2','7','raw:ZI019_ASU2','earthen'], // Earthen
     ['ZI019_ASU2','8','raw:ZI019_ASU2','gravel'], // Gravel
     ['ZI019_ASU2','9','raw:ZI019_ASU2','ice'], // Ice
     ['ZI019_ASU2','10','raw:ZI019_ASU2','landing_mat'], // Landing Mat
     ['ZI019_ASU2','11','raw:ZI019_ASU2','laterite'], // Laterite
     ['ZI019_ASU2','12','raw:ZI019_ASU2','macadam'], // Macadam
     ['ZI019_ASU2','13','raw:ZI019_ASU2','membrane'], // Membrane
     ['ZI019_ASU2','14','raw:ZI019_ASU2','non-bituminous_mix'], // Non-bituminous Mix
     ['ZI019_ASU2','15','raw:ZI019_ASU2','pierced_steel_planking'], // Pierced Steel Planking
     ['ZI019_ASU2','16','raw:ZI019_ASU2','sand'], // Sand
     ['ZI019_ASU2','17','raw:ZI019_ASU2','snow'], // Snow
     ['ZI019_ASU2','18','raw:ZI019_ASU2','stone'], // Stone
     ['ZI019_ASU2','19','raw:ZI019_ASU2','water'], // Water
     ['ZI019_ASU2','20','raw:ZI019_ASU2','wood'], // Wood
     ['ZI019_ASU2','21','raw:ZI019_ASU2','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ZI019_ASU2','999','raw:ZI019_ASU2','other'], // Other

     // ZI019_ASU3 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [3]
     // ['ZI019_ASU3','-999999',undefined,undefined], // noInformation
     ['ZI019_ASU3','1','raw:ZI019_ASU3','asphalt'], // Asphalt
     ['ZI019_ASU3','2','raw:ZI019_ASU3','bituminous_mix'], // Bituminous Mix
     ['ZI019_ASU3','3','raw:ZI019_ASU3','brick'], // Brick
     ['ZI019_ASU3','4','raw:ZI019_ASU3','clay'], // Clay
     ['ZI019_ASU3','5','raw:ZI019_ASU3','concrete'], // Concrete
     ['ZI019_ASU3','6','raw:ZI019_ASU3','coral'], // Coral
     ['ZI019_ASU3','7','raw:ZI019_ASU3','earthen'], // Earthen
     ['ZI019_ASU3','8','raw:ZI019_ASU3','gravel'], // Gravel
     ['ZI019_ASU3','9','raw:ZI019_ASU3','ice'], // Ice
     ['ZI019_ASU3','10','raw:ZI019_ASU3','landing_mat'], // Landing Mat
     ['ZI019_ASU3','11','raw:ZI019_ASU3','laterite'], // Laterite
     ['ZI019_ASU3','12','raw:ZI019_ASU3','macadam'], // Macadam
     ['ZI019_ASU3','13','raw:ZI019_ASU3','membrane'], // Membrane
     ['ZI019_ASU3','14','raw:ZI019_ASU3','non-bituminous_mix'], // Non-bituminous Mix
     ['ZI019_ASU3','15','raw:ZI019_ASU3','pierced_steel_planking'], // Pierced Steel Planking
     ['ZI019_ASU3','16','raw:ZI019_ASU3','sand'], // Sand
     ['ZI019_ASU3','17','raw:ZI019_ASU3','snow'], // Snow
     ['ZI019_ASU3','18','raw:ZI019_ASU3','stone'], // Stone
     ['ZI019_ASU3','19','raw:ZI019_ASU3','water'], // Water
     ['ZI019_ASU3','20','raw:ZI019_ASU3','wood'], // Wood
     ['ZI019_ASU3','21','raw:ZI019_ASU3','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ZI019_ASU3','999','raw:ZI019_ASU3','other'], // Other

     // ZI019_ASX - Aerodrome Pavement Information : Aerodrome Movement Area Surface Category
     // ['ZI019_ASX','-999999',undefined,undefined], // noInformation
     ['ZI019_ASX','1','raw:ZI019_ASX','completely_paved'], // Completely Paved
     ['ZI019_ASX','2','raw:ZI019_ASX','mostly_paved'], // Mostly Paved
     ['ZI019_ASX','3','raw:ZI019_ASX','unprepared'], // Unprepared
     ['ZI019_ASX','4','raw:ZI019_ASX','partially_paved'], // Partially Paved
     ['ZI019_ASX','5','raw:ZI019_ASX','unpaved'], // Unpaved
     ['ZI019_ASX','999','raw:ZI019_ASX','other'], // Other

     // ZI019_SFS - Aerodrome Pavement Information : Aerodrome Pavement Functional Status
     // ['ZI019_SFS','-999999',undefined,undefined], // noInformation
     ['ZI019_SFS','1','raw:ZI019_SFS','fair'], // Fair
     ['ZI019_SFS','2','raw:ZI019_SFS','fair_estimated'], // Fair Estimated
     ['ZI019_SFS','3','raw:ZI019_SFS','good'], // Good
     ['ZI019_SFS','4','raw:ZI019_SFS','good_estimated'], // Good Estimated
     ['ZI019_SFS','5','raw:ZI019_SFS','poor'], // Poor
     ['ZI019_SFS','6','raw:ZI019_SFS','poor_estimated'], // Poor Estimated
     ['ZI019_SFS','7','raw:ZI019_SFS','under_construction'], // Under Construction
     ['ZI019_SFS','8','raw:ZI019_SFS','unserviceable'], // Unserviceable
     ['ZI019_SFS','9','raw:ZI019_SFS','excellent'], // Excellent

     // ZI020_GE4 - (Location Country) Designation : GENC Short URN-based Identifier
     ['ZI020_GE4','ge:GENC:3:1-2:ABW','raw:ZI020_GE4','aruba'], // Aruba
     ['ZI020_GE4','ge:GENC:3:1-2:AFG','raw:ZI020_GE4','islamic_republic_of_afghanistan'], // Islamic Republic of Afghanistan
     ['ZI020_GE4','ge:GENC:3:1-2:AGO','raw:ZI020_GE4','republic_of_angola'], // Republic of Angola
     ['ZI020_GE4','ge:GENC:3:1-2:ALB','raw:ZI020_GE4','republic_of_albania'], // Republic of Albania
     ['ZI020_GE4','ge:GENC:3:1-2:AND','raw:ZI020_GE4','principality_of_andorra'], // Principality of Andorra
     ['ZI020_GE4','ge:GENC:3:1-2:ARE','raw:ZI020_GE4','united_arab_emirates'], // United Arab Emirates
     ['ZI020_GE4','ge:GENC:3:1-2:ARG','raw:ZI020_GE4','argentine_republic'], // Argentine Republic
     ['ZI020_GE4','ge:GENC:3:1-2:ARM','raw:ZI020_GE4','republic_of_armenia'], // Republic of Armenia
     ['ZI020_GE4','ge:GENC:3:1-2:ASM','raw:ZI020_GE4','territory_of_american_samoa'], // Territory of American Samoa
     ['ZI020_GE4','ge:GENC:3:1-2:ATF','raw:ZI020_GE4','french_southern_and_antarctic_lands'], // French Southern and Antarctic Lands
     ['ZI020_GE4','ge:GENC:3:1-2:ATG','raw:ZI020_GE4','antigua_and_barbuda'], // Antigua and Barbuda
     ['ZI020_GE4','ge:GENC:3:1-2:AUS','raw:ZI020_GE4','commonwealth_of_australia'], // Commonwealth of Australia
     ['ZI020_GE4','ge:GENC:3:1-2:AUT','raw:ZI020_GE4','republic_of_austria'], // Republic of Austria
     ['ZI020_GE4','ge:GENC:3:1-2:AX1','raw:ZI020_GE4','unknown'], // Unknown
     ['ZI020_GE4','ge:GENC:3:1-2:AX2','raw:ZI020_GE4','guantanamo_bay_naval_base'], // Guantanamo Bay Naval Base
     ['ZI020_GE4','ge:GENC:3:1-2:AZE','raw:ZI020_GE4','republic_of_azerbaijan'], // Republic of Azerbaijan
     ['ZI020_GE4','ge:GENC:3:1-2:BDI','raw:ZI020_GE4','republic_of_burundi'], // Republic of Burundi
     ['ZI020_GE4','ge:GENC:3:1-2:BEL','raw:ZI020_GE4','kingdom_of_belgium'], // Kingdom of Belgium
     ['ZI020_GE4','ge:GENC:3:1-2:BEN','raw:ZI020_GE4','republic_of_benin'], // Republic of Benin
     ['ZI020_GE4','ge:GENC:3:1-2:BES','raw:ZI020_GE4','bonaire,_sint_eustatius,_and_saba'], // Bonaire, Sint Eustatius, and Saba
     ['ZI020_GE4','ge:GENC:3:1-2:BFA','raw:ZI020_GE4','burkina_faso'], // Burkina Faso
     ['ZI020_GE4','ge:GENC:3:1-2:BGD','raw:ZI020_GE4','people's_republic_of_bangladesh'], // People's Republic of Bangladesh
     ['ZI020_GE4','ge:GENC:3:1-2:BGR','raw:ZI020_GE4','republic_of_bulgaria'], // Republic of Bulgaria
     ['ZI020_GE4','ge:GENC:3:1-2:BHR','raw:ZI020_GE4','kingdom_of_bahrain'], // Kingdom of Bahrain
     ['ZI020_GE4','ge:GENC:3:1-2:BHS','raw:ZI020_GE4','commonwealth_of_the_bahamas'], // Commonwealth of The Bahamas
     ['ZI020_GE4','ge:GENC:3:1-2:BLM','raw:ZI020_GE4','saint_barthelemy'], // Saint Barthelemy
     ['ZI020_GE4','ge:GENC:3:1-2:BLR','raw:ZI020_GE4','republic_of_belarus'], // Republic of Belarus
     ['ZI020_GE4','ge:GENC:3:1-2:BOL','raw:ZI020_GE4','plurinational_state_of_bolivia'], // Plurinational State of Bolivia
     ['ZI020_GE4','ge:GENC:3:1-2:BRA','raw:ZI020_GE4','federative_republic_of_brazil'], // Federative Republic of Brazil
     ['ZI020_GE4','ge:GENC:3:1-2:BRN','raw:ZI020_GE4','brunei_darussalam'], // Brunei Darussalam
     ['ZI020_GE4','ge:GENC:3:1-2:BTN','raw:ZI020_GE4','kingdom_of_bhutan'], // Kingdom of Bhutan
     ['ZI020_GE4','ge:GENC:3:1-2:BWA','raw:ZI020_GE4','republic_of_botswana'], // Republic of Botswana
     ['ZI020_GE4','ge:GENC:3:1-2:CAF','raw:ZI020_GE4','central_african_republic'], // Central African Republic
     ['ZI020_GE4','ge:GENC:3:1-2:CAN','raw:ZI020_GE4','canada'], // Canada
     ['ZI020_GE4','ge:GENC:3:1-2:CCK','raw:ZI020_GE4','territory_of_cocos_(keeling)_islands'], // Territory of Cocos (Keeling) Islands
     ['ZI020_GE4','ge:GENC:3:1-2:CHE','raw:ZI020_GE4','swiss_confederation'], // Swiss Confederation
     ['ZI020_GE4','ge:GENC:3:1-2:CHL','raw:ZI020_GE4','republic_of_chile'], // Republic of Chile
     ['ZI020_GE4','ge:GENC:3:1-2:CHN','raw:ZI020_GE4','people's_republic_of_china'], // People's Republic of China
     ['ZI020_GE4','ge:GENC:3:1-2:CIV','raw:ZI020_GE4','republic_of_côte_d'ivoire'], // Republic of Côte d'Ivoire
     ['ZI020_GE4','ge:GENC:3:1-2:CMR','raw:ZI020_GE4','republic_of_cameroon'], // Republic of Cameroon
     ['ZI020_GE4','ge:GENC:3:1-2:COD','raw:ZI020_GE4','democratic_republic_of_the_congo'], // Democratic Republic of the Congo
     ['ZI020_GE4','ge:GENC:3:1-2:COG','raw:ZI020_GE4','republic_of_the_congo'], // Republic of the Congo
     ['ZI020_GE4','ge:GENC:3:1-2:COK','raw:ZI020_GE4','cook_islands'], // Cook Islands
     ['ZI020_GE4','ge:GENC:3:1-2:COL','raw:ZI020_GE4','republic_of_colombia'], // Republic of Colombia
     ['ZI020_GE4','ge:GENC:3:1-2:COM','raw:ZI020_GE4','union_of_the_comoros'], // Union of the Comoros
     ['ZI020_GE4','ge:GENC:3:1-2:CPT','raw:ZI020_GE4','clipperton_island'], // Clipperton Island
     ['ZI020_GE4','ge:GENC:3:1-2:CPV','raw:ZI020_GE4','republic_of_cape_verde'], // Republic of Cape Verde
     ['ZI020_GE4','ge:GENC:3:1-2:CRI','raw:ZI020_GE4','republic_of_costa_rica'], // Republic of Costa Rica
     ['ZI020_GE4','ge:GENC:3:1-2:CUB','raw:ZI020_GE4','republic_of_cuba'], // Republic of Cuba
     ['ZI020_GE4','ge:GENC:3:1-2:CUW','raw:ZI020_GE4','curaçao'], // Curaçao
     ['ZI020_GE4','ge:GENC:3:1-2:CXR','raw:ZI020_GE4','territory_of_christmas_island'], // Territory of Christmas Island
     ['ZI020_GE4','ge:GENC:3:1-2:CYM','raw:ZI020_GE4','cayman_islands'], // Cayman Islands
     ['ZI020_GE4','ge:GENC:3:1-2:CYP','raw:ZI020_GE4','republic_of_cyprus'], // Republic of Cyprus
     ['ZI020_GE4','ge:GENC:3:1-2:CZE','raw:ZI020_GE4','czech_republic'], // Czech Republic
     ['ZI020_GE4','ge:GENC:3:1-2:DEU','raw:ZI020_GE4','federal_republic_of_germany'], // Federal Republic of Germany
     ['ZI020_GE4','ge:GENC:3:1-2:DGA','raw:ZI020_GE4','diego_garcia'], // Diego Garcia
     ['ZI020_GE4','ge:GENC:3:1-2:DJI','raw:ZI020_GE4','republic_of_djibouti'], // Republic of Djibouti
     ['ZI020_GE4','ge:GENC:3:1-2:DMA','raw:ZI020_GE4','commonwealth_of_dominica'], // Commonwealth of Dominica
     ['ZI020_GE4','ge:GENC:3:1-2:DNK','raw:ZI020_GE4','kingdom_of_denmark'], // Kingdom of Denmark
     ['ZI020_GE4','ge:GENC:3:1-2:DOM','raw:ZI020_GE4','dominican_republic'], // Dominican Republic
     ['ZI020_GE4','ge:GENC:3:1-2:DZA','raw:ZI020_GE4','people's_democratic_republic_of_algeria'], // People's Democratic Republic of Algeria
     ['ZI020_GE4','ge:GENC:3:1-2:ECU','raw:ZI020_GE4','republic_of_ecuador'], // Republic of Ecuador
     ['ZI020_GE4','ge:GENC:3:1-2:EGY','raw:ZI020_GE4','arab_republic_of_egypt'], // Arab Republic of Egypt
     ['ZI020_GE4','ge:GENC:3:1-2:ERI','raw:ZI020_GE4','state_of_eritrea'], // State of Eritrea
     ['ZI020_GE4','ge:GENC:3:1-2:ESH','raw:ZI020_GE4','western_sahara'], // Western Sahara
     ['ZI020_GE4','ge:GENC:3:1-2:ESP','raw:ZI020_GE4','kingdom_of_spain'], // Kingdom of Spain
     ['ZI020_GE4','ge:GENC:3:1-2:EST','raw:ZI020_GE4','republic_of_estonia'], // Republic of Estonia
     ['ZI020_GE4','ge:GENC:3:1-2:ETH','raw:ZI020_GE4','federal_democratic_republic_of_ethiopia'], // Federal Democratic Republic of Ethiopia
     ['ZI020_GE4','ge:GENC:3:1-2:FIN','raw:ZI020_GE4','republic_of_finland'], // Republic of Finland
     ['ZI020_GE4','ge:GENC:3:1-2:FJI','raw:ZI020_GE4','republic_of_fiji'], // Republic of Fiji
     ['ZI020_GE4','ge:GENC:3:1-2:FLK','raw:ZI020_GE4','falkland_islands_(islas_malvinas)'], // Falkland Islands (Islas Malvinas)
     ['ZI020_GE4','ge:GENC:3:1-2:FRA','raw:ZI020_GE4','french_republic'], // French Republic
     ['ZI020_GE4','ge:GENC:3:1-2:FRO','raw:ZI020_GE4','faroe_islands'], // Faroe Islands
     ['ZI020_GE4','ge:GENC:3:1-2:FSM','raw:ZI020_GE4','federated_states_of_micronesia'], // Federated States of Micronesia
     ['ZI020_GE4','ge:GENC:3:1-2:GAB','raw:ZI020_GE4','gabonese_republic'], // Gabonese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:GBR','raw:ZI020_GE4','united_kingdom_of_great_britain_and_northern_ireland'], // United Kingdom of Great Britain and Northern Ireland
     ['ZI020_GE4','ge:GENC:3:1-2:GEO','raw:ZI020_GE4','georgia'], // Georgia
     ['ZI020_GE4','ge:GENC:3:1-2:GGY','raw:ZI020_GE4','bailiwick_of_guernsey'], // Bailiwick of Guernsey
     ['ZI020_GE4','ge:GENC:3:1-2:GHA','raw:ZI020_GE4','republic_of_ghana'], // Republic of Ghana
     ['ZI020_GE4','ge:GENC:3:1-2:GIN','raw:ZI020_GE4','republic_of_guinea'], // Republic of Guinea
     ['ZI020_GE4','ge:GENC:3:1-2:GLP','raw:ZI020_GE4','department_of_guadeloupe'], // Department of Guadeloupe
     ['ZI020_GE4','ge:GENC:3:1-2:GMB','raw:ZI020_GE4','republic_of_the_gambia'], // Republic of The Gambia
     ['ZI020_GE4','ge:GENC:3:1-2:GNB','raw:ZI020_GE4','republic_of_guinea-bissau'], // Republic of Guinea-Bissau
     ['ZI020_GE4','ge:GENC:3:1-2:GNQ','raw:ZI020_GE4','republic_of_equatorial_guinea'], // Republic of Equatorial Guinea
     ['ZI020_GE4','ge:GENC:3:1-2:GRC','raw:ZI020_GE4','hellenic_republic'], // Hellenic Republic
     ['ZI020_GE4','ge:GENC:3:1-2:GTM','raw:ZI020_GE4','republic_of_guatemala'], // Republic of Guatemala
     ['ZI020_GE4','ge:GENC:3:1-2:GUF','raw:ZI020_GE4','department_of_guiana'], // Department of Guiana
     ['ZI020_GE4','ge:GENC:3:1-2:GUM','raw:ZI020_GE4','territory_of_guam'], // Territory of Guam
     ['ZI020_GE4','ge:GENC:3:1-2:GUY','raw:ZI020_GE4','co-operative_republic_of_guyana'], // Co-operative Republic of Guyana
     ['ZI020_GE4','ge:GENC:3:1-2:HKG','raw:ZI020_GE4','hong_kong_special_administrative_region'], // Hong Kong Special Administrative Region
     ['ZI020_GE4','ge:GENC:3:1-2:HMD','raw:ZI020_GE4','territory_of_heard_island_and_mcdonald_islands'], // Territory of Heard Island and McDonald Islands
     ['ZI020_GE4','ge:GENC:3:1-2:HND','raw:ZI020_GE4','republic_of_honduras'], // Republic of Honduras
     ['ZI020_GE4','ge:GENC:3:1-2:HRV','raw:ZI020_GE4','republic_of_croatia'], // Republic of Croatia
     ['ZI020_GE4','ge:GENC:3:1-2:HTI','raw:ZI020_GE4','republic_of_haiti'], // Republic of Haiti
     ['ZI020_GE4','ge:GENC:3:1-2:IDN','raw:ZI020_GE4','republic_of_indonesia'], // Republic of Indonesia
     ['ZI020_GE4','ge:GENC:3:1-2:IND','raw:ZI020_GE4','republic_of_india'], // Republic of India
     ['ZI020_GE4','ge:GENC:3:1-2:IOT','raw:ZI020_GE4','british_indian_ocean_territory'], // British Indian Ocean Territory
     ['ZI020_GE4','ge:GENC:3:1-2:IRN','raw:ZI020_GE4','islamic_republic_of_iran'], // Islamic Republic of Iran
     ['ZI020_GE4','ge:GENC:3:1-2:IRQ','raw:ZI020_GE4','republic_of_iraq'], // Republic of Iraq
     ['ZI020_GE4','ge:GENC:3:1-2:ISL','raw:ZI020_GE4','republic_of_iceland'], // Republic of Iceland
     ['ZI020_GE4','ge:GENC:3:1-2:ISR','raw:ZI020_GE4','state_of_israel'], // State of Israel
     ['ZI020_GE4','ge:GENC:3:1-2:ITA','raw:ZI020_GE4','italian_republic'], // Italian Republic
     ['ZI020_GE4','ge:GENC:3:1-2:JEY','raw:ZI020_GE4','bailiwick_of_jersey'], // Bailiwick of Jersey
     ['ZI020_GE4','ge:GENC:3:1-2:JOR','raw:ZI020_GE4','hashemite_kingdom_of_jordan'], // Hashemite Kingdom of Jordan
     ['ZI020_GE4','ge:GENC:3:1-2:KAZ','raw:ZI020_GE4','republic_of_kazakhstan'], // Republic of Kazakhstan
     ['ZI020_GE4','ge:GENC:3:1-2:KEN','raw:ZI020_GE4','republic_of_kenya'], // Republic of Kenya
     ['ZI020_GE4','ge:GENC:3:1-2:KGZ','raw:ZI020_GE4','kyrgyz_republic'], // Kyrgyz Republic
     ['ZI020_GE4','ge:GENC:3:1-2:KHM','raw:ZI020_GE4','kingdom_of_cambodia'], // Kingdom of Cambodia
     ['ZI020_GE4','ge:GENC:3:1-2:KIR','raw:ZI020_GE4','republic_of_kiribati'], // Republic of Kiribati
     ['ZI020_GE4','ge:GENC:3:1-2:KNA','raw:ZI020_GE4','federation_of_saint_kitts_and_nevis'], // Federation of Saint Kitts and Nevis
     ['ZI020_GE4','ge:GENC:3:1-2:KOR','raw:ZI020_GE4','republic_of_korea'], // Republic of Korea
     ['ZI020_GE4','ge:GENC:3:1-2:KWT','raw:ZI020_GE4','state_of_kuwait'], // State of Kuwait
     ['ZI020_GE4','ge:GENC:3:1-2:LAO','raw:ZI020_GE4','lao_people's_democratic_republic'], // Lao People's Democratic Republic
     ['ZI020_GE4','ge:GENC:3:1-2:LBN','raw:ZI020_GE4','lebanese_republic'], // Lebanese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:LBR','raw:ZI020_GE4','republic_of_liberia'], // Republic of Liberia
     ['ZI020_GE4','ge:GENC:3:1-2:LIE','raw:ZI020_GE4','principality_of_liechtenstein'], // Principality of Liechtenstein
     ['ZI020_GE4','ge:GENC:3:1-2:LKA','raw:ZI020_GE4','democratic_socialist_republic_of_sri_lanka'], // Democratic Socialist Republic of Sri Lanka
     ['ZI020_GE4','ge:GENC:3:1-2:LSO','raw:ZI020_GE4','kingdom_of_lesotho'], // Kingdom of Lesotho
     ['ZI020_GE4','ge:GENC:3:1-2:LTU','raw:ZI020_GE4','republic_of_lithuania'], // Republic of Lithuania
     ['ZI020_GE4','ge:GENC:3:1-2:LUX','raw:ZI020_GE4','grand_duchy_of_luxembourg'], // Grand Duchy of Luxembourg
     ['ZI020_GE4','ge:GENC:3:1-2:LVA','raw:ZI020_GE4','republic_of_latvia'], // Republic of Latvia
     ['ZI020_GE4','ge:GENC:3:1-2:MAC','raw:ZI020_GE4','macau_special_administrative_region'], // Macau Special Administrative Region
     ['ZI020_GE4','ge:GENC:3:1-2:MAF','raw:ZI020_GE4','saint_martin'], // Saint Martin
     ['ZI020_GE4','ge:GENC:3:1-2:MAR','raw:ZI020_GE4','kingdom_of_morocco'], // Kingdom of Morocco
     ['ZI020_GE4','ge:GENC:3:1-2:MCO','raw:ZI020_GE4','principality_of_monaco'], // Principality of Monaco
     ['ZI020_GE4','ge:GENC:3:1-2:MDA','raw:ZI020_GE4','republic_of_moldova'], // Republic of Moldova
     ['ZI020_GE4','ge:GENC:3:1-2:MDG','raw:ZI020_GE4','republic_of_madagascar'], // Republic of Madagascar
     ['ZI020_GE4','ge:GENC:3:1-2:MDV','raw:ZI020_GE4','republic_of_maldives'], // Republic of Maldives
     ['ZI020_GE4','ge:GENC:3:1-2:MEX','raw:ZI020_GE4','united_mexican_states'], // United Mexican States
     ['ZI020_GE4','ge:GENC:3:1-2:MHL','raw:ZI020_GE4','republic_of_the_marshall_islands'], // Republic of the Marshall Islands
     ['ZI020_GE4','ge:GENC:3:1-2:MKD','raw:ZI020_GE4','republic_of_macedonia'], // Republic of Macedonia
     ['ZI020_GE4','ge:GENC:3:1-2:MLI','raw:ZI020_GE4','republic_of_mali'], // Republic of Mali
     ['ZI020_GE4','ge:GENC:3:1-2:MLT','raw:ZI020_GE4','republic_of_malta'], // Republic of Malta
     ['ZI020_GE4','ge:GENC:3:1-2:MMR','raw:ZI020_GE4','union_of_burma'], // Union of Burma
     ['ZI020_GE4','ge:GENC:3:1-2:MNG','raw:ZI020_GE4','mongolia'], // Mongolia
     ['ZI020_GE4','ge:GENC:3:1-2:MNP','raw:ZI020_GE4','commonwealth_of_the_northern_mariana_islands'], // Commonwealth of the Northern Mariana Islands
     ['ZI020_GE4','ge:GENC:3:1-2:MOZ','raw:ZI020_GE4','republic_of_mozambique'], // Republic of Mozambique
     ['ZI020_GE4','ge:GENC:3:1-2:MRT','raw:ZI020_GE4','islamic_republic_of_mauritania'], // Islamic Republic of Mauritania
     ['ZI020_GE4','ge:GENC:3:1-2:MTQ','raw:ZI020_GE4','department_of_martinique'], // Department of Martinique
     ['ZI020_GE4','ge:GENC:3:1-2:MUS','raw:ZI020_GE4','republic_of_mauritius'], // Republic of Mauritius
     ['ZI020_GE4','ge:GENC:3:1-2:MWI','raw:ZI020_GE4','republic_of_malawi'], // Republic of Malawi
     ['ZI020_GE4','ge:GENC:3:1-2:MYS','raw:ZI020_GE4','malaysia'], // Malaysia
     ['ZI020_GE4','ge:GENC:3:1-2:MYT','raw:ZI020_GE4','department_of_mayotte'], // Department of Mayotte
     ['ZI020_GE4','ge:GENC:3:1-2:NAM','raw:ZI020_GE4','republic_of_namibia'], // Republic of Namibia
     ['ZI020_GE4','ge:GENC:3:1-2:NCL','raw:ZI020_GE4','new_caledonia'], // New Caledonia
     ['ZI020_GE4','ge:GENC:3:1-2:NER','raw:ZI020_GE4','republic_of_the_niger'], // Republic of the Niger
     ['ZI020_GE4','ge:GENC:3:1-2:NFK','raw:ZI020_GE4','territory_of_norfolk_island'], // Territory of Norfolk Island
     ['ZI020_GE4','ge:GENC:3:1-2:NGA','raw:ZI020_GE4','federal_republic_of_nigeria'], // Federal Republic of Nigeria
     ['ZI020_GE4','ge:GENC:3:1-2:NIC','raw:ZI020_GE4','republic_of_nicaragua'], // Republic of Nicaragua
     ['ZI020_GE4','ge:GENC:3:1-2:NLD','raw:ZI020_GE4','kingdom_of_the_netherlands'], // Kingdom of the Netherlands
     ['ZI020_GE4','ge:GENC:3:1-2:NOR','raw:ZI020_GE4','kingdom_of_norway'], // Kingdom of Norway
     ['ZI020_GE4','ge:GENC:3:1-2:NPL','raw:ZI020_GE4','federal_democratic_republic_of_nepal'], // Federal Democratic Republic of Nepal
     ['ZI020_GE4','ge:GENC:3:1-2:NRU','raw:ZI020_GE4','republic_of_nauru'], // Republic of Nauru
     ['ZI020_GE4','ge:GENC:3:1-2:NZL','raw:ZI020_GE4','new_zealand'], // New Zealand
     ['ZI020_GE4','ge:GENC:3:1-2:OMN','raw:ZI020_GE4','sultanate_of_oman'], // Sultanate of Oman
     ['ZI020_GE4','ge:GENC:3:1-2:PAK','raw:ZI020_GE4','islamic_republic_of_pakistan'], // Islamic Republic of Pakistan
     ['ZI020_GE4','ge:GENC:3:1-2:PAN','raw:ZI020_GE4','republic_of_panama'], // Republic of Panama
     ['ZI020_GE4','ge:GENC:3:1-2:PCN','raw:ZI020_GE4','pitcairn,_henderson,_ducie,_and_oeno_islands'], // Pitcairn, Henderson, Ducie, and Oeno Islands
     ['ZI020_GE4','ge:GENC:3:1-2:PER','raw:ZI020_GE4','republic_of_peru'], // Republic of Peru
     ['ZI020_GE4','ge:GENC:3:1-2:PHL','raw:ZI020_GE4','republic_of_the_philippines'], // Republic of the Philippines
     ['ZI020_GE4','ge:GENC:3:1-2:PLW','raw:ZI020_GE4','republic_of_palau'], // Republic of Palau
     ['ZI020_GE4','ge:GENC:3:1-2:POL','raw:ZI020_GE4','republic_of_poland'], // Republic of Poland
     ['ZI020_GE4','ge:GENC:3:1-2:PRI','raw:ZI020_GE4','commonwealth_of_puerto_rico'], // Commonwealth of Puerto Rico
     ['ZI020_GE4','ge:GENC:3:1-2:PRK','raw:ZI020_GE4','democratic_people's_republic_of_korea'], // Democratic People's Republic of Korea
     ['ZI020_GE4','ge:GENC:3:1-2:PRT','raw:ZI020_GE4','portuguese_republic'], // Portuguese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:PRY','raw:ZI020_GE4','republic_of_paraguay'], // Republic of Paraguay
     ['ZI020_GE4','ge:GENC:3:1-2:PSE','raw:ZI020_GE4','palestinian_territory'], // Palestinian Territory
     ['ZI020_GE4','ge:GENC:3:1-2:PYF','raw:ZI020_GE4','french_polynesia'], // French Polynesia
     ['ZI020_GE4','ge:GENC:3:1-2:QAT','raw:ZI020_GE4','state_of_qatar'], // State of Qatar
     ['ZI020_GE4','ge:GENC:3:1-2:REU','raw:ZI020_GE4','department_of_reunion'], // Department of Reunion
     ['ZI020_GE4','ge:GENC:3:1-2:RUS','raw:ZI020_GE4','russian_federation'], // Russian Federation
     ['ZI020_GE4','ge:GENC:3:1-2:RWA','raw:ZI020_GE4','republic_of_rwanda'], // Republic of Rwanda
     ['ZI020_GE4','ge:GENC:3:1-2:SAU','raw:ZI020_GE4','kingdom_of_saudi_arabia'], // Kingdom of Saudi Arabia
     ['ZI020_GE4','ge:GENC:3:1-2:SDN','raw:ZI020_GE4','republic_of_the_sudan'], // Republic of the Sudan
     ['ZI020_GE4','ge:GENC:3:1-2:SEN','raw:ZI020_GE4','republic_of_senegal'], // Republic of Senegal
     ['ZI020_GE4','ge:GENC:3:1-2:SGP','raw:ZI020_GE4','republic_of_singapore'], // Republic of Singapore
     ['ZI020_GE4','ge:GENC:3:1-2:SGS','raw:ZI020_GE4','south_georgia_and_south_sandwich_islands'], // South Georgia and South Sandwich Islands
     ['ZI020_GE4','ge:GENC:3:1-2:SHN','raw:ZI020_GE4','saint_helena,_ascension,_and_tristan_da_cunha'], // Saint Helena, Ascension, and Tristan da Cunha
     ['ZI020_GE4','ge:GENC:3:1-2:SLB','raw:ZI020_GE4','solomon_islands'], // Solomon Islands
     ['ZI020_GE4','ge:GENC:3:1-2:SLE','raw:ZI020_GE4','republic_of_sierra_leone'], // Republic of Sierra Leone
     ['ZI020_GE4','ge:GENC:3:1-2:SLV','raw:ZI020_GE4','republic_of_el_salvador'], // Republic of El Salvador
     ['ZI020_GE4','ge:GENC:3:1-2:SMR','raw:ZI020_GE4','republic_of_san_marino'], // Republic of San Marino
     ['ZI020_GE4','ge:GENC:3:1-2:SOM','raw:ZI020_GE4','somalia,_federal_republic_of'], // Somalia, Federal Republic of
     ['ZI020_GE4','ge:GENC:3:1-2:SPM','raw:ZI020_GE4','territorial_collectivity_of_saint_pierre_and_miquelon'], // Territorial Collectivity of Saint Pierre and Miquelon
     ['ZI020_GE4','ge:GENC:3:1-2:SRB','raw:ZI020_GE4','republic_of_serbia'], // Republic of Serbia
     ['ZI020_GE4','ge:GENC:3:1-2:SSD','raw:ZI020_GE4','republic_of_south_sudan'], // Republic of South Sudan
     ['ZI020_GE4','ge:GENC:3:1-2:STP','raw:ZI020_GE4','democratic_republic_of_sao_tome_and_principe'], // Democratic Republic of Sao Tome and Principe
     ['ZI020_GE4','ge:GENC:3:1-2:SUR','raw:ZI020_GE4','republic_of_suriname'], // Republic of Suriname
     ['ZI020_GE4','ge:GENC:3:1-2:SVK','raw:ZI020_GE4','slovak_republic'], // Slovak Republic
     ['ZI020_GE4','ge:GENC:3:1-2:SVN','raw:ZI020_GE4','republic_of_slovenia'], // Republic of Slovenia
     ['ZI020_GE4','ge:GENC:3:1-2:SWE','raw:ZI020_GE4','kingdom_of_sweden'], // Kingdom of Sweden
     ['ZI020_GE4','ge:GENC:3:1-2:SWZ','raw:ZI020_GE4','kingdom_of_swaziland'], // Kingdom of Swaziland
     ['ZI020_GE4','ge:GENC:3:1-2:SXM','raw:ZI020_GE4','sint_maarten'], // Sint Maarten
     ['ZI020_GE4','ge:GENC:3:1-2:SYC','raw:ZI020_GE4','republic_of_seychelles'], // Republic of Seychelles
     ['ZI020_GE4','ge:GENC:3:1-2:SYR','raw:ZI020_GE4','syrian_arab_republic'], // Syrian Arab Republic
     ['ZI020_GE4','ge:GENC:3:1-2:TCA','raw:ZI020_GE4','turks_and_caicos_islands'], // Turks and Caicos Islands
     ['ZI020_GE4','ge:GENC:3:1-2:TCD','raw:ZI020_GE4','republic_of_chad'], // Republic of Chad
     ['ZI020_GE4','ge:GENC:3:1-2:TGO','raw:ZI020_GE4','togolese_republic'], // Togolese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:THA','raw:ZI020_GE4','kingdom_of_thailand'], // Kingdom of Thailand
     ['ZI020_GE4','ge:GENC:3:1-2:TJK','raw:ZI020_GE4','republic_of_tajikistan'], // Republic of Tajikistan
     ['ZI020_GE4','ge:GENC:3:1-2:TLS','raw:ZI020_GE4','democratic_republic_of_timor-leste'], // Democratic Republic of Timor-Leste
     ['ZI020_GE4','ge:GENC:3:1-2:TON','raw:ZI020_GE4','kingdom_of_tonga'], // Kingdom of Tonga
     ['ZI020_GE4','ge:GENC:3:1-2:TTO','raw:ZI020_GE4','republic_of_trinidad_and_tobago'], // Republic of Trinidad and Tobago
     ['ZI020_GE4','ge:GENC:3:1-2:TUN','raw:ZI020_GE4','tunisian_republic'], // Tunisian Republic
     ['ZI020_GE4','ge:GENC:3:1-2:TUR','raw:ZI020_GE4','republic_of_turkey'], // Republic of Turkey
     ['ZI020_GE4','ge:GENC:3:1-2:TWN','raw:ZI020_GE4','taiwan'], // Taiwan
     ['ZI020_GE4','ge:GENC:3:1-2:TZA','raw:ZI020_GE4','united_republic_of_tanzania'], // United Republic of Tanzania
     ['ZI020_GE4','ge:GENC:3:1-2:UGA','raw:ZI020_GE4','republic_of_uganda'], // Republic of Uganda
     ['ZI020_GE4','ge:GENC:3:1-2:UKR','raw:ZI020_GE4','ukraine'], // Ukraine
     ['ZI020_GE4','ge:GENC:3:1-2:URY','raw:ZI020_GE4','oriental_republic_of_uruguay'], // Oriental Republic of Uruguay
     ['ZI020_GE4','ge:GENC:3:1-2:USA','raw:ZI020_GE4','united_states_of_america'], // United States of America
     ['ZI020_GE4','ge:GENC:3:1-2:UZB','raw:ZI020_GE4','republic_of_uzbekistan'], // Republic of Uzbekistan
     ['ZI020_GE4','ge:GENC:3:1-2:VAT','raw:ZI020_GE4','state_of_the_vatican_city'], // State of the Vatican City
     ['ZI020_GE4','ge:GENC:3:1-2:VCT','raw:ZI020_GE4','saint_vincent_and_the_grenadines'], // Saint Vincent and the Grenadines
     ['ZI020_GE4','ge:GENC:3:1-2:VEN','raw:ZI020_GE4','bolivarian_republic_of_venezuela'], // Bolivarian Republic of Venezuela
     ['ZI020_GE4','ge:GENC:3:1-2:VGB','raw:ZI020_GE4','virgin_islands,_british'], // Virgin Islands, British
     ['ZI020_GE4','ge:GENC:3:1-2:VIR','raw:ZI020_GE4','united_states_virgin_islands'], // United States Virgin Islands
     ['ZI020_GE4','ge:GENC:3:1-2:VNM','raw:ZI020_GE4','socialist_republic_of_vietnam'], // Socialist Republic of Vietnam
     ['ZI020_GE4','ge:GENC:3:1-2:VUT','raw:ZI020_GE4','republic_of_vanuatu'], // Republic of Vanuatu
     ['ZI020_GE4','ge:GENC:3:1-2:WLF','raw:ZI020_GE4','wallis_and_futuna'], // Wallis and Futuna
     ['ZI020_GE4','ge:GENC:3:1-2:WSM','raw:ZI020_GE4','independent_state_of_samoa'], // Independent State of Samoa
     ['ZI020_GE4','ge:GENC:3:1-2:XAC','raw:ZI020_GE4','territory_of_ashmore_and_cartier_islands'], // Territory of Ashmore and Cartier Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XAZ','raw:ZI020_GE4','entity_1'], // Entity 1
     ['ZI020_GE4','ge:GENC:3:1-2:XBI','raw:ZI020_GE4','bassas_da_india'], // Bassas da India
     ['ZI020_GE4','ge:GENC:3:1-2:XBK','raw:ZI020_GE4','baker_island'], // Baker Island
     ['ZI020_GE4','ge:GENC:3:1-2:XCR','raw:ZI020_GE4','entity_2'], // Entity 2
     ['ZI020_GE4','ge:GENC:3:1-2:XCS','raw:ZI020_GE4','coral_sea_islands_territory'], // Coral Sea Islands Territory
     ['ZI020_GE4','ge:GENC:3:1-2:XCY','raw:ZI020_GE4','entity_3'], // Entity 3
     ['ZI020_GE4','ge:GENC:3:1-2:XEU','raw:ZI020_GE4','europa_island'], // Europa Island
     ['ZI020_GE4','ge:GENC:3:1-2:XGL','raw:ZI020_GE4','glorioso_islands'], // Glorioso Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XGZ','raw:ZI020_GE4','gaza_strip'], // Gaza Strip
     ['ZI020_GE4','ge:GENC:3:1-2:XHO','raw:ZI020_GE4','howland_island'], // Howland Island
     ['ZI020_GE4','ge:GENC:3:1-2:XJA','raw:ZI020_GE4','johnston_atoll'], // Johnston Atoll
     ['ZI020_GE4','ge:GENC:3:1-2:XJM','raw:ZI020_GE4','jan_mayen'], // Jan Mayen
     ['ZI020_GE4','ge:GENC:3:1-2:XJN','raw:ZI020_GE4','juan_de_nova_island'], // Juan de Nova Island
     ['ZI020_GE4','ge:GENC:3:1-2:XJV','raw:ZI020_GE4','jarvis_island'], // Jarvis Island
     ['ZI020_GE4','ge:GENC:3:1-2:XKM','raw:ZI020_GE4','entity_4'], // Entity 4
     ['ZI020_GE4','ge:GENC:3:1-2:XKN','raw:ZI020_GE4','entity_5'], // Entity 5
     ['ZI020_GE4','ge:GENC:3:1-2:XKR','raw:ZI020_GE4','kingman_reef'], // Kingman Reef
     ['ZI020_GE4','ge:GENC:3:1-2:XKS','raw:ZI020_GE4','republic_of_kosovo'], // Republic of Kosovo
     ['ZI020_GE4','ge:GENC:3:1-2:XMW','raw:ZI020_GE4','midway_islands'], // Midway Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XNV','raw:ZI020_GE4','navassa_island'], // Navassa Island
     ['ZI020_GE4','ge:GENC:3:1-2:XPL','raw:ZI020_GE4','palmyra_atoll'], // Palmyra Atoll
     ['ZI020_GE4','ge:GENC:3:1-2:XPR','raw:ZI020_GE4','paracel_islands'], // Paracel Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XQP','raw:ZI020_GE4','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // Etorofu, Habomai, Kunashiri, and Shikotan Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XQZ','raw:ZI020_GE4','akrotiri'], // Akrotiri
     ['ZI020_GE4','ge:GENC:3:1-2:XSP','raw:ZI020_GE4','spratly_islands'], // Spratly Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XSV','raw:ZI020_GE4','svalbard'], // Svalbard
     ['ZI020_GE4','ge:GENC:3:1-2:XTR','raw:ZI020_GE4','tromelin_island'], // Tromelin Island
     ['ZI020_GE4','ge:GENC:3:1-2:XWB','raw:ZI020_GE4','west_bank'], // West Bank
     ['ZI020_GE4','ge:GENC:3:1-2:XWK','raw:ZI020_GE4','wake_island'], // Wake Island
     ['ZI020_GE4','ge:GENC:3:1-2:XXD','raw:ZI020_GE4','dhekelia'], // Dhekelia
     ['ZI020_GE4','ge:GENC:3:1-2:XXX','raw:ZI020_GE4','no_man's_land'], // No Man's Land
     ['ZI020_GE4','ge:GENC:3:1-2:YEM','raw:ZI020_GE4','republic_of_yemen'], // Republic of Yemen
     ['ZI020_GE4','ge:GENC:3:1-2:ZAF','raw:ZI020_GE4','republic_of_south_africa'], // Republic of South Africa
     ['ZI020_GE4','ge:GENC:3:1-2:ZMB','raw:ZI020_GE4','republic_of_zambia'], // Republic of Zambia
     ['ZI020_GE4','ge:GENC:3:1-2:ZWE','raw:ZI020_GE4','republic_of_zimbabwe'], // Republic of Zimbabwe
     ['ZI020_GE4','ge:ISO1:3:VI-15:AIA','raw:ZI020_GE4','anguilla'], // Anguilla
     ['ZI020_GE4','ge:ISO1:3:VI-15:ATA','raw:ZI020_GE4','antarctica'], // Antarctica
     ['ZI020_GE4','ge:ISO1:3:VI-15:BIH','raw:ZI020_GE4','bosnia_and_herzegovina'], // Bosnia and Herzegovina
     ['ZI020_GE4','ge:ISO1:3:VI-15:BLZ','raw:ZI020_GE4','belize'], // Belize
     ['ZI020_GE4','ge:ISO1:3:VI-15:BMU','raw:ZI020_GE4','bermuda'], // Bermuda
     ['ZI020_GE4','ge:ISO1:3:VI-15:BRB','raw:ZI020_GE4','barbados'], // Barbados
     ['ZI020_GE4','ge:ISO1:3:VI-15:BVT','raw:ZI020_GE4','bouvet_island'], // Bouvet Island
     ['ZI020_GE4','ge:ISO1:3:VI-15:GIB','raw:ZI020_GE4','gibraltar'], // Gibraltar
     ['ZI020_GE4','ge:ISO1:3:VI-15:GRD','raw:ZI020_GE4','grenada'], // Grenada
     ['ZI020_GE4','ge:ISO1:3:VI-15:GRL','raw:ZI020_GE4','greenland'], // Greenland
     ['ZI020_GE4','ge:ISO1:3:VI-15:HUN','raw:ZI020_GE4','hungary'], // Hungary
     ['ZI020_GE4','ge:ISO1:3:VI-15:IMN','raw:ZI020_GE4','isle_of_man'], // Isle of Man
     ['ZI020_GE4','ge:ISO1:3:VI-15:IRL','raw:ZI020_GE4','ireland'], // Ireland
     ['ZI020_GE4','ge:ISO1:3:VI-15:JAM','raw:ZI020_GE4','jamaica'], // Jamaica
     ['ZI020_GE4','ge:ISO1:3:VI-15:JPN','raw:ZI020_GE4','japan'], // Japan
     ['ZI020_GE4','ge:ISO1:3:VI-15:LBY','raw:ZI020_GE4','libya'], // Libya
     ['ZI020_GE4','ge:ISO1:3:VI-15:LCA','raw:ZI020_GE4','saint_lucia'], // Saint Lucia
     ['ZI020_GE4','ge:ISO1:3:VI-15:MNE','raw:ZI020_GE4','montenegro'], // Montenegro
     ['ZI020_GE4','ge:ISO1:3:VI-15:MSR','raw:ZI020_GE4','montserrat'], // Montserrat
     ['ZI020_GE4','ge:ISO1:3:VI-15:NIU','raw:ZI020_GE4','niue'], // Niue
     ['ZI020_GE4','ge:ISO1:3:VI-15:PNG','raw:ZI020_GE4','independent_state_of_papua_new_guinea'], // Independent State of Papua New Guinea
     ['ZI020_GE4','ge:ISO1:3:VI-15:ROU','raw:ZI020_GE4','romania'], // Romania
     ['ZI020_GE4','ge:ISO1:3:VI-15:TKL','raw:ZI020_GE4','tokelau'], // Tokelau
     ['ZI020_GE4','ge:ISO1:3:VI-15:TKM','raw:ZI020_GE4','turkmenistan'], // Turkmenistan
     ['ZI020_GE4','ge:ISO1:3:VI-15:TUV','raw:ZI020_GE4','tuvalu'], // Tuvalu
     ['ZI020_GE4','noInformation','raw:ZI020_GE4','noinformation'], // noInformation
     ['ZI020_GE4','other','raw:ZI020_GE4','other'], // Other

     // ZI020_GE42 - Location Country Designation : GENC Short URN-based Identifier second
     ['ZI020_GE42','ge:GENC:3:1-2:ABW','raw:ZI020_GE42','aruba'], // Aruba
     ['ZI020_GE42','ge:GENC:3:1-2:AFG','raw:ZI020_GE42','islamic_republic_of_afghanistan'], // Islamic Republic of Afghanistan
     ['ZI020_GE42','ge:GENC:3:1-2:AGO','raw:ZI020_GE42','republic_of_angola'], // Republic of Angola
     ['ZI020_GE42','ge:GENC:3:1-2:ALB','raw:ZI020_GE42','republic_of_albania'], // Republic of Albania
     ['ZI020_GE42','ge:GENC:3:1-2:AND','raw:ZI020_GE42','principality_of_andorra'], // Principality of Andorra
     ['ZI020_GE42','ge:GENC:3:1-2:ARE','raw:ZI020_GE42','united_arab_emirates'], // United Arab Emirates
     ['ZI020_GE42','ge:GENC:3:1-2:ARG','raw:ZI020_GE42','argentine_republic'], // Argentine Republic
     ['ZI020_GE42','ge:GENC:3:1-2:ARM','raw:ZI020_GE42','republic_of_armenia'], // Republic of Armenia
     ['ZI020_GE42','ge:GENC:3:1-2:ASM','raw:ZI020_GE42','territory_of_american_samoa'], // Territory of American Samoa
     ['ZI020_GE42','ge:GENC:3:1-2:ATF','raw:ZI020_GE42','french_southern_and_antarctic_lands'], // French Southern and Antarctic Lands
     ['ZI020_GE42','ge:GENC:3:1-2:ATG','raw:ZI020_GE42','antigua_and_barbuda'], // Antigua and Barbuda
     ['ZI020_GE42','ge:GENC:3:1-2:AUS','raw:ZI020_GE42','commonwealth_of_australia'], // Commonwealth of Australia
     ['ZI020_GE42','ge:GENC:3:1-2:AUT','raw:ZI020_GE42','republic_of_austria'], // Republic of Austria
     ['ZI020_GE42','ge:GENC:3:1-2:AX1','raw:ZI020_GE42','unknown'], // Unknown
     ['ZI020_GE42','ge:GENC:3:1-2:AX2','raw:ZI020_GE42','guantanamo_bay_naval_base'], // Guantanamo Bay Naval Base
     ['ZI020_GE42','ge:GENC:3:1-2:AZE','raw:ZI020_GE42','republic_of_azerbaijan'], // Republic of Azerbaijan
     ['ZI020_GE42','ge:GENC:3:1-2:BDI','raw:ZI020_GE42','republic_of_burundi'], // Republic of Burundi
     ['ZI020_GE42','ge:GENC:3:1-2:BEL','raw:ZI020_GE42','kingdom_of_belgium'], // Kingdom of Belgium
     ['ZI020_GE42','ge:GENC:3:1-2:BEN','raw:ZI020_GE42','republic_of_benin'], // Republic of Benin
     ['ZI020_GE42','ge:GENC:3:1-2:BES','raw:ZI020_GE42','bonaire,_sint_eustatius,_and_saba'], // Bonaire, Sint Eustatius, and Saba
     ['ZI020_GE42','ge:GENC:3:1-2:BFA','raw:ZI020_GE42','burkina_faso'], // Burkina Faso
     ['ZI020_GE42','ge:GENC:3:1-2:BGD','raw:ZI020_GE42','people's_republic_of_bangladesh'], // People's Republic of Bangladesh
     ['ZI020_GE42','ge:GENC:3:1-2:BGR','raw:ZI020_GE42','republic_of_bulgaria'], // Republic of Bulgaria
     ['ZI020_GE42','ge:GENC:3:1-2:BHR','raw:ZI020_GE42','kingdom_of_bahrain'], // Kingdom of Bahrain
     ['ZI020_GE42','ge:GENC:3:1-2:BHS','raw:ZI020_GE42','commonwealth_of_the_bahamas'], // Commonwealth of The Bahamas
     ['ZI020_GE42','ge:GENC:3:1-2:BLM','raw:ZI020_GE42','saint_barthelemy'], // Saint Barthelemy
     ['ZI020_GE42','ge:GENC:3:1-2:BLR','raw:ZI020_GE42','republic_of_belarus'], // Republic of Belarus
     ['ZI020_GE42','ge:GENC:3:1-2:BOL','raw:ZI020_GE42','plurinational_state_of_bolivia'], // Plurinational State of Bolivia
     ['ZI020_GE42','ge:GENC:3:1-2:BRA','raw:ZI020_GE42','federative_republic_of_brazil'], // Federative Republic of Brazil
     ['ZI020_GE42','ge:GENC:3:1-2:BRN','raw:ZI020_GE42','brunei_darussalam'], // Brunei Darussalam
     ['ZI020_GE42','ge:GENC:3:1-2:BTN','raw:ZI020_GE42','kingdom_of_bhutan'], // Kingdom of Bhutan
     ['ZI020_GE42','ge:GENC:3:1-2:BWA','raw:ZI020_GE42','republic_of_botswana'], // Republic of Botswana
     ['ZI020_GE42','ge:GENC:3:1-2:CAF','raw:ZI020_GE42','central_african_republic'], // Central African Republic
     ['ZI020_GE42','ge:GENC:3:1-2:CAN','raw:ZI020_GE42','canada'], // Canada
     ['ZI020_GE42','ge:GENC:3:1-2:CCK','raw:ZI020_GE42','territory_of_cocos_(keeling)_islands'], // Territory of Cocos (Keeling) Islands
     ['ZI020_GE42','ge:GENC:3:1-2:CHE','raw:ZI020_GE42','swiss_confederation'], // Swiss Confederation
     ['ZI020_GE42','ge:GENC:3:1-2:CHL','raw:ZI020_GE42','republic_of_chile'], // Republic of Chile
     ['ZI020_GE42','ge:GENC:3:1-2:CHN','raw:ZI020_GE42','people's_republic_of_china'], // People's Republic of China
     ['ZI020_GE42','ge:GENC:3:1-2:CIV','raw:ZI020_GE42','republic_of_côte_d'ivoire'], // Republic of Côte d'Ivoire
     ['ZI020_GE42','ge:GENC:3:1-2:CMR','raw:ZI020_GE42','republic_of_cameroon'], // Republic of Cameroon
     ['ZI020_GE42','ge:GENC:3:1-2:COD','raw:ZI020_GE42','democratic_republic_of_the_congo'], // Democratic Republic of the Congo
     ['ZI020_GE42','ge:GENC:3:1-2:COG','raw:ZI020_GE42','republic_of_the_congo'], // Republic of the Congo
     ['ZI020_GE42','ge:GENC:3:1-2:COK','raw:ZI020_GE42','cook_islands'], // Cook Islands
     ['ZI020_GE42','ge:GENC:3:1-2:COL','raw:ZI020_GE42','republic_of_colombia'], // Republic of Colombia
     ['ZI020_GE42','ge:GENC:3:1-2:COM','raw:ZI020_GE42','union_of_the_comoros'], // Union of the Comoros
     ['ZI020_GE42','ge:GENC:3:1-2:CPT','raw:ZI020_GE42','clipperton_island'], // Clipperton Island
     ['ZI020_GE42','ge:GENC:3:1-2:CPV','raw:ZI020_GE42','republic_of_cape_verde'], // Republic of Cape Verde
     ['ZI020_GE42','ge:GENC:3:1-2:CRI','raw:ZI020_GE42','republic_of_costa_rica'], // Republic of Costa Rica
     ['ZI020_GE42','ge:GENC:3:1-2:CUB','raw:ZI020_GE42','republic_of_cuba'], // Republic of Cuba
     ['ZI020_GE42','ge:GENC:3:1-2:CUW','raw:ZI020_GE42','curaçao'], // Curaçao
     ['ZI020_GE42','ge:GENC:3:1-2:CXR','raw:ZI020_GE42','territory_of_christmas_island'], // Territory of Christmas Island
     ['ZI020_GE42','ge:GENC:3:1-2:CYM','raw:ZI020_GE42','cayman_islands'], // Cayman Islands
     ['ZI020_GE42','ge:GENC:3:1-2:CYP','raw:ZI020_GE42','republic_of_cyprus'], // Republic of Cyprus
     ['ZI020_GE42','ge:GENC:3:1-2:CZE','raw:ZI020_GE42','czech_republic'], // Czech Republic
     ['ZI020_GE42','ge:GENC:3:1-2:DEU','raw:ZI020_GE42','federal_republic_of_germany'], // Federal Republic of Germany
     ['ZI020_GE42','ge:GENC:3:1-2:DGA','raw:ZI020_GE42','diego_garcia'], // Diego Garcia
     ['ZI020_GE42','ge:GENC:3:1-2:DJI','raw:ZI020_GE42','republic_of_djibouti'], // Republic of Djibouti
     ['ZI020_GE42','ge:GENC:3:1-2:DMA','raw:ZI020_GE42','commonwealth_of_dominica'], // Commonwealth of Dominica
     ['ZI020_GE42','ge:GENC:3:1-2:DNK','raw:ZI020_GE42','kingdom_of_denmark'], // Kingdom of Denmark
     ['ZI020_GE42','ge:GENC:3:1-2:DOM','raw:ZI020_GE42','dominican_republic'], // Dominican Republic
     ['ZI020_GE42','ge:GENC:3:1-2:DZA','raw:ZI020_GE42','people's_democratic_republic_of_algeria'], // People's Democratic Republic of Algeria
     ['ZI020_GE42','ge:GENC:3:1-2:ECU','raw:ZI020_GE42','republic_of_ecuador'], // Republic of Ecuador
     ['ZI020_GE42','ge:GENC:3:1-2:EGY','raw:ZI020_GE42','arab_republic_of_egypt'], // Arab Republic of Egypt
     ['ZI020_GE42','ge:GENC:3:1-2:ERI','raw:ZI020_GE42','state_of_eritrea'], // State of Eritrea
     ['ZI020_GE42','ge:GENC:3:1-2:ESH','raw:ZI020_GE42','western_sahara'], // Western Sahara
     ['ZI020_GE42','ge:GENC:3:1-2:ESP','raw:ZI020_GE42','kingdom_of_spain'], // Kingdom of Spain
     ['ZI020_GE42','ge:GENC:3:1-2:EST','raw:ZI020_GE42','republic_of_estonia'], // Republic of Estonia
     ['ZI020_GE42','ge:GENC:3:1-2:ETH','raw:ZI020_GE42','federal_democratic_republic_of_ethiopia'], // Federal Democratic Republic of Ethiopia
     ['ZI020_GE42','ge:GENC:3:1-2:FIN','raw:ZI020_GE42','republic_of_finland'], // Republic of Finland
     ['ZI020_GE42','ge:GENC:3:1-2:FJI','raw:ZI020_GE42','republic_of_fiji'], // Republic of Fiji
     ['ZI020_GE42','ge:GENC:3:1-2:FLK','raw:ZI020_GE42','falkland_islands_(islas_malvinas)'], // Falkland Islands (Islas Malvinas)
     ['ZI020_GE42','ge:GENC:3:1-2:FRA','raw:ZI020_GE42','french_republic'], // French Republic
     ['ZI020_GE42','ge:GENC:3:1-2:FRO','raw:ZI020_GE42','faroe_islands'], // Faroe Islands
     ['ZI020_GE42','ge:GENC:3:1-2:FSM','raw:ZI020_GE42','federated_states_of_micronesia'], // Federated States of Micronesia
     ['ZI020_GE42','ge:GENC:3:1-2:GAB','raw:ZI020_GE42','gabonese_republic'], // Gabonese Republic
     ['ZI020_GE42','ge:GENC:3:1-2:GBR','raw:ZI020_GE42','united_kingdom_of_great_britain_and_northern_ireland'], // United Kingdom of Great Britain and Northern Ireland
     ['ZI020_GE42','ge:GENC:3:1-2:GEO','raw:ZI020_GE42','georgia'], // Georgia
     ['ZI020_GE42','ge:GENC:3:1-2:GGY','raw:ZI020_GE42','bailiwick_of_guernsey'], // Bailiwick of Guernsey
     ['ZI020_GE42','ge:GENC:3:1-2:GHA','raw:ZI020_GE42','republic_of_ghana'], // Republic of Ghana
     ['ZI020_GE42','ge:GENC:3:1-2:GIN','raw:ZI020_GE42','republic_of_guinea'], // Republic of Guinea
     ['ZI020_GE42','ge:GENC:3:1-2:GLP','raw:ZI020_GE42','department_of_guadeloupe'], // Department of Guadeloupe
     ['ZI020_GE42','ge:GENC:3:1-2:GMB','raw:ZI020_GE42','republic_of_the_gambia'], // Republic of The Gambia
     ['ZI020_GE42','ge:GENC:3:1-2:GNB','raw:ZI020_GE42','republic_of_guinea-bissau'], // Republic of Guinea-Bissau
     ['ZI020_GE42','ge:GENC:3:1-2:GNQ','raw:ZI020_GE42','republic_of_equatorial_guinea'], // Republic of Equatorial Guinea
     ['ZI020_GE42','ge:GENC:3:1-2:GRC','raw:ZI020_GE42','hellenic_republic'], // Hellenic Republic
     ['ZI020_GE42','ge:GENC:3:1-2:GTM','raw:ZI020_GE42','republic_of_guatemala'], // Republic of Guatemala
     ['ZI020_GE42','ge:GENC:3:1-2:GUF','raw:ZI020_GE42','department_of_guiana'], // Department of Guiana
     ['ZI020_GE42','ge:GENC:3:1-2:GUM','raw:ZI020_GE42','territory_of_guam'], // Territory of Guam
     ['ZI020_GE42','ge:GENC:3:1-2:GUY','raw:ZI020_GE42','co-operative_republic_of_guyana'], // Co-operative Republic of Guyana
     ['ZI020_GE42','ge:GENC:3:1-2:HKG','raw:ZI020_GE42','hong_kong_special_administrative_region'], // Hong Kong Special Administrative Region
     ['ZI020_GE42','ge:GENC:3:1-2:HMD','raw:ZI020_GE42','territory_of_heard_island_and_mcdonald_islands'], // Territory of Heard Island and McDonald Islands
     ['ZI020_GE42','ge:GENC:3:1-2:HND','raw:ZI020_GE42','republic_of_honduras'], // Republic of Honduras
     ['ZI020_GE42','ge:GENC:3:1-2:HRV','raw:ZI020_GE42','republic_of_croatia'], // Republic of Croatia
     ['ZI020_GE42','ge:GENC:3:1-2:HTI','raw:ZI020_GE42','republic_of_haiti'], // Republic of Haiti
     ['ZI020_GE42','ge:GENC:3:1-2:IDN','raw:ZI020_GE42','republic_of_indonesia'], // Republic of Indonesia
     ['ZI020_GE42','ge:GENC:3:1-2:IND','raw:ZI020_GE42','republic_of_india'], // Republic of India
     ['ZI020_GE42','ge:GENC:3:1-2:IOT','raw:ZI020_GE42','british_indian_ocean_territory'], // British Indian Ocean Territory
     ['ZI020_GE42','ge:GENC:3:1-2:IRN','raw:ZI020_GE42','islamic_republic_of_iran'], // Islamic Republic of Iran
     ['ZI020_GE42','ge:GENC:3:1-2:IRQ','raw:ZI020_GE42','republic_of_iraq'], // Republic of Iraq
     ['ZI020_GE42','ge:GENC:3:1-2:ISL','raw:ZI020_GE42','republic_of_iceland'], // Republic of Iceland
     ['ZI020_GE42','ge:GENC:3:1-2:ISR','raw:ZI020_GE42','state_of_israel'], // State of Israel
     ['ZI020_GE42','ge:GENC:3:1-2:ITA','raw:ZI020_GE42','italian_republic'], // Italian Republic
     ['ZI020_GE42','ge:GENC:3:1-2:JEY','raw:ZI020_GE42','bailiwick_of_jersey'], // Bailiwick of Jersey
     ['ZI020_GE42','ge:GENC:3:1-2:JOR','raw:ZI020_GE42','hashemite_kingdom_of_jordan'], // Hashemite Kingdom of Jordan
     ['ZI020_GE42','ge:GENC:3:1-2:KAZ','raw:ZI020_GE42','republic_of_kazakhstan'], // Republic of Kazakhstan
     ['ZI020_GE42','ge:GENC:3:1-2:KEN','raw:ZI020_GE42','republic_of_kenya'], // Republic of Kenya
     ['ZI020_GE42','ge:GENC:3:1-2:KGZ','raw:ZI020_GE42','kyrgyz_republic'], // Kyrgyz Republic
     ['ZI020_GE42','ge:GENC:3:1-2:KHM','raw:ZI020_GE42','kingdom_of_cambodia'], // Kingdom of Cambodia
     ['ZI020_GE42','ge:GENC:3:1-2:KIR','raw:ZI020_GE42','republic_of_kiribati'], // Republic of Kiribati
     ['ZI020_GE42','ge:GENC:3:1-2:KNA','raw:ZI020_GE42','federation_of_saint_kitts_and_nevis'], // Federation of Saint Kitts and Nevis
     ['ZI020_GE42','ge:GENC:3:1-2:KOR','raw:ZI020_GE42','republic_of_korea'], // Republic of Korea
     ['ZI020_GE42','ge:GENC:3:1-2:KWT','raw:ZI020_GE42','state_of_kuwait'], // State of Kuwait
     ['ZI020_GE42','ge:GENC:3:1-2:LAO','raw:ZI020_GE42','lao_people's_democratic_republic'], // Lao People's Democratic Republic
     ['ZI020_GE42','ge:GENC:3:1-2:LBN','raw:ZI020_GE42','lebanese_republic'], // Lebanese Republic
     ['ZI020_GE42','ge:GENC:3:1-2:LBR','raw:ZI020_GE42','republic_of_liberia'], // Republic of Liberia
     ['ZI020_GE42','ge:GENC:3:1-2:LIE','raw:ZI020_GE42','principality_of_liechtenstein'], // Principality of Liechtenstein
     ['ZI020_GE42','ge:GENC:3:1-2:LKA','raw:ZI020_GE42','democratic_socialist_republic_of_sri_lanka'], // Democratic Socialist Republic of Sri Lanka
     ['ZI020_GE42','ge:GENC:3:1-2:LSO','raw:ZI020_GE42','kingdom_of_lesotho'], // Kingdom of Lesotho
     ['ZI020_GE42','ge:GENC:3:1-2:LTU','raw:ZI020_GE42','republic_of_lithuania'], // Republic of Lithuania
     ['ZI020_GE42','ge:GENC:3:1-2:LUX','raw:ZI020_GE42','grand_duchy_of_luxembourg'], // Grand Duchy of Luxembourg
     ['ZI020_GE42','ge:GENC:3:1-2:LVA','raw:ZI020_GE42','republic_of_latvia'], // Republic of Latvia
     ['ZI020_GE42','ge:GENC:3:1-2:MAC','raw:ZI020_GE42','macau_special_administrative_region'], // Macau Special Administrative Region
     ['ZI020_GE42','ge:GENC:3:1-2:MAF','raw:ZI020_GE42','saint_martin'], // Saint Martin
     ['ZI020_GE42','ge:GENC:3:1-2:MAR','raw:ZI020_GE42','kingdom_of_morocco'], // Kingdom of Morocco
     ['ZI020_GE42','ge:GENC:3:1-2:MCO','raw:ZI020_GE42','principality_of_monaco'], // Principality of Monaco
     ['ZI020_GE42','ge:GENC:3:1-2:MDA','raw:ZI020_GE42','republic_of_moldova'], // Republic of Moldova
     ['ZI020_GE42','ge:GENC:3:1-2:MDG','raw:ZI020_GE42','republic_of_madagascar'], // Republic of Madagascar
     ['ZI020_GE42','ge:GENC:3:1-2:MDV','raw:ZI020_GE42','republic_of_maldives'], // Republic of Maldives
     ['ZI020_GE42','ge:GENC:3:1-2:MEX','raw:ZI020_GE42','united_mexican_states'], // United Mexican States
     ['ZI020_GE42','ge:GENC:3:1-2:MHL','raw:ZI020_GE42','republic_of_the_marshall_islands'], // Republic of the Marshall Islands
     ['ZI020_GE42','ge:GENC:3:1-2:MKD','raw:ZI020_GE42','republic_of_macedonia'], // Republic of Macedonia
     ['ZI020_GE42','ge:GENC:3:1-2:MLI','raw:ZI020_GE42','republic_of_mali'], // Republic of Mali
     ['ZI020_GE42','ge:GENC:3:1-2:MLT','raw:ZI020_GE42','republic_of_malta'], // Republic of Malta
     ['ZI020_GE42','ge:GENC:3:1-2:MMR','raw:ZI020_GE42','union_of_burma'], // Union of Burma
     ['ZI020_GE42','ge:GENC:3:1-2:MNG','raw:ZI020_GE42','mongolia'], // Mongolia
     ['ZI020_GE42','ge:GENC:3:1-2:MNP','raw:ZI020_GE42','commonwealth_of_the_northern_mariana_islands'], // Commonwealth of the Northern Mariana Islands
     ['ZI020_GE42','ge:GENC:3:1-2:MOZ','raw:ZI020_GE42','republic_of_mozambique'], // Republic of Mozambique
     ['ZI020_GE42','ge:GENC:3:1-2:MRT','raw:ZI020_GE42','islamic_republic_of_mauritania'], // Islamic Republic of Mauritania
     ['ZI020_GE42','ge:GENC:3:1-2:MTQ','raw:ZI020_GE42','department_of_martinique'], // Department of Martinique
     ['ZI020_GE42','ge:GENC:3:1-2:MUS','raw:ZI020_GE42','republic_of_mauritius'], // Republic of Mauritius
     ['ZI020_GE42','ge:GENC:3:1-2:MWI','raw:ZI020_GE42','republic_of_malawi'], // Republic of Malawi
     ['ZI020_GE42','ge:GENC:3:1-2:MYS','raw:ZI020_GE42','malaysia'], // Malaysia
     ['ZI020_GE42','ge:GENC:3:1-2:MYT','raw:ZI020_GE42','department_of_mayotte'], // Department of Mayotte
     ['ZI020_GE42','ge:GENC:3:1-2:NAM','raw:ZI020_GE42','republic_of_namibia'], // Republic of Namibia
     ['ZI020_GE42','ge:GENC:3:1-2:NCL','raw:ZI020_GE42','new_caledonia'], // New Caledonia
     ['ZI020_GE42','ge:GENC:3:1-2:NER','raw:ZI020_GE42','republic_of_the_niger'], // Republic of the Niger
     ['ZI020_GE42','ge:GENC:3:1-2:NFK','raw:ZI020_GE42','territory_of_norfolk_island'], // Territory of Norfolk Island
     ['ZI020_GE42','ge:GENC:3:1-2:NGA','raw:ZI020_GE42','federal_republic_of_nigeria'], // Federal Republic of Nigeria
     ['ZI020_GE42','ge:GENC:3:1-2:NIC','raw:ZI020_GE42','republic_of_nicaragua'], // Republic of Nicaragua
     ['ZI020_GE42','ge:GENC:3:1-2:NLD','raw:ZI020_GE42','kingdom_of_the_netherlands'], // Kingdom of the Netherlands
     ['ZI020_GE42','ge:GENC:3:1-2:NOR','raw:ZI020_GE42','kingdom_of_norway'], // Kingdom of Norway
     ['ZI020_GE42','ge:GENC:3:1-2:NPL','raw:ZI020_GE42','federal_democratic_republic_of_nepal'], // Federal Democratic Republic of Nepal
     ['ZI020_GE42','ge:GENC:3:1-2:NRU','raw:ZI020_GE42','republic_of_nauru'], // Republic of Nauru
     ['ZI020_GE42','ge:GENC:3:1-2:NZL','raw:ZI020_GE42','new_zealand'], // New Zealand
     ['ZI020_GE42','ge:GENC:3:1-2:OMN','raw:ZI020_GE42','sultanate_of_oman'], // Sultanate of Oman
     ['ZI020_GE42','ge:GENC:3:1-2:PAK','raw:ZI020_GE42','islamic_republic_of_pakistan'], // Islamic Republic of Pakistan
     ['ZI020_GE42','ge:GENC:3:1-2:PAN','raw:ZI020_GE42','republic_of_panama'], // Republic of Panama
     ['ZI020_GE42','ge:GENC:3:1-2:PCN','raw:ZI020_GE42','pitcairn,_henderson,_ducie,_and_oeno_islands'], // Pitcairn, Henderson, Ducie, and Oeno Islands
     ['ZI020_GE42','ge:GENC:3:1-2:PER','raw:ZI020_GE42','republic_of_peru'], // Republic of Peru
     ['ZI020_GE42','ge:GENC:3:1-2:PHL','raw:ZI020_GE42','republic_of_the_philippines'], // Republic of the Philippines
     ['ZI020_GE42','ge:GENC:3:1-2:PLW','raw:ZI020_GE42','republic_of_palau'], // Republic of Palau
     ['ZI020_GE42','ge:GENC:3:1-2:POL','raw:ZI020_GE42','republic_of_poland'], // Republic of Poland
     ['ZI020_GE42','ge:GENC:3:1-2:PRI','raw:ZI020_GE42','commonwealth_of_puerto_rico'], // Commonwealth of Puerto Rico
     ['ZI020_GE42','ge:GENC:3:1-2:PRK','raw:ZI020_GE42','democratic_people's_republic_of_korea'], // Democratic People's Republic of Korea
     ['ZI020_GE42','ge:GENC:3:1-2:PRT','raw:ZI020_GE42','portuguese_republic'], // Portuguese Republic
     ['ZI020_GE42','ge:GENC:3:1-2:PRY','raw:ZI020_GE42','republic_of_paraguay'], // Republic of Paraguay
     ['ZI020_GE42','ge:GENC:3:1-2:PSE','raw:ZI020_GE42','palestinian_territory'], // Palestinian Territory
     ['ZI020_GE42','ge:GENC:3:1-2:PYF','raw:ZI020_GE42','french_polynesia'], // French Polynesia
     ['ZI020_GE42','ge:GENC:3:1-2:QAT','raw:ZI020_GE42','state_of_qatar'], // State of Qatar
     ['ZI020_GE42','ge:GENC:3:1-2:REU','raw:ZI020_GE42','department_of_reunion'], // Department of Reunion
     ['ZI020_GE42','ge:GENC:3:1-2:RUS','raw:ZI020_GE42','russian_federation'], // Russian Federation
     ['ZI020_GE42','ge:GENC:3:1-2:RWA','raw:ZI020_GE42','republic_of_rwanda'], // Republic of Rwanda
     ['ZI020_GE42','ge:GENC:3:1-2:SAU','raw:ZI020_GE42','kingdom_of_saudi_arabia'], // Kingdom of Saudi Arabia
     ['ZI020_GE42','ge:GENC:3:1-2:SDN','raw:ZI020_GE42','republic_of_the_sudan'], // Republic of the Sudan
     ['ZI020_GE42','ge:GENC:3:1-2:SEN','raw:ZI020_GE42','republic_of_senegal'], // Republic of Senegal
     ['ZI020_GE42','ge:GENC:3:1-2:SGP','raw:ZI020_GE42','republic_of_singapore'], // Republic of Singapore
     ['ZI020_GE42','ge:GENC:3:1-2:SGS','raw:ZI020_GE42','south_georgia_and_south_sandwich_islands'], // South Georgia and South Sandwich Islands
     ['ZI020_GE42','ge:GENC:3:1-2:SHN','raw:ZI020_GE42','saint_helena,_ascension,_and_tristan_da_cunha'], // Saint Helena, Ascension, and Tristan da Cunha
     ['ZI020_GE42','ge:GENC:3:1-2:SLB','raw:ZI020_GE42','solomon_islands'], // Solomon Islands
     ['ZI020_GE42','ge:GENC:3:1-2:SLE','raw:ZI020_GE42','republic_of_sierra_leone'], // Republic of Sierra Leone
     ['ZI020_GE42','ge:GENC:3:1-2:SLV','raw:ZI020_GE42','republic_of_el_salvador'], // Republic of El Salvador
     ['ZI020_GE42','ge:GENC:3:1-2:SMR','raw:ZI020_GE42','republic_of_san_marino'], // Republic of San Marino
     ['ZI020_GE42','ge:GENC:3:1-2:SOM','raw:ZI020_GE42','somalia,_federal_republic_of'], // Somalia, Federal Republic of
     ['ZI020_GE42','ge:GENC:3:1-2:SPM','raw:ZI020_GE42','territorial_collectivity_of_saint_pierre_and_miquelon'], // Territorial Collectivity of Saint Pierre and Miquelon
     ['ZI020_GE42','ge:GENC:3:1-2:SRB','raw:ZI020_GE42','republic_of_serbia'], // Republic of Serbia
     ['ZI020_GE42','ge:GENC:3:1-2:SSD','raw:ZI020_GE42','republic_of_south_sudan'], // Republic of South Sudan
     ['ZI020_GE42','ge:GENC:3:1-2:STP','raw:ZI020_GE42','democratic_republic_of_sao_tome_and_principe'], // Democratic Republic of Sao Tome and Principe
     ['ZI020_GE42','ge:GENC:3:1-2:SUR','raw:ZI020_GE42','republic_of_suriname'], // Republic of Suriname
     ['ZI020_GE42','ge:GENC:3:1-2:SVK','raw:ZI020_GE42','slovak_republic'], // Slovak Republic
     ['ZI020_GE42','ge:GENC:3:1-2:SVN','raw:ZI020_GE42','republic_of_slovenia'], // Republic of Slovenia
     ['ZI020_GE42','ge:GENC:3:1-2:SWE','raw:ZI020_GE42','kingdom_of_sweden'], // Kingdom of Sweden
     ['ZI020_GE42','ge:GENC:3:1-2:SWZ','raw:ZI020_GE42','kingdom_of_swaziland'], // Kingdom of Swaziland
     ['ZI020_GE42','ge:GENC:3:1-2:SXM','raw:ZI020_GE42','sint_maarten'], // Sint Maarten
     ['ZI020_GE42','ge:GENC:3:1-2:SYC','raw:ZI020_GE42','republic_of_seychelles'], // Republic of Seychelles
     ['ZI020_GE42','ge:GENC:3:1-2:SYR','raw:ZI020_GE42','syrian_arab_republic'], // Syrian Arab Republic
     ['ZI020_GE42','ge:GENC:3:1-2:TCA','raw:ZI020_GE42','turks_and_caicos_islands'], // Turks and Caicos Islands
     ['ZI020_GE42','ge:GENC:3:1-2:TCD','raw:ZI020_GE42','republic_of_chad'], // Republic of Chad
     ['ZI020_GE42','ge:GENC:3:1-2:TGO','raw:ZI020_GE42','togolese_republic'], // Togolese Republic
     ['ZI020_GE42','ge:GENC:3:1-2:THA','raw:ZI020_GE42','kingdom_of_thailand'], // Kingdom of Thailand
     ['ZI020_GE42','ge:GENC:3:1-2:TJK','raw:ZI020_GE42','republic_of_tajikistan'], // Republic of Tajikistan
     ['ZI020_GE42','ge:GENC:3:1-2:TLS','raw:ZI020_GE42','democratic_republic_of_timor-leste'], // Democratic Republic of Timor-Leste
     ['ZI020_GE42','ge:GENC:3:1-2:TON','raw:ZI020_GE42','kingdom_of_tonga'], // Kingdom of Tonga
     ['ZI020_GE42','ge:GENC:3:1-2:TTO','raw:ZI020_GE42','republic_of_trinidad_and_tobago'], // Republic of Trinidad and Tobago
     ['ZI020_GE42','ge:GENC:3:1-2:TUN','raw:ZI020_GE42','tunisian_republic'], // Tunisian Republic
     ['ZI020_GE42','ge:GENC:3:1-2:TUR','raw:ZI020_GE42','republic_of_turkey'], // Republic of Turkey
     ['ZI020_GE42','ge:GENC:3:1-2:TWN','raw:ZI020_GE42','taiwan'], // Taiwan
     ['ZI020_GE42','ge:GENC:3:1-2:TZA','raw:ZI020_GE42','united_republic_of_tanzania'], // United Republic of Tanzania
     ['ZI020_GE42','ge:GENC:3:1-2:UGA','raw:ZI020_GE42','republic_of_uganda'], // Republic of Uganda
     ['ZI020_GE42','ge:GENC:3:1-2:UKR','raw:ZI020_GE42','ukraine'], // Ukraine
     ['ZI020_GE42','ge:GENC:3:1-2:URY','raw:ZI020_GE42','oriental_republic_of_uruguay'], // Oriental Republic of Uruguay
     ['ZI020_GE42','ge:GENC:3:1-2:USA','raw:ZI020_GE42','united_states_of_america'], // United States of America
     ['ZI020_GE42','ge:GENC:3:1-2:UZB','raw:ZI020_GE42','republic_of_uzbekistan'], // Republic of Uzbekistan
     ['ZI020_GE42','ge:GENC:3:1-2:VAT','raw:ZI020_GE42','state_of_the_vatican_city'], // State of the Vatican City
     ['ZI020_GE42','ge:GENC:3:1-2:VCT','raw:ZI020_GE42','saint_vincent_and_the_grenadines'], // Saint Vincent and the Grenadines
     ['ZI020_GE42','ge:GENC:3:1-2:VEN','raw:ZI020_GE42','bolivarian_republic_of_venezuela'], // Bolivarian Republic of Venezuela
     ['ZI020_GE42','ge:GENC:3:1-2:VGB','raw:ZI020_GE42','virgin_islands,_british'], // Virgin Islands, British
     ['ZI020_GE42','ge:GENC:3:1-2:VIR','raw:ZI020_GE42','united_states_virgin_islands'], // United States Virgin Islands
     ['ZI020_GE42','ge:GENC:3:1-2:VNM','raw:ZI020_GE42','socialist_republic_of_vietnam'], // Socialist Republic of Vietnam
     ['ZI020_GE42','ge:GENC:3:1-2:VUT','raw:ZI020_GE42','republic_of_vanuatu'], // Republic of Vanuatu
     ['ZI020_GE42','ge:GENC:3:1-2:WLF','raw:ZI020_GE42','wallis_and_futuna'], // Wallis and Futuna
     ['ZI020_GE42','ge:GENC:3:1-2:WSM','raw:ZI020_GE42','independent_state_of_samoa'], // Independent State of Samoa
     ['ZI020_GE42','ge:GENC:3:1-2:XAC','raw:ZI020_GE42','territory_of_ashmore_and_cartier_islands'], // Territory of Ashmore and Cartier Islands
     ['ZI020_GE42','ge:GENC:3:1-2:XAZ','raw:ZI020_GE42','entity_1'], // Entity 1
     ['ZI020_GE42','ge:GENC:3:1-2:XBI','raw:ZI020_GE42','bassas_da_india'], // Bassas da India
     ['ZI020_GE42','ge:GENC:3:1-2:XBK','raw:ZI020_GE42','baker_island'], // Baker Island
     ['ZI020_GE42','ge:GENC:3:1-2:XCR','raw:ZI020_GE42','entity_2'], // Entity 2
     ['ZI020_GE42','ge:GENC:3:1-2:XCS','raw:ZI020_GE42','coral_sea_islands_territory'], // Coral Sea Islands Territory
     ['ZI020_GE42','ge:GENC:3:1-2:XCY','raw:ZI020_GE42','entity_3'], // Entity 3
     ['ZI020_GE42','ge:GENC:3:1-2:XEU','raw:ZI020_GE42','europa_island'], // Europa Island
     ['ZI020_GE42','ge:GENC:3:1-2:XGL','raw:ZI020_GE42','glorioso_islands'], // Glorioso Islands
     ['ZI020_GE42','ge:GENC:3:1-2:XGZ','raw:ZI020_GE42','gaza_strip'], // Gaza Strip
     ['ZI020_GE42','ge:GENC:3:1-2:XHO','raw:ZI020_GE42','howland_island'], // Howland Island
     ['ZI020_GE42','ge:GENC:3:1-2:XJA','raw:ZI020_GE42','johnston_atoll'], // Johnston Atoll
     ['ZI020_GE42','ge:GENC:3:1-2:XJM','raw:ZI020_GE42','jan_mayen'], // Jan Mayen
     ['ZI020_GE42','ge:GENC:3:1-2:XJN','raw:ZI020_GE42','juan_de_nova_island'], // Juan de Nova Island
     ['ZI020_GE42','ge:GENC:3:1-2:XJV','raw:ZI020_GE42','jarvis_island'], // Jarvis Island
     ['ZI020_GE42','ge:GENC:3:1-2:XKM','raw:ZI020_GE42','entity_4'], // Entity 4
     ['ZI020_GE42','ge:GENC:3:1-2:XKN','raw:ZI020_GE42','entity_5'], // Entity 5
     ['ZI020_GE42','ge:GENC:3:1-2:XKR','raw:ZI020_GE42','kingman_reef'], // Kingman Reef
     ['ZI020_GE42','ge:GENC:3:1-2:XKS','raw:ZI020_GE42','republic_of_kosovo'], // Republic of Kosovo
     ['ZI020_GE42','ge:GENC:3:1-2:XMW','raw:ZI020_GE42','midway_islands'], // Midway Islands
     ['ZI020_GE42','ge:GENC:3:1-2:XNV','raw:ZI020_GE42','navassa_island'], // Navassa Island
     ['ZI020_GE42','ge:GENC:3:1-2:XPL','raw:ZI020_GE42','palmyra_atoll'], // Palmyra Atoll
     ['ZI020_GE42','ge:GENC:3:1-2:XPR','raw:ZI020_GE42','paracel_islands'], // Paracel Islands
     ['ZI020_GE42','ge:GENC:3:1-2:XQP','raw:ZI020_GE42','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // Etorofu, Habomai, Kunashiri, and Shikotan Islands
     ['ZI020_GE42','ge:GENC:3:1-2:XQZ','raw:ZI020_GE42','akrotiri'], // Akrotiri
     ['ZI020_GE42','ge:GENC:3:1-2:XSP','raw:ZI020_GE42','spratly_islands'], // Spratly Islands
     ['ZI020_GE42','ge:GENC:3:1-2:XSV','raw:ZI020_GE42','svalbard'], // Svalbard
     ['ZI020_GE42','ge:GENC:3:1-2:XTR','raw:ZI020_GE42','tromelin_island'], // Tromelin Island
     ['ZI020_GE42','ge:GENC:3:1-2:XWB','raw:ZI020_GE42','west_bank'], // West Bank
     ['ZI020_GE42','ge:GENC:3:1-2:XWK','raw:ZI020_GE42','wake_island'], // Wake Island
     ['ZI020_GE42','ge:GENC:3:1-2:XXD','raw:ZI020_GE42','dhekelia'], // Dhekelia
     ['ZI020_GE42','ge:GENC:3:1-2:XXX','raw:ZI020_GE42','no_man's_land'], // No Man's Land
     ['ZI020_GE42','ge:GENC:3:1-2:YEM','raw:ZI020_GE42','republic_of_yemen'], // Republic of Yemen
     ['ZI020_GE42','ge:GENC:3:1-2:ZAF','raw:ZI020_GE42','republic_of_south_africa'], // Republic of South Africa
     ['ZI020_GE42','ge:GENC:3:1-2:ZMB','raw:ZI020_GE42','republic_of_zambia'], // Republic of Zambia
     ['ZI020_GE42','ge:GENC:3:1-2:ZWE','raw:ZI020_GE42','republic_of_zimbabwe'], // Republic of Zimbabwe
     ['ZI020_GE42','ge:ISO1:3:VI-15:AIA','raw:ZI020_GE42','anguilla'], // Anguilla
     ['ZI020_GE42','ge:ISO1:3:VI-15:ATA','raw:ZI020_GE42','antarctica'], // Antarctica
     ['ZI020_GE42','ge:ISO1:3:VI-15:BIH','raw:ZI020_GE42','bosnia_and_herzegovina'], // Bosnia and Herzegovina
     ['ZI020_GE42','ge:ISO1:3:VI-15:BLZ','raw:ZI020_GE42','belize'], // Belize
     ['ZI020_GE42','ge:ISO1:3:VI-15:BMU','raw:ZI020_GE42','bermuda'], // Bermuda
     ['ZI020_GE42','ge:ISO1:3:VI-15:BRB','raw:ZI020_GE42','barbados'], // Barbados
     ['ZI020_GE42','ge:ISO1:3:VI-15:BVT','raw:ZI020_GE42','bouvet_island'], // Bouvet Island
     ['ZI020_GE42','ge:ISO1:3:VI-15:GIB','raw:ZI020_GE42','gibraltar'], // Gibraltar
     ['ZI020_GE42','ge:ISO1:3:VI-15:GRD','raw:ZI020_GE42','grenada'], // Grenada
     ['ZI020_GE42','ge:ISO1:3:VI-15:GRL','raw:ZI020_GE42','greenland'], // Greenland
     ['ZI020_GE42','ge:ISO1:3:VI-15:HUN','raw:ZI020_GE42','hungary'], // Hungary
     ['ZI020_GE42','ge:ISO1:3:VI-15:IMN','raw:ZI020_GE42','isle_of_man'], // Isle of Man
     ['ZI020_GE42','ge:ISO1:3:VI-15:IRL','raw:ZI020_GE42','ireland'], // Ireland
     ['ZI020_GE42','ge:ISO1:3:VI-15:JAM','raw:ZI020_GE42','jamaica'], // Jamaica
     ['ZI020_GE42','ge:ISO1:3:VI-15:JPN','raw:ZI020_GE42','japan'], // Japan
     ['ZI020_GE42','ge:ISO1:3:VI-15:LBY','raw:ZI020_GE42','libya'], // Libya
     ['ZI020_GE42','ge:ISO1:3:VI-15:LCA','raw:ZI020_GE42','saint_lucia'], // Saint Lucia
     ['ZI020_GE42','ge:ISO1:3:VI-15:MNE','raw:ZI020_GE42','montenegro'], // Montenegro
     ['ZI020_GE42','ge:ISO1:3:VI-15:MSR','raw:ZI020_GE42','montserrat'], // Montserrat
     ['ZI020_GE42','ge:ISO1:3:VI-15:NIU','raw:ZI020_GE42','niue'], // Niue
     ['ZI020_GE42','ge:ISO1:3:VI-15:PNG','raw:ZI020_GE42','independent_state_of_papua_new_guinea'], // Independent State of Papua New Guinea
     ['ZI020_GE42','ge:ISO1:3:VI-15:ROU','raw:ZI020_GE42','romania'], // Romania
     ['ZI020_GE42','ge:ISO1:3:VI-15:TKL','raw:ZI020_GE42','tokelau'], // Tokelau
     ['ZI020_GE42','ge:ISO1:3:VI-15:TKM','raw:ZI020_GE42','turkmenistan'], // Turkmenistan
     ['ZI020_GE42','ge:ISO1:3:VI-15:TUV','raw:ZI020_GE42','tuvalu'], // Tuvalu
     ['ZI020_GE42','noInformation','raw:ZI020_GE42','noinformation'], // noInformation
     ['ZI020_GE42','other','raw:ZI020_GE42','other'], // Other

     // ZI020_GE43 - Location Country Designation : GENC Short URN-based Identifier third
     ['ZI020_GE43','ge:GENC:3:1-2:ABW','raw:ZI020_GE43','aruba'], // Aruba
     ['ZI020_GE43','ge:GENC:3:1-2:AFG','raw:ZI020_GE43','islamic_republic_of_afghanistan'], // Islamic Republic of Afghanistan
     ['ZI020_GE43','ge:GENC:3:1-2:AGO','raw:ZI020_GE43','republic_of_angola'], // Republic of Angola
     ['ZI020_GE43','ge:GENC:3:1-2:ALB','raw:ZI020_GE43','republic_of_albania'], // Republic of Albania
     ['ZI020_GE43','ge:GENC:3:1-2:AND','raw:ZI020_GE43','principality_of_andorra'], // Principality of Andorra
     ['ZI020_GE43','ge:GENC:3:1-2:ARE','raw:ZI020_GE43','united_arab_emirates'], // United Arab Emirates
     ['ZI020_GE43','ge:GENC:3:1-2:ARG','raw:ZI020_GE43','argentine_republic'], // Argentine Republic
     ['ZI020_GE43','ge:GENC:3:1-2:ARM','raw:ZI020_GE43','republic_of_armenia'], // Republic of Armenia
     ['ZI020_GE43','ge:GENC:3:1-2:ASM','raw:ZI020_GE43','territory_of_american_samoa'], // Territory of American Samoa
     ['ZI020_GE43','ge:GENC:3:1-2:ATF','raw:ZI020_GE43','french_southern_and_antarctic_lands'], // French Southern and Antarctic Lands
     ['ZI020_GE43','ge:GENC:3:1-2:ATG','raw:ZI020_GE43','antigua_and_barbuda'], // Antigua and Barbuda
     ['ZI020_GE43','ge:GENC:3:1-2:AUS','raw:ZI020_GE43','commonwealth_of_australia'], // Commonwealth of Australia
     ['ZI020_GE43','ge:GENC:3:1-2:AUT','raw:ZI020_GE43','republic_of_austria'], // Republic of Austria
     ['ZI020_GE43','ge:GENC:3:1-2:AX1','raw:ZI020_GE43','unknown'], // Unknown
     ['ZI020_GE43','ge:GENC:3:1-2:AX2','raw:ZI020_GE43','guantanamo_bay_naval_base'], // Guantanamo Bay Naval Base
     ['ZI020_GE43','ge:GENC:3:1-2:AZE','raw:ZI020_GE43','republic_of_azerbaijan'], // Republic of Azerbaijan
     ['ZI020_GE43','ge:GENC:3:1-2:BDI','raw:ZI020_GE43','republic_of_burundi'], // Republic of Burundi
     ['ZI020_GE43','ge:GENC:3:1-2:BEL','raw:ZI020_GE43','kingdom_of_belgium'], // Kingdom of Belgium
     ['ZI020_GE43','ge:GENC:3:1-2:BEN','raw:ZI020_GE43','republic_of_benin'], // Republic of Benin
     ['ZI020_GE43','ge:GENC:3:1-2:BES','raw:ZI020_GE43','bonaire,_sint_eustatius,_and_saba'], // Bonaire, Sint Eustatius, and Saba
     ['ZI020_GE43','ge:GENC:3:1-2:BFA','raw:ZI020_GE43','burkina_faso'], // Burkina Faso
     ['ZI020_GE43','ge:GENC:3:1-2:BGD','raw:ZI020_GE43','people's_republic_of_bangladesh'], // People's Republic of Bangladesh
     ['ZI020_GE43','ge:GENC:3:1-2:BGR','raw:ZI020_GE43','republic_of_bulgaria'], // Republic of Bulgaria
     ['ZI020_GE43','ge:GENC:3:1-2:BHR','raw:ZI020_GE43','kingdom_of_bahrain'], // Kingdom of Bahrain
     ['ZI020_GE43','ge:GENC:3:1-2:BHS','raw:ZI020_GE43','commonwealth_of_the_bahamas'], // Commonwealth of The Bahamas
     ['ZI020_GE43','ge:GENC:3:1-2:BLM','raw:ZI020_GE43','saint_barthelemy'], // Saint Barthelemy
     ['ZI020_GE43','ge:GENC:3:1-2:BLR','raw:ZI020_GE43','republic_of_belarus'], // Republic of Belarus
     ['ZI020_GE43','ge:GENC:3:1-2:BOL','raw:ZI020_GE43','plurinational_state_of_bolivia'], // Plurinational State of Bolivia
     ['ZI020_GE43','ge:GENC:3:1-2:BRA','raw:ZI020_GE43','federative_republic_of_brazil'], // Federative Republic of Brazil
     ['ZI020_GE43','ge:GENC:3:1-2:BRN','raw:ZI020_GE43','brunei_darussalam'], // Brunei Darussalam
     ['ZI020_GE43','ge:GENC:3:1-2:BTN','raw:ZI020_GE43','kingdom_of_bhutan'], // Kingdom of Bhutan
     ['ZI020_GE43','ge:GENC:3:1-2:BWA','raw:ZI020_GE43','republic_of_botswana'], // Republic of Botswana
     ['ZI020_GE43','ge:GENC:3:1-2:CAF','raw:ZI020_GE43','central_african_republic'], // Central African Republic
     ['ZI020_GE43','ge:GENC:3:1-2:CAN','raw:ZI020_GE43','canada'], // Canada
     ['ZI020_GE43','ge:GENC:3:1-2:CCK','raw:ZI020_GE43','territory_of_cocos_(keeling)_islands'], // Territory of Cocos (Keeling) Islands
     ['ZI020_GE43','ge:GENC:3:1-2:CHE','raw:ZI020_GE43','swiss_confederation'], // Swiss Confederation
     ['ZI020_GE43','ge:GENC:3:1-2:CHL','raw:ZI020_GE43','republic_of_chile'], // Republic of Chile
     ['ZI020_GE43','ge:GENC:3:1-2:CHN','raw:ZI020_GE43','people's_republic_of_china'], // People's Republic of China
     ['ZI020_GE43','ge:GENC:3:1-2:CIV','raw:ZI020_GE43','republic_of_côte_d'ivoire'], // Republic of Côte d'Ivoire
     ['ZI020_GE43','ge:GENC:3:1-2:CMR','raw:ZI020_GE43','republic_of_cameroon'], // Republic of Cameroon
     ['ZI020_GE43','ge:GENC:3:1-2:COD','raw:ZI020_GE43','democratic_republic_of_the_congo'], // Democratic Republic of the Congo
     ['ZI020_GE43','ge:GENC:3:1-2:COG','raw:ZI020_GE43','republic_of_the_congo'], // Republic of the Congo
     ['ZI020_GE43','ge:GENC:3:1-2:COK','raw:ZI020_GE43','cook_islands'], // Cook Islands
     ['ZI020_GE43','ge:GENC:3:1-2:COL','raw:ZI020_GE43','republic_of_colombia'], // Republic of Colombia
     ['ZI020_GE43','ge:GENC:3:1-2:COM','raw:ZI020_GE43','union_of_the_comoros'], // Union of the Comoros
     ['ZI020_GE43','ge:GENC:3:1-2:CPT','raw:ZI020_GE43','clipperton_island'], // Clipperton Island
     ['ZI020_GE43','ge:GENC:3:1-2:CPV','raw:ZI020_GE43','republic_of_cape_verde'], // Republic of Cape Verde
     ['ZI020_GE43','ge:GENC:3:1-2:CRI','raw:ZI020_GE43','republic_of_costa_rica'], // Republic of Costa Rica
     ['ZI020_GE43','ge:GENC:3:1-2:CUB','raw:ZI020_GE43','republic_of_cuba'], // Republic of Cuba
     ['ZI020_GE43','ge:GENC:3:1-2:CUW','raw:ZI020_GE43','curaçao'], // Curaçao
     ['ZI020_GE43','ge:GENC:3:1-2:CXR','raw:ZI020_GE43','territory_of_christmas_island'], // Territory of Christmas Island
     ['ZI020_GE43','ge:GENC:3:1-2:CYM','raw:ZI020_GE43','cayman_islands'], // Cayman Islands
     ['ZI020_GE43','ge:GENC:3:1-2:CYP','raw:ZI020_GE43','republic_of_cyprus'], // Republic of Cyprus
     ['ZI020_GE43','ge:GENC:3:1-2:CZE','raw:ZI020_GE43','czech_republic'], // Czech Republic
     ['ZI020_GE43','ge:GENC:3:1-2:DEU','raw:ZI020_GE43','federal_republic_of_germany'], // Federal Republic of Germany
     ['ZI020_GE43','ge:GENC:3:1-2:DGA','raw:ZI020_GE43','diego_garcia'], // Diego Garcia
     ['ZI020_GE43','ge:GENC:3:1-2:DJI','raw:ZI020_GE43','republic_of_djibouti'], // Republic of Djibouti
     ['ZI020_GE43','ge:GENC:3:1-2:DMA','raw:ZI020_GE43','commonwealth_of_dominica'], // Commonwealth of Dominica
     ['ZI020_GE43','ge:GENC:3:1-2:DNK','raw:ZI020_GE43','kingdom_of_denmark'], // Kingdom of Denmark
     ['ZI020_GE43','ge:GENC:3:1-2:DOM','raw:ZI020_GE43','dominican_republic'], // Dominican Republic
     ['ZI020_GE43','ge:GENC:3:1-2:DZA','raw:ZI020_GE43','people's_democratic_republic_of_algeria'], // People's Democratic Republic of Algeria
     ['ZI020_GE43','ge:GENC:3:1-2:ECU','raw:ZI020_GE43','republic_of_ecuador'], // Republic of Ecuador
     ['ZI020_GE43','ge:GENC:3:1-2:EGY','raw:ZI020_GE43','arab_republic_of_egypt'], // Arab Republic of Egypt
     ['ZI020_GE43','ge:GENC:3:1-2:ERI','raw:ZI020_GE43','state_of_eritrea'], // State of Eritrea
     ['ZI020_GE43','ge:GENC:3:1-2:ESH','raw:ZI020_GE43','western_sahara'], // Western Sahara
     ['ZI020_GE43','ge:GENC:3:1-2:ESP','raw:ZI020_GE43','kingdom_of_spain'], // Kingdom of Spain
     ['ZI020_GE43','ge:GENC:3:1-2:EST','raw:ZI020_GE43','republic_of_estonia'], // Republic of Estonia
     ['ZI020_GE43','ge:GENC:3:1-2:ETH','raw:ZI020_GE43','federal_democratic_republic_of_ethiopia'], // Federal Democratic Republic of Ethiopia
     ['ZI020_GE43','ge:GENC:3:1-2:FIN','raw:ZI020_GE43','republic_of_finland'], // Republic of Finland
     ['ZI020_GE43','ge:GENC:3:1-2:FJI','raw:ZI020_GE43','republic_of_fiji'], // Republic of Fiji
     ['ZI020_GE43','ge:GENC:3:1-2:FLK','raw:ZI020_GE43','falkland_islands_(islas_malvinas)'], // Falkland Islands (Islas Malvinas)
     ['ZI020_GE43','ge:GENC:3:1-2:FRA','raw:ZI020_GE43','french_republic'], // French Republic
     ['ZI020_GE43','ge:GENC:3:1-2:FRO','raw:ZI020_GE43','faroe_islands'], // Faroe Islands
     ['ZI020_GE43','ge:GENC:3:1-2:FSM','raw:ZI020_GE43','federated_states_of_micronesia'], // Federated States of Micronesia
     ['ZI020_GE43','ge:GENC:3:1-2:GAB','raw:ZI020_GE43','gabonese_republic'], // Gabonese Republic
     ['ZI020_GE43','ge:GENC:3:1-2:GBR','raw:ZI020_GE43','united_kingdom_of_great_britain_and_northern_ireland'], // United Kingdom of Great Britain and Northern Ireland
     ['ZI020_GE43','ge:GENC:3:1-2:GEO','raw:ZI020_GE43','georgia'], // Georgia
     ['ZI020_GE43','ge:GENC:3:1-2:GGY','raw:ZI020_GE43','bailiwick_of_guernsey'], // Bailiwick of Guernsey
     ['ZI020_GE43','ge:GENC:3:1-2:GHA','raw:ZI020_GE43','republic_of_ghana'], // Republic of Ghana
     ['ZI020_GE43','ge:GENC:3:1-2:GIN','raw:ZI020_GE43','republic_of_guinea'], // Republic of Guinea
     ['ZI020_GE43','ge:GENC:3:1-2:GLP','raw:ZI020_GE43','department_of_guadeloupe'], // Department of Guadeloupe
     ['ZI020_GE43','ge:GENC:3:1-2:GMB','raw:ZI020_GE43','republic_of_the_gambia'], // Republic of The Gambia
     ['ZI020_GE43','ge:GENC:3:1-2:GNB','raw:ZI020_GE43','republic_of_guinea-bissau'], // Republic of Guinea-Bissau
     ['ZI020_GE43','ge:GENC:3:1-2:GNQ','raw:ZI020_GE43','republic_of_equatorial_guinea'], // Republic of Equatorial Guinea
     ['ZI020_GE43','ge:GENC:3:1-2:GRC','raw:ZI020_GE43','hellenic_republic'], // Hellenic Republic
     ['ZI020_GE43','ge:GENC:3:1-2:GTM','raw:ZI020_GE43','republic_of_guatemala'], // Republic of Guatemala
     ['ZI020_GE43','ge:GENC:3:1-2:GUF','raw:ZI020_GE43','department_of_guiana'], // Department of Guiana
     ['ZI020_GE43','ge:GENC:3:1-2:GUM','raw:ZI020_GE43','territory_of_guam'], // Territory of Guam
     ['ZI020_GE43','ge:GENC:3:1-2:GUY','raw:ZI020_GE43','co-operative_republic_of_guyana'], // Co-operative Republic of Guyana
     ['ZI020_GE43','ge:GENC:3:1-2:HKG','raw:ZI020_GE43','hong_kong_special_administrative_region'], // Hong Kong Special Administrative Region
     ['ZI020_GE43','ge:GENC:3:1-2:HMD','raw:ZI020_GE43','territory_of_heard_island_and_mcdonald_islands'], // Territory of Heard Island and McDonald Islands
     ['ZI020_GE43','ge:GENC:3:1-2:HND','raw:ZI020_GE43','republic_of_honduras'], // Republic of Honduras
     ['ZI020_GE43','ge:GENC:3:1-2:HRV','raw:ZI020_GE43','republic_of_croatia'], // Republic of Croatia
     ['ZI020_GE43','ge:GENC:3:1-2:HTI','raw:ZI020_GE43','republic_of_haiti'], // Republic of Haiti
     ['ZI020_GE43','ge:GENC:3:1-2:IDN','raw:ZI020_GE43','republic_of_indonesia'], // Republic of Indonesia
     ['ZI020_GE43','ge:GENC:3:1-2:IND','raw:ZI020_GE43','republic_of_india'], // Republic of India
     ['ZI020_GE43','ge:GENC:3:1-2:IOT','raw:ZI020_GE43','british_indian_ocean_territory'], // British Indian Ocean Territory
     ['ZI020_GE43','ge:GENC:3:1-2:IRN','raw:ZI020_GE43','islamic_republic_of_iran'], // Islamic Republic of Iran
     ['ZI020_GE43','ge:GENC:3:1-2:IRQ','raw:ZI020_GE43','republic_of_iraq'], // Republic of Iraq
     ['ZI020_GE43','ge:GENC:3:1-2:ISL','raw:ZI020_GE43','republic_of_iceland'], // Republic of Iceland
     ['ZI020_GE43','ge:GENC:3:1-2:ISR','raw:ZI020_GE43','state_of_israel'], // State of Israel
     ['ZI020_GE43','ge:GENC:3:1-2:ITA','raw:ZI020_GE43','italian_republic'], // Italian Republic
     ['ZI020_GE43','ge:GENC:3:1-2:JEY','raw:ZI020_GE43','bailiwick_of_jersey'], // Bailiwick of Jersey
     ['ZI020_GE43','ge:GENC:3:1-2:JOR','raw:ZI020_GE43','hashemite_kingdom_of_jordan'], // Hashemite Kingdom of Jordan
     ['ZI020_GE43','ge:GENC:3:1-2:KAZ','raw:ZI020_GE43','republic_of_kazakhstan'], // Republic of Kazakhstan
     ['ZI020_GE43','ge:GENC:3:1-2:KEN','raw:ZI020_GE43','republic_of_kenya'], // Republic of Kenya
     ['ZI020_GE43','ge:GENC:3:1-2:KGZ','raw:ZI020_GE43','kyrgyz_republic'], // Kyrgyz Republic
     ['ZI020_GE43','ge:GENC:3:1-2:KHM','raw:ZI020_GE43','kingdom_of_cambodia'], // Kingdom of Cambodia
     ['ZI020_GE43','ge:GENC:3:1-2:KIR','raw:ZI020_GE43','republic_of_kiribati'], // Republic of Kiribati
     ['ZI020_GE43','ge:GENC:3:1-2:KNA','raw:ZI020_GE43','federation_of_saint_kitts_and_nevis'], // Federation of Saint Kitts and Nevis
     ['ZI020_GE43','ge:GENC:3:1-2:KOR','raw:ZI020_GE43','republic_of_korea'], // Republic of Korea
     ['ZI020_GE43','ge:GENC:3:1-2:KWT','raw:ZI020_GE43','state_of_kuwait'], // State of Kuwait
     ['ZI020_GE43','ge:GENC:3:1-2:LAO','raw:ZI020_GE43','lao_people's_democratic_republic'], // Lao People's Democratic Republic
     ['ZI020_GE43','ge:GENC:3:1-2:LBN','raw:ZI020_GE43','lebanese_republic'], // Lebanese Republic
     ['ZI020_GE43','ge:GENC:3:1-2:LBR','raw:ZI020_GE43','republic_of_liberia'], // Republic of Liberia
     ['ZI020_GE43','ge:GENC:3:1-2:LIE','raw:ZI020_GE43','principality_of_liechtenstein'], // Principality of Liechtenstein
     ['ZI020_GE43','ge:GENC:3:1-2:LKA','raw:ZI020_GE43','democratic_socialist_republic_of_sri_lanka'], // Democratic Socialist Republic of Sri Lanka
     ['ZI020_GE43','ge:GENC:3:1-2:LSO','raw:ZI020_GE43','kingdom_of_lesotho'], // Kingdom of Lesotho
     ['ZI020_GE43','ge:GENC:3:1-2:LTU','raw:ZI020_GE43','republic_of_lithuania'], // Republic of Lithuania
     ['ZI020_GE43','ge:GENC:3:1-2:LUX','raw:ZI020_GE43','grand_duchy_of_luxembourg'], // Grand Duchy of Luxembourg
     ['ZI020_GE43','ge:GENC:3:1-2:LVA','raw:ZI020_GE43','republic_of_latvia'], // Republic of Latvia
     ['ZI020_GE43','ge:GENC:3:1-2:MAC','raw:ZI020_GE43','macau_special_administrative_region'], // Macau Special Administrative Region
     ['ZI020_GE43','ge:GENC:3:1-2:MAF','raw:ZI020_GE43','saint_martin'], // Saint Martin
     ['ZI020_GE43','ge:GENC:3:1-2:MAR','raw:ZI020_GE43','kingdom_of_morocco'], // Kingdom of Morocco
     ['ZI020_GE43','ge:GENC:3:1-2:MCO','raw:ZI020_GE43','principality_of_monaco'], // Principality of Monaco
     ['ZI020_GE43','ge:GENC:3:1-2:MDA','raw:ZI020_GE43','republic_of_moldova'], // Republic of Moldova
     ['ZI020_GE43','ge:GENC:3:1-2:MDG','raw:ZI020_GE43','republic_of_madagascar'], // Republic of Madagascar
     ['ZI020_GE43','ge:GENC:3:1-2:MDV','raw:ZI020_GE43','republic_of_maldives'], // Republic of Maldives
     ['ZI020_GE43','ge:GENC:3:1-2:MEX','raw:ZI020_GE43','united_mexican_states'], // United Mexican States
     ['ZI020_GE43','ge:GENC:3:1-2:MHL','raw:ZI020_GE43','republic_of_the_marshall_islands'], // Republic of the Marshall Islands
     ['ZI020_GE43','ge:GENC:3:1-2:MKD','raw:ZI020_GE43','republic_of_macedonia'], // Republic of Macedonia
     ['ZI020_GE43','ge:GENC:3:1-2:MLI','raw:ZI020_GE43','republic_of_mali'], // Republic of Mali
     ['ZI020_GE43','ge:GENC:3:1-2:MLT','raw:ZI020_GE43','republic_of_malta'], // Republic of Malta
     ['ZI020_GE43','ge:GENC:3:1-2:MMR','raw:ZI020_GE43','union_of_burma'], // Union of Burma
     ['ZI020_GE43','ge:GENC:3:1-2:MNG','raw:ZI020_GE43','mongolia'], // Mongolia
     ['ZI020_GE43','ge:GENC:3:1-2:MNP','raw:ZI020_GE43','commonwealth_of_the_northern_mariana_islands'], // Commonwealth of the Northern Mariana Islands
     ['ZI020_GE43','ge:GENC:3:1-2:MOZ','raw:ZI020_GE43','republic_of_mozambique'], // Republic of Mozambique
     ['ZI020_GE43','ge:GENC:3:1-2:MRT','raw:ZI020_GE43','islamic_republic_of_mauritania'], // Islamic Republic of Mauritania
     ['ZI020_GE43','ge:GENC:3:1-2:MTQ','raw:ZI020_GE43','department_of_martinique'], // Department of Martinique
     ['ZI020_GE43','ge:GENC:3:1-2:MUS','raw:ZI020_GE43','republic_of_mauritius'], // Republic of Mauritius
     ['ZI020_GE43','ge:GENC:3:1-2:MWI','raw:ZI020_GE43','republic_of_malawi'], // Republic of Malawi
     ['ZI020_GE43','ge:GENC:3:1-2:MYS','raw:ZI020_GE43','malaysia'], // Malaysia
     ['ZI020_GE43','ge:GENC:3:1-2:MYT','raw:ZI020_GE43','department_of_mayotte'], // Department of Mayotte
     ['ZI020_GE43','ge:GENC:3:1-2:NAM','raw:ZI020_GE43','republic_of_namibia'], // Republic of Namibia
     ['ZI020_GE43','ge:GENC:3:1-2:NCL','raw:ZI020_GE43','new_caledonia'], // New Caledonia
     ['ZI020_GE43','ge:GENC:3:1-2:NER','raw:ZI020_GE43','republic_of_the_niger'], // Republic of the Niger
     ['ZI020_GE43','ge:GENC:3:1-2:NFK','raw:ZI020_GE43','territory_of_norfolk_island'], // Territory of Norfolk Island
     ['ZI020_GE43','ge:GENC:3:1-2:NGA','raw:ZI020_GE43','federal_republic_of_nigeria'], // Federal Republic of Nigeria
     ['ZI020_GE43','ge:GENC:3:1-2:NIC','raw:ZI020_GE43','republic_of_nicaragua'], // Republic of Nicaragua
     ['ZI020_GE43','ge:GENC:3:1-2:NLD','raw:ZI020_GE43','kingdom_of_the_netherlands'], // Kingdom of the Netherlands
     ['ZI020_GE43','ge:GENC:3:1-2:NOR','raw:ZI020_GE43','kingdom_of_norway'], // Kingdom of Norway
     ['ZI020_GE43','ge:GENC:3:1-2:NPL','raw:ZI020_GE43','federal_democratic_republic_of_nepal'], // Federal Democratic Republic of Nepal
     ['ZI020_GE43','ge:GENC:3:1-2:NRU','raw:ZI020_GE43','republic_of_nauru'], // Republic of Nauru
     ['ZI020_GE43','ge:GENC:3:1-2:NZL','raw:ZI020_GE43','new_zealand'], // New Zealand
     ['ZI020_GE43','ge:GENC:3:1-2:OMN','raw:ZI020_GE43','sultanate_of_oman'], // Sultanate of Oman
     ['ZI020_GE43','ge:GENC:3:1-2:PAK','raw:ZI020_GE43','islamic_republic_of_pakistan'], // Islamic Republic of Pakistan
     ['ZI020_GE43','ge:GENC:3:1-2:PAN','raw:ZI020_GE43','republic_of_panama'], // Republic of Panama
     ['ZI020_GE43','ge:GENC:3:1-2:PCN','raw:ZI020_GE43','pitcairn,_henderson,_ducie,_and_oeno_islands'], // Pitcairn, Henderson, Ducie, and Oeno Islands
     ['ZI020_GE43','ge:GENC:3:1-2:PER','raw:ZI020_GE43','republic_of_peru'], // Republic of Peru
     ['ZI020_GE43','ge:GENC:3:1-2:PHL','raw:ZI020_GE43','republic_of_the_philippines'], // Republic of the Philippines
     ['ZI020_GE43','ge:GENC:3:1-2:PLW','raw:ZI020_GE43','republic_of_palau'], // Republic of Palau
     ['ZI020_GE43','ge:GENC:3:1-2:POL','raw:ZI020_GE43','republic_of_poland'], // Republic of Poland
     ['ZI020_GE43','ge:GENC:3:1-2:PRI','raw:ZI020_GE43','commonwealth_of_puerto_rico'], // Commonwealth of Puerto Rico
     ['ZI020_GE43','ge:GENC:3:1-2:PRK','raw:ZI020_GE43','democratic_people's_republic_of_korea'], // Democratic People's Republic of Korea
     ['ZI020_GE43','ge:GENC:3:1-2:PRT','raw:ZI020_GE43','portuguese_republic'], // Portuguese Republic
     ['ZI020_GE43','ge:GENC:3:1-2:PRY','raw:ZI020_GE43','republic_of_paraguay'], // Republic of Paraguay
     ['ZI020_GE43','ge:GENC:3:1-2:PSE','raw:ZI020_GE43','palestinian_territory'], // Palestinian Territory
     ['ZI020_GE43','ge:GENC:3:1-2:PYF','raw:ZI020_GE43','french_polynesia'], // French Polynesia
     ['ZI020_GE43','ge:GENC:3:1-2:QAT','raw:ZI020_GE43','state_of_qatar'], // State of Qatar
     ['ZI020_GE43','ge:GENC:3:1-2:REU','raw:ZI020_GE43','department_of_reunion'], // Department of Reunion
     ['ZI020_GE43','ge:GENC:3:1-2:RUS','raw:ZI020_GE43','russian_federation'], // Russian Federation
     ['ZI020_GE43','ge:GENC:3:1-2:RWA','raw:ZI020_GE43','republic_of_rwanda'], // Republic of Rwanda
     ['ZI020_GE43','ge:GENC:3:1-2:SAU','raw:ZI020_GE43','kingdom_of_saudi_arabia'], // Kingdom of Saudi Arabia
     ['ZI020_GE43','ge:GENC:3:1-2:SDN','raw:ZI020_GE43','republic_of_the_sudan'], // Republic of the Sudan
     ['ZI020_GE43','ge:GENC:3:1-2:SEN','raw:ZI020_GE43','republic_of_senegal'], // Republic of Senegal
     ['ZI020_GE43','ge:GENC:3:1-2:SGP','raw:ZI020_GE43','republic_of_singapore'], // Republic of Singapore
     ['ZI020_GE43','ge:GENC:3:1-2:SGS','raw:ZI020_GE43','south_georgia_and_south_sandwich_islands'], // South Georgia and South Sandwich Islands
     ['ZI020_GE43','ge:GENC:3:1-2:SHN','raw:ZI020_GE43','saint_helena,_ascension,_and_tristan_da_cunha'], // Saint Helena, Ascension, and Tristan da Cunha
     ['ZI020_GE43','ge:GENC:3:1-2:SLB','raw:ZI020_GE43','solomon_islands'], // Solomon Islands
     ['ZI020_GE43','ge:GENC:3:1-2:SLE','raw:ZI020_GE43','republic_of_sierra_leone'], // Republic of Sierra Leone
     ['ZI020_GE43','ge:GENC:3:1-2:SLV','raw:ZI020_GE43','republic_of_el_salvador'], // Republic of El Salvador
     ['ZI020_GE43','ge:GENC:3:1-2:SMR','raw:ZI020_GE43','republic_of_san_marino'], // Republic of San Marino
     ['ZI020_GE43','ge:GENC:3:1-2:SOM','raw:ZI020_GE43','somalia,_federal_republic_of'], // Somalia, Federal Republic of
     ['ZI020_GE43','ge:GENC:3:1-2:SPM','raw:ZI020_GE43','territorial_collectivity_of_saint_pierre_and_miquelon'], // Territorial Collectivity of Saint Pierre and Miquelon
     ['ZI020_GE43','ge:GENC:3:1-2:SRB','raw:ZI020_GE43','republic_of_serbia'], // Republic of Serbia
     ['ZI020_GE43','ge:GENC:3:1-2:SSD','raw:ZI020_GE43','republic_of_south_sudan'], // Republic of South Sudan
     ['ZI020_GE43','ge:GENC:3:1-2:STP','raw:ZI020_GE43','democratic_republic_of_sao_tome_and_principe'], // Democratic Republic of Sao Tome and Principe
     ['ZI020_GE43','ge:GENC:3:1-2:SUR','raw:ZI020_GE43','republic_of_suriname'], // Republic of Suriname
     ['ZI020_GE43','ge:GENC:3:1-2:SVK','raw:ZI020_GE43','slovak_republic'], // Slovak Republic
     ['ZI020_GE43','ge:GENC:3:1-2:SVN','raw:ZI020_GE43','republic_of_slovenia'], // Republic of Slovenia
     ['ZI020_GE43','ge:GENC:3:1-2:SWE','raw:ZI020_GE43','kingdom_of_sweden'], // Kingdom of Sweden
     ['ZI020_GE43','ge:GENC:3:1-2:SWZ','raw:ZI020_GE43','kingdom_of_swaziland'], // Kingdom of Swaziland
     ['ZI020_GE43','ge:GENC:3:1-2:SXM','raw:ZI020_GE43','sint_maarten'], // Sint Maarten
     ['ZI020_GE43','ge:GENC:3:1-2:SYC','raw:ZI020_GE43','republic_of_seychelles'], // Republic of Seychelles
     ['ZI020_GE43','ge:GENC:3:1-2:SYR','raw:ZI020_GE43','syrian_arab_republic'], // Syrian Arab Republic
     ['ZI020_GE43','ge:GENC:3:1-2:TCA','raw:ZI020_GE43','turks_and_caicos_islands'], // Turks and Caicos Islands
     ['ZI020_GE43','ge:GENC:3:1-2:TCD','raw:ZI020_GE43','republic_of_chad'], // Republic of Chad
     ['ZI020_GE43','ge:GENC:3:1-2:TGO','raw:ZI020_GE43','togolese_republic'], // Togolese Republic
     ['ZI020_GE43','ge:GENC:3:1-2:THA','raw:ZI020_GE43','kingdom_of_thailand'], // Kingdom of Thailand
     ['ZI020_GE43','ge:GENC:3:1-2:TJK','raw:ZI020_GE43','republic_of_tajikistan'], // Republic of Tajikistan
     ['ZI020_GE43','ge:GENC:3:1-2:TLS','raw:ZI020_GE43','democratic_republic_of_timor-leste'], // Democratic Republic of Timor-Leste
     ['ZI020_GE43','ge:GENC:3:1-2:TON','raw:ZI020_GE43','kingdom_of_tonga'], // Kingdom of Tonga
     ['ZI020_GE43','ge:GENC:3:1-2:TTO','raw:ZI020_GE43','republic_of_trinidad_and_tobago'], // Republic of Trinidad and Tobago
     ['ZI020_GE43','ge:GENC:3:1-2:TUN','raw:ZI020_GE43','tunisian_republic'], // Tunisian Republic
     ['ZI020_GE43','ge:GENC:3:1-2:TUR','raw:ZI020_GE43','republic_of_turkey'], // Republic of Turkey
     ['ZI020_GE43','ge:GENC:3:1-2:TWN','raw:ZI020_GE43','taiwan'], // Taiwan
     ['ZI020_GE43','ge:GENC:3:1-2:TZA','raw:ZI020_GE43','united_republic_of_tanzania'], // United Republic of Tanzania
     ['ZI020_GE43','ge:GENC:3:1-2:UGA','raw:ZI020_GE43','republic_of_uganda'], // Republic of Uganda
     ['ZI020_GE43','ge:GENC:3:1-2:UKR','raw:ZI020_GE43','ukraine'], // Ukraine
     ['ZI020_GE43','ge:GENC:3:1-2:URY','raw:ZI020_GE43','oriental_republic_of_uruguay'], // Oriental Republic of Uruguay
     ['ZI020_GE43','ge:GENC:3:1-2:USA','raw:ZI020_GE43','united_states_of_america'], // United States of America
     ['ZI020_GE43','ge:GENC:3:1-2:UZB','raw:ZI020_GE43','republic_of_uzbekistan'], // Republic of Uzbekistan
     ['ZI020_GE43','ge:GENC:3:1-2:VAT','raw:ZI020_GE43','state_of_the_vatican_city'], // State of the Vatican City
     ['ZI020_GE43','ge:GENC:3:1-2:VCT','raw:ZI020_GE43','saint_vincent_and_the_grenadines'], // Saint Vincent and the Grenadines
     ['ZI020_GE43','ge:GENC:3:1-2:VEN','raw:ZI020_GE43','bolivarian_republic_of_venezuela'], // Bolivarian Republic of Venezuela
     ['ZI020_GE43','ge:GENC:3:1-2:VGB','raw:ZI020_GE43','virgin_islands,_british'], // Virgin Islands, British
     ['ZI020_GE43','ge:GENC:3:1-2:VIR','raw:ZI020_GE43','united_states_virgin_islands'], // United States Virgin Islands
     ['ZI020_GE43','ge:GENC:3:1-2:VNM','raw:ZI020_GE43','socialist_republic_of_vietnam'], // Socialist Republic of Vietnam
     ['ZI020_GE43','ge:GENC:3:1-2:VUT','raw:ZI020_GE43','republic_of_vanuatu'], // Republic of Vanuatu
     ['ZI020_GE43','ge:GENC:3:1-2:WLF','raw:ZI020_GE43','wallis_and_futuna'], // Wallis and Futuna
     ['ZI020_GE43','ge:GENC:3:1-2:WSM','raw:ZI020_GE43','independent_state_of_samoa'], // Independent State of Samoa
     ['ZI020_GE43','ge:GENC:3:1-2:XAC','raw:ZI020_GE43','territory_of_ashmore_and_cartier_islands'], // Territory of Ashmore and Cartier Islands
     ['ZI020_GE43','ge:GENC:3:1-2:XAZ','raw:ZI020_GE43','entity_1'], // Entity 1
     ['ZI020_GE43','ge:GENC:3:1-2:XBI','raw:ZI020_GE43','bassas_da_india'], // Bassas da India
     ['ZI020_GE43','ge:GENC:3:1-2:XBK','raw:ZI020_GE43','baker_island'], // Baker Island
     ['ZI020_GE43','ge:GENC:3:1-2:XCR','raw:ZI020_GE43','entity_2'], // Entity 2
     ['ZI020_GE43','ge:GENC:3:1-2:XCS','raw:ZI020_GE43','coral_sea_islands_territory'], // Coral Sea Islands Territory
     ['ZI020_GE43','ge:GENC:3:1-2:XCY','raw:ZI020_GE43','entity_3'], // Entity 3
     ['ZI020_GE43','ge:GENC:3:1-2:XEU','raw:ZI020_GE43','europa_island'], // Europa Island
     ['ZI020_GE43','ge:GENC:3:1-2:XGL','raw:ZI020_GE43','glorioso_islands'], // Glorioso Islands
     ['ZI020_GE43','ge:GENC:3:1-2:XGZ','raw:ZI020_GE43','gaza_strip'], // Gaza Strip
     ['ZI020_GE43','ge:GENC:3:1-2:XHO','raw:ZI020_GE43','howland_island'], // Howland Island
     ['ZI020_GE43','ge:GENC:3:1-2:XJA','raw:ZI020_GE43','johnston_atoll'], // Johnston Atoll
     ['ZI020_GE43','ge:GENC:3:1-2:XJM','raw:ZI020_GE43','jan_mayen'], // Jan Mayen
     ['ZI020_GE43','ge:GENC:3:1-2:XJN','raw:ZI020_GE43','juan_de_nova_island'], // Juan de Nova Island
     ['ZI020_GE43','ge:GENC:3:1-2:XJV','raw:ZI020_GE43','jarvis_island'], // Jarvis Island
     ['ZI020_GE43','ge:GENC:3:1-2:XKM','raw:ZI020_GE43','entity_4'], // Entity 4
     ['ZI020_GE43','ge:GENC:3:1-2:XKN','raw:ZI020_GE43','entity_5'], // Entity 5
     ['ZI020_GE43','ge:GENC:3:1-2:XKR','raw:ZI020_GE43','kingman_reef'], // Kingman Reef
     ['ZI020_GE43','ge:GENC:3:1-2:XKS','raw:ZI020_GE43','republic_of_kosovo'], // Republic of Kosovo
     ['ZI020_GE43','ge:GENC:3:1-2:XMW','raw:ZI020_GE43','midway_islands'], // Midway Islands
     ['ZI020_GE43','ge:GENC:3:1-2:XNV','raw:ZI020_GE43','navassa_island'], // Navassa Island
     ['ZI020_GE43','ge:GENC:3:1-2:XPL','raw:ZI020_GE43','palmyra_atoll'], // Palmyra Atoll
     ['ZI020_GE43','ge:GENC:3:1-2:XPR','raw:ZI020_GE43','paracel_islands'], // Paracel Islands
     ['ZI020_GE43','ge:GENC:3:1-2:XQP','raw:ZI020_GE43','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // Etorofu, Habomai, Kunashiri, and Shikotan Islands
     ['ZI020_GE43','ge:GENC:3:1-2:XQZ','raw:ZI020_GE43','akrotiri'], // Akrotiri
     ['ZI020_GE43','ge:GENC:3:1-2:XSP','raw:ZI020_GE43','spratly_islands'], // Spratly Islands
     ['ZI020_GE43','ge:GENC:3:1-2:XSV','raw:ZI020_GE43','svalbard'], // Svalbard
     ['ZI020_GE43','ge:GENC:3:1-2:XTR','raw:ZI020_GE43','tromelin_island'], // Tromelin Island
     ['ZI020_GE43','ge:GENC:3:1-2:XWB','raw:ZI020_GE43','west_bank'], // West Bank
     ['ZI020_GE43','ge:GENC:3:1-2:XWK','raw:ZI020_GE43','wake_island'], // Wake Island
     ['ZI020_GE43','ge:GENC:3:1-2:XXD','raw:ZI020_GE43','dhekelia'], // Dhekelia
     ['ZI020_GE43','ge:GENC:3:1-2:XXX','raw:ZI020_GE43','no_man's_land'], // No Man's Land
     ['ZI020_GE43','ge:GENC:3:1-2:YEM','raw:ZI020_GE43','republic_of_yemen'], // Republic of Yemen
     ['ZI020_GE43','ge:GENC:3:1-2:ZAF','raw:ZI020_GE43','republic_of_south_africa'], // Republic of South Africa
     ['ZI020_GE43','ge:GENC:3:1-2:ZMB','raw:ZI020_GE43','republic_of_zambia'], // Republic of Zambia
     ['ZI020_GE43','ge:GENC:3:1-2:ZWE','raw:ZI020_GE43','republic_of_zimbabwe'], // Republic of Zimbabwe
     ['ZI020_GE43','ge:ISO1:3:VI-15:AIA','raw:ZI020_GE43','anguilla'], // Anguilla
     ['ZI020_GE43','ge:ISO1:3:VI-15:ATA','raw:ZI020_GE43','antarctica'], // Antarctica
     ['ZI020_GE43','ge:ISO1:3:VI-15:BIH','raw:ZI020_GE43','bosnia_and_herzegovina'], // Bosnia and Herzegovina
     ['ZI020_GE43','ge:ISO1:3:VI-15:BLZ','raw:ZI020_GE43','belize'], // Belize
     ['ZI020_GE43','ge:ISO1:3:VI-15:BMU','raw:ZI020_GE43','bermuda'], // Bermuda
     ['ZI020_GE43','ge:ISO1:3:VI-15:BRB','raw:ZI020_GE43','barbados'], // Barbados
     ['ZI020_GE43','ge:ISO1:3:VI-15:BVT','raw:ZI020_GE43','bouvet_island'], // Bouvet Island
     ['ZI020_GE43','ge:ISO1:3:VI-15:GIB','raw:ZI020_GE43','gibraltar'], // Gibraltar
     ['ZI020_GE43','ge:ISO1:3:VI-15:GRD','raw:ZI020_GE43','grenada'], // Grenada
     ['ZI020_GE43','ge:ISO1:3:VI-15:GRL','raw:ZI020_GE43','greenland'], // Greenland
     ['ZI020_GE43','ge:ISO1:3:VI-15:HUN','raw:ZI020_GE43','hungary'], // Hungary
     ['ZI020_GE43','ge:ISO1:3:VI-15:IMN','raw:ZI020_GE43','isle_of_man'], // Isle of Man
     ['ZI020_GE43','ge:ISO1:3:VI-15:IRL','raw:ZI020_GE43','ireland'], // Ireland
     ['ZI020_GE43','ge:ISO1:3:VI-15:JAM','raw:ZI020_GE43','jamaica'], // Jamaica
     ['ZI020_GE43','ge:ISO1:3:VI-15:JPN','raw:ZI020_GE43','japan'], // Japan
     ['ZI020_GE43','ge:ISO1:3:VI-15:LBY','raw:ZI020_GE43','libya'], // Libya
     ['ZI020_GE43','ge:ISO1:3:VI-15:LCA','raw:ZI020_GE43','saint_lucia'], // Saint Lucia
     ['ZI020_GE43','ge:ISO1:3:VI-15:MNE','raw:ZI020_GE43','montenegro'], // Montenegro
     ['ZI020_GE43','ge:ISO1:3:VI-15:MSR','raw:ZI020_GE43','montserrat'], // Montserrat
     ['ZI020_GE43','ge:ISO1:3:VI-15:NIU','raw:ZI020_GE43','niue'], // Niue
     ['ZI020_GE43','ge:ISO1:3:VI-15:PNG','raw:ZI020_GE43','independent_state_of_papua_new_guinea'], // Independent State of Papua New Guinea
     ['ZI020_GE43','ge:ISO1:3:VI-15:ROU','raw:ZI020_GE43','romania'], // Romania
     ['ZI020_GE43','ge:ISO1:3:VI-15:TKL','raw:ZI020_GE43','tokelau'], // Tokelau
     ['ZI020_GE43','ge:ISO1:3:VI-15:TKM','raw:ZI020_GE43','turkmenistan'], // Turkmenistan
     ['ZI020_GE43','ge:ISO1:3:VI-15:TUV','raw:ZI020_GE43','tuvalu'], // Tuvalu
     ['ZI020_GE43','noInformation','raw:ZI020_GE43','noinformation'], // noInformation
     ['ZI020_GE43','other','raw:ZI020_GE43','other'], // Other

     // ZI020_GE44 - Location Country Designation : GENC Short URN-based Identifier fourth
     ['ZI020_GE44','ge:GENC:3:1-2:ABW','raw:ZI020_GE44','aruba'], // Aruba
     ['ZI020_GE44','ge:GENC:3:1-2:AFG','raw:ZI020_GE44','islamic_republic_of_afghanistan'], // Islamic Republic of Afghanistan
     ['ZI020_GE44','ge:GENC:3:1-2:AGO','raw:ZI020_GE44','republic_of_angola'], // Republic of Angola
     ['ZI020_GE44','ge:GENC:3:1-2:ALB','raw:ZI020_GE44','republic_of_albania'], // Republic of Albania
     ['ZI020_GE44','ge:GENC:3:1-2:AND','raw:ZI020_GE44','principality_of_andorra'], // Principality of Andorra
     ['ZI020_GE44','ge:GENC:3:1-2:ARE','raw:ZI020_GE44','united_arab_emirates'], // United Arab Emirates
     ['ZI020_GE44','ge:GENC:3:1-2:ARG','raw:ZI020_GE44','argentine_republic'], // Argentine Republic
     ['ZI020_GE44','ge:GENC:3:1-2:ARM','raw:ZI020_GE44','republic_of_armenia'], // Republic of Armenia
     ['ZI020_GE44','ge:GENC:3:1-2:ASM','raw:ZI020_GE44','territory_of_american_samoa'], // Territory of American Samoa
     ['ZI020_GE44','ge:GENC:3:1-2:ATF','raw:ZI020_GE44','french_southern_and_antarctic_lands'], // French Southern and Antarctic Lands
     ['ZI020_GE44','ge:GENC:3:1-2:ATG','raw:ZI020_GE44','antigua_and_barbuda'], // Antigua and Barbuda
     ['ZI020_GE44','ge:GENC:3:1-2:AUS','raw:ZI020_GE44','commonwealth_of_australia'], // Commonwealth of Australia
     ['ZI020_GE44','ge:GENC:3:1-2:AUT','raw:ZI020_GE44','republic_of_austria'], // Republic of Austria
     ['ZI020_GE44','ge:GENC:3:1-2:AX1','raw:ZI020_GE44','unknown'], // Unknown
     ['ZI020_GE44','ge:GENC:3:1-2:AX2','raw:ZI020_GE44','guantanamo_bay_naval_base'], // Guantanamo Bay Naval Base
     ['ZI020_GE44','ge:GENC:3:1-2:AZE','raw:ZI020_GE44','republic_of_azerbaijan'], // Republic of Azerbaijan
     ['ZI020_GE44','ge:GENC:3:1-2:BDI','raw:ZI020_GE44','republic_of_burundi'], // Republic of Burundi
     ['ZI020_GE44','ge:GENC:3:1-2:BEL','raw:ZI020_GE44','kingdom_of_belgium'], // Kingdom of Belgium
     ['ZI020_GE44','ge:GENC:3:1-2:BEN','raw:ZI020_GE44','republic_of_benin'], // Republic of Benin
     ['ZI020_GE44','ge:GENC:3:1-2:BES','raw:ZI020_GE44','bonaire,_sint_eustatius,_and_saba'], // Bonaire, Sint Eustatius, and Saba
     ['ZI020_GE44','ge:GENC:3:1-2:BFA','raw:ZI020_GE44','burkina_faso'], // Burkina Faso
     ['ZI020_GE44','ge:GENC:3:1-2:BGD','raw:ZI020_GE44','people's_republic_of_bangladesh'], // People's Republic of Bangladesh
     ['ZI020_GE44','ge:GENC:3:1-2:BGR','raw:ZI020_GE44','republic_of_bulgaria'], // Republic of Bulgaria
     ['ZI020_GE44','ge:GENC:3:1-2:BHR','raw:ZI020_GE44','kingdom_of_bahrain'], // Kingdom of Bahrain
     ['ZI020_GE44','ge:GENC:3:1-2:BHS','raw:ZI020_GE44','commonwealth_of_the_bahamas'], // Commonwealth of The Bahamas
     ['ZI020_GE44','ge:GENC:3:1-2:BLM','raw:ZI020_GE44','saint_barthelemy'], // Saint Barthelemy
     ['ZI020_GE44','ge:GENC:3:1-2:BLR','raw:ZI020_GE44','republic_of_belarus'], // Republic of Belarus
     ['ZI020_GE44','ge:GENC:3:1-2:BOL','raw:ZI020_GE44','plurinational_state_of_bolivia'], // Plurinational State of Bolivia
     ['ZI020_GE44','ge:GENC:3:1-2:BRA','raw:ZI020_GE44','federative_republic_of_brazil'], // Federative Republic of Brazil
     ['ZI020_GE44','ge:GENC:3:1-2:BRN','raw:ZI020_GE44','brunei_darussalam'], // Brunei Darussalam
     ['ZI020_GE44','ge:GENC:3:1-2:BTN','raw:ZI020_GE44','kingdom_of_bhutan'], // Kingdom of Bhutan
     ['ZI020_GE44','ge:GENC:3:1-2:BWA','raw:ZI020_GE44','republic_of_botswana'], // Republic of Botswana
     ['ZI020_GE44','ge:GENC:3:1-2:CAF','raw:ZI020_GE44','central_african_republic'], // Central African Republic
     ['ZI020_GE44','ge:GENC:3:1-2:CAN','raw:ZI020_GE44','canada'], // Canada
     ['ZI020_GE44','ge:GENC:3:1-2:CCK','raw:ZI020_GE44','territory_of_cocos_(keeling)_islands'], // Territory of Cocos (Keeling) Islands
     ['ZI020_GE44','ge:GENC:3:1-2:CHE','raw:ZI020_GE44','swiss_confederation'], // Swiss Confederation
     ['ZI020_GE44','ge:GENC:3:1-2:CHL','raw:ZI020_GE44','republic_of_chile'], // Republic of Chile
     ['ZI020_GE44','ge:GENC:3:1-2:CHN','raw:ZI020_GE44','people's_republic_of_china'], // People's Republic of China
     ['ZI020_GE44','ge:GENC:3:1-2:CIV','raw:ZI020_GE44','republic_of_côte_d'ivoire'], // Republic of Côte d'Ivoire
     ['ZI020_GE44','ge:GENC:3:1-2:CMR','raw:ZI020_GE44','republic_of_cameroon'], // Republic of Cameroon
     ['ZI020_GE44','ge:GENC:3:1-2:COD','raw:ZI020_GE44','democratic_republic_of_the_congo'], // Democratic Republic of the Congo
     ['ZI020_GE44','ge:GENC:3:1-2:COG','raw:ZI020_GE44','republic_of_the_congo'], // Republic of the Congo
     ['ZI020_GE44','ge:GENC:3:1-2:COK','raw:ZI020_GE44','cook_islands'], // Cook Islands
     ['ZI020_GE44','ge:GENC:3:1-2:COL','raw:ZI020_GE44','republic_of_colombia'], // Republic of Colombia
     ['ZI020_GE44','ge:GENC:3:1-2:COM','raw:ZI020_GE44','union_of_the_comoros'], // Union of the Comoros
     ['ZI020_GE44','ge:GENC:3:1-2:CPT','raw:ZI020_GE44','clipperton_island'], // Clipperton Island
     ['ZI020_GE44','ge:GENC:3:1-2:CPV','raw:ZI020_GE44','republic_of_cape_verde'], // Republic of Cape Verde
     ['ZI020_GE44','ge:GENC:3:1-2:CRI','raw:ZI020_GE44','republic_of_costa_rica'], // Republic of Costa Rica
     ['ZI020_GE44','ge:GENC:3:1-2:CUB','raw:ZI020_GE44','republic_of_cuba'], // Republic of Cuba
     ['ZI020_GE44','ge:GENC:3:1-2:CUW','raw:ZI020_GE44','curaçao'], // Curaçao
     ['ZI020_GE44','ge:GENC:3:1-2:CXR','raw:ZI020_GE44','territory_of_christmas_island'], // Territory of Christmas Island
     ['ZI020_GE44','ge:GENC:3:1-2:CYM','raw:ZI020_GE44','cayman_islands'], // Cayman Islands
     ['ZI020_GE44','ge:GENC:3:1-2:CYP','raw:ZI020_GE44','republic_of_cyprus'], // Republic of Cyprus
     ['ZI020_GE44','ge:GENC:3:1-2:CZE','raw:ZI020_GE44','czech_republic'], // Czech Republic
     ['ZI020_GE44','ge:GENC:3:1-2:DEU','raw:ZI020_GE44','federal_republic_of_germany'], // Federal Republic of Germany
     ['ZI020_GE44','ge:GENC:3:1-2:DGA','raw:ZI020_GE44','diego_garcia'], // Diego Garcia
     ['ZI020_GE44','ge:GENC:3:1-2:DJI','raw:ZI020_GE44','republic_of_djibouti'], // Republic of Djibouti
     ['ZI020_GE44','ge:GENC:3:1-2:DMA','raw:ZI020_GE44','commonwealth_of_dominica'], // Commonwealth of Dominica
     ['ZI020_GE44','ge:GENC:3:1-2:DNK','raw:ZI020_GE44','kingdom_of_denmark'], // Kingdom of Denmark
     ['ZI020_GE44','ge:GENC:3:1-2:DOM','raw:ZI020_GE44','dominican_republic'], // Dominican Republic
     ['ZI020_GE44','ge:GENC:3:1-2:DZA','raw:ZI020_GE44','people's_democratic_republic_of_algeria'], // People's Democratic Republic of Algeria
     ['ZI020_GE44','ge:GENC:3:1-2:ECU','raw:ZI020_GE44','republic_of_ecuador'], // Republic of Ecuador
     ['ZI020_GE44','ge:GENC:3:1-2:EGY','raw:ZI020_GE44','arab_republic_of_egypt'], // Arab Republic of Egypt
     ['ZI020_GE44','ge:GENC:3:1-2:ERI','raw:ZI020_GE44','state_of_eritrea'], // State of Eritrea
     ['ZI020_GE44','ge:GENC:3:1-2:ESH','raw:ZI020_GE44','western_sahara'], // Western Sahara
     ['ZI020_GE44','ge:GENC:3:1-2:ESP','raw:ZI020_GE44','kingdom_of_spain'], // Kingdom of Spain
     ['ZI020_GE44','ge:GENC:3:1-2:EST','raw:ZI020_GE44','republic_of_estonia'], // Republic of Estonia
     ['ZI020_GE44','ge:GENC:3:1-2:ETH','raw:ZI020_GE44','federal_democratic_republic_of_ethiopia'], // Federal Democratic Republic of Ethiopia
     ['ZI020_GE44','ge:GENC:3:1-2:FIN','raw:ZI020_GE44','republic_of_finland'], // Republic of Finland
     ['ZI020_GE44','ge:GENC:3:1-2:FJI','raw:ZI020_GE44','republic_of_fiji'], // Republic of Fiji
     ['ZI020_GE44','ge:GENC:3:1-2:FLK','raw:ZI020_GE44','falkland_islands_(islas_malvinas)'], // Falkland Islands (Islas Malvinas)
     ['ZI020_GE44','ge:GENC:3:1-2:FRA','raw:ZI020_GE44','french_republic'], // French Republic
     ['ZI020_GE44','ge:GENC:3:1-2:FRO','raw:ZI020_GE44','faroe_islands'], // Faroe Islands
     ['ZI020_GE44','ge:GENC:3:1-2:FSM','raw:ZI020_GE44','federated_states_of_micronesia'], // Federated States of Micronesia
     ['ZI020_GE44','ge:GENC:3:1-2:GAB','raw:ZI020_GE44','gabonese_republic'], // Gabonese Republic
     ['ZI020_GE44','ge:GENC:3:1-2:GBR','raw:ZI020_GE44','united_kingdom_of_great_britain_and_northern_ireland'], // United Kingdom of Great Britain and Northern Ireland
     ['ZI020_GE44','ge:GENC:3:1-2:GEO','raw:ZI020_GE44','georgia'], // Georgia
     ['ZI020_GE44','ge:GENC:3:1-2:GGY','raw:ZI020_GE44','bailiwick_of_guernsey'], // Bailiwick of Guernsey
     ['ZI020_GE44','ge:GENC:3:1-2:GHA','raw:ZI020_GE44','republic_of_ghana'], // Republic of Ghana
     ['ZI020_GE44','ge:GENC:3:1-2:GIN','raw:ZI020_GE44','republic_of_guinea'], // Republic of Guinea
     ['ZI020_GE44','ge:GENC:3:1-2:GLP','raw:ZI020_GE44','department_of_guadeloupe'], // Department of Guadeloupe
     ['ZI020_GE44','ge:GENC:3:1-2:GMB','raw:ZI020_GE44','republic_of_the_gambia'], // Republic of The Gambia
     ['ZI020_GE44','ge:GENC:3:1-2:GNB','raw:ZI020_GE44','republic_of_guinea-bissau'], // Republic of Guinea-Bissau
     ['ZI020_GE44','ge:GENC:3:1-2:GNQ','raw:ZI020_GE44','republic_of_equatorial_guinea'], // Republic of Equatorial Guinea
     ['ZI020_GE44','ge:GENC:3:1-2:GRC','raw:ZI020_GE44','hellenic_republic'], // Hellenic Republic
     ['ZI020_GE44','ge:GENC:3:1-2:GTM','raw:ZI020_GE44','republic_of_guatemala'], // Republic of Guatemala
     ['ZI020_GE44','ge:GENC:3:1-2:GUF','raw:ZI020_GE44','department_of_guiana'], // Department of Guiana
     ['ZI020_GE44','ge:GENC:3:1-2:GUM','raw:ZI020_GE44','territory_of_guam'], // Territory of Guam
     ['ZI020_GE44','ge:GENC:3:1-2:GUY','raw:ZI020_GE44','co-operative_republic_of_guyana'], // Co-operative Republic of Guyana
     ['ZI020_GE44','ge:GENC:3:1-2:HKG','raw:ZI020_GE44','hong_kong_special_administrative_region'], // Hong Kong Special Administrative Region
     ['ZI020_GE44','ge:GENC:3:1-2:HMD','raw:ZI020_GE44','territory_of_heard_island_and_mcdonald_islands'], // Territory of Heard Island and McDonald Islands
     ['ZI020_GE44','ge:GENC:3:1-2:HND','raw:ZI020_GE44','republic_of_honduras'], // Republic of Honduras
     ['ZI020_GE44','ge:GENC:3:1-2:HRV','raw:ZI020_GE44','republic_of_croatia'], // Republic of Croatia
     ['ZI020_GE44','ge:GENC:3:1-2:HTI','raw:ZI020_GE44','republic_of_haiti'], // Republic of Haiti
     ['ZI020_GE44','ge:GENC:3:1-2:IDN','raw:ZI020_GE44','republic_of_indonesia'], // Republic of Indonesia
     ['ZI020_GE44','ge:GENC:3:1-2:IND','raw:ZI020_GE44','republic_of_india'], // Republic of India
     ['ZI020_GE44','ge:GENC:3:1-2:IOT','raw:ZI020_GE44','british_indian_ocean_territory'], // British Indian Ocean Territory
     ['ZI020_GE44','ge:GENC:3:1-2:IRN','raw:ZI020_GE44','islamic_republic_of_iran'], // Islamic Republic of Iran
     ['ZI020_GE44','ge:GENC:3:1-2:IRQ','raw:ZI020_GE44','republic_of_iraq'], // Republic of Iraq
     ['ZI020_GE44','ge:GENC:3:1-2:ISL','raw:ZI020_GE44','republic_of_iceland'], // Republic of Iceland
     ['ZI020_GE44','ge:GENC:3:1-2:ISR','raw:ZI020_GE44','state_of_israel'], // State of Israel
     ['ZI020_GE44','ge:GENC:3:1-2:ITA','raw:ZI020_GE44','italian_republic'], // Italian Republic
     ['ZI020_GE44','ge:GENC:3:1-2:JEY','raw:ZI020_GE44','bailiwick_of_jersey'], // Bailiwick of Jersey
     ['ZI020_GE44','ge:GENC:3:1-2:JOR','raw:ZI020_GE44','hashemite_kingdom_of_jordan'], // Hashemite Kingdom of Jordan
     ['ZI020_GE44','ge:GENC:3:1-2:KAZ','raw:ZI020_GE44','republic_of_kazakhstan'], // Republic of Kazakhstan
     ['ZI020_GE44','ge:GENC:3:1-2:KEN','raw:ZI020_GE44','republic_of_kenya'], // Republic of Kenya
     ['ZI020_GE44','ge:GENC:3:1-2:KGZ','raw:ZI020_GE44','kyrgyz_republic'], // Kyrgyz Republic
     ['ZI020_GE44','ge:GENC:3:1-2:KHM','raw:ZI020_GE44','kingdom_of_cambodia'], // Kingdom of Cambodia
     ['ZI020_GE44','ge:GENC:3:1-2:KIR','raw:ZI020_GE44','republic_of_kiribati'], // Republic of Kiribati
     ['ZI020_GE44','ge:GENC:3:1-2:KNA','raw:ZI020_GE44','federation_of_saint_kitts_and_nevis'], // Federation of Saint Kitts and Nevis
     ['ZI020_GE44','ge:GENC:3:1-2:KOR','raw:ZI020_GE44','republic_of_korea'], // Republic of Korea
     ['ZI020_GE44','ge:GENC:3:1-2:KWT','raw:ZI020_GE44','state_of_kuwait'], // State of Kuwait
     ['ZI020_GE44','ge:GENC:3:1-2:LAO','raw:ZI020_GE44','lao_people's_democratic_republic'], // Lao People's Democratic Republic
     ['ZI020_GE44','ge:GENC:3:1-2:LBN','raw:ZI020_GE44','lebanese_republic'], // Lebanese Republic
     ['ZI020_GE44','ge:GENC:3:1-2:LBR','raw:ZI020_GE44','republic_of_liberia'], // Republic of Liberia
     ['ZI020_GE44','ge:GENC:3:1-2:LIE','raw:ZI020_GE44','principality_of_liechtenstein'], // Principality of Liechtenstein
     ['ZI020_GE44','ge:GENC:3:1-2:LKA','raw:ZI020_GE44','democratic_socialist_republic_of_sri_lanka'], // Democratic Socialist Republic of Sri Lanka
     ['ZI020_GE44','ge:GENC:3:1-2:LSO','raw:ZI020_GE44','kingdom_of_lesotho'], // Kingdom of Lesotho
     ['ZI020_GE44','ge:GENC:3:1-2:LTU','raw:ZI020_GE44','republic_of_lithuania'], // Republic of Lithuania
     ['ZI020_GE44','ge:GENC:3:1-2:LUX','raw:ZI020_GE44','grand_duchy_of_luxembourg'], // Grand Duchy of Luxembourg
     ['ZI020_GE44','ge:GENC:3:1-2:LVA','raw:ZI020_GE44','republic_of_latvia'], // Republic of Latvia
     ['ZI020_GE44','ge:GENC:3:1-2:MAC','raw:ZI020_GE44','macau_special_administrative_region'], // Macau Special Administrative Region
     ['ZI020_GE44','ge:GENC:3:1-2:MAF','raw:ZI020_GE44','saint_martin'], // Saint Martin
     ['ZI020_GE44','ge:GENC:3:1-2:MAR','raw:ZI020_GE44','kingdom_of_morocco'], // Kingdom of Morocco
     ['ZI020_GE44','ge:GENC:3:1-2:MCO','raw:ZI020_GE44','principality_of_monaco'], // Principality of Monaco
     ['ZI020_GE44','ge:GENC:3:1-2:MDA','raw:ZI020_GE44','republic_of_moldova'], // Republic of Moldova
     ['ZI020_GE44','ge:GENC:3:1-2:MDG','raw:ZI020_GE44','republic_of_madagascar'], // Republic of Madagascar
     ['ZI020_GE44','ge:GENC:3:1-2:MDV','raw:ZI020_GE44','republic_of_maldives'], // Republic of Maldives
     ['ZI020_GE44','ge:GENC:3:1-2:MEX','raw:ZI020_GE44','united_mexican_states'], // United Mexican States
     ['ZI020_GE44','ge:GENC:3:1-2:MHL','raw:ZI020_GE44','republic_of_the_marshall_islands'], // Republic of the Marshall Islands
     ['ZI020_GE44','ge:GENC:3:1-2:MKD','raw:ZI020_GE44','republic_of_macedonia'], // Republic of Macedonia
     ['ZI020_GE44','ge:GENC:3:1-2:MLI','raw:ZI020_GE44','republic_of_mali'], // Republic of Mali
     ['ZI020_GE44','ge:GENC:3:1-2:MLT','raw:ZI020_GE44','republic_of_malta'], // Republic of Malta
     ['ZI020_GE44','ge:GENC:3:1-2:MMR','raw:ZI020_GE44','union_of_burma'], // Union of Burma
     ['ZI020_GE44','ge:GENC:3:1-2:MNG','raw:ZI020_GE44','mongolia'], // Mongolia
     ['ZI020_GE44','ge:GENC:3:1-2:MNP','raw:ZI020_GE44','commonwealth_of_the_northern_mariana_islands'], // Commonwealth of the Northern Mariana Islands
     ['ZI020_GE44','ge:GENC:3:1-2:MOZ','raw:ZI020_GE44','republic_of_mozambique'], // Republic of Mozambique
     ['ZI020_GE44','ge:GENC:3:1-2:MRT','raw:ZI020_GE44','islamic_republic_of_mauritania'], // Islamic Republic of Mauritania
     ['ZI020_GE44','ge:GENC:3:1-2:MTQ','raw:ZI020_GE44','department_of_martinique'], // Department of Martinique
     ['ZI020_GE44','ge:GENC:3:1-2:MUS','raw:ZI020_GE44','republic_of_mauritius'], // Republic of Mauritius
     ['ZI020_GE44','ge:GENC:3:1-2:MWI','raw:ZI020_GE44','republic_of_malawi'], // Republic of Malawi
     ['ZI020_GE44','ge:GENC:3:1-2:MYS','raw:ZI020_GE44','malaysia'], // Malaysia
     ['ZI020_GE44','ge:GENC:3:1-2:MYT','raw:ZI020_GE44','department_of_mayotte'], // Department of Mayotte
     ['ZI020_GE44','ge:GENC:3:1-2:NAM','raw:ZI020_GE44','republic_of_namibia'], // Republic of Namibia
     ['ZI020_GE44','ge:GENC:3:1-2:NCL','raw:ZI020_GE44','new_caledonia'], // New Caledonia
     ['ZI020_GE44','ge:GENC:3:1-2:NER','raw:ZI020_GE44','republic_of_the_niger'], // Republic of the Niger
     ['ZI020_GE44','ge:GENC:3:1-2:NFK','raw:ZI020_GE44','territory_of_norfolk_island'], // Territory of Norfolk Island
     ['ZI020_GE44','ge:GENC:3:1-2:NGA','raw:ZI020_GE44','federal_republic_of_nigeria'], // Federal Republic of Nigeria
     ['ZI020_GE44','ge:GENC:3:1-2:NIC','raw:ZI020_GE44','republic_of_nicaragua'], // Republic of Nicaragua
     ['ZI020_GE44','ge:GENC:3:1-2:NLD','raw:ZI020_GE44','kingdom_of_the_netherlands'], // Kingdom of the Netherlands
     ['ZI020_GE44','ge:GENC:3:1-2:NOR','raw:ZI020_GE44','kingdom_of_norway'], // Kingdom of Norway
     ['ZI020_GE44','ge:GENC:3:1-2:NPL','raw:ZI020_GE44','federal_democratic_republic_of_nepal'], // Federal Democratic Republic of Nepal
     ['ZI020_GE44','ge:GENC:3:1-2:NRU','raw:ZI020_GE44','republic_of_nauru'], // Republic of Nauru
     ['ZI020_GE44','ge:GENC:3:1-2:NZL','raw:ZI020_GE44','new_zealand'], // New Zealand
     ['ZI020_GE44','ge:GENC:3:1-2:OMN','raw:ZI020_GE44','sultanate_of_oman'], // Sultanate of Oman
     ['ZI020_GE44','ge:GENC:3:1-2:PAK','raw:ZI020_GE44','islamic_republic_of_pakistan'], // Islamic Republic of Pakistan
     ['ZI020_GE44','ge:GENC:3:1-2:PAN','raw:ZI020_GE44','republic_of_panama'], // Republic of Panama
     ['ZI020_GE44','ge:GENC:3:1-2:PCN','raw:ZI020_GE44','pitcairn,_henderson,_ducie,_and_oeno_islands'], // Pitcairn, Henderson, Ducie, and Oeno Islands
     ['ZI020_GE44','ge:GENC:3:1-2:PER','raw:ZI020_GE44','republic_of_peru'], // Republic of Peru
     ['ZI020_GE44','ge:GENC:3:1-2:PHL','raw:ZI020_GE44','republic_of_the_philippines'], // Republic of the Philippines
     ['ZI020_GE44','ge:GENC:3:1-2:PLW','raw:ZI020_GE44','republic_of_palau'], // Republic of Palau
     ['ZI020_GE44','ge:GENC:3:1-2:POL','raw:ZI020_GE44','republic_of_poland'], // Republic of Poland
     ['ZI020_GE44','ge:GENC:3:1-2:PRI','raw:ZI020_GE44','commonwealth_of_puerto_rico'], // Commonwealth of Puerto Rico
     ['ZI020_GE44','ge:GENC:3:1-2:PRK','raw:ZI020_GE44','democratic_people's_republic_of_korea'], // Democratic People's Republic of Korea
     ['ZI020_GE44','ge:GENC:3:1-2:PRT','raw:ZI020_GE44','portuguese_republic'], // Portuguese Republic
     ['ZI020_GE44','ge:GENC:3:1-2:PRY','raw:ZI020_GE44','republic_of_paraguay'], // Republic of Paraguay
     ['ZI020_GE44','ge:GENC:3:1-2:PSE','raw:ZI020_GE44','palestinian_territory'], // Palestinian Territory
     ['ZI020_GE44','ge:GENC:3:1-2:PYF','raw:ZI020_GE44','french_polynesia'], // French Polynesia
     ['ZI020_GE44','ge:GENC:3:1-2:QAT','raw:ZI020_GE44','state_of_qatar'], // State of Qatar
     ['ZI020_GE44','ge:GENC:3:1-2:REU','raw:ZI020_GE44','department_of_reunion'], // Department of Reunion
     ['ZI020_GE44','ge:GENC:3:1-2:RUS','raw:ZI020_GE44','russian_federation'], // Russian Federation
     ['ZI020_GE44','ge:GENC:3:1-2:RWA','raw:ZI020_GE44','republic_of_rwanda'], // Republic of Rwanda
     ['ZI020_GE44','ge:GENC:3:1-2:SAU','raw:ZI020_GE44','kingdom_of_saudi_arabia'], // Kingdom of Saudi Arabia
     ['ZI020_GE44','ge:GENC:3:1-2:SDN','raw:ZI020_GE44','republic_of_the_sudan'], // Republic of the Sudan
     ['ZI020_GE44','ge:GENC:3:1-2:SEN','raw:ZI020_GE44','republic_of_senegal'], // Republic of Senegal
     ['ZI020_GE44','ge:GENC:3:1-2:SGP','raw:ZI020_GE44','republic_of_singapore'], // Republic of Singapore
     ['ZI020_GE44','ge:GENC:3:1-2:SGS','raw:ZI020_GE44','south_georgia_and_south_sandwich_islands'], // South Georgia and South Sandwich Islands
     ['ZI020_GE44','ge:GENC:3:1-2:SHN','raw:ZI020_GE44','saint_helena,_ascension,_and_tristan_da_cunha'], // Saint Helena, Ascension, and Tristan da Cunha
     ['ZI020_GE44','ge:GENC:3:1-2:SLB','raw:ZI020_GE44','solomon_islands'], // Solomon Islands
     ['ZI020_GE44','ge:GENC:3:1-2:SLE','raw:ZI020_GE44','republic_of_sierra_leone'], // Republic of Sierra Leone
     ['ZI020_GE44','ge:GENC:3:1-2:SLV','raw:ZI020_GE44','republic_of_el_salvador'], // Republic of El Salvador
     ['ZI020_GE44','ge:GENC:3:1-2:SMR','raw:ZI020_GE44','republic_of_san_marino'], // Republic of San Marino
     ['ZI020_GE44','ge:GENC:3:1-2:SOM','raw:ZI020_GE44','somalia,_federal_republic_of'], // Somalia, Federal Republic of
     ['ZI020_GE44','ge:GENC:3:1-2:SPM','raw:ZI020_GE44','territorial_collectivity_of_saint_pierre_and_miquelon'], // Territorial Collectivity of Saint Pierre and Miquelon
     ['ZI020_GE44','ge:GENC:3:1-2:SRB','raw:ZI020_GE44','republic_of_serbia'], // Republic of Serbia
     ['ZI020_GE44','ge:GENC:3:1-2:SSD','raw:ZI020_GE44','republic_of_south_sudan'], // Republic of South Sudan
     ['ZI020_GE44','ge:GENC:3:1-2:STP','raw:ZI020_GE44','democratic_republic_of_sao_tome_and_principe'], // Democratic Republic of Sao Tome and Principe
     ['ZI020_GE44','ge:GENC:3:1-2:SUR','raw:ZI020_GE44','republic_of_suriname'], // Republic of Suriname
     ['ZI020_GE44','ge:GENC:3:1-2:SVK','raw:ZI020_GE44','slovak_republic'], // Slovak Republic
     ['ZI020_GE44','ge:GENC:3:1-2:SVN','raw:ZI020_GE44','republic_of_slovenia'], // Republic of Slovenia
     ['ZI020_GE44','ge:GENC:3:1-2:SWE','raw:ZI020_GE44','kingdom_of_sweden'], // Kingdom of Sweden
     ['ZI020_GE44','ge:GENC:3:1-2:SWZ','raw:ZI020_GE44','kingdom_of_swaziland'], // Kingdom of Swaziland
     ['ZI020_GE44','ge:GENC:3:1-2:SXM','raw:ZI020_GE44','sint_maarten'], // Sint Maarten
     ['ZI020_GE44','ge:GENC:3:1-2:SYC','raw:ZI020_GE44','republic_of_seychelles'], // Republic of Seychelles
     ['ZI020_GE44','ge:GENC:3:1-2:SYR','raw:ZI020_GE44','syrian_arab_republic'], // Syrian Arab Republic
     ['ZI020_GE44','ge:GENC:3:1-2:TCA','raw:ZI020_GE44','turks_and_caicos_islands'], // Turks and Caicos Islands
     ['ZI020_GE44','ge:GENC:3:1-2:TCD','raw:ZI020_GE44','republic_of_chad'], // Republic of Chad
     ['ZI020_GE44','ge:GENC:3:1-2:TGO','raw:ZI020_GE44','togolese_republic'], // Togolese Republic
     ['ZI020_GE44','ge:GENC:3:1-2:THA','raw:ZI020_GE44','kingdom_of_thailand'], // Kingdom of Thailand
     ['ZI020_GE44','ge:GENC:3:1-2:TJK','raw:ZI020_GE44','republic_of_tajikistan'], // Republic of Tajikistan
     ['ZI020_GE44','ge:GENC:3:1-2:TLS','raw:ZI020_GE44','democratic_republic_of_timor-leste'], // Democratic Republic of Timor-Leste
     ['ZI020_GE44','ge:GENC:3:1-2:TON','raw:ZI020_GE44','kingdom_of_tonga'], // Kingdom of Tonga
     ['ZI020_GE44','ge:GENC:3:1-2:TTO','raw:ZI020_GE44','republic_of_trinidad_and_tobago'], // Republic of Trinidad and Tobago
     ['ZI020_GE44','ge:GENC:3:1-2:TUN','raw:ZI020_GE44','tunisian_republic'], // Tunisian Republic
     ['ZI020_GE44','ge:GENC:3:1-2:TUR','raw:ZI020_GE44','republic_of_turkey'], // Republic of Turkey
     ['ZI020_GE44','ge:GENC:3:1-2:TWN','raw:ZI020_GE44','taiwan'], // Taiwan
     ['ZI020_GE44','ge:GENC:3:1-2:TZA','raw:ZI020_GE44','united_republic_of_tanzania'], // United Republic of Tanzania
     ['ZI020_GE44','ge:GENC:3:1-2:UGA','raw:ZI020_GE44','republic_of_uganda'], // Republic of Uganda
     ['ZI020_GE44','ge:GENC:3:1-2:UKR','raw:ZI020_GE44','ukraine'], // Ukraine
     ['ZI020_GE44','ge:GENC:3:1-2:URY','raw:ZI020_GE44','oriental_republic_of_uruguay'], // Oriental Republic of Uruguay
     ['ZI020_GE44','ge:GENC:3:1-2:USA','raw:ZI020_GE44','united_states_of_america'], // United States of America
     ['ZI020_GE44','ge:GENC:3:1-2:UZB','raw:ZI020_GE44','republic_of_uzbekistan'], // Republic of Uzbekistan
     ['ZI020_GE44','ge:GENC:3:1-2:VAT','raw:ZI020_GE44','state_of_the_vatican_city'], // State of the Vatican City
     ['ZI020_GE44','ge:GENC:3:1-2:VCT','raw:ZI020_GE44','saint_vincent_and_the_grenadines'], // Saint Vincent and the Grenadines
     ['ZI020_GE44','ge:GENC:3:1-2:VEN','raw:ZI020_GE44','bolivarian_republic_of_venezuela'], // Bolivarian Republic of Venezuela
     ['ZI020_GE44','ge:GENC:3:1-2:VGB','raw:ZI020_GE44','virgin_islands,_british'], // Virgin Islands, British
     ['ZI020_GE44','ge:GENC:3:1-2:VIR','raw:ZI020_GE44','united_states_virgin_islands'], // United States Virgin Islands
     ['ZI020_GE44','ge:GENC:3:1-2:VNM','raw:ZI020_GE44','socialist_republic_of_vietnam'], // Socialist Republic of Vietnam
     ['ZI020_GE44','ge:GENC:3:1-2:VUT','raw:ZI020_GE44','republic_of_vanuatu'], // Republic of Vanuatu
     ['ZI020_GE44','ge:GENC:3:1-2:WLF','raw:ZI020_GE44','wallis_and_futuna'], // Wallis and Futuna
     ['ZI020_GE44','ge:GENC:3:1-2:WSM','raw:ZI020_GE44','independent_state_of_samoa'], // Independent State of Samoa
     ['ZI020_GE44','ge:GENC:3:1-2:XAC','raw:ZI020_GE44','territory_of_ashmore_and_cartier_islands'], // Territory of Ashmore and Cartier Islands
     ['ZI020_GE44','ge:GENC:3:1-2:XAZ','raw:ZI020_GE44','entity_1'], // Entity 1
     ['ZI020_GE44','ge:GENC:3:1-2:XBI','raw:ZI020_GE44','bassas_da_india'], // Bassas da India
     ['ZI020_GE44','ge:GENC:3:1-2:XBK','raw:ZI020_GE44','baker_island'], // Baker Island
     ['ZI020_GE44','ge:GENC:3:1-2:XCR','raw:ZI020_GE44','entity_2'], // Entity 2
     ['ZI020_GE44','ge:GENC:3:1-2:XCS','raw:ZI020_GE44','coral_sea_islands_territory'], // Coral Sea Islands Territory
     ['ZI020_GE44','ge:GENC:3:1-2:XCY','raw:ZI020_GE44','entity_3'], // Entity 3
     ['ZI020_GE44','ge:GENC:3:1-2:XEU','raw:ZI020_GE44','europa_island'], // Europa Island
     ['ZI020_GE44','ge:GENC:3:1-2:XGL','raw:ZI020_GE44','glorioso_islands'], // Glorioso Islands
     ['ZI020_GE44','ge:GENC:3:1-2:XGZ','raw:ZI020_GE44','gaza_strip'], // Gaza Strip
     ['ZI020_GE44','ge:GENC:3:1-2:XHO','raw:ZI020_GE44','howland_island'], // Howland Island
     ['ZI020_GE44','ge:GENC:3:1-2:XJA','raw:ZI020_GE44','johnston_atoll'], // Johnston Atoll
     ['ZI020_GE44','ge:GENC:3:1-2:XJM','raw:ZI020_GE44','jan_mayen'], // Jan Mayen
     ['ZI020_GE44','ge:GENC:3:1-2:XJN','raw:ZI020_GE44','juan_de_nova_island'], // Juan de Nova Island
     ['ZI020_GE44','ge:GENC:3:1-2:XJV','raw:ZI020_GE44','jarvis_island'], // Jarvis Island
     ['ZI020_GE44','ge:GENC:3:1-2:XKM','raw:ZI020_GE44','entity_4'], // Entity 4
     ['ZI020_GE44','ge:GENC:3:1-2:XKN','raw:ZI020_GE44','entity_5'], // Entity 5
     ['ZI020_GE44','ge:GENC:3:1-2:XKR','raw:ZI020_GE44','kingman_reef'], // Kingman Reef
     ['ZI020_GE44','ge:GENC:3:1-2:XKS','raw:ZI020_GE44','republic_of_kosovo'], // Republic of Kosovo
     ['ZI020_GE44','ge:GENC:3:1-2:XMW','raw:ZI020_GE44','midway_islands'], // Midway Islands
     ['ZI020_GE44','ge:GENC:3:1-2:XNV','raw:ZI020_GE44','navassa_island'], // Navassa Island
     ['ZI020_GE44','ge:GENC:3:1-2:XPL','raw:ZI020_GE44','palmyra_atoll'], // Palmyra Atoll
     ['ZI020_GE44','ge:GENC:3:1-2:XPR','raw:ZI020_GE44','paracel_islands'], // Paracel Islands
     ['ZI020_GE44','ge:GENC:3:1-2:XQP','raw:ZI020_GE44','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // Etorofu, Habomai, Kunashiri, and Shikotan Islands
     ['ZI020_GE44','ge:GENC:3:1-2:XQZ','raw:ZI020_GE44','akrotiri'], // Akrotiri
     ['ZI020_GE44','ge:GENC:3:1-2:XSP','raw:ZI020_GE44','spratly_islands'], // Spratly Islands
     ['ZI020_GE44','ge:GENC:3:1-2:XSV','raw:ZI020_GE44','svalbard'], // Svalbard
     ['ZI020_GE44','ge:GENC:3:1-2:XTR','raw:ZI020_GE44','tromelin_island'], // Tromelin Island
     ['ZI020_GE44','ge:GENC:3:1-2:XWB','raw:ZI020_GE44','west_bank'], // West Bank
     ['ZI020_GE44','ge:GENC:3:1-2:XWK','raw:ZI020_GE44','wake_island'], // Wake Island
     ['ZI020_GE44','ge:GENC:3:1-2:XXD','raw:ZI020_GE44','dhekelia'], // Dhekelia
     ['ZI020_GE44','ge:GENC:3:1-2:XXX','raw:ZI020_GE44','no_man's_land'], // No Man's Land
     ['ZI020_GE44','ge:GENC:3:1-2:YEM','raw:ZI020_GE44','republic_of_yemen'], // Republic of Yemen
     ['ZI020_GE44','ge:GENC:3:1-2:ZAF','raw:ZI020_GE44','republic_of_south_africa'], // Republic of South Africa
     ['ZI020_GE44','ge:GENC:3:1-2:ZMB','raw:ZI020_GE44','republic_of_zambia'], // Republic of Zambia
     ['ZI020_GE44','ge:GENC:3:1-2:ZWE','raw:ZI020_GE44','republic_of_zimbabwe'], // Republic of Zimbabwe
     ['ZI020_GE44','ge:ISO1:3:VI-15:AIA','raw:ZI020_GE44','anguilla'], // Anguilla
     ['ZI020_GE44','ge:ISO1:3:VI-15:ATA','raw:ZI020_GE44','antarctica'], // Antarctica
     ['ZI020_GE44','ge:ISO1:3:VI-15:BIH','raw:ZI020_GE44','bosnia_and_herzegovina'], // Bosnia and Herzegovina
     ['ZI020_GE44','ge:ISO1:3:VI-15:BLZ','raw:ZI020_GE44','belize'], // Belize
     ['ZI020_GE44','ge:ISO1:3:VI-15:BMU','raw:ZI020_GE44','bermuda'], // Bermuda
     ['ZI020_GE44','ge:ISO1:3:VI-15:BRB','raw:ZI020_GE44','barbados'], // Barbados
     ['ZI020_GE44','ge:ISO1:3:VI-15:BVT','raw:ZI020_GE44','bouvet_island'], // Bouvet Island
     ['ZI020_GE44','ge:ISO1:3:VI-15:GIB','raw:ZI020_GE44','gibraltar'], // Gibraltar
     ['ZI020_GE44','ge:ISO1:3:VI-15:GRD','raw:ZI020_GE44','grenada'], // Grenada
     ['ZI020_GE44','ge:ISO1:3:VI-15:GRL','raw:ZI020_GE44','greenland'], // Greenland
     ['ZI020_GE44','ge:ISO1:3:VI-15:HUN','raw:ZI020_GE44','hungary'], // Hungary
     ['ZI020_GE44','ge:ISO1:3:VI-15:IMN','raw:ZI020_GE44','isle_of_man'], // Isle of Man
     ['ZI020_GE44','ge:ISO1:3:VI-15:IRL','raw:ZI020_GE44','ireland'], // Ireland
     ['ZI020_GE44','ge:ISO1:3:VI-15:JAM','raw:ZI020_GE44','jamaica'], // Jamaica
     ['ZI020_GE44','ge:ISO1:3:VI-15:JPN','raw:ZI020_GE44','japan'], // Japan
     ['ZI020_GE44','ge:ISO1:3:VI-15:LBY','raw:ZI020_GE44','libya'], // Libya
     ['ZI020_GE44','ge:ISO1:3:VI-15:LCA','raw:ZI020_GE44','saint_lucia'], // Saint Lucia
     ['ZI020_GE44','ge:ISO1:3:VI-15:MNE','raw:ZI020_GE44','montenegro'], // Montenegro
     ['ZI020_GE44','ge:ISO1:3:VI-15:MSR','raw:ZI020_GE44','montserrat'], // Montserrat
     ['ZI020_GE44','ge:ISO1:3:VI-15:NIU','raw:ZI020_GE44','niue'], // Niue
     ['ZI020_GE44','ge:ISO1:3:VI-15:PNG','raw:ZI020_GE44','independent_state_of_papua_new_guinea'], // Independent State of Papua New Guinea
     ['ZI020_GE44','ge:ISO1:3:VI-15:ROU','raw:ZI020_GE44','romania'], // Romania
     ['ZI020_GE44','ge:ISO1:3:VI-15:TKL','raw:ZI020_GE44','tokelau'], // Tokelau
     ['ZI020_GE44','ge:ISO1:3:VI-15:TKM','raw:ZI020_GE44','turkmenistan'], // Turkmenistan
     ['ZI020_GE44','ge:ISO1:3:VI-15:TUV','raw:ZI020_GE44','tuvalu'], // Tuvalu
     ['ZI020_GE44','noInformation','raw:ZI020_GE44','noinformation'], // noInformation
     ['ZI020_GE44','other','raw:ZI020_GE44','other'], // Other

     // ZI024_HYP - Water Resource Information : Hydrologic Persistence
     // ['ZI024_HYP','-999999',undefined,undefined], // noInformation
     ['ZI024_HYP','1','raw:ZI024_HYP','perennial'], // Perennial
     ['ZI024_HYP','2','raw:ZI024_HYP','intermittent'], // Intermittent
     ['ZI024_HYP','4','raw:ZI024_HYP','dry'], // Dry

     // ZI024_SCC - Water Resource Information : Water Type
     // ['ZI024_SCC','-999999',undefined,undefined], // noInformation
     ['ZI024_SCC','1','raw:ZI024_SCC','alkaline'], // Alkaline
     ['ZI024_SCC','4','raw:ZI024_SCC','mineral'], // Mineral
     ['ZI024_SCC','10','raw:ZI024_SCC','saline'], // Saline
     ['ZI024_SCC','11','raw:ZI024_SCC','fresh'], // Fresh
     ['ZI024_SCC','12','raw:ZI024_SCC','brackish'], // Brackish
     ['ZI024_SCC','13','raw:ZI024_SCC','seawater'], // Seawater
     ['ZI024_SCC','14','raw:ZI024_SCC','brine'], // Brine
     ['ZI024_SCC','998','raw:ZI024_SCC','not_applicable'], // Not Applicable

     // ZI024_YWQ - Water Resource Information : Water Potability
     // ['ZI024_YWQ','-999999',undefined,undefined], // noInformation
     ['ZI024_YWQ','1','raw:ZI024_YWQ','potable'], // Potable
     ['ZI024_YWQ','2','raw:ZI024_YWQ','treatable'], // Treatable
     ['ZI024_YWQ','3','raw:ZI024_YWQ','contaminated'], // Contaminated
     ['ZI024_YWQ','4','raw:ZI024_YWQ','nonpotable'], // Nonpotable
     ['ZI024_YWQ','999','raw:ZI024_YWQ','other'], // Other

     // ZI025_WLE - Hydrographic Vertical Positioning Information : Water Level Effect
     // ['ZI025_WLE','-999999',undefined,undefined], // noInformation
     ['ZI025_WLE','1','raw:ZI025_WLE','partly_submerged'], // Partly Submerged
     ['ZI025_WLE','2','raw:ZI025_WLE','always_dry'], // Always Dry
     ['ZI025_WLE','3','raw:ZI025_WLE','always_submerged'], // Always Submerged
     ['ZI025_WLE','4','raw:ZI025_WLE','covers_and_uncovers'], // Covers and Uncovers
     ['ZI025_WLE','5','raw:ZI025_WLE','awash_at_low_water'], // Awash at Low Water
     ['ZI025_WLE','9','raw:ZI025_WLE','awash_at_chart_datum'], // Awash at Chart Datum
     ['ZI025_WLE','999','raw:ZI025_WLE','other'], // Other

     // ZI026_CTUC - Feature Metadata : Cartographic Usability Range <interval closure>
     ['ZI026_CTUC','2','raw:ZI026_CTUC','open_interval'], // Open Interval
     ['ZI026_CTUC','3','raw:ZI026_CTUC','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['ZI026_CTUC','4','raw:ZI026_CTUC','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['ZI026_CTUC','5','raw:ZI026_CTUC','closed_interval'], // Closed Interval
     ['ZI026_CTUC','6','raw:ZI026_CTUC','greater-than_semi-interval'], // Greater-than Semi-interval
     ['ZI026_CTUC','7','raw:ZI026_CTUC','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['ZI026_CTUC','8','raw:ZI026_CTUC','less-than_semi-interval'], // Less-than Semi-interval
     ['ZI026_CTUC','9','raw:ZI026_CTUC','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // ZI026_SUR - Feature Metadata : Survey Coverage Category
     // ['ZI026_SUR','-999999',undefined,undefined], // noInformation
     ['ZI026_SUR','1','raw:ZI026_SUR','surveyed'], // Surveyed
     ['ZI026_SUR','2','raw:ZI026_SUR','inadequately_surveyed'], // Inadequately Surveyed
     ['ZI026_SUR','3','raw:ZI026_SUR','unsurveyed'], // Unsurveyed

     // ZI032_GUG - Pylon Information : Guyed
     // ['ZI032_GUG','-999999',undefined,undefined], // noInformation
     ['ZI032_GUG','1000','raw:ZI032_GUG','no'],
     ['ZI032_GUG','1001','raw:ZI032_GUG','yes'],

     // ZI032_PYC - Pylon Information : Pylon Configuration
     // ['ZI032_PYC','-999999',undefined,undefined], // noInformation
     ['ZI032_PYC','1','raw:ZI032_PYC',''a''], // 'A'
     ['ZI032_PYC','2','raw:ZI032_PYC',''h''], // 'H'
     ['ZI032_PYC','3','raw:ZI032_PYC',''i''], // 'I'
     ['ZI032_PYC','4','raw:ZI032_PYC',''y''], // 'Y'
     ['ZI032_PYC','5','raw:ZI032_PYC',''t''], // 'T'
     ['ZI032_PYC','999','raw:ZI032_PYC','other'], // Other

     // ZI032_PYM - Pylon Information : Pylon Material
     // ['ZI032_PYM','-999999',undefined,undefined], // noInformation
     ['ZI032_PYM','1','raw:ZI032_PYM','aluminum'], // Aluminum
     ['ZI032_PYM','3','raw:ZI032_PYM','concrete'], // Concrete
     ['ZI032_PYM','4','raw:ZI032_PYM','masonry'], // Masonry
     ['ZI032_PYM','5','raw:ZI032_PYM','metal'], // Metal
     ['ZI032_PYM','7','raw:ZI032_PYM','wood'], // Wood
     ['ZI032_PYM','8','raw:ZI032_PYM','steel'], // Steel
     ['ZI032_PYM','9','raw:ZI032_PYM','fibreglass'], // Fibreglass
     ['ZI032_PYM','10','raw:ZI032_PYM','iron'], // Iron
     ['ZI032_PYM','999','raw:ZI032_PYM','other'], // Other

     // ZI032_TOS - Pylon Information : Tower Shape
     // ['ZI032_TOS','-999999',undefined,undefined], // noInformation
     ['ZI032_TOS','6','raw:ZI032_TOS','mast'], // Mast
     ['ZI032_TOS','8','raw:ZI032_TOS','pole'], // Pole
     ['ZI032_TOS','11','raw:ZI032_TOS','tripod'], // Tripod
     ['ZI032_TOS','12','raw:ZI032_TOS','truss'], // Truss
     ['ZI032_TOS','13','raw:ZI032_TOS','tubular'], // Tubular
     ['ZI032_TOS','999','raw:ZI032_TOS','other'], // Other

     // ZI037_REL - Religious Information : Religious Designation
     // ['ZI037_REL','-999999',undefined,undefined], // noInformation
     ['ZI037_REL','1','raw:ZI037_REL','buddhism'], // Buddhism
     ['ZI037_REL','2','raw:ZI037_REL','islam'], // Islam
     ['ZI037_REL','3','raw:ZI037_REL','roman_catholic'], // Roman Catholic
     ['ZI037_REL','4','raw:ZI037_REL','christian'], // Christian
     ['ZI037_REL','5','raw:ZI037_REL','judaism'], // Judaism
     ['ZI037_REL','6','raw:ZI037_REL','orthodox'], // Orthodox
     ['ZI037_REL','7','raw:ZI037_REL','protestant'], // Protestant
     ['ZI037_REL','8','raw:ZI037_REL','shinto'], // Shinto
     ['ZI037_REL','9','raw:ZI037_REL','hinduism'], // Hinduism
     ['ZI037_REL','10','raw:ZI037_REL','shia'], // Shia
     ['ZI037_REL','11','raw:ZI037_REL','sunni'], // Sunni
     ['ZI037_REL','12','raw:ZI037_REL','nestorian'], // Nestorian
     ['ZI037_REL','13','raw:ZI037_REL','chaldean'], // Chaldean
     ['ZI037_REL','14','raw:ZI037_REL','mixed_and/or_no_designation'], // Mixed and/or No Designation
     ['ZI037_REL','999','raw:ZI037_REL','other'], // Other

     // ZI037_RFA - Religious Information : Religious Facility Type
     // ['ZI037_RFA','-999999',undefined,undefined], // noInformation
     ['ZI037_RFA','1','raw:ZI037_RFA','cathedral'], // Cathedral
     ['ZI037_RFA','2','raw:ZI037_RFA','chapel'], // Chapel
     ['ZI037_RFA','3','raw:ZI037_RFA','church'], // Church
     ['ZI037_RFA','4','raw:ZI037_RFA','marabout'], // Marabout
     ['ZI037_RFA','5','raw:ZI037_RFA','minaret'], // Minaret
     ['ZI037_RFA','6','raw:ZI037_RFA','mission'], // Mission
     ['ZI037_RFA','7','raw:ZI037_RFA','mosque'], // Mosque
     ['ZI037_RFA','8','raw:ZI037_RFA','pagoda'], // Pagoda
     ['ZI037_RFA','9','raw:ZI037_RFA','religious_community'], // Religious Community
     ['ZI037_RFA','10','raw:ZI037_RFA','seminary'], // Seminary
     ['ZI037_RFA','11','raw:ZI037_RFA','shrine'], // Shrine
     ['ZI037_RFA','12','raw:ZI037_RFA','stupa'], // Stupa
     ['ZI037_RFA','13','raw:ZI037_RFA','synagogue'], // Synagogue
     ['ZI037_RFA','14','raw:ZI037_RFA','tabernacle'], // Tabernacle
     ['ZI037_RFA','15','raw:ZI037_RFA','temple'], // Temple
     ['ZI037_RFA','18','raw:ZI037_RFA','convent'], // Convent
     ['ZI037_RFA','19','raw:ZI037_RFA','monastery'], // Monastery
     ['ZI037_RFA','20','raw:ZI037_RFA','noviciate'], // Noviciate
     ['ZI037_RFA','21','raw:ZI037_RFA','hermitage'], // Hermitage
     ['ZI037_RFA','22','raw:ZI037_RFA','retreat'], // Retreat
     ['ZI037_RFA','999','raw:ZI037_RFA','other'], // Other

     // ZI071_FFN - Cave Information : Feature Function
     // ['ZI071_FFN','-999999',undefined,undefined], // noInformation
     ['ZI071_FFN','530','raw:ZI071_FFN','warehousing_and_storage'], // Warehousing and Storage
     ['ZI071_FFN','563','raw:ZI071_FFN','residence'], // Residence
     ['ZI071_FFN','835','raw:ZI071_FFN','defence_activities'], // Defence Activities
     ['ZI071_FFN','930','raw:ZI071_FFN','religious_activities'], // Religious Activities
     ['ZI071_FFN','999','raw:ZI071_FFN','other'], // Other

     // ZI071_FFN2 - Cave Information : Feature Function [2]
     // ['ZI071_FFN2','-999999',undefined,undefined], // noInformation
     ['ZI071_FFN2','530','raw:ZI071_FFN2','warehousing_and_storage'], // Warehousing and Storage
     ['ZI071_FFN2','563','raw:ZI071_FFN2','residence'], // Residence
     ['ZI071_FFN2','835','raw:ZI071_FFN2','defence_activities'], // Defence Activities
     ['ZI071_FFN2','930','raw:ZI071_FFN2','religious_activities'], // Religious Activities
     ['ZI071_FFN2','999','raw:ZI071_FFN2','other'], // Other

     // ZI071_FFN3 - Cave Information : Feature Function [3]
     // ['ZI071_FFN3','-999999',undefined,undefined], // noInformation
     ['ZI071_FFN3','530','raw:ZI071_FFN3','warehousing_and_storage'], // Warehousing and Storage
     ['ZI071_FFN3','563','raw:ZI071_FFN3','residence'], // Residence
     ['ZI071_FFN3','835','raw:ZI071_FFN3','defence_activities'], // Defence Activities
     ['ZI071_FFN3','930','raw:ZI071_FFN3','religious_activities'], // Religious Activities
     ['ZI071_FFN3','999','raw:ZI071_FFN3','other'], // Other

     // ZI071_UAO - Cave Information : Underground Access Orientation
     // ['ZI071_UAO','-999999',undefined,undefined], // noInformation
     ['ZI071_UAO','1','raw:ZI071_UAO','horizontal'], // Horizontal
     ['ZI071_UAO','2','raw:ZI071_UAO','slopes_downward'], // Slopes Downward
     ['ZI071_UAO','3','raw:ZI071_UAO','vertical_down'], // Vertical Down
     ['ZI071_UAO','4','raw:ZI071_UAO','slopes_upward'], // Slopes Upward
     ['ZI071_UAO','5','raw:ZI071_UAO','vertical_up'], // Vertical Up

     // ZSAX_RS0 - Restriction Information : Security Attributes Group <resource classification>
     ['ZSAX_RS0','C','raw:ZSAX_RS0','c'], // C
     ['ZSAX_RS0','R','raw:ZSAX_RS0','r'], // R
     ['ZSAX_RS0','S','raw:ZSAX_RS0','s'], // S
     ['ZSAX_RS0','TS','raw:ZSAX_RS0','ts'], // TS
     ['ZSAX_RS0','U','raw:ZSAX_RS0','u'], // U

    ], // End one2one

    // Input Translation Rules:
    // These are only used for IMPORT and get added to the standard one2one rules
    // This is mainly for TDS 3.0 attributes but we have added some "funky" TDSv40 attributes as well.
    one2oneIn : [
    // To account for an "error" in the TDSv61 sample GDB
    ['ZI001_VSC','Stereoscopic Imagery','source:vertical_source:type','stereoscopic_imagery'],
    ['ZI001_SRT','NTM Imagery','source:non_spatial_source:type','ntm_imagery'],

    // ['ZI016_ROC','-999999',undefined,undefined], // No Information
    ['ZI016_ROC','1','surface','ground'], // Unimproved
    ['ZI016_ROC','2','surface','compacted'], // Stabilized earth
    ['ZI016_ROC','3','surface','unpaved'], // Flexible Pavement
    ['ZI016_ROC','4','surface','gravel'], // Aggregate
    ['ZI016_ROC','5','surface','macadam'], // Macadam
    ['ZI016_ROC','6','surface','bound_surface'], // Bound Surface
    ['ZI016_ROC','7','surface','paved'], // Rigid Pavement
    ['ZI016_ROC','8','surface','concrete'], // Concrete
    ['ZI016_ROC','9','surface','asphalt'], // Asphalt
    ['ZI016_ROC','10','surface','asphalt_over_concrete'], // Asphalt over Concrete
    ['ZI016_ROC','11','surface','cobblestone'], // Cobble-stone
    ['ZI016_ROC','12','surface','brick'], // Brick
    ['ZI016_ROC','13','surface','metal'], // Metal
    ['ZI016_ROC','14','surface','wood'], // Wood
    ['ZI016_ROC','15','surface','corduroy'], // rough hewn logs...  // Corduroy
    ['ZI016_ROC','16','surface','wood_plank'], // Wood Plank
    ['ZI016_ROC','17','surface','ice'], // Ice
    ['ZI016_ROC','18','surface','snow'], // Snow
    ['ZI016_ROC','999','surface','other'], // Other

    ], // End one2oneIn

    // One2one translation table for converting "Other" OSM attributes to TDS
    // This is for Export only. The values are swapped before use
    one2oneOut : [
    ['ZSAX_RS0','U','security:classification','U'],
    ['ZSAX_RS0','R','security:classification','R'],
    ['ZSAX_RS0','C','security:classification','C'],
    ['ZSAX_RS0','S','security:classification','S'],
    ['ZSAX_RS0','TS','security:classification','TS'],

    // OTH Filler.  These are to build OTH values
    ['RTY','999','highway','other'],
    ['FFN','999','amenity','other'], 
    ['FFN','999','building','other'], 
    ['FFN','999','camp:type','other'], 
    ['FFN','999','craft','other'], 
    ['FFN','999','landuse','other'], 
    ['FFN','999','leisure','other'], 
    ['FFN','999','military','other'], 
    ['FFN','999','office','other'],
    ['FFN','999','repair','other'],
    ['FFN','999','shop','other'],
    ['FFN','999','social_facility','other'],
    ['FFN','999','tourism','other'],

    // Funky Road Type attributes from TDSv40
    ['RTY','4','ref:road:type','boulevard'], // Boulevard
    ['RTY','4','ref:road:type','avenue'], // Avenue
    ['RTY','4','ref:road:type','drive'], // Drive
    ['RTY','4','ref:road:type','circle'], // Circle
    ['RTY','4','ref:road:type','close'], // Close
    ['RTY','4','ref:road:type','parkway'], // Parkway
    ['RTY','4','ref:road:type','lane'], // Lane
    ['RTY','4','ref:road:type','terrace'], // Terrace
    ['RTY','4','ref:road:type','place'], // Place
    ['RTY','4','ref:roadtype','roundabout'], // Roundabout

    // Fix up the building/use tags from FFN
    [undefined,undefined,'building','industrial'], // Industrial
    [undefined,undefined,'man_made','works'], // Works
    ['FFN','2','building','farm'], // Agriculture

    ['FFN','827','amenity','embassy'], // Embassy

    ['FFN','850','building','school'],
    ['FFN','850','isced:level','0'], // Education
    ['FFN','850','amenity','kindergarten'], // Education
    // ['FFN','851','isced:level','1'], // Primary Education
    ['FFN','852','isced:level','2'], // Secondary Education
    ['FFN','852','isced:level','3'], // Secondary Education
    // ['FFN','852','isced:level','2,3'], // Secondary Education
    ['FFN','857','building','college'], // Vocational Education

    ['FFN','572','amenity','cafe'], // Restaurant
    ['FFN','572','building','restaurant'], // Restaurant

    // Backward compatibility for 'oil'
    ['ZI014_PPO','83','product','oil'], // Petroleum
    ['ZI014_PPO','83','product','fuel'], // Close...

    // Extra road stuff - see similarTable
    // ['ZI016_ROC','9','surface','paved'], // Paved/Asphalt from OSM
    // ['ZI016_ROC','3','surface','unpaved'], // Stabilized earth
    // ['ZI016_ROC','1','surface','dirt'], // Unimproved
    // ['ZI016_ROC','1','surface','earth'], // Unimproved

    // Fix up some of the Defaults
    // ['FFN','-999999','building','yes'], // Map to No Information
    // ['RRC','-999999','railway','rail'], // Map to No Information
    [undefined,undefined,'natural','water'], // to get rid of errors

    // From MGCP
    ['FFN','480','building','transportation_outbuilding'], // Transportation
    ['FFN','481','man_made','depot'], // not a good fit for depot terminal
    ['FFN','550','building','dwelling'], // Accomodation
    ['FFN','811','building','public_service_outbuilding'], // Government - Not Good
    ['FFN','811','amenity','government_administration_building'], //
    ['FFN','835','military','military_outbuilding'], // Military
    ['FFN','835','military','barracks'], // Military
    ['FFN','843','amenity','jail'], // Imprisonment
    ['FFN','980','building','cemetery_building'], // Death care services
    ['ZI014_PPO','-999999','product','unknown'], // Unknown
//     [undefined,undefined,'amenity','school'], //  converted in pre processing
//     [undefined,undefined,'amenity','hospital'], //  converted in pre processing
//     [undefined,undefined,'amenity','university'], //  converted in pre processing
//     [undefined,undefined,'amenity','college'], //  converted in pre processing
    [undefined,undefined,'amenity','house_of_worship'], //  converted in pre processing

    ['IWT','4','water','reservoir'], // Reservoir

    // From UFD
    ['CAA','-999999','controlling_authority','unknown'],
    ['FFN','190','use','oil_gas_facility'], //

    // From UTP
    // ['TTC','999','tower:type','radar'], //  Radar Tower -> Other
    ['FFN','482','amenity','bus_station'], // Station
    ['FFN','831','amenity','courthouse'], // Public Order - from spec
    ['FFN','610','building','communication'],
    ['FFN','530','building','warehouse'],
    ['FFN','835','building','military'],
    // ['FFN','999','landuse','institutional'], //


    // From NAVTEQ
    ['FFN','122','man_made','winery'], // Winery
    ['FFN','440','office','company'], // Commerce
    ['FFN','440','office','yes'], // Commerce
    ['FFN','464','shop','car'], // Specialized Store
    ['FFN','464','shop','motorcycle'], // Specialized Store
    ['FFN','465','shop','general'], // Non-specialized Store
    ['FFN','480','amenity','transportation'], // Transport
    ['FFN','481','aeroway','terminal'], // Terminal
    ['FFN','552','leisure','ski_resort'], // Resort Hotel
    ['FFN','572','amenity','fast_food'], // Restaurant
    ['FFN','775','tourism','information'], // Travel Agency
    ['FFN','850','amenity','education'], // Education
    ['FFN','860','building','hospital'], // Human Health Activities
    ['FFN','860','amenity','clinic'], // Human Health Activities
    ['FFN','860','amenity','medical'], // Human Health Activities
    ['FFN','890','amenity','cultural'], // Cultural, Arts and Entertainment
    ['FFN','900','tourism','yes'], // Sports, Amusement and Recreation
    // ['FFN','922','tourism','theme_park'], // Amusement
    ['FFN','923','sport','10pin'], // Hobbies and/or Leisure Activities
    ['FFN','909','amenity','casino'], // Gambling
    ['FFN','912','leisure','sports_complex'], // Sports Centre
    ['FFN','970','amenity','townhall'], // Meeting Place
    [undefined,undefined,'historic','yes'], // Not a good translation, push to o2s

    // ### From OSM - This list could never end.....
    ['FFN','464','shop','books'], // Specialized Store
    ['FFN','563','building','house'], // Residence
    ['FFN','558','building','dependents_housing'], // Dependents Housing
    ['FFN','610','office','telecommunication'], // telecommunication
    ['FFN','640','shop','bureau_de_change'], // Financial Services
    ['FFN','760','amenity','office'], // Business and Personal Support Services 
    ['FFN','815','building','palace'], // Palace

    // Added to give something for the OTH value to use
    ['ZI016_ROC','999','surface','other'], // Other

   ], // End one2oneOut

    // ##### End of One2One Rules #####

   
    // ##### Start of fuzzyTable #####
    // This list uses the new IsA, IsSimilar etc functions to map a number of input values to a single output
    fuzzyTable : [
    ['ZI016_ROC','1',schemaTools.simple('surface=ground',1)], // Unimproved
    ['ZI016_ROC','2',schemaTools.simple('surface=compacted',1)], // Stabilized earth
    ['ZI016_ROC','3',schemaTools.isA('surface=unpaved', 0.8)], // Flexible Pavement
    ['ZI016_ROC','4',schemaTools.simple('surface=gravel',1)], // Aggregate
    ['ZI016_ROC','5',schemaTools.simple('surface=macadam',1)], // Macadam
    ['ZI016_ROC','6',schemaTools.simple('surface=bound_surface',1)], // Bound Surface
    ['ZI016_ROC','7',schemaTools.isA('surface=paved', 0.8)], // Ridgid Pavement
    ['ZI016_ROC','8',schemaTools.simple('surface=concrete',1)], // Concrete
    ['ZI016_ROC','9',schemaTools.simple('surface=asphalt',1)], // Asphalt
    ['ZI016_ROC','10',schemaTools.simple('surface=asphalt_over_concrete',1)], // Asphalt over Concrete
    ['ZI016_ROC','11',"surface=cobblestone",schemaTools.isA('surface=cobblestone',1),schemaTools.simple('surface=sett')], // Cobble-stone
    ['ZI016_ROC','12',schemaTools.simple('surface=brick',1)], // Brick
    ['ZI016_ROC','13',schemaTools.simple('surface=metal',1)], // Metal
    ['ZI016_ROC','14',schemaTools.simple('surface=wood',1)], // Wood
    ['ZI016_ROC','15',schemaTools.simple('surface=corduroy',1)], // rough hewn logs...  // Corduroy
    ['ZI016_ROC','16',schemaTools.simple('surface=wood_plank',1)], // Wood Plank
    ['ZI016_ROC','17',schemaTools.simple('surface=ice',1)], // Ice
    ['ZI016_ROC','18',schemaTools.simple('surface=snow',1)], // Snow
    ['ZI016_ROC','999',schemaTools.simple('surface=other',1)], // Other
    ],
    // ##### End of fuzzyTable #####

    // ##### Start of ignoreList #####
    // This is taken from OSM pre processing and a few added
    ignoreList : [



    'APT2', 'APT3',
    'APU2', 'APU3',
    'AQO2', 'AQO3',
    'AQP2', 'AQP3',
    'ASU', 'ASU2', 'ASU3',
    'AT005_CAB', 'AT005_CAB2', 'AT005_CAB3',
    'BMC2', 'BMC3',
    'BSC2', 'BSC3',
    'CAB2', 'CAB3',
    'CSC2', 'CSC3',
    'CVT2', 'CVT3',
    'FCSUBTYPE','FCSubtype',
    'FFN2', 'FFN3',
    'FHC2', 'FHC3',
    'FRT2', 'FRT3',
    'F_CODE',
    'HST2', 'HST3',
    'HYP',
    'MCC2', 'MCC3',
    'MEM',
    'MST2', 'MST3',
    'PBY', 'PBY2', 'PBY3',
    'PFD',
    'PLT2', 'PLT3',
    'POS2', 'POS3',
    'PPO', 'PPO2', 'PPO3',
    'PRW', 'PRW2', 'PRW3',
    'RCG',
    'RIN_ROI2', 'RIN_ROI3',
    'RIN_RTN2', 'RIN_RTN3',
    'RRC2', 'RRC3',
    'RTN', 'RTN2', 'RTN3',
    'SBT2', 'SBT3',
    'SSR2', 'SSR3',
    'STL2', 'STL3',
    'SUR',
    'TRS2', 'TRS3',
    'TSM2', 'TSM3',
    'TTC2', 'TTC3',
    'UTY2', 'UTY3',
    'VCA2', 'VCA3',
    'VCM2', 'VCM3',
    'VCT2', 'VCT3',
    'VSP2', 'VSP3',
    'WBD',
    'WD1',
    'WEQ2', 'WEQ3',
    'YWQ',
    'ZI005_FNA2', 'ZI005_FNA3',
    'ZI005_NFN2', 'ZI005_NFN3',
    'ZI013_CSP2', 'ZI013_CSP3',
    'ZI013_FFP2', 'ZI013_FFP3',
    'ZI013_FMM2', 'ZI013_FMM3',
    'ZI014_PBY2', 'ZI014_PBY3',
    'ZI014_PPO2', 'ZI014_PPO3',
    'ZI014_PRW2', 'ZI014_PRW3',
    'ZI019_ASP2', 'ZI019_ASP3',
    'ZI019_ASU2', 'ZI019_ASU3',
    'ZI025_MAN',
    'ZI025_WLE',
    'ZI032_GUG',
    'ZI032_PYC',
    'ZI032_PYM',
    'ZI032_TOS',
    'ZI071_FFN', 'ZI071_FFN2', 'ZI071_FFN3',
    'ZSAX_RX3', 'ZSAX_RX4',
    'ZVH_VDT',
    ],
    // ##### End of ignoreList #####

    // ##### Start of fCodeMap #####
    // This is a map of FCODE's and filenames
    fCodeMap : [
    ['AF010', ['af010','smokestack_p']], // Smokestack
    ['AH025', ['ah025','engineered_earthwork_s','engineered_earthwork_p']], // Engineered Earthwork
    ['AH060', ['ah060','underground_bunker_s','underground_bunker_p']], // Underground Bunker
    ['AL010', ['al010','facility_s','facility_p']], // Facility
    ['AL013', ['al013','building_s','building_p']], // Building
    ['AL018', ['al018','building_superstructure_s','building_superstructure_c','building_superstructure_p']], // Building Superstructure
    ['AL020', ['al020','built-up_area_s','built-up_area_p']], // Built up area
    ['AL030', ['al030','cemetery_s','cemetery_p']], // Cemetary
    ['AL070', ['al070','fence_c']], // Fence
    ['AL099', ['al099','hut_p']], // Hut
    ['AL105', ['al105','settlement_s','settlement_p']], // Settlement
    ['AL130', ['al130','memorial_monument_s','memorial_monument_p']], // Memorial Monument
    ['AL200', ['al200','ruins_s','ruins_p']], // Ruins
    ['AL208', ['al208','shanty_town_s','shanty_town_p']], // Shanty Town
    ['AL241', ['al241','tower_s','tower_p']], // Tower
    ['AL260', ['al260','wall_c']], // Wall
    ['AM080', ['am080','water_tower_p','water_tower_s']], // Water Tower
    ['AN010', ['an010','railway_c']], // Railway
    ['AN050', ['an050','railway_sidetrack_c']], // Railway Sidetrack
    ['AN060', ['an060','railway_yard_s']], // Railway Yard
    ['AN075', ['an075','railway_turntable_p','railway_turntable_p']], // Railway Turntable
    ['AN076', ['an076','roundhouse_s','roundhouse_p']], // Roundhouse
    ['AP010', ['ap010','cart_track_c']], // Cart Track
    ['AP020', ['ap020','road_interchange_p']], // Interchange
    ['AP030', ['ap030','road_c']], // Road
    ['AP040', ['ap040','gate_c','gate_p']], // Gate
    ['AP041', ['ap041','vehicle_barrier_c','vehicle_barrier_p']], // Vehicle Barrier
    ['AP050', ['ap050','trail_c']], // Trail
    ['AQ040', ['aq040','bridge_c','bridge_p']], // Bridge
    ['AQ045', ['aq045','bridge_span_c','bridge_span_p']], // Bridge Span
    ['AQ065', ['aq065','culvert_c','culvert_p']], // Culvert
    ['AQ070', ['aq070','ferry_crossing_c']], // Ferry Crossing
    ['AQ095', ['aq095','tunnel_mouth_p']], // Tunnel Mouth
    ['AQ113', ['aq113','pipeline_c']], // Pipeline
    ['AQ125', ['aq125','transportation_station_s','transportation_station_p']], // Transportation Station
    ['AQ130', ['aq130','tunnel_c']], // Tunnel
    ['AQ140', ['aq140','vehicle_lot_s']], // Vehicle Lot
    ['AQ141', ['aq141','parking_garage_s','parking_garage_p']], // Parking Garage
    ['AQ170', ['aq170','motor_vehicle_station_s','motor_vehicle_station_p']], // Motor Vehicle Station
    ['AT010', ['at010','dish_aerial_p']], // Dish Aerial
    ['AT042', ['at042','pylon_p']], // Pylon
    ['BH010', ['bh010','aqueduct_s','aqueduct_c']], // Aqueduct
    ['BH020', ['bh020','canal_s','canal_c']], // Canal
    ['BH030', ['bh030','ditch_s','ditch_c']], // Ditch
    ['BH070', ['bh070','ford_c','ford_p']], // Ford
    ['BH082', ['bh082','inland_waterbody_s','inland_waterbody_p']], // Inland Waterbody
    ['BH140', ['bh140', 'river_s','river_c']], // River
    ['BH170', ['bh170','natural_pool_p']], // Natural Pool
    ['BH230', ['bh230', 'water_well_p','water_well_s']], // Water Well
    ['BI010', ['bi010', 'cistern_p']], // Cistern
    ['DB070', ['db070','cut_c']], // Cut
    ['DB150', ['db150','mountain_pass_p']], // Mountain Pass
    ['GB050', ['gb050','aircraft_revetment_c']], // Aircraft Revetment
    ['ZD040', ['zd040','named_location_s','named_location_c','named_location_p']], // Named Location
    ['ZD045', ['zd045','annotated_location_s','annotated_location_c','annotated_location_p']], // Named Location
    ],
    // ##### End of fCodeMap #####


    // ##### Start of closureList #####
    closureList : {


    'AQTC':['AQTL','AQTU'],
    'AYRC':['AYRL','AYRU'],
    'BPWHAC':['BPWHAL','BPWHAU'],
    'BPWHBC':['BPWHBL','BPWHBU'],
    'BPWSAC':['BPWSAL','BPWSAU'],
    'BPWSBC':['BPWSBL','BPWSBU'],
    'BWVCAC':['BWVCAL','BWVCAU'],
    'BWVCBC':['BWVCBL','BWVCBU'],
    'DMBC':['DMBL','DMBU'],
    'DPAC':['DPAL','DPAU'],
    'GSGCHC':['GSGCHL','GSGCHU'],
    'GSGCLC':['GSGCLL','GSGCLU'],
    'PWAC':['PWAL','PWAU'],
    'RMWC':['RMWL','RMWU'],
    'SDCC':['SDCL','SDCU'],
    'SDSC':['SDSL','SDSU'],
    'SGCC':['SGCL','SGCU'],
    'TSCC':['TSCL','TSCU'],
    'WDAC':['WDAL','WDAU'],
    'ZI026_CTUC':['ZI026_CTUL','ZI026_CTUU']
    },
    // ##### End of closureList #####

    // ##### Start of swapListIn #####
    // The What Were They Thinking? swap list.  Each of these is the _same_ attribute
    // but renamed in different features. We swap these so that there is only one
    // set of rules needed in the One2One section.
    // These get converted back on output - if we need to.
    swapListIn : {


    'ASU':'ZI019_ASU', 'ASU2':'ZI019_ASU3', 'ASU3':'ZI019_ASU3',
    'AT005_CAB':'CAB', 'AT005_CAB2':'CAB2', 'AT005_CAB3':'CAB3',
    'HYP':'ZI024_HYP',
    // 'LEN_':'LZN',
    'MEM':'ZI006_MEM',
    'PFD':'PWA',
    'PBY':'ZI014_PBY', 'PBY2':'ZI014_PBY2', 'PBY3':'ZI014_PBY3',
    'PPO':'ZI014_PPO', 'PPO2':'ZI014_PPO2', 'PPO3':'ZI014_PPO3',
    'PRW':'ZI014_PRW', 'PRW2':'ZI014_PRW2', 'PRW3':'ZI014_PRW3',
    'RCG':'ZI004_RCG',
    'RTN':'RIN_RTN', 'RTN2':'RIN_RTN2', 'RTN3':'RIN_RTN3',
    'SUR':'ZI026_SUR',
    'WBD':'PWA',
    'WD1':'ZI016_WD1',
    'YWQ':'ZI024_YWQ',
    'ZI025_MAN':'MAN',
    'ZI025_WLE':'WLE',
    'ZI032_GUG':'GUG',
    'ZI032_TOS':'TOS',
    'ZI032_PYC':'PYC',
    'ZI032_PYM':'PYM',
    'ZI071_FFN':'FFN', 'ZI071_FFN2':'FFN2', 'ZI071_FFN3':'FFN3',
    'ZVH_VDT':'VDT'
    },
    // ##### End of swapListIn #####

    // ##### Start of swapListOut #####
    // Format is: <FCODE>:{<from>:<to>}
    swapListOut : {
    'AA010':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AA020':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AA040':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AA052':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AA054':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AB000':{'ZI014_PBY':'PBY', 'ZI014_PBY2':'PBY2', 'ZI014_PBY3':'PBY3'},
    'AC060':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AD020':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AD025':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AJ050':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AL020':{'ZI005_NFN':'ZI005_NFN1'},
    'AM010':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AM040':{'ZI014_PRW':'PRW', 'ZI014_PRW2':'PRW2', 'ZI014_PRW3':'PRW3'},
    'AM060':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AM070':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AM071':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AM080':{'ZI014_YWQ':'YWQ'},
    'AQ059':{'ZI016_WD1':'WD1'},
    'AQ113':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AQ116':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AT005':{'WLE':'ZI025_WLE'},
    'AT042':{'GUG':'ZI032_GUG', 'PYC':'ZI032_PYC', 'PYM':'ZI032_PYM', 'TOS':'ZI032_TOS', 'CAB':'AT005_CAB','CAB2':'AT005_CAB2','CAB3':'AT005_CAB3'},
    'BD100':{'WLE':'ZI025_WLE'},
    'BH051':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'BH070':{'PWA':'WBD'},
    'DB029':{'FFN':'ZI071_FFN', 'FFN2':'ZI071_FFN2', 'FFN3':'ZI071_FFN3'},
    'ED010':{'ZI024_HYP':'HYP'},
    'GB045':{'ZI019_ASU':'ASU', 'ZI019_ASU2':'ASU2', 'ZI019_ASU3':'ASU3'},
    'BD115':{'MAN':'ZI025_MAN'},
    'AP055':{'RIN_RTN':'RTN', 'RIN_RTN2':'RTN2', 'RIN_RTN3':'RTN3'},
    'ZI031':{'ZI006_MEM':'MEM', 'ZI004_RCG':'RCG'},
    'ZI026':{'ZI026_SUR':'SUR'}
    },

    // ##### End of swapListOut #####

    // ##### Start of txtLength #####
    // This list is for validateing the lengths of text attributes prior to export
    txtLength : {
    'BA000_VDR':80, 'BEN':15, 'BRN':24, 'CID':20, 'CUD':20, 'ETZ':24, 'F_CODE':5, 'GB052_RIDH':14, 'GB052_RIDL':14,
    'IKO':14, 'MDE':20, 'NA8':80, 'PDA':14, 'PSE':14, 'RIN_RTN':24, 'RIN_RTN2':24, 'RIN_RTN3':24, 'RTN':24, 'RTN2':24,
    'RTN3':24, 'SAX_RS3':14, 'SAX_RS4':14, 'SAX_RS6':14, 'SAX_RX1':254, 'SAX_RX2':254, 'SAX_RX5':254, 'SAX_RX6':254,
    'SAX_RX7':254, 'SAX_RX8':14, 'SAX_RY1':254, 'SAX_RY2':254, 'SSE':14, 'UFI':254, 'URI':254, 'VCS_VCR':132, 'VOI':14,
    'WPI':14, 'ZI001_SDV':20, 'ZI001_VSD':20, 'ZI002_UFI':254, 'ZI005_FNA':200, 'ZI005_FNA2':200, 'ZI005_FNA3':200,
    'ZI005_NFN':18, 'ZI005_NFN2':18, 'ZI005_NFN3':18, 'ZI026T_UFI':254, 'ZI027T_UFI':254, 'ZI028_UFI':254, 'ZI031S_URI':254,
    'ZI031T_URI':254, 'ZI039S_UFI':254, 'ZI039T_UFI':254, 'ZSAX_RS0':14,
    'ZSAX_RX0':254, 'ZSAX_RX3':254, 'ZSAX_RX4':254
    },
    // ##### End of txtLength #####

    // ##### Start of intList #####
    // This list is for validateing the integer attributes prior to export
    intList : [
    'BC040_LCN', 'BC040_LVN', 'BNF', 'DEV', 'DZC', 'LC1', 'LC2', 'LC3', 'LC4', 'LTN', 'NOS', 'NPL', 'ZI026_CTUL',
    'ZI026_CTUU'
    ],
    // ##### End of intList #####

    // ##### Start of ge4List #####
    // This list is for validateing the XXX_GE4XX values
    ge4List : {
    'ge:GENC:3:1-2:ABW':'AW', // Aruba
    'ge:GENC:3:1-2:AFG':'AF', // Islamic Republic of Afghanistan
    'ge:GENC:3:1-2:AGO':'AO', // Republic of Angola
    'ge:GENC:3:1-2:ALB':'AL', // Republic of Albania
    'ge:GENC:3:1-2:AND':'AD', // Principality of Andorra
    'ge:GENC:3:1-2:ARE':'AE', // United Arab Emirates
    'ge:GENC:3:1-2:ARG':'AR', // Argentine Republic
    'ge:GENC:3:1-2:ARM':'AM', // Republic of Armenia
    'ge:GENC:3:1-2:ASM':'AS', // Territory of American Samoa
    'ge:GENC:3:1-2:ATF':'TF', // French Southern and Antarctic Lands
    'ge:GENC:3:1-2:ATG':'AG', // Antigua and Barbuda
    'ge:GENC:3:1-2:AUS':'AU', // Commonwealth of Australia
    'ge:GENC:3:1-2:AUT':'AT', // Republic of Austria
    'ge:GENC:3:1-2:AX1':'A1', // Unknown
    'ge:GENC:3:1-2:AX2':'A2', // Guantanamo Bay Naval Base
    'ge:GENC:3:1-2:AZE':'AZ', // Republic of Azerbaijan
    'ge:GENC:3:1-2:BDI':'BI', // Republic of Burundi
    'ge:GENC:3:1-2:BEL':'BE', // Kingdom of Belgium
    'ge:GENC:3:1-2:BEN':'BJ', // Republic of Benin
    'ge:GENC:3:1-2:BES':'BQ', // Bonaire, Sint Eustatius, and Saba
    'ge:GENC:3:1-2:BFA':'BF', // Burkina Faso
    'ge:GENC:3:1-2:BGD':'BD', // Peoples Republic of Bangladesh
    'ge:GENC:3:1-2:BGR':'BG', // Republic of Bulgaria
    'ge:GENC:3:1-2:BHR':'BH', // Kingdom of Bahrain
    'ge:GENC:3:1-2:BHS':'BS', // Commonwealth of The Bahamas
    'ge:GENC:3:1-2:BLM':'BL', // Saint Barthelemy
    'ge:GENC:3:1-2:BLR':'BY', // Republic of Belarus
    'ge:GENC:3:1-2:BOL':'BO', // Plurinational State of Bolivia
    'ge:GENC:3:1-2:BRA':'BR', // Federative Republic of Brazil
    'ge:GENC:3:1-2:BRN':'BN', // Brunei Darussalam
    'ge:GENC:3:1-2:BTN':'BT', // Kingdom of Bhutan
    'ge:GENC:3:1-2:BWA':'BW', // Republic of Botswana
    'ge:GENC:3:1-2:CAF':'CF', // Central African Republic
    'ge:GENC:3:1-2:CAN':'CA', // Canada
    'ge:GENC:3:1-2:CCK':'CC', // Territory of Cocos (Keeling) Islands
    'ge:GENC:3:1-2:CHE':'CH', // Swiss Confederation
    'ge:GENC:3:1-2:CHL':'CL', // Republic of Chile
    'ge:GENC:3:1-2:CHN':'CN', // Peoples Republic of China
    'ge:GENC:3:1-2:CIV':'CI', // Republic of Côte dIvoire
    'ge:GENC:3:1-2:CMR':'CM', // Republic of Cameroon
    'ge:GENC:3:1-2:COD':'CD', // Democratic Republic of the Congo
    'ge:GENC:3:1-2:COG':'CG', // Republic of the Congo
    'ge:GENC:3:1-2:COK':'CK', // Cook Islands
    'ge:GENC:3:1-2:COL':'CO', // Republic of Colombia
    'ge:GENC:3:1-2:COM':'KM', // Union of the Comoros
    'ge:GENC:3:1-2:CPT':'CP', // Clipperton Island
    'ge:GENC:3:1-2:CPV':'CV', // Republic of Cape Verde
    'ge:GENC:3:1-2:CRI':'CR', // Republic of Costa Rica
    'ge:GENC:3:1-2:CUB':'CU', // Republic of Cuba
    'ge:GENC:3:1-2:CUW':'CW', // Curaçao
    'ge:GENC:3:1-2:CXR':'CX', // Territory of Christmas Island
    'ge:GENC:3:1-2:CYM':'KY', // Cayman Islands
    'ge:GENC:3:1-2:CYP':'CY', // Republic of Cyprus
    'ge:GENC:3:1-2:CZE':'CZ', // Czech Republic
    'ge:GENC:3:1-2:DEU':'DE', // Federal Republic of Germany
    'ge:GENC:3:1-2:DGA':'DG', // Diego Garcia
    'ge:GENC:3:1-2:DJI':'DJ', // Republic of Djibouti
    'ge:GENC:3:1-2:DMA':'DM', // Commonwealth of Dominica
    'ge:GENC:3:1-2:DNK':'DK', // Kingdom of Denmark
    'ge:GENC:3:1-2:DOM':'DO', // Dominican Republic
    'ge:GENC:3:1-2:DZA':'DZ', // Peoples Democratic Republic of Algeria
    'ge:GENC:3:1-2:ECU':'EC', // Republic of Ecuador
    'ge:GENC:3:1-2:EGY':'EG', // Arab Republic of Egypt
    'ge:GENC:3:1-2:ERI':'ER', // State of Eritrea
    'ge:GENC:3:1-2:ESH':'EH', // Western Sahara
    'ge:GENC:3:1-2:ESP':'ES', // Kingdom of Spain
    'ge:GENC:3:1-2:EST':'EE', // Republic of Estonia
    'ge:GENC:3:1-2:ETH':'ET', // Federal Democratic Republic of Ethiopia
    'ge:GENC:3:1-2:FIN':'FI', // Republic of Finland
    'ge:GENC:3:1-2:FJI':'FJ', // Republic of Fiji
    'ge:GENC:3:1-2:FLK':'FK', // Falkland Islands (Islas Malvinas)
    'ge:GENC:3:1-2:FRA':'FR', // French Republic
    'ge:GENC:3:1-2:FRO':'FO', // Faroe Islands
    'ge:GENC:3:1-2:FSM':'FM', // Federated States of Micronesia
    'ge:GENC:3:1-2:GAB':'GA', // Gabonese Republic
    'ge:GENC:3:1-2:GBR':'GB', // United Kingdom of Great Britain and Northern Ireland
    'ge:GENC:3:1-2:GEO':'GE', // Georgia
    'ge:GENC:3:1-2:GGY':'GG', // Bailiwick of Guernsey
    'ge:GENC:3:1-2:GHA':'GH', // Republic of Ghana
    'ge:GENC:3:1-2:GIN':'GN', // Republic of Guinea
    'ge:GENC:3:1-2:GLP':'GP', // Department of Guadeloupe
    'ge:GENC:3:1-2:GMB':'GM', // Republic of The Gambia
    'ge:GENC:3:1-2:GNB':'GW', // Republic of Guinea-Bissau
    'ge:GENC:3:1-2:GNQ':'GQ', // Republic of Equatorial Guinea
    'ge:GENC:3:1-2:GRC':'GR', // Hellenic Republic
    'ge:GENC:3:1-2:GTM':'GT', // Republic of Guatemala
    'ge:GENC:3:1-2:GUF':'GF', // Department of Guiana
    'ge:GENC:3:1-2:GUM':'GU', // Territory of Guam
    'ge:GENC:3:1-2:GUY':'GY', // Co-operative Republic of Guyana
    'ge:GENC:3:1-2:HKG':'HK', // Hong Kong Special Administrative Region
    'ge:GENC:3:1-2:HMD':'HM', // Territory of Heard Island and McDonald Islands
    'ge:GENC:3:1-2:HND':'HN', // Republic of Honduras
    'ge:GENC:3:1-2:HRV':'HR', // Republic of Croatia
    'ge:GENC:3:1-2:HTI':'HT', // Republic of Haiti
    'ge:GENC:3:1-2:IDN':'ID', // Republic of Indonesia
    'ge:GENC:3:1-2:IND':'IN', // Republic of India
    'ge:GENC:3:1-2:IOT':'IO', // British Indian Ocean Territory
    'ge:GENC:3:1-2:IRN':'IR', // Islamic Republic of Iran
    'ge:GENC:3:1-2:IRQ':'IQ', // Republic of Iraq
    'ge:GENC:3:1-2:ISL':'IS', // Republic of Iceland
    'ge:GENC:3:1-2:ISR':'IL', // State of Israel
    'ge:GENC:3:1-2:ITA':'IT', // Italian Republic
    'ge:GENC:3:1-2:JEY':'JE', // Bailiwick of Jersey
    'ge:GENC:3:1-2:JOR':'JO', // Hashemite Kingdom of Jordan
    'ge:GENC:3:1-2:KAZ':'KZ', // Republic of Kazakhstan
    'ge:GENC:3:1-2:KEN':'KE', // Republic of Kenya
    'ge:GENC:3:1-2:KGZ':'KG', // Kyrgyz Republic
    'ge:GENC:3:1-2:KHM':'KH', // Kingdom of Cambodia
    'ge:GENC:3:1-2:KIR':'KI', // Republic of Kiribati
    'ge:GENC:3:1-2:KNA':'KN', // Federation of Saint Kitts and Nevis
    'ge:GENC:3:1-2:KOR':'KR', // Republic of Korea
    'ge:GENC:3:1-2:KWT':'KW', // State of Kuwait
    'ge:GENC:3:1-2:LAO':'LA', // Lao Peoples Democratic Republic
    'ge:GENC:3:1-2:LBN':'LB', // Lebanese Republic
    'ge:GENC:3:1-2:LBR':'LR', // Republic of Liberia
    'ge:GENC:3:1-2:LIE':'LI', // Principality of Liechtenstein
    'ge:GENC:3:1-2:LKA':'LK', // Democratic Socialist Republic of Sri Lanka
    'ge:GENC:3:1-2:LSO':'LS', // Kingdom of Lesotho
    'ge:GENC:3:1-2:LTU':'LT', // Republic of Lithuania
    'ge:GENC:3:1-2:LUX':'LU', // Grand Duchy of Luxembourg
    'ge:GENC:3:1-2:LVA':'LV', // Republic of Latvia
    'ge:GENC:3:1-2:MAC':'MO', // Macau Special Administrative Region
    'ge:GENC:3:1-2:MAF':'MF', // Saint Martin
    'ge:GENC:3:1-2:MAR':'MA', // Kingdom of Morocco
    'ge:GENC:3:1-2:MCO':'MC', // Principality of Monaco
    'ge:GENC:3:1-2:MDA':'MD', // Republic of Moldova
    'ge:GENC:3:1-2:MDG':'MG', // Republic of Madagascar
    'ge:GENC:3:1-2:MDV':'MV', // Republic of Maldives
    'ge:GENC:3:1-2:MEX':'MX', // United Mexican States
    'ge:GENC:3:1-2:MHL':'MH', // Republic of the Marshall Islands
    'ge:GENC:3:1-2:MKD':'MK', // Republic of Macedonia
    'ge:GENC:3:1-2:MLI':'ML', // Republic of Mali
    'ge:GENC:3:1-2:MLT':'MT', // Republic of Malta
    'ge:GENC:3:1-2:MMR':'MM', // Union of Burma
    'ge:GENC:3:1-2:MNG':'MN', // Mongolia
    'ge:GENC:3:1-2:MNP':'MP', // Commonwealth of the Northern Mariana Islands
    'ge:GENC:3:1-2:MOZ':'MZ', // Republic of Mozambique
    'ge:GENC:3:1-2:MRT':'MR', // Islamic Republic of Mauritania
    'ge:GENC:3:1-2:MTQ':'MQ', // Department of Martinique
    'ge:GENC:3:1-2:MUS':'MU', // Republic of Mauritius
    'ge:GENC:3:1-2:MWI':'MW', // Republic of Malawi
    'ge:GENC:3:1-2:MYS':'MY', // Malaysia
    'ge:GENC:3:1-2:MYT':'YT', // Department of Mayotte
    'ge:GENC:3:1-2:NAM':'NA', // Republic of Namibia
    'ge:GENC:3:1-2:NCL':'NC', // New Caledonia
    'ge:GENC:3:1-2:NER':'NE', // Republic of the Niger
    'ge:GENC:3:1-2:NFK':'NF', // Territory of Norfolk Island
    'ge:GENC:3:1-2:NGA':'NG', // Federal Republic of Nigeria
    'ge:GENC:3:1-2:NIC':'NI', // Republic of Nicaragua
    'ge:GENC:3:1-2:NLD':'NL', // Kingdom of the Netherlands
    'ge:GENC:3:1-2:NOR':'NO', // Kingdom of Norway
    'ge:GENC:3:1-2:NPL':'NP', // Federal Democratic Republic of Nepal
    'ge:GENC:3:1-2:NRU':'NR', // Republic of Nauru
    'ge:GENC:3:1-2:NZL':'NZ', // New Zealand
    'ge:GENC:3:1-2:OMN':'OM', // Sultanate of Oman
    'ge:GENC:3:1-2:PAK':'PK', // Islamic Republic of Pakistan
    'ge:GENC:3:1-2:PAN':'PA', // Republic of Panama
    'ge:GENC:3:1-2:PCN':'PN', // Pitcairn, Henderson, Ducie, and Oeno Islands
    'ge:GENC:3:1-2:PER':'PE', // Republic of Peru
    'ge:GENC:3:1-2:PHL':'PH', // Republic of the Philippines
    'ge:GENC:3:1-2:PLW':'PW', // Republic of Palau
    'ge:GENC:3:1-2:POL':'PL', // Republic of Poland
    'ge:GENC:3:1-2:PRI':'PR', // Commonwealth of Puerto Rico
    'ge:GENC:3:1-2:PRK':'KP', // Democratic Peoples Republic of Korea
    'ge:GENC:3:1-2:PRT':'PT', // Portuguese Republic
    'ge:GENC:3:1-2:PRY':'PY', // Republic of Paraguay
    'ge:GENC:3:1-2:PSE':'PS', // Palestinian Territory
    'ge:GENC:3:1-2:PYF':'PF', // French Polynesia
    'ge:GENC:3:1-2:QAT':'QA', // State of Qatar
    'ge:GENC:3:1-2:REU':'RE', // Department of Reunion
    'ge:GENC:3:1-2:RUS':'RU', // Russian Federation
    'ge:GENC:3:1-2:RWA':'RW', // Republic of Rwanda
    'ge:GENC:3:1-2:SAU':'SA', // Kingdom of Saudi Arabia
    'ge:GENC:3:1-2:SDN':'SD', // Republic of the Sudan
    'ge:GENC:3:1-2:SEN':'SN', // Republic of Senegal
    'ge:GENC:3:1-2:SGP':'SG', // Republic of Singapore
    'ge:GENC:3:1-2:SGS':'GS', // South Georgia and South Sandwich Islands
    'ge:GENC:3:1-2:SHN':'SH', // Saint Helena, Ascension, and Tristan da Cunha
    'ge:GENC:3:1-2:SLB':'SB', // Solomon Islands
    'ge:GENC:3:1-2:SLE':'SL', // Republic of Sierra Leone
    'ge:GENC:3:1-2:SLV':'SV', // Republic of El Salvador
    'ge:GENC:3:1-2:SMR':'SM', // Republic of San Marino
    'ge:GENC:3:1-2:SOM':'SO', // Somalia, Federal Republic of
    'ge:GENC:3:1-2:SPM':'PM', // Territorial Collectivity of Saint Pierre and Miquelon
    'ge:GENC:3:1-2:SRB':'RS', // Republic of Serbia
    'ge:GENC:3:1-2:SSD':'SS', // Republic of South Sudan
    'ge:GENC:3:1-2:STP':'ST', // Democratic Republic of Sao Tome and Principe
    'ge:GENC:3:1-2:SUR':'SR', // Republic of Suriname
    'ge:GENC:3:1-2:SVK':'SK', // Slovak Republic
    'ge:GENC:3:1-2:SVN':'SI', // Republic of Slovenia
    'ge:GENC:3:1-2:SWE':'SE', // Kingdom of Sweden
    'ge:GENC:3:1-2:SWZ':'SZ', // Kingdom of Swaziland
    'ge:GENC:3:1-2:SXM':'SX', // Sint Maarten
    'ge:GENC:3:1-2:SYC':'SC', // Republic of Seychelles
    'ge:GENC:3:1-2:SYR':'SY', // Syrian Arab Republic
    'ge:GENC:3:1-2:TCA':'TC', // Turks and Caicos Islands
    'ge:GENC:3:1-2:TCD':'TD', // Republic of Chad
    'ge:GENC:3:1-2:TGO':'TG', // Togolese Republic
    'ge:GENC:3:1-2:THA':'TH', // Kingdom of Thailand
    'ge:GENC:3:1-2:TJK':'TJ', // Republic of Tajikistan
    'ge:GENC:3:1-2:TLS':'TL', // Democratic Republic of Timor-Leste
    'ge:GENC:3:1-2:TON':'TO', // Kingdom of Tonga
    'ge:GENC:3:1-2:TTO':'TT', // Republic of Trinidad and Tobago
    'ge:GENC:3:1-2:TUN':'TN', // Tunisian Republic
    'ge:GENC:3:1-2:TUR':'TR', // Republic of Turkey
    'ge:GENC:3:1-2:TWN':'TW', // Taiwan
    'ge:GENC:3:1-2:TZA':'TZ', // United Republic of Tanzania
    'ge:GENC:3:1-2:UGA':'UG', // Republic of Uganda
    'ge:GENC:3:1-2:UKR':'UA', // Ukraine
    'ge:GENC:3:1-2:URY':'UY', // Oriental Republic of Uruguay
    'ge:GENC:3:1-2:USA':'US', // United States of America
    'ge:GENC:3:1-2:UZB':'UZ', // Republic of Uzbekistan
    'ge:GENC:3:1-2:VAT':'VA', // State of the Vatican City
    'ge:GENC:3:1-2:VCT':'VC', // Saint Vincent and the Grenadines
    'ge:GENC:3:1-2:VEN':'VE', // Bolivarian Republic of Venezuela
    'ge:GENC:3:1-2:VGB':'VG', // Virgin Islands, British
    'ge:GENC:3:1-2:VIR':'VI', // United States Virgin Islands
    'ge:GENC:3:1-2:VNM':'VN', // Socialist Republic of Vietnam
    'ge:GENC:3:1-2:VUT':'VU', // Republic of Vanuatu
    'ge:GENC:3:1-2:WLF':'WF', // Wallis and Futuna
    'ge:GENC:3:1-2:WSM':'WS', // Independent State of Samoa
    'ge:GENC:3:1-2:XAC':'XA', // Territory of Ashmore and Cartier Islands
    'ge:GENC:3:1-2:XAZ':'XAZ', // Entity 1
    'ge:GENC:3:1-2:XBI':'QS', // Bassas da India
    'ge:GENC:3:1-2:XBK':'XB', // Baker Island
    'ge:GENC:3:1-2:XCR':'XCR', // Entity 2
    'ge:GENC:3:1-2:XCS':'XC', // Coral Sea Islands Territory
    'ge:GENC:3:1-2:XCY':'XCY', // Entity 3
    'ge:GENC:3:1-2:XEU':'XE', // Europa Island
    'ge:GENC:3:1-2:XGL':'QX', // Glorioso Islands
    'ge:GENC:3:1-2:XGZ':'XG', // Gaza Strip
    'ge:GENC:3:1-2:XHO':'XH', // Howland Island
    'ge:GENC:3:1-2:XJA':'XU', // Johnston Atoll
    'ge:GENC:3:1-2:XJM':'XJ', // Jan Mayen
    'ge:GENC:3:1-2:XJN':'QU', // Juan de Nova Island
    'ge:GENC:3:1-2:XJV':'XQ', // Jarvis Island
    'ge:GENC:3:1-2:XKM':'XKM', // Entity 4
    'ge:GENC:3:1-2:XKN':'XKN', // Entity 5
    'ge:GENC:3:1-2:XKR':'XM', // Kingman Reef
    'ge:GENC:3:1-2:XKS':'XK', // Republic of Kosovo
    'ge:GENC:3:1-2:XMW':'QM', // Midway Islands
    'ge:GENC:3:1-2:XNV':'XV', // Navassa Island
    'ge:GENC:3:1-2:XPL':'XL', // Palmyra Atoll
    'ge:GENC:3:1-2:XPR':'XP', // Paracel Islands
    'ge:GENC:3:1-2:XQP':'QP', // Etorofu, Habomai, Kunashiri, and Shikotan Islands
    'ge:GENC:3:1-2:XQZ':'QZ', // Akrotiri
    'ge:GENC:3:1-2:XSP':'XS', // Spratly Islands
    'ge:GENC:3:1-2:XSV':'XR', // Svalbard
    'ge:GENC:3:1-2:XTR':'XT', // Tromelin Island
    'ge:GENC:3:1-2:XWB':'XW', // West Bank
    'ge:GENC:3:1-2:XWK':'QW', // Wake Island
    'ge:GENC:3:1-2:XXD':'XD', // Dhekelia
    'ge:GENC:3:1-2:XXX':'XXX', // No Mans Land
    'ge:GENC:3:1-2:YEM':'YE', // Republic of Yemen
    'ge:GENC:3:1-2:ZAF':'ZA', // Republic of South Africa
    'ge:GENC:3:1-2:ZMB':'ZM', // Republic of Zambia
    'ge:GENC:3:1-2:ZWE':'ZW', // Republic of Zimbabwe
    'ge:ISO1:3:VI-15:AIA':'AI', // Anguilla
    'ge:ISO1:3:VI-15:ATA':'AQ', // Antarctica
    'ge:ISO1:3:VI-15:BIH':'BA', // Bosnia and Herzegovina
    'ge:ISO1:3:VI-15:BLZ':'BZ', // Belize
    'ge:ISO1:3:VI-15:BMU':'BM', // Bermuda
    'ge:ISO1:3:VI-15:BRB':'BB', // Barbados
    'ge:ISO1:3:VI-15:BVT':'BV', // Bouvet Island
    'ge:ISO1:3:VI-15:GIB':'GI', // Gibraltar
    'ge:ISO1:3:VI-15:GRD':'GD', // Grenada
    'ge:ISO1:3:VI-15:GRL':'GL', // Greenland
    'ge:ISO1:3:VI-15:HUN':'HU', // Hungary
    'ge:ISO1:3:VI-15:IMN':'IM', // Isle of Man
    'ge:ISO1:3:VI-15:IRL':'IE', // Ireland
    'ge:ISO1:3:VI-15:JAM':'JM', // Jamaica
    'ge:ISO1:3:VI-15:JPN':'JP', // Japan
    'ge:ISO1:3:VI-15:LBY':'LY', // Libya
    'ge:ISO1:3:VI-15:LCA':'LC', // Saint Lucia
    'ge:ISO1:3:VI-15:MNE':'ME', // Montenegro
    'ge:ISO1:3:VI-15:MSR':'MS', // Montserrat
    'ge:ISO1:3:VI-15:NIU':'NU', // Niue
    'ge:ISO1:3:VI-15:PNG':'PG', // Independent State of Papua New Guinea
    'ge:ISO1:3:VI-15:ROU':'RO', // Romania
    'ge:ISO1:3:VI-15:TKL':'TK', // Tokelau
    'ge:ISO1:3:VI-15:TKM':'TM', // Turkmenistan
    'ge:ISO1:3:VI-15:TUV':'TV', // Tuvalu
    'other':'other', // Other
    },
    // ##### End of ge4List #####

    // ##### Start of Thematic Group Rules #####
    thematicGroupList : {
    'PAA010':'IndustryPnt', // Industry
    'AAA010':'IndustrySrf', // Industry
    'LAA011':'IndustryCrv', // Industry
    'PAA020':'IndustryPnt', // Industry
    'AAA020':'IndustrySrf', // Industry
    'PAA040':'IndustryPnt', // Industry
    'AAA040':'IndustrySrf', // Industry
    'PAA045':'IndustryPnt', // Industry
    'AAA052':'IndustrySrf', // Industry
    'PAA054':'IndustryPnt', // Industry
    'PAB000':'IndustryPnt', // Industry
    'AAB000':'IndustrySrf', // Industry
    'AAB010':'IndustrySrf', // Industry
    'PAB021':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAB040':'IndustrySrf', // Industry
    'PAB507':'IndustryPnt', // Industry
    'AAB507':'IndustrySrf', // Industry
    'PAC010':'IndustryPnt', // Industry
    'AAC010':'IndustrySrf', // Industry
    'PAC020':'IndustryPnt', // Industry
    'AAC020':'IndustrySrf', // Industry
    'AAC030':'IndustrySrf', // Industry
    'PAC040':'IndustryPnt', // Industry
    'AAC040':'IndustrySrf', // Industry
    'PAC060':'IndustryPnt', // Industry
    'AAC060':'IndustrySrf', // Industry
    'PAC507':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAC507':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD010':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD010':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD020':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD020':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD025':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD025':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD030':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD030':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD041':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD041':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD050':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD050':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD055':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD055':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAD060':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAD060':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAF010':'UtilityInfrastructurePnt', // Utility Infrastructure
    'LAF020':'IndustryCrv', // Industry
    'PAF030':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAF030':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAF040':'IndustryPnt', // Industry
    'AAF040':'IndustrySrf', // Industry
    'PAF050':'IndustryPnt', // Industry
    'LAF050':'IndustryCrv', // Industry
    'PAF060':'StructurePnt', // Structure
    'AAF060':'StructureSrf', // Structure
    'PAF070':'IndustryPnt', // Industry
    'PAF080':'IndustryPnt', // Industry
    'AAG030':'FacilitySrf', // Facility
    'AAG040':'FacilitySrf', // Facility
    'PAG050':'StructurePnt', // Structure
    'LAH025':'MilitaryCrv', // Military
    'AAH025':'MilitarySrf', // Military
    'PAH055':'MilitaryPnt', // Military
    'AAH055':'MilitarySrf', // Military
    'PAH060':'MilitaryPnt', // Military
    'AAH060':'MilitarySrf', // Military
    'PAH070':'TransportationGroundPnt', // Transportation - Ground
    'AAI020':'SettlementSrf', // Settlement
    'AAI021':'SettlementSrf', // Settlement
    'PAI030':'SettlementPnt', // Settlement
    'AAI030':'SettlementSrf', // Settlement
    'PAJ030':'AgriculturePnt', // Agriculture
    'AAJ030':'AgricultureSrf', // Agriculture
    'PAJ050':'AgriculturePnt', // Agriculture
    'AAJ050':'AgricultureSrf', // Agriculture
    'PAJ051':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAJ055':'IndustryPnt', // Industry
    'AAJ055':'IndustrySrf', // Industry
    'PAJ080':'AgriculturePnt', // Agriculture
    'AAJ080':'AgricultureSrf', // Agriculture
    'PAJ085':'AgriculturePnt', // Agriculture
    'AAJ085':'AgricultureSrf', // Agriculture
    'PAJ110':'AgriculturePnt', // Agriculture
    'AAJ110':'AgricultureSrf', // Agriculture
    'PAK020':'RecreationPnt', // Recreation
    'LAK020':'RecreationCrv', // Recreation
    'AAK020':'RecreationSrf', // Recreation
    'PAK030':'RecreationPnt', // Recreation
    'AAK030':'RecreationSrf', // Recreation
    'PAK040':'RecreationPnt', // Recreation
    'AAK040':'RecreationSrf', // Recreation
    'PAK060':'RecreationPnt', // Recreation
    'AAK060':'RecreationSrf', // Recreation
    'AAK061':'RecreationSrf', // Recreation
    'AAK070':'RecreationSrf', // Recreation
    'PAK080':'RecreationPnt', // Recreation
    'LAK080':'RecreationCrv', // Recreation
    'AAK090':'RecreationSrf', // Recreation
    'AAK100':'RecreationSrf', // Recreation
    'AAK101':'RecreationSrf', // Recreation
    'PAK110':'RecreationPnt', // Recreation
    'AAK110':'RecreationSrf', // Recreation
    'AAK120':'CultureSrf', // Culture
    'PAK121':'CulturePnt', // Culture
    'AAK121':'CultureSrf', // Culture
    'LAK130':'RecreationCrv', // Recreation
    'AAK130':'RecreationSrf', // Recreation
    'PAK150':'RecreationPnt', // Recreation
    'LAK150':'RecreationCrv', // Recreation
    'LAK155':'RecreationCrv', // Recreation
    'AAK155':'RecreationSrf', // Recreation
    'PAK160':'RecreationPnt', // Recreation
    'AAK160':'RecreationSrf', // Recreation
    'PAK161':'RecreationPnt', // Recreation
    'PAK164':'RecreationPnt', // Recreation
    'AAK164':'RecreationSrf', // Recreation
    'PAK170':'RecreationPnt', // Recreation
    'AAK170':'RecreationSrf', // Recreation
    'PAK180':'RecreationPnt', // Recreation
    'AAK180':'RecreationSrf', // Recreation
    'PAL010':'FacilityPnt', // Facility
    'AAL010':'FacilitySrf', // Facility
    'PAL011':'FacilityPnt', // Facility
    'AAL011':'FacilitySrf', // Facility
    'PAL012':'CulturePnt', // Culture
    'AAL012':'CultureSrf', // Culture
    'PAL013':'StructurePnt', // Structure
    'AAL013':'StructureSrf', // Structure
    'PAL014':'StructurePnt', // Structure
    'AAL014':'StructureSrf', // Structure
    'PAL017':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAL018':'StructurePnt', // Structure
    'LAL018':'StructureCrv', // Structure
    'AAL018':'StructureSrf', // Structure
    'PAL019':'StructurePnt', // Structure
    'AAL019':'StructureSrf', // Structure
    'PAL020':'SettlementPnt', // Settlement
    'AAL020':'SettlementSrf', // Settlement
    'PAL025':'CulturePnt', // Culture
    'PAL030':'CulturePnt', // Culture
    'AAL030':'CultureSrf', // Culture
    'PAL036':'CulturePnt', // Culture
    'AAL036':'CultureSrf', // Culture
    'LAL060':'MilitaryCrv', // Military
    'AAL060':'MilitarySrf', // Military
    'AAL065':'MilitarySrf', // Military
    'LAL070':'StructureCrv', // Structure
    'PAL073':'StructurePnt', // Structure
    'PAL080':'StructurePnt', // Structure
    'LAL080':'StructureCrv', // Structure
    'PAL099':'StructurePnt', // Structure
    'AAL099':'StructureSrf', // Structure
    'PAL105':'SettlementPnt', // Settlement
    'AAL105':'SettlementSrf', // Settlement
    'PAL110':'StructurePnt', // Structure
    'PAL120':'MilitaryPnt', // Military
    'AAL120':'MilitarySrf', // Military
    'PAL130':'CulturePnt', // Culture
    'LAL130':'CultureCrv', // Culture
    'AAL130':'CultureSrf', // Culture
    'LAL140':'StructureCrv', // Structure
    'AAL140':'StructureSrf', // Structure
    'PAL142':'StructurePnt', // Structure
    'AAL142':'StructureSrf', // Structure
    'PAL155':'TransportationGroundPnt', // Transportation - Ground
    'LAL155':'TransportationGroundCrv', // Transportation - Ground
    'PAL165':'TransportationGroundPnt', // Transportation - Ground
    'PAL170':'RecreationPnt', // Recreation
    'AAL170':'RecreationSrf', // Recreation
    'AAL175':'CultureSrf', // Culture
    'AAL180':'CultureSrf', // Culture
    'LAL195':'TransportationGroundCrv', // Transportation - Ground
    'AAL195':'TransportationGroundSrf', // Transportation - Ground
    'PAL200':'CulturePnt', // Culture
    'AAL200':'CultureSrf', // Culture
    'PAL201':'CulturePnt', // Culture
    'AAL201':'CultureSrf', // Culture
    'PAL208':'SettlementPnt', // Settlement
    'AAL208':'SettlementSrf', // Settlement
    'PAL211':'TransportationGroundPnt', // Transportation - Ground
    'LAL211':'TransportationGroundCrv', // Transportation - Ground
    'AAL211':'TransportationGroundSrf', // Transportation - Ground
    'PAL241':'StructurePnt', // Structure
    'AAL241':'StructureSrf', // Structure
    'PAL250':'StructurePnt', // Structure
    'LAL260':'StructureCrv', // Structure
    'PAL270':'AgriculturePnt', // Agriculture
    'AAL270':'AgricultureSrf', // Agriculture
    'PAL351':'AeronauticPnt', // Aeronautic
    'AAL351':'AeronauticSrf', // Aeronautic
    'PAL371':'StructurePnt', // Structure
    'AAL371':'StructureSrf', // Structure
    'PAL375':'MilitaryPnt', // Military
    'AAL375':'MilitarySrf', // Military
    'PAL376':'MilitaryPnt', // Military
    'AAL376':'MilitarySrf', // Military
    'PAL510':'AeronauticPnt', // Aeronautic
    'PAM010':'StoragePnt', // Storage
    'AAM010':'StorageSrf', // Storage
    'PAM011':'StoragePnt', // Storage
    'AAM011':'StorageSrf', // Storage
    'PAM020':'AgriculturePnt', // Agriculture
    'AAM020':'AgricultureSrf', // Agriculture
    'PAM030':'StoragePnt', // Storage
    'AAM030':'StorageSrf', // Storage
    'PAM040':'IndustryPnt', // Industry
    'AAM040':'IndustrySrf', // Industry
    'PAM060':'MilitaryPnt', // Military
    'AAM060':'MilitarySrf', // Military
    'PAM065':'StoragePnt', // Storage
    'AAM065':'StorageSrf', // Storage
    'PAM070':'StoragePnt', // Storage
    'AAM070':'StorageSrf', // Storage
    'PAM071':'StoragePnt', // Storage
    'AAM071':'StorageSrf', // Storage
    'PAM075':'StoragePnt', // Storage
    'AAM075':'StorageSrf', // Storage
    'PAM080':'StoragePnt', // Storage
    'AAM080':'StorageSrf', // Storage
    'LAN010':'TransportationGroundCrv', // Transportation - Ground
    'LAN050':'TransportationGroundCrv', // Transportation - Ground
    'AAN060':'TransportationGroundSrf', // Transportation - Ground
    'PAN075':'TransportationGroundPnt', // Transportation - Ground
    'AAN075':'TransportationGroundSrf', // Transportation - Ground
    'PAN076':'TransportationGroundPnt', // Transportation - Ground
    'AAN076':'TransportationGroundSrf', // Transportation - Ground
    'PAN085':'TransportationGroundPnt', // Transportation - Ground
    'LAP010':'TransportationGroundCrv', // Transportation - Ground
    'PAP020':'TransportationGroundPnt', // Transportation - Ground
    'LAP030':'TransportationGroundCrv', // Transportation - Ground
    'AAP030':'TransportationGroundSrf', // Transportation - Ground
    'PAP033':'TransportationGroundPnt', // Transportation - Ground
    'PAP040':'TransportationGroundPnt', // Transportation - Ground
    'LAP040':'TransportationGroundCrv', // Transportation - Ground
    'PAP041':'TransportationGroundPnt', // Transportation - Ground
    'LAP041':'TransportationGroundCrv', // Transportation - Ground
    'LAP050':'TransportationGroundCrv', // Transportation - Ground
    'AAP055':'TransportationGroundSrf', // Transportation - Ground
    'PAP056':'TransportationGroundPnt', // Transportation - Ground
    'AAP056':'TransportationGroundSrf', // Transportation - Ground
    'LAQ035':'TransportationGroundCrv', // Transportation - Ground
    'PAQ040':'TransportationGroundPnt', // Transportation - Ground
    'LAQ040':'TransportationGroundCrv', // Transportation - Ground
    'AAQ040':'TransportationGroundSrf', // Transportation - Ground
    'PAQ045':'TransportationGroundPnt', // Transportation - Ground
    'LAQ045':'TransportationGroundCrv', // Transportation - Ground
    'AAQ045':'TransportationGroundSrf', // Transportation - Ground
    'LAQ050':'TransportationGroundCrv', // Transportation - Ground
    'AAQ050':'TransportationGroundSrf', // Transportation - Ground
    'PAQ055':'TransportationGroundPnt', // Transportation - Ground
    'PAQ056':'TransportationGroundPnt', // Transportation - Ground
    'LAQ056':'TransportationGroundCrv', // Transportation - Ground
    'AAQ056':'TransportationGroundSrf', // Transportation - Ground
    'PAQ059':'TransportationGroundPnt', // Transportation - Ground
    'LAQ059':'TransportationGroundCrv', // Transportation - Ground
    'PAQ060':'AeronauticPnt', // Aeronautic
    'AAQ060':'AeronauticSrf', // Aeronautic
    'PAQ062':'TransportationGroundPnt', // Transportation - Ground
    'LAQ063':'TransportationGroundCrv', // Transportation - Ground
    'AAQ063':'TransportationGroundSrf', // Transportation - Ground
    'PAQ065':'TransportationGroundPnt', // Transportation - Ground
    'LAQ065':'TransportationGroundCrv', // Transportation - Ground
    'PAQ068':'TransportationGroundPnt', // Transportation - Ground
    'AAQ068':'TransportationGroundSrf', // Transportation - Ground
    'LAQ070':'TransportationWaterCrv', // Transportation - Water
    'LAQ075':'TransportationGroundCrv', // Transportation - Ground
    'PAQ080':'TransportationWaterPnt', // Transportation - Water
    'AAQ080':'TransportationWaterSrf', // Transportation - Water
    'PAQ095':'TransportationGroundPnt', // Transportation - Ground
    'PAQ110':'AeronauticPnt', // Aeronautic
    'PAQ111':'TransportationWaterPnt', // Transportation - Water
    'LAQ113':'UtilityInfrastructureCrv', // Utility Infrastructure
    'PAQ114':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAQ115':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAQ116':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAQ116':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PAQ118':'TransportationGroundPnt', // Transportation - Ground
    'LAQ120':'TransportationGroundCrv', // Transportation - Ground
    'PAQ125':'TransportationGroundPnt', // Transportation - Ground
    'AAQ125':'TransportationGroundSrf', // Transportation - Ground
    'LAQ130':'TransportationGroundCrv', // Transportation - Ground
    'AAQ130':'TransportationGroundSrf', // Transportation - Ground
    'PAQ135':'TransportationGroundPnt', // Transportation - Ground
    'AAQ135':'TransportationGroundSrf', // Transportation - Ground
    'AAQ140':'TransportationGroundSrf', // Transportation - Ground
    'PAQ141':'TransportationGroundPnt', // Transportation - Ground
    'AAQ141':'TransportationGroundSrf', // Transportation - Ground
    'LAQ150':'StructureCrv', // Structure
    'AAQ150':'StructureSrf', // Structure
    'LAQ151':'TransportationGroundCrv', // Transportation - Ground
    'AAQ151':'TransportationGroundSrf', // Transportation - Ground
    'PAQ160':'TransportationGroundPnt', // Transportation - Ground
    'PAQ161':'TransportationGroundPnt', // Transportation - Ground
    'PAQ162':'TransportationGroundPnt', // Transportation - Ground
    'PAQ170':'TransportationGroundPnt', // Transportation - Ground
    'AAQ170':'TransportationGroundSrf', // Transportation - Ground
    'LAT005':'UtilityInfrastructureCrv', // Utility Infrastructure
    'PAT010':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAT011':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAT012':'UtilityInfrastructurePnt', // Utility Infrastructure
    'AAT012':'UtilityInfrastructureSrf', // Utility Infrastructure
    'LAT041':'TransportationGroundCrv', // Transportation - Ground
    'PAT042':'UtilityInfrastructurePnt', // Utility Infrastructure
    'PAT045':'FacilityPnt', // Facility
    'AAT045':'FacilitySrf', // Facility
    'LBA010':'PhysiographyCrv', // Physiography
    'PBA030':'PhysiographyPnt', // Physiography
    'ABA030':'PhysiographySrf', // Physiography
    'ABA040':'HydrographySrf', // Hydrography
    'ABB005':'PortorHarbourSrf', // Port or Harbour
    'PBB009':'PortorHarbourPnt', // Port or Harbour
    'ABB009':'PortorHarbourSrf', // Port or Harbour
    'LBB081':'PortorHarbourCrv', // Port or Harbour
    'ABB081':'PortorHarbourSrf', // Port or Harbour
    'LBB082':'PortorHarbourCrv', // Port or Harbour
    'ABB082':'PortorHarbourSrf', // Port or Harbour
    'ABB090':'PortorHarbourSrf', // Port or Harbour
    'PBB110':'HydrographyPnt', // Hydrography
    'ABB110':'HydrographySrf', // Hydrography
    'ABB199':'PortorHarbourSrf', // Port or Harbour
    'PBB201':'PortorHarbourPnt', // Port or Harbour
    'ABB201':'PortorHarbourSrf', // Port or Harbour
    'PBB241':'PortorHarbourPnt', // Port or Harbour
    'ABB241':'PortorHarbourSrf', // Port or Harbour
    'PBC050':'HydrographicAidtoNavigationPnt', // Hydrographic Aid to Navigation
    'ABC050':'HydrographicAidtoNavigationSrf', // Hydrographic Aid to Navigation
    'PBC070':'HydrographicAidtoNavigationPnt', // Hydrographic Aid to Navigation
    'PBD100':'PortorHarbourPnt', // Port or Harbour
    'ABD100':'PortorHarbourSrf', // Port or Harbour
    'PBD115':'HydrographyPnt', // Hydrography
    'ABD115':'HydrographySrf', // Hydrography
    'PBD140':'HydrographyPnt', // Hydrography
    'ABD140':'HydrographySrf', // Hydrography
    'PBD181':'HydrographyPnt', // Hydrography
    'LBH010':'HydrographyCrv', // Hydrography
    'ABH010':'HydrographySrf', // Hydrography
    'PBH012':'HydrographyPnt', // Hydrography
    'ABH015':'VegetationSrf', // Vegetation
    'LBH020':'TransportationWaterCrv', // Transportation - Water
    'ABH020':'TransportationWaterSrf', // Transportation - Water
    'LBH030':'HydrographyCrv', // Hydrography
    'ABH030':'HydrographySrf', // Hydrography
    'ABH040':'IndustrySrf', // Industry
    'PBH051':'AgriculturePnt', // Agriculture
    'ABH051':'AgricultureSrf', // Agriculture
    'LBH065':'HydrographyCrv', // Hydrography
    'PBH070':'TransportationGroundPnt', // Transportation - Ground
    'LBH070':'TransportationGroundCrv', // Transportation - Ground
    'ABH070':'TransportationGroundSrf', // Transportation - Ground
    'PBH075':'CulturePnt', // Culture
    'ABH075':'CultureSrf', // Culture
    'ABH077':'VegetationSrf', // Vegetation
    'PBH082':'HydrographyPnt', // Hydrography
    'ABH082':'HydrographySrf', // Hydrography
    'ABH090':'HydrographySrf', // Hydrography
    'LBH100':'HydrographyCrv', // Hydrography
    'ABH100':'HydrographySrf', // Hydrography
    'LBH110':'HydrographyCrv', // Hydrography
    'ABH116':'SubterraneanSrf', // Subterranean
    'PBH120':'HydrographyPnt', // Hydrography
    'LBH120':'HydrographyCrv', // Hydrography
    'ABH120':'HydrographySrf', // Hydrography
    'ABH135':'AgricultureSrf', // Agriculture
    'LBH140':'HydrographyCrv', // Hydrography
    'ABH140':'HydrographySrf', // Hydrography
    'PBH145':'HydrographyPnt', // Hydrography
    'ABH150':'PhysiographySrf', // Physiography
    'PBH155':'IndustryPnt', // Industry
    'ABH155':'IndustrySrf', // Industry
    'ABH160':'PhysiographySrf', // Physiography
    'LBH165':'HydrographyCrv', // Hydrography
    'ABH165':'HydrographySrf', // Hydrography
    'PBH170':'HydrographyPnt', // Hydrography
    'ABH170':'HydrographySrf', // Hydrography
    'PBH180':'HydrographyPnt', // Hydrography
    'LBH180':'HydrographyCrv', // Hydrography
    'PBH220':'UtilityInfrastructurePnt', // Utility Infrastructure
    'ABH220':'UtilityInfrastructureSrf', // Utility Infrastructure
    'PBH230':'HydrographyPnt', // Hydrography
    'ABH230':'HydrographySrf', // Hydrography
    'ABI005':'PortorHarbourSrf', // Port or Harbour
    'PBI006':'TransportationWaterPnt', // Transportation - Water
    'LBI006':'TransportationWaterCrv', // Transportation - Water
    'ABI006':'TransportationWaterSrf', // Transportation - Water
    'PBI010':'HydrographyPnt', // Hydrography
    'PBI020':'HydrographyPnt', // Hydrography
    'LBI020':'HydrographyCrv', // Hydrography
    'ABI020':'HydrographySrf', // Hydrography
    'PBI030':'TransportationWaterPnt', // Transportation - Water
    'LBI030':'TransportationWaterCrv', // Transportation - Water
    'ABI030':'TransportationWaterSrf', // Transportation - Water
    'PBI040':'HydrographyPnt', // Hydrography
    'LBI040':'HydrographyCrv', // Hydrography
    'PBI044':'HydrographyPnt', // Hydrography
    'LBI044':'HydrographyCrv', // Hydrography
    'ABI044':'HydrographySrf', // Hydrography
    'PBI045':'TransportationWaterPnt', // Transportation - Water
    'LBI045':'TransportationWaterCrv', // Transportation - Water
    'PBI050':'HydrographyPnt', // Hydrography
    'ABI050':'HydrographySrf', // Hydrography
    'LBI060':'HydrographyCrv', // Hydrography
    'PBI070':'HydrographyPnt', // Hydrography
    'ABJ020':'PhysiographySrf', // Physiography
    'ABJ030':'PhysiographySrf', // Physiography
    'LBJ031':'PhysiographyCrv', // Physiography
    'ABJ031':'PhysiographySrf', // Physiography
    'LBJ040':'PhysiographyCrv', // Physiography
    'PBJ060':'PhysiographyPnt', // Physiography
    'ABJ065':'PhysiographySrf', // Physiography
    'ABJ080':'PhysiographySrf', // Physiography
    'ABJ099':'PhysiographySrf', // Physiography
    'ABJ100':'PhysiographySrf', // Physiography
    'ABJ110':'VegetationSrf', // Vegetation
    'LCA010':'HypsographyCrv', // Hypsography
    'PCA030':'HypsographyPnt', // Hypsography
    'ADA005':'PhysiographySrf', // Physiography
    'ADA010':'PhysiographySrf', // Physiography
    'LDB010':'PhysiographyCrv', // Physiography
    'ADB028':'SubterraneanSrf', // Subterranean
    'PDB029':'PhysiographyPnt', // Physiography
    'LDB061':'PhysiographyCrv', // Physiography
    'ADB061':'PhysiographySrf', // Physiography
    'LDB070':'PhysiographyCrv', // Physiography
    'LDB071':'PhysiographyCrv', // Physiography
    'ADB080':'PhysiographySrf', // Physiography
    'LDB090':'PhysiographyCrv', // Physiography
    'ADB090':'PhysiographySrf', // Physiography
    'LDB100':'PhysiographyCrv', // Physiography
    'LDB110':'PhysiographyCrv', // Physiography
    'PDB115':'PhysiographyPnt', // Physiography
    'ADB115':'PhysiographySrf', // Physiography
    'PDB150':'PhysiographyPnt', // Physiography
    'PDB160':'PhysiographyPnt', // Physiography
    'ADB160':'PhysiographySrf', // Physiography
    'ADB170':'PhysiographySrf', // Physiography
    'PDB180':'PhysiographyPnt', // Physiography
    'ADB180':'PhysiographySrf', // Physiography
    'LDB190':'PhysiographyCrv', // Physiography
    'ADB211':'PhysiographySrf', // Physiography
    'AEA010':'AgricultureSrf', // Agriculture
    'LEA020':'VegetationCrv', // Vegetation
    'AEA030':'AgricultureSrf', // Agriculture
    'AEA031':'CultureSrf', // Culture
    'AEA040':'AgricultureSrf', // Agriculture
    'AEA050':'AgricultureSrf', // Agriculture
    'AEA055':'AgricultureSrf', // Agriculture
    'AEB010':'VegetationSrf', // Vegetation
    'AEB020':'VegetationSrf', // Vegetation
    'AEB070':'VegetationSrf', // Vegetation
    'PEC005':'VegetationPnt', // Vegetation
    'AEC010':'AgricultureSrf', // Agriculture
    'LEC015':'VegetationCrv', // Vegetation
    'AEC015':'VegetationSrf', // Vegetation
    'PEC020':'PhysiographyPnt', // Physiography
    'AEC020':'PhysiographySrf', // Physiography
    'LEC040':'VegetationCrv', // Vegetation
    'AEC040':'VegetationSrf', // Vegetation
    'AEC060':'VegetationSrf', // Vegetation
    'AED010':'VegetationSrf', // Vegetation
    'AED020':'VegetationSrf', // Vegetation
    'AEE010':'VegetationSrf', // Vegetation
    'AEE030':'PhysiographySrf', // Physiography
    'PFA012':'CulturePnt', // Culture
    'AFA012':'CultureSrf', // Culture
    'PFA015':'MilitaryPnt', // Military
    'AFA015':'MilitarySrf', // Military
    'AFA100':'MilitarySrf', // Military
    'PFA165':'MilitaryPnt', // Military
    'AFA165':'MilitarySrf', // Military
    'AFA210':'CultureSrf', // Culture
    'PGB005':'AeronauticPnt', // Aeronautic
    'AGB005':'AeronauticSrf', // Aeronautic
    'AGB015':'AeronauticSrf', // Aeronautic
    'PGB030':'AeronauticPnt', // Aeronautic
    'AGB030':'AeronauticSrf', // Aeronautic
    'PGB035':'AeronauticPnt', // Aeronautic
    'AGB035':'AeronauticSrf', // Aeronautic
    'PGB040':'AeronauticPnt', // Aeronautic
    'AGB040':'AeronauticSrf', // Aeronautic
    'AGB045':'AeronauticSrf', // Aeronautic
    'LGB050':'MilitaryCrv', // Military
    'AGB055':'AeronauticSrf', // Aeronautic
    'PGB065':'AeronauticPnt', // Aeronautic
    'AGB065':'AeronauticSrf', // Aeronautic
    'AGB070':'AeronauticSrf', // Aeronautic
    'LGB075':'AeronauticCrv', // Aeronautic
    'AGB075':'AeronauticSrf', // Aeronautic
    'PGB230':'AeronauticPnt', // Aeronautic
    'AGB230':'AeronauticSrf', // Aeronautic
    'PGB250':'AeronauticPnt', // Aeronautic
    'AGB250':'AeronauticSrf', // Aeronautic
    'AIA040':'BoundarySrf', // Boundary
    'PSU001':'MilitaryPnt', // Military
    'ASU001':'MilitarySrf', // Military
    'ASU004':'MilitarySrf', // Military
    'LSU030':'MilitaryCrv', // Military
    'PZB030':'BoundaryPnt', // Boundary
    'PZB050':'HypsographyPnt', // Hypsography
    'AZD020':'InformationSrf', // Information
    'PZD040':'InformationPnt', // Information
    'PZD045':'InformationPnt', // Information
    'LZD045':'InformationCrv', // Information
    'AZD045':'InformationSrf', // Information
    'PZD070':'HydrographyPnt', // Hydrography
    'AZD070':'HydrographySrf', // Hydrography
    'AZI031':'ResourceSrf', // Resource
    'AZI039':'MetadataSrf' // Metadata
    }, // End of thematicGroupList
    // ##### End of Thematic Group Rules #####

    // ##### Start of ESRI FCSubtype Rules #####
    subtypeList : {


    'AA010':'100001', // Extraction Mine
    'AA011':'100002', // Shear Wall
    'AA020':'100003', // Mine Shaft Superstructure
    'AA040':'100004', // Rig
    'AA045':'100696', // Borehole
    'AA052':'100006', // Hydrocarbons Field
    'AA054':'155023', // Non-water Well
    'AB000':'100007', // Disposal Site
    'AB010':'100008', // Recycling Site
    'AB021':'100009', // Liquid Diffuser
    'AB040':'100010', // Aeration Basin
    'AB507':'177962', // Waste Heap
    'AC010':'100012', // Blast-furnace
    'AC020':'100013', // Catalytic Cracker
    'AC030':'100014', // Settling Pond
    'AC040':'100015', // Hydrocarbon Products Facility
    'AC060':'100016', // Industrial Furnace
    'AC507':'134665', // Sewage Treatment Plant
    'AD010':'100018', // Electric Power Station
    'AD020':'100019', // Solar Panel
    'AD025':'100020', // Solar Farm
    'AD030':'100021', // Power Substation
    'AD041':'100022', // Nuclear Reactor Containment
    'AD050':'100023', // Heating Facility
    'AD055':'100697', // Cooling Facility
    'AD060':'100687', // Wind Farm
    'AF010':'100025', // Smokestack
    'AF020':'100026', // Conveyor
    'AF030':'100028', // Cooling Tower
    'AF040':'100029', // Crane
    'AF050':'100030', // Excavating Machine
    'AF060':'100031', // Engine Test Cell
    'AF070':'100032', // Flare Pipe
    'AF080':'100033', // Hopper
    'AG030':'100034', // Shopping Complex
    'AG040':'100035', // Office Park
    'AG050':'121638', // Billboard
    'AH025':'132596', // Engineered Earthwork
    'AH055':'132626', // Fortified Building
    'AH060':'100038', // Underground Bunker
    'AH070':'100039', // Checkpoint
    'AI020':'100040', // Caravan Park
    'AI021':'133168', // Manufactured Home Park
    'AI030':'100041', // Camp
    'AJ030':'100043', // Holding Pen
    'AJ050':'100044', // Windmill
    'AJ051':'100045', // Wind Turbine
    'AJ055':'100046', // Water Mill
    'AJ080':'100049', // Stable
    'AJ085':'100691', // Barn
    'AJ110':'100052', // Greenhouse
    'AK020':'100053', // Amusement Park Attraction
    'AK030':'100054', // Amusement Park
    'AK040':'100055', // Sports Ground
    'AK060':'100057', // Camp-site
    'AK061':'100058', // Picnic Site
    'AK070':'100059', // Drive-in Theatre
    'AK080':'100060', // Outdoor Theatre Screen
    'AK090':'100061', // Fairground
    'AK100':'100062', // Golf Course
    'AK101':'100063', // Golf Driving Range
    'AK110':'100064', // Grandstand
    'AK120':'100065', // Park
    'AK121':'100066', // Lookout
    'AK130':'100069', // Racetrack
    'AK150':'100072', // Ski-jump
    'AK155':'100073', // Ski-run
    'AK160':'154703', // Stadium
    'AK161':'121747', // Scoreboard
    'AK164':'100074', // Amphitheatre
    'AK170':'100077', // Swimming Pool
    'AK180':'100078', // Zoo
    'AL010':'100080', // Facility
    'AL011':'100081', // Installation
    'AL012':'100082', // Archeological Site
    'AL013':'100083', // Building
    'AL014':'100084', // Non-building Structure
    'AL017':'100086', // Fire Hydrant
    'AL018':'100087', // Building Superstructure
    'AL019':'100088', // Shed
    'AL020':'100089', // Built-up Area
    'AL025':'100091', // Cairn
    'AL030':'100092', // Cemetery
    'AL036':'100094', // Tomb
    'AL060':'100096', // Dragon's Teeth
    'AL065':'100097', // Minefield
    'AL070':'100098', // Fence
    'AL073':'100099', // Flagpole
    'AL080':'100101', // Gantry
    'AL099':'100103', // Hut
    'AL105':'100104', // Settlement
    'AL110':'100105', // Light Support Structure
    'AL120':'100106', // Missile Site
    'AL130':'100108', // Memorial Monument
    'AL140':'100110', // Particle Accelerator
    'AL142':'100111', // Astronomical Observatory
    'AL155':'100112', // Overhead Obstruction
    'AL165':'100113', // Pipeline Crossing Point
    'AL170':'100114', // Public Square
    'AL175':'100688', // Courtyard
    'AL180':'100689', // Retail Stand
    'AL195':'100115', // Ramp
    'AL200':'100116', // Ruins
    'AL201':'100117', // Interest Site
    'AL208':'100118', // Shanty Town
    'AL211':'130921', // Transportation Route Protection Structure
    'AL241':'100122', // Tower
    'AL250':'100123', // Underground Dwelling
    'AL260':'100124', // Wall
    'AL270':'100129', // Industrial Farm
    'AL351':'100126', // Space Facility
    'AL371':'180086', // Manor House
    'AL375':'100128', // Castle
    'AL376':'132642', // Castle Complex
    'AL510':'100130', // Tethered Balloon
    'AM010':'100131', // Storage Depot
    'AM011':'100132', // Shipping Container
    'AM020':'100133', // Grain Storage Structure
    'AM030':'100134', // Grain Elevator
    'AM040':'100136', // Mineral Pile
    'AM060':'100137', // Surface Bunker
    'AM065':'100138', // Munition Storage Facility
    'AM070':'100139', // Storage Tank
    'AM071':'100140', // Tank Farm
    'AM075':'100141', // Fuel Storage Facility
    'AM080':'100142', // Water Tower
    'AN010':'100143', // Railway
    'AN050':'100144', // Railway Sidetrack
    'AN060':'100145', // Railway Yard
    'AN075':'100146', // Railway Turntable
    'AN076':'100147', // Roundhouse
    'AN085':'100149', // Railway Signal
    'AP010':'100150', // Cart Track
    'AP020':'100151', // Road Interchange
    'AP030':'100152', // Road
    'AP033':'179969', // Engineered Turnaround Site
    'AP040':'100154', // Gate
    'AP041':'100155', // Vehicle Barrier
    'AP050':'100156', // Trail
    'AP055':'179906', // Tank Trail
    'AP056':'180006', // Tank Crossing
    'AQ035':'100159', // Sidewalk
    'AQ040':'100161', // Bridge
    'AQ045':'100162', // Bridge Span
    'AQ050':'100163', // Bridge Superstructure
    'AQ055':'100164', // Bridge Tower
    'AQ056':'100165', // Bridge Pier
    'AQ059':'131083', // Transportation Route Characteristic Change
    'AQ060':'100167', // Control Tower
    'AQ062':'100168', // Crossing
    'AQ063':'130381', // Causeway Structure
    'AQ065':'100170', // Culvert
    'AQ068':'100171', // Transportation Block
    'AQ070':'100172', // Ferry Crossing
    'AQ075':'100173', // Ice Route
    'AQ080':'100174', // Ferry Station
    'AQ095':'100176', // Tunnel Mouth
    'AQ110':'100177', // Mooring Mast
    'AQ111':'100178', // Prepared Watercourse Crossing
    'AQ113':'100179', // Pipeline
    'AQ114':'100180', // Storm Drain
    'AQ115':'100181', // Utility Cover
    'AQ116':'100182', // Pumping Station
    'AQ118':'100183', // Sharp Curve
    'AQ120':'100185', // Steep Grade
    'AQ125':'100186', // Transportation Station
    'AQ130':'100187', // Tunnel
    'AQ135':'100188', // Roadside Rest Area
    'AQ140':'100189', // Vehicle Lot
    'AQ141':'100190', // Parking Garage
    'AQ150':'100191', // Stair
    'AQ151':'100192', // Arcade
    'AQ160':'100194', // Traffic Light
    'AQ161':'100195', // Street Lamp
    'AQ162':'100196', // Street Sign
    'AQ170':'100197', // Motor Vehicle Station
    'AT005':'100199', // Cable
    'AT010':'100200', // Dish Aerial
    'AT011':'100201', // Aerial
    'AT012':'100202', // Aerial Farm
    'AT041':'100206', // Cableway
    'AT042':'100558', // Pylon
    'AT045':'100207', // Radar Station
    'BA010':'100212', // Land Water Boundary
    'BA030':'100217', // Island
    'BA040':'100218', // Tidal Water
    'BB005':'100222', // Harbour
    'BB009':'100223', // Port
    'BB081':'100231', // Shoreline Construction
    'BB082':'100232', // Shoreline Ramp
    'BB090':'100233', // Dry Dock
    'BB110':'100236', // Fish Weir
    'BB199':'100243', // Floating Dry Dock
    'BB201':'100244', // Small Craft Facility
    'BB241':'100245', // Shipyard
    'BC050':'100253', // Lighthouse
    'BC070':'100256', // Light Vessel
    'BD100':'100271', // Structural Pile
    'BD115':'100272', // Offshore Construction
    'BD140':'100277', // Snag
    'BD181':'100279', // Hulk
    'BH010':'100295', // Aqueduct
    'BH012':'131749', // Qanat Shaft
    'BH015':'100296', // Bog
    'BH020':'100297', // Canal
    'BH030':'100298', // Ditch
    'BH040':'100299', // Water Treatment Bed
    'BH051':'191951', // Fish Farm Facility
    'BH065':'131810', // Water Race
    'BH070':'100302', // Ford
    'BH075':'100303', // Fountain
    'BH077':'100304', // Hummock
    'BH082':'130384', // Inland Waterbody
    'BH090':'100307', // Land Subject to Inundation
    'BH100':'100309', // Moat
    'BH110':'100310', // Penstock
    'BH116':'154640', // Aquifer
    'BH120':'100311', // Rapids
    'BH135':'100313', // Rice Field
    'BH140':'100314', // River
    'BH145':'100315', // Vanishing Point
    'BH150':'100316', // Salt Flat
    'BH155':'100317', // Salt Evaporator
    'BH160':'100318', // Sabkha
    'BH165':'100319', // Spillway
    'BH170':'100320', // Natural Pool
    'BH180':'100321', // Waterfall
    'BH220':'100325', // Waterwork
    'BH230':'100326', // Water Well
    'BI005':'100328', // Vessel Lift
    'BI006':'132749', // Ship Elevator
    'BI010':'100329', // Cistern
    'BI020':'100330', // Dam
    'BI030':'100331', // Lock
    'BI040':'100334', // Sluice Gate
    'BI044':'131207', // Flood Control Structure
    'BI045':'131206', // Basin Gate
    'BI050':'100337', // Water Intake Tower
    'BI060':'100338', // Fish Ladder
    'BI070':'100339', // Gauging Station
    'BJ020':'100340', // Moraine
    'BJ030':'100341', // Glacier
    'BJ031':'100342', // Crevasse
    'BJ040':'100343', // Ice Cliff
    'BJ060':'100344', // Ice Peak
    'BJ065':'100345', // Ice Shelf
    'BJ080':'100347', // Polar Ice
    'BJ099':'100348', // Ice-cap
    'BJ100':'100349', // Snow Field and/or Ice-field
    'BJ110':'100350', // Tundra
    'CA010':'100353', // Elevation Contour
    'CA030':'100355', // Spot Elevation
    'DA005':'100356', // Asphalt Lake
    'DA010':'100358', // Soil Surface Region
    'DB010':'100362', // Steep Terrain Face
    'DB028':'154959', // Cave Chamber
    'DB029':'154961', // Cave Mouth
    'DB061':'100365', // Crevice
    'DB070':'100366', // Cut
    'DB071':'192101', // Cut Line
    'DB080':'100367', // Depression
    'DB090':'100368', // Embankment
    'DB100':'100369', // Esker
    'DB110':'100370', // Geologic Fault
    'DB115':'100371', // Geothermal Outlet
    'DB150':'100372', // Mountain Pass
    'DB160':'100373', // Rock Formation
    'DB170':'100374', // Sand Dunes
    'DB180':'100375', // Volcano
    'DB190':'100377', // Volcanic Dyke
    'DB211':'100379', // Landslide Mass
    'EA010':'100380', // Crop Land
    'EA020':'100381', // Hedgerow
    'EA030':'100382', // Plant Nursery
    'EA031':'100383', // Botanic Garden
    'EA040':'100384', // Orchard
    'EA050':'100385', // Vineyard
    'EA055':'100386', // Hop Field
    'EB010':'100387', // Grassland
    'EB020':'100388', // Thicket
    'EB070':'100390', // Brush
    'EC005':'100392', // Tree
    'EC010':'100393', // Cane
    'EC015':'130380', // Forest
    'EC020':'100394', // Oasis
    'EC040':'100396', // Cleared Way
    'EC060':'100398', // Forest Clearing
    'ED010':'100399', // Marsh
    'ED020':'100400', // Swamp
    'EE010':'100401', // Logging Site
    'EE030':'100403', // Desert
    'FA012':'100409', // Contaminated Region
    'FA015':'100410', // Firing Range
    'FA100':'100414', // Test Site
    'FA165':'100416', // Training Site
    'FA210':'100417', // Conservation Area
    'GB005':'100436', // Land Aerodrome
    'GB015':'100438', // Apron
    'GB030':'100441', // Helipad
    'GB035':'100442', // Heliport
    'GB040':'100443', // Launch Pad
    'GB045':'100444', // Stopway
    'GB050':'100446', // Defensive Revetment
    'GB055':'100448', // Runway
    'GB065':'100452', // Water Aerodrome
    'GB070':'100453', // Seaplane Run
    'GB075':'100454', // Taxiway
    'GB230':'100456', // Aircraft Hangar
    'GB250':'100457', // Hardened Aircraft Shelter
    'IA040':'100458', // Land Parcel
    'SU001':'100462', // Military Installation
    'SU004':'170162', // Cantonment Area
    'SU030':'180061', // Installation Boundary
    'ZB030':'100465', // Boundary Monument
    'ZB050':'177997', // Survey Point
    'ZD020':'100473', // Void Collection Area
    'ZD040':'100475', // Named Location
    'ZD045':'100476', // Annotated Location
    'ZD070':'164755', // Water Measurement Location
    'ZI031':'121591', // Dataset
    'ZI039':'132721', // Entity Collection Metadata
    }, // End of subtypeList
    // ##### End of ESRI FCSubtype Rules #####

} // End of tds70.rules
