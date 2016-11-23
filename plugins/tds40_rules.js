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
    TDS One2one rules
*/

tds.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    // This is where anything "unique" to TDS goes.
    fcodeOne2oneIn : [
    ['F_CODE','ZB035','man_made','survey_point'], // Control Point - Superseded by ZB050
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
     ['F_CODE','AL105','tourism','caravan_site'], // From OSM data, not a building so...
     ['F_CODE','AL130','historic','memorial'], // From OSM data, not great
     ['F_CODE','AL200','historic','ruins'], // Ruins
     ['F_CODE','AL241','tower','communication'], 
     ['F_CODE','AL241','tower','yes'], // OSM
     ['F_CODE','AL241','man_made','mast'], // OSM
     ['F_CODE','AM080','tower:type','water'],
     ['F_CODE','AN060','railway','yes'], // Rail yards
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
     ['F_CODE','ZB035','man_made','survey_point'], // Control Point - Superseded by ZB050
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
     'BRN':'bridge:reference', // Bridge Reference Number
     'CID':'source:cell_identifier', // Cell Identifier
     'DQS':'source:data_quality', // Data Quality Statement
     'GB052_RIDH':'aeroway:runway_designator_high', // Runway Direction (high end) : Runway Designator (high)
     'GB052_RIDL':'aeroway:runway_designator_low', // Runway Direction (low end) : Runway Designator (low)
     'IKO':'icao', // ICAO Location Indicator
     'LINK_ID':'etds:link_id', // eLTDS40 Field
     'MDE':'source:update:datetime', // Maintenance Date and Time
     'NA8':'official_name', // Aerodrome Official Name
     'OTH':'note:oth', // Specified Domain Value(s)
     'RTN2':'ref:2', // Route Designation [2]
     'RTN3':'ref:3', // Route Designation [3]
     'RTN':'ref', // Route Designation
     'SSE':'seasonal_ice_limit', // Seasonal Ice Limit
     'UFI':'uuid', // Unique Feature Identifier. Handled in pre-processing
     'URI':'source_ref', // Unique Resource Identifier
     'VOI':'vertical_obstruction_identifier', // Vertical Obstruction Identifier
     'WAD':'well:description', // Water Well Construction Description
     'WPI':'world_port_index', // World Port Index Identifier - This is part of Built Up Area. What??
     'ZI001_NSD':'source:non_spatial_source:datetime', // Source Information : Non-spatial Source Date and Time
     'ZI001_NSN':'source:non_spatial_source:description', // Source Information : Non-spatial Source Description
     'ZI001_SSD':'source:datetime', // Source Information : Spatial Source Date and Time
     //'ZI001_SSN':'source:description', // Source Information : Spatial Source Description
     'ZI001_SSN':'source', // Source Information : Spatial Source Description
     'ZI001_VSD':'source:vertical_source:datetime', // Source Information : Vertical Source Date and Time
     'ZI001_VSN':'source:vertical_source:description', // Source Information : Vertical Source Description
     'ZI002_CCN':'source:copyright', // Restriction Information : Commercial Copyright Notice
     'ZI002_CDR':'source:commercial_distribution_restriction', // Restriction Information : Commercial Distribution Restriction
     'ZI004_PRE':'source:processing_information', // Process Step Information : Process Step Description
     'ZI005_FNA':'name', // Geographic Name Information : Full Name
     'ZI005_FNA2':'alt_name', // Geographic Name Information : (second) Full Name
     'ZI005_NFN':'gndb_id', // Geographic Name Information : Name Identifier
     'ZI005_NFN2':'gndb_id:2', // Geographic Name Information : (second) Name Identifier
     'ZI006_MEM':'note', // Note : Memorandum
     'ZSAX_RX3':'security:dissemination_control:non_ic', // Restriction Information : Security Attributes Group (resource non-intelligence community markings)
     'ZSAX_RX4':'security:resource_owner', //Restriction Information : Security Attributes Group (resource owner-producer)
     }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
     'AHA':'source:accuracy:horizontal', // Absolute Horizontal Accuracy (90%)
     'AOO':'angle', // Angle of Orientation
     'AQTL':'aquifer:thickness:lower', // Aquifer Thickness (lower value)
     'AQTU':'aquifer:thickness:upper', // Aquifer Thickness (upper value)
     'ARA':'feature_area', // Area - this has been moved from 'area' due to hoot conflicts
     'AVA':'source:accuracy:vertical', // Absolute Vertical Accuracy (90%)
     'AYRL':'aquifer:yield:lower', // Aquifer Yield Rating (lower value)
     'AYRU':'aquifer:yield:upper', // Aquifer Yield Rating (upper value)
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
     'CTL':'railway:track:length', // Cumulative Track Length
     'DEV':'level', // Deck Level
     'DIM':'diameter', // Diameter
     'DMBL':'undergrowth:density:lower', // Undergrowth Density (lower value)
     'DMBU':'undergrowth:density:upper', // Undergrowth Density (upper value)
     'DMT':'canopy_cover', // Canopy Cover
     'DOF':'flow_direction', // Direction of Flow
     'DPAL':'aquifer:depth:lower', // Aquifer Depth (lower value)
     'DPAU':'aquifer:depth:upper', // Aquifer Depth (upper value)
     'DZC':'deck_count', // Deck Count
     'DZP':'depth:maximum_below_surface', // Deepest Depth Below Surface Level
     'EVA':'source:accuracy:elevation', // Elevation Vertical Accuracy (90%)
     'ETZ':'source:extraction_specification:version', // Extraction Specification Version
     'FCL':'ferry:crossing_distance', // Ferry Crossing Distance
