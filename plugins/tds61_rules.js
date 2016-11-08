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
    TDSv61 One2one rules
*/

tds61.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    // This is where anything "unique" to TDS goes.
    fcodeOne2oneIn : [
     // ['F_CODE','AP030','highway','road'], // Road
     ], // End fcodeOne2oneIn

    // These are for finding an FCODE for export.
    // Note: These get swapped around before being used
    fcodeOne2oneOut : [
     ['F_CODE','AA010','landuse','quarry'], // Extraction Mine
     ['F_CODE','AK120','leisure','playground'], // Park
     ['F_CODE','AK120','landuse','recreation_ground'], // Park
     ['F_CODE','AK120','leisure','common'], // Park - close but not great
     ['F_CODE','AL020','landuse','residential'], // From OSM - Map to BUA
     ['F_CODE','AL020','landuse','industrial'], // From OSM - Map to BUA
     ['F_CODE','AL030','amenity','grave_yard'], // From OSM Data
     ['F_CODE','AL130','historic','memorial'], // From OSM data, not great
     ['F_CODE','AL200','historic','ruins'], // Ruins
     ['F_CODE','AL241','tower','communication'],
     ['F_CODE','AL241','tower','yes'],
     ['F_CODE','AM080','tower:type','water'],
//      ['F_CODE','AN060','railway','yes'], // Rail yards
     ['F_CODE','AP030','highway','motorway'], // Limited Access Motorway
     ['F_CODE','AP030','highway','motorway_link'], // Limited Access Motorway
     ['F_CODE','AP030','highway','trunk'], // Motorway
     ['F_CODE','AP030','highway','trunk_link'], // Motorway
     ['F_CODE','AP030','highway','primary'],
     ['F_CODE','AP030','highway','primary_link'],
     ['F_CODE','AP030','highway','secondary'],
     ['F_CODE','AP030','highway','secondary_link'],
     ['F_CODE','AP030','highway','tertiary'],
     ['F_CODE','AP030','highway','tertiary_link'],
     ['F_CODE','AP030','highway','residential'],
     ['F_CODE','AP030','highway','unclassified'],
     ['F_CODE','AP030','highway','service'],
     ['F_CODE','AP030','highway','yes'], // Just in case...
     ['F_CODE','AP040','railway','level_crossing'], // Level Crossing - See custom rules
     ['F_CODE','AP040','railway','crossing'], // Pedestrian Crossing - See custom rules
     ['F_CODE','AP040','barrier','lift_gate'], // Gate
     ['F_CODE','AP050','highway','footway'], // Trail
     ['F_CODE','AQ125','railway','station'], // Railway Station
     ['F_CODE','AQ125','highway','bus_stop'], // Transportaion Station
     ['F_CODE','AQ125','highway','bus_station'], // Transportaion Station
     ['F_CODE','AQ125','public_transport','stop_position'], 
     ['F_CODE','AQ141','parking','multi-story'], // Parking Garage
     ['F_CODE','AT042','power','pole'], // OSM
     ['F_CODE','AT042','power','tower'], // OSM
     ['F_CODE','BA030','place','islet'], // Island - From OSM
     ['F_CODE','BH140','waterway','yes'], // Make unknown waterways into rivers
     ['F_CODE','BH145','natural','sinkhole'], 
     // ['F_CODE','EA010','landuse','farm'], // Alt version of "landuse=farmland"
     // ['F_CODE','EC015','natural','wood'], // Alt version of Forest
     ['F_CODE','GB005','aeroway','sport_airport'], // Land Aerodrome
     ['F_CODE','ZD040','natural','hill'], // From GeoNames
     ['F_CODE','ZD040','natural','valley'], // From GeoNames
     ['F_CODE','ZD040','natural','cape'], // From OSM
     ['F_CODE','ZD040','natural','peak'], // From OSM
     ], // End fcodeOne2oneOut

    // One2one rules for Text Fields
    txtBiased : {
     'ADR':'addr:full', // Address
     'AQN':'aquifer_name', // Aquifer Name
     'BA000_VDR':'source:datum:sounding:name', // Water Line : Sounding Datum Name
     'BEN':'be_number', // Basic Encyclopedia (BE) Number
     'BRN':'bridge:reference', // Bridge Reference Number
     'CCN':'source:copyright', // Commercial Copyright Notice
     'CDM':'source:cadastral:measurement', // Cadastral Source Measurement
     'CDR':'source:commercial_distribution_restriction', // Commercial Distribution Restriction
     'CID':'source:cell_identifier', // Cell Identifier
     'CSI':'source:cadastral:identifier', // Cadastral Source Identifier
     'CSY':'source:cadastral:identifier:type', // Cadastral Source Identifier Type
     'CUD':'source:currency:datetime', // Currency Date and Time
     'DQS':'source:data_quality', // Data Quality Statement
     'ETZ':'source:extraction_specification:version', // Extraction Specification Version
//      'F_CODE':'raw:F_CODE', // Feature Code
     'GB052_RIDH':'aeroway:runway_designator_high', // Runway Direction (high end) : Runway Designator (high)
     'GB052_RIDL':'aeroway:runway_designator_low', // Runway Direction (low end) : Runway Designator (low)
     'IKO':'icao', // ICAO Location Indicator
     'LINK_ID':'etds:link_id', // eLTDS40 Field
     'MDE':'source:update:datetime', // Maintenance Date and Time
     'NA8':'official_name', // Aerodrome Official Name
     'NAO':'installation_operator_name', // Installation Operator
     'OTH':'note:oth', // Specified Domain Value(s)
     'PDA':'source:dfdd_accessor', // Property DFDD-compatible Accessor
     'PSE':'restriction_interval', // Periodic Restriction Month Interval
     'RIN_RTN2':'ref:2', // Route Identification [2] <route designation>
     'RIN_RTN3':'ref:3', // Route Identification [3] <route designation>
     'RIN_RTN':'ref', // Route Identification <route designation>
     'SAX_RS1':'security:classification:reason', // Security Attributes Group <resource classification reason>
     'SAX_RS2':'security:classification:classified_by', // Security Attributes Group <resource classified by>
     'SAX_RS3':'security:exempted_source:date', // Security Attributes Group <resource date of exempted source>
     'SAX_RS4':'security:declassification:date', // Security Attributes Group <resource declassification date>
     'SAX_RS5':'security:declassification:event', // Security Attributes Group <resource declassification event>
     'SAX_RS8':'security:classification:derived_by', // Security Attributes Group <resource derivatively classified by>
     'SAX_RS9':'security:classification:derived_from', // Security Attributes Group <resource derived from>
     'SAX_RX1':'security:foreign_government_information:open_source', // Security Attributes Group <resource foreign government information (open source)>
     'SAX_RX2':'security:foreign_government_information:protected_source', // Security Attributes Group <resource foreign government information (protected source)>
     'SAX_RX5':'security:releasability', // Security Attributes Group <resource releasable to>
     'SAX_RX6':'security:classification:sar', // Security Attributes Group <resource special-access-required program identifier>
     'SAX_RX7':'security:classification:sci', // Security Attributes Group <resource SCI controls>
     'SAX_RX9':'security:classification:compilation_reason', // Security Attributes Group <resource compilation reason>
     'SAX_RY0':'security:display_only_to', // Security Attributes Group <resource display only to>
     'SAX_RY1':'security:classification:sci:non_us', // Security Attributes Group <resource non-US controls>
     'SAX_RY2':'security:atomic_markings', // Security Attributes Group <resource atomic energy markings>
     'SSE':'seasonal_ice_limit', // Seasonal Ice Limit
     'UFI':'uuid', // Unique Feature Identifier. This gets cleaned up in post processing
     'URI':'source_ref', // Unique Resource Identifier
     'VCS_VCR':'max_clearance:note', // Vertical Clearance, Safe <reference level remark>
     'VOI':'vertical_obstruction_identifier', // Vertical Obstruction Identifier
     'WAD':'well:description', // Water Well Construction Description
     'WPI':'world_port_index', // World Port Index Identifier - This is part of Built Up Area. What??
     //'ZI001_SDP':'source:description', // Source Information : Source Description
     'ZI001_SDP':'source', // Source Information : Source Description
     'ZI001_SDV':'source:datetime', // Source Information : Source Date and Time
     'ZI001_VSD':'source:vertical_source:datetime', // Source Information : Vertical Source Date and Time
//      'ZI001_VSN':'source:vertical_source:description', // Source Information : Vertical Source Description
     'CCN':'source:copyright', // Restriction Information : Commercial Copyright Notice
     'CDR':'source:commercial_distribution_restriction', // Restriction Information : Commercial Distribution Restriction
     'ZI002_UFI':'security:restriction_ufi', // Restriction Information : Unique Entity Identifier
     'ZI004_PRE':'source:processing_information', // Process Step Information : Process Step Description
     'ZI005_FNA':'name', // Geographic Name Information : Full Name
     'ZI005_FNA2':'alt_name', // Geographic Name Information : (second) Full Name
     'ZI005_FNA3':'alt_name:2', // Geographic Name Information : Full Name (third)
     'ZI005_NFN':'gndb_id', // Geographic Name Information : Name Identifier
     'ZI005_NFN2':'gndb_id:2', // Geographic Name Information : (second) Name Identifier
     'ZI005_NFN3':'gndb_id:3', // Geographic Name Information : Name Identifier (third)
     'ZI006_MEM':'note', // Note : Memorandum
// We are going to look at valid values for the first country designation and then just treat the rest as plain text fields
//     'ZI020_GE4':'raw:ZI020_GE4', // (Location Country) Designation : GENC Short URN-based Identifier
     'ZI020_GE42':'country_code:second', // (Location Country) Designation : GENC Short URN-based Identifier (second)
     'ZI020_GE43':'country_code:third', // (Location Country) Designation : GENC Short URN-based Identifier (third)
     'ZI020_GE44':'country_code:fourth', // (Location Country) Designation : GENC Short URN-based Identifier (fourth)
     'ZI026T_UFI':'source:table:uuid', // Feature Metadata (Table) : Unique Entity Identifier
     'ZI027T_UFI':'source:attribute:uuid', // Feature Attribute Metadata (Table) : Unique Entity Identifier
     'ZI028_UFI':'source:feature:uuid', // Feature Entity : Unique Entity Identifier
     'ZI039S_UFI':'source:entity:surface:uuid', // Entity Collection Metadata (Surface) : Unique Entity Identifier
     'ZI039T_UFI':'source:entity:table:uuid', // Entity Collection Metadata (Table) : Unique Entity Identifier
     'ZI031S_URI':'source:dataset:surface:uuid', // Dataset (Surface) : Unique Resource Identifier
     'ZI031T_URI':'source:dataset:table:uuid', // Dataset (Table) : Unique Resource Identifier
     'ZSAX_RX0':'security:dissemination_control:ic', // Restriction Information : Security Attributes Group <resource dissemination controls>
     'ZSAX_RX3':'security:dissemination_control:non_ic', // Restriction Information : Security Attributes Group <resource non-intelligence community markings>
     'ZSAX_RX4':'security:resource_owner', // Restriction Information : Security Attributes Group <resource owner-producer>
     }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
     'AHA':'source:accuracy:horizontal', // Absolute Horizontal Accuracy (90%)
     'AOO':'angle', // Angle of Orientation
     'AQTL':'aquifer:thickness:lower', // Aquifer Thickness <lower value>
     'AQTU':'aquifer:thickness:upper', // Aquifer Thickness <upper value>
     'ARA':'feature_area', // Area - this has been moved from 'area' due to hoot conflicts
     'AVA':'source:accuracy:vertical', // Absolute Vertical Accuracy (90%)
     'AYRL':'aquifer:yield:lower', // Aquifer Yield Rating <lower value>
     'AYRU':'aquifer:yield:upper', // Aquifer Yield Rating <upper value>
     'BC040_EOL':'seamark:light:range', // Maritime Navigation Light : Nominal Range
     'BC040_LCN':'seamark:light:characteristic_number', // Maritime Navigation Light : Light Characteristic Number
     'BC040_LVN':'seamark:light:height', // Maritime Navigation Light : Light Elevation
     'BC040_PER':'seamark:light:period', // Maritime Navigation Light : Period of Light
     'BEL':'ele:base', // Base Elevation - conflict with "ele" but is only used in one feature: ZB035 Control Point
     'BH141_AWBA':'waterway:bank1:slope:above_water', // Inland Waterbody Bank (1) : Above Water Bank Slope (first bank)
     'BH141_AWBB':'waterway:bank2:slope:above_water', // Inland Waterbody Bank (2) : Above Water Bank Slope (second bank)
     'BH141_WBHA':'waterway:bank1:height', // Inland Waterbody Bank (1) : Waterbody Bank Height (first bank)
     'BH141_WBHB':'waterway:bank2:height', // Inland Waterbody Bank (2) : Waterbody Bank Height (second bank)
     'BNF':'building:levels', // Floor Count
     'BPWHAL':'waterway:bank1:height:lower', // Inland Waterbody Bank (1) : Predominant Waterbody Bank Height (first bank) (lower value)
     'BPWHAU':'waterway:bank1:height:upper', // Inland Waterbody Bank (1) : Predominant Waterbody Bank Height (first bank) (upper value)
     'BPWHBL':'waterway:bank2:height:lower', // Inland Waterbody Bank (2) : Predominant Waterbody Bank Height (second bank) (lower value)
     'BPWHBU':'waterway:bank2:height:upper', // Inland Waterbody Bank (2) : Predominant Waterbody Bank Height (second bank) (upper value)
     'BPWSAL':'waterway:bank1:slope:lower', // Inland Waterbody Bank (1) : Predominant Waterbody Bank Slope (first bank) (lower value)
     'BPWSAU':'waterway:bank1:slope:upper', // Inland Waterbody Bank (1) : Predominant Waterbody Bank Slope (first bank) (upper value)
     'BPWSBL':'waterway:bank2:slope:lower', // Inland Waterbody Bank (2) : Predominant Waterbody Bank Slope (second bank) (lower value)
     'BPWSBU':'waterway:bank2:slope:upper', // Inland Waterbody Bank (2) : Predominant Waterbody Bank Slope (second bank) (upper value)
     'BWVCAL':'waterway:bank1:cover:lower', // Inland Waterbody Bank (1) : Waterbody Bank Vegetation Cover (first bank) (lower value)
     'BWVCAU':'waterway:bank1:cover:upper', // Inland Waterbody Bank (1) : Waterbody Bank Vegetation Cover (first bank) (upper value)
     'BWVCBL':'waterway:bank2:cover:lower', // Inland Waterbody Bank (2) : Waterbody Bank Vegetation Cover (second bank) (lower value)
     'BWVCBU':'waterway:bank2:cover:upper', // Inland Waterbody Bank (2) : Waterbody Bank Vegetation Cover (second bank) (upper value)
     'CDL':'covered_drain:length', // Covered Drain Length
     'CTL':'railway:track:length', // Cumulative Track Length
     'DEV':'level', // Deck Level
     'DIM':'diameter', // Diameter
     'DMBL':'undergrowth:density:lower', // Undergrowth Density (lower value)
     'DMBU':'undergrowth:density:upper', // Undergrowth Density (upper value)
     'DMT':'canopy_cover', // Canopy Cover
     'DOF':'flow_direction', // Direction of Flow
     'DPAL':'aquifer:depth:lower', // Aquifer Depth <lower value>
     'DPAU':'aquifer:depth:upper', // Aquifer Depth <upper value>
     'DZC':'deck_count', // Deck Count
     'DZP':'depth:maximum_below_surface', // Deepest Depth Below Surface Level
     'EPW':'generator:output:electricity', // Electrical Power Generation Capacity
     'EVA':'source:accuracy:elevation', // Elevation Vertical Accuracy (90%)
     'FCL':'ferry:crossing_distance', // Ferry Crossing Distance
//     'FCSUBTYPE':'etds:fcsubtype', // Very ESRI Specific. Ignored for now
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
     'LCA':'crane:lifting_capacity', // Lifting Capacity
     'LDC':'length:crest', // Dam Crest Length
     'LEA':'depth:minimum_below_surface', // Least Depth Below Surface Level
     'LNU':'length:interior_useable', // Usable Length
     'LOG':'gradient:length', // Gradient Length
     'LTN':'lanes', // Track or Lane Count
     'LZN':'length', // Lengthf
     'MAG':'magnetic_variation', // Magnetic Variation
     'MWD':'depth:designed:maximum', // Maximum Design Water Depth
     'MWG':'divider:width', // Centerline Spacing
     'NOS':'bridge:span_count', // Span Count
     'NPL':'cables', // Parallel Line Count: See AT005
     'OHC':'max_clearance', // Overhead Clearance
     'PFH':'height:feature', // Predominant Feature Height
     'PVH':'height:vegetation', // Predominant Vegetation Height
     'PWA':'depth', // Predominant Water Depth
     'PWAL':'depth:lower', // Predominant Water Depth <lower value>
     'PWAU':'depth:upper', // Predominant Water Depth <upper value>
     'RAD':'curve_radius', // Curve Radius
     'RMWL':'median:min_width', // Route Median Width (lower value)
     'RMWU':'median:max_width', // Route Median Width (upper value)
     'SDCL':'soil:depth:lower', // Soil Depth <lower value>
     'SDCU':'soil:depth:upper', // Soil Depth <upper value>
     'SDO':'dune:orientation', // Sand Dune Orientation
     'SDSL':'tree:diameter:lower', // Stem Diameter (lower value)
     'SDSU':'tree:diameter:upper', // Stem Diameter (upper value)
     'SGCL':'incline:min', // Surface Slope (lower value)
     'SGCU':'incline:max', // Surface Slope (upper value)
     'SHC':'safe_clearance:horizontal', // Safe Horizontal Clearance
     'SPM':'maxspeed', // Speed Limit (KPH)
     'SWW':'water_level', // Static Water Level
     'THI':'thickness', // Thickness
     'TSCL':'tree:spacing:lower', // Tree Spacing (lower value)
     'TSCU':'tree:spacing:upper', // Tree Spacing (upper value)
     'UBC':'bridge:under_clearance', // Underbridge Clearance
     'VCS':'safe_clearance:vertical', // Vertical Clearance, Safe
     'VLM':'volume', // Volume
     'VTI':'trafficability_impact', // Vegetation Trafficability Impact
     'WD2':'width:total_usable', // Route Total Usable Width
     'WD3':'width:gap', // Terrain Gap Width
     'WD5':'width:top', // Width at Top
     'WDAL':'depth:average:lower', // Average Water Depth <lower value>
     'WDAU':'depth:average:upper', // Average Water Depth <upper value>
     'WDH':'depth:max', // Predominant Maximum Water Depth
     'WDL':'depth:min', // Predominant Minimum Water Depth
     'WDU':'width:interior_useable', // Usable Width
     'WID':'width', // Width
     'WOC':'width:crest', // Dam Crest Width
     'WT2':'width:second_way', // Width of Second Travelled Way
     'WVA':'water:velocity:average', // Predominant Average Water Velocity
     'WVH':'water:velocity:maximum', // Predominant Maximum Water Velocity
     'WVL':'water:velocity:minimum', // Predominant Minimum Water Velocity
     'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
     'ZI017_GAW':'gauge', // Track Information : Railway Gauge
     'ZI024_ASE':'water:arsnic_concentration', // Water Resource Information : Arsenic Concentration
     'ZI024_CFR':'water:coliform_concentration', // Water Resource Information : Coliform Concentration
     'ZI024_CLO':'water:chloride_concentration', // Water Resource Information : Chloride Concentration
     'ZI024_CYN':'water:cyanide_concentration', // Water Resource Information : Cyanide Concentration
     'ZI024_HAR':'water:hardness', // Water Resource Information : Water Hardness
     'ZI024_MGN':'water:magnesium_concentration', // Water Resource Information : Magnesium Concentration
     'ZI024_PHW':'water:ph', // Water Resource Information : pH
     'ZI024_SUL':'water:sulfate_concentration', // Water Resource Information : Sulfate Concentration
     'ZI024_TDS':'water:total_dissolved_solids', // Water Resource Information : Total Dissolved Solids
     'ZI024_TEP':'water:temperature', // Water Resource Information : Temperature
     'ZI024_TUR':'water:turbidity', // Water Resource Information : Nephelometric Turbidity
     'ZI024_WAC':'water:conductivity', // Water Resource Information : Water Conductivity
     'ZI026_CTUL':'cartographic_scale:lower', // Feature Metadata : Cartographic Usability Range <lower value>
     'ZI026_CTUU':'cartographic_scale:upper', // Feature Metadata : Cartographic Usability Range <upper value>
     'ZVA':'ele', // Aerodrome Elevation
     'ZVH_AVA':'source:accuracy:highest_elevation', // Highest Elevation <absolute vertical accuracy>
     'ZVH':'ele:max' // Highest Elevation
     }, // End numBiased
    

    // Common one2one rules. Used for both import and export
    // taken from TDSv60 with the NGA TDSv61 additions
    one2one : [
     // ['ADI','-999999',undefined,undefined], // No Information
     // ADI - Administrative Division - Admin_level may need some work
     ['ADI','1','admin_level','1'], // International or Supranational
     ['ADI','2','admin_level','2'], // National
     ['ADI','3','admin_level','4'], // Subnational
     ['ADI','4','admin_level','6'], // Local
     ['ADI','6','admin_level','8'], // Municipal
     ['ADI','999','admin_level','other'], // Other - Not great

     // AMA - Amusement Attraction Type
     // ['AMA','-999999',undefined,undefined], // No Information
     ['AMA','1','attraction','artificial_mountain'], // Artificial Mountain
     ['AMA','2','attraction','ferris_wheel'], // Ferris Wheel
     ['AMA','3','attraction','roller-coaster'], // Roller-coaster
     ['AMA','4','attraction','spherical'], // Spherical
     ['AMA','5','attraction','water_attraction'], // Water Attraction
     ['AMA','6','attraction','vertical_ride'], // Vertical Ride
     ['AMA','999','attraction','other'], // Other

          // APT - Airfield Use
     // ['APT','-999999',undefined,undefined], // No Information
     ['APT','1','aeroway:use','major_airfield'], // Major Airfield
     ['APT','2','aeroway:use','minor_airfield'], // Minor Airfield
     ['APT','3','aeroway:use','general_aviation_aircraft_operating_only'], // General Aviation Aircraft Operating Only
     ['APT','5','aeroway:use','glider_site'], // Glider Site
     ['APT','6','aeroway:use','ultralight_site'], // Ultralight Site
     ['APT','7','aeroway:use','hang_glider_site'], // Hang Glider Site
     ['APT','8','aeroway:use','winch_launched_hang_glider_site'], // Winch Launched Hang Glider Site
     ['APT','12','aeroway:use','emergency'], // Emergency
     ['APT','13','aeroway:use','parascending_site'], // Parascending Site
     ['APT','16','aeroway:use','search_and_rescue_airfield'], // Search and Rescue Airfield
     ['APT','999','aeroway:use','other'], // Other

     // APU - Apron Usage
     // ['APU','-999999',undefined,undefined], // No Information
     ['APU','1','apron:use','parking'], // Parking
     ['APU','2','apron:use','loading_and/or_unloading'], // Loading and/or Unloading
     ['APU','3','apron:use','fuelling'], // Fuelling
     ['APU','4','apron:use','transient'], // Transient
     ['APU','5','apron:use','compass_rose'], // Compass Rose
     ['APU','6','apron:use','engine_run_up'], // Engine Run Up
     ['APU','7','apron:use','maintenance'], // Maintenance
     ['APU','8','apron:use','alert'], // Alert
     ['APU','9','apron:use','firing-in'], // Firing-In
     ['APU','10','apron:use','hot_refuelling'], // Hot Refuelling
     ['APU','11','apron:use','weapon_loading'], // Weapon Loading
     ['APU','12','apron:use','de-icing_and/or_anti-icing'], // De-icing and/or Anti-icing
     ['APU','13','apron:use','ins_alignment'], // INS Alignment
     ['APU','14','apron:use','decontamination'], // Decontamination
     ['APU','999','apron:use','other'], // Other

     // APY - Apron Type
     // ['APY','-999999',undefined,undefined], // No Information
     ['APY','1','apron:type','passenger'], // Passenger
     ['APY','2','apron:type','cargo'], // Cargo
     ['APY','3','apron:type','remote_parking'], // Remote Parking
     ['APY','4','apron:type','services_and/or_hangar'], // Services and/or Hangar
     ['APY','5','apron:type','general_aviation'], // General Aviation
     ['APY','6','apron:type','military'], // Military
     ['APY','7','apron:type','dispersal'], // Dispersal
     ['APY','8','apron:type','holding'], // Holding
     ['APY','999','apron:type','other'], // Other

     // AQO - Aquifer Overburden
     // ['AQO','-999999',undefined,undefined], // No Information
     ['AQO','1','aquifer:overburden','basalt'], // Basalt
     ['AQO','2','aquifer:overburden','boulders'], // Boulders
     ['AQO','3','aquifer:overburden','clay'], // Clay
     ['AQO','4','aquifer:overburden','conglomerate'], // Conglomerate
     ['AQO','5','aquifer:overburden','dolomite'], // Dolomite
     ['AQO','6','aquifer:overburden','granite'], // Granite
     ['AQO','7','aquifer:overburden','gravel'], // Gravel
     ['AQO','8','aquifer:overburden','igneous_rock'], // Igneous Rock
     ['AQO','9','aquifer:overburden','karst'], // Karst
     ['AQO','10','aquifer:overburden','limestone'], // Limestone
     ['AQO','11','aquifer:overburden','marl'], // Marl
     ['AQO','12','aquifer:overburden','metamorphic_rock'], // Metamorphic Rock
     ['AQO','13','aquifer:overburden','mud'], // Mud
     ['AQO','14','aquifer:overburden','sand'], // Sand
     ['AQO','15','aquifer:overburden','schist'], // Schist
     ['AQO','16','aquifer:overburden','unconsolidated'], // Unconsolidated
     ['AQO','17','aquifer:overburden','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['AQO','18','aquifer:overburden','volcanic_rock'], // Volcanic Rock
     ['AQO','19','aquifer:overburden','sandstone'], // Sandstone
     ['AQO','20','aquifer:overburden','shale'], // Shale
     ['AQO','999','aquifer:overburden','other'], // Other

     // AQP - Aquifer Composition
     // ['AQP','-999999',undefined,undefined], // No Information
     ['AQP','1','aquifer:composition','basalt'], // Basalt
     ['AQP','2','aquifer:composition','boulders'], // Boulders
     ['AQP','3','aquifer:composition','clay'], // Clay
     ['AQP','4','aquifer:composition','conglomerate'], // Conglomerate
     ['AQP','5','aquifer:composition','dolomite'], // Dolomite
     ['AQP','6','aquifer:composition','granite'], // Granite
     ['AQP','7','aquifer:composition','gravel'], // Gravel
     ['AQP','8','aquifer:composition','igneous_rock'], // Igneous Rock
     ['AQP','9','aquifer:composition','karst'], // Karst
     ['AQP','10','aquifer:composition','limestone'], // Limestone
     ['AQP','11','aquifer:composition','marl'], // Marl
     ['AQP','12','aquifer:composition','metamorphic_rock'], // Metamorphic Rock
     ['AQP','13','aquifer:composition','mud'], // Mud
     ['AQP','14','aquifer:composition','sand'], // Sand
     ['AQP','15','aquifer:composition','schist'], // Schist
     ['AQP','16','aquifer:composition','unconsolidated'], // Unconsolidated
     ['AQP','17','aquifer:composition','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['AQP','18','aquifer:composition','volcanic_rock'], // Volcanic Rock
     ['AQP','19','aquifer:composition','sandstone'], // Sandstone
     ['AQP','20','aquifer:composition','shale'], // Shale
     ['AQP','999','aquifer:composition','other'], // Other

     // AQTC - Aquifer Thickness <interval closure>
     ['AQTC','2','aquifer:thickness:closure','open_interval'], // Open Interval
     ['AQTC','3','aquifer:thickness:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['AQTC','4','aquifer:thickness:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['AQTC','5','aquifer:thickness:closure','closed_interval'], // Closed Interval
     ['AQTC','6','aquifer:thickness:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['AQTC','7','aquifer:thickness:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['AQTC','8','aquifer:thickness:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['AQTC','9','aquifer:thickness:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // ASU - Aerodrome Movement Area Surface Composition - See ZI019_ASU
     // ['ASU','-999999',undefined,undefined], // No Information
//      ['ASU','1','raw:ASU','asphalt'], // Asphalt
//      ['ASU','2','raw:ASU','bituminous_mix'], // Bituminous Mix
//      ['ASU','3','raw:ASU','brick'], // Brick
//      ['ASU','4','raw:ASU','clay'], // Clay
//      ['ASU','5','raw:ASU','concrete'], // Concrete
//      ['ASU','6','raw:ASU','coral'], // Coral
//      ['ASU','7','raw:ASU','earthen'], // Earthen
//      ['ASU','8','raw:ASU','gravel'], // Gravel
//      ['ASU','9','raw:ASU','ice'], // Ice
//      ['ASU','10','raw:ASU','landing_mat'], // Landing Mat
//      ['ASU','11','raw:ASU','laterite'], // Laterite
//      ['ASU','12','raw:ASU','macadam'], // Macadam
//      ['ASU','13','raw:ASU','membrane'], // Membrane
//      ['ASU','14','raw:ASU','non-bituminous_mix'], // Non-bituminous Mix
//      ['ASU','15','raw:ASU','pierced_steel_planking'], // Pierced Steel Planking
//      ['ASU','16','raw:ASU','sand'], // Sand
//      ['ASU','17','raw:ASU','snow'], // Snow
//      ['ASU','18','raw:ASU','stone'], // Stone
//      ['ASU','19','raw:ASU','water'], // Water
//      ['ASU','20','raw:ASU','wood'], // Wood
//      ['ASU','21','raw:ASU','asphalt_over_concrete'], // Asphalt Over Concrete
//      ['ASU','999','raw:ASU','other'], // Other

     // ASY - Airfield Symbol Type
     // ['ASY','-999999',undefined,undefined], // No Information
     ['ASY','1','aeroway:aerodrome_symbol','active_civilian'], // A - Active Civilian
     ['ASY','2','aeroway:aerodrome_symbol','active_joint_(civilian/military)'], // B - Active Joint (Civilian/Military)
     ['ASY','3','aeroway:aerodrome_symbol','active_military'], // C - Active Military
     ['ASY','4','aeroway:aerodrome_symbol','active_(with_less_than_the_minimum_facilities)'], // D - Active (with less than the minimum facilities)
     ['ASY','5','aeroway:aerodrome_symbol','abandoned_or_closed_(with_usable_runways_or_landing_areas)'], // E - Abandoned or Closed (with usable runways or landing areas)
     ['ASY','6','aeroway:aerodrome_symbol','highway_strip'], // F - Highway Strip
     ['ASY','7','aeroway:aerodrome_symbol','unusable_for_landing_or_take-off'], // G - Unusable for landing or take-off
     ['ASY','8','aeroway:aerodrome_symbol','active_military_heliport'], // H - Active Military Heliport
     ['ASY','9','aeroway:aerodrome_symbol','active_civil_heliport'], // J - Active Civil Heliport
     ['ASY','10','aeroway:aerodrome_symbol','active_military_heliport_(with_less_than_minimum_facilities)'], // K - Active Military Heliport (with less than minimum facilities)
     ['ASY','11','aeroway:aerodrome_symbol','active_civil_heliport_(with_less_than_minimum_facilities)'], // L - Active Civil Heliport (with less than minimum facilities)
     ['ASY','12','aeroway:aerodrome_symbol','decoy'], // X - Decoy
     ['ASY','13','aeroway:aerodrome_symbol','active_joint_(civilian/military)_heliport'], // Active Joint (Civilian/Military) Heliport

     // AT005_CAB - Cable : Cable Type - See CAB
     // ['AT005_CAB','-999999',undefined,undefined], // No Information
//      ['AT005_CAB','2','raw:AT005_CAB','power_distribution_line'], // Power Distribution Line
//      ['AT005_CAB','3','raw:AT005_CAB','telephone'], // Telephone
//      ['AT005_CAB','4','raw:AT005_CAB','telegraph'], // Telegraph
//      ['AT005_CAB','6','raw:AT005_CAB','power_transmission_line'], // Power Transmission Line
//      ['AT005_CAB','7','raw:AT005_CAB','digital_communication_line'], // Digital Communication Line
//      ['AT005_CAB','8','raw:AT005_CAB','communication_line'], // Communication Line
//      ['AT005_CAB','9','raw:AT005_CAB','fibre-optic'], // Fibre-optic
//      ['AT005_CAB','10','raw:AT005_CAB','tethering'], // Tethering
//      ['AT005_CAB','11','raw:AT005_CAB','load_bearing'], // Load Bearing
//      ['AT005_CAB','12','raw:AT005_CAB','guide'], // Guide
//      ['AT005_CAB','13','raw:AT005_CAB','barrier'], // Barrier
//      ['AT005_CAB','14','raw:AT005_CAB','restraining'], // Restraining
//      ['AT005_CAB','15','raw:AT005_CAB','towing'], // Towing
//      ['AT005_CAB','19','raw:AT005_CAB','cableway'], // Cableway
//      ['AT005_CAB','999','raw:AT005_CAB','other'], // Other

     // ATB - Attached Building
     // ['ATB','-999999',undefined,undefined], // No Information
     ['ATB','1000','attached','no'],
     ['ATB','1001','attached','yes'],

     // ATC - Aqueduct Type
     // ['ATC','-999999',undefined,undefined], // No Information
     ['ATC','3','aqueduct:type','underground'], // Underground
     ['ATC','4','aqueduct:type','underwater'], // Underwater
     ['ATC','5','aqueduct:type','surface'], // Surface
     ['ATC','6','aqueduct:type','qanat'], // Qanat
     // AWP - Aeronautical Obstacle Light Present
     // ['AWP','-999999',undefined,undefined],
     ['AWP','1000','light:obstacle','no'],
     ['AWP','1001','light:obstacle','yes'],

     // AXS - Aerodrome Surface Status
     // ['AXS','-999999',undefined,undefined], // No Information
     ['AXS','1','aeroway:surface:status','closed'], // Closed
     ['AXS','2','aeroway:surface:status','open'], // Open
     ['AXS','3','aeroway:surface:status','work_in_progress'], // Work in Progress
     ['AXS','4','aeroway:surface:status','parked_or_disabled_aircraft'], // Parked or Disabled Aircraft

     // AYRC - Aquifer Yield Rating (interval closure)
     ['AYRC','2','aquifer:yield:closure','open_interval'], // Open Interval
     ['AYRC','3','aquifer:yield:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['AYRC','4','aquifer:yield:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['AYRC','5','aquifer:yield:closure','closed_interval'], // Closed Interval
     ['AYRC','6','aquifer:yield:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['AYRC','7','aquifer:yield:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['AYRC','8','aquifer:yield:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['AYRC','9','aquifer:yield:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // AZC - Man-made
     // ['AZC','-999999',undefined,undefined], // No Information
     ['AZC','1000','artificial','no'],
     ['AZC','1001','artificial','yes'],

     // BA000_VDC - Water Line : Sounding Datum
     // ['BA000_VDC','-999999',undefined,undefined], // No Information
     ['BA000_VDC','4','source:datum:sounding','indian_spring_low_water'], // Indian Spring Low Water
     ['BA000_VDC','7','source:datum:sounding','mean_high_water'], // Mean High Water
     ['BA000_VDC','9','source:datum:sounding','mean_high_water_springs'], // Mean High Water Springs
     ['BA000_VDC','10','source:datum:sounding','mean_higher_high_water'], // Mean Higher High Water
     ['BA000_VDC','11','source:datum:sounding','mean_low_water'], // Mean Low Water
     ['BA000_VDC','13','source:datum:sounding','mean_low_water_springs'], // Mean Low Water Springs
     ['BA000_VDC','14','source:datum:sounding','mean_lower_low_water'], // Mean Lower Low Water
     ['BA000_VDC','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
     ['BA000_VDC','19','source:datum:sounding','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
     ['BA000_VDC','20','source:datum:sounding','lowest_astronomical_tide'], // Lowest Astronomical Tide
     ['BA000_VDC','24','source:datum:sounding','mean_higher_high_water_springs'], // Mean Higher High Water Springs
     ['BA000_VDC','28','source:datum:sounding','highest_high_water'], // Highest High Water
     ['BA000_VDC','30','source:datum:sounding','indian_spring_high_water'], // Indian Spring High Water
     ['BA000_VDC','90','source:datum:sounding','lowest_low_water'], // Lowest Low Water
     ['BA000_VDC','999','source:datum:sounding','other'], // Other

     // BAC - Built-up Area Density Category
     // ['BAC','-999999',undefined,undefined], // No Information
     ['BAC','1','building:density','sparse'], // Sparse
     ['BAC','2','building:density','dense'], // Dense
     ['BAC','3','building:density','moderate'], // Moderate

     // BAT - Barrier Top Type
     // ['BAT','-999999',undefined,undefined], // No Information
     ['BAT','1','barrier:top','barbed_wire'], // Barbed Wire
     ['BAT','2','barrier:top','chain_link'], // Chain-link
     ['BAT','3','barrier:top','concertina_wire'], // Concertina Wire
     ['BAT','4','barrier:top','electrified_wire'], // Electrified Wire
     ['BAT','5','barrier:top','spiked'], // Spiked
     ['BAT','999','barrier:top','other'], // Other

     // BC034_MRT - Maritime Radiobeacon : Maritime Radiobeacon Type
     // ['BC034_MRT','-999999',undefined,undefined], // No Information
     ['BC034_MRT','1','seamark:radio_station:category ','ais'], // Automated Identification System (AIS)
     ['BC034_MRT','2','seamark:radio_station:category ','omnidirectional'], // Circular Radiobeacon
     ['BC034_MRT','3','seamark:radio_station:category ','directional'], // Directional Radiobeacon
     ['BC034_MRT','4','seamark:radio_station:category ','loran'], // Long Range Air Navigation System (LORAN) C
     ['BC034_MRT','5','seamark:radio_station:category ','qtg'], // QTG Station
     ['BC034_MRT','6','seamark:radar_transponder:category ','ramark'], // Radar Marker (RAMARK)
     ['BC034_MRT','7','seamark:radar_transponder:category ','racon'], // Radar Responder Beacon (RACON)
     ['BC034_MRT','8','seamark:radio_station:category ','rdf'], // Radio Direction Finding Station
     ['BC034_MRT','9','seamark:radio_station:category ','rotating_pattern'], // Rotating Pattern Radiobeacon
     ['BC034_MRT','999','seamark:radio_station:category ','other'], // Other

     // BC101_SST - Fog Signal : Fog Signal Type
     // ['BC101_SST','-999999',undefined,undefined], // No Information
     ['BC101_SST','1','seamark:fog_signal:category','bell'], // Bell
     ['BC101_SST','2','seamark:fog_signal:category','diaphone'], // Diaphone
     ['BC101_SST','3','seamark:fog_signal:category','explosive'], // Explosive
     ['BC101_SST','4','seamark:fog_signal:category','gong'], // Gong
     ['BC101_SST','6','seamark:fog_signal:category','horn'], // Horn
     ['BC101_SST','7','seamark:fog_signal:category','nautophone'], // Nautophone
     ['BC101_SST','8','seamark:fog_signal:category','radio'], // Radio
     ['BC101_SST','9','seamark:fog_signal:category','siren'], // Siren
     ['BC101_SST','10','seamark:fog_signal:category','submarine_bell'], // Submarine Bell
     ['BC101_SST','14','seamark:fog_signal:category','whistle'], // Whistle
     ['BC101_SST','15','seamark:fog_signal:category','reed'], // Reed
     ['BC101_SST','16','seamark:fog_signal:category','none'], // None
     ['BC101_SST','98','seamark:fog_signal:category','tyfon'], // Tyfon
     ['BC101_SST','999','seamark:fog_signal:category','other'], // Other

     // BCC - Bypass Condition
     // ['BCC','-999999',undefined,undefined], // No Information
     ['BCC','1','bypass','easy'],
     ['BCC','2','bypass','difficult'],
     ['BCC','3','bypass','impossible'],

     // BGT - Basin Gate Type
     // ['BGT','-999999',undefined,undefined], // No Information
     ['BGT','1','seamark:gate:category','caisson'], // Caisson
     ['BGT','2','seamark:gate:category','lock'], // Lock Gate
     ['BGT','3','seamark:gate:category','tide_lock'], // Tide Lock
     ['BGT','999','seamark:gate:category','other'], // Other

     // BH141_IBOA - Waterbody bank 1 orientation
     // ['BH141_IBOA','-999999',undefined,undefined],
     ['BH141_IBOA','1','waterway:bank1:orientation','right'],
     ['BH141_IBOA','2','waterway:bank1:orientation','left'],
     ['BH141_IBOA','3','waterway:bank1:orientation','indeterminate'],
     ['BH141_IBOA','4','waterway:bank1:orientation','no_flow'],
     ['BH141_IBOA','999','waterway:bank1:orientation','other'],

     // BH141_IBOB - Waterbody bank 2 orientation
     // ['BH141_IBOB','-999999',undefined,undefined],
     ['BH141_IBOB','1','waterway:bank2:orientation','right'],
     ['BH141_IBOB','2','waterway:bank2:orientation','left'],
     ['BH141_IBOB','3','waterway:bank2:orientation','indeterminate'],
     ['BH141_IBOB','4','waterway:bank2:orientation','no_flow'],
     ['BH141_IBOB','999','waterway:bank2:orientation','other'],

     // BH141_SHDA - Waterbody bank 1 Shoreline Delineated
     // ['BH141_SHDA','-999999',undefined,undefined],
     ['BH141_SHDA','1000','waterway:bank1:shoreline_delineated','no'],
     ['BH141_SHDA','1001','waterway:bank1:shoreline_delineated','yes'],

     // BH141_SHDB - Waterbody bank 2 Shoreline Delineated
     // ['BH141_SHDB','-999999',undefined,undefined],
     ['BH141_SHDB','1000','waterway:bank2:shoreline_delineated','no'],
     ['BH141_SHDB','1001','waterway:bank2:shoreline_delineated','yes'],

     // BH141_SLTA - Waterbody bank 1 shoreline type: No OSM even close, See MGCP SHR & SHL
     // ['BH141_SLTA','-999999',undefined,undefined], // Null
     ['BH141_SLTA','6','waterway:bank1:type','mangrove'],
     ['BH141_SLTA','8','waterway:bank1:type','marshy'],
     ['BH141_SLTA','10','waterway:bank1:type','stony'],
     ['BH141_SLTA','11','waterway:bank1:type','rubble'],
     ['BH141_SLTA','12','waterway:bank1:type','erosion_rubble'],
     ['BH141_SLTA','13','waterway:bank1:type','sandy'],
     ['BH141_SLTA','14','waterway:bank1:type','shingly'],
     ['BH141_SLTA','17','waterway:bank1:type','ice'],
     ['BH141_SLTA','18','waterway:bank1:type','mud'],
     ['BH141_SLTA','999','waterway:bank1:type','other'],

     // BH141_SLTB - Waterbody bank 2 shoreline type: No OSM even close, See MGCP SHR & SHL
     // ['BH141_SLTB','-999999',undefined,undefined], // Null
     ['BH141_SLTB','6','waterway:bank2:type','mangrove'],
     ['BH141_SLTB','8','waterway:bank2:type','marshy'],
     ['BH141_SLTB','10','waterway:bank2:type','stony'],
     ['BH141_SLTB','11','waterway:bank2:type','rubble'],
     ['BH141_SLTB','12','waterway:bank2:type','erosion_rubble'],
     ['BH141_SLTB','13','waterway:bank2:type','sandy'],
     ['BH141_SLTB','14','waterway:bank2:type','shingly'],
     ['BH141_SLTB','17','waterway:bank2:type','ice'],
     ['BH141_SLTB','18','waterway:bank2:type','mud'],
     ['BH141_SLTB','999','waterway:bank2:type','other'],

     // BMC - Bottom Material Type
     // ['BMC','-999999',undefined,undefined], // No Information
     ['BMC','1','waterway:bottom','clay_and_silt'], // Clay and Silt
     ['BMC','2','waterway:bottom','silty_sands'], // Silty Sands
     ['BMC','3','waterway:bottom','sand_and_gravel'], // Sand and Gravel
     ['BMC','4','waterway:bottom','gravel_and_cobble'], // Gravel and Cobble
     ['BMC','5','waterway:bottom','rocks_and_boulders'], // Rocks and Boulders
     ['BMC','6','waterway:bottom','bedrock'], // Bedrock
     ['BMC','7','waterway:bottom','paved'], // Paved
     ['BMC','8','waterway:bottom','peat'], // Peat
     ['BMC','9','waterway:bottom','sand_over_mud'], // Sand over Mud
     ['BMC','14','waterway:bottom','sand'], // Sand
     ['BMC','15','waterway:bottom','concrete'], // Concrete
     ['BMC','16','waterway:bottom','masonry'], // Masonry
     ['BMC','17','waterway:bottom','reinforced_concrete'], // Reinforced Concrete
     ['BMC','18','waterway:bottom','soil'], // Soil
     ['BMC','999','waterway:bottom','other'], // Other

     // BOC - Bog Type
     // ['BOC','-999999',undefined,undefined], // No Information
     ['BOC','1','bog:type','palsa'], // Palsa Bog
     ['BOC','2','bog:type','string'], // String Bog
     ['BOC','3','bog:type','atlantic_plateau'], // Atlantic Plateau Bog
     ['BOC','4','bog:type','basin'], // Basin Bog
     ['BOC','6','bog:type','blanket'], // Blanket Bog
     ['BOC','7','bog:type','cranberry'], // Cranberry Bog
     ['BOC','8','bog:type','domed'], // Domed Bog
     ['BOC','9','bog:type','fen'], // Fen
     ['BOC','10','bog:type','flat'], // Flat Bog
     ['BOC','11','bog:type','horizontal_fen'], // Horizontal Fen
     ['BOC','12','bog:type','lowland_polygon'], // Lowland Polygon Bog
     ['BOC','13','bog:type','northern_ribbed_fen'], // Northern Ribbed Fen
     ['BOC','14','bog:type','peat'], // Peat Bog
     ['BOC','15','bog:type','peat_plateau'], // Peat Plateau Bog
     ['BOC','16','bog:type','polygonal_peat_plateau'], // Polygonal Peat Plateau Bog
     ['BOC','17','bog:type','shore_fen'], // Shore Fen
     ['BOC','18','bog:type','slope'], // Slope Bog
     ['BOC','19','bog:type','slope_fen'], // Slope Fen
     ['BOC','20','bog:type','veneer'], // Veneer Bog
     ['BOC','999','bog:type','other'], // Other

     // BOT - Bridge Opening Type
     // ['BOT','-999999',undefined,undefined], // Null
     ['BOT','4','bridge:movable','bascule'],
     ['BOT','10','bridge:movable','swing'],
     ['BOT','11','bridge:movable','lift'],
     ['BOT','12','bridge:movable','retractable'],
     ['BOT','14','bridge:movable','submersible'],
     ['BOT','15','bridge:movable','drawbridge'],
     ['BOT','16','bridge:movable','opening'],
     ['BOT','17','bridge:movable','no'],
     ['BOT','999','bridge:movable','other'],

     // BPWHAC - Waterbody Bank 1 height interval closure
     ['BPWHAC','2','waterway:bank1:height:closure','open_interval'],
     ['BPWHAC','3','waterway:bank1:height:closure','greater_than_or_equal_to_less_than_interval'],
     ['BPWHAC','4','waterway:bank1:height:closure','greater_than_to_less_than_or_equal_interval'],
     ['BPWHAC','5','waterway:bank1:height:closure','closed_interval'],
     ['BPWHAC','6','waterway:bank1:height:closure','greater_than_semi_interval'],
     ['BPWHAC','7','waterway:bank1:height:closure','greater_than_or_equal_semi_interval'],
     ['BPWHAC','8','waterway:bank1:height:closure','less_than_semi_interval'],
     ['BPWHAC','9','waterway:bank1:height:closure','less_than_or_equal_semi_interval'],

     // BPWHBC - Waterbody Bank 2 height interval closure
     ['BPWHBC','2','waterway:bank2:height:closure','open_interval'],
     ['BPWHBC','3','waterway:bank2:height:closure','greater_than_or_equal_to_less_than_interval'],
     ['BPWHBC','4','waterway:bank2:height:closure','greater_than_to_less_than_or_equal_interval'],
     ['BPWHBC','5','waterway:bank2:height:closure','closed_interval'],
     ['BPWHBC','6','waterway:bank2:height:closure','greater_than_semi_interval'],
     ['BPWHBC','7','waterway:bank2:height:closure','greater_than_or_equal_semi_interval'],
     ['BPWHBC','8','waterway:bank2:height:closure','less_than_semi_interval'],
     ['BPWHBC','9','waterway:bank2:height:closure','less_than_or_equal_semi_interval'],

     // BPWSAC - Inland Waterbody Bank (1) : Predominant Waterbody Bank Slope (first bank) (interval closure)
     ['BPWSAC','2','waterway:bank1:slope:closure','open_interval'], // Open Interval
     ['BPWSAC','3','waterway:bank1:slope:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['BPWSAC','4','waterway:bank1:slope:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['BPWSAC','5','waterway:bank1:slope:closure','closed_interval'], // Closed Interval
     ['BPWSAC','6','waterway:bank1:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['BPWSAC','7','waterway:bank1:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['BPWSAC','8','waterway:bank1:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['BPWSAC','9','waterway:bank1:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // BPWSBC - Waterbody Bank 2 Slope interval closure
     ['BPWSBC','2','waterway:bank2:slope:closure','open_interval'],
     ['BPWSBC','3','waterway:bank2:slope:closure','greater_than_or_equal_to_less_than_interval'],
     ['BPWSBC','4','waterway:bank2:slope:closure','greater_than_to_less_than_or_equal_interval'],
     ['BPWSBC','5','waterway:bank2:slope:closure','closed_interval'],
     ['BPWSBC','6','waterway:bank2:slope:closure','greater_than_semi_interval'],
     ['BPWSBC','7','waterway:bank2:slope:closure','greater_than_or_equal_semi_interval'],
     ['BPWSBC','8','waterway:bank2:slope:closure','less_than_semi_interval'],
     ['BPWSBC','9','waterway:bank2:slope:closure','less_than_or_equal_semi_interval'],

     // BSC - Bridge Structure Type
     // ['BSC','-999999',undefined,undefined], // No Information
     ['BSC','1','bridge:structure','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC','2','bridge:structure','cantilever'], // Cantilever
     ['BSC','5','bridge:structure','floating'], // Floating
     ['BSC','6','bridge:structure','girder'], // Girder
     ['BSC','7','bridge:structure','tower_suspension'], // Tower Suspension
     ['BSC','8','bridge:structure','truss'], // Truss
     ['BSC','9','bridge:structure','suspension'], // Suspension
     ['BSC','12','bridge:structure','transporter'], // Transporter
     ['BSC','15','bridge:structure','slab'], // Slab
     ['BSC','16','bridge:structure','stringer_beam'], // Stringer Beam
     ['BSC','19','bridge:structure','bowstring'], // Bowstring-bridge
     ['BSC','22','bridge:structure','covered'], // Covered
     ['BSC','26','bridge:structure','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC','27','bridge:structure','cable_stayed'], // Cable Stayed
     ['BSC','30','bridge:structure','submersible'], // Submersible
     ['BSC','31','bridge:structure','arch'], // Arch
     ['BSC','32','bridge:structure','trestle'], // Trestle
     ['BSC','999','bridge:structure','other'], // Other

     // BSM - Mobile Bridge Span
     // ['BSM','-999999',undefined,undefined], // No Information
     ['BSM','1000','bridge:mobile_span','no'],
     ['BSM','1001','bridge:mobile_span','yes'],

     // BSU - Building Superstructure Type
     // ['BSU','-999999',undefined,undefined], // No Information
     ['BSU','1','building:superstructure:type','chimney'], // Chimney
     ['BSU','2','building:superstructure:type','elevator_machine_room'], // Elevator Machine Room
     ['BSU','3','building:superstructure:type','hvac_equipment'], // HVAC Equipment
     ['BSU','4','building:superstructure:type','lightning_rod'], // Lightning Rod
     ['BSU','5','building:superstructure:type','minaret'], // Minaret
     ['BSU','6','building:superstructure:type','ornamental_element'], // Ornamental Element
     ['BSU','7','building:superstructure:type','parapet'], // Parapet
     ['BSU','8','building:superstructure:type','roof_deck'], // Roof Deck
     ['BSU','9','building:superstructure:type','roof_garden'], // Roof Garden
     ['BSU','10','building:superstructure:type','signage'], // Signage
     ['BSU','11','building:superstructure:type','skylight'], // Skylight
     ['BSU','12','building:superstructure:type','clerestory'], // Clerestory
     ['BSU','13','building:superstructure:type','tower'], // Tower
     ['BSU','14','building:superstructure:type','cupola'], // Cupola
     ['BSU','15','building:superstructure:type','steeple'], // Steeple
     ['BSU','16','building:superstructure:type','turret'], // Turret
     ['BSU','17','building:superstructure:type','dome'], // Dome
     ['BSU','999','building:superstructure:type','other'], // Other

     // BWVCAC - Inland Waterbody Bank (1) : Waterbody Bank Vegetation Cover (first bank) (interval closure)
     ['BWVCAC','2','waterway:bank1:cover:closure','open_interval'], // Open Interval
     ['BWVCAC','3','waterway:bank1:cover:closure','greater_than_or_equal_to_less_than_interval'], // Greater-than-or-equal to Less-than Interval
     ['BWVCAC','4','waterway:bank1:cover:closure','greater_than_to_less_than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['BWVCAC','5','waterway:bank1:cover:closure','closed_interval'], // Closed Interval
     ['BWVCAC','6','waterway:bank1:cover:closure','greater_than_semi_interval'], // Greater-than Semi-interval
     ['BWVCAC','7','waterway:bank1:cover:closure','greater_than_or_equal_semi_interval'], // Greater-than or Equal Semi-interval
     ['BWVCAC','8','waterway:bank1:cover:closure','less_than_semi_interval'], // Less-than Semi-interval
     ['BWVCAC','9','waterway:bank1:cover:closure','less_than_or_equal_semi_interval'], // Less-than or Equal Semi-interval

     // BWVCBC - Inland Waterbody Bank (2) : Waterbody Bank Vegetation Cover (second bank) (interval closure)
     ['BWVCBC','2','waterway:bank2:cover:closure','open_interval'], // Open Interval
     ['BWVCBC','3','waterway:bank2:cover:closure','greater_than_or_equal_to_less_than_interval'], // Greater-than-or-equal to Less-than Interval
     ['BWVCBC','4','waterway:bank2:cover:closure','greater_than_to_less_than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['BWVCBC','5','waterway:bank2:cover:closure','closed_interval'], // Closed Interval
     ['BWVCBC','6','waterway:bank2:cover:closure','greater_than_semi_interval'], // Greater-than Semi-interval
     ['BWVCBC','7','waterway:bank2:cover:closure','greater_than_or_equal_semi_interval'], // Greater-than or Equal Semi-interval
     ['BWVCBC','8','waterway:bank2:cover:closure','less_than_semi_interval'], // Less-than Semi-interval
     ['BWVCBC','9','waterway:bank2:cover:closure','less_than_or_equal_semi_interval'], // Less-than or Equal Semi-interval

     // CAA - Controlling Authority - This does not play nicely in OSM
     // ['CAA','-999999',undefined,undefined], // No Information
     // ['CAA','0','controlling_authority','unknown'],
     // ['CAA','1','controlling_authority','national'],
     // ['CAA','2','controlling_authority','state'],
     ['CAA','3','controlling_authority','private'], // Private
     ['CAA','4','controlling_authority','tribal'], // Tribal
     ['CAA','5','controlling_authority','military'], // Military
     ['CAA','6','controlling_authority','municipal'], // Municipal
     ['CAA','7','controlling_authority','joint_military_civilian'], // Joint Military and Civilian
     // ['CAA','8','controlling_authority','international'],
     // ['CAA','11','controlling_authority','province'], // From NFDD
     // ['CAA','13','controlling_authority','regional'], // From NFDD
     ['CAA','16','controlling_authority','civilian'], // Civilian
     ['CAA','17','controlling_authority','public'], // Public
     // ['CAA','998',undefined,undefined],
     ['CAA','999','controlling_authority','other'], // Other

     // CAB - Cable Type
     // ['CAB','-999999',undefined,undefined], // No Information
     // ['CAB','2','power','line'], // Power Distribution Line
     ['CAB','2','cable:type','power'], // Power Line
     ['CAB','3','cable:type','telephone'], // Telephone
     ['CAB','4','cable:type','telegraph'], // Telegraph
     ['CAB','6','cable:type','transmission'], // Transmission Line
     ['CAB','7','cable:type','digital_communication'], // Digital Communication Line
     ['CAB','8','cable:type','communication'], // Communication Line
     ['CAB','9','cable:type','fibre-optic'], // Fibre-optic
     ['CAB','10','cable:type','tethering'], // Tethering
     ['CAB','11','cable:type','load_bearing'], // Load Bearing
     ['CAB','12','cable:type','guide'], // Guide
     ['CAB','13','cable:type','barrier'], // Barrier
     ['CAB','14','cable:type','restraining'], // Restraining
     ['CAB','15','cable:type','towing'], // Towing
     ['CAB','19','cable:type','cableway'], // Cableway
     ['CAB','999','cable:type','other'], // Other

     // CAM - Conservation Area Management Category
     // ['CAM','-999999',undefined,undefined], // No Information
     ['CAM','1','protect_class','1a'], // Strict Nature Reserve
     ['CAM','2','protect_class','1b'], // Wilderness Area
     ['CAM','3','protect_class','2'], // National Park
     ['CAM','4','protect_class','3'], // Natural Monument
     ['CAM','5','protect_class','4'], // Habitat and/or Species Management Area
     ['CAM','6','protection_object','habitat'], // Habitat Management Area
     ['CAM','7','protect_class','14'], // Species Management Area
     ['CAM','8','protection_object','breeding_ground'], // Breeding Ground
     ['CAM','9','protect_class','5'], // Protected Landscape or Seascape
     ['CAM','10','protect_class','6'], // Managed Resource Protected Area
     ['CAM','11','protect_class','15'], // Managed Forest Protected Area
     ['CAM','999','protect_class','other'], // Other

     // CAT - Cableway Type
     // ['CAT','-999999',undefined,undefined], // No Information
     ['CAT','2','cableway:type','chair-lift'], // Chair-lift
     ['CAT','3','cableway:type','ski_tow'], // Ski Tow
     ['CAT','5','cableway:type','aerial_tramway'], // Aerial Tramway
     ['CAT','6','cableway:type','gondola_lift'], // Gondola Lift
     ['CAT','7','cableway:type','t-bar_lift'], // T-bar Lift
     ['CAT','8','cableway:type','industrial_ropeway'], // Industrial Ropeway
     ['CAT','9','cableway:type','material_tramway'], // Material Tramway
     ['CAT','999','cableway:type','other'], // Other

     // CBP - Containment Berm Present
     // ['CBP','-999999',undefined,undefined], // No Information
     ['CBP','1000','containment_berm','no'],
     ['CBP','1001','containment_berm','yes'],

     // CCG - Military Environmental Hazard Category
     // ['CCG','-999999',undefined,undefined], // No Information
     ['CCG','1','military:hazard','biological'], // Biological
     ['CCG','2','military:hazard','chemical'], // Chemical
     ['CCG','3','military:hazard','nuclear_andor_radiological'], // Nuclear and/or Radiological
     ['CCG','4','military:hazard','thermal'], // Thermal
     ['CCG','999','military:hazard','other'], // Other

     // CCT - Cover Closure Type
     // ['CCT','-999999',undefined,undefined], // No Information
     ['CCT','1','cover:type','open'], // Open
     ['CCT','2','cover:type','partial'], // Partial
     ['CCT','3','cover:type','moveable'], // Moveable
     ['CCT','4','cover:type','complete'], // Complete

     // CDA - Covered Drain
     // ['CDA','-999999',undefined,undefined],
     ['CDA','1000','covered','no'],
     ['CDA','1001','covered','yes'],

     // CDY - Cadastral Source Type
     // ['CDY','-999999',undefined,undefined], // No Information
     ['CDY','1','source:cadastral:type','collateral_evidence'], // Collateral Evidence
     ['CDY','2','source:cadastral:type','field_notes'], // Field Notes
     ['CDY','3','source:cadastral:type','field_sketch'], // Field Sketch
     ['CDY','4','source:cadastral:type','gnss_survey'], // GNSS Survey
     ['CDY','5','source:cadastral:type','official_cadastral_survey'], // Official Cadastral Survey
     ['CDY','6','source:cadastral:type','orthophoto'], // Orthophoto
     ['CDY','7','source:cadastral:type','plat'], // Plat
     ['CDY','8','source:cadastral:type','relative_measurement'], // Relative Measurement
     ['CDY','9','source:cadastral:type','resurvey'], // Resurvey
     ['CDY','10','source:cadastral:type','survey'], // Survey
     ['CDY','11','source:cadastral:type','tenure_map'], // Tenure Map
     ['CDY','12','source:cadastral:type','topographic_map'], // Topographic Map
     ['CDY','13','source:cadastral:type','use_plat'], // Use Plat
     ['CDY','14','source:cadastral:type','video'], // Video
     ['CDY','999','source:cadastral:type','other'], // Other

     // CET - Grading Type
     // ['CET','-999999',undefined,undefined], // No Information
     ['CET','1','grading:type','one_side'], // One Side
     ['CET','2','grading:type','two_sides'], // Two Sides
     ['CET','3','grading:type','many_sides'], // Many Sides
     ['CET','4','grading:type','no_sides'], // No Sides
     ['CET','999','grading:type','other'], // Other

     // CNF - Confined
     // ['CNF','-999999',undefined,undefined], // No Information
     ['CNF','1000','aquifer:confined','no'],
     ['CNF','1001','aquifer:confined','yes'],

     // COD - Delineation Known
     ['COD','1000','deliniation','unknown'], // Limits and Information Unknown
     ['COD','1001','deliniation','known'], // Limits and Information Known

     //COE - Certainty of Existence - Really???
     // ['COE','-999999',undefined,undefined], // No Information
     ['COE','1',undefined,undefined], // Definite - Default?
     ['COE','2','existence','doubtful'], // Doubtful
     ['COE','3','existence','reported'], // Reported
     ['COE','6','existence','probable'], // Probable

     // CON - Route Width Constriction Type
     // ['CON','-999999',undefined,undefined], // No Information
     ['CON','1','constriction:type','dropgate'], // Dropgate
     ['CON','2','constriction:type','pass'], // Pass
     ['CON','3','constriction:type','building'], // Building
     ['CON','4','constriction:type','underpass'], // Underpass
     ['CON','5','constriction:type','arcade'], // Arcade
     ['CON','6','constriction:type','bridge_pier'], // Bridge Pier
     ['CON','7','constriction:type','bridge_superstructure'], // Bridge Superstructure
     ['CON','8','constriction:type','bridge_tower'], // Bridge Tower
     ['CON','9','constriction:type','causeway_structure'], // Causeway Structure
     ['CON','10','constriction:type','cave'], // Cave
     ['CON','11','constriction:type','culvert'], // Culvert
     ['CON','12','constriction:type','curb'], // Curb
     ['CON','13','constriction:type','cut'], // Cut
     ['CON','14','constriction:type','dam'], // Dam
     ['CON','15','constriction:type','embankment'], // Embankment
     ['CON','16','constriction:type','entrance_andor_exit'], // Entrance and/or Exit
     ['CON','17','constriction:type','fence'], // Fence
     ['CON','18','constriction:type','fire_hydrant'], // Fire Hydrant
     ['CON','19','constriction:type','ford'], // Ford
     ['CON','20','constriction:type','gallery'], // Gallery
     ['CON','21','constriction:type','gantry'], // Gantry
     ['CON','22','constriction:type','gate'], // Gate
     ['CON','23','constriction:type','hedgerow'], // Hedgerow
     ['CON','24','constriction:type','memorial_monument'], // Memorial Monument
     ['CON','25','constriction:type','non-building_structure'], // Non-building Structure
     ['CON','26','constriction:type','parking_garage'], // Parking Garage
     ['CON','27','constriction:type','pipeline'], // Pipeline
     ['CON','28','constriction:type','pipeline_crossing_point'], // Pipeline Crossing Point
     ['CON','29','constriction:type','prepared_watercourse_crossing'], // Prepared Watercourse Crossing
     ['CON','30','constriction:type','railway_signal'], // Railway Signal
     ['CON','31','constriction:type','railway_switch'], // Railway Switch
     ['CON','32','constriction:type','ramp'], // Ramp
     ['CON','33','constriction:type','reduced_track_or_lane_count'], // Reduced Track or Lane Count
     ['CON','34','constriction:type','retail_stand'], // Retail Stand
     ['CON','35','constriction:type','road_interchange'], // Road Interchange
     ['CON','36','constriction:type','rock_formation'], // Rock Formation
     ['CON','37','constriction:type','route-related_structure'], // Route-related Structure
     ['CON','38','constriction:type','stair'], // Stair
     ['CON','39','constriction:type','steep_terrain_face'], // Steep Terrain Face
     ['CON','40','constriction:type','transportation_block'], // Transportation Block
     ['CON','41','constriction:type','transportation_route_protection_structure'], // Transportation Route Protection Structure
     ['CON','42','constriction:type','tunnel'], // Tunnel
     ['CON','999','constriction:type','other'], // Other

     // COS - Facility Operational Status
     // ['COS','-999999',undefined,undefined], // No Information
     ['COS','4','operational_status','non_operational'], // Non-operational
     ['COS','6','operational_status','planned'], // Planned
     ['COS','8','operational_status','not_in_operation'], // Not in Operation ???
     ['COS','9','operational_status','temp_non_operational'], // Temporarily Non-operational
     ['COS','13','operational_status','operational'], // Operational
     ['COS','14','operational_status','partial'], // Partially Operational

     // CPS - Cell Partition Scheme
     // ['CPS','noInformation',undefined,undefined], // No Information
     ['CPS','fixed0r25','source:cell_partition_scheme','0.25_arc_degree'], // A global geodetic grid consisting of 15 by 15 arc minute cells over land.
     ['CPS','fixed0r5','source:cell_partition_scheme','0.5_arc_degree'], // A global geodetic grid consisting of 30 by 30 arc minute cells over land.
     ['CPS','fixed1r0','source:cell_partition_scheme','1_arc_degree'], // A global geodetic grid consisting of 1 by 1 arc degree cells.
     ['CPS','fixed5r0','source:cell_partition_scheme','5_arc_degree'], // A global geodetic grid consisting of 5 by 5 arc degree cells.
     ['CPS','variable','source:cell_partition_scheme','variable'], // A geodetic grid consisting of variable-sized cells situated so as to cover the extent of a region of interest.
     ['CPS','other','source:cell_partition_scheme','other'], // Other


     // CRA - Crane Type
     // ['CRA','-999999',undefined,undefined], // No Information
     ['CRA','2','crane:type','bridge'], // Bridge Crane
     ['CRA','3','crane:type','rotating'], // Rotating Crane
     ['CRA','7','crane:type','tower'], // Tower Crane
     ['CRA','99','crane:type','container'], // Container Crane
     ['CRA','999','crane:type','other'], // Other

     // ['CRM','-999999',undefined,undefined], // No Information
     // CRM - Crane Mobility Type
     ['CRM','1','crane:mobile','fixed'], // Fixed
     ['CRM','2','crane:mobile','travelling'], // Travelling
     ['CRM','3','crane:mobile','floating'], // Floating
     ['CRM','4','crane:mobile','yes'], // Mobile
     ['CRM','999','crane:mobile','other'], // Other

     // CSC - Intersection Control Type
     // ['CSC','-999999',undefined,undefined], // No Information
     ['CSC','2','crossing','traffic_signals'], // Signal Device
     ['CSC','3','intersection_control','stop_sign'], // Stop Sign
     ['CSC','4','crossing','uncontrolled'], // No Control
     ['CSC','5','crossing','ground_marking'], // Ground Marking
     ['CSC','6','intersection_control','give_way'], // Yield Sign
     ['CSC','7','intersection_control','priority_sign'], // Priority Sign
     ['CSC','999','crossing','other'], // Other

     // CSO - Contaminant Source
     // ['CSO','-999999',undefined,undefined], // No Information
     ['CSO','1','contaminant_source','dredging'], // Dredging
     ['CSO','2','contaminant_source','factories'], // Factories
     ['CSO','3','contaminant_source','farm_run-off'], // Farm Run-off
     ['CSO','4','contaminant_source','military_operations'], // Military Operations
     ['CSO','5','contaminant_source','naturally_occurring'], // Naturally Occurring
     ['CSO','6','contaminant_source','sewers'], // Sewers
     ['CSO','7','contaminant_source','spoil'], // Spoil
     ['CSO','8','contaminant_source','wrecks'], // Wrecks
     ['CSO','999','contaminant_source','other'], // Other

     // CST - Contained within Service Tunnel
     // ['CST','-999999',undefined,undefined], // No Information
     ['CST','1000','service_tunnel','no'],
     ['CST','1001','service_tunnel','yes'],

     // CTC - Culvert Type
     // ['CTC','-999999',undefined,undefined], // No Information
     ['CTC','1','culvert:type','regular_with_soil'], // Regular with Soil
     ['CTC','2','culvert:type','box_with_soil'], // Box with Soil
     ['CTC','3','culvert:type','load_bearing_box'], // Box with Load
     ['CTC','999','culvert:type','other'], // Other

     // CVT - Conveyor Type
     // ['CVT','-999999',undefined,undefined], // No Information
     ['CVT','1','conveyor:type','not_visible'], // Not Visible
     ['CVT','2','conveyor:type','belt'], // Belt
     ['CVT','3','conveyor:type','rollers'], // Rollers
     ['CVT','4','conveyor:type','bucket'], // Bucket
     ['CVT','5','conveyor:type','screw_auger'], // Screw Auger
     ['CVT','6','conveyor:type','apron'], // Apron
     ['CVT','7','conveyor:type','flight'], // Flight
     ['CVT','999','conveyor:type','other'], // Other

     // CWT - Contained Within Tunnel
     // ['CWT','-999999',undefined,undefined],
     ['CWT','1000','in_tunnel','no'],
     ['CWT','1001','in_tunnel','yes'], // Need to think about this

     // DFT - Dam Face Type
     // ['DFT','-999999',undefined,undefined], // No Information
     ['DFT','1','dam:face:type','vertical'], // Vertical
     ['DFT','2','dam:face:type','slope'], // Slope

     // DGC - Transportation Block Type
     // ['DGC','-999999',undefined,undefined], // No Information
     ['DGC','1','block:type','drop_gate'], // Drop Gate
     ['DGC','2','block:type','rolling_block'], // Rolling Block
     ['DGC','999','block:type','other'], // Other

     // DMBC - Undergrowth Density (interval closure)
     ['DMBC','2','undergrowth:density:closure','open_interval'], // Open Interval
     ['DMBC','3','undergrowth:density:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['DMBC','4','undergrowth:density:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['DMBC','5','undergrowth:density:closure','closed_interval'], // Closed Interval
     ['DMBC','6','undergrowth:density:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['DMBC','7','undergrowth:density:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['DMBC','8','undergrowth:density:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['DMBC','9','undergrowth:density:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // DMD - Dammed
     // ['DMD','-999999',undefined,undefined], // No Information
     ['DMD','1000','dammed','no'], // Just to get rid of errors
     ['DMD','1001','dammed','yes'], // yes

     // DPAC - Aquifer Depth <interval closure>
     ['DPAC','2','aquifer:depth:closure','open_interval'], // Open Interval
     ['DPAC','3','aquifer:depth:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['DPAC','4','aquifer:depth:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['DPAC','5','aquifer:depth:closure','closed_interval'], // Closed Interval
     ['DPAC','6','aquifer:depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['DPAC','7','aquifer:depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['DPAC','8','aquifer:depth:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['DPAC','9','aquifer:depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // DWT - Dam Type
     // ['DWT','-999999',undefined,undefined], // No Information
     ['DWT','2','dam:type','navigation'], // Navigation
     ['DWT','3','dam:type','flood_control'], // Flood Control
     ['DWT','4','dam:type','hydroelectric_power_generation'], // Hydroelectric Power Generation
     ['DWT','5','dam:type','weir'], // Weir
     ['DWT','999','dam:type','other'], // Other

     // EET - Engineered Earthwork Type
     // ['EET','-999999',undefined,undefined], // No Information
     ['EET','1','earthwork:type','battery'], // Battery
     ['EET','2','earthwork:type','military_parapet'], // Military Parapet
     ['EET','3','earthwork:type','military_trench'], // Military Trench
     ['EET','4','earthwork:type','rampart'], // Rampart
     ['EET','5','earthwork:type','redoubt'], // Redoubt
     ['EET','999','earthwork:type','other'], // Other

     // EQC - Equivalent Scale Category
     // ['EQC','noInformation',undefined,undefined], // No Information
     ['EQC','scale5k','source:equivalent_scale','1:5k'], // A map scale of 1:5,000.
     ['EQC','scale12r5k','source:equivalent_scale','1:12.5k'], // A map scale of 1:12,500.
     ['EQC','scale25k','source:equivalent_scale','1:25k'], // A map scale of 1:25,000.
     ['EQC','scale50k','source:equivalent_scale','1:50k'], // A map scale of 1:50,000.
     ['EQC','scale100k','source:equivalent_scale','1:100k'], // A map scale of 1:100,000.
     ['EQC','scale250k','source:equivalent_scale','1:250k'], // A map scale of 1:250,000.
     ['EQC','scale500k','source:equivalent_scale','1:500k'], // A map scale of 1:500,000.
     ['EQC','scale1m','source:equivalent_scale','1:1m'], // A map scale of 1:1,000,000.
     ['EQC','scale2m','source:equivalent_scale','1:2m'], // A map scale of 1:2,000,000.
     ['EQC','scale5m','source:equivalent_scale','1:5m'], // A map scale of 1:5,000,000.
     ['EQC','other','source:equivalent_scale','other'], // Other

     // ETS - Extraction Specification
     // ['ETS','noInformation',undefined,undefined], // No Information
     ['ETS','atc','source:extraction_specification','4aa-atc'], // 4AA-ATC
     ['ETS','combat','source:extraction_specification','2ad-combat'], // 2AD-Combat
     ['ETS','dfad1','source:extraction_specification','1ce-dfad-1'], // 1CE-DFAD-1
     ['ETS','dfad2','source:extraction_specification','1cg-dfad-2'], // 1CG-DFAD-2
     ['ETS','digitalFeg','source:extraction_specification','dfeg'], // DFEG
     ['ETS','dnc','source:extraction_specification','dnc'], // DNC
     ['ETS','dted1','source:extraction_specification','1cd-dted-1'], // 1CD-DTED-1
     ['ETS','dted2','source:extraction_specification','1cf-dted-2'], // 1CF-DTED-2
     ['ETS','dtop','source:extraction_specification','3kc/001-dtop'], // 3KC/001-DTOP
     ['ETS','ffd','source:extraction_specification','5ee-ffd'], // 5EE-FFD
     ['ETS','globalTdsEg','source:extraction_specification','gtds-eg'], // GTDS-EG
     ['ETS','hac1','source:extraction_specification','2aa/001-hac-1'], // 2AA/001-HAC-1
     ['ETS','hac2','source:extraction_specification','2aa/002-hac-2'], // 2AA/002-HAC-2
     ['ETS','hac3','source:extraction_specification','2aa/003-hac-3'], // 2AA/003-HAC-3
     ['ETS','hac4','source:extraction_specification','2aa/004-hac-4'], // 2AA/004-HAC-4
     ['ETS','hac5','source:extraction_specification','2aa/005-hac-5'], // 2AA/005-HAC-5
     ['ETS','hac6','source:extraction_specification','2aa/006-hac-6'], // 2AA/006-HAC-6
     ['ETS','hac7','source:extraction_specification','2aa/007-hac-7'], // 2AA/007-HAC-7
     ['ETS','hac8','source:extraction_specification','2aa/008-hac-8'], // 2AA/008-HAC-8
     ['ETS','hac9','source:extraction_specification','2aa/009-hac-9'], // 2AA/009-HAC-9
     ['ETS','jogAirGround','source:extraction_specification','1ae-jog-a/g'], // 1AE-JOG-A/G
     ['ETS','jogRadar','source:extraction_specification','4ac-jog-r'], // 4AC-JOG-R
     ['ETS','localTdsEg','source:extraction_specification','ltds-eg'], // LTDS-EG
     ['ETS','mgcpTrd','source:extraction_specification','mgcp_trd'], // MGCP TRD
     ['ETS','msd1','source:extraction_specification','msd1'], // MSD1
     ['ETS','msd2','source:extraction_specification','msd2'], // MSD2
     ['ETS','msd3','source:extraction_specification','msd3'], // MSD3
     ['ETS','msd4','source:extraction_specification','msd4'], // MSD4
     ['ETS','msd5','source:extraction_specification','msd5'], // MSD5
     ['ETS','onc','source:extraction_specification','1ab-onc'], // 1AB-ONC
     ['ETS','other','source:extraction_specification','other'], // Other
     ['ETS','regionalTdsEg','Fpc','source:extraction_specification','1aa-tpc'], // 1AA-TPC
     ['ETS','uvmap','source:extraction_specification','3ku-uvmap'], // 3KU-UVMap
     ['ETS','vitd','source:extraction_specification','3ka-vitd'], // 3KA-VITD
     ['ETS','vmap0','source:extraction_specification','3kl-vmap-0'], // 3KL-VMap-0
     ['ETS','vmap1','source:extraction_specification','3km-vmap-1'], // 3KM-VMap-1
     ['ETS','vmap2','source:extraction_specification','3kh-vmap-2'], // 3KH-VMap-2

     // ESC - Elevation Surface Category
     // ['ESC','-999999',undefined,undefined], // No Information
     ['ESC','1','elevation:surface','land'], // Land
     ['ESC','2','elevation:surface','snow_field_and/or_ice-field'], // Snow Field and/or Ice-field
     ['ESC','4','elevation:surface','vegetation'], // Vegetation
     ['ESC','5','elevation:surface','inland_water'], // Inland Water
     ['ESC','6','elevation:surface','tidal_water'], // Tidal Water

     // ETY - Engine Test Cell Type
     // ['ETY','-999999',undefined,undefined], // No Information
     ['ETY','1','test_cell:type','jet_engine_dismounted'], // Jet Engine Dismounted
     ['ETY','2','test_cell:type','jet_engine_mounted'], // Jet Engine Mounted
     ['ETY','3','test_cell:type','rocket_engine_upright'], // Rocket Engine Upright
     ['ETY','4','test_cell:type','rocket_engine_horizontal'], // Rocket Engine Horizontal
     ['ETY','999','test_cell:type','other'], // Other

     // EXP - Route Width Expansion Type
     // ['EXP','-999999',undefined,undefined], // No Information
     ['EXP','1','expansion:type','siding'], // Siding
     ['EXP','2','expansion:type','increased_track_or_lane_count'], // Increased Track or Lane Count
     ['EXP','3','expansion:type','railway_sidetrack'], // Railway Sidetrack
     ['EXP','4','expansion:type','roadside_rest_area'], // Roadside Rest Area
     ['EXP','5','expansion:type','shoulder'], // Shoulder
     ['EXP','999','expansion:type','other'], // Other

     // FAC - Solid Maritime Construction
     // ['FAC','-999999',undefined,undefined], // No Information
     ['FAC','1000','solid_construction','no'],
     ['FAC','1001','solid_construction','yes'],

     // FCO Feature Configuration
     // ['FCO','-999999',undefined,undefined],
     ['FCO','5','feature_configuration','same'],
     ['FCO','6','feature_configuration','different'],
     ['FCO','7','feature_configuration','non_divided'], // Ugly

     // FCS - Flood Control Structure Type
     // ['FCS','-999999',undefined,undefined], // No Information
     ['FCS','1','flood_control:type','dyke_gate'], // Dyke Gate
     ['FCS','2','flood_control:type','emergency_gate'], // Emergency Gate
     ['FCS','3','flood_control:type','flood_gate'], // Flood Gate
     ['FCS','4','flood_control:type','fixed_barrage'], // Fixed Barrage
     ['FCS','5','flood_control:type','movable_barrage'], // Movable Barrage
     ['FCS','999','flood_control:type','other'], // Other

     // FER - Ferry Crossing Type
     // ['FER','-999999',undefined,undefined], // No Information
     ['FER','1','ferry:type','cable'], // Cable
     ['FER','2','ferry:type','free-moving'], // Free-moving
     ['FER','3','ferry:type','ice'], // Ice
     ['FER','999','ferry:type','other'], // Other


     // FFN - Feature Function
     // ['FFN','-999999',undefined,undefined], // No Information
     ['FFN','2','use','agriculture'], // Agriculture
     ['FFN','3','use','growing_crops'], // Growing of Crops
     ['FFN','9','use','raising_animals'], // Raising of Animals
     ['FFN','14','use','grazing'], // Grazing
     ['FFN','15','use','mixed_farming'], // Mixed Farming
     ['FFN','16','camp:type','migrant_labour'], // Migrant Labour
     ['FFN','20','camp:type','forestry'], // Forestry and/or Logging
     ['FFN','21','landuse','silviculture'], // Silviculture
     ['FFN','27','amenity','forest_warden'], // Forest Warden
     ['FFN','30','leisure','fishing'], // Fishing
     ['FFN','35','use','aquaculture'], // Aquaculture
     ['FFN','40','use','mining_and_quarrying'], // Mining and Quarrying
     ['FFN','50','use','solid_mineral_fuel_mining'], // Solid Mineral Fuel Mining
     ['FFN','60','use','petroleum_andor_gas_extraction'], // Petroleum and/or Gas Extraction
     ['FFN','70','use','metal_ore_mining'], // Metal Ore Mining
     ['FFN','83','use','chemical_mining'], // Chemical Mining
     ['FFN','84','use','peat_mining'], // Peat Extraction
     ['FFN','85','use','salt_mining'], // Salt Extraction
     ['FFN','87','use','mineral_mining'], // Mineral Mining
     ['FFN','91','use','gas_oil_separation'], // Gas Oil Separation
     ['FFN','95','use','ore_dressing'], // Ore Dressing
     ['FFN','99','use','manufacturing'], // Manufacturing
     ['FFN','100','use','food_manufacturing'], // Food Product Manufacture
     ['FFN','101','use','food_processing'], // Food Processing
     ['FFN','102','use','meat_processing'], // Meat Processing
     ['FFN','103','use','seafood_processing'], // Seafood Processing
     ['FFN','104','use','fruit_vegetable_processing'], // Fruit and/or Vegetable Processing
     ['FFN','105','use','oil-mill'], // Oil-mill
     ['FFN','106','crop','dairy'], // Dairying
     ['FFN','107','use','grain-milling'], // Grain Milling
     ['FFN','110','use','baking'], // Baking
     ['FFN','111','use','sugar_manufacturing'], // Sugar Manufacture
     ['FFN','112','use','sugar_milling'], // Sugar Milling
     ['FFN','113','use','sugar_refining'], // Sugar Refining
     ['FFN','114','use','confectionry_manufacturing'], // Confection Manufacture
     ['FFN','115','use','pasta_manufacturing'], // Pasta Manufacture
     ['FFN','116','use','prepared_meals_manufacturing'], // Prepared Meal Manufacture
     ['FFN','118','use','beverage_manufacturing'], // Beverage Manufacture
     ['FFN','119','use','animal_feed_manufacturing'], // Animal Feed Manufacture
     ['FFN','120','use','ice_manufacturing'], // Ice Manufacture
     ['FFN','121','use','distillery'], // Spirit Distillery
     ['FFN','122','use','winery'], // Winery
     ['FFN','123','use','brewing'], // Brewing
     ['FFN','124','use','soft_drink_manufacturing'], // Soft Drink Manufacture
     ['FFN','125','use','tobacco_product_manufacturing'], // Tobacco Product Manufacture
     ['FFN','129','use','textile_apparel_and_leather_manufacturing'], // Textile, Apparel and Leather Manufacture
     ['FFN','130','use','textile_manufacturing'], // Textile Manufacture
     ['FFN','140','use','apparel_manufacturing'], // Apparel Manufacture
     ['FFN','150','use','leather_product_manufacturing'], // Leather Product Manufacture
     ['FFN','155','use','footwear_manufacturing'], // Footwear Manufacturing
     ['FFN','160','use','wood_product_manufacturing'], // Wood-based Manufacturing
     ['FFN','161','craft','sawmilling'], // Sawmilling
     ['FFN','165','use','wooden_construction_product_manufacturing'], // Wooden Construction Product Manufacture
     ['FFN','171','use','paper_mill'], // Paper-mill
     ['FFN','181','use','printing'], // Printing
     ['FFN','190','use','petroleum_and_coal_product_manufacturing'], // Petroleum and Coal Products Manufacturing
     ['FFN','191','use','coke_manufacturing'], // Coke Manufacture
     ['FFN','192','use','petroleum_refining'], // Petroleum Refining
     ['FFN','195','use','chemical_manufacturing'], // Chemical Manufacture
     ['FFN','210','use','medicinal_manufacturing'], // Medicinal Product Manufacture
     ['FFN','221','use','rubber_product_manufacturing'], // Rubber Product Manufacture
     ['FFN','225','use','plastic_product_manufacturing'], // Plastic Product Manufacture
     ['FFN','230','use','nonmetallic_mineral_product_manufacturing'], // Nonmetallic Mineral Product Manufacturing
     ['FFN','231','use','glass_product_manufacturing'], // Glass Product Manufacture
     ['FFN','232','use','refractory_product_manufacturing'], // Refractory Product Manufacture
     ['FFN','233','use','clay_product_manufacturing'], // Clay Product Manufacture
     ['FFN','234','use','ceramic_product_manufacturing'], // Ceramic Product Manufacture
     ['FFN','235','use','cement_mill'], // Cement Mill
     ['FFN','236','use','cement_product_manufacturing'], // Cement Product Manufacture
     ['FFN','237','use','stone_product_manufacturing'], // Stone Product Manufacture
     ['FFN','240','use','metal_manufacturing'], // Primary Metal Manufacturing
     ['FFN','241','use','steel_mill'], // Steel Mill
     ['FFN','242','use','metal_refining'], // Metal Refining
     ['FFN','243','use','foundry'], // Foundry
     ['FFN','250','use','metal_product_manufacturing'], // Metal Product Manufacturing
     ['FFN','251','use','structural_metal_product_manufacturing'], // Structural Metal Product Manufacture
     ['FFN','255','use','munitions_manufacturing'], // Munitions Manufacture
     ['FFN','257','use','fabricated_metal_product_manufacturing'], // Fabricated Metal Product Manufacture
     ['FFN','260','use','electronic_equipment_manufacturing'], // Electronic Equipment Manufacture
     ['FFN','270','use','electrical_equipment_manufacturing'], // Electrical Equipment Manufacture
     ['FFN','272','shop','gas'], // Propane Sale
     ['FFN','280','use','machinery_manufacturing'], // Machinery Manufacture
     ['FFN','289','use','transportation_equipment_manufacturing'], // Transportation Equipment Manufacturing
     ['FFN','290','use','motor_vehicle_manufacturing'], // Motor Vehicle Manufacture
     ['FFN','301','use','ship_manufacturing'], // Ship Construction
     ['FFN','304','use','railway_vehicle_manufacturing'], // Railway Vehicle Manufacture
     ['FFN','305','use','aircraft_manufacturing'], // Aircraft Manufacture
     ['FFN','306','use','military_vehicle_manufacturing'], // Military Vehicle Manufacture
     ['FFN','310','use','furniture_manufacturing'], // Furniture Manufacture
     ['FFN','320','use','miscellaneous_manufacturing'], // Miscellaneous Manufacturing
     ['FFN','321','use','jewellery_manufacturing'], // Jewellery Manufacture
     ['FFN','322','use','musical_instrument_manufacturing'], // Musical Instrument Manufacture
     ['FFN','323','use','sports_goods_manufacturing'], // Sports Goods Manufacture
     ['FFN','324','use','game_and_toy_manufacturing'], // Game and/or Toy Manufacture
     ['FFN','325','use','medical_equipment_manufacturing'], // Medical and/or Dental Equipment Manufacture
     ['FFN','330','repair','yes'], // General Repair
     ['FFN','331','repair','fabricated_metal_products'], // Fabricated Metal Product Repair
     ['FFN','332','repair','electronic_equipment'], // Electronic Equipment Repair
     ['FFN','333','repair','electrical_equipment'], // Electrical Equipment Repair
     ['FFN','334','repair','machinery'], // Machinery Repair
     ['FFN','340','repair','ship'], // Ship Repair
     ['FFN','341','repair','aircraft'], // Aircraft Repair
     ['FFN','342','repair','railway_vehicle'], // Railway Vehicle Repair
     ['FFN','343','shop','car_repair'], // Motor Vehicle Repair
     ['FFN','350','use','utilities'], // Utilities
     ['FFN','351','use','power_generation'], // Power Generation
     ['FFN','352','use','climate_control'], // Climate Control
     ['FFN','355','use','cooling'], // Cooling
     ['FFN','356','use','heating'], // Heating
     ['FFN','360','use','water_supply'], // Water Supply
     ['FFN','361','use','water_collection'], // Water Collection
     ['FFN','362','use','water_treatment'], // Water Treatment
     ['FFN','363','use','water_distribution'], // Water Distribution
     ['FFN','370','use','sewerage'], // Sewerage
     ['FFN','372','use','sewerage_screening'], // Sewerage Screening
     ['FFN','382','amenity','toilets'], // Restroom
     ['FFN','383','use','waste_treatment_and_disposal'], // Waste Treatment and Disposal
     ['FFN','385','use','materials_recovery'], // Materials Recovery
     ['FFN','440','use','commercial'], // Commerce
     ['FFN','459','shop','wholesale'], // Wholesale Merchant
     ['FFN','460','shop','yes'], // Retail Sale
     ['FFN','464','shop','specialized'], // Specialized Store
     ['FFN','465','shop','non-specialized'], // Non-specialized Store
     ['FFN','466','shop','convenience'], // Convenience Store
     ['FFN','470','amenity','fuel'], // Petrol Sale -- links to AQ170 Motor Vehicle Station
     ['FFN','473','amenity','sales_yard'], // Sales Yard
     ['FFN','474','shop','precious_metal'], // Precious Metal Merchant
     ['FFN','475','amenity','marketplace'], // Market
     ['FFN','476','shop','supermarket'], // Grocery
     ['FFN','477','amenity','pharmacy'], // Pharmacy
     ['FFN','478','shop','pet'], // Pet-shop
     ['FFN','480','use','transportation'], // Transport
     ['FFN','481','use','terminal'], // Terminal
     ['FFN','482','public_transport','station'], // Station
     ['FFN','483','amenity','stop'], // Stop
     ['FFN','484','amenity','transfer_hub'], // Transfer Hub
     ['FFN','486','use','signalling'], // Signalling
     ['FFN','487','use','transport_system_maintenance'], // Transport System Maintenance
     ['FFN','488','use','navigation'], // Navigation - Not great, used in a number of FCodes
     ['FFN','489','use','transportation_hub'], // Transportation Hub
     ['FFN','490','use','railway_transport'], // Railway Transport
     ['FFN','491','use','railway_passenger_transport'], // Railway Passenger Transport
     ['FFN','494','use','pedestrian_transport'], // Pedestrian Transport
     ['FFN','495','use','road_transport'], // Road Transport
     ['FFN','496','use','road_passenger_transport'], // Road Passenger Transport
     ['FFN','497','use','road_freight_transport'], // Road Freight Transport
     ['FFN','500','use','pipeline_transport'], // Pipeline Transport
     ['FFN','501','use','pumping'], // Pumping
     ['FFN','505','use','water_transport'], // Water Transport
     ['FFN','507','use','inland_waters_transport'], // Inland Waters Transport
     ['FFN','508','use','canal_transport'], // Canal Transport
     ['FFN','510','use','port_control'], // Port Control
     ['FFN','511','use','maritime_pilotage'], // Maritime Pilotage
     ['FFN','512','use','pilot_station'], // Pilot Station
     ['FFN','513','use','harbour_control'], // Harbour Control
     ['FFN','520','use','air_transport'], // Air Transport
     ['FFN','525','use','air_traffic_control'], // Air Traffic Control
     ['FFN','529','use','transportation_support'], // Transportation Support
     ['FFN','530','use','warehouse'], // Warehousing and Storage
     ['FFN','535','use','garage'], // Motor Vehicle Parking
     ['FFN','536','use','cargo_handling'], // Cargo Handling
     ['FFN','537','amenity','customs'], // Customs Checkpoint
     ['FFN','538','use','inspection_station'], // Inspection Station
     ['FFN','539','use','inspection'], // Inspection
     ['FFN','540','amenity','post_office'], // Postal Activities
     ['FFN','541','use','mail_and_package_transport'], // Mail and Package Transport
     ['FFN','545','use','courier_activities'], // Courier Activities
     ['FFN','548','use','short-term_accommodation'], // Short-term Accommodation
     ['FFN','550','use','accommodation'], // Accommodation
     ['FFN','551','tourism','hotel'], // Hotel
     ['FFN','552','tourism','resort_hotel'], // Resort Hotel
     ['FFN','553','tourism','motel'], // Motel
     ['FFN','554','tourism','guest_house'], // Guest-house
     ['FFN','555','tourism','hostel'], // Hostel
     ['FFN','556','use','dormitory'], // Dormitory
     ['FFN','557','use','vacation_cottage'], // Vacation Cottage
     ['FFN','558','use','dependents_housing'], // Dependents Housing
     ['FFN','559','amenity','public_bath'], // Communal Bath
     ['FFN','563','use','residential'], // Residence
     ['FFN','570','use','food_service'], // Food Service
     ['FFN','572','amenity','restaurant'], // Restaurant
     ['FFN','573','amenity','bar'], // Bar
     ['FFN','574','amenity','dining_hall'], // Dining Hall
     ['FFN','578','amenity','banquet_hall'], // Banquet Hall
     ['FFN','579','amenity','convention_centre'], // Convention Centre
     ['FFN','580','use','publishing_and_broadcasting'], // Publishing and Broadcasting
     ['FFN','582','use','print_publishing'], // Print Publishing
     ['FFN','594','amenity','cinema'], // Cinema
     ['FFN','601','use','radio_broadcasting'], // Radio Broadcasting
     ['FFN','604','use','television_broadcasting'], // Television Broadcasting
     ['FFN','610','use','communication'], // Telecommunications
     ['FFN','612','use','retail_telecommunications'], // Retail Telecommunications
     ['FFN','614','use','wired_telecommunications'], // Wired Telecommunications
     ['FFN','615','use','main_telephone_exchange'], // Main Telephone Exchange
     ['FFN','616','use','branch_telephone_exchange'], // Branch Telephone Exchange
     ['FFN','617','use','wired_repeater'], // Wired Repeater
     ['FFN','620','use','wireless_telecommunications'], // Wireless Telecommunications
     ['FFN','621','use','mobile_phone_service'], // Mobile Phone Service
     ['FFN','622','use','wireless_repeater'], // Wireless Repeater
     ['FFN','625','use','satellite_telecommunications'], // Satellite Telecommunications
     ['FFN','626','man_made','ground_station'], // Satellite Ground Control
     ['FFN','632','amenity','information_service'], // Information Service
     ['FFN','633','amenity','public_records'], // Public Records
     ['FFN','640','office','financial_services'], // Financial Services
     ['FFN','642','office','central_banking'], // Central Banking
     ['FFN','643','amenity','bank'], // Retail Banking
     ['FFN','651','office','insurance'], // Insurance
     ['FFN','662','office','financial_market_administration'], // Financial Market Administration
     ['FFN','663','office','security_brokerage'], // Security Brokerage
     ['FFN','671','office','fund_management'], // Fund Management
     ['FFN','680','office','estate_agent'], // Real Estate Activities
     ['FFN','681','office','professional_scientific_and_technical'], // Professional, Scientific and Technical
     ['FFN','691','office','legal_activities'], // Legal Activities
     ['FFN','696','office','accounting'], // Accounting
     ['FFN','701','office','head_office'], // Head Office
     ['FFN','706','office','business_management'], // Business Management
     ['FFN','711','office','architecture_consulting'], // Architecture Consulting
     ['FFN','714','office','engineering_design'], // Engineering Design
     ['FFN','717','office','surveying'], // Surveying
     ['FFN','720','use','research'], // Scientific Research and Development
     ['FFN','721','use','observation_station'], // Observation Station
     ['FFN','722','monitoring:weather','yes'], // Weather Station
     ['FFN','725','use','nuclear_research_centre'], // Nuclear Research Centre
     ['FFN','730','use','wind_tunnel'], // Wind Tunnel
     ['FFN','741','amenity','advertising'], // Advertising
     ['FFN','752','shop','photo'], // Photography
     ['FFN','757','amenity','veterinary'], // Veterinary
     ['FFN','760','office','business_support'], // Business and Personal Support Services
     ['FFN','761','amenity','car_rental'], // Motor Vehicle Rental
     ['FFN','770','office','employment_agency'], // Employment Agency
     ['FFN','775','shop','travel_agency'], // Travel Agency
     ['FFN','780','use','security_enforcement'], // Security Enforcement
     ['FFN','781','use','guard'], // Guard
     ['FFN','791','amenity','custodial_service'], // Custodial Service
     ['FFN','795','amenity','landscaping_service'], // Landscaping Service
     ['FFN','801','office','office_administration'], // Office Administration
     ['FFN','807','use','call_centre'], // Call Centre
     ['FFN','808','office','public_administration'], // Public Administration
     ['FFN','809','use','headquarters'], // Headquarters
     ['FFN','810','office','administrative'], // Administration
     ['FFN','811','office','government'], // Government
     ['FFN','812','office','local_government'], // Local Government
     ['FFN','813','office','subnational_government'], // Subnational Government
     ['FFN','814','office','national_government'], // National Government
     ['FFN','815','use','palace'], // Palace
     ['FFN','817','use','capitol'], // Capitol
     ['FFN','818','office','executive_activities'], // Executive Activities
     ['FFN','819','office','legislative_activities'], // Legislative Activities
     ['FFN','821','use','polling_station'], // Polling Station
     ['FFN','822','use','civil_activities'], // Civil Activities
     ['FFN','825','amenity','diplomacy'], // Diplomacy
     ['FFN','826','diplomatic','mission'], // Diplomatic Mission
     ['FFN','827','diplomatic','embassy'], // Embassy
     ['FFN','828','diplomatic','consulate'], // Consul
     ['FFN','829','use','maritime_defense'], // Maritime Defense
     ['FFN','830','use','public_order_safety_and_security_services'], // Public Order, Safety and Security Services
     ['FFN','831','use','public_order'], // Public Order
     ['FFN','832','use','safety'], // Safety
     ['FFN','833','use','security_services'], // Security Services
     ['FFN','835','use','military'], // Defence Activities
     ['FFN','836','military','armory'], // Armory
     ['FFN','837','military','reserve_activities'], // Military Reserve Activities
     ['FFN','838','military','recruitment'], // Military Recruitment
     ['FFN','839','military','cbrne_civilian_support'], // CBRNE Civilian Support
     ['FFN','840','amenity','judicial_activities'], // Judicial Activities
     ['FFN','841','amenity','police'], // Law Enforcement
     ['FFN','842','amenity','immigration_control'], // Immigration Control
     ['FFN','843','amenity','prison'], // Imprisonment
     ['FFN','844','amenity','juvenile_corrections'], // Juvenile Corrections
     ['FFN','845','amenity','fire_station'], // Firefighting
     ['FFN','846','amenity','rescue_and_paramedical_services'], // Rescue and Paramedical Services
     ['FFN','847','use','emergency_operations'], // Emergency Operations
     ['FFN','848','use','civil_intelligence'], // Civil Intelligence
     ['FFN','850','use','education'], // Education
     ['FFN','851','isced:level','1'], // Primary Education
     ['FFN','852','isced:level','2,3'], // Secondary Education
     ['FFN','855','building','university'], // Higher Education
     ['FFN','857','use','vocational_education'], // Vocational Education
     ['FFN','860','use','healthcare'], // Human Health Activities
     ['FFN','861','use','in-patient_care'], // In-patient Care
     ['FFN','862','use','out-patient_care'], // Out-patient Care
     ['FFN','863','use','urgent_medical_care'], // Urgent Medical Care
     ['FFN','864','use','human_tissue_repository'], // Human Tissue Repository
     ['FFN','865','use','public_health_activities'], // Public Health Activities
     ['FFN','866','use','leprosy_care'], // Leprosy Care
     ['FFN','871','use','intermediate_care'], // Intermediate Care
     ['FFN','873','use','psychiatric_in-patient_care'], // Psychiatric In-patient Care
     ['FFN','875','use','residential_care'], // Residential Care
     ['FFN','881','social_facility','emergency'], // Emergency Shelter
     ['FFN','882','social_facility','homeless'], // Homeless Shelter
     ['FFN','883','social_facility','refugee'], // Refugee Shelter
     ['FFN','884','social_facility','youth_emergency'], // Emergency Youth Shelter
     ['FFN','885','amenity','child_care'], // Day Care
     ['FFN','887','use','social_work'], // Social Work
     ['FFN','888','use','emergency_relief_services'], // Emergency Relief Services
     ['FFN','890','amenity','cultural_centre'], // Cultural, Arts and Entertainment
     ['FFN','891','amenity','theatre'], // Theatre
     ['FFN','892','amenity','auditorium'], // Auditorium
     ['FFN','893','amenity','community_centre'], // Community Centre
     ['FFN','894','tourism','opera_house'], // Opera House
     ['FFN','895','amenity','nightclub'], // Night Club
     ['FFN','900','use','sports_amusement_and_recreation'], // Sports, Amusement and Recreation
     ['FFN','902','amenity','library'], // Library
     ['FFN','905','tourism','museum'], // Museum
     ['FFN','906','tourism','aquarium'], // Aquarium
     ['FFN','907','use','botanical_and/or_zoological_reserve_activities'], // Botanical and/or Zoological Reserve Activities
     ['FFN','909','leisure','gambling'], // Gambling
     ['FFN','912','use','sports_centre'], // Sports Centre
     ['FFN','913','leisure','fitness_centre'], // Fitness Centre
     ['FFN','914','sport','shooting_range'], // Shooting Range
     ['FFN','919','use','animal_boarding'], // Animal Boarding
     ['FFN','921','use','recreation'], // Recreation
     ['FFN','922','use','amusement'], // Amusement
     ['FFN','923','leisure','hobbies_andor_leisure_activities'], // Hobbies and/or Leisure Activities
     ['FFN','930','use','religious_activities'], // Religious Activities
     ['FFN','931','amenity','place_of_worship'], // Place of Worship
     ['FFN','932','use','islamic_prayer_hall'], // Islamic Prayer Hall
     ['FFN','950','amenity','membership_organization'], // Membership Organization
     ['FFN','954','amenity','club'], // Club
     ['FFN','955','leisure','yacht_club'], // Yacht-club
     ['FFN','961','shop','laundry'], // Laundry
     ['FFN','962','shop','beauty'], // Beauty Treatment
     ['FFN','963','shop','funeral_directors'], // Funeral Services
     ['FFN','964','amenity','crematorium'], // Cremation
     ['FFN','965','amenity','mortuary'], // Mortuary Services
     ['FFN','966','amenity','adult_entertainment'], // Adult Entertainment
     ['FFN','967','amenity','storage_of_human_remains'], // Storage of Human Remains
     ['FFN','970','use','meeting_place'], // Meeting Place
     ['FFN','980','amenity','death_care_services'], // Death Care Services
     ['FFN','999','use','other'], // Other

     // FHC - Harbour Facility Function
     // ['FHC','-999999',undefined,undefined], // No Information
     ['FHC','1','harbour:use','ro-ro_terminal'], // Ro-Ro Terminal
     ['FHC','2','harbour:use','timber-yard'], // Timber-yard
     ['FHC','3','harbour:use','ferry_terminal'], // Ferry Terminal
     ['FHC','4','harbour:use','fishing_harbour'], // Fishing Harbour
     ['FHC','5','harbour:use','marina'], // Marina
     ['FHC','6','harbour:use','naval_base'], // Naval Base
     ['FHC','7','harbour:use','tanker_terminal'], // Tanker Terminal
     ['FHC','8','harbour:use','passenger_terminal'], // Passenger Terminal
     ['FHC','9','harbour:use','shipyard'], // Shipyard
     ['FHC','10','harbour:use','container_terminal'], // Container Terminal
     ['FHC','11','harbour:use','bulk_terminal'], // Bulk Terminal
     ['FHC','999','harbour:use','other'], // Other

     // FIC - Embankment Type
     // ['FIC','-999999',undefined,undefined], // No Information
     ['FIC','1','embankment:type','mound'], // Mound
     ['FIC','2','embankment:type','fill'], // Fill
     ['FIC','3','embankment:type','dyke'], // Dyke
     ['FIC','5','embankment:type','levee'], // Levee
     ['FIC','6','embankment:type','divider'], // Divider
     ['FIC','999','embankment:type','other'], // Other

     // FLO - Floating
     // ['FLO','-999999',undefined,undefined], // No Information
     ['FLO','1000','floating','no'],
     ['FLO','1001','floating','yes'],

     // FPT - Airfield Type
     // ['FPT','-999999',undefined,undefined], // No Information
     ['FPT','1','aeroway:airfield:type','major'], // Major
     ['FPT','2','aeroway:airfield:type','minor_and_hard'], // Minor and Hard
     ['FPT','3','aeroway:airfield:type','minor_and_soft'], // Minor and Soft
     ['FPT','4','aeroway:airfield:type','minor'], // Minor
     ['FPT','999','aeroway:airfield:type','other'], // Other

     // FRT - Weapons Range Type
     // ['FRT','-999999',undefined,undefined], // No Information
     ['FRT','1','range:type','small_arms'], // Small Arms
     ['FRT','2','range:type','tank'], // Tank
     ['FRT','3','range:type','artillery'], // Field Artillery
     ['FRT','4','range:type','grenade'], // Grenade
     ['FRT','5','range:type','demolition'], // Demolition Area
     ['FRT','6','range:type','impact_area'], // Impact Area
     ['FRT','999','range:type','other'], // Other

     // FSC - Flight Strip Capable
     // ['FSC','-999999',undefined,undefined], // No Information
     ['FSC','1000','road:runway','no'],
     ['FSC','1001','road:runway','yes'],

     // FTI - Fence Type
     // ['FTI','-999999',undefined,undefined], // No Information
     ['FTI','1','fence:type','metal'], // Metal
     ['FTI','2','fence:type','wood'], // Wood
     ['FTI','5','fence:type','barbed_wire'], // Barbed Wire
     ['FTI','6','fence:type','chain-link'], // Chain-link
     ['FTI','7','fence:type','electrified_wire'], // Electrified Wire
     ['FTI','8','fence:type','geotextile'], // Geotextile
     ['FTI','9','fence:type','netting'], // Netting
     ['FTI','999','fence:type','other'], // Other

     // FZR - Fortified Building Type
     // ['FZR','-999999',undefined,undefined], // No Information
     ['FZR','1','fortified:type','blockhouse'], // Blockhouse
     ['FZR','2','fortified:type','casement'], // Casement
     ['FZR','3','fortified:type','keep'], // Keep
     ['FZR','4','fortified:type','martello_tower'], // Martello Tower
     ['FZR','5','fortified:type','non-specific_fortified'], // Non-specific Fortified
     ['FZR','6','fortified:type','pillbox'], // Pillbox
     ['FZR','999','fortified:type','other'], // Other

     // GFT - Geologic Fault Trace Visible
     // ['GFT','-999999',undefined,undefined], // No Information
     ['GFT','1000','visible_fault','no'],
     ['GFT','1001','visible_fault','yes'],

     // GOT - Geothermal Outlet Type
     // ['GOT','-999999',undefined,undefined], // No Information
     ['GOT','1','natural','fissure'], // Fissure
     ['GOT','2','natural','fumarole'], // Fumarole
     ['GOT','3','natural','hot_spring'], // Hot Spring
     ['GOT','4','natural','sulphur_spring'], // Sulphur Spring
     ['GOT','5','natural','geyser'], // Geyser
     ['GOT','999','natural','other_geothermal_outlet'], // Other

     // GSGCHC - Runway Direction (high end) : Surface Slope (high) (interval closure)
     ['GSGCHC','2','aeroway:runway_highend:slope:closure','open_interval'], // Open Interval
     ['GSGCHC','3','aeroway:runway_highend:slope:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['GSGCHC','4','aeroway:runway_highend:slope:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['GSGCHC','5','aeroway:runway_highend:slope:closure','closed_interval'], // Closed Interval
     ['GSGCHC','6','aeroway:runway_highend:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['GSGCHC','7','aeroway:runway_highend:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['GSGCHC','8','aeroway:runway_highend:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['GSGCHC','9','aeroway:runway_highend:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // GSGCLC - Runway Direction (low end) : Surface Slope (low) (interval closure)
     ['GSGCLC','2','aeroway:runway_lowend:slope:closure','open_interval'], // Open Interval
     ['GSGCLC','3','aeroway:runway_lowend:slope:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['GSGCLC','4','aeroway:runway_lowend:slope:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['GSGCLC','5','aeroway:runway_lowend:slope:closure','closed_interval'], // Closed Interval
     ['GSGCLC','6','aeroway:runway_lowend:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['GSGCLC','7','aeroway:runway_lowend:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['GSGCLC','8','aeroway:runway_lowend:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['GSGCLC','9','aeroway:runway_lowend:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // GTC - Gate Use
     // ['GTC','-999999',undefined,undefined], // No Information
     ['GTC','1','barrier','toll_booth'], // Toll-gate
     ['GTC','2','barrier','crossing'], // Crossing
     ['GTC','3','barrier','entrance'], // Entrance - Not great but the closest thing OSM has to the enterence to a facility
     ['GTC','4','barrier','border_control'], // Border Crossing
     ['GTC','999','barrier','other'], // Other

     // GUG - Guyed
     ['GUG','-999999',undefined,undefined], // No Information
     ['GUG','1000','guyed','no'],
     ['GUG','1001','guyed','yes'],

     // HAF - Helipad Associated Facility
     // ['HAF','-999999',undefined,undefined], // No Information
     ['HAF','1','helipad:facility','heliport'], // Heliport
     ['HAF','2','helipad:facility','land_aerodrome'], // Land Aerodrome
     ['HAF','3','helipad:facility','hospital'], // Hospital
     ['HAF','4','helipad:facility','non-hospital_building'], // Non-hospital Building
     ['HAF','6','helipad:facility','rig'], // Rig
     ['HAF','9','helipad:facility','offshore_construction'], // Offshore Construction
     ['HAF','10','helipad:facility','water_aerodrome'], // Water Aerodrome
     ['HAF','11','helipad:facility','military_installation'], // Military Installation
     ['HAF','999','helipad:facility','other'], // Other

     // HHD - Homogenous Habitation Distribution -- Nothing in OSM even close to this
     // ['HHD','-999999',undefined,undefined], // No Information
     ['HHD','1000','homogenous_distribution','no'],
     ['HHD','1001','homogenous_distribution','yes'],

     // HQC - Hypsography Portrayal Type
     // ['HQC','-999999',undefined,undefined], // No Information
     ['HQC','1','contour:type','index_contour'], // Index Contour
     ['HQC','2','contour:type','intermediate_contour'], // Intermediate Contour
     ['HQC','3','contour:type','half_auxiliary_contour'], // Half Auxiliary Contour
     ['HQC','4','contour:type','form_line'], // Form Line
     ['HQC','5','contour:type','depression_index_contour'], // Depression Index Contour
     ['HQC','6','contour:type','depression_intermediate_contour'], // Depression Intermediate Contour
     ['HQC','7','contour:type','approximate_index_contour'], // Approximate Index Contour
     ['HQC','8','contour:type','mound_index_contour'], // Mound Index Contour
     ['HQC','9','contour:type','mound_intermediate_contour'], // Mound Intermediate Contour
     ['HQC','12','contour:type','approximate_intermediate_contour'], // Approximate Intermediate Contour
     ['HQC','13','contour:type','approximate_auxiliary_contour'], // Approximate Auxiliary Contour
     ['HQC','14','contour:type','quarter_auxiliary_contour'], // Quarter Auxiliary Contour
     ['HQC','15','contour:type','approximate_depression_index_contour'], // Approximate Depression Index Contour
     ['HQC','16','contour:type','auxiliary_contour'], // Auxiliary Contour
     ['HQC','18','contour:type','approximate_depression_intermediate_contour'], // Approximate Depression Intermediate Contour
     ['HQC','19','contour:type','intermediate_carrying_contour'], // Intermediate Carrying Contour
     ['HQC','20','contour:type','auxiliary_carrying_contour'], // Auxiliary Carrying Contour
     ['HQC','21','contour:type','index_carrying_contour'], // Index Carrying Contour
     ['HQC','22','contour:type','depression_auxiliary_contour'], // Depression Auxiliary Contour
     ['HQC','23','contour:type','approximate_depression_auxiliary_contour'], // Approximate Depression Auxiliary Contour
     ['HQC','98','contour:type','transition_line'], // Transition Line
     ['HQC','99','contour:type','connector_line'], // Connector Line
     ['HQC','999','contour:type','other'], // Other

     // HSS - Historic Significance
     // ['HSS','-999999',undefined,undefined], // No Information
     ['HSS','1','historic:significance','ancient'], // Ancient
     ['HSS','2','historic:significance','historic'], // Historic
     ['HSS','3','historic:significance','not_significant'], // Not Significant
     ['HSS','4','historic:significance','battlefield'], // Battlefield
     ['HSS','5','historic:significance','military'], // Military
     ['HSS','6','historic:significance','political'], // Political
     ['HSS','7','historic:significance','national'], // National
     ['HSS','8','historic:significance','religious'], // Religious
     ['HSS','999','historic:significance','other'], // Other

     // HST - Hazard Shelter Intended Use
     // ['HST','-999999',undefined,undefined], // No Information
     ['HST','1','shelter:type','bomb'], // Bomb Shelter
     ['HST','2','shelter:type','fallout'], // Fallout Shelter
     ['HST','3','shelter:type','storm'], // Storm Shelter
     ['HST','999','shelter:type','other'], // Other

     // HTP - Hangar Type Category
     // ['HTP','-999999',undefined,undefined], // No Information
     ['HTP','1','hangar:type','multi-bay'], // M - Multi-bay
     ['HTP','2','hangar:type','open_end'], // O - Open End
     ['HTP','3','hangar:type','nose_in'], // N - Nose In
     ['HTP','4','hangar:type','underground'], // G - Underground
     ['HTP','5','hangar:type','single_bay'], // S - Single Bay
     ['HTP','6','hangar:type','double_bay'], // D - Double Bay
     ['HTP','7','hangar:type','t-shaped'], // T - T-Shaped

     // HZD - Geodetic Datum
     // ['HZD','noInformation',undefined,undefined], // No Information
     ['HZD','other','source:datum:geodetic','other'], // Other
     ['HZD','worldGeodeticSystem1984','source:datum:geodetic','world_geodetic_system_1984'], // The geodetic datum known as 'World Geodetic System 1984' and intended to be used globally.
     ['HZD','adindanBurkinaFaso','source:datum:geodetic','adindan_(burkina_faso)'], // The geodetic datum known as 'Adindan' and intended to be used only in the geographic region of Burkina Faso.
     ['HZD','adindanCameroon','source:datum:geodetic','adindan_(cameroon)'], // The geodetic datum known as 'Adindan' and intended to be used only in the geographic region of Cameroon.
     ['HZD','adindanEthiopia','source:datum:geodetic','adindan_(ethiopia)'], // The geodetic datum known as 'Adindan' and intended to be used only in the geographic region of Ethiopia.
     ['HZD','adindanMali','source:datum:geodetic','adindan_(mali)'], // The geodetic datum known as 'Adindan' and intended to be used only in the geographic region of Mali.
     ['HZD','adindanMeanValue','source:datum:geodetic','adindan_(mean_value)'], // The geodetic datum known as 'Adindan' and intended as a mean solution for the geographic region of Ethiopia and Sudan.
     ['HZD','adindanSenegal','source:datum:geodetic','adindan_(senegal)'], // The geodetic datum known as 'Adindan' and intended to be used only in the geographic region of Senegal.
     ['HZD','adindanSudan','source:datum:geodetic','adindan_(sudan)'], // The geodetic datum known as 'Adindan' and intended to be used only in the geographic region of Sudan.
     ['HZD','afgooyeSomalia','source:datum:geodetic','afgooye_(somalia)'], // The geodetic datum known as 'Afgooye' and intended to be used only in the geographic region of Somalia.
     ['HZD','ainelAbd1970BahrainIsland','source:datum:geodetic','ain_el_abd_1970_(bahrain_island)'], // The geodetic datum known as 'Ain el Abd 1970' and intended to be used only in the geographic region of Bahrain Island.
     ['HZD','ainelAbd1970SaudiArabia','source:datum:geodetic','ain_el_abd_1970_(saudi_arabia)'], // The geodetic datum known as 'Ain el Abd 1970' and intended to be used only in the geographic region of Saudi Arabia.
     ['HZD','americanSamoaDatum1962','source:datum:geodetic','american_samoa_datum_1962'], // The geodetic datum known as 'American Samoa Datum 1962' and intended to be used in the geographic region of the American Samoa Islands.
     ['HZD','amersfoort1885dash1903','source:datum:geodetic','amersfoort_1885/1903_(netherlands)'], // The geodetic datum known as 'Amersfoort 1885/1903' and intended to be used only in the geographic region of Netherlands.
     ['HZD','anna1Astro1965CocosIslands','source:datum:geodetic','anna_1_astro_1965_(cocos_islands)'], // The geodetic datum known as 'Anna 1 Astro 1965' and intended to be used only in the geographic region of Cocos Islands.
     ['HZD','antiguaIslandAstro1943','source:datum:geodetic','antigua_island_astro_1943'], // The geodetic datum known as 'Antigua Island Astro 1943' and intended to be used only in the geographic region of Antigua and the Leeward Islands.
     ['HZD','approximateLuzonDatum','source:datum:geodetic','approximate_luzon_datum_(philippines)'], // The geodetic datum known as 'Approximate Luzon Datum' and intended to be used only in the geographic region of Philippines.
     ['HZD','arc1935Africa','source:datum:geodetic','arc_1935_(africa)'], // The geodetic datum known as 'Arc 1935' and intended to be used only in the geographic region of Africa.
     ['HZD','arc1950Botswana','source:datum:geodetic','arc_1950_(botswana)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Botswana.
     ['HZD','arc1950Burundi','source:datum:geodetic','arc_1950_(burundi)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Burundi.
     ['HZD','arc1950Lesotho','source:datum:geodetic','arc_1950_(lesotho)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Lesotho.
     ['HZD','arc1950Malawi','source:datum:geodetic','arc_1950_(malawi)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Malawi.
     ['HZD','arc1950MeanValue','source:datum:geodetic','arc_1950_(mean_value)'], // The geodetic datum known as 'Arc 1950' and intended as a mean solution for the geographic region of Botswana, Lesotho, Malawi, Swaziland, Zaire, Zambia and Zimbabwe.
     ['HZD','arc1950Swaziland','source:datum:geodetic','arc_1950_(swaziland)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Swaziland.
     ['HZD','arc1950Zaire','source:datum:geodetic','arc_1950_(zaire)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Zaire.
     ['HZD','arc1950Zambia','source:datum:geodetic','arc_1950_(zambia)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Zambia.
     ['HZD','arc1950Zimbabwe','source:datum:geodetic','arc_1950_(zimbabwe)'], // The geodetic datum known as 'Arc 1950' and intended to be used only in the geographic region of Zimbabwe.
     ['HZD','arc1960Kenya','source:datum:geodetic','arc_1960_(kenya)'], // The geodetic datum known as 'Arc 1960' and intended to be used only in the geographic region of Kenya.
     ['HZD','arc1960MeanValue','source:datum:geodetic','arc_1960_(mean_value)'], // The geodetic datum known as 'Arc 1960' and intended as a mean solution for the geographic region of Kenya and Tanzania.
     ['HZD','arc1960Tanzania','source:datum:geodetic','arc_1960_(tanzania)'], // The geodetic datum known as 'Arc 1960' and intended to be used only in the geographic region of Tanzania.
     ['HZD','ascensionIsland1958','source:datum:geodetic','ascension_island_1958_(ascension_island)'], // The geodetic datum known as 'Ascension Island 1958' and intended to be used only in the geographic region of Ascension Island.
     ['HZD','astroBeaconEIwoJimaIsland','source:datum:geodetic','astro_beacon_e_(iwo_jima_island)'], // The geodetic datum known as 'Astro Beacon 'E'' and intended to be used only in the geographic region of Iwo Jima Island.
     ['HZD','astroDos71dash4StHelena','source:datum:geodetic','astro_dos_71/4_(st._helena_island)'], // The geodetic datum known as 'Astro DOS 71/4' and intended to be used only in the geographic region of St. Helena Island.
     ['HZD','astroStation1952MarcusIs','source:datum:geodetic','astro_station_1952_(marcus_island)'], // The geodetic datum known as 'Astro Station 1952' and intended to be used only in the geographic region of Marcus Island.
     ['HZD','astroTernIsland1961Hawaii','source:datum:geodetic','astro_tern_island_1961_(tern_island_hawaii)'], // The geodetic datum known as 'Astro Tern Island 1961' and intended to be used only in the geographic region of Tern Island and Hawaii.
     ['HZD','australianGeodetic1966','source:datum:geodetic','australian_geod._1966_(australia_and_tasmania_island)'], // The geodetic datum known as 'Australian Geod. 1966' and intended to be used only in the geographic region of Australia and Tasmania Island.
     ['HZD','australianGeodetic1984','source:datum:geodetic','australian_geod._1984_(australia_and_tasmania_island)'], // The geodetic datum known as 'Australian Geod. 1984' and intended to be used only in the geographic region of Australia and Tasmania Island.
     ['HZD','averageTerrestrialSys1977','source:datum:geodetic','average_terrestrial_system_1977_new_brunswick'], // The geodetic datum known as 'Average Terrestrial System 1977' and intended to be used only in the geographic region of New Brunswick, Nova Scotia, and Prince Edward Island.
     ['HZD','ayabelleLighthouseDjibouti','source:datum:geodetic','ayabelle_lighthouse_(djibouti)'], // The geodetic datum known as 'Ayabelle Lighthouse' and intended to be used only in the geographic region of Djibouti.
     ['HZD','bekaaBaseSouthEndLebanon','source:datum:geodetic','bekaa_base_south_end_(lebanon)'], // The geodetic datum known as 'Bekaa Base South End' and intended to be used only in the geographic region of Lebanon.
     ['HZD','belgium1950SystemLommelSig','source:datum:geodetic','belgium_1950_system_(lommel_signal_belgium)'], // The geodetic datum known as 'Belgium 1950 System' and intended to be used only in the geographic region of Lommel Signal, Belgium.
     ['HZD','belgium1972Observatoire','source:datum:geodetic','belgium_1972_(observatoire_duccle)'], // The geodetic datum known as Belgium 1972 and intended to be used only in the geographic region of the Observatoire dUccle.
     ['HZD','bellevueIgnEfateErromango','source:datum:geodetic','bellevue_(ign)_(efate_and_erromango_islands)'], // The geodetic datum known as 'Bellevue IGN' and intended to be used only in the geographic region of the Efate and Erromango Islands.
     ['HZD','bermuda1957BermudaIslands','source:datum:geodetic','bermuda_1957_(bermuda_islands)'], // The geodetic datum known as 'Bermuda 1957' and intended to be used only in the geographic region of the Bermuda Islands.
     ['HZD','bern1898Switzerland','source:datum:geodetic','bern_1898_(switzerland)'], // The geodetic datum known as 'Bern 1898' and intended to be used only in the geographic region of Switzerland.
     ['HZD','bern1898ZeroMeridian','source:datum:geodetic','bern_1898_(switzerland)_with_zero_meridian_bern'], // The geodetic datum known as 'Bern 1898' whose zero meridian is set at Bern and is intended to be used only in the geographic region of Switzerland.
     ['HZD','bissauGuineaBissau','source:datum:geodetic','bissau_(guinea-bissau)'], // The geodetic datum known as 'Bissau' and intended to be used only in the geographic region of Guinea-Bissau.
     ['HZD','bjz54A954BeijingCoord','source:datum:geodetic','bjz54_(a954_beijing_coordinates)_(china)'], // The geodetic datum known as 'BJZ54' or 'A954 Beijing Coordinates' and intended to be used only in the geographic region of China.
     ['HZD','bogotaObservatoryColombia','source:datum:geodetic','bogota_observatory_(colombia)'], // The geodetic datum known as 'Bogota Observatory' and intended to be used only in the geographic region of Colombia.
     ['HZD','bogotaObsZeroMeridian','source:datum:geodetic','bogota_observatory_(colombia)_with_zero_meridian_bogota'], // The geodetic datum known as 'Bogota Observatory' whose zero meridian is set at Bogota and is intended to be used only in the geographic region of Colombia.
     ['HZD','bukitRimpahIndonesia','source:datum:geodetic','bukit_rimpah_(bangka_and_belitung_islands_indonesia)'], // The geodetic datum known as 'Bukit Rimpah' and intended to be used only in the geographic region of Bangka and Belitung Islands, Indonesia.
     ['HZD','camacupaBaseSwEndAngola','source:datum:geodetic','camacupa_base_sw_end_(campo_de_aviacao_angola)'], // The geodetic datum known as 'Camacupa Base SW End' and intended to be used only in the geographic region of Campo De Aviacao, Angola.
     ['HZD','campAreaAstroAntarctica','source:datum:geodetic','camp_area_astro_(camp_mcmurdo_area_antarctica)'], // The geodetic datum known as 'Camp Area Astro' and intended to be used only in the geographic region of Camp McMurdo Area, Antarctica.
     ['HZD','campoInchauspeArgentina','source:datum:geodetic','campo_inchauspe_(argentina)'], // The geodetic datum known as 'Campo Inchauspe' and intended to be used only in the geographic region of Argentina.
     ['HZD','cantonAstro1966PhoenixIs','source:datum:geodetic','canton_astro_1966_(phoenix_islands)'], // The geodetic datum known as 'Canton Astro 1966' and intended to be used only in the geographic region of Phoenix Islands.
     ['HZD','capeSouthAfrica','source:datum:geodetic','cape_(south_africa)'], // The geodetic datum known as 'Cape' and intended to be used only in the geographic region of South Africa.
     ['HZD','capeCanaveralMeanValue','source:datum:geodetic','cape_canaveral_(mean_value)'], // The geodetic datum known as 'Cape Canaveral' and intended as a mean solution for the geographic region of Florida and the Bahama Islands.
     ['HZD','carthageTunisia','source:datum:geodetic','carthage_(tunisia)'], // The geodetic datum known as 'Carthage' and intended to be used only in the geographic region of Tunisia.
     ['HZD','chatham1971NewZealand','source:datum:geodetic','chatham_1971_(chatham_island_new_zealand)'], // The geodetic datum known as 'Chatham 1971' and intended to be used only in the geographic region of Chatham Island, New Zealand.
     ['HZD','chuaAstroParaguay','source:datum:geodetic','chua_astro_(paraguay)'], // The geodetic datum known as 'Chua Astro' and intended to be used only in the geographic region of Paraguay.
     ['HZD','compensationGeoQuebec1977','source:datum:geodetic','compensation_geodetique_du_quebec_1977'], // The geodetic datum known as 'Compensation Geodetique du Quebec 1977' and whose geographic region of intended use is not specified.
     ['HZD','conakryPyramidGuinea','source:datum:geodetic','conakry_pyramid_of_the_service_geographique_(guinea)'], // The geodetic datum known as 'Conakry Pyramid of the Service Geographique' and intended to be used only in the geographic region of Guinea.
     ['HZD','estonia1937','source:datum:geodetic','co-ordinate_system_1937_of_estonia'], // The geodetic datum known as 'Co-ordinate System 1937 of Estonia' and intended to be used only in the geographic region of Estonia.
     ['HZD','corregoAlegreBrazil','source:datum:geodetic','corrego_alegre_(brazil)'], // The geodetic datum known as 'Corrego Alegre' and intended to be used only in the geographic region of Brazil.
     ['HZD','dabolaGuinea','source:datum:geodetic','dabola_(guinea)'], // The geodetic datum known as 'Dabola' and intended to be used only in the geographic region of Guinea.
     ['HZD','dcs3LighthouseLesserAnt','source:datum:geodetic','dcs-3_lighthouse_saint_lucia_lesser_antilles'], // The geodetic datum known as 'DCS-3 Lighthouse, Saint Lucia, Lesser Antilles' and whose geographic region of intended use is not specified.
     ['HZD','deceptionIslAntarctica','source:datum:geodetic','deception_island_antarctica'], // The geodetic datum known as 'Deception Island' or 'Deception 1993' and intended to be used only in the geographic region of Deception Island, Antarctica.
     ['HZD','djakartaBataviaIndonesia','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)'], // The geodetic datum known as 'Djakarta (Batavia)' and intended to be used only in the geographic region of Sumatra Island, Indonesia.
     ['HZD','djakartaBataviaZeroMerid','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)_with_zero_meridian_djakarta'], // The geodetic datum known as 'Djakarta (Batavia)' whose zero meridian is set at Djakarta and is intended to be used only in the geographic region of Sumatra Island, Indonesia.
     ['HZD','dominicaAstroM12LesserAnt','source:datum:geodetic','dominica_astro_m-12_dominica_lesser_antilles'], // The geodetic datum known as 'Dominica Astro M-12, Dominica, Lesser Antilles' and whose geographic region of intended use is not specified.
     ['HZD','dos1968GizoNewGeorgiaIs','source:datum:geodetic','dos_1968_(gizo_island_new_georgia_islands)'], // The geodetic datum known as 'DOS 1968' and intended to be used only in the geographic region of Gizo Island and the New Georgia Islands.
     ['HZD','easterIsland1967EasterIs','source:datum:geodetic','easter_island_1967_(easter_island)'], // The geodetic datum known as 'Easter Island 1967' and intended to be used only in the geographic region of Easter Island.
     ['HZD','european1950BritishIsles','source:datum:geodetic','european_1950_(british_isles)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of the British Isles to specifically encompass England, the Channel Islands, Ireland, Northern Ireland, Scotland, the Shetland Islands, and Wales.
     ['HZD','european1950Cyprus','source:datum:geodetic','european_1950_(cyprus)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Cyprus.
     ['HZD','european1950Egypt','source:datum:geodetic','european_1950_(egypt)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Egypt.
     ['HZD','european1950England','source:datum:geodetic','european_1950_(england_channel_islands_scotland_and_shetland_islands)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of England, Channel Islands, Scotland, and Shetland Islands.
     ['HZD','european1950Greece','source:datum:geodetic','european_1950_(greece)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Greece.
     ['HZD','european1950Iran','source:datum:geodetic','european_1950_(iran)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Iran.
     ['HZD','european1950IraqSyria','source:datum:geodetic','european_1950_(iraq_israel_jordan_kuwait_lebanon_saudi_arabia_and_syria)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Iraq, Israel, Jordan, Kuwait, Lebanon, Saudi Arabia, and Syria.
     ['HZD','european1950Malta','source:datum:geodetic','european_1950_(malta)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Malta.
     ['HZD','european1950MeanValue','source:datum:geodetic','european_1950_(mean_value)'], // The geodetic datum known as 'European 1950' and intended as a mean solution for the geographic region of Austria, Belgium, Denmark, Finland, France, Federal Republic of Germany, Gibraltar, Greece, Italy, Luxembourg, Netherlands, Norway, Portugal, Spain, S
     ['HZD','european1950NorwayFinland','source:datum:geodetic','european_1950_(norway_and_finland)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Norway and Finland.
     ['HZD','european1950PortugalSpain','source:datum:geodetic','european_1950_(portugal_and_spain)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Portugal and Spain.
     ['HZD','european1950Sardinia','source:datum:geodetic','european_1950_(sardinia)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Sardinia.
     ['HZD','european1950Sicily','source:datum:geodetic','european_1950_(sicily)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Sicily.
     ['HZD','european1950Tunisia','source:datum:geodetic','european_1950_(tunisia)'], // The geodetic datum known as 'European 1950' and intended to be used only in the geographic region of Tunisia.
     ['HZD','european1950WesternEurope','source:datum:geodetic','european_1950_(western_europe)'], // The geodetic datum known as 'European 1950' and intended as a mean solution for a geographic region of Western Europe encompassing Austria, Denmark, France, Federal Republic of Germany, Netherlands, and Switzerland.
     ['HZD','european1979MeanValue','source:datum:geodetic','european_1979_(mean_value)'], // The geodetic datum known as 'European 1979' and intended as a mean solution for the geographic region of Austria, Finland, Netherlands, Norway, Spain, Sweden, and Switzerland.
     ['HZD','etrs1989','source:datum:geodetic','european_terrestrial_reference_system_1989_(etrs89)'], // The geodetic datum known as 'European Terrestrial Reference System 1989' and intended to be used only in the geographic region of ETRS89.
     ['HZD','fortThomas1955LeewardIs','source:datum:geodetic','fort_thomas_1955_(nevis_st_kitts_leeward_islands)'], // The geodetic datum known as 'Fort Thomas 1955' and intended to be used only in the geographic region of Nevis, St Kitts, and the Leeward Islands.
     ['HZD','gan1970AdduAtoll','source:datum:geodetic','gan_1970_(addu_atoll_republic_of_maldives)'], // The geodetic datum known as 'Gan 1970' and intended to be used only in the geographic region of Addu Atoll, Republic of Maldives.
     ['HZD','gandajikaBaseZaire','source:datum:geodetic','gandajika_base_(zaire)'], // The geodetic datum known as 'Gandajika Base' and intended to be used only in the geographic region of Zaire.
     ['HZD','gdz80China','source:datum:geodetic','gdz80_(china)'], // The geodetic datum known as 'GDZ80' and intended to be used only in the geographic region of China.
     ['HZD','geocentricDatumOfAustralia','source:datum:geodetic','geocentric_datum_of_australia_(gda)'], // The geodetic datum known as 'Geocentric Datum of Australia' or 'GDA' and intended to be used only in the geographic region of Australia.
     ['HZD','geodeticDatum1949Zealand','source:datum:geodetic','datum:geodetic_1949_(new_zealand)'], // The geodetic datum known as 'Geodetic Datum 1949' and intended to be used only in the geographic region of New Zealand.
     ['HZD','graciosaBaseSWFaialAzores','source:datum:geodetic','graciosa_base_sw_(faial_graciosa_pico_sao_jorge_and_terceira_island_azores)'], // The geodetic datum known as 'Graciosa Base SW' and intended to be used only in the geographic region of Faial, Graciosa, Pico, Sao Jorge, and Terceira Island, Azores.
     ['HZD','greekDatumGreece','source:datum:geodetic','greek_datum_greece'], // The geodetic datum known as 'Greek Datum, Greece' and whose geographic region of intended use is not specified.
     ['HZD','greekGeodeticRefSystem1987','source:datum:geodetic','greek_geodetic_reference_system_1987_(ggrs_87)'], // The geodetic datum known as 'Greek Geodetic Reference System 1987' or 'GGRS 87' and whose geographic region of intended use is not specified.
     ['HZD','guam1963','source:datum:geodetic','guam_1963'], // The geodetic datum known as 'Guam 1963' and intended to be used only in the geographic region of Guam.
     ['HZD','gunongSegaraKalimantanIs','source:datum:geodetic','gunong_segara_(kalimantan_island_indonesia)'], // The geodetic datum known as 'Gunong Segara' and intended to be used only in the geographic region of Kalimantan Island, Indonesia.
     ['HZD','gunongSerindung','source:datum:geodetic','gunong_serindung'], // The geodetic datum known as 'Gunong Serindung' and whose geographic region of intended use is not specified.
     ['HZD','gux1AstroGuadacanalIs','source:datum:geodetic','gux_1_astro_(guadacanal_island)'], // The geodetic datum known as 'GUX 1 Astro' and intended to be used only in the geographic region of Guadacanal Island.
     ['HZD','guyanaCSG67','source:datum:geodetic','guyana_csg67'], // The geodetic datum known as 'Guyana CSG76' and whose geographic region of intended use is not specified.
     ['HZD','heratNorthAfganistan','source:datum:geodetic','herat_north_(afganistan)'], // The geodetic datum known as 'Herat North' and intended to be used only in the geographic region of Afganistan.
     ['HZD','hermannskogel','source:datum:geodetic','hermannskogel'], // The geodetic datum known as 'Hermannskogel' and intended to be used only in the geographic region of Yugoslavia (prior to 1990), Slovenia, Croatia, Bosnia and Herzegovina, and Serbia.
     ['HZD','hjorsey1955Iceland','source:datum:geodetic','hjorsey_1955_(iceland)'], // The geodetic datum known as 'Hjorsey 1955' and intended to be used only in the geographic region of Iceland.
     ['HZD','hongKong1929','source:datum:geodetic','hong_kong_1929'], // The geodetic datum known as 'Hong Kong 1929' and whose geographic region of intended use is not specified.
     ['HZD','hongKong1963HongKong','source:datum:geodetic','hong_kong_1963_(hong_kong)'], // The geodetic datum known as 'Hong Kong 1963' and intended to be used only in the geographic region of Hong Kong.
     ['HZD','hungarian1972','source:datum:geodetic','hungarian_1972'], // The geodetic datum known as 'Hungarian 1972' and whose geographic region of intended use is not specified.
     ['HZD','huTzuShan','source:datum:geodetic','hu-tzu-shan'], // The geodetic datum known as 'Hu-Tzu-Shan' and intended to be used only in the geographic region of Taiwan.
     ['HZD','indianBangladesh','source:datum:geodetic','indian_(bangladesh)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of Bangladesh.
     ['HZD','indianIndiaNepal','source:datum:geodetic','indian_(india_and_nepal)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of India and Nepal.
     ['HZD','indianPakistan','source:datum:geodetic','indian_(pakistan)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of Pakistan.
     ['HZD','indianThailandVietnam','source:datum:geodetic','indian_(thailand_and_vietnam)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of Thailand and Vietnam.
     ['HZD','indian1954Thailand','source:datum:geodetic','indian_1954_(thailand)'], // The geodetic datum known as 'Indian 1954' and intended to be used only in the geographic region of Thailand.
     ['HZD','indian1960ConSonIsland','source:datum:geodetic','indian_1960_(con_son_island_(vietnam))'], // The geodetic datum known as 'Indian 1960 (Con Son Island)' and intended to be used only in the geographic region of Con Son Island (Vietnam).
     ['HZD','indian1960Vietnam','source:datum:geodetic','indian_1960_(vietnam:_near_16_degrees_north)'], // The geodetic datum known as 'Indian 1960' and intended to be used only in the geographic region of Vietnam near 16 degrees North.
     ['HZD','indian1975Thailand','source:datum:geodetic','indian_1975_(thailand)'], // The geodetic datum known as 'Indian 1975' and intended to be used only in the geographic region of Thailand.
     ['HZD','indian1975ThailandCycle1','source:datum:geodetic','indian_1975_(thailand)_-_cycle_1'], // The geodetic datum known as 'Indian 1975 - Cycle 1' and intended to be used only in the geographic region of Thailand.
     ['HZD','indonesian1974','source:datum:geodetic','indonesian_1974'], // The geodetic datum known as 'Indonesian 1974' and intended to be used only in the geographic region of Indonesia.
     ['HZD','ireland1965IrelandNorthern','source:datum:geodetic','ireland_1965_(ireland_and_northern_ireland)'], // The geodetic datum known as 'Ireland 1965' and intended to be used only in the geographic region of Ireland and Northern Ireland.
     ['HZD','ists061Astro1968GeorgiaIs','source:datum:geodetic','ists_061_astro_1968_(south_georgia_islands)'], // The geodetic datum known as 'ISTS 061 Astro 1968' and intended to be used only in the geographic region of the South Georgia Islands.
     ['HZD','ists073Astro1969DiegoGar','source:datum:geodetic','ists_073_astro_1969_(diego_garcia)'], // The geodetic datum known as 'ISTS 073 Astro 1969' and intended to be used only in the geographic region of Diego Garcia.
     ['HZD','johnstonIsland1961','source:datum:geodetic','johnston_island_1961_(johnston_island)'], // The geodetic datum known as 'Johnston Island 1961' and intended to be used only in the geographic region of Johnston Island.
     ['HZD','kalianpurIndia','source:datum:geodetic','kalianpur_(india)'], // The geodetic datum known as 'Kalianpur' and intended to be used only in the geographic region of India.
     ['HZD','kandawalaSriLanka','source:datum:geodetic','kandawala_(sri_lanka)'], // The geodetic datum known as 'Kandawala' and intended to be used only in the geographic region of Sri Lanka.
     ['HZD','kcs2SierraLeone','source:datum:geodetic','kcs_2_sierra_leone'], // The geodetic datum known as 'KCS 2, Sierra Leone' and whose geographic region of intended use is not specified.
     ['HZD','kerguelenIsland1949','source:datum:geodetic','kerguelen_island_1949_(kerguelen_island)'], // The geodetic datum known as 'Kerguelen Island 1949' and intended to be used only in the geographic region of Kerguelen Island.
     ['HZD','kertau1948RevisedMalaysia','source:datum:geodetic','kertau_1948_(or_revised_kertau)_(west_malaysia_and_singapore)'], // The geodetic datum known as 'Kertau 1948' or 'Revised Kertau' and intended to be used only in the geographic region of West Malaysia and Singapore.
     ['HZD','kkjFinland','source:datum:geodetic','kkj_(or_kartastokoordinaattijarjestelma)_finland'], // The geodetic datum known as 'KKJ' or 'Kartastokoordinaattijarjestelma' and intended to be used only in the geographic region of Finland.
     ['HZD','koreanGeodeticSystem1995','source:datum:geodetic','korean_geodetic_system_1995_(south_korea)'], // The geodetic datum known as 'Korean Geodetic System 1995' and intended to be used only in the geographic region of South Korea.
     ['HZD','kusaieAstro1951','source:datum:geodetic','kusaie_astro_1951'], // The geodetic datum known as 'Kusaie Astro 1951' and intended to be used only in the geographic region of the Caroline Islands (Federated States of Micronesia).
     ['HZD','kuwaitOilCompanyK28','source:datum:geodetic','kuwait_oil_company_(k28)'], // The geodetic datum known as 'Kuwait Oil Company (K28)' and whose geographic region of intended use is not specified.
     ['HZD','lc5Astro1961CaymanBracIs','source:datum:geodetic','l.c._5_astro_1961_(cayman_brac_island)'], // The geodetic datum known as 'L.C. 5 Astro 1961' and intended to be used only in the geographic region of Cayman Brac Island.
     ['HZD','leigonGhana','source:datum:geodetic','leigon_(ghana)'], // The geodetic datum known as 'Leigon' and intended to be used only in the geographic region of Ghana.
     ['HZD','liberia1964Liberia','source:datum:geodetic','liberia_1964_(liberia)'], // The geodetic datum known as 'Liberia 1964' and intended to be used only in the geographic region of Liberia.
     ['HZD','lisbonCastelodiSaoJorge','source:datum:geodetic','lisbon_(castelo_di_sao_jorge)_portugal'], // The geodetic datum known as 'Lisbon (Castelo di Sao Jorge)' and intended to be used only in the geographic region of Portugal.
     ['HZD','localAstro','source:datum:geodetic','local_astro'], // A local geodetic datum established through astronomic observations.
     ['HZD','lomaQuintanaVenezuela','source:datum:geodetic','loma_quintana_(venezuela)'], // The geodetic datum known as 'Loma Quintana' and intended to be used only in the geographic region of Venezuela.
     ['HZD','luzonMindanaoIsland','source:datum:geodetic','luzon_(mindanao_island)'], // The geodetic datum known as 'Luzon' and intended to be used only in the geographic region of Mindanao Island.
     ['HZD','luzonPhilipinesNotMindanao','source:datum:geodetic','luzon_(philipines_except_mindanao_island)'], // The geodetic datum known as 'Luzon' and intended to be used only in the geographic region of the Philipines, excepting Mindanao Island.
     ['HZD','mahe1971MaheIsland','source:datum:geodetic','mahe_1971_(mahe_island)'], // The geodetic datum known as 'Mahe 1971' and intended to be used only in the geographic region of Mahe Island.
     ['HZD','manokwariWestIrian','source:datum:geodetic','manokwari_(west_irian)'], // The geodetic datum known as 'Manokwari' and intended to be used only in the geographic region of West Irian.
     ['HZD','marcoAstroSalvageIslands','source:datum:geodetic','marco_astro_(salvage_islands)'], // The geodetic datum known as 'Marco Astro' and intended to be used only in the geographic region of Salvage Islands.
     ['HZD','martiniqueFortDesaix','source:datum:geodetic','martinique_fort-desaix'], // The geodetic datum known as 'Martinique Fort-Desaix' and whose geographic region of intended use is not specified.
     ['HZD','massawEritreaEthiopia','source:datum:geodetic','massawa_(eritrea_ethiopia)'], // The geodetic datum known as 'Massawa' and intended to be used only in the geographic region of Eritrea, Ethiopia.
     ['HZD','mayotteCombani','source:datum:geodetic','mayotte_combani'], // The geodetic datum known as 'Mayotte Combani' and whose geographic region of intended use is not specified.
     ['HZD','merchichMorocco','source:datum:geodetic','merchich_(morocco)'], // The geodetic datum known as 'Merchich' and intended to be used only in the geographic region of Morocco.
     ['HZD','midwayAstro1961MidwayIs','source:datum:geodetic','midway_astro_1961_(midway_island)'], // The geodetic datum known as 'Midway Astro 1961' and intended to be used only in the geographic region of Midway Island.
     ['HZD','minnaCameroon','source:datum:geodetic','minna_(cameroon)'], // The geodetic datum known as 'Minna' and intended to be used only in the geographic region of Cameroon.
     ['HZD','minnaNigeria','source:datum:geodetic','minna_(nigeria)'], // The geodetic datum known as 'Minna' and intended to be used only in the geographic region of Nigeria.
     ['HZD','modifiedBJZ54China','source:datum:geodetic','modified_bjz54_(china)'], // The geodetic datum known as 'Modified BJZ54' and intended to be used only in the geographic region of China.
     ['HZD','montjongLowe','source:datum:geodetic','montjong_lowe'], // The geodetic datum known as 'Montjong Lowe' and whose geographic region of intended use is not specified.
     ['HZD','montserratIslandAstro1958','source:datum:geodetic','montserrat_island_astro_1958'], // The geodetic datum known as 'Montserrat Island Astro 1958' and intended to be used only in the geographic region of Montserrat and Leeward Islands.
     ['HZD','mountDillonTobago','source:datum:geodetic','mount_dillon_tobago'], // The geodetic datum known as 'Mount Dillon, Tobago' and whose geographic region of intended use is not specified.
     ['HZD','mPoralokoGabon','source:datum:geodetic','mporaloko_(gabon)'], // The geodetic datum known as 'M'Poraloko' and intended to be used only in the geographic region of Gabon.
     ['HZD','nahrwanMasirahIslandOman','source:datum:geodetic','nahrwan_(masirah_island_oman)'], // The geodetic datum known as 'Nahrwan' and intended to be used only in the geographic region of Masirah Island, Oman.
     ['HZD','nahrwanSaudiArabia','source:datum:geodetic','nahrwan_(saudi_arabia)'], // The geodetic datum known as 'Nahrwan' and intended to be used only in the geographic region of Saudi Arabia.
     ['HZD','nahrwanUnitedArabEmirates','source:datum:geodetic','nahrwan_(united_arab_emirates)'], // The geodetic datum known as 'Nahrwan' and intended to be used only in the geographic region of the United Arab Emirates.
     ['HZD','naparimaBwiTrinidadTobago','source:datum:geodetic','naparima_(bwi_trinidad_and_tobago)'], // The geodetic datum known as 'Naparima' and intended to be used only in the geographic region of the British West Indies, Trinidad and Tobago.
     ['HZD','newFrenchZeroMeridianParis','source:datum:geodetic','new_french_or_nouvelle_triangulation_francaise_(ntf)_with_zero_meridian_paris'], // The geodetic datum known as 'New French' or 'Nouvelle Triangulation Francaise' whose zero meridian is set at Paris and is intended to be used only in the geographic region of continental France, Corsica and other surrounding islands.
     ['HZD','northAm1927Alaska','source:datum:geodetic','north_american_1927_(alaska)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Alaska.
     ['HZD','northAm1927Alberta','source:datum:geodetic','north_american_1927_(alberta_and_british_columbia)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Alberta and British Columbia.
     ['HZD','northAm1927AleutianE','source:datum:geodetic','north_american_1927_(aleutian_islands_east_of_180_degrees_west)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Aleutian Islands East of 180 degrees West.
     ['HZD','northAm1927AleutianW','source:datum:geodetic','north_american_1927_(aleutian_islands_west_of_180_degrees_west)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Aleutian Islands West of 180 degrees West.
     ['HZD','northAm1927Bahamas','source:datum:geodetic','north_american_1927_(bahamas_except_san_salvador_island)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Bahamas, except San Salvador Island.
     ['HZD','northAm1927CanadMean','source:datum:geodetic','north_american_1927_(canada_mean)'], // The geodetic datum known as 'North American 1927' and intended as a mean solution for the geographic region of Canada.
     ['HZD','northAm1927CanalZone','source:datum:geodetic','north_american_1927_(canal_zone)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Canal Zone.
     ['HZD','northAm1927Caribbean','source:datum:geodetic','north_american_1927_(caribbean)'], // The geodetic datum known as 'North American 1927' and intended as a mean solution for the geographic region of the Caribbean (Barbados, Caicos Islands, Cuba, Dominican Republic, Grand Cayman, Jamaica, Leeward Islands, and Turks Islands).
     ['HZD','northAm1927CentAmer','source:datum:geodetic','north_american_1927_(central_america)'], // The geodetic datum known as 'North American 1927' and intended as a mean solution for the geographic region of Central America (Belize, Costa Rica, El Salvador, Guatemala, Honduras, and Nicaragua).
     ['HZD','northAm1927ConusMean','source:datum:geodetic','north_american_1927_(conus_mean)'], // The geodetic datum known as 'North American 1927 'and intended as a mean solution for the geographic region of the continental United States.
     ['HZD','northAm1927Cuba','source:datum:geodetic','north_american_1927_(cuba)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Cuba.
     ['HZD','northAm1927EasternUs','source:datum:geodetic','north_american_1927_(eastern_us)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Eastern United States.
     ['HZD','northAm1927HayesPen','source:datum:geodetic','north_american_1927_(hayes_peninsula_greenland)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Hayes Peninsula, Greenland.
     ['HZD','northAm1927Manitoba','source:datum:geodetic','north_american_1927_(manitoba_and_ontario)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Manitoba and Ontario.
     ['HZD','northAm1927Mexico','source:datum:geodetic','north_american_1927_(mexico)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Mexico.
     ['HZD','northAm1927Newfound','source:datum:geodetic','north_american_1927_(newfoundland_new_brunswick_nova_scotia_and_quebec)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Newfoundland, New Brunswick, Nova Scotia and Quebec.
     ['HZD','northAm1927NwTerSaskat','source:datum:geodetic','north_american_1927_(northwest_territories_and_saskatchewan)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Northwest Territories and Saskatchewan.
     ['HZD','northAm1927Salvador','source:datum:geodetic','north_american_1927_(san_salvador_island)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of San Salvador Island.
     ['HZD','northAm1927WesternUs','source:datum:geodetic','north_american_1927_(western_us)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of the Western United States.
     ['HZD','northAm1927Yukon','source:datum:geodetic','north_american_1927_(yukon)'], // The geodetic datum known as 'North American 1927' and intended to be used only in the geographic region of Yukon.
     ['HZD','northAm1983AlaskaExAleut','source:datum:geodetic','north_american_1983_(alaska_excluding_aleutian_islands)'], // The geodetic datum known as 'North American 1983' and intended to be used only in the geographic region of Alaska, excluding Aleutian Islands.
     ['HZD','northAm1983Aleutian','source:datum:geodetic','north_american_1983_(aleutian_islands)'], // The geodetic datum known as 'North American 1983' and intended to be used only in the geographic region of Aleutian Islands.
     ['HZD','northAm1983Canada','source:datum:geodetic','north_american_1983_(canada)'], // The geodetic datum known as 'North American 1983' and intended to be used only in the geographic region of Canada.
     ['HZD','northAm1983Conus','source:datum:geodetic','north_american_1983_(conus)'], // The geodetic datum known as 'North American 1983' and intended to be used only in the geographic region of the continental United States.
     ['HZD','northAm1983Hawaii','source:datum:geodetic','north_american_1983_(hawaii)'], // The geodetic datum known as 'North American 1983' and intended to be used only in the geographic region of Hawaii.
     ['HZD','northAm1983Mexico','source:datum:geodetic','north_american_1983_(mexico_and_central_america))'], // The geodetic datum known as 'North American 1983' and intended to be used only in the geographic region of Mexico and Central America..
     ['HZD','northSahara1959','source:datum:geodetic','north_sahara_1959'], // The geodetic datum known as 'North Sahara 1959' and intended to be used only in the geographic region of Algeria.
     ['HZD','observMeteorologico1939','source:datum:geodetic','observatorio_meteorologico_1939_(corvo_and_flores_islands_azores)'], // The geodetic datum known as 'Observatorio Meteorologico 1939' and intended to be used only in the geographic region of the Corvo and Flores Islands, Azores.
     ['HZD','ocotopequeGuatemala','source:datum:geodetic','ocotopeque_guatemala'], // The geodetic datum known as 'Ocotopeque, Guatemala' and whose geographic region of intended use is not specified.
     ['HZD','oldEgyptian','source:datum:geodetic','old_egyptian_(egypt)'], // The geodetic datum known as 'Old Egyptian' and intended to be used only in the geographic region of Egypt.
     ['HZD','oldHawaiianHawaiiIsland','source:datum:geodetic','old_hawaiian_(hawaii)'], // The geodetic datum known as 'Old Hawaiian' and intended to be used only in the geographic region of Hawaii.
     ['HZD','oldHawaiianKauaiIsland','source:datum:geodetic','old_hawaiian_(kauai)'], // The geodetic datum known as 'Old Hawaiian' and intended to be used only in the geographic region of Kauai.
     ['HZD','oldHawaiianMauiIsland','source:datum:geodetic','old_hawaiian_(maui)'], // The geodetic datum known as 'Old Hawaiian' and intended to be used only in the geographic region of Maui.
     ['HZD','oldHawaiianMeanValue','source:datum:geodetic','old_hawaiian_(mean_value)'], // The geodetic datum known as 'Old Hawaiian' and intended as a mean solution for the geographic region of the Hawaii Islands.
     ['HZD','oldHawaiianOahuIsland','source:datum:geodetic','old_hawaiian_(oahu)'], // The geodetic datum known as 'Old Hawaiian' and intended to be used only in the geographic region of Oahu.
     ['HZD','oman','source:datum:geodetic','oman_(oman)'], // The geodetic datum known as 'Oman' and intended to be used only in the geographic region of Oman.
     ['HZD','ordnanceSurvGB1936England','source:datum:geodetic','ordnance_survey_g.b._1936_(england)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of England.
     ['HZD','ordnanceSurvGB1936ScotWale','source:datum:geodetic','ordnance_survey_g.b._1936_(england_isle_of_man_and_wales)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of England, the Isle of Man, and Wales.
     ['HZD','ordnanceSurvGB1936MeanVal','source:datum:geodetic','ordnance_survey_g.b._1936_(mean_value)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended as a mean solution for the geographic region of England, the Isle of Man, Scotland, Shetland, and Wales.
     ['HZD','ordnanceSurvGB1936ScotShet','source:datum:geodetic','ordnance_survey_g.b._1936_(scotland_and_shetland_islands)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of Scotland and the Shetland Islands.
     ['HZD','ordnanceSurvGB1936Wales','source:datum:geodetic','ordnance_survey_g.b._1936_(wales)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of Wales.
     ['HZD','osloObservatoryOld','source:datum:geodetic','oslo_observatory_(old)_norway'], // The geodetic datum known as 'Oslo Observatory (Old)' and intended to be used only in the geographic region of Norway.
     ['HZD','padangBaseWestEnd','source:datum:geodetic','padang_base_west_end_(sumatra_indonesia)'], // The geodetic datum known as 'Padang Base West End' and intended to be used only in the geographic region of Sumatra, Indonesia.
     ['HZD','padangBaseWestEndZeroMerid','source:datum:geodetic','padang_base_west_end_(sumatra_indonesia)_with_zero_meridian_djakarta'], // The geodetic datum known as 'Padang Base West End' whose zero meridian is set at Djakarta and is intended to be used only in the geographic region of Sumatra, Indonesia.
     ['HZD','palestine1928','source:datum:geodetic','palestine_1928_(israel_jordan)'], // The geodetic datum known as 'Palestine 1928' and intended to be used only in the geographic region of Israel and Jordan.
     ['HZD','picoDeLasNievesCanaryIs','source:datum:geodetic','pico_de_las_nieves_(canary_islands)'], // The geodetic datum known as 'Pico de las Nieves' and intended to be used only in the geographic region of Canary Islands.
     ['HZD','pitcairnAstro1967','source:datum:geodetic','pitcairn_astro_1967_(pitcairn_island)'], // The geodetic datum known as 'Pitcairn Astro 1967' and intended to be used only in the geographic region of Pitcairn Island.
     ['HZD','point58MeanSolution','source:datum:geodetic','point_58_mean_solution_(burkina_faso_and_niger)'], // The geodetic datum known as 'Point 58 Mean Solution' and intended to be used only in the geographic region of Burkina Faso and Niger.
     ['HZD','pointeNoire1948','source:datum:geodetic','pointe_noire_1948'], // The geodetic datum known as 'Pointe Noire 1948' and intended to be used only in the geographic region of Congo.
     ['HZD','potsdamHelmertturmGermany','source:datum:geodetic','potsdam_or_helmertturm_(germany)'], // The geodetic datum known as 'Potsdam' or 'Helmertturm' and intended to be used only in the geographic region of Germany.
     ['HZD','provSouthAm1956Bolivia','source:datum:geodetic','prov._s._american_1956_(bolivia)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Bolivia.
     ['HZD','provSouthAm1956Columbia','source:datum:geodetic','prov._s._american_1956_(columbia)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Columbia.
     ['HZD','provSouthAm1956Ecuador','source:datum:geodetic','prov._s._american_1956_(ecuador)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Ecuador.
     ['HZD','provSouthAm1956Guyana','source:datum:geodetic','prov._s._american_1956_(guyana)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Guyana.
     ['HZD','provSouthAm1956MeanValue','source:datum:geodetic','prov._s._american_1956_(mean_value)'], // The geodetic datum known as 'Prov. S. American 1956' and intended as a mean solution for the geographic region of Bolivia, Chile, Colombia, Ecuador, Guyana, Peru, and Venezuela.
     ['HZD','provSouthAm1956NChile','source:datum:geodetic','prov._s._american_1956_(northern_chile_near_19_degrees_south)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Northern Chile near 19 degrees South.
     ['HZD','provSouthAm1956Peru','source:datum:geodetic','prov._s._american_1956_(peru)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Peru.
     ['HZD','provSouthAm1956SChile','source:datum:geodetic','prov._s._american_1956_(southern_chile_near_43_degrees_south)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Southern Chile near 43 degrees South.
     ['HZD','provSouthAm1956Venezuela','source:datum:geodetic','prov._s._american_1956_(venezuela)'], // The geodetic datum known as 'Prov. S. American 1956' and intended to be used only in the geographic region of Venezuela.
     ['HZD','provSouthChilean1963','source:datum:geodetic','provisional_south_chilean_1963_(or_hito_xviii_1963)_(s._chile_53_degrees_south)'], // The geodetic datum known as 'Provisional South Chilean 1963' or 'Hito XVIII 1963' and intended to be used only in the geographic region of Chile south of 53 degrees South.
     ['HZD','puertoRicoVirginIslands','source:datum:geodetic','puerto_rico_(puerto_rico_and_virgin_islands)'], // The geodetic datum known as 'Puerto Rico' and intended to be used only in the geographic region of Puerto Rico and the Virgin Islands.
     ['HZD','pulkovo1942Albania','source:datum:geodetic','pulkovo_1942_(albania)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Albania.
     ['HZD','pulkovo1942Czechoslovakia','source:datum:geodetic','pulkovo_1942_(czechoslovakia)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Czechoslovakia (prior to 1 January 1993).
     ['HZD','pulkovo1942Hungary','source:datum:geodetic','pulkovo_1942_(hungary)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Hungary.
     ['HZD','pulkovo1942Kazakhstan','source:datum:geodetic','pulkovo_1942_(kazakhstan)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Kazakhstan.
     ['HZD','pulkovo1942Latvia','source:datum:geodetic','pulkovo_1942_(latvia)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Latvia.
     ['HZD','pulkovo1942Poland','source:datum:geodetic','pulkovo_1942_(poland)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Poland.
     ['HZD','pulkovo1942Romania','source:datum:geodetic','pulkovo_1942_(romania)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Romania.
     ['HZD','pulkovo1942Russia','source:datum:geodetic','pulkovo_1942_(russia)'], // The geodetic datum known as 'Pulkovo 1942' and intended to be used only in the geographic region of Russia.
     ['HZD','qatarNationalQatar','source:datum:geodetic','qatar_national_(qatar)'], // The geodetic datum known as 'Qatar National' and intended to be used only in the geographic region of Qatar.
     ['HZD','qornoqSouthGreenland','source:datum:geodetic','qornoq_(south_greenland)'], // The geodetic datum known as 'Qornoq' and intended to be used only in the geographic region of South Greenland.
     ['HZD','rauenbergBerlinGermany','source:datum:geodetic','rauenberg_(berlin_germany)'], // The geodetic datum known as 'Rauenberg' and intended to be used only in the geographic region of Berlin, Germany.
     ['HZD','reconTriangulationMorocco','source:datum:geodetic','reconnaissance_triangulation_morocco'], // The geodetic datum known as 'Reconnaissance Triangulation, Morocco' and whose geographic region of intended use is not specified.
     ['HZD','reunion1947','source:datum:geodetic','reunion_1947'], // The geodetic datum known as 'Reunion 1947' and intended to be used only in the geographic region of the Mascarene Islands.
     ['HZD','revisedNahrwan','source:datum:geodetic','revised_nahrwan'], // The geodetic datum known as 'Revised Nahrwan' and whose geographic region of intended use is not specified.
     ['HZD','rome1940','source:datum:geodetic','rome_1940_(or_monte_mario_1940)_italy'], // The geodetic datum known as 'Rome 1940' or 'Monte Mario 1940' and intended to be used only in the geographic region of Italy.
     ['HZD','rome1940ZeroMeridianRome','source:datum:geodetic','rome_1940_(or_monte_mario_1940)_italy_with_zero_meridian_rome'], // The geodetic datum known as 'Rome 1940' or 'Monte Mario 1940' whose zero meridian is set at Rome and is intended to be used only in the geographic region of Italy.
     ['HZD','rt90StockholmSweden','source:datum:geodetic','rt90_stockholm_sweden'], // The geodetic datum known as 'RT90, Stockholm, Sweden' and whose geographic region of intended use is not specified.
     ['HZD','sainteAnneI1984Guadeloupe','source:datum:geodetic','sainte_anne_i_1984_(guadeloupe)'], // The geodetic datum known as 'Sainte Anne I 1984' and intended to be used only in the geographic region of Guadeloupe.
     ['HZD','santoDos1965EspiritoSanto','source:datum:geodetic','santo_(dos)_1965_(espirito_santo_island)'], // The geodetic datum known as 'Santo DOS. 1965' and intended to be used only in the geographic region of Espirito Santo Island.
     ['HZD','saoBrazSaoMiguelAzores','source:datum:geodetic','sao_braz_(sao_miguel_santa_maria_islands_azores)'], // The geodetic datum known as 'Sao Braz' and intended to be used only in the geographic region of Sao Miguel, the Santa Maria Islands, and the Azores.
     ['HZD','sapperHill1943EastFalkland','source:datum:geodetic','sapper_hill_1943_(east_falkland_islands)'], // The geodetic datum known as 'Sapper Hill 1943' and intended to be used only in the geographic region of the East Falkland Islands.
     ['HZD','schwarzeckNamibia','source:datum:geodetic','schwarzeck_(namibia)'], // The geodetic datum known as 'Schwarzeck' and intended to be used only in the geographic region of Namibia.
     ['HZD','seBasePortoSanto','source:datum:geodetic','se_base_(porto_santo)_(porto_santo_and_madeira_islands)'], // The geodetic datum known as 'SE Base Porto Santo' and intended to be used only in the geographic region of Porto Santo and the Madeira Islands.
     ['HZD','selvagemGrande1938Salvage','source:datum:geodetic','selvagem_grande_1938_(salvage_islands)'], // The geodetic datum known as 'Selvagem Grande 1938' and intended to be used only in the geographic region of the Salvage Islands.
     ['HZD','sierraLeone1960','source:datum:geodetic','sierra_leone_1960'], // The geodetic datum known as 'Sierra Leone 1960' and intended to be used only in the geographic region of Sierra Leone.
     ['HZD','sJtsk','source:datum:geodetic','s-jtsk'], // The geodetic datum known as 'S-JTSK' and intended to be used in the geographic region of the Czech Republic and Slovakia.
     ['HZD','southAfricanSouthAfrica','source:datum:geodetic','south_african_(south_africa)'], // The geodetic datum known as 'South African' and intended to be used only in the geographic region of South Africa.
     ['HZD','southAmerican1969Argentina','source:datum:geodetic','south_american_1969_(argentina)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Argentina.
     ['HZD','southAmerican1969BaltraIs','source:datum:geodetic','south_american_1969_(baltra_galapagos_islands)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Baltra and the Galapagos Islands.
     ['HZD','southAmerican1969Bolivia','source:datum:geodetic','south_american_1969_(bolivia)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Bolivia.
     ['HZD','southAmerican1969Brazil','source:datum:geodetic','south_american_1969_(brazil)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Brazil.
     ['HZD','southAmerican1969Chile','source:datum:geodetic','south_american_1969_(chile)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Chile.
     ['HZD','southAmerican1969Columbia','source:datum:geodetic','south_american_1969_(columbia)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Columbia.
     ['HZD','southAmerican1969Ecuador','source:datum:geodetic','south_american_1969_(ecuador)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Ecuador.
     ['HZD','southAmerican1969Guyana','source:datum:geodetic','south_american_1969_(guyana)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Guyana.
     ['HZD','southAmerican1969MeanValue','source:datum:geodetic','south_american_1969_(mean_value)'], // The geodetic datum known as 'South American 1969' and intended as a mean solution for the geographic region of Argentina, Bolivia, Brazil, Chile, Columbia, Ecuador, Guyana, Paraguay, Peru, Trinidad and Tobago, and Venezuela.
     ['HZD','southAmerican1969Paraguay','source:datum:geodetic','south_american_1969_(paraguay)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Paraguay.
     ['HZD','southAmerican1969Peru','source:datum:geodetic','south_american_1969_(peru)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Peru.
     ['HZD','southAmerican1969Trinidad','source:datum:geodetic','south_american_1969_(trinidad_and_tobago)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Trinidad and Tobago.
     ['HZD','southAmerican1969Venezuela','source:datum:geodetic','south_american_1969_(venezuela)'], // The geodetic datum known as 'South American 1969' and intended to be used only in the geographic region of Venezuela.
     ['HZD','sirgas','source:datum:geodetic','south_american_geocentric_reference_system_(sirgas)'], // The geodetic datum known as 'South American Geocentric Reference System' and intended to be used only in the geographic region of South America.
     ['HZD','southAsiaSingapore','source:datum:geodetic','south_asia_(southeast_asia_singapore)'], // The geodetic datum known as 'South Asia' and intended to be used only in the geographic region of Southeast Asia and Singapore.
     ['HZD','sovietGeodeticSystem1985','source:datum:geodetic','soviet_geodetic_system_1985'], // The geodetic datum known as 'Soviet Geodetic System 1985' and intended to be used only in the geographic region of the former Soviet Union.
     ['HZD','sovietGeodeticSystem1990','source:datum:geodetic','soviet_geodetic_system_1990'], // The geodetic datum known as 'Soviet Geodetic System 1990' and intended to be used only in the geographic region of the former Soviet Union.
     ['HZD','stPierreetMiquelon1950','source:datum:geodetic','st._pierre_et_miquelon_1950'], // The geodetic datum known as 'St. Pierre et Miquelon 1950' and whose geographic region of intended use is not specified.
     ['HZD','stockholm1938Sweden','source:datum:geodetic','stockholm_1938_(sweden)'], // The geodetic datum known as 'Stockholm 1938' and intended to be used only in the geographic region of Sweden.
     ['HZD','sydneyObservatoryNewSouth','source:datum:geodetic','sydney_observatory_new_south_wales_australia'], // The geodetic datum known as 'Sydney Observatory, New South Wales, Australia' and whose geographic region of intended use is not specified.
     ['HZD','tananariveObservatory1925','source:datum:geodetic','tananarive_observatory_1925'], // The geodetic datum known as 'Tananarive Observatory 1925' and intended to be used only in the geographic region of Madagascar.
     ['HZD','tananariveObs1925ZerMerPar','source:datum:geodetic','tananarive_observatory_1925_with_zero_meridian_paris'], // The geodetic datum known as 'Tananarive Observatory 1925' whose zero meridian is set at Paris and is intended to be used only in the geographic region of Madagascar.
     ['HZD','timbalai1948BruneiMalaysia','source:datum:geodetic','timbalai_1948_(brunei_and_east_malaysia_-_sarawak_and_sabah)'], // The geodetic datum known as 'Timbalai 1948' and intended to be used only in the geographic region of Brunei and East Malaysia (Sarawak and Sabah).
     ['HZD','timbalai1968','source:datum:geodetic','timbalai_1968'], // The geodetic datum known as 'Timbalai 1968' and whose geographic region of intended use is not specified.
     ['HZD','tokyoJapan','source:datum:geodetic','tokyo_(japan)'], // The geodetic datum known as 'Tokyo' and intended to be used only in the geographic region of Japan.
     ['HZD','tokyoKorea','source:datum:geodetic','tokyo_(korea)'], // The geodetic datum known as 'Tokyo' and intended to be used only in the geographic region of Korea.
     ['HZD','tokyoKoreaCycle1','source:datum:geodetic','tokyo_(korea)_-_cycle_1'], // The geodetic datum known as 'Tokyo - Cycle 1' and intended to be used only in the geographic region of Korea.
     ['HZD','tokyoMeanValue','source:datum:geodetic','tokyo_(mean_value)'], // The geodetic datum known as 'Tokyo' and intended as a mean solution for the geographic region of Japan, Korea, and Okinawa.
     ['HZD','tokyoOkinawa','source:datum:geodetic','tokyo_(okinawa)'], // The geodetic datum known as 'Tokyo' and intended to be used only in the geographic region of Okinawa.
     ['HZD','trinidad1903','source:datum:geodetic','trinidad_1903'], // The geodetic datum known as 'Trinidad 1903' and whose geographic region of intended use is not specified.
     ['HZD','tristanAstro1968Cunha','source:datum:geodetic','tristan_astro_1968_(tristan_da_cunha)'], // The geodetic datum known as 'Tristan Astro 1968' and intended to be used only in the geographic region of Tristan da Cunha.
     ['HZD','vitiLevu1916FijiIslands','source:datum:geodetic','viti_levu_1916_(viti_levu_island_fiji_islands)'], // The geodetic datum known as 'Viti Levu 1916' and intended to be used only in the geographic region of Viti Levu Island, Fiji Islands.
     ['HZD','voirol1875','source:datum:geodetic','voirol_1875'], // The geodetic datum known as 'Voirol 1875' and intended to be used only in the geographic region of Algeria.
     ['HZD','voirol1875ZeroMeridParis','source:datum:geodetic','voirol_1875_with_zero_meridian_paris'], // The geodetic datum known as 'Voirol 1875' whose zero meridian is set at Paris and is intended to be used only in the geographic region of Algeria.
     ['HZD','voirol1960Algeria','source:datum:geodetic','voirol_1960_algeria'], // The geodetic datum known as 'Voirol 1960' and intended to be used only in the geographic region of Algeria.
     ['HZD','voirol1960ZeroMeridParis','source:datum:geodetic','voirol_1960_algeria_with_zero_meridian_paris'], // The geodetic datum known as 'Voirol 1960' whose zero meridian is set at Paris and is intended to be used only in the geographic region of Algeria.
     ['HZD','wakeIslandAstro1952','source:datum:geodetic','wake_island_astro_1952'], // The geodetic datum known as 'Wake Island 1952' which is based on astronomic observations and is intended to be used only in the geographic region of the Wake Atoll.
     ['HZD','wakeEniwetok1960MarshallIs','source:datum:geodetic','wake-eniwetok_1960_(marshall_islands)'], // The geodetic datum known as 'Wake-Eniwetok 1960' and intended to be used only in the geographic region of the Marshall Islands.
     ['HZD','worldGeodeticSystem1960','source:datum:geodetic','world_geodetic_system_1960'], // The geodetic datum known as 'World Geodetic System 1960' and intended to be used globally.
     ['HZD','worldGeodeticSystem1966','source:datum:geodetic','world_geodetic_system_1966'], // The geodetic datum known as 'World Geodetic System 1966' and intended to be used globally.
     ['HZD','worldGeodeticSystem1972','source:datum:geodetic','world_geodetic_system_1972'], // The geodetic datum known as 'World Geodetic System 1972' and intended to be used globally.
     ['HZD','yacareUruguay','source:datum:geodetic','yacare_(uruguay)'], // The geodetic datum known as 'Yacare' and intended to be used only in the geographic region of Uruguay.
     ['HZD','zanderijSurinam','source:datum:geodetic','zanderij_(surinam)'], // The geodetic datum known as 'Zanderij' and intended to be used only in the geographic region of Surinam.

     // INU - Inundation Type
     // ['INU','-999999',undefined,undefined], // No Information
     ['INU','1','inundation','controlled'], // Controlled
     ['INU','2','inundation','natural'], // Natural

     // IWT - Inland Water Type
     // ['IWT','-999999',undefined,undefined], // No Information
     ['IWT','1','water','lake'], // Lake
     ['IWT','2','water','pond'], // Pond
     ['IWT','3','water','undifferentiated_water_body'], // Undifferentiated Water Body
     ['IWT','4','water','reservoir'], // Reservoir
     ['IWT','5','landuse','basin'], // Basin
     ['IWT','6','water','water-hole'], // Water-hole
     ['IWT','7','water','landlocked_sea'], // Landlocked Sea
     ['IWT','999','water','other'], // Other

     // LMC - Navigation Landmark
     // ['LMC','-999999',undefined,undefined], // No Information
     ['LMC','1000','navigation:landmark','no'],
     ['LMC','1001','navigation:landmark','yes'],

     // LND - Land Morphology
     // ['LND','-999999',undefined,undefined], // No Information
     ['LND','19','land:morphology','dome'], // Dome
     ['LND','41','land:morphology','ridge'], // Ridge
     ['LND','56','land:morphology','depression'], // Depression
     ['LND','999','land:morphology','other'], // Other

     // LOC - Vertical Relative Location
     // ['LOC','-999999',undefined,undefined], // No Information
     ['LOC','17','location','on_waterbody_bottom'], // On Waterbody Bottom
     ['LOC','23','location','below_waterbody_bottom'], // Below Waterbody Bottom
     ['LOC','40','location','underground'], // Below Ground Surface
     ['LOC','44','location','surface'], // The default is on the surface // On Surface
     ['LOC','45','location','overground'], // Above Surface
     ['LOC','46','location','above_waterbody_bottom'], // Above Waterbody Bottom
     ['LOC','47','location','underwater'], // Below Waterbody Surface

     // LUN - Located Underground
     // ['LUN','-999999',undefined,undefined], // No Information
     ['LUN','1000','underground','no'],
     ['LUN','1001','underground','yes'],

     // MAN - Maritime Navigation Marked
     // ['MAN','-999999',undefined,undefined], // No Information
     ['MAN','1000','navigation:maritime_landmark','no'],
     ['MAN','1001','navigation:maritime_landmark','yes'],

     // MCC - Structural Material Type
     // ['MCC','-999999',undefined,undefined], // No Information
     ['MCC','2','material','aluminum'], // Aluminum
     ['MCC','5','material','asphalt'], // Asphalt
     ['MCC','7','material','bedrock'], // Bedrock
     ['MCC','8','material','boulders'], // Boulders
     ['MCC','9','material','brick'], // Brick
     ['MCC','21','material','concrete'], // Concrete
     ['MCC','46','material','gravel'], // Gravel
     ['MCC','51','material','iron'], // Iron
     ['MCC','52','material','lava'], // Lava
     ['MCC','55','material','loess'], // Loess
     ['MCC','62','material','masonry'], // Masonry
     ['MCC','64','material','metal'], // Metal
     ['MCC','65','material','mud'], // Mud
     ['MCC','74','material','plastic'], // Plastic
     ['MCC','77','material','prestressed_concrete'], // Prestressed Concrete
     ['MCC','83','material','reinforced_concrete'], // Reinforced Concrete
     ['MCC','84','material','rock'], // Rock
     ['MCC','86','material','rubble'], // Rubble
     ['MCC','88','material','sand'], // Sand
     ['MCC','103','material','frozen_water'], // Frozen Water
     ['MCC','104','material','soil'], // Soil
     ['MCC','107','material','steel'], // Steel
     ['MCC','108','material','stone'], // Stone
     ['MCC','117','material','wood'], // Wood
     ['MCC','119','material','evaporite'], // Evaporite
     ['MCC','150','material','treated_timber'], // Treated Timber
     ['MCC','152','material','fibreglass'], // Fibreglass
     ['MCC','999','material','other'], // Other

     // MCX - Motorized Crossing
     // ['MCX','-999999',undefined,undefined], // No Information
     ['MCX','1000','ferry:motorized','no'],
     ['MCX','1001','ferry:motorized','yes'],

     // MES - Median Present
     // ['MES','-999999',undefined,undefined], // No Information
     ['MES','1000','median','no'],
     ['MES','1001','median','yes'],

     // MFB - Manufactured Building
     // ['MFB','-999999',undefined,undefined], // No Information
     ['MFB','1000','building:manufactured','no'],
     ['MFB','1001','building:manufactured','yes'],

     // MNS - Man-made Shoreline
     // ['MNS','-999999',undefined,undefined], // No Information
     ['MNS','1000','man_made:shoreline','no'],
     ['MNS','1001','man_made:shoreline','yes'],

     // MST - Missile Site Type
     // ['MST','-999999',undefined,undefined], // No Information
     ['MST','1','missile','anti-ballistic_missile'], // Anti-ballistic Missile (ABM)
     ['MST','2','missile','intercontinental_ballistic_missile'], // Intercontinental Ballistic Missile (ICBM)
     ['MST','3','missile','intermediate-range_ballistic_missile'], // Intermediate-range Ballistic Missile (IRBM)
     ['MST','4','missile','sa-1_guild'], // SA-1 Guild
     ['MST','5','missile','sa-2_guideline'], // SA-2 Guideline
     ['MST','6','missile','sa-3_goa'], // SA-3 Goa
     ['MST','7','missile','sa-4_ganef'], // SA-4 Ganef
     ['MST','8','missile','sa-5_gammon'], // SA-5 Gammon
     ['MST','9','missile','sa-6_gainful'], // SA-6 Gainful
     ['MST','10','missile','sa-7_grail'], // SA-7 Grail
     ['MST','11','missile','sa-8_gecko'], // SA-8 Gecko
     ['MST','12','missile','sa-9_gaskin'], // SA-9 Gaskin
     ['MST','13','missile','medium-range_ballistic_missile'], // Medium-range Ballistic Missile (MRBM)
     ['MST','14','missile','surface-to-surface_missile'], // Surface-to-surface Missile (SSM)
     ['MST','15','missile','surface-to-air_missile'], // Surface-to-air Missile (SAM)
     ['MST','17','missile','sa-10_grumble'], // SA-10 Grumble
     ['MST','18','missile','sa-11_gadfly'], // SA-11 Gadfly
     ['MST','19','missile','sa-12_gladiator/giant'], // SA-12 Gladiator/Giant
     ['MST','20','missile','sa-13_gopher'], // SA-13 Gopher
     ['MST','21','missile','sa-14_gremlin'], // SA-14 Gremlin
     ['MST','22','missile','sa-15_gauntlet'], // SA-15 Gauntlet
     ['MST','23','missile','sa-16_gimlet'], // SA-16 Gimlet
     ['MST','24','missile','sa-17_grizzly'], // SA-17 Grizzly
     ['MST','25','missile','sa-18_grouse'], // SA-18 Grouse
     ['MST','26','missile','sa-19_grisom'], // SA-19 Grisom
     ['MST','999','missile','other'], // Other

     // MUB - Multi-unit Building
     // ['MUB','-999999',undefined,undefined], // No Information
     ['MUB','1000','building:multi_unit','no'],
     ['MUB','1001','building:multi_unit','yes'],

     // MZN - Extraction Mine Type
     // ['MZN','-999999',undefined,undefined], // No Information
     ['MZN','1','mine:type','borrow-pit'], // Borrow-pit
     ['MZN','3','mine:type','opencast'], // Opencast
     ['MZN','4','mine:type','placer'], // Placer
     ['MZN','5','mine:type','prospect'], // Prospect
     ['MZN','6','mine:type','area_strip-mine'], // Area Strip-mine
     ['MZN','8','mine:type','peatery'], // Peatery
     ['MZN','9','mine:type','below_surface'], // Below Surface
     //['MZN','10','mine:type','quarry'], // Quarry
     ['MZN','10','landuse','quarry'], // Quarry
     ['MZN','11','mine:type','contour_strip-mine'], // Contour Strip-mine
     ['MZN','12','mine:type','strip-mine'], // Strip-mine
     ['MZN','13','mine:type','dredge'], // Dredge
     ['MZN','999','mine:type','other'], // Other

     // NLT - Named Location Type
     // ['NLT','-999999',undefined,undefined], // No Information
     ['NLT','1','named_location:type','area'], // Area
     ['NLT','2','named_location:type','locality'], // Locality
     ['NLT','3','named_location:type','region'], // Region
     ['NLT','4','named_location:type','arctic_land'], // Arctic Land
     ['NLT','5','named_location:type','populated_locality'], // Populated Locality
     ['NLT','999','named_location:type','other'], // Other

     // NVS - Navigability Information
     // ['NVS','-999999',undefined,undefined], // No Information
     ['NVS','1','navigation','yes_operational'], // Navigable and Operational
     ['NVS','2','navigation','yes_abandoned'], // Navigable but Abandoned
     ['NVS','3','navigation','yes'], // Navigable
     ['NVS','4','navigation','yes_restrictions'], // Navigable with Periodic Restrictions
     ['NVS','5','navigation','no'], // Not Navigable
     ['NVS','999','navigation','other'], // Other

     // OCS - Offshore Construction Primary Structure
     // ['OCS','-999999',undefined,undefined], // No Information
     ['OCS','1','structure:type','platform'], // Platform
     ['OCS','2','structure:type','terminal_buoy'], // Terminal Buoy
     ['OCS','3','structure:type','catenary_turntable'], // Catenary Turntable
     ['OCS','4','structure:type','submerged_turret'], // Submerged Turret
     ['OCS','5','structure:type','vessel'], // Vessel
     ['OCS','6','structure:type','barge'], // Barge
     ['OCS','7','structure:type','submerged_platform'], // Submerged Platform
     ['OCS','999','structure:type','other'], // Other

     // ONE - One-way
     // ['ONE','-999999',undefined,undefined], // No Information
     ['ONE','1000','oneway','no'], // Value from OSM
     ['ONE','1001','oneway','yes'],

     // OOC - Overhead Obstruction Type
     // ['OOC','-999999',undefined,undefined], // No Information
     ['OOC','1','overhead_obstruction:type','frame_bridge_span'], // Frame Bridge Span
     ['OOC','2','overhead_obstruction:type','arch_bridge_span'], // Arch Bridge Span
     ['OOC','3','overhead_obstruction:type','roof'], // Roof
     ['OOC','4','overhead_obstruction:type','railway_power_line'], // Railway Power Line
     ['OOC','6','overhead_obstruction:type','bridge_superstructure'], // Bridge Superstructure
     ['OOC','7','overhead_obstruction:type','building'], // Building
     ['OOC','8','overhead_obstruction:type','bridge_span'], // Bridge Span
     ['OOC','9','overhead_obstruction:type','gantry'], // Gantry
     ['OOC','10','overhead_obstruction:type','scaffold'], // Scaffold
     ['OOC','11','overhead_obstruction:type','arcade'], // Arcade
     ['OOC','12','overhead_obstruction:type','building_overhang'], // Building Overhang
     ['OOC','13','overhead_obstruction:type','cable'], // Cable
     ['OOC','14','overhead_obstruction:type','cableway'], // Cableway
     ['OOC','15','overhead_obstruction:type','conveyor'], // Conveyor
     ['OOC','16','overhead_obstruction:type','entrance_and/or_exit'], // Entrance and/or Exit
     ['OOC','17','overhead_obstruction:type','memorial_monument'], // Memorial Monument
     ['OOC','18','overhead_obstruction:type','non-building_structure'], // Non-building Structure
     ['OOC','19','overhead_obstruction:type','overhead_walkway'], // Overhead Walkway
     ['OOC','20','overhead_obstruction:type','parking_garage'], // Parking Garage
     ['OOC','21','overhead_obstruction:type','pipeline'], // Pipeline
     ['OOC','22','overhead_obstruction:type','pipeline_crossing_point'], // Pipeline Crossing Point
     ['OOC','23','overhead_obstruction:type','route-related_structure'], // Route-related Structure
     ['OOC','24','overhead_obstruction:type','transportation_block'], // Transportation Block
     ['OOC','25','overhead_obstruction:type','transportation_route_protection_structure'], // Transportation Route Protection Structure
     ['OOC','26','overhead_obstruction:type','tunnel'], // Tunnel
     ['OOC','27','overhead_obstruction:type','traffic_sign'], // Traffic Sign
     ['OOC','999','overhead_obstruction:type','other'], // Other

     // OPT - Operating Cycle
     // ['OPT','-999999',undefined,undefined], // No Information
     ['OPT','1','operating_cycle','daytime'], // Daytime
     ['OPT','2','operating_cycle','night-time'], // Night-time
     ['OPT','3','operating_cycle','continuous'], // Continuously Operating
     ['OPT','4','operating_cycle','summer_season'], // Summer Season
     ['OPT','5','operating_cycle','winter_season'], // Winter Season
     ['OPT','6','operating_cycle','restricted'], // Restricted
     ['OPT','7','operating_cycle','never_operating'], // Never Operating
     ['OPT','999','operating_cycle','other'], // Other

     // OWO - Waterbody Overhead Obstruction
     // ['OWO','-999999',undefined,undefined], // No Information
     ['OWO','1000','waterway:overhead_obstruction','no'],
     ['OWO','1001','waterway:overhead_obstruction','yes'],

//      // PBY - By-product - See ZI014PBY
//      // ['PBY','-999999',undefined,undefined], // No Information
//      ['PBY','8','raw:PBY','refuse'], // Refuse
//      ['PBY','15','raw:PBY','slag'], // Slag
//      ['PBY','16','raw:PBY','sludge'], // Sludge
//      ['PBY','17','raw:PBY','spoil'], // Spoil
//      ['PBY','999','raw:PBY','other'], // Other

     // PCF - Physical Condition
     // ['PCF','-999999',undefined,undefined], // No Information
     ['PCF','1','condition','construction'], // Construction
     ['PCF','2','condition','functional'], // Intact in spec, using for MGCP compatibility
     ['PCF','3','condition','abandoned'], // Unmaintained in spec
     ['PCF','4','condition','damaged'], // Damaged
     ['PCF','5','condition','dismantled'], // Dismantled
     ['PCF','6','condition','destroyed'], // Destroyed

     // PEC - Port of Entry
     // ['PEC','-999999',undefined,undefined], // No Information
     ['PEC','1000','port_of_entry','no'],
     ['PEC','1001','port_of_entry','yes'],

     // PLT - Pipeline Type
     // ['PLT','-999999',undefined,undefined], // No Information
     ['PLT','1','pipeline:type','transport'], // Transport Pipe
     ['PLT','2','pipeline:type','outfall'], // Outfall Pipe
     ['PLT','3','pipeline:type','intake'], // Intake Pipe
     ['PLT','4','pipeline:type','sewer'], // Sewer
     ['PLT','5','pipeline:type','pipeline_valve'], // Pipeline Valve
     ['PLT','7','pipeline:type','bubbler_system'], // Bubbler System
     ['PLT','999','pipeline:type','other'], // Other

     // POS - Power Source
     // ['POS','-999999',undefined,undefined], // No Information
     ['POS','1','generator:source','geothermal'], // Geothermal
     ['POS','2','generator:source','hydro'], // Hydro-electric
     ['POS','3','generator:source','nuclear'], // Nuclear
     ['POS','4','generator:source','thermal'], // Thermal
     ['POS','5','generator:source','tidal'], // Tidal
     ['POS','999','generator:source','other'], // Other

     // This attribute is handled in Pre and Post processing. See ZI014_PPO
//      // PPO - Product
//      // ['PPO','-999999',undefined,undefined], // No Information
//      ['PPO','1','raw:PPO','aircraft'], // Aircraft
//      ['PPO','3','raw:PPO','ammunition'], // Ammunition
//      ['PPO','8','raw:PPO','basalt'], // Basalt
//      ['PPO','9','raw:PPO','bauxite'], // Bauxite
//      ['PPO','11','raw:PPO','bivalve_mollusc'], // Bivalve Mollusc
//      ['PPO','13','raw:PPO','brick'], // Brick
//      ['PPO','15','raw:PPO','cement'], // Cement
//      ['PPO','16','raw:PPO','chemical'], // Chemical
//      ['PPO','17','raw:PPO','clay'], // Clay
//      ['PPO','18','raw:PPO','coal'], // Coal
//      ['PPO','19','raw:PPO','cobbles'], // Cobbles
//      ['PPO','21','raw:PPO','coke'], // Coke
//      ['PPO','26','raw:PPO','copper'], // Copper
//      ['PPO','29','raw:PPO','crustacean'], // Crustacean
//      ['PPO','30','raw:PPO','cultivated_shellfish'], // Cultivated Shellfish
//      ['PPO','33','raw:PPO','diamond'], // Diamond
//      ['PPO','34','raw:PPO','diatomaceous_earth'], // Diatomaceous Earth
//      ['PPO','35','raw:PPO','dolomite'], // Dolomite
//      ['PPO','37','raw:PPO','electric_power'], // Electric Power
//      ['PPO','38','raw:PPO','explosive'], // Explosive
//      ['PPO','39','raw:PPO','fish'], // Fish
//      ['PPO','45','raw:PPO','gas'], // Gas
//      ['PPO','46','raw:PPO','petrol'], // Petrol
//      ['PPO','48','raw:PPO','gold'], // Gold
//      ['PPO','50','raw:PPO','granite'], // Granite
//      ['PPO','53','raw:PPO','gravel'], // Gravel
//      ['PPO','58','raw:PPO','iron'], // Iron
//      ['PPO','59','raw:PPO','lead'], // Lead
//      ['PPO','60','raw:PPO','lime'], // Lime
//      ['PPO','61','raw:PPO','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
//      ['PPO','62','raw:PPO','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
//      ['PPO','63','raw:PPO','lumber'], // Lumber
//      ['PPO','65','raw:PPO','manganese'], // Manganese
//      ['PPO','66','raw:PPO','marble'], // Marble
//      ['PPO','72','raw:PPO','mussels'], // Mussels
//      ['PPO','77','raw:PPO','oysters'], // Oysters
//      ['PPO','83','raw:PPO','petroleum'], // Petroleum
//      ['PPO','85','raw:PPO','porphyry'], // Porphyry
//      ['PPO','88','raw:PPO','pumice'], // Pumice
//      ['PPO','89','raw:PPO','quartz'], // Quartz
//      ['PPO','90','raw:PPO','radioactive_material'], // Radioactive Material
//      ['PPO','93','raw:PPO','rock'], // Rock
//      ['PPO','95','raw:PPO','salt'], // Salt
//      ['PPO','96','raw:PPO','sand'], // Sand
//      ['PPO','97','raw:PPO','sandstone'], // Sandstone
//      ['PPO','101','raw:PPO','sewage'], // Sewage
//      ['PPO','105','raw:PPO','silver'], // Silver
//      ['PPO','110','raw:PPO','stone'], // Stone
//      ['PPO','116','raw:PPO','timber'], // Timber
//      ['PPO','117','raw:PPO','tobacco'], // Tobacco
//      ['PPO','118','raw:PPO','travertine'], // Travertine
//      ['PPO','120','raw:PPO','uranium'], // Uranium
//      ['PPO','122','raw:PPO','water'], // Water
//      ['PPO','126','raw:PPO','zinc'], // Zinc
//      ['PPO','130','raw:PPO','chalk'], // Chalk
//      ['PPO','146','raw:PPO','heating_steam_and/or_water'], // Heating Steam and/or Water
//      ['PPO','149','raw:PPO','brine'], // Brine
//      ['PPO','151','raw:PPO','chromium'], // Chromium
//      ['PPO','152','raw:PPO','nickel'], // Nickel
//      ['PPO','153','raw:PPO','tin'], // Tin
//      ['PPO','158','raw:PPO','pottery'], // Pottery
//      ['PPO','159','raw:PPO','charcoal'], // Charcoal
//      ['PPO','160','raw:PPO','milled_grain'], // Milled Grain
//      ['PPO','161','raw:PPO','coalbed_methane'], // Coalbed Methane
//      ['PPO','162','raw:PPO','natural_gas_condensate'], // Natural Gas Condensate
//      ['PPO','163','raw:PPO','helium'], // Helium
//      ['PPO','164','raw:PPO','hydrothermal_fluid'], // Hydrothermal Fluid
//      ['PPO','170','raw:PPO','beverage'], // Beverage
//      ['PPO','173','raw:PPO','fluorite'], // Fluorite
//      ['PPO','204','raw:PPO','phosphate'], // Phosphate
//      ['PPO','214','raw:PPO','biodiesel'], // Biodiesel
//      ['PPO','224','raw:PPO','vanadium'], // Vanadium
//      ['PPO','283','raw:PPO','mica'], // Mica
//      ['PPO','339','raw:PPO','selenium'], // Selenium
//      ['PPO','999','raw:PPO','other'], // Other

     // PRM - Permanent
     // ['PRM','-999999',undefined,undefined], // No Information
     ['PRM','1000','permanent','no'],
     ['PRM','1001','permanent','yes'],

     // This attribute is handled in Pre and Post processing. See ZI014_PRW
     // PRW - Raw Material
     // ['PRW','-999999',undefined,undefined], // No Information
//      ['PRW','12','raw:PRW','coal'], // Coal
//      ['PRW','14','raw:PRW','coke'], // Coke
//      ['PRW','34','raw:PRW','gravel'], // Gravel
//      ['PRW','54','raw:PRW','ore'], // Ore
//      ['PRW','67','raw:PRW','salt'], // Salt
//      ['PRW','68','raw:PRW','sand'], // Sand
//      ['PRW','84','raw:PRW','stone'], // Stone
//      ['PRW','154','raw:PRW','sulphur'], // Sulphur
//      ['PRW','999','raw:PRW','other'], // Other

     // PWAC - Predominant Water Depth <interval closure>
     ['PWAC','2','depth:closure','open_interval'], // Open Interval
     ['PWAC','3','depth:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['PWAC','4','depth:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['PWAC','5','depth:closure','closed_interval'], // Closed Interval
     ['PWAC','6','depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['PWAC','7','depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['PWAC','8','depth:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['PWAC','9','depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // PWC - Shoreline Construction Type
     // ['PWC','-999999',undefined,undefined], // No Information
     ['PWC','1','man_made','pier'], // Pier
     ['PWC','2','waterway','dock'], // Wharf
     ['PWC','3','man_made','quay'], // Quay
     ['PWC','4','man_made','breakwater'], // Breakwater
     ['PWC','5','man_made','groyne'], // Groin
     ['PWC','6','man_made','mole'], // Mole
     ['PWC','7','man_made','recreational_pier'], // Recreational Pier
     ['PWC','8','man_made','training_wall'], // Training Wall
     ['PWC','9','construction:type','riprap'], // Riprap
     ['PWC','10','man_made','marine_revetment'], // Revetment (Marine)
     ['PWC','11','wall','seawall'], // Seawall
     ['PWC','18','man_made','promenade'], // Promenade
     ['PWC','999','construction:type','other'], // Other

     // PYC - Pylon Configuration
     // ['PYC','-999999',undefined,undefined], // No Information
     ['PYC','1','design','a-frame'], // 'A'
     ['PYC','2','design','h-frame'], // 'H'
     ['PYC','3','design','i-frame'], // 'I'
     ['PYC','4','design','y-frame'], // 'Y'
     ['PYC','5','design','t-frame'], // 'T'
     ['PYC','999','design','other'], // Other

     // PYM - Pylon Material
     // ['PYM','-999999',undefined,undefined], // No Information
     ['PYM','1','tower:material','aluminum'], // Aluminum
     ['PYM','3','tower:material','concrete'], // Concrete
     ['PYM','4','tower:material','masonry'], // Masonry
     ['PYM','5','tower:material','metal'], // Metal
     ['PYM','7','tower:material','wood'], // Wood
     ['PYM','8','tower:material','steel'], // Steel
     ['PYM','9','tower:material','fibreglass'], // Fibreglass
     ['PYM','10','tower:material','iron'], // Iron
     ['PYM','999','tower:material','other'], // Other

     // RAC - Radar Antenna Configuration
     // ['RAC','-999999',undefined,undefined], // No Information
     ['RAC','1','radar:configuration','dome_enclosed'], // Dome Enclosed
     ['RAC','2','radar:configuration','mast_mounted'], // Mast Mounted
     ['RAC','3','radar:configuration','radome'], // Radome
     ['RAC','4','radar:configuration','radome_on_tower'], // Radome on Tower
     ['RAC','5','radar:configuration','scanner'], // Scanner
     ['RAC','6','radar:configuration','tower_mounted'], // Tower Mounted
     ['RAC','999','radar:configuration','other'], // Other

     // RAS - Radar Station Function
     // ['RAS','-999999',undefined,undefined], // No Information
     ['RAS','1','radar:use','general_surveillance'], // General Surveillance
     ['RAS','2','radar:use','coastal_radar'], // Coastal Radar
     ['RAS','5','radar:use','early_warning'], // Early Warning
     ['RAS','6','radar:use','weather'], // Weather
     ['RAS','7','radar:use','satellite_tracking'], // Satellite Tracking
     ['RAS','8','radar:use','aircraft_flight_tracking'], // Aircraft Flight Tracking
     ['RAS','9','radar:use','fire_control_tracking'], // Fire Control Tracking
     ['RAS','10','radar:use','launch_control_tracking'], // Launch Control Tracking
     ['RAS','11','radar:use','aerodrome_ground_surveillance'], // Aerodrome Ground Surveillance
     ['RAS','12','radar:use','precision_approach'], // Precision Approach
     ['RAS','999','radar:use','other'], // Other

     // RFD - Roofed (really?)
     // ['RFD','-999999',undefined,undefined], // No Information
     ['RFD','1000','bridge:roof','no'],
     ['RFD','1001','bridge:roof','yes'],

     // RIN_ROI - Route Designation (route designation type)
     // ['RIN_ROI','-999999',undefined,undefined], // No Information
     ['RIN_ROI','1','ref:road:class','international'], // International
     ['RIN_ROI','2','ref:road:class','national_motorway'], // National Motorway
     ['RIN_ROI','3','ref:road:class','primary'], // National
     ['RIN_ROI','4','ref:road:class','secondary'], // Secondary
     ['RIN_ROI','5','ref:road:class','local'], // Local
     ['RIN_ROI','999','ref:road:class','other'], // Other

     // RIP - Rig Present
     // ['RIP','-999999',undefined,undefined], // No Information
     ['RIP','1000','rig','no'],
     ['RIP','1001','rig','yes'],

     // RIT - Road Interchange Type
     // ['RIT','-999999',undefined,undefined], // No Information
     ['RIT','1','junction','cloverleaf'], // Cloverleaf
     ['RIT','2','junction','diamond'], // Diamond
     ['RIT','3','junction','fork'], // Fork
     ['RIT','4','junction','rotary'], // Rotary
     ['RIT','5','junction','staggered_ramps'], // Staggered Ramps
     ['RIT','6','junction','standard_ramps'], // Standard Ramps
     ['RIT','7','junction','symmetrical_ramps'], // Symmetrical Ramps
     ['RIT','8','junction','trumpet'], // Trumpet
     ['RIT','9','junction','turban'], // Turban
     ['RIT','10','junction','wye'], // Wye
     ['RIT','999','junction','other'], // Other

     // RKF - Rock Formation Structure
     // ['RKF','-999999',undefined,undefined], // No Information
     ['RKF','1','rock_formation','columnar'], // Columnar
     ['RKF','2','rock_formation','needle'], // Needle
     ['RKF','3','rock_formation','pinnacle'], // Pinnacle
     ['RKF','4','rock_formation','fossilized_forest'], // Fossilized Forest
     ['RKF','999','rock_formation','other'], // Other

     // RLE - Relative Level: Need to do-conflict with LOC
     // ['RLE','-999999',undefined,undefined], // No Information
     ['RLE','1','layer','1'], // Raised
     ['RLE','2','layer','0'], // Level with surrounding area
     ['RLE','3','layer','-1'], // Depressed
     ['RLE','998',undefined,undefined], // Not Applicable

     // RMWC - Route Median Width (interval closure): Nothing in OSM
     // ['RMWC','-999999',undefined,undefined],
     ['RMWC','2','median:interval:closure','open_interval'], // Open Interval
     ['RMWC','3','median:interval:closure','greater_than_or_equal_to_less_than_interval'], // Greater-than-or-equal to Less-than Interval
     ['RMWC','4','median:interval:closure','greater_than_to_less_than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['RMWC','5','median:interval:closure','closed_interval'], // Closed Interval
     ['RMWC','6','median:interval:closure','greater_than_semi_interval'], // Greater-than Semi-interval
     ['RMWC','7','median:interval:closure','greater_than_or_equal_semi_interval'], // Greater-than or Equal Semi-interval
     ['RMWC','8','median:interval:closure','less_than_semi_interval'], // Less-than Semi-interval
     ['RMWC','9','median:interval:closure','less_than_or_equal_semi_interval'], // Less-than or Equal Semi-interval

     // This gets fixed in pre/post processing
     // ROR - Road Interchange Ramp
     // ['ROR','-999999',undefined,undefined], // No Information
     ['ROR','1000',undefined,undefined], // The default: No ramp
     ['ROR','1001','interchange_ramp','yes'],

     // RRC - Railway Use
     // ['RRC','-999999',undefined,undefined], // No Information
     // ['RRC','0','railway','rail'],
     ['RRC','2','railway','carline'], // Carline
     ['RRC','6','railway','subway'], // Underground Railway
     ['RRC','8','railway','logging'], // Logging
     ['RRC','11','railway','rapid_transit'], // Rail Rapid Transit
     ['RRC','13','railway','marine_railway'], // Marine Railway
     ['RRC','14','railway','tram'], // Tramway
     ['RRC','15','railway','funicular'], // Funicular
     ['RRC','24','railway','museum'], // Museum
     ['RRC','32','railway','automated_transit_system'], // Automated Transit System
     ['RRC','33','railway','longhaul'], // Long-haul
     ['RRC','999','railway','Other'], // Other

     // RSA - Branch Railway Type
     // ['RSA','-999999',undefined,undefined], // No Information
     ['RSA','1','service','spur'], // Spur
     ['RSA','2','service','siding'], // Siding
     ['RSA','3','service','passing'], // Passing
     ['RSA','999','service','other'], // Other

     // RTA - Linear Feature Arrangement
     // ['RTA','-999999',undefined,undefined], // No Information
     // ['RTA','0',undefined,undefined], // This also gets used by pipelines
     ['RTA','1','arrangement','single'], // Single Arrangement
     ['RTA','2','arrangement','double'], // Double Arrangement
     ['RTA','3','arrangement','multiple'], // Multiple Arrangements
     ['RTA','999','arrangement','other'], // Other

     // These give the basic highway types. Post processing fixes them
     // RTY - Roadway Type
     // ['RTY','-999999',undefined,undefined], // No Information
     ['RTY','1','ref:road:type','motorway'], // Motorway
     ['RTY','2','ref:road:type','limited_access_motorway'], // Limited Access Motorway
     ['RTY','3','ref:road:type','road'], // Road - Def: Roads outside a BUA
     ['RTY','4','ref:road:type','street'], // Street - Def: Roads inside a BUA
     ['RTY','999','ref:road:type','other'], // Other

     // RWC - Railway Class
     // ['RWC','-999999',undefined,undefined], // No Information
     ['RWC','1','usage','high_speed_rail'], // High Speed Rail
     ['RWC','2','usage','main'], // Main Line
     ['RWC','3','usage','branch'], // Branch-line
     ['RWC','999','usage','other'], // Other

     // SAD - Sand Dune Stabilized
     // ['SAD','-999999',undefined,undefined], // No Information
     ['SAD','1000','stabilized','no'],
     ['SAD','1001','stabilized','yes'],

     // SAX_RS6 - Security Attributes Group <resource declassification exception>
     ['SAX_RS6','AEA','security:declassification:exception','AEA'],
     ['SAX_RS6','25X1','security:declassification:exception','25X1'],
     ['SAX_RS6','25X1-human','security:declassification:exception','25X1-human'],
     ['SAX_RS6','25X1-EO-12951','security:declassification:exception','25X1-EO-12951'],
     ['SAX_RS6','25X2','security:declassification:exception','25X2'],
     ['SAX_RS6','25X3','security:declassification:exception','25X3'],
     ['SAX_RS6','25X4','security:declassification:exception','25X4'],
     ['SAX_RS6','25X5','security:declassification:exception','25X5'],
     ['SAX_RS6','25X6','security:declassification:exception','25X6'],
     ['SAX_RS6','25X7','security:declassification:exception','25X7'],
     ['SAX_RS6','25X8','security:declassification:exception','25X8'],
     ['SAX_RS6','25X9','security:declassification:exception','25X9'],
     ['SAX_RS6','50X1-HUM','security:declassification:exception','50X1-HUM'],
     ['SAX_RS6','50X2-WMD','security:declassification:exception','50X2-WMD'],

     // SAX_RX8 - Security Attributes Group <resource type of exempted source>
     ['SAX_RX8','OADR','security:exempted_source:type','OADR'], // Source Marked OADR (Originating Agency's Determination Required)
     ['SAX_RX8','X1','security:exempted_source:type','X1'], // Source Marked X1
     ['SAX_RX8','X2','security:exempted_source:type','X2'], // Source Marked X2
     ['SAX_RX8','X3','security:exempted_source:type','X3'], // Source Marked X3
     ['SAX_RX8','X4','security:exempted_source:type','X4'], // Source Marked X4
     ['SAX_RX8','X5','security:exempted_source:type','X5'], // Source Marked X5
     ['SAX_RX8','X6','security:exempted_source:type','X6'], // Source Marked X6
     ['SAX_RX8','X7','security:exempted_source:type','X7'], // Source Marked X7
     ['SAX_RX8','X8','security:exempted_source:type','X8'], // Source Marked X8

     // SBB - Supported by Bridge Span
     // ['SBB','-999999',undefined,undefined], // No Information
     ['SBB','1000','on_bridge','no'], // Do we need this??
     ['SBB','1001','on_bridge','yes'],

     // SBC - Shelter Belt
     // ['SBC','-999999',undefined,undefined], // No Information
     ['SBC','1000','shelter_belt','no'],
     ['SBC','1001','shelter_belt','yes'],

     // SBT - Substation Type: Not a good translation.
     // ['SBT','-999999',undefined,undefined], // No Information
     ['SBT','1','substation','switched'], // Switched Substation
     ['SBT','2','substation','transformer'], // Transformer Substation
     ['SBT','3','substation','converter'], // Converter Substation
     ['SBT','999','substation','other'], // Other

     // SDCC - Soil Depth <interval closure>
     ['SDCC','2','soil:depth:closure','open_interval'], // Open Interval
     ['SDCC','3','soil:depth:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['SDCC','4','soil:depth:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['SDCC','5','soil:depth:closure','closed_interval'], // Closed Interval
     ['SDCC','6','soil:depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['SDCC','7','soil:depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['SDCC','8','soil:depth:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['SDCC','9','soil:depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // SDQ - Structurally-dissected
     // ['SDQ','-999999',undefined,undefined], // No Information
     ['SDQ','1000','structurally_dissected','no'],
     ['SDQ','1001','structurally_dissected','yes'],

     // SDSC - Stem Diameter (interval closure)
     ['SDSC','2','tree:diameter:closure','open_interval'], // Open Interval
     ['SDSC','3','tree:diameter:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['SDSC','4','tree:diameter:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['SDSC','5','tree:diameter:closure','closed_interval'], // Closed Interval
     ['SDSC','6','tree:diameter:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['SDSC','7','tree:diameter:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['SDSC','8','tree:diameter:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['SDSC','9','tree:diameter:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // SDT - Sand Dune Type
     // ['SDT','-999999',undefined,undefined], // No Information
     ['SDT','1','dune:type','crescent'], // Crescent
     ['SDT','2','dune:type','dome'], // Dome
     ['SDT','3','dune:type','transverse'], // Transverse
     ['SDT','4','dune:type','linear'], // Linear
     ['SDT','5','dune:type','parabolic'], // Parabolic
     ['SDT','6','dune:type','ripple'], // Ripple
     ['SDT','7','dune:type','star'], // Star
     ['SDT','8','dune:type','dome_and_transverse'], // Dome and Transverse
     ['SDT','999','dune:type','other'], // Other

     // SEP - Divided
     // ['SEP','-999999',undefined,undefined], // No Information
     ['SEP','1000','is_divided','no'],
     ['SEP','1001','is_divided','yes'],

     // SGCC - Surface Slope (interval closure)
     ['SGCC','2','incline:interval:closure','open_interval'], // Open Interval
     ['SGCC','3','incline:interval:closure','greater_than_or_equal_to_less_than_interval'], // Greater-than-or-equal to Less-than Interval
     ['SGCC','4','incline:interval:closure','greater_than_to_less_than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['SGCC','5','incline:interval:closure','closed_interval'], // Closed Interval
     ['SGCC','6','incline:interval:closure','greater_than_semi_interval'], // Greater-than Semi-interval
     ['SGCC','7','incline:interval:closure','greater_than_or_equal_semi_interval'], // Greater-than or Equal Semi-interval
     ['SGCC','8','incline:interval:closure','less_than_semi_interval'], // Less-than Semi-interval
     ['SGCC','9','incline:interval:closure','less_than_or_equal_semi_interval'], // Less-than or Equal Semi-interval

     // SHD - Shoreline Delineated
     // ['SHD','-999999',undefined,undefined], // No Information
     ['SHD','1000','shoreline:delineated','no'],
     ['SHD','1001','shoreline:delineated','yes'],

     // SHT - Shipping Container Type
     // ['SHT','-999999',undefined,undefined], // No Information
     ['SHT','1','shipping_container:type','deliverable_container'], // Deliverable Container
     ['SHT','2','shipping_container:type','dumpster'], // Dumpster
     ['SHT','3','shipping_container:type','iso_container'], // ISO Container
     ['SHT','4','shipping_container:type','roll-off_dumpster'], // Roll-off Dumpster
     ['SHT','5','shipping_container:type','tank_container'], // Tank Container
     ['SHT','999','shipping_container:type','other'], // Other

     // SIC - Frozen Cover Type
     // ['SIC','-999999',undefined,undefined], // No Information
     ['SIC','1','landcover','snowfield'], // Snowfield
     ['SIC','2','landcover','ice-field'], // Ice-field

     // SLR - Shoreline Ramp Type
     // ['SLR','-999999',undefined,undefined], // No Information
     ['SLR','1','shoreline:ramp','log'], // Log Ramp
     ['SLR','2','shoreline:ramp','marine'], // Marine Ramp
     ['SLR','3','leisure','slipway'], // Slipway
     ['SLR','999','shoreline:ramp','other'], // Other

     // ['SLT','-999999',undefined,undefined], // No Information
     // SLT - Shoreline Type
     ['SLT','6','shoreline:type','mangrove'], // Mangrove
     ['SLT','8','shoreline:type','marshy'], // Marshy
     ['SLT','10','shoreline:type','stony'], // Stony
     ['SLT','11','shoreline:type','building_rubble'], // Building Rubble
     ['SLT','12','shoreline:type','erosion_rubble'], // Erosion Rubble
     ['SLT','13','shoreline:type','sandy'], // Sandy
     ['SLT','14','shoreline:type','shingly'], // Shingly
     ['SLT','16','shoreline:type','coral'], // Coral
     ['SLT','17','shoreline:type','ice'], // Ice
     ['SLT','18','shoreline:type','mud'], // Mud
     // ['SLT','998','shoreline:type','not_applicable'], // Not Applicable
     ['SLT','998',undefined,undefined], // Not Applicable
     ['SLT','999','shoreline:type','other'], // Other

     // SPT - Supported
     // ['SPT','-999999',undefined,undefined], // No Information
     ['SPT','1000','supported','no'],
     ['SPT','1001','supported','yes'],

     // SRD - Terrain Morphology
     // ['SRD','-999999',undefined,undefined], // No Information
     ['SRD','1','terrain:morphology','no_effect'], // No Effect
     ['SRD','2','terrain:morphology','landslide_potential'], // Landslide Potential
     ['SRD','11','terrain:morphology','numerous_cobbles_and_boulders'], // Numerous Cobbles and Boulders
     ['SRD','12','terrain:morphology','stony_areas'], // Stony Areas
     ['SRD','13','terrain:morphology','stony_soil_with_surface_rock'], // Stony Soil with Surface Rock
     ['SRD','14','terrain:morphology','stony_soil_with_scattered_boulders'], // Stony Soil with scattered Boulders
     ['SRD','15','terrain:morphology','stony_soil_with_numerous_boulders'], // Stony Soil with numerous Boulders
     ['SRD','16','terrain:morphology','numerous_boulders'], // Numerous Boulders
     ['SRD','17','terrain:morphology','numerous_rock_outcrops'], // Numerous Rock Outcrops
     ['SRD','18','terrain:morphology','scattered_boulders'], // Scattered Boulders
     ['SRD','19','terrain:morphology','talus'], // Talus
     ['SRD','20','terrain:morphology','boulder_field'], // Boulder Field
     ['SRD','31','terrain:morphology','highly_fractured_rock'], // Highly Fractured Rock
     ['SRD','32','terrain:morphology','weathered_lava'], // Weathered Lava
     ['SRD','33','terrain:morphology','unweathered_lava'], // Unweathered Lava
     ['SRD','34','terrain:morphology','stony_soil_with_numerous_rock_outcrops'], // Stony Soil with numerous Rock Outcrops
     ['SRD','35','terrain:morphology','irregular_with_deep_foliation_fractures'], // Irregular with deep Foliation Fractures
     ['SRD','36','terrain:morphology','rugged_with_numerous_rock_outcrops'], // Rugged with numerous Rock Outcrops
     ['SRD','37','terrain:morphology','rugged_bedrock'], // Rugged Bedrock
     ['SRD','43','terrain:morphology','highly_distorted_with_sharp_rocky_ridges'], // Highly Distorted with sharp Rocky Ridges
     ['SRD','51','terrain:morphology','stony_soil_with_numerous_gullies'], // Stony Soil with numerous Gullies
     ['SRD','52','terrain:morphology','moderately_dissected'], // Moderately Dissected
     ['SRD','53','terrain:morphology','moderately_dissected_with_scattered_rock_outcrops'], // Moderately Dissected with scattered Rock Outcrops
     ['SRD','54','terrain:morphology','dissected_floodplain'], // Dissected Floodplain
     ['SRD','55','terrain:morphology','highly_dissected'], // Highly Dissected
     ['SRD','56','terrain:morphology','deep_erosional_gullies'], // Deep Erosional Gullies
     ['SRD','57','terrain:morphology','steep_rugged_dissected_with_narrow_gullies'], // Steep Rugged Dissected with narrow Gullies
     ['SRD','58','terrain:morphology','karst_with_numerous_sinkholes_and_solution_valleys'], // Karst with numerous Sinkholes and Solution Valleys
     ['SRD','59','terrain:morphology','karst_with_numerous_sinkholes'], // Karst with numerous Sinkholes
     ['SRD','60','terrain:morphology','hummocky_karst_with_large_hills'], // Hummocky Karst with Large Hills
     ['SRD','61','terrain:morphology','hummocky_karst_with_low_mounds'], // Hummocky Karst with Low Mounds
     ['SRD','63','terrain:morphology','playa'], // Playa
     ['SRD','64','terrain:morphology','meander_scars_and_lakes'], // Meander Scars and Lakes
     ['SRD','65','terrain:morphology','solifluction_lobes_and_frost_scars'], // Solifluction Lobes and Frost Scars
     ['SRD','66','terrain:morphology','hummocky_with_frost_heaves'], // Hummocky with Frost Heaves
     ['SRD','67','terrain:morphology','frost_polygons'], // Frost Polygons
     ['SRD','69','terrain:morphology','numerous_small_lakes_and_ponds'], // Numerous Small Lakes and Ponds
     ['SRD','70','terrain:morphology','numerous_crevasses'], // Numerous Crevasses
     ['SRD','81','terrain:morphology','numerous_terraces'], // Numerous Terraces
     ['SRD','86','terrain:morphology','mine_tailing(s)'], // Mine Tailing(s)
     ['SRD','88','terrain:morphology','numerous_dykes'], // Numerous Dykes
     ['SRD','89','terrain:morphology','numerous_dyked_fields'], // Numerous Dyked Fields
     ['SRD','90','terrain:morphology','numerous_fences'], // Numerous Fences
     ['SRD','91','terrain:morphology','numerous_stone_walls'], // Numerous Stone Walls
     ['SRD','92','terrain:morphology','numerous_man-made_drainage'], // Numerous Man-made Drainage
     ['SRD','93','terrain:morphology','numerous_terraced_fields'], // Numerous Terraced Fields
     ['SRD','94','terrain:morphology','parallel_earthen_rows'], // Parallel Earthen Rows
     ['SRD','95','terrain:morphology','numerous_hedgerows'], // Numerous Hedgerows
     ['SRD','999','terrain:morphology','other'], // Other

     // SRE - Traffic Restriction Type
     // ['SRE','-999999',undefined,undefined], // No Information
     ['SRE','1','traffic_restriction','railway_switch'], // Railway Switch
     ['SRE','2','traffic_restriction','reduced_track_or_lane_count'], // Reduced Track or Lane Count
     ['SRE','3','traffic_restriction','road_interchange'], // Road Interchange
     ['SRE','8','traffic_restriction','sharp_curves'], // Sharp Curves
     ['SRE','9','traffic_restriction','steep_grades'], // Steep Grades
     ['SRE','999','traffic_restriction','other'], // Other

     // SRL - Location Referenced to Shoreline -- deconflicted with LOC
     // ['SRL','-999999',undefined,undefined], // No Information
     ['SRL','1','location','offshore'], // Offshore
     ['SRL','2','location','inland'], // Inland
     ['SRL','3','location','at_shoreline'], // At Shoreline

     // SSC - Structure Shape
     // ['SSC','-999999',undefined,undefined], // No Information
     ['SSC','2','shape','horizontal_capped_cylindrical'], // Horizontal Capped Cylindrical
     ['SSC','4','shape','vertical_capped_cylindrical'], // Vertical Capped Cylindrical
     ['SSC','12','shape','pyramidal'], // Pyramidal
     ['SSC','17','shape','spherical'], // Spherical
     ['SSC','59','shape','cylindrical_with_framework'], // Cylindrical with Framework
     ['SSC','65','shape','cylindrical_with_flat_top'], // Cylindrical with Flat Top
     ['SSC','66','shape','cylindrical_with_domed_top'], // Cylindrical with Domed Top
     ['SSC','71','shape','cylindrical_with_conical_top'], // Cylindrical with Conical Top
     ['SSC','77','shape','arched'], // Arched
     ['SSC','78','shape','multiple_arched'], // Multiple Arched
     ['SSC','87','shape','domed'], // Domed
     ['SSC','88','shape','spherical_on_column'], // Spherical on Column
     ['SSC','89','shape','cylindrical_on_tower'], // Cylindrical on Tower
     ['SSC','91','shape','vertical_cylindrical'], // Vertical Cylindrical
     ['SSC','92','shape','cubic'], // Cubic
     ['SSC','94','shape','board-like_prism'], // Board-like Prism
     ['SSC','95','shape','columnar'], // Columnar
     ['SSC','96','shape','plaque'], // Plaque
     ['SSC','97','shape','statue'], // Statue
     ['SSC','98','shape','cross'], // Cross
     ['SSC','99','shape','horizontal_cylindrical'], // Horizontal Cylindrical
     ['SSC','100','shape','rectangular_prism'], // Rectangular Prism
     ['SSC','101','shape','square_prism'], // Square Prism
     ['SSC','109','shape','obelisk'], // Obelisk
     ['SSC','112','shape','statue_on_pedestal'], // Statue on Pedestal
     ['SSC','999','shape','other'], // Other

     // SSG - Street Sign Type
     // ['SSG','-999999',undefined,undefined], // No Information
     ['SSG','1','street_sign:type','direction'], // Direction
     ['SSG','2','street_sign:type','electronic_message'], // Electronic Message
     ['SSG','3','street_sign:type','general_information'], // General Information
     ['SSG','4','street_sign:type','mandatory_regulation'], // Mandatory Regulation
     ['SSG','5','street_sign:type','place_identification'], // Place Identification
     ['SSG','6','street_sign:type','prohibited_or_restricted_activity'], // Prohibited or Restricted Activity
     ['SSG','7','street_sign:type','road_identification'], // Road Identification
     ['SSG','8','street_sign:type','route_marker'], // Route Marker
     ['SSG','9','street_sign:type','traffic_control'], // Traffic Control
     ['SSG','10','street_sign:type','warning'], // Warning
     ['SSG','999','street_sign:type','other'], // Other

     // SSR - Roof Shape
     // ['SSR','-999999',undefined,undefined], // No Information
     ['SSR','6','roof:shape','conical'], // Conical
     ['SSR','7','roof:shape','pyramidal'], // Pyramidal
     ['SSR','38','roof:shape','semi-cylindrical'], // Semi-cylindrical
     ['SSR','40','roof:shape','domed'], // Domed
     ['SSR','41','roof:shape','flat'], // Flat
     ['SSR','42','roof:shape','pitched'], // Pitched
     ['SSR','47','roof:shape','sawtoothed'], // Sawtoothed
     ['SSR','50','roof:shape','with_clerestory'], // With Clerestory
     ['SSR','55','roof:shape','flat_with_clerestory'], // Flat with Clerestory
     ['SSR','64','roof:shape','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR','999','roof:shape','other'], // Other

     // STL - Nomadic Seasonal Location -- Nothing in OSM is close to this
     // ['STL','-999999',undefined,undefined], // No Information
     ['STL','1','seasonal_location','winter'], // Winter
     ['STL','2','seasonal_location','summer'], // Summer
     ['STL','3','seasonal_location','spring'], // Spring
     ['STL','4','seasonal_location','autumn'], // Autumn
     ['STL','999','seasonal_location','other'], // Other

     // STP - Soil Type
     // ['STP','-999999',undefined,undefined], // No Information
     ['STP','1','soil:type','gw:well-graded_gravel'], // GW: Well-graded Gravel
     ['STP','2','soil:type','gp:poorly-graded_gravel'], // GP: Poorly-graded Gravel
     ['STP','3','soil:type','gm:silty_gravel_sand'], // GM: Silty Gravel Sand
     ['STP','4','soil:type','gc:clayey_gravel'], // GC: Clayey Gravel
     ['STP','5','soil:type','sw:well-graded_sand'], // SW: Well-graded Sand
     ['STP','6','soil:type','sp:poorly-graded_sand'], // SP: Poorly-graded Sand
     ['STP','7','soil:type','sm:silty_sand'], // SM: Silty Sand
     ['STP','8','soil:type','sc:clayey_sand'], // SC: Clayey Sand
     ['STP','9','soil:type','ml:silt_and_fine_sand'], // ML: Silt and Fine Sand
     ['STP','10','soil:type','cl:lean_clay'], // CL: Lean Clay
     ['STP','11','soil:type','ol:organic_silt_and_clay'], // OL: Organic Silt and Clay
     ['STP','12','soil:type','ch:fat_clay'], // CH: Fat Clay
     ['STP','13','soil:type','mh:micraceous'], // MH: Micraceous
     ['STP','14','soil:type','oh:organic_clay'], // OH: Organic Clay
     ['STP','15','soil:type','pt:peat'], // PT: Peat
     ['STP','17','soil:type','ml-cl:silt,_fine_sand_and_lean_clay'], // ML-CL: Silt, Fine Sand and Lean Clay
     ['STP','18','soil:type','evaporite'], // Evaporite
     ['STP','99','soil:type','not_evaluated'], // Not Evaluated
     ['STP','999','soil:type','other'], // Other

     // SUY - Survey Point Type
     // ['SUY','-999999',undefined,undefined], // No Information
     ['SUY','1','survey_point:type','astronomic_position'], // Astronomic Position
     ['SUY','2','survey_point:type','benchmark'], // Benchmark
     ['SUY','3','survey_point:type','cadastral_control_point'], // Cadastral Control Point
     ['SUY','4','survey_point:type','camera_station'], // Camera Station
     ['SUY','5','survey_point:type','geodetic_point'], // Geodetic Point
     ['SUY','6','survey_point:type','gravity_point'], // Gravity Point
     ['SUY','7','survey_point:type','magnetic_station'], // Magnetic Station
     ['SUY','8','survey_point:type','theodolite_station'], // Theodolite Station
     ['SUY','9','survey_point:type','tidal_benchmark'], // Tidal Benchmark
     ['SUY','10','survey_point:type','transit_station'], // Transit Station
     ['SUY','999','survey_point:type','other'], // Other

     // SWC - Soil Wetness Condition
     // ['SWC','-999999',undefined,undefined], // No Information
     ['SWC','1','soil:wetness','normally_dry'], // Normally Dry
     ['SWC','2','soil:wetness','normally_moist'], // Normally Moist
     ['SWC','3','soil:wetness','normally_wet'], // Normally Wet
     ['SWC','4','soil:wetness','normally_frozen'], // Normally Frozen

     // SWT - Natural Pool Type
     // ['SWT','-999999',undefined,undefined], // No Information
     ['SWT','4','natural','spring'], // Spring
     ['SWT','6','natural','walled-in_spring'], // Walled-in Spring
     ['SWT','7','natural','resurgence'], // Resurgence
     ['SWT','999','natural','other_pool_type'], // Other -- Not great

     // TCS - Cross-sectional Profile
     // ['TCS','-999999',undefined,undefined], // No Information
     ['TCS','1','culvert:profile','arch'], // Arch
     ['TCS','2','culvert:profile','box'], // Box
     ['TCS','3','culvert:profile','semicircular'], // Semicircular
     ['TCS','999','culvert:profile','other'], // Other

     // TEL - Telescope Type
     // ['TEL','-999999',undefined,undefined], // No Information
     ['TEL','1','telescope:type','optical'], // Optical
     ['TEL','2','telescope:type','parabolic_radio_aerial'], // Parabolic Radio Aerial
     ['TEL','3','telescope:type','radio_aerial_array'], // Radio Aerial Array
     ['TEL','999','telescope:type','other'], // Other

     // THR - Through Route
     // ['THR','-999999',undefined,undefined], // No Information
     ['THR','1000','highway:through_route','no'],
     ['THR','1001','highway:through_route','yes'], // Need to look at custom rules to deconflict this

     // TID - Tide Influenced
     // ['TID','-999999',undefined,undefined], // No Information
     ['TID','1000','tidal','no'],
     ['TID','1001','tidal','yes'],

     // TOS - Tower Shape
     // ['TOS','-999999',undefined,undefined], // No Information
     ['TOS','1','tower:shape','a_frame'], // 'A' Frame
     ['TOS','2','tower:shape','h_frame'], // 'H' Frame
     ['TOS','3','tower:shape','i_frame'], // 'I' Frame
     ['TOS','5','tower:shape','y_frame'], // 'Y' Frame
     ['TOS','6','tower:shape','mast'], // Mast
     ['TOS','8','tower:shape','pole'], // Pole
     ['TOS','11','tower:shape','tripod'], // Tripod
     ['TOS','12','tower:shape','truss'], // Truss
     ['TOS','13','tower:shape','tubular'], // Tubular
     ['TOS','14','tower:shape','tower_mill'], // Tower Mill
     ['TOS','999','tower:shape','other'], // Other

     // TRA - Pedestrian Traversable
     // ['TRA','-999999',undefined,undefined], // No Information
     ['TRA','1000','foot','no'],
     ['TRA','1001','foot','yes'],

     // TRE - Foliage Type
     // ['TRE','-999999',undefined,undefined], // No Information
     ['TRE','1','wood','deciduous'], // Deciduous
     ['TRE','2','wood','evergreen'], // Evergreen
     ['TRE','3','wood','mixed'], // Mixed
     ['TRE','999','wood','other'], // Other

     // TRP - Transportation Route Protection Structure Type
     // ['TRP','-999999',undefined,undefined], // No Information
     ['TRP','1','tunnel','avalanche_protector'], // Gallery
     ['TRP','2','man_made','rock_shed'], // Rock Protection Shed
     ['TRP','3','man_made','snow_fence'], // Snow Protection Shed
     ['TRP','4','man_made','protection_shed'], // Protection Shed
     ['TRP','999','protection_structure','other'], // Other

     // TRS - Transportation System Type
     // ['TRS','-999999',undefined,undefined], // No Information
     ['TRS','1','transport:type','air'],
     ['TRS','2','transport:type','aqueduct'], // Aqueduct
     ['TRS','3','transport:type','automotive'], // From TRD3.0 to stop warnings Automotive
     ['TRS','4','transport:type','bus'], // Bus
     ['TRS','5','transport:type','canal'], // Canal
     ['TRS','6','transport:type','caravan_route'], // Caravan Route
     ['TRS','7','transport:type','maritime'],
     ['TRS','8',undefined,undefined], // No Transportation System
     ['TRS','9','transport:type','pedestrian'], // Pedestrian
     ['TRS','10','transport:type','pipeline'], // Pipeline
     ['TRS','11','transport:type','portage'], // Portage
     ['TRS','12','transport:type','railway'], // Railway
     ['TRS','13','transport:type','road'], // Road
     // ['TRS','14','transport:type','road_and_railway'],
     ['TRS','15','transport:type','truck'], // Truck
     ['TRS','16','transport:type','underground_railway'], // Underground Railway
     ['TRS','17','transport:type','inland_waterway'], // Inland Waterway
     ['TRS','18','transport:type','pack-road'], // Pack-road
     ['TRS','20','transport:type','taxiway'], // Taxiway
     ['TRS','21','transport:type','pipeline_maintenance'], // Pipeline Maintenance
     ['TRS','22','transport:type','powerline_maintenance'], // Power Line Maintenance
     ['TRS','23','transport:type','drove'], // Drove
     ['TRS','25','transport:type','runway'], // Runway
     ['TRS','30','transport:type','cableway'], // Cableway
     ['TRS','999','transport:type','other'], // Other

     // TSCC - Tree Spacing (interval closure)
     ['TSCC','2','tree:spacing:closure','open_interval'], // Open Interval
     ['TSCC','3','tree:spacing:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['TSCC','4','tree:spacing:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['TSCC','5','tree:spacing:closure','closed_interval'], // Closed Interval
     ['TSCC','6','tree:spacing:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['TSCC','7','tree:spacing:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['TSCC','8','tree:spacing:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['TSCC','9','tree:spacing:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // TSM - Terrain Surface Material - Added to ZI016_ROC list
     ['TSM','-999999',undefined,undefined], // No Information
     ['TSM','1','desert_surface','asphalt'], // Asphalt
     ['TSM','2','desert_surface','bedrock'], // Bedrock
     ['TSM','3','desert_surface','boulders'], // Boulders
     ['TSM','4','desert_surface','concrete'], // Concrete
     ['TSM','5','desert_surface','evaporite'], // Evaporite
     ['TSM','6','desert_surface','frozen_water'], // Frozen Water
     ['TSM','7','desert_surface','gravel'], // Gravel
     ['TSM','8','desert_surface','lava_flow'], // Lava Flow
     ['TSM','9','desert_surface','loess'], // Loess
     ['TSM','10','desert_surface','mud'], // Mud
     ['TSM','11','desert_surface','paved'], // Paved
     ['TSM','12','desert_surface','rock'], // Rock
     ['TSM','13','desert_surface','sand'], // Sand
     ['TSM','14','desert_surface','soil'], // Soil
     ['TSM','999','desert_surface','other'], // Other

     // TST - Cable Suspended Shape
     // ['TST','-999999',undefined,undefined], // No Information
     ['TST','1','cable:shape','symmetric_catenary'], // Symmetric Catenary
     ['TST','2','cable:shape','mountain_catenary'], // Mountain Catenary
     ['TST','3','cable:shape','overwater_catenary'], // Overwater Catenary
     ['TST','999','cable:shape','other'], // Other

     // TTC - Tower Type
     // ['TTC','-999999',undefined,undefined], // No Information
     ['TTC','2','tower:type','observation'], // Observation Tower
     ['TTC','10','tower:type','lookout'], // Lookout Tower
     ['TTC','12','tower:type','fire'], // Fire Tower
     ['TTC','20','tower:type','communication'], // Telecommunication Tower
     ['TTC','21','tower:type','guard'], // Guard Tower
     ['TTC','22','tower:type','industrial'], // Industrial Tower
     ['TTC','23','tower:type','drop'], // Drop Tower
     ['TTC','24','tower:type','solar_power'], // Solar Power Tower
     ['TTC','999','tower:type','other'], // Other

     // TTY - Tomb Type
     // ['TTY','-999999',undefined,undefined], // No Information
     ['TTY','1','tomb','cave'], // Cave
     ['TTY','2','tomb','catacomb'], // Catacomb
     ['TTY','3','tomb','crypt'], // Crypt
     ['TTY','4','tomb','surface_vault'], // Surface Vault
     ['TTY','5','tomb','mausoleum'], // Mausoleum
     ['TTY','6','tomb','burial_mound'], // Burial Mound
     ['TTY','7','tomb','columbarium'], // Columbarium
     ['TTY','999','tomb','other'], // Other

     // TXP - Taxiway Type
     // ['TXP','-999999',undefined,undefined], // No Information
     ['TXP','1','taxiway:type','parallel'], // Parallel Taxiway
     ['TXP','2','taxiway:type','stub'], // Stub Taxiway
     ['TXP','3','taxiway:type','rapid_exit_and/or_turnoff'], // Rapid Exit and/or Turnoff Taxiway
     ['TXP','4','taxiway:type','turnaround'], // Turnaround Taxiway
     ['TXP','5','taxiway:type','dispersal'], // Dispersal
     ['TXP','6','taxiway:type','loop'], // Loop
     ['TXP','7','taxiway:type','perimeter'], // Perimeter
     ['TXP','8','taxiway:type','apron'], // Apron Taxiway
     ['TXP','9','taxiway:type','aircraft_stand_taxilane'], // Aircraft Stand Taxilane
     ['TXP','10','taxiway:type','lead-in_taxilane'], // Lead-in Taxilane
     ['TXP','11','taxiway:type','lead-out_taxilane'], // Lead-out Taxilane
     ['TXP','12','taxiway:type','air'], // Air Taxiway
     ['TXP','13','taxiway:type','helicopter_ground_taxiway'], // Helicopter Ground Taxiway
     ['TXP','999','taxiway:type','other'], // Other

     // UDQ - Underwater Delineation Quality
     // ['UDQ','-999999',undefined,undefined], // No Information
     ['UDQ','1','deliniation:quality','excellent'], // Excellent Delineation
     ['UDQ','2','deliniation:quality','good'], // Good Delineation
     ['UDQ','3','deliniation:quality','average'], // Average Delineation
     ['UDQ','4','deliniation:quality','poor'], // Poor Delineation
     ['UDQ','5','deliniation:quality','highly_obscured'], // Highly Obscured Delineation

     // UMA - Underground Mine Access
     // ['UMA','-999999',undefined,undefined], // No Information
     ['UMA','1','mine:access','drift'], // Drift
     ['UMA','2','mine:access','slope'], // Slope
     ['UMA','3','mine:access','shaft'], // Shaft
     ['UMA','998',undefined,undefined], // Not Applicable

     // UTY - Accessible Utility Type
     // ['UTY','-999999',undefined,undefined], // No Information
     ['UTY','1','manhole','cable_television'], // Cable Television
     ['UTY','2','manhole','cooling_fluid_circulation'], // Cooling Fluid Circulation
     ['UTY','3','manhole','digital_fibre-optic_system'], // Digital Fibre-optic System
     ['UTY','4','manhole','electric_power_distribution'], // Electric Power Distribution
     ['UTY','5','manhole','heating_fluid_circulation'], // Heating Fluid Circulation
     ['UTY','6','manhole','natural_gas_distribution'], // Natural Gas Distribution
     ['UTY','7','manhole','sewage'], // Sewage
     ['UTY','8','manhole','storm_sewer'], // Storm Sewer
     ['UTY','9','manhole','street_light'], // Street Light
     ['UTY','10','manhole','telegraph'], // Telegraph
     ['UTY','11','manhole','telephone'], // Telephone
     ['UTY','12','manhole','traffic_light'], // Traffic Light
     ['UTY','13','manhole','water_distribution'], // Water Distribution
     ['UTY','999','manhole','other'], // Other

     // VCA - Void Collection Reason
     // ['VCA','-999999',undefined,undefined], // No Information
     ['VCA','1','void_collection:reason','not_requested'], // Not Requested
     ['VCA','2','void_collection:reason','inaccessible'], // Inaccessible
     ['VCA','3','void_collection:reason','no_available_imagery'], // No Available Imagery
     ['VCA','6','void_collection:reason','no_available_map_source'], // No Available Map Source
     ['VCA','7','void_collection:reason','no_suitable_imagery'], // No Suitable Imagery
     ['VCA','8','void_collection:reason','not_required'], // Not Required
     ['VCA','9','void_collection:reason','cloud_cover'], // Cloud Cover
     ['VCA','10','void_collection:reason','snow_cover'], // Snow Cover
     ['VCA','11','void_collection:reason','dark_shade'], // Dark Shade
     ['VCA','12','void_collection:reason','vegetation_cover'], // Vegetation Cover
     ['VCA','13','void_collection:reason','flooded'], // Flooded
     ['VCA','16','void_collection:reason','no_available_survey'], // No Available Survey
     ['VCA','999','void_collection:reason','other'], // Other

     // VCM - Vertical Construction Material
     // ['VCM','-999999',undefined,undefined], // No Information
     ['VCM','1','material:vertical','adobe_brick'], // Adobe Brick
     ['VCM','2','material:vertical','aluminum'], // Aluminum
     ['VCM','3','material:vertical','brick'], // Brick
     ['VCM','4','material:vertical','concrete'], // Concrete
     ['VCM','5','material:vertical','fibreglass'], // Fibreglass
     ['VCM','6','material:vertical','glass'], // Glass
     ['VCM','7','material:vertical','iron'], // Iron
     ['VCM','8','material:vertical','masonry'], // Masonry
     ['VCM','9','material:vertical','metal'], // Metal
     ['VCM','10','material:vertical','mud-based_construction'], // Mud-based Construction
     ['VCM','11','material:vertical','plant_material'], // Plant Material
     ['VCM','12','material:vertical','prestressed_concrete'], // Prestressed Concrete
     ['VCM','13','material:vertical','reinforced_concrete'], // Reinforced Concrete
     ['VCM','14','material:vertical','sod'], // Sod
     ['VCM','15','material:vertical','steel'], // Steel
     ['VCM','16','material:vertical','stone'], // Stone
     ['VCM','17','material:vertical','treated_wood'], // Treated Timber
     ['VCM','18','material:vertical','wood'], // Wood
     ['VCM','999','material:vertical','other'], // Other

     // VCS_VCD - Vertical Clearance, Safe <reference level>
     // ['VCS_VCD','-999999',undefined,undefined], // No Information
     ['VCS_VCD','7','safe_clearance:vertical:reference','mean_high_water'], // Mean High Water
     ['VCS_VCD','9','safe_clearance:vertical:reference','mean_high_water_springs'], // Mean High Water Springs
     ['VCS_VCD','10','safe_clearance:vertical:reference','mean_higher_high_water'], // Mean Higher High Water
     ['VCS_VCD','15','safe_clearance:vertical:reference','mean_sea_level'], // Mean Sea Level
     ['VCS_VCD','24','safe_clearance:vertical:reference','mean_higher_high_water_springs'], // Mean Higher High Water Springs
     ['VCS_VCD','28','safe_clearance:vertical:reference','highest_high_water'], // Highest High Water
     ['VCS_VCD','30','safe_clearance:vertical:reference','indian_spring_high_water'], // Indian Spring High Water
     ['VCS_VCD','999','safe_clearance:vertical:reference','other'], // Other

     // VCT - Void Collection Type
     // ['VCT','-999999',undefined,undefined], // No Information
     ['VCT','1','void_collection:type','hypsography'], // Hypsography
     ['VCT','3','void_collection:type','bathymetry'], // Bathymetry
     ['VCT','4','void_collection:type','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT','999','void_collection:type','other'], // Other

     // VDT - Vertical Datum
     // ['VDT','noInformation',undefined,undefined], // No Information
     ['VDT','other','source:datum:vertical','other'], // Other
     ['VDT','groundLevel','source:datum:vertical','ground_level'], // Ground Level
     ['VDT','meanSeaLevel','source:datum:vertical','mean_sea_level_(msl)'], // Mean Sea Level (MSL)
     ['VDT','navd88','source:datum:vertical','north_american_vertical_datum_(navd)_1988'], // North American Vertical Datum (NAVD) 1988
     ['VDT','ngvd29','source:datum:vertical','national_geodetic_vertical_datum_(ngvd)_1929'], // National Geodetic Vertical Datum (NGVD) 1929
     ['VDT','wgs84','source:datum:vertical','wgs_84_ellipsoid'], // WGS 84 Ellipsoid
     ['VDT','wgs84Egm08','source:datum:vertical','wgs_84_egm08_geoid'], // WGS 84 EGM08 Geoid
     ['VDT','wgs84Egm96','source:datum:vertical','wgs_84_egm96_geoid'], // WGS 84 EGM96 Geoid

     // VEG - Vegetation Characteristic
     // ['VEG','-999999',undefined,undefined], // No Information
     ['VEG','8','grassland:type','grassland'], // Grassland
     ['VEG','9','grassland:type','grassland_with_trees'], // Grassland with Trees
     ['VEG','10','grassland:type','tropical_grass'], // Tropical Grass
     ['VEG','56','grassland:type','without_trees'], // Without Trees
     ['VEG','83','grassland:type','meadow'], // Meadow
     ['VEG','999','grassland:type','other'], // Other

     // VET - Vehicle Type
     // ['VET','-999999',undefined,undefined], // No Information
     ['VET','1','vehicle:type','aircraft'], // Aircraft
     ['VET','2','vehicle:type','automobile'], // Automobile
     ['VET','3','vehicle:type','barge'], // Barge
     ['VET','4','vehicle:type','bicycle'], // Bicycle
     ['VET','5','vehicle:type','boat'], // Boat
     ['VET','6','vehicle:type','bus'], // Bus
     ['VET','7','vehicle:type','caravanette'], // Caravanette
     ['VET','8','vehicle:type','caravan'], // Caravan
     ['VET','9','vehicle:type','motorcycle'], // Motorcycle
     ['VET','10','vehicle:type','ship'], // Ship
     ['VET','999','vehicle:type','other'], // Other

     // VGT - Volcano Shape
     // ['VGT','-999999',undefined,undefined], // No Information
     ['VGT','1','volcano:type','cone'], // Cone
     ['VGT','2','volcano:type','cinder_cone'], // Cinder Cone
     ['VGT','3','volcano:type','shield'], // Shield
     ['VGT','4','volcano:type','caldera'], // Caldera
     ['VGT','5','volcano:type','composite'], // Composite

     // VOA - Volcanic Activity
     // ['VOA','-999999',undefined,undefined], // No Information
     ['VOA','1','volcano:status','active'], // Active
     ['VOA','2','volcano:status','dormant'], // Dormant
     ['VOA','3','volcano:status','inactive_or_extinct'], // Inactive or Extinct

     // VSP - Vegetation Species
     // ['VSP','-999999',undefined,undefined], // No Information
     ['VSP','8','vegetation:type','cypress'], // Cypress
     ['VSP','19','vegetation:type','mangrove'], // Mangrove
     ['VSP','22','vegetation:type','nipa'], // Nipa
     ['VSP','25','vegetation:type','palm'], // Palm
     ['VSP','34','vegetation:type','bamboo'], // Bamboo
     ['VSP','64','vegetation:type','swamp_cypress'], // Swamp Cypress
     ['VSP','999','vegetation:type','other'], // Other

     // WCC - Watercourse Channel Type
     // ['WCC','-999999',undefined,undefined], // No Information
     ['WCC','1','waterway','stream'], // Channelized Stream
     ['WCC','2','waterway','braided_stream'], // Braided Stream
     ['WCC','3','waterway','gorge'], // Gorge
     ['WCC','4','waterway','wadi'], // Wadi
     ['WCC','7','waterway','river'], // Normal Channel
     ['WCC','8','waterway','lost_watercourse'], // Lost Watercourse
     ['WCC','999','waterway','other'], // Other

     // WDAC - Average Water Depth <interval closure>
     ['WDAC','2','depth:average:closure','open_interval'], // Open Interval
     ['WDAC','3','depth:average:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['WDAC','4','depth:average:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['WDAC','5','depth:average:closure','closed_interval'], // Closed Interval
     ['WDAC','6','depth:average:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['WDAC','7','depth:average:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['WDAC','8','depth:average:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['WDAC','9','depth:average:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // WEQ - Well Equipment
     // ['WEQ','-999999',undefined,undefined], // No Information
     ['WEQ','1','well:equipment','wellhead'], // Wellhead
     ['WEQ','2','well:equipment','christmas_tree'], // Christmas Tree
     ['WEQ','3','well:equipment','manifold'], // Manifold
     ['WEQ','4','well:equipment','protective_structure'], // Protective Structure
     ['WEQ','5','pump','yes'], // Pump
     ['WEQ','6','well:equipment','rod_pump'], // Rod Pump
     ['WEQ','7','well:equipment','separator'], // Separator
     ['WEQ','8','well:equipment','stock_tank'], // Stock Tank
     ['WEQ','9','well:equipment','treater'], // Treater
     ['WEQ','10','well:equipment','capped'], // Capped
     ['WEQ','999','well:equipment','other'], // Other

     // WFT - Well Type
     // ['WFT','-999999',undefined,undefined], // No Information
     ['WFT','2','water_well:type','walled-in'], // Walled-in
     ['WFT','3','water_well:type','artesian'], // Artesian
     ['WFT','5','water_well:type','dug_or_drilled'], // Dug or Drilled
     ['WFT','6','water_well:type','dug'], // Dug
     ['WFT','7','water_well:type','drilled'], // Drilled
     ['WFT','999','water_well:type','other'], // Other

     // WLE - Hydrographic Vertical Positioning Information : Water Level Effect
     // ['WLE','-999999',undefined,undefined], // No Information
     ['WLE','1','hydrographic_vertical_position','partly_submerged'], // Partly Submerged
     ['WLE','2','hydrographic_vertical_position','always_dry'], // Always Dry
     ['WLE','3','hydrographic_vertical_position','always_submerged'], // Always Submerged
     ['WLE','4','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
     ['WLE','5','hydrographic_vertical_position','awash_at_low_water'], // Awash at Low Water
     ['WLE','9','hydrographic_vertical_position','awash_at_chart_datum'], // Awash at Chart Datum
     ['WLE','999','hydrographic_vertical_position','other'], // Other

     // WMT - Watercourse Morphology - Really?
     // ['WMT','-999999',undefined,undefined], // No Information
     ['WMT','1','water:morphology','mouth'], // Mouth
     ['WMT','2','water:morphology','pool'], // Pool
     ['WMT','3','water:morphology','bend'], // Bend
     ['WMT','4','water:morphology','confluence'], // Confluence
     ['WMT','5','water:morphology','cut-off'], // Cut-off
     ['WMT','6','water:morphology','reach'], // Reach
     ['WMT','7','water:morphology','anabrach'], // Anabranch
     ['WMT','8','water:morphology','canalized'], // Canalized
     ['WMT','9','water:morphology','distributary'], // Distributary
     ['WMT','10','water:morphology','headwaters'], // Headwaters
     ['WMT','11','water:morphology','abandoned'], // Abandoned
     ['WMT','12','water:morphology','meander'], // Meander
     ['WMT','999','water:morphology','other'], // Other

     // WRT - Water Race Type
     // ['WRT','-999999',undefined,undefined], // No Information
     ['WRT','1','water:race','sluice'], // Sluice
     ['WRT','2','water:race','flume'], // Flume
     ['WRT','3','water:race','headrace'], // Headrace
     ['WRT','4','water:race','tailrace'], // Tailrace

     // WST - Watercourse Sink Type: No OSM even close
     // ['WST','-999999',undefined,undefined], // No Information
     ['WST','1','water:sink:type','dissipating'], // Dissipating
     ['WST','2','water:sink:type','disappearing'], // Disappearing
     ['WST','3','water:sink:type','sinkhole'], // Sinkhole
     ['WST','4','water:sink:type','hole'], // Hole
     ['WST','999','water:sink:type','other'], // Other

     // WTI - Wall Type
     // ['WTI','-999999',undefined,undefined], // No Information
     ['WTI','1','wall','free-standing'], // Free-standing
     ['WTI','2','wall','retaining'], // Retaining
     ['WTI','999','wall','other'], // Other

     // ['YSU','-999999',undefined,undefined], // No Information
     // YSU - Military Service Branch
     ['YSU','1','military:service','air_force'], // Air Force
     ['YSU','2','military:service','army'], // Army
     ['YSU','3','military:service','coastguard'], // Coastguard
     ['YSU','4','military:service','marines'], // Marines
     ['YSU','5','military:service','navy'], // Navy
     ['YSU','7','military:service','joint'], // Joint
     ['YSU','8','military:service','air_national_guard'], // Air National Guard
     ['YSU','9','military:service','army_national_guard'], // Army National Guard
     // ['YSU','998','military:service','not_applicable'], // Not Applicable
     ['YSU','998',undefined,undefined], // Not Applicable
     ['YSU','999','military:service','other'], // Other

     // YWQ - Water Potability - See ZI024_YWQ
     // ['YWQ','-999999',undefined,undefined], // No Information
//      ['YWQ','1','raw:YWQ','potable'], // Potable
//      ['YWQ','2','raw:YWQ','treatable'], // Treatable
//      ['YWQ','3','raw:YWQ','contaminated'], // Contaminated
//      ['YWQ','4','raw:YWQ','nonpotable'], // Nonpotable
//      ['YWQ','999','raw:YWQ','other'], // Other

     // ZI001_SPS - Source Information : Spatial Source
     // ['ZI001_SPS','-999999',undefined,undefined], // No Information
     ['ZI001_SPS','1000','source:spatial_source','no'],
     ['ZI001_SPS','1001','source:spatial_source','yes'],

     // ZI001_SRT - Source Information : Source Type
     // ['ZI001_SRT','noInformation',undefined,undefined], // No Information
     // ['ZI001_SRT','noInformation',undefined,undefined], // No Information
     ['ZI001_SRT','other','source:non_spatial_source:type','other'], // Other
     ['ZI001_SRT','commercial','source:non_spatial_source:type','commercial_data'], // Commercial Data
     ['ZI001_SRT','deLormeDigitalAtlasEarth','source:non_spatial_source:type','delorme_digital_atlas_of_the_earth_(dae)'], // DeLorme Digital Atlas of the Earth (DAE)
     ['ZI001_SRT','gpsBasedFieldCollect','source:non_spatial_source:type','gps'], // GPS
     ['ZI001_SRT','ikonosImagery','source:non_spatial_source:type','ikonos_imagery'], // Ikonos Imagery
     ['ZI001_SRT','imageryUnspecified','source:non_spatial_source:type','imagery'], // Imagery
     ['ZI001_SRT','militaryMapData','source:non_spatial_source:type','military_map_data'], // Military Map Data
     ['ZI001_SRT','navteqData','source:non_spatial_source:type','navteq_data'], // NAVTEQ Data
     ['ZI001_SRT','ngaAutoAirFacInfoFile','source:non_spatial_source:type','aafif'], // AAFIF
     ['ZI001_SRT','ngaControlledImageBase1','source:non_spatial_source:type','cib1'], // CIB1
     ['ZI001_SRT','ngaDigitalAirFltInfoFile','source:non_spatial_source:type','dafif'], // DAFIF
     ['ZI001_SRT','ngaDigitalNauticalChart','source:non_spatial_source:type','dnc'], // DNC
     ['ZI001_SRT','ngaDigitalVertObstruction','source:non_spatial_source:type','dvof'], // DVOF
     ['ZI001_SRT','ngaFoundationFeatureData','source:non_spatial_source:type','ffd'], // FFD
     ['ZI001_SRT','ngaGeoNames','source:non_spatial_source:type','geonames'], // GeoNames
     ['ZI001_SRT','ngaInterimTerrainData','source:non_spatial_source:type','itd'], // ITD
     ['ZI001_SRT','ngaInterimVectorData','source:non_spatial_source:type','ivd'], // IVD
     ['ZI001_SRT','ngaStereoAirfieldCollect','source:non_spatial_source:type','sac'], // SAC
     ['ZI001_SRT','ngaUrbanVectorMap','source:non_spatial_source:type','uvmap'], // UVMap
     ['ZI001_SRT','ngaVectorInterimTerrain','source:non_spatial_source:type','vitd'], // VITD
     ['ZI001_SRT','ngaVectorMap2','source:non_spatial_source:type','vmap_2'], // VMap 2
     ['ZI001_SRT','nonMilitaryMap','source:non_spatial_source:type','non-military_map'], // Non-military Map
     ['ZI001_SRT','openSource','source:non_spatial_source:type','open_source_information'], // Open Source Information
     ['ZI001_SRT','operationsData','source:non_spatial_source:type','operations_data'], // Operations Data
     ['ZI001_SRT','quickBirdImagery','source:non_spatial_source:type','quickbird_imagery'], // QuickBird Imagery
     ['ZI001_SRT','tomTomData','source:non_spatial_source:type','tomtom_data'], // TomTom Data
     ['ZI001_SRT','usModernizedIntegratedDB','source:non_spatial_source:type','midb'], // MIDB
     ['ZI001_SRT','usNtmImagery','source:non_spatial_source:type','ntm_imagery'], // NTM Imagery

     // ZI001_VSC - Source Information : Vertical Source Category
     // ['ZI001_VSC','noInformation',undefined,undefined], // No Information
     ['ZI001_VSC','other','source:vertical_source:type','other'], // Other
     ['ZI001_VSC','interpolatedDted1','source:vertical_source:type','dted_1'], // DTED 1
     ['ZI001_VSC','interpolatedDted2','source:vertical_source:type','dted_2'], // DTED 2
     ['ZI001_VSC','noElevations','source:vertical_source:type','no_elevations'], // No Elevations
     ['ZI001_VSC','reflectiveSurface','source:vertical_source:type','reflective_surface'], // Reflective Surface
     ['ZI001_VSC','stereoscopicImagery','source:vertical_source:type','stereoscopic_imagery'], // Stereoscopic Imagery
     ['ZI001_VSC','tinData','source:vertical_source:type','tin_data'], // TIN Data

     // ZI004_RCG - Process Step Information : Resource Content Originator
     // This is just plain Ugly
     // ['ZI004_RCG','noInformation',undefined,undefined], // No Information
     ['ZI004_RCG','other','attribution','other'], // Other
     ['ZI004_RCG','armyGeoAgencyNetherlands','attribution','army_geographic_agency_(netherlands)'], // The Royal Netherlands Army Geographic Agency (RNLAGA) in the Dutch Ministry of Defence.
     ['ZI004_RCG','armyGeographicCentreSpain','attribution','army_geographic_centre_(spain)'], // The Army Geographic Centre of the Spanish Armed Forces.
     ['ZI004_RCG','armyGeoInstitutePortugal','attribution','army_geographic_institute_(portugal)'], // The Army Geographic Institute (IGeoE),whose mission is to provide geographic information and geographic support the Portugese Army, to provide geographical information to other branches of the Portugese Armed Forces and the civilian community, ensuring th
     ['ZI004_RCG','bundeswehrGeoinfoOffice','attribution','bundeswehr_geoinformation_office_(germany)'], // The Bundeswehr Geoinformation Office (BGIO) of the German Federal Armed Forces.
     ['ZI004_RCG','defenceAcqLogOrgDenmark','attribution','defence_acquisition_and_logistics_organization_(denmark)'], // The Danish Defence Acquisition and Logistics Organisation (DALO) is the specialized materiel centre and logistics authority of the Danish Defence.
     ['ZI004_RCG','defenceGeoCentreIntColGrp','attribution','defence_geographic_centre_intelligence_collection_group_(united_kingdom)'], // The Defence Geographic Centre (DGC) Intelligence Collection Group (ICG) of the Ministry of Defence of the United Kingdom.
     ['ZI004_RCG','defenceImageryGeoOrg','attribution','defence_imagery_and_geospatial_organisation_(australia)'], // The Defence Imagery and Geospatial Organisation (DIGO) provides geospatial intelligence, from imagery and other sources, in support of Australia's defence and national interests.
     ['ZI004_RCG','defenseInfoSecurityItaly','attribution','defense_information_security_(italy)'], // The Reparto Informazioni e Sicurezza dello Stato Maggiore della Difesa (SMD-RIS), an intelligence agency of the Defense Ministry of Italy.
     ['ZI004_RCG','generalCommandMapping','attribution','general_command_of_mapping_(turkey)'], // The General Command of Mapping (GCM) is the National Mapping Agency for Turkey and is responsible for the official topographical mapping of the country in hard-copy and digital forms for: military, government, administrative, and educational uses.
     ['ZI004_RCG','geoServiceBelgium','attribution','geographic_service_(belgium)'], // The Geographic Service of Belgium.
     ['ZI004_RCG','geoServCzechArmedForces','attribution','geographic_service_of_the_czech_armed_forces_(czech_republic)'], // The Geographic Service of the Armed Forces of the Czech Republic (GeoS AER).
     ['ZI004_RCG','geoInfoAgencyLatvia','attribution','geospatial_information_agency_(latvia)'], // The Geospatial Information Agency of the Defence Ministry of Latvia.
     ['ZI004_RCG','geoIntelOrgNewZealand','attribution','geospatial_intelligence_organisation_(new_zealand)'], // The Geospatial Intelligence Organisation of the New Zealand Defence Force (NZDF), whose mission is to acquire, collate and maintain high integrity databases for production and dissemination of geospatial products and services to static and deployed NZDF H
     ['ZI004_RCG','hellenicMilitaryGeoServ','attribution','hellenic_military_geographic_service_(greece)'], // The Hellenic Military Geographic Service of the Hellenic Army General Service Directorate of Geographical Corps, who produce, manage and provide geographical data for the entire Hellenic region including the neighbouring countries of Greece.
     ['ZI004_RCG','jointGeoBureauFrance','attribution','joint_geography_bureau_(france)'], // The Bureau Geographique Interarmees (BGI), Etat-Major des Armees (EMA), Department of the Ministry of Defense of the French Republic, who is in charge of geospatial information support to the French military forces.
     ['ZI004_RCG','mapChartEstablishment','attribution','mapping_and_charting_establishment_(canada)'], // The Mapping and Charting Establishment (MCE) of Canada.
     ['ZI004_RCG','mappingServiceHungary','attribution','mapping_service_(hungary)'], // The Mapping Service of the Hungarian Defence Forces.
     ['ZI004_RCG','milGeoGroupEstonia','attribution','military_geographic_group_(estonia)'], // The Military Geographic Group of the Estonian Defence Forces.
     ['ZI004_RCG','milGeoServiceNorway','attribution','military_geographic_service_(norway)'], // The Norwegian Military Geographic Service of the Defence Command Norway.
     ['ZI004_RCG','milGeogDivisionPoland','attribution','military_geography_division_(poland)'], // The Military Geography Division of the General Staff of the Polish Armed Forces.
     ['ZI004_RCG','milMapCentreLithuania','attribution','military_mapping_centre_(lithuania)'], // The Military Mapping Centre of the Lithuanian National Defence Forces, whose mission is to ensure that the troops and commands of the Lithuanian Armed Forces are provided with the relevant mapping material, geographic information and geospatial support re
     ['ZI004_RCG','milTopoDirectRomania','attribution','military_topographic_directorate_(romania)'], // The Military Topographic Directorate of the Romanian Armed Forces.
     ['ZI004_RCG','milTopoServiceBulgaria','attribution','military_topographic_service_(bulgaria)'], // The Military Topographic Service of the Bulgarian Army.
     ['ZI004_RCG','natArmyTopoServiceMoldova','attribution','national_army_topographic_service_(moldova)'], // The National Army Topographic Service of Moldova.
     ['ZI004_RCG','swedishArmedForces','attribution','swedish_armed_forces_(sweden)'], // The Swedish Armed Forces (Forsvarsmakten).
     ['ZI004_RCG','topoInstituteSlovakia','attribution','topographic_institute_(slovakia)'], // The Topograficky Ustav (TOPU) of Slovakia.
     ['ZI004_RCG','topoServiceFinland','attribution','topographic_service_(finland)'], // The Topographic Service of the Finnish Defence Forces.
     ['ZI004_RCG','usAfricaCommand','attribution','u.s._africa_command_(usafricom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for U.S. military operations and military relations with 53 African nations.
     ['ZI004_RCG','usAirForce','attribution','u.s._air_force'], // The aerial warfare branch of the U.S. Armed Forces.
     ['ZI004_RCG','usArmy','attribution','u.s._army'], // The land-based military operations branch of the U.S. Armed Forces.
     ['ZI004_RCG','usArmyGeospatialCenter','attribution','u.s._army_geospatial_center_(agc)'], // A laboratory of the Engineer Research and Development Center (ERDC) that is responsible for coordination, integration and synchronization of geospatial information requirements and standards across the U.S. Army; development and fielding of geospatial-ent
     ['ZI004_RCG','usCentralCommand','attribution','u.s._central_command_(uscentcom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for U.S. military operations and military relations in the Middle East, extending from Egypt through Central Asia.
     ['ZI004_RCG','usCentralIntelAgency','attribution','u.s._central_intelligence_agency_(cia)'], // An independent U.S. Government agency responsible for providing national security intelligence to senior U.S. policymakers.
     ['ZI004_RCG','usCoastGuard','attribution','u.s._coast_guard'], // The branch of the U.S. Armed Forces responsible for protecting the public, the environment, and the U.S. economic and security interests in any maritime region in which those interests may be at risk, including international waters and America's coasts, p
     ['ZI004_RCG','usDefenseIntelAgency','attribution','u.s._defense_intelligence_agency_(dia)'], // A U.S. Department of Defense (DoD) combat support agency that is responsible for providing intelligence in support of military planning and operations and weapons acquisition.
     ['ZI004_RCG','usDeptOfEnergy','attribution','u.s._department_of_energy_(doe)'], // The cabinet-level department of the U.S. Government that is responsible for maintaining national energy policy, nuclear power and nuclear weapons programs, and the national energy research labs.
     ['ZI004_RCG','usDeptOfHomelandSecurity','attribution','u.s._department_of_homeland_security_(dhs)'], // The cabinet-level department of the U.S. Government that is responsible for protecting the territory of the U.S. from terrorist attacks and for responding to natural disasters.
     ['ZI004_RCG','usDeptOfState','attribution','u.s._department_of_state'], // The cabinet-level foreign affairs department of the U.S. Government, often referred to as the State Department.
     ['ZI004_RCG','usEuropeanCommand','attribution','u.s._european_command_(useucom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for U.S. military operations and military relations in Europe, extending from Greenland/Iceland through Israel.
     ['ZI004_RCG','usFedBurOfInvestigation','attribution','u.s._federal_bureau_of_investigation_(fbi)'], // The principal investigative arm of the U.S. Department of Justice (the Cabinet-level department of the U.S. Government responsible for enforcing federal laws).
     ['ZI004_RCG','usGeologicalSurvey','attribution','u.s._geological_survey_(usgs)'], // The U.S. Department of the Interior (a cabinet-level department of the U.S. Government) component agency that provides reliable scientific information to describe and understand the Earth; minimize loss of life and property from natural disasters; manage
     ['ZI004_RCG','usJointForcesCommand','attribution','u.s._joint_forces_command_(usjfcom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is a functional command providing specific services to the U.S. military.
     ['ZI004_RCG','usMarineCorps','attribution','u.s._marine_corps'], // The branch of the U.S. Armed Forces responsible for providing force projection from the sea, using the mobility of the U.S. Navy to rapidly deliver combined-arms task forces.
     ['ZI004_RCG','usNatCivilAppsProgram','attribution','u.s._national_civil_applications_program_(ncap)'], // The component of the U.S. Geological Survey (USGS) Mapping, Remote Sensing, and Geographic Investigations Program that serves Federal civil agencies by providing for the acquisition, dissemination, and exploitation of classified remote sensing systems and
     ['ZI004_RCG','usNationalGeoIntelAgency','attribution','u.s._national_geospatial-intelligence_agency_(nga)'], // A U.S. Department of Defense (DoD) combat support agency that provides geographic intelligence in support of national security.
     ['ZI004_RCG','usNatOceanAtmosAdmin','attribution','u.s._national_oceanic_and_atmospheric_administration'], // The U.S. Department of Commerce (a cabinet-level department of the U.S. Government) component agency that provides reliable scientific information to describe and understand the conditions of the oceans and the atmosphere, including: warning of dangerous
     ['ZI004_RCG','usNationalReconnOffice','attribution','u.s._national_reconnaissance_office_(nro)'], // An intelligence agency in the U.S. Department of Defense (DoD) that is responsible for designing, building and operating space reconnaissance systems to: detect trouble spots worldwide; monitor arms control agreements and environmental issues; and help pl
     ['ZI004_RCG','usNationalSecurityAgency','attribution','u.s._national_security_agency_(nsa)'], // A U.S. Department of Defense (DoD) combat support agency that is responsible for providing cryptologic products and services to the U.S. DoD, the U.S. Intelligence Community, U.S. Government agencies, industry partners, and select allies and coalition par
     ['ZI004_RCG','usNavy','attribution','u.s._navy'], // The sea-based military operations branch of the U.S. Armed Forces.
     ['ZI004_RCG','usNorthernCommand','attribution','u.s._northern_command_(usnorthcom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for protecting the United States homeland and supporting local, state, and federal authorities.
     ['ZI004_RCG','usPacificCommand','attribution','u.s._pacific_command_(pacom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for U.S. military operations and military relations in the region of the Pacific Ocean, extending westwards to Mongolia, the People's Republic of China, and Southest
     ['ZI004_RCG','usSouthernCommand','attribution','u.s._southern_command_(ussouthcom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for providing contingency planning, operations, and security cooperation for Central and South America, the Caribbean (except U.S. commonwealths, territories, and po
     ['ZI004_RCG','usSpecialOperCommand','attribution','u.s._special_operations_command_(ussocom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for overseeing the various Special Operations Commands of the Army, Air Force, Navy and Marine Corps of the U.S. Armed Forces.
     ['ZI004_RCG','usStrategicCommand','attribution','u.s._strategic_command_(usstratcom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that controls the nuclear weapons of the U.S. military and is a globally focused command charged with the missions of space operations, information operations, integrated missile defens
     ['ZI004_RCG','usTransportationCommand','attribution','u.s._transportation_command_(ustranscom)'], // The unified combatant command of the U.S. Department of Defense (DoD) that is responsible for providing air, land and sea transportation for the DoD, both in time of peace and time of war.

     // ZI013_CSP - Crop Information : Crop Species
     // ['ZI013_CSP','-999999',undefined,undefined], // No Information
     ['ZI013_CSP','1','crop','almond'], // Almond
     ['ZI013_CSP','2','crop','apple'], // Apple
     ['ZI013_CSP','3','crop','barley'], // Barley
     ['ZI013_CSP','4','crop','berry'], // Berry
     ['ZI013_CSP','5','crop','carob'], // Carob
     ['ZI013_CSP','6','crop','chestnut'], // Chestnut
     ['ZI013_CSP','7','crop','citrus'], // Citrus
     ['ZI013_CSP','8','crop','cork-oak'], // Cork-Oak
     ['ZI013_CSP','9','crop','cacao'], // Cacao
     ['ZI013_CSP','10','crop','coffee'], // Coffee
     ['ZI013_CSP','11','crop','maize'], // Maize
     ['ZI013_CSP','13','crop','dry_crop'], // Dry Crop
     ['ZI013_CSP','14','crop','fibre_crop'], // Fibre Crop
     ['ZI013_CSP','15','crop','fruit_tree'], // Fruit Tree
     ['ZI013_CSP','16','crop','grape'], // Grape
     ['ZI013_CSP','17','crop','hazelnut'], // Hazelnut
     ['ZI013_CSP','18','crop','hop'], // Hop
     ['ZI013_CSP','19','crop','maple'], // Maple
     ['ZI013_CSP','20','crop','millet'], // Millet
     ['ZI013_CSP','21','crop','oat'], // Oat
     ['ZI013_CSP','22','crop','oil_crop'], // Oil Crop
     ['ZI013_CSP','23','crop','olive'], // Olive
     ['ZI013_CSP','24','crop','peach'], // Peach
     ['ZI013_CSP','25','crop','peanut'], // Peanut
     ['ZI013_CSP','26','crop','potato'], // Potato
     ['ZI013_CSP','27','crop','pulse'], // Pulse
     ['ZI013_CSP','28','crop','rice'], // Rice
     ['ZI013_CSP','29','crop','rubber'], // Rubber
     ['ZI013_CSP','30','crop','rye'], // Rye
     ['ZI013_CSP','31','crop','sisal'], // Sisal
     ['ZI013_CSP','32','crop','sorghum'], // Sorghum
     ['ZI013_CSP','33','crop','sugar_crop'], // Sugar Crop
     ['ZI013_CSP','34','crop','tea'], // Tea
     ['ZI013_CSP','35','crop','tobacco'], // Tobacco
     ['ZI013_CSP','36','crop','tuber'], // Tuber
     ['ZI013_CSP','37','crop','vegetable'], // Vegetable Crop
     ['ZI013_CSP','38','crop','walnut'], // Walnut
     ['ZI013_CSP','39','crop','wheat'], // Wheat
     ['ZI013_CSP','40','crop','date'], // Date
     ['ZI013_CSP','41','crop','banana'], // Banana
     ['ZI013_CSP','42','crop','oil_palm'], // Oil Palm
     ['ZI013_CSP','43','crop','coconut'], // Coconut
     ['ZI013_CSP','44','crop','sugar_cane'], // Sugar Cane
     ['ZI013_CSP','45','crop','cotton'], // Cotton
     ['ZI013_CSP','46','crop','bamboo'], // Bamboo
     ['ZI013_CSP','47','crop','palmetto'], // Palmetto
     ['ZI013_CSP','48','crop','timber'], // Timber
     ['ZI013_CSP','117','crop','opium_poppy'], // Opium Poppy
     ['ZI013_CSP','152','crop','cannabis'], // Cannabis
     ['ZI013_CSP','159','crop','ornamental'], // Ornamental Crop
     ['ZI013_CSP','999','crop','other'], // Other

     // ZI013_FFP - Crop Information : Farming Pattern
     // ['ZI013_FFP','-999999',undefined,undefined], // No Information
     ['ZI013_FFP','1','farm:pattern','linear'], // Linear
     ['ZI013_FFP','2','farm:pattern','regular'], // Regular
     ['ZI013_FFP','3','farm:pattern','terraced'], // Terraced
     ['ZI013_FFP','4','farm:pattern','intermingled_woods'], // Intermingled Woods
     ['ZI013_FFP','5','farm:pattern','intermingled_trees'], // Intermingled Trees
     ['ZI013_FFP','6','farm:pattern','treeless'], // Treeless
     ['ZI013_FFP','7','farm:pattern','trellised'], // Trellised
     ['ZI013_FFP','8','farm:pattern','irregular'], // Irregular
     ['ZI013_FFP','999','farm:pattern','other'], // Other

     // ZI013_FMM - Crop Information : Farming Method
     // ['ZI013_FMM','-999999',undefined,undefined], // No Information
     ['ZI013_FMM','1','farm:method','fallow'], // Fallow
     ['ZI013_FMM','2','farm:method','grazing'], // Grazing
     ['ZI013_FMM','3','farm:method','permanent'], // Permanent
     ['ZI013_FMM','4','farm:method','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM','999','farm:method','other'], // Other

     // ZI013_IRG - Crop Information : Irrigation Method
     // ['ZI013_IRG','-999999',undefined,undefined], // No Information
     ['ZI013_IRG','1','farm:irrigation','furrow'], // Furrow
     ['ZI013_IRG','2','farm:irrigation','overhead'], // Overhead
     ['ZI013_IRG','3','farm:irrigation','centre_pivot'], // Center Pivot
     ['ZI013_IRG','4','farm:irrigation','linear_move'], // Linear Move
     ['ZI013_IRG','5','farm:irrigation','lateral_move'], // Lateral Move
     ['ZI013_IRG','6','farm:irrigation','drip'], // Drip
     ['ZI013_IRG','7','farm:irrigation','subirrigation'], // Subirrigation
     ['ZI013_IRG','8','farm:irrigation','terrace'], // Terrace
     ['ZI013_IRG','999','farm:irrigation','other'], // Other

     // ZI013_PIG - Crop Information : Permanent Irrigation
     // ['ZI013_PIG','-999999',undefined,undefined], // No Information
     ['ZI013_PIG','1000','permanent_irrigation','no'],
     ['ZI013_PIG','1001','permanent_irrigation','yes'],

     // ZI014_PBY - Manufacturing Information : By-product
     // ['ZI014_PBY','-999999',undefined,undefined], // No Information
     ['ZI014_PBY','1','by_product','ash'], // Ash
     ['ZI014_PBY','2','by_product','cinders'], // Cinders
     ['ZI014_PBY','6','by_product','no_byproduct'], // No Byproduct
     ['ZI014_PBY','7','by_product','radioactive_material'], // Radioactive Material
     ['ZI014_PBY','8','by_product','refuse'], // Refuse
     ['ZI014_PBY','12','by_product','scrap-metal'], // Scrap-metal
     ['ZI014_PBY','13','by_product','sewage'], // Sewage
     ['ZI014_PBY','15','by_product','slag'], // Slag
     ['ZI014_PBY','999','by_product','other'], // Other

     // ZI014_PPO - Manufacturing Information : Product
     // ['ZI014_PPO','-999999',undefined,undefined], // No Information
     ['ZI014_PPO','1','product','aircraft'], // Aircraft
     ['ZI014_PPO','2','product','aluminum'], // Aluminum
     ['ZI014_PPO','3','product','ammunition'], // Ammunition
     ['ZI014_PPO','4','product','asphalt'], // Asphalt
     ['ZI014_PPO','5','product','motor_vehicle'], // Motor Vehicle
     ['ZI014_PPO','8','product','basalt'], // Basalt -- From PPO
     ['ZI014_PPO','9','product','bauxite'], // Bauxite -- From PPO
     ['ZI014_PPO','11','product','bivalve_mollusc'], // Bivalve Mollusc -- From PPO
     ['ZI014_PPO','13','product','brick'], // Brick
     ['ZI014_PPO','15','product','cement'], // Cement
     ['ZI014_PPO','16','product','chemical'], // Chemical
     ['ZI014_PPO','17','product','clay'], // Clay -- From PPO
     ['ZI014_PPO','18','product','coal'], // Coal
     ['ZI014_PPO','19','product','cobbles'], // Cobbles -- From PPO
     ['ZI014_PPO','20','product','coffee'], // Coffee
     ['ZI014_PPO','21','product','coke'], // Coke
     ['ZI014_PPO','23','product','concrete'], // Concrete
     ['ZI014_PPO','25','product','consumer_goods'], // Consumer Goods
     ['ZI014_PPO','26','product','copper'], // Copper
     ['ZI014_PPO','28','product','cotton'], // Cotton
     ['ZI014_PPO','29','product','crustacean'], // Crustacean -- From PPO
     ['ZI014_PPO','30','product','cultivated_shellfish'], // Cultivated Shellfish -- From PPO
     ['ZI014_PPO','32','product','desalinated_water'], // Desalinated Water
     ['ZI014_PPO','33','product','diamond'], // Diamond -- From PPO
     ['ZI014_PPO','34','product','diatomaceous_earth'], // Diatomaceous Earth -- From PPO
     ['ZI014_PPO','35','product','dolomite'], // Dolomite -- From PPO
     ['ZI014_PPO','37','product','electric_power'], // Electric Power
     ['ZI014_PPO','38','product','explosive'], // Explosive
     ['ZI014_PPO','39','product','fish'], // Fish
     ['ZI014_PPO','41','product','food'], // Food
     ['ZI014_PPO','43','product','frozen_water'], // Frozen Water
     ['ZI014_PPO','44','product','fruit'], // Fruit
     ['ZI014_PPO','45','product','gas'], // Gas
     ['ZI014_PPO','46','product','petrol'], // Petrol
     ['ZI014_PPO','47','product','glass'], // Glass
     ['ZI014_PPO','48','product','gold'], // Gold
     ['ZI014_PPO','50','product','granite'], // Granite -- From PPO
     ['ZI014_PPO','53','product','gravel'], // Gravel -- From PPO
     ['ZI014_PPO','57','product','ice'], // Ice
     ['ZI014_PPO','58','product','iron'], // Iron
     ['ZI014_PPO','59','product','lead'], // Lead
     ['ZI014_PPO','60','product','lime'], // Lime
     ['ZI014_PPO','61','product','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['ZI014_PPO','62','product','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['ZI014_PPO','63','product','lumber'], // Lumber
     ['ZI014_PPO','65','product','manganese'], // Manganese
     ['ZI014_PPO','66','product','marble'], // Marble -- From PPO
     ['ZI014_PPO','69','product','metal'], // Metal
     ['ZI014_PPO','70','product','milk'], // Milk
     ['ZI014_PPO','72','product','mussels'], // Mussels -- From PPO
     ['ZI014_PPO','73','product','no_product'], // No Product
     ['ZI014_PPO','74','product','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
     ['ZI014_PPO','77','product','oysters'], // Oysters -- From PPO
     ['ZI014_PPO','80','product','paper'], // Paper
     ['ZI014_PPO','83','product','petroleum'], // Petroleum
     ['ZI014_PPO','84','product','plastic'], // Plastic
     ['ZI014_PPO','85','product','porphyry'], // Porphyry -- From PPO
     ['ZI014_PPO','87','product','prestressed_concrete'], // Prestressed Concrete
     ['ZI014_PPO','88','product','pumice'], // Pumice -- From PPO
     ['ZI014_PPO','89','product','quartz'], // Quartz -- From PPO
     ['ZI014_PPO','90','product','radioactive_material'], // Radioactive Material
     ['ZI014_PPO','92','product','rice'], // Rice
     ['ZI014_PPO','93','product','rock'], // Rock -- From PPO
     ['ZI014_PPO','94','product','rubber'], // Rubber
     ['ZI014_PPO','95','product','salt'], // Salt
     ['ZI014_PPO','96','product','sand'], // Sand -- From PPO
     ['ZI014_PPO','97','product','sandstone'], // Sandstone -- From PPO
     ['ZI014_PPO','101','product','sewage'], // Sewage
     ['ZI014_PPO','105','product','silver'], // Silver
     ['ZI014_PPO','106','product','snow'], // Snow
     ['ZI014_PPO','109','product','steel'], // Steel
     ['ZI014_PPO','110','product','stone'], // Stone -- From PPO
     ['ZI014_PPO','111','product','sugar'], // Sugar
     ['ZI014_PPO','114','product','textile'], // Textile
     ['ZI014_PPO','116','product','timber'], // Timber -- From PPO
     ['ZI014_PPO','117','product','tobacco'], // Tobacco
     ['ZI014_PPO','118','product','travertine'], // Travertine -- From PPO
     ['ZI014_PPO','120','product','uranium'], // Uranium
     ['ZI014_PPO','121','product','vegetation_product'], // Vegetation Product
     ['ZI014_PPO','122','product','water'], // Water
     ['ZI014_PPO','123','product','wine'], // Wine
     ['ZI014_PPO','126','product','zinc'], // Zinc
     ['ZI014_PPO','130','product','chalk'], // Chalk -- From PPO
     ['ZI014_PPO','136','product','biochemical'], // Biochemical
     ['ZI014_PPO','137','product','petrochemical'], // Petrochemical
     ['ZI014_PPO','146','product','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['ZI014_PPO','147','product','electronic_equipment'], // Electronic Equipment
     ['ZI014_PPO','148','product','electrical_equipment'], // Electrical Equipment
     ['ZI014_PPO','149','product','brine'], // Brine -- From PPO
     ['ZI014_PPO','150','product','fertilizer'], // Fertilizer
     ['ZI014_PPO','151','product','chromium'], // Chromium -- From PPO
     ['ZI014_PPO','152','product','nickel'], // Nickel -- From PPO
     ['ZI014_PPO','153','product','tin'], // Tin -- From PPO
     ['ZI014_PPO','154','product','munitions'], // Munitions
     ['ZI014_PPO','155','product','olive_oil'], // Olive Oil
     ['ZI014_PPO','156','product','whale_products'], // Whale Products
     ['ZI014_PPO','157','product','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PPO','158','product','pottery'], // Pottery -- From PPO
     ['ZI014_PPO','159','product','charcoal'], // Charcoal -- From PPO
     ['ZI014_PPO','160','product','milled_grain'], // Milled Grain
     ['ZI014_PPO','161','product','coalbed_methane'], // Coalbed Methane -- From PPO
     ['ZI014_PPO','162','product','natural_gas_condensate'], // Natural Gas Condensate -- From PPO
     ['ZI014_PPO','163','product','helium'], // Helium -- From PPO
     ['ZI014_PPO','164','product','hydrothermal_fluid'], // Hydrothermal Fluid -- From PPO
     ['ZI014_PPO','165','product','clothing'], // Clothing
     ['ZI014_PPO','173','product','fluorite'], // Fluorite -- From PPO
     ['ZI014_PPO','192','product','petroleum_lubricant'], // Petroleum Lubricant
     ['ZI014_PPO','204','product','phosphate'], // Phosphate -- From PPO
     ['ZI014_PPO','214','product','biodiesel'], // Biodiesel
     ['ZI014_PPO','224','product','vanadium'], // Vanadium -- From PPO
     ['ZI014_PPO','279','product','fluorine'], // Fluorine
     ['ZI014_PPO','283','product','mica'], // Mica -- From PPO
     ['ZI014_PPO','325','product','phosphorus'], // Phosphorus
     ['ZI014_PPO','339','product','selenium'], // Selenium -- From PPO
     ['ZI014_PPO','435','product','nuclear_fuel'], // Nuclear Fuel
     ['ZI014_PPO','999','product','other'], // Other

     // ZI014_PRW - Manufacturing Information : Raw Material -- Includes codes from PRW
     // ['ZI014_PRW','-999999',undefined,undefined], // No Information
     ['ZI014_PRW','1','raw_material','aluminum'], // Aluminum
     ['ZI014_PRW','2','raw_material','asphalt'], // Asphalt
     ['ZI014_PRW','5','raw_material','bauxite'], // Bauxite
     ['ZI014_PRW','9','raw_material','cement'], // Cement
     ['ZI014_PRW','10','raw_material','chemical'], // Chemical
     ['ZI014_PRW','11','raw_material','clay'], // Clay
     ['ZI014_PRW','12','raw_material','coal'], // Coal
     ['ZI014_PRW','14','raw_material','coke'], // Coke
     ['ZI014_PRW','16','raw_material','copper'], // Copper
     ['ZI014_PRW','18','raw_material','cotton'], // Cotton
     ['ZI014_PRW','27','raw_material','gas'], // Gas
     ['ZI014_PRW','28','raw_material','glass'], // Glass
     ['ZI014_PRW','29','raw_material','gold'], // Gold
     ['ZI014_PRW','33','raw_material','plant_material'], // Plant Material
     ['ZI014_PRW','34','raw_material','gravel'], // Gravel -- Added from PRW
     ['ZI014_PRW','38','raw_material','ice'], // Ice
     ['ZI014_PRW','39','raw_material','iron'], // Iron
     ['ZI014_PRW','41','raw_material','lead'], // Lead
     ['ZI014_PRW','45','raw_material','lumber'], // Lumber
     ['ZI014_PRW','46','raw_material','manganese'], // Manganese
     ['ZI014_PRW','48','raw_material','metal'], // Metal
     ['ZI014_PRW','50','raw_material','no_raw_material'], // No Raw Material
     ['ZI014_PRW','52','raw_material','oil'], // Oil
     ['ZI014_PRW','54','raw_material','ore'], // Ore
     ['ZI014_PRW','57','raw_material','paper'], // Paper
     ['ZI014_PRW','60','raw_material','plastic'], // Plastic
     ['ZI014_PRW','64','raw_material','radioactive_material'], // Radioactive Material
     ['ZI014_PRW','66','raw_material','rubber'], // Rubber
     ['ZI014_PRW','67','raw_material','salt'], // Salt -- Added from PRW
     ['ZI014_PRW','68','raw_material','sand'], // Sand -- Added from PRW
     ['ZI014_PRW','75','raw_material','sewage'], // Sewage
     ['ZI014_PRW','78','raw_material','silver'], // Silver
     ['ZI014_PRW','79','raw_material','snow'], // Snow
     ['ZI014_PRW','83','raw_material','steel'], // Steel
     ['ZI014_PRW','84','raw_material','stone'], // Stone -- Added from PRW
     ['ZI014_PRW','85','raw_material','sugar'], // Sugar
     ['ZI014_PRW','87','raw_material','textile'], // Textile
     ['ZI014_PRW','90','raw_material','tobacco'], // Tobacco
     ['ZI014_PRW','93','raw_material','uranium'], // Uranium
     ['ZI014_PRW','94','raw_material','vegetation'], // Vegetation
     ['ZI014_PRW','96','raw_material','water'], // Water
     ['ZI014_PRW','97','raw_material','wood'], // Wood
     ['ZI014_PRW','99','raw_material','zinc'], // Zinc
     ['ZI014_PRW','118','raw_material','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PRW','154','raw_material','sulphur'], // Sulphur -- Added from PRW
     ['ZI014_PRW','999','raw_material','other'], // Other

     // Some of these have been added to the fuzzy table for export
     // ZI016_ROC - Route Pavement Information : Route Surface Composition
     // ['ZI016_ROC','-999999','surface','unknown'], // Trying this instead of undefined
     ['ZI016_ROC','-999999',undefined,undefined], // No Information
//      ['ZI016_ROC','1','surface','ground'], // Unimproved
//      ['ZI016_ROC','2','surface','compacted'], // Stabilized earth
//      ['ZI016_ROC','3','surface','flexible_pavement'], // Flexible Pavement
//      ['ZI016_ROC','4','surface','gravel'], // Aggregate
//      ['ZI016_ROC','5','surface','macadam'], // Macadam
//      ['ZI016_ROC','6','surface','bound_surface'], // Bound Surface
//      ['ZI016_ROC','7','surface','ridgid_pavement'], // Rigid Pavement
//      ['ZI016_ROC','8','surface','concrete'], // Concrete
//      ['ZI016_ROC','9','surface','asphalt'], // Asphalt
//      ['ZI016_ROC','10','surface','asphalt_over_concrete'], // Asphalt over Concrete
//      ['ZI016_ROC','11','surface','cobblestone'], // Cobble-stone
//      ['ZI016_ROC','12','surface','brick'], // Brick
//      ['ZI016_ROC','13','surface','metal'], // Metal
//      ['ZI016_ROC','14','surface','wood'], // Wood
//      ['ZI016_ROC','15','surface','corduroy'], // rough hewn logs...  // Corduroy
//      ['ZI016_ROC','16','surface','wood_plank'], // Wood Plank
//      ['ZI016_ROC','17','surface','ice'], // Ice
//      ['ZI016_ROC','18','surface','snow'], // Snow
//      ['ZI016_ROC','999','surface','other'], // Other

     // ZI016_WTC - Route Pavement Information : Road Weather Restriction
     // ['ZI016_WTC','-999999',undefined,undefined], // No Information
     ['ZI016_WTC','1','seasonal','no'], // All-weather
     ['ZI016_WTC','2','seasonal','fair'], // possibly seasonal // Fair-weather
     ['ZI016_WTC','3','seasonal','winter'], // Winter Only
     ['ZI016_WTC','4','seasonal','limited'], // Limited all-weather
     ['ZI016_WTC','5','seasonal','not_winter'], // Closed in Winter
     ['ZI016_WTC','999','seasonal','other'], // Other

     // ZI017_RGC - Track Information : Railway Gauge Classification
     // ['ZI017_RGC','-999999',undefined,undefined], // No Information
     ['ZI017_RGC','1','gauge:type','broad'], // Broad
     ['ZI017_RGC','2','gauge:type','narrow'], // Narrow
     ['ZI017_RGC','3','gauge:type','standard'], // Standard

     // ZI017_RIR - Track Information : Railway in Road
     // ['ZI017_RIR','-999999',undefined,undefined], // No Information
     ['ZI017_RIR','1000','railway:in_road','separated'],
     ['ZI017_RIR','1001','railway:in_road','yes'],

     // ZI017_RRA - Track Information : Railway Power Method
     // ['ZI017_RRA','-999999',undefined,undefined], // No Information
     ['ZI017_RRA','1','electrified','rail'], // Electrified Track
     ['ZI017_RRA','3','electrified','overhead'], // Overhead Electrified
     ['ZI017_RRA','4','electrified','no'], // Non-electrified
     ['ZI017_RRA','999','electrified','other'], // Other

     // ZI017_TRT - Track Information : Track Type
     // ['ZI017_TRT','-999999',undefined,undefined], // No Information
     ['ZI017_TRT','1','railway:track','crane'], // Crane Track
     ['ZI017_TRT','2','railway:track','drill'], // Drill Track
     ['ZI017_TRT','3','railway:track','house'], // House Track
     ['ZI017_TRT','4','railway:track','joint'], // Joint Track
     ['ZI017_TRT','5','railway:track','ladder'], // Ladder Track
     ['ZI017_TRT','6','railway:track','paired'], // Paired Track
     ['ZI017_TRT','7','railway:track','rip'], // Rip Track
     ['ZI017_TRT','9','railway:track','stub'], // Stub Track
     ['ZI017_TRT','10','railway:track','team'], // Team Track
     ['ZI017_TRT','12','railway:track','monorail'],
     // ['ZI017_TRT','12','railway','monorail'], // We used this in MGCP // Monorail
     ['ZI017_TRT','13','railway:track','maglev'], // Maglev
     ['ZI017_TRT','999','railway:track','other'], // Other

     // ZI018_WIT - Wireless Telecommunication Information : Wireless Telecommunication Type
     // ['ZI018_WIT','-999999',undefined,undefined], // No Information
     ['ZI018_WIT','1','communication:cellular_phone','yes'], // Cellular Phone
     ['ZI018_WIT','2','communication:microwave_radio_relay','yes'], // Microwave Radio Relay
     ['ZI018_WIT','3','communication:mobile_phone','yes'], // Mobile Phone
     ['ZI018_WIT','4','communication:radio_broadcast','yes'], // Radio Broadcast
     ['ZI018_WIT','5','communication:radio_telephone','yes'], // Radio Telephone
     ['ZI018_WIT','6','communication:radio-telegraph','yes'], // Radio-telegraph
     ['ZI018_WIT','7','communication:television','yes'], // Television (TV)
     ['ZI018_WIT','999','communication:other','yes'], // Other

     // ZI019_ASP - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method
     // ['ZI019_ASP','-999999',undefined,undefined], // No Information
     ['ZI019_ASP','1','aeroway:pavement:preparation','aggregate_seal_coat'], // Aggregate Seal Coat
     ['ZI019_ASP','2','aeroway:pavement:preparation','graded'], // Graded
     ['ZI019_ASP','3','aeroway:pavement:preparation','grass'], // Grass
     ['ZI019_ASP','4','aeroway:pavement:preparation','grooved'], // Grooved
     ['ZI019_ASP','5','aeroway:pavement:preparation','oiled'], // Oiled
     ['ZI019_ASP','6','aeroway:pavement:preparation','porous_friction_course'], // Porous Friction Course
     ['ZI019_ASP','7','aeroway:pavement:preparation','rolled'], // Rolled
     ['ZI019_ASP','8','aeroway:pavement:preparation','rubberized_seal_coat'], // Rubberized Seal Coat
     ['ZI019_ASP','999','aeroway:pavement:preparation','other'], // Other

     // ZI019_ASU - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition
     // ['ZI019_ASU','-999999',undefined,undefined], // No Information
     ['ZI019_ASU','1','aeroway:surface','asphalt'], // Asphalt
     ['ZI019_ASU','2','aeroway:surface','bituminous_mix'], // Bituminous Mix
     ['ZI019_ASU','3','aeroway:surface','brick'], // Brick
     ['ZI019_ASU','4','aeroway:surface','clay'], // Clay
     ['ZI019_ASU','5','aeroway:surface','concrete'], // Concrete
     ['ZI019_ASU','6','aeroway:surface','coral'], // Coral
     ['ZI019_ASU','7','aeroway:surface','earth'], // Earthen
     ['ZI019_ASU','8','aeroway:surface','gravel'], // Gravel
     ['ZI019_ASU','9','aeroway:surface','ice'], // Ice
     ['ZI019_ASU','10','aeroway:surface','landing_mat'], // Landing Mat
     ['ZI019_ASU','11','aeroway:surface','laterite'], // Laterite
     ['ZI019_ASU','12','aeroway:surface','macadam'], // Macadam
     ['ZI019_ASU','13','aeroway:surface','membrane'], // Membrane
     ['ZI019_ASU','14','aeroway:surface','non-bituminous_mix'], // Non-bituminous Mix
     ['ZI019_ASU','15','aeroway:surface','pierced_steel_planking'], // Pierced Steel Planking
     ['ZI019_ASU','16','aeroway:surface','sand'], // Sand
     ['ZI019_ASU','17','aeroway:surface','snow'], // Snow
     ['ZI019_ASU','18','aeroway:surface','stone'], // Stone
     ['ZI019_ASU','19','aeroway:surface','water'], // Water
     ['ZI019_ASU','20','aeroway:surface','wood'], // Wood
     ['ZI019_ASU','21','aeroway:surface','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ZI019_ASU','999','aeroway:surface','other'], // Other

     // ZI019_ASX - Aerodrome Pavement Information : Aerodrome Movement Area Surface Category
     // ['ZI019_ASX','-999999',undefined,undefined], // No Information
     ['ZI019_ASX','1','aeroway:pavement','paved'], // Completely Paved
     ['ZI019_ASX','2','aeroway:pavement','mostly_paved'], // Mostly Paved
     ['ZI019_ASX','3','aeroway:pavement','unprepared'], // Unprepared
     ['ZI019_ASX','4','aeroway:pavement','partially_paved'], // Partially Paved
     ['ZI019_ASX','5','aeroway:pavement','unpaved'], // Unpaved
     ['ZI019_ASX','999','aeroway:pavement','other'], // Other

     // ZI019_SFS - Aerodrome Pavement Information : Aerodrome Pavement Functional Status
     // ['ZI019_SFS','-999999',undefined,undefined], // No Information
     ['ZI019_SFS','1','surface:condition','fair'], // Fair
     ['ZI019_SFS','2','surface:condition','fair_estimated'], // Fair Estimated
     ['ZI019_SFS','3','surface:condition','good'], // Good
     ['ZI019_SFS','4','surface:condition','good_estimated'], // Good Estimated
     ['ZI019_SFS','5','surface:condition','poor'], // Poor
     ['ZI019_SFS','6','surface:condition','poor_estimated'], // Poor Estimated
     ['ZI019_SFS','7','surface:condition','under_construction'], // Under Construction
     ['ZI019_SFS','8','surface:condition','unserviceable'], // Unserviceable
     ['ZI019_SFS','9','surface:condition','excellent'], // Excellent

     // ZI020_GE4 - (Location Country) Designation : GENC Short URN-based Identifier
     // ['ZI020_GE4','noInformation',undefined,undefined], // No Information
     ['ZI020_GE4','other','is_in:country_code','other'], // Other
     ['ZI020_GE4','ge:GENC:3:1-2:AFG','is_in:country_code','AF'], // Islamic Republic of Afghanistan
     ['ZI020_GE4','ge:GENC:3:1-2:XQZ','is_in:country_code','QZ'], // Akrotiri
     ['ZI020_GE4','ge:GENC:3:1-2:ALB','is_in:country_code','AL'], // Republic of Albania
     ['ZI020_GE4','ge:GENC:3:1-2:DZA','is_in:country_code','DZ'], // Peoples Democratic Republic of Algeria
     ['ZI020_GE4','ge:GENC:3:1-2:ASM','is_in:country_code','AS'], // Territory of American Samoa
     ['ZI020_GE4','ge:GENC:3:1-2:AND','is_in:country_code','AD'], // Principality of Andorra
     ['ZI020_GE4','ge:GENC:3:1-2:AGO','is_in:country_code','AO'], // Republic of Angola
     ['ZI020_GE4','ge:ISO1:3:VI-15:AIA','is_in:country_code','AI'], // Anguilla
     ['ZI020_GE4','ge:ISO1:3:VI-15:ATA','is_in:country_code','AQ'], // Antarctica
     ['ZI020_GE4','ge:GENC:3:1-2:ATG','is_in:country_code','AG'], // Antigua and Barbuda
     ['ZI020_GE4','ge:GENC:3:1-2:ARG','is_in:country_code','AR'], // Argentine Republic
     ['ZI020_GE4','ge:GENC:3:1-2:ARM','is_in:country_code','AM'], // Republic of Armenia
     ['ZI020_GE4','ge:GENC:3:1-2:ABW','is_in:country_code','AW'], // Aruba
     ['ZI020_GE4','ge:GENC:3:1-2:XAC','is_in:country_code','XA'], // Territory of Ashmore and Cartier Islands
     ['ZI020_GE4','ge:GENC:3:1-2:AUS','is_in:country_code','AU'], // Commonwealth of Australia
     ['ZI020_GE4','ge:GENC:3:1-2:AUT','is_in:country_code','AT'], // Republic of Austria
     ['ZI020_GE4','ge:GENC:3:1-2:AZE','is_in:country_code','AZ'], // Republic of Azerbaijan
     ['ZI020_GE4','ge:GENC:3:1-2:BHS','is_in:country_code','BS'], // Commonwealth of The Bahamas
     ['ZI020_GE4','ge:GENC:3:1-2:BHR','is_in:country_code','BH'], // Kingdom of Bahrain
     ['ZI020_GE4','ge:GENC:3:1-2:XBK','is_in:country_code','XB'], // Baker Island
     ['ZI020_GE4','ge:GENC:3:1-2:BGD','is_in:country_code','BD'], // Peoples Republic of Bangladesh
     ['ZI020_GE4','ge:ISO1:3:VI-15:BRB','is_in:country_code','BB'], // Barbados
     ['ZI020_GE4','ge:GENC:3:1-2:XBI','is_in:country_code','QS'], // Bassas da India
     ['ZI020_GE4','ge:GENC:3:1-2:BLR','is_in:country_code','BY'], // Republic of Belarus
     ['ZI020_GE4','ge:GENC:3:1-2:BEL','is_in:country_code','BE'], // Kingdom of Belgium
     ['ZI020_GE4','ge:ISO1:3:VI-15:BLZ','is_in:country_code','BZ'], // Belize
     ['ZI020_GE4','ge:GENC:3:1-2:BEN','is_in:country_code','BJ'], // Republic of Benin
     ['ZI020_GE4','ge:ISO1:3:VI-15:BMU','is_in:country_code','BM'], // Bermuda
     ['ZI020_GE4','ge:GENC:3:1-2:BTN','is_in:country_code','BT'], // Kingdom of Bhutan
     ['ZI020_GE4','ge:GENC:3:1-2:BOL','is_in:country_code','BO'], // Plurinational State of Bolivia
     ['ZI020_GE4','ge:GENC:3:1-2:BES','is_in:country_code','BQ'], // Bonaire, Sint Eustatius, and Saba
     ['ZI020_GE4','ge:ISO1:3:VI-15:BIH','is_in:country_code','BA'], // Bosnia and Herzegovina
     ['ZI020_GE4','ge:GENC:3:1-2:BWA','is_in:country_code','BW'], // Republic of Botswana
     ['ZI020_GE4','ge:ISO1:3:VI-15:BVT','is_in:country_code','BV'], // Bouvet Island
     ['ZI020_GE4','ge:GENC:3:1-2:BRA','is_in:country_code','BR'], // Federative Republic of Brazil
     ['ZI020_GE4','ge:GENC:3:1-2:IOT','is_in:country_code','IO'], // British Indian Ocean Territory
     ['ZI020_GE4','ge:GENC:3:1-2:BRN','is_in:country_code','BN'], // Brunei Darussalam
     ['ZI020_GE4','ge:GENC:3:1-2:BGR','is_in:country_code','BG'], // Republic of Bulgaria
     ['ZI020_GE4','ge:GENC:3:1-2:BFA','is_in:country_code','BF'], // Burkina Faso
     ['ZI020_GE4','ge:GENC:3:1-2:MMR','is_in:country_code','MM'], // Union of Burma
     ['ZI020_GE4','ge:GENC:3:1-2:BDI','is_in:country_code','BI'], // Republic of Burundi
     ['ZI020_GE4','ge:GENC:3:1-2:KHM','is_in:country_code','KH'], // Kingdom of Cambodia
     ['ZI020_GE4','ge:GENC:3:1-2:CMR','is_in:country_code','CM'], // Republic of Cameroon
     ['ZI020_GE4','ge:GENC:3:1-2:CAN','is_in:country_code','CA'], // Canada
     ['ZI020_GE4','ge:GENC:3:1-2:CPV','is_in:country_code','CV'], // Republic of Cape Verde
     ['ZI020_GE4','ge:GENC:3:1-2:CYM','is_in:country_code','KY'], // Cayman Islands
     ['ZI020_GE4','ge:GENC:3:1-2:CAF','is_in:country_code','CF'], // Central African Republic
     ['ZI020_GE4','ge:GENC:3:1-2:TCD','is_in:country_code','TD'], // Republic of Chad
     ['ZI020_GE4','ge:GENC:3:1-2:CHL','is_in:country_code','CL'], // Republic of Chile
     ['ZI020_GE4','ge:GENC:3:1-2:CHN','is_in:country_code','CN'], // Peoples Republic of China
     ['ZI020_GE4','ge:GENC:3:1-2:CXR','is_in:country_code','CX'], // Territory of Christmas Island
     ['ZI020_GE4','ge:GENC:3:1-2:CPT','is_in:country_code','CP'], // Clipperton Island
     ['ZI020_GE4','ge:GENC:3:1-2:CCK','is_in:country_code','CC'], // Territory of Cocos (Keeling) Islands
     ['ZI020_GE4','ge:GENC:3:1-2:COL','is_in:country_code','CO'], // Republic of Colombia
     ['ZI020_GE4','ge:GENC:3:1-2:COM','is_in:country_code','KM'], // Union of the Comoros
     ['ZI020_GE4','ge:GENC:3:1-2:COG','is_in:country_code','CG'], // Republic of the Congo
     ['ZI020_GE4','ge:GENC:3:1-2:COD','is_in:country_code','CD'], // Democratic Republic of the Congo
     ['ZI020_GE4','ge:GENC:3:1-2:COK','is_in:country_code','CK'], // Cook Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XCS','is_in:country_code','XC'], // Coral Sea Islands Territory
     ['ZI020_GE4','ge:GENC:3:1-2:CRI','is_in:country_code','CR'], // Republic of Costa Rica
     ['ZI020_GE4','ge:GENC:3:1-2:CIV','is_in:country_code','CI'], // Republic of Côte dIvoire
     ['ZI020_GE4','ge:GENC:3:1-2:HRV','is_in:country_code','HR'], // Republic of Croatia
     ['ZI020_GE4','ge:GENC:3:1-2:CUB','is_in:country_code','CU'], // Republic of Cuba
     ['ZI020_GE4','ge:GENC:3:1-2:CUW','is_in:country_code','CW'], // Curaçao
     ['ZI020_GE4','ge:GENC:3:1-2:CYP','is_in:country_code','CY'], // Republic of Cyprus
     ['ZI020_GE4','ge:GENC:3:1-2:CZE','is_in:country_code','CZ'], // Czech Republic
     ['ZI020_GE4','ge:GENC:3:1-2:DNK','is_in:country_code','DK'], // Kingdom of Denmark
     ['ZI020_GE4','ge:GENC:3:1-2:XXD','is_in:country_code','XD'], // Dhekelia
     ['ZI020_GE4','ge:GENC:3:1-2:DGA','is_in:country_code','DG'], // Diego Garcia
     ['ZI020_GE4','ge:GENC:3:1-2:DJI','is_in:country_code','DJ'], // Republic of Djibouti
     ['ZI020_GE4','ge:GENC:3:1-2:DMA','is_in:country_code','DM'], // Commonwealth of Dominica
     ['ZI020_GE4','ge:GENC:3:1-2:DOM','is_in:country_code','DO'], // Dominican Republic
     ['ZI020_GE4','ge:GENC:3:1-2:ECU','is_in:country_code','EC'], // Republic of Ecuador
     ['ZI020_GE4','ge:GENC:3:1-2:EGY','is_in:country_code','EG'], // Arab Republic of Egypt
     ['ZI020_GE4','ge:GENC:3:1-2:SLV','is_in:country_code','SV'], // Republic of El Salvador
     ['ZI020_GE4','ge:GENC:3:1-2:XAZ','is_in:country_code','XAZ'], // Entity 1
     ['ZI020_GE4','ge:GENC:3:1-2:XCR','is_in:country_code','XCR'], // Entity 2
     ['ZI020_GE4','ge:GENC:3:1-2:XCY','is_in:country_code','XCY'], // Entity 3
     ['ZI020_GE4','ge:GENC:3:1-2:XKM','is_in:country_code','XKM'], // Entity 4
     ['ZI020_GE4','ge:GENC:3:1-2:XKN','is_in:country_code','XKN'], // Entity 5
     ['ZI020_GE4','ge:GENC:3:1-2:GNQ','is_in:country_code','GQ'], // Republic of Equatorial Guinea
     ['ZI020_GE4','ge:GENC:3:1-2:ERI','is_in:country_code','ER'], // State of Eritrea
     ['ZI020_GE4','ge:GENC:3:1-2:EST','is_in:country_code','EE'], // Republic of Estonia
     ['ZI020_GE4','ge:GENC:3:1-2:ETH','is_in:country_code','ET'], // Federal Democratic Republic of Ethiopia
     ['ZI020_GE4','ge:GENC:3:1-2:XQP','is_in:country_code','QP'], // Etorofu, Habomai, Kunashiri, and Shikotan Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XEU','is_in:country_code','XE'], // Europa Island
     ['ZI020_GE4','ge:GENC:3:1-2:FLK','is_in:country_code','FK'], // Falkland Islands (Islas Malvinas)
     ['ZI020_GE4','ge:GENC:3:1-2:FRO','is_in:country_code','FO'], // Faroe Islands
     ['ZI020_GE4','ge:GENC:3:1-2:FJI','is_in:country_code','FJ'], // Republic of Fiji
     ['ZI020_GE4','ge:GENC:3:1-2:FIN','is_in:country_code','FI'], // Republic of Finland
     ['ZI020_GE4','ge:GENC:3:1-2:FRA','is_in:country_code','FR'], // French Republic
     ['ZI020_GE4','ge:GENC:3:1-2:GUF','is_in:country_code','GF'], // Department of Guiana
     ['ZI020_GE4','ge:GENC:3:1-2:PYF','is_in:country_code','PF'], // French Polynesia
     ['ZI020_GE4','ge:GENC:3:1-2:ATF','is_in:country_code','TF'], // French Southern and Antarctic Lands
     ['ZI020_GE4','ge:GENC:3:1-2:GAB','is_in:country_code','GA'], // Gabonese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:GMB','is_in:country_code','GM'], // Republic of The Gambia
     ['ZI020_GE4','ge:GENC:3:1-2:XGZ','is_in:country_code','XG'], // Gaza Strip
     ['ZI020_GE4','ge:GENC:3:1-2:GEO','is_in:country_code','GE'], // Georgia
     ['ZI020_GE4','ge:GENC:3:1-2:DEU','is_in:country_code','DE'], // Federal Republic of Germany
     ['ZI020_GE4','ge:GENC:3:1-2:GHA','is_in:country_code','GH'], // Republic of Ghana
     ['ZI020_GE4','ge:ISO1:3:VI-15:GIB','is_in:country_code','GI'], // Gibraltar
     ['ZI020_GE4','ge:GENC:3:1-2:XGL','is_in:country_code','QX'], // Glorioso Islands
     ['ZI020_GE4','ge:GENC:3:1-2:GRC','is_in:country_code','GR'], // Hellenic Republic
     ['ZI020_GE4','ge:ISO1:3:VI-15:GRL','is_in:country_code','GL'], // Greenland
     ['ZI020_GE4','ge:ISO1:3:VI-15:GRD','is_in:country_code','GD'], // Grenada
     ['ZI020_GE4','ge:GENC:3:1-2:GLP','is_in:country_code','GP'], // Department of Guadeloupe
     ['ZI020_GE4','ge:GENC:3:1-2:GUM','is_in:country_code','GU'], // Territory of Guam
     ['ZI020_GE4','ge:GENC:3:1-2:AX2','is_in:country_code','A2'], // Guantanamo Bay Naval Base
     ['ZI020_GE4','ge:GENC:3:1-2:GTM','is_in:country_code','GT'], // Republic of Guatemala
     ['ZI020_GE4','ge:GENC:3:1-2:GGY','is_in:country_code','GG'], // Bailiwick of Guernsey
     ['ZI020_GE4','ge:GENC:3:1-2:GIN','is_in:country_code','GN'], // Republic of Guinea
     ['ZI020_GE4','ge:GENC:3:1-2:GNB','is_in:country_code','GW'], // Republic of Guinea-Bissau
     ['ZI020_GE4','ge:GENC:3:1-2:GUY','is_in:country_code','GY'], // Co-operative Republic of Guyana
     ['ZI020_GE4','ge:GENC:3:1-2:HTI','is_in:country_code','HT'], // Republic of Haiti
     ['ZI020_GE4','ge:GENC:3:1-2:HMD','is_in:country_code','HM'], // Territory of Heard Island and McDonald Islands
     ['ZI020_GE4','ge:GENC:3:1-2:HND','is_in:country_code','HN'], // Republic of Honduras
     ['ZI020_GE4','ge:GENC:3:1-2:HKG','is_in:country_code','HK'], // Hong Kong Special Administrative Region
     ['ZI020_GE4','ge:GENC:3:1-2:XHO','is_in:country_code','XH'], // Howland Island
     ['ZI020_GE4','ge:ISO1:3:VI-15:HUN','is_in:country_code','HU'], // Hungary
     ['ZI020_GE4','ge:GENC:3:1-2:ISL','is_in:country_code','IS'], // Republic of Iceland
     ['ZI020_GE4','ge:GENC:3:1-2:IND','is_in:country_code','IN'], // Republic of India
     ['ZI020_GE4','ge:GENC:3:1-2:IDN','is_in:country_code','ID'], // Republic of Indonesia
     ['ZI020_GE4','ge:GENC:3:1-2:IRN','is_in:country_code','IR'], // Islamic Republic of Iran
     ['ZI020_GE4','ge:GENC:3:1-2:IRQ','is_in:country_code','IQ'], // Republic of Iraq
     ['ZI020_GE4','ge:ISO1:3:VI-15:IRL','is_in:country_code','IE'], // Ireland
     ['ZI020_GE4','ge:ISO1:3:VI-15:IMN','is_in:country_code','IM'], // Isle of Man
     ['ZI020_GE4','ge:GENC:3:1-2:ISR','is_in:country_code','IL'], // State of Israel
     ['ZI020_GE4','ge:GENC:3:1-2:ITA','is_in:country_code','IT'], // Italian Republic
     ['ZI020_GE4','ge:ISO1:3:VI-15:JAM','is_in:country_code','JM'], // Jamaica
     ['ZI020_GE4','ge:GENC:3:1-2:XJM','is_in:country_code','XJ'], // Jan Mayen
     ['ZI020_GE4','ge:ISO1:3:VI-15:JPN','is_in:country_code','JP'], // Japan
     ['ZI020_GE4','ge:GENC:3:1-2:XJV','is_in:country_code','XQ'], // Jarvis Island
     ['ZI020_GE4','ge:GENC:3:1-2:JEY','is_in:country_code','JE'], // Bailiwick of Jersey
     ['ZI020_GE4','ge:GENC:3:1-2:XJA','is_in:country_code','XU'], // Johnston Atoll
     ['ZI020_GE4','ge:GENC:3:1-2:JOR','is_in:country_code','JO'], // Hashemite Kingdom of Jordan
     ['ZI020_GE4','ge:GENC:3:1-2:XJN','is_in:country_code','QU'], // Juan de Nova Island
     ['ZI020_GE4','ge:GENC:3:1-2:KAZ','is_in:country_code','KZ'], // Republic of Kazakhstan
     ['ZI020_GE4','ge:GENC:3:1-2:KEN','is_in:country_code','KE'], // Republic of Kenya
     ['ZI020_GE4','ge:GENC:3:1-2:XKR','is_in:country_code','XM'], // Kingman Reef
     ['ZI020_GE4','ge:GENC:3:1-2:KIR','is_in:country_code','KI'], // Republic of Kiribati
     ['ZI020_GE4','ge:GENC:3:1-2:PRK','is_in:country_code','KP'], // Democratic Peoples Republic of Korea
     ['ZI020_GE4','ge:GENC:3:1-2:KOR','is_in:country_code','KR'], // Republic of Korea
     ['ZI020_GE4','ge:GENC:3:1-2:XKS','is_in:country_code','XK'], // Republic of Kosovo
     ['ZI020_GE4','ge:GENC:3:1-2:KWT','is_in:country_code','KW'], // State of Kuwait
     ['ZI020_GE4','ge:GENC:3:1-2:KGZ','is_in:country_code','KG'], // Kyrgyz Republic
     ['ZI020_GE4','ge:GENC:3:1-2:LAO','is_in:country_code','LA'], // Lao Peoples Democratic Republic
     ['ZI020_GE4','ge:GENC:3:1-2:LVA','is_in:country_code','LV'], // Republic of Latvia
     ['ZI020_GE4','ge:GENC:3:1-2:LBN','is_in:country_code','LB'], // Lebanese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:LSO','is_in:country_code','LS'], // Kingdom of Lesotho
     ['ZI020_GE4','ge:GENC:3:1-2:LBR','is_in:country_code','LR'], // Republic of Liberia
     ['ZI020_GE4','ge:ISO1:3:VI-15:LBY','is_in:country_code','LY'], // Libya
     ['ZI020_GE4','ge:GENC:3:1-2:LIE','is_in:country_code','LI'], // Principality of Liechtenstein
     ['ZI020_GE4','ge:GENC:3:1-2:LTU','is_in:country_code','LT'], // Republic of Lithuania
     ['ZI020_GE4','ge:GENC:3:1-2:LUX','is_in:country_code','LU'], // Grand Duchy of Luxembourg
     ['ZI020_GE4','ge:GENC:3:1-2:MAC','is_in:country_code','MO'], // Macau Special Administrative Region
     ['ZI020_GE4','ge:GENC:3:1-2:MKD','is_in:country_code','MK'], // Republic of Macedonia
     ['ZI020_GE4','ge:GENC:3:1-2:MDG','is_in:country_code','MG'], // Republic of Madagascar
     ['ZI020_GE4','ge:GENC:3:1-2:MWI','is_in:country_code','MW'], // Republic of Malawi
     ['ZI020_GE4','ge:GENC:3:1-2:MYS','is_in:country_code','MY'], // Malaysia
     ['ZI020_GE4','ge:GENC:3:1-2:MDV','is_in:country_code','MV'], // Republic of Maldives
     ['ZI020_GE4','ge:GENC:3:1-2:MLI','is_in:country_code','ML'], // Republic of Mali
     ['ZI020_GE4','ge:GENC:3:1-2:MLT','is_in:country_code','MT'], // Republic of Malta
     ['ZI020_GE4','ge:GENC:3:1-2:MHL','is_in:country_code','MH'], // Republic of the Marshall Islands
     ['ZI020_GE4','ge:GENC:3:1-2:MTQ','is_in:country_code','MQ'], // Department of Martinique
     ['ZI020_GE4','ge:GENC:3:1-2:MRT','is_in:country_code','MR'], // Islamic Republic of Mauritania
     ['ZI020_GE4','ge:GENC:3:1-2:MUS','is_in:country_code','MU'], // Republic of Mauritius
     ['ZI020_GE4','ge:GENC:3:1-2:MYT','is_in:country_code','YT'], // Department of Mayotte
     ['ZI020_GE4','ge:GENC:3:1-2:MEX','is_in:country_code','MX'], // United Mexican States
     ['ZI020_GE4','ge:GENC:3:1-2:FSM','is_in:country_code','FM'], // Federated States of Micronesia
     ['ZI020_GE4','ge:GENC:3:1-2:XMW','is_in:country_code','QM'], // Midway Islands
     ['ZI020_GE4','ge:GENC:3:1-2:MDA','is_in:country_code','MD'], // Republic of Moldova
     ['ZI020_GE4','ge:GENC:3:1-2:MCO','is_in:country_code','MC'], // Principality of Monaco
     ['ZI020_GE4','ge:GENC:3:1-2:MNG','is_in:country_code','MN'], // Mongolia
     ['ZI020_GE4','ge:ISO1:3:VI-15:MNE','is_in:country_code','ME'], // Montenegro
     ['ZI020_GE4','ge:ISO1:3:VI-15:MSR','is_in:country_code','MS'], // Montserrat
     ['ZI020_GE4','ge:GENC:3:1-2:MAR','is_in:country_code','MA'], // Kingdom of Morocco
     ['ZI020_GE4','ge:GENC:3:1-2:MOZ','is_in:country_code','MZ'], // Republic of Mozambique
     ['ZI020_GE4','ge:GENC:3:1-2:NAM','is_in:country_code','NA'], // Republic of Namibia
     ['ZI020_GE4','ge:GENC:3:1-2:NRU','is_in:country_code','NR'], // Republic of Nauru
     ['ZI020_GE4','ge:GENC:3:1-2:XNV','is_in:country_code','XV'], // Navassa Island
     ['ZI020_GE4','ge:GENC:3:1-2:NPL','is_in:country_code','NP'], // Federal Democratic Republic of Nepal
     ['ZI020_GE4','ge:GENC:3:1-2:NLD','is_in:country_code','NL'], // Kingdom of the Netherlands
     ['ZI020_GE4','ge:GENC:3:1-2:NCL','is_in:country_code','NC'], // New Caledonia
     ['ZI020_GE4','ge:GENC:3:1-2:NZL','is_in:country_code','NZ'], // New Zealand
     ['ZI020_GE4','ge:GENC:3:1-2:NIC','is_in:country_code','NI'], // Republic of Nicaragua
     ['ZI020_GE4','ge:GENC:3:1-2:NER','is_in:country_code','NE'], // Republic of the Niger
     ['ZI020_GE4','ge:GENC:3:1-2:NGA','is_in:country_code','NG'], // Federal Republic of Nigeria
     ['ZI020_GE4','ge:ISO1:3:VI-15:NIU','is_in:country_code','NU'], // Niue
     ['ZI020_GE4','ge:GENC:3:1-2:XXX','is_in:country_code','[None]'], // No Mans Land
     ['ZI020_GE4','ge:GENC:3:1-2:NFK','is_in:country_code','NF'], // Territory of Norfolk Island
     ['ZI020_GE4','ge:GENC:3:1-2:MNP','is_in:country_code','MP'], // Commonwealth of the Northern Mariana Islands
     ['ZI020_GE4','ge:GENC:3:1-2:NOR','is_in:country_code','NO'], // Kingdom of Norway
     ['ZI020_GE4','ge:GENC:3:1-2:OMN','is_in:country_code','OM'], // Sultanate of Oman
     ['ZI020_GE4','ge:GENC:3:1-2:PAK','is_in:country_code','PK'], // Islamic Republic of Pakistan
     ['ZI020_GE4','ge:GENC:3:1-2:PLW','is_in:country_code','PW'], // Republic of Palau
     ['ZI020_GE4','ge:GENC:3:1-2:PSE','is_in:country_code','PS'], // Palestinian Territory
     ['ZI020_GE4','ge:GENC:3:1-2:XPL','is_in:country_code','XL'], // Palmyra Atoll
     ['ZI020_GE4','ge:GENC:3:1-2:PAN','is_in:country_code','PA'], // Republic of Panama
     ['ZI020_GE4','ge:ISO1:3:VI-15:PNG','is_in:country_code','PG'], // Independent State of Papua New Guinea
     ['ZI020_GE4','ge:GENC:3:1-2:XPR','is_in:country_code','XP'], // Paracel Islands
     ['ZI020_GE4','ge:GENC:3:1-2:PRY','is_in:country_code','PY'], // Republic of Paraguay
     ['ZI020_GE4','ge:GENC:3:1-2:PER','is_in:country_code','PE'], // Republic of Peru
     ['ZI020_GE4','ge:GENC:3:1-2:PHL','is_in:country_code','PH'], // Republic of the Philippines
     ['ZI020_GE4','ge:GENC:3:1-2:PCN','is_in:country_code','PN'], // Pitcairn, Henderson, Ducie, and Oeno Islands
     ['ZI020_GE4','ge:GENC:3:1-2:POL','is_in:country_code','PL'], // Republic of Poland
     ['ZI020_GE4','ge:GENC:3:1-2:PRT','is_in:country_code','PT'], // Portuguese Republic
     ['ZI020_GE4','ge:GENC:3:1-2:PRI','is_in:country_code','PR'], // Commonwealth of Puerto Rico
     ['ZI020_GE4','ge:GENC:3:1-2:QAT','is_in:country_code','QA'], // State of Qatar
     ['ZI020_GE4','ge:GENC:3:1-2:REU','is_in:country_code','RE'], // Department of Reunion
     ['ZI020_GE4','ge:ISO1:3:VI-15:ROU','is_in:country_code','RO'], // Romania
     ['ZI020_GE4','ge:GENC:3:1-2:RUS','is_in:country_code','RU'], // Russian Federation
     ['ZI020_GE4','ge:GENC:3:1-2:RWA','is_in:country_code','RW'], // Republic of Rwanda
     ['ZI020_GE4','ge:GENC:3:1-2:BLM','is_in:country_code','BL'], // Saint Barthelemy
     ['ZI020_GE4','ge:GENC:3:1-2:SHN','is_in:country_code','SH'], // Saint Helena, Ascension, and Tristan da Cunha
     ['ZI020_GE4','ge:GENC:3:1-2:KNA','is_in:country_code','KN'], // Federation of Saint Kitts and Nevis
     ['ZI020_GE4','ge:ISO1:3:VI-15:LCA','is_in:country_code','LC'], // Saint Lucia
     ['ZI020_GE4','ge:GENC:3:1-2:MAF','is_in:country_code','MF'], // Saint Martin
     ['ZI020_GE4','ge:GENC:3:1-2:SPM','is_in:country_code','PM'], // Territorial Collectivity of Saint Pierre and Miquelon
     ['ZI020_GE4','ge:GENC:3:1-2:VCT','is_in:country_code','VC'], // Saint Vincent and the Grenadines
     ['ZI020_GE4','ge:GENC:3:1-2:WSM','is_in:country_code','WS'], // Independent State of Samoa
     ['ZI020_GE4','ge:GENC:3:1-2:SMR','is_in:country_code','SM'], // Republic of San Marino
     ['ZI020_GE4','ge:GENC:3:1-2:STP','is_in:country_code','ST'], // Democratic Republic of Sao Tome and Principe
     ['ZI020_GE4','ge:GENC:3:1-2:SAU','is_in:country_code','SA'], // Kingdom of Saudi Arabia
     ['ZI020_GE4','ge:GENC:3:1-2:SEN','is_in:country_code','SN'], // Republic of Senegal
     ['ZI020_GE4','ge:GENC:3:1-2:SRB','is_in:country_code','RS'], // Republic of Serbia
     ['ZI020_GE4','ge:GENC:3:1-2:SYC','is_in:country_code','SC'], // Republic of Seychelles
     ['ZI020_GE4','ge:GENC:3:1-2:SLE','is_in:country_code','SL'], // Republic of Sierra Leone
     ['ZI020_GE4','ge:GENC:3:1-2:SGP','is_in:country_code','SG'], // Republic of Singapore
     ['ZI020_GE4','ge:GENC:3:1-2:SXM','is_in:country_code','SX'], // Sint Maarten
     ['ZI020_GE4','ge:GENC:3:1-2:SVK','is_in:country_code','SK'], // Slovak Republic
     ['ZI020_GE4','ge:GENC:3:1-2:SVN','is_in:country_code','SI'], // Republic of Slovenia
     ['ZI020_GE4','ge:GENC:3:1-2:SLB','is_in:country_code','SB'], // Solomon Islands
     ['ZI020_GE4','ge:GENC:3:1-2:SOM','is_in:country_code','SO'], // Somalia, Federal Republic of
     ['ZI020_GE4','ge:GENC:3:1-2:ZAF','is_in:country_code','ZA'], // Republic of South Africa
     ['ZI020_GE4','ge:GENC:3:1-2:SGS','is_in:country_code','GS'], // South Georgia and South Sandwich Islands
     ['ZI020_GE4','ge:GENC:3:1-2:SSD','is_in:country_code','SS'], // Republic of South Sudan
     ['ZI020_GE4','ge:GENC:3:1-2:ESP','is_in:country_code','ES'], // Kingdom of Spain
     ['ZI020_GE4','ge:GENC:3:1-2:XSP','is_in:country_code','XS'], // Spratly Islands
     ['ZI020_GE4','ge:GENC:3:1-2:LKA','is_in:country_code','LK'], // Democratic Socialist Republic of Sri Lanka
     ['ZI020_GE4','ge:GENC:3:1-2:SDN','is_in:country_code','SD'], // Republic of the Sudan
     ['ZI020_GE4','ge:GENC:3:1-2:SUR','is_in:country_code','SR'], // Republic of Suriname
     ['ZI020_GE4','ge:GENC:3:1-2:XSV','is_in:country_code','XR'], // Svalbard
     ['ZI020_GE4','ge:GENC:3:1-2:SWZ','is_in:country_code','SZ'], // Kingdom of Swaziland
     ['ZI020_GE4','ge:GENC:3:1-2:SWE','is_in:country_code','SE'], // Kingdom of Sweden
     ['ZI020_GE4','ge:GENC:3:1-2:CHE','is_in:country_code','CH'], // Swiss Confederation
     ['ZI020_GE4','ge:GENC:3:1-2:SYR','is_in:country_code','SY'], // Syrian Arab Republic
     ['ZI020_GE4','ge:GENC:3:1-2:TWN','is_in:country_code','TW'], // Taiwan
     ['ZI020_GE4','ge:GENC:3:1-2:TJK','is_in:country_code','TJ'], // Republic of Tajikistan
     ['ZI020_GE4','ge:GENC:3:1-2:TZA','is_in:country_code','TZ'], // United Republic of Tanzania
     ['ZI020_GE4','ge:GENC:3:1-2:THA','is_in:country_code','TH'], // Kingdom of Thailand
     ['ZI020_GE4','ge:GENC:3:1-2:TLS','is_in:country_code','TL'], // Democratic Republic of Timor-Leste
     ['ZI020_GE4','ge:GENC:3:1-2:TGO','is_in:country_code','TG'], // Togolese Republic
     ['ZI020_GE4','ge:ISO1:3:VI-15:TKL','is_in:country_code','TK'], // Tokelau
     ['ZI020_GE4','ge:GENC:3:1-2:TON','is_in:country_code','TO'], // Kingdom of Tonga
     ['ZI020_GE4','ge:GENC:3:1-2:TTO','is_in:country_code','TT'], // Republic of Trinidad and Tobago
     ['ZI020_GE4','ge:GENC:3:1-2:XTR','is_in:country_code','XT'], // Tromelin Island
     ['ZI020_GE4','ge:GENC:3:1-2:TUN','is_in:country_code','TN'], // Tunisian Republic
     ['ZI020_GE4','ge:GENC:3:1-2:TUR','is_in:country_code','TR'], // Republic of Turkey
     ['ZI020_GE4','ge:ISO1:3:VI-15:TKM','is_in:country_code','TM'], // Turkmenistan
     ['ZI020_GE4','ge:GENC:3:1-2:TCA','is_in:country_code','TC'], // Turks and Caicos Islands
     ['ZI020_GE4','ge:ISO1:3:VI-15:TUV','is_in:country_code','TV'], // Tuvalu
     ['ZI020_GE4','ge:GENC:3:1-2:UGA','is_in:country_code','UG'], // Republic of Uganda
     ['ZI020_GE4','ge:GENC:3:1-2:UKR','is_in:country_code','UA'], // Ukraine
     ['ZI020_GE4','ge:GENC:3:1-2:ARE','is_in:country_code','AE'], // United Arab Emirates
     ['ZI020_GE4','ge:GENC:3:1-2:GBR','is_in:country_code','GB'], // United Kingdom of Great Britain and Northern Ireland
     ['ZI020_GE4','ge:GENC:3:1-2:USA','is_in:country_code','US'], // United States of America
     ['ZI020_GE4','ge:GENC:3:1-2:AX1','is_in:country_code','A1'], // Unknown
     ['ZI020_GE4','ge:GENC:3:1-2:URY','is_in:country_code','UY'], // Oriental Republic of Uruguay
     ['ZI020_GE4','ge:GENC:3:1-2:UZB','is_in:country_code','UZ'], // Republic of Uzbekistan
     ['ZI020_GE4','ge:GENC:3:1-2:VUT','is_in:country_code','VU'], // Republic of Vanuatu
     ['ZI020_GE4','ge:GENC:3:1-2:VAT','is_in:country_code','VA'], // State of the Vatican City
     ['ZI020_GE4','ge:GENC:3:1-2:VEN','is_in:country_code','VE'], // Bolivarian Republic of Venezuela
     ['ZI020_GE4','ge:GENC:3:1-2:VNM','is_in:country_code','VN'], // Socialist Republic of Vietnam
     ['ZI020_GE4','ge:GENC:3:1-2:VGB','is_in:country_code','VG'], // Virgin Islands, British
     ['ZI020_GE4','ge:GENC:3:1-2:VIR','is_in:country_code','VI'], // United States Virgin Islands
     ['ZI020_GE4','ge:GENC:3:1-2:XWK','is_in:country_code','QW'], // Wake Island
     ['ZI020_GE4','ge:GENC:3:1-2:WLF','is_in:country_code','WF'], // Wallis and Futuna
     ['ZI020_GE4','ge:GENC:3:1-2:XWB','is_in:country_code','XW'], // West Bank
     ['ZI020_GE4','ge:GENC:3:1-2:ESH','is_in:country_code','EH'], // Western Sahara
     ['ZI020_GE4','ge:GENC:3:1-2:YEM','is_in:country_code','YE'], // Republic of Yemen
     ['ZI020_GE4','ge:GENC:3:1-2:ZMB','is_in:country_code','ZM'], // Republic of Zambia
     ['ZI020_GE4','ge:GENC:3:1-2:ZWE','is_in:country_code','ZW'], // Republic of Zimbabwe

     // ZI024_HYP - Water Resource Information : Hydrologic Persistence
     // ['ZI024_HYP','-999999',undefined,undefined], // No Information
     ['ZI024_HYP','1','intermittent','no'], // Perennial
     ['ZI024_HYP','2','intermittent','yes'],
     ['ZI024_HYP','4','intermittent','dry'], // Dry

     // ZI024_SCC - Water Resource Information : Water Type
     // ['ZI024_SCC','-999999',undefined,undefined], // No Information
     ['ZI024_SCC','1','water:type','alkaline'], // Alkaline
     ['ZI024_SCC','4','water:type','mineral'], // Mineral
     ['ZI024_SCC','10','water:type','salt'], // Saline
     ['ZI024_SCC','11','water:type','fresh'], // Fresh
     ['ZI024_SCC','12','water:type','brackish'], // Brackish
     ['ZI024_SCC','13','water:type','seawater'], // Seawater
     ['ZI024_SCC','14','water:type','brine'], // Brine
     // ['ZI024_SCC','998','water:type','not_applicable'], // Not Applicable
     ['ZI024_SCC','998',undefined,undefined], // Not Applicable

     // ZI024_YWQ - Water Resource Information : Water Potability
     // ['ZI024_YWQ','-999999',undefined,undefined], // No Information
     ['ZI024_YWQ','1','water:potable','yes'], // Potable
     ['ZI024_YWQ','2','water:potable','treatable'], // Treatable
     ['ZI024_YWQ','3','water:potable','contaminated'], // Contaminated
     ['ZI024_YWQ','4','water:potable','no'], // Nonpotable
     ['ZI024_YWQ','999','water:potable','other'], // Other

     // See MAN
     // ZI025_MAN - Hydrographic Vertical Positioning Information : Maritime Navigation Marked
     // ['ZI025_MAN','-999999',undefined,undefined], // No Information
//      ['ZI025_MAN','1000','raw:ZI025_MAN','no'],
//      ['ZI025_MAN','1001','raw:ZI025_MAN','yes'],

     // See WLE
     // ZI025_WLE - Hydrographic Vertical Positioning Information : Water Level Effect
     // ['ZI025_WLE','-999999',undefined,undefined], // No Information
//      ['ZI025_WLE','1','raw:ZI025_WLE','partly_submerged'], // Partly Submerged
//      ['ZI025_WLE','2','raw:ZI025_WLE','always_dry'], // Always Dry
//      ['ZI025_WLE','3','raw:ZI025_WLE','always_submerged'], // Always Submerged
//      ['ZI025_WLE','4','raw:ZI025_WLE','covers_and_uncovers'], // Covers and Uncovers
//      ['ZI025_WLE','5','raw:ZI025_WLE','awash_at_low_water'], // Awash at Low Water
//      ['ZI025_WLE','9','raw:ZI025_WLE','awash_at_chart_datum'], // Awash at Chart Datum
//      ['ZI025_WLE','999','raw:ZI025_WLE','other'], // Other

     // ZI026_CTUC - Feature Metadata : Cartographic Usability Range <interval closure>
     ['ZI026_CTUC','2','cartographic_scale:closure','open_interval'], // Open Interval
     ['ZI026_CTUC','3','cartographic_scale:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['ZI026_CTUC','4','cartographic_scale:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['ZI026_CTUC','5','cartographic_scale:closure','closed_interval'], // Closed Interval
     ['ZI026_CTUC','6','cartographic_scale:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['ZI026_CTUC','7','cartographic_scale:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['ZI026_CTUC','8','cartographic_scale:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['ZI026_CTUC','9','cartographic_scale:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // ZI026_SUR - Feature Metadata : Survey Coverage Category
     // ['ZI026_SUR','-999999',undefined,undefined], // No Information
     ['ZI026_SUR','1','source:surveyed','yes'], // Surveyed
     ['ZI026_SUR','2','source:surveyed','inadequately_surveyed'], // Inadequately Surveyed
     ['ZI026_SUR','3','source:surveyed','no'], // Unsurveyed

     // ZI032_GUG - Pylon Information : Guyed - See GUG
     // ['ZI032_GUG','-999999',undefined,undefined], // No Information
//      ['ZI032_GUG','1000','raw:ZI032_GUG','no'],
//      ['ZI032_GUG','1001','raw:ZI032_GUG','yes'],

     // ZI032_PYC - Pylon Information : Pylon Configuration - See PYC
     // ['ZI032_PYC','-999999',undefined,undefined], // No Information
//      ['ZI032_PYC','1','raw:ZI032_PYC',''a''], // 'A'
//      ['ZI032_PYC','2','raw:ZI032_PYC',''h''], // 'H'
//      ['ZI032_PYC','3','raw:ZI032_PYC',''i''], // 'I'
//      ['ZI032_PYC','4','raw:ZI032_PYC',''y''], // 'Y'
//      ['ZI032_PYC','5','raw:ZI032_PYC',''t''], // 'T'
//      ['ZI032_PYC','999','raw:ZI032_PYC','other'], // Other

     // ZI032_PYM - Pylon Information : Pylon Material - See PYM
     // ['ZI032_PYM','-999999',undefined,undefined], // No Information
//      ['ZI032_PYM','1','raw:ZI032_PYM','aluminum'], // Aluminum
//      ['ZI032_PYM','3','raw:ZI032_PYM','concrete'], // Concrete
//      ['ZI032_PYM','4','raw:ZI032_PYM','masonry'], // Masonry
//      ['ZI032_PYM','5','raw:ZI032_PYM','metal'], // Metal
//      ['ZI032_PYM','7','raw:ZI032_PYM','wood'], // Wood
//      ['ZI032_PYM','8','raw:ZI032_PYM','steel'], // Steel
//      ['ZI032_PYM','9','raw:ZI032_PYM','fibreglass'], // Fibreglass
//      ['ZI032_PYM','10','raw:ZI032_PYM','iron'], // Iron
//      ['ZI032_PYM','999','raw:ZI032_PYM','other'], // Other

     // ZI032_TOS - Pylon Information : Tower Shape = See TOS
     // ['ZI032_TOS','-999999',undefined,undefined], // No Information
//      ['ZI032_TOS','6','raw:ZI032_TOS','mast'], // Mast
//      ['ZI032_TOS','8','raw:ZI032_TOS','pole'], // Pole
//      ['ZI032_TOS','11','raw:ZI032_TOS','tripod'], // Tripod
//      ['ZI032_TOS','12','raw:ZI032_TOS','truss'], // Truss
//      ['ZI032_TOS','13','raw:ZI032_TOS','tubular'], // Tubular
//      ['ZI032_TOS','999','raw:ZI032_TOS','other'], // Other

     // ZI037_REL - Religious Information : Religious Designation
     // ['ZI037_REL','-999999',undefined,undefined], // No Information
     ['ZI037_REL','1','religion','buddhist'], // Buddhism
     ['ZI037_REL','2','religion','muslim'], // Islam
     ['ZI037_REL','3','denomination','roman_catholic'], // Roman Catholic
     ['ZI037_REL','4','religion','christian'], // Christian
     ['ZI037_REL','5','religion','jewish'], // Judaism
     ['ZI037_REL','6','denomination','orthodox'], // Orthodox
     ['ZI037_REL','7','denomination','protestant'], // Protestant
     ['ZI037_REL','8','religion','shinto'], // Shinto
     ['ZI037_REL','9','religion','hindu'], // Hinduism
     ['ZI037_REL','10','denomination','shia'], // Shia
     ['ZI037_REL','11','denomination','sunni'], // Sunni
     ['ZI037_REL','12','denomination','nestorian'], // Nestorian
     ['ZI037_REL','13','denomination','chaldean_catholic'], // Chaldean
     ['ZI037_REL','14','religion','mixed_or_none'], // Mixed and/or No Designation
     ['ZI037_REL','999','religion','other'], // Other

     // ZI037_RFA - Religious Information : Religious Facility Type
     ['ZI037_RFA','-999999','building:religious','unknown'], // No Information
     ['ZI037_RFA','1','building','cathedral'], // Cathedral
     ['ZI037_RFA','2','building','chapel'], // Chapel
     ['ZI037_RFA','3','building','church'], // Church
     ['ZI037_RFA','4','building','marabout'], // Marabout
     ['ZI037_RFA','5','tower:type','minaret'], // Minaret // Fixed in pre/post processing
     ['ZI037_RFA','6','building','mission'], // Mission
     ['ZI037_RFA','7','building','mosque'], // Mosque
     ['ZI037_RFA','8','building','pagoda'], // Pagoda
     ['ZI037_RFA','9','building','religious_community'], // Religious Community
     ['ZI037_RFA','10','building','seminary'], // Seminary
     ['ZI037_RFA','11','building','shrine'], // Shrine
     ['ZI037_RFA','12','building','stupa'], // Stupa
     ['ZI037_RFA','13','building','synagogue'], // Synagogue
     ['ZI037_RFA','14','building','tabernacle'], // Tabernacle
     ['ZI037_RFA','15','building','temple'], // Temple
     ['ZI037_RFA','18','building','convent'], // Convent
     ['ZI037_RFA','19','building','monastery'], // Monastery
     ['ZI037_RFA','20','building','noviciate'], // Noviciate
     ['ZI037_RFA','21','building','hermitage'], // Hermitage
     ['ZI037_RFA','22','building','retreat'], // Retreat
     ['ZI037_RFA','999','building:religious','other'], // Other

     // ZI071_CTX - Cave Information : Cave Type
     // ['ZI071_CTX','-999999',undefined,undefined], // No Information
     ['ZI071_CTX','1','cave:type','solution'], // Solution
     ['ZI071_CTX','2','cave:type','erosional'], // Erosional
     ['ZI071_CTX','3','cave:type','aeolian'], // Aeolian
     ['ZI071_CTX','4','cave:type','waterbody'], // Waterbody
     ['ZI071_CTX','5','cave:type','glacier'], // Glacier
     ['ZI071_CTX','6','cave:type','lava'], // Lava
     ['ZI071_CTX','7','cave:type','talus'], // Talus
     ['ZI071_CTX','999','cave:type','other'], // Other

     // ZI071_FFN - Cave Information : Feature Function - See FFN
     // ['ZI071_FFN','-999999',undefined,undefined], // No Information
//      ['ZI071_FFN','530','raw:ZI071_FFN','warehousing_and_storage'], // Warehousing and Storage
//      ['ZI071_FFN','563','raw:ZI071_FFN','residence'], // Residence
//      ['ZI071_FFN','835','raw:ZI071_FFN','defence_activities'], // Defence Activities
//      ['ZI071_FFN','930','raw:ZI071_FFN','religious_activities'], // Religious Activities
//      ['ZI071_FFN','999','raw:ZI071_FFN','other'], // Other

     // ZI071_UAO - Cave Information : Underground Access Orientation
     // ['ZI071_UAO','-999999',undefined,undefined], // No Information
     ['ZI071_UAO','1','direction','horizontal'], // Horizontal
     ['ZI071_UAO','2','direction','slopes_downward'], // Slopes Downward
     ['ZI071_UAO','3','direction','vertical_down'], // Vertical Down
     ['ZI071_UAO','4','direction','slopes_upward'], // Slopes Upward
     ['ZI071_UAO','5','direction','vertical_up'], // Vertical Up

     // ZSAX_RS0 - Restriction Information : Security Attributes Group <resource classification>
     ['ZSAX_RS0','U','security:classification','UNCLASSIFIED'],
     ['ZSAX_RS0','R','security:classification','RESTRICTED'],
     ['ZSAX_RS0','C','security:classification','CONFIDENTIAL'],
     ['ZSAX_RS0','S','security:classification','SECRET'],
     ['ZSAX_RS0','TS','security:classification','TOP_SECRET'],

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
//XX     [undefined,undefined,'amenity','school'], //  converted in pre processing
//      [undefined,undefined,'amenity','hospital'], //  converted in pre processing
//      [undefined,undefined,'amenity','university'], //  converted in pre processing
//      [undefined,undefined,'amenity','college'], //  converted in pre processing
     [undefined,undefined,'amenity','house_of_worship'], //  converted in pre processing

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
     ['ZI014_PPO','83','product','oil'], // Petroleum

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
    // ##### End of intList#####


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

} // End of tds61.rules
