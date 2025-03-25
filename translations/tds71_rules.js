//  * This file is part of Hootenanny.
//  *
//  * Hootenanny is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  *
//  * --------------------------------------------------------------------
//  *
//  * The following copyright notices are generated automatically. If you
//  * have a new notice to add, please use the format:
//  * " * @copyright Copyright ..."
//  * This will properly maintain the copyright information. DigitalGlobe
//  * copyrights will be updated automatically.
//  *
//  * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)


// /*
//     TDSv71 One2one rules
// */

var _global = (0, eval)('this');
if (!_global.schemaTools)
{
  hoot.require('SchemaTools');
}


tds71.rules = {
  // ##### Start of One2One Rules #####

  // FCODE rules for Import
  // This is where anything "unique" to TDS goes.
  fcodeOne2oneIn : [
    ['F_CODE','FA091','geophysical_data_track','yes'], // Geophysical Data Track Line
    ['F_CODE','FLWAR','flow_arrow','yes'], // Flow Arrow - No F_CODE
    ['F_CODE','GA005','airspace','yes'], // Airspace
    ['F_CODE','GA035','navaid:aeronautical','yes'], // Aeronautical Navaids
    ['F_CODE','GA036','navaid:beacon','vor'], // Vhf Omni Radio Beacon
    ['F_CODE','GA037','navaid:beacon','tacan'], // Tactical Air Navigation Aid Beacon
    ['F_CODE','GA038','navaid:beacon','ndb'], // Nondirectional Radio Beacon
    ['F_CODE','GA041','navaid:beacon','dme'], // Distance Measuring Equipment
    ['F_CODE','LDRLN','leader_line','yes'], // Leader Line - No F_CODE
    ['F_CODE','MXELE','maximum_elevation','yes'], // Maximum Elevation - No F_CODE
    ['F_CODE','RDMKR','road_marker','yes'], // Road Marker - No F_CODE
    ['F_CODE','UNSCW','unescow_point','yes'], // Unescow - No F_CODE
    ['F_CODE','VOBST','vertical_obstruction','yes'], // Vertical Obstruction - No F_CODE
    ['F_CODE','ZC050','isogonic_line','yes'], // Isogonic Line
    ['F_CODE','ZD015','point_of_change','yes'], // Point Of Change

  ], // End fcodeOne2oneIn

  // These are for finding an FCODE for export.
  // Note: These get swapped around before being used
  fcodeOne2oneOut : [
    ['F_CODE','DA010','natural','desert'], // EE030 Desert - mapped to DA010 Soil Surface Region
    ['F_CODE','DA010','landuse','brownfield'], // DA010 Soil Surface Region
    ['F_CODE','DA010','natural','beach'], // Beach
    ['F_CODE','FA091','geophysical_data_track','yes'], // Geophysical Data Track Line
    ['F_CODE','GA005','airspace','yes'], // Airspace
    ['F_CODE','GA035','navaid:aeronautical','yes'], // Aeronautical Navaids
    ['F_CODE','GA036','navaid:beacon','vor'], // Vhf Omni Radio Beacon
    ['F_CODE','GA037','navaid:beacon','tacan'], // Tactical Air Navigation Aid Beacon
    ['F_CODE','GA038','navaid:beacon','ndb'], // Nondirectional Radio Beacon
    ['F_CODE','GA041','navaid:beacon','dme'], // Distance Measuring Equipment
    ['F_CODE','ZC050','isogonic_line','yes'], // Isogonic Line
    ['F_CODE','ZD015','point_of_change','yes'], // Point Of Change
    ['F_CODE','FLWAR','flow_arrow','yes'], // Flow Arrow - No F_CODE
    ['F_CODE','LDRLN','leader_line','yes'], // Leader Line - No F_CODE
    ['F_CODE','MXELE','maximum_elevation','yes'], // Maximum Elevation - No F_CODE
    ['F_CODE','RDMKR','road_marker','yes'], // Road Marker - No F_CODE
    ['F_CODE','UNSCW','unescow_point','yes'], // Unescow - No F_CODE
    ['F_CODE','VOBST','vertical_obstruction','yes'], // Vertical Obstruction - No F_CODE
    //     ['F_CODE','AN060','railway','yes'], // Rail yards
    // ['F_CODE','EA010','landuse','farm'], // Alt version of "landuse=farmland"
    // ['F_CODE','EC015','natural','wood'], // Alt version of Forest
    ['F_CODE','AA010','landuse','quarry'], // Extraction Mine
    ['F_CODE','AA054','man_made','petroleum_well'], // Non-Water Well
    // ['F_CODE','AD010','power','plant'], // Electric Power Station, Power Plant
    ['F_CODE','AJ110','building','greenhouse_horticulture'], // Greenhouse
    ['F_CODE','AK030','leisure','water_park'], // Amusement Park
    ['F_CODE','AK040','landuse','recreation_ground'], // Park
    ['F_CODE','AK120','landuse','village_green'], // Park
    ['F_CODE','AK120','leisure','common'], // Park - close but not great
    ['F_CODE','AK120','leisure','nature_reserve'], // Park
    ['F_CODE','AK120','leisure','playground'], // Park
    ['F_CODE','AK170','amenity','swimming_pool'], // Pool
    ['F_CODE','AL010','facility','processing'], // From MGCP - Map to Facility
    ['F_CODE','AL020','landuse','commercial'], // From OSM - Map to BUA
    ['F_CODE','AL020','landuse','industrial'], // From OSM - Map to BUA
    ['F_CODE','AL020','landuse','residential'], // From OSM - Map to BUA
    ['F_CODE','AL030','amenity','grave_yard'], // From OSM Data
    ['F_CODE','AL130','historic','memorial'], // From OSM data, not great
    ['F_CODE','AL200','ruins','yes'], // Ruins
    ['F_CODE','AL241','tower','communication'],
    ['F_CODE','AL241','tower','yes'],
    ['F_CODE','AL260','barrier','city_wall'], // Wall
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
    // ['F_CODE','AP040','railway','crossing'], // Pedestrian Crossing - See custom rules
    // ['F_CODE','AP040','railway','level_crossing'], // Level Crossing - See custom rules
    ['F_CODE','AP050','highway','footway'], // Trail
    ['F_CODE','AQ060','control_tower','yes'], // Control Tower
    ['F_CODE','AQ062','railway','crossing'],
    ['F_CODE','AQ062','railway','level_crossing'],
    ['F_CODE','AQ095','man_made','adit'], // Tunnel Mouth
    ['F_CODE','AQ125','amenity','bus_station'],
    ['F_CODE','AQ125','highway','bus_station'], // Transportaion Station
    ['F_CODE','AQ125','highway','bus_stop'], // Transportaion Station
    ['F_CODE','AQ125','public_transport','stop_position'],
    ['F_CODE','AQ125','railway','station'], // Railway Station
    ['F_CODE','AQ125','railway','halt'], // A type of stop at a Railway Station
    ['F_CODE','AQ141','parking','multi-story'], // Parking Garage
    ['F_CODE','AT042','power','pole'], // OSM
    ['F_CODE','AT042','power','tower'], // OSM
    ['F_CODE','BA010','natural','land_water_boundary'], // Land/Water Boundary - old translation
    ['F_CODE','BA030','place','islet'], // Island - From OSM
    ['F_CODE','BA040','water','tidal'], // Tidal Water
    ['F_CODE','BH082','natural','waterhole'], // Water-hole
    ['F_CODE','BH135','landuse','rice_field'], // Rice Field - tag change
    ['F_CODE','BH140','waterway','yes'], // Make unknown waterways into rivers
    ['F_CODE','BH140','waterway','stream'], // Make unknown waterways into rivers
    ['F_CODE','BH145','natural','sinkhole'],
    ['F_CODE','BH155','man_made','salt_pond'], // Due to the default translation changing
    ['F_CODE','DB090','embankment','mound'], // Embankment
    ['F_CODE','DB090','embankment','berm'],
    ['F_CODE','DB090','man_made','dyke'],
    ['F_CODE','DB090','embankment','fill'],
    ['F_CODE','DB090','embankment','levee'],
    ['F_CODE','DB090','embankment','divider'],
    ['F_CODE','EA055','landuse','hop_garden'], // Hop Field
    ['F_CODE','EB070','natural','heath'], // Brush
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
    'AIA':'airspace_identifier', // Airspace Identifier
    'BA000_VDR':'source:datum:sounding:name', // Water Line : Sounding Datum Name
    'BEN':'be_number', // Basic Encyclopedia (BE) Number
    'BRN':'bridge:reference', // Bridge Reference Number
    'CCN':'source:copyright', // Commercial Copyright Notice
    'CDR':'source:commercial_distribution_restriction', // Commercial Distribution Restriction
    'CHAN':'navaid:channel', // CHAN
    'CID':'source:cell_identifier', // Cell Identifier
    'DIA':'diam:classification', // DIAM Functional Classification
    'DQS':'source:data_quality', // Data Quality Statement
    'EEI':'external_entity_identifier', // External Entity Identifier
    'ETZ':'source:extraction_specification:version', // Extraction Specification Version
    //     'F_CODE':'raw:F_CODE', // Feature Code
    'FREQ':'navigation_aid:broadcast_freq', // FREQ
    'GA032_NSX':'navaid:service_name', // Aeronautical Radio Navigation Service Name
    'GB052_RIDH':'aeroway:runway_designator_high', // Runway Direction (high end) : Runway Designator (high)
    'GB052_RIDL':'aeroway:runway_designator_low', // Runway Direction (low end) : Runway Designator (low)
    'GLOBALID':'uuid:global', // Not sure where this comes from. It is in the sample data
    'ICAO_RGN':'icao:rgn', // ICAO_RGN
    'IKO':'icao', // ICAO Location Indicator
    'MCA':'navigationaid:characters_emitted', // Characters Emitted
    'MDE':'source:update:datetime', // Maintenance Date and Time
    // 'MEM':'raw:MEM', // Memorandum
    'NA8':'official_name', // Aerodrome Official Name
    'NAA':'airspace:name', //Airspace Name
    'NAM':'navaid:name', // NAM
    'NAV_COUNTRY':'navaid:country', // NAV_COUNTRY
    'NAV_IDENT':'navaid:identifier', // NAV_IDENT
    'OIQ':'object_identification_quality', // Object Identification Quality
    'OTH':'note:oth', // Specified Domain Value(s)
    'PROVINCE':'province:name', // PROVINCE
    'PSE':'restriction_interval', // Periodic Restriction Month Interval
    'RIN_RTN':'ref', // Route Identification <route designation>
    'RIN_RTN2':'ref:2', // Route Identification [2] <route designation>
    'RIN_RTN3':'ref:3', // Route Identification [3] <route designation>
    'RTL':'source:title', // Resource Title
    'SSE':'seasonal_ice_limit', // Seasonal Ice Limit
    'UFI':'uuid', // Unique Feature Identifier. This gets cleaned up in post processing
    'URI':'source:ref', // Unique Resource Identifier
    'USAGE':'navaid:usage', // USAGE
    'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
    'WAC':'world_aeronautical_chart', // WAC
    'WPI':'world_port_index', // World Port Index Identifier - This is part of Built Up Area. What??
    //'ZI001_SDP':'source:description', // Source Information : Source Description
    'ZI001_SDP':'source', // Source Information : Source Description
    'ZI001_SDV':'source:datetime', // Source Information : Source Date and Time
    'ZI001_VSD':'source:vertical_source:datetime', // Source Information : Vertical Source Date and Time
    'ZI001_VSN':'source:vertical_source:description', // Source Information : Vertical Source Description
    'ZI002_UFI':'security:restriction_ufi', // Restriction Information : Unique Entity Identifier
    'ZI004_PRE':'source:processing_information', // Process Step Information : Process Step Description
    'ZI005_FNA':'name', // Geographic Name Information : Full Name
    'ZI005_FNA2':'alt_name', // Geographic Name Information : (second) Full Name
    'ZI005_FNAA':'name:first_side', // Geo Name Info (1) : Full Name (first side)
    'ZI005_FNAB':'name:second_side', // Geo Name Info (2) : Full Name (second side)
    'ZI005_NFN':'gndb_id', // Geographic Name Information : Name Identifier
    'ZI005_NFN2':'gndb_id:2', // Geographic Name Information : (second) Name Identifier
    'ZI005_NFNA':'gndb_id:first_side', // Geo Name Info (1) : Name Identifier (first side)
    'ZI005_NFNB':'gndb_id:second_side', // Geo Name Info (2) : Name Identifier (second side)
    'ZI006_MEM':'note', // Note : Memorandum
    // We fix ZI020_GE4X in post processing
    'ZI020_GE4':'addr:country', // (Location Country) Designation : GENC Short URN-based Identifier
    'ZI020_GE42':'addr:country:second', // (Location Country) Designation : GENC Short URN-based Identifier (second)
    'ZI020_GE43':'addr:country:third', // (Location Country) Designation : GENC Short URN-based Identifier (third)
    'ZI020_GE44':'addr:country:fourth', // (Location Country) Designation : GENC Short URN-based Identifier (fourth)
    // 'ZSAX_RS0':'security:classification', // Restriction Information : Security Attributes Group <resource classification>
    'ZSAX_RX0':'security:dissemination_control:ic', // Restriction Information : Security Attributes Group <resource dissemination controls>
    'ZSAX_RX3':'security:dissemination_control:non_ic', // Restriction Information : Security Attributes Group <resource non-intelligence community markings>
    'ZSAX_RX4':'security:resource_owner', // Restriction Information : Security Attributes Group <resource owner-producer>
    'imagery_id':'source:imagery:id', // Image_id - non-spec
    'legacy_id':'source:imagery:legacy_id', // Image_id - non-spec    '
    'img_layer':'source:imagery:layerName', // Layer name - non-spec
    'early_date':'source:imagery:earliestDate', // Earliest image in a mosaic - non-spec
    'late_date':'source:imagery:latestDate', // Latest image in a mosaic - non-spec
    'PFI':'source:imagery:sensor', // Acquisition Platform Identifier - non-spec
  }, // End txtBiased

  // One2one ruled for Number fields
  numBiased : {
    'AHA':'source:accuracy:horizontal', // Absolute Horizontal Accuracy (90%)
    'AOO':'angle', // Angle of Orientation
    'ARA':'feature_area', // Area - this has been moved from 'area' due to hoot conflicts
    'AVA':'source:accuracy:vertical', // Absolute Vertical Accuracy (90%)
    'BC040_LVN':'seamark:light:height', // Maritime Navigation Light : Light Elevation
    'BC040_EOL':'seamark:light:range', // Maritime Navigation Light : Nominal Range
    'BEL':'ele:base', // Base Elevation
    'BNF':'building:levels', // Floor Count
    'CRV':'depth:value', // Depth Curve or Contour value
    'CTL':'railway:track:length', // Cumulative Track Length
    'DEV':'level', // Deck Level
    'DIM':'diameter', // Diameter
    'DMT':'canopy_cover', // Canopy Cover
    'DOF':'flow_direction', // Direction of Flow
    'DZC':'deck_count', // Deck Count
    'DZP':'depth:maximum_below_surface', // Deepest Depth Below Surface Level
    'EPW':'generator:output:electricity', // Electrical Power Generation Capacity
    'EVA':'source:accuracy:elevation', // Elevation Vertical Accuracy (90%)
    // 'FCSUBTYPE':'etds:fcsubtype', // Very ESRI Specific. Ignored for now
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
    'LC1':'mlc:wheeled_oneway', // Load Class Type 1
    'LC2':'mlc:wheeled', // Load Class Type 2
    'LC3':'mlc:tracked_oneway', // Load Class Type 3
    'LC4':'mlc:tracked', // Load Class Type 4
    'LDC':'length:crest', // Dam Crest Length
    'LEA':'depth:minimum_below_surface', // Least Depth Below Surface Level
    'LTN':'lanes', // Track or Lane Count
    'LZN':'length', // Length
    'MAG':'magnetic_variation', // Magnetic Variation
    'MWG':'divider:width', // Centerline Spacing
    'NAV_KEYCODE':'navaid:keycode', // NAV_KEYCODE
    'NOS':'bridge:span_count', // Span Count
    'NPL':'cables', // Parallel Line Count: See AT005
    'OHC':'max_clearance', // Overhead Clearance
    'PFD':'depth:predominant', // Predominant Feature Depth
    'PFH':'height:feature', // Predominant Feature Height
    'PVH':'height:vegetation', // Predominant Vegetation Height
    'PWA':'depth', // Predominant Water Depth
    'PWR':'navigationaid:power', // NAVAID Power
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
    // 'WBD':'raw:WBD', // Waterbody Depth See PWA
    'WD2':'width:total_usable', // Route Total Usable Width
    'WD3':'width:gap', // Terrain Gap Width
    'WDU':'width:interior_useable', // Usable Width
    'WID':'width', // Width
    'WOC':'width:crest', // Dam Crest Width
    'WT2':'width:second_way', // Width of Second Travelled Way
    'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
    'ZI017_GAW':'gauge', // Track Information : Railway Gauge
    'ZI026_CTUL':'cartographic_scale:lower', // Feature Metadata : Cartographic Usability Range <lower value>
    'ZI026_CTUU':'cartographic_scale:upper', // Feature Metadata : Cartographic Usability Range <upper value>
    'ZVA':'ele', // Aerodrome Elevation
    'ZVH':'ele:max', // Highest Elevation
    'ZVH_AVA':'source:accuracy:highest_elevation', // Highest Elevation <absolute vertical accuracy>
  }, // End numBiased


  // Common one2one rules. Used for both import and export
  one2one : [
    // img_mosaic - Is the image a mosaic? - Added field
    ['img_mosaic','yes','source:imagery:mosaic','yes'],
    ['img_mosaic','no','source:imagery:mosaic','no'],

    // ACC - Horizontal Accuracy Category
    ['ACC','-999999','source:accuracy:horizontal:category','unknown'], // noInformation
    ['ACC','1','source:accuracy:horizontal:category','accurate'],
    ['ACC','2','source:accuracy:horizontal:category','approximate'],
    ['ACC','3','source:accuracy:horizontal:category','doubtful'],

    // AMA - Amusement Attraction Type
    ['AMA','-999999','attraction','unknown'], // noInformation
    ['AMA','1','attraction','artificial_mountain'], // Artificial Mountain
    ['AMA','2','attraction','ferris_wheel'], // Ferris Wheel
    ['AMA','3','attraction','roller-coaster'], // Roller-coaster
    ['AMA','4','attraction','spherical'], // Spherical
    ['AMA','5','attraction','water_attraction'], // Water Attraction
    ['AMA','6','attraction','vertical_ride'], // Vertical Ride
    ['AMA','999','attraction','other'], // Other

    // AMB - Special Administrative Unit
    ['AMB','-999999','administrative_unit:type','unknown'], // No Information
    ['AMB','1','administrative_unit:type','lease_area'], // Lease Area
    ['AMB','2','administrative_unit:type','concession_area'], // Concession Area
    ['AMB','3','administrative_unit:type','free_trade_zone'], // Free Trade Zone
    ['AMB','4','administrative_unit:type','economic_region'], // Economic Region
    ['AMB','5','administrative_unit:type','postal_district'], // Postal District
    ['AMB','6','administrative_unit:type','census_district'], // Census District
    ['AMB','7','administrative_unit:type','water_management_district'], // Water Management District
    ['AMB','8','administrative_unit:type','congressional_district'], // Congressional District
    ['AMB','9','administrative_unit:type','environmental_protection_agency_region'], // Environmental Protection Agency Region
    ['AMB','10','administrative_unit:type','federal_emergency_management_agency_region'], // Federal Emergency Management Agency Region
    ['AMB','11','administrative_unit:type','federal_energy_regulatory_commission_region'], // Federal Energy Regulatory Commission Region
    ['AMB','12','administrative_unit:type','native_american_reservation'], // Native American Reservation
    ['AMB','13','administrative_unit:type','radiological_assistance_program_region'], // Radiological Assistance Program Region
    ['AMB','14','administrative_unit:type','federal_aviation_administration_region'], // Federal Aviation Administration Region
    ['AMB','15','administrative_unit:type','army_corps_of_engineers_district'], // Army Corps of Engineers District
    ['AMB','16','administrative_unit:type','army_corps_of_engineers_division'], // Army Corps of Engineers Division
    ['AMB','17','administrative_unit:type','coast_guard_sector'], // Coast Guard Sector
    ['AMB','18','administrative_unit:type','border_patrol_sector'], // Border Patrol Sector
    ['AMB','19','administrative_unit:type','fbi_field_office_jurisdiction'], // FBI Field Office Jurisdiction
    ['AMB','20','administrative_unit:type','fbi_resident_agency_district'], // FBI Resident Agency District

    // APT - Airfield Use
    ['APT','-999999','aeroway:use','unknown'], // No Information
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

    // APT2 - Airfield Use [2]
    // APT3 - Airfield Use [3]

    // APU - Apron Usage
    ['APU','-999999','apron:use','unknown'], // No Information
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

    // APU2 - Apron Usage [2]
    // APU3 - Apron Usage [3]

    // APY - Apron Type
    ['APY','-999999','apron:type','unknown'], // No Information
    ['APY','1','apron:type','passenger'], // Passenger
    ['APY','2','apron:type','cargo'], // Cargo
    ['APY','3','apron:type','remote_parking'], // Remote Parking
    ['APY','4','apron:type','services_and/or_hangar'], // Services and/or Hangar
    ['APY','5','apron:type','general_aviation'], // General Aviation
    ['APY','6','apron:type','military'], // Military
    ['APY','7','apron:type','dispersal'], // Dispersal
    ['APY','8','apron:type','holding'], // Holding
    ['APY','999','apron:type','other'], // Other

    // ART - Aeronautical Route Category
    ['ART','-999999','navigationaid:route_type','unknown'], // No Information
    ['ART','1','navigationaid:route_type','low'], // Low
    ['ART','2','navigationaid:route_type','high'], // High
    ['ART','3','navigationaid:route_type','high_and_low'], // Both

    // ASO - Aeronautical Service Operational Status
    ['ASO','-999999','aeronautical_service_status','unknown'], // No Information
    ['ASO','1','aeronautical_service_status','operational'], // Operational
    ['ASO','2','aeronautical_service_status','unserviceable'], // Unserviceable
    ['ASO','3','aeronautical_service_status','on-test'], // On-Test
    ['ASO','4','aeronautical_service_status','interrupt'], // Interrupt
    ['ASO','6','aeronautical_service_status','conditional'], // Conditional
    ['ASO','7','aeronautical_service_status','false_indication_definite'], // False Indication Definite
    ['ASO','8','aeronautical_service_status','false_indication_possible'], // False Indication Possible
    ['ASO','10','aeronautical_service_status','displaced'], // Displaced
    ['ASO','11','aeronautical_service_status','in_construction'], // In Construction
    ['ASO','12','aeronautical_service_status','withdrawn'], // Withdrawn
    ['ASO','13','aeronautical_service_status','intermittent'], // Intermittent
    ['ASO','14','aeronautical_service_status','irregular'], // Irregular
    ['ASO','15','aeronautical_service_status','navaid_dme_out-of-service'], // NAVAID DME Out-of-service
    ['ASO','16','aeronautical_service_status','navaid_frequency_out-of-service'], // NAVAID Frequency Out-of-service
    ['ASO','17','aeronautical_service_status','navaid_partial_service'], // NAVAID Partial Service

    // ASU - Aerodrome Movement Area Surface Composition - See ZI019_ASU
    // ['ASU','-999999','raw:ASU','unknown'], // noInformation
    // ['ASU','1','raw:ASU','asphalt'], // Asphalt
    // ['ASU','2','raw:ASU','bituminous_mix'], // Bituminous Mix
    // ['ASU','3','raw:ASU','brick'], // Brick
    // ['ASU','4','raw:ASU','clay'], // Clay
    // ['ASU','5','raw:ASU','concrete'], // Concrete
    // ['ASU','6','raw:ASU','coral'], // Coral
    // ['ASU','7','raw:ASU','earthen'], // Earthen
    // ['ASU','8','raw:ASU','gravel'], // Gravel
    // ['ASU','9','raw:ASU','ice'], // Ice
    // ['ASU','10','raw:ASU','landing_mat'], // Landing Mat
    // ['ASU','11','raw:ASU','laterite'], // Laterite
    // ['ASU','12','raw:ASU','macadam'], // Macadam
    // ['ASU','13','raw:ASU','membrane'], // Membrane
    // ['ASU','14','raw:ASU','non-bituminous_mix'], // Non-bituminous Mix
    // ['ASU','15','raw:ASU','pierced_steel_planking'], // Pierced Steel Planking
    // ['ASU','16','raw:ASU','sand'], // Sand
    // ['ASU','17','raw:ASU','snow'], // Snow
    // ['ASU','18','raw:ASU','stone'], // Stone
    // ['ASU','19','raw:ASU','water'], // Water
    // ['ASU','20','raw:ASU','wood'], // Wood
    // ['ASU','21','raw:ASU','asphalt_over_concrete'], // Asphalt Over Concrete
    // ['ASU','999','raw:ASU','other'], // Other

    // ASU2 - Aerodrome Movement Area Surface Composition [2]
    // ASU3 - Aerodrome Movement Area Surface Composition [3]

    // ASY - Airfield Symbol Type
    ['ASY','-999999','aeroway:aerodrome_symbol','unknown'], // No Information
    ['ASY','1','aeroway:aerodrome_symbol','active_civilian'], // A - Active Civilian
    ['ASY','2','aeroway:aerodrome_symbol','active_joint_(civilian/military)'], // B - Active Joint (Civilian/Military)
    ['ASY','3','aeroway:aerodrome_symbol','active_military'], // C - Active Military
    ['ASY','4','aeroway:aerodrome_symbol','active_(with_less-than_the_minimum_facilities)'], // D - Active (with less than the minimum facilities)
    ['ASY','5','aeroway:aerodrome_symbol','abandoned_or_closed_(with_usable_runways_or_landing_areas)'], // E - Abandoned or Closed (with usable runways or landing areas)
    ['ASY','6','aeroway:aerodrome_symbol','highway_strip'], // F - Highway Strip
    ['ASY','7','aeroway:aerodrome_symbol','unusable_for_landing_or_take-off'], // G - Unusable for landing or take-off
    ['ASY','8','aeroway:aerodrome_symbol','active_military_heliport'], // H - Active Military Heliport
    ['ASY','9','aeroway:aerodrome_symbol','active_civil_heliport'], // J - Active Civil Heliport
    ['ASY','10','aeroway:aerodrome_symbol','active_military_heliport_(with_less-than_minimum_facilities)'], // K - Active Military Heliport (with less than minimum facilities)
    ['ASY','11','aeroway:aerodrome_symbol','active_civil_heliport_(with_less-than_minimum_facilities)'], // L - Active Civil Heliport (with less than minimum facilities)
    ['ASY','12','aeroway:aerodrome_symbol','decoy'], // X - Decoy
    ['ASY','13','aeroway:aerodrome_symbol','active_joint_(civilian/military)_heliport'], // Active Joint (Civilian/Military) Heliport

    // AT005_CAB - Cable : Cable Type See CAB
    // AT005_CAB2 - Cable : Cable Type [2]
    // AT005_CAB3 - Cable : Cable Type [3]

    // ATC - Aqueduct Type
    ['ATC','-999999','aqueduct:type','unknown'], // No Information
    ['ATC','3','aqueduct:type','underground'], // Underground
    ['ATC','4','aqueduct:type','underwater'], // Underwater
    ['ATC','5','aqueduct:type','surface'], // Surface
    ['ATC','6','aqueduct:type','qanat'], // Qanat

    // ATY - Airspace Type
    ['ATY','-999999','airspace:type','unknown'], // noInformation
    ['ATY','1','airspace:type','air_defense_identification_zone'], // Air Defense Identification Zone
    ['ATY','2','airspace:type','airway'], // Airway
    ['ATY','3','airspace:type','alert_area'], // Alert Area
    ['ATY','4','airspace:type','altimeter_setting_region'], // Altimeter Setting Region
    ['ATY','5','airspace:type','amc_manageable_danger_area'], // AMC Manageable Danger Area
    ['ATY','6','airspace:type','amc_manageable_restricted_area'], // AMC Manageable Restricted Area
    ['ATY','7','airspace:type','control_area'], // Control Area
    ['ATY','8','airspace:type','control_sector'], // Control Sector
    ['ATY','9','airspace:type','control_zone'], // Control Zone
    ['ATY','10','airspace:type','cross_border_area'], // Cross Border Area
    ['ATY','11','airspace:type','danger_area'], // Danger Area
    ['ATY','12','airspace:type','flight_peripheral_zone'], // Flight Peripheral Zone
    ['ATY','13','airspace:type','area_minimum_altitude'], // Area Minimum Altitude
    ['ATY','14','airspace:type','oceanic_control_area'], // Oceanic Control Area
    ['ATY','15','airspace:type','prohibited_area'], // Prohibited Area
    ['ATY','16','airspace:type','restricted_area'], // Restricted Area
    ['ATY','17','airspace:type','temporary_segregated_area'], // Temporary Segregated Area
    ['ATY','18','airspace:type','temporary_reserved_area'], // Temporary Reserved Area
    ['ATY','19','airspace:type','terminal_control_area'], // Terminal Control Area
    ['ATY','20','airspace:type','upper_control_area'], // Upper Control Area
    ['ATY','21','airspace:type','upper_flight_information_region'], // Upper Flight Information Region
    ['ATY','22','airspace:type','lower_traffic_area'], // Lower Traffic Area
    ['ATY','23','airspace:type','advisory_area'], // Advisory Area
    ['ATY','24','airspace:type','upper_advisory_area'], // Upper Advisory Area
    ['ATY','25','airspace:type','military_aerodrome_traffic_zone'], // Military Aerodrome Traffic Zone
    ['ATY','26','airspace:type','aerodrome_traffic_zone'], // Aerodrome Traffic Zone
    ['ATY','27','airspace:type','military_terminal_control_area'], // Military Terminal Control Area
    ['ATY','28','airspace:type','standard_pressure_region'], // Standard Pressure Region
    ['ATY','29','airspace:type','reduced_co-ordination_area'], // Reduced Co-ordination Area
    ['ATY','30','airspace:type','french_peripheral_zone'], // French Peripheral Zone
    ['ATY','31','airspace:type','warning_area'], // Warning Area
    ['ATY','32','airspace:type','german_night_low_flying_system'], // German Night Low Flying System
    ['ATY','33','airspace:type','german_deconfliction_line'], // German Deconfliction Line
    ['ATY','34','airspace:type','german_low_flying_areas'], // German Low Flying Areas
    ['ATY','35','airspace:type','special_rules_zone'], // Special Rules Zone
    ['ATY','36','airspace:type','military_operations_area'], // Military Operations Area
    ['ATY','41','airspace:type','no-fly_area'], // No-fly Area
    ['ATY','42','airspace:type','buffer_zone'], // Buffer Zone
    ['ATY','999','airspace:type','other'], // Other

    // AWP - Aeronautical Obstacle Light Present
    ['AWP','-999999','light:obstacle','unknown'], // No Information
    ['AWP','1000','light:obstacle','no'],
    ['AWP','1001','light:obstacle','yes'],
// ['AWP','-1001','raw:AWP','true'], // TRUE
// ['AWP','-1000','raw:AWP','false'], // FALSE


    // AXS - Aerodrome Surface Status
    ['AXS','-999999','aeroway:surface:status','unknown'], // No Information
    ['AXS','1','aeroway:surface:status','closed'], // Closed
    ['AXS','2','aeroway:surface:status','open'], // Open
    ['AXS','3','aeroway:surface:status','work_in_progress'], // Work in Progress
    ['AXS','4','aeroway:surface:status','parked_or_disabled_aircraft'], // Parked or Disabled Aircraft

    // AZC - Man-made
    ['AZC','-999999','artificial','unknown'], // No Information
    ['AZC','1000','artificial','no'],
    ['AZC','1001','artificial','yes'],

    // BA000_VDC - Water Line : Sounding Datum
    ['BA000_VDC','-999999','source:datum:sounding','unknown'], // No Information
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
    ['BAC','-999999','building:density','unknown'], // No Information
    ['BAC','1','building:density','sparse'], // Sparse
    ['BAC','2','building:density','dense'], // Dense
    ['BAC','3','building:density','moderate'], // Moderate

    // BAL - BGN Administrative Level
    ['BAL','-999999','administrative_subdivision','unknown'], // noInformation
    ['BAL','1','administrative_subdivision','first-order'], // First-order
    ['BAL','2','administrative_subdivision','second-order'], // Second-order
    ['BAL','3','administrative_subdivision','third-order'], // Third-order
    ['BAL','4','administrative_subdivision','fourth-order'], // Fourth-order
    ['BAL','5','administrative_subdivision','undifferentiated'], // Undifferentiated
    ['BAL','999','administrative_subdivision','other'], // Other

    // BAT - Barrier Top Type
    ['BAT','-999999','barrier:top','unknown'], // No Information
    ['BAT','1','barrier:top','barbed_wire'], // Barbed Wire
    ['BAT','2','barrier:top','chain_link'], // Chain-link
    ['BAT','3','barrier:top','concertina_wire'], // Concertina Wire
    ['BAT','4','barrier:top','electrified_wire'], // Electrified Wire
    ['BAT','5','barrier:top','spiked'], // Spiked
    ['BAT','999','barrier:top','other'], // Other

    // BC034_MRT - Maritime Radiobeacon : Maritime Radiobeacon Type
    ['BC034_MRT','-999999','seamark:radio_station:category','unknown'], // noInformation
    ['BC034_MRT','1','seamark:radio_station:category','ais'], // Automated Identification System (AIS)
    ['BC034_MRT','2','seamark:radio_station:category','omnidirectional'], // Circular Radiobeacon
    ['BC034_MRT','3','seamark:radio_station:category','directional'], // Directional Radiobeacon
    ['BC034_MRT','4','seamark:radio_station:category','loran'], // Long Range Air Navigation System (LORAN) C
    ['BC034_MRT','5','seamark:radio_station:category','qtg'], // QTG Station
    ['BC034_MRT','6','seamark:radar_transponder:category','ramark'], // Radar Marker (RAMARK)
    ['BC034_MRT','7','seamark:radar_transponder:category','racon'], // Radar Responder Beacon (RACON)
    ['BC034_MRT','8','seamark:radio_station:category','rdf'], // Radio Direction Finding Station
    ['BC034_MRT','9','seamark:radio_station:category','rotating_pattern'], // Rotating Pattern Radiobeacon
    ['BC034_MRT','999','seamark:radio_station:category','other'], // Other

   // BC101_SST - Fog Signal : Fog Signal Type
   ['BC101_SST','-999999','seamark:fog_signal:category','unknown'], // noInformation
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
    ['BC101_SST','998','seamark:fog_signal:category','not_applicable'], // Not Applicable
    ['BC101_SST','999','seamark:fog_signal:category','other'], // Other

    // BCC - Bypass Condition
    ['BCC','-999999','bypass','unknown'], // No Information
    ['BCC','1','bypass','easy'],
    ['BCC','2','bypass','difficult'],
    ['BCC','3','bypass','impossible'],

    // BGT - Basin Gate Type
    ['BGT','-999999','seamark:gate:category','unknown'], // No Information
    ['BGT','1','seamark:gate:category','caisson'], // Caisson
    ['BGT','2','seamark:gate:category','lock'], // Lock Gate
    ['BGT','3','seamark:gate:category','tide_lock'], // Tide Lock
    ['BGT','999','seamark:gate:category','other'], // Other

    // BH141_SLTA - Waterbody bank 1 shoreline type: No OSM even close, See MGCP SHR & SHL
    ['BH141_SLTA','-999999','waterway:bank1:type','unknown'], // Null
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
    ['BH141_SLTB','-999999','waterway:bank2:type','unknown'], // Null
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
    ['BMC','-999999','waterway:bottom','unknown'], // No Information
    ['BMC','1','waterway:bottom','clay_and_silt'], // Clay and Silt
    ['BMC','2','waterway:bottom','silty_sands'], // Silty Sands
    ['BMC','3','waterway:bottom','sand_and_gravel'], // Sand and Gravel
    ['BMC','4','waterway:bottom','gravel_and_cobble'], // Gravel and Cobble
    ['BMC','5','waterway:bottom','rocks_and_boulders'], // Rocks and Boulders
    ['BMC','6','waterway:bottom','bedrock'], // Bedrock
    ['BMC','7','waterway:bottom','paved'], // Paved
    ['BMC','8','waterway:bottom','peat'], // Peat
    ['BMC','9','waterway:bottom','sand_over_mud'], // Sand over Mud
    ['BMC','11','waterway:bottom','coral'], // Coral
    ['BMC','14','waterway:bottom','sand'], // Sand
    ['BMC','15','waterway:bottom','concrete'], // Concrete
    ['BMC','16','waterway:bottom','masonry'], // Masonry
    ['BMC','17','waterway:bottom','reinforced_concrete'], // Reinforced Concrete
    ['BMC','18','waterway:bottom','soil'], // Soil
    ['BMC','999','waterway:bottom','other'], // Other

    // BMC2 - Bottom Material Type [2]
    // BMC3 - Bottom Material Type [3]

    // BOC - Bog Type
    ['BOC','-999999','bog:type','unknown'], // No Information
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
    ['BOT','-999999','bridge:movable','unknown'], // Null
    ['BOT','4','bridge:movable','bascule'],
    ['BOT','10','bridge:movable','swing'],
    ['BOT','11','bridge:movable','lift'],
    ['BOT','12','bridge:movable','retractable'],
    ['BOT','14','bridge:movable','submersible'],
    ['BOT','15','bridge:movable','drawbridge'],
    ['BOT','16','bridge:movable','opening'],
    ['BOT','17','bridge:movable','no'],
    ['BOT','999','bridge:movable','other'],

    // BSC - Bridge Structure Type
    ['BSC','-999999','bridge:structure','unknown'], // No Information
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

    // BSC2 - Bridge Structure Type [2]
    // BSC3 - Bridge Structure Type [3]

    // BSM - Mobile Bridge Span
    ['BSM','-999999','bridge:mobile_span','unknown'], // No Information
    ['BSM','1000','bridge:mobile_span','no'],
    ['BSM','1001','bridge:mobile_span','yes'],

    // BST - Boundary Status
    ['BST','-999999','boundary:status','unknown'], // No Information
    ['BST','1','boundary:status','definite'], // Definite
    ['BST','2','boundary:status','indefinite'], // Indefinite
    ['BST','3','boundary:status','in_dispute'], // In Dispute
    ['BST','4','boundary:status','no_defined_boundary'], // No Defined Boundary

    // BSU - Building Superstructure Type
    ['BSU','-999999','building:superstructure:type','unknown'], // No Information
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
    ['BSU','18','building:superstructure:type','solar_panels'], // Solar Panels
    ['BSU','999','building:superstructure:type','other'], // Other

    // CAA - Controlling Authority - This does not play nicely in OSM
    ['CAA','-999999','operator','unknown'], // No Information
    ['CAA','1','operator','national'],
    ['CAA','2','operator','state'],
    ['CAA','3','operator','private'], // Private
    ['CAA','4','operator','tribal'], // Tribal
    ['CAA','5','operator','military'], // Military
    ['CAA','6','operator','municipal'], // Municipal
    ['CAA','7','operator','joint_military_civilian'], // Joint Military and Civilian
    ['CAA','8','operator','international'],
    ['CAA','10','operator','insular'], // Insular
    ['CAA','11','operator','province'], // Province
    ['CAA','16','operator','civilian'], // Civilian
    ['CAA','17','operator','public'], // Public
    ['CAA','999','operator','other'], // Other

    // CAB - Cable Type
    ['CAB','-999999','cable:type','unknown'], // No Information
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

    // CAB2 - Cable Type [2]
    // CAB3 - Cable Type [3]

    // CAM - Conservation Area Management Category
    ['CAM','-999999','protect_class','unknown'], // No Information
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
    ['CAT','-999999','cableway:type','unknown'], // No Information
    ['CAT','2','cableway:type','chair-lift'], // Chair-lift
    ['CAT','3','cableway:type','ski_tow'], // Ski Tow
    ['CAT','5','cableway:type','aerial_tramway'], // Aerial Tramway
    ['CAT','6','cableway:type','gondola_lift'], // Gondola Lift
    ['CAT','7','cableway:type','t-bar_lift'], // T-bar Lift
    ['CAT','8','cableway:type','industrial_ropeway'], // Industrial Ropeway
    ['CAT','9','cableway:type','material_tramway'], // Material Tramway
    ['CAT','999','cableway:type','other'], // Other

    // CBP - Containment Berm Present
    ['CBP','-999999','containment_berm','unknown'], // No Information
    ['CBP','1000','containment_berm','no'],
    ['CBP','1001','containment_berm','yes'],

    // CCT - Cover Closure Type
    ['CCT','-999999','cover:type','unknown'], // No Information
    ['CCT','1','cover:type','open'], // Open
    ['CCT','2','cover:type','partial'], // Partial
    ['CCT','3','cover:type','moveable'], // Moveable
    ['CCT','4','cover:type','complete'], // Complete

    // CDA - Covered Drain
    ['CDA','-999999','covered','unknown'], // No Information
    ['CDA','1000','covered','no'],
    ['CDA','1001','covered','yes'],

    // CET - Grading Type
    ['CET','-999999','grading:type','unknown'], // No Information
    ['CET','1','grading:type','one_side'], // One Side
    ['CET','2','grading:type','two_sides'], // Two Sides
    ['CET','3','grading:type','many_sides'], // Many Sides
    ['CET','4','grading:type','no_sides'], // No Sides
    ['CET','999','grading:type','other'], // Other

    // CHA - Navigation Light Charateristic
    ['CHA','-999999','beacon:light','unknown'], // noInformation
    ['CHA','1','beacon:light','alternating'], // Alternating
    ['CHA','4','beacon:light','ultra_quick-flashing'], // Ultra Quick-flashing
    ['CHA','5','beacon:light','fixed'], // Fixed
    ['CHA','6','beacon:light','fixed_and_flashing'], // Fixed and Flashing
    ['CHA','8','beacon:light','flashing'], // Flashing
    ['CHA','11','beacon:light','interrupted_quick-flashing'], // Interrupted Quick-flashing
    ['CHA','12','beacon:light','interrupted_ultra_quick-flashing'], // Interrupted Ultra Quick-flashing
    ['CHA','13','beacon:light','interrupted_very_quick-flashing'], // Interrupted Very Quick-flashing
    ['CHA','14','beacon:light','isophase'], // Isophase
    ['CHA','15','beacon:light','long-flashing'], // Long-flashing
    ['CHA','16','beacon:light','morse_code'], // Morse Code
    ['CHA','17','beacon:light','occulting'], // Occulting
    ['CHA','28','beacon:light','group_quick-flashing'], // Group Quick-flashing
    ['CHA','29','beacon:light','group_very_quick-flashing'], // Group Very Quick-flashing
    ['CHA','44','beacon:light','quick-flashing'], // Quick-flashing
    ['CHA','45','beacon:light','very_quick-flashing'], // Very Quick-flashing
    ['CHA','46','beacon:light','flashing_with_long-flash'], // Flashing with Long-flash
    ['CHA','47','beacon:light','occulting_flashing'], // Occulting Flashing
    ['CHA','48','beacon:light','fixed_long-flashing'], // Fixed Long-flashing
    ['CHA','49','beacon:light','occulting_alternating'], // Occulting Alternating
    ['CHA','50','beacon:light','long-flashing_alternating'], // Long-flashing Alternating
    ['CHA','51','beacon:light','flashing_alternating'], // Flashing Alternating
    ['CHA','57','beacon:light','quick-flashing_with_long-flash'], // Quick-flashing with Long-flash
    ['CHA','58','beacon:light','very_quick-flashing_with_long-flash'], // Very Quick-flashing with Long-flash
    ['CHA','59','beacon:light','ultra_quick-flashing_with_long-flash'], // Ultra Quick-flashing with Long-flash
    ['CHA','60','beacon:light','fixed_with_alternating_flashing'], // Fixed with Alternating Flashing
    ['CHA','999','beacon:light','other'], // Other

    // CIW - Closed in Winter
    ['CIW','-999999','closed_in_winter','unknown'], // noInformation
    ['CIW','1000','closed_in_winter','no'],
    ['CIW','1001','closed_in_winter','yes'],

    // CMS - Commissioned Status
    ['CMS','-999999','commissioned','unknown'], // No Information
    ['CMS','1','commissioned','commissioned_and_operational'], // Commissioned and Operational
    ['CMS','2','commissioned','commissioned_and_on_test'], // Commissioned and on Test
    ['CMS','3','commissioned','commissioned_but_out_of_service'], // Commissioned but Out of Service
    ['CMS','4','commissioned','operational_but_not_commissioned'], // Operational but Not Commissioned
    ['CMS','5','commissioned','on_test_but_not_commissioned'], // On Test but Not Commissioned
    ['CMS','6','commissioned','not_commissioned_and_out_of_service'], // Not Commissioned and Out of Service

    // COS - Facility Operational Status
    ['COS','-999999','operational_status','unknown'], // noInformation
    ['COS','4','operational_status','non-operational'], // Non-operational
    ['COS','6','operational_status','planned'], // Planned
    ['COS','9','operational_status','temporarily_non-operational'], // Temporarily Non-operational
    ['COS','13','operational_status','operational'], // Operational
    ['COS','14','operational_status','partially_operational'], // Partially Operational

    // Handled with GE4
    // COUNTRY_CODE - Country Code
    // ['COUNTRY_CODE','AA','raw:COUNTRY_CODE','aa'], // AA
    // ['COUNTRY_CODE','AC','raw:COUNTRY_CODE','ac'], // AC
//
    // ['COUNTRY_CODE','ZA','raw:COUNTRY_CODE','za'], // ZA
    // ['COUNTRY_CODE','ZI','raw:COUNTRY_CODE','zi'], // ZI
    // ['COUNTRY_CODE','noInformation','raw:COUNTRY_CODE','unknown'], // noInformation

    // CPS - Cell Partition Scheme
    ['CPS','-999999','source:cell_partition_scheme','unknown'], // No Information
    ['CPS','1','source:cell_partition_scheme','5_arc_degree'], // A global geodetic grid consisting of 5 by 5 arc degree cells.
    ['CPS','2','source:cell_partition_scheme','1_arc_degree'], // A global geodetic grid consisting of 1 by 1 arc degree cells.
    ['CPS','3','source:cell_partition_scheme','0.5_arc_degree'], // A global geodetic grid consisting of 30 by 30 arc minute cells over land.
    ['CPS','4','source:cell_partition_scheme','0.25_arc_degree'], // A global geodetic grid consisting of 15 by 15 arc minute cells over land.
    ['CPS','5','source:cell_partition_scheme','variable'], // A geodetic grid consisting of variable-sized cells situated so as to cover the extent of a region of interest.
    ['CPS','998','source:cell_partition_scheme','not_applicable'], // Not Applicable
    ['CPS','999','source:cell_partition_scheme','other'], // Other

    // CRA - Crane Type
    ['CRA','-999999','crane:type','unknown'], // No Information
    ['CRA','2','crane:type','bridge'], // Bridge Crane
    ['CRA','3','crane:type','rotating'], // Rotating Crane
    ['CRA','7','crane:type','tower'], // Tower Crane
    ['CRA','99','crane:type','container'], // Container Crane
    ['CRA','999','crane:type','other'], // Other

    // CRM - Crane Mobility Type
    ['CRM','-999999','crane:mobile','unknown'], // No Information
    ['CRM','1','crane:mobile','fixed'], // Fixed
    ['CRM','2','crane:mobile','travelling'], // Travelling
    ['CRM','3','crane:mobile','floating'], // Floating
    ['CRM','4','crane:mobile','yes'], // Mobile
    ['CRM','999','crane:mobile','other'], // Other

    // CST - Contained within Service Tunnel
    ['CST','-999999','service_tunnel','unknown'], // No Information
    ['CST','1000','service_tunnel','no'],
    ['CST','1001','service_tunnel','yes'],

    // CTC - Culvert Type
    ['CTC','-999999','culvert:type','unknown'], // No Information
    ['CTC','1','culvert:type','regular_with_soil'], // Regular with Soil
    ['CTC','2','culvert:type','box_with_soil'], // Box with Soil
    ['CTC','3','culvert:type','load_bearing_box'], // Box with Load
    ['CTC','999','culvert:type','other'], // Other

    // CUR - Current Type Category
    ['CUR','-999999','current','unknown'], // No Information
    ['CUR','4','current','river_flow'], // River Flow
    ['CUR','5','current','ocean_current'], // Ocean Current
    ['CUR','9','current','tidal_flow'], // Tidal Flow

    // CVT - Conveyor Type
    ['CVT','-999999','conveyor:type','unknown'], // No Information
    ['CVT','1','conveyor:type','not_visible'], // Not Visible
    ['CVT','2','conveyor:type','belt'], // Belt
    ['CVT','3','conveyor:type','rollers'], // Rollers
    ['CVT','4','conveyor:type','bucket'], // Bucket
    ['CVT','5','conveyor:type','screw_auger'], // Screw Auger
    ['CVT','6','conveyor:type','apron'], // Apron
    ['CVT','7','conveyor:type','flight'], // Flight
    ['CVT','999','conveyor:type','other'], // Other

    // CVT2 - Conveyor Type [2]
    // CVT3 - Conveyor Type [3]

    // CWT - Contained Within Tunnel
    ['CWT','-999999','in_tunnel','unknown'], // No Information
    ['CWT','1000','in_tunnel','no'],
    ['CWT','1001','in_tunnel','yes'], // Need to think about this

    // DFT - Dam Face Type
    ['DFT','-999999','dam:face:type','unknown'], // No Information
    ['DFT','1','dam:face:type','vertical'], // Vertical
    ['DFT','2','dam:face:type','slope'], // Slope

    // DFU - Dam Function
    ['DFU','-999999','dam:type','unknown'], // No Information
    ['DFU','1','dam:type','impoundment'], // Impoundment
    ['DFU','2','dam:type','navigation'], // Navigation
    ['DFU','3','dam:type','flood_control'], // Flood Control
    ['DFU','4','dam:type','hydroelectric_power_generation'], // Hydroelectric Power Generation
    ['DFU','5','dam:type','weir'], // Weir
    ['DFU','6','dam:type','irrigation'], // Irrigation
    ['DFU','7','dam:type','recreation'], // Recreation
    ['DFU','8','dam:type','water_supply'], // Water Supply
    ['DFU','999','dam:type','other'], // Other

    // DGC - Transportation Block Type
    ['DGC','-999999','block:type','unknown'], // No Information
    ['DGC','1','block:type','drop_gate'], // Drop Gate
    ['DGC','2','block:type','rolling_block'], // Rolling Block
    ['DGC','999','block:type','other'], // Other

    // DIR - Directivity
    ['DIR','-999999','directivity','unknown'], // No Information
    ['DIR','1','directivity','unidirectional'], // Unidirectional
    ['DIR','2','directivity','bidirectional'], // Bidirectional
    ['DIR','3','directivity','omnidirectional'], // Omnidirectional

    // DMD - Dammed
    ['DMD','-999999','dammed','unknown'], // No Information
    ['DMD','1000','dammed','no'], // Just to get rid of errors
    ['DMD','1001','dammed','yes'], // yes

   // DOF - Direction of Flow
   // ####

    // DSP - Boundary Dispute Type
    ['DSP','-999999','boundary:dispute_type','unknown'], // No Information
    ['DSP','1','boundary:dispute_type','in_dispute'], // In Dispute
    ['DSP','2','boundary:dispute_type','undisputed'], // Undisputed
    ['DSP','3','boundary:dispute_type','de_facto'], // De Facto
    ['DSP','4','boundary:dispute_type','de_jure'], // De Jure

    // EET - Engineered Earthwork Type
    ['EET','-999999','trench','unknown'], // No Information
    ['EET','1','trench','battery'], // Battery
    ['EET','2','trench','parapet'], // Military Parapet
    ['EET','3','military','trench'], // Military Trench
    ['EET','4','trench','rampart'], // Rampart
    ['EET','5','trench','redoubt'], // Redoubt
    ['EET','999','trench','other'], // Other

    // ESC - Elevation Surface Category
    ['ESC','-999999','elevation:surface','unknown'], // No Information
    ['ESC','1','elevation:surface','land'], // Land
    ['ESC','2','elevation:surface','snow_field_and/or_ice-field'], // Snow Field and/or Ice-field
    ['ESC','4','elevation:surface','vegetation'], // Vegetation
    ['ESC','5','elevation:surface','inland_water'], // Inland Water
    ['ESC','6','elevation:surface','tidal_water'], // Tidal Water

    // ETS - Extraction Specification
    ['ETS','-999999','source:extraction_specification','unknown'], // No Information
    ['ETS','1','source:extraction_specification','1aa-tpc'], // 1AA-TPC
    ['ETS','2','source:extraction_specification','1ab-onc'], // 1AB-ONC
    ['ETS','3','source:extraction_specification','1ae-jog-a/g'], // 1AE-JOG-A/G
    ['ETS','4','source:extraction_specification','1cd-dted-1'], // 1CD-DTED-1
    ['ETS','5','source:extraction_specification','1ce-dfad-1'], // 1CE-DFAD-1
    ['ETS','6','source:extraction_specification','1cf-dted-2'], // 1CF-DTED-2
    ['ETS','7','source:extraction_specification','1cg-dfad-2'], // 1CG-DFAD-2
    ['ETS','8','source:extraction_specification','2aa/001-hac-1'], // 2AA/001-HAC-1
    ['ETS','9','source:extraction_specification','2aa/002-hac-2'], // 2AA/002-HAC-2
    ['ETS','10','source:extraction_specification','2aa/003-hac-3'], // 2AA/003-HAC-3
    ['ETS','11','source:extraction_specification','2aa/004-hac-4'], // 2AA/004-HAC-4
    ['ETS','12','source:extraction_specification','2aa/005-hac-5'], // 2AA/005-HAC-5
    ['ETS','13','source:extraction_specification','2aa/006-hac-6'], // 2AA/006-HAC-6
    ['ETS','14','source:extraction_specification','2aa/007-hac-7'], // 2AA/007-HAC-7
    ['ETS','15','source:extraction_specification','2aa/008-hac-8'], // 2AA/008-HAC-8
    ['ETS','16','source:extraction_specification','2aa/009-hac-9'], // 2AA/009-HAC-9
    ['ETS','17','source:extraction_specification','2ad-combat'], // 2AD-Combat
    ['ETS','18','source:extraction_specification','3aa-tlm50'], // 3AA-TLM50
    ['ETS','19','source:extraction_specification','3ag-tlm100'], // 3AG-TLM100
    ['ETS','20','source:extraction_specification','3ka-vitd'], // 3KA-VITD
    ['ETS','21','source:extraction_specification','3kc/001-dtop'], // 3KC/001-DTOP
    ['ETS','22','source:extraction_specification','3kh-vmap-2'], // 3KH-VMap-2
    ['ETS','23','source:extraction_specification','3kl-vmap-0'], // 3KL-VMap-0
    ['ETS','24','source:extraction_specification','3km-vmap-1'], // 3KM-VMap-1
    ['ETS','25','source:extraction_specification','3ku-uvmap'], // 3KU-UVMap
    ['ETS','26','source:extraction_specification','4aa-atc'], // 4AA-ATC
    ['ETS','27','source:extraction_specification','4ac-jog-r'], // 4AC-JOG-R
    ['ETS','28','source:extraction_specification','4ge-tercom-l'], // 4GE-TERCOM-L
    ['ETS','29','source:extraction_specification','4gf-tercom-e'], // 4GF-TERCOM-E
    ['ETS','30','source:extraction_specification','4gg-tercom-t'], // 4GG-TERCOM-T
    ['ETS','31','source:extraction_specification','5ee-ffd'], // 5EE-FFD
    ['ETS','43','source:extraction_specification','dnc'], // DNC
    ['ETS','44','source:extraction_specification','msd1'], // MSD1
    ['ETS','45','source:extraction_specification','msd2'], // MSD2
    ['ETS','46','source:extraction_specification','msd3'], // MSD3
    ['ETS','47','source:extraction_specification','msd4'], // MSD4
    ['ETS','48','source:extraction_specification','msd5'], // MSD5
    ['ETS','49','source:extraction_specification','tod0'], // TOD0
    ['ETS','50','source:extraction_specification','tod1'], // TOD1
    ['ETS','51','source:extraction_specification','tod2'], // TOD2
    ['ETS','52','source:extraction_specification','tod3'], // TOD3
    ['ETS','53','source:extraction_specification','tod4'], // TOD4
    ['ETS','56','source:extraction_specification','dfeg'], // DFEG
    ['ETS','57','source:extraction_specification','gtds-eg'], // GTDS-EG
    ['ETS','58','source:extraction_specification','rtds-eg'], // RTDS-EG
    ['ETS','59','source:extraction_specification','ltds-eg'], // LTDS-EG
    ['ETS','60','source:extraction_specification','s-utds-eg'], // S-UTDS-EG
    ['ETS','61','source:extraction_specification','mgcp_trd'], // MGCP TRD
    ['ETS','998','source:extraction_specification','not_applicable'], // Not Applicable
    ['ETS','999','source:extraction_specification','other'], // Other

    // ETY - Engine Test Cell Type
    ['ETY','-999999','test_cell:type','unknown'], // No Information
    ['ETY','1','test_cell:type','jet_engine_dismounted'], // Jet Engine Dismounted
    ['ETY','2','test_cell:type','jet_engine_mounted'], // Jet Engine Mounted
    ['ETY','3','test_cell:type','rocket_engine_upright'], // Rocket Engine Upright
    ['ETY','4','test_cell:type','rocket_engine_horizontal'], // Rocket Engine Horizontal
    ['ETY','999','test_cell:type','other'], // Other

    // FAC - Solid Maritime Construction
    ['FAC','-999999','solid_construction','unknown'], // No Information
    ['FAC','1000','solid_construction','no'],
    ['FAC','1001','solid_construction','yes'],

    // FCO - Feature Configuration
    ['FCO','-999999','feature_configuration','unknown'], // No Information
    ['FCO','5','feature_configuration','same'],
    ['FCO','6','feature_configuration','different'],
    ['FCO','7','feature_configuration','non_divided'], // Ugly

    // FCS - Flood Control Structure Type
    ['FCS','-999999','flood_control:type','unknown'], // No Information
    ['FCS','1','flood_control:type','dyke_gate'], // Dyke Gate
    ['FCS','2','flood_control:type','emergency_gate'], // Emergency Gate
    ['FCS','3','flood_control:type','flood_gate'], // Flood Gate
    ['FCS','4','flood_control:type','fixed_barrage'], // Fixed Barrage
    ['FCS','5','flood_control:type','movable_barrage'], // Movable Barrage
    ['FCS','999','flood_control:type','other'], // Other

    // FER - Ferry Crossing Type
    ['FER','-999999','ferry:type','unknown'], // No Information
    ['FER','1','ferry:type','cable'], // Cable
    ['FER','2','ferry:type','free-moving'], // Free-moving
    ['FER','3','ferry:type','ice'], // Ice
    ['FER','999','ferry:type','other'], // Other

    // FFN - Feature Function
    ['FFN','-999999','use','unknown'], // No Information
    ['FFN','2','use','agriculture'], // Agriculture
    ['FFN','3','use','growing_crops'], // Growing of Crops
    ['FFN','9','use','raising_animals'], // Raising of Animals
    ['FFN','14','use','grazing'], // Grazing
    ['FFN','15','use','mixed_farming'], // Mixed Farming
    ['FFN','16','use','migrant_labour'], // Migrant Labour
    ['FFN','19','amenity','hunting'], // Hunting
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
    ['FFN','382','toilets','yes'], // Restroom
    ['FFN','383','use','waste_treatment_and_disposal'], // Waste Treatment and Disposal
    ['FFN','385','use','materials_recovery'], // Materials Recovery
    ['FFN','440','use','commercial'], // Commerce
    ['FFN','459','shop','wholesale'], // Wholesale Merchant
    // ['FFN','460','shop','yes'], // Retail Sale
    ['FFN','460','use','retail_sale'], // Retail Sale
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
    ['FFN','484','use','transfer_hub'], // Transfer Hub
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
    ['FFN','549','use','long-term_accommodation'], // Long-term Accommodation
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
    ['FFN','915','leisure','water_park'], // Water Park
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

    // FFN2 - Feature Function [2]
    // FFN3 - Feature Function [3]

    // FHC - Harbour Facility Function
    ['FHC','-999999','harbour:use','unknown'], // No Information
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

    // FHC2 - Harbour Facility Function [2]
    // FHC3 - Harbour Facility Function [3]

    // FIC - Embankment Type
    ['FIC','-999999','embankment','unknown'], // No Information
    ['FIC','1','embankment','mound'], // Mound
    ['FIC','2','embankment','fill'], // Fill
    ['FIC','3','man-made','dyke'], // Dyke
    ['FIC','5','embankment','levee'], // Levee
    ['FIC','6','embankment','divider'], // Divider
    ['FIC','999','embankment','other'], // Other

    // FLO - Floating
    ['FLO','-999999','floating','unknown'], // No Information
    ['FLO','1000','floating','no'],
    ['FLO','1001','floating','yes'],

    // FPT - Airfield Type
    ['FPT','-999999','aeroway:airfield:type','unknown'], // No Information
    ['FPT','1','aeroway:airfield:type','major'], // Major
    ['FPT','2','aeroway:airfield:type','minor_and_hard'], // Minor and Hard
    ['FPT','3','aeroway:airfield:type','minor_and_soft'], // Minor and Soft
    ['FPT','4','aeroway:airfield:type','minor'], // Minor
    ['FPT','999','aeroway:airfield:type','other'], // Other

    // FRT - Weapons Range Type
    ['FRT','-999999','range:type','unknown'], // No Information
    ['FRT','1','range:type','small_arms'], // Small Arms
    ['FRT','2','range:type','tank'], // Tank
    ['FRT','3','range:type','artillery'], // Field Artillery
    ['FRT','4','range:type','grenade'], // Grenade
    ['FRT','5','range:type','demolition'], // Demolition Area
    ['FRT','6','range:type','impact_area'], // Impact Area
    ['FRT','999','range:type','other'], // Other

    // FRT2 - Weapons Range Type [2]
    // FRT3 - Weapons Range Type [3]

    // FSC - Flight Strip Capable
    ['FSC','-999999','road:runway','unknown'], // No Information
    ['FSC','1000','road:runway','no'],
    ['FSC','1001','road:runway','yes'],

    // FTI - Fence Type
    ['FTI','-999999','fence:type','unknown'], // No Information
    ['FTI','1','fence:type','metal'], // Metal
    ['FTI','2','fence:type','wood'], // Wood
    ['FTI','5','fence:type','barbed_wire'], // Barbed Wire
    ['FTI','6','fence:type','chain-link'], // Chain-link
    ['FTI','7','fence:type','electrified_wire'], // Electrified Wire
    ['FTI','8','fence:type','geotextile'], // Geotextile
    ['FTI','9','fence:type','netting'], // Netting
    ['FTI','999','fence:type','other'], // Other

    // FZR - Fortified Building Type
    ['FZR','-999999','bunker_type','unknown'], // No Information
    ['FZR','1','bunker_type','blockhouse'], // Blockhouse
    ['FZR','2','bunker_type','casement'], // Casement
    ['FZR','3','defensive','keep'], // Keep
    ['FZR','4','defensive','martello_tower'], // Martello Tower
    ['FZR','5','fortified:type','non-specific'], // Non-specific Fortified
    ['FZR','6','bunker_type','pillbox'], // Pillbox
    ['FZR','999','fortified:type','other'], // Other

    // GEC - Geopolitical Entity Type
    ['GEC','-999999','geopolitical_entity','unknown'], // No Information
    ['GEC','5','geopolitical_entity','area_of_no_sovereignty'], // Area of No Sovereignty
    ['GEC','7','geopolitical_entity','demilitarized_zone'], // Demilitarized Zone (DMZ)
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
    ['GEC','999','geopolitical_entity','other'], // Other

    // GFT - Geologic Fault Trace Visible
    ['GFT','-999999','visible_fault','unknown'], // No Information
    ['GFT','1000','visible_fault','no'],
    ['GFT','1001','visible_fault','yes'],

    // GOT - Geothermal Outlet Type
    ['GOT','-999999','natural','unknown'], // No Information
    ['GOT','1','natural','fissure'], // Fissure
    ['GOT','2','natural','fumarole'], // Fumarole
    ['GOT','3','natural','hot_spring'], // Hot Spring
    ['GOT','4','natural','sulphur_spring'], // Sulphur Spring
    ['GOT','5','natural','geyser'], // Geyser
    ['GOT','999','natural','other_geothermal_outlet'], // Other

    // GSGCHC - Runway Direction (high end) : Surface Slope (high) (interval closure)
    ['GSGCHC','2','aeroway:runway_highend:slope:closure','open_interval'], // Open Interval
    ['GSGCHC','3','aeroway:runway_highend:slope:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['GSGCHC','4','aeroway:runway_highend:slope:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['GSGCHC','5','aeroway:runway_highend:slope:closure','closed_interval'], // Closed Interval
    ['GSGCHC','6','aeroway:runway_highend:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['GSGCHC','7','aeroway:runway_highend:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['GSGCHC','8','aeroway:runway_highend:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['GSGCHC','9','aeroway:runway_highend:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // GSGCLC - Runway Direction (low end) : Surface Slope (low) (interval closure)
    ['GSGCLC','2','aeroway:runway_lowend:slope:closure','open_interval'], // Open Interval
    ['GSGCLC','3','aeroway:runway_lowend:slope:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['GSGCLC','4','aeroway:runway_lowend:slope:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['GSGCLC','5','aeroway:runway_lowend:slope:closure','closed_interval'], // Closed Interval
    ['GSGCLC','6','aeroway:runway_lowend:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['GSGCLC','7','aeroway:runway_lowend:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['GSGCLC','8','aeroway:runway_lowend:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['GSGCLC','9','aeroway:runway_lowend:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // GTC - Gate Use
    ['GTC','-999999','barrier','unknown'], // No Information
    ['GTC','1','barrier','toll_booth'], // Toll-gate
    ['GTC','2','barrier','crossing'], // Crossing
    ['GTC','3','barrier','entrance'], // Entrance - Not great but the closest thing OSM has to the enterence to a facility
    ['GTC','4','barrier','border_control'], // Border Crossing
    ['GTC','999','barrier','other'], // Other

    // GUG - Guyed
    ['GUG','-999999','guyed','unknown'], // No Information
    ['GUG','1000','guyed','no'],
    ['GUG','1001','guyed','yes'],

    // HAF - Helipad Associated Facility
    ['HAF','-999999','helipad:facility','unknown'], // No Information
    ['HAF','1','helipad:facility','heliport'], // Heliport
    ['HAF','2','helipad:facility','land_aerodrome'], // Land Aerodrome
    ['HAF','3','helipad:facility','hospital'], // Hospital
    ['HAF','4','helipad:facility','non-hospital_building'], // Non-hospital Building
    ['HAF','6','helipad:facility','rig'], // Rig
    ['HAF','9','helipad:facility','offshore_construction'], // Offshore Construction
    ['HAF','10','helipad:facility','water_aerodrome'], // Water Aerodrome
    ['HAF','11','helipad:facility','military_installation'], // Military Installation
    ['HAF','999','helipad:facility','other'], // Other

    // HQC - Hypsography Portrayal Type
    ['HQC','-999999','contour:type','unknown'], // No Information
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

    // HST - Hazard Shelter Intended Use
    ['HST','-999999','shelter:type','unknown'], // No Information
    ['HST','1','shelter:type','bomb'], // Bomb Shelter
    ['HST','2','shelter:type','fallout'], // Fallout Shelter
    ['HST','3','shelter:type','storm'], // Storm Shelter
    ['HST','999','shelter:type','other'], // Other

    // HST2 - Hazard Shelter Intended Use [2]
    // HST3 - Hazard Shelter Intended Use [3]

    // HTP - Hangar Type Category
    ['HTP','-999999','hangar:type','unknown'], // No Information
    ['HTP','1','hangar:type','multi-bay'], // M - Multi-bay
    ['HTP','2','hangar:type','open_end'], // O - Open End
    ['HTP','3','hangar:type','nose_in'], // N - Nose In
    ['HTP','4','hangar:type','underground'], // G - Underground
    ['HTP','5','hangar:type','single_bay'], // S - Single Bay
    ['HTP','6','hangar:type','double_bay'], // D - Double Bay
    ['HTP','7','hangar:type','t-shaped'], // T - T-Shaped

    // HZD - Geodetic Datum
    ['HZD','-999999','source:datum:geodetic','unknown'], // No Information
    ['HZD','2','source:datum:geodetic','adindan_(ethiopia)'], // Adindan (Ethiopia)
    ['HZD','3','source:datum:geodetic','adindan_(sudan)'], // Adindan (Sudan)
    ['HZD','4','source:datum:geodetic','adindan_(mali)'], // Adindan (Mali)
    ['HZD','5','source:datum:geodetic','adindan_(senegal)'], // Adindan (Senegal)
    ['HZD','6','source:datum:geodetic','adindan_(burkina_faso)'], // Adindan (Burkina Faso)
    ['HZD','7','source:datum:geodetic','adindan_(cameroon)'], // Adindan (Cameroon)
    ['HZD','8','source:datum:geodetic','adindan_(mean_value)'], // Adindan (mean value)
    ['HZD','9','source:datum:geodetic','afgooye_(somalia)'], // Afgooye (Somalia)
    ['HZD','10','source:datum:geodetic','antigua_island_astro_1943'], // Antigua Island Astro 1943
    ['HZD','12','source:datum:geodetic','ain_el_abd_1970_(bahrain_island)'], // Ain el Abd 1970 (Bahrain Island)
    ['HZD','13','source:datum:geodetic','ain_el_abd_1970_(saudi_arabia)'], // Ain el Abd 1970 (Saudi Arabia)
    ['HZD','14','source:datum:geodetic','american_samoa_datum_1962'], // American Samoa Datum 1962
    ['HZD','15','source:datum:geodetic','amersfoort_1885/1903_(netherlands)'], // Amersfoort 1885/1903 (Netherlands)
    ['HZD','16','source:datum:geodetic','anna_1_astro_1965_(cocos_islands)'], // Anna 1 Astro 1965 (Cocos Islands)
    ['HZD','17','source:datum:geodetic','approximate_luzon_datum_(philippines)'], // Approximate Luzon Datum (Philippines)
    ['HZD','19','source:datum:geodetic','arc_1950_(botswana)'], // Arc 1950 (Botswana)
    ['HZD','20','source:datum:geodetic','arc_1950_(lesotho)'], // Arc 1950 (Lesotho)
    ['HZD','21','source:datum:geodetic','arc_1950_(malawi)'], // Arc 1950 (Malawi)
    ['HZD','22','source:datum:geodetic','arc_1950_(swaziland)'], // Arc 1950 (Swaziland)
    ['HZD','23','source:datum:geodetic','arc_1950_(zaire)'], // Arc 1950 (Zaire)
    ['HZD','24','source:datum:geodetic','arc_1950_(zambia)'], // Arc 1950 (Zambia)
    ['HZD','25','source:datum:geodetic','arc_1950_(zimbabwe)'], // Arc 1950 (Zimbabwe)
    ['HZD','26','source:datum:geodetic','arc_1950_(burundi)'], // Arc 1950 (Burundi)
    ['HZD','27','source:datum:geodetic','arc_1950_(mean_value)'], // Arc 1950 (mean value)
    ['HZD','29','source:datum:geodetic','arc_1960_(kenya)'], // Arc 1960 (Kenya)
    ['HZD','30','source:datum:geodetic','arc_1960_(tanzania)'], // Arc 1960 (Tanzania)
    ['HZD','31','source:datum:geodetic','arc_1960_(mean_value)'], // Arc 1960 (mean value)
    ['HZD','32','source:datum:geodetic','arc_1935_(africa)'], // Arc 1935 (Africa)
    ['HZD','33','source:datum:geodetic','ascension_island_1958_(ascension_island)'], // Ascension Island 1958 (Ascension Island)
    ['HZD','34','source:datum:geodetic','montserrat_island_astro_1958'], // Montserrat Island Astro 1958
    ['HZD','35','source:datum:geodetic','astro_station_1952_(marcus_island)'], // Astro Station 1952 (Marcus Island)
    ['HZD','36','source:datum:geodetic','astro_beacon_e_(iwo_jima_island)'], // Astro Beacon 'E' (Iwo Jima Island)
    ['HZD','37','source:datum:geodetic','average_terrestrial_system_1977_new_brunswick'], // Average Terrestrial System 1977, New Brunswick
    ['HZD','38','source:datum:geodetic','australian_geod._1966_(australia_and_tasmania_island)'], // Australian Geod. 1966 (Australia and Tasmania Island)
    ['HZD','39','source:datum:geodetic','australian_geod._1984_(australia_and_tasmania_island)'], // Australian Geod. 1984 (Australia and Tasmania Island)
    ['HZD','40','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)'], // Djakarta (Batavia) (Sumatra Island, Indonesia)
    ['HZD','41','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)_with_zero_meridian_djakarta'], // Djakarta (Batavia) (Sumatra Island, Indonesia) with Zero Meridian Djakarta
    ['HZD','42','source:datum:geodetic','bekaa_base_south_end_(lebanon)'], // Bekaa Base South End (Lebanon)
    ['HZD','43','source:datum:geodetic','belgium_1950_system_(lommel_signal_belgium)'], // Belgium 1950 System (Lommel Signal, Belgium)
    ['HZD','44','source:datum:geodetic','bermuda_1957_(bermuda_islands)'], // Bermuda 1957 (Bermuda Islands)
    ['HZD','45','source:datum:geodetic','bissau_(guinea-bissau)'], // Bissau (Guinea-Bissau)
    ['HZD','46','source:datum:geodetic','modified_bjz54_(china)'], // Modified BJZ54 (China)
    ['HZD','47','source:datum:geodetic','bjz54_(a954_beijing_coordinates)_(china)'], // BJZ54 (A954 Beijing Coordinates) (China)
    ['HZD','48','source:datum:geodetic','bogota_observatory_(colombia)'], // Bogota Observatory (Colombia)
    ['HZD','49','source:datum:geodetic','bogota_observatory_(colombia)_with_zero_meridian_bogota'], // Bogota Observatory (Colombia) with Zero Meridian Bogota
    ['HZD','50','source:datum:geodetic','bern_1898_(switzerland)'], // Bern 1898 (Switzerland)
    ['HZD','51','source:datum:geodetic','bern_1898_(switzerland)_with_zero_meridian_bern'], // Bern 1898 (Switzerland) with Zero Meridian Bern
    ['HZD','52','source:datum:geodetic','bukit_rimpah_(bangka_and_belitung_islands_indonesia)'], // Bukit Rimpah (Bangka and Belitung Islands, Indonesia)
    ['HZD','53','source:datum:geodetic','cape_canaveral_(mean_value)'], // Cape Canaveral (mean value)
    ['HZD','54','source:datum:geodetic','campo_inchauspe_(argentina)'], // Campo Inchauspe (Argentina)
    ['HZD','55','source:datum:geodetic','camacupa_base_sw_end_(campo_de_aviacao_angola)'], // Camacupa Base SW End (Campo De Aviacao, Angola)
    ['HZD','56','source:datum:geodetic','canton_astro_1966_(phoenix_islands)'], // Canton Astro 1966 (Phoenix Islands)
    ['HZD','57','source:datum:geodetic','cape_(south_africa)'], // Cape (South Africa)
    ['HZD','58','source:datum:geodetic','camp_area_astro_(camp_mcmurdo_area_antarctica)'], // Camp Area Astro (Camp McMurdo Area, Antarctica)
    ['HZD','59','source:datum:geodetic','s-jtsk'], // S-JTSK
    ['HZD','60','source:datum:geodetic','carthage_(tunisia)'], // Carthage (Tunisia)
    ['HZD','61','source:datum:geodetic','compensation_geodetique_du_quebec_1977'], // Compensation Geodetique du Quebec 1977
    ['HZD','62','source:datum:geodetic','chatham_1971_(chatham_island_new_zealand)'], // Chatham 1971 (Chatham Island, New Zealand)
    ['HZD','63','source:datum:geodetic','chua_astro_(paraguay)'], // Chua Astro (Paraguay)
    ['HZD','64','source:datum:geodetic','corrego_alegre_(brazil)'], // Corrego Alegre (Brazil)
    ['HZD','65','source:datum:geodetic','conakry_pyramid_of_the_service_geographique_(guinea)'], // Conakry Pyramid of the Service Geographique (Guinea)
    ['HZD','66','source:datum:geodetic','guyana_csg67'], // Guyana CSG67
    ['HZD','67','source:datum:geodetic','dabola_(guinea)'], // Dabola (Guinea)
    ['HZD','68','source:datum:geodetic','dcs-3_lighthouse_saint_lucia_lesser_antilles'], // DCS-3 Lighthouse, Saint Lucia, Lesser Antilles
    ['HZD','69','source:datum:geodetic','deception_island_antarctica'], // Deception Island, Antarctica
    ['HZD','70','source:datum:geodetic','gux_1_astro_(guadacanal_island)'], // GUX 1 Astro (Guadacanal Island)
    ['HZD','71','source:datum:geodetic','dominica_astro_m-12_dominica_lesser_antilles'], // Dominica Astro M-12, Dominica, Lesser Antilles
    ['HZD','72','source:datum:geodetic','easter_island_1967_(easter_island)'], // Easter Island 1967 (Easter Island)
    ['HZD','73','source:datum:geodetic','wake-eniwetok_1960_(marshall_islands)'], // Wake-Eniwetok 1960 (Marshall Islands)
    ['HZD','75','source:datum:geodetic','european_1950_(western_europe)'], // European 1950 (Western Europe)
    ['HZD','76','source:datum:geodetic','european_1950_(greece)'], // European 1950 (Greece)
    ['HZD','77','source:datum:geodetic','european_1950_(norway_and_finland)'], // European 1950 (Norway and Finland)
    ['HZD','78','source:datum:geodetic','european_1950_(portugal_and_spain)'], // European 1950 (Portugal and Spain)
    ['HZD','79','source:datum:geodetic','european_1950_(cyprus)'], // European 1950 (Cyprus)
    ['HZD','80','source:datum:geodetic','european_1950_(egypt)'], // European 1950 (Egypt)
    ['HZD','81','source:datum:geodetic','european_1950_(england_channel_islands_scotland_and_shetland_islands)'], // European 1950 (England, Channel Islands, Scotland, and Shetland Islands)
    ['HZD','82','source:datum:geodetic','european_1950_(iran)'], // European 1950 (Iran)
    ['HZD','83','source:datum:geodetic','european_1950_(sardinia)'], // European 1950 (Sardinia)
    ['HZD','84','source:datum:geodetic','european_1950_(sicily)'], // European 1950 (Sicily)
    ['HZD','85','source:datum:geodetic','european_1950_(british_isles)'], // European 1950 (British Isles)
    ['HZD','86','source:datum:geodetic','european_1950_(malta)'], // European 1950 (Malta)
    ['HZD','87','source:datum:geodetic','european_1950_(mean_value)'], // European 1950 (mean value)
    ['HZD','88','source:datum:geodetic','european_1950_(iraq_israel_jordan_kuwait_lebanon_saudi_arabia_and_syria)'], // European 1950 (Iraq, Israel, Jordan, Kuwait, Lebanon, Saudi Arabia, and Syria)
    ['HZD','89','source:datum:geodetic','european_1950_(tunisia)'], // European 1950 (Tunisia)
    ['HZD','90','source:datum:geodetic','european_1979_(mean_value)'], // European 1979 (mean value)
    ['HZD','91','source:datum:geodetic','european_terrestrial_reference_system_1989_(etrs89)'], // European Terrestrial Reference System 1989 (ETRS89)
    ['HZD','92','source:datum:geodetic','oman_(oman)'], // Oman (Oman)
    ['HZD','93','source:datum:geodetic','observatorio_meteorologico_1939_(corvo_and_flores_islands_azores)'], // Observatorio Meteorologico 1939 (Corvo and Flores Islands, Azores)
    ['HZD','94','source:datum:geodetic','fort_thomas_1955_(nevis_st_kitts_leeward_islands)'], // Fort Thomas 1955 (Nevis, St Kitts, Leeward Islands)
    ['HZD','95','source:datum:geodetic','gan_1970_(addu_atoll_republic_of_maldives)'], // Gan 1970 (Addu Atoll, Republic of Maldives)
    ['HZD','96','source:datum:geodetic','gandajika_base_(zaire)'], // Gandajika Base (Zaire)
    ['HZD','97','source:datum:geodetic','geocentric_datum_of_australia_(gda)'], // Geocentric Datum of Australia (GDA)
    ['HZD','98','source:datum:geodetic','gdz80_(china)'], // GDZ80 (China)
    ['HZD','99','source:datum:geodetic','geodetic_datum_1949_(new_zealand)'], // Geodetic Datum 1949 (New Zealand)
    ['HZD','100','source:datum:geodetic','dos_1968_(gizo_island_new_georgia_islands)'], // DOS 1968 (Gizo Island, New Georgia Islands)
    ['HZD','101','source:datum:geodetic','graciosa_base_sw_(faial_graciosa_pico_sao_jorge_and_terceira_island_azores)'], // Graciosa Base SW (Faial, Graciosa, Pico, Sao Jorge, and Terceira Island, Azores)
    ['HZD','102','source:datum:geodetic','greek_datum_greece'], // Greek Datum, Greece
    ['HZD','103','source:datum:geodetic','greek_geodetic_reference_system_1987_(ggrs_87)'], // Greek Geodetic Reference System 1987 (GGRS 87)
    ['HZD','104','source:datum:geodetic','gunong_segara_(kalimantan_island_indonesia)'], // Gunong Segara (Kalimantan Island, Indonesia)
    ['HZD','105','source:datum:geodetic','gunong_serindung'], // Gunong Serindung
    ['HZD','106','source:datum:geodetic','guam_1963'], // Guam 1963
    ['HZD','107','source:datum:geodetic','herat_north_(afganistan)'], // Herat North (Afganistan)
    ['HZD','108','source:datum:geodetic','hermannskogel'], // Hermannskogel
    ['HZD','109','source:datum:geodetic','provisional_south_chilean_1963_(or_hito_xviii_1963)_(s._chile_53_degrees_south)'], // Provisional South Chilean 1963 (or Hito XVIII 1963) (S. Chile, 53 degrees South)
    ['HZD','110','source:datum:geodetic','hjorsey_1955_(iceland)'], // Hjorsey 1955 (Iceland)
    ['HZD','111','source:datum:geodetic','hong_kong_1963_(hong_kong)'], // Hong Kong 1963 (Hong Kong)
    ['HZD','112','source:datum:geodetic','hong_kong_1929'], // Hong Kong 1929
    ['HZD','113','source:datum:geodetic','hu-tzu-shan'], // Hu-Tzu-Shan
    ['HZD','114','source:datum:geodetic','hungarian_1972'], // Hungarian 1972
    ['HZD','115','source:datum:geodetic','bellevue_(ign)_(efate_and_erromango_islands)'], // Bellevue (IGN) (Efate and Erromango Islands)
    ['HZD','116','source:datum:geodetic','indonesian_1974'], // Indonesian 1974
    ['HZD','118','source:datum:geodetic','indian_(thailand_and_vietnam)'], // Indian (Thailand and Vietnam)
    ['HZD','119','source:datum:geodetic','indian_(bangladesh)'], // Indian (Bangladesh)
    ['HZD','120','source:datum:geodetic','indian_(india_and_nepal)'], // Indian (India and Nepal)
    ['HZD','121','source:datum:geodetic','indian_(pakistan)'], // Indian (Pakistan)
    ['HZD','123','source:datum:geodetic','indian_1954_(thailand)'], // Indian 1954 (Thailand)
    ['HZD','125','source:datum:geodetic','indian_1960_(vietnam:_near_16_degrees_north)'], // Indian 1960 (Vietnam: near 16 degrees North)
    ['HZD','126','source:datum:geodetic','indian_1960_(con_son_island_(vietnam))'], // Indian 1960 (Con Son Island (Vietnam))
    ['HZD','128','source:datum:geodetic','indian_1975_(thailand)'], // Indian 1975 (Thailand)
    ['HZD','129','source:datum:geodetic','ireland_1965_(ireland_and_northern_ireland)'], // Ireland 1965 (Ireland and Northern Ireland)
    ['HZD','130','source:datum:geodetic','ists_061_astro_1968_(south_georgia_islands)'], // ISTS 061 Astro 1968 (South Georgia Islands)
    ['HZD','131','source:datum:geodetic','ists_073_astro_1969_(diego_garcia)'], // ISTS 073 Astro 1969 (Diego Garcia)
    ['HZD','132','source:datum:geodetic','johnston_island_1961_(johnston_island)'], // Johnston Island 1961 (Johnston Island)
    ['HZD','133','source:datum:geodetic','kalianpur_(india)'], // Kalianpur (India)
    ['HZD','134','source:datum:geodetic','kandawala_(sri_lanka)'], // Kandawala (Sri Lanka)
    ['HZD','135','source:datum:geodetic','kertau_1948_(or_revised_kertau)_(west_malaysia_and_singapore)'], // Kertau 1948 (or Revised Kertau) (West Malaysia and Singapore)
    ['HZD','136','source:datum:geodetic','kcs_2_sierra_leone'], // KCS 2, Sierra Leone
    ['HZD','137','source:datum:geodetic','kerguelen_island_1949_(kerguelen_island)'], // Kerguelen Island 1949 (Kerguelen Island)
    ['HZD','138','source:datum:geodetic','korean_geodetic_system_1995_(south_korea)'], // Korean Geodetic System 1995 (South Korea)
    ['HZD','139','source:datum:geodetic','kkj_(or_kartastokoordinaattijarjestelma)_finland'], // KKJ (or Kartastokoordinaattijarjestelma), Finland
    ['HZD','140','source:datum:geodetic','kusaie_astro_1951'], // Kusaie Astro 1951
    ['HZD','141','source:datum:geodetic','kuwait_oil_company_(k28)'], // Kuwait Oil Company (K28)
    ['HZD','142','source:datum:geodetic','l.c._5_astro_1961_(cayman_brac_island)'], // L.C. 5 Astro 1961 (Cayman Brac Island)
    ['HZD','143','source:datum:geodetic','leigon_(ghana)'], // Leigon (Ghana)
    ['HZD','144','source:datum:geodetic','liberia_1964_(liberia)'], // Liberia 1964 (Liberia)
    ['HZD','145','source:datum:geodetic','lisbon_(castelo_di_sao_jorge)_portugal'], // Lisbon (Castelo di Sao Jorge), Portugal
    ['HZD','146','source:datum:geodetic','local_astro'], // Local Astro
    ['HZD','147','source:datum:geodetic','loma_quintana_(venezuela)'], // Loma Quintana (Venezuela)
    ['HZD','149','source:datum:geodetic','luzon_(philipines_except_mindanao_island)'], // Luzon (Philipines except Mindanao Island)
    ['HZD','150','source:datum:geodetic','luzon_(mindanao_island)'], // Luzon (Mindanao Island)
    ['HZD','151','source:datum:geodetic','marco_astro_(salvage_islands)'], // Marco Astro (Salvage Islands)
    ['HZD','152','source:datum:geodetic','martinique_fort-desaix'], // Martinique Fort-Desaix
    ['HZD','153','source:datum:geodetic','massawa_(eritrea_ethiopia)'], // Massawa (Eritrea, Ethiopia)
    ['HZD','154','source:datum:geodetic','manokwari_(west_irian)'], // Manokwari (West Irian)
    ['HZD','155','source:datum:geodetic','mayotte_combani'], // Mayotte Combani
    ['HZD','156','source:datum:geodetic','mount_dillon_tobago'], // Mount Dillon, Tobago
    ['HZD','157','source:datum:geodetic','merchich_(morocco)'], // Merchich (Morocco)
    ['HZD','158','source:datum:geodetic','midway_astro_1961_(midway_island)'], // Midway Astro 1961 (Midway Island)
    ['HZD','159','source:datum:geodetic','mahe_1971_(mahe_island)'], // Mahe 1971 (Mahe Island)
    ['HZD','161','source:datum:geodetic','minna_(cameroon)'], // Minna (Cameroon)
    ['HZD','162','source:datum:geodetic','minna_(nigeria)'], // Minna (Nigeria)
    ['HZD','163','source:datum:geodetic','rome_1940_(or_monte_mario_1940)_italy'], // Rome 1940 (or Monte Mario 1940), Italy
    ['HZD','164','source:datum:geodetic','rome_1940_(or_monte_mario_1940)_italy_with_zero_meridian_rome'], // Rome 1940 (or Monte Mario 1940), Italy, with Zero Meridian Rome
    ['HZD','165','source:datum:geodetic','montjong_lowe'], // Montjong Lowe
    ['HZD','166','source:datum:geodetic','mporaloko_(gabon)'], // M'Poraloko (Gabon)
    ['HZD','167','source:datum:geodetic','viti_levu_1916_(viti_levu_island_fiji_islands)'], // Viti Levu 1916 (Viti Levu Island, Fiji Islands)
    ['HZD','169','source:datum:geodetic','nahrwan_(masirah_island_oman)'], // Nahrwan (Masirah Island, Oman)
    ['HZD','170','source:datum:geodetic','nahrwan_(united_arab_emirates)'], // Nahrwan (United Arab Emirates)
    ['HZD','171','source:datum:geodetic','nahrwan_(saudi_arabia)'], // Nahrwan (Saudi Arabia)
    ['HZD','172','source:datum:geodetic','naparima_(bwi_trinidad_and_tobago)'], // Naparima (BWI, Trinidad and Tobago)
    ['HZD','174','source:datum:geodetic','north_american_1983_(alaska_excluding_aleutian_islands)'], // North American 1983 (Alaska, excluding Aleutian Islands)
    ['HZD','175','source:datum:geodetic','north_american_1983_(canada)'], // North American 1983 (Canada)
    ['HZD','176','source:datum:geodetic','north_american_1983_(conus)'], // North American 1983 (CONUS)
    ['HZD','177','source:datum:geodetic','north_american_1983_(mexico_and_central_america))'], // North American 1983 (Mexico and Central America))
    ['HZD','178','source:datum:geodetic','north_american_1983_(aleutian_islands)'], // North American 1983 (Aleutian Islands)
    ['HZD','179','source:datum:geodetic','north_american_1983_(hawaii)'], // North American 1983 (Hawaii)
    ['HZD','181','source:datum:geodetic','north_american_1927_(eastern_us)'], // North American 1927 (Eastern US)
    ['HZD','182','source:datum:geodetic','north_american_1927_(western_us)'], // North American 1927 (Western US)
    ['HZD','183','source:datum:geodetic','north_american_1927_(conus_mean)'], // North American 1927 (CONUS mean)
    ['HZD','184','source:datum:geodetic','north_american_1927_(alaska)'], // North American 1927 (Alaska)
    ['HZD','185','source:datum:geodetic','north_american_1927_(canada_mean)'], // North American 1927 (Canada mean)
    ['HZD','186','source:datum:geodetic','north_american_1927_(alberta_and_british_columbia)'], // North American 1927 (Alberta and British Columbia)
    ['HZD','187','source:datum:geodetic','north_american_1927_(newfoundland_new_brunswick_nova_scotia_and_quebec)'], // North American 1927 (Newfoundland, New Brunswick, Nova Scotia and Quebec)
    ['HZD','188','source:datum:geodetic','north_american_1927_(manitoba_and_ontario)'], // North American 1927 (Manitoba and Ontario)
    ['HZD','189','source:datum:geodetic','north_american_1927_(northwest_territories_and_saskatchewan)'], // North American 1927 (Northwest Territories and Saskatchewan)
    ['HZD','190','source:datum:geodetic','north_american_1927_(yukon)'], // North American 1927 (Yukon)
    ['HZD','191','source:datum:geodetic','north_american_1927_(mexico)'], // North American 1927 (Mexico)
    ['HZD','192','source:datum:geodetic','north_american_1927_(central_america)'], // North American 1927 (Central America)
    ['HZD','193','source:datum:geodetic','north_american_1927_(canal_zone)'], // North American 1927 (Canal Zone)
    ['HZD','194','source:datum:geodetic','north_american_1927_(caribbean)'], // North American 1927 (Caribbean)
    ['HZD','195','source:datum:geodetic','north_american_1927_(bahamas_except_san_salvador_island)'], // North American 1927 (Bahamas, except San Salvador Island)
    ['HZD','196','source:datum:geodetic','north_american_1927_(san_salvador_island)'], // North American 1927 (San Salvador Island)
    ['HZD','197','source:datum:geodetic','north_american_1927_(cuba)'], // North American 1927 (Cuba)
    ['HZD','198','source:datum:geodetic','north_american_1927_(hayes_peninsula_greenland)'], // North American 1927 (Hayes Peninsula, Greenland)
    ['HZD','199','source:datum:geodetic','north_american_1927_(aleutian_islands_east_of_180_degrees_west)'], // North American 1927 (Aleutian Islands East of 180 degrees West)
    ['HZD','200','source:datum:geodetic','north_american_1927_(aleutian_islands_west_of_180_degrees_west)'], // North American 1927 (Aleutian Islands West of 180 degrees West)
    ['HZD','201','source:datum:geodetic','revised_nahrwan'], // Revised Nahrwan
    ['HZD','202','source:datum:geodetic','new_french_or_nouvelle_triangulation_francaise_(ntf)_with_zero_meridian_paris'], // New French or Nouvelle Triangulation Francaise (NTF) with Zero Meridian Paris
    ['HZD','204','source:datum:geodetic','north_sahara_1959'], // North Sahara 1959
    ['HZD','205','source:datum:geodetic','ocotopeque_guatemala'], // Ocotopeque, Guatemala
    ['HZD','206','source:datum:geodetic','belgium_1972_(observatoire_duccle)'], // Belgium 1972 (Observatoire d'Uccle)
    ['HZD','207','source:datum:geodetic','old_egyptian_(egypt)'], // Old Egyptian (Egypt)
    ['HZD','209','source:datum:geodetic','ordnance_survey_g.b._1936_(england)'], // Ordnance Survey G.B. 1936 (England)
    ['HZD','210','source:datum:geodetic','ordnance_survey_g.b._1936_(england_isle_of_man_and_wales)'], // Ordnance Survey G.B. 1936 (England, Isle of Man, and Wales)
    ['HZD','211','source:datum:geodetic','ordnance_survey_g.b._1936_(scotland_and_shetland_islands)'], // Ordnance Survey G.B. 1936 (Scotland and Shetland Islands)
    ['HZD','212','source:datum:geodetic','ordnance_survey_g.b._1936_(wales)'], // Ordnance Survey G.B. 1936 (Wales)
    ['HZD','213','source:datum:geodetic','ordnance_survey_g.b._1936_(mean_value)'], // Ordnance Survey G.B. 1936 (mean value)
    ['HZD','215','source:datum:geodetic','old_hawaiian_(hawaii)'], // Old Hawaiian (Hawaii)
    ['HZD','216','source:datum:geodetic','old_hawaiian_(kauai)'], // Old Hawaiian (Kauai)
    ['HZD','217','source:datum:geodetic','old_hawaiian_(maui)'], // Old Hawaiian (Maui)
    ['HZD','218','source:datum:geodetic','old_hawaiian_(oahu)'], // Old Hawaiian (Oahu)
    ['HZD','219','source:datum:geodetic','old_hawaiian_(mean_value)'], // Old Hawaiian (mean value)
    ['HZD','220','source:datum:geodetic','oslo_observatory_(old)_norway'], // Oslo Observatory (Old), Norway
    ['HZD','221','source:datum:geodetic','padang_base_west_end_(sumatra_indonesia)'], // Padang Base West End (Sumatra, Indonesia)
    ['HZD','222','source:datum:geodetic','padang_base_west_end_(sumatra_indonesia)_with_zero_meridian_djakarta'], // Padang Base West End (Sumatra, Indonesia) with Zero Meridian Djakarta
    ['HZD','223','source:datum:geodetic','palestine_1928_(israel_jordan)'], // Palestine 1928 (Israel, Jordan)
    ['HZD','224','source:datum:geodetic','potsdam_or_helmertturm_(germany)'], // Potsdam or Helmertturm (Germany)
    ['HZD','225','source:datum:geodetic','ayabelle_lighthouse_(djibouti)'], // Ayabelle Lighthouse (Djibouti)
    ['HZD','226','source:datum:geodetic','pitcairn_astro_1967_(pitcairn_island)'], // Pitcairn Astro 1967 (Pitcairn Island)
    ['HZD','227','source:datum:geodetic','pico_de_las_nieves_(canary_islands)'], // Pico de las Nieves (Canary Islands)
    ['HZD','228','source:datum:geodetic','se_base_(porto_santo)_(porto_santo_and_madeira_islands)'], // SE Base (Porto Santo) (Porto Santo and Madeira Islands)
    ['HZD','230','source:datum:geodetic','prov._s._american_1956_(bolivia)'], // Prov. S. American 1956 (Bolivia)
    ['HZD','231','source:datum:geodetic','prov._s._american_1956_(northern_chile_near_19_degrees_south)'], // Prov. S. American 1956 (Northern Chile near 19 degrees South)
    ['HZD','232','source:datum:geodetic','prov._s._american_1956_(southern_chile_near_43_degrees_south)'], // Prov. S. American 1956 (Southern Chile near 43 degrees South)
    ['HZD','233','source:datum:geodetic','prov._s._american_1956_(columbia)'], // Prov. S. American 1956 (Columbia)
    ['HZD','234','source:datum:geodetic','prov._s._american_1956_(ecuador)'], // Prov. S. American 1956 (Ecuador)
    ['HZD','235','source:datum:geodetic','prov._s._american_1956_(guyana)'], // Prov. S. American 1956 (Guyana)
    ['HZD','236','source:datum:geodetic','prov._s._american_1956_(peru)'], // Prov. S. American 1956 (Peru)
    ['HZD','237','source:datum:geodetic','prov._s._american_1956_(venezuela)'], // Prov. S. American 1956 (Venezuela)
    ['HZD','238','source:datum:geodetic','prov._s._american_1956_(mean_value)'], // Prov. S. American 1956 (mean value)
    ['HZD','239','source:datum:geodetic','point_58_mean_solution_(burkina_faso_and_niger)'], // Point 58 Mean Solution (Burkina Faso and Niger)
    ['HZD','240','source:datum:geodetic','pointe_noire_1948'], // Pointe Noire 1948
    ['HZD','241','source:datum:geodetic','pulkovo_1942_(russia)'], // Pulkovo 1942 (Russia)
    ['HZD','242','source:datum:geodetic','puerto_rico_(puerto_rico_and_virgin_islands)'], // Puerto Rico (Puerto Rico and Virgin Islands)
    ['HZD','243','source:datum:geodetic','qatar_national_(qatar)'], // Qatar National (Qatar)
    ['HZD','244','source:datum:geodetic','qornoq_(south_greenland)'], // Qornoq (South Greenland)
    ['HZD','245','source:datum:geodetic','rauenberg_(berlin_germany)'], // Rauenberg (Berlin, Germany)
    ['HZD','246','source:datum:geodetic','reconnaissance_triangulation_morocco'], // Reconnaissance Triangulation, Morocco
    ['HZD','247','source:datum:geodetic','reunion_1947'], // Reunion 1947
    ['HZD','248','source:datum:geodetic','rt90_stockholm_sweden'], // RT90, Stockholm, Sweden
    ['HZD','249','source:datum:geodetic','santo_(dos)_1965_(espirito_santo_island)'], // Santo (DOS) 1965 (Espirito Santo Island)
    ['HZD','250','source:datum:geodetic','south_african_(south_africa)'], // South African (South Africa)
    ['HZD','251','source:datum:geodetic','sainte_anne_i_1984_(guadeloupe)'], // Sainte Anne I 1984 (Guadeloupe)
    ['HZD','253','source:datum:geodetic','south_american_1969_(argentina)'], // South American 1969 (Argentina)
    ['HZD','254','source:datum:geodetic','south_american_1969_(bolivia)'], // South American 1969 (Bolivia)
    ['HZD','255','source:datum:geodetic','south_american_1969_(brazil)'], // South American 1969 (Brazil)
    ['HZD','256','source:datum:geodetic','south_american_1969_(chile)'], // South American 1969 (Chile)
    ['HZD','257','source:datum:geodetic','south_american_1969_(columbia)'], // South American 1969 (Columbia)
    ['HZD','258','source:datum:geodetic','south_american_1969_(ecuador)'], // South American 1969 (Ecuador)
    ['HZD','259','source:datum:geodetic','south_american_1969_(guyana)'], // South American 1969 (Guyana)
    ['HZD','260','source:datum:geodetic','south_american_1969_(paraguay)'], // South American 1969 (Paraguay)
    ['HZD','261','source:datum:geodetic','south_american_1969_(peru)'], // South American 1969 (Peru)
    ['HZD','262','source:datum:geodetic','south_american_1969_(baltra_galapagos_islands)'], // South American 1969 (Baltra, Galapagos Islands)
    ['HZD','263','source:datum:geodetic','south_american_1969_(trinidad_and_tobago)'], // South American 1969 (Trinidad and Tobago)
    ['HZD','264','source:datum:geodetic','south_american_1969_(venezuela)'], // South American 1969 (Venezuela)
    ['HZD','265','source:datum:geodetic','south_american_1969_(mean_value)'], // South American 1969 (mean value)
    ['HZD','266','source:datum:geodetic','sao_braz_(sao_miguel_santa_maria_islands_azores)'], // Sao Braz (Sao Miguel, Santa Maria Islands, Azores)
    ['HZD','267','source:datum:geodetic','sapper_hill_1943_(east_falkland_islands)'], // Sapper Hill 1943 (East Falkland Islands)
    ['HZD','268','source:datum:geodetic','schwarzeck_(namibia)'], // Schwarzeck (Namibia)
    ['HZD','269','source:datum:geodetic','soviet_geodetic_system_1985'], // Soviet Geodetic System 1985
    ['HZD','270','source:datum:geodetic','soviet_geodetic_system_1990'], // Soviet Geodetic System 1990
    ['HZD','271','source:datum:geodetic','selvagem_grande_1938_(salvage_islands)'], // Selvagem Grande 1938 (Salvage Islands)
    ['HZD','272','source:datum:geodetic','astro_dos_71/4_(st._helena_island)'], // Astro DOS 71/4 (St. Helena Island)
    ['HZD','273','source:datum:geodetic','sierra_leone_1960'], // Sierra Leone 1960
    ['HZD','274','source:datum:geodetic','south_asia_(southeast_asia_singapore)'], // South Asia (Southeast Asia, Singapore)
    ['HZD','276','source:datum:geodetic','st._pierre_et_miquelon_1950'], // St. Pierre et Miquelon 1950
    ['HZD','277','source:datum:geodetic','stockholm_1938_(sweden)'], // Stockholm 1938 (Sweden)
    ['HZD','278','source:datum:geodetic','sydney_observatory_new_south_wales_australia'], // Sydney Observatory, New South Wales, Australia
    ['HZD','279','source:datum:geodetic','tananarive_observatory_1925'], // Tananarive Observatory 1925
    ['HZD','280','source:datum:geodetic','tananarive_observatory_1925_with_zero_meridian_paris'], // Tananarive Observatory 1925, with Zero Meridian Paris
    ['HZD','281','source:datum:geodetic','tristan_astro_1968_(tristan_da_cunha)'], // Tristan Astro 1968 (Tristan da Cunha)
    ['HZD','282','source:datum:geodetic','timbalai_1948_(brunei_and_east_malaysia_-_sarawak_and_sabah)'], // Timbalai 1948 (Brunei and East Malaysia - Sarawak and Sabah)
    ['HZD','283','source:datum:geodetic','timbalai_1968'], // Timbalai 1968
    ['HZD','285','source:datum:geodetic','tokyo_(japan)'], // Tokyo (Japan)
    ['HZD','286','source:datum:geodetic','tokyo_(korea)'], // Tokyo (Korea)
    ['HZD','287','source:datum:geodetic','tokyo_(okinawa)'], // Tokyo (Okinawa)
    ['HZD','288','source:datum:geodetic','tokyo_(mean_value)'], // Tokyo (mean value)
    ['HZD','289','source:datum:geodetic','trinidad_1903'], // Trinidad 1903
    ['HZD','290','source:datum:geodetic','astro_tern_island_1961_(tern_island_hawaii)'], // Astro Tern Island 1961 (Tern Island, Hawaii)
    ['HZD','292','source:datum:geodetic','voirol_1875'], // Voirol 1875
    ['HZD','293','source:datum:geodetic','voirol_1875_with_zero_meridian_paris'], // Voirol 1875 with Zero Meridian Paris
    ['HZD','294','source:datum:geodetic','voirol_1960_algeria'], // Voirol 1960, Algeria
    ['HZD','295','source:datum:geodetic','voirol_1960_algeria_with_zero_meridian_paris'], // Voirol 1960, Algeria, with Zero Meridian Paris
    ['HZD','296','source:datum:geodetic','wake_island_astro_1952'], // Wake Island Astro 1952
    ['HZD','297','source:datum:geodetic','world_geodetic_system_1960'], // World Geodetic System 1960
    ['HZD','298','source:datum:geodetic','world_geodetic_system_1966'], // World Geodetic System 1966
    ['HZD','299','source:datum:geodetic','world_geodetic_system_1972'], // World Geodetic System 1972
    ['HZD','300','source:datum:geodetic','world_geodetic_system_1984'], // World Geodetic System 1984
    ['HZD','301','source:datum:geodetic','yacare_(uruguay)'], // Yacare (Uruguay)
    ['HZD','302','source:datum:geodetic','zanderij_(surinam)'], // Zanderij (Surinam)
    ['HZD','305','source:datum:geodetic','co-ordinate_system_1937_of_estonia'], // Co-ordinate System 1937 of Estonia
    ['HZD','306','source:datum:geodetic','indian_1975_(thailand)_-_cycle_1'], // Indian 1975 (Thailand) - Cycle 1
    ['HZD','307','source:datum:geodetic','south_american_geocentric_reference_system_(sirgas)'], // South American Geocentric Reference System (SIRGAS)
    ['HZD','308','source:datum:geodetic','pulkovo_1942_(hungary)'], // Pulkovo 1942 (Hungary)
    ['HZD','309','source:datum:geodetic','pulkovo_1942_(poland)'], // Pulkovo 1942 (Poland)
    ['HZD','310','source:datum:geodetic','pulkovo_1942_(czechoslovakia)'], // Pulkovo 1942 (Czechoslovakia)
    ['HZD','311','source:datum:geodetic','pulkovo_1942_(latvia)'], // Pulkovo 1942 (Latvia)
    ['HZD','312','source:datum:geodetic','pulkovo_1942_(kazakhstan)'], // Pulkovo 1942 (Kazakhstan)
    ['HZD','313','source:datum:geodetic','pulkovo_1942_(albania)'], // Pulkovo 1942 (Albania)
    ['HZD','314','source:datum:geodetic','pulkovo_1942_(romania)'], // Pulkovo 1942 (Romania)
    ['HZD','315','source:datum:geodetic','tokyo_(korea)_-_cycle_1'], // Tokyo (Korea) - Cycle 1
    ['HZD','998','source:datum:geodetic','not_applicable'], // Not Applicable
    ['HZD','999','source:datum:geodetic','other'], // Other

    // INU - Inundation Type
    ['INU','-999999','inundation','unknown'], // No Information
    ['INU','1','inundation','controlled'], // Controlled
    ['INU','2','inundation','natural'], // Natural

    // IWT - Inland Water Type
    ['IWT','-999999','water','unknown'], // No Information
    ['IWT','1','water','lake'], // Lake
    ['IWT','2','water','pond'], // Pond
    ['IWT','3','water','undifferentiated_water_body'], // Undifferentiated Water Body
    ['IWT','4','water','reservoir'], // Reservoir
    ['IWT','5','landuse','basin'], // Basin
    ['IWT','6','natural','waterhole'], // Water-hole
    ['IWT','7','water','landlocked_sea'], // Landlocked Sea
    ['IWT','999','water','other'], // Other

    // LMC - Navigation Landmark
    ['LMC','-999999','navigation:landmark','unknown'], // No Information
    ['LMC','1000','navigation:landmark','no'],
    ['LMC','1001','navigation:landmark','yes'],

    // LND - Land Morphology
    ['LND','-999999','land:morphology','unknown'], // No Information
    ['LND','19','land:morphology','dome'], // Dome
    ['LND','41','land:morphology','ridge'], // Ridge
    ['LND','56','land:morphology','depression'], // Depression
    ['LND','999','land:morphology','other'], // Other

    // LOC - Vertical Relative Location
    ['LOC','-999999','location','unknown'], // No Information
    ['LOC','17','location','on_waterbody_bottom'], // On Waterbody Bottom
    ['LOC','23','location','below_waterbody_bottom'], // Below Waterbody Bottom
    ['LOC','40','location','underground'], // Below Ground Surface
    ['LOC','44','location','surface'], // The default is on the surface // On Surface
    ['LOC','45','location','overground'], // Above Surface
    ['LOC','46','location','above_waterbody_bottom'], // Above Waterbody Bottom
    ['LOC','47','location','underwater'], // Below Waterbody Surface

    // LSP - Geopolitical Line Type
    ['LSP','-999999','geopolitical_line','unknown'], // No Information
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

    // LUN - Located Underground
    ['LUN','-999999','underground','unknown'], // No Information
    ['LUN','1000','underground','no'],
    ['LUN','1001','underground','yes'],

    // MAN - Maritime Navigation Marked
    ['MAN','-999999','navigation:maritime_landmark','unknown'], // No Information
    ['MAN','1000','navigation:maritime_landmark','no'],
    ['MAN','1001','navigation:maritime_landmark','yes'],

    // MCC - Structural Material Type
    ['MCC','-999999','material','unknown'], // No Information
    ['MCC','2','material','aluminium'], // Aluminum
    ['MCC','5','material','asphalt'], // Asphalt
    ['MCC','7','material','bedrock'], // Bedrock
    ['MCC','8','material','boulders'], // Boulders
    ['MCC','9','material','brick'], // Brick
    ['MCC','21','material','concrete'], // Concrete
    ['MCC','24','material','coral'], // Coral
    ['MCC','46','material','gravel'], // Gravel
    ['MCC','51','material','iron'], // Iron
    ['MCC','52','material','lava'], // Lava
    ['MCC','55','material','loess'], // Loess
    ['MCC','62','material','masonry'], // Masonry
    ['MCC','64','material','metal'], // Metal
    ['MCC','65','material','mud'], // Mud
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
    ['MCC','120','material','glass_reinforced_plastic_(grp)'], // Glass Reinforced Plastic (GRP)
    ['MCC','131','material','ceramic'], // Ceramic
    ['MCC','150','material','treated_timber'], // Treated Timber
    ['MCC','152','material','fibreglass'], // Fibreglass
    ['MCC','999','material','other'], // Other

    // MCC2 - Structural Material Type [2]
    // MCC3 - Structural Material Type [3]

    // MCX - Motorized Crossing
    ['MCX','-999999','ferry:motorized','unknown'], // No Information
    ['MCX','1000','ferry:motorized','no'],
    ['MCX','1001','ferry:motorized','yes'],

    // MES - Median Present
    ['MES','-999999','median','unknown'], // No Information
    ['MES','1000','median','no'],
    ['MES','1001','median','yes'],

    // MGL - Maritime Geopolitical Limit Type
    ['MGL','-999999','geopolitical_line:maritime','unknown'], // No Information
    ['MGL','1','geopolitical_line:maritime','international_boundary'], // International Boundary
    ['MGL','2','geopolitical_line:maritime','territorial_sea_baseline'], // Territorial Sea Baseline
    ['MGL','3','geopolitical_line:maritime','three_nautical_mile_limit'], // Three Nautical Mile Limit
    ['MGL','4','geopolitical_line:maritime','territorial_waters_limit_of_sovereignty'], // Territorial Waters Limit of Sovereignty
    ['MGL','5','geopolitical_line:maritime','contiguous_zone_limit'], // Contiguous Zone Limit
    ['MGL','6','geopolitical_line:maritime','continental_shelf_limit'], // Continental Shelf Limit
    ['MGL','7','geopolitical_line:maritime','extended_continental_shelf_limit'], // Extended Continental Shelf Limit
    ['MGL','8','geopolitical_line:maritime','exclusive_economic_zone_limit'], // Exclusive Economic Zone Limit
    ['MGL','9','geopolitical_line:maritime','customs_boundary'], // Customs Boundary
    ['MGL','999','geopolitical_line:maritime','other'], // Other


    // MNS - Man-made Shoreline
    ['MNS','-999999','man_made:shoreline','unknown'], // No Information
    ['MNS','1000','man_made:shoreline','no'],
    ['MNS','1001','man_made:shoreline','yes'],

    // MZN - Extraction Mine Type
    ['MZN','-999999','mine:type','unknown'], // No Information
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
    ['MZN','13','mine:type','dredge'], // Dredge
    ['MZN','999','mine:type','other'], // Other

    // NAV_TYPE - NAV_TYPE
    ['NAV_TYPE','-999999','beacon:type','unknown'], // noInformation
    ['NAV_TYPE','1','beacon:type','vor'], // VOR
    ['NAV_TYPE','2','beacon:type','vortac'], // VORTAC
    ['NAV_TYPE','3','beacon:type','tacan'], // TACAN
    ['NAV_TYPE','4','beacon:type','vor-dme'], // VOR-DME
    ['NAV_TYPE','5','beacon:type','ndb'], // NDB
    ['NAV_TYPE','7','beacon:type','ndb-dme'], // NDB-DME
    ['NAV_TYPE','9','beacon:type','dme'], // DME

    // NLT - Named Location Type
    ['NLT','-999999','named_location','unknown'], // No Information
    ['NLT','1','named_location:type','area'], // Area
    ['NLT','2','named_location:type','locality'], // Locality
    ['NLT','3','named_location:type','region'], // Region
    ['NLT','4','named_location:type','arctic_land'], // Arctic Land
    ['NLT','5','named_location:type','populated_locality'], // Populated Locality
    ['NLT','999','named_location:type','other'], // Other

    // NVS - Navigability Information
    ['NVS','-999999','navigation','unknown'], // No Information
    ['NVS','1','navigation','yes_operational'], // Navigable and Operational
    ['NVS','2','navigation','yes_abandoned'], // Navigable but Abandoned
    ['NVS','3','navigation','yes'], // Navigable
    ['NVS','4','navigation','yes_restrictions'], // Navigable with Periodic Restrictions
    ['NVS','5','navigation','no'], // Not Navigable
    ['NVS','999','navigation','other'], // Other

    // OCS - Offshore Construction Primary Structure
    ['OCS','-999999','structure:type','unknown'], // No Information
    ['OCS','1','structure:type','platform'], // Platform
    ['OCS','2','structure:type','terminal_buoy'], // Terminal Buoy
    ['OCS','3','structure:type','catenary_turntable'], // Catenary Turntable
    ['OCS','4','structure:type','submerged_turret'], // Submerged Turret
    ['OCS','5','structure:type','vessel'], // Vessel
    ['OCS','6','structure:type','barge'], // Barge
    ['OCS','7','structure:type','submerged_platform'], // Submerged Platform
    ['OCS','999','structure:type','other'], // Other

    // ONE - One-way
    ['ONE','-999999','oneway','unknown'], // No Information
    ['ONE','1000','oneway','no'], // Value from OSM
    ['ONE','1001','oneway','yes'],

    // OOC - Overhead Obstruction Type
    ['OOC','-999999','overhead_obstruction:type','unknown'], // No Information
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
    ['OPT','-999999','operating_cycle','unknown'], // No Information
    ['OPT','1','operating_cycle','daytime'], // Daytime
    ['OPT','2','operating_cycle','night-time'], // Night-time
    ['OPT','3','operating_cycle','continuous'], // Continuously Operating
    ['OPT','4','operating_cycle','summer_season'], // Summer Season
    ['OPT','5','operating_cycle','winter_season'], // Winter Season
    ['OPT','6','operating_cycle','restricted'], // Restricted
    ['OPT','7','operating_cycle','never_operating'], // Never Operating
    ['OPT','999','operating_cycle','other'], // Other

    // OWO - Waterbody Overhead Obstruction
    ['OWO','-999999','waterway:overhead_obstruction','unknown'], // No Information
    ['OWO','1000','waterway:overhead_obstruction','no'],
    ['OWO','1001','waterway:overhead_obstruction','yes'],

    // PBY - By-product See ZI014_PBY
    // ['PBY','-999999','raw:PBY','unknown'], // noInformation
    // ['PBY','8','raw:PBY','refuse'], // Refuse
    // ['PBY','15','raw:PBY','slag'], // Slag
    // ['PBY','16','raw:PBY','sludge'], // Sludge
    // ['PBY','17','raw:PBY','spoil'], // Spoil
    // ['PBY','999','raw:PBY','other'], // Other

    // PBY2 - By-product [2]
    // PBY3 - By-product [3]

    // PCF - Physical Condition
    ['PCF','-999999','condition','unknown'], // No Information
    ['PCF','1','condition','construction'], // Construction
    ['PCF','2','condition','functional'], // Intact in spec, using for MGCP compatibility
    ['PCF','3','condition','abandoned'], // Unmaintained in spec
    ['PCF','4','condition','damaged'], // Damaged
    ['PCF','5','condition','dismantled'], // Dismantled
    ['PCF','6','condition','destroyed'], // Destroyed

    // PEC - Port of Entry
    ['PEC','-999999','port_of_entry','unknown'], // No Information
    ['PEC','1000','port_of_entry','no'],
    ['PEC','1001','port_of_entry','yes'],

    // PLT - Pipeline Type
    ['PLT','-999999','pipeline:type','unknown'], // No Information
    ['PLT','1','pipeline:type','transport'], // Transport Pipe
    ['PLT','2','pipeline:type','outfall'], // Outfall Pipe
    ['PLT','3','pipeline:type','intake'], // Intake Pipe
    ['PLT','4','pipeline:type','sewer'], // Sewer
    ['PLT','5','pipeline:type','pipeline_valve'], // Pipeline Valve
    ['PLT','7','pipeline:type','bubbler_system'], // Bubbler System
    ['PLT','999','pipeline:type','other'], // Other

    // PLT2 - Pipeline Type [2]
    // PLT3 - Pipeline Type [3]

    // POS - Power Source
    ['POS','-999999','generator:source','unknown'], // No Information
    ['POS','1','generator:source','geothermal'], // Geothermal
    ['POS','2','generator:source','hydro'], // Hydro-electric
    ['POS','3','generator:source','nuclear'], // Nuclear
    ['POS','4','generator:source','thermal'], // Thermal
    ['POS','5','generator:source','tidal'], // Tidal
    ['POS','999','generator:source','other'], // Other

    // POS2 - Power Source [2]
    // POS3 - Power Source [3]

    // PPO - Product - See ZI014_PPO
    // PPO2 - Product [2]
    // PPO3 - Product [3]

    // PRM - Permanent
    ['PRM','-999999','permanent','unknown'], // No Information
    ['PRM','1000','permanent','no'],
    ['PRM','1001','permanent','yes'],
    // ['PRM','-1001','raw:PRM','true'], // TRUE
    // ['PRM','-1000','raw:PRM','false'], // FALSE


    // PRW - Raw Material See ZI014_PRW
    // ['PRW','-999999','raw:PRW','unknown'], // noInformation
    // ['PRW','12','raw:PRW','coal'], // Coal
    // ['PRW','14','raw:PRW','coke'], // Coke
    // ['PRW','34','raw:PRW','gravel'], // Gravel
    // ['PRW','54','raw:PRW','ore'], // Ore
    // ['PRW','67','raw:PRW','salt'], // Salt
    // ['PRW','68','raw:PRW','sand'], // Sand
    // ['PRW','84','raw:PRW','stone'], // Stone
    // ['PRW','154','raw:PRW','sulphur'], // Sulphur
    // ['PRW','999','raw:PRW','other'], // Other

    // PRW2 - Raw Material [2]
    // PRW3 - Raw Material [3]

    // PWC - Shoreline Construction Type
    ['PWC','-999999','shoreline_construction:type','unknown'], // No Information
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
    ['PYC','-999999','design','unknown'], // No Information
    ['PYC','1','design','a-frame'], // 'A'
    ['PYC','2','design','h-frame'], // 'H'
    ['PYC','3','design','i-frame'], // 'I'
    ['PYC','4','design','y-frame'], // 'Y'
    ['PYC','5','design','t-frame'], // 'T'
    ['PYC','999','design','other'], // Other

    // PYM - Pylon Material
    ['PYM','-999999','tower:material','unknown'], // No Information
    ['PYM','1','tower:material','aluminium'], // Aluminum
    ['PYM','3','tower:material','concrete'], // Concrete
    ['PYM','4','tower:material','masonry'], // Masonry
    ['PYM','5','tower:material','metal'], // Metal
    ['PYM','7','tower:material','wood'], // Wood
    ['PYM','8','tower:material','steel'], // Steel
    ['PYM','9','tower:material','fibreglass'], // Fibreglass
    ['PYM','10','tower:material','iron'], // Iron
    ['PYM','999','tower:material','other'], // Other

    // RAC - Radar Antenna Configuration
    ['RAC','-999999','radar:configuration','unknown'], // No Information
    ['RAC','1','radar:configuration','dome_enclosed'], // Dome Enclosed
    ['RAC','3','radar:configuration','radome'], // Radome
    ['RAC','4','radar:configuration','radome_on_tower'], // Radome on Tower
    ['RAC','6','radar:configuration','tower_mounted'], // Tower Mounted
    ['RAC','999','radar:configuration','other'], // Other

    // RAS - Radar Station Function
    ['RAS','-999999','radar:use','unknown'], // No Information
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

    // RCG - Resource Content Originator - See ZI004_RCG

    // RFD - Roofed
    ['RFD','-999999','bridge:roof','unknown'], // No Information
    ['RFD','1000','bridge:roof','no'],
    ['RFD','1001','bridge:roof','yes'],

    // RIN_ROI - Route Identification <route designation type>
    ['RIN_ROI','-999999','ref:road:class','unknown'], // No Information
    ['RIN_ROI','1','ref:road:class','international'], // International
    ['RIN_ROI','2','ref:road:class','national_motorway'], // National Motorway
    ['RIN_ROI','3','ref:road:class','primary'], // National
    ['RIN_ROI','4','ref:road:class','secondary'], // Secondary
    ['RIN_ROI','5','ref:road:class','local'], // Local
    ['RIN_ROI','999','ref:road:class','other'], // Other

    // RIN_ROI2 - Route Identification [2] <route designation type>
    // RIN_ROI3 - Route Identification [3] <route designation type>

    // RIP - Rig Present
    ['RIP','-999999','rig','unknown'], // No Information
    ['RIP','1000','rig','no'],
    ['RIP','1001','rig','yes'],

    // RIT - Road Interchange Type
    ['RIT','-999999','junction','unknown'], // No Information
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
    ['RKF','-999999','rock_formation','unknown'], // No Information
    ['RKF','1','rock_formation','columnar'], // Columnar
    ['RKF','2','rock_formation','needle'], // Needle
    ['RKF','3','rock_formation','pinnacle'], // Pinnacle
    ['RKF','4','rock_formation','fossilized_forest'], // Fossilized Forest
    ['RKF','999','rock_formation','other'], // Other

    // RLE - Relative Level
    ['RLE','-999999','layer','unknown'], // No Information
    ['RLE','1','layer','1'], // Raised
    ['RLE','2','layer','0'], // Level with surrounding area
    ['RLE','3','layer','-1'], // Depressed
    // ['RLE','998',undefined,undefined], // Not Applicable
    ['RLE','998','relative_level','not_applicable'], // Not Applicable

    // RMWC - Route Median Width (interval closure): Nothing in OSM
    ['RMWC','2','median:interval:closure','open_interval'], // Open Interval
    ['RMWC','3','median:interval:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['RMWC','4','median:interval:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['RMWC','5','median:interval:closure','closed_interval'], // Closed Interval
    ['RMWC','6','median:interval:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['RMWC','7','median:interval:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['RMWC','8','median:interval:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['RMWC','9','median:interval:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // ROR - Road Interchange Ramp
    ['ROR','-999999','interchange_ramp','unknown'], // No Information
    ['ROR','1000','interchange_ramp','no'], // The default: No ramp
    ['ROR','1001','interchange_ramp','yes'],

    // RPC - Boundary Representation Policy
    ['RPC','-999999','boundary:representation','unknown'], // No Information
    ['RPC','1','boundary:representation','show_no_line'], // Show No Line
    ['RPC','2','boundary:representation','definite'], // Definite
    ['RPC','3','boundary:representation','indefinite'], // Indefinite
    ['RPC','4','boundary:representation','in_dispute'], // In Dispute
    ['RPC','5','boundary:representation','administrative_as_international'], // Administrative as International
    ['RPC','999','boundary:representation','other'], // Other

    // RRC - Railway Use
    ['RRC','-999999','railway:use','unknown'], // No Information
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

    // RRC2 - Railway Use [2]
    // RRC3 - Railway Use [3]

    // RSA - Branch Railway Type
    ['RSA','-999999','service','unknown'], // No Information
    ['RSA','1','service','spur'], // Spur
    ['RSA','2','service','siding'], // Siding
    ['RSA','3','service','passing'], // Passing
    ['RSA','999','service','other'], // Other

    // RSI - Radar Significance
    ['RSI','-999999','airmark:radar_significance','unknown'], // No Information
    ['RSI','1','airmark:radar_significance','metal'], // Metal
    ['RSI','2','airmark:radar_significance','part_metal'], // Part Metal
    ['RSI','3','airmark:radar_significance','masonry'], // Masonry
    ['RSI','4','airmark:radar_significance','composition'], // Composition
    ['RSI','5','airmark:radar_significance','earthen'], // Earthen
    ['RSI','999','airmark:radar_significance','other'], // Other

    // RTA - Linear Feature Arrangement
    ['RTA','-999999','arrangement','unknown'], // No Information
    ['RTA','1','arrangement','single'], // Single Arrangement
    ['RTA','2','arrangement','double'], // Double Arrangement
    ['RTA','3','arrangement','multiple'], // Multiple Arrangements
    ['RTA','999','arrangement','other'], // Other

    // These give the basic highway types. Post processing fixes them
    // RTY - Roadway Type
    ['RTY','-999999','ref:road:type','unknown'], // No Information
    ['RTY','1','ref:road:type','motorway'], // Motorway
    ['RTY','2','ref:road:type','limited_access_motorway'], // Limited Access Motorway
    ['RTY','3','ref:road:type','road'], // Road - Def: Roads outside a BUA
    ['RTY','4','ref:road:type','street'], // Street - Def: Roads inside a BUA
    ['RTY','999','ref:road:type','other'], // Other

    // RWC - Railway Class
    ['RWC','-999999','usage','unknown'], // No Information
    ['RWC','1','usage','high_speed_rail'], // High Speed Rail
    ['RWC','2','usage','main'], // Main Line
    ['RWC','3','usage','branch'], // Branch-line
    ['RWC','999','usage','other'], // Other

    // SAD - Sand Dune Stabilized
    ['SAD','-999999','stabilized','unknown'], // No Information
    ['SAD','1000','stabilized','no'],
    ['SAD','1001','stabilized','yes'],


    // SBB - Supported by Bridge Span
    ['SBB','-999999','on_bridge','unknown'], // No Information
    ['SBB','1000','on_bridge','no'], // Do we need this??
    ['SBB','1001','on_bridge','yes'],

    // SBC - Shelter Belt
    ['SBC','-999999','shelter_belt','unknown'], // No Information
    ['SBC','1000','shelter_belt','no'],
    ['SBC','1001','shelter_belt','yes'],

    // SBT - Substation Type: Not a good translation.
    ['SBT','-999999','substation:type','unknown'], // No Information
    ['SBT','1','substation','switched'], // Switched Substation
    ['SBT','2','substation','transformer'], // Transformer Substation
    ['SBT','3','substation','converter'], // Converter Substation
    ['SBT','999','substation','other'], // Other

    // SBT2 - Substation Type [2]
    // SBT3 - Substation Type [3]

    // SDT - Sand Dune Type
    ['SDT','-999999','dune:type','unknown'], // No Information
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
    ['SEP','-999999','is_divided','unknown'], // No Information
    ['SEP','1000','is_divided','no'],
    ['SEP','1001','is_divided','yes'],

    // SGCC - Surface Slope (interval closure)
    ['SGCC','2','incline:interval:closure','open_interval'], // Open Interval
    ['SGCC','3','incline:interval:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['SGCC','4','incline:interval:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['SGCC','5','incline:interval:closure','closed_interval'], // Closed Interval
    ['SGCC','6','incline:interval:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['SGCC','7','incline:interval:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['SGCC','8','incline:interval:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['SGCC','9','incline:interval:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // SIC - Frozen Cover Type
    ['SIC','-999999','landcover','unknown'], // No Information
    ['SIC','1','landcover','snowfield'], // Snowfield
    ['SIC','2','landcover','ice-field'], // Ice-field

    // SLR - Shoreline Ramp Type
    ['SLR','-999999','shoreline:ramp','unknown'], // No Information
    ['SLR','1','shoreline:ramp','log'], // Log Ramp
    ['SLR','2','shoreline:ramp','marine'], // Marine Ramp
    ['SLR','3','leisure','slipway'], // Slipway
    ['SLR','999','shoreline:ramp','other'], // Other

    // SLT - Shoreline Type
    ['SLT','-999999','shoreline:type','unknown'], // No Information
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
    ['SLT','998','shoreline:type','not_applicable'], // Not Applicable
    ['SLT','999','shoreline:type','other'], // Other

    // SPT - Supported
    ['SPT','-999999','supported','unknown'], // No Information
    ['SPT','1000','supported','no'],
    ['SPT','1001','supported','yes'],

    // SRD - Terrain Morphology
    ['SRD','-999999','terrain:morphology','unknown'], // No Information
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

    // SRL - Location Referenced to Shoreline -- deconflicted with LOC
    ['SRL','-999999','referenced_to_shoreline','unknown'], // No Information
    ['SRL','1','location','offshore'], // Offshore
    ['SRL','2','location','inland'], // Inland
    ['SRL','3','location','at_shoreline'], // At Shoreline

    // SSC - Structure Shape
    ['SSC','-999999','shape','unknown'], // No Information
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

    // SSR - Roof Shape
    ['SSR','-999999','roof:shape','unknown'], // No Information
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

    // SSR2 - Roof Shape [2]
    // SSR3 - Roof Shape [3]

    // STL - Nomadic Seasonal Location
    ['STL','-999999','seasonal_location','unknown'], // No Information
    ['STL','1','seasonal_location','winter'], // Winter
    ['STL','2','seasonal_location','summer'], // Summer
    ['STL','3','seasonal_location','spring'], // Spring
    ['STL','4','seasonal_location','autumn'], // Autumn
    ['STL','999','seasonal_location','other'], // Other

    // STL2 - Nomadic Seasonal Location [2]
    // STL3 - Nomadic Seasonal Location [3]

    // STP - Soil Type
    ['STP','-999999','soil:type','unknown'], // No Information
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

    // SUY - Survey Point Type
    ['SUY','-999999','survey_point:type','unknown'], // No Information
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
    ['SWC','-999999','soil:wetness','unknown'], // No Information
    ['SWC','1','soil:wetness','normally_dry'], // Normally Dry
    ['SWC','2','soil:wetness','normally_moist'], // Normally Moist
    ['SWC','3','soil:wetness','normally_wet'], // Normally Wet
    ['SWC','4','soil:wetness','normally_frozen'], // Normally Frozen

    // SWT - Natural Pool Type
    ['SWT','-999999','spring:type','unknown'], // No Information
    ['SWT','4','spring:type','spring'], // Spring - Will remove this in post processing
    ['SWT','6','spring:type','walled-in'], // Walled-in Spring
    ['SWT','7','spring:type','resurgence'], // Resurgence
    ['SWT','999','spring:type','other'], // Other -- Not great

    // TAC - Tactical Air Navigation Channel Code
    ['TAC','001X','navaid:tactical_channel','channel_001x'], // Channel 001X
    ['TAC','001Y','navaid:tactical_channel','channel_001y'], // Channel 001Y
    ['TAC','002X','navaid:tactical_channel','channel_002x'], // Channel 002X
    ['TAC','002Y','navaid:tactical_channel','channel_002y'], // Channel 002Y
    ['TAC','003X','navaid:tactical_channel','channel_003x'], // Channel 003X
    ['TAC','003Y','navaid:tactical_channel','channel_003y'], // Channel 003Y
    ['TAC','004X','navaid:tactical_channel','channel_004x'], // Channel 004X
    ['TAC','004Y','navaid:tactical_channel','channel_004y'], // Channel 004Y
    ['TAC','005X','navaid:tactical_channel','channel_005x'], // Channel 005X
    ['TAC','005Y','navaid:tactical_channel','channel_005y'], // Channel 005Y
    ['TAC','006X','navaid:tactical_channel','channel_006x'], // Channel 006X
    ['TAC','006Y','navaid:tactical_channel','channel_006y'], // Channel 006Y
    ['TAC','007X','navaid:tactical_channel','channel_007x'], // Channel 007X
    ['TAC','007Y','navaid:tactical_channel','channel_007y'], // Channel 007Y
    ['TAC','008X','navaid:tactical_channel','channel_008x'], // Channel 008X
    ['TAC','008Y','navaid:tactical_channel','channel_008y'], // Channel 008Y
    ['TAC','009X','navaid:tactical_channel','channel_009x'], // Channel 009X
    ['TAC','009Y','navaid:tactical_channel','channel_009y'], // Channel 009Y
    ['TAC','010X','navaid:tactical_channel','channel_010x'], // Channel 010X
    ['TAC','010Y','navaid:tactical_channel','channel_010y'], // Channel 010Y
    ['TAC','011X','navaid:tactical_channel','channel_011x'], // Channel 011X
    ['TAC','011Y','navaid:tactical_channel','channel_011y'], // Channel 011Y
    ['TAC','012X','navaid:tactical_channel','channel_012x'], // Channel 012X
    ['TAC','012Y','navaid:tactical_channel','channel_012y'], // Channel 012Y
    ['TAC','013X','navaid:tactical_channel','channel_013x'], // Channel 013X
    ['TAC','013Y','navaid:tactical_channel','channel_013y'], // Channel 013Y
    ['TAC','014X','navaid:tactical_channel','channel_014x'], // Channel 014X
    ['TAC','014Y','navaid:tactical_channel','channel_014y'], // Channel 014Y
    ['TAC','015X','navaid:tactical_channel','channel_015x'], // Channel 015X
    ['TAC','015Y','navaid:tactical_channel','channel_015y'], // Channel 015Y
    ['TAC','016X','navaid:tactical_channel','channel_016x'], // Channel 016X
    ['TAC','016Y','navaid:tactical_channel','channel_016y'], // Channel 016Y
    ['TAC','017X','navaid:tactical_channel','channel_017x'], // Channel 017X
    ['TAC','017Y','navaid:tactical_channel','channel_017y'], // Channel 017Y
    ['TAC','018X','navaid:tactical_channel','channel_018x'], // Channel 018X
    ['TAC','018Y','navaid:tactical_channel','channel_018y'], // Channel 018Y
    ['TAC','019X','navaid:tactical_channel','channel_019x'], // Channel 019X
    ['TAC','019Y','navaid:tactical_channel','channel_019y'], // Channel 019Y
    ['TAC','020X','navaid:tactical_channel','channel_020x'], // Channel 020X
    ['TAC','020Y','navaid:tactical_channel','channel_020y'], // Channel 020Y
    ['TAC','021X','navaid:tactical_channel','channel_021x'], // Channel 021X
    ['TAC','021Y','navaid:tactical_channel','channel_021y'], // Channel 021Y
    ['TAC','022X','navaid:tactical_channel','channel_022x'], // Channel 022X
    ['TAC','022Y','navaid:tactical_channel','channel_022y'], // Channel 022Y
    ['TAC','023X','navaid:tactical_channel','channel_023x'], // Channel 023X
    ['TAC','023Y','navaid:tactical_channel','channel_023y'], // Channel 023Y
    ['TAC','024X','navaid:tactical_channel','channel_024x'], // Channel 024X
    ['TAC','024Y','navaid:tactical_channel','channel_024y'], // Channel 024Y
    ['TAC','025X','navaid:tactical_channel','channel_025x'], // Channel 025X
    ['TAC','025Y','navaid:tactical_channel','channel_025y'], // Channel 025Y
    ['TAC','026X','navaid:tactical_channel','channel_026x'], // Channel 026X
    ['TAC','026Y','navaid:tactical_channel','channel_026y'], // Channel 026Y
    ['TAC','027X','navaid:tactical_channel','channel_027x'], // Channel 027X
    ['TAC','027Y','navaid:tactical_channel','channel_027y'], // Channel 027Y
    ['TAC','028X','navaid:tactical_channel','channel_028x'], // Channel 028X
    ['TAC','028Y','navaid:tactical_channel','channel_028y'], // Channel 028Y
    ['TAC','029X','navaid:tactical_channel','channel_029x'], // Channel 029X
    ['TAC','029Y','navaid:tactical_channel','channel_029y'], // Channel 029Y
    ['TAC','030X','navaid:tactical_channel','channel_030x'], // Channel 030X
    ['TAC','030Y','navaid:tactical_channel','channel_030y'], // Channel 030Y
    ['TAC','031X','navaid:tactical_channel','channel_031x'], // Channel 031X
    ['TAC','031Y','navaid:tactical_channel','channel_031y'], // Channel 031Y
    ['TAC','032X','navaid:tactical_channel','channel_032x'], // Channel 032X
    ['TAC','032Y','navaid:tactical_channel','channel_032y'], // Channel 032Y
    ['TAC','033X','navaid:tactical_channel','channel_033x'], // Channel 033X
    ['TAC','033Y','navaid:tactical_channel','channel_033y'], // Channel 033Y
    ['TAC','034X','navaid:tactical_channel','channel_034x'], // Channel 034X
    ['TAC','034Y','navaid:tactical_channel','channel_034y'], // Channel 034Y
    ['TAC','035X','navaid:tactical_channel','channel_035x'], // Channel 035X
    ['TAC','035Y','navaid:tactical_channel','channel_035y'], // Channel 035Y
    ['TAC','036X','navaid:tactical_channel','channel_036x'], // Channel 036X
    ['TAC','036Y','navaid:tactical_channel','channel_036y'], // Channel 036Y
    ['TAC','037X','navaid:tactical_channel','channel_037x'], // Channel 037X
    ['TAC','037Y','navaid:tactical_channel','channel_037y'], // Channel 037Y
    ['TAC','038X','navaid:tactical_channel','channel_038x'], // Channel 038X
    ['TAC','038Y','navaid:tactical_channel','channel_038y'], // Channel 038Y
    ['TAC','039X','navaid:tactical_channel','channel_039x'], // Channel 039X
    ['TAC','039Y','navaid:tactical_channel','channel_039y'], // Channel 039Y
    ['TAC','040X','navaid:tactical_channel','channel_040x'], // Channel 040X
    ['TAC','040Y','navaid:tactical_channel','channel_040y'], // Channel 040Y
    ['TAC','041X','navaid:tactical_channel','channel_041x'], // Channel 041X
    ['TAC','041Y','navaid:tactical_channel','channel_041y'], // Channel 041Y
    ['TAC','042X','navaid:tactical_channel','channel_042x'], // Channel 042X
    ['TAC','042Y','navaid:tactical_channel','channel_042y'], // Channel 042Y
    ['TAC','043X','navaid:tactical_channel','channel_043x'], // Channel 043X
    ['TAC','043Y','navaid:tactical_channel','channel_043y'], // Channel 043Y
    ['TAC','044X','navaid:tactical_channel','channel_044x'], // Channel 044X
    ['TAC','044Y','navaid:tactical_channel','channel_044y'], // Channel 044Y
    ['TAC','045X','navaid:tactical_channel','channel_045x'], // Channel 045X
    ['TAC','045Y','navaid:tactical_channel','channel_045y'], // Channel 045Y
    ['TAC','046X','navaid:tactical_channel','channel_046x'], // Channel 046X
    ['TAC','046Y','navaid:tactical_channel','channel_046y'], // Channel 046Y
    ['TAC','047X','navaid:tactical_channel','channel_047x'], // Channel 047X
    ['TAC','047Y','navaid:tactical_channel','channel_047y'], // Channel 047Y
    ['TAC','048X','navaid:tactical_channel','channel_048x'], // Channel 048X
    ['TAC','048Y','navaid:tactical_channel','channel_048y'], // Channel 048Y
    ['TAC','049X','navaid:tactical_channel','channel_049x'], // Channel 049X
    ['TAC','049Y','navaid:tactical_channel','channel_049y'], // Channel 049Y
    ['TAC','050X','navaid:tactical_channel','channel_050x'], // Channel 050X
    ['TAC','050Y','navaid:tactical_channel','channel_050y'], // Channel 050Y
    ['TAC','051X','navaid:tactical_channel','channel_051x'], // Channel 051X
    ['TAC','051Y','navaid:tactical_channel','channel_051y'], // Channel 051Y
    ['TAC','052X','navaid:tactical_channel','channel_052x'], // Channel 052X
    ['TAC','052Y','navaid:tactical_channel','channel_052y'], // Channel 052Y
    ['TAC','053X','navaid:tactical_channel','channel_053x'], // Channel 053X
    ['TAC','053Y','navaid:tactical_channel','channel_053y'], // Channel 053Y
    ['TAC','054X','navaid:tactical_channel','channel_054x'], // Channel 054X
    ['TAC','054Y','navaid:tactical_channel','channel_054y'], // Channel 054Y
    ['TAC','055X','navaid:tactical_channel','channel_055x'], // Channel 055X
    ['TAC','055Y','navaid:tactical_channel','channel_055y'], // Channel 055Y
    ['TAC','056X','navaid:tactical_channel','channel_056x'], // Channel 056X
    ['TAC','056Y','navaid:tactical_channel','channel_056y'], // Channel 056Y
    ['TAC','057X','navaid:tactical_channel','channel_057x'], // Channel 057X
    ['TAC','057Y','navaid:tactical_channel','channel_057y'], // Channel 057Y
    ['TAC','058X','navaid:tactical_channel','channel_058x'], // Channel 058X
    ['TAC','058Y','navaid:tactical_channel','channel_058y'], // Channel 058Y
    ['TAC','059X','navaid:tactical_channel','channel_059x'], // Channel 059X
    ['TAC','059Y','navaid:tactical_channel','channel_059y'], // Channel 059Y
    ['TAC','060X','navaid:tactical_channel','channel_060x'], // Channel 060X
    ['TAC','060Y','navaid:tactical_channel','channel_060y'], // Channel 060Y
    ['TAC','061X','navaid:tactical_channel','channel_061x'], // Channel 061X
    ['TAC','061Y','navaid:tactical_channel','channel_061y'], // Channel 061Y
    ['TAC','062X','navaid:tactical_channel','channel_062x'], // Channel 062X
    ['TAC','062Y','navaid:tactical_channel','channel_062y'], // Channel 062Y
    ['TAC','063X','navaid:tactical_channel','channel_063x'], // Channel 063X
    ['TAC','063Y','navaid:tactical_channel','channel_063y'], // Channel 063Y
    ['TAC','064X','navaid:tactical_channel','channel_064x'], // Channel 064X
    ['TAC','064Y','navaid:tactical_channel','channel_064y'], // Channel 064Y
    ['TAC','065X','navaid:tactical_channel','channel_065x'], // Channel 065X
    ['TAC','065Y','navaid:tactical_channel','channel_065y'], // Channel 065Y
    ['TAC','066X','navaid:tactical_channel','channel_066x'], // Channel 066X
    ['TAC','066Y','navaid:tactical_channel','channel_066y'], // Channel 066Y
    ['TAC','067X','navaid:tactical_channel','channel_067x'], // Channel 067X
    ['TAC','067Y','navaid:tactical_channel','channel_067y'], // Channel 067Y
    ['TAC','068X','navaid:tactical_channel','channel_068x'], // Channel 068X
    ['TAC','068Y','navaid:tactical_channel','channel_068y'], // Channel 068Y
    ['TAC','069X','navaid:tactical_channel','channel_069x'], // Channel 069X
    ['TAC','069Y','navaid:tactical_channel','channel_069y'], // Channel 069Y
    ['TAC','070X','navaid:tactical_channel','channel_070x'], // Channel 070X
    ['TAC','070Y','navaid:tactical_channel','channel_070y'], // Channel 070Y
    ['TAC','071X','navaid:tactical_channel','channel_071x'], // Channel 071X
    ['TAC','071Y','navaid:tactical_channel','channel_071y'], // Channel 071Y
    ['TAC','072X','navaid:tactical_channel','channel_072x'], // Channel 072X
    ['TAC','072Y','navaid:tactical_channel','channel_072y'], // Channel 072Y
    ['TAC','073X','navaid:tactical_channel','channel_073x'], // Channel 073X
    ['TAC','073Y','navaid:tactical_channel','channel_073y'], // Channel 073Y
    ['TAC','074X','navaid:tactical_channel','channel_074x'], // Channel 074X
    ['TAC','074Y','navaid:tactical_channel','channel_074y'], // Channel 074Y
    ['TAC','075X','navaid:tactical_channel','channel_075x'], // Channel 075X
    ['TAC','075Y','navaid:tactical_channel','channel_075y'], // Channel 075Y
    ['TAC','076X','navaid:tactical_channel','channel_076x'], // Channel 076X
    ['TAC','076Y','navaid:tactical_channel','channel_076y'], // Channel 076Y
    ['TAC','077X','navaid:tactical_channel','channel_077x'], // Channel 077X
    ['TAC','077Y','navaid:tactical_channel','channel_077y'], // Channel 077Y
    ['TAC','078X','navaid:tactical_channel','channel_078x'], // Channel 078X
    ['TAC','078Y','navaid:tactical_channel','channel_078y'], // Channel 078Y
    ['TAC','079X','navaid:tactical_channel','channel_079x'], // Channel 079X
    ['TAC','079Y','navaid:tactical_channel','channel_079y'], // Channel 079Y
    ['TAC','080X','navaid:tactical_channel','channel_080x'], // Channel 080X
    ['TAC','080Y','navaid:tactical_channel','channel_080y'], // Channel 080Y
    ['TAC','081X','navaid:tactical_channel','channel_081x'], // Channel 081X
    ['TAC','081Y','navaid:tactical_channel','channel_081y'], // Channel 081Y
    ['TAC','082X','navaid:tactical_channel','channel_082x'], // Channel 082X
    ['TAC','082Y','navaid:tactical_channel','channel_082y'], // Channel 082Y
    ['TAC','083X','navaid:tactical_channel','channel_083x'], // Channel 083X
    ['TAC','083Y','navaid:tactical_channel','channel_083y'], // Channel 083Y
    ['TAC','084X','navaid:tactical_channel','channel_084x'], // Channel 084X
    ['TAC','084Y','navaid:tactical_channel','channel_084y'], // Channel 084Y
    ['TAC','085X','navaid:tactical_channel','channel_085x'], // Channel 085X
    ['TAC','085Y','navaid:tactical_channel','channel_085y'], // Channel 085Y
    ['TAC','086X','navaid:tactical_channel','channel_086x'], // Channel 086X
    ['TAC','086Y','navaid:tactical_channel','channel_086y'], // Channel 086Y
    ['TAC','087X','navaid:tactical_channel','channel_087x'], // Channel 087X
    ['TAC','087Y','navaid:tactical_channel','channel_087y'], // Channel 087Y
    ['TAC','088X','navaid:tactical_channel','channel_088x'], // Channel 088X
    ['TAC','088Y','navaid:tactical_channel','channel_088y'], // Channel 088Y
    ['TAC','089X','navaid:tactical_channel','channel_089x'], // Channel 089X
    ['TAC','089Y','navaid:tactical_channel','channel_089y'], // Channel 089Y
    ['TAC','090X','navaid:tactical_channel','channel_090x'], // Channel 090X
    ['TAC','090Y','navaid:tactical_channel','channel_090y'], // Channel 090Y
    ['TAC','091X','navaid:tactical_channel','channel_091x'], // Channel 091X
    ['TAC','091Y','navaid:tactical_channel','channel_091y'], // Channel 091Y
    ['TAC','092X','navaid:tactical_channel','channel_092x'], // Channel 092X
    ['TAC','092Y','navaid:tactical_channel','channel_092y'], // Channel 092Y
    ['TAC','093X','navaid:tactical_channel','channel_093x'], // Channel 093X
    ['TAC','093Y','navaid:tactical_channel','channel_093y'], // Channel 093Y
    ['TAC','094X','navaid:tactical_channel','channel_094x'], // Channel 094X
    ['TAC','094Y','navaid:tactical_channel','channel_094y'], // Channel 094Y
    ['TAC','095X','navaid:tactical_channel','channel_095x'], // Channel 095X
    ['TAC','095Y','navaid:tactical_channel','channel_095y'], // Channel 095Y
    ['TAC','096X','navaid:tactical_channel','channel_096x'], // Channel 096X
    ['TAC','096Y','navaid:tactical_channel','channel_096y'], // Channel 096Y
    ['TAC','097X','navaid:tactical_channel','channel_097x'], // Channel 097X
    ['TAC','097Y','navaid:tactical_channel','channel_097y'], // Channel 097Y
    ['TAC','098X','navaid:tactical_channel','channel_098x'], // Channel 098X
    ['TAC','098Y','navaid:tactical_channel','channel_098y'], // Channel 098Y
    ['TAC','099X','navaid:tactical_channel','channel_099x'], // Channel 099X
    ['TAC','099Y','navaid:tactical_channel','channel_099y'], // Channel 099Y
    ['TAC','100X','navaid:tactical_channel','channel_100x'], // Channel 100X
    ['TAC','100Y','navaid:tactical_channel','channel_100y'], // Channel 100Y
    ['TAC','101X','navaid:tactical_channel','channel_101x'], // Channel 101X
    ['TAC','101Y','navaid:tactical_channel','channel_101y'], // Channel 101Y
    ['TAC','102X','navaid:tactical_channel','channel_102x'], // Channel 102X
    ['TAC','102Y','navaid:tactical_channel','channel_102y'], // Channel 102Y
    ['TAC','103X','navaid:tactical_channel','channel_103x'], // Channel 103X
    ['TAC','103Y','navaid:tactical_channel','channel_103y'], // Channel 103Y
    ['TAC','104X','navaid:tactical_channel','channel_104x'], // Channel 104X
    ['TAC','104Y','navaid:tactical_channel','channel_104y'], // Channel 104Y
    ['TAC','105X','navaid:tactical_channel','channel_105x'], // Channel 105X
    ['TAC','105Y','navaid:tactical_channel','channel_105y'], // Channel 105Y
    ['TAC','106X','navaid:tactical_channel','channel_106x'], // Channel 106X
    ['TAC','106Y','navaid:tactical_channel','channel_106y'], // Channel 106Y
    ['TAC','107X','navaid:tactical_channel','channel_107x'], // Channel 107X
    ['TAC','107Y','navaid:tactical_channel','channel_107y'], // Channel 107Y
    ['TAC','108X','navaid:tactical_channel','channel_108x'], // Channel 108X
    ['TAC','108Y','navaid:tactical_channel','channel_108y'], // Channel 108Y
    ['TAC','109X','navaid:tactical_channel','channel_109x'], // Channel 109X
    ['TAC','109Y','navaid:tactical_channel','channel_109y'], // Channel 109Y
    ['TAC','110X','navaid:tactical_channel','channel_110x'], // Channel 110X
    ['TAC','110Y','navaid:tactical_channel','channel_110y'], // Channel 110Y
    ['TAC','111X','navaid:tactical_channel','channel_111x'], // Channel 111X
    ['TAC','111Y','navaid:tactical_channel','channel_111y'], // Channel 111Y
    ['TAC','112X','navaid:tactical_channel','channel_112x'], // Channel 112X
    ['TAC','112Y','navaid:tactical_channel','channel_112y'], // Channel 112Y
    ['TAC','113X','navaid:tactical_channel','channel_113x'], // Channel 113X
    ['TAC','113Y','navaid:tactical_channel','channel_113y'], // Channel 113Y
    ['TAC','114X','navaid:tactical_channel','channel_114x'], // Channel 114X
    ['TAC','114Y','navaid:tactical_channel','channel_114y'], // Channel 114Y
    ['TAC','115X','navaid:tactical_channel','channel_115x'], // Channel 115X
    ['TAC','115Y','navaid:tactical_channel','channel_115y'], // Channel 115Y
    ['TAC','116X','navaid:tactical_channel','channel_116x'], // Channel 116X
    ['TAC','116Y','navaid:tactical_channel','channel_116y'], // Channel 116Y
    ['TAC','117X','navaid:tactical_channel','channel_117x'], // Channel 117X
    ['TAC','117Y','navaid:tactical_channel','channel_117y'], // Channel 117Y
    ['TAC','118X','navaid:tactical_channel','channel_118x'], // Channel 118X
    ['TAC','118Y','navaid:tactical_channel','channel_118y'], // Channel 118Y
    ['TAC','119X','navaid:tactical_channel','channel_119x'], // Channel 119X
    ['TAC','119Y','navaid:tactical_channel','channel_119y'], // Channel 119Y
    ['TAC','120X','navaid:tactical_channel','channel_120x'], // Channel 120X
    ['TAC','120Y','navaid:tactical_channel','channel_120y'], // Channel 120Y
    ['TAC','121X','navaid:tactical_channel','channel_121x'], // Channel 121X
    ['TAC','121Y','navaid:tactical_channel','channel_121y'], // Channel 121Y
    ['TAC','122X','navaid:tactical_channel','channel_122x'], // Channel 122X
    ['TAC','122Y','navaid:tactical_channel','channel_122y'], // Channel 122Y
    ['TAC','123X','navaid:tactical_channel','channel_123x'], // Channel 123X
    ['TAC','123Y','navaid:tactical_channel','channel_123y'], // Channel 123Y
    ['TAC','124X','navaid:tactical_channel','channel_124x'], // Channel 124X
    ['TAC','124Y','navaid:tactical_channel','channel_124y'], // Channel 124Y
    ['TAC','125X','navaid:tactical_channel','channel_125x'], // Channel 125X
    ['TAC','125Y','navaid:tactical_channel','channel_125y'], // Channel 125Y
    ['TAC','126X','navaid:tactical_channel','channel_126x'], // Channel 126X
    ['TAC','126Y','navaid:tactical_channel','channel_126y'], // Channel 126Y
    ['TAC','500X','navaid:tactical_channel','channel_500x'], // Channel 500X
    ['TAC','500Y','navaid:tactical_channel','channel_500y'], // Channel 500Y
    ['TAC','501X','navaid:tactical_channel','channel_501x'], // Channel 501X
    ['TAC','501Y','navaid:tactical_channel','channel_501y'], // Channel 501Y
    ['TAC','502X','navaid:tactical_channel','channel_502x'], // Channel 502X
    ['TAC','502Y','navaid:tactical_channel','channel_502y'], // Channel 502Y
    ['TAC','503X','navaid:tactical_channel','channel_503x'], // Channel 503X
    ['TAC','503Y','navaid:tactical_channel','channel_503y'], // Channel 503Y
    ['TAC','504X','navaid:tactical_channel','channel_504x'], // Channel 504X
    ['TAC','504Y','navaid:tactical_channel','channel_504y'], // Channel 504Y
    ['TAC','505X','navaid:tactical_channel','channel_505x'], // Channel 505X
    ['TAC','505Y','navaid:tactical_channel','channel_505y'], // Channel 505Y
    ['TAC','506X','navaid:tactical_channel','channel_506x'], // Channel 506X
    ['TAC','506Y','navaid:tactical_channel','channel_506y'], // Channel 506Y
    ['TAC','507X','navaid:tactical_channel','channel_507x'], // Channel 507X
    ['TAC','507Y','navaid:tactical_channel','channel_507y'], // Channel 507Y
    ['TAC','508X','navaid:tactical_channel','channel_508x'], // Channel 508X
    ['TAC','508Y','navaid:tactical_channel','channel_508y'], // Channel 508Y
    ['TAC','509X','navaid:tactical_channel','channel_509x'], // Channel 509X
    ['TAC','509Y','navaid:tactical_channel','channel_509y'], // Channel 509Y
    ['TAC','510X','navaid:tactical_channel','channel_510x'], // Channel 510X
    ['TAC','510Y','navaid:tactical_channel','channel_510y'], // Channel 510Y
    ['TAC','511X','navaid:tactical_channel','channel_511x'], // Channel 511X
    ['TAC','511Y','navaid:tactical_channel','channel_511y'], // Channel 511Y
    ['TAC','512X','navaid:tactical_channel','channel_512x'], // Channel 512X
    ['TAC','512Y','navaid:tactical_channel','channel_512y'], // Channel 512Y
    ['TAC','513X','navaid:tactical_channel','channel_513x'], // Channel 513X
    ['TAC','513Y','navaid:tactical_channel','channel_513y'], // Channel 513Y
    ['TAC','514X','navaid:tactical_channel','channel_514x'], // Channel 514X
    ['TAC','514Y','navaid:tactical_channel','channel_514y'], // Channel 514Y
    ['TAC','515X','navaid:tactical_channel','channel_515x'], // Channel 515X
    ['TAC','515Y','navaid:tactical_channel','channel_515y'], // Channel 515Y
    ['TAC','516X','navaid:tactical_channel','channel_516x'], // Channel 516X
    ['TAC','516Y','navaid:tactical_channel','channel_516y'], // Channel 516Y
    ['TAC','517X','navaid:tactical_channel','channel_517x'], // Channel 517X
    ['TAC','517Y','navaid:tactical_channel','channel_517y'], // Channel 517Y
    ['TAC','518X','navaid:tactical_channel','channel_518x'], // Channel 518X
    ['TAC','518Y','navaid:tactical_channel','channel_518y'], // Channel 518Y
    ['TAC','519X','navaid:tactical_channel','channel_519x'], // Channel 519X
    ['TAC','519Y','navaid:tactical_channel','channel_519y'], // Channel 519Y
    ['TAC','520X','navaid:tactical_channel','channel_520x'], // Channel 520X
    ['TAC','520Y','navaid:tactical_channel','channel_520y'], // Channel 520Y
    ['TAC','521X','navaid:tactical_channel','channel_521x'], // Channel 521X
    ['TAC','521Y','navaid:tactical_channel','channel_521y'], // Channel 521Y
    ['TAC','522X','navaid:tactical_channel','channel_522x'], // Channel 522X
    ['TAC','522Y','navaid:tactical_channel','channel_522y'], // Channel 522Y
    ['TAC','523X','navaid:tactical_channel','channel_523x'], // Channel 523X
    ['TAC','523Y','navaid:tactical_channel','channel_523y'], // Channel 523Y
    ['TAC','524X','navaid:tactical_channel','channel_524x'], // Channel 524X
    ['TAC','524Y','navaid:tactical_channel','channel_524y'], // Channel 524Y
    ['TAC','525X','navaid:tactical_channel','channel_525x'], // Channel 525X
    ['TAC','525Y','navaid:tactical_channel','channel_525y'], // Channel 525Y
    ['TAC','526X','navaid:tactical_channel','channel_526x'], // Channel 526X
    ['TAC','526Y','navaid:tactical_channel','channel_526y'], // Channel 526Y
    ['TAC','527X','navaid:tactical_channel','channel_527x'], // Channel 527X
    ['TAC','527Y','navaid:tactical_channel','channel_527y'], // Channel 527Y
    ['TAC','528X','navaid:tactical_channel','channel_528x'], // Channel 528X
    ['TAC','528Y','navaid:tactical_channel','channel_528y'], // Channel 528Y
    ['TAC','529X','navaid:tactical_channel','channel_529x'], // Channel 529X
    ['TAC','529Y','navaid:tactical_channel','channel_529y'], // Channel 529Y
    ['TAC','530X','navaid:tactical_channel','channel_530x'], // Channel 530X
    ['TAC','530Y','navaid:tactical_channel','channel_530y'], // Channel 530Y
    ['TAC','531X','navaid:tactical_channel','channel_531x'], // Channel 531X
    ['TAC','531Y','navaid:tactical_channel','channel_531y'], // Channel 531Y
    ['TAC','532X','navaid:tactical_channel','channel_532x'], // Channel 532X
    ['TAC','532Y','navaid:tactical_channel','channel_532y'], // Channel 532Y
    ['TAC','533X','navaid:tactical_channel','channel_533x'], // Channel 533X
    ['TAC','533Y','navaid:tactical_channel','channel_533y'], // Channel 533Y
    ['TAC','534X','navaid:tactical_channel','channel_534x'], // Channel 534X
    ['TAC','534Y','navaid:tactical_channel','channel_534y'], // Channel 534Y
    ['TAC','535X','navaid:tactical_channel','channel_535x'], // Channel 535X
    ['TAC','535Y','navaid:tactical_channel','channel_535y'], // Channel 535Y
    ['TAC','536X','navaid:tactical_channel','channel_536x'], // Channel 536X
    ['TAC','536Y','navaid:tactical_channel','channel_536y'], // Channel 536Y
    ['TAC','537X','navaid:tactical_channel','channel_537x'], // Channel 537X
    ['TAC','537Y','navaid:tactical_channel','channel_537y'], // Channel 537Y
    ['TAC','538X','navaid:tactical_channel','channel_538x'], // Channel 538X
    ['TAC','538Y','navaid:tactical_channel','channel_538y'], // Channel 538Y
    ['TAC','539X','navaid:tactical_channel','channel_539x'], // Channel 539X
    ['TAC','539Y','navaid:tactical_channel','channel_539y'], // Channel 539Y
    ['TAC','540X','navaid:tactical_channel','channel_540x'], // Channel 540X
    ['TAC','540Y','navaid:tactical_channel','channel_540y'], // Channel 540Y
    ['TAC','541X','navaid:tactical_channel','channel_541x'], // Channel 541X
    ['TAC','541Y','navaid:tactical_channel','channel_541y'], // Channel 541Y
    ['TAC','542X','navaid:tactical_channel','channel_542x'], // Channel 542X
    ['TAC','542Y','navaid:tactical_channel','channel_542y'], // Channel 542Y
    ['TAC','543X','navaid:tactical_channel','channel_543x'], // Channel 543X
    ['TAC','543Y','navaid:tactical_channel','channel_543y'], // Channel 543Y
    ['TAC','544X','navaid:tactical_channel','channel_544x'], // Channel 544X
    ['TAC','544Y','navaid:tactical_channel','channel_544y'], // Channel 544Y
    ['TAC','545X','navaid:tactical_channel','channel_545x'], // Channel 545X
    ['TAC','545Y','navaid:tactical_channel','channel_545y'], // Channel 545Y
    ['TAC','546X','navaid:tactical_channel','channel_546x'], // Channel 546X
    ['TAC','546Y','navaid:tactical_channel','channel_546y'], // Channel 546Y
    ['TAC','547X','navaid:tactical_channel','channel_547x'], // Channel 547X
    ['TAC','547Y','navaid:tactical_channel','channel_547y'], // Channel 547Y
    ['TAC','548X','navaid:tactical_channel','channel_548x'], // Channel 548X
    ['TAC','548Y','navaid:tactical_channel','channel_548y'], // Channel 548Y
    ['TAC','549X','navaid:tactical_channel','channel_549x'], // Channel 549X
    ['TAC','549Y','navaid:tactical_channel','channel_549y'], // Channel 549Y
    ['TAC','550X','navaid:tactical_channel','channel_550x'], // Channel 550X
    ['TAC','550Y','navaid:tactical_channel','channel_550y'], // Channel 550Y
    ['TAC','551X','navaid:tactical_channel','channel_551x'], // Channel 551X
    ['TAC','551Y','navaid:tactical_channel','channel_551y'], // Channel 551Y
    ['TAC','552X','navaid:tactical_channel','channel_552x'], // Channel 552X
    ['TAC','552Y','navaid:tactical_channel','channel_552y'], // Channel 552Y
    ['TAC','553X','navaid:tactical_channel','channel_553x'], // Channel 553X
    ['TAC','553Y','navaid:tactical_channel','channel_553y'], // Channel 553Y
    ['TAC','554X','navaid:tactical_channel','channel_554x'], // Channel 554X
    ['TAC','554Y','navaid:tactical_channel','channel_554y'], // Channel 554Y
    ['TAC','555X','navaid:tactical_channel','channel_555x'], // Channel 555X
    ['TAC','555Y','navaid:tactical_channel','channel_555y'], // Channel 555Y
    ['TAC','556X','navaid:tactical_channel','channel_556x'], // Channel 556X
    ['TAC','556Y','navaid:tactical_channel','channel_556y'], // Channel 556Y
    ['TAC','557X','navaid:tactical_channel','channel_557x'], // Channel 557X
    ['TAC','557Y','navaid:tactical_channel','channel_557y'], // Channel 557Y
    ['TAC','558X','navaid:tactical_channel','channel_558x'], // Channel 558X
    ['TAC','558Y','navaid:tactical_channel','channel_558y'], // Channel 558Y
    ['TAC','559X','navaid:tactical_channel','channel_559x'], // Channel 559X
    ['TAC','559Y','navaid:tactical_channel','channel_559y'], // Channel 559Y
    ['TAC','560X','navaid:tactical_channel','channel_560x'], // Channel 560X
    ['TAC','560Y','navaid:tactical_channel','channel_560y'], // Channel 560Y
    ['TAC','561X','navaid:tactical_channel','channel_561x'], // Channel 561X
    ['TAC','561Y','navaid:tactical_channel','channel_561y'], // Channel 561Y
    ['TAC','562X','navaid:tactical_channel','channel_562x'], // Channel 562X
    ['TAC','562Y','navaid:tactical_channel','channel_562y'], // Channel 562Y
    ['TAC','563X','navaid:tactical_channel','channel_563x'], // Channel 563X
    ['TAC','563Y','navaid:tactical_channel','channel_563y'], // Channel 563Y
    ['TAC','564X','navaid:tactical_channel','channel_564x'], // Channel 564X
    ['TAC','564Y','navaid:tactical_channel','channel_564y'], // Channel 564Y
    ['TAC','565X','navaid:tactical_channel','channel_565x'], // Channel 565X
    ['TAC','565Y','navaid:tactical_channel','channel_565y'], // Channel 565Y
    ['TAC','566X','navaid:tactical_channel','channel_566x'], // Channel 566X
    ['TAC','566Y','navaid:tactical_channel','channel_566y'], // Channel 566Y
    ['TAC','567X','navaid:tactical_channel','channel_567x'], // Channel 567X
    ['TAC','567Y','navaid:tactical_channel','channel_567y'], // Channel 567Y
    ['TAC','568X','navaid:tactical_channel','channel_568x'], // Channel 568X
    ['TAC','568Y','navaid:tactical_channel','channel_568y'], // Channel 568Y
    ['TAC','569X','navaid:tactical_channel','channel_569x'], // Channel 569X
    ['TAC','569Y','navaid:tactical_channel','channel_569y'], // Channel 569Y
    ['TAC','570X','navaid:tactical_channel','channel_570x'], // Channel 570X
    ['TAC','570Y','navaid:tactical_channel','channel_570y'], // Channel 570Y
    ['TAC','571X','navaid:tactical_channel','channel_571x'], // Channel 571X
    ['TAC','571Y','navaid:tactical_channel','channel_571y'], // Channel 571Y
    ['TAC','572X','navaid:tactical_channel','channel_572x'], // Channel 572X
    ['TAC','572Y','navaid:tactical_channel','channel_572y'], // Channel 572Y
    ['TAC','573X','navaid:tactical_channel','channel_573x'], // Channel 573X
    ['TAC','573Y','navaid:tactical_channel','channel_573y'], // Channel 573Y
    ['TAC','574X','navaid:tactical_channel','channel_574x'], // Channel 574X
    ['TAC','574Y','navaid:tactical_channel','channel_574y'], // Channel 574Y
    ['TAC','575X','navaid:tactical_channel','channel_575x'], // Channel 575X
    ['TAC','575Y','navaid:tactical_channel','channel_575y'], // Channel 575Y
    ['TAC','576X','navaid:tactical_channel','channel_576x'], // Channel 576X
    ['TAC','576Y','navaid:tactical_channel','channel_576y'], // Channel 576Y
    ['TAC','577X','navaid:tactical_channel','channel_577x'], // Channel 577X
    ['TAC','577Y','navaid:tactical_channel','channel_577y'], // Channel 577Y
    ['TAC','578X','navaid:tactical_channel','channel_578x'], // Channel 578X
    ['TAC','578Y','navaid:tactical_channel','channel_578y'], // Channel 578Y
    ['TAC','579X','navaid:tactical_channel','channel_579x'], // Channel 579X
    ['TAC','579Y','navaid:tactical_channel','channel_579y'], // Channel 579Y
    ['TAC','580X','navaid:tactical_channel','channel_580x'], // Channel 580X
    ['TAC','580Y','navaid:tactical_channel','channel_580y'], // Channel 580Y
    ['TAC','581X','navaid:tactical_channel','channel_581x'], // Channel 581X
    ['TAC','581Y','navaid:tactical_channel','channel_581y'], // Channel 581Y
    ['TAC','582X','navaid:tactical_channel','channel_582x'], // Channel 582X
    ['TAC','582Y','navaid:tactical_channel','channel_582y'], // Channel 582Y
    ['TAC','583X','navaid:tactical_channel','channel_583x'], // Channel 583X
    ['TAC','583Y','navaid:tactical_channel','channel_583y'], // Channel 583Y
    ['TAC','584X','navaid:tactical_channel','channel_584x'], // Channel 584X
    ['TAC','584Y','navaid:tactical_channel','channel_584y'], // Channel 584Y
    ['TAC','585X','navaid:tactical_channel','channel_585x'], // Channel 585X
    ['TAC','585Y','navaid:tactical_channel','channel_585y'], // Channel 585Y
    ['TAC','586X','navaid:tactical_channel','channel_586x'], // Channel 586X
    ['TAC','586Y','navaid:tactical_channel','channel_586y'], // Channel 586Y
    ['TAC','587X','navaid:tactical_channel','channel_587x'], // Channel 587X
    ['TAC','587Y','navaid:tactical_channel','channel_587y'], // Channel 587Y
    ['TAC','588X','navaid:tactical_channel','channel_588x'], // Channel 588X
    ['TAC','588Y','navaid:tactical_channel','channel_588y'], // Channel 588Y
    ['TAC','589X','navaid:tactical_channel','channel_589x'], // Channel 589X
    ['TAC','589Y','navaid:tactical_channel','channel_589y'], // Channel 589Y
    ['TAC','590X','navaid:tactical_channel','channel_590x'], // Channel 590X
    ['TAC','590Y','navaid:tactical_channel','channel_590y'], // Channel 590Y
    ['TAC','591X','navaid:tactical_channel','channel_591x'], // Channel 591X
    ['TAC','591Y','navaid:tactical_channel','channel_591y'], // Channel 591Y
    ['TAC','592X','navaid:tactical_channel','channel_592x'], // Channel 592X
    ['TAC','592Y','navaid:tactical_channel','channel_592y'], // Channel 592Y
    ['TAC','593X','navaid:tactical_channel','channel_593x'], // Channel 593X
    ['TAC','593Y','navaid:tactical_channel','channel_593y'], // Channel 593Y
    ['TAC','594X','navaid:tactical_channel','channel_594x'], // Channel 594X
    ['TAC','594Y','navaid:tactical_channel','channel_594y'], // Channel 594Y
    ['TAC','595X','navaid:tactical_channel','channel_595x'], // Channel 595X
    ['TAC','595Y','navaid:tactical_channel','channel_595y'], // Channel 595Y
    ['TAC','596X','navaid:tactical_channel','channel_596x'], // Channel 596X
    ['TAC','596Y','navaid:tactical_channel','channel_596y'], // Channel 596Y
    ['TAC','597X','navaid:tactical_channel','channel_597x'], // Channel 597X
    ['TAC','597Y','navaid:tactical_channel','channel_597y'], // Channel 597Y
    ['TAC','598X','navaid:tactical_channel','channel_598x'], // Channel 598X
    ['TAC','598Y','navaid:tactical_channel','channel_598y'], // Channel 598Y
    ['TAC','599X','navaid:tactical_channel','channel_599x'], // Channel 599X
    ['TAC','599Y','navaid:tactical_channel','channel_599y'], // Channel 599Y
    ['TAC','600X','navaid:tactical_channel','channel_600x'], // Channel 600X
    ['TAC','600Y','navaid:tactical_channel','channel_600y'], // Channel 600Y
    ['TAC','601X','navaid:tactical_channel','channel_601x'], // Channel 601X
    ['TAC','601Y','navaid:tactical_channel','channel_601y'], // Channel 601Y
    ['TAC','602X','navaid:tactical_channel','channel_602x'], // Channel 602X
    ['TAC','602Y','navaid:tactical_channel','channel_602y'], // Channel 602Y
    ['TAC','603X','navaid:tactical_channel','channel_603x'], // Channel 603X
    ['TAC','603Y','navaid:tactical_channel','channel_603y'], // Channel 603Y
    ['TAC','604X','navaid:tactical_channel','channel_604x'], // Channel 604X
    ['TAC','604Y','navaid:tactical_channel','channel_604y'], // Channel 604Y
    ['TAC','605X','navaid:tactical_channel','channel_605x'], // Channel 605X
    ['TAC','605Y','navaid:tactical_channel','channel_605y'], // Channel 605Y
    ['TAC','606X','navaid:tactical_channel','channel_606x'], // Channel 606X
    ['TAC','606Y','navaid:tactical_channel','channel_606y'], // Channel 606Y
    ['TAC','607X','navaid:tactical_channel','channel_607x'], // Channel 607X
    ['TAC','607Y','navaid:tactical_channel','channel_607y'], // Channel 607Y
    ['TAC','608X','navaid:tactical_channel','channel_608x'], // Channel 608X
    ['TAC','608Y','navaid:tactical_channel','channel_608y'], // Channel 608Y
    ['TAC','609X','navaid:tactical_channel','channel_609x'], // Channel 609X
    ['TAC','609Y','navaid:tactical_channel','channel_609y'], // Channel 609Y
    ['TAC','610X','navaid:tactical_channel','channel_610x'], // Channel 610X
    ['TAC','610Y','navaid:tactical_channel','channel_610y'], // Channel 610Y
    ['TAC','611X','navaid:tactical_channel','channel_611x'], // Channel 611X
    ['TAC','611Y','navaid:tactical_channel','channel_611y'], // Channel 611Y
    ['TAC','612X','navaid:tactical_channel','channel_612x'], // Channel 612X
    ['TAC','612Y','navaid:tactical_channel','channel_612y'], // Channel 612Y
    ['TAC','613X','navaid:tactical_channel','channel_613x'], // Channel 613X
    ['TAC','613Y','navaid:tactical_channel','channel_613y'], // Channel 613Y
    ['TAC','614X','navaid:tactical_channel','channel_614x'], // Channel 614X
    ['TAC','614Y','navaid:tactical_channel','channel_614y'], // Channel 614Y
    ['TAC','615X','navaid:tactical_channel','channel_615x'], // Channel 615X
    ['TAC','615Y','navaid:tactical_channel','channel_615y'], // Channel 615Y
    ['TAC','616X','navaid:tactical_channel','channel_616x'], // Channel 616X
    ['TAC','616Y','navaid:tactical_channel','channel_616y'], // Channel 616Y
    ['TAC','617X','navaid:tactical_channel','channel_617x'], // Channel 617X
    ['TAC','617Y','navaid:tactical_channel','channel_617y'], // Channel 617Y
    ['TAC','618X','navaid:tactical_channel','channel_618x'], // Channel 618X
    ['TAC','618Y','navaid:tactical_channel','channel_618y'], // Channel 618Y
    ['TAC','619X','navaid:tactical_channel','channel_619x'], // Channel 619X
    ['TAC','619Y','navaid:tactical_channel','channel_619y'], // Channel 619Y
    ['TAC','620X','navaid:tactical_channel','channel_620x'], // Channel 620X
    ['TAC','620Y','navaid:tactical_channel','channel_620y'], // Channel 620Y
    ['TAC','621X','navaid:tactical_channel','channel_621x'], // Channel 621X
    ['TAC','621Y','navaid:tactical_channel','channel_621y'], // Channel 621Y
    ['TAC','622X','navaid:tactical_channel','channel_622x'], // Channel 622X
    ['TAC','622Y','navaid:tactical_channel','channel_622y'], // Channel 622Y
    ['TAC','623X','navaid:tactical_channel','channel_623x'], // Channel 623X
    ['TAC','623Y','navaid:tactical_channel','channel_623y'], // Channel 623Y
    ['TAC','624X','navaid:tactical_channel','channel_624x'], // Channel 624X
    ['TAC','624Y','navaid:tactical_channel','channel_624y'], // Channel 624Y
    ['TAC','625X','navaid:tactical_channel','channel_625x'], // Channel 625X
    ['TAC','625Y','navaid:tactical_channel','channel_625y'], // Channel 625Y
    ['TAC','626X','navaid:tactical_channel','channel_626x'], // Channel 626X
    ['TAC','626Y','navaid:tactical_channel','channel_626y'], // Channel 626Y
    ['TAC','627X','navaid:tactical_channel','channel_627x'], // Channel 627X
    ['TAC','627Y','navaid:tactical_channel','channel_627y'], // Channel 627Y
    ['TAC','628X','navaid:tactical_channel','channel_628x'], // Channel 628X
    ['TAC','628Y','navaid:tactical_channel','channel_628y'], // Channel 628Y
    ['TAC','629X','navaid:tactical_channel','channel_629x'], // Channel 629X
    ['TAC','629Y','navaid:tactical_channel','channel_629y'], // Channel 629Y
    ['TAC','630X','navaid:tactical_channel','channel_630x'], // Channel 630X
    ['TAC','630Y','navaid:tactical_channel','channel_630y'], // Channel 630Y
    ['TAC','631X','navaid:tactical_channel','channel_631x'], // Channel 631X
    ['TAC','631Y','navaid:tactical_channel','channel_631y'], // Channel 631Y
    ['TAC','632X','navaid:tactical_channel','channel_632x'], // Channel 632X
    ['TAC','632Y','navaid:tactical_channel','channel_632y'], // Channel 632Y
    ['TAC','633X','navaid:tactical_channel','channel_633x'], // Channel 633X
    ['TAC','633Y','navaid:tactical_channel','channel_633y'], // Channel 633Y
    ['TAC','634X','navaid:tactical_channel','channel_634x'], // Channel 634X
    ['TAC','634Y','navaid:tactical_channel','channel_634y'], // Channel 634Y
    ['TAC','635X','navaid:tactical_channel','channel_635x'], // Channel 635X
    ['TAC','635Y','navaid:tactical_channel','channel_635y'], // Channel 635Y
    ['TAC','636X','navaid:tactical_channel','channel_636x'], // Channel 636X
    ['TAC','636Y','navaid:tactical_channel','channel_636y'], // Channel 636Y
    ['TAC','637X','navaid:tactical_channel','channel_637x'], // Channel 637X
    ['TAC','637Y','navaid:tactical_channel','channel_637y'], // Channel 637Y
    ['TAC','638X','navaid:tactical_channel','channel_638x'], // Channel 638X
    ['TAC','638Y','navaid:tactical_channel','channel_638y'], // Channel 638Y
    ['TAC','639X','navaid:tactical_channel','channel_639x'], // Channel 639X
    ['TAC','639Y','navaid:tactical_channel','channel_639y'], // Channel 639Y
    ['TAC','640X','navaid:tactical_channel','channel_640x'], // Channel 640X
    ['TAC','640Y','navaid:tactical_channel','channel_640y'], // Channel 640Y
    ['TAC','641X','navaid:tactical_channel','channel_641x'], // Channel 641X
    ['TAC','641Y','navaid:tactical_channel','channel_641y'], // Channel 641Y
    ['TAC','642X','navaid:tactical_channel','channel_642x'], // Channel 642X
    ['TAC','642Y','navaid:tactical_channel','channel_642y'], // Channel 642Y
    ['TAC','643X','navaid:tactical_channel','channel_643x'], // Channel 643X
    ['TAC','643Y','navaid:tactical_channel','channel_643y'], // Channel 643Y
    ['TAC','644X','navaid:tactical_channel','channel_644x'], // Channel 644X
    ['TAC','644Y','navaid:tactical_channel','channel_644y'], // Channel 644Y
    ['TAC','645X','navaid:tactical_channel','channel_645x'], // Channel 645X
    ['TAC','645Y','navaid:tactical_channel','channel_645y'], // Channel 645Y
    ['TAC','646X','navaid:tactical_channel','channel_646x'], // Channel 646X
    ['TAC','646Y','navaid:tactical_channel','channel_646y'], // Channel 646Y
    ['TAC','647X','navaid:tactical_channel','channel_647x'], // Channel 647X
    ['TAC','647Y','navaid:tactical_channel','channel_647y'], // Channel 647Y
    ['TAC','648X','navaid:tactical_channel','channel_648x'], // Channel 648X
    ['TAC','648Y','navaid:tactical_channel','channel_648y'], // Channel 648Y
    ['TAC','649X','navaid:tactical_channel','channel_649x'], // Channel 649X
    ['TAC','649Y','navaid:tactical_channel','channel_649y'], // Channel 649Y
    ['TAC','650X','navaid:tactical_channel','channel_650x'], // Channel 650X
    ['TAC','650Y','navaid:tactical_channel','channel_650y'], // Channel 650Y
    ['TAC','651X','navaid:tactical_channel','channel_651x'], // Channel 651X
    ['TAC','651Y','navaid:tactical_channel','channel_651y'], // Channel 651Y
    ['TAC','652X','navaid:tactical_channel','channel_652x'], // Channel 652X
    ['TAC','652Y','navaid:tactical_channel','channel_652y'], // Channel 652Y
    ['TAC','653X','navaid:tactical_channel','channel_653x'], // Channel 653X
    ['TAC','653Y','navaid:tactical_channel','channel_653y'], // Channel 653Y
    ['TAC','654X','navaid:tactical_channel','channel_654x'], // Channel 654X
    ['TAC','654Y','navaid:tactical_channel','channel_654y'], // Channel 654Y
    ['TAC','655X','navaid:tactical_channel','channel_655x'], // Channel 655X
    ['TAC','655Y','navaid:tactical_channel','channel_655y'], // Channel 655Y
    ['TAC','656X','navaid:tactical_channel','channel_656x'], // Channel 656X
    ['TAC','656Y','navaid:tactical_channel','channel_656y'], // Channel 656Y
    ['TAC','657X','navaid:tactical_channel','channel_657x'], // Channel 657X
    ['TAC','657Y','navaid:tactical_channel','channel_657y'], // Channel 657Y
    ['TAC','658X','navaid:tactical_channel','channel_658x'], // Channel 658X
    ['TAC','658Y','navaid:tactical_channel','channel_658y'], // Channel 658Y
    ['TAC','659X','navaid:tactical_channel','channel_659x'], // Channel 659X
    ['TAC','659Y','navaid:tactical_channel','channel_659y'], // Channel 659Y
    ['TAC','660X','navaid:tactical_channel','channel_660x'], // Channel 660X
    ['TAC','660Y','navaid:tactical_channel','channel_660y'], // Channel 660Y
    ['TAC','661X','navaid:tactical_channel','channel_661x'], // Channel 661X
    ['TAC','661Y','navaid:tactical_channel','channel_661y'], // Channel 661Y
    ['TAC','662X','navaid:tactical_channel','channel_662x'], // Channel 662X
    ['TAC','662Y','navaid:tactical_channel','channel_662y'], // Channel 662Y
    ['TAC','663X','navaid:tactical_channel','channel_663x'], // Channel 663X
    ['TAC','663Y','navaid:tactical_channel','channel_663y'], // Channel 663Y
    ['TAC','664X','navaid:tactical_channel','channel_664x'], // Channel 664X
    ['TAC','664Y','navaid:tactical_channel','channel_664y'], // Channel 664Y
    ['TAC','665X','navaid:tactical_channel','channel_665x'], // Channel 665X
    ['TAC','665Y','navaid:tactical_channel','channel_665y'], // Channel 665Y
    ['TAC','666X','navaid:tactical_channel','channel_666x'], // Channel 666X
    ['TAC','666Y','navaid:tactical_channel','channel_666y'], // Channel 666Y
    ['TAC','667X','navaid:tactical_channel','channel_667x'], // Channel 667X
    ['TAC','667Y','navaid:tactical_channel','channel_667y'], // Channel 667Y
    ['TAC','668X','navaid:tactical_channel','channel_668x'], // Channel 668X
    ['TAC','668Y','navaid:tactical_channel','channel_668y'], // Channel 668Y
    ['TAC','669X','navaid:tactical_channel','channel_669x'], // Channel 669X
    ['TAC','669Y','navaid:tactical_channel','channel_669y'], // Channel 669Y
    ['TAC','670X','navaid:tactical_channel','channel_670x'], // Channel 670X
    ['TAC','670Y','navaid:tactical_channel','channel_670y'], // Channel 670Y
    ['TAC','671X','navaid:tactical_channel','channel_671x'], // Channel 671X
    ['TAC','671Y','navaid:tactical_channel','channel_671y'], // Channel 671Y
    ['TAC','672X','navaid:tactical_channel','channel_672x'], // Channel 672X
    ['TAC','672Y','navaid:tactical_channel','channel_672y'], // Channel 672Y
    ['TAC','673X','navaid:tactical_channel','channel_673x'], // Channel 673X
    ['TAC','673Y','navaid:tactical_channel','channel_673y'], // Channel 673Y
    ['TAC','674X','navaid:tactical_channel','channel_674x'], // Channel 674X
    ['TAC','674Y','navaid:tactical_channel','channel_674y'], // Channel 674Y
    ['TAC','675X','navaid:tactical_channel','channel_675x'], // Channel 675X
    ['TAC','675Y','navaid:tactical_channel','channel_675y'], // Channel 675Y
    ['TAC','676X','navaid:tactical_channel','channel_676x'], // Channel 676X
    ['TAC','676Y','navaid:tactical_channel','channel_676y'], // Channel 676Y
    ['TAC','677X','navaid:tactical_channel','channel_677x'], // Channel 677X
    ['TAC','677Y','navaid:tactical_channel','channel_677y'], // Channel 677Y
    ['TAC','678X','navaid:tactical_channel','channel_678x'], // Channel 678X
    ['TAC','678Y','navaid:tactical_channel','channel_678y'], // Channel 678Y
    ['TAC','679X','navaid:tactical_channel','channel_679x'], // Channel 679X
    ['TAC','679Y','navaid:tactical_channel','channel_679y'], // Channel 679Y
    ['TAC','680X','navaid:tactical_channel','channel_680x'], // Channel 680X
    ['TAC','680Y','navaid:tactical_channel','channel_680y'], // Channel 680Y
    ['TAC','681X','navaid:tactical_channel','channel_681x'], // Channel 681X
    ['TAC','681Y','navaid:tactical_channel','channel_681y'], // Channel 681Y
    ['TAC','682X','navaid:tactical_channel','channel_682x'], // Channel 682X
    ['TAC','682Y','navaid:tactical_channel','channel_682y'], // Channel 682Y
    ['TAC','683X','navaid:tactical_channel','channel_683x'], // Channel 683X
    ['TAC','683Y','navaid:tactical_channel','channel_683y'], // Channel 683Y
    ['TAC','684X','navaid:tactical_channel','channel_684x'], // Channel 684X
    ['TAC','684Y','navaid:tactical_channel','channel_684y'], // Channel 684Y
    ['TAC','685X','navaid:tactical_channel','channel_685x'], // Channel 685X
    ['TAC','685Y','navaid:tactical_channel','channel_685y'], // Channel 685Y
    ['TAC','686X','navaid:tactical_channel','channel_686x'], // Channel 686X
    ['TAC','686Y','navaid:tactical_channel','channel_686y'], // Channel 686Y
    ['TAC','687X','navaid:tactical_channel','channel_687x'], // Channel 687X
    ['TAC','687Y','navaid:tactical_channel','channel_687y'], // Channel 687Y
    ['TAC','688X','navaid:tactical_channel','channel_688x'], // Channel 688X
    ['TAC','688Y','navaid:tactical_channel','channel_688y'], // Channel 688Y
    ['TAC','689X','navaid:tactical_channel','channel_689x'], // Channel 689X
    ['TAC','689Y','navaid:tactical_channel','channel_689y'], // Channel 689Y
    ['TAC','690X','navaid:tactical_channel','channel_690x'], // Channel 690X
    ['TAC','690Y','navaid:tactical_channel','channel_690y'], // Channel 690Y
    ['TAC','691X','navaid:tactical_channel','channel_691x'], // Channel 691X
    ['TAC','691Y','navaid:tactical_channel','channel_691y'], // Channel 691Y
    ['TAC','692X','navaid:tactical_channel','channel_692x'], // Channel 692X
    ['TAC','692Y','navaid:tactical_channel','channel_692y'], // Channel 692Y
    ['TAC','693X','navaid:tactical_channel','channel_693x'], // Channel 693X
    ['TAC','693Y','navaid:tactical_channel','channel_693y'], // Channel 693Y
    ['TAC','694X','navaid:tactical_channel','channel_694x'], // Channel 694X
    ['TAC','694Y','navaid:tactical_channel','channel_694y'], // Channel 694Y
    ['TAC','695X','navaid:tactical_channel','channel_695x'], // Channel 695X
    ['TAC','695Y','navaid:tactical_channel','channel_695y'], // Channel 695Y
    ['TAC','696X','navaid:tactical_channel','channel_696x'], // Channel 696X
    ['TAC','696Y','navaid:tactical_channel','channel_696y'], // Channel 696Y
    ['TAC','697X','navaid:tactical_channel','channel_697x'], // Channel 697X
    ['TAC','697Y','navaid:tactical_channel','channel_697y'], // Channel 697Y
    ['TAC','698X','navaid:tactical_channel','channel_698x'], // Channel 698X
    ['TAC','698Y','navaid:tactical_channel','channel_698y'], // Channel 698Y
    ['TAC','699X','navaid:tactical_channel','channel_699x'], // Channel 699X
    ['TAC','699Y','navaid:tactical_channel','channel_699y'], // Channel 699Y
    ['TAC','700X','navaid:tactical_channel','channel_700x'], // Channel 700X
    ['TAC','700Y','navaid:tactical_channel','channel_700y'], // Channel 700Y
    ['TAC','NULL','navaid:tactical_channel','null'], // NULL

    // TCS - Cross-sectional Profile
    ['TCS','-999999','culvert:profile','unknown'], // No Information
    ['TCS','1','culvert:profile','arch'], // Arch
    ['TCS','2','culvert:profile','box'], // Box
    ['TCS','3','culvert:profile','semicircular'], // Semicircular
    ['TCS','999','culvert:profile','other'], // Other

    // TEL - Telescope Type
    ['TEL','-999999','telescope:type','unknown'], // No Information
    ['TEL','1','telescope:type','optical'], // Optical
    ['TEL','2','telescope:type','parabolic_radio_aerial'], // Parabolic Radio Aerial
    ['TEL','3','telescope:type','radio_aerial_array'], // Radio Aerial Array
    ['TEL','999','telescope:type','other'], // Other

    // THR - Through Route
    ['THR','-999999','highway:through_route','unknown'], // No Information
    ['THR','1000','highway:through_route','no'],
    ['THR','1001','highway:through_route','yes'], // Need to look at custom rules to deconflict this

    // TID - Tide Influenced
    ['TID','-999999','tidal','unknown'], // No Information
    ['TID','1000','tidal','no'],
    ['TID','1001','tidal','yes'],

    // TOS - Tower Shape
    ['TOS','-999999','tower:shape','unknown'], // No Information
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
    ['TRA','-999999','foot','unknown'], // No Information
    ['TRA','1000','foot','no'],
    ['TRA','1001','foot','yes'],

    // TRE - Foliage Type
    ['TRE','-999999','leaf_cycle','unknown'], // No Information
    ['TRE','1','leaf_cycle','deciduous'], // Deciduous
    ['TRE','2','leaf_cycle','evergreen'], // Evergreen
    ['TRE','3','leaf_cycle','mixed'], // Mixed
    ['TRE','999','leaf_cycle','other'], // Other

    // TRP - Transportation Route Protection Structure Type
    ['TRP','-999999','protection_structure','unknown'], // No Information
    ['TRP','1','tunnel','avalanche_protector'], // Gallery
    ['TRP','2','man_made','rock_shed'], // Rock Protection Shed
    ['TRP','3','man_made','snow_fence'], // Snow Protection Shed
    ['TRP','4','man_made','protection_shed'], // Protection Shed
    ['TRP','999','protection_structure','other'], // Other

    // TRS - Transportation System Type
    ['TRS','-999999','transport:type','unknown'], // No Information
    ['TRS','1','transport:type','air'],
    ['TRS','2','transport:type','aqueduct'], // Aqueduct
    ['TRS','3','transport:type','automotive'], // From TRD3.0 to stop warnings Automotive
    ['TRS','4','transport:type','bus'], // Bus
    ['TRS','5','transport:type','canal'], // Canal
    ['TRS','6','transport:type','caravan_route'], // Caravan Route
    ['TRS','7','transport:type','maritime'],
    ['TRS','8','transport:type','none'], // No Transportation System
    ['TRS','9','transport:type','pedestrian'], // Pedestrian
    ['TRS','10','transport:type','pipeline'], // Pipeline
    ['TRS','11','transport:type','portage'], // Portage
    ['TRS','12','transport:type','railway'], // Railway
    ['TRS','13','transport:type','road'], // Road
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

    // TRS2 - Transportation System Type [2]
    // TRS3 - Transportation System Type [3]

    // TSM - Terrain Surface Material - Added to ZI016_ROC list
    ['TSM','-999999','terrain_surface','unknown'], // No Information
    ['TSM','1','terrain_surface','asphalt'], // Asphalt
    ['TSM','2','terrain_surface','bedrock'], // Bedrock
    ['TSM','3','terrain_surface','boulders'], // Boulders
    ['TSM','4','terrain_surface','concrete'], // Concrete
    ['TSM','5','terrain_surface','evaporite'], // Evaporite
    ['TSM','6','terrain_surface','frozen_water'], // Frozen Water
    ['TSM','7','terrain_surface','gravel'], // Gravel
    ['TSM','8','terrain_surface','lava_flow'], // Lava Flow
    ['TSM','9','terrain_surface','loess'], // Loess
    ['TSM','10','terrain_surface','mud'], // Mud
    ['TSM','11','terrain_surface','paved'], // Paved
    ['TSM','12','terrain_surface','rock'], // Rock
    ['TSM','13','terrain_surface','sand'], // Sand
    ['TSM','14','terrain_surface','soil'], // Soil
    ['TSM','999','terrain_surface','other'], // Other

    // TSM2 - Terrain Surface Material [2]
    // TSM3 - Terrain Surface Material [3]

    // TST - Cable Suspended Shape
    ['TST','-999999','cable:shape','unknown'], // No Information
    ['TST','1','cable:shape','symmetric_catenary'], // Symmetric Catenary
    ['TST','2','cable:shape','mountain_catenary'], // Mountain Catenary
    ['TST','3','cable:shape','overwater_catenary'], // Overwater Catenary
    ['TST','999','cable:shape','other'], // Other

    // TTC - Tower Type
    ['TTC','-999999','tower:type','unknown'], // No Information
    ['TTC','2','tower:type','observation'], // Observation Tower
    ['TTC','10','tower:type','lookout'], // Lookout Tower
    ['TTC','12','tower:type','fire'], // Fire Tower
    ['TTC','20','tower:type','communication'], // Telecommunication Tower
    ['TTC','21','tower:type','guard'], // Guard Tower
    ['TTC','22','tower:type','industrial'], // Industrial Tower
    ['TTC','23','tower:type','drop'], // Drop Tower
    ['TTC','24','tower:type','solar_power'], // Solar Power Tower
    ['TTC','999','tower:type','other'], // Other

    // TTC2 - Tower Type [2]
    // TTC3 - Tower Type [3]

    // TTY - Tomb Type
    ['TTY','-999999','tomb','unknown'], // No Information
    ['TTY','1','tomb','cave'], // Cave
    ['TTY','2','tomb','catacomb'], // Catacomb
    ['TTY','3','tomb','crypt'], // Crypt
    ['TTY','4','tomb','surface_vault'], // Surface Vault
    ['TTY','5','tomb','mausoleum'], // Mausoleum
    ['TTY','6','tomb','burial_mound'], // Burial Mound
    ['TTY','7','tomb','columbarium'], // Columbarium
    ['TTY','999','tomb','other'], // Other

    // TXP - Taxiway Type
    ['TXP','-999999','taxiway:type','unknown'], // No Information
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
    ['UMA','-999999','mine:access','unknown'], // No Information
    ['UMA','1','mine:access','drift'], // Drift
    ['UMA','2','mine:access','slope'], // Slope
    ['UMA','3','mine:access','shaft'], // Shaft
    // ['UMA','998',undefined,undefined], // Not Applicable
    ['UMA','998','mine:access','not_applicable'], // Not Applicable

    // VCA - Void Collection Reason
    ['VCA','-999999','void_collection:reason','unknown'], // No Information
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

    // VCA2 - Void Collection Reason [2]
    // VCA3 - Void Collection Reason [3]

    // VCM - Vertical Construction Material
    ['VCM','-999999','material:vertical','unknown'], // No Information
    ['VCM','1','material:vertical','adobe_brick'], // Adobe Brick
    ['VCM','2','material:vertical','aluminium'], // Aluminum
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

    // VCM2 - Vertical Construction Material [2]
    // VCM3 - Vertical Construction Material [3]

    // VCT - Void Collection Type
    ['VCT','-999999','void_collection:type','unknown'], // No Information
    ['VCT','1','void_collection:type','hypsography'], // Hypsography
    ['VCT','3','void_collection:type','bathymetry'], // Bathymetry
    ['VCT','4','void_collection:type','waterbody_bottom_composition'], // Waterbody Bottom Composition
    ['VCT','999','void_collection:type','other'], // Other

    // VCT2 - Void Collection Type [2]
    // VCT3 - Void Collection Type [3]

    // VDT - Vertical Datum
    ['VDT','-999999','source:datum:vertical','unknown'], // No Information
    ['VDT','1','source:datum:vertical','wgs_84_ellipsoid'], // WGS 84 Ellipsoid
    ['VDT','2','source:datum:vertical','wgs_84_egm96_geoid'], // WGS 84 EGM96 Geoid
    ['VDT','3','source:datum:vertical','mean_sea_level_(msl)'], // Mean Sea Level (MSL)
    ['VDT','4','source:datum:vertical','north_american_vertical_datum_(navd)_1988'], // North American Vertical Datum (NAVD) 1988
    ['VDT','5','source:datum:vertical','national_geodetic_vertical_datum_(ngvd)_1929'], // National Geodetic Vertical Datum (NGVD) 1929
    ['VDT','6','source:datum:vertical','ground_level'], // Ground Level
    ['VDT','7','source:datum:vertical','wgs_84_egm08_geoid'], // WGS 84 EGM08 Geoid
    ['VDT','998','source:datum:vertical','not_applicable'], // Not Applicable
    ['VDT','999','source:datum:vertical','other'], // Other


    // VEG - Vegetation Characteristic
    ['VEG','-999999','grassland:type','unknown'], // No Information
    ['VEG','8','grassland:type','grassland'], // Grassland
    ['VEG','9','grassland:type','grassland_with_trees'], // Grassland with Trees
    ['VEG','10','grassland:type','tropical_grass'], // Tropical Grass
    ['VEG','56','grassland:type','without_trees'], // Without Trees
    ['VEG','83','grassland:type','meadow'], // Meadow
    ['VEG','999','grassland:type','other'], // Other

    // VET - Vehicle Type
    ['VET','-999999','vehicle:type','unknown'], // No Information
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
    ['VGT','-999999','volcano:type','unknown'], // No Information
    ['VGT','1','volcano:type','cone'], // Cone
    ['VGT','2','volcano:type','cinder_cone'], // Cinder Cone
    ['VGT','3','volcano:type','shield'], // Shield
    ['VGT','4','volcano:type','caldera'], // Caldera
    ['VGT','5','volcano:type','composite'], // Composite

    // VOA - Volcanic Activity
    ['VOA','-999999','volcano:status','unknown'], // No Information
    ['VOA','1','volcano:status','active'], // Active
    ['VOA','2','volcano:status','dormant'], // Dormant
    ['VOA','3','volcano:status','inactive_or_extinct'], // Inactive or Extinct

    // VSP - Vegetation Species
    ['VSP','-999999','vegetation:type','unknown'], // No Information
    ['VSP','8','vegetation:type','cypress'], // Cypress
    ['VSP','19','vegetation:type','mangrove'], // Mangrove
    ['VSP','22','vegetation:type','nipa'], // Nipa
    ['VSP','25','vegetation:type','palm'], // Palm
    ['VSP','34','vegetation:type','bamboo'], // Bamboo
    ['VSP','64','vegetation:type','swamp_cypress'], // Swamp Cypress
    ['VSP','999','vegetation:type','other'], // Other

    // VSP2 - Vegetation Species [2]
    // VSP3 - Vegetation Species [3]

    // WCC - Watercourse Channel Type
    ['WCC','-999999','channel:type','unknown'], // No Information
    ['WCC','1','channel:type','channelized_stream'], // Channelized Stream
    ['WCC','2','channel:type','braided_stream'], // Braided Stream
    ['WCC','3','natural','gorge'], // Gorge
    ['WCC','4','wadi','yes'], // Wadi
    ['WCC','7','channel:type','normal'], // Normal Channel
    ['WCC','999','waterway','other'], // Other

    // WEQ - Well Equipment
    ['WEQ','-999999','well:equipment','unknown'], // No Information
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

    // WEQ2 - Well Equipment [2]
    // WEQ3 - Well Equipment [3]

    // WFT - Well Type
    ['WFT','-999999','water_well:type','unknown'], // No Information
    ['WFT','2','water_well:type','walled-in'], // Walled-in
    ['WFT','3','water_well:type','artesian'], // Artesian
    ['WFT','5','water_well:type','dug_or_drilled'], // Dug or Drilled
    ['WFT','6','water_well:type','dug'], // Dug
    ['WFT','7','water_well:type','drilled'], // Drilled
    ['WFT','999','water_well:type','other'], // Other

    // WLE - Hydrographic Vertical Positioning Information : Water Level Effect
    ['WLE','-999999','hydrographic_vertical_position','unknown'], // No Information
    ['WLE','1','hydrographic_vertical_position','partly_submerged'], // Partly Submerged
    ['WLE','2','hydrographic_vertical_position','always_dry'], // Always Dry
    ['WLE','3','hydrographic_vertical_position','always_submerged'], // Always Submerged
    ['WLE','4','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
    ['WLE','5','hydrographic_vertical_position','awash_at_low_water'], // Awash at Low Water
    ['WLE','9','hydrographic_vertical_position','awash_at_chart_datum'], // Awash at Chart Datum
    ['WLE','999','hydrographic_vertical_position','other'], // Other

    // WLO - Wreck or Hulk Exposure
    ['WLO','-999999','wreck_exposure','unknown'], // No Information
    ['WLO','1','wreck_exposure','hull_showing'], // Hull Showing
    ['WLO','2','wreck_exposure','masts_showing'], // Masts Showing
    ['WLO','3','wreck_exposure','funnel_showing'], // Funnel Showing
    ['WLO','4','wreck_exposure','superstructure_showing'], // Superstructure Showing
    ['WLO','5','wreck_exposure','masts_and_funnel_showing'], // Masts and Funnel Showing
    ['WLO','999','wreck_exposure','other'], // Other

    // WMT - Watercourse Morphology
    ['WMT','-999999','water:morphology','unknown'], // No Information
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
    ['WRT','-999999','water:race','unknown'], // No Information
    ['WRT','1','water:race','sluice'], // Sluice
    ['WRT','2','water:race','flume'], // Flume
    ['WRT','3','water:race','headrace'], // Headrace
    ['WRT','4','water:race','tailrace'], // Tailrace

    // WST - Watercourse Sink Type: No OSM even close
    ['WST','-999999','water:sink:type','unknown'], // No Information
    ['WST','1','water:sink:type','dissipating'], // Dissipating
    ['WST','2','water:sink:type','disappearing'], // Disappearing
    ['WST','3','water:sink:type','sinkhole'], // Sinkhole
    ['WST','4','water:sink:type','hole'], // Hole
    ['WST','999','water:sink:type','other'], // Other

    // WTI - Wall Type
    ['WTI','-999999','wall','unknown'], // No Information
    ['WTI','1','wall','free-standing'], // Free-standing
    ['WTI','2','wall','retaining'], // Retaining
    ['WTI','999','wall','other'], // Other

    // YSU - Military Service Branch
    ['YSU','-999999','military:service','unknown'], // noInformation
    ['YSU','1','military:service','air_force'], // Air Force
    ['YSU','2','military:service','army'], // Army
    ['YSU','3','military:service','coastguard'], // Coastguard
    ['YSU','4','military:service','marines'], // Marines
    ['YSU','5','military:service','navy'], // Navy
    ['YSU','7','military:service','joint'], // Joint
    ['YSU','8','military:service','air_national_guard'], // Air National Guard
    ['YSU','9','military:service','army_national_guard'], // Army National Guard
    ['YSU','998',undefined,undefined], // Not Applicable
    ['YSU','998','military:service','not_applicable'], // Not Applicable
    ['YSU','999','military:service','other'], // Other

    // YWQ - Water Potability See ZI024_YWQ
    // ['YWQ','-999999','raw:YWQ','unknown'], // noInformation
    // ['YWQ','1','raw:YWQ','potable'], // Potable
    // ['YWQ','2','raw:YWQ','treatable'], // Treatable
    // ['YWQ','3','raw:YWQ','contaminated'], // Contaminated
    // ['YWQ','4','raw:YWQ','nonpotable'], // Nonpotable
    // ['YWQ','999','raw:YWQ','other'], // Other

    // ZHBH_TEC - Hydrographic Base Height : Sounding Metadata : Bathymetric Measurement Technique
    ['ZHBH_TEC','-999999','source:datum:sounding:method','unknown'], // noInformation
    ['ZHBH_TEC','6','source:datum:sounding:method','area_swept_by_wire-drag'], // Area Swept by Wire-drag
    ['ZHBH_TEC','999','source:datum:sounding:method','other'], // Other
    ['ZHBH_TEC','1001','source:datum:sounding:method','depth_known_by_other_then_wire-drag'],  // Depth Known by Other Than Wire Drag

    // ZI001_SPS - Source Information : Spatial Source
    ['ZI001_SPS','-999999','source:spatial_source','unknown'], // No Information
    ['ZI001_SPS','1000','source:spatial_source','no'],
    ['ZI001_SPS','1001','source:spatial_source','yes'],

    // ZI001_SRT - Source : Source Type
    ['ZI001_SRT','-999999','source:name','unknown'], // No Information
    ['ZI001_SRT','2','source:name','automated_aeronautical_facilities_information_file_(aafif)'], // AAFIF
    ['ZI001_SRT','7','source:name','controlled_imagery_base_1_(cib1)'], // CIB1
    ['ZI001_SRT','9','source:name','quickbird_imagery'], // QuickBird Imagery
    ['ZI001_SRT','10','source:name','digital_nautical_chart_(dnc)'], // DNC
    ['ZI001_SRT','21','source:name','digital_vertical_obstruction_file_(dvof)'], // DVOF
    ['ZI001_SRT','22','source:name','foundation_feature_data_(ffd)'], // FFD
    ['ZI001_SRT','25','source:name','geonames'], // GeoNames
    ['ZI001_SRT','26','source:name','gps_field_collected_open_source'], // GPS
    ['ZI001_SRT','28','source:name','ikonos_imagery'], // Ikonos Imagery
    ['ZI001_SRT','29','source:name','ntm_imagery'], // NTM Imagery
    ['ZI001_SRT','30','source:name','imagery'], // Imagery
    ['ZI001_SRT','31','source:name','interim_terrain_data_(itd)'], // ITD
    ['ZI001_SRT','32','source:name','interim_vector_data_(ivd)'], // IVD
    ['ZI001_SRT','43','source:name','modernized_integrated_data_base_(midb)'], // MIDB
    ['ZI001_SRT','60','source:name','urban_vector_map_(uvmap)'], // UVMap
    ['ZI001_SRT','61','source:name','vector_interim_terrain_data_(vitd)'], // VITD
    ['ZI001_SRT','64','source:name','vector_map_lv2_(vmap2)'], // VMap 2
    ['ZI001_SRT','108','source:name','navteq_data'], // NAVTEQ Data
    ['ZI001_SRT','159','source:name','commercial_data'], // Commercial Data
    ['ZI001_SRT','160','source:name','open_source_information'], // Open Source Information
    ['ZI001_SRT','165','source:name','digital_aeronautical_flight_information_file_(dafif)'], // DAFIF
    ['ZI001_SRT','166','source:name','delorme_digital_atlas_of_the_earth_(dae)'], // DeLorme Digital Atlas of the Earth (DAE)
    ['ZI001_SRT','167','source:name','military_map_data'], // Military Map Data
    ['ZI001_SRT','168','source:name','non-military_map'], // Non-military Map
    ['ZI001_SRT','169','source:name','operations_data'], // Operations Data
    ['ZI001_SRT','170','source:name','stereo_airfield_collection_(sac)'], // SAC
    ['ZI001_SRT','171','source:name','tomtom_data'], // TomTom Data
    ['ZI001_SRT','999','source:name','other'], // Other

    // ZI001_VSC - Source Information : Vertical Source Category
    ['ZI001_VSC','-999999','source:vertical_source:type','unknown'], // No Information
    ['ZI001_VSC','1','source:vertical_source:type','dted_1'], // DTED 1
    ['ZI001_VSC','2','source:vertical_source:type','dted_2'], // DTED 2
    ['ZI001_VSC','3','source:vertical_source:type','no_elevations'], // No Elevations
    ['ZI001_VSC','4','source:vertical_source:type','reflective_surface'], // Reflective Surface
    ['ZI001_VSC','5','source:vertical_source:type','stereoscopic_imagery'], // Stereoscopic Imagery
    ['ZI001_VSC','6','source:vertical_source:type','tin_data'], // TIN Data
    ['ZI001_VSC','998','source:vertical_source:type','not_applicable'], // Not Applicable
    ['ZI001_VSC','999','source:vertical_source:type','other'], // Other

    // ZI004_RCG - Feature Metadata : Process Step Information : Resource Content Originator
    ['ZI004_RCG','-999999','attribution','unknown'], // noInformation
    ['ZI004_RCG','1','attribution','u.s._army'], // U.S. Army
    ['ZI004_RCG','2','attribution','u.s._navy'], // U.S. Navy
    ['ZI004_RCG','3','attribution','u.s._air_force'], // U.S. Air Force
    ['ZI004_RCG','4','attribution','u.s._marine_corps'], // U.S. Marine Corps
    ['ZI004_RCG','5','attribution','u.s._coast_guard'], // U.S. Coast Guard
    ['ZI004_RCG','6','attribution','u.s._africa_command_(usafricom)'], // U.S. Africa Command (USAFRICOM)
    ['ZI004_RCG','7','attribution','u.s._central_command_(uscentcom)'], // U.S. Central Command (USCENTCOM)
    ['ZI004_RCG','8','attribution','u.s._european_command_(useucom)'], // U.S. European Command (USEUCOM)
    ['ZI004_RCG','9','attribution','u.s._joint_forces_command_(usjfcom)'], // U.S. Joint Forces Command (USJFCOM)
    ['ZI004_RCG','10','attribution','u.s._northern_command_(usnorthcom)'], // U.S. Northern Command (USNORTHCOM)
    ['ZI004_RCG','11','attribution','u.s._pacific_command_(pacom)'], // U.S. Pacific Command (PACOM)
    ['ZI004_RCG','12','attribution','u.s._special_operations_command_(ussocom)'], // U.S. Special Operations Command (USSOCOM)
    ['ZI004_RCG','13','attribution','u.s._southern_command_(ussouthcom)'], // U.S. Southern Command (USSOUTHCOM)
    ['ZI004_RCG','14','attribution','u.s._strategic_command_(usstratcom)'], // U.S. Strategic Command (USSTRATCOM)
    ['ZI004_RCG','15','attribution','u.s._transportation_command_(ustranscom)'], // U.S. Transportation Command (USTRANSCOM)
    ['ZI004_RCG','16','attribution','u.s._central_intelligence_agency_(cia)'], // U.S. Central Intelligence Agency (CIA)
    ['ZI004_RCG','17','attribution','u.s._defense_intelligence_agency_(dia)'], // U.S. Defense Intelligence Agency (DIA)
    ['ZI004_RCG','18','attribution','u.s._national_security_agency_(nsa)'], // U.S. National Security Agency (NSA)
    ['ZI004_RCG','19','attribution','u.s._national_geospatial-intelligence_agency_(nga)'], // U.S. National Geospatial-Intelligence Agency (NGA)
    ['ZI004_RCG','20','attribution','u.s._national_reconnaissance_office_(nro)'], // U.S. National Reconnaissance Office (NRO)
    ['ZI004_RCG','21','attribution','u.s._department_of_state'], // U.S. Department of State
    ['ZI004_RCG','22','attribution','u.s._department_of_homeland_security_(dhs)'], // U.S. Department of Homeland Security (DHS)
    ['ZI004_RCG','23','attribution','u.s._department_of_energy_(doe)'], // U.S. Department of Energy (DOE)
    ['ZI004_RCG','24','attribution','u.s._federal_bureau_of_investigation_(fbi)'], // U.S. Federal Bureau of Investigation (FBI)
    ['ZI004_RCG','25','attribution','u.s._geological_survey_(usgs)'], // U.S. Geological Survey (USGS)
    ['ZI004_RCG','26','attribution','u.s._national_civil_applications_program_(ncap)'], // U.S. National Civil Applications Program (NCAP)
    ['ZI004_RCG','27','attribution','u.s._national_oceanic_and_atmospheric_administration'], // U.S. National Oceanic and Atmospheric Administration
    ['ZI004_RCG','28','attribution','australian_geospatial-intelligence_organization_(australia)'], // Australian Geospatial-Intelligence Organization (Australia)
    ['ZI004_RCG','29','attribution','geographic_service_(belgium)'], // Geographic Service (Belgium)
    ['ZI004_RCG','30','attribution','military_topographic_service_(bulgaria)'], // Military Topographic Service (Bulgaria)
    ['ZI004_RCG','31','attribution','mapping_and_charting_establishment_(canada)'], // Mapping and Charting Establishment (Canada)
    ['ZI004_RCG','32','attribution','geographic_service_of_the_czech_armed_forces_(czech_republic)'], // Geographic Service of the Czech Armed Forces (Czech Republic)
    ['ZI004_RCG','33','attribution','defence_acquisition_and_logistics_organization_(denmark)'], // Defence Acquisition and Logistics Organization (Denmark)
    ['ZI004_RCG','34','attribution','military_geographic_group_(estonia)'], // Military Geographic Group (Estonia)
    ['ZI004_RCG','35','attribution','topographic_service_(finland)'], // Topographic Service (Finland)
    ['ZI004_RCG','36','attribution','bureau_geographie_hydrographie_oceanographie_et_meteorologie_(france)'], // Bureau Geographie, Hydrographie, Oceanographie et Meteorologie (France)
    ['ZI004_RCG','37','attribution','bundeswehr_geoinformation_office_(germany)'], // Bundeswehr Geoinformation Office (Germany)
    ['ZI004_RCG','38','attribution','hellenic_military_geographic_service_(greece)'], // Hellenic Military Geographic Service (Greece)
    ['ZI004_RCG','39','attribution','geoinformation_service_of_the_hungarian_defence_forces_(hungary)'], // Geoinformation Service of the Hungarian Defence Forces (Hungary)
    ['ZI004_RCG','40','attribution','defense_information_security_(italy)'], // Defense Information Security (Italy)
    ['ZI004_RCG','41','attribution','geospatial_information_agency_(latvia)'], // Geospatial Information Agency (Latvia)
    ['ZI004_RCG','42','attribution','military_mapping_centre_(lithuania)'], // Military Mapping Centre (Lithuania)
    ['ZI004_RCG','43','attribution','national_army_topographic_service_(moldova)'], // National Army Topographic Service (Moldova)
    ['ZI004_RCG','44','attribution','army_geographic_agency_(netherlands)'], // Army Geographic Agency (Netherlands)
    ['ZI004_RCG','45','attribution','geoint_new_zealand_(new_zealand)'], // GEOINT New Zealand (New Zealand)
    ['ZI004_RCG','46','attribution','military_geographic_service_(norway)'], // Military Geographic Service (Norway)
    ['ZI004_RCG','47','attribution','military_geography_division_(poland)'], // Military Geography Division (Poland)
    ['ZI004_RCG','48','attribution','army_geographic_institute_(portugal)'], // Army Geographic Institute (Portugal)
    ['ZI004_RCG','49','attribution','military_topographic_directorate_(romania)'], // Military Topographic Directorate (Romania)
    ['ZI004_RCG','50','attribution','topographic_institute_(slovakia)'], // Topographic Institute (Slovakia)
    ['ZI004_RCG','51','attribution','army_geographic_centre_(spain)'], // Army Geographic Centre (Spain)
    ['ZI004_RCG','52','attribution','swedish_armed_forces_(sweden)'], // Swedish Armed Forces (Sweden)
    ['ZI004_RCG','53','attribution','general_command_of_mapping_(turkey)'], // General Command of Mapping (Turkey)
    ['ZI004_RCG','54','attribution','defence_geographic_centre_intelligence_collection_group_(united_kingdom)'], // Defence Geographic Centre Intelligence Collection Group (United Kingdom)
    ['ZI004_RCG','55','attribution','u.s._army_geospatial_center_(agc)'], // U.S. Army Geospatial Center (AGC)
    ['ZI004_RCG','56','attribution','army_(australia)'], // Army (Australia)
    ['ZI004_RCG','57','attribution','military_geographic_division_(croatia)'], // Military Geographic Division (Croatia)
    ['ZI004_RCG','58','attribution','directorate_geospatial_information_(south_africa)'], // Directorate Geospatial Information (South Africa)
    ['ZI004_RCG','59','attribution','korean_defense_intelligence_agency_(south_korea)'], // Korean Defense Intelligence Agency (South Korea)
    ['ZI004_RCG','60','attribution','national_intelligence_service_(south_korea)'], // National Intelligence Service (South Korea)
    ['ZI004_RCG','61','attribution','imagery_support_group_(singapore)'], // Imagery Support Group (Singapore)
    ['ZI004_RCG','62','attribution','national_security_bureau_(taiwan)'], // National Security Bureau (Taiwan)
    ['ZI004_RCG','63','attribution','materiel_production_center_(taiwan)'], // Materiel Production Center (Taiwan)
    ['ZI004_RCG','64','attribution','ministry_of_defense_of_japan_(japan)'], // Ministry of Defense of Japan (Japan)
    ['ZI004_RCG','65','attribution','ministry_of_construction_and_urban_development_(mongolia)'], // Ministry of Construction and Urban Development (Mongolia)
    ['ZI004_RCG','66','attribution','national_mapping_and_resource_information_authority_(philippines)'], // National Mapping and Resource Information Authority (Philippines)
    ['ZI004_RCG','67','attribution','royal_jordanian_geographic_centre_(jordan)'], // Royal Jordanian Geographic Centre (Jordan)
    ['ZI004_RCG','68','attribution','national_survey_authority_(oman)'], // National Survey Authority (Oman)
    ['ZI004_RCG','69','attribution','armed_forces_general_headquarters_(qatar)'], // Armed Forces General Headquarters (Qatar)
    ['ZI004_RCG','70','attribution','ministry_of_defense_of_saudi_arabia_(saudi_arabia)'], // Ministry of Defense of Saudi Arabia (Saudi Arabia)
    ['ZI004_RCG','71','attribution','directorate_of_survey_(kuwait)'], // Directorate of Survey (Kuwait)
    ['ZI004_RCG','72','attribution','military_survey_department_(united_arab_emirates)'], // Military Survey Department (United Arab Emirates)
    ['ZI004_RCG','73','attribution','information_network_security_agency_(ethiopia)'], // Information Network Security Agency (Ethiopia)
    ['ZI004_RCG','74','attribution','ministry_of_state_for_defense_(kenya)'], // Ministry of State for Defense (Kenya)
    ['ZI004_RCG','75','attribution','el_instituto_nacional_de_estadistica_y_geografia_(mexico)'], // El Instituto Nacional de Estadistica y Geografia (Mexico)
    ['ZI004_RCG','76','attribution','instituto_geografico_militar_(chile)'], // Instituto Geografico Militar (Chile)
    ['ZI004_RCG','77','attribution','servicio_geografico_militar_(uruguay)'], // Servicio Geografico Militar (Uruguay)
    ['ZI004_RCG','78','attribution','direccion_del_servicio_geografico_military_(paraguay)'], // Direccion del Servicio Geografico Military (Paraguay)
    ['ZI004_RCG','79','attribution','instituto_geografico_nacional_(peru)'], // Instituto Geografico Nacional (Peru)
    ['ZI004_RCG','80','attribution','instituto_geografico_agustin_codazzi_(colombia)'], // Instituto Geografico Agustin Codazzi (Colombia)
    ['ZI004_RCG','81','attribution','instituto_geografico_y_del_catastro_nacional_(el_salvador)'], // Instituto Geografico y del Catastro Nacional (El Salvador)
    ['ZI004_RCG','82','attribution','instituto_geografico_nacional_(guatemala)'], // Instituto Geografico Nacional (Guatemala)
    ['ZI004_RCG','83','attribution','servicio_geografico_militar_(guatemala)'], // Servicio Geografico Militar (Guatemala)
    ['ZI004_RCG','84','attribution','instituto_cartografico_militar_(dominican_republic)'], // Instituto Cartografico Militar (Dominican Republic)
    ['ZI004_RCG','85','attribution','instituto_nicaraguense_de_estudios_terretoriales_(nicaragua)'], // Instituto Nicaraguense de Estudios Terretoriales (Nicaragua)
    ['ZI004_RCG','86','attribution','direccion_general_de_registros_catastro_y_geografia_(honduras)'], // Direccion General de Registros, Catastro, y Geografia (Honduras)
    ['ZI004_RCG','87','attribution','instituto_geografico_militar_(ecuador)'], // Instituto Geografico Militar (Ecuador)
    ['ZI004_RCG','88','attribution','instituto_geografico_nacional_tommy_guardia_(panama)'], // Instituto Geografico Nacional "Tommy Guardia" (Panama)
    ['ZI004_RCG','89','attribution','instituto_geografico_nacional_(argentina)'], // Instituto Geografico Nacional (Argentina)
    ['ZI004_RCG','90','attribution','diretoria_de_servico_geografico_(brazil)'], // Diretoria de Servico Geografico (Brazil)
    ['ZI004_RCG','998','attribution','not_applicable'], // Not Applicable
    ['ZI004_RCG','999','attribution','other'], // Other

    // ZI013_CSP - Crop Information : Crop Species
    ['ZI013_CSP','-999999','crop','unknown'], // No Information
    ['ZI013_CSP','1','crop','almond'], // Almond
    ['ZI013_CSP','2','trees','apple_trees'], // Apple
    ['ZI013_CSP','3','crop','barley'], // Barley
    ['ZI013_CSP','4','trees','berry'], // Berry
    ['ZI013_CSP','5','trees','carob_trees'], // Carob
    ['ZI013_CSP','6','trees','chestnut_trees'], // Chestnut
    ['ZI013_CSP','7','trees','citrus_trees'], // Citrus
    ['ZI013_CSP','8','crop','cork-oak'], // Cork-Oak
    ['ZI013_CSP','9','trees','cacao_trees'], // Cacao
    ['ZI013_CSP','10','trees','coffea_plants'], // Coffee
    ['ZI013_CSP','11','crop','maize'], // Maize
    ['ZI013_CSP','13','crop','dry_crop'], // Dry Crop
    ['ZI013_CSP','14','crop','fibre_crop'], // Fibre Crop
    ['ZI013_CSP','15','trees','fruit_tree'], // Fruit Tree
    ['ZI013_CSP','16','crop','grape'], // Grape
    ['ZI013_CSP','17','trees','hazel_plants'], // Hazelnut
    ['ZI013_CSP','18','crop','hop'], // Hop
    ['ZI013_CSP','19','trees','maple_trees'], // Maple
    ['ZI013_CSP','20','crop','millet'], // Millet
    ['ZI013_CSP','21','crop','oats'], // Oat
    ['ZI013_CSP','22','crop','oil_crop'], // Oil Crop
    ['ZI013_CSP','23','trees','olive_trees'], // Olive
    ['ZI013_CSP','24','trees','peach_trees'], // Peach
    ['ZI013_CSP','25','crop','peanut'], // Peanut
    ['ZI013_CSP','26','crop','potato'], // Potato
    ['ZI013_CSP','27','crop','pulse'], // Pulse
    ['ZI013_CSP','28','crop','rice'], // Rice
    ['ZI013_CSP','29','trees','rubber_trees'], // Rubber
    ['ZI013_CSP','30','crop','rye'], // Rye
    ['ZI013_CSP','31','crop','sisal'], // Sisal
    ['ZI013_CSP','32','crop','sorghum'], // Sorghum
    ['ZI013_CSP','33','crop','sugarcane'], // Sugar Crop
    ['ZI013_CSP','34','trees','tea_plants'], // Tea
    ['ZI013_CSP','35','crop','tobacco'], // Tobacco
    ['ZI013_CSP','36','crop','tuber'], // Tuber
    ['ZI013_CSP','37','crop','vegetable'], // Vegetable Crop
    ['ZI013_CSP','38','trees','walnut_trees'], // Walnut
    ['ZI013_CSP','39','crop','wheat'], // Wheat
    ['ZI013_CSP','40','trees','date_palms'], // Date
    ['ZI013_CSP','41','trees','banana_plants'], // Banana
    ['ZI013_CSP','42','trees','oil_palm'], // Oil Palm
    ['ZI013_CSP','43','trees','coconut_palms'], // Coconut
    ['ZI013_CSP','44','crop','sugar_cane'], // Sugar Cane
    ['ZI013_CSP','45','crop','cotton'], // Cotton
    ['ZI013_CSP','46','crop','bamboo'], // Bamboo
    ['ZI013_CSP','47','crop','palmetto'], // Palmetto
    ['ZI013_CSP','48','crop','timber'], // Timber
    ['ZI013_CSP','117','crop','opium_poppy'], // Opium Poppy
    ['ZI013_CSP','152','crop','cannabis'], // Cannabis
    ['ZI013_CSP','159','crop','ornamental'], // Ornamental Crop
    ['ZI013_CSP','999','crop','other'], // Other

    // ZI013_CSP2 - Crop Information : Crop Species [2]
    // ZI013_CSP3 - Crop Information : Crop Species [3]

    // ZI013_FFP - Crop Information : Farming Pattern
    ['ZI013_FFP','-999999','farm:pattern','unknown'], // No Information
    ['ZI013_FFP','1','farm:pattern','linear'], // Linear
    ['ZI013_FFP','2','farm:pattern','regular'], // Regular
    ['ZI013_FFP','3','farm:pattern','terraced'], // Terraced
    ['ZI013_FFP','4','farm:pattern','intermingled_woods'], // Intermingled Woods
    ['ZI013_FFP','5','farm:pattern','intermingled_trees'], // Intermingled Trees
    ['ZI013_FFP','6','farm:pattern','treeless'], // Treeless
    ['ZI013_FFP','7','farm:pattern','trellised'], // Trellised
    ['ZI013_FFP','8','farm:pattern','irregular'], // Irregular
    ['ZI013_FFP','999','farm:pattern','other'], // Other

    // ZI013_FFP2 - Crop Information : Farming Pattern [2]
    // ZI013_FFP3 - Crop Information : Farming Pattern [3]

    // ZI013_FMM - Crop Information : Farming Method
    ['ZI013_FMM','-999999','farm:method','unknown'], // No Information
    ['ZI013_FMM','1','farm:method','fallow'], // Fallow
    ['ZI013_FMM','2','farm:method','grazing'], // Grazing
    ['ZI013_FMM','3','farm:method','permanent'], // Permanent
    ['ZI013_FMM','4','farm:method','slash_and_burn'], // Slash and Burn
    ['ZI013_FMM','999','farm:method','other'], // Other

    // ZI013_FMM2 - Crop Information : Farming Method [2]
    // ZI013_FMM3 - Crop Information : Farming Method [3]

    // ZI013_IRG - Crop Information : Irrigation Method
    ['ZI013_IRG','-999999','farm:irrigation','unknown'], // No Information
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
    ['ZI013_PIG','-999999','permanent_irrigation','unknown'], // No Information
    ['ZI013_PIG','1000','permanent_irrigation','no'],
    ['ZI013_PIG','1001','permanent_irrigation','yes'],

    // ZI014_PBY - Manufacturing Information : By-product
    ['ZI014_PBY','-999999','by_product','unknown'], // No Information
    ['ZI014_PBY','1','by_product','ash'], // Ash
    ['ZI014_PBY','2','by_product','cinders'], // Cinders
    ['ZI014_PBY','6','by_product','no_byproduct'], // No Byproduct
    ['ZI014_PBY','7','by_product','radioactive_material'], // Radioactive Material
    ['ZI014_PBY','8','by_product','refuse'], // Refuse
    ['ZI014_PBY','12','by_product','scrap-metal'], // Scrap-metal
    ['ZI014_PBY','13','by_product','sewage'], // Sewage
    ['ZI014_PBY','15','by_product','slag'], // Slag
    ['ZI014_PBY','999','by_product','other'], // Other
    ['ZI014_PBY','16','by_product','sludge'], // Sludge from PBY
    ['ZI014_PBY','17','by_product','spoil'], // Spoil From PBY

    // ZI014_PBY2 - Manufacturing Information : By-product [2]
    // ZI014_PBY3 - Manufacturing Information : By-product [3]

    // ZI014_PPO - Manufacturing Information : Product
    ['ZI014_PPO','-999999','product','unknown'], // No Information
    ['ZI014_PPO','1','product','aircraft'], // Aircraft
    ['ZI014_PPO','2','product','aluminium'], // Aluminium
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
    ['ZI014_PPO','37','product','electric_power'], // Electric Power
    ['ZI014_PPO','35','product','dolomite'], // Dolomite -- From PPO
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
    ['ZI014_PPO','170','product','beverage'], // Beverage -- From PPO
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

    // ZI014_PPO2 - Manufacturing Information : Product [2]
    // ZI014_PPO3 - Manufacturing Information : Product [3]

    // ZI014_PRW - Manufacturing Information : Raw Material
    ['ZI014_PRW','-999999','raw_material','unknown'], // No Information
    ['ZI014_PRW','1','raw_material','aluminium'], // Aluminium
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
    ['ZI014_PRW','30','raw_material','grain'], // Grain
    ['ZI014_PRW','33','raw_material','plant_material'], // Plant Material
    ['ZI014_PRW','34','raw_material','gravel'], // Gravel From PRW
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
    ['ZI014_PRW','67','raw_material','salt'], // Salt From PRW
    ['ZI014_PRW','68','raw_material','sand'], // Sand From PRW
    ['ZI014_PRW','75','raw_material','sewage'], // Sewage
    ['ZI014_PRW','78','raw_material','silver'], // Silver
    ['ZI014_PRW','79','raw_material','snow'], // Snow
    ['ZI014_PRW','83','raw_material','steel'], // Steel
    ['ZI014_PRW','84','raw_material','stone'], // Stone From PRW
    ['ZI014_PRW','85','raw_material','sugar'], // Sugar
    ['ZI014_PRW','87','raw_material','textile'], // Textile
    ['ZI014_PRW','90','raw_material','tobacco'], // Tobacco
    ['ZI014_PRW','93','raw_material','uranium'], // Uranium
    ['ZI014_PRW','94','raw_material','vegetation'], // Vegetation
    ['ZI014_PRW','96','raw_material','water'], // Water
    ['ZI014_PRW','97','raw_material','wood'], // Wood
    ['ZI014_PRW','99','raw_material','zinc'], // Zinc
    ['ZI014_PRW','118','raw_material','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
    ['ZI014_PRW','154','raw_material','sulphur'], // Sulphur From PRW
    ['ZI014_PRW','999','raw_material','other'], // Other

    // ZI014_PRW2 - Manufacturing Information : Raw Material [2]
    // ZI014_PRW3 - Manufacturing Information : Raw Material [3]

    // ZI016_ROC - Route Pavement Information : Route Surface Composition
    ['ZI016_ROC','-999999','surface','unknown'], // No Information
    ['ZI016_ROC','1','surface','unimproved'], // Unimproved
    ['ZI016_ROC','2','surface','stabilized_earth'], // Stabilized Earth
    ['ZI016_ROC','3','surface','flexible_pavement'], // Flexible Pavement
    ['ZI016_ROC','4','surface','aggregate'], // Aggregate
    ['ZI016_ROC','5','surface','macadam'], // Macadam
    ['ZI016_ROC','6','surface','bound_surface'], // Bound Surface
    ['ZI016_ROC','7','surface','rigid_pavement'], // Rigid Pavement
    ['ZI016_ROC','8','surface','concrete'], // Concrete
    ['ZI016_ROC','9','surface','asphalt'], // Asphalt
    ['ZI016_ROC','10','surface','asphalt_over_concrete'], // Asphalt over Concrete
    ['ZI016_ROC','11','surface','cobble-stone'], // Cobble-stone
    ['ZI016_ROC','12','surface','brick'], // Brick
    ['ZI016_ROC','13','surface','metal'], // Metal
    ['ZI016_ROC','14','surface','wood'], // Wood
    ['ZI016_ROC','15','surface','corduroy'], // Corduroy
    ['ZI016_ROC','16','surface','wood_plank'], // Wood Plank
    ['ZI016_ROC','17','surface','ice'], // Ice
    ['ZI016_ROC','18','surface','snow'], // Snow
    ['ZI016_ROC','999','surface','other'], // Other

    // ZI016_WTC - Route Pavement Information : Road Weather Restriction
    ['ZI016_WTC','-999999','seasonal','unknown'], // No Information
    ['ZI016_WTC','1','seasonal','no'], // All-weather
    ['ZI016_WTC','2','seasonal','fair'], // possibly seasonal // Fair-weather
    ['ZI016_WTC','3','seasonal','winter'], // Winter Only
    ['ZI016_WTC','4','seasonal','limited'], // Limited all-weather
    ['ZI016_WTC','5','seasonal','not_winter'], // Closed in Winter
    ['ZI016_WTC','999','seasonal','other'], // Other

    // ZI017_RGC - Track Information : Railway Gauge Classification
    ['ZI017_RGC','-999999','gauge:type','unknown'], // No Information
    ['ZI017_RGC','1','gauge:type','broad'], // Broad
    ['ZI017_RGC','2','gauge:type','narrow'], // Narrow
    ['ZI017_RGC','3','gauge:type','standard'], // Standard

    // ZI017_RIR - Track Information : Railway in Road
    ['ZI017_RIR','-999999','railway:in_road','unknown'], // No Information
    ['ZI017_RIR','1000','railway:in_road','separated'],
    ['ZI017_RIR','1001','railway:in_road','yes'],

    // ZI017_RRA - Track Information : Railway Power Method
    ['ZI017_RRA','-999999','electrified','unknown'], // No Information
    ['ZI017_RRA','1','electrified','rail'], // Electrified Track
    ['ZI017_RRA','3','electrified','overhead'], // Overhead Electrified
    ['ZI017_RRA','4','electrified','no'], // Non-electrified
    ['ZI017_RRA','999','electrified','other'], // Other

    // ZI017_TRT - Track Information : Track Type
    ['ZI017_TRT','-999999','railway:track','unknown'], // No Information
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
    ['ZI018_WIT','-999999','communication:wireless:type','unknown'], // No Information
    ['ZI018_WIT','1','communication:cellular_phone','yes'], // Cellular Phone
    ['ZI018_WIT','2','communication:microwave_radio_relay','yes'], // Microwave Radio Relay
    ['ZI018_WIT','3','communication:mobile_phone','yes'], // Mobile Phone
    ['ZI018_WIT','4','communication:radio_broadcast','yes'], // Radio Broadcast
    ['ZI018_WIT','5','communication:radio_telephone','yes'], // Radio Telephone
    ['ZI018_WIT','6','communication:radio-telegraph','yes'], // Radio-telegraph
    ['ZI018_WIT','7','communication:television','yes'], // Television (TV)
    ['ZI018_WIT','999','communication:other','yes'], // Other

    // ZI019_ASP - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method
    ['ZI019_ASP','-999999','aeroway:pavement:preparation','unknown'], // No Information
    ['ZI019_ASP','1','aeroway:pavement:preparation','aggregate_seal_coat'], // Aggregate Seal Coat
    ['ZI019_ASP','2','aeroway:pavement:preparation','graded'], // Graded
    ['ZI019_ASP','3','aeroway:pavement:preparation','grass'], // Grass
    ['ZI019_ASP','4','aeroway:pavement:preparation','grooved'], // Grooved
    ['ZI019_ASP','5','aeroway:pavement:preparation','oiled'], // Oiled
    ['ZI019_ASP','6','aeroway:pavement:preparation','porous_friction_course'], // Porous Friction Course
    ['ZI019_ASP','7','aeroway:pavement:preparation','rolled'], // Rolled
    ['ZI019_ASP','8','aeroway:pavement:preparation','rubberized_seal_coat'], // Rubberized Seal Coat
    ['ZI019_ASP','999','aeroway:pavement:preparation','other'], // Other

    // ZI019_ASP2 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [2]
    // ZI019_ASP3 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [3]

    // ZI019_ASU - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition
    ['ZI019_ASU','-999999','aeroway:surface','unknown'], // No Information
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

    // ZI019_ASU2 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [2]
    // ZI019_ASU3 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [3]

    // ZI019_ASX - Aerodrome Pavement Information : Aerodrome Movement Area Surface Category
    ['ZI019_ASX','-999999','aeroway:pavement','unknown'], // No Information
    ['ZI019_ASX','1','aeroway:pavement','paved'], // Completely Paved
    ['ZI019_ASX','2','aeroway:pavement','mostly_paved'], // Mostly Paved
    ['ZI019_ASX','3','aeroway:pavement','unprepared'], // Unprepared
    ['ZI019_ASX','4','aeroway:pavement','partially_paved'], // Partially Paved
    ['ZI019_ASX','5','aeroway:pavement','unpaved'], // Unpaved
    ['ZI019_ASX','999','aeroway:pavement','other'], // Other

    // ZI019_SFS - Aerodrome Pavement Information : Aerodrome Pavement Functional Status
    ['ZI019_SFS','-999999','surface:condition','unknown'], // No Information
    ['ZI019_SFS','1','surface:condition','fair'], // Fair
    ['ZI019_SFS','2','surface:condition','fair_estimated'], // Fair Estimated
    ['ZI019_SFS','3','surface:condition','good'], // Good
    ['ZI019_SFS','4','surface:condition','good_estimated'], // Good Estimated
    ['ZI019_SFS','5','surface:condition','poor'], // Poor
    ['ZI019_SFS','6','surface:condition','poor_estimated'], // Poor Estimated
    ['ZI019_SFS','7','surface:condition','under_construction'], // Under Construction
    ['ZI019_SFS','8','surface:condition','unserviceable'], // Unserviceable
    ['ZI019_SFS','9','surface:condition','excellent'], // Excellent

    // We fix these in post processing
    // ZI020_GE4 - (Location Country) Designation : GENC Short URN-based Identifier
    // ZI020_GE42 - (Location Country) Designation : GENC Short URN-based Identifier (second)
    // ZI020_GE43 - (Location Country) Designation : GENC Short URN-based Identifier (third)
    // ZI020_GE44 - (Location Country) Designation : GENC Short URN-based Identifier (fourth)

    // ZI024_HYP - Water Resource Information : Hydrologic Persistence
    ['ZI024_HYP','-999999','intermittent','unknown'], // No Information
    ['ZI024_HYP','1','intermittent','no'], // Perennial
    ['ZI024_HYP','2','intermittent','yes'],
    ['ZI024_HYP','4','intermittent','dry'], // Dry

    // ZI024_SCC - Water Resource Information : Water Type
    ['ZI024_SCC','-999999','water:type','unknown'], // No Information
    ['ZI024_SCC','1','water:type','alkaline'], // Alkaline
    ['ZI024_SCC','4','water:type','mineral'], // Mineral
    ['ZI024_SCC','10','water:type','salt'], // Saline
    ['ZI024_SCC','11','water:type','fresh'], // Fresh
    ['ZI024_SCC','12','water:type','brackish'], // Brackish
    ['ZI024_SCC','13','water:type','seawater'], // Seawater
    ['ZI024_SCC','14','water:type','brine'], // Brine
    // ['ZI024_SCC','998',undefined,undefined], // Not Applicable
    ['ZI024_SCC','998','water:type','not_applicable'], // Not Applicable

    // ZI024_YWQ - Water Resource Information : Water Potability
    ['ZI024_YWQ','-999999','water:potable','unknown'], // No Information
    ['ZI024_YWQ','1','water:potable','yes'], // Potable
    ['ZI024_YWQ','2','water:potable','treatable'], // Treatable
    ['ZI024_YWQ','3','water:potable','contaminated'], // Contaminated
    ['ZI024_YWQ','4','water:potable','no'], // Nonpotable
    ['ZI024_YWQ','999','water:potable','other'], // Other

    // ZI025_WLE - Hydrographic Vertical Positioning Information : Water Level Effect - See WLE

    // ZI026_CTUC - Feature Metadata : Cartographic Usability Range <interval closure>
    ['ZI026_CTUC','2','cartographic_scale:closure','open_interval'], // Open Interval
    ['ZI026_CTUC','3','cartographic_scale:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['ZI026_CTUC','4','cartographic_scale:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['ZI026_CTUC','5','cartographic_scale:closure','closed_interval'], // Closed Interval
    ['ZI026_CTUC','6','cartographic_scale:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['ZI026_CTUC','7','cartographic_scale:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['ZI026_CTUC','8','cartographic_scale:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['ZI026_CTUC','9','cartographic_scale:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // ZI026_SUR - Feature Metadata : Survey Coverage Category
    ['ZI026_SUR','-999999','surface:surveyed','unknown'], // No Information
    ['ZI026_SUR','1','source:surveyed','yes'], // Surveyed
    ['ZI026_SUR','2','source:surveyed','inadequately_surveyed'], // Inadequately Surveyed
    ['ZI026_SUR','3','source:surveyed','no'], // Unsurveyed

    // ZI032_GUG - Pylon Information : Guyed - See GUG

    // ZI032_PYC - Pylon Information : Pylon Configuration - See PYC

    // ZI032_PYM - Pylon Information : Pylon Material - See PYM

    // ZI032_TOS - Pylon Information : Tower Shape - See TOS

    // ZI037_REL - Religious Information : Religious Designation
    ['ZI037_REL','-999999','religion:type','unknown'], // No Information
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
    ['ZI037_RFA','25','building','islamic_prayer_hall'], // Islamic Prayer Hall
    ['ZI037_RFA','999','building:religious','other'], // Other

    // ZI071_FFN - Cave Information : Feature Function See FFN
    // ['ZI071_FFN','-999999','raw:ZI071_FFN','unknown'], // noInformation
    // ['ZI071_FFN','530','raw:ZI071_FFN','warehousing_and_storage'], // Warehousing and Storage
    // ['ZI071_FFN','563','raw:ZI071_FFN','residence'], // Residence
    // ['ZI071_FFN','835','raw:ZI071_FFN','defence_activities'], // Defence Activities
    // ['ZI071_FFN','930','raw:ZI071_FFN','religious_activities'], // Religious Activities
    // ['ZI071_FFN','999','raw:ZI071_FFN','other'], // Other

    // ZI071_FFN2 - Cave Information : Feature Function [2]
    // ZI071_FFN3 - Cave Information : Feature Function [3]

    // ZI071_UAO - Cave Information : Underground Access Orientation
    ['ZI071_UAO','-999999','direction','unknown'], // No Information
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

    // ZVH_VDT - Highest Elevation <vertical datum> - See VDT
  ], // End one2one

  // Input Translation Rules:
  // These are only used for IMPORT and get added to the standard one2one rules
  one2oneIn : [
    // To account for an "error" in the TDSv61 sample GDB
    ['ZI001_VSC','Stereoscopic Imagery','source:vertical_source:type','stereoscopic_imagery'],

    ['ZI001_SRT','NTM Imagery','source:non_spatial_source:type','ntm_imagery'],

    // Difference of opinion between the Spec and the ESRI FGDB for some of these.
    // ZI001_SRT - Source : Source Type
    // ['ZI001_SRT','-999999',undefined,undefined], // No Information
    ['ZI001_SRT','2','source:non_spatial_source:type','aafif'], // AAFIF
    ['ZI001_SRT','7','source:non_spatial_source:type','cib1'], // CIB1
    ['ZI001_SRT','9','source:non_spatial_source:type','quickbird_imagery'], // QuickBird Imagery
    ['ZI001_SRT','10','source:non_spatial_source:type','dnc'], // DNC
    ['ZI001_SRT','21','source:non_spatial_source:type','dvof'], // DVOF
    ['ZI001_SRT','22','source:non_spatial_source:type','ffd'], // FFD
    ['ZI001_SRT','25','source:non_spatial_source:type','geonames'], // GeoNames
    ['ZI001_SRT','26','source:non_spatial_source:type','gps'], // GPS
    ['ZI001_SRT','28','source:non_spatial_source:type','ikonos_imagery'], // Ikonos Imagery
    ['ZI001_SRT','29','source:non_spatial_source:type','ntm_imagery'], // NTM Imagery
    ['ZI001_SRT','30','source:non_spatial_source:type','imagery'], // Imagery
    ['ZI001_SRT','31','source:non_spatial_source:type','itd'], // ITD
    ['ZI001_SRT','32','source:non_spatial_source:type','ivd'], // IVD
    ['ZI001_SRT','43','source:non_spatial_source:type','midb'], // MIDB
    ['ZI001_SRT','60','source:non_spatial_source:type','uvmap'], // UVMap
    ['ZI001_SRT','61','source:non_spatial_source:type','vitd'], // VITD
    ['ZI001_SRT','64','source:non_spatial_source:type','vmap_2'], // VMap 2
    ['ZI001_SRT','108','source:non_spatial_source:type','navteq_data'], // NAVTEQ Data
    ['ZI001_SRT','159','source:non_spatial_source:type','commercial_data'], // Commercial Data
    ['ZI001_SRT','160','source:non_spatial_source:type','open_source_information'], // Open Source Information
    ['ZI001_SRT','165','source:non_spatial_source:type','dafif'], // DAFIF
    ['ZI001_SRT','166','source:non_spatial_source:type','delorme_digital_atlas_of_the_earth_(dae)'], // DeLorme Digital Atlas of the Earth (DAE)
    ['ZI001_SRT','167','source:non_spatial_source:type','military_map_data'], // Military Map Data
    ['ZI001_SRT','168','source:non_spatial_source:type','non-military_map'], // Non-military Map
    ['ZI001_SRT','169','source:non_spatial_source:type','operations_data'], // Operations Data
    ['ZI001_SRT','170','source:non_spatial_source:type','sac'], // SAC
    ['ZI001_SRT','171','source:non_spatial_source:type','tomtom_data'], // TomTom Data
    ['ZI001_SRT','999','source:non_spatial_source:type','other'], // Other

  ], // End one2oneIn

  // One2one translation table for converting "Other" OSM attributes to TDS
  // This is for Export only. The values are swapped before use
  one2oneOut : [
    ['FFN','2','building','farm'], // Agriculture
    ['FFN','122','man_made','winery'], // Winery
    ['FFN','190','use','oil_gas_facility'], //
    ['FFN','342','railway','engine_shed'],
    ['FFN','342','railway','workshop'],
    ['FFN','440','office','company'], // Commerce
    ['FFN','440','office','yes'], // Commerce
    ['FFN','464','shop','books'], // Specialized Store
    ['FFN','464','shop','car'], // Specialized Store
    ['FFN','464','shop','motorcycle'], // Specialized Store
    ['FFN','465','shop','general'], // Non-specialized Store
    ['FFN','480','amenity','transportation'], // Transport
    ['FFN','480','building','transportation_outbuilding'], // Transportation
    ['FFN','481','aeroway','terminal'], // Terminal
    ['FFN','481','man_made','depot'], // not a good fit for depot terminal
    ['FFN','482','amenity','bus_station'], // Station
    ['FFN','486','railway','crossing_box'],
    ['FFN','530','building','warehouse'],
    ['FFN','550','building','dwelling'], // Accomodation
    ['FFN','552','leisure','ski_resort'], // Resort Hotel
    ['FFN','558','building','dependents_housing'], // Dependents Housing
    ['FFN','563','building','house'], // Residence
    ['FFN','572','amenity','fast_food'], // Restaurant
    ['FFN','610','building','communication'],
    ['FFN','610','office','telecommunication'], // telecommunication
    ['FFN','640','shop','bureau_de_change'], // Financial Services
    ['FFN','760','amenity','office'], // Business and Personal Support Services
    ['FFN','775','tourism','information'], // Travel Agency
    ['FFN','811','amenity','government_administration_building'], //
    ['FFN','811','building','public_service_outbuilding'], // Government - Not Good
    ['FFN','815','building','palace'], // Palace
    ['FFN','827','amenity','embassy'], // Embassy
    ['FFN','831','amenity','courthouse'], // Public Order - from spec
    ['FFN','835','building','military'],
    ['FFN','835','military','barracks'], // Military
    ['FFN','835','military','military_outbuilding'], // Military
    ['FFN','843','amenity','jail'], // Imprisonment
    ['FFN','850','amenity','education'], // Education
    ['FFN','850','amenity','kindergarten'], // Education
    ['FFN','850','building','school'],
    ['FFN','850','isced:level','0'], // Education
    // ['FFN','851','isced:level','1'], // Primary Education
    // ['FFN','852','isced:level','2,3'], // Secondary Education
    ['FFN','852','isced:level','2'], // Secondary Education
    ['FFN','852','isced:level','3'], // Secondary Education
    ['FFN','857','building','college'], // Vocational Education
    ['FFN','860','amenity','clinic'], // Human Health Activities
    ['FFN','860','amenity','medical'], // Human Health Activities
    ['FFN','860','building','hospital'], // Human Health Activities
    ['FFN','890','amenity','cultural'], // Cultural, Arts and Entertainment
    ['FFN','900','tourism','yes'], // Sports, Amusement and Recreation
    ['FFN','909','amenity','casino'], // Gambling
    ['FFN','912','leisure','sports_complex'], // Sports Centre
    ['FFN','923','sport','10pin'], // Hobbies and/or Leisure Activities
    ['FFN','970','amenity','townhall'], // Meeting Place
    ['FFN','980','building','cemetery_building'], // Death care services
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

    ['FFN','572','amenity','cafe'], // Restaurant
    ['FFN','572','building','restaurant'], // Restaurant
    ['FIC','1','embankment:type','mound'], // Mound
    ['FIC','1','embankment:type','berm'],
    ['FIC','1','embankment','berm'],
    ['FIC','2','embankment:type','fill'], // Fill
    ['FIC','3','embankment','dyke'], // Dyke
    ['FIC','3','embankment:type','dyke'], // Dyke
    ['FIC','5','embankment:type','levee'], // Levee
    ['FIC','6','embankment:type','divider'], // Divider
    ['FIC','999','embankment:type','other'], // Other

    ['IWT','4','man_made','reservoir'], // Reservoir

    ['MCC','2','material','aluminum'], // Aluminum

    ['ONE','1001','oneway','-1'], // Yes, it is one way but it is reversed from the drawing order

    ['PYM','1','tower:material','aluminum'], // Aluminum

    // RTY - Roadway Type Funky Road Type attributes from TDSv40
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

    // TRE - Foliage Type
    ['TRE','1','wood','deciduous'], // Deciduous
    ['TRE','2','wood','evergreen'], // Evergreen
    ['TRE','3','wood','mixed'], // Mixed
    ['TRE','999','wood','other'], // Other

    ['VCM','2','material:vertical','aluminum'], // Aluminum

    ['WCC','3','channel:type','gorge'], // Gorge

    ['ZI013_CSP','2','crop','apple'], // Apple
    ['ZI013_CSP','4','crop','berry'], // Berry
    ['ZI013_CSP','5','crop','carob'], // Carob
    ['ZI013_CSP','6','crop','chestnut'], // Chestnut
    ['ZI013_CSP','7','crop','citrus'], // Citrus
    ['ZI013_CSP','9','crop','cacao'], // Cacao
    ['ZI013_CSP','10','crop','coffee'], // Coffee
    ['ZI013_CSP','15','crop','fruit_tree'], // Fruit Tree
    ['ZI013_CSP','17','crop','hazelnut'], // Hazelnut
    ['ZI013_CSP','18','trees','hop_plants'], // Hop
    ['ZI013_CSP','19','crop','maple'], // Maple
    ['ZI013_CSP','23','crop','olive'], // Olive
    ['ZI013_CSP','24','crop','peach'], // Peach
    ['ZI013_CSP','29','crop','rubber'], // Rubber
    ['ZI013_CSP','34','crop','tea'], // Tea
    ['ZI013_CSP','38','crop','walnut'], // Walnut
    ['ZI013_CSP','40','crop','date'], // Date
    ['ZI013_CSP','41','crop','banana'], // Banana
    ['ZI013_CSP','42','crop','oil_palm'], // Oil Palm
    ['ZI013_CSP','43','crop','coconut'], // Coconut
    ['ZI013_CSP','159','trees','ornamental'], // Ornamental Trees

    // Backward compatibility for 'oil'
    ['ZI014_PPO','83','product','oil'], // Petroleum
    ['ZI014_PPO','83','product','fuel'], // Close...
    ['ZI014_PPO','2','product','aluminum'], // Aluminum
    ['ZI014_PRW','1','raw_material','aluminum'], // Aluminum
    ['ZI004_RCG','28','attribution','defence_imagery_and_geospatial_organisation_(australia)'], // Defence Imagery and Geospatial Organisation (Australia)
    ['ZI004_RCG','36','attribution','joint_geography_bureau_(france)'], // Joint Geography Bureau (France)
    ['ZI004_RCG','39','attribution','mapping_service_(hungary)'], // Mapping Service (Hungary)
    ['ZI004_RCG','45','attribution','geospatial_intelligence_organisation_(new_zealand)'], // Geospatial Intelligence Organisation (New Zealand)

    // OTH Filler.  These are to build OTH values
    // ['RTY','999','highway','other'],

    // Fix up the building/use tags from FFN
    [undefined,undefined,'building','industrial'], // Industrial
    [undefined,undefined,'man_made','works'], // Works

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
    //     [undefined,undefined,'amenity','school'], //  converted in pre processing
    //     [undefined,undefined,'amenity','hospital'], //  converted in pre processing
    //     [undefined,undefined,'amenity','university'], //  converted in pre processing
    //     [undefined,undefined,'amenity','college'], //  converted in pre processing
    [undefined,undefined,'amenity','house_of_worship'], //  converted in pre processing
    //[undefined,undefined,'historic','yes'], // Not a good translation, push to o2s
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
    ['ZI016_ROC','11','surface=cobblestone',schemaTools.isA('surface=cobblestone',1),schemaTools.simple('surface=sett')], // Cobble-stone
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
    'APT2','APT3',
    'APU2','APU3',
    'AQO2','AQO3',
    'AQP2','AQP3',
    'ASU','ASU2','ASU3',
    'AT005_CAB','AT005_CAB2','AT005_CAB3',
    'BMC2','BMC3',
    'BSC2','BSC3',
    'CAB2','CAB3',
    'CSC2','CSC3',
    'CVT2','CVT3',
    'FCSUBTYPE','FCSubtype',
    'FFN2','FFN3',
    'FHC2','FHC3',
    'FRT2','FRT3',
    'F_CODE',
    'HST2','HST3',
    'MCC2','MCC3',
    'MEM',
    'MST2','MST3',
    'PBY','PBY2','PBY3',
    'PFD',
    'PLT2','PLT3',
    'POS2','POS3',
    'PPO','PPO2','PPO3',
    'PRW','PRW2','PRW3',
    'RCG',
    'RIN_ROI2','RIN_ROI3',
    'RIN_RTN2','RIN_RTN3',
    'RRC2','RRC3',
    'SBT2','SBT3',
    'SSR2','SSR3',
    'STL2','STL3',
    'TRS2','TRS3',
    'TSM2','TSM3',
    'TTC2','TTC3',
    'UTY2','UTY3',
    'VCA2','VCA3',
    'VCM2','VCM3',
    'VCT2','VCT3',
    'VSP2','VSP3',
    'WBD',
    'WEQ2','WEQ3',
    'YWQ',
    'ZI005_FNA2','ZI005_FNA3',
    'ZI005_NFN2','ZI005_NFN3',
    'ZI013_CSP2','ZI013_CSP3',
    'ZI013_FFP2','ZI013_FFP3',
    'ZI013_FMM2','ZI013_FMM3',
    'ZI014_PBY2','ZI014_PBY3',
    'ZI014_PPO2','ZI014_PPO3',
    'ZI014_PRW2','ZI014_PRW3',
    'ZI019_ASP2','ZI019_ASP3',
    'ZI019_ASU2','ZI019_ASU3',
    'ZI025_WLE',
    'ZI032_GUG',
    'ZI032_PYC',
    'ZI032_PYM',
    'ZI032_TOS',
    'ZI071_FFN','ZI071_FFN2','ZI071_FFN3',
    'ZSAX_RX3','ZSAX_RX4',
    ],
    // ##### End of ignoreList #####

  // multiList - List of things that have multiple versions. E.g. FFN, FFN2, FFN3
  multiList : [
    'APT','APU','ASU','AT005_CAB','BMC','BSC','CVT','FFN','FHC','FRT','HST','MCC','PBY','PLT','POS','PPO','PRW','RIN_ROI','RRC','SBT',
    'SSR','STL','TRS','TSM','TTC','VCA','VCM','VSP','ZI013_CSP','ZI013_FFP','ZI013_FMM','ZI014_PBY','ZI014_PPO','ZI014_PRW','ZI019_ASP',
    'ZI019_ASU','ZI071_FFN'
    ],
  // ##### End of multiList #####

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
    ['BH140', ['bh140','river_s','river_c']], // River
    ['BH170', ['bh170','natural_pool_p']], // Natural Pool
    ['BH230', ['bh230','water_well_p','water_well_s']], // Water Well
    ['BI010', ['bi010','cistern_p']], // Cistern
    ['DB070', ['db070','cut_c']], // Cut
    ['DB150', ['db150','mountain_pass_p']], // Mountain Pass
    ['GB050', ['gb050','aircraft_revetment_c']], // Aircraft Revetment
    ['ZD040', ['zd040','named_location_s','named_location_c','named_location_p']], // Named Location
    ['ZD045', ['zd045','annotated_location_s','annotated_location_c','annotated_location_p']], // Named Location
  ],
  // ##### End of fCodeMap #####


  // ##### Start of closureList #####
  closureList : {
    'GSGCHC':['GSGCHL','GSGCHU'],
    'GSGCLC':['GSGCLL','GSGCLU'],
    'RMWC':['RMWL','RMWU'],
    'SGCC':['SGCL','SGCU'],
    'ZI026_CTUC':['ZI026_CTUL','ZI026_CTUU']
  },
  // ##### End of closureList #####

  // ##### Start of swapListIn #####
  // The What Were They Thinking? swap list.  Each of these is the _same_ attribute
  // but renamed in different features. We swap these so that there is only one
  // set of rules needed in the One2One section.
  // These get converted back on output - if we need to.
  swapListIn : {
    'ASU':'ZI019_ASU','ASU2':'ZI019_ASU3','ASU3':'ZI019_ASU3',
    'AT005_CAB':'CAB','AT005_CAB2':'CAB2','AT005_CAB3':'CAB3',
    'desert_surface':'terrain_surface',
    'MEM':'ZI006_MEM',
    'PBY':'ZI014_PBY','PBY2':'ZI014_PBY2','PBY3':'ZI014_PBY3',
    'PFD':'PWA',
    'PPO':'ZI014_PPO','PPO2':'ZI014_PPO2','PPO3':'ZI014_PPO3',
    'PRW':'ZI014_PRW','PRW2':'ZI014_PRW2','PRW3':'ZI014_PRW3',
    'RCG':'ZI004_RCG',
    'URI':'UFI',
    'WBD':'PWA',
    'YWQ':'ZI024_YWQ',
    'ZI002_UFI':'UFI',
    'ZI025_WLE':'WLE',
    'ZI032_GUG':'GUG',
    'ZI032_PYC':'PYC',
    'ZI032_PYM':'PYM',
    'ZI032_TOS':'TOS',
    'ZI071_FFN':'FFN','ZI071_FFN2':'FFN2','ZI071_FFN3':'FFN3',
    'ZVH_VDT':'VDT'
  },
  // ##### End of swapListIn #####

  // ##### Start of swapListOut #####
  // Format is: <FCODE>:{<from>:<to>}
  swapListOut : {
    'AA010':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AA020':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AA040':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AA052':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AA054':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AB000':{'ZI014_PBY':'PBY','ZI014_PBY2':'PBY2','ZI014_PBY3':'PBY3'},
    'AC060':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AD020':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AD025':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AJ050':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AL020':{'ZI005_NFN':'ZI005_NFN1'},
    'AM010':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AM040':{'ZI014_PRW':'PRW','ZI014_PRW2':'PRW2','ZI014_PRW3':'PRW3'},
    'AM060':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AM070':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AM071':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AM080':{'ZI014_YWQ':'YWQ'},
    'AQ113':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AQ116':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AT005':{'WLE':'ZI025_WLE'},
    'AT042':{'GUG':'ZI032_GUG','PYC':'ZI032_PYC','PYM':'ZI032_PYM','TOS':'ZI032_TOS','CAB':'AT005_CAB','CAB2':'AT005_CAB2','CAB3':'AT005_CAB3'},
    'BD100':{'WLE':'ZI025_WLE'},
    'BH051':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'BH070':{'PWA':'WBD'},
    'CA030':{'VDT':'ZVH_VDT'},
    'DB029':{'FFN':'ZI071_FFN','FFN2':'ZI071_FFN2','FFN3':'ZI071_FFN3'},
    'GB045':{'ZI019_ASU':'ASU','ZI019_ASU2':'ASU2','ZI019_ASU3':'ASU3'},
    'ZI031':{'ZI006_MEM':'MEM','ZI004_RCG':'RCG','UFI':'URI'},
    },
  // ##### End of swapListOut #####

  // ##### Start of txtLength #####
  // This list is for validateing the lengths of text attributes prior to export
  txtLength : {
    'BA000_VDR':80,'BEN':15,'BRN':24,'CCN':254,'CID':20,'ETZ':24,'GB052_RIDH':14,'GB052_RIDL':14,'IKO':14,'MDE':20,'NA8':80,
    'RIN_RTN':24,'RIN_RTN2':24,'RIN_RTN3':24,'UFI':254,'URI':254,'VOI':14,'WPI':14,'ZI001_SDV':20,'ZI001_VSD':20,
    'ZI002_UFI':254,'ZI005_FNA':200,'ZI005_FNA2':200,'ZI005_NFN':18,'ZI005_NFN2':18,'ZSAX_RS0':14,'ZSAX_RX0':254,
    'ZSAX_RX3':254,'ZSAX_RX4':254,'imagery_id':254,'legacy_id':254,'earlyDate':20,'lateDate':20,'img_layer':254,
    'img_mosaic':3,'PFI':15
  },
  // ##### End of txtLength #####

  // ##### Start of intList #####
  // This list is for validateing the integer attributes prior to export
  intList : ['BC040_LVN','BNF','DEV','DZC','LC1','LC2','LC3','LC4','LTN','NOS','NPL','ZI026_CTUL','ZI026_CTUU'],
  // ##### End of intList #####

  // List of attributes that can't have '0' as a value
  noZeroList : ['BNF','DZC','LC1','LC2','LC3','LC4','LTN','NOS','NPL','VST','WD1','WD2','WT2','ZI016_WD1'],
  // ##### End of noZeroList #####

  // Attribute values that are to be dropped since they are defaults or don't add value.
  dropList : { '-999999.0':1,'-999999':1,'noinformation':1,'unknown':1},
  // ##### End of dropList #####

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
    'ge:GENC:3:1-2:CUW':'CW', // Curacao
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

  // F_Code list. Used when finding which rules to use on import
  fcList : ["FA000","FA003","AB040","AT012","GB013","GA035","GA033","GB230","GA005","AK164","AK020","AK030","BB010","ZD045","GB015","BD061","BH010","AQ151","AL012","DA005","AL142","AJ085","BI045","AG050","AC010","BH015","ZB030","AQ040","AQ045","AQ050","AQ055","EB070","AL013","AL018","AL020","AT041","AT005","AL025","AI030","AK060","EC010","AI020","AP010","AL376","AL375","AC020","AQ063","DB029","AL030","AH070","BI010","EC040","FA210","AQ060","AF020","AF030","AL175","AF040","BJ031","DB061","EA010","AQ065","DB070","DB071","BI020","ZI031","GB050","BE010","AT010","AB000","GA041","BH030","BB080","AL060","AK070","BB090","AD010","CA010","DB090","AH025","AP033","AF060","ZI039","DB100","AF050","AA010","AL010","AK090","AL070","AQ070","AQ080","FA015","BH051","BB110","AL073","AF070","BB199","BI044","FLWAR","BH070","BA023","EC015","EC060","AH055","BH075","AM075","AL080","AP040","DB110","FA091","FA002","DB115","BJ030","AK100","AK101","AM030","AM020","AK110","EB010","AJ110","BB005","GB250","BD130","EA020","GB030","GB035","AJ030","AF080","EA055","BD181","BH077","AL099","AA052","BJ099","BJ040","BJ060","AQ075","BJ065","AL270","AC060","BH082","AL011","FA110","BA030","ZC050","GB005","BH090","BA010","GB040","LDRLN","BC050","AL110","BC070","BI030","EE010","AL371","AI021","BH050","BC040","ED010","MXELE","AL130","SU001","AL065","AM040","AA020","BH100","AQ110","BJ020","AQ170","DB150","AM065","ZD040","BH170","BH020","GA038","AL014","AA054","AD041","EC020","BD115","EA040","AK080","AL155","AQ141","AK120","AK040","AL140","BH110","AQ113","AL165","EA030","ZD015","BJ080","BB009","AD030","AL170","AQ116","AT042","BH012","AK130","AT045","AN010","AN050","AN075","AN060","BH120","AB010","BD120","BH135","AA040","BH140","AQ135","AP030","AP020","RDMKR","DB160","AN076","AL200","GB055","BH160","BH155","BH150","DB170","AK161","GB070","AL105","AC030","AL208","AQ118","AA011","AL019","BI006","AG030","BB081","BB082","AQ035","AK150","BI040","AF010","BD140","BJ100","DA010","AD025","AD020","AL351","BH165","AK040","CA030","AK160","AQ150","DB010","GB045","AM010","AM070","BD100","AM060","ZB050","ED020","AK170","GA037","AM071","GB075","FA100","AL510","EB020","BA040","AL036","AL241","AP050","FA165","AQ068","AQ125","AL211","EC005","BJ110","AQ130","AQ095","AH060","AL250","UNSCW","BH145","AP041","AQ140","VOBST","BI005","GA036","EA050","ZD020","DB190","DB180","AL260","AB507","BH180","GB065","BI050","AJ055","BG010","BH065","AM080","BH040","BH230","AJ050","AD060","AJ051","BD180","AK180"],

  fcSubtypeList : {"FA000":"100405","FA003":"100407","AB040":"100010","AT012":"100202","GB013":"114580","GA035":"100497","GA033":"500002","GB230":"100456","GA005":"800078","AK164":"100074","AK020":"100053","AK030":"100054","BB010":"100224","ZD045":"100476","GB015":"100438","BD061":"100692","BH010":"100295","AQ151":"100192","AL012":"100082","DA005":"100356","AL142":"100111","AJ085":"100691","BI045":"131206","AG050":"121638","AC010":"100012","BH015":"100296","ZB030":"100465","AQ040":"100161","AQ045":"100162","AQ050":"100163","AQ055":"100164","EB070":"100390","AL013":"100083","AL018":"100087","AL020":"100089","AT041":"100206","AT005":"100199","AL025":"100091","AI030":"100041","AK060":"100057","EC010":"100393","AI020":"100040","AP010":"100150","AL376":"132642","AL375":"100128","AC020":"100013","AQ063":"130381","DB029":"154961","AL030":"100092","AH070":"100039","BI010":"100329","EC040":"100396","FA210":"100417","AQ060":"100167","AF020":"100026","AF030":"100028","AL175":"100688","AF040":"100029","BJ031":"100342","DB061":"100365","EA010":"100380","AQ065":"100170","DB070":"100366","DB071":"192101","BI020":"100330","ZI031":"121591","GB050":"100446","BE010":"100310","AT010":"100200","AB000":"100007","GA041":"800291","BH030":"100298","BB080":"100230","AL060":"100096","AK070":"100059","BB090":"100233","AD010":"100018","CA010":"100353","DB090":"100368","AH025":"132596","AP033":"179969","AF060":"100031","ZI039":"132721","DB100":"100369","AF050":"100030","AA010":"100001","AL010":"100080","AK090":"100061","AL070":"100098","AQ070":"100172","AQ080":"100174","FA015":"100410","BH051":"191951","BB110":"100236","AL073":"100099","AF070":"100032","BB199":"100243","BI044":"131207","FLWAR":"999999","BH070":"100302","BA023":"100215","EC015":"130380","EC060":"100398","AH055":"132626","BH075":"100303","AM075":"100141","AL080":"100101","AP040":"100154","DB110":"100370","FA091":"100413","FA002":"100406","DB115":"100371","BJ030":"100341","AK100":"100062","AK101":"100063","AM030":"100134","AM020":"100133","AK110":"100064","EB010":"100387","AJ110":"100052","BB005":"100222","GB250":"100457","BD130":"100306","EA020":"100381","GB030":"100441","GB035":"100442","AJ030":"100043","AF080":"100033","EA055":"100386","BD181":"100279","BH077":"100304","AL099":"100103","AA052":"100006","BJ099":"100348","BJ040":"100343","BJ060":"100344","AQ075":"100173","BJ065":"100345","AL270":"100129","AC060":"100016","BH082":"130384","AL011":"100081","FA110":"800591","BA030":"100217","ZC050":"800597","GB005":"100436","BH090":"100307","BA010":"100212","GB040":"100443","LDRLN":"999997","BC050":"100253","AL110":"100105","BC070":"100256","BI030":"100331","EE010":"100401","AL371":"180086","AI021":"133168","BH050":"114168","BC040":"800668","ED010":"100399","MXELE":"500001","AL130":"100108","SU001":"100462","AL065":"100097","AM040":"100136","AA020":"100003","BH100":"100309","AQ110":"100177","BJ020":"100340","AQ170":"100197","DB150":"100372","AM065":"100138","ZD040":"100475","BH170":"100320","BH020":"100297","GA038":"800786","AL014":"100084","AA054":"155023","AD041":"100022","EC020":"100394","BD115":"100272","EA040":"100384","AK080":"100060","AL155":"100112","AQ141":"100190","AK120":"100065","AK040":"100055","AL140":"100110","BH110":"100310","AQ113":"100179","AL165":"100113","EA030":"100382","ZD015":"100578","BJ080":"100347","BB009":"100223","AD030":"100021","AL170":"100114","AQ116":"100182","AT042":"100558","BH012":"131749","AK130":"100069","AT045":"100207","AN010":"100143","AN050":"100144","AN075":"100146","AN060":"100145","BH120":"100311","AB010":"100008","BD120":"100301","BH135":"100313","AA040":"100004","BH140":"100314","AQ135":"100188","AP030":"100152","AP020":"100151","RDMKR":"999998","DB160":"100373","AN076":"100147","AL200":"100116","GB055":"100448","BH160":"100318","BH155":"100317","BH150":"100316","DB170":"100374","AK161":"121747","GB070":"100453","AL105":"100104","AC030":"100014","AL208":"100118","AQ118":"100183","AA011":"100002","AL019":"100088","BI006":"132749","AG030":"100034","BB081":"100231","BB082":"100232","AQ035":"100159","AK150":"100072","BI040":"100334","AF010":"100025","BD140":"100277","BJ100":"100349","DA010":"100358","AD025":"100020","AD020":"100019","AL351":"100126","BH165":"100319","AK040":"100055","CA030":"100355","AK160":"154703","AQ150":"100191","DB010":"100362","GB045":"100444","AM010":"100131","AM070":"100139","BD100":"100271","AM060":"100137","ZB050":"177997","ED020":"100400","AK170":"100077","GA037":"801074","AM071":"100140","GB075":"100454","FA100":"100414","AL510":"100130","EB020":"100388","BA040":"100218","AL036":"100094","AL241":"100122","AP050":"100156","FA165":"100416","AQ068":"100171","AQ125":"100186","AL211":"130921","EC005":"100392","BJ110":"100350","AQ130":"100187","AQ095":"100176","AH060":"100038","AL250":"100123","UNSCW":"999996","BH145":"100315","AP041":"100155","AQ140":"100189","VOBST":"999995","BI005":"100328","GA036":"801161","EA050":"100385","ZD020":"100473","DB190":"100377","DB180":"100375","AL260":"100124","AB507":"177962","BH180":"100321","GB065":"100452","BI050":"100337","AJ055":"100046","BG010":"100215","BH065":"131810","AM080":"100142","BH040":"100299","BH230":"100326","AJ050":"100044","AD060":"100687","AJ051":"100045","BD180":"100278","AK180":"100078"},

}; // End of tds71.rules