//     'FCSUBTYPE':'etds:fcsubtype', // eLTDS40 Field. Very ESRI Specific. Ignored for now
     'GSGCHL':'aeroway:runway_highend:slope:high_lowervalue', // Runway Direction (high end) : Surface Slope (high) (lower value)
     'GSGCHU':'aeroway:runway_highend:slope:high_uppervalue', // Runway Direction (high end) : Surface Slope (high) (upper value)
     'GSGCLL':'aeroway:runway_highend:slope:low_lowervalue', // Runway Direction (low end) : Surface Slope (low) (lower value)
     'GSGCLU':'aeroway:runway_highend:slope:low_uppervalue', // Runway Direction (low end) : Surface Slope (low) (upper value)
     'HCA':'bridge:horizontal_clearance', // Horizontal Clearance
     'HEI':'height:object', // Height of Object
     'HGT':'height', // Height Above Surface Level
     'HVA':'source:accuracy:height', // Height Vertical Accuracy (90%)
     'KVA':'voltage', // Power Line Maximum Voltage
     'LC1':'bridge:load_class1', // Load Class Type 1
     'LC2':'bridge:load_class2', // Load Class Type 2
     'LC3':'bridge:load_class3', // Load Class Type 3
     'LC4':'bridge:load_class4', // Load Class Type 4
     'LCA':'crane:max_load', // Lifting Capacity
     'LEA':'depth:minimum_below_surface', // Least Depth Below Surface Level
     'LNU':'length:interior_useable', // Usable Length
     'LOG':'gradient:length', // Gradient Length
     'LTN':'lanes', // Track or Lane Count
     'LZN':'length', // Length
     'MWG':'divider:width', // Centerline Spacing
     'NOS':'bridge:span_count', // Span Count
     'NPL':'cables', // Parallel Line Count: See AT005
     'OHC':'max_clearance', // Overhead Clearance
     'PFD':'depth:predominant', // Predominant Feature Depth
     'PFH':'height:predominant', // Predominant Feature Height
     'PVH':'raw:PVH', // Predominant Vegetation Height
     'PWA':'waterway:predominant_depth', // Predominant Water Depth
     'RAD':'curve_radius', // Curve Radius
     'RMWL':'median:min_width', // Route Median Width (lower value)
     'RMWU':'median:max_width', // Route Median Width (upper value)
     'SCAMAX':'cartographic_scale:upper', // eLTDS40 Field
     'SCAMIN':'cartographic_scale:lower', // eLTDS40 Field
     'SDCL':'soil:depth:lower', // Soil Depth (lower value)
     'SDCU':'soil:depth:upper', // Soil Depth (upper value)
     'SDO':'dune:orientation', // Sand Dune Orientation
     'SDSL':'tree:diameter:lower', // Stem Diameter (lower value)
     'SDSU':'tree:diameter:upper', // Stem Diameter (upper value)
     'SGCL':'incline:min', // Surface Slope (lower value)
     'SGCU':'incline:max', // Surface Slope (upper value)
     'SHC':'safe_clearance:horizontal', // Safe Horizontal Clearance
     'SWW':'water_level', // Static Water Level
     'THI':'thickness', // Thickness
     'TSCL':'tree:spacing:lower', // Tree Spacing (lower value)
     'TSCU':'tree:spacing:upper', // Tree Spacing (upper value)
     'UBC':'bridge:under_clearance', // Underbridge Clearance
     'VLM':'volume', // Volume
     'VST':'vehicle_scale_count', // Vehicle Scale Count
     'WD2':'width:total_usable', // Route Total Usable Width
     'WD3':'width:gap', // Terrain Gap Width
     'WD5':'width:top', // Width at Top
     'WDH':'waterway:predominant_depth:max', // Predominant Maximum Water Depth
     'WDL':'waterway:predominant_depth:min', // Predominant Minimum Water Depth
     'WDU':'width:interior_useable', // Usable Width
     'WID':'width', // Width
     'WT2':'width:second_way', // Width of Second Travelled Way
     'WVA':'water:velocity:average', // Predominant Average Water Velocity
     'WVH':'water:velocity:maximum', // Predominant Maximum Water Velocity
     'WVL':'water:velocity:minimum', // Predominant Minimum Water Velocity
     'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
     'ZI017_GAW':'gauge', // Track Information : Railway Gauge
     'ZVA':'ele', // Aerodrome Elevation
     'ZVH':'ele:max' // Highest Elevation
     }, // End numBiased
    

    // Common one2one rules. Used for both import and export
    // taken from TDS v40 and semi-auto generated
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
     ['AQP','999','aquifer:composition','other'], // Other

     // AQTC - Aquifer Thickness (interval closure)
     ['AQTC','2','aquifer:thickness:closure','open_interval'], // Open Interval
     ['AQTC','3','aquifer:thickness:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['AQTC','4','aquifer:thickness:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['AQTC','5','aquifer:thickness:closure','closed_interval'], // Closed Interval
     ['AQTC','6','aquifer:thickness:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['AQTC','7','aquifer:thickness:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['AQTC','8','aquifer:thickness:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['AQTC','9','aquifer:thickness:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     /*
     // ASU - Aerodrome Movement Area Surface Composition - See ZI019_ASU
     // ['ASU','-999999',undefined,undefined], // No Information
     ['ASU','1','ASU','asphalt'], // Asphalt
     ['ASU','2','ASU','bituminous_mix'], // Bituminous Mix
     ['ASU','3','ASU','brick'], // Brick
     ['ASU','4','ASU','clay'], // Clay
     ['ASU','5','ASU','concrete'], // Concrete
     ['ASU','6','ASU','coral'], // Coral
     ['ASU','7','ASU','earthen'], // Earthen
     ['ASU','8','ASU','gravel'], // Gravel
     ['ASU','9','ASU','ice'], // Ice
     ['ASU','10','ASU','landing_mat'], // Landing Mat
     ['ASU','11','ASU','laterite'], // Laterite
     ['ASU','12','ASU','macadam'], // Macadam
     ['ASU','13','ASU','membrane'], // Membrane
     ['ASU','14','ASU','non-bituminous_mix'], // Non-bituminous Mix
     ['ASU','15','ASU','pierced_steel_planking'], // Pierced Steel Planking
     ['ASU','16','ASU','sand'], // Sand
     ['ASU','17','ASU','snow'], // Snow
     ['ASU','18','ASU','stone'], // Stone
     ['ASU','19','ASU','water'], // Water
     ['ASU','20','ASU','wood'], // Wood
     ['ASU','21','ASU','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ASU','999','ASU','other'], // Other
     */

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

     // AT005_CAB - Cable : Cable Type -- See CAB
     // ['AT005_CAB','-999999',undefined,undefined], // No Information
     // ['AT005_CAB','2','cable:type','power_line'], // Power Line
     // ['AT005_CAB','3','cable:type','telephone'], // Telephone
     // ['AT005_CAB','4','cable:type','telegraph'], // Telegraph
     // ['AT005_CAB','6','cable:type','transmission_line'], // Transmission Line
     // ['AT005_CAB','7','cable:type','digital_communication_line'], // Digital Communication Line
     // ['AT005_CAB','8','cable:type','communication_line'], // Communication Line
     // ['AT005_CAB','9','cable:type','fibre-optic'], // Fibre-optic
     // ['AT005_CAB','10','cable:type','tethering'], // Tethering
     // ['AT005_CAB','11','cable:type','load_bearing'], // Load Bearing
     // ['AT005_CAB','12','cable:type','guide'], // Guide
     // ['AT005_CAB','13','cable:type','barrier'], // Barrier
     // ['AT005_CAB','14','cable:type','restraining'], // Restraining
     // ['AT005_CAB','15','cable:type','towing'], // Towing
     // ['AT005_CAB','19','cable:type','cableway'], // Cableway
     // ['AT005_CAB','999','cable:type','other'], // Other

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

     // BCC - Bypass condition
     // ['BCC','-999999',undefined,undefined],
     ['BCC','1','bypass','easy'],
     ['BCC','2','bypass','difficult'],
     ['BCC','3','bypass','impossible'],

     // BGT - Basin Gate Type
     // ['BGT','-999999',undefined,undefined], // No Information
     ['BGT','1','seamark:gate:category','caisson'], // Caisson
     ['BGT','2','seamark:gate:category','lock_gate'], // Lock Gate
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
     // ['BH141_SLTA','-999999',undefined,undefined],  // Null
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
     // ['BH141_SLTB','-999999',undefined,undefined],  // Null
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
     ['BOT','13',undefined,undefined], // In data but not in any spec!
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
     // ['CAB','2','power','line'], // Power Line
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
     ['CCG','1','raw:CCG','biological'], // Biological
     ['CCG','2','raw:CCG','chemical'], // Chemical
     ['CCG','3','raw:CCG','nuclear_and/or_radiological'], // Nuclear and/or Radiological
     ['CCG','4','raw:CCG','thermal'], // Thermal
     ['CCG','999','raw:CCG','other'], // Other

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

     // CET - Grading Type
     // ['CET','-999999',undefined,undefined], // No Information
     ['CET','1','grading:type','one_side'], // One Side
     ['CET','2','grading:type','two_sides'], // Two Sides
     ['CET','3','grading:type','many_sides'], // Many Sides
     ['CET','4','grading:type','no_sides'], // No Sides
     ['CET','999','grading:type','other'], // Other

     // CNS - Controlled
     // ['CNS','-999999',undefined,undefined], // No Information
     ['CNS','1000','controlled','no'],
     ['CNS','1001','controlled','yes'],

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
     ['CON','16','constriction:type','entrance_and/or_exit'], // Entrance and/or Exit
     ['CON','17','constriction:type','fence'], // Fence
     ['CON','18','constriction:type','fire_hydrant'], // Fire Hydrant
     ['CON','19','constriction:type','ford'], // Ford
     ['CON','20','constriction:type','avalanche_protector'], // Gallery
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
     ['CON','41','constriction:type','transportation_route_protection_structure'], // Traneportation Route Protection Structure
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

     // CPA - Survey Control Point Type
     // ['CPA','-999999',undefined,undefined], // No Information
     ['CPA','2','survey_point:control','horizontal_position'], // Horizontal Position
     ['CPA','3','survey_point:control','horizontal_position_and_benchmark'], // Horizontal Position and Benchmark
     ['CPA','5','survey_point:control','vertical_position'], // Vertical Position

     // CPS - Cell Partition Scheme
     // ['CPS','-999999',undefined,undefined], // No Information
     ['CPS','fixed0r25','source:cell_partition_scheme','0r25_arc_degree'], // Fixed Cells, 0.25 Arc Degree
     ['CPS','fixed0r5','source:cell_partition_scheme','0r5_arc_degree'], // Fixed Cells, 0.5 Arc Degree
     ['CPS','fixed1r0','source:cell_partition_scheme','1_arc_degree'], // Fixed Cells, 1 Arc Degree
     ['CPS','fixed5r0','source:cell_partition_scheme','5_arc_degree'], // Fixed Cells, 5 Arc Degree
     ['CPS','variable','source:cell_partition_scheme','variable'], // Variable Cells
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

     // CUL - Cultural Context Type
     // ['CUL','-999999',undefined,undefined], // No Information
     ['CUL','1','raw:CUL','national_identity'], // National Identity
     ['CUL','2','raw:CUL','ethnic_group'], // Ethnic Group
     ['CUL','3','raw:CUL','tribe'], // Tribe
     ['CUL','4','raw:CUL','clan'], // Clan
     ['CUL','5','raw:CUL','language'], // Language
     ['CUL','6','raw:CUL','religion'], // Religion
     ['CUL','999','raw:CUL','other'], // Other

     // CWT - Contained Within Tunnel
     // ['CWT','-999999',undefined,undefined],
     ['CWT','1000','in_tunnel','no'],
     ['CWT','1001','in_tunnel','yes'], // Need to think about this

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

     // DPAC - Aquifer Depth (interval closure)
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
     // ['EQC','-999999',undefined,undefined], // No Information
     ['EQC','scale1m','source:equivalent_scale','1:1m'], // 1:1,000,000
     ['EQC','scale100k','source:equivalent_scale','1:100k'], // 1:100,000
     ['EQC','scale12r5k','source:equivalent_scale','1:12.5k'], // 1:12,500
     ['EQC','scale2m','source:equivalent_scale','1:2m'], // 1:2,000,000
     ['EQC','scale25k','source:equivalent_scale','1:25k'], // 1:25,000
     ['EQC','scale250k','source:equivalent_scale','1:250K'], // 1:250,000
     ['EQC','scale5k','source:equivalent_scale','1:5k'], // 1:5,000
     ['EQC','scale5m','source:equivalent_scale','1:5m'], // 1:5,000,000
     ['EQC','scale50k','source:equivalent_scale','1:50k'], // 1:50,000
     ['EQC','scale500k','source:equivalent_scale','1:500k'], // 1:500,000
     ['EQC','other','source:equivalent_scale','other'], // Other

     // ESC - Elevation Surface Category
     // ['ESC','-999999',undefined,undefined], // No Information
     ['ESC','1','elevation:surface','land'], // Land
     ['ESC','2','elevation:surface','snow_field_and/or_ice-field'], // Snow Field and/or Ice-field
     ['ESC','3','elevation:surface','water'], // Water
     ['ESC','4','elevation:surface','vegetation'], // Vegetation
     ['ESC','5','elevation:surface','inland_water'], // Inland Water

     // ETS - Extraction Specification
     // ['ETS','-999999',undefined,undefined], // No Information
     ['ETS','tpc','source:extraction_specification','1aa-tpc'], // 1AA-TPC
     ['ETS','onc','source:extraction_specification','1ab-onc'], // 1AB-ONC
     ['ETS','jogAirGround','source:extraction_specification','1ae-jog-a/g'], // 1AE-JOG-A/G
     ['ETS','dted1','source:extraction_specification','1cd-dted-1'], // 1CD-DTED-1
     ['ETS','dfad1','source:extraction_specification','1ce-dfad-1'], // 1CE-DFAD-1
     ['ETS','dted2','source:extraction_specification','1cf-dted-2'], // 1CF-DTED-2
     ['ETS','dfad2','source:extraction_specification','1cg-dfad-2'], // 1CG-DFAD-2
     ['ETS','hac1','source:extraction_specification','2aa/001-hac-1'], // 2AA/001-HAC-1
     ['ETS','hac2','source:extraction_specification','2aa/002-hac-2'], // 2AA/002-HAC-2
     ['ETS','hac3','source:extraction_specification','2aa/003-hac-3'], // 2AA/003-HAC-3
     ['ETS','hac4','source:extraction_specification','2aa/004-hac-4'], // 2AA/004-HAC-4
     ['ETS','hac5','source:extraction_specification','2aa/005-hac-5'], // 2AA/005-HAC-5
     ['ETS','hac6','source:extraction_specification','2aa/006-hac-6'], // 2AA/006-HAC-6
     ['ETS','hac7','source:extraction_specification','2aa/007-hac-7'], // 2AA/007-HAC-7
     ['ETS','hac8','source:extraction_specification','2aa/008-hac-8'], // 2AA/008-HAC-8
     ['ETS','hac9','source:extraction_specification','2aa/009-hac-9'], // 2AA/009-HAC-9
     ['ETS','combat','source:extraction_specification','2ad-combat'], // 2AD-Combat
     ['ETS','tlm50','source:extraction_specification','3aa-tlm50'], // 3AA-TLM50
     ['ETS','tlm100','source:extraction_specification','3ag-tlm100'], // 3AG-TLM100
     ['ETS','vitd','source:extraction_specification','3ka-vitd'], // 3KA-VITD
     ['ETS','dtop','source:extraction_specification','3kc/001-dtop'], // 3KC/001-DTOP
     ['ETS','vmap2','source:extraction_specification','3kh-vmap-2'], // 3KH-VMap-2
     ['ETS','vmap0','source:extraction_specification','3kl-vmap-0'], // 3KL-VMap-0
     ['ETS','vmap1','source:extraction_specification','3km-vmap-1'], // 3KM-VMap-1
     ['ETS','uvmap','source:extraction_specification','3ku-uvmap'], // 3KU-UVMap
     ['ETS','atc','source:extraction_specification','4aa-atc'], // 4AA-ATC
     ['ETS','jogRadar','source:extraction_specification','4ac-jog-r'], // 4AC-JOG-R
     ['ETS','tercomL','source:extraction_specification','4ge-tercom-l'], // 4GE-TERCOM-L
     ['ETS','tercomE','source:extraction_specification','4gf-tercom-e'], // 4GF-TERCOM-E
     ['ETS','tercomT','source:extraction_specification','4gg-tercom-t'], // 4GG-TERCOM-T
     ['ETS','ffd','source:extraction_specification','5ee-ffd'], // 5EE-FFD
     ['ETS','digitalFeg','source:extraction_specification','dfeg'], // DFEG
     ['ETS','dnc','source:extraction_specification','dnc'], // DNC
     ['ETS','globalTdsEg','source:extraction_specification','gtds-eg'], // GTDS-EG
     ['ETS','localTdsEg','source:extraction_specification','ltds-eg'], // LTDS-EG
     ['ETS','mgcpTrd','source:extraction_specification','mgcp-trd'], // MGCP TRD
     ['ETS','msd1','source:extraction_specification','msd1'], // MSD1
     ['ETS','msd2','source:extraction_specification','msd2'], // MSD2
     ['ETS','msd3','source:extraction_specification','msd3'], // MSD3
     ['ETS','msd4','source:extraction_specification','msd4'], // MSD4
     ['ETS','msd5','source:extraction_specification','msd5'], // MSD5
     ['ETS','regionalTdsEg','source:extraction_specification','rtds-eg'], // RTDS-EG
     ['ETS','specUrbanTdsEg','source:extraction_specification','s-utds-eg'], // S-UTDS-EG
     ['ETS','tod0','source:extraction_specification','tod0'], // TOD0
     ['ETS','tod1','source:extraction_specification','tod1'], // TOD1
     ['ETS','tod2','source:extraction_specification','tod2'], // TOD2
     ['ETS','tod3','source:extraction_specification','tod3'], // TOD3
     ['ETS','tod4','source:extraction_specification','tod4'], // TOD4
     ['ETS','other','source:extraction_specification','other'], // Other

     // ETY - Engine Test Cell Type -- Wow....
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
     // ['FFN','0','building','yes'], // Good default?
     ['FFN','2','use','agriculture'], // Agriculture
     ['FFN','3','use','growing_crops'], // Growing of Crops
     ['FFN','9','use','raising_animals'], // Raising of Animals
     ['FFN','14','use','grazing'], // Grazing
     ['FFN','15','use','mixed_farming'], // Mixed Farming
     ['FFN','16','camp:type','migrant_labour'], // Migrant Labour
     ['FFN','20','camp:type','forestry'], // Forestry and/or Logging
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
     ['FFN','272','shop','propane'], // Propane Sale
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
     ['FFN','470','shop','fuel'], // Petrol Sale -- links to AQ170 Motor Vehicle Station
     ['FFN','473','amenity','sales_yard'], // Sales Yard
     ['FFN','474','shop','precious_metal'], // Precious Metal Merchant
     ['FFN','475','amenity','marketplace'], // Market
     ['FFN','476','shop','grocery'], // Grocery
     ['FFN','477','amenity','pharmacy'], // Pharmacy
     ['FFN','478','shop','pet-shop'], // Pet-shop
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
     ['FFN','554','tourism','guest-house'], // Guest-house
     ['FFN','555','tourism','hostel'], // Hostel
     ['FFN','556','use','dormitory'], // Dormitory
     ['FFN','557','use','vacation_cottage'], // Vacation Cottage
     ['FFN','558','use','dependents_housing'], // Dependents Housing
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
     ['FFN','633','use','public_records'], // Public Records
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
     ['FFN','890','amenity','arts_centre'], // Cultural, Arts and Entertainment
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
     ['FFN','962','shop','beauty_treatment'], // Beauty Treatment
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

     // This attribute is handled in Pre and Post processing. See ZI024_HYP
     // HYP - Hydrologic Persistence
/*
     ['HYP','-999999',undefined,undefined], // No Information
     ['HYP','1','HYP','perennial'], // Perennial
     ['HYP','2','HYP','intermittent'], // Intermittent
*/

     // HZD - Geodetic Datum
     // ['HZD','-999999',undefined,undefined], // No Information
     ['HZD','adindanBurkinaFaso','source:datum:geodetic','adindan_(burkina_faso)'], // Adindan_(Burkina_Faso)
     ['HZD','adindanCameroon','source:datum:geodetic','adindan_(cameroon)'], // Adindan_(Cameroon)
     ['HZD','adindanEthiopia','source:datum:geodetic','adindan_(ethiopia)'], // Adindan_(Ethiopia)
     ['HZD','adindanMali','source:datum:geodetic','adindan_(mali)'], // Adindan_(Mali)
     ['HZD','adindanMeanValue','source:datum:geodetic','adindan_(mean_value)'], // Adindan_(mean_value)
     ['HZD','adindanSenegal','source:datum:geodetic','adindan_(senegal)'], // Adindan_(Senegal)
     ['HZD','adindanSudan','source:datum:geodetic','adindan_(sudan)'], // Adindan_(Sudan)
     ['HZD','afgooyeSomalia','source:datum:geodetic','afgooye_(somalia)'], // Afgooye_(Somalia)
     ['HZD','ainelAbd1970BahrainIsland','source:datum:geodetic','ain_el_abd_1970_(bahrain_island)'], // Ain_el_Abd_1970_(Bahrain_Island)
     ['HZD','ainelAbd1970SaudiArabia','source:datum:geodetic','ain_el_abd_1970_(saudi_arabia)'], // Ain_el_Abd_1970_(Saudi_Arabia)
     ['HZD','americanSamoaDatum1962','source:datum:geodetic','american_samoa_datum_1962'], // American_Samoa_Datum_1962
     ['HZD','amersfoort1885dash1903','source:datum:geodetic','amersfoort_1885/1903_(netherlands)'], // Amersfoort_1885/1903_(Netherlands)
     ['HZD','anna1Astro1965CocosIslands','source:datum:geodetic','anna_1_astro_1965_(cocos_islands)'], // Anna_1_Astro_1965_(Cocos_Islands)
     ['HZD','antiguaIslandAstro1943','source:datum:geodetic','antigua_island_astro_1943'], // Antigua_Island_Astro_1943
     ['HZD','approximateLuzonDatum','source:datum:geodetic','approximate_luzon_datum_(philippines)'], // Approximate_Luzon_Datum_(Philippines)
     ['HZD','arc1935Africa','source:datum:geodetic','arc_1935_(africa)'], // Arc_1935_(Africa)
     ['HZD','arc1950Botswana','source:datum:geodetic','arc_1950_(botswana)'], // Arc_1950_(Botswana)
     ['HZD','arc1950Burundi','source:datum:geodetic','arc_1950_(burundi)'], // Arc_1950_(Burundi)
     ['HZD','arc1950Lesotho','source:datum:geodetic','arc_1950_(lesotho)'], // Arc_1950_(Lesotho)
     ['HZD','arc1950Malawi','source:datum:geodetic','arc_1950_(malawi)'], // Arc_1950_(Malawi)
     ['HZD','arc1950MeanValue','source:datum:geodetic','arc_1950_(mean_value)'], // Arc_1950_(mean_value)
     ['HZD','arc1950Swaziland','source:datum:geodetic','arc_1950_(swaziland)'], // Arc_1950_(Swaziland)
     ['HZD','arc1950Zaire','source:datum:geodetic','arc_1950_(zaire)'], // Arc_1950_(Zaire)
     ['HZD','arc1950Zambia','source:datum:geodetic','arc_1950_(zambia)'], // Arc_1950_(Zambia)
     ['HZD','arc1950Zimbabwe','source:datum:geodetic','arc_1950_(zimbabwe)'], // Arc_1950_(Zimbabwe)
     ['HZD','arc1960Kenya','source:datum:geodetic','arc_1960_(kenya)'], // Arc_1960_(Kenya)
     ['HZD','arc1960MeanValue','source:datum:geodetic','arc_1960_(mean_value)'], // Arc_1960_(mean_value)
     ['HZD','arc1960Tanzania','source:datum:geodetic','arc_1960_(tanzania)'], // Arc_1960_(Tanzania)
     ['HZD','ascensionIsland1958','source:datum:geodetic','ascension_island_1958_(ascension_island)'], // Ascension_Island_1958_(Ascension_Island)
     ['HZD','astroBeaconEIwoJimaIsland','source:datum:geodetic','astro_beacon_e_(iwo_jima_island)'], // Astro_Beacon_'E'_(Iwo_Jima_Island)
     ['HZD','astroDos71dash4StHelena','source:datum:geodetic','astro_dos_71/4_(st._helena_island)'], // Astro_DOS_71/4_(St._Helena_Island)
     ['HZD','astroStation1952MarcusIs','source:datum:geodetic','astro_station_1952_(marcus_island)'], // Astro_Station_1952_(Marcus_Island)
     ['HZD','astroTernIsland1961Hawaii','source:datum:geodetic','astro_tern_island_1961_(tern_island_hawaii)'], // Astro_Tern_Island_1961_(Tern_Island_Hawaii)
     ['HZD','australianGeodetic1966','source:datum:geodetic','australian_geod._1966_(australia_and_tasmania_island)'], // Australian_Geod._1966_(Australia_and_Tasmania_Island)
     ['HZD','australianGeodetic1984','source:datum:geodetic','australian_geod._1984_(australia_and_tasmania_island)'], // Australian_Geod._1984_(Australia_and_Tasmania_Island)
     ['HZD','averageTerrestrialSys1977','source:datum:geodetic','average_terrestrial_system_1977_new_brunswick'], // Average_Terrestrial_System_1977_New_Brunswick
     ['HZD','ayabelleLighthouseDjibouti','source:datum:geodetic','ayabelle_lighthouse_(djibouti)'], // Ayabelle_Lighthouse_(Djibouti)
     ['HZD','bekaaBaseSouthEndLebanon','source:datum:geodetic','bekaa_base_south_end_(lebanon)'], // Bekaa_Base_South_End_(Lebanon)
     ['HZD','belgium1950SystemLommelSig','source:datum:geodetic','belgium_1950_system_(lommel_signal_belgium)'], // Belgium_1950_System_(Lommel_Signal_Belgium)
     ['HZD','belgium1972Observatoire','source:datum:geodetic','belgium_1972_(observatoire_duccle)'], // Belgium_1972_(Observatoire_d'Uccle)
     ['HZD','bellevueIgnEfateErromango','source:datum:geodetic','bellevue_(ign)_(efate_and_erromango_islands)'], // Bellevue_(IGN)_(Efate_and_Erromango_Islands)
     ['HZD','bermuda1957BermudaIslands','source:datum:geodetic','bermuda_1957_(bermuda_islands)'], // Bermuda_1957_(Bermuda_Islands)
     ['HZD','bern1898Switzerland','source:datum:geodetic','bern_1898_(switzerland)'], // Bern_1898_(Switzerland)
     ['HZD','bern1898ZeroMeridian','source:datum:geodetic','bern_1898_(switzerland)_with_zero_meridian_bern'], // Bern_1898_(Switzerland)_with_Zero_Meridian_Bern
     ['HZD','bissauGuineaBissau','source:datum:geodetic','bissau_(guinea-bissau)'], // Bissau_(Guinea-Bissau)
     ['HZD','bjz54A954BeijingCoord','source:datum:geodetic','bjz54_(a954_beijing_coordinates)_(china)'], // BJZ54_(A954_Beijing_Coordinates)_(China)
     ['HZD','bogotaObservatoryColombia','source:datum:geodetic','bogota_observatory_(colombia)'], // Bogota_Observatory_(Colombia)
     ['HZD','bogotaObsZeroMeridian','source:datum:geodetic','bogota_observatory_(colombia)_with_zero_meridian_bogota'], // Bogota_Observatory_(Colombia)_with_Zero_Meridian_Bogota
     ['HZD','bukitRimpahIndonesia','source:datum:geodetic','bukit_rimpah_(bangka_and_belitung_islands_indonesia)'], // Bukit_Rimpah_(Bangka_and_Belitung_Islands_Indonesia)
     ['HZD','camacupaBaseSwEndAngola','source:datum:geodetic','camacupa_base_sw_end_(campo_de_aviacao_angola)'], // Camacupa_Base_SW_End_(Campo_De_Aviacao_Angola)
     ['HZD','campAreaAstroAntarctica','source:datum:geodetic','camp_area_astro_(camp_mcmurdo_area_antarctica)'], // Camp_Area_Astro_(Camp_McMurdo_Area_Antarctica)
     ['HZD','campoInchauspeArgentina','source:datum:geodetic','campo_inchauspe_(argentina)'], // Campo_Inchauspe_(Argentina)
     ['HZD','cantonAstro1966PhoenixIs','source:datum:geodetic','canton_astro_1966_(phoenix_islands)'], // Canton_Astro_1966_(Phoenix_Islands)
     ['HZD','capeSouthAfrica','source:datum:geodetic','cape_(south_africa)'], // Cape_(South_Africa)
     ['HZD','capeCanaveralMeanValue','source:datum:geodetic','cape_canaveral_(mean_value)'], // Cape_Canaveral_(mean_value)
     ['HZD','carthageTunisia','source:datum:geodetic','carthage_(tunisia)'], // Carthage_(Tunisia)
     ['HZD','chatham1971NewZealand','source:datum:geodetic','chatham_1971_(chatham_island_new_zealand)'], // Chatham_1971_(Chatham_Island_New_Zealand)
     ['HZD','chuaAstroParaguay','source:datum:geodetic','chua_astro_(paraguay)'], // Chua_Astro_(Paraguay)
     ['HZD','compensationGeoQuebec1977','source:datum:geodetic','compensation_geodetique_du_quebec_1977'], // Compensation_Geodetique_du_Quebec_1977
     ['HZD','conakryPyramidGuinea','source:datum:geodetic','conakry_pyramid_of_the_service_geographique_(guinea)'], // Conakry_Pyramid_of_the_Service_Geographique_(Guinea)
     ['HZD','estonia1937','source:datum:geodetic','co-ordinate_system_1937_of_estonia'], // Co-ordinate_System_1937_of_Estonia
     ['HZD','corregoAlegreBrazil','source:datum:geodetic','corrego_alegre_(brazil)'], // Corrego_Alegre_(Brazil)
     ['HZD','dabolaGuinea','source:datum:geodetic','dabola_(guinea)'], // Dabola_(Guinea)
     ['HZD','dcs3LighthouseLesserAnt','source:datum:geodetic','dcs-3_lighthouse_saint_lucia_lesser_antilles'], // DCS-3_Lighthouse_Saint_Lucia_Lesser_Antilles
     ['HZD','deceptionIslAntarctica','source:datum:geodetic','deception_island_antarctica'], // Deception_Island_Antarctica
     ['HZD','djakartaBataviaIndonesia','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)'], // Djakarta_(Batavia)_(Sumatra_Island_Indonesia)
     ['HZD','djakartaBataviaZeroMerid','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)_with_zero_meridian_djakarta'], // Djakarta_(Batavia)_(Sumatra_Island_Indonesia)_with_Zero_Meridian_Djakarta
     ['HZD','dominicaAstroM12LesserAnt','source:datum:geodetic','dominica_astro_m-12_dominica_lesser_antilles'], // Dominica_Astro_M-12_Dominica_Lesser_Antilles
     ['HZD','dos1968GizoNewGeorgiaIs','source:datum:geodetic','dos_1968_(gizo_island_new_georgia_islands)'], // DOS_1968_(Gizo_Island_New_Georgia_Islands)
     ['HZD','easterIsland1967EasterIs','source:datum:geodetic','easter_island_1967_(easter_island)'], // Easter_Island_1967_(Easter_Island)
     ['HZD','european1950BritishIsles','source:datum:geodetic','european_1950_(british_isles)'], // European_1950_(British_Isles)
     ['HZD','european1950Cyprus','source:datum:geodetic','european_1950_(cyprus)'], // European_1950_(Cyprus)
     ['HZD','european1950Egypt','source:datum:geodetic','european_1950_(egypt)'], // European_1950_(Egypt)
     ['HZD','european1950England','source:datum:geodetic','european_1950_(england_channel_islands_scotland_and_shetland_islands)'], // European_1950_(England_Channel_Islands_Scotland_and_Shetland_Islands)
     ['HZD','european1950Greece','source:datum:geodetic','european_1950_(greece)'], // European_1950_(Greece)
     ['HZD','european1950Iran','source:datum:geodetic','european_1950_(iran)'], // European_1950_(Iran)
     ['HZD','european1950IraqSyria','source:datum:geodetic','european_1950_(iraq_israel_jordan_kuwait_lebanon_saudi_arabia_and_syria)'], // European_1950_(Iraq_Israel_Jordan_Kuwait_Lebanon_Saudi_Arabia_and_Syria)
     ['HZD','european1950Malta','source:datum:geodetic','european_1950_(malta)'], // European_1950_(Malta)
     ['HZD','european1950MeanValue','source:datum:geodetic','european_1950_(mean_value)'], // European_1950_(mean_value)
     ['HZD','european1950NorwayFinland','source:datum:geodetic','european_1950_(norway_and_finland)'], // European_1950_(Norway_and_Finland)
     ['HZD','european1950PortugalSpain','source:datum:geodetic','european_1950_(portugal_and_spain)'], // European_1950_(Portugal_and_Spain)
     ['HZD','european1950Sardinia','source:datum:geodetic','european_1950_(sardinia)'], // European_1950_(Sardinia)
     ['HZD','european1950Sicily','source:datum:geodetic','european_1950_(sicily)'], // European_1950_(Sicily)
     ['HZD','european1950Tunisia','source:datum:geodetic','european_1950_(tunisia)'], // European_1950_(Tunisia)
     ['HZD','european1950WesternEurope','source:datum:geodetic','european_1950_(western_europe)'], // European_1950_(Western_Europe)
     ['HZD','european1979MeanValue','source:datum:geodetic','european_1979_(mean_value)'], // European_1979_(mean_value)
     ['HZD','etrs1989','source:datum:geodetic','european_terrestrial_reference_system_1989_(etrs89)'], // European_Terrestrial_Reference_System_1989_(ETRS89)
     ['HZD','fortThomas1955LeewardIs','source:datum:geodetic','fort_thomas_1955_(nevis_st_kitts_leeward_islands)'], // Fort_Thomas_1955_(Nevis_St_Kitts_Leeward_Islands)
     ['HZD','gan1970AdduAtoll','source:datum:geodetic','gan_1970_(addu_atoll_republic_of_maldives)'], // Gan_1970_(Addu_Atoll_Republic_of_Maldives)
     ['HZD','gandajikaBaseZaire','source:datum:geodetic','gandajika_base_(zaire)'], // Gandajika_Base_(Zaire)
     ['HZD','gdz80China','source:datum:geodetic','gdz80_(china)'], // GDZ80_(China)
     ['HZD','geocentricDatumOfAustralia','source:datum:geodetic','geocentric_datum_of_australia_(gda)'], // Geocentric_Datum_of_Australia_(GDA)
     ['HZD','geodeticDatum1949Zealand','source:datum:geodetic','geodetic_datum_1949_(new_zealand)'], // Geodetic_Datum_1949_(New_Zealand)
     ['HZD','graciosaBaseSWFaialAzores','source:datum:geodetic','graciosa_base_sw_(faial_graciosa_pico_sao_jorge_and_terceira_island_azores)'], // Graciosa_Base_SW_(Faial_Graciosa_Pico_Sao_Jorge_and_Terceira_Island_Azores)
     ['HZD','greekDatumGreece','source:datum:geodetic','greek_datum_greece'], // Greek_Datum_Greece
     ['HZD','greekGeodeticRefSystem1987','source:datum:geodetic','greek_geodetic_reference_system_1987_(ggrs_87)'], // Greek_Geodetic_Reference_System_1987_(GGRS_87)
     ['HZD','guam1963','source:datum:geodetic','guam_1963'], // Guam_1963
     ['HZD','gunongSegaraKalimantanIs','source:datum:geodetic','gunong_segara_(kalimantan_island_indonesia)'], // Gunong_Segara_(Kalimantan_Island_Indonesia)
     ['HZD','gunongSerindung','source:datum:geodetic','gunong_serindung'], // Gunong_Serindung
     ['HZD','gux1AstroGuadacanalIs','source:datum:geodetic','gux_1_astro_(guadacanal_island)'], // GUX_1_Astro_(Guadacanal_Island)
     ['HZD','guyanaCSG67','source:datum:geodetic','guyana_csg67'], // Guyana_CSG67
     ['HZD','heratNorthAfganistan','source:datum:geodetic','herat_north_(afganistan)'], // Herat_North_(Afganistan)
     ['HZD','hermannskogel','source:datum:geodetic','hermannskogel'], // Hermannskogel
     ['HZD','hjorsey1955Iceland','source:datum:geodetic','hjorsey_1955_(iceland)'], // Hjorsey_1955_(Iceland)
     ['HZD','hongKong1929','source:datum:geodetic','hong_kong_1929'], // Hong_Kong_1929
     ['HZD','hongKong1963HongKong','source:datum:geodetic','hong_kong_1963_(hong_kong)'], // Hong_Kong_1963_(Hong_Kong)
     ['HZD','hungarian1972','source:datum:geodetic','hungarian_1972'], // Hungarian_1972
     ['HZD','huTzuShan','source:datum:geodetic','hu-tzu-shan'], // Hu-Tzu-Shan
     ['HZD','indianBangladesh','source:datum:geodetic','indian_(bangladesh)'], // Indian_(Bangladesh)
     ['HZD','indianIndiaNepal','source:datum:geodetic','indian_(india_and_nepal)'], // Indian_(India_and_Nepal)
     ['HZD','indianPakistan','source:datum:geodetic','indian_(pakistan)'], // Indian_(Pakistan)
     ['HZD','indianThailandVietnam','source:datum:geodetic','indian_(thailand_and_vietnam)'], // Indian_(Thailand_and_Vietnam)
     ['HZD','indian1954Thailand','source:datum:geodetic','indian_1954_(thailand)'], // Indian_1954_(Thailand)
     ['HZD','indian1960ConSonIsland','source:datum:geodetic','indian_1960_(con_son_island_(vietnam))'], // Indian_1960_(Con_Son_Island_(Vietnam))
     ['HZD','indian1960Vietnam','source:datum:geodetic','indian_1960_(vietnam:_near_16_degrees_north)'], // Indian_1960_(Vietnam:_near_16_degrees_North)
     ['HZD','indian1975Thailand','source:datum:geodetic','indian_1975_(thailand)'], // Indian_1975_(Thailand)
     ['HZD','indian1975ThailandCycle1','source:datum:geodetic','indian_1975_(thailand)_-_cycle_1'], // Indian_1975_(Thailand)_-_Cycle_1
     ['HZD','indonesian1974','source:datum:geodetic','indonesian_1974'], // Indonesian_1974
     ['HZD','ireland1965IrelandNorthern','source:datum:geodetic','ireland_1965_(ireland_and_northern_ireland)'], // Ireland_1965_(Ireland_and_Northern_Ireland)
     ['HZD','ists061Astro1968GeorgiaIs','source:datum:geodetic','ists_061_astro_1968_(south_georgia_islands)'], // ISTS_061_Astro_1968_(South_Georgia_Islands)
     ['HZD','ists073Astro1969DiegoGar','source:datum:geodetic','ists_073_astro_1969_(diego_garcia)'], // ISTS_073_Astro_1969_(Diego_Garcia)
     ['HZD','johnstonIsland1961','source:datum:geodetic','johnston_island_1961_(johnston_island)'], // Johnston_Island_1961_(Johnston_Island)
     ['HZD','kalianpurIndia','source:datum:geodetic','kalianpur_(india)'], // Kalianpur_(India)
     ['HZD','kandawalaSriLanka','source:datum:geodetic','kandawala_(sri_lanka)'], // Kandawala_(Sri_Lanka)
     ['HZD','kcs2SierraLeone','source:datum:geodetic','kcs_2_sierra_leone'], // KCS_2_Sierra_Leone
     ['HZD','kerguelenIsland1949','source:datum:geodetic','kerguelen_island_1949_(kerguelen_island)'], // Kerguelen_Island_1949_(Kerguelen_Island)
     ['HZD','kertau1948RevisedMalaysia','source:datum:geodetic','kertau_1948_(or_revised_kertau)_(west_malaysia_and_singapore)'], // Kertau_1948_(or_Revised_Kertau)_(West_Malaysia_and_Singapore)
     ['HZD','kkjFinland','source:datum:geodetic','kkj_(or_kartastokoordinaattijarjestelma)_finland'], // KKJ_(or_Kartastokoordinaattijarjestelma)_Finland
     ['HZD','koreanGeodeticSystem1995','source:datum:geodetic','korean_geodetic_system_1995_(south_korea)'], // Korean_Geodetic_System_1995_(South_Korea)
     ['HZD','kusaieAstro1951','source:datum:geodetic','kusaie_astro_1951'], // Kusaie_Astro_1951
     ['HZD','kuwaitOilCompanyK28','source:datum:geodetic','kuwait_oil_company_(k28)'], // Kuwait_Oil_Company_(K28)
     ['HZD','lc5Astro1961CaymanBracIs','source:datum:geodetic','l.c._5_astro_1961_(cayman_brac_island)'], // L.C._5_Astro_1961_(Cayman_Brac_Island)
     ['HZD','leigonGhana','source:datum:geodetic','leigon_(ghana)'], // Leigon_(Ghana)
     ['HZD','liberia1964Liberia','source:datum:geodetic','liberia_1964_(liberia)'], // Liberia_1964_(Liberia)
     ['HZD','lisbonCastelodiSaoJorge','source:datum:geodetic','lisbon_(castelo_di_sao_jorge)_portugal'], // Lisbon_(Castelo_di_Sao_Jorge)_Portugal
     ['HZD','localAstro','source:datum:geodetic','local_astro'], // Local_Astro
     ['HZD','lomaQuintanaVenezuela','source:datum:geodetic','loma_quintana_(venezuela)'], // Loma_Quintana_(Venezuela)
     ['HZD','luzonMindanaoIsland','source:datum:geodetic','luzon_(mindanao_island)'], // Luzon_(Mindanao_Island)
     ['HZD','luzonPhilipinesNotMindanao','source:datum:geodetic','luzon_(philipines_except_mindanao_island)'], // Luzon_(Philipines_except_Mindanao_Island)
     ['HZD','mahe1971MaheIsland','source:datum:geodetic','mahe_1971_(mahe_island)'], // Mahe_1971_(Mahe_Island)
     ['HZD','manokwariWestIrian','source:datum:geodetic','manokwari_(west_irian)'], // Manokwari_(West_Irian)
     ['HZD','marcoAstroSalvageIslands','source:datum:geodetic','marco_astro_(salvage_islands)'], // Marco_Astro_(Salvage_Islands)
     ['HZD','martiniqueFortDesaix','source:datum:geodetic','martinique_fort-desaix'], // Martinique_Fort-Desaix
     ['HZD','massawEritreaEthiopia','source:datum:geodetic','massawa_(eritrea_ethiopia)'], // Massawa_(Eritrea_Ethiopia)
     ['HZD','mayotteCombani','source:datum:geodetic','mayotte_combani'], // Mayotte_Combani
     ['HZD','merchichMorocco','source:datum:geodetic','merchich_(morocco)'], // Merchich_(Morocco)
     ['HZD','midwayAstro1961MidwayIs','source:datum:geodetic','midway_astro_1961_(midway_island)'], // Midway_Astro_1961_(Midway_Island)
     ['HZD','minnaCameroon','source:datum:geodetic','minna_(cameroon)'], // Minna_(Cameroon)
     ['HZD','minnaNigeria','source:datum:geodetic','minna_(nigeria)'], // Minna_(Nigeria)
     ['HZD','modifiedBJZ54China','source:datum:geodetic','modified_bjz54_(china)'], // Modified_BJZ54_(China)
     ['HZD','montjongLowe','source:datum:geodetic','montjong_lowe'], // Montjong_Lowe
     ['HZD','montserratIslandAstro1958','source:datum:geodetic','montserrat_island_astro_1958'], // Montserrat_Island_Astro_1958
     ['HZD','mountDillonTobago','source:datum:geodetic','mount_dillon_tobago'], // Mount_Dillon_Tobago
     ['HZD','mPoralokoGabon','source:datum:geodetic','mporaloko_(gabon)'], // M'Poraloko_(Gabon)
     ['HZD','nahrwanMasirahIslandOman','source:datum:geodetic','nahrwan_(masirah_island_oman)'], // Nahrwan_(Masirah_Island_Oman)
     ['HZD','nahrwanSaudiArabia','source:datum:geodetic','nahrwan_(saudi_arabia)'], // Nahrwan_(Saudi_Arabia)
     ['HZD','nahrwanUnitedArabEmirates','source:datum:geodetic','nahrwan_(united_arab_emirates)'], // Nahrwan_(United_Arab_Emirates)
     ['HZD','naparimaBwiTrinidadTobago','source:datum:geodetic','naparima_(bwi_trinidad_and_tobago)'], // Naparima_(BWI_Trinidad_and_Tobago)
     ['HZD','newFrenchZeroMeridianParis','source:datum:geodetic','new_french_or_nouvelle_triangulation_francaise_(ntf)_with_zero_meridian_paris'], // New_French_or_Nouvelle_Triangulation_Francaise_(NTF)_with_Zero_Meridian_Paris
     ['HZD','northAm1927Alaska','source:datum:geodetic','north_american_1927_(alaska)'], // North_American_1927_(Alaska)
     ['HZD','northAm1927Alberta','source:datum:geodetic','north_american_1927_(alberta_and_british_columbia)'], // North_American_1927_(Alberta_and_British_Columbia)
     ['HZD','northAm1927AleutianE','source:datum:geodetic','north_american_1927_(aleutian_islands_east_of_180_degrees_west)'], // North_American_1927_(Aleutian_Islands_East_of_180_degrees_West)
     ['HZD','northAm1927AleutianW','source:datum:geodetic','north_american_1927_(aleutian_islands_west_of_180_degrees_west)'], // North_American_1927_(Aleutian_Islands_West_of_180_degrees_West)
     ['HZD','northAm1927Bahamas','source:datum:geodetic','north_american_1927_(bahamas_except_san_salvador_island)'], // North_American_1927_(Bahamas_except_San_Salvador_Island)
     ['HZD','northAm1927CanadMean','source:datum:geodetic','north_american_1927_(canada_mean)'], // North_American_1927_(Canada_mean)
     ['HZD','northAm1927CanalZone','source:datum:geodetic','north_american_1927_(canal_zone)'], // North_American_1927_(Canal_Zone)
     ['HZD','northAm1927Caribbean','source:datum:geodetic','north_american_1927_(caribbean)'], // North_American_1927_(Caribbean)
     ['HZD','northAm1927CentAmer','source:datum:geodetic','north_american_1927_(central_america)'], // North_American_1927_(Central_America)
     ['HZD','northAm1927ConusMean','source:datum:geodetic','north_american_1927_(conus_mean)'], // North_American_1927_(CONUS_mean)
     ['HZD','northAm1927Cuba','source:datum:geodetic','north_american_1927_(cuba)'], // North_American_1927_(Cuba)
     ['HZD','northAm1927EasternUs','source:datum:geodetic','north_american_1927_(eastern_us)'], // North_American_1927_(Eastern_US)
     ['HZD','northAm1927HayesPen','source:datum:geodetic','north_american_1927_(hayes_peninsula_greenland)'], // North_American_1927_(Hayes_Peninsula_Greenland)
     ['HZD','northAm1927Manitoba','source:datum:geodetic','north_american_1927_(manitoba_and_ontario)'], // North_American_1927_(Manitoba_and_Ontario)
     ['HZD','northAm1927Mexico','source:datum:geodetic','north_american_1927_(mexico)'], // North_American_1927_(Mexico)
     ['HZD','northAm1927Newfound','source:datum:geodetic','north_american_1927_(newfoundland_new_brunswick_nova_scotia_and_quebec)'], // North_American_1927_(Newfoundland_New_Brunswick_Nova_Scotia_and_Quebec)
     ['HZD','northAm1927NwTerSaskat','source:datum:geodetic','north_american_1927_(northwest_territories_and_saskatchewan)'], // North_American_1927_(Northwest_Territories_and_Saskatchewan)
     ['HZD','northAm1927Salvador','source:datum:geodetic','north_american_1927_(san_salvador_island)'], // North_American_1927_(San_Salvador_Island)
     ['HZD','northAm1927WesternUs','source:datum:geodetic','north_american_1927_(western_us)'], // North_American_1927_(Western_US)
     ['HZD','northAm1927Yukon','source:datum:geodetic','north_american_1927_(yukon)'], // North_American_1927_(Yukon)
     ['HZD','northAm1983AlaskaExAleut','source:datum:geodetic','north_american_1983_(alaska_excluding_aleutian_islands)'], // North_American_1983_(Alaska_excluding_Aleutian_Islands)
     ['HZD','northAm1983Aleutian','source:datum:geodetic','north_american_1983_(aleutian_islands)'], // North_American_1983_(Aleutian_Islands)
     ['HZD','northAm1983Canada','source:datum:geodetic','north_american_1983_(canada)'], // North_American_1983_(Canada)
     ['HZD','northAm1983Conus','source:datum:geodetic','north_american_1983_(conus)'], // North_American_1983_(CONUS)
     ['HZD','northAm1983Hawaii','source:datum:geodetic','north_american_1983_(hawaii)'], // North_American_1983_(Hawaii)
     ['HZD','northAm1983Mexico','source:datum:geodetic','north_american_1983_(mexico_and_central_america))'], // North_American_1983_(Mexico_and_Central_America))
     ['HZD','northSahara1959','source:datum:geodetic','north_sahara_1959'], // North_Sahara_1959
     ['HZD','observMeteorologico1939','source:datum:geodetic','observatorio_meteorologico_1939_(corvo_and_flores_islands_azores)'], // Observatorio_Meteorologico_1939_(Corvo_and_Flores_Islands_Azores)
     ['HZD','ocotopequeGuatemala','source:datum:geodetic','ocotopeque_guatemala'], // Ocotopeque_Guatemala
     ['HZD','oldEgyptian','source:datum:geodetic','old_egyptian_(egypt)'], // Old_Egyptian_(Egypt)
     ['HZD','oldHawaiianHawaiiIsland','source:datum:geodetic','old_hawaiian_(hawaii)'], // Old_Hawaiian_(Hawaii)
     ['HZD','oldHawaiianKauaiIsland','source:datum:geodetic','old_hawaiian_(kauai)'], // Old_Hawaiian_(Kauai)
     ['HZD','oldHawaiianMauiIsland','source:datum:geodetic','old_hawaiian_(maui)'], // Old_Hawaiian_(Maui)
     ['HZD','oldHawaiianMeanValue','source:datum:geodetic','old_hawaiian_(mean_value)'], // Old_Hawaiian_(mean_value)
     ['HZD','oldHawaiianOahuIsland','source:datum:geodetic','old_hawaiian_(oahu)'], // Old_Hawaiian_(Oahu)
     ['HZD','oman','source:datum:geodetic','oman_(oman)'], // Oman_(Oman)
     ['HZD','ordnanceSurvGB1936England','source:datum:geodetic','ordnance_survey_g.b._1936_(england)'], // Ordnance_Survey_G.B._1936_(England)
     ['HZD','ordnanceSurvGB1936ScotWale','source:datum:geodetic','ordnance_survey_g.b._1936_(england_isle_of_man_and_wales)'], // Ordnance_Survey_G.B._1936_(England_Isle_of_Man_and_Wales)
     ['HZD','ordnanceSurvGB1936MeanVal','source:datum:geodetic','ordnance_survey_g.b._1936_(mean_value)'], // Ordnance_Survey_G.B._1936_(mean_value)
     ['HZD','ordnanceSurvGB1936ScotShet','source:datum:geodetic','ordnance_survey_g.b._1936_(scotland_and_shetland_islands)'], // Ordnance_Survey_G.B._1936_(Scotland_and_Shetland_Islands)
     ['HZD','ordnanceSurvGB1936Wales','source:datum:geodetic','ordnance_survey_g.b._1936_(wales)'], // Ordnance_Survey_G.B._1936_(Wales)
     ['HZD','osloObservatoryOld','source:datum:geodetic','oslo_observatory_(old)_norway'], // Oslo_Observatory_(Old)_Norway
     ['HZD','padangBaseWestEnd','source:datum:geodetic','padang_base_west_end_(sumatra_indonesia)'], // Padang_Base_West_End_(Sumatra_Indonesia)
     ['HZD','padangBaseWestEndZeroMerid','source:datum:geodetic','padang_base_west_end_(sumatra_indonesia)_with_zero_meridian_djakarta'], // Padang_Base_West_End_(Sumatra_Indonesia)_with_Zero_Meridian_Djakarta
     ['HZD','palestine1928','source:datum:geodetic','palestine_1928_(israel_jordan)'], // Palestine_1928_(Israel_Jordan)
     ['HZD','picoDeLasNievesCanaryIs','source:datum:geodetic','pico_de_las_nieves_(canary_islands)'], // Pico_de_las_Nieves_(Canary_Islands)
     ['HZD','pitcairnAstro1967','source:datum:geodetic','pitcairn_astro_1967_(pitcairn_island)'], // Pitcairn_Astro_1967_(Pitcairn_Island)
     ['HZD','point58MeanSolution','source:datum:geodetic','point_58_mean_solution_(burkina_faso_and_niger)'], // Point_58_Mean_Solution_(Burkina_Faso_and_Niger)
     ['HZD','pointeNoire1948','source:datum:geodetic','pointe_noire_1948'], // Pointe_Noire_1948
     ['HZD','potsdamHelmertturmGermany','source:datum:geodetic','potsdam_or_helmertturm_(germany)'], // Potsdam_or_Helmertturm_(Germany)
     ['HZD','provSouthAm1956Bolivia','source:datum:geodetic','prov._s._american_1956_(bolivia)'], // Prov._S._American_1956_(Bolivia)
     ['HZD','provSouthAm1956Columbia','source:datum:geodetic','prov._s._american_1956_(columbia)'], // Prov._S._American_1956_(Columbia)
     ['HZD','provSouthAm1956Ecuador','source:datum:geodetic','prov._s._american_1956_(ecuador)'], // Prov._S._American_1956_(Ecuador)
     ['HZD','provSouthAm1956Guyana','source:datum:geodetic','prov._s._american_1956_(guyana)'], // Prov._S._American_1956_(Guyana)
     ['HZD','provSouthAm1956MeanValue','source:datum:geodetic','prov._s._american_1956_(mean_value)'], // Prov._S._American_1956_(mean_value)
     ['HZD','provSouthAm1956NChile','source:datum:geodetic','prov._s._american_1956_(northern_chile_near_19_degrees_south)'], // Prov._S._American_1956_(Northern_Chile_near_19_degrees_South)
     ['HZD','provSouthAm1956Peru','source:datum:geodetic','prov._s._american_1956_(peru)'], // Prov._S._American_1956_(Peru)
     ['HZD','provSouthAm1956SChile','source:datum:geodetic','prov._s._american_1956_(southern_chile_near_43_degrees_south)'], // Prov._S._American_1956_(Southern_Chile_near_43_degrees_South)
     ['HZD','provSouthAm1956Venezuela','source:datum:geodetic','prov._s._american_1956_(venezuela)'], // Prov._S._American_1956_(Venezuela)
     ['HZD','provSouthChilean1963','source:datum:geodetic','provisional_south_chilean_1963_(or_hito_xviii_1963)_(s._chile_53_degrees_south)'], // Provisional_South_Chilean_1963_(or_Hito_XVIII_1963)_(S._Chile_53_degrees_South)
     ['HZD','puertoRicoVirginIslands','source:datum:geodetic','puerto_rico_(puerto_rico_and_virgin_islands)'], // Puerto_Rico_(Puerto_Rico_and_Virgin_Islands)
     ['HZD','pulkovo1942Albania','source:datum:geodetic','pulkovo_1942_(albania)'], // Pulkovo_1942_(Albania)
     ['HZD','pulkovo1942Czechoslovakia','source:datum:geodetic','pulkovo_1942_(czechoslovakia)'], // Pulkovo_1942_(Czechoslovakia)
     ['HZD','pulkovo1942Hungary','source:datum:geodetic','pulkovo_1942_(hungary)'], // Pulkovo_1942_(Hungary)
     ['HZD','pulkovo1942Kazakhstan','source:datum:geodetic','pulkovo_1942_(kazakhstan)'], // Pulkovo_1942_(Kazakhstan)
     ['HZD','pulkovo1942Latvia','source:datum:geodetic','pulkovo_1942_(latvia)'], // Pulkovo_1942_(Latvia)
     ['HZD','pulkovo1942Poland','source:datum:geodetic','pulkovo_1942_(poland)'], // Pulkovo_1942_(Poland)
     ['HZD','pulkovo1942Romania','source:datum:geodetic','pulkovo_1942_(romania)'], // Pulkovo_1942_(Romania)
     ['HZD','pulkovo1942Russia','source:datum:geodetic','pulkovo_1942_(russia)'], // Pulkovo_1942_(Russia)
     ['HZD','qatarNationalQatar','source:datum:geodetic','qatar_national_(qatar)'], // Qatar_National_(Qatar)
     ['HZD','qornoqSouthGreenland','source:datum:geodetic','qornoq_(south_greenland)'], // Qornoq_(South_Greenland)
     ['HZD','rauenbergBerlinGermany','source:datum:geodetic','rauenberg_(berlin_germany)'], // Rauenberg_(Berlin_Germany)
     ['HZD','reconTriangulationMorocco','source:datum:geodetic','reconnaissance_triangulation_morocco'], // Reconnaissance_Triangulation_Morocco
     ['HZD','reunion1947','source:datum:geodetic','reunion_1947'], // Reunion_1947
     ['HZD','revisedNahrwan','source:datum:geodetic','revised_nahrwan'], // Revised_Nahrwan
     ['HZD','rome1940','source:datum:geodetic','rome_1940_(or_monte_mario_1940)_italy'], // Rome_1940_(or_Monte_Mario_1940)_Italy
     ['HZD','rome1940ZeroMeridianRome','source:datum:geodetic','rome_1940_(or_monte_mario_1940)_italy_with_zero_meridian_rome'], // Rome_1940_(or_Monte_Mario_1940)_Italy_with_Zero_Meridian_Rome
     ['HZD','rt90StockholmSweden','source:datum:geodetic','rt90_stockholm_sweden'], // RT90_Stockholm_Sweden
     ['HZD','sainteAnneI1984Guadeloupe','source:datum:geodetic','sainte_anne_i_1984_(guadeloupe)'], // Sainte_Anne_I_1984_(Guadeloupe)
     ['HZD','santoDos1965EspiritoSanto','source:datum:geodetic','santo_(dos)_1965_(espirito_santo_island)'], // Santo_(DOS)_1965_(Espirito_Santo_Island)
     ['HZD','saoBrazSaoMiguelAzores','source:datum:geodetic','sao_braz_(sao_miguel_santa_maria_islands_azores)'], // Sao_Braz_(Sao_Miguel_Santa_Maria_Islands_Azores)
     ['HZD','sapperHill1943EastFalkland','source:datum:geodetic','sapper_hill_1943_(east_falkland_islands)'], // Sapper_Hill_1943_(East_Falkland_Islands)
     ['HZD','schwarzeckNamibia','source:datum:geodetic','schwarzeck_(namibia)'], // Schwarzeck_(Namibia)
     ['HZD','seBasePortoSanto','source:datum:geodetic','se_base_(porto_santo)_(porto_santo_and_madeira_islands)'], // SE_Base_(Porto_Santo)_(Porto_Santo_and_Madeira_Islands)
     ['HZD','selvagemGrande1938Salvage','source:datum:geodetic','selvagem_grande_1938_(salvage_islands)'], // Selvagem_Grande_1938_(Salvage_Islands)
     ['HZD','sierraLeone1960','source:datum:geodetic','sierra_leone_1960'], // Sierra_Leone_1960
     ['HZD','sJtsk','source:datum:geodetic','s-jtsk'], // S-JTSK
     ['HZD','southAfricanSouthAfrica','source:datum:geodetic','south_african_(south_africa)'], // South_African_(South_Africa)
     ['HZD','southAmerican1969Argentina','source:datum:geodetic','south_american_1969_(argentina)'], // South_American_1969_(Argentina)
     ['HZD','southAmerican1969BaltraIs','source:datum:geodetic','south_american_1969_(baltra_galapagos_islands)'], // South_American_1969_(Baltra_Galapagos_Islands)
     ['HZD','southAmerican1969Bolivia','source:datum:geodetic','south_american_1969_(bolivia)'], // South_American_1969_(Bolivia)
     ['HZD','southAmerican1969Brazil','source:datum:geodetic','south_american_1969_(brazil)'], // South_American_1969_(Brazil)
     ['HZD','southAmerican1969Chile','source:datum:geodetic','south_american_1969_(chile)'], // South_American_1969_(Chile)
     ['HZD','southAmerican1969Columbia','source:datum:geodetic','south_american_1969_(columbia)'], // South_American_1969_(Columbia)
     ['HZD','southAmerican1969Ecuador','source:datum:geodetic','south_american_1969_(ecuador)'], // South_American_1969_(Ecuador)
     ['HZD','southAmerican1969Guyana','source:datum:geodetic','south_american_1969_(guyana)'], // South_American_1969_(Guyana)
     ['HZD','southAmerican1969MeanValue','source:datum:geodetic','south_american_1969_(mean_value)'], // South_American_1969_(mean_value)
     ['HZD','southAmerican1969Paraguay','source:datum:geodetic','south_american_1969_(paraguay)'], // South_American_1969_(Paraguay)
     ['HZD','southAmerican1969Peru','source:datum:geodetic','south_american_1969_(peru)'], // South_American_1969_(Peru)
     ['HZD','southAmerican1969Trinidad','source:datum:geodetic','south_american_1969_(trinidad_and_tobago)'], // South_American_1969_(Trinidad_and_Tobago)
     ['HZD','southAmerican1969Venezuela','source:datum:geodetic','south_american_1969_(venezuela)'], // South_American_1969_(Venezuela)
     ['HZD','sirgas','source:datum:geodetic','south_american_geocentric_reference_system_(sirgas)'], // South_American_Geocentric_Reference_System_(SIRGAS)
     ['HZD','southAsiaSingapore','source:datum:geodetic','south_asia_(southeast_asia_singapore)'], // South_Asia_(Southeast_Asia_Singapore)
     ['HZD','sovietGeodeticSystem1985','source:datum:geodetic','soviet_geodetic_system_1985'], // Soviet_Geodetic_System_1985
     ['HZD','sovietGeodeticSystem1990','source:datum:geodetic','soviet_geodetic_system_1990'], // Soviet_Geodetic_System_1990
     ['HZD','stPierreetMiquelon1950','source:datum:geodetic','st._pierre_et_miquelon_1950'], // St._Pierre_et_Miquelon_1950
     ['HZD','stockholm1938Sweden','source:datum:geodetic','stockholm_1938_(sweden)'], // Stockholm_1938_(Sweden)
     ['HZD','sydneyObservatoryNewSouth','source:datum:geodetic','sydney_observatory_new_south_wales_australia'], // Sydney_Observatory_New_South_Wales_Australia
     ['HZD','tananariveObservatory1925','source:datum:geodetic','tananarive_observatory_1925'], // Tananarive_Observatory_1925
     ['HZD','tananariveObs1925ZerMerPar','source:datum:geodetic','tananarive_observatory_1925_with_zero_meridian_paris'], // Tananarive_Observatory_1925_with_Zero_Meridian_Paris
     ['HZD','timbalai1948BruneiMalaysia','source:datum:geodetic','timbalai_1948_(brunei_and_east_malaysia_-_sarawak_and_sabah)'], // Timbalai_1948_(Brunei_and_East_Malaysia_-_Sarawak_and_Sabah)
     ['HZD','timbalai1968','source:datum:geodetic','timbalai_1968'], // Timbalai_1968
     ['HZD','tokyoJapan','source:datum:geodetic','tokyo_(japan)'], // Tokyo_(Japan)
     ['HZD','tokyoKorea','source:datum:geodetic','tokyo_(korea)'], // Tokyo_(Korea)
     ['HZD','tokyoKoreaCycle1','source:datum:geodetic','tokyo_(korea)_-_cycle_1'], // Tokyo_(Korea)_-_Cycle_1
     ['HZD','tokyoMeanValue','source:datum:geodetic','tokyo_(mean_value)'], // Tokyo_(mean_value)
     ['HZD','tokyoOkinawa','source:datum:geodetic','tokyo_(okinawa)'], // Tokyo_(Okinawa)
     ['HZD','trinidad1903','source:datum:geodetic','trinidad_1903'], // Trinidad_1903
     ['HZD','tristanAstro1968Cunha','source:datum:geodetic','tristan_astro_1968_(tristan_da_cunha)'], // Tristan_Astro_1968_(Tristan_da_Cunha)
     ['HZD','vitiLevu1916FijiIslands','source:datum:geodetic','viti_levu_1916_(viti_levu_island_fiji_islands)'], // Viti_Levu_1916_(Viti_Levu_Island_Fiji_Islands)
     ['HZD','voirol1875','source:datum:geodetic','voirol_1875'], // Voirol_1875
     ['HZD','voirol1875ZeroMeridParis','source:datum:geodetic','voirol_1875_with_zero_meridian_paris'], // Voirol_1875_with_Zero_Meridian_Paris
     ['HZD','voirol1960Algeria','source:datum:geodetic','voirol_1960_algeria'], // Voirol_1960_Algeria
     ['HZD','voirol1960ZeroMeridParis','source:datum:geodetic','voirol_1960_algeria_with_zero_meridian_paris'], // Voirol_1960_Algeria_with_Zero_Meridian_Paris
     ['HZD','wakeIslandAstro1952','source:datum:geodetic','wake_island_astro_1952'], // Wake_Island_Astro_1952
     ['HZD','wakeEniwetok1960MarshallIs','source:datum:geodetic','wake-eniwetok_1960_(marshall_islands)'], // Wake-Eniwetok_1960_(Marshall_Islands)
     ['HZD','worldGeodeticSystem1960','source:datum:geodetic','world_geodetic_system_1960'], // World_Geodetic_System_1960
     ['HZD','worldGeodeticSystem1966','source:datum:geodetic','world_geodetic_system_1966'], // World_Geodetic_System_1966
     ['HZD','worldGeodeticSystem1972','source:datum:geodetic','world_geodetic_system_1972'], // World_Geodetic_System_1972
     ['HZD','worldGeodeticSystem1984','source:datum:geodetic','world_geodetic_system_1984'], // World_Geodetic_System_1984
     ['HZD','yacareUruguay','source:datum:geodetic','yacare_(uruguay)'], // Yacare_(Uruguay)
     ['HZD','zanderijSurinam','source:datum:geodetic','zanderij_(surinam)'], // Zanderij_(Surinam)
     ['HZD','other','source:datum:geodetic','other'], // Other

     // IC2 - ISO 3166-1 Country Code (alpha3)
     // ['IC2','-999999',undefined,undefined], // No Information
     ['IC2','AFG','is_in:country_code3','afg'], // AFGHANISTAN
     ['IC2','ALA','is_in:country_code3','ala'], // ALAND ISLANDS
     ['IC2','ALB','is_in:country_code3','alb'], // ALBANIA
     ['IC2','DZA','is_in:country_code3','dza'], // ALGERIA
     ['IC2','ASM','is_in:country_code3','asm'], // AMERICAN SAMOA
     ['IC2','AND','is_in:country_code3','and'], // ANDORRA
     ['IC2','AGO','is_in:country_code3','ago'], // ANGOLA
     ['IC2','AIA','is_in:country_code3','aia'], // ANGUILLA
     ['IC2','ATA','is_in:country_code3','ata'], // ANTARCTICA
     ['IC2','ATG','is_in:country_code3','atg'], // ANTIGUA AND BARBUDA
     ['IC2','ARG','is_in:country_code3','arg'], // ARGENTINA
     ['IC2','ARM','is_in:country_code3','arm'], // ARMENIA
     ['IC2','ABW','is_in:country_code3','abw'], // ARUBA
     ['IC2','AUS','is_in:country_code3','aus'], // AUSTRALIA
     ['IC2','AUT','is_in:country_code3','aut'], // AUSTRIA
     ['IC2','AZE','is_in:country_code3','aze'], // AZERBAIJAN
     ['IC2','BHS','is_in:country_code3','bhs'], // BAHAMAS
     ['IC2','BHR','is_in:country_code3','bhr'], // BAHRAIN
     ['IC2','BGD','is_in:country_code3','bgd'], // BANGLADESH
     ['IC2','BRB','is_in:country_code3','brb'], // BARBADOS
     ['IC2','BLR','is_in:country_code3','blr'], // BELARUS
     ['IC2','BEL','is_in:country_code3','bel'], // BELGIUM
     ['IC2','BLZ','is_in:country_code3','blz'], // BELIZE
     ['IC2','BEN','is_in:country_code3','ben'], // BENIN
     ['IC2','BMU','is_in:country_code3','bmu'], // BERMUDA
     ['IC2','BTN','is_in:country_code3','btn'], // BHUTAN
     ['IC2','BOL','is_in:country_code3','bol'], // BOLIVIA PLURINATIONAL STATE OF
     ['IC2','BES','is_in:country_code3','bes'], // BONAIRE SINT EUSTATIUS AND SABA
     ['IC2','BIH','is_in:country_code3','bih'], // BOSNIA AND HERZEGOVINA
     ['IC2','BWA','is_in:country_code3','bwa'], // BOTSWANA
     ['IC2','BVT','is_in:country_code3','bvt'], // BOUVET ISLAND
     ['IC2','BRA','is_in:country_code3','bra'], // BRAZIL
     ['IC2','IOT','is_in:country_code3','iot'], // BRITISH INDIAN OCEAN TERRITORY
     ['IC2','BRN','is_in:country_code3','brn'], // BRUNEI DARUSSALAM
     ['IC2','BGR','is_in:country_code3','bgr'], // BULGARIA
     ['IC2','BFA','is_in:country_code3','bfa'], // BURKINA FASO
     ['IC2','BDI','is_in:country_code3','bdi'], // BURUNDI
     ['IC2','KHM','is_in:country_code3','khm'], // CAMBODIA
     ['IC2','CMR','is_in:country_code3','cmr'], // CAMEROON
     ['IC2','CAN','is_in:country_code3','can'], // CANADA
     ['IC2','CPV','is_in:country_code3','cpv'], // CAPE VERDE
     ['IC2','CYM','is_in:country_code3','cym'], // CAYMAN ISLANDS
     ['IC2','CAF','is_in:country_code3','caf'], // CENTRAL AFRICAN REPUBLIC
     ['IC2','TCD','is_in:country_code3','tcd'], // CHAD
     ['IC2','CHL','is_in:country_code3','chl'], // CHILE
     ['IC2','CHN','is_in:country_code3','chn'], // CHINA
     ['IC2','CXR','is_in:country_code3','cxr'], // CHRISTMAS ISLAND
     ['IC2','CCK','is_in:country_code3','cck'], // COCOS (KEELING) ISLANDS
     ['IC2','COL','is_in:country_code3','col'], // COLOMBIA
     ['IC2','COM','is_in:country_code3','com'], // COMOROS
     ['IC2','COG','is_in:country_code3','cog'], // CONGO
     ['IC2','COD','is_in:country_code3','cod'], // CONGO DEMOCRATIC REPUBLIC OF THE
     ['IC2','COK','is_in:country_code3','cok'], // COOK ISLANDS
     ['IC2','CRI','is_in:country_code3','cri'], // COSTA RICA
     ['IC2','CIV','is_in:country_code3','civ'], // COTE DIVOIRE
     ['IC2','HRV','is_in:country_code3','hrv'], // CROATIA
     ['IC2','CUB','is_in:country_code3','cub'], // CUBA
     ['IC2','CUW','is_in:country_code3','cuw'], // CURACAO
     ['IC2','CYP','is_in:country_code3','cyp'], // CYPRUS
     ['IC2','CZE','is_in:country_code3','cze'], // CZECH REPUBLIC
     ['IC2','DNK','is_in:country_code3','dnk'], // DENMARK
     ['IC2','DJI','is_in:country_code3','dji'], // DJIBOUTI
     ['IC2','DMA','is_in:country_code3','dma'], // DOMINICA
     ['IC2','DOM','is_in:country_code3','dom'], // DOMINICAN REPUBLIC
     ['IC2','ECU','is_in:country_code3','ecu'], // ECUADOR
     ['IC2','EGY','is_in:country_code3','egy'], // EGYPT
     ['IC2','SLV','is_in:country_code3','slv'], // EL SALVADOR
     ['IC2','GNQ','is_in:country_code3','gnq'], // EQUATORIAL GUINEA
     ['IC2','ERI','is_in:country_code3','eri'], // ERITREA
     ['IC2','EST','is_in:country_code3','est'], // ESTONIA
     ['IC2','ETH','is_in:country_code3','eth'], // ETHIOPIA
     ['IC2','FLK','is_in:country_code3','flk'], // FALKLAND ISLANDS (MALVINAS)
     ['IC2','FRO','is_in:country_code3','fro'], // FAROE ISLANDS
     ['IC2','FJI','is_in:country_code3','fji'], // FIJI
     ['IC2','FIN','is_in:country_code3','fin'], // FINLAND
     ['IC2','FRA','is_in:country_code3','fra'], // FRANCE
     ['IC2','GUF','is_in:country_code3','guf'], // FRENCH GUIANA
     ['IC2','PYF','is_in:country_code3','pyf'], // FRENCH POLYNESIA
     ['IC2','ATF','is_in:country_code3','atf'], // FRENCH SOUTHERN TERRITORIES
     ['IC2','GAB','is_in:country_code3','gab'], // GABON
     ['IC2','GMB','is_in:country_code3','gmb'], // GAMBIA
     ['IC2','GEO','is_in:country_code3','geo'], // GEORGIA
     ['IC2','DEU','is_in:country_code3','deu'], // GERMANY
     ['IC2','GHA','is_in:country_code3','gha'], // GHANA
     ['IC2','GIB','is_in:country_code3','gib'], // GIBRALTAR
     ['IC2','GRC','is_in:country_code3','grc'], // GREECE
     ['IC2','GRL','is_in:country_code3','grl'], // GREENLAND
     ['IC2','GRD','is_in:country_code3','grd'], // GRENADA
     ['IC2','GLP','is_in:country_code3','glp'], // GUADELOUPE
     ['IC2','GUM','is_in:country_code3','gum'], // GUAM
     ['IC2','GTM','is_in:country_code3','gtm'], // GUATEMALA
     ['IC2','GGY','is_in:country_code3','ggy'], // GUERNSEY
     ['IC2','GIN','is_in:country_code3','gin'], // GUINEA
     ['IC2','GNB','is_in:country_code3','gnb'], // GUINEA-BISSAU
     ['IC2','GUY','is_in:country_code3','guy'], // GUYANA
     ['IC2','HTI','is_in:country_code3','hti'], // HAITI
     ['IC2','HMD','is_in:country_code3','hmd'], // HEARD ISLAND AND MCDONALD ISLANDS
     ['IC2','VAT','is_in:country_code3','vat'], // HOLY SEE (VATICAN CITY STATE)
     ['IC2','HND','is_in:country_code3','hnd'], // HONDURAS
     ['IC2','HKG','is_in:country_code3','hkg'], // HONG KONG
     ['IC2','HUN','is_in:country_code3','hun'], // HUNGARY
     ['IC2','ISL','is_in:country_code3','isl'], // ICELAND
     ['IC2','IND','is_in:country_code3','ind'], // INDIA
     ['IC2','IDN','is_in:country_code3','idn'], // INDONESIA
     ['IC2','IRN','is_in:country_code3','irn'], // IRAN ISLAMIC REPUBLIC OF
     ['IC2','IRQ','is_in:country_code3','irq'], // IRAQ
     ['IC2','IRL','is_in:country_code3','irl'], // IRELAND
     ['IC2','IMN','is_in:country_code3','imn'], // ISLE OF MAN
     ['IC2','ISR','is_in:country_code3','isr'], // ISRAEL
     ['IC2','ITA','is_in:country_code3','ita'], // ITALY
     ['IC2','JAM','is_in:country_code3','jam'], // JAMAICA
     ['IC2','JPN','is_in:country_code3','jpn'], // JAPAN
     ['IC2','JEY','is_in:country_code3','jey'], // JERSEY
     ['IC2','JOR','is_in:country_code3','jor'], // JORDAN
     ['IC2','KAZ','is_in:country_code3','kaz'], // KAZAKHSTAN
     ['IC2','KEN','is_in:country_code3','ken'], // KENYA
     ['IC2','KIR','is_in:country_code3','kir'], // KIRIBATI
     ['IC2','PRK','is_in:country_code3','prk'], // KOREA DEMOCRATIC PEOPLE'S REPUBLIC OF
     ['IC2','KOR','is_in:country_code3','kor'], // KOREA REPUBLIC OF
     ['IC2','KWT','is_in:country_code3','kwt'], // KUWAIT
     ['IC2','KGZ','is_in:country_code3','kgz'], // KYRGYZSTAN
     ['IC2','LAO','is_in:country_code3','lao'], // LAO PEOPLES DEMOCRATIC REPUBLIC
     ['IC2','LVA','is_in:country_code3','lva'], // LATVIA
     ['IC2','LBN','is_in:country_code3','lbn'], // LEBANON
     ['IC2','LSO','is_in:country_code3','lso'], // LESOTHO
     ['IC2','LBR','is_in:country_code3','lbr'], // LIBERIA
     ['IC2','LBY','is_in:country_code3','lby'], // LIBYA
     ['IC2','LIE','is_in:country_code3','lie'], // LIECHTENSTEIN
     ['IC2','LTU','is_in:country_code3','ltu'], // LITHUANIA
     ['IC2','LUX','is_in:country_code3','lux'], // LUXEMBOURG
     ['IC2','MAC','is_in:country_code3','mac'], // MACAO
     ['IC2','MKD','is_in:country_code3','mkd'], // MACEDONIA THE FORMER YUGOSLAV REPUBLIC OF
     ['IC2','MDG','is_in:country_code3','mdg'], // MADAGASCAR
     ['IC2','MWI','is_in:country_code3','mwi'], // MALAWI
     ['IC2','MYS','is_in:country_code3','mys'], // MALAYSIA
     ['IC2','MDV','is_in:country_code3','mdv'], // MALDIVES
     ['IC2','MLI','is_in:country_code3','mli'], // MALI
     ['IC2','MLT','is_in:country_code3','mlt'], // MALTA
     ['IC2','MHL','is_in:country_code3','mhl'], // MARSHALL ISLANDS
     ['IC2','MTQ','is_in:country_code3','mtq'], // MARTINIQUE
     ['IC2','MRT','is_in:country_code3','mrt'], // MAURITANIA
     ['IC2','MUS','is_in:country_code3','mus'], // MAURITIUS
     ['IC2','MYT','is_in:country_code3','myt'], // MAYOTTE
     ['IC2','MEX','is_in:country_code3','mex'], // MEXICO
     ['IC2','FSM','is_in:country_code3','fsm'], // MICRONESIA FEDERATED STATES OF
     ['IC2','MDA','is_in:country_code3','mda'], // MOLDOVA REPUBLIC OF
     ['IC2','MCO','is_in:country_code3','mco'], // MONACO
     ['IC2','MNG','is_in:country_code3','mng'], // MONGOLIA
     ['IC2','MNE','is_in:country_code3','mne'], // MONTENEGRO
     ['IC2','MSR','is_in:country_code3','msr'], // MONTSERRAT
     ['IC2','MAR','is_in:country_code3','mar'], // MOROCCO
     ['IC2','MOZ','is_in:country_code3','moz'], // MOZAMBIQUE
     ['IC2','MMR','is_in:country_code3','mmr'], // MYANMAR
     ['IC2','NAM','is_in:country_code3','nam'], // NAMIBIA
     ['IC2','NRU','is_in:country_code3','nru'], // NAURU
     ['IC2','NPL','is_in:country_code3','npl'], // NEPAL
     ['IC2','NLD','is_in:country_code3','nld'], // NETHERLANDS
     ['IC2','NCL','is_in:country_code3','ncl'], // NEW CALEDONIA
     ['IC2','NZL','is_in:country_code3','nzl'], // NEW ZEALAND
     ['IC2','NIC','is_in:country_code3','nic'], // NICARAGUA
     ['IC2','NER','is_in:country_code3','ner'], // NIGER
     ['IC2','NGA','is_in:country_code3','nga'], // NIGERIA
     ['IC2','NIU','is_in:country_code3','niu'], // NIUE
     ['IC2','NFK','is_in:country_code3','nfk'], // NORFOLK ISLAND
     ['IC2','MNP','is_in:country_code3','mnp'], // NORTHERN MARIANA ISLANDS
     ['IC2','NOR','is_in:country_code3','nor'], // NORWAY
     ['IC2','OMN','is_in:country_code3','omn'], // OMAN
     ['IC2','PAK','is_in:country_code3','pak'], // PAKISTAN
     ['IC2','PLW','is_in:country_code3','plw'], // PALAU
     ['IC2','PSE','is_in:country_code3','pse'], // PALESTINIAN TERRITORY OCCUPIED
     ['IC2','PAN','is_in:country_code3','pan'], // PANAMA
     ['IC2','PNG','is_in:country_code3','png'], // PAPUA NEW GUINEA
     ['IC2','PRY','is_in:country_code3','pry'], // PARAGUAY
     ['IC2','PER','is_in:country_code3','per'], // PERU
     ['IC2','PHL','is_in:country_code3','phl'], // PHILIPPINES
     ['IC2','PCN','is_in:country_code3','pcn'], // PITCAIRN
     ['IC2','POL','is_in:country_code3','pol'], // POLAND
     ['IC2','PRT','is_in:country_code3','prt'], // PORTUGAL
     ['IC2','PRI','is_in:country_code3','pri'], // PUERTO RICO
     ['IC2','QAT','is_in:country_code3','qat'], // QATAR
     ['IC2','REU','is_in:country_code3','reu'], // RÉUNION
     ['IC2','ROU','is_in:country_code3','rou'], // ROMANIA
     ['IC2','RUS','is_in:country_code3','rus'], // RUSSIAN FEDERATION
     ['IC2','RWA','is_in:country_code3','rwa'], // RWANDA
     ['IC2','BLM','is_in:country_code3','blm'], // SAINT BARTHELEMY
     ['IC2','SHN','is_in:country_code3','shn'], // SAINT HELENA ASCENSION AND TRISTAN DA CUNHA
     ['IC2','KNA','is_in:country_code3','kna'], // SAINT KITTS AND NEVIS
     ['IC2','LCA','is_in:country_code3','lca'], // SAINT LUCIA
     ['IC2','MAF','is_in:country_code3','maf'], // SAINT MARTIN (FRENCH PART)
     ['IC2','SPM','is_in:country_code3','spm'], // SAINT PIERRE AND MIQUELON
     ['IC2','VCT','is_in:country_code3','vct'], // SAINT VINCENT AND THE GRENADINES
     ['IC2','WSM','is_in:country_code3','wsm'], // SAMOA
     ['IC2','SMR','is_in:country_code3','smr'], // SAN MARINO
     ['IC2','STP','is_in:country_code3','stp'], // SAO TOME AND PRINCIPE
     ['IC2','SAU','is_in:country_code3','sau'], // SAUDI ARABIA
     ['IC2','SEN','is_in:country_code3','sen'], // SENEGAL
     ['IC2','SRB','is_in:country_code3','srb'], // SERBIA
     ['IC2','SYC','is_in:country_code3','syc'], // SEYCHELLES
     ['IC2','SLE','is_in:country_code3','sle'], // SIERRA LEONE
     ['IC2','SGP','is_in:country_code3','sgp'], // SINGAPORE
     ['IC2','SXM','is_in:country_code3','sxm'], // SINT MAARTEN (DUTCH PART)
     ['IC2','SVK','is_in:country_code3','svk'], // SLOVAKIA
     ['IC2','SVN','is_in:country_code3','svn'], // SLOVENIA
     ['IC2','SLB','is_in:country_code3','slb'], // SOLOMON ISLANDS
     ['IC2','SOM','is_in:country_code3','som'], // SOMALIA
     ['IC2','ZAF','is_in:country_code3','zaf'], // SOUTH AFRICA
     ['IC2','SGS','is_in:country_code3','sgs'], // SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS
     ['IC2','SSD','is_in:country_code3','ssd'], // SOUTH SUDAN
     ['IC2','ESP','is_in:country_code3','esp'], // SPAIN
     ['IC2','LKA','is_in:country_code3','lka'], // SRI LANKA
     ['IC2','SDN','is_in:country_code3','sdn'], // SUDAN
     ['IC2','SUR','is_in:country_code3','sur'], // SURINAME
     ['IC2','SJM','is_in:country_code3','sjm'], // SVALBARD AND JAN MAYEN
     ['IC2','SWZ','is_in:country_code3','swz'], // SWAZILAND
     ['IC2','SWE','is_in:country_code3','swe'], // SWEDEN
     ['IC2','CHE','is_in:country_code3','che'], // SWITZERLAND
     ['IC2','SYR','is_in:country_code3','syr'], // SYRIAN ARAB REPUBLIC
     ['IC2','TWN','is_in:country_code3','twn'], // TAIWAN PROVINCE OF CHINA
     ['IC2','TJK','is_in:country_code3','tjk'], // TAJIKISTAN
     ['IC2','TZA','is_in:country_code3','tza'], // TANZANIA UNITED REPUBLIC OF
     ['IC2','THA','is_in:country_code3','tha'], // THAILAND
     ['IC2','TLS','is_in:country_code3','tls'], // TIMOR-LESTE
     ['IC2','TGO','is_in:country_code3','tgo'], // TOGO
     ['IC2','TKL','is_in:country_code3','tkl'], // TOKELAU
     ['IC2','TON','is_in:country_code3','ton'], // TONGA
     ['IC2','TTO','is_in:country_code3','tto'], // TRINIDAD AND TOBAGO
     ['IC2','TUN','is_in:country_code3','tun'], // TUNISIA
     ['IC2','TUR','is_in:country_code3','tur'], // TURKEY
     ['IC2','TKM','is_in:country_code3','tkm'], // TURKMENISTAN
     ['IC2','TCA','is_in:country_code3','tca'], // TURKS AND CAICOS ISLANDS
     ['IC2','TUV','is_in:country_code3','tuv'], // TUVALU
     ['IC2','UGA','is_in:country_code3','uga'], // UGANDA
     ['IC2','UKR','is_in:country_code3','ukr'], // UKRAINE
     ['IC2','ARE','is_in:country_code3','are'], // UNITED ARAB EMIRATES
     ['IC2','GBR','is_in:country_code3','gbr'], // UNITED KINGDOM
     ['IC2','USA','is_in:country_code3','usa'], // UNITED STATES
     ['IC2','UMI','is_in:country_code3','umi'], // UNITED STATES MINOR OUTLYING ISLANDS
     ['IC2','URY','is_in:country_code3','ury'], // URUGUAY
     ['IC2','UZB','is_in:country_code3','uzb'], // UZBEKISTAN
     ['IC2','VUT','is_in:country_code3','vut'], // VANUATU
     ['IC2','VEN','is_in:country_code3','ven'], // VENEZUELA BOLIVARIAN REPUBLIC OF
     ['IC2','VNM','is_in:country_code3','vnm'], // VIET NAM
     ['IC2','VGB','is_in:country_code3','vgb'], // VIRGIN ISLANDS BRITISH
     ['IC2','VIR','is_in:country_code3','vir'], // VIRGIN ISLANDS U.S.
     ['IC2','WLF','is_in:country_code3','wlf'], // WALLIS AND FUTUNA
     ['IC2','ESH','is_in:country_code3','esh'], // WESTERN SAHARA
     ['IC2','YEM','is_in:country_code3','yem'], // YEMEN
     ['IC2','ZMB','is_in:country_code3','zmb'], // ZAMBIA
     ['IC2','ZWE','is_in:country_code3','zwe'], // ZIMBABWE

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

     // PBY - By-product - See ZI014_PBY
     // ['PBY','-999999',undefined,undefined], // No Information
     // ['PBY','8','PBY','refuse'], // Refuse
     // ['PBY','15','PBY','slag'], // Slag
     // ['PBY','16','PBY','sludge'], // Sludge
     // ['PBY','17','PBY','spoil'], // Spoil
     // ['PBY','999','PBY','other'], // Other

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

     // POS - Power Source
     // ['POS','-999999',undefined,undefined], // No Information
     ['POS','1','generator:source','geothermal'], // Geothermal
     ['POS','2','generator:source','hydro'], // Hydro-electric
     ['POS','3','generator:source','nuclear'], // Nuclear
     ['POS','4','generator:source','thermal'], // Thermal
     ['POS','5','generator:source','tidal'], // Tidal
     ['POS','999','generator:source','other'], // Other

     // This attribute is handled in Pre and Post processing. See ZI014_PPO
     // PPO - Product
     // ['PPO','-999999',undefined,undefined], // No Information
/*
     ['PPO','1','PPO','aircraft'], // Aircraft
     ['PPO','3','PPO','ammunition'], // Ammunition
     ['PPO','8','PPO','basalt'], // Basalt
     ['PPO','9','PPO','bauxite'], // Bauxite
     ['PPO','11','PPO','bivalve_mollusc'], // Bivalve Mollusc
     ['PPO','13','PPO','brick'], // Brick
     ['PPO','15','PPO','cement'], // Cement
     ['PPO','16','PPO','chemical'], // Chemical
     ['PPO','17','PPO','clay'], // Clay
     ['PPO','18','PPO','coal'], // Coal
     ['PPO','19','PPO','cobbles'], // Cobbles
     ['PPO','21','PPO','coke'], // Coke
     ['PPO','26','PPO','copper'], // Copper
     ['PPO','29','PPO','crustacean'], // Crustacean
     ['PPO','30','PPO','cultivated_shellfish'], // Cultivated Shellfish
     ['PPO','33','PPO','diamond'], // Diamond
     ['PPO','34','PPO','diatomaceous_earth'], // Diatomaceous Earth
     ['PPO','35','PPO','dolomite'], // Dolomite
     ['PPO','37','PPO','electric_power'], // Electric Power
     ['PPO','38','PPO','explosive'], // Explosive
     ['PPO','39','PPO','fish'], // Fish
     ['PPO','45','PPO','gas'], // Gas
     ['PPO','46','PPO','petrol'], // Petrol
     ['PPO','48','PPO','gold'], // Gold
     ['PPO','50','PPO','granite'], // Granite
     ['PPO','53','PPO','gravel'], // Gravel
     ['PPO','58','PPO','iron'], // Iron
     ['PPO','59','PPO','lead'], // Lead
     ['PPO','60','PPO','lime'], // Lime
     ['PPO','61','PPO','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['PPO','62','PPO','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['PPO','63','PPO','lumber'], // Lumber
     ['PPO','65','PPO','manganese'], // Manganese
     ['PPO','66','PPO','marble'], // Marble
     ['PPO','72','PPO','mussels'], // Mussels
     ['PPO','77','PPO','oysters'], // Oysters
     ['PPO','83','PPO','petroleum'], // Petroleum
     ['PPO','85','PPO','porphyry'], // Porphyry
     ['PPO','88','PPO','pumice'], // Pumice
     ['PPO','89','PPO','quartz'], // Quartz
     ['PPO','90','PPO','radioactive_material'], // Radioactive Material
     ['PPO','93','PPO','rock'], // Rock
     ['PPO','95','PPO','salt'], // Salt
     ['PPO','96','PPO','sand'], // Sand
     ['PPO','97','PPO','sandstone'], // Sandstone
     ['PPO','101','PPO','sewage'], // Sewage
     ['PPO','105','PPO','silver'], // Silver
     ['PPO','110','PPO','stone'], // Stone
     ['PPO','116','PPO','timber'], // Timber
     ['PPO','117','PPO','tobacco'], // Tobacco
     ['PPO','118','PPO','travertine'], // Travertine
     ['PPO','120','PPO','uranium'], // Uranium
     ['PPO','122','PPO','water'], // Water
     ['PPO','126','PPO','zinc'], // Zinc
     ['PPO','130','PPO','chalk'], // Chalk
     ['PPO','146','PPO','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['PPO','149','PPO','brine'], // Brine
     ['PPO','151','PPO','chromium'], // Chromium
     ['PPO','152','PPO','nickel'], // Nickel
     ['PPO','153','PPO','tin'], // Tin
     ['PPO','158','PPO','pottery'], // Pottery
     ['PPO','159','PPO','charcoal'], // Charcoal
     ['PPO','160','PPO','milled_grain'], // Milled Grain
     ['PPO','161','PPO','coalbed_methane'], // Coalbed Methane
     ['PPO','162','PPO','natural_gas_condensate'], // Natural Gas Condensate
     ['PPO','163','PPO','helium'], // Helium
     ['PPO','164','PPO','hydrothermal_fluid'], // Hydrothermal Fluid
     ['PPO','173','PPO','fluorite'], // Fluorite
     ['PPO','204','PPO','phosphate'], // Phosphate
     ['PPO','214','PPO','biodiesel'], // Biodiesel
     ['PPO','224','PPO','vanadium'], // Vanadium
     ['PPO','283','PPO','mica'], // Mica
     ['PPO','339','PPO','selenium'], // Selenium
     ['PPO','999','PPO','other'], // Other
*/
     // This attribute is handled in Pre and Post processing. See ZI014_PRW
     // PRW - Raw Material
     // ['PRW','-999999',undefined,undefined], // No Information
     // ['PRW','12','resource','coal'], // Coal
     // ['PRW','14','resource','coke'], // Coke
     // ['PRW','34','resource','gravel'], // Gravel
     // ['PRW','54','resource','ore'], // Ore
     // ['PRW','67','resource','salt'], // Salt
     // ['PRW','68','resource','sand'], // Sand
     // ['PRW','84','resource','stone'], // Stone
     // ['PRW','154','resource','sulphur'], // Sulphur
     // ['PRW','999','resource','other'], // Other

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
     ['RAS','999','radar:use','other'], // Other

     // RFD - Roofed (really?)
     // ['RFD','-999999',undefined,undefined], // No Information
     ['RFD','1000','bridge:roof','no'],
     ['RFD','1001','bridge:roof','yes'],

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

     // RIP - Rig Present
     // ['RIP','-999999',undefined,undefined], // No Information
     ['RIP','1000','rig','no'],
     ['RIP','1001','rig','yes'],

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
     ['RTA','4','arrangement','juxtaposition'], // Juxtaposition
     ['RTA','999','arrangement','other'], // Other

     // RTN_ROI - Route Designation (route designation type)
     // ['RTN_ROI','-999999',undefined,undefined], // No Information
     ['RTN_ROI','1','ref:road:class','international'], // International
     ['RTN_ROI','2','ref:road:class','national_motorway'], // National Motorway
     ['RTN_ROI','3','ref:road:class','primary'], // National
     ['RTN_ROI','4','ref:road:class','secondary'], // Secondary
     ['RTN_ROI','5','ref:road:class','local'], // Local
     ['RTN_ROI','999','ref:road:class','other'], // Other

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

     // SDCC - Soil Depth (interval closure)
     ['SDCC','2','soil:depth:closure','open_interval'], // Open Interval
     ['SDCC','3','soil:depth:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['SDCC','4','soil:depth:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['SDCC','5','soil:depth:closure','closed_interval'], // Closed Interval
     ['SDCC','6','soil:depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['SDCC','7','soil:depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['SDCC','8','soil:depth:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['SDCC','9','soil:depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

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

     // ['SRD','-999999',undefined,undefined], // No Information
     // SRD - Terrain Morphology
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
     ['SRE','4','traffic_restriction','sharp_curve'], // Sharp Curve
     ['SRE','5','traffic_restriction','steep_grade'], // Steep Grade
     ['SRE','8','traffic_restriction','sharp_curves'], // Sharp Curves
     ['SRE','9','traffic_restriction','steep_curves'], // Steep Curves
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
     ['SSR','51','roof:shape','with_steeple'], // With Steeple
     ['SSR','55','roof:shape','flat_with_clerestory'], // Flat with Clerestory
     ['SSR','64','roof:shape','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR','77','roof:shape','with_cupola'], // With Cupola
     ['SSR','78','roof:shape','with_turret'], // With Turret
     ['SSR','79','roof:shape','with_tower'], // With Tower
     ['SSR','80','roof:shape','with_minaret'], // With Minaret
     ['SSR','81','roof:shape','with_smokestack'], // With Smokestack
     ['SSR','82','roof:shape','flat_with_parapet'], // Flat with Parapet
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
     ['STP','17','soil:type','ml-cl:silt_fine_sand_and_lean_clay'], // ML-CL: Silt, Fine Sand and Lean Clay
     ['STP','18','soil:type','evaporite'], // Evaporite
     ['STP','99','soil:type','not_evaluated'], // Not Evaluated
     ['STP','999','soil:type','other'], // Other

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
     ['TRS','999','transport:type','other'], // Other
     // ['TRS','0','transport:type','unknown'],
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

     // TSCC - Tree Spacing (interval closure)
     ['TSCC','2','tree:spacing:closure','open_interval'], // Open Interval
     ['TSCC','3','tree:spacing:closure','greater-than-or-equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['TSCC','4','tree:spacing:closure','greater-than_to_less-than-or-equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['TSCC','5','tree:spacing:closure','closed_interval'], // Closed Interval
     ['TSCC','6','tree:spacing:closure','greater-than_semi-interval'], // Greater-than Semi-interval
     ['TSCC','7','tree:spacing:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['TSCC','8','tree:spacing:closure','less-than_semi-interval'], // Less-than Semi-interval
     ['TSCC','9','tree:spacing:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

     // TSM - Terrain Surface Material
     // ['TSM','-999999',undefined,undefined], // No Information
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

     // TYP - Thoroughfare Type: Not pretty, need to look at custom rules
     // ['TYP','-999999',undefined,undefined], // No Information
     ['TYP','1','ref:road:type','road'], // Road
     ['TYP','5','ref:road:type','boulevard'], // Boulevard
     ['TYP','9','ref:road:type','avenue'], // Avenue
     ['TYP','13','ref:road:type','drive'], // Drive
     ['TYP','17','ref:road:type','circle'], // Circle
     ['TYP','21','ref:road:type','close'], // Close
     ['TYP','25','ref:road:type','parkway'], // Parkway
     ['TYP','29','ref:road:type','lane'], // Lane
     ['TYP','33','ref:road:type','street'], // Street
     ['TYP','37','ref:road:type','terrace'], // Terrace
     ['TYP','41','ref:road:type','motorway'], // Motorway
     ['TYP','45','ref:road:type','place'], // Place
     ['TYP','47','ref:road:type','limited_access_motorway'], // Limited Access Motorway
     ['TYP','48','junction','roundabout'], // Roundabout from AP020 Road Interchange
     ['TYP','50','link_road','yes'], // Ramp, from AP020 Road Interchange
     ['TYP','999','ref:road:type','other'], 

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

     // VCT - Void Collection Type
     // ['VCT','-999999',undefined,undefined], // No Information
     ['VCT','1','void_collection:type','hypsography'], // Hypsography
     ['VCT','3','void_collection:type','bathymetry'], // Bathymetry
     ['VCT','4','void_collection:type','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT','999','void_collection:type','other'], // Other

     // VDT - Vertical Datum
     // ['VDT','-999999',undefined,undefined], // No Information
     ['VDT','groundLevel','source:datum:vertical','ground_level'], // Ground Level
     ['VDT','meanSeaLevel','source:datum:vertical','mean_sea_level'], // Mean Sea Level (MSL)
     ['VDT','ngvd29','source:datum:vertical','national_geodetic_source:datum:vertical_1929'], // National Geodetic Vertical Datum (NGVD) 1929
     ['VDT','navd88','source:datum:vertical','north_american_source:datum:vertical_1988'], // North American Vertical Datum (NAVD) 1988
     ['VDT','wgs84Egm08','source:datum:vertical','wgs_84_egm08_geoid'], // WGS 84 EGM08 Geoid
     ['VDT','wgs84Egm96','source:datum:vertical','wgs_84_egm96_geoid'], // WGS 84 EGM96 Geoid
     ['VDT','wgs84','source:datum:vertical','wgs_84_ellipsoid'], // WGS 84 Ellipsoid
     ['VDT','other','source:datum:vertical','other'], // Other

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
     ['VSP','4','vegetation:type','casuarina'], // Casuarina
     ['VSP','6','vegetation:type','conifer'], // Conifer
     ['VSP','8','vegetation:type','cypress'], // Cypress
     ['VSP','10','vegetation:type','eucalyptus'], // Eucalyptus
     ['VSP','11','vegetation:type','filao'], // Filao
     ['VSP','19','vegetation:type','mangrove'], // Mangrove
     ['VSP','22','vegetation:type','nipa'], // Nipa
     ['VSP','25','vegetation:type','palm'], // Palm
     ['VSP','34','vegetation:type','bamboo'], // Bamboo
     ['VSP','999','vegetation:type','other'], // Other

     // WCC - Watercourse Channel Type
     // ['WCC','-999999',undefined,undefined], // No Information
     // ['WCC','0','waterway','river'],
     ['WCC','1','waterway','stream'], // Channelized Stream
     ['WCC','2','waterway','braided_stream'], // Braided Stream
     ['WCC','3','waterway','gorge'], // Gorge
     ['WCC','4','waterway','wadi'], // Wadi
     ['WCC','6','waterway','split_stream'], // Split Stream
     ['WCC','7','waterway','river'], // Normal Channel
     ['WCC','8','waterway','lost_watercourse'], // Lost Watercourse
     ['WCC','999','waterway','other'], // Other

     // WEQ - Well Equipment
     // ['WEQ','-999999',undefined,undefined], // No Information
     ['WEQ','1','well:equipment','wellhead'], // Wellhead
     ['WEQ','2','well:equipment','christmas_tree'], // Christmas Tree
     ['WEQ','3','well:equipment','manifold'], // Manifold
     ['WEQ','4','well:equipment','protective_structure'], // Protective Structure
     ['WEQ','5','well:equipment','pump'], // Pump
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
     // ['YWQ','1','YWQ','potable'], // Potable
     // ['YWQ','2','YWQ','treatable'], // Treatable
     // ['YWQ','3','YWQ','contaminated'], // Contaminated
     // ['YWQ','4','YWQ','nonpotable'], // Nonpotable
     // ['YWQ','999','YWQ','other'], // Other

     // ZI001_NSP - Source Information : Non-spatial Source Type
     ['ZI001_NSP','ngaAutoAirFacInfoFile','source:non_spatial_source:type','aafif'], // AAFIF
     ['ZI001_NSP','ngaControlledImageBase1','source:non_spatial_source:type','cib1'], // CIB 1
     ['ZI001_NSP','commercialData','source:non_spatial_source:type','commercial_data'], // Commercial Data
     ['ZI001_NSP','ngaDigitalAirFltInfoFile','source:non_spatial_source:type','dafif'], // DAFIF
     ['ZI001_NSP','digitalGlobeImagery','source:non_spatial_source:type','digitalglobe_imagery'], // DigitalGlobe Imagery
     ['ZI001_NSP','ngaDigitalNauticalChart','source:non_spatial_source:type','dnc'], // DNC
     ['ZI001_NSP','ngaDigitalVertObstruction','source:non_spatial_source:type','dvof'], // DVOF
     ['ZI001_NSP','ngaFoundationFeatureData','source:non_spatial_source:type','ffd'], // FFD
     ['ZI001_NSP','ngaGeoNames','source:non_spatial_source:type','geonames'], // GeoNames
     ['ZI001_NSP','gpsBasedFieldCollect','source:non_spatial_source:type','gps'], // GPS
     ['ZI001_NSP','ikonosImagery','source:non_spatial_source:type','ikonos_imagery'], // Ikonos Imagery
     ['ZI001_NSP','imageryUnspecified','source:non_spatial_source:type','imagery'], // Imagery
     ['ZI001_NSP','ngaInterimTerrainData','source:non_spatial_source:type','itd'], // ITD
     ['ZI001_NSP','ngaInterimVectorData','source:non_spatial_source:type','ivd'], // IVD
     ['ZI001_NSP','landsatImagery','source:non_spatial_source:type','landsat_imagery'], // Landsat Imagery
     ['ZI001_NSP','usModernizedIntegratedDB','source:non_spatial_source:type','midb'], // MIDB
     ['ZI001_NSP','militaryMapData','source:non_spatial_source:type','military_map_data'], // Map Data
     ['ZI001_NSP','nonMilitaryMap','source:non_spatial_source:type','non-military_map'], // Non-military Map
     ['ZI001_NSP','usNtmImagery','source:non_spatial_source:type','ntm_imagery'], // NTM Imagery
     ['ZI001_NSP','openSourceInfo','source:non_spatial_source:type','open_source_information'], // Open Source Information
     ['ZI001_NSP','operationsData','source:non_spatial_source:type','operations_data'], // Operations Data
     ['ZI001_NSP','orbimageImagery','source:non_spatial_source:type','orbimage_imagery'], // ORBIMAGE Imagery
     ['ZI001_NSP','quickBirdImagery','source:non_spatial_source:type','quickbird_imagery'], // QuickBird Imagery
     ['ZI001_NSP','ngaStereoAirfieldCollect','source:non_spatial_source:type','sac'], // SAC
     ['ZI001_NSP','ngaUrbanVectorMap','source:non_spatial_source:type','uvmap'], // UVMap
     ['ZI001_NSP','ngaVectorInterimTerrain','source:non_spatial_source:type','vitd'], // VITD
     ['ZI001_NSP','ngaVectorMap2','source:non_spatial_source:type','vmap2'], // VMap 2
     ['ZI001_NSP','other','source:non_spatial_source:type','other'], // Other

     // ZI001_SSY - Source Information : Spatial Source Type
     ['ZI001_SSY','ngaControlledImageBase1','source:spatial_source:type','cib1'], // CIB 1
     ['ZI001_SSY','digitalGlobeImagery','source:spatial_source:type','digitalglobe_imagery'], // DigitalGlobe Imagery
     ['ZI001_SSY','ngaDigitalNauticalChart','source:spatial_source:type','dnc'], // DNC
     ['ZI001_SSY','ngaFoundationFeatureData','source:spatial_source:type','ffd'], // FFD
     ['ZI001_SSY','geoEye1','source:spatial_source:type','geoeye1_imagery'], // GeoEye 1 Imagery
     ['ZI001_SSY','gpsBasedFieldCollect','source:spatial_source:type','gps'], // GPS
     ['ZI001_SSY','ikonosImagery','source:spatial_source:type','ikonos_imagery'], // Ikonos Imagery
     ['ZI001_SSY','imageryUnspecified','source:spatial_source:type','imagery'], // Imagery
     ['ZI001_SSY','ngaInterimTerrainData','source:spatial_source:type','itd'], // ITD
     ['ZI001_SSY','usModernizedIntegratedDB','source:spatial_source:type','midb'], // MIDB
     ['ZI001_SSY','usNtmImagery','source:spatial_source:type','ntm_imagery'], // NTM Imagery
     ['ZI001_SSY','operationsData','source:spatial_source:type','operations_data'], // Operations Data
     ['ZI001_SSY','orbimageImagery','source:spatial_source:type','orbimage_imagery'], // ORBIMAGE Imagery
     ['ZI001_SSY','quickBirdImagery','source:spatial_source:type','quickbird_imagery'], // QuickBird Imagery
     ['ZI001_SSY','other','source:spatial_source:type','other'], // Other

     // ZI001_VSC - Source Information : Vertical Source Category
     ['ZI001_VSC','interpolatedDted1','source:vertical_source:type','dted1'], // DTED 1
     ['ZI001_VSC','interpolatedDted2','source:vertical_source:type','dted2'], // DTED 2
     ['ZI001_VSC','noElevations','source:vertical_source:type','no_elevations'], // No Elevations
     ['ZI001_VSC','reflectiveSurface','source:vertical_source:type','reflective_surface'], // Reflective Surface
     ['ZI001_VSC','stereoscopicImagery','source:vertical_source:type','stereoscopic_imagery'], // Stereoscopic Imagery
     ['ZI001_VSC','tinData','source:vertical_source:type','tin_data'], // TIN Data
     ['ZI001_VSC','other','source:vertical_source:type','other'], // Other

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
     ['ZI013_FMM','5','farm:method','permanent_irrigation'], // Permanent Irrigation
     ['ZI013_FMM','999','farm:method','other'], // Other

     // ZI013_IRG - Crop Information : Irrigation Method
     // ['ZI013_IRG','-999999',undefined,undefined], // No Information
     ['ZI013_IRG','1','farm:irrigation','furrow'], // Furrow
     ['ZI013_IRG','2','farm:irrigation','overhead'], // Overhead
     ['ZI013_IRG','3','farm:irrigation','centre_pivot'], // Center Pivot
     ['ZI013_IRG','4','farm:irrigation','linear_move'], // Linear Move
     ['ZI013_IRG','999','farm:irrigation','other'], // Other

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
     ['ZI014_PBY','16','by_product','sludge'], // Sludge
     ['ZI014_PBY','17','by_product','spoil'], // Spoil
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


     // ZI016_ROC - Route Pavement Information : Route Surface Composition
     // ['ZI016_ROC','-999999','surface','unknown'], // Trying this instead of undefined
     ['ZI016_ROC','-999999',undefined,undefined], // No Information
     ['ZI016_ROC','1','surface','ground'], // Unimproved 
     ['ZI016_ROC','2','surface','compacted'], // Stabilized earth
     ['ZI016_ROC','3','surface','flexible_pavement'], // Flexible Pavement
     ['ZI016_ROC','4','surface','gravel'], // Aggregate 
     ['ZI016_ROC','5','surface','macadam'], // Macadam
     ['ZI016_ROC','6','surface','bound_surface'], // Bound Surface
     ['ZI016_ROC','7','surface','ridgid_pavement'], // Rigid Pavement
     ['ZI016_ROC','8','surface','concrete'], // Concrete
     ['ZI016_ROC','9','surface','asphalt'], // Asphalt 
     ['ZI016_ROC','10','surface','asphalt_over_concrete'], // Asphalt over Concrete
     ['ZI016_ROC','11','surface','cobblestone'], // Cebble-stone
     ['ZI016_ROC','12','surface','brick'], // Brick
     ['ZI016_ROC','13','surface','metal'], // Metal
     ['ZI016_ROC','14','surface','wood'], // Wood
     ['ZI016_ROC','15','surface','corduroy'], // rough hewn logs...  // Corduroy
     ['ZI016_ROC','16','surface','wood_plank'], // Wood Plank
     ['ZI016_ROC','17','surface','ice'], // Ice
     ['ZI016_ROC','18','surface','snow'], // Snow
     ['ZI016_ROC','999','surface','other'], // Other

     // ZI016_WTC - Route Pavement Information : Road Weather Restriction
     // ['ZI016_WTC','-999999',undefined,undefined], // No Information
     ['ZI016_WTC','1','seasonal','no'], // All-weather
     ['ZI016_WTC','2','seasonal','fair'], // possibly seasonal // Fair-weather
     ['ZI016_WTC','3','seasonal','winter'], // Winter Only
     ['ZI016_WTC','4','seasonal','limited'], // Limited all-weather 
     ['ZI016_WTC','5','seasonal','not_winter'], // Closed in Winter
     ['ZI016_WTC','999','seasonal','other'], // Other

     // ZI017_RGC - Track Information : Railway Gauge Classification
     // ['ZI017_RGC','0','gauge:type','unknown'],
     // ['ZI017_RGC','-999999',undefined,undefined], // No Information
     ['ZI017_RGC','1','gauge:type','broad'], // Broad
     ['ZI017_RGC','2','gauge:type','narrow'], // Narrow
     ['ZI017_RGC','3','gauge:type','standard'], // Standard

     // ZI017_RIR - Track Information : Railway in Road
     // ['ZI017_RIR','-999999',undefined,undefined], // No Information
     // ['ZI017_RIR','0','railway:in_road','unknown'],
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

     /* See WLE
     // ZI025_WLE - Hydrographic Vertical Positioning Information : Water Level Effect
     // ['ZI025_WLE','-999999',undefined,undefined], // No Information
     ['ZI025_WLE','1','ZI025_WLE','partly_submerged'], // Partly Submerged
     ['ZI025_WLE','2','ZI025_WLE','always_dry'], // Always Dry
     ['ZI025_WLE','3','ZI025_WLE','always_submerged'], // Always Submerged
     ['ZI025_WLE','4','ZI025_WLE','covers_and_uncovers'], // Covers and Uncovers
     ['ZI025_WLE','5','ZI025_WLE','awash_at_low_water'], // Awash at Low Water
     ['ZI025_WLE','9','ZI025_WLE','awash_at_chart_datum'], // Awash at Chart Datum
     ['ZI025_WLE','999','ZI025_WLE','other'], // Other
     */

     // ZI032_GUG - Pylon Information : Guyed - See GUG
     // ['ZI032_GUG','-999999',undefined,undefined], // No Information
     // ['ZI032_GUG','1000','guyed','no'],
     // ['ZI032_GUG','1001','guyed','yes'],

     // ZI032_PYC - Pylon Information : Pylon Configuration -- See PYC
     // ['ZI032_PYC','-999999',undefined,undefined], // No Information
     // ['ZI032_PYC','1','ZI032_PYC','a'], // 'A'
     // ['ZI032_PYC','2','ZI032_PYC','h'], // 'H'
     // ['ZI032_PYC','3','ZI032_PYC','i'], // 'I'
     // ['ZI032_PYC','4','ZI032_PYC','y'], // 'Y'
     // ['ZI032_PYC','5','ZI032_PYC','t'], // 'T'
     // ['ZI032_PYC','999','ZI032_PYC','other'], // Other

     // ZI032_PYM - Pylon Information : Pylon Material -- See PYM
     // ['ZI032_PYM','-999999',undefined,undefined], // No Information
     // ['ZI032_PYM','1','ZI032_PYM','aluminum'], // Aluminum
     // ['ZI032_PYM','3','ZI032_PYM','concrete'], // Concrete
     // ['ZI032_PYM','4','ZI032_PYM','masonry'], // Masonry
     // ['ZI032_PYM','5','ZI032_PYM','metal'], // Metal
     // ['ZI032_PYM','7','ZI032_PYM','wood'], // Wood
     // ['ZI032_PYM','8','ZI032_PYM','steel'], // Steel
     // ['ZI032_PYM','9','ZI032_PYM','fibreglass'], // Fibreglass
     // ['ZI032_PYM','10','ZI032_PYM','iron'], // Iron
     // ['ZI032_PYM','999','ZI032_PYM','other'], // Other

     // ZI032_TOS - Pylon Information : Tower Shape - See TOS
     // ['ZI032_TOS','-999999',undefined,undefined], // No Information
     // ['ZI032_TOS','6','ZI032_TOS','mast'], // Mast
     // ['ZI032_TOS','8','ZI032_TOS','pole'], // Pole
     // ['ZI032_TOS','11','ZI032_TOS','tripod'], // Tripod
     // ['ZI032_TOS','12','ZI032_TOS','truss'], // Truss
     // ['ZI032_TOS','13','ZI032_TOS','tubular'], // Tubular
     // ['ZI032_TOS','999','ZI032_TOS','other'], // Other

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
     ['ZI037_REL','999','religion','other'], // Other

     // ZI037_RFA - Religious Information : Religious Facility Type
     // ['ZI037_RFA','-999999',undefined,undefined], // No Information
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
     ['ZI037_RFA','23','building','burial_site'], // Burial Site
     ['ZI037_RFA','999','building:religious','other'], // Other - to avoid conflicts
     ['ZI037_RFA','-999999','building:religious','unknown'], // Other - to avoid conflicts

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

     // ZI071_FFN - Cave Information : Feature Function -- See FFN
     // ['ZI071_FFN','-999999',undefined,undefined], // No Information
     // ['ZI071_FFN','530','building','warehouse'], // Warehousing and Storage
     // ['ZI071_FFN','563','building','residence'], // Residence
     // ['ZI071_FFN','835','building','military'], // Defence Activities
     // ['ZI071_FFN','930','amenity','religious_activities'], // Religious Activities
     // ['ZI071_FFN','999','building','other'], // Other

     // ZI071_UAO - Cave Information : Underground Access Orientation
     // ['ZI071_UAO','-999999',undefined,undefined], // No Information
     ['ZI071_UAO','1','direction','horizontal'], // Horizontal
     ['ZI071_UAO','2','direction','slopes_downward'], // Slopes Downward
     ['ZI071_UAO','3','direction','vertical_down'], // Vertical Down
     ['ZI071_UAO','4','direction','slopes_upward'], // Slopes Upward
     ['ZI071_UAO','5','direction','vertical_up'], // Vertical Up

//   #######################
     // Text Enumerations - We swapped them for strings in the Schema. 
     // Some of these are ugly and will probably cause problems with dirty-word checkers

     // ZSAX_RS0 - Restriction Information : Security Attributes Group (resource classification)
     ['ZSAX_RS0','U','security:classification','UNCLASSIFIED'],
     ['ZSAX_RS0','R','security:classification','RESTRICTED'],
     ['ZSAX_RS0','C','security:classification','CONFIDENTIAL'],
     ['ZSAX_RS0','S','security:classification','SECRET'],
     ['ZSAX_RS0','TS','security:classification','TOP_SECRET'],

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

     // End Text Enumeration
     ], // End one2one

     // Input Translation Rules:
     // These are only used for IMPORT and get added to the standard one2one rules
     // This is mainly for TDS 3.0 attributes but we have added some "funky" TDSv40 attributes as well.
     one2oneIn : [
     // HCT - Thoroughfare Class TDSv30
     // ['HCT','-999999',undefined,undefined], // Null
     ['HCT','1','ref:road:class','primary'], // Primary Route
     ['HCT','2','ref:road:class','secondary'], // Secondary
     ['HCT','3','ref:road:class','national_motorway'], // National Motorway
     ['HCT','4','ref:road:class','local'], // Local
     ['HCT','999','ref:road:class','other'],

     // FUN - Condition of Facility TDSv30
     // ['FUN','-999999',undefined,undefined], // Null
     ['FUN','1','condition','construction'], // Check on this
     ['FUN','2','condition','abandoned'],
     ['FUN','3','condition','destroyed'],
     ['FUN','4','condition','dismantled'],
     ['FUN','5','condition','proposed'],
     ['FUN','6','condition','functional'], //Fully Functional - default
     ['FUN','7','condition','ruined'],
     ['FUN','9','condition','partly_functional'],
     ['FUN','11','condition','partly_destroyed'],
     ['FUN','13','condition','damaged'],
     ['FUN','16','condition','disused'],
     ['FUN','17','condition','planned'],
     ['FUN','998',undefined,undefined], // From data, no value in spec

     // AMB - Special Administrative Unit TDSv30
     // ['AMB','-999999',undefined,undefined], // No Information
     ['AMB','1','raw:AMB','lease_area'], // Lease Area
     ['AMB','2','raw:AMB','concession_area'], // Concession Area
     ['AMB','3','raw:AMB','free_trade_zone'], // Free Trade Zone
     ['AMB','4','raw:AMB','economic_region'], // Economic Region
     ['AMB','5','raw:AMB','postal_district'], // Postal District
     ['AMB','6','raw:AMB','census_district'], // Census District
     ['AMB','7','raw:AMB','water_management_district'], // Water Management District
     ['AMB','8','raw:AMB','congressional_district'], // Congressional District
     ['AMB','9','raw:AMB','environmental_protection_agency_region'], // Environmental Protection Agency Region
     ['AMB','10','raw:AMB','federal_emergency_management_agency_region'], // Federal Emergency Management Agency Region
     ['AMB','11','raw:AMB','federal_energy_regulatory_commission_region'], // Federal Energy Regulatory Commission Region
     ['AMB','12','raw:AMB','native_american_reservation'], // Native American Reservation
     ['AMB','13','raw:AMB','radiological_assistance_program_region'], // Radiological Assistance Program Region
     ['AMB','14','raw:AMB','federal_aviation_administration_region'], // Federal Aviation Administration Region
     ['AMB','15','raw:AMB','army_corps_of_engineers_district'], // Army Corps of Engineers District
     ['AMB','16','raw:AMB','army_corps_of_engineers_division'], // Army Corps of Engineers Division
     ['AMB','17','raw:AMB','coast_guard_sector'], // Coast Guard Sector
     ['AMB','18','raw:AMB','border_patrol_sector'], // Border Patrol Sector
     ['AMB','19','raw:AMB','fbi_field_office_jurisdiction'], // FBI Field Office Jurisdiction
     ['AMB','20','raw:AMB','fbi_resident_agency_district'], // FBI Resident Agency District

     // BAL - BGN Administrative Level TDSv30
     // ['BAL','-999999',undefined,undefined], // No Information
     ['BAL','1','raw:BAL','first-order'], // First-order
     ['BAL','2','raw:BAL','second-order'], // Second-order
     ['BAL','3','raw:BAL','third-order'], // Third-order
     ['BAL','4','raw:BAL','fourth-order'], // Fourth-order
     ['BAL','5','raw:BAL','undifferentiated'], // Undifferentiated
     ['BAL','999','raw:BAL','other'], // Other

     // BST - Boundary Status TDSv30
     // ['BST','-999999',undefined,undefined], // No Information
     ['BST','1','raw:BST','definite'], // Definite
     ['BST','2','raw:BST','indefinite'], // Indefinite
     ['BST','3','raw:BST','in_dispute'], // In Dispute
     ['BST','4','raw:BST','no_defined_boundary'], // No Defined Boundary

     // COS - Facility Operational Status TDSv30
     ['COS','1','operational_status','operational'], // Continuous
     ['COS','2','operational_status','intermittent'], // Intermittent

     // DSP - Boundary Dispute Type TDSv30
     // ['DSP','-999999',undefined,undefined], // No Information
     ['DSP','1','raw:DSP','in_dispute'], // In Dispute
     ['DSP','2','raw:DSP','undisputed'], // Undisputed
     ['DSP','3','raw:DSP','de_facto'], // De Facto
     ['DSP','4','raw:DSP','de_jure'], // De Jure

     // FFN From TDSv30
     ['FFN','19','amenity','hunting'], // Hunting
     ['FFN','21','landuse','silviculture'], // Silviculture - it's like forestry
     ['FFN','559','building','apartment_house'], // Apartment House
     ['FFN','560','building','detached_house'], // Detached House
     ['FFN','561','building','attached_house'], // Attached House
     ['FFN','562','building','manufactured_house'], // Manufactured House
     ['FFN','960','shop','retail_service'], // Retail Service

     // GEC - Geopolitical Entity Type TDSv30
     // ['GEC','-999999',undefined,undefined], // No Information
     ['GEC','5','raw:GEC','area_of_no_sovereignty'], // Area of No Sovereignty
     ['GEC','7','raw:GEC','demilitarized_zone_(dmz)'], // Demilitarized Zone (DMZ)
     ['GEC','8','raw:GEC','zone_of_occupation'], // Zone of Occupation
     ['GEC','9','raw:GEC','leased_area'], // Leased Area
     ['GEC','10','raw:GEC','political_entity'], // Political Entity
     ['GEC','11','raw:GEC','dependent_political_entity'], // Dependent Political Entity
     ['GEC','12','raw:GEC','freely_associated_state'], // Freely Associated State
     ['GEC','13','raw:GEC','independent_political_entity'], // Independent Political Entity
     ['GEC','14','raw:GEC','semi-independent_political_entity'], // Semi-independent Political Entity
     ['GEC','15','raw:GEC','economic_region'], // Economic Region
     ['GEC','16','raw:GEC','territory'], // Territory
     ['GEC','17','raw:GEC','buffer_zone'], // Buffer Zone
     ['GEC','999','raw:GEC','other'], // Other

     // LOC TDSv30
     ['LOC','1','location','raised'], // Raised
     ['LOC','2','location','level'], // Level
     ['LOC','3','location','depressed'], // Depressed

     // LSP - Geopolitical Line Type TDSv30
     // ['LSP','-999999',undefined,undefined], // No Information
     ['LSP','1','raw:LSP','generic_administrative_boundary'], // Generic Administrative Boundary
     ['LSP','2','raw:LSP','provisional_administrative_line'], // Provisional Administrative Line
     ['LSP','3','raw:LSP','armistice_line'], // Armistice Line
     ['LSP','4','raw:LSP','line_of_control'], // Line of Control
     ['LSP','5','raw:LSP','demarcation_line'], // Demarcation Line
     ['LSP','6','raw:LSP','line_of_convenience'], // Line of Convenience
     ['LSP','7','raw:LSP','cease_fire_line'], // Cease Fire Line
     ['LSP','8','raw:LSP','convention_line'], // Convention Line
     ['LSP','9','raw:LSP','claim_line'], // Claim Line
     ['LSP','10','raw:LSP','intercolonial_line'], // Intercolonial Line
     ['LSP','11','raw:LSP','interentity_line'], // Interentity Line
     ['LSP','12','raw:LSP','line_of_adjacency'], // Line of Adjacency
     ['LSP','13','raw:LSP','line_of_withdrawal'], // Line of Withdrawal
     ['LSP','14','raw:LSP','military_disengagement_line'], // Military Disengagement Line
     ['LSP','15','raw:LSP','treaty_line'], // Treaty Line
     ['LSP','16','raw:LSP','unclos_claim_boundary'], // UNCLOS Claim Boundary
     ['LSP','17','raw:LSP','generic_international_boundary'], // Generic International Boundary
     ['LSP','999','raw:LSP','other'], // Other

     // MGL1 - Maritime Geopolitical Limit Type [1] TDSv30
     // ['MGL1','-999999',undefined,undefined], // No Information
     ['MGL1','1','raw:MGL1','international_boundary'], // International Boundary
     ['MGL1','2','raw:MGL1','territorial_sea_baseline'], // Territorial Sea Baseline
     ['MGL1','3','raw:MGL1','three_nautical_mile_limit'], // Three Nautical Mile Limit
     ['MGL1','4','raw:MGL1','territorial_waters_limit_of_sovereignty'], // Territorial Waters Limit of Sovereignty
     ['MGL1','5','raw:MGL1','contiguous_zone_limit'], // Contiguous Zone Limit
     ['MGL1','6','raw:MGL1','continental_shelf_limit'], // Continental Shelf Limit
     ['MGL1','7','raw:MGL1','extended_continental_shelf_limit'], // Extended Continental Shelf Limit
     ['MGL1','8','raw:MGL1','exclusive_economic_zone_limit'], // Exclusive Economic Zone Limit
     ['MGL1','9','raw:MGL1','customs_boundary'], // Customs Boundary
     ['MGL1','999','raw:MGL1','other'], // Other

     // PPC - Power Station Type TDSv30
     // ['PPC','-999999',undefined,undefined], // No Information
     ['PPC','1','generator:source','hydro'], // Hydro-electric
     ['PPC','2','generator:source','nuclear'], // Nuclear
     ['PPC','4','generator:source','geothermal'], // Geothermal
     ['PPC','6','generator:source','tidal'], // Tidal
     ['PPC','7','generator:source','thermal'], // Thermal
     ['PPC','999','generator:source','other'], // Other

     // RPC - Boundary Representation Policy TDSv30
     // ['RPC','-999999',undefined,undefined], // No Information
     ['RPC','1','raw:RPC','show_no_line'], // Show No Line
     ['RPC','2','raw:RPC','definite'], // Definite
     ['RPC','3','raw:RPC','indefinite'], // Indefinite
     ['RPC','4','raw:RPC','in_dispute'], // In Dispute
     ['RPC','5','raw:RPC','administrative_as_international'], // Administrative as International
     ['RPC','999','raw:RPC','other'], // Other

     // VDT - Vertical Datum
     // From MAAX data
     ['VDT','WGS 84 Ellipsoid','source:datum:vertical','wgs_84_ellipsoid'], // WGS 84 Ellipsoid

    ], // End one2oneIn


    // One2one translation table for converting "Other" OSM attributes to NFDD
    // This is for Export only. The values are swapped before use
    one2oneOut : [
     // ZSAX_RS0 - Restriction Information : Security Attributes Group (resource classification)
     ['ZSAX_RS0','U','security:classification','U'],
     ['ZSAX_RS0','R','security:classification','R'],
     ['ZSAX_RS0','C','security:classification','C'],
     ['ZSAX_RS0','S','security:classification','S'],
     ['ZSAX_RS0','TS','security:classification','TS'],

     // OTH Filler.  These are to build OTH values
     ['RTN_ROI','999','highway','other'],
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

     // Extra road stuff
     ['ZI016_ROC','9','surface','paved'], // Paved/Asphalt from OSM
     ['ZI016_ROC','2','surface','unpaved'], // Stabilized earth
     ['ZI016_ROC','1','surface','dirt'], // Unimproved 
     ['ZI016_ROC','1','surface','earth'], // Unimproved 

     // Fix up some of the Defaults
     // ['FFN','-999999','building','yes'], // Map to No Information
     // ['RRC','-999999','railway','rail'], // Map to No Information
     [undefined,undefined,'natural','water'], // to get rid of errors

     // From MGCP
     ['FFN','480','building','transportation_outbuilding'], // Transportation
     ['FFN','481','man_made','depot'], // not a good fit for depot terminal
     ['FFN','550','building','dwelling'], // Accomodation 
     ['FFN','811','building','public_service_outbuilding'], // Government - Not Good
     ['FFN','811','amenity','government_administration_building'],
     ['FFN','835','military','military_outbuilding'], // Military
     ['FFN','835','military','barracks'], // Military
     ['FFN','843','amenity','jail'], // Imprisonment
     ['FFN','980','building','cemetery_building'], // Death care services
     ['ZI014_PPO','-999999','product','unknown'], // Unknown
//      [undefined,undefined,'amenity','school'], //  converted in pre processing
//      [undefined,undefined,'amenity','hospital'], //  converted in pre processing
//      [undefined,undefined,'amenity','university'], //  converted in pre processing
//      [undefined,undefined,'amenity','college'], //  converted in pre processing
     [undefined,undefined,'amenity','house_of_worship'], //  converted in pre processing

     // From UFD
     ['CAA','-999999','controlling_authority','unknown'],
     ['FFN','190','use','oil_gas_facility'],

     // From UTP
     // ['TTC','999','tower:type','radar'], //  Radar Tower -> Other
     ['FFN','482','amenity','bus_station'], // Station
     ['FFN','831','amenity','courthouse'], // Public Order - from spec
     ['FFN','610','building','communication'],
     ['FFN','530','building','warehouse'],
     ['FFN','835','building','military'],
     // ['FFN','999','landuse','institutional'],
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
     ['FFN','465','shop','supermarket'], // Non-specialized
     ['FFN','563','building','house'], // Residence
     ['FFN','558','building','dependents_housing'], // Dependents Housing
     ['FFN','610','office','telecommunication'], // telecommunication
     ['FFN','640','shop','bureau_de_change'], // Financial Services
     ['FFN','760','amenity','office'], // Business and Personal Support Services 
     ['FFN','815','building','palace'], // Palace

   ], // End one2oneOut

    // ##### End of One2One Rules #####

   // ##### Start of txtLength #####
    // This list is for validateing the lengths of text attributes prior to export
    txtLength : {
     'BA000_VDR':80, 'BRN':24, 'CID':20, 'CPS':30, 'EQC':30, 'ETS':30, 'ETZ':24, 'GB052_RIDH':14, 'GB052_RIDL':14,
     'HZD':30, 'IC2':14, 'IKO':14, 'MDE':20, 'NA8':80, 'RCG':30, 'RTN2':24, 'RTN':24, 'RTN3':24, 'SSE':14, 'UFI':254,
     'URI':254, 'VDT':30, 'VOI':14, 'WPI':14, 'ZI001_NSD':20, 'ZI001_NSP':30, 'ZI001_SSD':20, 'ZI001_SSY':30, 'ZI001_VSC':30,
     'ZI001_VSD':20, 'ZI004_RCG':30, 'ZI005_FNA1':200, 'ZI005_FNA':200, 'ZI005_FNA2':200, 'ZI005_NFN1':18, 'ZI005_NFN':18,
     'ZI005_NFN2':18, 'ZSAX_RS0':2, 'ZSAX_RX3':254, 'ZSAX_RX4':254
    },
    // ##### End of txtLength #####

    // ##### Start of intList #####
    // This list is for validateing the integer attributes prior to export
    intList : [ 'BNF', 'DEV', 'DZC', 'LC1', 'LC2', 'LC3', 'LC4', 'LTN', 'NOS', 'NPL', 'VST' ],
    // ##### End of intList#####

    // ##### Start of Thematic Group Rules #####
    thematicGroupList : {
       'PGB230':'AeronauticPnt', // AircraftHangar
       'AGB230':'AeronauticSrf', // AircraftHangar
       'AGB015':'AeronauticSrf', // Apron
       'PAQ060':'AeronauticPnt', // ControlTower
       'AAQ060':'AeronauticSrf', // ControlTower
       'PGB250':'AeronauticPnt', // HardenedAircraftShelter
       'AGB250':'AeronauticSrf', // HardenedAircraftShelter
       'PGB030':'AeronauticPnt', // Helipad
       'AGB030':'AeronauticSrf', // Helipad
       'PGB035':'AeronauticPnt', // Heliport
       'AGB035':'AeronauticSrf', // Heliport
       'PGB005':'AeronauticPnt', // LandAerodrome
       'AGB005':'AeronauticSrf', // LandAerodrome
       'PGB040':'AeronauticPnt', // LaunchPad
       'AGB040':'AeronauticSrf', // LaunchPad
       'PAQ110':'AeronauticPnt', // MooringMast
       'AGB055':'AeronauticSrf', // Runway
       'AGB070':'AeronauticSrf', // SeaplaneRun
       'PAL351':'AeronauticPnt', // SpaceFacility
       'AAL351':'AeronauticSrf', // SpaceFacility
       'AGB045':'AeronauticSrf', // Stopway
       'LGB075':'AeronauticCrv', // Taxiway
       'AGB075':'AeronauticSrf', // Taxiway
       'PAL510':'AeronauticPnt', // TetheredBalloon
       'PGB065':'AeronauticPnt', // WaterAerodrome
       'AGB065':'AeronauticSrf', // WaterAerodrome
       'PAJ085':'AgriculturePnt', // Barn
       'AAJ085':'AgricultureSrf', // Barn
       'AEC010':'AgricultureSrf', // Cane
       'AEA010':'AgricultureSrf', // CropLand
       'PBH051':'AgriculturePnt', // FishFarmFacility
       'ABH051':'AgricultureSrf', // FishFarmFacility
       'PAM020':'AgriculturePnt', // GrainStorageStructure
       'AAM020':'AgricultureSrf', // GrainStorageStructure
       'PAJ110':'AgriculturePnt', // Greenhouse
       'AAJ110':'AgricultureSrf', // Greenhouse
       'PAJ030':'AgriculturePnt', // HoldingPen
       'AAJ030':'AgricultureSrf', // HoldingPen
       'AEA055':'AgricultureSrf', // HopField
       'PAL270':'AgriculturePnt', // IndustrialFarm
       'AAL270':'AgricultureSrf', // IndustrialFarm
       'AEA040':'AgricultureSrf', // Orchard
       'AEA030':'AgricultureSrf', // PlantNursery
       'ABH135':'AgricultureSrf', // RiceField
       'PAJ080':'AgriculturePnt', // Stable
       'AAJ080':'AgricultureSrf', // Stable
       'AEA050':'AgricultureSrf', // Vineyard
       'PAJ050':'AgriculturePnt', // Windmill
       'AAJ050':'AgricultureSrf', // Windmill
       'PZB030':'BoundaryPnt', // BoundaryMonument
       'LFA110':'BoundaryCrv', // InternationalDateLine
       'AZI040':'CollectionSrf', // Spatial Entity Metadata Collection
       'AZI041':'CollectionSrf', // Non-Spatial Entity Metadata Collection
       'PAL012':'CulturePnt', // ArcheologicalSite
       'AAL012':'CultureSrf', // ArcheologicalSite
       'AEA031':'CultureSrf', // BotanicGarden
       'PAL025':'CulturePnt', // Cairn
       'PAL030':'CulturePnt', // Cemetery
       'AAL030':'CultureSrf', // Cemetery
       'AFA210':'CultureSrf', // ConservationArea
       'PFA012':'CulturePnt', // ContaminatedRegion
       'AFA012':'CultureSrf', // ContaminatedRegion
       'AAL175':'CultureSrf', // Courtyard
       'PZD030':'CulturePnt', // CulturalContextLocation
       'AZD030':'CultureSrf', // CulturalContextLocation
       'PBH075':'CulturePnt', // Fountain
       'ABH075':'CultureSrf', // Fountain
       'PAL201':'CulturePnt', // InterestSite
       'AAL201':'CultureSrf', // InterestSite
       'PAK121':'CulturePnt', // Lookout
       'AAK121':'CultureSrf', // Lookout
       'PAL130':'CulturePnt', // MemorialMonument
       'LAL130':'CultureCrv', // MemorialMonument
       'AAL130':'CultureSrf', // MemorialMonument
       'AAK120':'CultureSrf', // Park
       'AAL180':'CultureSrf', // RetailStand
       'PAL200':'CulturePnt', // Ruins
       'AAL200':'CultureSrf', // Ruins
       'PAL036':'CulturePnt', // Tomb
       'AAL036':'CultureSrf', // Tomb
       'AZI031':'DatasetSrf', // Dataset Metadata
       'PAL010':'FacilityPnt', // Facility
       'AAL010':'FacilitySrf', // Facility
       'PAL011':'FacilityPnt', // Installation
       'AAL011':'FacilitySrf', // Installation
       'AAG040':'FacilitySrf', // OfficePark
       'PAT045':'FacilityPnt', // RadarStation
       'AAT045':'FacilitySrf', // RadarStation
       'AAG030':'FacilitySrf', // ShoppingComplex
       'PBC050':'HydroAidNavigationPnt', // Lighthouse
       'ABC050':'HydroAidNavigationSrf', // Lighthouse
       'PBC070':'HydroAidNavigationPnt', // LightVessel
       'LBH010':'HydrographyCrv', // Aqueduct
       'ABH010':'HydrographySrf', // Aqueduct
       'ABH116':'HydrographySrf', // Aquifer
       'PBI010':'HydrographyPnt', // Cistern
       'PBI020':'HydrographyPnt', // Dam
       'LBI020':'HydrographyCrv', // Dam
       'ABI020':'HydrographySrf', // Dam
       'LBH030':'HydrographyCrv', // Ditch
       'ABH030':'HydrographySrf', // Ditch
       'LBI060':'HydrographyCrv', // FishLadder
       'PBB110':'HydrographyPnt', // FishWeir
       'PBI044':'HydrographyPnt', // FloodControlStructure
       'LBI044':'HydrographyCrv', // FloodControlStructure
       'ABI044':'HydrographySrf', // FloodControlStructure
       'PBI070':'HydrographyPnt', // GaugingStation
       'PBD181':'HydrographyPnt', // Hulk
       'PBH082':'HydrographyPnt', // InlandWaterbody
       'ABH082':'HydrographySrf', // InlandWaterbody
       'ABH090':'HydrographySrf', // LandSubjectToInundation
       'LBH100':'HydrographyCrv', // Moat
       'ABH100':'HydrographySrf', // Moat
       'PBH170':'HydrographyPnt', // NaturalPool
       'ABH170':'HydrographySrf', // NaturalPool
       'PBD115':'HydrographyPnt', // OffshoreConstruction
       'ABD115':'HydrographySrf', // OffshoreConstruction
       'LBH110':'HydrographyCrv', // Penstock
       'PBH012':'HydrographyPnt', // QanatShaft
       'PBH120':'HydrographyPnt', // Rapids
       'LBH120':'HydrographyCrv', // Rapids
       'ABH120':'HydrographySrf', // Rapids
       'LBH140':'HydrographyCrv', // River
       'ABH140':'HydrographySrf', // River
       'PBI040':'HydrographyPnt', // SluiceGate
       'LBI040':'HydrographyCrv', // SluiceGate
       'PBD140':'HydrographyPnt', // Snag
       'LBH165':'HydrographyCrv', // Spillway
       'ABH165':'HydrographySrf', // Spillway
       'ABA040':'HydrographySrf', // TidalWater
       'PBH145':'HydrographyPnt', // VanishingPoint
       'PBH180':'HydrographyPnt', // Waterfall
       'LBH180':'HydrographyCrv', // Waterfall
       'PBI050':'HydrographyPnt', // WaterIntakeTower
       'ABI050':'HydrographySrf', // WaterIntakeTower
       'LBH065':'HydrographyCrv', // WaterRace
       'PBH230':'HydrographyPnt', // WaterWell
       'ABH230':'HydrographySrf', // WaterWell
       'PZB035':'HypsographyPnt', // ControlPoint
       'LCA010':'HypsographyCrv', // ElevationContour
       'PCA030':'HypsographyPnt', // SpotElevation
       'AAB040':'IndustrySrf', // AerationBasin
       'PAC010':'IndustryPnt', // BlastFurnace
       'AAC010':'IndustrySrf', // BlastFurnace
       'PAA045':'IndustryPnt', // Borehole
       'PAF021':'IndustryPnt', // BucketElevator
       'LAF021':'IndustryCrv', // BucketElevator
       'PAC020':'IndustryPnt', // CatalyticCracker
       'AAC020':'IndustrySrf', // CatalyticCracker
       'LAF020':'IndustryCrv', // Conveyor
       'PAF040':'IndustryPnt', // Crane
       'AAF040':'IndustrySrf', // Crane
       'PAB000':'IndustryPnt', // DisposalSite
       'AAB000':'IndustrySrf', // DisposalSite
       'PAF050':'IndustryPnt', // ExcavatingMachine
       'LAF050':'IndustryCrv', // ExcavatingMachine
       'PAA010':'IndustryPnt', // ExtractionMine
       'AAA010':'IndustrySrf', // ExtractionMine
       'PAF070':'IndustryPnt', // FlarePipe
       'PAF080':'IndustryPnt', // Hopper
       'PAC040':'IndustryPnt', // HydrocarbonProdFacility
       'AAC040':'IndustrySrf', // HydrocarbonProdFacility
       'AAA052':'IndustrySrf', // HydrocarbonsField
       'PAC060':'IndustryPnt', // IndustrialFurnace
       'AAC060':'IndustrySrf', // IndustrialFurnace
       'PAM040':'IndustryPnt', // MineralPile
       'AAM040':'IndustrySrf', // MineralPile
       'PAA020':'IndustryPnt', // MineShaftSuperstructure
       'AAA020':'IndustrySrf', // MineShaftSuperstructure
       'PAA054':'IndustryPnt', // NonWaterWell
       'AAB010':'IndustrySrf', // RecyclingSite
       'PAA040':'IndustryPnt', // Rig
       'AAA040':'IndustrySrf', // Rig
       'PBH155':'IndustryPnt', // SaltEvaporator
       'ABH155':'IndustrySrf', // SaltEvaporator
       'AAC030':'IndustrySrf', // SettlingPond
       'LAA011':'IndustryCrv', // ShearWall
       'PAJ055':'IndustryPnt', // WaterMill
       'AAJ055':'IndustrySrf', // WaterMill
       'ABH040':'IndustrySrf', // WaterTreatmentBed
       'PZD045':'InformationPnt', // AnnotatedLocation
       'LZD045':'InformationCrv', // AnnotatedLocation
       'AZD045':'InformationSrf', // AnnotatedLocation
       'PZD040':'InformationPnt', // NamedLocation
       'LZD040':'InformationCrv', // NamedLocation
       'AZD040':'InformationSrf', // NamedLocation
       'AZD020':'InformationSrf', // VoidCollectionArea
       'LGB050':'MilitaryCrv', // AircraftRevetment
       'ASU004':'MilitarySrf', // CantonmentArea
       'PAL375':'MilitaryPnt', // Castle
       'AAL375':'MilitarySrf', // Castle
       'PAL376':'MilitaryPnt', // CastleComplex
       'AAL376':'MilitarySrf', // CastleComplex
       'LAL060':'MilitaryCrv', // DragonsTeeth
       'AAL060':'MilitarySrf', // DragonsTeeth
       'LAH025':'MilitaryCrv', // EngineeredEarthwork
       'AAH025':'MilitarySrf', // EngineeredEarthwork
       'PFA015':'MilitaryPnt', // FiringRange
       'AFA015':'MilitarySrf', // FiringRange
       'PAH055':'MilitaryPnt', // FortifiedBuilding
       'AAH055':'MilitarySrf', // FortifiedBuilding
       'PSU001':'MilitaryPnt', // MilitaryInstallation
       'ASU001':'MilitarySrf', // MilitaryInstallation
       'AAL065':'MilitarySrf', // Minefield
       'PAL120':'MilitaryPnt', // MissileSite
       'AAL120':'MilitarySrf', // MissileSite
       'PAM060':'MilitaryPnt', // SurfaceBunker
       'AAM060':'MilitarySrf', // SurfaceBunker
       'AFA100':'MilitarySrf', // TestSite
       'PFA165':'MilitaryPnt', // TrainingSite
       'AFA165':'MilitarySrf', // TrainingSite
       'PAH060':'MilitaryPnt', // UndergroundBunker
       'AAH060':'MilitarySrf', // UndergroundBunker
       'ADA005':'PhysiographySrf', // AsphaltLake
       'PDB029':'PhysiographyPnt', // CaveMouth
       'LBJ031':'PhysiographyCrv', // Crevasse
       'ABJ031':'PhysiographySrf', // Crevasse
       'LDB061':'PhysiographyCrv', // Crevice
       'ADB061':'PhysiographySrf', // Crevice
       'LDB070':'PhysiographyCrv', // Cut
       'LDB071':'PhysiographyCrv', // CutLine
       'ADB080':'PhysiographySrf', // Depression
       'AEE030':'PhysiographySrf', // Desert
       'LDB090':'PhysiographyCrv', // Embankment
       'ADB090':'PhysiographySrf', // Embankment
       'LDB100':'PhysiographyCrv', // Esker
       'LDB110':'PhysiographyCrv', // GeologicFault
       'PDB115':'PhysiographyPnt', // GeothermalOutlet
       'ADB115':'PhysiographySrf', // GeothermalOutlet
       'ABJ030':'PhysiographySrf', // Glacier
       'ABJ099':'PhysiographySrf', // IceCap
       'LBJ040':'PhysiographyCrv', // IceCliff
       'PBJ060':'PhysiographyPnt', // IcePeak
       'ABJ065':'PhysiographySrf', // IceShelf
       'PBA030':'PhysiographyPnt', // Island
       'ABA030':'PhysiographySrf', // Island
       'ADB211':'PhysiographySrf', // LandslideMass
       'LBA010':'PhysiographyCrv', // LandWaterBoundary
       'ABJ020':'PhysiographySrf', // Moraine
       'PDB150':'PhysiographyPnt', // MountainPass
       'PEC020':'PhysiographyPnt', // Oasis
       'AEC020':'PhysiographySrf', // Oasis
       'ABJ080':'PhysiographySrf', // PolarIce
       'PDB160':'PhysiographyPnt', // RockFormation
       'ADB160':'PhysiographySrf', // RockFormation
       'ABH160':'PhysiographySrf', // Sabkha
       'ABH150':'PhysiographySrf', // SaltFlat
       'ADB170':'PhysiographySrf', // SandDunes
       'ABJ100':'PhysiographySrf', // SnowIceField
       'ADA010':'PhysiographySrf', // SoilSurfaceRegion
       'LDB010':'PhysiographyCrv', // SteepTerrainFace
       'LDB190':'PhysiographyCrv', // VolcanicDyke
       'PDB180':'PhysiographyPnt', // Volcano
       'ABB090':'PortHarbourSrf', // DryDock
       'ABB199':'PortHarbourSrf', // FloatingDryDock
       'ABB005':'PortHarbourSrf', // Harbour
       'PBB009':'PortHarbourPnt', // Port
       'ABB009':'PortHarbourSrf', // Port
       'PBB241':'PortHarbourPnt', // Shipyard
       'ABB241':'PortHarbourSrf', // Shipyard
       'LBB081':'PortHarbourCrv', // ShorelineConstruction
       'ABB081':'PortHarbourSrf', // ShorelineConstruction
       'LBB082':'PortHarbourCrv', // ShorelineRamp
       'ABB082':'PortHarbourSrf', // ShorelineRamp
       'PBB201':'PortHarbourPnt', // SmallCraftFacility
       'ABB201':'PortHarbourSrf', // SmallCraftFacility
       'PBD100':'PortHarbourPnt', // StructuralPile
       'ABD100':'PortHarbourSrf', // StructuralPile
       'ABI005':'PortHarbourSrf', // VesselLift
       'PAK164':'RecreationPnt', // Amphitheatre
       'AAK164':'RecreationSrf', // Amphitheatre
       'PAK030':'RecreationPnt', // AmusementPark
       'AAK030':'RecreationSrf', // AmusementPark
       'PAK020':'RecreationPnt', // AmusementParkAttraction
       'LAK020':'RecreationCrv', // AmusementParkAttraction
       'AAK020':'RecreationSrf', // AmusementParkAttraction
       'PAK060':'RecreationPnt', // CampSite
       'AAK060':'RecreationSrf', // CampSite
       'AAK070':'RecreationSrf', // DriveInTheatre
       'AAK090':'RecreationSrf', // Fairground
       'AAK100':'RecreationSrf', // GolfCourse
       'AAK101':'RecreationSrf', // GolfDrivingRange
       'PAK110':'RecreationPnt', // Grandstand
       'AAK110':'RecreationSrf', // Grandstand
       'PAK080':'RecreationPnt', // OutdoorTheatreScreen
       'LAK080':'RecreationCrv', // OutdoorTheatreScreen
       'AAK061':'RecreationSrf', // PicnicSite
       'AAL170':'RecreationSrf', // PublicSquare
       'LAK130':'RecreationCrv', // Racetrack
       'AAK130':'RecreationSrf', // Racetrack
       'PAK161':'RecreationPnt', // Scoreboard
       'PAK150':'RecreationPnt', // SkiJump
       'LAK150':'RecreationCrv', // SkiJump
       'LAK155':'RecreationCrv', // SkiRun
       'AAK155':'RecreationSrf', // SkiRun
       'PAK040':'RecreationPnt', // SportsGround
       'AAK040':'RecreationSrf', // SportsGround
       'PAK160':'RecreationPnt', // Stadium
       'AAK160':'RecreationSrf', // Stadium
       'PAK170':'RecreationPnt', // SwimmingPool
       'AAK170':'RecreationSrf', // SwimmingPool
       'PAK180':'RecreationPnt', // Zoo
       'AAK180':'RecreationSrf', // Zoo
       'PAL020':'SettlementPnt', // BuiltUpArea
       'AAL020':'SettlementSrf', // BuiltUpArea
       'PAI030':'SettlementPnt', // Camp
       'AAI030':'SettlementSrf', // Camp
       'AAI020':'SettlementSrf', // CaravanPark
       'AAI021':'SettlementSrf', // MobileHomePark
       'PAL105':'SettlementPnt', // Settlement
       'AAL105':'SettlementSrf', // Settlement
       'PAL208':'SettlementPnt', // ShantyTown
       'AAL208':'SettlementSrf', // ShantyTown
       'PAM075':'StoragePnt', // FuelStorageFacility
       'AAM075':'StorageSrf', // FuelStorageFacility
       'PAM030':'StoragePnt', // GrainElevator
       'AAM030':'StorageSrf', // GrainElevator
       'PAM065':'StoragePnt', // MunitionStorageFacility
       'AAM065':'StorageSrf', // MunitionStorageFacility
       'PAM011':'StoragePnt', // ShippingContainer
       'AAM011':'StorageSrf', // ShippingContainer
       'PAM010':'StoragePnt', // StorageDepot
       'AAM010':'StorageSrf', // StorageDepot
       'PAM070':'StoragePnt', // StorageTank
       'AAM070':'StorageSrf', // StorageTank
       'PAM071':'StoragePnt', // TankFarm
       'AAM071':'StorageSrf', // TankFarm
       'PAM080':'StoragePnt', // WaterTower
       'AAM080':'StorageSrf', // WaterTower
       'PAL142':'StructurePnt', // AstronomicalObservatory
       'AAL142':'StructureSrf', // AstronomicalObservatory
       'PAG050':'StructurePnt', // Billboard
       'PAL013':'StructurePnt', // Building
       'AAL013':'StructureSrf', // Building
       'PAL018':'StructurePnt', // BuildingSuperstructure
       'LAL018':'StructureCrv', // BuildingSuperstructure
       'AAL018':'StructureSrf', // BuildingSuperstructure
       'PAF060':'StructurePnt', // EngineTestCell
       'AAF060':'StructureSrf', // EngineTestCell
       'LAL070':'StructureCrv', // Fence
       'PAL073':'StructurePnt', // Flagpole
       'PAL080':'StructurePnt', // Gantry
       'LAL080':'StructureCrv', // Gantry
       'PAL099':'StructurePnt', // Hut
       'AAL099':'StructureSrf', // Hut
       'PAL110':'StructurePnt', // LightSupportStructure
       'PAL014':'StructurePnt', // NonBuildingStructure
       'AAL014':'StructureSrf', // NonBuildingStructure
       'LAL140':'StructureCrv', // ParticleAccelerator
       'AAL140':'StructureSrf', // ParticleAccelerator
       'PAL019':'StructurePnt', // Shed
       'AAL019':'StructureSrf', // Shed
       'LAQ150':'StructureCrv', // Stair
       'AAQ150':'StructureSrf', // Stair
       'PAL241':'StructurePnt', // Tower
       'AAL241':'StructureSrf', // Tower
       'PAL250':'StructurePnt', // UndergroundDwelling
       'LAL260':'StructureCrv', // Wall
       'LAQ151':'TransportationGroundCrv', // Arcade
       'AAQ151':'TransportationGroundSrf', // Arcade
       'PAQ040':'TransportationGroundPnt', // Bridge
       'LAQ040':'TransportationGroundCrv', // Bridge
       'AAQ040':'TransportationGroundSrf', // Bridge
       'PAQ056':'TransportationGroundPnt', // BridgePier
       'LAQ056':'TransportationGroundCrv', // BridgePier
       'AAQ056':'TransportationGroundSrf', // BridgePier
       'PAQ045':'TransportationGroundPnt', // BridgeSpan
       'LAQ045':'TransportationGroundCrv', // BridgeSpan
       'AAQ045':'TransportationGroundSrf', // BridgeSpan
       'LAQ050':'TransportationGroundCrv', // BridgeSuperstructure
       'AAQ050':'TransportationGroundSrf', // BridgeSuperstructure
       'PAQ055':'TransportationGroundPnt', // BridgeTower
       'LAT041':'TransportationGroundCrv', // Cableway
       'LAP010':'TransportationGroundCrv', // CartTrack
       'LAQ063':'TransportationGroundCrv', // CausewayStructure
       'AAQ063':'TransportationGroundSrf', // CausewayStructure
       'PAH070':'TransportationGroundPnt', // Checkpoint
       'PAQ062':'TransportationGroundPnt', // Crossing
       'PAQ065':'TransportationGroundPnt', // Culvert
       'LAQ065':'TransportationGroundCrv', // Culvert
       'PBH070':'TransportationGroundPnt', // Ford
       'LBH070':'TransportationGroundCrv', // Ford
       'ABH070':'TransportationGroundSrf', // Ford
       'PAP040':'TransportationGroundPnt', // Gate
       'LAP040':'TransportationGroundCrv', // Gate
       'LAQ075':'TransportationGroundCrv', // IceRoute
       'PAQ170':'TransportationGroundPnt', // MotorVehicleStation
       'AAQ170':'TransportationGroundSrf', // MotorVehicleStation
       'PAL155':'TransportationGroundPnt', // OverheadObstruction
       'LAL155':'TransportationGroundCrv', // OverheadObstruction
       'PAQ141':'TransportationGroundPnt', // ParkingGarage
       'AAQ141':'TransportationGroundSrf', // ParkingGarage
       'PAL165':'TransportationGroundPnt', // PipelineCrossingPoint
       'LAN010':'TransportationGroundCrv', // Railway
       'LAN050':'TransportationGroundCrv', // RailwaySidetrack
       'PAN085':'TransportationGroundPnt', // RailwaySignal
       'PAN075':'TransportationGroundPnt', // RailwayTurntable
       'AAN075':'TransportationGroundSrf', // RailwayTurntable
       'AAN060':'TransportationGroundSrf', // RailwayYard
       'LAL195':'TransportationGroundCrv', // Ramp
       'AAL195':'TransportationGroundSrf', // Ramp
       'LAP030':'TransportationGroundCrv', // Road
       'AAP030':'TransportationGroundSrf', // Road
       'PAP020':'TransportationGroundPnt', // RoadInterchange
       'PAQ135':'TransportationGroundPnt', // RoadsideRestArea
       'AAQ135':'TransportationGroundSrf', // RoadsideRestArea
       'PAN076':'TransportationGroundPnt', // Roundhouse
       'AAN076':'TransportationGroundSrf', // Roundhouse
       'PAQ118':'TransportationGroundPnt', // SharpCurve
       'LAQ035':'TransportationGroundCrv', // Sidewalk
       'LAQ120':'TransportationGroundCrv', // SteepGrade
       'PAQ161':'TransportationGroundPnt', // StreetLamp
       'PAQ162':'TransportationGroundPnt', // StreetSign
       'PAQ160':'TransportationGroundPnt', // TrafficLight
       'LAP050':'TransportationGroundCrv', // Trail
       'PAQ068':'TransportationGroundPnt', // TransportationBlock
       'AAQ068':'TransportationGroundSrf', // TransportationBlock
       'PAQ125':'TransportationGroundPnt', // TransportationStation
       'AAQ125':'TransportationGroundSrf', // TransportationStation
       'PAQ059':'TransportationGroundPnt', // TransRouteCharacterChange
       'LAQ059':'TransportationGroundCrv', // TransRouteCharacterChange
       'PAL211':'TransportationGroundPnt', // TransRouteProtectStruct
       'LAL211':'TransportationGroundCrv', // TransRouteProtectStruct
       'AAL211':'TransportationGroundSrf', // TransRouteProtectStruct
       'LAQ130':'TransportationGroundCrv', // Tunnel
       'AAQ130':'TransportationGroundSrf', // Tunnel
       'PAQ095':'TransportationGroundPnt', // TunnelMouth
       'PAP041':'TransportationGroundPnt', // VehicleBarrier
       'LAP041':'TransportationGroundCrv', // VehicleBarrier
       'AAQ140':'TransportationGroundSrf', // VehicleLot
       'PAQ180':'TransportationGroundPnt', // WeighStation
       'AAQ180':'TransportationGroundSrf', // WeighStation
       'PBI045':'TransportationWaterPnt', // BasinGate
       'LBI045':'TransportationWaterCrv', // BasinGate
       'LBH020':'TransportationWaterCrv', // Canal
       'ABH020':'TransportationWaterSrf', // Canal
       'LAQ070':'TransportationWaterCrv', // FerryCrossing
       'PAQ080':'TransportationWaterPnt', // FerryStation
       'AAQ080':'TransportationWaterSrf', // FerryStation
       'PBI030':'TransportationWaterPnt', // Lock
       'LBI030':'TransportationWaterCrv', // Lock
       'ABI030':'TransportationWaterSrf', // Lock
       'PAQ111':'TransportationWaterPnt', // PreparedWatercourseCross
       'LAQ111':'TransportationWaterCrv', // PreparedWatercourseCross
       'PBI006':'TransportationWaterPnt', // ShipElevator
       'LBI006':'TransportationWaterCrv', // ShipElevator
       'ABI006':'TransportationWaterSrf', // ShipElevator
       'PAT011':'UtilityInfrastructurePnt', // Aerial
       'PAT012':'UtilityInfrastructurePnt', // AerialFarm
       'AAT012':'UtilityInfrastructureSrf', // AerialFarm
       'LAT005':'UtilityInfrastructureCrv', // Cable
       'PAD055':'UtilityInfrastructurePnt', // CoolingFacility
       'AAD055':'UtilityInfrastructureSrf', // CoolingFacility
       'PAF030':'UtilityInfrastructurePnt', // CoolingTower
       'AAF030':'UtilityInfrastructureSrf', // CoolingTower
       'PAT010':'UtilityInfrastructurePnt', // DishAerial
       'PAD010':'UtilityInfrastructurePnt', // ElectricPowerStation
       'AAD010':'UtilityInfrastructureSrf', // ElectricPowerStation
       'PAL017':'UtilityInfrastructurePnt', // FireHydrant
       'PAD050':'UtilityInfrastructurePnt', // HeatingFacility
       'AAD050':'UtilityInfrastructureSrf', // HeatingFacility
       'PAB021':'UtilityInfrastructurePnt', // LiquidDiffuser
       'PAD041':'UtilityInfrastructurePnt', // NuclearReactorContainment
       'AAD041':'UtilityInfrastructureSrf', // NuclearReactorContainment
       'LAQ113':'UtilityInfrastructureCrv', // Pipeline
       'PAD030':'UtilityInfrastructurePnt', // PowerSubstation
       'AAD030':'UtilityInfrastructureSrf', // PowerSubstation
       'PAQ116':'UtilityInfrastructurePnt', // PumpingStation
       'AAQ116':'UtilityInfrastructureSrf', // PumpingStation
       'PAT042':'UtilityInfrastructurePnt', // Pylon
       'PAC507':'UtilityInfrastructurePnt', // SewageTreatmentPlant
       'AAC507':'UtilityInfrastructureSrf', // SewageTreatmentPlant
       'PAF010':'UtilityInfrastructurePnt', // Smokestack
       'PAD025':'UtilityInfrastructurePnt', // SolarFarm
       'AAD025':'UtilityInfrastructureSrf', // SolarFarm
       'PAD020':'UtilityInfrastructurePnt', // SolarPanel
       'AAD020':'UtilityInfrastructureSrf', // SolarPanel
       'PAQ114':'UtilityInfrastructurePnt', // StormDrain
       'PAQ115':'UtilityInfrastructurePnt', // UtilityCover
       'PBH220':'UtilityInfrastructurePnt', // Waterwork
       'ABH220':'UtilityInfrastructureSrf', // Waterwork
       'PAD060':'UtilityInfrastructurePnt', // WindFarm
       'AAD060':'UtilityInfrastructureSrf', // WindFarm
       'PAJ051':'UtilityInfrastructurePnt', // WindTurbine
       'ABH015':'VegetationSrf', // Bog
       'AEB070':'VegetationSrf', // Brush
       'LEC040':'VegetationCrv', // ClearedWay
       'AEC040':'VegetationSrf', // ClearedWay
       'LEC015':'VegetationCrv', // Forest
       'AEC015':'VegetationSrf', // Forest
       'AEC060':'VegetationSrf', // ForestClearing
       'AEB010':'VegetationSrf', // Grassland
       'LEA020':'VegetationCrv', // Hedgerow
       'ABH077':'VegetationSrf', // Hummock
       'AEE010':'VegetationSrf', // LoggingSite
       'AED010':'VegetationSrf', // Marsh
       'AED020':'VegetationSrf', // Swamp
       'AEB020':'VegetationSrf', // Thicket
       'PEC005':'VegetationPnt', // Tree
       'ABJ110':'VegetationSrf', // Tundra
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
       'AF021':'100027', // Bucket Elevator
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
       'AI021':'133168', // Mobile Home Park
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
       'AP040':'100154', // Gate
       'AP041':'100155', // Vehicle Barrier
       'AP050':'100156', // Trail
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
       'AQ180':'100198', // Weigh Station
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
       'FA110':'100415', // International Date Line
       'FA165':'100416', // Training Site
       'FA210':'100417', // Conservation Area
       'GB005':'100436', // Land Aerodrome
       'GB015':'100438', // Apron
       'GB030':'100441', // Helipad
       'GB035':'100442', // Heliport
       'GB040':'100443', // Launch Pad
       'GB045':'100444', // Stopway
       'GB050':'100446', // Aircraft Revetment
       'GB055':'100448', // Runway
       'GB065':'100452', // Water Aerodrome
       'GB070':'100453', // Seaplane Run
       'GB075':'100454', // Taxiway
       'GB230':'100456', // Aircraft Hangar
       'GB250':'100457', // Hardened Aircraft Shelter
       'SU001':'100462', // Military Installation
       'SU004':'170162', // Cantonment Area
       'ZB030':'100465', // Boundary Monument
       'ZB035':'100466', // Control Point
       'ZD020':'100473', // Void Collection Area
       'ZD030':'100474', // Cultural Context Location
       'ZD040':'100475', // Named Location
       'ZD045':'100476', // Annotated Location
       'ZI031':'121591', // Dataset
       'ZI040':'133102', // Non Spatial Metadata Entity Collection
       'ZI041':'133101', // Spatial Metadata Entity Collection
       }, // End of subtypeList
    // ##### End of ESRI FCSubtype Rules #####

} // End of tds.rules
