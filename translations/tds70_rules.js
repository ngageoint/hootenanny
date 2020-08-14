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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
    ['F_CODE','AD010','power','plant'], // Electric Power Station, Power Plant
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
    ['F_CODE','AQ125','amenity','bus_station'],
    ['F_CODE','AQ125','highway','bus_station'], // Transportaion Station
    ['F_CODE','AQ125','highway','bus_stop'], // Transportaion Station
    ['F_CODE','AQ125','public_transport','stop_position'],
    ['F_CODE','AQ125','railway','station'], // Railway Station
    ['F_CODE','AQ141','parking','multi-story'], // Parking Garage
    ['F_CODE','AT042','power','pole'], // OSM
    ['F_CODE','AT042','power','tower'], // OSM
    ['F_CODE','BA010','natural','land_water_boundary'], // Land/Water Boundary - old translation
    ['F_CODE','BA030','place','islet'], // Island - From OSM
    ['F_CODE','BA040','water','tidal'], // Tidal Water
    ['F_CODE','BH140','waterway','yes'], // Make unknown waterways into rivers
    ['F_CODE','BH145','natural','sinkhole'],
    ['F_CODE','BH155','man_made','salt_pond'], // Due to the default translation changing
    ['F_CODE','DB090','embankment','mound'], // Embankment
    ['F_CODE','DB090','embankment','berm'],
    ['F_CODE','DB090','man_made','dyke'],
    ['F_CODE','DB090','embankment','fill'],
    ['F_CODE','DB090','embankment','levee'],
    ['F_CODE','DB090','embankment','divider'],
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
    'BA000_VDR':'source:datum:sounding:name', // Water Line : Sounding Datum Name
    'BEN':'be_number', // Basic Encyclopedia (BE) Number
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
    'GLOBALID':'uuid:global', // Not sure where this comes from. It is in the sample data
    'URI':'source:ref', // Unique Resource Identifier
    'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
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
    'ZI005_NFN':'gndb_id', // Geographic Name Information : Name Identifier
    'ZI005_NFN2':'gndb_id:2', // Geographic Name Information : (second) Name Identifier
    'ZI006_MEM':'note', // Note : Memorandum
    // We fix ZI020_GE4X in post processing
    'ZI020_GE4':'addr:country', // (Location Country) Designation : GENC Short URN-based Identifier
    'ZI020_GE42':'addr:country:second', // (Location Country) Designation : GENC Short URN-based Identifier (second)
    'ZI020_GE43':'addr:country:third', // (Location Country) Designation : GENC Short URN-based Identifier (third)
    'ZI020_GE44':'addr:country:fourth', // (Location Country) Designation : GENC Short URN-based Identifier (fourth)
    'ZSAX_RS0':'security:classification', // Restriction Information : Security Attributes Group <resource classification>
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
    'LC1':'mlc:wheeled_oneway', // Load Class Type 1
    'LC2':'mlc:wheeled', // Load Class Type 2
    'LC3':'mlc:tracked_oneway', // Load Class Type 3
    'LC4':'mlc:tracked', // Load Class Type 4
    'LDC':'length:crest', // Dam Crest Length
    'LTN':'lanes', // Track or Lane Count
    'LZN':'length', // Length
    'MAG':'magnetic_variation', // Magnetic Variation
    'MWG':'divider:width', // Centerline Spacing
    'NOS':'bridge:span_count', // Span Count
    'NPL':'cables', // Parallel Line Count: See AT005
    'OHC':'max_clearance', // Overhead Clearance
    'PFD':'depth:predominant', // Predominant Feature Depth
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
    // 'WBD':'raw:WBD', // Waterbody Depth See PWA
    'WD3':'width:gap', // Terrain Gap Width
    'WDU':'width:interior_useable', // Usable Width
    'WID':'width', // Width
    'WOC':'width:crest', // Dam Crest Width
    'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
    'ZI016_WD2':'width:total_usable', // Route Pavement Information : Route Total Usable Width
    'ZI016_WT2':'width:second_way', // Route Pavement Information : Width of Second Travelled Way
    'ZI017_GAW':'gauge', // Track Information : Railway Gauge
    'ZI026_CTUL':'cartographic_scale:lower', // Feature Metadata : Cartographic Usability Range <lower value>
    'ZI026_CTUU':'cartographic_scale:upper', // Feature Metadata : Cartographic Usability Range <upper value>
    'ZVA':'ele', // Aerodrome Elevation
    'ZVH':'ele:max' // Highest Elevation
  }, // End numBiased


  // Common one2one rules. Used for both import and export
  one2one : [
    // ACC - Horizontal Accuracy Category
    // ['ACC','-999999',undefined,undefined], // noInformation
    ['ACC','1','source:accuracy:horizontal:category','accurate'],
    ['ACC','2','source:accuracy:horizontal:category','approximate'],
    ['ACC','3','source:accuracy:horizontal:category','doubtful'],

    // AMA - Amusement Attraction Type
    // ['AMA','-999999',undefined,undefined], // No Information
    ['AMA','1','attraction','artificial_mountain'], // Artificial Mountain
    ['AMA','2','attraction','ferris_wheel'], // Ferris Wheel
    ['AMA','3','attraction','roller-coaster'], // Roller-coaster
    ['AMA','4','attraction','spherical'], // Spherical
    ['AMA','5','attraction','water_attraction'], // Water Attraction
    ['AMA','6','attraction','vertical_ride'], // Vertical Ride
    ['AMA','999','attraction','other'], // Other

    // AMB - Special Administrative Unit
    // ['AMB','-999999',undefined,undefined], // No Information
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

    // APT2 - Airfield Use [2]
    // APT3 - Airfield Use [3]

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

    // APU2 - Apron Usage [2]
    // APU3 - Apron Usage [3]

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

    // ART - Aeronautical Route Category
    // ['ART','-999999',undefined,undefined], // No Information
    ['ART','1','navigationaid:route_type','low'], // Low
    ['ART','2','navigationaid:route_type','high'], // High
    ['ART','3','navigationaid:route_type','high_and_low'], // Both

    // ASO - Aeronautical Service Operational Status
    // ['ASO','-999999',undefined,undefined], // No Information
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
    // ['ASU','-999999',undefined,undefined], // noInformation
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
    // ['ASY','-999999',undefined,undefined], // No Information
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
    // ['ATC','-999999',undefined,undefined], // No Information
    ['ATC','3','aqueduct:type','underground'], // Underground
    ['ATC','4','aqueduct:type','underwater'], // Underwater
    ['ATC','5','aqueduct:type','surface'], // Surface
    ['ATC','6','aqueduct:type','qanat'], // Qanat

    // ATY - Airspace Type
    // ['ATY','-999999',undefined,undefined], // noInformation
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
    ['ATY','999','airspace:type','other'], // Other

    // AWP - Aeronautical Obstacle Light Present
    // ['AWP','-999999',undefined,undefined], // No Information
    ['AWP','1000','light:obstacle','no'],
    ['AWP','1001','light:obstacle','yes'],

    // AXS - Aerodrome Surface Status
    // ['AXS','-999999',undefined,undefined], // No Information
    ['AXS','1','aeroway:surface:status','closed'], // Closed
    ['AXS','2','aeroway:surface:status','open'], // Open
    ['AXS','3','aeroway:surface:status','work_in_progress'], // Work in Progress
    ['AXS','4','aeroway:surface:status','parked_or_disabled_aircraft'], // Parked or Disabled Aircraft

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

    // BAL - BGN Administrative Level
    // ['BAL','-999999',undefined,undefined], // noInformation
    ['BAL','1','administrative_subdivision','first-order'], // First-order
    ['BAL','2','administrative_subdivision','second-order'], // Second-order
    ['BAL','3','administrative_subdivision','third-order'], // Third-order
    ['BAL','4','administrative_subdivision','fourth-order'], // Fourth-order
    ['BAL','5','administrative_subdivision','undifferentiated'], // Undifferentiated
    ['BAL','999','administrative_subdivision','other'], // Other

    // BAT - Barrier Top Type
    // ['BAT','-999999',undefined,undefined], // No Information
    ['BAT','1','barrier:top','barbed_wire'], // Barbed Wire
    ['BAT','2','barrier:top','chain_link'], // Chain-link
    ['BAT','3','barrier:top','concertina_wire'], // Concertina Wire
    ['BAT','4','barrier:top','electrified_wire'], // Electrified Wire
    ['BAT','5','barrier:top','spiked'], // Spiked
    ['BAT','999','barrier:top','other'], // Other

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

    // BSC2 - Bridge Structure Type [2]
    // BSC3 - Bridge Structure Type [3]

    // BSM - Mobile Bridge Span
    // ['BSM','-999999',undefined,undefined], // No Information
    ['BSM','1000','bridge:mobile_span','no'],
    ['BSM','1001','bridge:mobile_span','yes'],

    // BST - Boundary Status
    // ['BST','-999999',undefined,undefined], // No Information
    ['BST','1','boundary:status','definite'], // Definite
    ['BST','2','boundary:status','indefinite'], // Indefinite
    ['BST','3','boundary:status','in_dispute'], // In Dispute
    ['BST','4','boundary:status','no_defined_boundary'], // No Defined Boundary

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
    ['BSU','18','building:superstructure:type','solar_panels'], // Solar Panels
    ['BSU','999','building:superstructure:type','other'], // Other

    // CAA - Controlling Authority - This does not play nicely in OSM
    // ['CAA','-999999',undefined,undefined], // No Information
    ['CAA','0','operator','unknown'],
    ['CAA','1','operator','national'],
    ['CAA','2','operator','state'],
    ['CAA','3','operator','private'], // Private
    ['CAA','4','operator','tribal'], // Tribal
    ['CAA','5','operator','military'], // Military
    ['CAA','6','operator','municipal'], // Municipal
    ['CAA','7','operator','joint_military_civilian'], // Joint Military and Civilian
    ['CAA','8','operator','international'],
    ['CAA','11','operator','province'], // From NFDD
    // ['CAA','13','operator','regional'], // From NFDD
    ['CAA','16','operator','civilian'], // Civilian
    ['CAA','17','operator','public'], // Public
    ['CAA','999','operator','other'], // Other

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

    // CAB2 - Cable Type [2]
    // CAB3 - Cable Type [3]

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

    // CCT - Cover Closure Type
    // ['CCT','-999999',undefined,undefined], // No Information
    ['CCT','1','cover:type','open'], // Open
    ['CCT','2','cover:type','partial'], // Partial
    ['CCT','3','cover:type','moveable'], // Moveable
    ['CCT','4','cover:type','complete'], // Complete

    // CDA - Covered Drain
    // ['CDA','-999999',undefined,undefined], // No Information
    ['CDA','1000','covered','no'],
    ['CDA','1001','covered','yes'],

    // CET - Grading Type
    // ['CET','-999999',undefined,undefined], // No Information
    ['CET','1','grading:type','one_side'], // One Side
    ['CET','2','grading:type','two_sides'], // Two Sides
    ['CET','3','grading:type','many_sides'], // Many Sides
    ['CET','4','grading:type','no_sides'], // No Sides
    ['CET','999','grading:type','other'], // Other

    // CHA - Navigation Light Charateristic
    // ['CHA','-999999',undefined,undefined], // noInformation
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

    // CMS - Commissioned Status
    // ['CMS','-999999',undefined,undefined], // No Information
    ['CMS','1','commissioned','commissioned_and_operational'], // Commissioned and Operational
    ['CMS','2','commissioned','commissioned_and_on_test'], // Commissioned and on Test
    ['CMS','3','commissioned','commissioned_but_out_of_service'], // Commissioned but Out of Service
    ['CMS','4','commissioned','operational_but_not_commissioned'], // Operational but Not Commissioned
    ['CMS','5','commissioned','on_test_but_not_commissioned'], // On Test but Not Commissioned
    ['CMS','6','commissioned','not_commissioned_and_out_of_service'], // Not Commissioned and Out of Service

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

    // CRM - Crane Mobility Type
    // ['CRM','-999999',undefined,undefined], // No Information
    ['CRM','1','crane:mobile','fixed'], // Fixed
    ['CRM','2','crane:mobile','travelling'], // Travelling
    ['CRM','3','crane:mobile','floating'], // Floating
    ['CRM','4','crane:mobile','yes'], // Mobile
    ['CRM','999','crane:mobile','other'], // Other

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

    // CUR - Current Type Category
    // ['CUR','-999999',undefined,undefined], // No Information
    ['CUR','4','current','river_flow'], // River Flow
    ['CUR','5','current','ocean_current'], // Ocean Current
    ['CUR','9','current','tidal_flow'], // Tidal Flow

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

    // CVT2 - Conveyor Type [2]
    // CVT3 - Conveyor Type [3]

    // CWT - Contained Within Tunnel
    // ['CWT','-999999',undefined,undefined], // No Information
    ['CWT','1000','in_tunnel','no'],
    ['CWT','1001','in_tunnel','yes'], // Need to think about this

    // DFT - Dam Face Type
    // ['DFT','-999999',undefined,undefined], // No Information
    ['DFT','1','dam:face:type','vertical'], // Vertical
    ['DFT','2','dam:face:type','slope'], // Slope

    // DFU - Dam Function
    // ['DFU','-999999',undefined,undefined], // No Information
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
    // ['DGC','-999999',undefined,undefined], // No Information
    ['DGC','1','block:type','drop_gate'], // Drop Gate
    ['DGC','2','block:type','rolling_block'], // Rolling Block
    ['DGC','999','block:type','other'], // Other

    // DMD - Dammed
    // ['DMD','-999999',undefined,undefined], // No Information
    ['DMD','1000','dammed','no'], // Just to get rid of errors
    ['DMD','1001','dammed','yes'], // yes

    // DSP - Boundary Dispute Type
    // ['DSP','-999999',undefined,undefined], // No Information
    ['DSP','1','boundary:dispute_type','in_dispute'], // In Dispute
    ['DSP','2','boundary:dispute_type','undisputed'], // Undisputed
    ['DSP','3','boundary:dispute_type','de_facto'], // De Facto
    ['DSP','4','boundary:dispute_type','de_jure'], // De Jure

    // EET - Engineered Earthwork Type
    // ['EET','-999999',undefined,undefined], // No Information
    ['EET','1','trench','battery'], // Battery
    ['EET','2','trench','parapet'], // Military Parapet
    ['EET','3','military','trench'], // Military Trench
    ['EET','4','trench','rampart'], // Rampart
    ['EET','5','trench','redoubt'], // Redoubt
    ['EET','999','trench','other'], // Other

    // ESC - Elevation Surface Category
    // ['ESC','-999999',undefined,undefined], // No Information
    ['ESC','1','elevation:surface','land'], // Land
    ['ESC','2','elevation:surface','snow_field_and/or_ice-field'], // Snow Field and/or Ice-field
    ['ESC','4','elevation:surface','vegetation'], // Vegetation
    ['ESC','5','elevation:surface','inland_water'], // Inland Water
    ['ESC','6','elevation:surface','tidal_water'], // Tidal Water

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
    ['ETS','regionalTdsEg','source:extraction_specification','rtds-eg'], // RTDS-EG
    ['ETS','specUrbanTdsEg','source:extraction_specification','s-utds-eg'], // S-UTDS-EG
    ['ETS','tercomE','source:extraction_specification','4gf-tercom-e'], // 4GF-TERCOM-E
    ['ETS','tercomL','source:extraction_specification','4ge-tercom-l'], // 4GE-TERCOM-L
    ['ETS','tercomT','source:extraction_specification','4gg-tercom-t'], // 4GG-TERCOM-T
    ['ETS','tlm100','source:extraction_specification','3ag-tlm100'], // 3AG-TLM100
    ['ETS','tlm50','source:extraction_specification','3aa-tlm50'], // 3AA-TLM50
    ['ETS','tod0','source:extraction_specification','tod0'], // TOD0
    ['ETS','tod1','source:extraction_specification','tod1'], // TOD1
    ['ETS','tod2','source:extraction_specification','tod2'], // TOD2
    ['ETS','tod3','source:extraction_specification','tod3'], // TOD3
    ['ETS','tod4','source:extraction_specification','tod4'], // TOD4
    ['ETS','tpc','source:extraction_specification','1aa-tpc'], // 1AA-TPC
    ['ETS','uvmap','source:extraction_specification','3ku-uvmap'], // 3KU-UVMap
    ['ETS','vitd','source:extraction_specification','3ka-vitd'], // 3KA-VITD
    ['ETS','vmap0','source:extraction_specification','3kl-vmap-0'], // 3KL-VMap-0
    ['ETS','vmap1','source:extraction_specification','3km-vmap-1'], // 3KM-VMap-1
    ['ETS','vmap2','source:extraction_specification','3kh-vmap-2'], // 3KH-VMap-2

    // FAC - Solid Maritime Construction
    // ['FAC','-999999',undefined,undefined], // No Information
    ['FAC','1000','solid_construction','no'],
    ['FAC','1001','solid_construction','yes'],

    // FCO - Feature Configuration
    // ['FCO','-999999',undefined,undefined], // No Information
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

    // FHC2 - Harbour Facility Function [2]
    // FHC3 - Harbour Facility Function [3]

    // FIC - Embankment Type
    // ['FIC','-999999',undefined,undefined], // No Information
    ['FIC','1','embankment','mound'], // Mound
    ['FIC','2','embankment','fill'], // Fill
    ['FIC','3','man-made','dyke'], // Dyke
    ['FIC','5','embankment','levee'], // Levee
    ['FIC','6','embankment','divider'], // Divider
    ['FIC','999','embankment','other'], // Other

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

    // FRT2 - Weapons Range Type [2]
    // FRT3 - Weapons Range Type [3]

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
    ['FZR','1','bunker_type','blockhouse'], // Blockhouse
    ['FZR','2','bunker_type','casement'], // Casement
    ['FZR','3','defensive','keep'], // Keep
    ['FZR','4','defensive','martello_tower'], // Martello Tower
    ['FZR','5','fortified:type','non-specific'], // Non-specific Fortified
    ['FZR','6','bunker_type','pillbox'], // Pillbox
    ['FZR','999','fortified:type','other'], // Other

    // GEC - Geopolitical Entity Type
    // ['GEC','-999999',undefined,undefined], // No Information
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
    // ['GTC','-999999',undefined,undefined], // No Information
    ['GTC','1','barrier','toll_booth'], // Toll-gate
    ['GTC','2','barrier','crossing'], // Crossing
    ['GTC','3','barrier','entrance'], // Entrance - Not great but the closest thing OSM has to the enterence to a facility
    ['GTC','4','barrier','border_control'], // Border Crossing
    ['GTC','999','barrier','other'], // Other

    // GUG - Guyed
    // ['GUG','-999999',undefined,undefined], // No Information
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

    // HST - Hazard Shelter Intended Use
    // ['HST','-999999',undefined,undefined], // No Information
    ['HST','1','shelter:type','bomb'], // Bomb Shelter
    ['HST','2','shelter:type','fallout'], // Fallout Shelter
    ['HST','3','shelter:type','storm'], // Storm Shelter
    ['HST','999','shelter:type','other'], // Other

    // HST2 - Hazard Shelter Intended Use [2]
    // HST3 - Hazard Shelter Intended Use [3]

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
    ['HZD','capeCanaveralMeanValue','source:datum:geodetic','cape_canaveral_(mean_value)'], // The geodetic datum known as 'Cape Canaveral' and intended as a mean solution for the geographic region of Florida and the Bahama Islands.
    ['HZD','capeSouthAfrica','source:datum:geodetic','cape_(south_africa)'], // The geodetic datum known as 'Cape' and intended to be used only in the geographic region of South Africa.
    ['HZD','carthageTunisia','source:datum:geodetic','carthage_(tunisia)'], // The geodetic datum known as 'Carthage' and intended to be used only in the geographic region of Tunisia.
    ['HZD','chatham1971NewZealand','source:datum:geodetic','chatham_1971_(chatham_island_new_zealand)'], // The geodetic datum known as 'Chatham 1971' and intended to be used only in the geographic region of Chatham Island, New Zealand.
    ['HZD','chuaAstroParaguay','source:datum:geodetic','chua_astro_(paraguay)'], // The geodetic datum known as 'Chua Astro' and intended to be used only in the geographic region of Paraguay.
    ['HZD','compensationGeoQuebec1977','source:datum:geodetic','compensation_geodetique_du_quebec_1977'], // The geodetic datum known as 'Compensation Geodetique du Quebec 1977' and whose geographic region of intended use is not specified.
    ['HZD','conakryPyramidGuinea','source:datum:geodetic','conakry_pyramid_of_the_service_geographique_(guinea)'], // The geodetic datum known as 'Conakry Pyramid of the Service Geographique' and intended to be used only in the geographic region of Guinea.
    ['HZD','corregoAlegreBrazil','source:datum:geodetic','corrego_alegre_(brazil)'], // The geodetic datum known as 'Corrego Alegre' and intended to be used only in the geographic region of Brazil.
    ['HZD','dabolaGuinea','source:datum:geodetic','dabola_(guinea)'], // The geodetic datum known as 'Dabola' and intended to be used only in the geographic region of Guinea.
    ['HZD','dcs3LighthouseLesserAnt','source:datum:geodetic','dcs-3_lighthouse_saint_lucia_lesser_antilles'], // The geodetic datum known as 'DCS-3 Lighthouse, Saint Lucia, Lesser Antilles' and whose geographic region of intended use is not specified.
    ['HZD','deceptionIslAntarctica','source:datum:geodetic','deception_island_antarctica'], // The geodetic datum known as 'Deception Island' or 'Deception 1993' and intended to be used only in the geographic region of Deception Island, Antarctica.
    ['HZD','djakartaBataviaIndonesia','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)'], // The geodetic datum known as 'Djakarta (Batavia)' and intended to be used only in the geographic region of Sumatra Island, Indonesia.
    ['HZD','djakartaBataviaZeroMerid','source:datum:geodetic','djakarta_(batavia)_(sumatra_island_indonesia)_with_zero_meridian_djakarta'], // The geodetic datum known as 'Djakarta (Batavia)' whose zero meridian is set at Djakarta and is intended to be used only in the geographic region of Sumatra Island, Indonesia.
    ['HZD','dominicaAstroM12LesserAnt','source:datum:geodetic','dominica_astro_m-12_dominica_lesser_antilles'], // The geodetic datum known as 'Dominica Astro M-12, Dominica, Lesser Antilles' and whose geographic region of intended use is not specified.
    ['HZD','dos1968GizoNewGeorgiaIs','source:datum:geodetic','dos_1968_(gizo_island_new_georgia_islands)'], // The geodetic datum known as 'DOS 1968' and intended to be used only in the geographic region of Gizo Island and the New Georgia Islands.
    ['HZD','easterIsland1967EasterIs','source:datum:geodetic','easter_island_1967_(easter_island)'], // The geodetic datum known as 'Easter Island 1967' and intended to be used only in the geographic region of Easter Island.
    ['HZD','estonia1937','source:datum:geodetic','co-ordinate_system_1937_of_estonia'], // The geodetic datum known as 'Co-ordinate System 1937 of Estonia' and intended to be used only in the geographic region of Estonia.
    ['HZD','etrs1989','source:datum:geodetic','european_terrestrial_reference_system_1989_(etrs89)'], // The geodetic datum known as 'European Terrestrial Reference System 1989' and intended to be used only in the geographic region of ETRS89.
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
    ['HZD','indian1954Thailand','source:datum:geodetic','indian_1954_(thailand)'], // The geodetic datum known as 'Indian 1954' and intended to be used only in the geographic region of Thailand.
    ['HZD','indian1960ConSonIsland','source:datum:geodetic','indian_1960_(con_son_island_(vietnam))'], // The geodetic datum known as 'Indian 1960 (Con Son Island)' and intended to be used only in the geographic region of Con Son Island (Vietnam).
    ['HZD','indian1960Vietnam','source:datum:geodetic','indian_1960_(vietnam:_near_16_degrees_north)'], // The geodetic datum known as 'Indian 1960' and intended to be used only in the geographic region of Vietnam near 16 degrees North.
    ['HZD','indian1975Thailand','source:datum:geodetic','indian_1975_(thailand)'], // The geodetic datum known as 'Indian 1975' and intended to be used only in the geographic region of Thailand.
    ['HZD','indian1975ThailandCycle1','source:datum:geodetic','indian_1975_(thailand)_-_cycle_1'], // The geodetic datum known as 'Indian 1975 - Cycle 1' and intended to be used only in the geographic region of Thailand.
    ['HZD','indianBangladesh','source:datum:geodetic','indian_(bangladesh)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of Bangladesh.
    ['HZD','indianIndiaNepal','source:datum:geodetic','indian_(india_and_nepal)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of India and Nepal.
    ['HZD','indianPakistan','source:datum:geodetic','indian_(pakistan)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of Pakistan.
    ['HZD','indianThailandVietnam','source:datum:geodetic','indian_(thailand_and_vietnam)'], // The geodetic datum known as 'Indian' and intended to be used only in the geographic region of Thailand and Vietnam.
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
    ['HZD','ordnanceSurvGB1936MeanVal','source:datum:geodetic','ordnance_survey_g.b._1936_(mean_value)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended as a mean solution for the geographic region of England, the Isle of Man, Scotland, Shetland, and Wales.
    ['HZD','ordnanceSurvGB1936ScotShet','source:datum:geodetic','ordnance_survey_g.b._1936_(scotland_and_shetland_islands)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of Scotland and the Shetland Islands.
    ['HZD','ordnanceSurvGB1936ScotWale','source:datum:geodetic','ordnance_survey_g.b._1936_(england_isle_of_man_and_wales)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of England, the Isle of Man, and Wales.
    ['HZD','ordnanceSurvGB1936Wales','source:datum:geodetic','ordnance_survey_g.b._1936_(wales)'], // The geodetic datum known as 'Ordnance Survey G.B. 1936' and intended to be used only in the geographic region of Wales.
    ['HZD','osloObservatoryOld','source:datum:geodetic','oslo_observatory_(old)_norway'], // The geodetic datum known as 'Oslo Observatory (Old)' and intended to be used only in the geographic region of Norway.
    ['HZD','other','source:datum:geodetic','other'], // Other
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
    ['HZD','sirgas','source:datum:geodetic','south_american_geocentric_reference_system_(sirgas)'], // The geodetic datum known as 'South American Geocentric Reference System' and intended to be used only in the geographic region of South America.
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
    ['HZD','southAsiaSingapore','source:datum:geodetic','south_asia_(southeast_asia_singapore)'], // The geodetic datum known as 'South Asia' and intended to be used only in the geographic region of Southeast Asia and Singapore.
    ['HZD','sovietGeodeticSystem1985','source:datum:geodetic','soviet_geodetic_system_1985'], // The geodetic datum known as 'Soviet Geodetic System 1985' and intended to be used only in the geographic region of the former Soviet Union.
    ['HZD','sovietGeodeticSystem1990','source:datum:geodetic','soviet_geodetic_system_1990'], // The geodetic datum known as 'Soviet Geodetic System 1990' and intended to be used only in the geographic region of the former Soviet Union.
    ['HZD','stockholm1938Sweden','source:datum:geodetic','stockholm_1938_(sweden)'], // The geodetic datum known as 'Stockholm 1938' and intended to be used only in the geographic region of Sweden.
    ['HZD','stPierreetMiquelon1950','source:datum:geodetic','st._pierre_et_miquelon_1950'], // The geodetic datum known as 'St. Pierre et Miquelon 1950' and whose geographic region of intended use is not specified.
    ['HZD','sydneyObservatoryNewSouth','source:datum:geodetic','sydney_observatory_new_south_wales_australia'], // The geodetic datum known as 'Sydney Observatory, New South Wales, Australia' and whose geographic region of intended use is not specified.
    ['HZD','tananariveObs1925ZerMerPar','source:datum:geodetic','tananarive_observatory_1925_with_zero_meridian_paris'], // The geodetic datum known as 'Tananarive Observatory 1925' whose zero meridian is set at Paris and is intended to be used only in the geographic region of Madagascar.
    ['HZD','tananariveObservatory1925','source:datum:geodetic','tananarive_observatory_1925'], // The geodetic datum known as 'Tananarive Observatory 1925' and intended to be used only in the geographic region of Madagascar.
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
    ['HZD','wakeEniwetok1960MarshallIs','source:datum:geodetic','wake-eniwetok_1960_(marshall_islands)'], // The geodetic datum known as 'Wake-Eniwetok 1960' and intended to be used only in the geographic region of the Marshall Islands.
    ['HZD','wakeIslandAstro1952','source:datum:geodetic','wake_island_astro_1952'], // The geodetic datum known as 'Wake Island 1952' which is based on astronomic observations and is intended to be used only in the geographic region of the Wake Atoll.
    ['HZD','worldGeodeticSystem1960','source:datum:geodetic','world_geodetic_system_1960'], // The geodetic datum known as 'World Geodetic System 1960' and intended to be used globally.
    ['HZD','worldGeodeticSystem1966','source:datum:geodetic','world_geodetic_system_1966'], // The geodetic datum known as 'World Geodetic System 1966' and intended to be used globally.
    ['HZD','worldGeodeticSystem1972','source:datum:geodetic','world_geodetic_system_1972'], // The geodetic datum known as 'World Geodetic System 1972' and intended to be used globally.
    ['HZD','worldGeodeticSystem1984','source:datum:geodetic','world_geodetic_system_1984'], // The geodetic datum known as 'World Geodetic System 1984' and intended to be used globally.
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

    // LSP - Geopolitical Line Type
    // ['LSP','-999999',undefined,undefined], // No Information
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
    // ['LUN','-999999',undefined,undefined], // No Information
    ['LUN','1000','underground','no'],
    ['LUN','1001','underground','yes'],

    // MCC - Structural Material Type
    // ['MCC','-999999',undefined,undefined], // No Information
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
    // ['MCX','-999999',undefined,undefined], // No Information
    ['MCX','1000','ferry:motorized','no'],
    ['MCX','1001','ferry:motorized','yes'],

    // MES - Median Present
    // ['MES','-999999',undefined,undefined], // No Information
    ['MES','1000','median','no'],
    ['MES','1001','median','yes'],

    // MGL - Maritime Geopolitical Limit Type
    // ['MGL','-999999',undefined,undefined], // No Information
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
    // ['MNS','-999999',undefined,undefined], // No Information
    ['MNS','1000','man_made:shoreline','no'],
    ['MNS','1001','man_made:shoreline','yes'],

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

    // PBY - By-product See ZI014_PBY
    // ['PBY','-999999',undefined,undefined], // noInformation
    // ['PBY','8','raw:PBY','refuse'], // Refuse
    // ['PBY','15','raw:PBY','slag'], // Slag
    // ['PBY','16','raw:PBY','sludge'], // Sludge
    // ['PBY','17','raw:PBY','spoil'], // Spoil
    // ['PBY','999','raw:PBY','other'], // Other

    // PBY2 - By-product [2]
    // PBY3 - By-product [3]

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

    // PLT2 - Pipeline Type [2]
    // PLT3 - Pipeline Type [3]

    // POS - Power Source
    // ['POS','-999999',undefined,undefined], // No Information
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
    // ['PRM','-999999',undefined,undefined], // No Information
    ['PRM','1000','permanent','no'],
    ['PRM','1001','permanent','yes'],

    // PRW - Raw Material See ZI014_PRW
    // ['PRW','-999999',undefined,undefined], // noInformation
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
    // ['RAC','-999999',undefined,undefined], // No Information
    ['RAC','1','radar:configuration','dome_enclosed'], // Dome Enclosed
    ['RAC','3','radar:configuration','radome'], // Radome
    ['RAC','4','radar:configuration','radome_on_tower'], // Radome on Tower
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

    // RCG - Resource Content Originator - See ZI004_RCG

    // RFD - Roofed
    // ['RFD','-999999',undefined,undefined], // No Information
    ['RFD','1000','bridge:roof','no'],
    ['RFD','1001','bridge:roof','yes'],

    // RIN_ROI - Route Identification <route designation type>
    // ['RIN_ROI','-999999',undefined,undefined], // No Information
    ['RIN_ROI','1','ref:road:class','international'], // International
    ['RIN_ROI','2','ref:road:class','national_motorway'], // National Motorway
    ['RIN_ROI','3','ref:road:class','primary'], // National
    ['RIN_ROI','4','ref:road:class','secondary'], // Secondary
    ['RIN_ROI','5','ref:road:class','local'], // Local
    ['RIN_ROI','999','ref:road:class','other'], // Other

    // RIN_ROI2 - Route Identification [2] <route designation type>
    // RIN_ROI3 - Route Identification [3] <route designation type>

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

    // RLE - Relative Level
    // ['RLE','-999999',undefined,undefined], // No Information
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
    // ['ROR','-999999',undefined,undefined], // No Information
    ['ROR','1000',undefined,undefined], // The default: No ramp
    ['ROR','1001','interchange_ramp','yes'],

    // RPC - Boundary Representation Policy
    // ['RPC','-999999',undefined,undefined], // No Information
    ['RPC','1','boundary:representation','show_no_line'], // Show No Line
    ['RPC','2','boundary:representation','definite'], // Definite
    ['RPC','3','boundary:representation','indefinite'], // Indefinite
    ['RPC','4','boundary:representation','in_dispute'], // In Dispute
    ['RPC','5','boundary:representation','administrative_as_international'], // Administrative as International
    ['RPC','999','boundary:representation','other'], // Other

    // RRC - Railway Use
    // ['RRC','-999999',undefined,undefined], // No Information
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
    // ['RSA','-999999',undefined,undefined], // No Information
    ['RSA','1','service','spur'], // Spur
    ['RSA','2','service','siding'], // Siding
    ['RSA','3','service','passing'], // Passing
    ['RSA','999','service','other'], // Other

    // RSI - Radar Significance
    // ['RSI','-999999',undefined,undefined], // No Information
    ['RSI','1','airmark:radar_significance','metal'], // Metal
    ['RSI','2','airmark:radar_significance','part_metal'], // Part Metal
    ['RSI','3','airmark:radar_significance','masonry'], // Masonry
    ['RSI','4','airmark:radar_significance','composition'], // Composition
    ['RSI','5','airmark:radar_significance','earthen'], // Earthen
    ['RSI','999','airmark:radar_significance','other'], // Other

    // RTA - Linear Feature Arrangement
    // ['RTA','-999999',undefined,undefined], // No Information
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

    // SBT2 - Substation Type [2]
    // SBT3 - Substation Type [3]

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
    ['SGCC','3','incline:interval:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['SGCC','4','incline:interval:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['SGCC','5','incline:interval:closure','closed_interval'], // Closed Interval
    ['SGCC','6','incline:interval:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['SGCC','7','incline:interval:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['SGCC','8','incline:interval:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['SGCC','9','incline:interval:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

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

    // SLT - Shoreline Type
    // ['SLT','-999999',undefined,undefined], // No Information
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

    // SSR2 - Roof Shape [2]
    // SSR3 - Roof Shape [3]

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
    ['SWT','4','spring:type','spring'], // Spring - Will remove this in post processing
    ['SWT','6','spring:type','walled-in'], // Walled-in Spring
    ['SWT','7','spring:type','resurgence'], // Resurgence
    ['SWT','999','spring:type','other'], // Other -- Not great

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

    // TSM2 - Terrain Surface Material [2]
    // TSM3 - Terrain Surface Material [3]

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

    // TTC2 - Tower Type [2]
    // TTC3 - Tower Type [3]

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

    // UMA - Underground Mine Access
    // ['UMA','-999999',undefined,undefined], // No Information
    ['UMA','1','mine:access','drift'], // Drift
    ['UMA','2','mine:access','slope'], // Slope
    ['UMA','3','mine:access','shaft'], // Shaft
    // ['UMA','998',undefined,undefined], // Not Applicable
    ['UMA','998','mine:access','not_applicable'], // Not Applicable

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

    // VCA2 - Void Collection Reason [2]
    // VCA3 - Void Collection Reason [3]

    // VCM - Vertical Construction Material
    // ['VCM','-999999',undefined,undefined], // No Information
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
    // ['VCT','-999999',undefined,undefined], // No Information
    ['VCT','1','void_collection:type','hypsography'], // Hypsography
    ['VCT','3','void_collection:type','bathymetry'], // Bathymetry
    ['VCT','4','void_collection:type','waterbody_bottom_composition'], // Waterbody Bottom Composition
    ['VCT','999','void_collection:type','other'], // Other

    // VCT2 - Void Collection Type [2]
    // VCT3 - Void Collection Type [3]

    // VDT - Vertical Datum
    // ['VDT','noInformation',undefined,undefined], // No Information
    ['VDT','other','source:datum:vertical','other'], // Other
    ['VDT','groundLevel','source:datum:vertical','ground_level'], // Ground Level
    ['VDT','meanSeaLevel','source:datum:vertical','mean_sea_level'], // Mean Sea Level (MSL)
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

    // VSP2 - Vegetation Species [2]
    // VSP3 - Vegetation Species [3]

    // WCC - Watercourse Channel Type
    // ['WCC','-999999',undefined,undefined], // No Information
    ['WCC','1','waterway','stream'], // Channelized Stream
    ['WCC','2','waterway','braided_stream'], // Braided Stream
    ['WCC','3','waterway','gorge'], // Gorge
    ['WCC','4','wadi','yes'], // Wadi
    ['WCC','7','waterway','river'], // Normal Channel
    ['WCC','999','waterway','other'], // Other

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

    // WEQ2 - Well Equipment [2]
    // WEQ3 - Well Equipment [3]

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

    // WLO - Wreck or Hulk Exposure
    // ['WLO','-999999',undefined,undefined], // No Information
    ['WLO','1','wreck_exposure','hull_showing'], // Hull Showing
    ['WLO','2','wreck_exposure','masts_showing'], // Masts Showing
    ['WLO','3','wreck_exposure','funnel_showing'], // Funnel Showing
    ['WLO','4','wreck_exposure','superstructure_showing'], // Superstructure Showing
    ['WLO','5','wreck_exposure','masts_and_funnel_showing'], // Masts and Funnel Showing
    ['WLO','999','wreck_exposure','other'], // Other

    // WMT - Watercourse Morphology
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

    // YSU - Military Service Branch
    ['YSU','1','military:service','air_force'], // Air Force
    ['YSU','2','military:service','army'], // Army
    ['YSU','3','military:service','coastguard'], // Coastguard
    ['YSU','4','military:service','marines'], // Marines
    ['YSU','5','military:service','navy'], // Navy
    ['YSU','7','military:service','joint'], // Joint
    ['YSU','8','military:service','air_national_guard'], // Air National Guard
    ['YSU','9','military:service','army_national_guard'], // Army National Guard
    // ['YSU','998',undefined,undefined], // Not Applicable
    ['YSU','998','military:service','not_applicable'], // Not Applicable
    ['YSU','999','military:service','other'], // Other

    // YWQ - Water Potability See ZI024_YWQ
    // ['YWQ','-999999',undefined,undefined], // noInformation
    // ['YWQ','1','raw:YWQ','potable'], // Potable
    // ['YWQ','2','raw:YWQ','treatable'], // Treatable
    // ['YWQ','3','raw:YWQ','contaminated'], // Contaminated
    // ['YWQ','4','raw:YWQ','nonpotable'], // Nonpotable
    // ['YWQ','999','raw:YWQ','other'], // Other

    // ZHBH_TEC - Hydrographic Base Height : Sounding Metadata : Bathymetric Measurement Technique
    // ['ZHBH_TEC','-999999',undefined,undefined], // noInformation
    ['ZHBH_TEC','6','source:datum:sounding:method','area_swept_by_wire-drag'], // Area Swept by Wire-drag
    ['ZHBH_TEC','999','source:datum:sounding:method','other'], // Other
    ['ZHBH_TEC','1001','source:datum:sounding:method','depth_known_by_other_then_wire-drag'],  // Depth Known by Other Than Wire Drag

    // ZI001_SPS - Source Information : Spatial Source
    // ['ZI001_SPS','-999999',undefined,undefined], // No Information
    ['ZI001_SPS','1000','source:spatial_source','no'],
    ['ZI001_SPS','1001','source:spatial_source','yes'],

    // ZI001_SRT - Source Information : Source Type
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

    // ZI004_RCG - Feature Metadata : Process Step Information : Resource Content Originator
    // ['ZI004_RCG','-999999',undefined,undefined], // noInformation
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
    ['ZI004_RCG','36','attribution','bureau_geographie,_hydrographie,_oceanographie_et_meteorologie_(france)'], // Bureau Geographie, Hydrographie, Oceanographie et Meteorologie (France)
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
    ['ZI004_RCG','86','attribution','direccion_general_de_registros,_catastro,_y_geografia_(honduras)'], // Direccion General de Registros, Catastro, y Geografia (Honduras)
    ['ZI004_RCG','87','attribution','instituto_geografico_militar_(ecuador)'], // Instituto Geografico Militar (Ecuador)
    ['ZI004_RCG','88','attribution','instituto_geografico_nacional_tommy_guardia_(panama)'], // Instituto Geografico Nacional "Tommy Guardia" (Panama)
    ['ZI004_RCG','89','attribution','instituto_geografico_nacional_(argentina)'], // Instituto Geografico Nacional (Argentina)
    ['ZI004_RCG','90','attribution','diretoria_de_servico_geografico_(brazil)'], // Diretoria de Servico Geografico (Brazil)
    ['ZI004_RCG','999','attribution','other'], // Other

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

    // ZI013_CSP2 - Crop Information : Crop Species [2]
    // ZI013_CSP3 - Crop Information : Crop Species [3]

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

    // ZI013_FFP2 - Crop Information : Farming Pattern [2]
    // ZI013_FFP3 - Crop Information : Farming Pattern [3]

    // ZI013_FMM - Crop Information : Farming Method
    // ['ZI013_FMM','-999999',undefined,undefined], // No Information
    ['ZI013_FMM','1','farm:method','fallow'], // Fallow
    ['ZI013_FMM','2','farm:method','grazing'], // Grazing
    ['ZI013_FMM','3','farm:method','permanent'], // Permanent
    ['ZI013_FMM','4','farm:method','slash_and_burn'], // Slash and Burn
    ['ZI013_FMM','999','farm:method','other'], // Other

    // ZI013_FMM2 - Crop Information : Farming Method [2]
    // ZI013_FMM3 - Crop Information : Farming Method [3]

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
    ['ZI014_PBY','16','by_product','sludge'], // Sludge from PBY
    ['ZI014_PBY','17','by_product','spoil'], // Spoil From PBY

    // ZI014_PBY2 - Manufacturing Information : By-product [2]
    // ZI014_PBY3 - Manufacturing Information : By-product [3]

    // ZI014_PPO - Manufacturing Information : Product
    // ['ZI014_PPO','-999999',undefined,undefined], // No Information
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
    // ['ZI014_PRW','-999999',undefined,undefined], // No Information
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
    // ['ZI016_ROC','-999999',undefined,undefined], // No Information
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

    // ZI019_ASP2 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [2]
    // ZI019_ASP3 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Preparation Method [3]

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

    // ZI019_ASU2 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [2]
    // ZI019_ASU3 - Aerodrome Pavement Information : Aerodrome Movement Area Surface Composition [3]

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

    // We fix these in post processing
    // ZI020_GE4 - (Location Country) Designation : GENC Short URN-based Identifier
    // ZI020_GE42 - (Location Country) Designation : GENC Short URN-based Identifier (second)
    // ZI020_GE43 - (Location Country) Designation : GENC Short URN-based Identifier (third)
    // ZI020_GE44 - (Location Country) Designation : GENC Short URN-based Identifier (fourth)

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
    // ['ZI024_SCC','998',undefined,undefined], // Not Applicable
    ['ZI024_SCC','998','water:type','not_applicable'], // Not Applicable

    // ZI024_YWQ - Water Resource Information : Water Potability
    // ['ZI024_YWQ','-999999',undefined,undefined], // No Information
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
    // ['ZI026_SUR','-999999',undefined,undefined], // No Information
    ['ZI026_SUR','1','source:surveyed','yes'], // Surveyed
    ['ZI026_SUR','2','source:surveyed','inadequately_surveyed'], // Inadequately Surveyed
    ['ZI026_SUR','3','source:surveyed','no'], // Unsurveyed

    // ZI032_GUG - Pylon Information : Guyed - See GUG


    // ZI032_PYC - Pylon Information : Pylon Configuration - See PYC

    // ZI032_PYM - Pylon Information : Pylon Material - See PYM

    // ZI032_TOS - Pylon Information : Tower Shape - See TOS

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

    // ZI071_FFN - Cave Information : Feature Function See FFN
    // ['ZI071_FFN','-999999',undefined,undefined], // noInformation
    // ['ZI071_FFN','530','raw:ZI071_FFN','warehousing_and_storage'], // Warehousing and Storage
    // ['ZI071_FFN','563','raw:ZI071_FFN','residence'], // Residence
    // ['ZI071_FFN','835','raw:ZI071_FFN','defence_activities'], // Defence Activities
    // ['ZI071_FFN','930','raw:ZI071_FFN','religious_activities'], // Religious Activities
    // ['ZI071_FFN','999','raw:ZI071_FFN','other'], // Other

    // ZI071_FFN2 - Cave Information : Feature Function [2]
    // ZI071_FFN3 - Cave Information : Feature Function [3]

    // ZI071_UAO - Cave Information : Underground Access Orientation
    // ['ZI071_UAO','-999999',undefined,undefined], // No Information
    ['ZI071_UAO','1','direction','horizontal'], // Horizontal
    ['ZI071_UAO','2','direction','slopes_downward'], // Slopes Downward
    ['ZI071_UAO','3','direction','vertical_down'], // Vertical Down
    ['ZI071_UAO','4','direction','slopes_upward'], // Slopes Upward
    ['ZI071_UAO','5','direction','vertical_up'], // Vertical Up

    // ZSAX_RS0 - Restriction Information : Security Attributes Group <resource classification>
    ['ZSAX_RS0','U','security:classification','U'],
    ['ZSAX_RS0','R','security:classification','R'],
    ['ZSAX_RS0','C','security:classification','C'],
    ['ZSAX_RS0','S','security:classification','S'],
    ['ZSAX_RS0','TS','security:classification','TS'],

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
    ['ZSAX_RS0','U','security:classification','UNCLASSIFIED'],
    ['ZSAX_RS0','R','security:classification','RESTRICTED'],
    ['ZSAX_RS0','C','security:classification','CONFIDENTIAL'],
    ['ZSAX_RS0','S','security:classification','SECRET'],
    ['ZSAX_RS0','TS','security:classification','TOP_SECRET'],

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

    ['FIC','1','embankment:type','mound'], // Mound
    ['FIC','1','embankment:type','berm'],
    ['FIC','1','embankment','berm'],
    ['FIC','2','embankment:type','fill'], // Fill
    ['FIC','3','embankment','dyke'], // Dyke
    ['FIC','3','embankment:type','dyke'], // Dyke
    ['FIC','5','embankment:type','levee'], // Levee
    ['FIC','6','embankment:type','divider'], // Divider
    ['FIC','999','embankment:type','other'], // Other

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

    ['MCC','2','material','aluminum'], // Aluminum
    ['PYM','1','tower:material','aluminum'], // Aluminum
    ['VCM','2','material:vertical','aluminum'], // Aluminum
    ['ZI014_PPO','2','product','aluminum'], // Aluminum
    ['ZI014_PRW','1','raw_material','aluminum'], // Aluminum

    ['ZI004_RCG','28','attribution','defence_imagery_and_geospatial_organisation_(australia)'], // Defence Imagery and Geospatial Organisation (Australia)
    ['ZI004_RCG','36','attribution','joint_geography_bureau_(france)'], // Joint Geography Bureau (France)
    ['ZI004_RCG','39','attribution','mapping_service_(hungary)'], // Mapping Service (Hungary)
    ['ZI004_RCG','45','attribution','geospatial_intelligence_organisation_(new_zealand)'], // Geospatial Intelligence Organisation (New Zealand)

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

    ['IWT','4','man_made','reservoir'], // Reservoir

    ['ONE','1001','oneway','-1'], // Yes, it is one way but it is reversed from the drawing order

    // From UFD
    ['CAA','-999999','operator','unknown'],
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
    //[undefined,undefined,'historic','yes'], // Not a good translation, push to o2s

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
    'SBT2', 'SBT3',
    'SSR2', 'SSR3',
    'STL2', 'STL3',
    'TRS2', 'TRS3',
    'TSM2', 'TSM3',
    'TTC2', 'TTC3',
    'UTY2', 'UTY3',
    'VCA2', 'VCA3',
    'VCM2', 'VCM3',
    'VCT2', 'VCT3',
    'VSP2', 'VSP3',
    'WBD',
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
    'ZI025_WLE',
    'ZI032_GUG',
    'ZI032_PYC',
    'ZI032_PYM',
    'ZI032_TOS',
    'ZI071_FFN', 'ZI071_FFN2', 'ZI071_FFN3',
    'ZSAX_RX3', 'ZSAX_RX4',
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
    'ASU':'ZI019_ASU', 'ASU2':'ZI019_ASU3', 'ASU3':'ZI019_ASU3',
    'AT005_CAB':'CAB', 'AT005_CAB2':'CAB2', 'AT005_CAB3':'CAB3',
    'MEM':'ZI006_MEM',
    'RCG':'ZI004_RCG',
    'ZI002_UFI':'UFI',
    'ZI025_WLE':'WLE',
    'ZI032_GUG':'GUG',
    'PFD':'PWA',
    'PBY':'ZI014_PBY', 'PBY2':'ZI014_PBY2', 'PBY3':'ZI014_PBY3',
    'PPO':'ZI014_PPO', 'PPO2':'ZI014_PPO2', 'PPO3':'ZI014_PPO3',
    'PRW':'ZI014_PRW', 'PRW2':'ZI014_PRW2', 'PRW3':'ZI014_PRW3',
    'WBD':'PWA',
    'YWQ':'ZI024_YWQ',
    'ZI032_TOS':'TOS',
    'ZI032_PYC':'PYC',
    'ZI032_PYM':'PYM',
    'ZI071_FFN':'FFN', 'ZI071_FFN2':'FFN2', 'ZI071_FFN3':'FFN3',
  },
  // ##### End of swapListIn #####

  // ##### Start of swapListOut #####
  // Format is: <FCODE>:{<from>:<to>}
  swapListOut : {
    'AT042':{'GUG':'ZI032_GUG', 'PYC':'ZI032_PYC', 'PYM':'ZI032_PYM', 'TOS':'ZI032_TOS', 'CAB':'AT005_CAB','CAB2':'AT005_CAB2','CAB3':'AT005_CAB3'},
    'GB045':{'ZI019_ASU':'ASU', 'ZI019_ASU2':'ASU2', 'ZI019_ASU3':'ASU3'},
    'ZI031':{'ZI006_MEM':'MEM', 'ZI004_RCG':'RCG', 'UFI':'ZI002_UFI'},
    'AT005':{'WLE':'ZI025_WLE'},
    'BD100':{'WLE':'ZI025_WLE'},
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
    'AQ113':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'AQ116':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'BH051':{'ZI014_PPO':'PPO', 'ZI014_PPO2':'PPO2', 'ZI014_PPO3':'PPO3'},
    'BH070':{'PWA':'WBD'},
    'DB029':{'FFN':'ZI071_FFN', 'FFN2':'ZI071_FFN2', 'FFN3':'ZI071_FFN3'},
    },

  // ##### End of swapListOut #####

  // ##### Start of txtLength #####
  // This list is for validateing the lengths of text attributes prior to export
  txtLength : {
    'BA000_VDR':80,'BEN':15,'BRN':24,'CID':20,'ETZ':24,'GB052_RIDH':14,'GB052_RIDL':14,'IKO':14,'MDE':20,'NA8':80,
    'RIN_RTN':24,'RIN_RTN2':24,'RIN_RTN3':24,'UFI':254,'URI':254,'VOI':14,'WPI':14,'ZI001_SDV':20,'ZI001_VSD':20,
    'ZI002_UFI':254,'ZI005_FNA':200,'ZI005_FNA2':200,'ZI005_NFN':18,'ZI005_NFN2':18,'ZSAX_RS0':14,'ZSAX_RX0':254,
    'ZSAX_RX3':254, 'ZSAX_RX4':254
  },
  // ##### End of txtLength #####

  // ##### Start of intList #####
  // This list is for validateing the integer attributes prior to export
  intList : ['BNF','DEV','DZC','LC1','LC2','LC3','LC4','LTN','NOS','NPL','ZI026_CTUL','ZI026_CTUU'],
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

  // ##### Start of Thematic Group Rules #####
  thematicGroupList : {
    'AAA010':'IndustrySrf', // EXTRACTION_MINE_S
    'AAA020':'IndustrySrf', // MINE_SHAFT_SUPERSTRUCTURE_S
    'AAA040':'IndustrySrf', // RIG_S
    'AAA052':'IndustrySrf', // HYDROCARBONS_FIELD_S
    'AAB000':'IndustrySrf', // DISPOSAL_SITE_S
    'AAB010':'IndustrySrf', // RECYCLING_SITE_S
    'AAB040':'IndustrySrf', // AERATION_BASIN_S
    'AAC010':'IndustrySrf', // BLAST_FURNACE_S
    'AAC020':'IndustrySrf', // CATALYTIC_CRACKER_S
    'AAC030':'IndustrySrf', // SETTLING_POND_S
    'AAC060':'IndustrySrf', // INDUSTRIAL_FURNACE_S
    'AAD010':'UtilityInfrastructureSrf', // ELECTRIC_POWER_STATION_S
    'AAD020':'UtilityInfrastructureSrf', // SOLAR_PANEL_S
    'AAD025':'UtilityInfrastructureSrf', // SOLAR_FARM_S
    'AAD030':'UtilityInfrastructureSrf', // POWER_SUBSTATION_S
    'AAD060':'UtilityInfrastructureSrf', // WIND_FARM_S
    'AAF030':'UtilityInfrastructureSrf', // COOLING_TOWER_S
    'AAG030':'FacilitySrf', // SHOPPING_COMPLEX_S
    'AAH025':'MilitarySrf', // ENGINEERED_EARTHWORK_S
    'AAH055':'MilitarySrf', // FORTIFIED_BUILDING_S
    'AAH060':'MilitarySrf', // UNDERGROUND_BUNKER_S
    'AAI020':'SettlementSrf', // CARAVAN_PARK_S
    'AAI021':'SettlementSrf', // MANUFACTURED_HOME_PARK_S
    'AAJ030':'AgricultureSrf', // HOLDING_PEN_S
    'AAJ050':'AgricultureSrf', // WINDMILL_S
    'AAJ055':'IndustrySrf', // WATER_MILL_S
    'AAJ085':'AgricultureSrf', // BARN_S
    'AAJ110':'AgricultureSrf', // GREENHOUSE_S
    'AAK020':'RecreationSrf', // AMUSEMENT_PARK_ATTRACTION_S
    'AAK030':'RecreationSrf', // AMUSEMENT_PARK_S
    'AAK040':'RecreationSrf', // SPORTS_GROUND_S
    'AAK060':'RecreationSrf', // CAMP_SITE_S
    'AAK070':'RecreationSrf', // DRIVE_IN_THEATRE_S
    'AAK090':'RecreationSrf', // FAIRGROUND_S
    'AAK100':'RecreationSrf', // GOLF_COURSE_S
    'AAK101':'RecreationSrf', // GOLF_DRIVING_RANGE_S
    'AAK110':'RecreationSrf', // GRANDSTAND_S
    'AAK120':'CultureSrf', // PARK_S
    'AAK130':'RecreationSrf', // RACETRACK_S
    'AAK160':'RecreationSrf', // STADIUM_S
    'AAK164':'RecreationSrf', // AMPHITHEATRE_S
    'AAK170':'RecreationSrf', // SWIMMING_POOL_S
    'AAK180':'RecreationSrf', // ZOO_S
    'AAL010':'FacilitySrf', // FACILITY_S
    'AAL011':'FacilitySrf', // INSTALLATION_S
    'AAL012':'CultureSrf', // ARCHEOLOGICAL_SITE_S
    'AAL013':'StructureSrf', // BUILDING_S
    'AAL014':'StructureSrf', // NON_BUILDING_STRUCTURE_S
    'AAL018':'StructureSrf', // BUILDING_SUPERSTRUCTURE_S
    'AAL019':'StructureSrf', // SHED_S
    'AAL020':'SettlementSrf', // BUILT_UP_AREA_S
    'AAL030':'CultureSrf', // CEMETERY_S
    'AAL036':'CultureSrf', // TOMB_S
    'AAL060':'MilitarySrf', // DRAGONS_TEETH_S
    'AAL065':'MilitarySrf', // MINEFIELD_S
    'AAL099':'StructureSrf', // HUT_S
    'AAL105':'SettlementSrf', // SETTLEMENT_S
    'AAL130':'CultureSrf', // MEMORIAL_MONUMENT_S
    'AAL140':'StructureSrf', // PARTICLE_ACCELERATOR_S
    'AAL142':'StructureSrf', // ASTRONOMICAL_OBSERVATORY_S
    'AAL170':'RecreationSrf', // PUBLIC_SQUARE_S
    'AAL175':'CultureSrf', // COURTYARD_S
    'AAL200':'CultureSrf', // RUINS_S
    'AAL208':'SettlementSrf', // SHANTY_TOWN_S
    'AAL211':'TransportationGroundSrf', // TRANS_ROUTE_PROTECT_STRUCT_S
    'AAL241':'StructureSrf', // TOWER_S
    'AAL270':'AgricultureSrf', // INDUSTRIAL_FARM_S
    'AAL351':'AeronauticSrf', // SPACE_FACILITY_S
    'AAL375':'MilitarySrf', // CASTLE_S
    'AAL376':'MilitarySrf', // CASTLE_COMPLEX_S
    'AAM010':'StorageSrf', // STORAGE_DEPOT_S
    'AAM020':'AgricultureSrf', // GRAIN_STORAGE_STRUCTURE_S
    'AAM030':'StorageSrf', // GRAIN_ELEVATOR_S
    'AAM040':'IndustrySrf', // MINERAL_PILE_S
    'AAM060':'MilitarySrf', // SURFACE_BUNKER_S
    'AAM065':'StorageSrf', // MUNITION_STORAGE_FACILITY_S
    'AAM070':'StorageSrf', // STORAGE_TANK_S
    'AAM071':'StorageSrf', // TANK_FARM_S
    'AAM075':'StorageSrf', // FUEL_STORAGE_FACILITY_S
    'AAM080':'StorageSrf', // WATER_TOWER_S
    'AAN060':'TransportationGroundSrf', // RAILWAY_YARD_S
    'AAN075':'TransportationGroundSrf', // RAILWAY_TURNTABLE_S
    'AAN076':'TransportationGroundSrf', // ROUNDHOUSE_S
    'AAQ040':'TransportationGroundSrf', // BRIDGE_S
    'AAQ045':'TransportationGroundSrf', // BRIDGE_SPAN_S
    'AAQ060':'AeronauticSrf', // CONTROL_TOWER_S
    'AAQ063':'TransportationGroundSrf', // CAUSEWAY_STRUCTURE_S
    'AAQ068':'TransportationGroundSrf', // TRANSPORTATION_BLOCK_S
    'AAQ080':'TransportationWaterSrf', // FERRY_STATION_S
    'AAQ116':'UtilityInfrastructureSrf', // PUMPING_STATION_S
    'AAQ125':'TransportationGroundSrf', // TRANSPORTATION_STATION_S
    'AAQ130':'TransportationGroundSrf', // TUNNEL_S
    'AAQ135':'TransportationGroundSrf', // ROADSIDE_REST_AREA_S
    'AAQ140':'TransportationGroundSrf', // VEHICLE_LOT_S
    'AAQ141':'TransportationGroundSrf', // PARKING_GARAGE_S
    'AAQ150':'StructureSrf', // STAIR_S
    'AAQ151':'TransportationGroundSrf', // ARCADE_S
    'AAQ170':'TransportationGroundSrf', // MOTOR_VEHICLE_STATION_S
    'AAT012':'UtilityInfrastructureSrf', // AERIAL_FARM_S
    'AAT045':'FacilitySrf', // RADAR_STATION_S
    'ABA023':'ForeshoreSrf', // FORESHORE_S
    'ABA030':'PhysiographySrf', // ISLAND_S
    'ABA040':'HydrographySrf', // TIDAL_WATER_S
    'ABB005':'PortHarbourSrf', // HARBOUR_S
    'ABB009':'PortHarbourSrf', // PORT_S
    'ABB010':'AnchorageSrf', // ANCHORAGE_S
    'ABB081':'PortHarbourSrf', // SHORELINE_CONSTRUCTION_S
    'ABB082':'PortHarbourSrf', // SHORELINE_RAMP_S
    'ABB090':'PortHarbourSrf', // DRY_DOCK_S
    'ABB199':'PortHarbourSrf', // FLOATING_DRY_DOCK_S
    'ABC050':'HydroAidNavigationSrf', // LIGHTHOUSE_S
    'ABD061':'AquaticVegetationSrf', // AQUATIC_VEGETATION_S
    'ABD100':'PortHarbourSrf', // STRUCTURAL_PILE_S
    'ABD115':'HydrographySrf', // OFFSHORE_CONSTRUCTION_S
    'ABD120':'ReefSrf', // REEF_S
    'ABH010':'HydrographySrf', // AQUEDUCT_S
    'ABH015':'VegetationSrf', // BOG_S
    'ABH020':'TransportationWaterSrf', // NAVIGABLE_CANAL_S
    'ABH030':'HydrographySrf', // DITCH_S
    'ABH040':'IndustrySrf', // WATER_TREATMENT_BED_S
    'ABH050':'MaricultureSiteSrf', // MARICULTURE_SITE_S
    'ABH051':'AgricultureSrf', // FISH_FARM_FACILITY_S
    'ABH075':'CultureSrf', // FOUNTAIN_S
    'ABH077':'VegetationSrf', // HUMMOCK_S
    'ABH082':'HydrographySrf', // INLAND_WATERBODY_S
    'ABH090':'HydrographySrf', // LAND_SUBJECT_TO_INUNDATION_S
    'ABH120':'HydrographySrf', // RAPIDS_S
    'ABH135':'AgricultureSrf', // RICE_FIELD_S
    'ABH140':'HydrographySrf', // RIVER_S
    'ABH150':'PhysiographySrf', // SALT_FLAT_S
    'ABH155':'IndustrySrf', // SALT_EVAPORATOR_S
    'ABH160':'PhysiographySrf', // SABKHA_S
    'ABH165':'HydrographySrf', // SPILLWAY_S
    'ABH230':'HydrographySrf', // WATER_WELL_S
    'ABI005':'PortHarbourSrf', // VESSEL_LIFT_S
    'ABI006':'TransportationWaterSrf', // SHIP_ELEVATOR_S
    'ABI020':'HydrographySrf', // DAM_S
    'ABI030':'TransportationWaterSrf', // LOCK_S
    'ABI044':'HydrographySrf', // FLOOD_CONTROL_STRUCTURE_S
    'ABI050':'HydrographySrf', // WATER_INTAKE_TOWER_S
    'ABJ020':'PhysiographySrf', // MORAINE_S
    'ABJ030':'PhysiographySrf', // GLACIER_S
    'ABJ031':'PhysiographySrf', // CREVASSE_S
    'ABJ065':'IceShelfSrf', // ICE_SHELF_S
    'ABJ080':'PolarIceSrf', // POLAR_ICE_S
    'ABJ099':'IceCapSrf', // ICE_CAP_S
    'ABJ100':'PhysiographySrf', // SNOW_ICE_FIELD_S
    'ABJ110':'VegetationSrf', // TUNDRA_S
    'ADA005':'PhysiographySrf', // ASPHALT_LAKE_S
    'ADA010':'PhysiographySrf', // SOIL_SURFACE_REGION_S
    'ADB061':'PhysiographySrf', // CREVICE_S
    'ADB090':'PhysiographySrf', // EMBANKMENT_S
    'ADB115':'PhysiographySrf', // GEOTHERMAL_OUTLET_S
    'ADB160':'PhysiographySrf', // ROCK_FORMATION_S
    'ADB170':'PhysiographySrf', // SAND_DUNES_S
    'ADB180':'PhysiographySrf', // VOLCANO_S
    'AEA010':'AgricultureSrf', // CROP_LAND_S
    'AEA030':'AgricultureSrf', // PLANT_NURSERY_S
    'AEA040':'AgricultureSrf', // ORCHARD_S
    'AEA050':'AgricultureSrf', // VINEYARD_S
    'AEA055':'AgricultureSrf', // HOP_FIELD_S
    'AEB010':'VegetationSrf', // GRASSLAND_S
    'AEB020':'VegetationSrf', // THICKET_S
    'AEB070':'VegetationSrf', // BRUSH_S
    'AEC010':'AgricultureSrf', // CANE_S
    'AEC015':'VegetationSrf', // FOREST_S
    'AEC020':'PhysiographySrf', // OASIS_S
    'AEC040':'VegetationSrf', // CLEARED_WAY_S
    'AEC060':'VegetationSrf', // FOREST_CLEARING_S
    'AED010':'VegetationSrf', // MARSH_S
    'AED020':'VegetationSrf', // SWAMP_S
    'AEE010':'VegetationSrf', // LOGGING_SITE_S
    'AFA002':'GeopoliticalEntitySrf', // GEOPOLITICAL_ENTITY_S
    'AFA003':'AdministrativeBoundarySrf', // ADMINISTRATIVE_DIVISION_S
    'AFA015':'MilitarySrf', // FIRING_RANGE_S
    'AFA100':'MilitarySrf', // TEST_SITE_S
    'AFA165':'MilitarySrf', // TRAINING_SITE_S
    'AFA210':'CultureSrf', // CONSERVATION_AREA_S
    'AGA005':'AirspaceSrf', // AIRSPACE_S
    'AGB005':'AeronauticSrf', // LAND_AERODROME_S
    'AGB015':'AeronauticSrf', // APRON_S
    'AGB030':'AeronauticSrf', // HELIPAD_S
    'AGB035':'AeronauticSrf', // HELIPORT_S
    'AGB040':'AeronauticSrf', // LAUNCH_PAD_S
    'AGB045':'AeronauticSrf', // STOPWAY_S
    'AGB055':'AeronauticSrf', // RUNWAY_S
    'AGB065':'AeronauticSrf', // WATER_AERODROME_S
    'AGB070':'AeronauticSrf', // SEAPLANE_RUN_S
    'AGB075':'AeronauticSrf', // TAXIWAY_S
    'AGB230':'AeronauticSrf', // AIRCRAFT_HANGAR_S
    'AGB250':'AeronauticSrf', // HARDENED_AIRCRAFT_SHELTER_S
    'ASU001':'MilitarySrf', // MILITARY_INSTALLATION_S
    'AZD020':'InformationSrf', // VOID_COLLECTION_AREA_S
    'AZD045':'InformationSrf', // ANNOTATED_LOCATION_S
    'AZI031':'ResourceSrf', // DATASET_S
    'AZI039':'MetadataSrf', // ENTITY_COLLECTION_METADATA_S
    'LAA011':'IndustryCrv', // SHEAR_WALL_C
    'LAF020':'IndustryCrv', // CONVEYOR_C
    'LAH025':'MilitaryCrv', // ENGINEERED_EARTHWORK_C
    'LAK020':'RecreationCrv', // AMUSEMENT_PARK_ATTRACTION_C
    'LAK130':'RecreationCrv', // RACETRACK_C
    'LAK150':'RecreationCrv', // SKI_JUMP_C
    'LAL018':'StructureCrv', // BUILDING_SUPERSTRUCTURE_C
    'LAL060':'MilitaryCrv', // DRAGONS_TEETH_C
    'LAL070':'StructureCrv', // FENCE_C
    'LAL130':'CultureCrv', // MEMORIAL_MONUMENT_C
    'LAL140':'StructureCrv', // PARTICLE_ACCELERATOR_C
    'LAL155':'TransportationGroundCrv', // OVERHEAD_OBSTRUCTION_C
    'LAL211':'TransportationGroundCrv', // TRANS_ROUTE_PROTECT_STRUCT_C
    'LAL260':'StructureCrv', // WALL_C
    'LAN010':'TransportationGroundCrv', // RAILWAY_C
    'LAN050':'TransportationGroundCrv', // RAILWAY_SIDETRACK_C
    'LAP010':'TransportationGroundCrv', // CART_TRACK_C
    'LAP030':'TransportationGroundCrv', // ROAD_C
    'LAP040':'TransportationGroundCrv', // GATE_C
    'LAP041':'TransportationGroundCrv', // VEHICLE_BARRIER_C
    'LAP050':'TransportationGroundCrv', // TRAIL_C
    'LAQ035':'TransportationGroundCrv', // SIDEWALK_C
    'LAQ040':'TransportationGroundCrv', // BRIDGE_C
    'LAQ045':'TransportationGroundCrv', // BRIDGE_SPAN_C
    'LAQ063':'TransportationGroundCrv', // CAUSEWAY_STRUCTURE_C
    'LAQ065':'TransportationGroundCrv', // CULVERT_C
    'LAQ070':'TransportationWaterCrv', // FERRY_CROSSING_C
    'LAQ075':'TransportationGroundCrv', // ICE_ROUTE_C
    'LAQ113':'UtilityInfrastructureCrv', // PIPELINE_C
    'LAQ130':'TransportationGroundCrv', // TUNNEL_C
    'LAQ150':'StructureCrv', // STAIR_C
    'LAQ151':'TransportationGroundCrv', // ARCADE_C
    'LAT005':'UtilityInfrastructureCrv', // CABLE_C
    'LAT041':'TransportationGroundCrv', // CABLEWAY_C
    'LBA010':'PhysiographyCrv', // LAND_WATER_BOUNDARY_C
    'LBB081':'PortHarbourCrv', // SHORELINE_CONSTRUCTION_C
    'LBB082':'PortHarbourCrv', // SHORELINE_RAMP_C
    'LBD120':'ReefCrv', // REEF_C
    'LBE010':'DepthCurveCrv', // DEPTH_CURVE_C
    'LBH010':'HydrographyCrv', // AQUEDUCT_C
    'LBH020':'TransportationWaterCrv', // NAVIGABLE_CANAL_C
    'LBH030':'HydrographyCrv', // DITCH_C
    'LBH065':'HydrographyCrv', // WATER_RACE_C
    'LBH070':'TransportationGroundCrv', // FORD_C
    'LBH100':'HydrographyCrv', // MOAT_C
    'LBH110':'HydrographyCrv', // PENSTOCK_C
    'LBH120':'HydrographyCrv', // RAPIDS_C
    'LBH140':'HydrographyCrv', // RIVER_C
    'LBH165':'HydrographyCrv', // SPILLWAY_C
    'LBH180':'HydrographyCrv', // WATERFALL_C
    'LBI006':'TransportationWaterCrv', // SHIP_ELEVATOR_C
    'LBI020':'HydrographyCrv', // DAM_C
    'LBI030':'TransportationWaterCrv', // LOCK_C
    'LBI040':'HydrographyCrv', // SLUICE_GATE_C
    'LBI044':'HydrographyCrv', // FLOOD_CONTROL_STRUCTURE_C
    'LBI045':'TransportationWaterCrv', // BASIN_GATE_C
    'LBJ031':'PhysiographyCrv', // CREVASSE_C
    'LBJ040':'PhysiographyCrv', // ICE_CLIFF_C
    'LCA010':'HypsographyCrv', // ELEVATION_CONTOUR_C
    'LDB010':'PhysiographyCrv', // STEEP_TERRAIN_FACE_C
    'LDB061':'PhysiographyCrv', // CREVICE_C
    'LDB070':'PhysiographyCrv', // CUT_C
    'LDB071':'PhysiographyCrv', // CUT_LINE_C
    'LDB090':'PhysiographyCrv', // EMBANKMENT_C
    'LDB100':'PhysiographyCrv', // ESKER_C
    'LDB110':'PhysiographyCrv', // GEOLOGIC_FAULT_C
    'LEA020':'VegetationCrv', // HEDGEROW_C
    'LEC015':'VegetationCrv', // FOREST_C
    'LEC040':'VegetationCrv', // CLEARED_WAY_C
    'LFA000':'AdministrativeBoundaryCrv', // ADMINISTRATIVE_BOUNDARY_C
    'LFA091':'GeophysicalDataTrackLineCrv', // GEOPHYSICAL_DATA_TRACK_LINE_C
    'LFA110':'InternationalDateLineCrv', // INTERNATIONAL_DATE_LINE_C
    'LGB050':'MilitaryCrv', // DEFENSIVE_REVETMENT_C
    'LGB075':'AeronauticCrv', // TAXIWAY_C
    'LZC050':'IsogonicLineCrv', // ISOGONIC_LINE_C
    'LZD045':'InformationCrv', // ANNOTATED_LOCATION_C
    'PAA010':'IndustryPnt', // EXTRACTION_MINE_P
    'PAA020':'IndustryPnt', // MINE_SHAFT_SUPERSTRUCTURE_P
    'PAA040':'IndustryPnt', // RIG_P
    'PAA054':'IndustryPnt', // NON_WATER_WELL_P
    'PAB000':'IndustryPnt', // DISPOSAL_SITE_P
    'PAC010':'IndustryPnt', // BLAST_FURNACE_P
    'PAC020':'IndustryPnt', // CATALYTIC_CRACKER_P
    'PAC060':'IndustryPnt', // INDUSTRIAL_FURNACE_P
    'PAD010':'UtilityInfrastructurePnt', // ELECTRIC_POWER_STATION_P
    'PAD020':'UtilityInfrastructurePnt', // SOLAR_PANEL_P
    'PAD025':'UtilityInfrastructurePnt', // SOLAR_FARM_P
    'PAD030':'UtilityInfrastructurePnt', // POWER_SUBSTATION_P
    'PAD060':'UtilityInfrastructurePnt', // WIND_FARM_P
    'PAF010':'UtilityInfrastructurePnt', // SMOKESTACK_P
    'PAF020':'IndustryPnt', // CONVEYOR_P
    'PAF030':'UtilityInfrastructurePnt', // COOLING_TOWER_P
    'PAF040':'IndustryPnt', // CRANE_P
    'PAF070':'IndustryPnt', // FLARE_PIPE_P
    'PAF080':'IndustryPnt', // HOPPER_P
    'PAH055':'MilitaryPnt', // FORTIFIED_BUILDING_P
    'PAH060':'MilitaryPnt', // UNDERGROUND_BUNKER_P
    'PAH070':'TransportationGroundPnt', // CHECKPOINT_P
    'PAJ030':'AgriculturePnt', // HOLDING_PEN_P
    'PAJ050':'AgriculturePnt', // WINDMILL_P
    'PAJ051':'UtilityInfrastructurePnt', // WIND_TURBINE_P
    'PAJ055':'IndustryPnt', // WATER_MILL_P
    'PAJ085':'AgriculturePnt', // BARN_P
    'PAJ110':'AgriculturePnt', // GREENHOUSE_P
    'PAK020':'RecreationPnt', // AMUSEMENT_PARK_ATTRACTION_P
    'PAK030':'RecreationPnt', // AMUSEMENT_PARK_P
    'PAK040':'RecreationPnt', // SPORTS_GROUND_P
    'PAK060':'RecreationPnt', // CAMP_SITE_P
    'PAK080':'RecreationPnt', // OUTDOOR_THEATRE_SCREEN_P
    'PAK110':'RecreationPnt', // GRANDSTAND_P
    'PAK150':'RecreationPnt', // SKI_JUMP_P
    'PAK160':'RecreationPnt', // STADIUM_P
    'PAK161':'RecreationPnt', // SCOREBOARD_P
    'PAK164':'RecreationPnt', // AMPHITHEATRE_P
    'PAK170':'RecreationPnt', // SWIMMING_POOL_P
    'PAK180':'RecreationPnt', // ZOO_P
    'PAL010':'FacilityPnt', // FACILITY_P
    'PAL012':'CulturePnt', // ARCHEOLOGICAL_SITE_P
    'PAL013':'StructurePnt', // BUILDING_P
    'PAL014':'StructurePnt', // NON_BUILDING_STRUCTURE_P
    'PAL018':'StructurePnt', // BUILDING_SUPERSTRUCTURE_P
    'PAL019':'StructurePnt', // SHED_P
    'PAL020':'SettlementPnt', // BUILT_UP_AREA_P
    'PAL025':'CulturePnt', // CAIRN_P
    'PAL030':'CulturePnt', // CEMETERY_P
    'PAL036':'CulturePnt', // TOMB_P
    'PAL080':'StructurePnt', // GANTRY_P
    'PAL099':'StructurePnt', // HUT_P
    'PAL105':'SettlementPnt', // SETTLEMENT_P
    'PAL110':'StructurePnt', // LIGHT_SUPPORT_STRUCTURE_P
    'PAL130':'CulturePnt', // MEMORIAL_MONUMENT_P
    'PAL142':'StructurePnt', // ASTRONOMICAL_OBSERVATORY_P
    'PAL155':'TransportationGroundPnt', // OVERHEAD_OBSTRUCTION_P
    'PAL165':'TransportationGroundPnt', // PIPELINE_CROSSING_POINT_P
    'PAL200':'CulturePnt', // RUINS_P
    'PAL211':'TransportationGroundPnt', // TRANS_ROUTE_PROTECT_STRUCT_P
    'PAL241':'StructurePnt', // TOWER_P
    'PAL250':'StructurePnt', // UNDERGROUND_DWELLING_P
    'PAL270':'AgriculturePnt', // INDUSTRIAL_FARM_P
    'PAL351':'AeronauticPnt', // SPACE_FACILITY_P
    'PAL375':'MilitaryPnt', // CASTLE_P
    'PAL376':'MilitaryPnt', // CASTLE_COMPLEX_P
    'PAL510':'AeronauticPnt', // TETHERED_BALLOON_P
    'PAM010':'StoragePnt', // STORAGE_DEPOT_P
    'PAM020':'AgriculturePnt', // GRAIN_STORAGE_STRUCTURE_P
    'PAM030':'StoragePnt', // GRAIN_ELEVATOR_P
    'PAM040':'IndustryPnt', // MINERAL_PILE_P
    'PAM060':'MilitaryPnt', // SURFACE_BUNKER_P
    'PAM065':'StoragePnt', // MUNITION_STORAGE_FACILITY_P
    'PAM070':'StoragePnt', // STORAGE_TANK_P
    'PAM071':'StoragePnt', // TANK_FARM_P
    'PAM075':'StoragePnt', // FUEL_STORAGE_FACILITY_P
    'PAM080':'StoragePnt', // WATER_TOWER_P
    'PAN075':'TransportationGroundPnt', // RAILWAY_TURNTABLE_P
    'PAN076':'TransportationGroundPnt', // ROUNDHOUSE_P
    'PAP020':'TransportationGroundPnt', // ROAD_INTERCHANGE_P
    'PAP033':'TransportationGroundPnt', // ENGINEERED_TURNAROUND_SITE_P
    'PAP040':'TransportationGroundPnt', // GATE_P
    'PAP041':'TransportationGroundPnt', // VEHICLE_BARRIER_P
    'PAQ040':'TransportationGroundPnt', // BRIDGE_P
    'PAQ045':'TransportationGroundPnt', // BRIDGE_SPAN_P
    'PAQ055':'TransportationGroundPnt', // BRIDGE_TOWER_P
    'PAQ060':'AeronauticPnt', // CONTROL_TOWER_P
    'PAQ065':'TransportationGroundPnt', // CULVERT_P
    'PAQ068':'TransportationGroundPnt', // TRANSPORTATION_BLOCK_P
    'PAQ080':'TransportationWaterPnt', // FERRY_STATION_P
    'PAQ095':'TransportationGroundPnt', // TUNNEL_MOUTH_P
    'PAQ110':'AeronauticPnt', // MOORING_MAST_P
    'PAQ116':'UtilityInfrastructurePnt', // PUMPING_STATION_P
    'PAQ118':'TransportationGroundPnt', // SHARP_CURVE_P
    'PAQ125':'TransportationGroundPnt', // TRANSPORTATION_STATION_P
    'PAQ135':'TransportationGroundPnt', // ROADSIDE_REST_AREA_P
    'PAQ141':'TransportationGroundPnt', // PARKING_GARAGE_P
    'PAQ170':'TransportationGroundPnt', // MOTOR_VEHICLE_STATION_P
    'PAT010':'UtilityInfrastructurePnt', // DISH_AERIAL_P
    'PAT012':'UtilityInfrastructurePnt', // AERIAL_FARM_P
    'PAT042':'UtilityInfrastructurePnt', // PYLON_P
    'PAT045':'FacilityPnt', // RADAR_STATION_P
    'PBB009':'PortHarbourPnt', // PORT_P
    'PBB010':'AnchoragePnt', // ANCHORAGE_P
    'PBB080':'DolphinPnt', // DOLPHIN_P
    'PBC040':'MaritimeNavigationLightPnt', // MARITIME_NAVIGATION_LIGHT_P
    'PBC050':'HydroAidNavigationPnt', // LIGHTHOUSE_P
    'PBD100':'PortHarbourPnt', // STRUCTURAL_PILE_P
    'PBD115':'HydrographyPnt', // OFFSHORE_CONSTRUCTION_P
    'PBD120':'ReefPnt', // REEF_P
    'PBD130':'HazardousRockPnt', // HAZARDOUS_ROCK_P
    'PBD180':'WreckPnt', // WRECK_P
    'PBD181':'HydrographyPnt', // HULK_P
    'PBG010':'WaterMovementDataLocationPnt', // WATER_MOVEMENT_DATA_LOCATION_P
    'PBH012':'HydrographyPnt', // QANAT_SHAFT_P
    'PBH050':'MaricultureSitePnt', // MARICULTURE_SITE_P
    'PBH051':'AgriculturePnt', // FISH_FARM_FACILITY_P
    'PBH070':'TransportationGroundPnt', // FORD_P
    'PBH075':'CulturePnt', // FOUNTAIN_P
    'PBH082':'HydrographyPnt', // INLAND_WATERBODY_P
    'PBH120':'HydrographyPnt', // RAPIDS_P
    'PBH145':'HydrographyPnt', // VANISHING_POINT_P
    'PBH155':'IndustryPnt', // SALT_EVAPORATOR_P
    'PBH170':'HydrographyPnt', // NATURAL_POOL_P
    'PBH180':'HydrographyPnt', // WATERFALL_P
    'PBH230':'HydrographyPnt', // WATER_WELL_P
    'PBI006':'TransportationWaterPnt', // SHIP_ELEVATOR_P
    'PBI010':'HydrographyPnt', // CISTERN_P
    'PBI020':'HydrographyPnt', // DAM_P
    'PBI030':'TransportationWaterPnt', // LOCK_P
    'PBI040':'HydrographyPnt', // SLUICE_GATE_P
    'PBI044':'HydrographyPnt', // FLOOD_CONTROL_STRUCTURE_P
    'PBI045':'TransportationWaterPnt', // BASIN_GATE_P
    'PBI050':'HydrographyPnt', // WATER_INTAKE_TOWER_P
    'PBJ060':'PhysiographyPnt', // ICE_PEAK_P
    'PCA030':'HypsographyPnt', // SPOT_ELEVATION_P
    'PDB029':'PhysiographyPnt', // CAVE_MOUTH_P
    'PDB115':'PhysiographyPnt', // GEOTHERMAL_OUTLET_P
    'PDB150':'PhysiographyPnt', // MOUNTAIN_PASS_P
    'PDB160':'PhysiographyPnt', // ROCK_FORMATION_P
    'PDB180':'PhysiographyPnt', // VOLCANO_P
    'PEC005':'VegetationPnt', // TREE_P
    'PEC020':'PhysiographyPnt', // OASIS_P
    'PFA015':'MilitaryPnt', // FIRING_RANGE_P
    'PFA165':'MilitaryPnt', // TRAINING_SITE_P
    'PGA033':'AeroRadioNavInstallationPnt', // AERONAUTICAL_RADIO_NAVIGATION_INSTALLATION_P
    'PGA036':'VhfOmniRadioBeaconPnt', // VHF_OMNI_RADIO_BEACON_P
    'PGA037':'TacticalAirNavAidBeaconPnt', // TACTICAL_AIR_NAVIGATION_AID_BEACON_P
    'PGA038':'NonDirectionalRadioBeaconPnt', // NONDIRECTIONAL_RADIO_BEACON_P
    'PGA041':'DistanceMeasuringEquipmentPnt', // DISTANCE_MEASURING_EQUIPMENT_P
    'PGB005':'AeronauticPnt', // LAND_AERODROME_P
    'PGB013':'AerodromeBeaconPnt', // AERODROME_BEACON_P
    'PGB030':'AeronauticPnt', // HELIPAD_P
    'PGB035':'AeronauticPnt', // HELIPORT_P
    'PGB040':'AeronauticPnt', // LAUNCH_PAD_P
    'PGB065':'AeronauticPnt', // WATER_AERODROME_P
    'PGB230':'AeronauticPnt', // AIRCRAFT_HANGAR_P
    'PGB250':'AeronauticPnt', // HARDENED_AIRCRAFT_SHELTER_P
    'PSU001':'MilitaryPnt', // MILITARY_INSTALLATION_P
    'PZB030':'BoundaryPnt', // BOUNDARY_MONUMENT_P
    'PZB050':'HypsographyPnt', // SURVEY_POINT_P
    'PZD015':'PointofChangePnt', // POINT_OF_CHANGE_P
    'PZD040':'InformationPnt', // NAMED_LOCATION_P
    'PZD045':'InformationPnt', // ANNOTATED_LOCATION_P
  }, // End of thematicGroupList
  // ##### End of Thematic Group Rules #####

  // ##### Start of ESRI FCSubtype Rules #####
  subtypeList : {
    'AA010':'100001','AA011':'100002','AA020':'100003','AA040':'100004','AA052':'100006',
    'AA054':'155023','AB000':'100007','AB010':'100008','AB040':'100010','AC010':'100012',
    'AC020':'100013','AC030':'100014','AC060':'100016','AD010':'100018','AD020':'100019',
    'AD025':'100020','AD030':'100021','AD060':'100687','AF010':'100025','AF020':'100026',
    'AF030':'100028','AF040':'100029','AF070':'100032','AF080':'100033','AG030':'100034',
    'AH025':'132596','AH055':'132626','AH060':'100038','AH070':'100039','AI020':'100040',
    'AI021':'133168','AJ030':'100043','AJ050':'100044','AJ051':'100045','AJ055':'100046',
    'AJ085':'100691','AJ110':'100052','AK020':'100053','AK030':'100054','AK040':'100055',
    'AK060':'100057','AK070':'100059','AK080':'100060','AK090':'100061','AK100':'100062',
    'AK101':'100063','AK110':'100064','AK120':'100065','AK130':'100069','AK150':'100072',
    'AK160':'154703','AK161':'121747','AK164':'100074','AK170':'100077','AK180':'100078',
    'AL010':'100080','AL011':'100081','AL012':'100082','AL013':'100083','AL014':'100084',
    'AL018':'100087','AL019':'100088','AL020':'100089','AL025':'100091','AL030':'100092',
    'AL036':'100094','AL060':'100096','AL065':'100097','AL070':'100098','AL080':'100101',
    'AL099':'100103','AL105':'100104','AL110':'100105','AL130':'100108','AL140':'100110',
    'AL142':'100111','AL155':'100112','AL165':'100113','AL170':'100114','AL175':'100688',
    'AL200':'100116','AL208':'100118','AL211':'130921','AL241':'100122','AL250':'100123',
    'AL260':'100124','AL270':'100129','AL351':'100126','AL375':'100128','AL376':'132642',
    'AL510':'100130','AM010':'100131','AM020':'100133','AM030':'100134','AM040':'100136',
    'AM060':'100137','AM065':'100138','AM070':'100139','AM071':'100140','AM075':'100141',
    'AM080':'100142','AN010':'100143','AN050':'100144','AN060':'100145','AN075':'100146',
    'AN076':'100147','AP010':'100150','AP020':'100151','AP030':'100152','AP033':'179969',
    'AP040':'100154','AP041':'100155','AP050':'100156','AQ035':'100159','AQ040':'100161',
    'AQ045':'100162','AQ055':'100164','AQ060':'100167','AQ063':'130381','AQ065':'100170',
    'AQ068':'100171','AQ070':'100172','AQ075':'100173','AQ080':'100174','AQ095':'100176',
    'AQ110':'100177','AQ113':'100179','AQ116':'100182','AQ118':'100183','AQ125':'100186',
    'AQ130':'100187','AQ135':'100188','AQ140':'100189','AQ141':'100190','AQ150':'100191',
    'AQ151':'100192','AQ170':'100197','AT005':'100199','AT010':'100200','AT012':'100202',
    'AT041':'100206','AT042':'100558','AT045':'100207','BA010':'100212','BA023':'100215',
    'BA030':'100217','BA040':'100218','BB005':'100222','BB009':'100223','BB010':'100224',
    'BB080':'100230','BB081':'100231','BB082':'100232','BB090':'100233','BB199':'100243',
    'BC040':'800668','BC050':'100253','BD061':'100692','BD100':'100271','BD115':'100272',
    'BD120':'100301','BD130':'100306','BD180':'100278','BD181':'100279','BE010':'100310',
    'BG010':'100323','BH010':'100295','BH012':'131749','BH015':'100296','BH020':'100297',
    'BH030':'100298','BH040':'100299','BH050':'114168','BH051':'191951','BH065':'131810',
    'BH070':'100302','BH075':'100303','BH077':'100304','BH082':'130384','BH090':'100307',
    'BH100':'100309','BH110':'100310','BH120':'100311','BH135':'100313','BH140':'100314',
    'BH145':'100315','BH150':'100316','BH155':'100317','BH160':'100318','BH165':'100319',
    'BH170':'100320','BH180':'100321','BH230':'100326','BI005':'100328','BI006':'132749',
    'BI010':'100329','BI020':'100330','BI030':'100331','BI040':'100334','BI044':'131207',
    'BI045':'131206','BI050':'100337','BJ020':'100340','BJ030':'100341','BJ031':'100342',
    'BJ040':'100343','BJ060':'100344','BJ065':'100345','BJ080':'100347','BJ099':'100348',
    'BJ100':'100349','BJ110':'100350','CA010':'100353','CA030':'100355','DA005':'100356',
    'DA010':'100358','DB010':'100362','DB029':'154961','DB061':'100365','DB070':'100366',
    'DB071':'192101','DB090':'100368','DB100':'100369','DB110':'100370','DB115':'100371',
    'DB150':'100372','DB160':'100373','DB170':'100374','DB180':'100375','EA010':'100380',
    'EA020':'100381','EA030':'100382','EA040':'100384','EA050':'100385','EA055':'100386',
    'EB010':'100387','EB020':'100388','EB070':'100390','EC005':'100392','EC010':'100393',
    'EC015':'130380','EC020':'100394','EC040':'100396','EC060':'100398','ED010':'100399',
    'ED020':'100400','EE010':'100401','FA000':'100405','FA002':'100406','FA003':'100407',
    'FA015':'100410','FA091':'100413','FA100':'100414','FA110':'800591','FA165':'100416',
    'FA210':'100417','GA005':'800078','GA033':'500002','GA036':'801161','GA037':'801074',
    'GA038':'800786','GA041':'800291','GB005':'100436','GB013':'114580','GB015':'100438',
    'GB030':'100441','GB035':'100442','GB040':'100443','GB045':'100444','GB050':'100446',
    'GB055':'100448','GB065':'100452','GB070':'100453','GB075':'100454','GB230':'100456',
    'GB250':'100457','SU001':'100462','ZB030':'100465','ZB050':'177997','ZC050':'800597',
    'ZD015':'100578','ZD020':'100473','ZD040':'100475','ZD045':'100476','ZI031':'121591',
    'ZI039':'132721'
  }, // End of subtypeList
  // ##### End of ESRI FCSubtype Rules #####

}; // End of tds70.rules
