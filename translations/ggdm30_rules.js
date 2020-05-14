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
    GGDMv3.0 One2one rules

    With import rules for GGDMv2.1
*/

ggdm30.rules = {
  // ##### Start of One2One Rules #####

  // FCODE rules for Import
  // This is where anything "unique" to GGDM goes.
  fcodeOne2oneIn : [
    // ['F_CODE','AP030','highway','road'], // Road
  ], // End fcodeOne2oneIn

  // These are for finding an FCODE for export.
  // Note: These get swapped around before being used
  fcodeOne2oneOut : [
    // ['F_CODE','AN060','railway','yes'], // Rail yards
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
    ['F_CODE','AL013','man_made','depot'],
    ['F_CODE','AL020','landuse','industrial'], // From OSM - Map to BUA
    ['F_CODE','AL020','landuse','residential'], // From OSM - Map to BUA
    ['F_CODE','AL024','place','neighborhood'], // Neighbourhood - US spelling
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
    ['F_CODE','AP030','highway','construction'],
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
    ['F_CODE','GB013','aeroway','approach_light'], // From OSM
    ['F_CODE','GB013','aeroway','landing_light'], // From OSM
    ['F_CODE','GB013','aeroway','navigation_aid'], // From OSM
    ['F_CODE','GB013','aeroway','papi'], // From OSM
    ['F_CODE','GB013','aeroway','vasi'], // From OSM
    ['F_CODE','GB005','aeroway','sport_airport'], // Land Aerodrome
    ['F_CODE','ZD040','natural','bay'], // From OSM
    ['F_CODE','ZD040','natural','cape'], // From OSM
    ['F_CODE','ZD040','natural','hill'], // From GeoNames
    ['F_CODE','ZD040','natural','peak'], // From OSM
    ['F_CODE','ZD040','natural','valley'], // From GeoNames
  ], // End fcodeOne2oneOut

  // One2one rules for Text Fields
  txtBiased : {
    'ADR':'addr:full', // Address
    'AQN':'aquifer_name', // Aquifer Name
    // 'BA000_VDR':'source:datum:sounding:name', // Water Line : Sounding Datum Name - ZHDP_VDR
    'BC040_COL':'seamark:light:character', // Mar Nav Lt : Character of Light
    'BC040_MLR':'seamark:light:ranges', // Mar Nav Lt : Multiple Light Ranges
    'BEN':'be_number', // Basic Encyclopedia (BE) Number
    'BER':'seamark:berth:number', // Berth Identifier
    'BRN':'bridge:reference', // Bridge Reference Number
    'BRR':'seamark:bearing:category', // Bearing and Reciprocal Category
    'CCR':'seamark:light_support:description', // Colour of Navigation Mark Description
    'CDM':'source:cadastral:measurement', // Cadastral Source Measurement
    'CID':'source:cell_identifier', // Cell Identifier
    'CNCP':'construction_completion_date', // Construction Completion Date
    // 'COL':'seamark:light:character', // Character of Light - Same as BC040_COL
    'CSI':'source:cadastral:identifier', // Cadastral Source Identifier
    'CSY':'source:cadastral:identifier:type', // Cadastral Source Identifier Type
    'CUD':'source:currency:datetime', // Currency Date and Time
    'DAN':'navigationaid:description', // Description of Aids to Navigation
    'DIA':'diam:classification', // DIAM Functional Classification
    'DQS':'source:data_quality', // Data Quality Statement
    'DRP':'reference_point:description', // Description of Reference Point
    'ETZ':'source:extraction_specification:version', // Extraction Specification Version
    'FUFI':'uuid:feature', // Feature Unique Identifier
    'FURL':'source_ref:external', // External References
    // 'F_CODE':'F_CODE', // Feature Code
    'GA032_NSX':'navigationaid:name', // Aero Rad Nav Inst : Aeronautical Radio Navigation Service Name
    'GB001_AID':'aeroway:uuid', // Aerodrome : Aerodrome Identifier
    'GB052_RIDH':'aeroway:runway_designator_high', // Runway Direction : Runway Designator (high end)
    'GB052_RIDL':'aeroway:runway_designator_low', // Runway Direction : Runway Designator (low end)
    'HSE':'marine_current_month_interval', // Periodic Marine Current Month Interval
    'IKO':'icao', // ICAO Location Indicator
    'LASZ':'aeroway:largest_aircraft', // Largest Aircraft
    'MCA':'navigationaid:characters_emitted', // Characters Emitted
    'MDE':'source:update:datetime', // Maintenance Date and Time
    // 'MEM':'raw:MEM', // Memorandum - ZI006_MEM
    // 'MLR':'raw:MLR', // Multiple Light Ranges - BC040_MLR
    'NA8':'official_name', // Aerodrome Official Name
    'NAO':'installation_operator_name', // Installation Operator
    'OTH':'note:oth', // Specified Domain Value(s)
    'PDA':'source:dfdd_accessor', // Property DFDD-compatible Accessor
    'PRVR':'primary_river_uuid', // Primary River
    'PSE':'restriction_interval', // Periodic Restriction Month Interval
    'RIN_RTN':'ref', // Route Identification <route designation>
    'RIN_RTN2':'ref:2', // Route Identification [2] <route designation>
    'RIN_RTN3':'ref:3', // Route Identification [3] <route designation>
    'RTL':'source:title', // Resource Title
    // 'RTN':'raw:RTN', // Route Designation - RIN_RTN
    // 'RTN2':'raw:RTN2', // Route Designation [2]
    // 'RTN3':'raw:RTN3', // Route Designation [3]
    'SCVN':'source:schema:version', // Schema Version
    'SSE':'seasonal_ice_limit', // Seasonal Ice Limit
    'TAN':'navigationaid:tss', // TSS-associated Aids to Navigation
    'UFI':'uuid', // Unique Feature Identifier. This gets cleaned up in post processing
    'URI':'source:ref', // Unique Resource Identifier
    'VCS_VCR':'max_clearance:note', // Vertical Clearance, Safe <reference level remark>
    'VOI':'aeroway:obstruction', // Vertical Obstruction Identifier
    'WAD':'well:description', // Water Well Construction Description
    'WPI':'world_port_index', // World Port Index Identifier - This is part of Built Up Area. What??
    'WTCL':'water:colour', // Water Colour
    // 'ZHBH_VDR':'raw:ZHBH_VDR', // Base Hgt : Snd Meta : Sounding Datum Name - ZHDP_VDR
    'ZHDP_VDR':'source:datum:sounding:name', // Hydro Dpth : Snd Meta : Sounding Datum Name
    'ZI001_SDP':'source', // Source : Source Description
    'ZI001_SDV':'source:datetime', // Source : Source Date and Time
    'ZI001_VSD':'source:vertical_source:datetime', // Source : Vertical Source Date and Time
    'ZI001_VSN':'source:vertical_source:description', // Source : Vertical Source Description
    'ZI002_UFI':'security:restriction_ufi', // Restriction Information : Unique Entity Identifier
    'ZI004_PRE':'source:processing_information', // Process Step Information : Process Step Description
    'ZI005_FNA':'name', // Geo Name Info : Full Name
    'ZI005_FNA2':'alt_name', // Geo Name Info : Full Name [2]
    'ZI005_FNA3':'alt_name:2', // Geo Name Info : Full Name [3]
    'ZI005_FNAA':'name:first_side', // Geo Name Info (1) : Full Name (first side)
    'ZI005_FNAB':'name:second_side', // Geo Name Info (2) : Full Name (second side)
    'ZI005_NFN':'gndb_id', // Geo Name Info : Name Identifier
    'ZI005_NFN2':'gndb_id:2', // Geo Name Info : Name Identifier [2]
    'ZI005_NFN3':'gndb_id:3', // Geo Name Info : Name Identifier [3]
    'ZI005_NFNA':'gndb_id:first_side', // Geo Name Info (1) : Name Identifier (first side)
    'ZI005_NFNB':'gndb_id:second_side', // Geo Name Info (2) : Name Identifier (second side)
    'ZI006_MEM':'note', // Note : Memorandum
    // 'ZI012_VDR':'raw:ZI012_VDR', // Dpth Crv : Snd Meta : Sounding Datum Name - ZHDP_VDR
    'ZI020_FI2A':'country_name:first_side', // Designation : FIPS 10-4 Country Name (first side)
    'ZI020_FI2B':'country_name:second_side', // Designation : FIPS 10-4 Country Name (second side)

    // NOTE: These are validated in post-processing
    'ZI020_GE4':'is_in:country_code', // Designation : GENC Short URN-based Identifier
    'ZI020_GE42':'country_code:second', // Designation : GENC Short URN-based Identifier [2]
    'ZI020_GE43':'country_code:third', // Designation : GENC Short URN-based Identifier [3]
    'ZI020_GE44':'country_code:fourth', // Designation : GENC Short URN-based Identifier [4]
    'ZI020_GE4A':'country_code:first_side', // Designation : GENC Short URN-based Identifier
    'ZI020_GE4B':'country_code:second_side', // Designation : GENC Short URN-based Identifier
    'ZI020_IC4':'is_in:country:en', // Designation : ISO 3166-1 English Country Name
    'ZI031S_URI':'source:dataset:surface:uuid', // Dataset (Surface) : Unique Resource Identifier
    'ZI039S_UFI':'source:entity:surface:uuid', // Entity Collection Metadata (Surface) : Unique Entity Identifier
    'ZI103_MVD':'magnetic_variation:date', // Magnetic Variation Information : Magnetic Variation Date
    'ZSAX_RX0':'security:dissemination_control:ic', // Restriction : Security <resource dissemination controls>
    'ZSAX_RX3':'security:dissemination_control:non_ic', // Restriction : Security <resource non-intelligence community markings>
    'ZSAX_RX4':'security:resource_owner', // Restriction : Security <resource owner-producer>
  }, // End txtBiased

  // One2one ruled for Number fields
  numBiased : {
    'ADUR':'water_supply_duration', // Average Water Supply Duration
    'AHA':'source:accuracy:horizontal', // Absolute Horizontal Accuracy (90%)
    'ANFL':'annual_flow_rate', // Annual Flow Rate
    'AOO':'angle', // Angle of Orientation
    'AQTL':'aquifer:thickness:lower', // Aquifer Thickness <lower value>
    'AQTU':'aquifer:thickness:upper', // Aquifer Thickness <upper value>
    'ARA':'feature_area', // Area - this has been moved from 'area' due to hoot conflicts
    'AVA':'source:accuracy:vertical', // Absolute Vertical Accuracy (90%)
    'AYRL':'aquifer:yield:lower', // Aquifer Yield Rating <lower value>
    'AYRU':'aquifer:yield:upper', // Aquifer Yield Rating <upper value>
    'BC034_BRF':'seamark:radio_station:frequency', // Mar Radiobeacon : Broadcast Frequency
    'BC034_BRF2':'seamark:radio_station:frequency:2', // Mar Radiobeacon : Broadcast Frequency [2]
    'BC040_EOL':'seamark:light:height', // Mar Nav Lt : Light Elevation
    'BC040_LCN':'seamark:light:characteristic_number', // Mar Nav Lt : Light Characteristic Number
    'BC040_LVN':'seamark:light:range', // Mar Nav Lt : Mar Nav Nominal Range
    'BC040_PER':'seamark:light:period', // Mar Nav Lt : Period of Light
    'BC040_ZVH':'seamark:light:height:max', // Mar Nav Lt : Highest Elevation
    'BEL':'ele:base', // Base Elevation - conflict with "ele" but is only used in one feature: ZB035 Control Point
    'BH141_AWBA':'waterway:bank1:slope:above_water', // Wtrbdy Bank : Above Water Bank Slope (first bank)
    'BH141_AWBB':'waterway:bank2:slope:above_water', // Above Water Bank Slope (second bank)
    'BH141_WBHA':'waterway:bank1:height', // Wtrbdy Bank : Waterbody Bank Height (first bank)
    'BH141_WBHB':'waterway:bank2:height', // Wtrbdy Bank : Waterbody Bank Height (second bank)
    'BNF':'building:levels', // Floor Count
    'BPWHAL':'waterway:bank1:height:lower', // Wtrbdy Bank : Predom Wtrbdy Bank Height (first bank) <lower value>
    'BPWHAU':'waterway:bank1:height:upper', // Wtrbdy Bank : Predom Wtrbdy Bank Height (first bank) <upper value>
    'BPWHBL':'waterway:bank2:height:lower', // Predom Wtrbdy Bank Height (second bank) <lower value>
    'BPWHBU':'waterway:bank2:height:upper', // Predom Wtrbdy Bank Height (second bank) <upper value>
    'BPWSAL':'waterway:bank1:slope:lower', // Predom Wtrbdy Bank Slope (first bank) <lower value>
    'BPWSAU':'waterway:bank1:slope:upper', // Predom Wtrbdy Bank Slope (first bank) <upper value>
    'BPWSBL':'waterway:bank2:slope:lower', // Predom Wtrbdy Bank Slope (second bank) <lower value>
    'BPWSBU':'waterway:bank2:slope:upper', // Predom Wtrbdy Bank Slope (second bank) <upper value>
    // 'BRF':'raw:BRF', // Broadcast Frequency - BC034_BRF
    // 'BRF2':'raw:BRF2', // Broadcast Frequency [2] - BC034_BRF2
    'BRG':'bearing:to_object', // Bearing of Object
    'BRS':'bearing:from_seaward', // Bearing from Seaward
    'BWVCAL':'waterway:bank1:cover:lower', // Wtrbdy Bank : Wtrbdy Bank Veg Cover (first bank) <lower value>
    'BWVCAU':'waterway:bank1:cover:upper', // Wtrbdy Bank : Wtrbdy Bank Veg Cover (first bank) <upper value>
    'BWVCBL':'waterway:bank2:cover:lower', // Wtrbdy Bank : Wtrbdy Bank Veg Cover (second bank) <lower value>
    'BWVCBU':'waterway:bank2:cover:upper', // Wtrbdy Bank Veg Cover (second bank) <upper value>
    'C35':'seamark:observation_station:current:speed:5hr_before', // Tidal Current Speed 5 Hours Before
    'C36':'seamark:observation_station:current:speed:4hr_before', // Tidal Current Speed 4 Hours Before
    'C37':'seamark:observation_station:current:speed:3hr_before', // Tidal Current Speed 3 Hours Before
    'C38':'seamark:observation_station:current:speed:2hr_before', // Tidal Current Speed 2 Hours Before
    'C39':'seamark:observation_station:current:speed:1hr_before', // Tidal Current Speed 1 Hour Before
    'C40':'seamark:observation_station:current:speed', // Tidal Current Speed
    'C41':'seamark:observation_station:current:speed:1hr_after', // Tidal Current Speed 1 Hour After
    'C42':'seamark:observation_station:current:speed:2hr_after', // Tidal Current Speed 2 Hours After
    'C43':'seamark:observation_station:current:speed:3hr_after', // Tidal Current Speed 3 Hours After
    'C44':'seamark:observation_station:current:speed:4hr_after', // Tidal Current Speed 4 Hours After
    'C45':'seamark:observation_station:current:speed:5hr_after', // Tidal Current Speed 5 Hours After
    'C46':'seamark:observation_station:current:speed:6hr_after', // Tidal Current Speed 6 Hours After
    'CDI':'well:casing_diameter', // Casing Diameter
    'CDL':'covered_drain:length', // Covered Drain Length
    'CRN':'water:current:speed:min', // Current Rate Minimum
    'CRS':'water:current:speed', // Current Rate (Speed)
    'CRV':'depth:value', // Depth Curve or Contour Value
    'CRX':'water:current:speed:max', // Current Rate Maximum
    'CTL':'railway:track:length', // Cumulative Track Length
    'D35':'seamark:observation_station:current:direction:5hr_before', // Tidal Current Direction 5 Hours Before
    'D36':'seamark:observation_station:current:direction:4hr_before', // Tidal Current Direction 4 Hours Before
    'D37':'seamark:observation_station:current:direction:3hr_before', // Tidal Current Direction 3 Hours Before
    'D38':'seamark:observation_station:current:direction:2hr_before', // Tidal Current Direction 2 Hours Before
    'D39':'seamark:observation_station:current:direction:1hr_before', // Tidal Current Direction 1 Hour Before
    'D40':'seamark:observation_station:current:direction', // Tidal Current Direction
    'D41':'seamark:observation_station:current:direction:1hr_after', // Tidal Current Direction 1 Hour After
    'D42':'seamark:observation_station:current:direction:2hr_after', // Tidal Current Direction 2 Hours After
    'D43':'seamark:observation_station:current:direction:3hr_after', // Tidal Current Direction 3 Hours After
    'D44':'seamark:observation_station:current:direction:4hr_after', // Tidal Current Direction 4 Hours After
    'D45':'seamark:observation_station:current:direction:5hr_after', // Tidal Current Direction 5 Hours After
    'D46':'seamark:observation_station:current:direction:6hr_after', // Tidal Current Direction 6 Hours After
    'DCAP':'daily_capacity', // Daily Capacity
    'DEV':'level', // Deck Level
    'DF1':'seamark:calling-in_point:traffic_direction', // Direction of Traffic - 1
    'DF2':'seamark:calling-in_point:traffic_direction:2', // Direction of Traffic - 2
    'DF3':'seamark:calling-in_point:traffic_direction:3', // Direction of Traffic - 3
    'DF4':'seamark:calling-in_point:traffic_direction:4', // Direction of Traffic - 4
    'DIM':'diameter', // Diameter
    'DMBL':'undergrowth:density:lower', // Undergrowth Density (lower value)
    'DMBU':'undergrowth:density:upper', // Undergrowth Density (upper value)
    'DMF':'feature_count', // Feature Count
    'DMT':'canopy_cover', // Canopy Cover
    'DOF':'flow_direction', // Direction of Flow
    'DOXY':'disolved_oxygen', // Dissolved Oxygen
    'DPAL':'aquifer:depth:lower', // Aquifer Depth <lower value>
    'DPAU':'aquifer:depth:upper', // Aquifer Depth <upper value>
    'DWL':'water:dynamic_level', // Dynamic Water Level
    'DZC':'deck_count', // Deck Count
    'DZP':'depth:maximum_below_surface', // Deepest Depth Below Surface Level
    // 'EOL':'raw:EOL', // Light Elevation - BC040_EOL
    'EPW':'generator:output:electricity', // Electrical Power Generation Capacity
    'EQS':'density_equivalent_scale', // Density Equivalent Scale
    'EVA':'source:accuracy:elevation', // Elevation Vertical Accuracy (90%)
    'FCL':'ferry:crossing_distance', // Ferry Crossing Distance
    //    'FCSUBTYPE':'etds:fcsubtype', // Very ESRI Specific. Ignored for now
    'FDI':'geological:dip', // Geologic Dip
    'FEO':'feature_orientation', // Feature Element Orientation
    'GB052_TRHH':'aeroway:runway_highend:true_heading', // Runway Direction (high end) : Runway True Heading (high)
    'GB052_TRHL':'aeroway:runway_lowend:true_heading', // Runway Direction (low end) : Runway True Heading (low)
    'GSGCHL':'aeroway:runway_highend:slope:high_lowervalue', // Runway Direction : Surface Slope (high end) <lower value>
    'GSGCHU':'aeroway:runway_highend:slope:high_uppervalue', // Runway Direction : Surface Slope (high end) <upper value>
    'GSGCLL':'aeroway:runway_highend:slope:low_lowervalue', // Runway Direction : Surface Slope (low end) <lower value>
    'GSGCLU':'aeroway:runway_highend:slope:low_uppervalue', // Runway Direction : Surface Slope (low end) <upper value>
    'GWTR':'water:transmissivity', // Groundwater Transmissivity
    'HBH':'height:hydrographic_base', // Hydrographic Base Height
    'HCA':'bridge:horizontal_clearance', // Horizontal Clearance
    'HDH':'hydrographic_drying_height', // Hydrographic Drying Height
    // 'HDP':'raw:HDP', // Hydrographic Depth - ZI025_HDP
    'HEI':'height:object', // Height of Object
    'HGS':'spillway:height', // Spillway Height
    'HGT':'height', // Height Above Surface Level
    'HSC':'beds', // Hospital Number of Beds
    'HSCD':'hose_connection_diameter', // Hose Connection Diameter
    'HVA':'source:accuracy:height', // Height Vertical Accuracy (90%)
    'HYCD':'hydraulic_conductivity', // Hydraulic Conductivity
    'KVA':'voltage', // Power Line Maximum Voltage
    'LC1':'mlc:wheeled_oneway', // Load Class Type 1
    'LC2':'mlc:wheeled', // Load Class Type 2
    'LC3':'mlc:tracked_oneway', // Load Class Type 3
    'LC4':'mlc:tracked', // Load Class Type 4
    'LCA':'crane:lifting_capacity', // Lifting Capacity
    // 'LCN':'raw:LCN', // Light Characteristic Number - BC040_LCN
    'LDC':'length:crest', // Dam Crest Length
    'LEA':'depth:minimum_below_surface', // Least Depth Below Surface Level
    'LNU':'length:interior_useable', // Usable Length
    'LOG':'gradient:length', // Gradient Length
    'LOR':'length:of_range', // Length of Range
    'LSA':'seamark:light:sector:angle', // Light Sector Angle
    'LSI':'seamark:light:sector_start', // Light Sector Initial Limit
    'LST':'seamark:light:sector_end', // Light Sector Terminal Limit
    'LTN':'lanes', // Track or Lane Count
    'LVN':'seamark:light:radius', // Maritime Navigation Nominal Range
    'LZN':'length', // Length
    'MAG':'magnetic_variation', // Magnetic Variation
    'MWD':'depth:designed:maximum', // Maximum Design Water Depth
    'MWG':'divider:width', // Centerline Spacing
    'NOR':'rooms', // Number of Rooms
    'NOS':'bridge:span_count', // Span Count
    'NPL':'cables', // Parallel Line Count: See AT005
    'OHC':'max_clearance', // Overhead Clearance
    'ORC':'seamark:radio_station:range', // Primary Operating Range
    'PER':'seamark:light:period', // Period of Light
    'PFD':'depth:predominant', // Predominant Feature Depth
    'PFH':'height:feature', // Predominant Feature Height
    'PFHL':'height:feature:lower', // Predominant Feature Height <lower value>
    'PFHU':'height:feature:upper', // Predominant Feature Height <upper value>
    'PPL':'population', // Population Count
    'PSN':'sinuosity', // Path Sinuosity
    'PUD':'depth:pump', // Pump Depth
    'PVH':'height:vegetation', // Predominant Vegetation Height
    'PWA':'depth', // Predominant Water Depth
    'PWAL':'depth:lower', // Predominant Water Depth <lower value>
    'PWAU':'depth:upper', // Predominant Water Depth <upper value>
    'PWR':'navigationaid:power', // NAVAID Power
    'RAD':'curve_radius', // Curve Radius
    'RMWL':'median:min_width', // Route Median Width <lower value>
    'RMWU':'median:max_width', // Route Median Width <upper value>
    'SDCL':'soil:depth:lower', // Soil Depth <lower value>
    'SDCU':'soil:depth:upper', // Soil Depth <upper value>
    'SDI':'geological:strike_direction', // Geologic Strike Direction
    'SDO':'dune:orientation', // Sand Dune Orientation
    'SDSL':'tree:diameter:lower', // Stem Diameter <lower value>
    'SDSU':'tree:diameter:upper', // Stem Diameter <upper value>
    'SGCL':'incline:min', // Surface Slope <lower value>
    'SGCU':'incline:max', // Surface Slope <upper value>
    'SHC':'safe_clearance:horizontal', // Safe Horizontal Clearance
    'SNDL':'ice:depth:lower', // Snow or Ice Depth <lower value>
    'SNDU':'ice:depth:upper', // Snow or Ice Depth <upper value>
    'SPD':'maxspeed:mph', // Speed Limit (MPH)
    'SPM':'maxspeed', // Speed Limit (KPH)
    'STAF':'staff', // Total Staff
    'STNB':'strahler_order', // Strahler Number
    'STR':'canopy_cover:summer', // Summer Canopy Cover
    'SWCP':'water:capacity', // Specific Water Capacity
    'SWW':'water_level', // Static Water Level
    'THI':'thickness', // Thickness
    'TSCL':'tree:spacing:lower', // Tree Spacing <lower value>
    'TSCU':'tree:spacing:upper', // Tree Spacing <upper value>
    'UBC':'bridge:under_clearance', // Underbridge Clearance
    'VAVL':'magnetic_variation:lower', // Magnetic Variation Anomaly <lower value>
    'VAVU':'magnetic_variation:upper', // Magnetic Variation Anomaly <upper value>
    'VCO':'safe_clearance:open', // Vertical Clearance, Open
    'VCS':'safe_clearance:vertical', // Vertical Clearance, Safe
    'VEC':'vehicle_capacity', // Vehicle Capacity
    'VLM':'volume', // Volume
    'VTI':'trafficability_impact', // Vegetation Trafficability Impact
    // 'WBD':'raw:WBD', // Waterbody Depth - PWA
    'WCAC':'water:alkalinity', // Alkalinity
    'WCL':'canopy_cover:winter', // Winter Canopy Cover
    // 'WD1':'raw:WD1', // Route Minimum Travelled Way Width - ZI016_WD1
    'WD2':'width:total_usable', // Route Total Usable Width
    'WD3':'width:gap', // Terrain Gap Width
    'WDAL':'depth:average:lower', // Average Water Depth <lower value>
    'WDAU':'depth:average:upper', // Average Water Depth <upper value>
    'WDH':'depth:max', // Predominant Maximum Water Depth
    'WDL':'depth:min', // Predominant Minimum Water Depth
    'WDU':'width:interior_useable', // Usable Width
    'WHCO':'water:bicarbonate_concentration', // Bicarbonate
    'WID':'width', // Width
    'WLSS':'water:loss', // Water Loss
    'WOC':'width:crest', // Dam Crest Width
    'WPST':'windows_per_story', // Windows Per Story
    'WRCL':'water:recharge_rate:lower', // Water Recharge Rate <lower value>
    'WRCU':'water:recharge_rate:upper', // Water Recharge Rate <upper value>
    'WT2':'width:second_way', // Width of Second Travelled Way
    // 'WTBB':'raw:WTBB', // Boron - See ZI024_XXX
    // 'WTCA':'raw:WTCA', // Calcium
    // 'WTFE':'raw:WTFE', // Total Iron
    // 'WTKK':'raw:WTKK', // Potassium
    // 'WTMN':'raw:WTMN', // Manganese
    // 'WTNA':'raw:WTNA', // Sodium
    // 'WTNO':'raw:WTNO', // Nitrate
    // 'WTPO':'raw:WTPO', // Phosphate
    // 'WTSI':'raw:WTSI', // Silicon
    'WVA':'water:velocity:average', // Predominant Average Water Velocity
    'WVH':'water:velocity:maximum', // Predominant Maximum Water Velocity
    'WVL':'water:velocity:minimum', // Predominant Minimum Water Velocity
    'WWRL':'water:withdrawl_rate:lower', // Water Withdrawal Rate <lower value>
    'WWRU':'water:withdrawl_rate:upper', // Water Withdrawal Rate <upper value>
    'ZI005_GNR':'gndb:rank', // Geo Name Info : Geographic Name Rank
    'ZI005_GNR2':'gndb:rank:2', // Geo Name Info : Geographic Name Rank [2]
    'ZI005_GNR3':'gndb:rank:3', // Geo Name Info : Geographic Name Rank [3]
    'ZI015_GCUL':'cartographic_scale:name:lower', // Geo Name Collection : Geographic Name Cartographic Usability Range <lower value>
    'ZI015_GCUU':'cartographic_scale:name:upper', // Geo Name Collection : Geographic Name Cartographic Usability Range <upper value>
    'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
    'ZI017_GAW':'gauge', // Track Information : Railway Gauge
    // 'ZI018_BRF':'raw:ZI018_BRF', // Wireless : Broadcast Frequency - See BC034_BRF
    'ZI024_ASE':'water:arsnic_concentration', // Water : Arsenic Concentration
    'ZI024_AWO':'water:flow', // Water : Predominant Water Flow
    'ZI024_AWOI':'water:flow:max', // Water : Predominant Water Flow - Maximum
    'ZI024_AWOX':'water:flow:min', // Water : Predominant Water Flow - Minimum
    'ZI024_CFR':'water:coliform_concentration', // Water : Coliform Concentration
    'ZI024_CLO':'water:chloride_concentration', // Water : Chloride Concentration
    'ZI024_CYN':'water:cyanide_concentration', // Water : Cyanide Concentration
    'ZI024_FMR':'water:freshwater_production:max', // Water : Freshwater Production Maximum Rate
    'ZI024_FRR':'water:freshwater_production', // Water : Freshwater Production Rate
    'ZI024_HAR':'water:hardness', // Water : Water Hardness
    'ZI024_MGN':'water:magnesium_concentration', // Water : Magnesium Concentration
    'ZI024_PHW':'water:ph', // Water: pH
    'ZI024_SUL':'water:sulfate_concentration', // Water : Sulfate Concentration
    'ZI024_TDS':'water:total_dissolved_solids', // Water : Total Dissolved Solids
    'ZI024_TEP':'water:temperature', // Water : Temperature
    'ZI024_TUR':'water:turbidity', // Water : Nephelometric Turbidity
    'ZI024_WAC':'water:conductivity', // Water : Water Conductivity
    'ZI024_WTBB':'water:boron_concentration', // Water : Boron
    'ZI024_WTCA':'water:calcium_concentration', // Water : Calcium
    'ZI024_WTFE':'water:iron_concentration', // Water : Total Iron
    'ZI024_WTKK':'water:potassium_concentration', // Water : Potassium
    'ZI024_WTMN':'water:manganese_concentration', // Water : Manganese
    'ZI024_WTNA':'water:sodium_concentration', // Water : Sodium
    'ZI024_WTNO':'water:nitrate_concentration', // Water : Nitrate
    'ZI024_WTPO':'water:phosphate_concentration', // Water : Phosphate
    'ZI024_WTSI':'water:silicon_concentration', // Water : Silicon
    'ZI025_HDP':'depth:hydrographic', // Vert Pos : Hydrographic Depth
    'ZI026_CTUL':'cartographic_scale:lower', // Physical Object Metadata : Cartographic Topography Usability Range <lower value>
    'ZI026_CTUU':'cartographic_scale:upper', // Physical Object Metadata : Cartographic Topography Usability Range <upper value>
    'ZVA':'ele', // Aerodrome Elevation
    'ZVH':'ele:max', // Highest Elevation
    'ZVH_AVA':'source:accuracy:highest_elevation', // Highest Elevation <absolute vertical accuracy>
  }, // End numBiased


  // Common one2one rules. Used for both import and export
  // taken from the GGDM30 spec
  one2one : [

    // ACH - Anchorage Type
    // ['ACH','-999999',undefined,undefined], // No Information
    ['ACH','1','seamark:anchorage:category','unrestricted'], // Unrestricted Anchorage
    ['ACH','2','seamark:anchorage:category','deep_water'], // Deep Water Anchorage
    ['ACH','3','seamark:anchorage:category','tanker'], // Tanker Anchorage
    ['ACH','4','seamark:anchorage:category','explosives'], // Explosives Anchorage
    ['ACH','5','seamark:anchorage:category','quarantine'], // Quarantine Anchorage
    ['ACH','6','seamark:anchorage:category','seaplane'], // Seaplane Anchorage
    ['ACH','7','seamark:anchorage:category','small_craft'], // Small Craft Anchorage
    ['ACH','8','seamark:anchorage:category','small_craft_mooring'], // Small Craft Mooring Area
    ['ACH','9','seamark:anchorage:category','24_hours'], // Anchorage for up to 24 hours
    ['ACH','10','seamark:anchorage:category','limited_period'], // Time Limited
    ['ACH','11','seamark:anchorage:category','reserved_anchorage'], // Reserved Anchorage
    ['ACH','12','seamark:anchorage:category','naval_reserve'], // Naval Reserve Anchorage
    ['ACH','13','seamark:anchorage:category','berth'], // Anchorage Berth
    ['ACH','14','seamark:anchorage:category','waiting'], // Waiting Anchorage
    ['ACH','999','seamark:anchorage:category','other'], // Other

    // ADI - Administrative Division
    // ['ADI','-999999',undefined,undefined], // No Information
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

    // AQF - Aquaculture Facility Type
    // ['AQF','-999999',undefined,undefined], // No Information
    ['AQF','1','aquaculture','marine_culture'], // Marine Culture
    ['AQF','2','aquaculture','sea_ranch'], // Sea Ranch
    ['AQF','3','aquaculture','shrimp'], // Shrimp Farm
    ['AQF','4','aquaculture','kelp'], // Kelp Farm
    ['AQF','8','aquaculture','pearl_culture'], // Pearl Culture Farm

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

    // AQO2 - Aquifer Overburden [2]
    // AQO3 - Aquifer Overburden [3]

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

    // AQP2 - Aquifer Composition [2]
    // AQP3 - Aquifer Composition [3]

    // AQTC - Aquifer Thickness <interval closure>
    ['AQTC','2','aquifer:thickness:closure','open_interval'], // Open Interval
    ['AQTC','3','aquifer:thickness:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['AQTC','4','aquifer:thickness:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['AQTC','5','aquifer:thickness:closure','closed_interval'], // Closed Interval
    ['AQTC','6','aquifer:thickness:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['AQTC','7','aquifer:thickness:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['AQTC','8','aquifer:thickness:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['AQTC','9','aquifer:thickness:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

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
    // ASU2 - Aerodrome Movement Area Surface Composition [2]  - See ZI019_ASU2
    // ASU3 - Aerodrome Movement Area Surface Composition [3] - See ZI019_ASU3

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

    // AT005_CAB - Cable : Cable Type - See CAB
    // AT005_CAB2 - Cable : Cable Type [2] - See CAB2
    // AT005_CAB3 - Cable : Cable Type [3] - See CAB3

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
    // ['AWP','-999999',undefined,undefined], // No Information
    ['AWP','1000','light:obstacle','no'],
    ['AWP','1001','light:obstacle','yes'],

    // AXS - Aerodrome Surface Status
    // ['AXS','-999999',undefined,undefined], // No Information
    ['AXS','1','aeroway:surface:status','closed'], // Closed
    ['AXS','2','aeroway:surface:status','open'], // Open
    ['AXS','3','aeroway:surface:status','work_in_progress'], // Work in Progress
    ['AXS','4','aeroway:surface:status','parked_or_disabled_aircraft'], // Parked or Disabled Aircraft

    // AYRC - Aquifer Yield Rating <interval closure>
    ['AYRC','2','aquifer:yield:closure','open_interval'], // Open Interval
    ['AYRC','3','aquifer:yield:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['AYRC','4','aquifer:yield:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['AYRC','5','aquifer:yield:closure','closed_interval'], // Closed Interval
    ['AYRC','6','aquifer:yield:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['AYRC','7','aquifer:yield:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['AYRC','8','aquifer:yield:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['AYRC','9','aquifer:yield:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // AZC - Man-made
    // ['AZC','-999999',undefined,undefined], // No Information
    ['AZC','1000','artificial','no'],
    ['AZC','1001','artificial','yes'],

    // BA000_VDC - Water Line : Sounding Datum - See ZHDP_VDC

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

    // BBMCA - Wtrbdy Bank (1) : Bottom Material Type (first bank)
    // ['BBMCA','-999999',undefined,undefined], // No Information
    ['BBMCA','1','waterway:bank1:bottom','clay_and_silt'], // Clay and Silt
    ['BBMCA','2','waterway:bank1:bottom','silty_sands'], // Silty Sands
    ['BBMCA','3','waterway:bank1:bottom','sand_and_gravel'], // Sand and Gravel
    ['BBMCA','4','waterway:bank1:bottom','gravel_and_cobble'], // Gravel and Cobble
    ['BBMCA','5','waterway:bank1:bottom','rocks_and_boulders'], // Rocks and Boulders
    ['BBMCA','6','waterway:bank1:bottom','bedrock'], // Bedrock
    ['BBMCA','7','waterway:bank1:bottom','paved'], // Paved
    ['BBMCA','8','waterway:bank1:bottom','peat'], // Peat
    ['BBMCA','9','waterway:bank1:bottom','sand_over_mud'], // Sand over Mud
    ['BBMCA','11','waterway:bank1:bottom','coral'], // Coral
    ['BBMCA','14','waterway:bank1:bottom','sand'], // Sand
    ['BBMCA','15','waterway:bank1:bottom','concrete'], // Concrete
    ['BBMCA','16','waterway:bank1:bottom','masonry'], // Masonry
    ['BBMCA','17','waterway:bank1:bottom','reinforced_concrete'], // Reinforced Concrete
    ['BBMCA','18','waterway:bank1:bottom','soil'], // Soil
    ['BBMCA','999','waterway:bank1:bottom','other'], // Other

    // BBMCA2 - Wtrbdy Bank (1) : Bottom Material Type (first bank) [2]
    // ['BBMCA2','-999999',undefined,undefined], // No Information
    ['BBMCA2','1','waterway:bank1:bottom:2','clay_and_silt'], // Clay and Silt
    ['BBMCA2','2','waterway:bank1:bottom:2','silty_sands'], // Silty Sands
    ['BBMCA2','3','waterway:bank1:bottom:2','sand_and_gravel'], // Sand and Gravel
    ['BBMCA2','4','waterway:bank1:bottom:2','gravel_and_cobble'], // Gravel and Cobble
    ['BBMCA2','5','waterway:bank1:bottom:2','rocks_and_boulders'], // Rocks and Boulders
    ['BBMCA2','6','waterway:bank1:bottom:2','bedrock'], // Bedrock
    ['BBMCA2','7','waterway:bank1:bottom:2','paved'], // Paved
    ['BBMCA2','8','waterway:bank1:bottom:2','peat'], // Peat
    ['BBMCA2','9','waterway:bank1:bottom:2','sand_over_mud'], // Sand over Mud
    ['BBMCA2','11','waterway:bank1:bottom:2','coral'], // Coral
    ['BBMCA2','14','waterway:bank1:bottom:2','sand'], // Sand
    ['BBMCA2','15','waterway:bank1:bottom:2','concrete'], // Concrete
    ['BBMCA2','16','waterway:bank1:bottom:2','masonry'], // Masonry
    ['BBMCA2','17','waterway:bank1:bottom:2','reinforced_concrete'], // Reinforced Concrete
    ['BBMCA2','18','waterway:bank1:bottom:2','soil'], // Soil
    ['BBMCA2','999','waterway:bank1:bottom:2','other'], // Other

    // BBMCA3 - Wtrbdy Bank (1) : Bottom Material Type (first bank) [3]
    // ['BBMCA3','-999999',undefined,undefined], // No Information
    ['BBMCA3','1','waterway:bank1:bottom:3','clay_and_silt'], // Clay and Silt
    ['BBMCA3','2','waterway:bank1:bottom:3','silty_sands'], // Silty Sands
    ['BBMCA3','3','waterway:bank1:bottom:3','sand_and_gravel'], // Sand and Gravel
    ['BBMCA3','4','waterway:bank1:bottom:3','gravel_and_cobble'], // Gravel and Cobble
    ['BBMCA3','5','waterway:bank1:bottom:3','rocks_and_boulders'], // Rocks and Boulders
    ['BBMCA3','6','waterway:bank1:bottom:3','bedrock'], // Bedrock
    ['BBMCA3','7','waterway:bank1:bottom:3','paved'], // Paved
    ['BBMCA3','8','waterway:bank1:bottom:3','peat'], // Peat
    ['BBMCA3','9','waterway:bank1:bottom:3','sand_over_mud'], // Sand over Mud
    ['BBMCA3','11','waterway:bank1:bottom:3','coral'], // Coral
    ['BBMCA3','14','waterway:bank1:bottom:3','sand'], // Sand
    ['BBMCA3','15','waterway:bank1:bottom:3','concrete'], // Concrete
    ['BBMCA3','16','waterway:bank1:bottom:3','masonry'], // Masonry
    ['BBMCA3','17','waterway:bank1:bottom:3','reinforced_concrete'], // Reinforced Concrete
    ['BBMCA3','18','waterway:bank1:bottom:3','soil'], // Soil
    ['BBMCA3','999','waterway:bank1:bottom:3','other'], // Other

    // BBMCB - Wtrbdy Bank (2) : Bottom Material Type (second bank)
    // ['BBMCB','-999999',undefined,undefined], // No Information
    ['BBMCB','1','waterway:bank2:bottom','clay_and_silt'], // Clay and Silt
    ['BBMCB','2','waterway:bank2:bottom','silty_sands'], // Silty Sands
    ['BBMCB','3','waterway:bank2:bottom','sand_and_gravel'], // Sand and Gravel
    ['BBMCB','4','waterway:bank2:bottom','gravel_and_cobble'], // Gravel and Cobble
    ['BBMCB','5','waterway:bank2:bottom','rocks_and_boulders'], // Rocks and Boulders
    ['BBMCB','6','waterway:bank2:bottom','bedrock'], // Bedrock
    ['BBMCB','7','waterway:bank2:bottom','paved'], // Paved
    ['BBMCB','8','waterway:bank2:bottom','peat'], // Peat
    ['BBMCB','9','waterway:bank2:bottom','sand_over_mud'], // Sand over Mud
    ['BBMCB','11','waterway:bank2:bottom','coral'], // Coral
    ['BBMCB','14','waterway:bank2:bottom','sand'], // Sand
    ['BBMCB','15','waterway:bank2:bottom','concrete'], // Concrete
    ['BBMCB','16','waterway:bank2:bottom','masonry'], // Masonry
    ['BBMCB','17','waterway:bank2:bottom','reinforced_concrete'], // Reinforced Concrete
    ['BBMCB','18','waterway:bank2:bottom','soil'], // Soil
    ['BBMCB','999','waterway:bank2:bottom','other'], // Other

    // BBMCB2 - Wtrbdy Bank (2) : Bottom Material Type (second bank) [2]
    // BBMCB3 - Wtrbdy Bank (2) : Bottom Material Type (second bank) [3]
    // BC010_REF - Mar Nav Bcn : Radar Reflector Present - See REF
    // BC010_TZP - Mar Nav Bcn : Topmark Shape - See TZP

    // BC034_MRT - Mar Radiobeacon : Maritime Radiobeacon Type
    // ['BC034_MRT','-999999',undefined,undefined], // No Information
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

    // BC034_MRT2 - Mar Radiobeacon : Maritime Radiobeacon Type [2]
    // BC034_MRT3 - Mar Radiobeacon : Maritime Radiobeacon Type [3]
    // BC040_CAA - Mar Nav Lt : Controlling Authority - See CAA

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
    ['BC101_SST','998','seamark:fog_signal:category','not_applicable'], // Not Applicable
    ['BC101_SST','999','seamark:fog_signal:category','other'], // Other

    // BCC - Bypass Condition
    // ['BCC','-999999',undefined,undefined], // No Information
    ['BCC','1','bypass','easy'], // Easy
    ['BCC','2','bypass','difficult'], // Difficult
    ['BCC','3','bypass','impossible'], // Impossible

    // BDS - Bidirectional
    // ['BDS','-999999',undefined,undefined], // No Information
    ['BDS','1000','bidirectional','no'],
    ['BDS','1001','bidirectional','yes'],

    // BET - Maritime Navigation Beacon Type
    // ['BET','-999999',undefined,undefined], // No Information
    ['BET','7','seamark:beacon','mooring'], // Mooring
    ['BET','11','seamark:beacon','landfall'], // Landfall
    ['BET','20','seamark:beacon','fairway'], // Fairway
    ['BET','21','seamark:beacon','mid-channel'], // Mid-Channel
    ['BET','22','seamark:beacon','bifurcation'], // Bifurcation
    ['BET','23','seamark:beacon','junction'], // Junction
    ['BET','24','seamark:beacon','wreck'], // Wreck
    ['BET','25','seamark:beacon','obstruction'], // Obstruction
    ['BET','26','seamark:beacon','telegraph_cable'], // Telegraph Cable
    ['BET','27','seamark:beacon','warping'], // Warping
    ['BET','28','seamark:beacon','quarantine'], // Quarantine
    ['BET','29','seamark:beacon','exercise_area'], // Exercise Area
    ['BET','30','seamark:beacon','explosive_anchorage'], // Explosive Anchorage
    ['BET','31','seamark:beacon','aeronautical_anchorage'], // Aeronautical Anchorage
    ['BET','32','seamark:beacon','compass_adjustment'], // Compass Adjustment
    ['BET','33','seamark:beacon','fish_trap'], // Fish Trap
    ['BET','34','seamark:beacon','spoil_ground'], // Spoil Ground
    ['BET','39','seamark:beacon','trot'], // Trot
    ['BET','44','seamark:beacon','firing_danger_area'], // Firing Danger Area
    ['BET','45','seamark:beacon','target'], // Target
    ['BET','46','seamark:beacon','marker_ship'], // Marker Ship
    ['BET','47','seamark:beacon','degaussing_range'], // Degaussing Range
    ['BET','48','seamark:beacon','barge'], // Barge
    ['BET','49','seamark:beacon','cable'], // Cable
    ['BET','50','seamark:beacon','outfall'], // Outfall
    ['BET','51','seamark:beacon','recording'], // Recording
    ['BET','52','seamark:beacon','recreation_zone'], // Recreation Zone
    ['BET','53','seamark:beacon','leading_line'], // Leading Line
    ['BET','54','seamark:beacon','measured_distance'], // Measured Distance
    ['BET','55','seamark:beacon','tss'], // TSS
    ['BET','56','seamark:beacon','anchoring_prohibited'], // Anchoring Prohibited
    ['BET','57','seamark:beacon','berthing_prohibited'], // Berthing Prohibited
    ['BET','58','seamark:beacon','overtaking_prohibited'], // Overtaking Prohibited
    ['BET','59','seamark:beacon','two-way_traffic_prohibited'], // Two-Way Traffic Prohibited
    ['BET','60','seamark:beacon','reduced_wake'], // Reduced Wake
    ['BET','61','seamark:beacon','speed_limit'], // Speed Limit
    ['BET','62','seamark:beacon','stop'], // Stop
    ['BET','63','seamark:beacon','sound_ship"s_siren'], // Sound Ship"s Siren
    ['BET','64','seamark:beacon','restricted_vertical_clearance'], // Restricted Vertical Clearance
    ['BET','65','seamark:beacon','maximum_vessel"s_draught'], // Maximum Vessel"s Draught
    ['BET','66','seamark:beacon','restricted_horizontal_clearance'], // Restricted Horizontal Clearance
    ['BET','67','seamark:beacon','strong_current_warning'], // Strong Current Warning
    ['BET','68','seamark:beacon','berthing_permitted'], // Berthing Permitted
    ['BET','69','seamark:beacon','overhead_power_cable'], // Overhead Power Cable
    ['BET','70','seamark:beacon','channel_edge_gradient'], // Channel Edge Gradient
    ['BET','71','seamark:beacon','telephone'], // Telephone
    ['BET','72','seamark:beacon','ferry_crossing'], // Ferry Crossing
    ['BET','73','seamark:beacon','pipeline'], // Pipeline
    ['BET','74','seamark:beacon','clearing_line'], // Clearing Line
    ['BET','75','seamark:beacon','refuge'], // Refuge
    ['BET','76','seamark:beacon','foul_ground'], // Foul Ground
    ['BET','77','seamark:beacon','yachting'], // Yachting
    ['BET','78','seamark:beacon','heliport'], // Heliport
    ['BET','79','seamark:beacon','gps_mark'], // GPS Mark
    ['BET','80','seamark:beacon','seaplane_landing'], // Seaplane Landing
    ['BET','81','seamark:beacon','diving'], // Diving
    ['BET','86','seamark:beacon','private'], // Private
    ['BET','87','seamark:beacon','swim'], // Swim
    ['BET','91','seamark:beacon','preferred_channel_to_port'], // Preferred Channel to Port
    ['BET','92','seamark:beacon','preferred_channel_to_starboard'], // Preferred Channel to Starboard
    ['BET','93','seamark:beacon','starboard-hand_edge'], // Starboard-hand Edge
    ['BET','94','seamark:beacon','port-hand_edge'], // Port-hand Edge
    ['BET','95','seamark:beacon','west_cardinal'], // West Cardinal
    ['BET','96','seamark:beacon','south_cardinal'], // South Cardinal
    ['BET','97','seamark:beacon','east_cardinal'], // East Cardinal
    ['BET','98','seamark:beacon','north_cardinal'], // North Cardinal
    ['BET','101','seamark:beacon','wavemeter'], // Wavemeter
    ['BET','102','seamark:beacon','entry_prohibited'], // Entry Prohibited
    ['BET','103','seamark:beacon','work_in_progress'], // Work In Progress
    ['BET','111','seamark:beacon','wellhead'], // Wellhead
    ['BET','113','seamark:beacon','marine_farm'], // Marine Farm
    ['BET','114','seamark:beacon','artificial_reef'], // Artificial Reef
    ['BET','115','seamark:beacon','special'], // Special
    ['BET','116','seamark:beacon','notice'], // Notice
    ['BET','117','seamark:beacon','general_warning'], // General Warning
    ['BET','118','seamark:beacon','anchorage'], // Anchorage
    ['BET','119','seamark:beacon','control'], // Control
    ['BET','122','seamark:beacon','safe_water'], // Safe Water
    ['BET','123','seamark:beacon','isolated_danger'], // Isolated Danger
    ['BET','998','seamark:beacon','not_applicable'], // Not Applicable
    ['BET','999','seamark:beacon','other'], // Other

    // BGT - Basin Gate Type
    // ['BGT','-999999',undefined,undefined], // No Information
    ['BGT','1','seamark:gate:category','caisson'], // Caisson
    ['BGT','2','seamark:gate:category','lock'], // Lock Gate
    ['BGT','3','seamark:gate:category','tide_lock'], // Tide Lock
    ['BGT','999','seamark:gate:category','other'], // Other

    // BH010_CAA - Aqueduct : Controlling Authority - See CAA

    // BH141_IBOA - Wtrbdy Bank : Bank Orientation (first bank)
    // ['BH141_IBOA','-999999',undefined,undefined], // No Information
    ['BH141_IBOA','1','waterway:bank1:orientation','right'], // Right
    ['BH141_IBOA','2','waterway:bank1:orientation','left'], // Left
    ['BH141_IBOA','3','waterway:bank1:orientation','indeterminate'], // Indeterminate
    ['BH141_IBOA','4','waterway:bank1:orientation','no_flow'], // No Flow
    ['BH141_IBOA','999','waterway:bank1:orientation','other'], // Other

    // BH141_IBOB - Wtrbdy Bank : Bank Orientation (second bank)
    // ['BH141_IBOB','-999999',undefined,undefined], // No Information
    ['BH141_IBOB','1','waterway:bank2:orientation','right'],
    ['BH141_IBOB','2','waterway:bank2:orientation','left'],
    ['BH141_IBOB','3','waterway:bank2:orientation','indeterminate'],
    ['BH141_IBOB','4','waterway:bank2:orientation','no_flow'],
    ['BH141_IBOB','999','waterway:bank2:orientation','other'],

    // BH141_SHDA - Wtrbdy Bank : Shoreline Delineated (first bank)
    // ['BH141_SHDA','-999999',undefined,undefined], // No Information
    ['BH141_SHDA','1000','waterway:bank1:shoreline_delineated','no'],
    ['BH141_SHDA','1001','waterway:bank1:shoreline_delineated','yes'],

    // BH141_SHDB - Wtrbdy Bank : Shoreline Delineated (second bank)
    // ['BH141_SHDB','-999999',undefined,undefined], // No Information
    ['BH141_SHDB','1000','waterway:bank2:shoreline_delineated','no'],
    ['BH141_SHDB','1001','waterway:bank2:shoreline_delineated','yes'],

    // BH141_SLTA - Wtrbdy Bank : Shoreline Type (first bank)
    // ['BH141_SLTA','-999999',undefined,undefined], // No Information
    ['BH141_SLTA','6','waterway:bank1:type','mangrove'], // Mangrove
    ['BH141_SLTA','8','waterway:bank1:type','marshy'], // Marshy
    ['BH141_SLTA','10','waterway:bank1:type','stony'], // Stony
    ['BH141_SLTA','11','waterway:bank1:type','building_rubble'], // Building Rubble
    ['BH141_SLTA','12','waterway:bank1:type','erosion_rubble'], // Erosion Rubble
    ['BH141_SLTA','13','waterway:bank1:type','sandy'], // Sandy
    ['BH141_SLTA','14','waterway:bank1:type','shingly'], // Shingly
    ['BH141_SLTA','16','waterway:bank1:type','coral'], // Coral
    ['BH141_SLTA','17','waterway:bank1:type','ice'], // Ice
    ['BH141_SLTA','18','waterway:bank1:type','mud'], // Mud
    ['BH141_SLTA','999','waterway:bank1:type','other'], // Other

    // BH141_SLTB - Wtrbdy Bank : Shoreline Type (second bank)
    // ['BH141_SLTB','-999999',undefined,undefined], // No Information
    ['BH141_SLTB','6','waterway:bank2:type','mangrove'], // Mangrove
    ['BH141_SLTB','8','waterway:bank2:type','marshy'], // Marshy
    ['BH141_SLTB','10','waterway:bank2:type','stony'], // Stony
    ['BH141_SLTB','11','waterway:bank2:type','building_rubble'], // Building Rubble
    ['BH141_SLTB','12','waterway:bank2:type','erosion_rubble'], // Erosion Rubble
    ['BH141_SLTB','13','waterway:bank2:type','sandy'], // Sandy
    ['BH141_SLTB','14','waterway:bank2:type','shingly'], // Shingly
    ['BH141_SLTB','16','waterway:bank2:type','coral'], // Coral
    ['BH141_SLTB','17','waterway:bank2:type','ice'], // Ice
    ['BH141_SLTB','18','waterway:bank2:type','mud'], // Mud
    ['BH141_SLTB','999','waterway:bank2:type','other'], // Other

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
    // ['BOT','-999999',undefined,undefined], // No Information
    ['BOT','4','bridge:movable','bascule'], // Bascule
    ['BOT','10','bridge:movable','swing'], // Swing-bridge
    ['BOT','11','bridge:movable','lift'], // Lift-bridge
    ['BOT','12','bridge:movable','retractable'], // Retractable
    ['BOT','14','bridge:movable','submersible'], // Submersible
    ['BOT','15','bridge:movable','drawbridge'], // Drawbridge
    ['BOT','16','bridge:movable','opening'], // Opening
    ['BOT','17','bridge:movable','no'], // Fixed
    ['BOT','999','bridge:movable','other'], // Other

    // BPWHAC - Wtrbdy Bank : Predom Wtrbdy Bank Height (first bank) <interval closure>
    ['BPWHAC','2','waterway:bank1:height:closure','open_interval'], // Open Interval
    ['BPWHAC','3','waterway:bank1:height:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['BPWHAC','4','waterway:bank1:height:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['BPWHAC','5','waterway:bank1:height:closure','closed_interval'], // Closed Interval
    ['BPWHAC','6','waterway:bank1:height:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['BPWHAC','7','waterway:bank1:height:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['BPWHAC','8','waterway:bank1:height:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['BPWHAC','9','waterway:bank1:height:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // BPWHBC - Wtrbdy Bank : Predom Wtrbdy Bank Height (second bank) <interval closure>
    ['BPWHBC','2','waterway:bank2:height:closure','open_interval'], // Open Interval
    ['BPWHBC','3','waterway:bank2:height:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['BPWHBC','4','waterway:bank2:height:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['BPWHBC','5','waterway:bank2:height:closure','closed_interval'], // Closed Interval
    ['BPWHBC','6','waterway:bank2:height:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['BPWHBC','7','waterway:bank2:height:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['BPWHBC','8','waterway:bank2:height:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['BPWHBC','9','waterway:bank2:height:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // BPWSAC - Wtrbdy Bank : Predom Wtrbdy Bank Slope (first bank) <interval closure>
    ['BPWSAC','2','waterway:bank1:slope:closure','open_interval'], // Open Interval
    ['BPWSAC','3','waterway:bank1:slope:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['BPWSAC','4','waterway:bank1:slope:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['BPWSAC','5','waterway:bank1:slope:closure','closed_interval'], // Closed Interval
    ['BPWSAC','6','waterway:bank1:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['BPWSAC','7','waterway:bank1:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['BPWSAC','8','waterway:bank1:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['BPWSAC','9','waterway:bank1:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // BPWSBC - Wtrbdy Bank : Predom Wtrbdy Bank Slope (second bank) <interval closure>
    ['BPWSBC','2','waterway:bank2:slope:closure','open_interval'], // Open Interval
    ['BPWSBC','3','waterway:bank2:slope:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['BPWSBC','4','waterway:bank2:slope:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['BPWSBC','5','waterway:bank2:slope:closure','closed_interval'], // Closed Interval
    ['BPWSBC','6','waterway:bank2:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['BPWSBC','7','waterway:bank2:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['BPWSBC','8','waterway:bank2:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['BPWSBC','9','waterway:bank2:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // BSC - Bridge Structure Type
    // ['BSC','-999999',undefined,undefined], // No Information
    ['BSC','1','bridge:structure','open_spandrel_arch'], // Open Spandrel Arch
    ['BSC','2','bridge:structure','cantilever'], // Cantilever
    ['BSC','3','bridge:structure','deck'], // Deck
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
    ['BSC','28','bridge:structure','viaduct'], // Viaduct
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
    ['BSU','13','building:superstructure:type','tower'], // Tower
    ['BSU','14','building:superstructure:type','cupola'], // Cupola
    ['BSU','15','building:superstructure:type','steeple'], // Steeple
    ['BSU','16','building:superstructure:type','turret'], // Turret
    ['BSU','17','building:superstructure:type','dome'], // Dome
    ['BSU','18','building:superstructure:type','solar_panels'], // Solar Panels
    ['BSU','999','building:superstructure:type','other'], // Other

    // BUS - Buoy Shape
    // ['BUS','-999999',undefined,undefined], // No Information
    ['BUS','1','seamark:buoy_shape','conical'], // Conical
    ['BUS','2','seamark:buoy_shape','can'], // Can
    ['BUS','3','seamark:buoy_shape','spherical'], // Spherical
    ['BUS','4','seamark:buoy_shape','pillar'], // Pillar
    ['BUS','5','seamark:buoy_shape','spar'], // Spar
    ['BUS','6','seamark:buoy_shape','barrel'], // Barrel
    ['BUS','7','seamark:buoy_shape','super-buoy'], // Super-buoy
    ['BUS','8','seamark:buoy_shape','ice_buoy'], // Ice Buoy
    ['BUS','9','seamark:buoy_shape','diamond'], // Diamond
    ['BUS','999','seamark:buoy_shape','other'], // Other

    // BUT - Buoy Type
    // ['BUT','-999999',undefined,undefined], // No Information
    ['BUT','3','seamark:buoy:type','isolated_danger'], // Isolated Danger
    ['BUT','4','seamark:buoy:type','large_automatic_navigation_buoy'], // Large Automatic Navigation Buoy
    ['BUT','6','seamark:type','light_float'], // Light Float
    ['BUT','7','seamark:buoy:type','mooring'], // Mooring
    ['BUT','10','seamark:buoy:type','ocean_data_acquisition_system_(odas)_buoy'], // Ocean Data Acquisition System (ODAS) Buoy
    ['BUT','11','seamark:buoy:type','landfall'], // Landfall
    ['BUT','15','seamark:buoy:type','special'], // Special
    ['BUT','18','seamark:buoy:type','safe_water'], // Safe Water
    ['BUT','19','seamark:buoy:type','anchorage'], // Anchorage
    ['BUT','20','seamark:buoy:type','fairway'], // Fairway
    ['BUT','21','seamark:buoy:type','mid-channel'], // Mid-Channel
    ['BUT','22','seamark:buoy:type','bifurcation'], // Bifurcation
    ['BUT','23','seamark:buoy:type','junction'], // Junction
    ['BUT','24','seamark:buoy:type','wreck'], // Wreck
    ['BUT','25','seamark:buoy:type','obstruction'], // Obstruction
    ['BUT','26','seamark:buoy:type','telegraph_cable'], // Telegraph Cable
    ['BUT','27','seamark:buoy:type','warping'], // Warping
    ['BUT','28','seamark:buoy:type','quarantine'], // Quarantine
    ['BUT','29','seamark:buoy:type','exercise_area'], // Exercise Area
    ['BUT','30','seamark:buoy:type','explosive_anchorage'], // Explosive Anchorage
    ['BUT','31','seamark:buoy:type','aeronautical_anchorage'], // Aeronautical Anchorage
    ['BUT','32','seamark:buoy:type','compass_adjustment'], // Compass Adjustment
    ['BUT','33','seamark:buoy:type','fish_trap'], // Fish Trap
    ['BUT','34','seamark:buoy:type','spoil_ground'], // Spoil Ground
    ['BUT','35','seamark:buoy:type','articulated_light'], // Articulated Light
    ['BUT','39','seamark:buoy:type','trot'], // Trot
    ['BUT','81','seamark:buoy:type','diving'], // Diving
    ['BUT','82','seamark:buoy:type','notice'], // Notice
    ['BUT','84','seamark:buoy:type','apex_float'], // APEX Float
    ['BUT','85','seamark:buoy:type','general_warning'], // General Warning
    ['BUT','86','seamark:buoy:type','private'], // Private
    ['BUT','87','seamark:buoy:type','swim'], // Swim
    ['BUT','88','seamark:buoy:type','control'], // Control
    ['BUT','89','seamark:buoy:type','entry_prohibited'], // Entry Prohibited
    ['BUT','91','seamark:buoy:type','preferred_channel_to_port'], // Preferred Channel to Port
    ['BUT','92','seamark:buoy:type','preferred_channel_to_starboard'], // Preferred Channel to Starboard
    ['BUT','93','seamark:buoy:type','starboard-hand_edge'], // Starboard-hand Edge
    ['BUT','94','seamark:buoy:type','port-hand_edge'], // Port-hand Edge
    ['BUT','95','seamark:buoy_cardinal:category','west'], // West Cardinal
    ['BUT','96','seamark:buoy_cardinal:category','south'], // South Cardinal
    ['BUT','97','seamark:buoy_cardinal:category','east'], // East Cardinal
    ['BUT','98','seamark:buoy_cardinal:category','north'], // North Cardinal
    ['BUT','99','seamark:buoy:type','installation'], // Installation
    ['BUT','100','seamark:buoy:type','accelerometer'], // Accelerometer
    ['BUT','101','seamark:buoy:type','wavemeter'], // Wavemeter
    ['BUT','102','seamark:buoy:type','navigation,_communication_and_control_buoy_(nccb)'], // Navigation, communication and control buoy (NCCB)
    ['BUT','103','seamark:buoy:type','ice_buoy'], // Ice Buoy
    ['BUT','104','seamark:buoy:type','firing_danger_area'], // Firing Danger Area
    ['BUT','105','seamark:buoy:type','target'], // Target
    ['BUT','106','seamark:buoy:type','marker_ship'], // Marker Ship
    ['BUT','107','seamark:buoy:type','degaussing_range'], // Degaussing Range
    ['BUT','108','seamark:buoy:type','barge'], // Barge
    ['BUT','109','seamark:buoy:type','cable'], // Cable
    ['BUT','110','seamark:buoy:type','outfall'], // Outfall
    ['BUT','111','seamark:buoy:type','recording'], // Recording
    ['BUT','112','seamark:buoy:type','recreation_zone'], // Recreation Zone
    ['BUT','113','seamark:buoy:type','leading_line'], // Leading Line
    ['BUT','114','seamark:buoy:type','measured_distance'], // Measured Distance
    ['BUT','115','seamark:buoy:type','tss'], // TSS
    ['BUT','116','seamark:buoy:type','anchoring_prohibited'], // Anchoring Prohibited
    ['BUT','117','seamark:buoy:type','berthing_prohibited'], // Berthing Prohibited
    ['BUT','118','seamark:buoy:type','overtaking_prohibited'], // Overtaking Prohibited
    ['BUT','119','seamark:buoy:type','two-way_traffic_prohibited'], // Two-Way Traffic Prohibited
    ['BUT','120','seamark:buoy:type','reduced_wake'], // Reduced Wake
    ['BUT','121','seamark:buoy:type','speed_limit'], // Speed Limit
    ['BUT','122','seamark:buoy:type','stop'], // Stop
    ['BUT','123','seamark:buoy:type','sound_ship"s_siren'], // Sound Ship"s Siren
    ['BUT','124','seamark:buoy:type','restricted_vertical_clearance'], // Restricted Vertical Clearance
    ['BUT','125','seamark:buoy:type','maximum_vessel"s_draft'], // Maximum Vessel"s Draft
    ['BUT','126','seamark:buoy:type','restricted_horizontal_clearance'], // Restricted Horizontal Clearance
    ['BUT','127','seamark:buoy:type','strong_current_warning'], // Strong Current Warning
    ['BUT','128','seamark:buoy:type','berthing_permitted'], // Berthing Permitted
    ['BUT','129','seamark:buoy:type','overhead_power_cable'], // Overhead Power Cable
    ['BUT','130','seamark:buoy:type','channel_edge_gradient'], // Channel Edge Gradient
    ['BUT','131','seamark:buoy:type','telephone'], // Telephone
    ['BUT','132','seamark:buoy:type','ferry_crossing'], // Ferry Crossing
    ['BUT','133','seamark:buoy:type','pipeline'], // Pipeline
    ['BUT','134','seamark:buoy:type','clearing_line'], // Clearing Line
    ['BUT','135','seamark:buoy:type','refuge'], // Refuge
    ['BUT','136','seamark:buoy:type','foul_ground'], // Foul Ground
    ['BUT','137','seamark:buoy:type','yachting'], // Yachting
    ['BUT','138','seamark:buoy:type','heliport'], // Heliport
    ['BUT','139','seamark:buoy:type','gps_mark'], // GPS Mark
    ['BUT','140','seamark:buoy:type','work_in_progress'], // Work In Progress
    ['BUT','141','seamark:buoy:type','artificial_reef'], // Artificial Reef
    ['BUT','142','seamark:buoy:type','marine_farm'], // Marine Farm
    ['BUT','143','seamark:buoy:type','seaplane_landing'], // Seaplane Landing
    ['BUT','144','seamark:buoy:type','wellhead'], // Wellhead
    ['BUT','145','seamark:buoy:type','water_column_sensor'], // Water Column Sensor
    ['BUT','149','seamark:buoy:type','emergency_wreck'], // Emergency Wreck
    ['BUT','150','seamark:buoy:type','mark_of_unknown_intent'], // Mark of Unknown Intent
    ['BUT','999','seamark:buoy:type','other'], // Other

    // BWVCAC - Wtrbdy Bank : Wtrbdy Bank Veg Cover (first bank) <interval closure>
    ['BWVCAC','2','waterway:bank1:cover:closure','open_interval'], // Open Interval
    ['BWVCAC','3','waterway:bank1:cover:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['BWVCAC','4','waterway:bank1:cover:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['BWVCAC','5','waterway:bank1:cover:closure','closed_interval'], // Closed Interval
    ['BWVCAC','6','waterway:bank1:cover:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['BWVCAC','7','waterway:bank1:cover:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['BWVCAC','8','waterway:bank1:cover:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['BWVCAC','9','waterway:bank1:cover:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // BWVCBC - Wtrbdy Bank : Wtrbdy Bank Veg Cover (second bank) <interval closure>
    ['BWVCBC','2','waterway:bank2:cover:closure','open_interval'], // Open Interval
    ['BWVCBC','3','waterway:bank2:cover:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['BWVCBC','4','waterway:bank2:cover:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['BWVCBC','5','waterway:bank2:cover:closure','closed_interval'], // Closed Interval
    ['BWVCBC','6','waterway:bank2:cover:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['BWVCBC','7','waterway:bank2:cover:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['BWVCBC','8','waterway:bank2:cover:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['BWVCBC','9','waterway:bank2:cover:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // CAA - Controlling Authority
    // ['CAA','-999999',undefined,undefined], // No Information
    ['CAA','1','controlling_authority','national'], // National
    ['CAA','2','controlling_authority','state'], // State
    ['CAA','3','controlling_authority','private'], // Private
    ['CAA','4','controlling_authority','tribal'], // Tribal
    ['CAA','5','controlling_authority','military'], // Military
    ['CAA','6','controlling_authority','municipal'], // Municipal
    ['CAA','7','controlling_authority','joint_military_and_civilian'], // Joint Military and Civilian
    ['CAA','8','controlling_authority','international'], // International
    ['CAA','11','controlling_authority','province'], // Province
    ['CAA','13','controlling_authority','regional'], // Regional
    ['CAA','16','controlling_authority','civilian'], // Civilian
    ['CAA','17','controlling_authority','public'], // Public
    ['CAA','999','controlling_authority','other'], // Other

    // CAB - Cable Type
    // ['CAB','-999999',undefined,undefined], // No Information
    ['CAB','2','cable:type','power'], // Power Distribution Line
    ['CAB','3','cable:type','telephone'], // Telephone
    ['CAB','4','cable:type','telegraph'], // Telegraph
    ['CAB','6','cable:type','transmission'], // Power Transmission Line
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
    // ['CDA','-999999',undefined,undefined], // No Information
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

    // CFT - Boundary Determination Method
    // ['CFT','-999999',undefined,undefined], // No Information
    ['CFT','3','seamark:boundary:determination','cadastral'], // Cadastral
    ['CFT','6','seamark:boundary:determination','economic'], // Economic
    ['CFT','12','seamark:boundary:determination','left_bank'], // Left Bank
    ['CFT','13','seamark:boundary:determination','right_bank'], // Right Bank
    ['CFT','14','seamark:boundary:determination','shoreline'], // Shoreline
    ['CFT','16','seamark:boundary:determination','meridian'], // Meridian
    ['CFT','17','seamark:boundary:determination','median'], // Median
    ['CFT','18','seamark:boundary:determination','parallel'], // Parallel
    ['CFT','19','seamark:boundary:determination','straight_segmented'], // Straight Segmented
    ['CFT','20','seamark:boundary:determination','straight'], // Straight
    ['CFT','21','seamark:boundary:determination','geodesic'], // Geodesic
    ['CFT','22','seamark:boundary:determination','great_circle'], // Great Circle
    ['CFT','23','seamark:boundary:determination','arc'], // Arc
    ['CFT','24','seamark:boundary:determination','normal_baseline'], // Normal Baseline
    ['CFT','25','seamark:boundary:determination','straight_baseline'], // Straight Baseline
    ['CFT','27','seamark:boundary:determination','normal_continental_shelf'], // Normal Continental Shelf
    ['CFT','28','seamark:boundary:determination','extended_continental_shelf'], // Extended Continental Shelf
    ['CFT','29','seamark:boundary:determination','main_channel'], // Main Channel
    ['CFT','30','seamark:boundary:determination','archipelagic_baseline'], // Archipelagic Baseline
    ['CFT','999','seamark:boundary:determination','other'], // Other

    // CHA - Navigation Light Characteristic
    // ['CHA','-999999',undefined,undefined], // No Information
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

    // CIW - Closed in Winter
    // ['CIW','-999999',undefined,undefined], // No Information
    ['CIW','1000','closed_in_winter','no'],
    ['CIW','1001','closed_in_winter','yes'],

    // CMS - Commissioned Status
    // ['CMS','-999999',undefined,undefined], // No Information
    ['CMS','1','commissioned','commissioned_and_operational'], // Commissioned and Operational
    ['CMS','2','commissioned','commissioned_and_on_test'], // Commissioned and on Test
    ['CMS','3','commissioned','commissioned_but_out_of_service'], // Commissioned but Out of Service
    ['CMS','4','commissioned','operational_but_not_commissioned'], // Operational but Not Commissioned
    ['CMS','5','commissioned','on_test_but_not_commissioned'], // On Test but Not Commissioned
    ['CMS','6','commissioned','not_commissioned_and_out_of_service'], // Not Commissioned and Out of Service

    // CNF - Confined
    // ['CNF','-999999',undefined,undefined], // No Information
    ['CNF','1000','aquifer:confined','no'],
    ['CNF','1001','aquifer:confined','yes'],

    // COA - Conspicuous Air Category
    // ['COA','-999999',undefined,undefined], // No Information
    ['COA','2','conspicuous:from_air','visual'], // Visual
    ['COA','4','conspicuous:from_air','no'], // Not Conspicuous

    // COC - Conspicuous Sea Category
    // ['COC','-999999',undefined,undefined], // No Information
    ['COC','1','conspicuous:from_sea','visual_and_radar'], // Visual and Radar
    ['COC','2','conspicuous:from_sea','visual'], // Visual
    ['COC','3','conspicuous:from_sea','radar'], // Radar
    ['COC','4','conspicuous:from_sea','no'], // Not Conspicuous

    // COG - Conspicuous Ground Category
    // ['COG','-999999',undefined,undefined], // No Information
    ['COG','2','conspicuous:from_ground','visual'], // Visual
    ['COG','4','conspicuous:from_ground','no'], // Not Conspicuous

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
    ['COS','1','operational_status','continuous'], // Continuous
    ['COS','2','operational_status','intermittent'], // Intermittent
    ['COS','3','operational_status','scheduled'], // Scheduled
    ['COS','4','operational_status','non-operational'], // Non-operational
    ['COS','6','operational_status','planned'], // Planned
    ['COS','8','operational_status','not_in_operation'], // Not in Operation
    ['COS','9','operational_status','temporarily_non-operational'], // Temporarily Non-operational
    ['COS','13','operational_status','operational'], // Operational
    ['COS','14','operational_status','partial'], // Partially Operational

    // CPA - Survey Control Point Type
    // ['CPA','-999999',undefined,undefined], // No Information
    ['CPA','1','survey_point:control','benchmark'], // Benchmark
    ['CPA','2','survey_point:control','horizontal'], // Horizontal Position
    ['CPA','3','survey_point:control','horizontal_and_benchmark'], // Horizontal Position and Benchmark
    ['CPA','5','survey_point:control','vertical'], // Vertical Position

    // CPS - Cell Partition Scheme
    // ['CPS','-999999',undefined,undefined], // No Information
    ['CPS','1','source:cell_partition_scheme','5_arc_degree'], // Fixed Cells, 5 Arc Degree
    ['CPS','2','source:cell_partition_scheme','1_arc_degree'], // Fixed Cells, 1 Arc Degree
    ['CPS','3','source:cell_partition_scheme','0.5_arc_degree'], // Fixed Cells, 0.5 Arc Degree
    ['CPS','4','source:cell_partition_scheme','0.25_arc_degree'], // Fixed Cells, 0.25 Arc Degree
    ['CPS','5','source:cell_partition_scheme','variable'], // Variable Cells
    ['CPS','998','source:cell_partition_scheme','not_applicable'], // Not Applicable
    ['CPS','999','source:cell_partition_scheme','other'], // Other

    // CPT - Colour Pattern
    // ['CPT','-999999',undefined,undefined], // No Information
    ['CPT','1','seamark:light_support:colour_pattern','horizontal_stripes'], // Horizontal Stripes
    ['CPT','2','seamark:light_support:colour_pattern','vertical_stripes'], // Vertical Stripes
    ['CPT','3','seamark:light_support:colour_pattern','diagonal_stripes'], // Diagonal Stripes
    ['CPT','4','seamark:light_support:colour_pattern','squared'], // Squared
    ['CPT','5','seamark:light_support:colour_pattern','stripes'], // Stripes
    ['CPT','6','seamark:light_support:colour_pattern','border_stripe'], // Border Stripe
    ['CPT','7','seamark:light_support:colour_pattern','single_colour'], // Single Colour

    // CRA - Crane Type
    // ['CRA','-999999',undefined,undefined], // No Information
    ['CRA','2','crane:type','bridge'], // Bridge Crane
    ['CRA','3','crane:type','rotating'], // Rotating Crane
    ['CRA','7','crane:type','tower'], // Tower Crane
    ['CRA','8','crane:type','shear-legs'], // Shear-Legs
    ['CRA','9','crane:type','a-frame'], // A-frame Crane
    ['CRA','10','crane:type','goliath'], // Goliath Crane
    ['CRA','99','crane:type','container'], // Container Crane
    ['CRA','999','crane:type','other'], // Other

    // CRM - Crane Mobility Type
    // ['CRM','-999999',undefined,undefined], // No Information
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

    // CSC2 - Intersection Control Type [2]
    // ['CSC2','-999999',undefined,undefined], // No Information
    ['CSC2','2','intersection_control:2','traffic_signals'], // Signal Device
    ['CSC2','3','intersection_control:2','stop_sign'], // Stop Sign
    ['CSC2','4','intersection_control:2','uncontrolled'], // No Control
    ['CSC2','5','intersection_control:2','ground_marking'], // Ground Marking
    ['CSC2','6','intersection_control:2','give_way'], // Yield Sign
    ['CSC2','7','intersection_control:2','priority_sign'], // Priority Sign
    ['CSC2','999','intersection_control:2','other'], // Other

    // CSC3 - Intersection Control Type [3]
    // ['CSC3','-999999',undefined,undefined], // No Information
    ['CSC3','2','intersection_control:3','traffic_signals'], // Signal Device
    ['CSC3','3','intersection_control:3','stop_sign'], // Stop Sign
    ['CSC3','4','intersection_control:3','uncontrolled'], // No Control
    ['CSC3','5','intersection_control:3','ground_marking'], // Ground Marking
    ['CSC3','6','intersection_control:3','give_way'], // Yield Sign
    ['CSC3','7','intersection_control:3','priority_sign'], // Priority Sign
    ['CSC3','999','intersection_control:3','other'], // Other

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

    // CSS - Cross-sectional Shape
    // ['CSS','-999999',undefined,undefined], // No Information
    ['CSS','1','seamark:bouy:cross_section','irregular'], // Irregular
    ['CSS','4','seamark:bouy:cross_section','circular'], // Circular
    ['CSS','5','seamark:bouy:cross_section','square'], // Square
    ['CSS','6','seamark:bouy:cross_section','triangular'], // Triangular
    ['CSS','999','seamark:bouy:cross_section','other'], // Other

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
    ['CUL','1','cultural_context','national_identity'], // National Identity
    ['CUL','2','cultural_context','ethnic_group'], // Ethnic Group
    ['CUL','3','cultural_context','tribe'], // Tribe
    ['CUL','4','cultural_context','clan'], // Clan
    ['CUL','5','cultural_context','language'], // Language
    ['CUL','6','cultural_context','religion'], // Religion
    ['CUL','999','cultural_context','other'], // Other

    // CUR - Current Type Category
    // ['CUR','-999999',undefined,undefined], // No Information
    ['CUR','1','current','ebb_stream'], // Ebb Stream
    ['CUR','2','current','flood_stream'], // Flood Stream
    ['CUR','4','current','river_flow'], // River Flow
    ['CUR','5','current','ocean_current'], // Ocean Current
    ['CUR','6','current','rip'], // Rip
    ['CUR','7','current','longshore'], // Longshore
    ['CUR','8','current','underwater_river_flow'], // Underwater River Flow
    ['CUR','9','current','tidal_flow'], // Tidal Flow
    ['CUR','999','current','other'], // Other

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

    // DFU2 - Dam Function [2]
    // DFU3 - Dam Function [3]

    // DGC - Transportation Block Type
    // ['DGC','-999999',undefined,undefined], // No Information
    ['DGC','1','block:type','drop_gate'], // Drop Gate
    ['DGC','2','block:type','rolling_block'], // Rolling Block
    ['DGC','999','block:type','other'], // Other

    // DINF - Water Disinfection Method
    // ['DINF','-999999',undefined,undefined], // No Information
    ['DINF','1','disinfection_method','chlorination'], // Chlorination
    ['DINF','2','disinfection_method','ozone'], // Ozone
    ['DINF','3','disinfection_method','uv_light'], // UV Light
    ['DINF','4','disinfection_method','iodine'], // Iodine
    ['DINF','5','disinfection_method','bromine'], // Bromine
    ['DINF','6','disinfection_method','bases_-_sodium_hydroxide_or_lime'], // Bases - Sodium hydroxide or lime
    ['DINF','7','disinfection_method','ultrasonic'], // Ultrasonic
    ['DINF','8','disinfection_method','heat'], // Heat
    ['DINF','998','disinfection_method','not_applicable'], // Not Applicable
    ['DINF','999','disinfection_method','other'], // Other

    // DIR - Directivity
    // ['DIR','-999999',undefined,undefined], // No Information
    ['DIR','1','directivity','unidirectional'], // Unidirectional
    ['DIR','2','directivity','bidirectional'], // Bidirectional
    ['DIR','3','directivity','omnidirectional'], // Omnidirectional

    // DIT - Ditch Function
    // ['DIT','-999999',undefined,undefined], // No Information
    ['DIT','1','ditch:type','irrigation'], // Irrigation
    ['DIT','2','ditch:type','drainage'], // Drainage
    ['DIT','3','ditch:type','sewage'], // Sewage
    ['DIT','999','ditch:type','other'], // Other

    // DKC - Bathymetric Measurement Quality Category - See ZHDP_DKC
    // DKC2 - Bathymetric Measurement Quality Category [2] - See ZHDP_DKC2
    // DKC3 - Bathymetric Measurement Quality Category [3] - See ZHDP_DKC3

    // DKS - Daymark Shape
    // ['DKS','-999999',undefined,undefined], // No Information
    ['DKS','1','seamark:daymark:shape','rectangle'], // Rectangle
    ['DKS','2','seamark:daymark:shape','square'], // Square
    ['DKS','3','seamark:daymark:shape','triangle'], // Triangle
    ['DKS','4','seamark:daymark:shape','cage'], // Cage
    ['DKS','5','seamark:daymark:shape','diamond'], // Diamond
    ['DKS','6','seamark:daymark:shape','circle'], // Circular
    ['DKS','7','seamark:daymark:shape','trapezium'], // Trapezoidal

    // DMBC - Undergrowth Density <interval closure>
    ['DMBC','2','undergrowth:density:closure','open_interval'], // Open Interval
    ['DMBC','3','undergrowth:density:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['DMBC','4','undergrowth:density:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['DMBC','5','undergrowth:density:closure','closed_interval'], // Closed Interval
    ['DMBC','6','undergrowth:density:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['DMBC','7','undergrowth:density:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['DMBC','8','undergrowth:density:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['DMBC','9','undergrowth:density:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // DMC - Boundary Demarcated
    // ['DMC','-999999',undefined,undefined], // No Information
    ['DMC','1000','boundary:demarcated','no'],
    ['DMC','1001','boundary:demarcated','yes'],

    // DMD - Dammed
    // ['DMD','-999999',undefined,undefined], // No Information
    ['DMD','1000','dammed','no'], // Just to get rid of errors
    ['DMD','1001','dammed','yes'], // yes

    // DNE - Human Development Index
    // ['DNE','-999999',undefined,undefined], // No Information
    ['DNE','1','human_development_index','highest_development'], // Highest Development
    ['DNE','2','human_development_index','high_development'], // High Development
    ['DNE','3','human_development_index','rapid_development'], // Rapid Development
    ['DNE','4','human_development_index','developing'], // Developing
    ['DNE','5','human_development_index','low_development'], // Low Development

    // DPAC - Aquifer Depth <interval closure>
    ['DPAC','2','aquifer:depth:closure','open_interval'], // Open Interval
    ['DPAC','3','aquifer:depth:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['DPAC','4','aquifer:depth:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['DPAC','5','aquifer:depth:closure','closed_interval'], // Closed Interval
    ['DPAC','6','aquifer:depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['DPAC','7','aquifer:depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['DPAC','8','aquifer:depth:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['DPAC','9','aquifer:depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // DPG - Dumping Ground Type
    // ['DPG','-999999',undefined,undefined], // No Information
    ['DPG','1','seamark:dumping_ground:category','hazardous_material'], // Hazardous Material
    ['DPG','2','seamark:dumping_ground:category','chemical'], // Chemical Waste
    ['DPG','3','seamark:dumping_ground:category','nuclear'], // Nuclear Waste
    ['DPG','4','seamark:dumping_ground:category','explosives'], // Explosives
    ['DPG','5','seamark:dumping_ground:category','spoil'], // Spoil
    ['DPG','6','seamark:dumping_ground:category','vessel'], // Vessels
    ['DPG','999','seamark:dumping_ground:category','other'], // Other

    // DPG2 - Dumping Ground Type [2]
    // DPG3 - Dumping Ground Type [3]

    // DSD - Dam Structural Design Type
    // ['DSD','-999999',undefined,undefined], // No Information
    ['DSD','101','dam:structural_design','arch'], // Arch
    ['DSD','102','dam:structural_design','buttress'], // Buttress
    ['DSD','103','dam:structural_design','gravity'], // Gravity
    ['DSD','104','dam:structural_design','multi-arch'], // Multi-Arch
    ['DSD','105','dam:structural_design','earthen'], // Earthen
    ['DSD','106','dam:structural_design','rockfill'], // Rockfill
    ['DSD','107','dam:structural_design','asphalt-concrete_core'], // Asphalt-concrete core
    ['DSD','999','dam:structural_design','other'], // Other

    // DSP - Boundary Dispute Type
    // ['DSP','-999999',undefined,undefined], // No Information
    ['DSP','1','boundary:dispute_type','in_dispute'], // In Dispute
    ['DSP','2','boundary:dispute_type','undisputed'], // Undisputed
    ['DSP','3','boundary:dispute_type','de_facto'], // De Facto
    ['DSP','4','boundary:dispute_type','de_jure'], // De Jure

    // DTN - Dangerous to Navigation
    // ['DTN','-999999',undefined,undefined], // No Information
    ['DTN','1000','dangerous_to_navigation','no'],
    ['DTN','1001','dangerous_to_navigation','yes'],

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
    // ['ETS','-999999',undefined,undefined], // No Information
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
    ['EXP','999','expansion:type','other'], // Otherr

    // FA003_BAL - Administrative Subdivision : BGN Administrative Level
    // ['FA003_BAL','-999999',undefined,undefined], // No Information
    ['FA003_BAL','1','administrative_subdivision','first-order'], // First-order
    ['FA003_BAL','2','administrative_subdivision','second-order'], // Second-order
    ['FA003_BAL','3','administrative_subdivision','third-order'], // Third-order
    ['FA003_BAL','4','administrative_subdivision','fourth-order'], // Fourth-order
    ['FA003_BAL','5','administrative_subdivision','undifferentiated'], // Undifferentiated
    ['FA003_BAL','999','administrative_subdivision','other'], // Other

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

    // FEF - Fence Function
    // ['FEF','-999999',undefined,undefined], // No Information
    ['FEF','1','fence:use','animal_containment'], // Animal Containment
    ['FEF','2','fence:use','erosion-control'], // Erosion-control
    ['FEF','3','fence:use','exclusion'], // Exclusion
    ['FEF','4','fence:use','security'], // Security
    ['FEF','5','fence:use','wind'], // Wind
    ['FEF','999','fence:use','other'], // Other

    // FER - Ferry Crossing Type
    // ['FER','-999999',undefined,undefined], // No Information
    ['FER','1','ferry:type','cable'], // Cable
    ['FER','2','ferry:type','free-moving'], // Free-moving
    ['FER','3','ferry:type','ice'], // Ice
    ['FER','4','ferry:type','high-speed_crossing'], // High-speed Crossing
    ['FER','999','ferry:type','other'], // Other

    // FFN - Feature Function
    // ['FFN','-999999',undefined,undefined], // No Information
    ['FFN','2','use','agriculture'], // Agriculture
    ['FFN','3','use','growing_crops'], // Growing of Crops
    ['FFN','9','use','raising_animals'], // Raising of Animals
    ['FFN','14','use','grazing'], // Grazing
    ['FFN','15','use','mixed_farming'], // Mixed Farming
    ['FFN','16','camp:type','migrant_labour'], // Migrant Labour
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

    // FFN2 - Feature Function
    // FFN3 - Feature Function

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

    // FLT - Floodlit
    // ['FLT','-999999',undefined,undefined], // No Information
    ['FLT','1000','floodlit','no'],
    ['FLT','1001','floodlit','yes'],

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

    // FRZ - Seasonally Frozen
    // ['FRZ','-999999',undefined,undefined], // No Information
    ['FRZ','1000','seasonally_frozen','no'],
    ['FRZ','1001','seasonally_frozen','yes'],

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

    // GDBV - Geodatabase Schema Version
    // ['GDBV','-999999',undefined,undefined], // No Information
    ['GDBV','1','source:schema','tds_4_0'], // TDS_4_0
    ['GDBV','2','source:schema','tds_6_0'], // TDS_6_0
    ['GDBV','3','source:schema','tds_6_1'],    // TDS_6_1
    ['GDBV','4','source:schema','ggdm_2_0'], // GGDM_2_0
    ['GDBV','5','source:schema','ggdm_2_1'], // GGDM_2_1
    ['GDBV','6','source:schema','ggdm_2_2'], // GGDM_2_2
    ['GDBV','7','source:schema','ggdm_3_0'], // GGDM_3_0
    ['GDBV','999','source:schema','other'], // Other

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

    // GFY - Geologic Fault Type
    // ['GFY','-999999',undefined,undefined], // No Information
    ['GFY','1','geological:fault','normal'], // Normal
    ['GFY','2','geological:fault','thrust'], // Thrust
    ['GFY','3','geological:fault','reverse'], // Reverse
    ['GFY','4','geological:fault','strike-slip'], // Strike-slip
    ['GFY','5','geological:fault','extension'], // Extension
    ['GFY','6','geological:fault','transpression'], // Transpression
    ['GFY','7','geological:fault','restraining_bend'], // Restraining Bend
    ['GFY','8','geological:fault','transextension'], // Transextension
    ['GFY','9','geological:fault','releasing_bend'], // Releasing Bend
    ['GFY','10','geological:fault','oblique'], // Oblique
    ['GFY','11','geological:fault','decollement'], // Decollement
    ['GFY','12','geological:fault','shear_zone'], // Shear Zone
    ['GFY','13','geological:fault','transverse'], // Transverse
    ['GFY','999','geological:fault','other'], // Other

    // GOT - Geothermal Outlet Type
    // ['GOT','-999999',undefined,undefined], // No Information
    ['GOT','1','natural','fissure'], // Fissure
    ['GOT','2','natural','fumarole'], // Fumarole
    ['GOT','3','natural','hot_spring'], // Hot Spring
    ['GOT','4','natural','sulphur_spring'], // Sulphur Spring
    ['GOT','5','natural','geyser'], // Geyser
    ['GOT','999','natural','other_geothermal_outlet'], // Other

    // GRD - Guarded
    // ['GRD','-999999',undefined,undefined], // No Information
    ['GRD','1000','guarded','no'],
    ['GRD','1001','guarded','yes'],

    // GSGCHC - Runway Direction : Surface Slope (high end) <interval closure>
    ['GSGCHC','2','aeroway:runway_highend:slope:closure','open_interval'], // Open Interval
    ['GSGCHC','3','aeroway:runway_highend:slope:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['GSGCHC','4','aeroway:runway_highend:slope:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['GSGCHC','5','aeroway:runway_highend:slope:closure','closed_interval'], // Closed Interval
    ['GSGCHC','6','aeroway:runway_highend:slope:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['GSGCHC','7','aeroway:runway_highend:slope:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['GSGCHC','8','aeroway:runway_highend:slope:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['GSGCHC','9','aeroway:runway_highend:slope:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // GSGCLC - Runway Direction : Surface Slope (low end) <interval closure>
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

    // GWS - Groundwater Potential Score
    // ['GWS','-999999',undefined,undefined], // No Information
    ['GWS','1','groundwater_potential','unsuitable'], // Unsuitable
    ['GWS','2','groundwater_potential','poor'], // Poor
    ['GWS','3','groundwater_potential','moderate'], // Moderate
    ['GWS','4','groundwater_potential','good'], // Good

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

    // HHD - Homogenous Habitation Distribution
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
    ['HTP','999','hangar:type','other'], // Other

    // HZD - Geodetic Datum
    // ['HZD','-999999',undefined,undefined], // No Information
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
    ['HZD','37','source:datum:geodetic','average_terrestrial_system_1977,_new_brunswick'], // Average Terrestrial System 1977, New Brunswick
    ['HZD','38','source:datum:geodetic','australian_geod._1966_(australia_and_tasmania_island)'], // Australian Geod. 1966 (Australia and Tasmania Island)
    ['HZD','39','source:datum:geodetic','australian_geod._1984_(australia_and_tasmania_island)'], // Australian Geod. 1984 (Australia and Tasmania Island)
    ['HZD','40','source:datum:geodetic','djakarta_(batavia)_(sumatra_island,_indonesia)'], // Djakarta (Batavia) (Sumatra Island, Indonesia)
    ['HZD','41','source:datum:geodetic','djakarta_(batavia)_(sumatra_island,_indonesia)_with_zero_meridian_djakarta'], // Djakarta (Batavia) (Sumatra Island, Indonesia) with Zero Meridian Djakarta
    ['HZD','42','source:datum:geodetic','bekaa_base_south_end_(lebanon)'], // Bekaa Base South End (Lebanon)
    ['HZD','43','source:datum:geodetic','belgium_1950_system_(lommel_signal,_belgium)'], // Belgium 1950 System (Lommel Signal, Belgium)
    ['HZD','44','source:datum:geodetic','bermuda_1957_(bermuda_islands)'], // Bermuda 1957 (Bermuda Islands)
    ['HZD','45','source:datum:geodetic','bissau_(guinea-bissau)'], // Bissau (Guinea-Bissau)
    ['HZD','46','source:datum:geodetic','modified_bjz54_(china)'], // Modified BJZ54 (China)
    ['HZD','47','source:datum:geodetic','bjz54_(a954_beijing_coordinates)_(china)'], // BJZ54 (A954 Beijing Coordinates) (China)
    ['HZD','48','source:datum:geodetic','bogota_observatory_(colombia)'], // Bogota Observatory (Colombia)
    ['HZD','49','source:datum:geodetic','bogota_observatory_(colombia)_with_zero_meridian_bogota'], // Bogota Observatory (Colombia) with Zero Meridian Bogota
    ['HZD','50','source:datum:geodetic','bern_1898_(switzerland)'], // Bern 1898 (Switzerland)
    ['HZD','51','source:datum:geodetic','bern_1898_(switzerland)_with_zero_meridian_bern'], // Bern 1898 (Switzerland) with Zero Meridian Bern
    ['HZD','52','source:datum:geodetic','bukit_rimpah_(bangka_and_belitung_islands,_indonesia)'], // Bukit Rimpah (Bangka and Belitung Islands, Indonesia)
    ['HZD','53','source:datum:geodetic','cape_canaveral_(mean_value)'], // Cape Canaveral (mean value)
    ['HZD','54','source:datum:geodetic','campo_inchauspe_(argentina)'], // Campo Inchauspe (Argentina)
    ['HZD','55','source:datum:geodetic','camacupa_base_sw_end_(campo_de_aviacao,_angola)'], // Camacupa Base SW End (Campo De Aviacao, Angola)
    ['HZD','56','source:datum:geodetic','canton_astro_1966_(phoenix_islands)'], // Canton Astro 1966 (Phoenix Islands)
    ['HZD','57','source:datum:geodetic','cape_(south_africa)'], // Cape (South Africa)
    ['HZD','58','source:datum:geodetic','camp_area_astro_(camp_mcmurdo_area,_antarctica)'], // Camp Area Astro (Camp McMurdo Area, Antarctica)
    ['HZD','59','source:datum:geodetic','s-jtsk'], // S-JTSK
    ['HZD','60','source:datum:geodetic','carthage_(tunisia)'], // Carthage (Tunisia)
    ['HZD','61','source:datum:geodetic','compensation_geodetique_du_quebec_1977'], // Compensation Geodetique du Quebec 1977
    ['HZD','62','source:datum:geodetic','chatham_1971_(chatham_island,_new_zealand)'], // Chatham 1971 (Chatham Island, New Zealand)
    ['HZD','63','source:datum:geodetic','chua_astro_(paraguay)'], // Chua Astro (Paraguay)
    ['HZD','64','source:datum:geodetic','corrego_alegre_(brazil)'], // Corrego Alegre (Brazil)
    ['HZD','65','source:datum:geodetic','conakry_pyramid_of_the_service_geographique_(guinea)'], // Conakry Pyramid of the Service Geographique (Guinea)
    ['HZD','66','source:datum:geodetic','guyana_csg67'], // Guyana CSG67
    ['HZD','67','source:datum:geodetic','dabola_(guinea)'], // Dabola (Guinea)
    ['HZD','68','source:datum:geodetic','dcs-3_lighthouse,_saint_lucia,_lesser_antilles'], // DCS-3 Lighthouse, Saint Lucia, Lesser Antilles
    ['HZD','69','source:datum:geodetic','deception_island,_antarctica'], // Deception Island, Antarctica
    ['HZD','70','source:datum:geodetic','gux_1_astro_(guadacanal_island)'], // GUX 1 Astro (Guadacanal Island)
    ['HZD','71','source:datum:geodetic','dominica_astro_m-12,_dominica,_lesser_antilles'], // Dominica Astro M-12, Dominica, Lesser Antilles
    ['HZD','72','source:datum:geodetic','easter_island_1967_(easter_island)'], // Easter Island 1967 (Easter Island)
    ['HZD','73','source:datum:geodetic','wake-eniwetok_1960_(marshall_islands)'], // Wake-Eniwetok 1960 (Marshall Islands)
    ['HZD','75','source:datum:geodetic','european_1950_(western_europe)'], // European 1950 (Western Europe)
    ['HZD','76','source:datum:geodetic','european_1950_(greece)'], // European 1950 (Greece)
    ['HZD','77','source:datum:geodetic','european_1950_(norway_and_finland)'], // European 1950 (Norway and Finland)
    ['HZD','78','source:datum:geodetic','european_1950_(portugal_and_spain)'], // European 1950 (Portugal and Spain)
    ['HZD','79','source:datum:geodetic','european_1950_(cyprus)'], // European 1950 (Cyprus)
    ['HZD','80','source:datum:geodetic','european_1950_(egypt)'], // European 1950 (Egypt)
    ['HZD','81','source:datum:geodetic','european_1950_(england,_channel_islands,_scotland,_and_shetland_islands)'], // European 1950 (England, Channel Islands, Scotland, and Shetland Islands)
    ['HZD','82','source:datum:geodetic','european_1950_(iran)'], // European 1950 (Iran)
    ['HZD','83','source:datum:geodetic','european_1950_(sardinia)'], // European 1950 (Sardinia)
    ['HZD','84','source:datum:geodetic','european_1950_(sicily)'], // European 1950 (Sicily)
    ['HZD','85','source:datum:geodetic','european_1950_(british_isles)'], // European 1950 (British Isles)
    ['HZD','86','source:datum:geodetic','european_1950_(malta)'], // European 1950 (Malta)
    ['HZD','87','source:datum:geodetic','european_1950_(mean_value)'], // European 1950 (mean value)
    ['HZD','88','source:datum:geodetic','european_1950_(iraq,_israel,_jordan,_kuwait,_lebanon,_saudi_arabia,_and_syria)'], // European 1950 (Iraq, Israel, Jordan, Kuwait, Lebanon, Saudi Arabia, and Syria)
    ['HZD','89','source:datum:geodetic','european_1950_(tunisia)'], // European 1950 (Tunisia)
    ['HZD','90','source:datum:geodetic','european_1979_(mean_value)'], // European 1979 (mean value)
    ['HZD','91','source:datum:geodetic','european_terrestrial_reference_system_1989_(etrs89)'], // European Terrestrial Reference System 1989 (ETRS89)
    ['HZD','92','source:datum:geodetic','oman_(oman)'], // Oman (Oman)
    ['HZD','93','source:datum:geodetic','observatorio_meteorologico_1939_(corvo_and_flores_islands,_azores)'], // Observatorio Meteorologico 1939 (Corvo and Flores Islands, Azores)
    ['HZD','94','source:datum:geodetic','fort_thomas_1955_(nevis,_st_kitts,_leeward_islands)'], // Fort Thomas 1955 (Nevis, St Kitts, Leeward Islands)
    ['HZD','95','source:datum:geodetic','gan_1970_(addu_atoll,_republic_of_maldives)'], // Gan 1970 (Addu Atoll, Republic of Maldives)
    ['HZD','96','source:datum:geodetic','gandajika_base_(zaire)'], // Gandajika Base (Zaire)
    ['HZD','97','source:datum:geodetic','geocentric_datum_of_australia_(gda)'], // Geocentric Datum of Australia (GDA)
    ['HZD','98','source:datum:geodetic','gdz80_(china)'], // GDZ80 (China)
    ['HZD','99','source:datum:geodetic','geodetic_datum_1949_(new_zealand)'], // Geodetic Datum 1949 (New Zealand)
    ['HZD','100','source:datum:geodetic','dos_1968_(gizo_island,_new_georgia_islands)'], // DOS 1968 (Gizo Island, New Georgia Islands)
    ['HZD','101','source:datum:geodetic','graciosa_base_sw_(faial,_graciosa,_pico,_sao_jorge,_and_terceira_island,_azores)'], // Graciosa Base SW (Faial, Graciosa, Pico, Sao Jorge, and Terceira Island, Azores)
    ['HZD','102','source:datum:geodetic','greek_datum,_greece'], // Greek Datum, Greece
    ['HZD','103','source:datum:geodetic','greek_geodetic_reference_system_1987_(ggrs_87)'], // Greek Geodetic Reference System 1987 (GGRS 87)
    ['HZD','104','source:datum:geodetic','gunong_segara_(kalimantan_island,_indonesia)'], // Gunong Segara (Kalimantan Island, Indonesia)
    ['HZD','105','source:datum:geodetic','gunong_serindung'], // Gunong Serindung
    ['HZD','106','source:datum:geodetic','guam_1963'], // Guam 1963
    ['HZD','107','source:datum:geodetic','herat_north_(afganistan)'], // Herat North (Afganistan)
    ['HZD','108','source:datum:geodetic','hermannskogel'], // Hermannskogel
    ['HZD','109','source:datum:geodetic','provisional_south_chilean_1963_(or_hito_xviii_1963)_(s._chile,_53_degrees_south)'], // Provisional South Chilean 1963 (or Hito XVIII 1963) (S. Chile, 53 degrees South)
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
    ['HZD','136','source:datum:geodetic','kcs_2,_sierra_leone'], // KCS 2, Sierra Leone
    ['HZD','137','source:datum:geodetic','kerguelen_island_1949_(kerguelen_island)'], // Kerguelen Island 1949 (Kerguelen Island)
    ['HZD','138','source:datum:geodetic','korean_geodetic_system_1995_(south_korea)'], // Korean Geodetic System 1995 (South Korea)
    ['HZD','139','source:datum:geodetic','kkj_(or_kartastokoordinaattijarjestelma),_finland'], // KKJ (or Kartastokoordinaattijarjestelma), Finland
    ['HZD','140','source:datum:geodetic','kusaie_astro_1951'], // Kusaie Astro 1951
    ['HZD','141','source:datum:geodetic','kuwait_oil_company_(k28)'], // Kuwait Oil Company (K28)
    ['HZD','142','source:datum:geodetic','l.c._5_astro_1961_(cayman_brac_island)'], // L.C. 5 Astro 1961 (Cayman Brac Island)
    ['HZD','143','source:datum:geodetic','leigon_(ghana)'], // Leigon (Ghana)
    ['HZD','144','source:datum:geodetic','liberia_1964_(liberia)'], // Liberia 1964 (Liberia)
    ['HZD','145','source:datum:geodetic','lisbon_(castelo_di_sao_jorge),_portugal'], // Lisbon (Castelo di Sao Jorge), Portugal
    ['HZD','146','source:datum:geodetic','local_astro'], // Local Astro
    ['HZD','147','source:datum:geodetic','loma_quintana_(venezuela)'], // Loma Quintana (Venezuela)
    ['HZD','149','source:datum:geodetic','luzon_(philipines_except_mindanao_island)'], // Luzon (Philipines except Mindanao Island)
    ['HZD','150','source:datum:geodetic','luzon_(mindanao_island)'], // Luzon (Mindanao Island)
    ['HZD','151','source:datum:geodetic','marco_astro_(salvage_islands)'], // Marco Astro (Salvage Islands)
    ['HZD','152','source:datum:geodetic','martinique_fort-desaix'], // Martinique Fort-Desaix
    ['HZD','153','source:datum:geodetic','massawa_(eritrea,_ethiopia)'], // Massawa (Eritrea, Ethiopia)
    ['HZD','154','source:datum:geodetic','manokwari_(west_irian)'], // Manokwari (West Irian)
    ['HZD','155','source:datum:geodetic','mayotte_combani'], // Mayotte Combani
    ['HZD','156','source:datum:geodetic','mount_dillon,_tobago'], // Mount Dillon, Tobago
    ['HZD','157','source:datum:geodetic','merchich_(morocco)'], // Merchich (Morocco)
    ['HZD','158','source:datum:geodetic','midway_astro_1961_(midway_island)'], // Midway Astro 1961 (Midway Island)
    ['HZD','159','source:datum:geodetic','mahe_1971_(mahe_island)'], // Mahe 1971 (Mahe Island)
    ['HZD','161','source:datum:geodetic','minna_(cameroon)'], // Minna (Cameroon)
    ['HZD','162','source:datum:geodetic','minna_(nigeria)'], // Minna (Nigeria)
    ['HZD','163','source:datum:geodetic','rome_1940_(or_monte_mario_1940),_italy'], // Rome 1940 (or Monte Mario 1940), Italy
    ['HZD','164','source:datum:geodetic','rome_1940_(or_monte_mario_1940),_italy,_with_zero_meridian_rome'], // Rome 1940 (or Monte Mario 1940), Italy, with Zero Meridian Rome
    ['HZD','165','source:datum:geodetic','montjong_lowe'], // Montjong Lowe
    ['HZD','166','source:datum:geodetic','mporaloko_(gabon)'], // M'Poraloko (Gabon)
    ['HZD','167','source:datum:geodetic','viti_levu_1916_(viti_levu_island,_fiji_islands)'], // Viti Levu 1916 (Viti Levu Island, Fiji Islands)
    ['HZD','169','source:datum:geodetic','nahrwan_(masirah_island,_oman)'], // Nahrwan (Masirah Island, Oman)
    ['HZD','170','source:datum:geodetic','nahrwan_(united_arab_emirates)'], // Nahrwan (United Arab Emirates)
    ['HZD','171','source:datum:geodetic','nahrwan_(saudi_arabia)'], // Nahrwan (Saudi Arabia)
    ['HZD','172','source:datum:geodetic','naparima_(bwi,_trinidad_and_tobago)'], // Naparima (BWI, Trinidad and Tobago)
    ['HZD','174','source:datum:geodetic','north_american_1983_(alaska,_excluding_aleutian_islands)'], // North American 1983 (Alaska, excluding Aleutian Islands)
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
    ['HZD','187','source:datum:geodetic','north_american_1927_(newfoundland,_new_brunswick,_nova_scotia_and_quebec)'], // North American 1927 (Newfoundland, New Brunswick, Nova Scotia and Quebec)
    ['HZD','188','source:datum:geodetic','north_american_1927_(manitoba_and_ontario)'], // North American 1927 (Manitoba and Ontario)
    ['HZD','189','source:datum:geodetic','north_american_1927_(northwest_territories_and_saskatchewan)'], // North American 1927 (Northwest Territories and Saskatchewan)
    ['HZD','190','source:datum:geodetic','north_american_1927_(yukon)'], // North American 1927 (Yukon)
    ['HZD','191','source:datum:geodetic','north_american_1927_(mexico)'], // North American 1927 (Mexico)
    ['HZD','192','source:datum:geodetic','north_american_1927_(central_america)'], // North American 1927 (Central America)
    ['HZD','193','source:datum:geodetic','north_american_1927_(canal_zone)'], // North American 1927 (Canal Zone)
    ['HZD','194','source:datum:geodetic','north_american_1927_(caribbean)'], // North American 1927 (Caribbean)
    ['HZD','195','source:datum:geodetic','north_american_1927_(bahamas,_except_san_salvador_island)'], // North American 1927 (Bahamas, except San Salvador Island)
    ['HZD','196','source:datum:geodetic','north_american_1927_(san_salvador_island)'], // North American 1927 (San Salvador Island)
    ['HZD','197','source:datum:geodetic','north_american_1927_(cuba)'], // North American 1927 (Cuba)
    ['HZD','198','source:datum:geodetic','north_american_1927_(hayes_peninsula,_greenland)'], // North American 1927 (Hayes Peninsula, Greenland)
    ['HZD','199','source:datum:geodetic','north_american_1927_(aleutian_islands_east_of_180_degrees_west)'], // North American 1927 (Aleutian Islands East of 180 degrees West)
    ['HZD','200','source:datum:geodetic','north_american_1927_(aleutian_islands_west_of_180_degrees_west)'], // North American 1927 (Aleutian Islands West of 180 degrees West)
    ['HZD','201','source:datum:geodetic','revised_nahrwan'], // Revised Nahrwan
    ['HZD','202','source:datum:geodetic','new_french_or_nouvelle_triangulation_francaise_(ntf)_with_zero_meridian_paris'], // New French or Nouvelle Triangulation Francaise (NTF) with Zero Meridian Paris
    ['HZD','204','source:datum:geodetic','north_sahara_1959'], // North Sahara 1959
    ['HZD','205','source:datum:geodetic','ocotopeque,_guatemala'], // Ocotopeque, Guatemala
    ['HZD','206','source:datum:geodetic','belgium_1972_(observatoire_duccle)'], // Belgium 1972 (Observatoire d'Uccle)
    ['HZD','207','source:datum:geodetic','old_egyptian_(egypt)'], // Old Egyptian (Egypt)
    ['HZD','209','source:datum:geodetic','ordnance_survey_g.b._1936_(england)'], // Ordnance Survey G.B. 1936 (England)
    ['HZD','210','source:datum:geodetic','ordnance_survey_g.b._1936_(england,_isle_of_man,_and_wales)'], // Ordnance Survey G.B. 1936 (England, Isle of Man, and Wales)
    ['HZD','211','source:datum:geodetic','ordnance_survey_g.b._1936_(scotland_and_shetland_islands)'], // Ordnance Survey G.B. 1936 (Scotland and Shetland Islands)
    ['HZD','212','source:datum:geodetic','ordnance_survey_g.b._1936_(wales)'], // Ordnance Survey G.B. 1936 (Wales)
    ['HZD','213','source:datum:geodetic','ordnance_survey_g.b._1936_(mean_value)'], // Ordnance Survey G.B. 1936 (mean value)
    ['HZD','215','source:datum:geodetic','old_hawaiian_(hawaii)'], // Old Hawaiian (Hawaii)
    ['HZD','216','source:datum:geodetic','old_hawaiian_(kauai)'], // Old Hawaiian (Kauai)
    ['HZD','217','source:datum:geodetic','old_hawaiian_(maui)'], // Old Hawaiian (Maui)
    ['HZD','218','source:datum:geodetic','old_hawaiian_(oahu)'], // Old Hawaiian (Oahu)
    ['HZD','219','source:datum:geodetic','old_hawaiian_(mean_value)'], // Old Hawaiian (mean value)
    ['HZD','220','source:datum:geodetic','oslo_observatory_(old),_norway'], // Oslo Observatory (Old), Norway
    ['HZD','221','source:datum:geodetic','padang_base_west_end_(sumatra,_indonesia)'], // Padang Base West End (Sumatra, Indonesia)
    ['HZD','222','source:datum:geodetic','padang_base_west_end_(sumatra,_indonesia)_with_zero_meridian_djakarta'], // Padang Base West End (Sumatra, Indonesia) with Zero Meridian Djakarta
    ['HZD','223','source:datum:geodetic','palestine_1928_(israel,_jordan)'], // Palestine 1928 (Israel, Jordan)
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
    ['HZD','245','source:datum:geodetic','rauenberg_(berlin,_germany)'], // Rauenberg (Berlin, Germany)
    ['HZD','246','source:datum:geodetic','reconnaissance_triangulation,_morocco'], // Reconnaissance Triangulation, Morocco
    ['HZD','247','source:datum:geodetic','reunion_1947'], // Reunion 1947
    ['HZD','248','source:datum:geodetic','rt90,_stockholm,_sweden'], // RT90, Stockholm, Sweden
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
    ['HZD','262','source:datum:geodetic','south_american_1969_(baltra,_galapagos_islands)'], // South American 1969 (Baltra, Galapagos Islands)
    ['HZD','263','source:datum:geodetic','south_american_1969_(trinidad_and_tobago)'], // South American 1969 (Trinidad and Tobago)
    ['HZD','264','source:datum:geodetic','south_american_1969_(venezuela)'], // South American 1969 (Venezuela)
    ['HZD','265','source:datum:geodetic','south_american_1969_(mean_value)'], // South American 1969 (mean value)
    ['HZD','266','source:datum:geodetic','sao_braz_(sao_miguel,_santa_maria_islands,_azores)'], // Sao Braz (Sao Miguel, Santa Maria Islands, Azores)
    ['HZD','267','source:datum:geodetic','sapper_hill_1943_(east_falkland_islands)'], // Sapper Hill 1943 (East Falkland Islands)
    ['HZD','268','source:datum:geodetic','schwarzeck_(namibia)'], // Schwarzeck (Namibia)
    ['HZD','269','source:datum:geodetic','soviet_geodetic_system_1985'], // Soviet Geodetic System 1985
    ['HZD','270','source:datum:geodetic','soviet_geodetic_system_1990'], // Soviet Geodetic System 1990
    ['HZD','271','source:datum:geodetic','selvagem_grande_1938_(salvage_islands)'], // Selvagem Grande 1938 (Salvage Islands)
    ['HZD','272','source:datum:geodetic','astro_dos_71/4_(st._helena_island)'], // Astro DOS 71/4 (St. Helena Island)
    ['HZD','273','source:datum:geodetic','sierra_leone_1960'], // Sierra Leone 1960
    ['HZD','274','source:datum:geodetic','south_asia_(southeast_asia,_singapore)'], // South Asia (Southeast Asia, Singapore)
    ['HZD','276','source:datum:geodetic','st._pierre_et_miquelon_1950'], // St. Pierre et Miquelon 1950
    ['HZD','277','source:datum:geodetic','stockholm_1938_(sweden)'], // Stockholm 1938 (Sweden)
    ['HZD','278','source:datum:geodetic','sydney_observatory,_new_south_wales,_australia'], // Sydney Observatory, New South Wales, Australia
    ['HZD','279','source:datum:geodetic','tananarive_observatory_1925'], // Tananarive Observatory 1925
    ['HZD','280','source:datum:geodetic','tananarive_observatory_1925,_with_zero_meridian_paris'], // Tananarive Observatory 1925, with Zero Meridian Paris
    ['HZD','281','source:datum:geodetic','tristan_astro_1968_(tristan_da_cunha)'], // Tristan Astro 1968 (Tristan da Cunha)
    ['HZD','282','source:datum:geodetic','timbalai_1948_(brunei_and_east_malaysia_-_sarawak_and_sabah)'], // Timbalai 1948 (Brunei and East Malaysia - Sarawak and Sabah)
    ['HZD','283','source:datum:geodetic','timbalai_1968'], // Timbalai 1968
    ['HZD','285','source:datum:geodetic','tokyo_(japan)'], // Tokyo (Japan)
    ['HZD','286','source:datum:geodetic','tokyo_(korea)'], // Tokyo (Korea)
    ['HZD','287','source:datum:geodetic','tokyo_(okinawa)'], // Tokyo (Okinawa)
    ['HZD','288','source:datum:geodetic','tokyo_(mean_value)'], // Tokyo (mean value)
    ['HZD','289','source:datum:geodetic','trinidad_1903'], // Trinidad 1903
    ['HZD','290','source:datum:geodetic','astro_tern_island_1961_(tern_island,_hawaii)'], // Astro Tern Island 1961 (Tern Island, Hawaii)
    ['HZD','292','source:datum:geodetic','voirol_1875'], // Voirol 1875
    ['HZD','293','source:datum:geodetic','voirol_1875_with_zero_meridian_paris'], // Voirol 1875 with Zero Meridian Paris
    ['HZD','294','source:datum:geodetic','voirol_1960,_algeria'], // Voirol 1960, Algeria
    ['HZD','295','source:datum:geodetic','voirol_1960,_algeria,_with_zero_meridian_paris'], // Voirol 1960, Algeria, with Zero Meridian Paris
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

    // IAA - IALA A or B Adopted
    // ['IAA','-999999',undefined,undefined], // No Information
    ['IAA','1000','iala_adopted','no'],
    ['IAA','1001','iala_adopted','yes'],

    // IAS - IMO Adopted
    // ['IAS','-999999',undefined,undefined], // No Information
    ['IAS','1000','imo_adopted','no'],
    ['IAS','1001','imo_adopted','yes'],

    // INU - Inundation Type
    // ['INU','-999999',undefined,undefined], // No Information
    ['INU','1','inundation','controlled'], // Controlled
    ['INU','2','inundation','natural'], // Natural

    // IWT - Inland Water Type
    // ['IWT','-999999',undefined,undefined], // No Information
    ['IWT','1','water','lake'], // Lake
    ['IWT','2','water','pond'], // Pond
    ['IWT','3','water','undifferentiated_water_body'], // Undifferentiated Water Body
    ['IWT','4','man_made','reservoir'], // Reservoir
    ['IWT','5','landuse','basin'], // Basin
    ['IWT','6','water','water-hole'], // Water-hole
    ['IWT','7','water','landlocked_sea'], // Landlocked Sea
    ['IWT','999','water','other'], // Other

    // LAF - Maritime Navigation Line Features
    // ['LAF','-999999',undefined,undefined], // No Information
    ['LAF','1','navigation_line_feature','one_other_than_directional_light'], // One other than Directional Light
    ['LAF','2','navigation_line_feature','directional_light'], // Directional Light
    ['LAF','3','navigation_line_feature','two_lights_or_more'], // Two Lights or more
    ['LAF','4','navigation_line_feature','two_beacons_or_more'], // Two Beacons or more
    ['LAF','5','navigation_line_feature','two_other_than_two_lights_or_beacons'], // Two other than Two Lights or Beacons
    ['LAF','6','navigation_line_feature','measured_distance_markers'], // Measured Distance Markers
    ['LAF','999','navigation_line_feature','other'], // Other

    // LFC - Maritime Light Type
    // ['LFC','-999999',undefined,undefined], // No Information
    ['LFC','1','seamark:light:category','directional'], // Directional Function
    ['LFC','4','seamark:light:category','leading'], // Leading Light
    ['LFC','6','seamark:light:category','air_obstruction'], // Air Obstruction Light
    ['LFC','7','seamark:light:category','fog_detector'], // Fog Detector Light
    ['LFC','8','seamark:light:category','flood_light'], // Flood Light
    ['LFC','9','seamark:light:category','strip_light'], // Strip Light
    ['LFC','10','seamark:light:category','subsidiary'], // Subsidiary Light
    ['LFC','11','seamark:light:category','spotlight'], // Spotlight
    ['LFC','12','seamark:light:category','emergency_light'], // Emergency Light
    ['LFC','13','seamark:light:category','bearing'], // Bearing Light
    ['LFC','14','seamark:light:category','upper'], // Upper
    ['LFC','16','seamark:light:category','lower'], // Lower
    ['LFC','17','seamark:light:category','horizontal'], // Horizontally Disposed
    ['LFC','18','seamark:light:category','vertical'], // Vertically Disposed
    ['LFC','19','seamark:light:category','front'], // Front
    ['LFC','20','seamark:light:category','rear'], // Rear
    ['LFC','21','seamark:light:category','moire'], // Moire Effect Light
    ['LFC','22','seamark:light:category','sectored_light'], // Sectored Light
    ['LFC','25','seamark:light:category','occasional_light'], // Occasional Light
    ['LFC','999','seamark:light:category','other'], // Other

    // LFC2 - Maritime Light Type [2]
    // LFC3 - Maritime Light Type [3]

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
    ['LOC','48','location','portions_below_ground_surface'], // Portions Below Ground Surface

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

    // MAN - Maritime Navigation Marked
    // ['MAN','-999999',undefined,undefined], // No Information
    ['MAN','1000','navigation:maritime_landmark','no'],
    ['MAN','1001','navigation:maritime_landmark','yes'],

    // MAS - Maintained
    // ['MAS','-999999',undefined,undefined], // No Information
    ['MAS','1000','maintained','no'],
    ['MAS','1001','maintained','yes'],

    // MBC_MCC1 - Bot Char (First Material Type)
    // ['MBC_MCC1','-999999',undefined,undefined], // No Information
    ['MBC_MCC1','4','waterway:bottom:material:1','ash'], // Ash
    ['MBC_MCC1','7','waterway:bottom:material:1','bedrock'], // Bedrock
    ['MBC_MCC1','8','waterway:bottom:material:1','boulders'], // Boulders
    ['MBC_MCC1','10','waterway:bottom:material:1','calcareous'], // Calcareous
    ['MBC_MCC1','12','waterway:bottom:material:1','chalk'], // Chalk
    ['MBC_MCC1','14','waterway:bottom:material:1','cinders'], // Cinders
    ['MBC_MCC1','15','waterway:bottom:material:1','cirripedia'], // Cirripedia
    ['MBC_MCC1','16','waterway:bottom:material:1','clay'], // Clay
    ['MBC_MCC1','18','waterway:bottom:material:1','cobbles'], // Cobbles
    ['MBC_MCC1','22','waterway:bottom:material:1','conglomerate'], // Conglomerate
    ['MBC_MCC1','24','waterway:bottom:material:1','coral'], // Coral
    ['MBC_MCC1','25','waterway:bottom:material:1','coral_head'], // Coral Head
    ['MBC_MCC1','28','waterway:bottom:material:1','diatomaceous_earth'], // Diatomaceous Earth
    ['MBC_MCC1','36','waterway:bottom:material:1','foraminifera'], // Foraminifera
    ['MBC_MCC1','37','waterway:bottom:material:1','fucus'], // Fucus
    ['MBC_MCC1','45','waterway:bottom:material:1','plant_material'], // Plant Material
    ['MBC_MCC1','46','waterway:bottom:material:1','gravel'], // Gravel
    ['MBC_MCC1','49','waterway:bottom:material:1','ground_shell'], // Ground Shell
    ['MBC_MCC1','52','waterway:bottom:material:1','lava'], // Lava
    ['MBC_MCC1','55','waterway:bottom:material:1','loess'], // Loess
    ['MBC_MCC1','58','waterway:bottom:material:1','madrepore'], // Madrepore
    ['MBC_MCC1','59','waterway:bottom:material:1','manganese'], // Manganese
    ['MBC_MCC1','61','waterway:bottom:material:1','marl'], // Marl
    ['MBC_MCC1','63','waterway:bottom:material:1','matte'], // Matte
    ['MBC_MCC1','65','waterway:bottom:material:1','mud'], // Mud
    ['MBC_MCC1','66','waterway:bottom:material:1','mussels'], // Mussels
    ['MBC_MCC1','69','waterway:bottom:material:1','ooze'], // Ooze
    ['MBC_MCC1','70','waterway:bottom:material:1','oysters'], // Oysters
    ['MBC_MCC1','73','waterway:bottom:material:1','pebbles'], // Pebbles
    ['MBC_MCC1','75','waterway:bottom:material:1','sea_moss'], // Sea Moss
    ['MBC_MCC1','79','waterway:bottom:material:1','pumice'], // Pumice
    ['MBC_MCC1','80','waterway:bottom:material:1','quartz'], // Quartz
    ['MBC_MCC1','81','waterway:bottom:material:1','radiolaria'], // Radiolaria
    ['MBC_MCC1','84','waterway:bottom:material:1','rocks'], // Rocks
    ['MBC_MCC1','88','waterway:bottom:material:1','sand'], // Sand
    ['MBC_MCC1','90','waterway:bottom:material:1','schist'], // Schist
    ['MBC_MCC1','92','waterway:bottom:material:1','scoria'], // Scoria
    ['MBC_MCC1','96','waterway:bottom:material:1','shell'], // Shell
    ['MBC_MCC1','98','waterway:bottom:material:1','shingle'], // Shingle
    ['MBC_MCC1','99','waterway:bottom:material:1','silt'], // Silt
    ['MBC_MCC1','103','waterway:bottom:material:1','frozen_water'], // Frozen Water
    ['MBC_MCC1','104','waterway:bottom:material:1','soil'], // Soil
    ['MBC_MCC1','105','waterway:bottom:material:1','spicules'], // Spicules
    ['MBC_MCC1','106','waterway:bottom:material:1','sponge'], // Sponge
    ['MBC_MCC1','108','waterway:bottom:material:1','stone'], // Stone
    ['MBC_MCC1','111','waterway:bottom:material:1','tufa'], // Tufa
    ['MBC_MCC1','115','waterway:bottom:material:1','volcanic_ash'], // Volcanic Ash
    ['MBC_MCC1','999','waterway:bottom:material:1','other'], // Other

    // MBC_MCC2 - Bot Char (Second Material Type)
    // ['MBC_MCC2','-999999',undefined,undefined], // No Information
    ['MBC_MCC2','4','waterway:bottom:material:2','ash'], // Ash
    ['MBC_MCC2','7','waterway:bottom:material:2','bedrock'], // Bedrock
    ['MBC_MCC2','8','waterway:bottom:material:2','boulders'], // Boulders
    ['MBC_MCC2','10','waterway:bottom:material:2','calcareous'], // Calcareous
    ['MBC_MCC2','12','waterway:bottom:material:2','chalk'], // Chalk
    ['MBC_MCC2','14','waterway:bottom:material:2','cinders'], // Cinders
    ['MBC_MCC2','15','waterway:bottom:material:2','cirripedia'], // Cirripedia
    ['MBC_MCC2','16','waterway:bottom:material:2','clay'], // Clay
    ['MBC_MCC2','18','waterway:bottom:material:2','cobbles'], // Cobbles
    ['MBC_MCC2','22','waterway:bottom:material:2','conglomerate'], // Conglomerate
    ['MBC_MCC2','24','waterway:bottom:material:2','coral'], // Coral
    ['MBC_MCC2','25','waterway:bottom:material:2','coral_head'], // Coral Head
    ['MBC_MCC2','28','waterway:bottom:material:2','diatomaceous_earth'], // Diatomaceous Earth
    ['MBC_MCC2','36','waterway:bottom:material:2','foraminifera'], // Foraminifera
    ['MBC_MCC2','37','waterway:bottom:material:2','fucus'], // Fucus
    ['MBC_MCC2','45','waterway:bottom:material:2','plant_material'], // Plant Material
    ['MBC_MCC2','46','waterway:bottom:material:2','gravel'], // Gravel
    ['MBC_MCC2','49','waterway:bottom:material:2','ground_shell'], // Ground Shell
    ['MBC_MCC2','52','waterway:bottom:material:2','lava'], // Lava
    ['MBC_MCC2','55','waterway:bottom:material:2','loess'], // Loess
    ['MBC_MCC2','58','waterway:bottom:material:2','madrepore'], // Madrepore
    ['MBC_MCC2','59','waterway:bottom:material:2','manganese'], // Manganese
    ['MBC_MCC2','61','waterway:bottom:material:2','marl'], // Marl
    ['MBC_MCC2','63','waterway:bottom:material:2','matte'], // Matte
    ['MBC_MCC2','65','waterway:bottom:material:2','mud'], // Mud
    ['MBC_MCC2','66','waterway:bottom:material:2','mussels'], // Mussels
    ['MBC_MCC2','69','waterway:bottom:material:2','ooze'], // Ooze
    ['MBC_MCC2','70','waterway:bottom:material:2','oysters'], // Oysters
    ['MBC_MCC2','73','waterway:bottom:material:2','pebbles'], // Pebbles
    ['MBC_MCC2','75','waterway:bottom:material:2','sea_moss'], // Sea Moss
    ['MBC_MCC2','79','waterway:bottom:material:2','pumice'], // Pumice
    ['MBC_MCC2','80','waterway:bottom:material:2','quartz'], // Quartz
    ['MBC_MCC2','81','waterway:bottom:material:2','radiolaria'], // Radiolaria
    ['MBC_MCC2','84','waterway:bottom:material:2','rocks'], // Rocks
    ['MBC_MCC2','88','waterway:bottom:material:2','sand'], // Sand
    ['MBC_MCC2','90','waterway:bottom:material:2','schist'], // Schist
    ['MBC_MCC2','92','waterway:bottom:material:2','scoria'], // Scoria
    ['MBC_MCC2','96','waterway:bottom:material:2','shell'], // Shell
    ['MBC_MCC2','98','waterway:bottom:material:2','shingle'], // Shingle
    ['MBC_MCC2','99','waterway:bottom:material:2','silt'], // Silt
    ['MBC_MCC2','103','waterway:bottom:material:2','frozen_water'], // Frozen Water
    ['MBC_MCC2','104','waterway:bottom:material:2','soil'], // Soil
    ['MBC_MCC2','105','waterway:bottom:material:2','spicules'], // Spicules
    ['MBC_MCC2','106','waterway:bottom:material:2','sponge'], // Sponge
    ['MBC_MCC2','108','waterway:bottom:material:2','stone'], // Stone
    ['MBC_MCC2','111','waterway:bottom:material:2','tufa'], // Tufa
    ['MBC_MCC2','115','waterway:bottom:material:2','volcanic_ash'], // Volcanic Ash
    ['MBC_MCC2','999','waterway:bottom:material:2','other'], // Other

    // MBC_PSC1 - Bot Char (first material quality)
    // ['MBC_PSC1','-999999',undefined,undefined], // No Information
    ['MBC_PSC1','1','waterway:bottom:material:quality:1','broken'], // Broken
    ['MBC_PSC1','2','waterway:bottom:material:quality:1','coarse'], // Coarse
    ['MBC_PSC1','3','waterway:bottom:material:quality:1','decayed'], // Decayed
    ['MBC_PSC1','4','waterway:bottom:material:quality:1','fine'], // Fine
    ['MBC_PSC1','5','waterway:bottom:material:quality:1','gritty'], // Gritty
    ['MBC_PSC1','6','waterway:bottom:material:quality:1','hard'], // Hard
    ['MBC_PSC1','7','waterway:bottom:material:quality:1','rotten'], // Rotten
    ['MBC_PSC1','8','waterway:bottom:material:quality:1','soft'], // Soft
    ['MBC_PSC1','9','waterway:bottom:material:quality:1','sticky'], // Sticky
    ['MBC_PSC1','10','waterway:bottom:material:quality:1','stiff'], // Stiff
    ['MBC_PSC1','11','waterway:bottom:material:quality:1','streaky'], // Streaky
    ['MBC_PSC1','12','waterway:bottom:material:quality:1','tenacious'], // Tenacious
    ['MBC_PSC1','13','waterway:bottom:material:quality:1','irregular'], // Irregular
    ['MBC_PSC1','14','waterway:bottom:material:quality:1','bare'], // Bare
    ['MBC_PSC1','17','waterway:bottom:material:quality:1','calcareous'], // Calcareous
    ['MBC_PSC1','18','waterway:bottom:material:quality:1','flinty'], // Flinty
    ['MBC_PSC1','19','waterway:bottom:material:quality:1','glacial'], // Glacial
    ['MBC_PSC1','20','waterway:bottom:material:quality:1','ground'], // Ground
    ['MBC_PSC1','21','waterway:bottom:material:quality:1','large'], // Large
    ['MBC_PSC1','22','waterway:bottom:material:quality:1','rocky'], // Rocky
    ['MBC_PSC1','23','waterway:bottom:material:quality:1','small'], // Small
    ['MBC_PSC1','24','waterway:bottom:material:quality:1','speckled'], // Speckled
    ['MBC_PSC1','25','waterway:bottom:material:quality:1','varied'], // Varied
    ['MBC_PSC1','26','waterway:bottom:material:quality:1','volcanic'], // Volcanic
    ['MBC_PSC1','27','waterway:bottom:material:quality:1','medium'], // Medium
    ['MBC_PSC1','29','waterway:bottom:material:quality:1','mobile_bottom'], // Mobile Bottom
    ['MBC_PSC1','999','waterway:bottom:material:quality:1','other'], // Other

    // MBC_PSC2 - Bot Char (second material quality)
    // ['MBC_PSC2','-999999',undefined,undefined], // No Information
    ['MBC_PSC2','1','waterway:bottom:material:quality:2','broken'], // Broken
    ['MBC_PSC2','2','waterway:bottom:material:quality:2','coarse'], // Coarse
    ['MBC_PSC2','3','waterway:bottom:material:quality:2','decayed'], // Decayed
    ['MBC_PSC2','4','waterway:bottom:material:quality:2','fine'], // Fine
    ['MBC_PSC2','5','waterway:bottom:material:quality:2','gritty'], // Gritty
    ['MBC_PSC2','6','waterway:bottom:material:quality:2','hard'], // Hard
    ['MBC_PSC2','7','waterway:bottom:material:quality:2','rotten'], // Rotten
    ['MBC_PSC2','8','waterway:bottom:material:quality:2','soft'], // Soft
    ['MBC_PSC2','9','waterway:bottom:material:quality:2','sticky'], // Sticky
    ['MBC_PSC2','10','waterway:bottom:material:quality:2','stiff'], // Stiff
    ['MBC_PSC2','11','waterway:bottom:material:quality:2','streaky'], // Streaky
    ['MBC_PSC2','12','waterway:bottom:material:quality:2','tenacious'], // Tenacious
    ['MBC_PSC2','13','waterway:bottom:material:quality:2','irregular'], // Irregular
    ['MBC_PSC2','14','waterway:bottom:material:quality:2','bare'], // Bare
    ['MBC_PSC2','17','waterway:bottom:material:quality:2','calcareous'], // Calcareous
    ['MBC_PSC2','18','waterway:bottom:material:quality:2','flinty'], // Flinty
    ['MBC_PSC2','19','waterway:bottom:material:quality:2','glacial'], // Glacial
    ['MBC_PSC2','20','waterway:bottom:material:quality:2','ground'], // Ground
    ['MBC_PSC2','21','waterway:bottom:material:quality:2','large'], // Large
    ['MBC_PSC2','22','waterway:bottom:material:quality:2','rocky'], // Rocky
    ['MBC_PSC2','23','waterway:bottom:material:quality:2','small'], // Small
    ['MBC_PSC2','24','waterway:bottom:material:quality:2','speckled'], // Speckled
    ['MBC_PSC2','25','waterway:bottom:material:quality:2','varied'], // Varied
    ['MBC_PSC2','26','waterway:bottom:material:quality:2','volcanic'], // Volcanic
    ['MBC_PSC2','27','waterway:bottom:material:quality:2','medium'], // Medium
    ['MBC_PSC2','29','waterway:bottom:material:quality:2','mobile_bottom'], // Mobile Bottom
    ['MBC_PSC2','999','waterway:bottom:material:quality:2','other'], // Other

    // MBC_SCO1 - Bot Char (First Sediment Colour)
    // ['MBC_SCO1','-999999',undefined,undefined], // No Information
    ['MBC_SCO1','1','waterway:sediment_colour:1','grayish_green'], // Grayish Green
    ['MBC_SCO1','2','waterway:sediment_colour:1','pale_grayish_green'], // Pale Grayish Green
    ['MBC_SCO1','3','waterway:sediment_colour:1','very_pale_green'], // Very Pale Green
    ['MBC_SCO1','4','waterway:sediment_colour:1','dusky_yellowish_green'], // Dusky Yellowish Green
    ['MBC_SCO1','5','waterway:sediment_colour:1','dark_yellowish_green'], // Dark Yellowish Green
    ['MBC_SCO1','6','waterway:sediment_colour:1','grayish_yellowish_green'], // Grayish Yellowish Green
    ['MBC_SCO1','7','waterway:sediment_colour:1','moderate_yellowish_green'], // Moderate Yellowish Green
    ['MBC_SCO1','8','waterway:sediment_colour:1','pale_yellowish_green'], // Pale Yellowish Green
    ['MBC_SCO1','9','waterway:sediment_colour:1','very_dusky_red'], // Very Dusky Red
    ['MBC_SCO1','10','waterway:sediment_colour:1','dark_reddish_brown'], // Dark Reddish Brown
    ['MBC_SCO1','11','waterway:sediment_colour:1','grayish_red'], // Grayish Red
    ['MBC_SCO1','12','waterway:sediment_colour:1','moderate_reddish_brown'], // Moderate Reddish Brown
    ['MBC_SCO1','13','waterway:sediment_colour:1','pale_reddish_brown'], // Pale Reddish Brown
    ['MBC_SCO1','14','waterway:sediment_colour:1','pale_red'], // Pale Red
    ['MBC_SCO1','15','waterway:sediment_colour:1','moderate_reddish_orange'], // Moderate Reddish Orange
    ['MBC_SCO1','16','waterway:sediment_colour:1','moderate_orange_pink'], // Moderate Orange Pink
    ['MBC_SCO1','17','waterway:sediment_colour:1','grayish_orange_pink'], // Grayish Orange Pink
    ['MBC_SCO1','18','waterway:sediment_colour:1','greenish_olive'], // Greenish Olive
    ['MBC_SCO1','19','waterway:sediment_colour:1','light_olive'], // Light Olive
    ['MBC_SCO1','20','waterway:sediment_colour:1','pale_olive'], // Pale Olive
    ['MBC_SCO1','21','waterway:sediment_colour:1','dark_greenish_yellow'], // Dark Greenish Yellow
    ['MBC_SCO1','22','waterway:sediment_colour:1','moderate_greenish_yellow'], // Moderate Greenish Yellow
    ['MBC_SCO1','23','waterway:sediment_colour:1','pale_greenish_yellow'], // Pale Greenish Yellow
    ['MBC_SCO1','24','waterway:sediment_colour:1','dusky_green'], // Dusky Green
    ['MBC_SCO1','25','waterway:sediment_colour:1','moderate_grayish_green'], // Moderate Grayish Green
    ['MBC_SCO1','26','waterway:sediment_colour:1','moderate_green'], // Moderate Green
    ['MBC_SCO1','27','waterway:sediment_colour:1','brilliant_green'], // Brilliant Green
    ['MBC_SCO1','28','waterway:sediment_colour:1','pale_green'], // Pale Green
    ['MBC_SCO1','29','waterway:sediment_colour:1','light_green'], // Light Green
    ['MBC_SCO1','30','waterway:sediment_colour:1','light_yellowish_green'], // Light Yellowish Green
    ['MBC_SCO1','31','waterway:sediment_colour:1','very_dusky_purple'], // Very Dusky Purple
    ['MBC_SCO1','32','waterway:sediment_colour:1','pale_purple'], // Pale Purple
    ['MBC_SCO1','33','waterway:sediment_colour:1','very_dark_red'], // Very Dark Red
    ['MBC_SCO1','34','waterway:sediment_colour:1','dusky_red'], // Dusky Red
    ['MBC_SCO1','35','waterway:sediment_colour:1','moderate_red'], // Moderate Red
    ['MBC_SCO1','36','waterway:sediment_colour:1','light_red'], // Light Red
    ['MBC_SCO1','37','waterway:sediment_colour:1','moderate_pink'], // Moderate Pink
    ['MBC_SCO1','38','waterway:sediment_colour:1','very_dusky_red_purple'], // Very Dusky Red Purple
    ['MBC_SCO1','39','waterway:sediment_colour:1','pale_red_purple'], // Pale Red Purple
    ['MBC_SCO1','40','waterway:sediment_colour:1','pale_pink'], // Pale Pink
    ['MBC_SCO1','41','waterway:sediment_colour:1','moderate_olive_brown'], // Moderate Olive Brown
    ['MBC_SCO1','42','waterway:sediment_colour:1','light_olive_brown'], // Light Olive Brown
    ['MBC_SCO1','43','waterway:sediment_colour:1','dusky_yellow'], // Dusky Yellow
    ['MBC_SCO1','44','waterway:sediment_colour:1','moderate_yellow'], // Moderate Yellow
    ['MBC_SCO1','45','waterway:sediment_colour:1','grayish_yellow'], // Grayish Yellow
    ['MBC_SCO1','46','waterway:sediment_colour:1','dusky_blue_green'], // Dusky Blue Green
    ['MBC_SCO1','47','waterway:sediment_colour:1','light_blue_green'], // Light Blue Green
    ['MBC_SCO1','48','waterway:sediment_colour:1','white'], // White
    ['MBC_SCO1','49','waterway:sediment_colour:1','black'], // Black
    ['MBC_SCO1','999','waterway:sediment_colour:1','other'], // Other

    // MBC_SCO2 - Bot Char (Second Sediment Colour)
    // ['MBC_SCO2','-999999',undefined,undefined], // No Information
    ['MBC_SCO2','1','waterway:sediment_colour:2','grayish_green'], // Grayish Green
    ['MBC_SCO2','2','waterway:sediment_colour:2','pale_grayish_green'], // Pale Grayish Green
    ['MBC_SCO2','3','waterway:sediment_colour:2','very_pale_green'], // Very Pale Green
    ['MBC_SCO2','4','waterway:sediment_colour:2','dusky_yellowish_green'], // Dusky Yellowish Green
    ['MBC_SCO2','5','waterway:sediment_colour:2','dark_yellowish_green'], // Dark Yellowish Green
    ['MBC_SCO2','6','waterway:sediment_colour:2','grayish_yellowish_green'], // Grayish Yellowish Green
    ['MBC_SCO2','7','waterway:sediment_colour:2','moderate_yellowish_green'], // Moderate Yellowish Green
    ['MBC_SCO2','8','waterway:sediment_colour:2','pale_yellowish_green'], // Pale Yellowish Green
    ['MBC_SCO2','9','waterway:sediment_colour:2','very_dusky_red'], // Very Dusky Red
    ['MBC_SCO2','10','waterway:sediment_colour:2','dark_reddish_brown'], // Dark Reddish Brown
    ['MBC_SCO2','11','waterway:sediment_colour:2','grayish_red'], // Grayish Red
    ['MBC_SCO2','12','waterway:sediment_colour:2','moderate_reddish_brown'], // Moderate Reddish Brown
    ['MBC_SCO2','13','waterway:sediment_colour:2','pale_reddish_brown'], // Pale Reddish Brown
    ['MBC_SCO2','14','waterway:sediment_colour:2','pale_red'], // Pale Red
    ['MBC_SCO2','15','waterway:sediment_colour:2','moderate_reddish_orange'], // Moderate Reddish Orange
    ['MBC_SCO2','16','waterway:sediment_colour:2','moderate_orange_pink'], // Moderate Orange Pink
    ['MBC_SCO2','17','waterway:sediment_colour:2','grayish_orange_pink'], // Grayish Orange Pink
    ['MBC_SCO2','18','waterway:sediment_colour:2','greenish_olive'], // Greenish Olive
    ['MBC_SCO2','19','waterway:sediment_colour:2','light_olive'], // Light Olive
    ['MBC_SCO2','20','waterway:sediment_colour:2','pale_olive'], // Pale Olive
    ['MBC_SCO2','21','waterway:sediment_colour:2','dark_greenish_yellow'], // Dark Greenish Yellow
    ['MBC_SCO2','22','waterway:sediment_colour:2','moderate_greenish_yellow'], // Moderate Greenish Yellow
    ['MBC_SCO2','23','waterway:sediment_colour:2','pale_greenish_yellow'], // Pale Greenish Yellow
    ['MBC_SCO2','24','waterway:sediment_colour:2','dusky_green'], // Dusky Green
    ['MBC_SCO2','25','waterway:sediment_colour:2','moderate_grayish_green'], // Moderate Grayish Green
    ['MBC_SCO2','26','waterway:sediment_colour:2','moderate_green'], // Moderate Green
    ['MBC_SCO2','27','waterway:sediment_colour:2','brilliant_green'], // Brilliant Green
    ['MBC_SCO2','28','waterway:sediment_colour:2','pale_green'], // Pale Green
    ['MBC_SCO2','29','waterway:sediment_colour:2','light_green'], // Light Green
    ['MBC_SCO2','30','waterway:sediment_colour:2','light_yellowish_green'], // Light Yellowish Green
    ['MBC_SCO2','31','waterway:sediment_colour:2','very_dusky_purple'], // Very Dusky Purple
    ['MBC_SCO2','32','waterway:sediment_colour:2','pale_purple'], // Pale Purple
    ['MBC_SCO2','33','waterway:sediment_colour:2','very_dark_red'], // Very Dark Red
    ['MBC_SCO2','34','waterway:sediment_colour:2','dusky_red'], // Dusky Red
    ['MBC_SCO2','35','waterway:sediment_colour:2','moderate_red'], // Moderate Red
    ['MBC_SCO2','36','waterway:sediment_colour:2','light_red'], // Light Red
    ['MBC_SCO2','37','waterway:sediment_colour:2','moderate_pink'], // Moderate Pink
    ['MBC_SCO2','38','waterway:sediment_colour:2','very_dusky_red_purple'], // Very Dusky Red Purple
    ['MBC_SCO2','39','waterway:sediment_colour:2','pale_red_purple'], // Pale Red Purple
    ['MBC_SCO2','40','waterway:sediment_colour:2','pale_pink'], // Pale Pink
    ['MBC_SCO2','41','waterway:sediment_colour:2','moderate_olive_brown'], // Moderate Olive Brown
    ['MBC_SCO2','42','waterway:sediment_colour:2','light_olive_brown'], // Light Olive Brown
    ['MBC_SCO2','43','waterway:sediment_colour:2','dusky_yellow'], // Dusky Yellow
    ['MBC_SCO2','44','waterway:sediment_colour:2','moderate_yellow'], // Moderate Yellow
    ['MBC_SCO2','45','waterway:sediment_colour:2','grayish_yellow'], // Grayish Yellow
    ['MBC_SCO2','46','waterway:sediment_colour:2','dusky_blue_green'], // Dusky Blue Green
    ['MBC_SCO2','47','waterway:sediment_colour:2','light_blue_green'], // Light Blue Green
    ['MBC_SCO2','48','waterway:sediment_colour:2','white'], // White
    ['MBC_SCO2','49','waterway:sediment_colour:2','black'], // Black
    ['MBC_SCO2','999','waterway:sediment_colour:2','other'], // Other

    // MCC - Structural Material Type
    // ['MCC','-999999',undefined,undefined], // No Information
    ['MCC','2','material','aluminium'], // Aluminium
    ['MCC','5','material','asphalt'], // Asphalt
    ['MCC','7','material','bedrock'], // Bedrock
    ['MCC','8','material','boulders'], // Boulders
    ['MCC','9','material','brick'], // Brick
    ['MCC','21','material','concrete'], // Concrete
    ['MCC','23','material','copper'], // Copper
    ['MCC','24','material','coral'], // Coral
    ['MCC','25','material','coral_head'], // Coral Head
    ['MCC','46','material','gravel'], // Gravel
    ['MCC','51','material','iron'], // Iron
    ['MCC','52','material','lava'], // Lava
    ['MCC','54','material','lead'], // Lead
    ['MCC','55','material','loess'], // Loess
    ['MCC','62','material','masonry'], // Masonry
    ['MCC','64','material','metal'], // Metal
    ['MCC','65','material','mud'], // Mud
    ['MCC','74','material','plastic'], // Plastic
    ['MCC','77','material','prestressed_concrete'], // Prestressed Concrete
    ['MCC','83','material','reinforced_concrete'], // Reinforced Concrete
    ['MCC','84','material','rock'], // Rocks
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
    ['MCC','160','material','earthen'], // Earthen
    ['MCC','999','material','other'], // Other

    // MCC2 - Structural Material Type [2]
    // MCC3 - Structural Material Type [3]

    // MCX - Motorized Crossing
    // ['MCX','-999999',undefined,undefined], // No Information
    ['MCX','1000','ferry:motorized','no'],
    ['MCX','1001','ferry:motorized','yes'],

    // MCY - Maritime Caution Type
    // ['MCY','-999999',undefined,undefined], // No Information
    ['MCY','1','seamark:precautionary_area','cargo_transshipment'], // Cargo Transshipment
    ['MCY','2','seamark:precautionary_area','ice_advisory'], // Ice Advisory
    ['MCY','3','seamark:precautionary_area','intake_area'], // Intake Area
    ['MCY','4','seamark:precautionary_area','outfall_area'], // Outfall Area
    ['MCY','5','seamark:precautionary_area','pollution_zone'], // Pollution Zone
    ['MCY','6','seamark:precautionary_area','submarine_cable'], // Submarine Cable
    ['MCY','7','seamark:precautionary_area','submarine_pipeline'], // Submarine Pipeline
    ['MCY','8','seamark:precautionary_area','submarine_sewer'], // Submarine Sewer
    ['MCY','9','seamark:precautionary_area','unexploded_ordnance'], // Unexploded Ordnance
    ['MCY','10','seamark:precautionary_area','clearing_line'], // Clearing Line
    ['MCY','11','seamark:precautionary_area','colregs_demarcation_line'], // COLREGS Demarcation Line
    ['MCY','12','seamark:precautionary_area','harbour_area'], // Harbour Area
    ['MCY','13','seamark:precautionary_area','ocean_current_margin'], // Ocean Current Margin
    ['MCY','14','seamark:precautionary_area','works_in_progress_area'], // Works in Progress Area
    ['MCY','15','seamark:precautionary_area','unsurveyed_area'], // Unsurveyed Area
    ['MCY','999','seamark:precautionary_area','other'], // Other

    // MCY2 - Maritime Caution Type [2]
    // MCY3 - Maritime Caution Type [3]

    // MEL - Medical Service
    // ['MEL','-999999',undefined,undefined], // No Information
    ['MEL','1','healthcare:speciality','emergency'], // Accident and Emergency
    ['MEL','2','healthcare:speciality','anaesthetics'], // Anaesthetics
    ['MEL','3','healthcare:speciality','cardiology'], // Cardiology
    ['MEL','4','healthcare:speciality','critical_care'], // Critical Care
    ['MEL','5','healthcare:speciality','diagnostic_imaging'], // Diagnostic Imaging
    ['MEL','6','healthcare:speciality','ear,_nose_and_throat'], // Ear, Nose and Throat
    ['MEL','7','healthcare:speciality','surgery'], // General Surgery
    ['MEL','8','healthcare:speciality','gynaecology'], // Gynaecology
    ['MEL','9','healthcare:speciality','maternity'], // Maternity
    ['MEL','10','healthcare:speciality','neurology'], // Neurology
    ['MEL','11','healthcare:speciality','oncology'], // Oncology
    ['MEL','12','healthcare:speciality','ophthalmology'], // Ophthalmology
    ['MEL','13','healthcare:speciality','orthopaedics'], // Orthopaedics
    ['MEL','14','healthcare:speciality','pharmacy'], // Pharmacy
    ['MEL','15','healthcare:speciality','urology'], // Urology
    ['MEL','16','healthcare:speciality','sexual_health'], // Sexual Health
    ['MEL','17','healthcare:speciality','haematology'], // Haematology
    ['MEL','18','healthcare:speciality','physiotherapy'], // Physiotherapy
    ['MEL','19','healthcare:speciality','dietetics'], // Dietetics
    ['MEL','20','healthcare:speciality','preventive_medicine'], // Preventive Medicine
    ['MEL','21','healthcare:speciality','blood_bank'], // Blood Bank
    ['MEL','22','healthcare:speciality','paediatrics'], // Paediatrics
    ['MEL','23','healthcare:speciality','psychiatry'], // Psychiatry
    ['MEL','24','healthcare:speciality','burns_unit'], // Burns Unit
    ['MEL','25','healthcare:speciality','general'], // General Practice
    ['MEL','26','healthcare:speciality','clinical_services'], // Clinical Services
    ['MEL','999','healthcare:speciality','other'], // Other

    // MES - Median Present
    // ['MES','-999999',undefined,undefined], // No Information
    ['MES','1000','median','no'],
    ['MES','1001','median','yes'],

    // MFB - Manufactured Building
    // ['MFB','-999999',undefined,undefined], // No Information
    ['MFB','1000','building:manufactured','no'],
    ['MFB','1001','building:manufactured','yes'],

    // MFE - Marine Farm Enclosure Method
    // ['MFE','-999999',undefined,undefined], // No Information
    ['MFE','1','aquaculture:enclosure','fencing'], // Fencing
    ['MFE','2','aquaculture:enclosure','fishing_stakes'], // Fishing Stakes
    ['MFE','3','aquaculture:enclosure','nets'], // Nets
    ['MFE','999','aquaculture:enclosure','other'], // Other

    // MFY - Medical Facility Type
    // ['MFY','-999999',undefined,undefined], // No Information
    ['MFY','1','health_facility:type','general_medical_and_surgical'], // General Medical and Surgical
    ['MFY','2','health_facility:type','long-term_acute_care'], // Long-term Acute Care
    ['MFY','3','health_facility:type','chemical_dependency'], // Chemical Dependency
    ['MFY','4','health_facility:type','chronic_disease'], // Chronic Disease
    ['MFY','5','health_facility:type','eye,_ear,_nose_and_throat'], // Eye, Ear, Nose and Throat
    ['MFY','6','health_facility:type','intellectual_disablement_care'], // Intellectual Disablement Care
    ['MFY','7','health_facility:type','obstetrics_and_gynaecology'], // Obstetrics and Gynaecology
    ['MFY','8','health_facility:type','orthopaedics'], // Orthopaedics
    ['MFY','9','health_facility:type','psychiatric'], // Psychiatric
    ['MFY','10','health_facility:type','rehabilitation'], // Rehabilitation
    ['MFY','11','health_facility:type','respiratory_disease'], // Respiratory Disease
    ['MFY','12','health_facility:type','specialty_care'], // Specialty Care
    ['MFY','13','health_facility:type','unit_within_institution'], // Unit within Institution
    ['MFY','14','health_facility:type','unit_within_institution_for_the_intellectually_disabled'], // Unit within Institution for the Intellectually Disabled
    ['MFY','15','health_facility:type','pediatric_medical_and_surgical'], // Pediatric Medical and Surgical
    ['MFY','16','health_facility:type','paediatric_chronic_disease'], // Paediatric Chronic Disease
    ['MFY','17','health_facility:type','paediatric_eye,_ear,_nose_and_throat'], // Paediatric Eye, Ear, Nose and Throat
    ['MFY','18','health_facility:type','paediatric_orthopaedic'], // Paediatric Orthopaedic
    ['MFY','19','health_facility:type','pediatric_psychiatric'], // Pediatric Psychiatric
    ['MFY','20','health_facility:type','paediatric_rehabilitation'], // Paediatric Rehabilitation
    ['MFY','21','health_facility:type','paediatric_respiratory_disease'], // Paediatric Respiratory Disease
    ['MFY','22','health_facility:type','paediatric_specialty_care'], // Paediatric Specialty Care
    ['MFY','23','health_facility:type','paediatric_unit_within_institution'], // Paediatric Unit within Institution
    ['MFY','999','health_facility:type','other'], // Other

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

    // MGL2 - Maritime Geopolitical Limit Type [2]
    // MGL3 - Maritime Geopolitical Limit Type [3]

    // MKSZ - Market Size
    // ['MKSZ','-999999',undefined,undefined], // No Information
    ['MKSZ','1','market_size','small'], // Small
    ['MKSZ','2','market_size','medium'], // Medium
    ['MKSZ','3','market_size','large'], // Large
    ['MKSZ','998','market_size','not_applicable'], // Not Applicable

    // MLO - Limit(s) Physically Based
    // ['MLO','-999999',undefined,undefined], // No Information
    ['MLO','1000','physical_limit','no'],
    ['MLO','1001','physical_limit','yes'],

    // MLOP - Military Operations Type
    // ['MLOP','-999999',undefined,undefined], // No Information
    ['MLOP','1','military:type','command_post'], // Command Post
    ['MLOP','2','military:type','command_outpost'], // Command Outpost
    ['MLOP','3','military:type','brigade_maintenance_area'], // Brigade Maintenance Area
    ['MLOP','4','military:type','air_departure_point'], // Air Departure Point
    ['MLOP','5','military:type','amphibious_departure_point'], // Amphibious Departure Point
    ['MLOP','6','military:type','evacuee_handling_centre'], // Evacuee Handling Centre
    ['MLOP','7','military:type','evacuee_collection_point'], // Evacuee Collection Point
    ['MLOP','8','military:type','observation_post'], // Observation Post
    ['MLOP','9','military:type','forward_operating_base'], // Forward Operating Base
    ['MLOP','10','military:type','patrol_base'], // Patrol Base
    ['MLOP','11','military:type','evacuation_point'], // Evacuation Point
    ['MLOP','12','military:type','evacuee_assembly_area'], // Evacuee Assembly Area
    ['MLOP','999','military:type','other'], // Other

    // MLUS - Military Use Type
    // ['MLUS','-999999',undefined,undefined], // No Information
    ['MLUS','1','military:use','tactical_area_of_responsibility'], // Tactical Area of Responsibility
    ['MLUS','2','military:use','named_area_of_interest'], // Named Area of Interest
    ['MLUS','3','military:use','target_area_of_interest'], // Target Area of Interest
    ['MLUS','4','military:use','area_of_operations'], // Area of Operations
    ['MLUS','5','military:use','area_of_interest'], // Area of Interest
    ['MLUS','6','military:use','out_of_bounds'], // Out of Bounds
    ['MLUS','7','military:use','no_go_area'], // No Go Area
    ['MLUS','8','military:use','no_fly_zone'], // No Fly Zone
    ['MLUS','9','military:use','restricted_operation_zone'], // Restricted Operation Zone
    ['MLUS','10','military:use','temporary_area_of_operations'], // Temporary Area of Operations
    ['MLUS','11','military:use','report_line'], // Report line
    ['MLUS','12','military:use','main_supply_route'], // Supply Route, Main
    ['MLUS','13','military:use','secondary_supply_route'], // Supply Route, Secondary
    ['MLUS','14','military:use','alternate_supply_route'], // Supply Route, Alternate
    ['MLUS','15','military:use','forward_edge_of_battle_area'], // Forward Edge of Battle Area
    ['MLUS','16','military:use','bomb_release_line'], // Bomb release line
    ['MLUS','17','military:use','forward_line_of_own_troops'], // Forward Line of Own Troops
    ['MLUS','18','military:use','missile_release_line'], // Missile release line
    ['MLUS','19','military:use','forward_line_of_enemy_troops'], // Forward Line of Enemy Troops
    ['MLUS','999','military:use','other'], // Other

    // MNR - Maritime Navigation Restriction
    // ['MNR','-999999',undefined,undefined], // No Information
    ['MNR','1','seamark:precautionary_area:restriction','anchoring_prohibited'], // Anchoring Prohibited
    ['MNR','2','seamark:precautionary_area:restriction','anchoring_restricted'], // Anchoring Restricted
    ['MNR','3','seamark:precautionary_area:restriction','fishing_prohibited'], // Fishing Prohibited
    ['MNR','4','seamark:precautionary_area:restriction','fishing_restricted'], // Fishing Restricted
    ['MNR','5','seamark:precautionary_area:restriction','trawling_prohibited'], // Trawling Prohibited
    ['MNR','6','seamark:precautionary_area:restriction','trawling_restricted'], // Trawling Restricted
    ['MNR','7','seamark:precautionary_area:restriction','entry_prohibited'], // Entry Prohibited
    ['MNR','8','seamark:precautionary_area:restriction','entry_restricted'], // Entry Restricted
    ['MNR','9','seamark:precautionary_area:restriction','dredging_prohibited'], // Dredging Prohibited
    ['MNR','10','seamark:precautionary_area:restriction','dredging_restricted'], // Dredging Restricted
    ['MNR','11','seamark:precautionary_area:restriction','diving_prohibited'], // Diving Prohibited
    ['MNR','12','seamark:precautionary_area:restriction','diving_restricted'], // Diving Restricted
    ['MNR','13','seamark:precautionary_area:restriction','reduced_wake_area'], // Reduced Wake Area
    ['MNR','14','seamark:precautionary_area:restriction','avoidance_area'], // Avoidance Area
    ['MNR','15','seamark:precautionary_area:restriction','construction_prohibited'], // Construction Prohibited
    ['MNR','16','seamark:precautionary_area:restriction','discharging_prohibited'], // Discharging Prohibited
    ['MNR','17','seamark:precautionary_area:restriction','discharging_restricted'], // Discharging Restricted
    ['MNR','18','seamark:precautionary_area:restriction','exploration_and/or_development_prohibited'], // Exploration and/or Development Prohibited
    ['MNR','19','seamark:precautionary_area:restriction','exploration_and/or_development_restricted'], // Exploration and/or Development Restricted
    ['MNR','20','seamark:precautionary_area:restriction','drilling_prohibited'], // Drilling Prohibited
    ['MNR','21','seamark:precautionary_area:restriction','drilling_restricted'], // Drilling Restricted
    ['MNR','22','seamark:precautionary_area:restriction','artefact_removal_prohibited'], // Artefact Removal Prohibited
    ['MNR','23','seamark:precautionary_area:restriction','cargo_transshipment_prohibited'], // Cargo Transshipment Prohibited
    ['MNR','24','seamark:precautionary_area:restriction','dragging_prohibited'], // Dragging Prohibited
    ['MNR','25','seamark:precautionary_area:restriction','stopping_prohibited'], // Stopping Prohibited
    ['MNR','26','seamark:precautionary_area:restriction','landing_prohibited'], // Landing Prohibited
    ['MNR','27','seamark:precautionary_area:restriction','limited_speed_area'], // Limited Speed Area
    ['MNR','28','seamark:precautionary_area:restriction','navigational_aid_safety_zone'], // Navigational Aid Safety Zone
    ['MNR','29','seamark:precautionary_area:restriction','offshore_safety_zone'], // Offshore Safety Zone
    ['MNR','30','seamark:precautionary_area:restriction','shipping_prohibited'], // Shipping Prohibited
    ['MNR','31','seamark:precautionary_area:restriction','swimming_prohibited'], // Swimming Prohibited
    ['MNR','999','seamark:precautionary_area:restriction','other'], // Other

    // MNR2 - Maritime Navigation Restriction [2]
    // MNR3 - Maritime Navigation Restriction [3]

    // MNS - Man-made Shoreline
    // ['MNS','-999999',undefined,undefined], // No Information
    ['MNS','1000','man_made:shoreline','no'],
    ['MNS','1001','man_made:shoreline','yes'],

    // MRP - Maritime Radiobeacon Present
    // ['MRP','-999999',undefined,undefined], // No Information
    ['MRP','1000','radiobeacon_present','no'],
    ['MRP','1001','radiobeacon_present','yes'],

    // MRR - Maritime Area Restriction
    // ['MRR','-999999',undefined,undefined], // No Information
    ['MRR','1','seamark:restricted_area:category','unrestricted_area'], // Unrestricted Area
    ['MRR','2','seamark:restricted_area:category','bird_sanctuary'], // Bird Sanctuary
    ['MRR','3','seamark:restricted_area:category','conservation_and_management_zone'], // Conservation and Management Zone
    ['MRR','4','seamark:restricted_area:category','ecological_reserve'], // Ecological Reserve
    ['MRR','5','seamark:restricted_area:category','fish_sanctuary'], // Fish Sanctuary
    ['MRR','6','seamark:restricted_area:category','game_preserve'], // Game Preserve
    ['MRR','7','seamark:restricted_area:category','marine_nature_reserve'], // Marine Nature Reserve
    ['MRR','8','seamark:restricted_area:category','nature_reserve'], // Nature Reserve
    ['MRR','9','seamark:restricted_area:category','seal_sanctuary'], // Seal Sanctuary
    ['MRR','10','seamark:restricted_area:category','fishing_ground'], // Fishing Ground
    ['MRR','11','seamark:restricted_area:category','fishery_zone'], // Fishery Zone
    ['MRR','12','seamark:restricted_area:category','foreign_fishing_rights_zone'], // Foreign Fishing Rights Zone
    ['MRR','13','seamark:restricted_area:category','unilateral_fishing_zone'], // Unilateral Fishing Zone
    ['MRR','14','seamark:restricted_area:category','international_council_for_the_seas_(ices)_fishery_zone'], // International Council for the Seas (ICES) Fishery Zone
    ['MRR','15','seamark:restricted_area:category','compass_adjustment'], // Compass Adjustment
    ['MRR','16','seamark:restricted_area:category','degaussing_range'], // Degaussing Range
    ['MRR','17','seamark:restricted_area:category','dredging_area'], // Dredging Area
    ['MRR','18','seamark:restricted_area:category','fairway'], // Fairway
    ['MRR','19','seamark:restricted_area:category','historic_wreck_area'], // Historic Wreck Area
    ['MRR','20','seamark:restricted_area:category','incineration_area'], // Incineration Area
    ['MRR','21','seamark:restricted_area:category','joint_economic_development_zone'], // Joint Economic Development Zone
    ['MRR','22','seamark:restricted_area:category','maritime_mass_grave'], // Maritime Mass Grave
    ['MRR','23','seamark:restricted_area:category','maritime_pollution_(marpol)_reporting_area'], // Maritime Pollution (MARPOL) Reporting Area
    ['MRR','24','seamark:restricted_area:category','mine_danger_area'], // Mine Danger Area
    ['MRR','25','seamark:restricted_area:category','former_mine_danger_area'], // Former Mine Danger Area
    ['MRR','26','seamark:restricted_area:category','natural_gas_field'], // Natural Gas Field
    ['MRR','27','seamark:restricted_area:category','petroleum_field'], // Petroleum Field
    ['MRR','28','seamark:restricted_area:category','reclamation_area'], // Reclamation Area
    ['MRR','29','seamark:restricted_area:category','research'], // Research Area
    ['MRR','30','seamark:restricted_area:category','stranding_danger_area'], // Stranding Danger Area
    ['MRR','31','seamark:restricted_area:category','swimming_area'], // Swimming Area
    ['MRR','32','seamark:restricted_area:category','swinging_area'], // Swinging Area
    ['MRR','33','seamark:restricted_area:category','waiting_area'], // Waiting Area
    ['MRR','34','seamark:restricted_area:category','water_skiing_area'], // Water Skiing Area
    ['MRR','35','seamark:restricted_area:category','exclusive_economic_zone'], // Exclusive Economic Zone
    ['MRR','36','seamark:restricted_area:category','extended_continental_shelf_area'], // Extended Continental Shelf Area
    ['MRR','37','seamark:restricted_area:category','continental_shelf_area'], // Continental Shelf Area
    ['MRR','38','seamark:restricted_area:category','contiguous_zone'], // Contiguous Zone
    ['MRR','39','seamark:restricted_area:category','territorial_sea_area'], // Territorial Sea Area
    ['MRR','40','seamark:restricted_area:category','internal_waters'], // Internal Waters
    ['MRR','41','seamark:restricted_area:category','no_wake_area'], // No Wake Area
    ['MRR','42','seamark:restricted_area:category','traffic_services_limit'], // Traffic Services Limit
    ['MRR','45','seamark:restricted_area:category','environmentally_sensitive_sea_area'], // Environmentally Sensitive Sea Area
    ['MRR','46','seamark:restricted_area:category','particularly_sensitive_sea_area'], // Particularly Sensitive Sea Area
    ['MRR','47','seamark:restricted_area:category','military_area'], // Military Area
    ['MRR','52','seamark:restricted_area:category','disengagement_area'], // Disengagement Area
    ['MRR','999','seamark:restricted_area:category','other'], // Other

    // MRR2 - Maritime Area Restriction [2]
    // MRR3 - Maritime Area Restriction [3]
    // MRT - Maritime Radiobeacon Type - See BC034_MRT

    // MST - Missile Site Type
    // ['MST','-999999',undefined,undefined], // No Information
    ['MST','1','missile','anti-ballistic_missile_(abm)'], // Anti-ballistic Missile (ABM)
    ['MST','2','missile','intercontinental_ballistic_missile_(icbm)'], // Intercontinental Ballistic Missile (ICBM)
    ['MST','3','missile','intermediate-range_ballistic_missile_(irbm)'], // Intermediate-range Ballistic Missile (IRBM)
    ['MST','4','missile','sa-1_guild'], // SA-1 Guild
    ['MST','5','missile','sa-2_guideline'], // SA-2 Guideline
    ['MST','6','missile','sa-3_goa'], // SA-3 Goa
    ['MST','7','missile','sa-4_ganef'], // SA-4 Ganef
    ['MST','8','missile','sa-5_gammon'], // SA-5 Gammon
    ['MST','9','missile','sa-6_gainful'], // SA-6 Gainful
    ['MST','10','missile','sa-7_grail'], // SA-7 Grail
    ['MST','11','missile','sa-8_gecko'], // SA-8 Gecko
    ['MST','12','missile','sa-9_gaskin'], // SA-9 Gaskin
    ['MST','13','missile','medium-range_ballistic_missile_(mrbm)'], // Medium-range Ballistic Missile (MRBM)
    ['MST','14','missile','surface-to-surface_missile_(ssm)'], // Surface-to-surface Missile (SSM)
    ['MST','15','missile','surface-to-air_missile_(sam)'], // Surface-to-air Missile (SAM)
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

    // MST2 - Missile Site Type [2]
    // MST3 - Missile Site Type [3]

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

    // NDX - Neighbourhood-defining Characteristic
    // ['NDX','-999999',undefined,undefined], // No Information
    ['NDX','1','neighbourhood:characteristic','economic_activity'], // Economic Activity
    ['NDX','2','neighbourhood:characteristic','ethnicity'], // Ethnicity
    ['NDX','3','neighbourhood:characteristic','historical'], // Historical
    ['NDX','4','neighbourhood:characteristic','linguistics'], // Linguistics
    ['NDX','5','neighbourhood:characteristic','physical_structure'], // Physical Structure
    ['NDX','6','neighbourhood:characteristic','religion'], // Religion
    ['NDX','7','neighbourhood:characteristic','socioeconomics'], // Socioeconomics
    ['NDX','999','neighbourhood:characteristic','other'], // Other

    // NDX2 - Neighbourhood-defining Characteristic [2]
    // NDX3 - Neighbourhood-defining Characteristic [3]

    // NFP - Naval Firing and/or Practice Type
    // ['NFP','-999999',undefined,undefined], // No Information
    ['NFP','4','naval:practice_type','firing_danger_area'], // Firing Danger Area
    ['NFP','6','naval:practice_type','mine_laying_practice_area'], // Mine Laying Practice Area
    ['NFP','999','naval:practice_type','other'], // Other

    // NFP2 - Naval Firing and/or Practice Type [2]
    // NFP3 - Naval Firing and/or Practice Type [3]

    // NLT - Named Location Type
    // ['NLT','-999999',undefined,undefined], // No Information
    ['NLT','1','named_location:type','area'], // Area
    ['NLT','2','named_location:type','locality'], // Locality
    ['NLT','3','named_location:type','region'], // Region
    ['NLT','4','named_location:type','arctic_land'], // Arctic Land
    ['NLT','5','named_location:type','populated_locality'], // Populated Locality
    ['NLT','999','named_location:type','other'], // Other

    // NMC - Navigation Mark Colour
    // ['NMC','-999999',undefined,undefined], // No Information
    ['NMC','1','seamark:navigation_mark:colour','white'], // White
    ['NMC','2','seamark:navigation_mark:colour','black'], // Black
    ['NMC','3','seamark:navigation_mark:colour','red'], // Red
    ['NMC','4','seamark:navigation_mark:colour','green'], // Green
    ['NMC','5','seamark:navigation_mark:colour','blue'], // Blue
    ['NMC','6','seamark:navigation_mark:colour','yellow'], // Yellow
    ['NMC','7','seamark:navigation_mark:colour','grey'], // Grey
    ['NMC','8','seamark:navigation_mark:colour','brown'], // Brown
    ['NMC','9','seamark:navigation_mark:colour','amber'], // Amber
    ['NMC','10','seamark:navigation_mark:colour','violet'], // Violet
    ['NMC','11','seamark:navigation_mark:colour','orange'], // Orange
    ['NMC','12','seamark:navigation_mark:colour','magenta'], // Magenta
    ['NMC','13','seamark:navigation_mark:colour','pink'], // Pink
    ['NMC','999','seamark:navigation_mark:colour','other'], // Other

    // NMC2 - Navigation Mark Colour [2]
    // NMC3 - Navigation Mark Colour [3]

    // NOA - Naval Operations Type
    // ['NOA','-999999',undefined,undefined], // No Information
    ['NOA','14','naval:operation_type','submarine_exercise_area'], // Submarine Exercise Area
    ['NOA','27','naval:operation_type','foracs_v_limit'], // FORACS V Limit
    ['NOA','999','naval:operation_type','other'], // Other

    // NOA2 - Naval Operations Type [2]
    // NOA3 - Naval Operations Type [3]

    // NSC - Hydrographic NAVAID System
    // ['NSC','-999999',undefined,undefined], // No Information
    ['NSC','2','seamark:navaid_system','iala_region_a'], // IALA Region A
    ['NSC','3','seamark:navaid_system','iala_region_b'], // IALA Region B
    ['NSC','6','seamark:navaid_system','signi'], // SIGNI
    ['NSC','7','seamark:navaid_system','us_intracoastal_waterway'], // US Intracoastal Waterway
    ['NSC','8','seamark:navaid_system','us_uniform_state'], // US Uniform State
    ['NSC','9','seamark:navaid_system','us_western_rivers'], // US Western Rivers
    ['NSC','10','seamark:navaid_system','no_system'], // No System

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

    // ORS - Operating Restriction
    // ['ORS','-999999',undefined,undefined], // No Information
    ['ORS','3','seamark:precautionary_area:operating_restriction','no_restriction'], // No Restriction
    ['ORS','4','seamark:precautionary_area:operating_restriction','snow'], // Snow
    ['ORS','5','seamark:precautionary_area:operating_restriction','icing'], // Icing
    ['ORS','6','seamark:precautionary_area:operating_restriction','flooding'], // Flooding
    ['ORS','7','seamark:precautionary_area:operating_restriction','special_restriction'], // Special Restriction
    ['ORS','8','seamark:precautionary_area:operating_restriction','wind'], // Wind
    ['ORS','9','seamark:precautionary_area:operating_restriction','reduced_visibility'], // Reduced Visibility
    ['ORS','10','seamark:precautionary_area:operating_restriction','time'], // Time
    ['ORS','999','seamark:precautionary_area:operating_restriction','other'], // Other

    // ORS2 - Operating Restriction [2]
    // ORS3 - Operating Restriction [3]

    // OWO - Waterbody Overhead Obstruction
    // ['OWO','-999999',undefined,undefined], // No Information
    ['OWO','1000','waterway:overhead_obstruction','no'],
    ['OWO','1001','waterway:overhead_obstruction','yes'],

    // PBY - By-product - See ZI014_PBY
    // PBY2 - By-product [2] - See ZI014_PBY2
    // PBY3 - By-product [3] - See ZI014_PBY3

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

    // PFHC - Predominant Feature Height <interval closure>
    ['PFHC','2','height:feature:closure','open_interval'], // Open Interval
    ['PFHC','3','height:feature:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['PFHC','4','height:feature:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['PFHC','5','height:feature:closure','closed_interval'], // Closed Interval
    ['PFHC','6','height:feature:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['PFHC','7','height:feature:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['PFHC','8','height:feature:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['PFHC','9','height:feature:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // PLC - Pile Type
    // ['PLC','-999999',undefined,undefined], // No Information
    ['PLC','1','structural_pile:type','stake'], // Stake
    ['PLC','3','structural_pile:type','post'], // Post
    ['PLC','4','structural_pile:type','tripodal'], // Tripodal
    ['PLC','5','structural_pile:type','linear'], // Linear
    ['PLC','999','structural_pile:type','other'], // Other

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

    // PMPR - Pump Present
    // ['PMPR','-999999',undefined,undefined], // No Information
    ['PMPR','1000','have_pump','no'],
    ['PMPR','1001','have_pump','yes'],

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
    // PPO - Physical Product - See ZI014_PPO
    // PPO2 - Physical Product [2] - See ZI014_PPO2
    // PPO3 - Physical Product [3] - See ZI014_PPO3

    // PRM - Permanent
    // ['PRM','-999999',undefined,undefined], // No Information
    ['PRM','1000','permanent','no'],
    ['PRM','1001','permanent','yes'],

    // PRW - Raw Material - See ZI014_PRW

    // PTG - Portage Use
    // ['PTG','-999999',undefined,undefined], // No Information
    ['PTG','1000','portage','no'],
    ['PTG','1001','portage','yes'],

    // PWAC - Predominant Water Depth <interval closure>
    ['PWAC','2','depth:closure','open_interval'], // Open Interval
    ['PWAC','3','depth:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['PWAC','4','depth:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
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

    // RAY - Racing Type
    // ['RAY','-999999',undefined,undefined], // No Information
    ['RAY','1','racing:type','bicycle'], // Bicycle
    ['RAY','2','racing:type','camel'], // Camel
    ['RAY','3','racing:type','greyhound'], // Greyhound
    ['RAY','4','racing:type','harness'], // Harness
    ['RAY','5','racing:type','horse'], // Horse
    ['RAY','6','racing:type','ice_skate'], // Ice Skate
    ['RAY','7','racing:type','motor_vehicle'], // Motor Vehicle
    ['RAY','8','racing:type','roller_skate'], // Roller Skate
    ['RAY','9','racing:type','track_and_field'], // Track and Field
    ['RAY','999','racing:type','other'], // Other

    // RCG - Resource Content Originator - See ZI004_RCG

    // REF - Radar Reflector Present
    // ['REF','-999999',undefined,undefined], // No Information
    ['REF','1000','radar_reflector','no'],
    ['REF','1001','radar_reflector','yes'],

    // REW - Reference Water Level
    // ['REW','-999999',undefined,undefined], // No Information
    ['REW','1','water:reference','high_tide'], // High Tide
    ['REW','2','water:reference','low_tide'], // Low Tide

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

    // RMWC - Route Median Width <interval closure>
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

    // RTT - Route Intended Use
    // ['RTT','-999999',undefined,undefined], // No Information
    ['RTT','2','seamark:route','recommended_track_for_other_than_deep_draft_vessels'], // Recommended Track for Other Than Deep Draft Vessels
    ['RTT','3','seamark:route','recommended_track_for_deep_draft_vessels'], // Recommended Track for Deep Draft Vessels
    ['RTT','4','seamark:route','deep_water_route'], // Deep Water Route
    ['RTT','5','seamark:route','transit_route'], // Transit Route
    ['RTT','11','seamark:route','two-way_route'], // Two-way Route
    ['RTT','13','seamark:route','recommended_direction_of_traffic_flow'], // Recommended Direction of Traffic Flow
    ['RTT','17','seamark:route','q-route'], // Q-Route
    ['RTT','18','seamark:route','recommended_route'], // Recommended Route
    ['RTT','999','seamark:route','other'], // Other

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

    // SBT - Substation Type
    // ['SBT','-999999',undefined,undefined], // No Information
    ['SBT','1','substation','switched'], // Switched Substation
    ['SBT','2','substation','transformer'], // Transformer Substation
    ['SBT','3','substation','converter'], // Converter Substation
    ['SBT','999','substation','other'], // Other

    // SBT2 - Substation Type [2]
    // SBT3 - Substation Type [3]

    // SCOG - Sample Collector Organization
    // ['SCOG','-999999',undefined,undefined], // No Information
    ['SCOG','1','sample_collector','government_official'], // Government Official
    ['SCOG','2','sample_collector','non_government'], // Non Government
    ['SCOG','3','sample_collector','military'], // Military
    ['SCOG','4','sample_collector','educational'], // Educational
    ['SCOG','998','sample_collector','not_applicable'], // Not Applicable
    ['SCOG','999','sample_collector','other'], // Other

    // SDCC - Soil Depth <interval closure>
    ['SDCC','2','soil:depth:closure','open_interval'], // Open Interval
    ['SDCC','3','soil:depth:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['SDCC','4','soil:depth:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['SDCC','5','soil:depth:closure','closed_interval'], // Closed Interval
    ['SDCC','6','soil:depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['SDCC','7','soil:depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['SDCC','8','soil:depth:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['SDCC','9','soil:depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // SDQ - Structurally-dissected
    // ['SDQ','-999999',undefined,undefined], // No Information
    ['SDQ','1000','structurally_dissected','no'],
    ['SDQ','1001','structurally_dissected','yes'],

    // SDSC - Stem Diameter <interval closure>
    ['SDSC','2','tree:diameter:closure','open_interval'], // Open Interval
    ['SDSC','3','tree:diameter:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['SDSC','4','tree:diameter:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
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

    // SERP - Surface Permanence
    // ['SERP','-999999',undefined,undefined], // No Information
    ['SERP','6','aeroway:runway_permanence','natural'], // Natural
    ['SERP','7','aeroway:runway_permanence','permanent'], // Permanent
    ['SERP','8','aeroway:runway_permanence','temporary'], // Temporary

    // SGCC - Surface Slope <interval closure>
    ['SGCC','2','incline:interval:closure','open_interval'], // Open Interval
    ['SGCC','3','incline:interval:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['SGCC','4','incline:interval:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['SGCC','5','incline:interval:closure','closed_interval'], // Closed Interval
    ['SGCC','6','incline:interval:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['SGCC','7','incline:interval:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['SGCC','8','incline:interval:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['SGCC','9','incline:interval:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // SHD - Shoreline Delineated
    // ['SHD','-999999',undefined,undefined], // No Information
    ['SHD','1000','shoreline:delineated','no'],
    ['SHD','1001','shoreline:delineated','yes'],

    // SHO - Coast Type
    // ['SHO','-999999',undefined,undefined], // No Information
    ['SHO','1','coast:type','hillocks'], // Hillocks
    ['SHO','2','coast:type','flat_coast'], // Flat Coast
    ['SHO','7','coast:type','glacier'], // Glacier
    ['SHO','8','coast:type','ice_coast'], // Ice Coast
    ['SHO','9','coast:type','steep_coast'], // Steep Coast
    ['SHO','999','coast:type','other'], // Other

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
    // ['SLT','998',undefined,undefined], // Not Applicable
    ['SLT','998','shoreline:type','not_applicable'], // Not Applicable
    ['SLT','999','shoreline:type','other'], // Other

    // SNDC - Snow or Ice Depth <interval closure>
    ['SNDC','2','ice:depth:closure','open_interval'], // Open Interval
    ['SNDC','3','ice:depth:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['SNDC','4','ice:depth:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['SNDC','5','ice:depth:closure','closed_interval'], // Closed Interval
    ['SNDC','6','ice:depth:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['SNDC','7','ice:depth:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['SNDC','8','ice:depth:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['SNDC','9','ice:depth:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

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

    // SRL - Location Referenced to Shoreline
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

    // SSR2 - Roof Shape [2]
    // SSR3 - Roof Shape [3]

    // STA - Maritime Station Type
    // ['STA','-999999',undefined,undefined], // No Information
    ['STA','1','seamark:signal_station','coast_guard'], // Coast Guard Station
    ['STA','2','seamark:signal_station','fireboat'], // Fireboat Station
    ['STA','3','seamark:signal_station','water-police'], // Water-police Station
    ['STA','4','seamark:signal_station','ice_signal'], // Ice Signal Station
    ['STA','5','seamark:signal_station','rescue'], // Rescue Station
    ['STA','6','seamark:signal_station','port_control'], // Port Control Station
    ['STA','11','seamark:signal_station','pilot'], // Pilot Station
    ['STA','13','seamark:signal_station','signal'], // Signal Station
    ['STA','14','seamark:signal_station','signal_mast'], // Signal Mast
    ['STA','15','seamark:signal_station','storm_signal'], // Storm Signal Station
    ['STA','17','seamark:signal_station','tide'], // Tide Station
    ['STA','18','seamark:signal_station','time_ball'], // Time Ball Station
    ['STA','19','seamark:signal_station','time_signal'], // Time Signal Station
    ['STA','20','seamark:signal_station','unmanned_oceanographic'], // Unmanned Oceanographic Station
    ['STA','21','seamark:signal_station','weather_signal'], // Weather Signal Station
    ['STA','22','seamark:signal_station','fog_signal'], // Fog Signal
    ['STA','27','seamark:signal_station','tidal_stream'], // Tidal Stream Station
    ['STA','28','seamark:signal_station','traffic_signal'], // Traffic Signal Station
    ['STA','29','seamark:signal_station','bridge_signal'], // Bridge Signal Station
    ['STA','30','seamark:signal_station','lock_signal'], // Lock Signal Station
    ['STA','32','seamark:signal_station','international_port_signals'], // International Port Signals Station
    ['STA','33','seamark:signal_station','military_practice_signal'], // Military Practice Signal Station
    ['STA','35','seamark:signal_station','warning_signal'], // Warning Signal Station
    ['STA','36','seamark:signal_station','radar_surveillance'], // Radar Surveillance Station
    ['STA','37','seamark:signal_station','pilot_lookout'], // Pilot Lookout Station
    ['STA','48','seamark:signal_station','semaphore'], // Semaphore Station
    ['STA','49','seamark:signal_station','berthing_control'], // Berthing Control Station
    ['STA','50','seamark:signal_station','cable_warning'], // Cable Warning Station
    ['STA','51','seamark:signal_station','clock'], // Clock Station
    ['STA','52','seamark:signal_station','danger_warning'], // Danger Warning Station
    ['STA','53','seamark:signal_station','distress_warning'], // Distress Warning Station
    ['STA','54','seamark:signal_station','diving_warning'], // Diving Warning Station
    ['STA','55','seamark:signal_station','dock_control'], // Dock Control Station
    ['STA','56','seamark:signal_station','dredging_operations'], // Dredging Operations Station
    ['STA','57','seamark:signal_station','flood_barrage_control'], // Flood Barrage Control Station
    ['STA','58','seamark:signal_station','maritime_obstruction_warning'], // Maritime Obstruction Warning Station
    ['STA','59','seamark:signal_station','port_entry_and_departure_control'], // Port Entry and Departure Control Station
    ['STA','60','seamark:signal_station','tide_scale'], // Tide Scale Station
    ['STA','999','seamark:signal_station','other'], // Other

    // STA2 - Maritime Station Type [2]
    // STA3 - Maritime Station Type [3]

    // STL - Nomadic Seasonal Location
    // ['STL','-999999',undefined,undefined], // No Information
    ['STL','1','seasonal_location','winter'], // Winter
    ['STL','2','seasonal_location','summer'], // Summer
    ['STL','3','seasonal_location','spring'], // Spring
    ['STL','4','seasonal_location','autumn'], // Autumn
    ['STL','999','seasonal_location','other'], // Other

    // STL2 - Nomadic Seasonal Location [2]
    // STL3 - Nomadic Seasonal Location [3]

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
    ['SWT','4','spring:type','spring'], // Spring
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
    ['TRS','3','transport:type','automotive'], // Automotive
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
    ['TRS','16','transport:type','underground_railway'], // Underground Railway
    ['TRS','17','transport:type','inland_waterway'], // Inland Waterway
    ['TRS','18','transport:type','pack-road'], // Pack-road
    ['TRS','20','transport:type','taxiway'], // Taxiway
    ['TRS','23','transport:type','drove'], // Drove
    ['TRS','25','transport:type','runway'], // Runway
    ['TRS','999','transport:type','other'], // Other

    // TRS2 - Transportation System Type [2]
    // TRS3 - Transportation System Type [3]

    // TSCC - Tree Spacing <interval closure>
    ['TSCC','2','tree:spacing:closure','open_interval'], // Open Interval
    ['TSCC','3','tree:spacing:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['TSCC','4','tree:spacing:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
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

    // TSM2 - Terrain Surface Material [2]
    // TSM3 - Terrain Surface Material [3]

    // TSP - Maritime Traffic Separation Scheme Component
    // ['TSP','-999999',undefined,undefined], // No Information
    ['TSP','2','seamark:traffic_separation_scheme','boundary'], // Boundary
    ['TSP','3','seamark:traffic_separation_scheme','separation_zone'], // Separation Zone
    ['TSP','4','seamark:traffic_separation_scheme','line'], // Line
    ['TSP','5','seamark:traffic_separation_scheme','point'], // Point
    ['TSP','6','seamark:traffic_separation_scheme','inbound_lane'], // Inbound Lane
    ['TSP','7','seamark:traffic_separation_scheme','outbound_lane'], // Outbound Lane
    ['TSP','8','seamark:traffic_separation_scheme','lane_part'], // Lane Part
    ['TSP','9','seamark:traffic_separation_scheme','roundabout'], // Roundabout
    ['TSP','10','seamark:traffic_separation_scheme','crossing'], // Crossing
    ['TSP','11','seamark:traffic_separation_scheme','precautionary_area'], // Precautionary Area
    ['TSP','12','seamark:traffic_separation_scheme','inshore_traffic_zone'], // Inshore Traffic Zone
    ['TSP','999','seamark:traffic_separation_scheme','other'], // Other

    // TSSM - Terrain Subsurface Material
    // ['TSSM','-999999',undefined,undefined], // No Information
    ['TSSM','1','well:subsurface','basalt'], // Basalt
    ['TSSM','2','well:subsurface','boulders'], // Boulders
    ['TSSM','3','well:subsurface','clay'], // Clay
    ['TSSM','4','well:subsurface','conglomerate'], // Conglomerate
    ['TSSM','5','well:subsurface','dolomite'], // Dolomite
    ['TSSM','6','well:subsurface','granite'], // Granite
    ['TSSM','7','well:subsurface','gravel'], // Gravel
    ['TSSM','8','well:subsurface','igneous_rock'], // Igneous Rock
    ['TSSM','9','well:subsurface','karst'], // Karst
    ['TSSM','10','well:subsurface','limestone'], // Limestone
    ['TSSM','11','well:subsurface','marl'], // Marl
    ['TSSM','12','well:subsurface','metamorphic_rock'], // Metamorphic Rock
    ['TSSM','13','well:subsurface','mud'], // Mud
    ['TSSM','14','well:subsurface','sand'], // Sand
    ['TSSM','15','well:subsurface','schist'], // Schist
    ['TSSM','16','well:subsurface','unconsolidated'], // Unconsolidated
    ['TSSM','17','well:subsurface','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
    ['TSSM','18','well:subsurface','volcanic_rock'], // Volcanic Rock
    ['TSSM','19','well:subsurface','sandstone'], // Sandstone
    ['TSSM','20','well:subsurface','shale'], // Shale
    ['TSSM','999','well:subsurface','other'], // Other

    // TSSM2 - Terrain Subsurface Material [2]
    // TSSM3 - Terrain Subsurface Material [3]

    // TST - Cable Suspended Shape
    // ['TST','-999999',undefined,undefined], // No Information
    ['TST','1','cable:shape','symmetric_catenary'], // Symmetric Catenary
    ['TST','2','cable:shape','mountain_catenary'], // Mountain Catenary
    ['TST','3','cable:shape','overwater_catenary'], // Overwater Catenary
    ['TST','999','cable:shape','other'], // Other

    // TSTE - Water Taste
    // ['TSTE','-999999',undefined,undefined], // No Information
    ['TSTE','1','water:taste','salty_or_brackish'], // Salty or brackish
    ['TSTE','2','water:taste','alkali_or_soapy'], // Alkali or Soapy
    ['TSTE','3','water:taste','metallic'], // Metallic
    ['TSTE','4','water:taste','chemical'], // Chemical
    ['TSTE','998','water:taste','not_applicable'], // Not Applicable
    ['TSTE','999','water:taste','other'], // Other

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

    // TWS - Trees Widely Scattered
    // ['TWS','-999999',undefined,undefined], // No Information
    ['TWS','1000','tree:widely_scattered','no'],
    ['TWS','1001','tree:widely_scattered','yes'],

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

    // TZP - Topmark Shape
    // ['TZP','-999999',undefined,undefined], // No Information
    ['TZP','1','seamark:topmark_shape','cone_point_upward'], // Cone Point Upward
    ['TZP','2','seamark:topmark_shape','cone_point_downward'], // Cone Point Downward
    ['TZP','3','seamark:topmark_shape','sphere'], // Sphere
    ['TZP','4','seamark:topmark_shape','2_spheres_one_over_other'], // 2 Spheres One over Other
    ['TZP','5','seamark:topmark_shape','cylinder'], // Cylinder
    ['TZP','6','seamark:topmark_shape','board'], // Board
    ['TZP','7','seamark:topmark_shape','x-shaped'], // X-shaped
    ['TZP','8','seamark:topmark_shape','upright_cross'], // Upright Cross
    ['TZP','9','seamark:topmark_shape','cube_point_upward'], // Cube Point Upward
    ['TZP','10','seamark:topmark_shape','2_cones_point_to_point'], // 2 Cones Point to Point
    ['TZP','11','seamark:topmark_shape','2_cones_base_to_base'], // 2 Cones Base to Base
    ['TZP','12','seamark:topmark_shape','diamond'], // Diamond
    ['TZP','13','seamark:topmark_shape','2_cones_points_upward'], // 2 Cones Points Upward
    ['TZP','14','seamark:topmark_shape','2_cones_points_downward'], // 2 Cones Points Downward
    ['TZP','15','seamark:topmark_shape','besom_point_upward'], // Besom Point Upward
    ['TZP','16','seamark:topmark_shape','besom_point_downward'], // Besom Point Downward
    ['TZP','17','seamark:topmark_shape','flag'], // Flag
    ['TZP','18','seamark:topmark_shape','sphere_over_diamond'], // Sphere over Diamond
    ['TZP','19','seamark:topmark_shape','square'], // Square
    ['TZP','20','seamark:topmark_shape','horizontal_rectangle'], // Horizontal Rectangle
    ['TZP','21','seamark:topmark_shape','vertical_rectangle'], // Vertical Rectangle
    ['TZP','22','seamark:topmark_shape','trapezium_short_upward'], // Trapezium Short Upward
    ['TZP','23','seamark:topmark_shape','trapezium_short_downward'], // Trapezium Short Downward
    ['TZP','24','seamark:topmark_shape','triangle_point_upward'], // Triangle Point Upward
    ['TZP','25','seamark:topmark_shape','triangle_point_downward'], // Triangle Point Downward
    ['TZP','26','seamark:topmark_shape','circle'], // Circle
    ['TZP','27','seamark:topmark_shape','2_upright_crosses'], // 2 Upright Crosses
    ['TZP','28','seamark:topmark_shape','t-shaped'], // T-shaped
    ['TZP','29','seamark:topmark_shape','triangle_point_upward_over_circle'], // Triangle Point Upward over Circle
    ['TZP','30','seamark:topmark_shape','upright_cross_over_circle'], // Upright Cross over Circle
    ['TZP','31','seamark:topmark_shape','diamond_over_circle'], // Diamond over Circle
    ['TZP','32','seamark:topmark_shape','circle_over_triangle_point_upward'], // Circle over Triangle Point Upward
    ['TZP','33','seamark:topmark_shape','ball_over_cone'], // Ball over Cone
    ['TZP','34','seamark:topmark_shape','can_over_ball'], // Can over Ball
    ['TZP','35','seamark:topmark_shape','cone_over_ball'], // Cone over Ball
    ['TZP','36','seamark:topmark_shape','cross_over_ball'], // Cross over Ball
    ['TZP','37','seamark:topmark_shape','diamond_over_ball'], // Diamond over Ball
    ['TZP','998','seamark:topmark_shape','not_applicable'], // Not Applicable
    ['TZP','999','seamark:topmark_shape','other'], // Other

    // UMA - Underground Mine Access
    // ['UMA','-999999',undefined,undefined], // No Information
    ['UMA','1','mine:access','drift'], // Drift
    ['UMA','2','mine:access','slope'], // Slope
    ['UMA','3','mine:access','shaft'], // Shaft
    // ['UMA','998',undefined,undefined], // Not Applicable
    ['UMA','998','mine:access','not_applicable'], // Not Applicable

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

    // UTY2 - Accessible Utility Type [2]
    // UTY3 - Accessible Utility Type [3]

    // VAVC - Magnetic Variation Anomaly <interval closure>
    ['VAVC','2','magnetic_variation:closure','open_interval'], // Open Interval
    ['VAVC','3','magnetic_variation:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['VAVC','4','magnetic_variation:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['VAVC','5','magnetic_variation:closure','closed_interval'], // Closed Interval
    ['VAVC','6','magnetic_variation:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['VAVC','7','magnetic_variation:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['VAVC','8','magnetic_variation:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['VAVC','9','magnetic_variation:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

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

    // VCT2 - Void Collection Type [2]
    // VCT3 - Void Collection Type [3]

    // VDT - Vertical Datum
    // ['VDT','-999999',undefined,undefined], // No Information
    ['VDT','1','source:datum:vertical','wgs_84_ellipsoid'], // WGS 84 Ellipsoid
    ['VDT','2','source:datum:vertical','wgs_84_egm96_geoid'], // WGS 84 EGM96 Geoid
    ['VDT','3','source:datum:vertical','mean_sea_level'], // Mean Sea Level (MSL)
    ['VDT','4','source:datum:vertical','north_american_vertical_datum_(navd)_1988'], // North American Vertical Datum (NAVD) 1988
    ['VDT','5','source:datum:vertical','national_geodetic_vertical_datum_(ngvd)_1929'], // National Geodetic Vertical Datum (NGVD) 1929
    ['VDT','6','source:datum:vertical','ground_level'], // Ground Level
    ['VDT','7','source:datum:vertical','wgs_84_egm08_geoid'], // WGS 84 EGM08 Geoid
    ['VDT','998','source:datum:vertical','not_applicable'], // Not Applicable
    ['VDT','999','source:datum:vertical','other'], // Other

    // VEG - Vegetation Characteristic
    // ['VEG','-999999',undefined,undefined], // No Information
    ['VEG','8','grassland:type','grassland'], // Grassland
    ['VEG','10','grassland:type','tropical_grass'], // Tropical Grass
    ['VEG','61','vegetation','algae'], // Algae
    ['VEG','62','vegetation','sea_grass'], // Sea Grass
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

    // VIS - Light Visibility
    // ['VIS','-999999',undefined,undefined], // No Information
    ['VIS','1','light_visibility','high_intensity'], // High Intensity
    ['VIS','2','light_visibility','low_intensity'], // Low Intensity
    ['VIS','3','light_visibility','faint'], // Faint
    ['VIS','4','light_visibility','intensified'], // Intensified
    ['VIS','5','light_visibility','unintensified'], // Unintensified
    ['VIS','6','light_visibility','visibility_deliberately_restricted'], // Visibility Deliberately Restricted
    ['VIS','7','light_visibility','obscured'], // Obscured
    ['VIS','8','light_visibility','partially_obscured'], // Partially Obscured
    ['VIS','999','light_visibility','other'], // Other

    // VIS2 - Light Visibility [2]
    // VIS3 - Light Visibility [3]

    // VOA - Volcanic Activity
    // ['VOA','-999999',undefined,undefined], // No Information
    ['VOA','1','volcano:status','active'], // Active
    ['VOA','2','volcano:status','dormant'], // Dormant
    ['VOA','3','volcano:status','inactive_or_extinct'], // Inactive or Extinct

    // VSP - Vegetation Species
    // ['VSP','-999999',undefined,undefined], // No Information
    ['VSP','8','vegetation:type','cypress'], // Cypress
    ['VSP','17','vegetation:type','kelp'], // Kelp
    ['VSP','19','vegetation:type','mangrove'], // Mangrove
    ['VSP','22','vegetation:type','nipa'], // Nipa
    ['VSP','25','vegetation:type','palm'], // Palm
    ['VSP','28','vegetation:type','posidonia'], // Posidonia
    ['VSP','29','vegetation:type','reed'], // Reed
    ['VSP','30','vegetation:type','sargassum'], // Sargassum
    ['VSP','31','vegetation:type','sea-tangle'], // Sea-tangle
    ['VSP','34','vegetation:type','bamboo'], // Bamboo
    ['VSP','35','vegetation:type','water_hyacinth'], // Water Hyacinth
    ['VSP','36','vegetation:type','algae'], // Algae
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

    // WDAC - Average Water Depth <interval closure>
    ['WDAC','2','depth:average:closure','open_interval'], // Open Interval
    ['WDAC','3','depth:average:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['WDAC','4','depth:average:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['WDAC','5','depth:average:closure','closed_interval'], // Closed Interval
    ['WDAC','6','depth:average:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['WDAC','7','depth:average:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['WDAC','8','depth:average:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['WDAC','9','depth:average:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // WDST - Water Distribution System Type
    // ['WDST','-999999',undefined,undefined], // No Information
    ['WDST','1','water:distribution','none'], // None
    ['WDST','2','water:distribution','no_current_system_but_nearby_system'], // No Current System but Nearby System
    ['WDST','3','water:distribution','manual_labor-transported'], // Manual Labor-Transported
    ['WDST','4','water:distribution','water_storage_tanks/trucks'], // Water Storage Tanks/Trucks
    ['WDST','5','water:distribution','simple_gravity'], // Simple Gravity
    ['WDST','6','water:distribution','complex_gravity'], // Complex Gravity
    ['WDST','7','water:distribution','small_pumped_and_piped'], // Small Pumped and Piped
    ['WDST','8','water:distribution','large_urban_system'], // Large Urban System
    ['WDST','998','water:distribution','not_applicable'], // Not Applicable
    ['WDST','999','water:distribution','other'], // Other

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

    // WLE - Water Level Effect
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
    ['WLO','6','wreck_exposure','not_exposed'], // Not Exposed
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

    // WODR - Water Odour
    // ['WODR','-999999',undefined,undefined], // No Information
    ['WODR','1','water:odour','none'], // None
    ['WODR','2','water:odour','sulphur/rotten_egg'], // Sulphur/Rotten Egg
    ['WODR','3','water:odour','detergent'], // Detergent
    ['WODR','4','water:odour','gasoline/oil'], // Gasoline/Oil
    ['WODR','5','water:odour','chlorine'], // Chlorine
    ['WODR','6','water:odour','methane_gas/earthy'], // Methane Gas/Earthy
    ['WODR','7','water:odour','phenolic_or_chemical'], // Phenolic or Chemical
    ['WODR','998','water:odour','not_applicable'], // Not Applicable
    ['WODR','999','water:odour','other'], // Other

    // WPC - Coastal Work Type
    // ['WPC','-999999',undefined,undefined], // No Information
    ['WPC','1','coastal_work_type','land_reclamation'], // Land Reclamation
    ['WPC','2','coastal_work_type','construction_of_structures'], // Construction of Structures
    ['WPC','999','coastal_work_type','other'], // Other

    // WPC2 - Coastal Work Type [2]
    // WPC3 - Coastal Work Type [3]

    // WRCC - Water Recharge Rate <interval closure>
    ['WRCC','2','water:recharge_rate:closure','open_interval'], // Open Interval
    ['WRCC','3','water:recharge_rate:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['WRCC','4','water:recharge_rate:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['WRCC','5','water:recharge_rate:closure','closed_interval'], // Closed Interval
    ['WRCC','6','water:recharge_rate:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['WRCC','7','water:recharge_rate:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['WRCC','8','water:recharge_rate:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['WRCC','9','water:recharge_rate:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // WRT - Water Race Type
    // ['WRT','-999999',undefined,undefined], // No Information
    ['WRT','1','water:race','sluice'], // Sluice
    ['WRT','2','water:race','flume'], // Flume
    ['WRT','3','water:race','headrace'], // Headrace
    ['WRT','4','water:race','tailrace'], // Tailrace

    // WSRC - Source of Water
    // ['WSRC','-999999',undefined,undefined], // No Information
    ['WSRC','1','water:source','well/ground_water'], // Well/Ground Water
    ['WSRC','2','water:source','surface_water'], // Surface Water
    ['WSRC','3','water:source','piped_water_supply'], // Piped Water Supply
    ['WSRC','4','water:source','delivered_water_supply'], // Delivered Water Supply
    ['WSRC','5','water:source','rain_fed/cistern'], // Rain fed/Cistern
    ['WSRC','998','water:source','not_applicable'], // Not Applicable
    ['WSRC','999','water:source','other'], // Other

    // WST - Watercourse Sink Type
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

    // WTST - Water Test Type
    // ['WTST','-999999',undefined,undefined], // No Information
    ['WTST','1','water:test_type','quick_field_test'], // Quick Field Test
    ['WTST','2','water:test_type','complex_lab_test'], // Complex Lab Test
    ['WTST','998','water:test_type','not_applicable'], // Not Applicable
    ['WTST','999','water:test_type','other'], // Other

    // WTT - Water Turbulence Type
    // ['WTT','-999999',undefined,undefined], // No Information
    ['WTT','1','water:turbulence_type','spilling_breakers'], // Spilling Breakers
    ['WTT','2','water:turbulence_type','plunging_breakers'], // Plunging Breakers
    ['WTT','3','water:turbulence_type','surging_breakers'], // Surging Breakers
    ['WTT','4','water:turbulence_type','eddies'], // Eddies
    ['WTT','5','water:turbulence_type','overfalls'], // Overfalls
    ['WTT','6','water:turbulence_type','tide_rips'], // Tide Rips
    ['WTT','7','water:turbulence_type','whirlpool'], // Whirlpool
    ['WTT','8','water:turbulence_type','breakers'], // Breakers
    ['WTT','9','water:turbulence_type','confused_seas'], // Confused Seas
    ['WTT','999','water:turbulence_type','other'], // Other

    // WTYP - Window Type
    // ['WTYP','-999999',undefined,undefined], // No Information
    ['WTYP','1','window_type','open_with_bars'], // Open with Bars
    ['WTYP','2','window_type','open_without_bars'], // Open without Bars
    ['WTYP','3','window_type','plastic_with_bars'], // Plastic with Bars
    ['WTYP','4','window_type','plastic_without_bars'], // Plastic without Bars
    ['WTYP','999','window_type','other'], // Other

    // WWM - Sub-Surface Stratum Type
    // ['WWM','-999999',undefined,undefined], // No Information
    ['WWM','1','well:subsurface_stratum','basalt'], // Basalt
    ['WWM','2','well:subsurface_stratum','boulders'], // Boulders
    ['WWM','3','well:subsurface_stratum','clay'], // Clay
    ['WWM','4','well:subsurface_stratum','conglomerate'], // Conglomerate
    ['WWM','5','well:subsurface_stratum','dolomite'], // Dolomite
    ['WWM','6','well:subsurface_stratum','granite'], // Granite
    ['WWM','7','well:subsurface_stratum','gravel'], // Gravel
    ['WWM','8','well:subsurface_stratum','igneous_rock'], // Igneous Rock
    ['WWM','9','well:subsurface_stratum','karst'], // Karst
    ['WWM','10','well:subsurface_stratum','limestone'], // Limestone
    ['WWM','11','well:subsurface_stratum','marl'], // Marl
    ['WWM','12','well:subsurface_stratum','metamorphic_rock'], // Metamorphic Rock
    ['WWM','13','well:subsurface_stratum','mud'], // Mud
    ['WWM','14','well:subsurface_stratum','sand'], // Sand
    ['WWM','15','well:subsurface_stratum','schist'], // Schist
    ['WWM','16','well:subsurface_stratum','unconsolidated'], // Unconsolidated
    ['WWM','17','well:subsurface_stratum','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
    ['WWM','18','well:subsurface_stratum','volcanic_rock'], // Volcanic Rock
    ['WWM','19','well:subsurface_stratum','sandstone'], // Sandstone
    ['WWM','20','well:subsurface_stratum','shale'], // Shale
    ['WWM','999','well:subsurface_stratum','other'], // Other

    // WWM2 - Sub-Surface Stratum Type [2]
    // WWM3 - Sub-Surface Stratum Type [3]

    // WWRC - Water Withdrawal Rate <interval closure>
    ['WWRC','2','water:withdrawl_rate:closure','open_interval'], // Open Interval
    ['WWRC','3','water:withdrawl_rate:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['WWRC','4','water:withdrawl_rate:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['WWRC','5','water:withdrawl_rate:closure','closed_interval'], // Closed Interval
    ['WWRC','6','water:withdrawl_rate:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['WWRC','7','water:withdrawl_rate:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['WWRC','8','water:withdrawl_rate:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['WWRC','9','water:withdrawl_rate:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // YSU - Military Service Branch
    // ['YSU','-999999',undefined,undefined], // No Information
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

    // YWQ - Water Potability - See ZI024_YWQ
    // ZHBH_DCR - Base Hgt : Snd Meta : Vessel Draft Correction - See ZHDP_DCR
    // ZHBH_DKC - Base Hgt : Snd Meta : Bathymetric Measurement Quality Category - See ZHDP_DKC
    // ZHBH_DKC2 - Base Hgt : Snd Meta : Bathymetric Measurement Quality Category [2]  - See ZHDP_DKC2
    // ZHBH_DKC3 - Base Hgt : Snd Meta : Bathymetric Measurement Quality Category [3] - See ZHDP_DKC3
    // ZHBH_SVC - Base Hgt : Snd Meta : Sounding Velocity Correction Method - See ZHDP_SVC
    // ZHBH_TEC - Base Hgt : Snd Meta : Bathymetric Measurement Technique - See ZHDP_TEC
    // ZHBH_TEC2 - Base Hgt : Snd Meta : Bathymetric Measurement Technique [2] - See ZHDP_TEC2
    // ZHBH_TEC3 - Base Hgt : Snd Meta : Bathymetric Measurement Technique [3] - See ZHDP_TEC3
    // ZHBH_VDC - Base Hgt : Snd Meta : Sounding Datum - See ZHDP_VCD

    // ZHDP_DCR - Hydro Dpth : Snd Meta : Vessel Draft Correction
    // ['ZHDP_DCR','-999999',undefined,undefined], // No Information
    ['ZHDP_DCR','1000','source:datum:sounding:draft_correction','no'],
    ['ZHDP_DCR','1001','source:datum:sounding:draft_correction','yes'],

    // ZHDP_DKC - Hydro Dpth : Snd Meta : Bathymetric Measurement Quality Category
    // ['ZHDP_DKC','-999999',undefined,undefined], // No Information
    ['ZHDP_DKC','1','depth:hydrographic:quality','depth_known'], // Depth Known
    ['ZHDP_DKC','3','depth:hydrographic:quality','safe_clearance_known'], // Safe Clearance Known
    ['ZHDP_DKC','4','depth:hydrographic:quality','depth_unknown'], // Depth Unknown
    ['ZHDP_DKC','5','depth:hydrographic:quality','depth_doubtful'], // Depth Doubtful
    ['ZHDP_DKC','6','depth:hydrographic:quality','depth_unreliable'], // Depth Unreliable
    ['ZHDP_DKC','7','depth:hydrographic:quality','no_bottom'], // No Bottom
    ['ZHDP_DKC','8','depth:hydrographic:quality','least_depth'], // Least Depth
    ['ZHDP_DKC','9','depth:hydrographic:quality','reported_depth'], // Reported Depth
    ['ZHDP_DKC','10','depth:hydrographic:quality','unconfirmed_depth'], // Unconfirmed Depth
    ['ZHDP_DKC','11','depth:hydrographic:quality','maintained_depth'], // Maintained Depth
    ['ZHDP_DKC','12','depth:hydrographic:quality','unmaintained_depth'], // Unmaintained Depth
    ['ZHDP_DKC','998','depth:hydrographic:quality','not_applicable'], // Not Applicable
    ['ZHDP_DKC','999','depth:hydrographic:quality','other'], // Other

    // ZHDP_DKC2 - Hydro Dpth : Snd Meta : Bathymetric Measurement Quality Category [2]
    // ZHDP_DKC3 - Hydro Dpth : Snd Meta : Bathymetric Measurement Quality Category [3]

    // ZHDP_SVC - Hydro Dpth : Snd Meta : Sounding Velocity Correction Method
    // ['ZHDP_SVC','-999999',undefined,undefined], // No Information
    ['ZHDP_SVC','1','source:datum:sounding:correction','sounder_4800_calibrated'], // Sounder 4800 Calibrated
    ['ZHDP_SVC','2','source:datum:sounding:correction','sounder_1500_calibrated'], // Sounder 1500 Calibrated
    ['ZHDP_SVC','3','source:datum:sounding:correction','mathews_tables'], // Mathews Tables
    ['ZHDP_SVC','4','source:datum:sounding:correction','sound_velocity_meter'], // Sound Velocity Meter
    ['ZHDP_SVC','5','source:datum:sounding:correction','sounder_other_calibrated'], // Sounder Other Calibrated
    ['ZHDP_SVC','998','source:datum:sounding:correction','not_applicable'], // Not Applicable
    ['ZHDP_SVC','999','source:datum:sounding:correction','other'], // Other

    // ZHDP_TEC - Hydro Dpth : Snd Meta : Bathymetric Measurement Technique
    // ['ZHDP_TEC','-999999',undefined,undefined], // No Information
    ['ZHDP_TEC','1','source:datum:sounding:method','echo_sounder'], // Echo Sounder
    ['ZHDP_TEC','2','source:datum:sounding:method','side-scan_sonar'], // Side-scan Sonar
    ['ZHDP_TEC','3','source:datum:sounding:method','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
    ['ZHDP_TEC','4','source:datum:sounding:method','diver'], // Diver
    ['ZHDP_TEC','5','source:datum:sounding:method','lead-line'], // Lead-line
    ['ZHDP_TEC','6','source:datum:sounding:method','area_swept_by_wire-drag'], // Area Swept by Wire-drag
    ['ZHDP_TEC','8','source:datum:sounding:method','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
    ['ZHDP_TEC','9','source:datum:sounding:method','electromagnetic_sensor'], // Electromagnetic Sensor
    ['ZHDP_TEC','10','source:datum:sounding:method','photogrammetry'], // Photogrammetry
    ['ZHDP_TEC','11','source:datum:sounding:method','satellite_imagery'], // Satellite Imagery
    ['ZHDP_TEC','12','source:datum:sounding:method','levelling'], // Levelling
    ['ZHDP_TEC','13','source:datum:sounding:method','computer_generated'], // Computer Generated
    ['ZHDP_TEC','14','source:datum:sounding:method','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
    ['ZHDP_TEC','15','source:datum:sounding:method','bathymetric_lidar'], // Bathymetric LiDAR
    ['ZHDP_TEC','17','source:datum:sounding:method','singlebeam'], // Singlebeam
    ['ZHDP_TEC','18','source:datum:sounding:method','underwater_utility_vehicle'], // Underwater Utility Vehicle
    ['ZHDP_TEC','999','source:datum:sounding:method','other'], // Other

    // ZHDP_TEC2 - Hydro Dpth : Snd Meta : Bathymetric Measurement Technique [2]
    // ZHDP_TEC3 - Hydro Dpth : Snd Meta : Bathymetric Measurement Technique [3]

    // ZHDP_VDC - Hydro Dpth : Snd Meta : Sounding Datum
    // ['ZHDP_VDC','-999999',undefined,undefined], // No Information
    ['ZHDP_VDC','2','source:datum:sounding','high_water'], // High Water
    ['ZHDP_VDC','3','source:datum:sounding','higher_high_water'], // Higher High Water
    ['ZHDP_VDC','4','source:datum:sounding','indian_spring_low_water'], // Indian Spring Low Water
    ['ZHDP_VDC','5','source:datum:sounding','low_water'], // Low Water
    ['ZHDP_VDC','6','source:datum:sounding','lower_low_water'], // Lower Low Water
    ['ZHDP_VDC','7','source:datum:sounding','mean_high_water'], // Mean High Water
    ['ZHDP_VDC','8','source:datum:sounding','mean_high_water_neaps'], // Mean High Water Neaps
    ['ZHDP_VDC','9','source:datum:sounding','mean_high_water_springs'], // Mean High Water Springs
    ['ZHDP_VDC','10','source:datum:sounding','mean_higher_high_water'], // Mean Higher High Water
    ['ZHDP_VDC','11','source:datum:sounding','mean_low_water'], // Mean Low Water
    ['ZHDP_VDC','12','source:datum:sounding','mean_low_water_neaps'], // Mean Low Water Neaps
    ['ZHDP_VDC','13','source:datum:sounding','mean_low_water_springs'], // Mean Low Water Springs
    ['ZHDP_VDC','14','source:datum:sounding','mean_lower_low_water'], // Mean Lower Low Water
    ['ZHDP_VDC','15','source:datum:sounding','mean_sea_level'], // Mean Sea Level
    ['ZHDP_VDC','16','source:datum:sounding','mean_tide_level'], // Mean Tide Level
    ['ZHDP_VDC','17','source:datum:sounding','neap_tide'], // Neap Tide
    ['ZHDP_VDC','18','source:datum:sounding','spring_tide'], // Spring Tide
    ['ZHDP_VDC','19','source:datum:sounding','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
    ['ZHDP_VDC','20','source:datum:sounding','lowest_astronomical_tide'], // Lowest Astronomical Tide
    ['ZHDP_VDC','21','source:datum:sounding','chart_datum_(unspecified)'], // Chart Datum (Unspecified)
    ['ZHDP_VDC','22','source:datum:sounding','highest_astronomical_tide'], // Highest Astronomical Tide
    ['ZHDP_VDC','24','source:datum:sounding','mean_higher_high_water_springs'], // Mean Higher High Water Springs
    ['ZHDP_VDC','28','source:datum:sounding','highest_high_water'], // Highest High Water
    ['ZHDP_VDC','30','source:datum:sounding','indian_spring_high_water'], // Indian Spring High Water
    ['ZHDP_VDC','90','source:datum:sounding','lowest_low_water'], // Lowest Low Water
    ['ZHDP_VDC','91','source:datum:sounding','lowest_low_water_springs'], // Lowest Low Water Springs
    ['ZHDP_VDC','92','source:datum:sounding','approximate_mean_low_water_springs'], // Approximate Mean Low Water Springs
    ['ZHDP_VDC','93','source:datum:sounding','low_water_springs'], // Low Water Springs
    ['ZHDP_VDC','94','source:datum:sounding','approximate_lowest_astronomical_tide'], // Approximate Lowest Astronomical Tide
    ['ZHDP_VDC','95','source:datum:sounding','nearly_lowest_low_water'], // Nearly Lowest Low Water
    ['ZHDP_VDC','96','source:datum:sounding','approximate_mean_low_water'], // Approximate Mean Low Water
    ['ZHDP_VDC','97','source:datum:sounding','approximate_mean_lower_low_water'], // Approximate Mean Lower Low Water
    ['ZHDP_VDC','98','source:datum:sounding','approximate_mean_sea_level'], // Approximate Mean Sea Level
    ['ZHDP_VDC','99','source:datum:sounding','high_water_springs'], // High Water Springs
    ['ZHDP_VDC','100','source:datum:sounding','equinoctial_spring_low_water'], // Equinoctial Spring Low Water
    ['ZHDP_VDC','101','source:datum:sounding','local_datum'], // Local Datum
    ['ZHDP_VDC','102','source:datum:sounding','international_great_lakes_datum_1985'], // International Great Lakes Datum 1985
    ['ZHDP_VDC','103','source:datum:sounding','mean_water_level'], // Mean Water Level
    ['ZHDP_VDC','104','source:datum:sounding','lower_low_water_large_tide'], // Lower Low Water Large Tide
    ['ZHDP_VDC','105','source:datum:sounding','higher_high_water_large_tide'], // Higher High Water Large Tide
    ['ZHDP_VDC','107','source:datum:sounding','nearly_highest_high_water'], // Nearly Highest High Water
    ['ZHDP_VDC','999','source:datum:sounding','other'], // Other

    // ZI001_SPS - Source : Entity Spatial Source
    // ['ZI001_SPS','-999999',undefined,undefined], // No Information
    ['ZI001_SPS','1000','source:spatial_source','no'],
    ['ZI001_SPS','1001','source:spatial_source','yes'],

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

    // ZI001_VSC - Source : Vertical Source Category
    // ['ZI001_VSC','-999999',undefined,undefined], // No Information
    ['ZI001_VSC','1','source:vertical_source:type','dted_1'], // DTED 1
    ['ZI001_VSC','2','source:vertical_source:type','dted_2'], // DTED 2
    ['ZI001_VSC','3','source:vertical_source:type','no_elevations'], // No Elevations
    ['ZI001_VSC','4','source:vertical_source:type','reflective_surface'], // Reflective Surface
    ['ZI001_VSC','5','source:vertical_source:type','stereoscopic_imagery'], // Stereoscopic Imagery
    ['ZI001_VSC','6','source:vertical_source:type','tin_data'], // TIN Data
    ['ZI001_VSC','998','source:vertical_source:type','not_applicable'], // Not Applicable
    ['ZI001_VSC','999','source:vertical_source:type','other'], // Other

    // ZI004_RCG - Physical Object Metadata : Process Step Information : Resource Content Originator
    // ['ZI004_RCG','-999999',undefined,undefined], // No Information
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
    ['ZI004_RCG','28','attribution','defence_imagery_and_geospatial_organisation_(australia)'], // Defence Imagery and Geospatial Organisation (Australia)
    ['ZI004_RCG','29','attribution','geographic_service_(belgium)'], // Geographic Service (Belgium)
    ['ZI004_RCG','30','attribution','military_topographic_service_(bulgaria)'], // Military Topographic Service (Bulgaria)
    ['ZI004_RCG','31','attribution','mapping_and_charting_establishment_(canada)'], // Mapping and Charting Establishment (Canada)
    ['ZI004_RCG','32','attribution','geographic_service_of_the_czech_armed_forces_(czech_republic)'], // Geographic Service of the Czech Armed Forces (Czech Republic)
    ['ZI004_RCG','33','attribution','defence_acquisition_and_logistics_organization_(denmark)'], // Defence Acquisition and Logistics Organization (Denmark)
    ['ZI004_RCG','34','attribution','military_geographic_group_(estonia)'], // Military Geographic Group (Estonia)
    ['ZI004_RCG','35','attribution','topographic_service_(finland)'], // Topographic Service (Finland)
    ['ZI004_RCG','36','attribution','joint_geography_bureau_(france)'], // Joint Geography Bureau (France)
    ['ZI004_RCG','37','attribution','bundeswehr_geoinformation_office_(germany)'], // Bundeswehr Geoinformation Office (Germany)
    ['ZI004_RCG','38','attribution','hellenic_military_geographic_service_(greece)'], // Hellenic Military Geographic Service (Greece)
    ['ZI004_RCG','39','attribution','mapping_service_(hungary)'], // Mapping Service (Hungary)
    ['ZI004_RCG','40','attribution','defense_information_security_(italy)'], // Defense Information Security (Italy)
    ['ZI004_RCG','41','attribution','geospatial_information_agency_(latvia)'], // Geospatial Information Agency (Latvia)
    ['ZI004_RCG','42','attribution','military_mapping_centre_(lithuania)'], // Military Mapping Centre (Lithuania)
    ['ZI004_RCG','43','attribution','national_army_topographic_service_(moldova)'], // National Army Topographic Service (Moldova)
    ['ZI004_RCG','44','attribution','army_geographic_agency_(netherlands)'], // Army Geographic Agency (Netherlands)
    ['ZI004_RCG','45','attribution','geospatial_intelligence_organisation_(new_zealand)'], // Geospatial Intelligence Organisation (New Zealand)
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
    ['ZI004_RCG','998','attribution','not_applicable'], // Not Applicable
    ['ZI004_RCG','999','attribution','other'], // Other

    // ZI005_GNT - Geo Name Info : Geographic Name Type
    // ['ZI005_GNT','-999999',undefined,undefined], // No Information
    ['ZI005_GNT','1','name:type','conventional'], // Conventional
    ['ZI005_GNT','2','name:type','native_transliterated'], // Native Transliterated
    ['ZI005_GNT','3','name:type','variant_transliterated'], // Variant Transliterated
    ['ZI005_GNT','4','name:type','unverified_transliterated'], // Unverified Transliterated
    ['ZI005_GNT','5','name:type','native_original'], // Native Original
    ['ZI005_GNT','6','name:type','variant_original'], // Variant Original
    ['ZI005_GNT','7','name:type','unverified_original'], // Unverified Original
    ['ZI005_GNT','8','name:type','historical_transliterated'], // Historical Transliterated
    ['ZI005_GNT','9','name:type','historical_original'], // Historical Original
    ['ZI005_GNT','10','name:type','provisional'], // Provisional
    ['ZI005_GNT','999','name:type','other'], // Other

    // ZI005_GNT2 - Geo Name Info : Geographic Name Type [2]
    // ZI005_GNT3 - Geo Name Info : Geographic Name Type [3]

    // ZI005_GNTA - Geo Name Info (1) : Geographic Name Type (first side)
    // ['ZI005_GNTA','-999999',undefined,undefined], // No Information
    ['ZI005_GNTA','1','name:type:first_side','conventional'], // Conventional
    ['ZI005_GNTA','2','name:type:first_side','native_transliterated'], // Native Transliterated
    ['ZI005_GNTA','3','name:type:first_side','variant_transliterated'], // Variant Transliterated
    ['ZI005_GNTA','4','name:type:first_side','unverified_transliterated'], // Unverified Transliterated
    ['ZI005_GNTA','5','name:type:first_side','native_original'], // Native Original
    ['ZI005_GNTA','6','name:type:first_side','variant_original'], // Variant Original
    ['ZI005_GNTA','7','name:type:first_side','unverified_original'], // Unverified Original
    ['ZI005_GNTA','8','name:type:first_side','historical_transliterated'], // Historical Transliterated
    ['ZI005_GNTA','9','name:type:first_side','historical_original'], // Historical Original
    ['ZI005_GNTA','10','name:type:first_side','provisional'], // Provisional
    ['ZI005_GNTA','999','name:type:first_side','other'], // Other

    // ZI005_GNTB - Geo Name Info (2) : Geographic Name Type (second side)
    // ['ZI005_GNTB','-999999',undefined,undefined], // No Information
    ['ZI005_GNTB','1','name:type:second_side','conventional'], // Conventional
    ['ZI005_GNTB','2','name:type:second_side','native_transliterated'], // Native Transliterated
    ['ZI005_GNTB','3','name:type:second_side','variant_transliterated'], // Variant Transliterated
    ['ZI005_GNTB','4','name:type:second_side','unverified_transliterated'], // Unverified Transliterated
    ['ZI005_GNTB','5','name:type:second_side','native_original'], // Native Original
    ['ZI005_GNTB','6','name:type:second_side','variant_original'], // Variant Original
    ['ZI005_GNTB','7','name:type:second_side','unverified_original'], // Unverified Original
    ['ZI005_GNTB','8','name:type:second_side','historical_transliterated'], // Historical Transliterated
    ['ZI005_GNTB','9','name:type:second_side','historical_original'], // Historical Original
    ['ZI005_GNTB','10','name:type:second_side','provisional'], // Provisional
    ['ZI005_GNTB','999','name:type:second_side','other'], // Other

    // ZI012_DCR - Dpth Crv : Snd Meta : Vessel Draft Correction - See ZHDP_DCR
    // ZI012_DKC - Dpth Crv : Snd Meta : Bathymetric Measurement Quality Category - See ZHDP_DKC
    // ZI012_DKC2 - Dpth Crv : Snd Meta : Bathymetric Measurement Quality Category [2] - See ZHDP_DKC2
    // ZI012_DKC3 - Dpth Crv : Snd Meta : Bathymetric Measurement Quality Category [3] - See ZHDP_DKC3
    // ZI012_SVC - Dpth Crv : Snd Meta : Sounding Velocity Correction Method - See ZHDP_SVC
    // ZI012_TEC - Dpth Crv : Snd Meta : Bathymetric Measurement Technique - See ZHDP_TEC
    // ZI012_TEC2 - Dpth Crv : Snd Meta : Bathymetric Measurement Technique [2] - See ZHDP_TEC2
    // ZI012_TEC3 - Dpth Crv : Snd Meta : Bathymetric Measurement Technique [3] - See ZHDP_TEC3
    // ZI012_VDC - Dpth Crv : Snd Meta : Sounding Datum - See ZHDP_VDC

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
    ['ZI014_PBY','16','by_product','sludge'], // Sludge - PBY
    ['ZI014_PBY','17','by_product','spoil'], // Spoil - PBY
    ['ZI014_PBY','89','by_product','timber'], // Timber
    ['ZI014_PBY','999','by_product','other'], // Other

    // ZI014_PBY2 - Manufacturing Information : By-product [2]
    // ZI014_PBY3 - Manufacturing Information : By-product [3]

    // ZI014_PPO - Manufacturing Information : Physical Product
    // ['ZI014_PPO','-999999',undefined,undefined], // No Information
    ['ZI014_PPO','1','product','aircraft'], // Aircraft
    ['ZI014_PPO','2','product','aluminium'], // Aluminium
    ['ZI014_PPO','3','product','ammunition'], // Ammunition
    ['ZI014_PPO','4','product','asphalt'], // Asphalt
    ['ZI014_PPO','5','product','motor_vehicle'], // Motor Vehicle
    ['ZI014_PPO','13','product','brick'], // Brick
    ['ZI014_PPO','15','product','cement'], // Cement
    ['ZI014_PPO','16','product','chemical'], // Chemical
    ['ZI014_PPO','18','product','coal'], // Coal
    ['ZI014_PPO','20','product','coffee'], // Coffee
    ['ZI014_PPO','21','product','coke'], // Coke
    ['ZI014_PPO','23','product','concrete'], // Concrete
    ['ZI014_PPO','25','product','consumer_goods'], // Consumer Goods
    ['ZI014_PPO','26','product','copper'], // Copper
    ['ZI014_PPO','28','product','cotton'], // Cotton
    ['ZI014_PPO','32','product','desalinated_water'], // Desalinated Water
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
    ['ZI014_PPO','57','product','ice'], // Ice
    ['ZI014_PPO','58','product','iron'], // Iron
    ['ZI014_PPO','59','product','lead'], // Lead
    ['ZI014_PPO','60','product','lime'], // Lime
    ['ZI014_PPO','61','product','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
    ['ZI014_PPO','62','product','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
    ['ZI014_PPO','63','product','lumber'], // Lumber
    ['ZI014_PPO','65','product','manganese'], // Manganese
    ['ZI014_PPO','69','product','metal'], // Metal
    ['ZI014_PPO','70','product','milk'], // Milk
    ['ZI014_PPO','73','product','no_product'], // No Product
    ['ZI014_PPO','74','product','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
    ['ZI014_PPO','80','product','paper'], // Paper
    ['ZI014_PPO','83','product','petroleum'], // Petroleum
    ['ZI014_PPO','84','product','plastic'], // Plastic
    ['ZI014_PPO','87','product','prestressed_concrete'], // Prestressed Concrete
    ['ZI014_PPO','90','product','radioactive_material'], // Radioactive Material
    ['ZI014_PPO','92','product','rice'], // Rice
    ['ZI014_PPO','94','product','rubber'], // Rubber
    ['ZI014_PPO','95','product','salt'], // Salt
    ['ZI014_PPO','101','product','sewage'], // Sewage
    ['ZI014_PPO','105','product','silver'], // Silver
    ['ZI014_PPO','106','product','snow'], // Snow
    ['ZI014_PPO','109','product','steel'], // Steel
    ['ZI014_PPO','111','product','sugar'], // Sugar
    ['ZI014_PPO','114','product','textile'], // Textile
    ['ZI014_PPO','117','product','tobacco'], // Tobacco
    ['ZI014_PPO','120','product','uranium'], // Uranium
    ['ZI014_PPO','121','product','vegetation_product'], // Vegetation Product
    ['ZI014_PPO','122','product','water'], // Water
    ['ZI014_PPO','123','product','wine'], // Wine
    ['ZI014_PPO','126','product','zinc'], // Zinc
    ['ZI014_PPO','136','product','biochemical'], // Biochemical
    ['ZI014_PPO','137','product','petrochemical'], // Petrochemical
    ['ZI014_PPO','146','product','heating_steam_and/or_water'], // Heating Steam and/or Water
    ['ZI014_PPO','147','product','electronic_equipment'], // Electronic Equipment
    ['ZI014_PPO','148','product','electrical_equipment'], // Electrical Equipment
    ['ZI014_PPO','150','product','fertilizer'], // Fertilizer
    ['ZI014_PPO','154','product','munitions'], // Munitions
    ['ZI014_PPO','155','product','olive_oil'], // Olive Oil
    ['ZI014_PPO','156','product','whale_products'], // Whale Products
    ['ZI014_PPO','157','product','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
    ['ZI014_PPO','160','product','milled_grain'], // Milled Grain
    ['ZI014_PPO','165','product','clothing'], // Clothing
    ['ZI014_PPO','192','product','petroleum_lubricant'], // Petroleum Lubricant
    ['ZI014_PPO','200','product','iron_ingot'], // Iron Ingot
    ['ZI014_PPO','214','product','biodiesel'], // Biodiesel
    ['ZI014_PPO','279','product','fluorine'], // Fluorine
    ['ZI014_PPO','325','product','phosphorus'], // Phosphorus
    ['ZI014_PPO','435','product','nuclear_fuel'], // Nuclear Fuel
    ['ZI014_PPO','999','product','other'], // Other

    // ZI014_PPO2 - Manufacturing Information : Physical Product [2]
    // ZI014_PPO3 - Manufacturing Information : Physical Product [3]

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
    ['ZI014_PRW','68','raw_material','sand'], // Sand
    ['ZI014_PRW','70','raw_material','wood_fragments'], // Wood Fragments
    ['ZI014_PRW','75','raw_material','sewage'], // Sewage
    ['ZI014_PRW','78','raw_material','silver'], // Silver
    ['ZI014_PRW','79','raw_material','snow'], // Snow
    ['ZI014_PRW','83','raw_material','steel'], // Steel
    ['ZI014_PRW','84','raw_material','stone'], // Stone
    ['ZI014_PRW','85','raw_material','sugar'], // Sugar
    ['ZI014_PRW','87','raw_material','textile'], // Textile
    ['ZI014_PRW','90','raw_material','tobacco'], // Tobacco
    ['ZI014_PRW','93','raw_material','uranium'], // Uranium
    ['ZI014_PRW','94','raw_material','vegetation'], // Vegetation
    ['ZI014_PRW','96','raw_material','water'], // Water
    ['ZI014_PRW','97','raw_material','wood'], // Wood
    ['ZI014_PRW','99','raw_material','zinc'], // Zinc
    ['ZI014_PRW','118','raw_material','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
    ['ZI014_PRW','119','raw_material','biodiesel'], // Biodiesel
    ['ZI014_PRW','999','raw_material','other'], // Other

    // ZI014_PRW2 - Manufacturing Information : Raw Material [2]
    // ZI014_PRW3 - Manufacturing Information : Raw Material [3]

    // ZI015_GCUC - Geo Name Collection : Geographic Name Cartographic Usability Range <interval closure>
    ['ZI015_GCUC','2','cartographic_scale:name:closure','open_interval'], // Open Interval
    ['ZI015_GCUC','3','cartographic_scale:name:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['ZI015_GCUC','4','cartographic_scale:name:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['ZI015_GCUC','5','cartographic_scale:name:closure','closed_interval'], // Closed Interval
    ['ZI015_GCUC','6','cartographic_scale:name:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['ZI015_GCUC','7','cartographic_scale:name:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['ZI015_GCUC','8','cartographic_scale:name:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['ZI015_GCUC','9','cartographic_scale:name:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // ZI016_ROC - Route Pavement Information : Route Surface Composition #####
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

    // ZI018_WIT - Wireless : Wireless Telecommunication Type
    // ['ZI018_WIT','-999999',undefined,undefined], // No Information
    ['ZI018_WIT','1','communication:cellular_phone','yes'], // Cellular Phone
    ['ZI018_WIT','2','communication:microwave_radio_relay','yes'], // Microwave Radio Relay
    ['ZI018_WIT','3','communication:mobile_phone','yes'], // Mobile Phone
    ['ZI018_WIT','4','communication:radio_broadcast','yes'], // Radio Broadcast
    ['ZI018_WIT','5','communication:radio_telephone','yes'], // Radio Telephone
    ['ZI018_WIT','6','communication:radio-telegraph','yes'], // Radio-telegraph
    ['ZI018_WIT','7','communication:television','yes'], // Television (TV)
    ['ZI018_WIT','999','communication:other','yes'], // Other

    // ZI019_ASP - Aero Pvmt : Aerodrome Movement Area Surface Preparation Method
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

    // ZI019_ASP2 - Aero Pvmt : Aerodrome Movement Area Surface Preparation Method [2]
    // ZI019_ASP3 - Aero Pvmt : Aerodrome Movement Area Surface Preparation Method [3]

    // ZI019_ASU - Aero Pvmt : Aerodrome Movement Area Surface Composition
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
    ['ZI019_ASU','20','aeroway:surface','wood'], // Wood
    ['ZI019_ASU','21','aeroway:surface','asphalt_over_concrete'], // Asphalt Over Concrete
    ['ZI019_ASU','999','aeroway:surface','other'], // Other

    // ZI019_ASU2 - Aero Pvmt : Aerodrome Movement Area Surface Composition [2]
    // ZI019_ASU3 - Aero Pvmt : Aerodrome Movement Area Surface Composition [3]

    // ZI019_ASX - Aero Pvmt : Aerodrome Movement Area Surface Category
    // ['ZI019_ASX','-999999',undefined,undefined], // No Information
    ['ZI019_ASX','1','aeroway:pavement','paved'], // Completely Paved
    ['ZI019_ASX','2','aeroway:pavement','mostly_paved'], // Mostly Paved
    ['ZI019_ASX','3','aeroway:pavement','unprepared'], // Unprepared
    ['ZI019_ASX','4','aeroway:pavement','partially_paved'], // Partially Paved
    ['ZI019_ASX','5','aeroway:pavement','unpaved'], // Unpaved
    ['ZI019_ASX','999','aeroway:pavement','other'], // Other

    // ZI019_SFS - Aero Pvmt : Aerodrome Pavement Functional Status
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

    // NOTE: These are validated in post-processing
    // ZI020_GE4 - Designation : GENC Short URN-based Identifier
    // ZI020_GE42 - Designation : GENC Short URN-based Identifier [2]
    // ZI020_GE43 - Designation : GENC Short URN-based Identifier [3]
    // ZI020_GE44 - Designation : GENC Short URN-based Identifier [4]
    // ZI020_GE4A - Designation : GENC Short URN-based Identifier
    // ZI020_GE4B - Designation : GENC Short URN-based Identifier

    // ZI024_DRL - Water : Device Relative Location
    // ['ZI024_DRL','-999999',undefined,undefined], // No Information
    ['ZI024_DRL','17','device:location','on_waterbody_bottom'], // On Waterbody Bottom
    ['ZI024_DRL','40','device:location','below_ground_surface'], // Below Ground Surface
    ['ZI024_DRL','44','device:location','on_surface'], // On Surface
    ['ZI024_DRL','45','device:location','above_surface'], // Above Surface
    ['ZI024_DRL','50','device:location','submerged'], // Submerged
    ['ZI024_DRL','999','device:location','other'], // Other

    // ZI024_DVPT - Water : Device Power Type
    // ['ZI024_DVPT','-999999',undefined,undefined], // No Information
    ['ZI024_DVPT','1','device:power','electricity'], // Electricity
    ['ZI024_DVPT','2','device:power','petroleum_product'], // Petroleum Product
    ['ZI024_DVPT','3','device:power','manual'], // Manual
    ['ZI024_DVPT','4','device:power','gravity'], // Gravity
    ['ZI024_DVPT','5','device:power','compressed_air'], // Compressed Air
    ['ZI024_DVPT','6','device:power','steam'], // Steam
    ['ZI024_DVPT','7','device:power','solar'], // Solar
    ['ZI024_DVPT','999','device:power','other'], // Other

    // ZI024_HYP - Water : Hydrologic Persistence
    // ['ZI024_HYP','-999999',undefined,undefined], // No Information
    ['ZI024_HYP','1','intermittent','no'], // Perennial
    ['ZI024_HYP','2','intermittent','yes'], // Intermittent
    ['ZI024_HYP','4','intermittent','dry'], // Dry

    // ZI024_PMP - Water : Pump Type
    // ['ZI024_PMP','-999999',undefined,undefined], // No Information
    ['ZI024_PMP','1','pump:type','airlift'], // Airlift
    ['ZI024_PMP','2','pump:type','centrifugal'], // Centrifugal
    ['ZI024_PMP','3','pump:type','jet'], // Jet
    ['ZI024_PMP','4','pump:type','reciprocating'], // Reciprocating
    ['ZI024_PMP','5','pump:type','turbine'], // Turbine
    ['ZI024_PMP','6','pump:type','gravity'], // Gravity
    ['ZI024_PMP','7','pump:type','vacuum'], // Vacuum
    ['ZI024_PMP','8','pump:type','rotary'], // Rotary
    ['ZI024_PMP','999','pump:type','other'], // Other

    // ZI024_PUR - Water : Purification Process
    // ['ZI024_PUR','-999999',undefined,undefined], // No Information
    ['ZI024_PUR','1','water_purification','desalination'], // Desalination
    ['ZI024_PUR','2','water_purification','filtration'], // Filtration
    ['ZI024_PUR','3','water_purification','sedimentation/coagulation'], // Sedimentation/coagulation
    ['ZI024_PUR','4','water_purification','reverse_osmosis'], // Reverse Osmosis
    ['ZI024_PUR','5','water_purification','electrolysis'], // Electrolysis
    ['ZI024_PUR','6','water_purification','disinfection'], // Disinfection
    ['ZI024_PUR','7','water_purification','blending'], // Blending
    ['ZI024_PUR','8','water_purification','freezing'], // Freezing
    ['ZI024_PUR','9','water_purification','multi-stage_flash'], // Multi-stage flash
    ['ZI024_PUR','10','water_purification','multi-effect_evaporation'], // Multi-effect evaporation
    ['ZI024_PUR','11','water_purification','vapor_compression'], // Vapor Compression
    ['ZI024_PUR','999','water_purification','other'], // Other

    // ZI024_PUR2 - Water : Purification Process [2]
    // ZI024_PUR3 - Water : Purification Process [3]
    // ZI024_PUR4 - Water : Purification Process [4]
    // ZI024_PUR5 - Water : Purification Process [5]

    // ZI024_RTP - Water : Reservoir Type
    // ['ZI024_RTP','-999999',undefined,undefined], // No Information
    ['ZI024_RTP','1','reservoir:type','constructed_basin'], // Constructed Basin
    ['ZI024_RTP','2','reservoir:type','back-up_water_impounded_by_a_dam'], // Back-up Water Impounded by a Dam
    ['ZI024_RTP','999','reservoir:type','other'], // Other

    // ZI024_SCC - Water : Water Type
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

    // ZI024_SQN - Water : Water Flow Rate Category
    // ['ZI024_SQN','-999999',undefined,undefined], // No Information
    ['ZI024_SQN','1','water:flow_rate','small'], // Small
    ['ZI024_SQN','2','water:flow_rate','moderate'], // Moderate
    ['ZI024_SQN','3','water:flow_rate','large'], // Large
    ['ZI024_SQN','999','water:flow_rate','other'], // Other

    // ZI024_WPAC - Water : Water Pump Accessibility
    // ['ZI024_WPAC','-999999',undefined,undefined], // No Information
    ['ZI024_WPAC','100','pump:accessibility','from_left'], // From Left
    ['ZI024_WPAC','101','pump:accessibility','from_right'], // From Right
    ['ZI024_WPAC','102','pump:accessibility','from_all_sides'], // From All Sides
    ['ZI024_WPAC','103','pump:accessibility','no_accessibility'], // No Accessibility
    ['ZI024_WPAC','104','pump:accessibility','from_north'], // From North
    ['ZI024_WPAC','105','pump:accessibility','from_south'], // From South
    ['ZI024_WPAC','106','pump:accessibility','from_east'], // From East
    ['ZI024_WPAC','107','pump:accessibility','from_west'], // From West

    // ZI024_WSG - Water : Water Stage
    // ['ZI024_WSG','-999999',undefined,undefined], // No Information
    ['ZI024_WSG','1','water:stage','low'], // Low
    ['ZI024_WSG','2','water:stage','mean'], // Mean
    ['ZI024_WSG','3','water:stage','high'], // High
    ['ZI024_WSG','999','water:stage','other'], // Other

    // ZI024_WUR - Water : Water Use
    // ['ZI024_WUR','-999999',undefined,undefined], // No Information
    ['ZI024_WUR','1','water:use','agricultural_irrigation'], // Agricultural Irrigation
    ['ZI024_WUR','2','water:use','commercial'], // Commercial
    ['ZI024_WUR','3','water:use','domestic_irrigation'], // Domestic Irrigation
    ['ZI024_WUR','4','water:use','industrial'], // Industrial
    ['ZI024_WUR','5','water:use','institutional'], // Institutional
    ['ZI024_WUR','6','water:use','livestock'], // Livestock
    ['ZI024_WUR','7','water:use','municipal'], // Municipal
    ['ZI024_WUR','8','water:use','power_generation'], // Power Generation
    ['ZI024_WUR','9','water:use','recreational'], // Recreational
    ['ZI024_WUR','10','water:use','sanitary_domestic'], // Sanitary Domestic
    ['ZI024_WUR','11','water:use','military'], // Military
    ['ZI024_WUR','999','water:use','other'], // Other

    // ZI024_WUR2 - Water : Water Use [2]
    // ZI024_WUR3 - Water : Water Use [3]
    // ZI024_WUR4 - Water : Water Use [4]

    // ZI024_YWQ - Water : Water Potability
    // ['ZI024_YWQ','-999999',undefined,undefined], // No Information
    ['ZI024_YWQ','1','water:potable','yes'], // Potable
    ['ZI024_YWQ','2','water:potable','treatable'], // Treatable
    ['ZI024_YWQ','3','water:potable','contaminated'], // Contaminated
    ['ZI024_YWQ','4','water:potable','no'], // Nonpotable
    ['ZI024_YWQ','999','water:potable','other'], // Other

    // ZI025_MAN - Vert Pos : Maritime Navigation Marked - See MAN
    // ['ZI025_MAN','-999999',undefined,undefined], // No Information
    // ['ZI025_MAN','1000','raw:ZI025_MAN','no'],
    // ['ZI025_MAN','1001','raw:ZI025_MAN','yes'],

    // ZI025_UHS - Vert Pos : Uncovering Height Known
    // ['ZI025_UHS','-999999',undefined,undefined], // No Information
    ['ZI025_UHS','1000','hydrographic_depth_information','uncovering_height_unknown'],
    ['ZI025_UHS','1001','hydrographic_depth_information','uncovering_height_known'],

    // ZI025_WLE - Vert Pos : Water Level Effect - See WLE
    // ['ZI025_WLE','-999999',undefined,undefined], // No Information
    // ['ZI025_WLE','1','raw:ZI025_WLE','partly_submerged'], // Partly Submerged
    // ['ZI025_WLE','2','raw:ZI025_WLE','always_dry'], // Always Dry
    // ['ZI025_WLE','3','raw:ZI025_WLE','always_submerged'], // Always Submerged
    // ['ZI025_WLE','4','raw:ZI025_WLE','covers_and_uncovers'], // Covers and Uncovers
    // ['ZI025_WLE','5','raw:ZI025_WLE','awash_at_low_water'], // Awash at Low Water
    // ['ZI025_WLE','9','raw:ZI025_WLE','awash_at_chart_datum'], // Awash at Chart Datum
    // ['ZI025_WLE','999','raw:ZI025_WLE','other'], // Other

    // ZI026_CTUC - Physical Object Metadata : Cartographic Topography Usability Range <interval closure>
    ['ZI026_CTUC','2','cartographic_scale:closure','open_interval'], // Open Interval
    ['ZI026_CTUC','3','cartographic_scale:closure','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
    ['ZI026_CTUC','4','cartographic_scale:closure','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
    ['ZI026_CTUC','5','cartographic_scale:closure','closed_interval'], // Closed Interval
    ['ZI026_CTUC','6','cartographic_scale:closure','greater-than_semi-interval'], // Greater-than Semi-interval
    ['ZI026_CTUC','7','cartographic_scale:closure','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
    ['ZI026_CTUC','8','cartographic_scale:closure','less-than_semi-interval'], // Less-than Semi-interval
    ['ZI026_CTUC','9','cartographic_scale:closure','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

    // ZI026_SUR - Physical Object Metadata : Survey Coverage Category
    // ['ZI026_SUR','-999999',undefined,undefined], // No Information
    ['ZI026_SUR','1','source:surveyed','yes'], // Surveyed
    ['ZI026_SUR','2','source:surveyed','inadequately_surveyed'], // Inadequately Surveyed
    ['ZI026_SUR','3','source:surveyed','no'], // Unsurveyed

    // ZI032_GUG - Pylon Information : Guyed - See GUG
    // ['ZI032_GUG','-999999',undefined,undefined], // No Information
    // ['ZI032_GUG','1000','raw:ZI032_GUG','no'],
    // ['ZI032_GUG','1001','raw:ZI032_GUG','yes'],

    // ZI032_PYC - Pylon Information : Pylon Configuration - See PYC
    // ['ZI032_PYC','-999999',undefined,undefined], // No Information
    // ['ZI032_PYC','1','raw:ZI032_PYC','a"'], // A"
    // ['ZI032_PYC','2','raw:ZI032_PYC','h"'], // H"
    // ['ZI032_PYC','3','raw:ZI032_PYC','i"'], // I"
    // ['ZI032_PYC','4','raw:ZI032_PYC','y"'], // Y"
    // ['ZI032_PYC','5','raw:ZI032_PYC','t"'], // T"
    // ['ZI032_PYC','999','raw:ZI032_PYC','other'], // Other

    // ZI032_PYM - Pylon Information : Pylon Material - See PYM
    // ['ZI032_PYM','-999999',undefined,undefined], // No Information
    // ['ZI032_PYM','1','raw:ZI032_PYM','aluminium'], // Aluminium
    // ['ZI032_PYM','3','raw:ZI032_PYM','concrete'], // Concrete
    // ['ZI032_PYM','4','raw:ZI032_PYM','masonry'], // Masonry
    // ['ZI032_PYM','5','raw:ZI032_PYM','metal'], // Metal
    // ['ZI032_PYM','7','raw:ZI032_PYM','wood'], // Wood
    // ['ZI032_PYM','8','raw:ZI032_PYM','steel'], // Steel
    // ['ZI032_PYM','9','raw:ZI032_PYM','fibreglass'], // Fibreglass
    // ['ZI032_PYM','10','raw:ZI032_PYM','iron'], // Iron
    // ['ZI032_PYM','999','raw:ZI032_PYM','other'], // Other

    // ZI032_TOS - Pylon Information : Tower Shape - See TOS
    // ['ZI032_TOS','-999999',undefined,undefined], // No Information
    // ['ZI032_TOS','6','raw:ZI032_TOS','mast'], // Mast
    // ['ZI032_TOS','8','raw:ZI032_TOS','pole'], // Pole
    // ['ZI032_TOS','11','raw:ZI032_TOS','tripod'], // Tripod
    // ['ZI032_TOS','12','raw:ZI032_TOS','truss'], // Truss
    // ['ZI032_TOS','13','raw:ZI032_TOS','tubular'], // Tubular
    // ['ZI032_TOS','999','raw:ZI032_TOS','other'], // Other

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

    // ZI037_RFA - Religious Information : Religious Site Type
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
    // ZI071_FFN2 - Cave Information : Feature Function [2] - See FFN2
    // ZI071_FFN3 - Cave Information : Feature Function [3] - See FFN3

    // ZI071_UAO - Cave Information : Underground Access Orientation
    // ['ZI071_UAO','-999999',undefined,undefined], // No Information
    ['ZI071_UAO','1','direction','horizontal'], // Horizontal
    ['ZI071_UAO','2','direction','slopes_downward'], // Slopes Downward
    ['ZI071_UAO','3','direction','vertical_down'], // Vertical Down
    ['ZI071_UAO','4','direction','slopes_upward'], // Slopes Upward
    ['ZI071_UAO','5','direction','vertical_up'], // Vertical Up

    // ZSAX_RS0 - Restriction : Security <resource classification>
    ['ZSAX_RS0','C','security:classification','CONFIDENTIAL'], // (U) CONFIDENTIAL
    ['ZSAX_RS0','R','security:classification','RESTRICTED'], // (U) RESTRICTED
    ['ZSAX_RS0','S','security:classification','SECRET'], // (U) SECRET
    ['ZSAX_RS0','TS','security:classification','TOP_SECRET'], // (U) TOP SECRET
    ['ZSAX_RS0','U','security:classification','UNCLASSIFIED'], // (U) UNCLASSIFIED
    ['ZSAX_RS0','noInformation','security:classification','no_information'], // No Information
    ['ZSAX_RS0','other','security:classification','other'], // Other

    // ZVH_VDT - Highest Elevation <vertical datum> - See VDT

  ], // End one2one

  // Input Translation Rules:
  // These are only used for IMPORT and get added to the standard one2one rules
  one2oneIn : [
  ], // End one2oneIn

  // One2one translation table for converting "Other" OSM attributes to GGDM
  // This is for Export only. The values are swapped before use
  one2oneOut : [
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

    // Funky Road Type attributes
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

    ['FIC','1','embankment:type','mound'], // Mound
    ['FIC','1','embankment','berm'], // Dyke
    ['FIC','1','embankment:type','berm'], // Dyke
    ['FIC','2','embankment:type','fill'], // Fill
    ['FIC','3','embankment','dyke'], // Dyke
    ['FIC','3','embankment:type','dyke'], // Dyke
    ['FIC','5','embankment:type','levee'], // Levee
    ['FIC','6','embankment:type','divider'], // Divider
    ['FIC','999','embankment:type','other'], // Other

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

    ['VDT','meanSeaLevel','source:datum:vertical','mean_sea_level_(msl)'], // Mean Sea Level (MSL)
    ['ZI004_RCG','28','attribution','australian_geospatial-intelligence_organization_(australia)'], // Australian Geospatial-Intelligence Organization (Australia)
    ['ZI004_RCG','36','attribution','bureau_geographie,_hydrographie,_oceanographie_et_meteorologie_(france)'], // Bureau Geographie, Hydrographie, Oceanographie et Meteorologie (France)
    ['ZI004_RCG','39','attribution','geoinformation_service_of_the_hungarian_defence_forces_(hungary)'], // Geoinformation Service of the Hungarian Defence Forces (Hungary)
    ['ZI004_RCG','45','attribution','geoint_new_zealand_(new_zealand)'], // GEOINT New Zealand (New Zealand)

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

    ['ONE','1001','oneway','-1'], // Yes, it is one way but it is reversed from the drawing order

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
    // [undefined,undefined,'historic','yes'], // Not a good translation, push to o2s

    // ### From OSM - This list could never end.....
    ['FFN','464','shop','books'], // Specialized Store
    ['FFN','563','building','house'], // Residence
    ['FFN','558','building','dependents_housing'], // Dependents Housing
    ['FFN','610','office','telecommunication'], // telecommunication
    ['FFN','640','shop','bureau_de_change'], // Financial Services
    ['FFN','760','amenity','office'], // Business and Personal Support Services
    ['FFN','815','building','palace'], // Palace
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

  // ##### Start of txtLength #####
  // This list is for validateing the lengths of text attributes prior to export
  txtLength : {
    'BA000_VDR':80, 'BC040_COL':254, 'BC040_MLR':254, 'BEN':15, 'BER':254, 'BRN':24, 'BRR':14, 'CID':20, 'CNCP':20,
    'COL':80, 'DIA':14, 'ETZ':24, 'FUFI':254, 'F_CODE':5, 'GA032_NSX':80, 'GB001_AID':14, 'GB052_RIDH':14,
    'GB052_RIDL':14, 'HSE':14, 'IKO':14, 'LASZ':254, 'MCA':80, 'MDE':20, 'MLR':254, 'NA8':80, 'PRVR':254,
    'PSE':14, 'RIN_RTN':24, 'RIN_RTN2':24, 'RIN_RTN3':24, 'RTL':254, 'RTN':24, 'RTN2':24, 'RTN3':24, 'SCVN':254,
    'SSE':14, 'UFI':254, 'URI':254, 'VCS_VCR':132, 'VOI':14, 'WPI':14, 'WTCL':254, 'ZHBH_VDR':80, 'ZHDP_VDR':80,
    'ZI001_SDV':20, 'ZI001_VSD':20, 'ZI002_UFI':254, 'ZI005_FNA':200, 'ZI005_FNA2':200, 'ZI005_FNA3':200,
    'ZI005_FNAA':200, 'ZI005_FNAB':200, 'ZI005_NFN':18, 'ZI005_NFN2':18, 'ZI005_NFN3':18, 'ZI005_NFNA':18,
    'ZI005_NFNB':18, 'ZI012_VDR':80, 'ZI020_FI2A':14, 'ZI020_FI2B':14, 'ZI020_IC4':200, 'ZI031S_URI':254,
    'ZI039S_UFI':254, 'ZI103_MVD':20, 'ZSAX_RX0':254, 'ZSAX_RX3':254, 'ZSAX_RX4':254
  },
  // ##### End of txtLength #####

  // ##### Start of intList #####
  // This list is for validateing the integer attributes prior to export
  intList : [
    'ADUR','BC034_BRF','BC034_BRF2','BC040_LCN','BC040_LVN','BNF','BRF','BRF2','DEV','DF1','DF2','DF3',
    'DF4','DMF','DZC','EQS','HSC','LC1','LC2','LC3','LC4','LCN','LSA','LSI','LST','LTN','LVN',
    'NOR','NOS','NPL','PPL','STAF','STNB','VEC','WPST','ZI005_GNR','ZI005_GNR2','ZI005_GNR3',
    'ZI015_GCUL','ZI015_GCUU','ZI018_BRF','ZI026_CTUL','ZI026_CTUU'
  ],
  // ##### End of intList #####

  // ##### Start of swapListIn #####
  // The What Were They Thinking? swap list.  Each of these is the _same_ attribute
  // but renamed in different features. We swap these so that there is only one
  // set of rules needed in the One2One section.
  // These get converted back on output - if we need to.
  swapListIn : {
    'ASU':'ZI019_ASU', 'ASU2':'ZI019_ASU3', 'ASU3':'ZI019_ASU3',
    'AT005_CAB':'CAB', 'AT005_CAB2':'CAB2', 'AT005_CAB3':'CAB3',
    'BA000_VDC':'ZHDP_VDC',
    'BA000_VDR':'ZHDP_VDR',
    'BC010_REF':'REF',
    'BC010_TZP':'TZP',
    'BC040_CAA':'CAA',
    'BH010_CAA':'CAA',
    'BH010_TZP':'TZP',
    'EOL':'BC040_EOL',
    'BRF':'BC034_BRF',
    'BRF2':'BC034_BRF2',
    'COL':'BC040_COL',
    'DKC':'ZHDP_DKC',
    'DKC2':'ZHDP_DKC2',
    'DKC3':'ZHDP_DKC3',
    'HDP':'ZI025_HDP',
    'HYP':'ZI024_HYP',
    // 'LEN_':'LZN',
    'LCN':'BC040_LCN',
    'MEM':'ZI006_MEM',
    'MLR':'BC040_MLR',
    'MRT':'BC034_MRT',
    'PFD':'PWA',
    'PBY':'ZI014_PBY', 'PBY2':'ZI014_PBY2', 'PBY3':'ZI014_PBY3',
    'PPO':'ZI014_PPO', 'PPO2':'ZI014_PPO2', 'PPO3':'ZI014_PPO3',
    'PRW':'ZI014_PRW', 'PRW2':'ZI014_PRW2', 'PRW3':'ZI014_PRW3',
    'RCG':'ZI004_RCG',
    'RTN':'RIN_RTN', 'RTN2':'RIN_RTN2', 'RTN3':'RIN_RTN3',
    'SUR':'ZI026_SUR',
    'WBD':'PWA',
    'WD1':'ZI016_WD1',
    'WTBB':'ZI024_WTBB',
    'WTCA':'ZI024_WTCA',
    'WTFE':'ZI024_WTFE',
    'WTKK':'ZI024_WTKK',
    'WTMN':'ZI024_WTMN',
    'WTNA':'ZI024_WTNA',
    'WTNO':'ZI024_WTNO',
    'WTPO':'ZI024_WTPO',
    'WTSI':'ZI024_WTSI',
    'YWQ':'ZI024_YWQ',
    'ZHBH_DCR':'ZHDP_DCR',
    'ZHBH_DKC':'ZHDP_DKC',
    'ZHBH_SVC':'ZHDP_SVC',
    'ZHBH_TEC':'ZHDP_TEC',
    'ZHBH_TEC2':'ZHDP_TEC2',
    'ZHBH_TEC3':'ZHDP_TEC3',
    'ZHBH_VDC':'ZHDP_VDC',
    'ZHBH_VDR':'ZHDP_VDR',
    'ZI012_DCR':'ZHDP_DCR',
    'ZI012_DKC':'ZHDP_DKC',
    'ZI012_DKC2':'ZHDP_DKC2',
    'ZI012_DKC3':'ZHDP_DKC3',
    'ZI012_SVC':'ZHDP_SVC',
    'ZI012_TEC':'ZHDP_TEC',
    'ZI012_TEC2':'ZHDP_TEC2',
    'ZI012_TEC3':'ZHDP_TEC3',
    'ZI012_VDC':'ZHDP_VDC',
    'ZI012_VDR':'ZHDP_VDR',
    'ZI018_BRF':'BC034_BRF',
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
  // Format is: <FCODE>:[<from>:<to>]
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
    'AP010':{'RIN_RTN':'RTN','RIN_RTN2':'RTN2','RIN_RTN3':'RTN3'},
    'AP055':{'RIN_RTN':'RTN','RIN_RTN2':'RTN2','RIN_RTN3':'RTN3'},
    'AQ059':{'ZI016_WD1':'WD1'},
    'AQ113':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AQ116':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'AT005':{'WLE':'ZI025_WLE'},
    'AT042':{'GUG':'ZI032_GUG','PYC':'ZI032_PYC','PYM':'ZI032_PYM',
      'TOS':'ZI032_TOS','CAB':'AT005_CAB'},
    'BA010':{'ZHDP_VDR':'BA000_VDR','ZHDP_VDC':'BA000_VDC'},
    'BA030':{'ZHDP_VDR':'BA000_VDR','ZHDP_VDC':'BA000_VDC'},
    'BB010':{'ZI025_HDP':'HDP'},
    'BB155':{'BC034_BRF':'ZI018_BRF'},
    'BC034':{'BC034_BRF':'BRF','BC034_BRF2':'BRF2','BC010_REF':'REF',
      'BC010_TZP':'TZP','BC034_MRT':'MRT'},
    'BC040':{'BC040_EOL':'EOL','BC010_REF':'REF','BC040_COL':'COL',
      'BC040_MLR':'MLR','BC010_TZP':'TZP'},
    'BC060':{'BC040_COL':'COL','CAA':'BC040_CAA'},
    'BC080':{'ZHDP_VDR':'ZHBH_VDR','ZHDP_DKC':'ZHBH_DKC','ZHDP_DKC2':'ZHBH_DKC2',
      'ZHDP_DKC3':'ZHBH_DKC3','ZHDP_DCR':'ZHBH_DCR','ZHDP_VDC':'ZHBH_VDC',
      'ZHDP_SVC':'ZHBH_SVC','ZHDP_TEC':'ZHBH_TEC','ZHDP_TEC2':'ZHBH_TEC2',
      'ZHDP_TEC3':'ZHBH_TEC3'},
    'BD020':{'ZHDP_TEC':'ZHBH_TEC','ZHDP_TEC2':'ZHBH_TEC2','ZHDP_TEC3':'ZHBH_TEC3'},
    'BD100':{'WLE':'ZI025_WLE'},
    'BD115':{'MAN':'ZI025_MAN'},
    'BD130':{'ZHDP_TEC':'ZHBH_TEC','ZHDP_TEC2':'ZHBH_TEC2','ZHDP_TEC3':'ZHBH_TEC3'},
    'BE010':{'ZHDP_VDR':'ZI012_VDR','ZHDP_DKC':'ZI012_DKC','ZHDP_DKC2':'ZI012_DKC2',
      'ZHDP_DKC3':'ZI012_DKC3','ZHDP_DCR':'ZI012_DCR',
      'ZHDP_VDC':'ZI012_VDC','ZHDP_SVC':'ZI012_SVC',
      'ZHDP_TEC':'ZI012_TEC','ZHDP_TEC2':'ZI012_TEC2','ZHDP_TEC3':'ZI012_TEC3'},
    'BE020':{'ZI025_HDP':'HDP'},
    'BG010':{'ZI025_HDP':'HDP'},
    'BH012':{'CAA':'BH010_CAA'},
    'BH020':{'ZI025_HDP':'HDP'},
    'BH030':{'ZI025_HDP':'HDP'},
    'BH051':{'ZI014_PPO':'PPO','ZI014_PPO2':'PPO2','ZI014_PPO3':'PPO3'},
    'BH070':{'PWA':'WBD'},
    'BI030':{'ZI025_HDP':'HDP'},
    'BI044':{'ZI025_HDP':'HDP'},
    'BI045':{'ZI025_HDP':'HDP'},
    'DB029':{'FFN':'ZI071_FFN','FFN2':'ZI071_FFN2','FFN3':'ZI071_FFN3'},
    'DB070':{'ZI025_HDP':'HDP'},
    'ED010':{'ZI024_HYP':'HYP'},
    'ED020':{'ZI025_HDP':'HDP'},
    'FC034':{'ZI025_HDP':'HDP'},
    'FC165':{'ZI025_HDP':'HDP'},
    'FC177':{'ZI025_HDP':'HDP'},
    'GB013':{'BC040_COL':'COL','BC040_LCN':'LCN'},
    'GB045':{'ZI019_ASU':'ASU','ZI019_ASU2':'ASU2','ZI019_ASU3':'ASU3'},
    'NM020':{'ZI025_HDP':'HDP'},
    'ZD070':{'ZI024_WTBB':'WTBB','ZI024_WTCA':'WTCA','ZI024_WTFE':'WTFE',
      'ZI024_WTKK':'WTKK','ZI024_WTMN':'WTMN','ZI024_WTNA':'WTNA',
      'ZI024_WTNO':'WTNO','ZI024_WTPO':'WTPO','ZI024_WTSI':'WTSI'},
    'ZI031':{'ZI006_MEM':'MEM','ZI004_RCG':'RCG'},
    'ZI026':{'ZI026_SUR':'SUR'}
  },
  // ##### End of swapListOut #####

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
    'PFHC':['PFHL','PFHU'],
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

  // ##### Start of ge4List #####
  // This list is for validateing the XXX_GE4XX values
  ge4List : {
    'ge:GENC:3:1-2:ABW':'AW', // ARUBA AS OF 2013-06-30
    'ge:GENC:3:1-2:AFG':'AF', // AFGHANISTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:AGO':'AO', // ANGOLA AS OF 2013-06-30
    'ge:GENC:3:1-2:ALB':'AL', // ALBANIA AS OF 2013-06-30
    'ge:GENC:3:1-2:AND':'AD', // ANDORRA AS OF 2013-06-30
    'ge:GENC:3:1-2:ARE':'AE', // UNITED ARAB EMIRATES AS OF 2013-06-30
    'ge:GENC:3:1-2:ARG':'AR', // ARGENTINA AS OF 2013-06-30
    'ge:GENC:3:1-2:ARM':'AM', // ARMENIA AS OF 2013-06-30
    'ge:GENC:3:1-2:ASM':'AS', // AMERICAN SAMOA AS OF 2013-06-30
    'ge:GENC:3:1-2:ATF':'TF', // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:ATG':'AG', // ANTIGUA AND BARBUDA AS OF 2013-06-30
    'ge:GENC:3:1-2:AUS':'AU', // AUSTRALIA AS OF 2013-06-30
    'ge:GENC:3:1-2:AUT':'AT', // AUSTRIA AS OF 2013-06-30
    'ge:GENC:3:1-2:AX1':'A1', // UNKNOWN AS OF 2013-06-30
    'ge:GENC:3:1-2:AX2':'A2', // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
    'ge:GENC:3:1-2:AZE':'AZ', // AZERBAIJAN AS OF 2013-06-30
    'ge:GENC:3:1-2:BDI':'BI', // BURUNDI AS OF 2013-06-30
    'ge:GENC:3:1-2:BEL':'BE', // BELGIUM AS OF 2013-06-30
    'ge:GENC:3:1-2:BEN':'BJ', // BENIN AS OF 2013-06-30
    'ge:GENC:3:1-2:BES':'BQ', // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
    'ge:GENC:3:1-2:BFA':'BF', // BURKINA FASO AS OF 2013-06-30
    'ge:GENC:3:1-2:BGD':'BD', // BANGLADESH AS OF 2013-06-30
    'ge:GENC:3:1-2:BGR':'BG', // BULGARIA AS OF 2013-06-30
    'ge:GENC:3:1-2:BHR':'BH', // BAHRAIN AS OF 2013-06-30
    'ge:GENC:3:1-2:BHS':'BS', // BAHAMAS, THE AS OF 2013-06-30
    'ge:GENC:3:1-2:BLM':'BL', // SAINT BARTHELEMY AS OF 2013-06-30
    'ge:GENC:3:1-2:BLR':'BY', // BELARUS AS OF 2013-06-30
    'ge:GENC:3:1-2:BOL':'BO', // BOLIVIA AS OF 2013-06-30
    'ge:GENC:3:1-2:BRA':'BR', // BRAZIL AS OF 2013-06-30
    'ge:GENC:3:1-2:BRN':'BN', // BRUNEI AS OF 2013-06-30
    'ge:GENC:3:1-2:BTN':'BT', // BHUTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:BWA':'BW', // BOTSWANA AS OF 2013-06-30
    'ge:GENC:3:1-2:CAF':'CF', // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
    'ge:GENC:3:1-2:CAN':'CA', // CANADA AS OF 2013-06-30
    'ge:GENC:3:1-2:CCK':'CC', // COCOS (KEELING) ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:CHE':'CH', // SWITZERLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:CHL':'CL', // CHILE AS OF 2013-06-30
    'ge:GENC:3:1-2:CHN':'CN', // CHINA AS OF 2013-06-30
    'ge:GENC:3:1-2:CIV':'CI', // COTE D"IVOIRE AS OF 2013-06-30
    'ge:GENC:3:1-2:CMR':'CM', // CAMEROON AS OF 2013-06-30
    'ge:GENC:3:1-2:COD':'CD', // CONGO (KINSHASA) AS OF 2013-06-30
    'ge:GENC:3:1-2:COG':'CG', // CONGO (BRAZZAVILLE) AS OF 2013-06-30
    'ge:GENC:3:1-2:COK':'CK', // COOK ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:COL':'CO', // COLOMBIA AS OF 2013-06-30
    'ge:GENC:3:1-2:COM':'KM', // COMOROS AS OF 2013-06-30
    'ge:GENC:3:1-2:CPT':'CP', // CLIPPERTON ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:CPV':'CV', // CAPE VERDE AS OF 2013-06-30
    'ge:GENC:3:1-2:CRI':'CR', // COSTA RICA AS OF 2013-06-30
    'ge:GENC:3:1-2:CUB':'CU', // CUBA AS OF 2013-06-30
    'ge:GENC:3:1-2:CUW':'CW', // CURACAO AS OF 2013-06-30
    'ge:GENC:3:1-2:CXR':'CX', // CHRISTMAS ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:CYM':'KY', // CAYMAN ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:CYP':'CY', // CYPRUS AS OF 2013-06-30
    'ge:GENC:3:1-2:CZE':'CZ', // CZECH REPUBLIC AS OF 2013-06-30
    'ge:GENC:3:1-2:DEU':'DE', // GERMANY AS OF 2013-06-30
    'ge:GENC:3:1-2:DGA':'DG', // DIEGO GARCIA AS OF 2013-06-30
    'ge:GENC:3:1-2:DJI':'DJ', // DJIBOUTI AS OF 2013-06-30
    'ge:GENC:3:1-2:DMA':'DM', // DOMINICA AS OF 2013-06-30
    'ge:GENC:3:1-2:DNK':'DK', // DENMARK AS OF 2013-06-30
    'ge:GENC:3:1-2:DOM':'DO', // DOMINICAN REPUBLIC AS OF 2013-06-30
    'ge:GENC:3:1-2:DZA':'DZ', // ALGERIA AS OF 2013-06-30
    'ge:GENC:3:1-2:ECU':'EC', // ECUADOR AS OF 2013-06-30
    'ge:GENC:3:1-2:EGY':'EG', // EGYPT AS OF 2013-06-30
    'ge:GENC:3:1-2:ERI':'ER', // ERITREA AS OF 2013-06-30
    'ge:GENC:3:1-2:ESH':'EH', // WESTERN SAHARA AS OF 2013-06-30
    'ge:GENC:3:1-2:ESP':'ES', // SPAIN AS OF 2013-06-30
    'ge:GENC:3:1-2:EST':'EE', // ESTONIA AS OF 2013-06-30
    'ge:GENC:3:1-2:ETH':'ET', // ETHIOPIA AS OF 2013-06-30
    'ge:GENC:3:1-2:FIN':'FI', // FINLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:FJI':'FJ', // FIJI AS OF 2013-06-30
    'ge:GENC:3:1-2:FLK':'FK', // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
    'ge:GENC:3:1-2:FRA':'FR', // FRANCE AS OF 2013-06-30
    'ge:GENC:3:1-2:FRO':'FO', // FAROE ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:FSM':'FM', // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
    'ge:GENC:3:1-2:GAB':'GA', // GABON AS OF 2013-06-30
    'ge:GENC:3:1-2:GBR':'GB', // UNITED KINGDOM AS OF 2013-06-30
    'ge:GENC:3:1-2:GEO':'GE', // GEORGIA AS OF 2013-06-30
    'ge:GENC:3:1-2:GGY':'GG', // GUERNSEY AS OF 2013-06-30
    'ge:GENC:3:1-2:GHA':'GH', // GHANA AS OF 2013-06-30
    'ge:GENC:3:1-2:GIN':'GN', // GUINEA AS OF 2013-06-30
    'ge:GENC:3:1-2:GLP':'GP', // GUADELOUPE AS OF 2013-06-30
    'ge:GENC:3:1-2:GMB':'GM', // GAMBIA, THE AS OF 2013-06-30
    'ge:GENC:3:1-2:GNB':'GW', // GUINEA-BISSAU AS OF 2013-06-30
    'ge:GENC:3:1-2:GNQ':'GQ', // EQUATORIAL GUINEA AS OF 2013-06-30
    'ge:GENC:3:1-2:GRC':'GR', // GREECE AS OF 2013-06-30
    'ge:GENC:3:1-2:GTM':'GT', // GUATEMALA AS OF 2013-06-30
    'ge:GENC:3:1-2:GUF':'GF', // FRENCH GUIANA AS OF 2013-06-30
    'ge:GENC:3:1-2:GUM':'GU', // GUAM AS OF 2013-06-30
    'ge:GENC:3:1-2:GUY':'GY', // GUYANA AS OF 2013-06-30
    'ge:GENC:3:1-2:HKG':'HK', // HONG KONG AS OF 2013-06-30
    'ge:GENC:3:1-2:HMD':'HM', // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:HND':'HN', // HONDURAS AS OF 2013-06-30
    'ge:GENC:3:1-2:HRV':'HR', // CROATIA AS OF 2013-06-30
    'ge:GENC:3:1-2:HTI':'HT', // HAITI AS OF 2013-06-30
    'ge:GENC:3:1-2:IDN':'ID', // INDONESIA AS OF 2013-06-30
    'ge:GENC:3:1-2:IND':'IN', // INDIA AS OF 2013-06-30
    'ge:GENC:3:1-2:IOT':'IO', // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
    'ge:GENC:3:1-2:IRN':'IR', // IRAN AS OF 2013-06-30
    'ge:GENC:3:1-2:IRQ':'IQ', // IRAQ AS OF 2013-06-30
    'ge:GENC:3:1-2:ISL':'IS', // ICELAND AS OF 2013-06-30
    'ge:GENC:3:1-2:ISR':'IL', // ISRAEL AS OF 2013-06-30
    'ge:GENC:3:1-2:ITA':'IT', // ITALY AS OF 2013-06-30
    'ge:GENC:3:1-2:JEY':'JE', // JERSEY AS OF 2013-06-30
    'ge:GENC:3:1-2:JOR':'JO', // JORDAN AS OF 2013-06-30
    'ge:GENC:3:1-2:KAZ':'KZ', // KAZAKHSTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:KEN':'KE', // KENYA AS OF 2013-06-30
    'ge:GENC:3:1-2:KGZ':'KG', // KYRGYZSTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:KHM':'KH', // CAMBODIA AS OF 2013-06-30
    'ge:GENC:3:1-2:KIR':'KI', // KIRIBATI AS OF 2013-06-30
    'ge:GENC:3:1-2:KNA':'KN', // SAINT KITTS AND NEVIS AS OF 2013-06-30
    'ge:GENC:3:1-2:KOR':'KR', // KOREA, SOUTH AS OF 2013-06-30
    'ge:GENC:3:1-2:KWT':'KW', // KUWAIT AS OF 2013-06-30
    'ge:GENC:3:1-2:LAO':'LA', // LAOS AS OF 2013-06-30
    'ge:GENC:3:1-2:LBN':'LB', // LEBANON AS OF 2013-06-30
    'ge:GENC:3:1-2:LBR':'LR', // LIBERIA AS OF 2013-06-30
    'ge:GENC:3:1-2:LIE':'LI', // LIECHTENSTEIN AS OF 2013-06-30
    'ge:GENC:3:1-2:LKA':'LK', // SRI LANKA AS OF 2013-06-30
    'ge:GENC:3:1-2:LSO':'LS', // LESOTHO AS OF 2013-06-30
    'ge:GENC:3:1-2:LTU':'LT', // LITHUANIA AS OF 2013-06-30
    'ge:GENC:3:1-2:LUX':'LU', // LUXEMBOURG AS OF 2013-06-30
    'ge:GENC:3:1-2:LVA':'LV', // LATVIA AS OF 2013-06-30
    'ge:GENC:3:1-2:MAC':'MO', // MACAU AS OF 2013-06-30
    'ge:GENC:3:1-2:MAF':'MF', // SAINT MARTIN AS OF 2013-06-30
    'ge:GENC:3:1-2:MAR':'MA', // MOROCCO AS OF 2013-06-30
    'ge:GENC:3:1-2:MCO':'MC', // MONACO AS OF 2013-06-30
    'ge:GENC:3:1-2:MDA':'MD', // MOLDOVA AS OF 2013-06-30
    'ge:GENC:3:1-2:MDG':'MG', // MADAGASCAR AS OF 2013-06-30
    'ge:GENC:3:1-2:MDV':'MV', // MALDIVES AS OF 2013-06-30
    'ge:GENC:3:1-2:MEX':'MX', // MEXICO AS OF 2013-06-30
    'ge:GENC:3:1-2:MHL':'MH', // MARSHALL ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:MKD':'MK', // MACEDONIA AS OF 2013-06-30
    'ge:GENC:3:1-2:MLI':'ML', // MALI AS OF 2013-06-30
    'ge:GENC:3:1-2:MLT':'MT', // MALTA AS OF 2013-06-30
    'ge:GENC:3:1-2:MMR':'MM', // BURMA AS OF 2013-06-30
    'ge:GENC:3:1-2:MNG':'MN', // MONGOLIA AS OF 2013-06-30
    'ge:GENC:3:1-2:MNP':'MP', // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:MOZ':'MZ', // MOZAMBIQUE AS OF 2013-06-30
    'ge:GENC:3:1-2:MRT':'MR', // MAURITANIA AS OF 2013-06-30
    'ge:GENC:3:1-2:MTQ':'MQ', // MARTINIQUE AS OF 2013-06-30
    'ge:GENC:3:1-2:MUS':'MU', // MAURITIUS AS OF 2013-06-30
    'ge:GENC:3:1-2:MWI':'MW', // MALAWI AS OF 2013-06-30
    'ge:GENC:3:1-2:MYS':'MY', // MALAYSIA AS OF 2013-06-30
    'ge:GENC:3:1-2:MYT':'YT', // MAYOTTE AS OF 2013-06-30
    'ge:GENC:3:1-2:NAM':'NA', // NAMIBIA AS OF 2013-06-30
    'ge:GENC:3:1-2:NCL':'NC', // NEW CALEDONIA AS OF 2013-06-30
    'ge:GENC:3:1-2:NER':'NE', // NIGER AS OF 2013-06-30
    'ge:GENC:3:1-2:NFK':'NF', // NORFOLK ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:NGA':'NG', // NIGERIA AS OF 2013-06-30
    'ge:GENC:3:1-2:NIC':'NI', // NICARAGUA AS OF 2013-06-30
    'ge:GENC:3:1-2:NLD':'NL', // NETHERLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:NOR':'NO', // NORWAY AS OF 2013-06-30
    'ge:GENC:3:1-2:NPL':'NP', // NEPAL AS OF 2013-06-30
    'ge:GENC:3:1-2:NRU':'NR', // NAURU AS OF 2013-06-30
    'ge:GENC:3:1-2:NZL':'NZ', // NEW ZEALAND AS OF 2013-06-30
    'ge:GENC:3:1-2:OMN':'OM', // OMAN AS OF 2013-06-30
    'ge:GENC:3:1-2:PAK':'PK', // PAKISTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:PAN':'PA', // PANAMA AS OF 2013-06-30
    'ge:GENC:3:1-2:PCN':'PN', // PITCAIRN ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:PER':'PE', // PERU AS OF 2013-06-30
    'ge:GENC:3:1-2:PHL':'PH', // PHILIPPINES AS OF 2013-06-30
    'ge:GENC:3:1-2:PLW':'PW', // PALAU AS OF 2013-06-30
    'ge:GENC:3:1-2:POL':'PL', // POLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:PRI':'PR', // PUERTO RICO AS OF 2013-06-30
    'ge:GENC:3:1-2:PRK':'KP', // KOREA, NORTH AS OF 2013-06-30
    'ge:GENC:3:1-2:PRT':'PT', // PORTUGAL AS OF 2013-06-30
    'ge:GENC:3:1-2:PRY':'PY', // PARAGUAY AS OF 2013-06-30
    'ge:GENC:3:1-2:PSE':'PS', // PALESTINIAN TERRITORY AS OF 2013-06-30
    'ge:GENC:3:1-2:PYF':'PF', // FRENCH POLYNESIA AS OF 2013-06-30
    'ge:GENC:3:1-2:QAT':'QA', // QATAR AS OF 2013-06-30
    'ge:GENC:3:1-2:REU':'RE', // REUNION AS OF 2013-06-30
    'ge:GENC:3:1-2:RUS':'RU', // RUSSIA AS OF 2013-06-30
    'ge:GENC:3:1-2:RWA':'RW', // RWANDA AS OF 2013-06-30
    'ge:GENC:3:1-2:SAU':'SA', // SAUDI ARABIA AS OF 2013-06-30
    'ge:GENC:3:1-2:SDN':'SD', // SUDAN AS OF 2013-06-30
    'ge:GENC:3:1-2:SEN':'SN', // SENEGAL AS OF 2013-06-30
    'ge:GENC:3:1-2:SGP':'SG', // SINGAPORE AS OF 2013-06-30
    'ge:GENC:3:1-2:SGS':'GS', // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:SHN':'SH', // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
    'ge:GENC:3:1-2:SLB':'SB', // SOLOMON ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:SLE':'SL', // SIERRA LEONE AS OF 2013-06-30
    'ge:GENC:3:1-2:SLV':'SV', // EL SALVADOR AS OF 2013-06-30
    'ge:GENC:3:1-2:SMR':'SM', // SAN MARINO AS OF 2013-06-30
    'ge:GENC:3:1-2:SOM':'SO', // SOMALIA AS OF 2013-06-30
    'ge:GENC:3:1-2:SPM':'PM', // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
    'ge:GENC:3:1-2:SRB':'RS', // SERBIA AS OF 2013-06-30
    'ge:GENC:3:1-2:SSD':'SS', // SOUTH SUDAN AS OF 2013-06-30
    'ge:GENC:3:1-2:STP':'ST', // SAO TOME AND PRINCIPE AS OF 2013-06-30
    'ge:GENC:3:1-2:SUR':'SR', // SURINAME AS OF 2013-06-30
    'ge:GENC:3:1-2:SVK':'SK', // SLOVAKIA AS OF 2013-06-30
    'ge:GENC:3:1-2:SVN':'SI', // SLOVENIA AS OF 2013-06-30
    'ge:GENC:3:1-2:SWE':'SE', // SWEDEN AS OF 2013-06-30
    'ge:GENC:3:1-2:SWZ':'SZ', // SWAZILAND AS OF 2013-06-30
    'ge:GENC:3:1-2:SXM':'SX', // SINT MAARTEN AS OF 2013-06-30
    'ge:GENC:3:1-2:SYC':'SC', // SEYCHELLES AS OF 2013-06-30
    'ge:GENC:3:1-2:SYR':'SY', // SYRIA AS OF 2013-06-30
    'ge:GENC:3:1-2:TCA':'TC', // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:TCD':'TD', // CHAD AS OF 2013-06-30
    'ge:GENC:3:1-2:TGO':'TG', // TOGO AS OF 2013-06-30
    'ge:GENC:3:1-2:THA':'TH', // THAILAND AS OF 2013-06-30
    'ge:GENC:3:1-2:TJK':'TJ', // TAJIKISTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:TLS':'TL', // TIMOR-LESTE AS OF 2013-06-30
    'ge:GENC:3:1-2:TON':'TO', // TONGA AS OF 2013-06-30
    'ge:GENC:3:1-2:TTO':'TT', // TRINIDAD AND TOBAGO AS OF 2013-06-30
    'ge:GENC:3:1-2:TUN':'TN', // TUNISIA AS OF 2013-06-30
    'ge:GENC:3:1-2:TUR':'TR', // TURKEY AS OF 2013-06-30
    'ge:GENC:3:1-2:TWN':'TW', // TAIWAN AS OF 2013-06-30
    'ge:GENC:3:1-2:TZA':'TZ', // TANZANIA AS OF 2013-06-30
    'ge:GENC:3:1-2:UGA':'UG', // UGANDA AS OF 2013-06-30
    'ge:GENC:3:1-2:UKR':'UA', // UKRAINE AS OF 2013-06-30
    'ge:GENC:3:1-2:URY':'UY', // URUGUAY AS OF 2013-06-30
    'ge:GENC:3:1-2:USA':'US', // UNITED STATES AS OF 2013-06-30
    'ge:GENC:3:1-2:UZB':'UZ', // UZBEKISTAN AS OF 2013-06-30
    'ge:GENC:3:1-2:VAT':'VA', // VATICAN CITY AS OF 2013-06-30
    'ge:GENC:3:1-2:VCT':'VC', // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
    'ge:GENC:3:1-2:VEN':'VE', // VENEZUELA AS OF 2013-06-30
    'ge:GENC:3:1-2:VGB':'VG', // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
    'ge:GENC:3:1-2:VIR':'VI', // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
    'ge:GENC:3:1-2:VNM':'VN', // VIETNAM AS OF 2013-06-30
    'ge:GENC:3:1-2:VUT':'VU', // VANUATU AS OF 2013-06-30
    'ge:GENC:3:1-2:WLF':'WF', // WALLIS AND FUTUNA AS OF 2013-06-30
    'ge:GENC:3:1-2:WSM':'WS', // SAMOA AS OF 2013-06-30
    'ge:GENC:3:1-2:XAC':'XA', // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XAZ':'XAZ', // ENTITY 1 AS OF 2013-06-30
    'ge:GENC:3:1-2:XBI':'QS', // BASSAS DA INDIA AS OF 2013-06-30
    'ge:GENC:3:1-2:XBK':'XB', // BAKER ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XCR':'XCR', // ENTITY 2 AS OF 2013-06-30
    'ge:GENC:3:1-2:XCS':'XC', // CORAL SEA ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XCY':'XCY', // ENTITY 3 AS OF 2013-06-30
    'ge:GENC:3:1-2:XEU':'XE', // EUROPA ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XGL':'QX', // GLORIOSO ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XGZ':'XG', // GAZA STRIP AS OF 2013-06-30
    'ge:GENC:3:1-2:XHO':'XH', // HOWLAND ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XJA':'XU', // JOHNSTON ATOLL AS OF 2013-06-30
    'ge:GENC:3:1-2:XJM':'XJ', // JAN MAYEN AS OF 2013-06-30
    'ge:GENC:3:1-2:XJN':'QU', // JUAN DE NOVA ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XJV':'XQ', // JARVIS ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XKM':'XKM', // ENTITY 4 AS OF 2013-06-30
    'ge:GENC:3:1-2:XKN':'XKN', // ENTITY 5 AS OF 2013-06-30
    'ge:GENC:3:1-2:XKR':'XM', // KINGMAN REEF AS OF 2013-06-30
    'ge:GENC:3:1-2:XKS':'XK', // KOSOVO AS OF 2013-06-30
    'ge:GENC:3:1-2:XMW':'QM', // MIDWAY ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XNV':'XV', // NAVASSA ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XPL':'XL', // PALMYRA ATOLL AS OF 2013-06-30
    'ge:GENC:3:1-2:XPR':'XP', // PARACEL ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XQP':'QP', // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XQZ':'QZ', // AKROTIRI AS OF 2013-06-30
    'ge:GENC:3:1-2:XSP':'XS', // SPRATLY ISLANDS AS OF 2013-06-30
    'ge:GENC:3:1-2:XSV':'XR', // SVALBARD AS OF 2013-06-30
    'ge:GENC:3:1-2:XTR':'XT', // TROMELIN ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XWB':'XW', // WEST BANK AS OF 2013-06-30
    'ge:GENC:3:1-2:XWK':'QW', // WAKE ISLAND AS OF 2013-06-30
    'ge:GENC:3:1-2:XXD':'XD', // DHEKELIA AS OF 2013-06-30
    'ge:GENC:3:1-2:XXX':'XXX', // NO MAN"S LAND AS OF 2013-06-30
    'ge:GENC:3:1-2:YEM':'YE', // YEMEN AS OF 2013-06-30
    'ge:GENC:3:1-2:ZAF':'ZA', // SOUTH AFRICA AS OF 2013-06-30
    'ge:GENC:3:1-2:ZMB':'ZM', // ZAMBIA AS OF 2013-06-30
    'ge:GENC:3:1-2:ZWE':'ZW', // ZIMBABWE AS OF 2013-06-30
    'ge:GENC:3:ed3:AX3':'AX3', // ENTITY 6 AS OF 2015-06-30
    'ge:ISO1:3:VI-15:AIA':'AI', // ANGUILLA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:ATA':'AQ', // ANTARCTICA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:BIH':'BA', // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:BLZ':'BZ', // BELIZE AS OF 2013-05-10
    'ge:ISO1:3:VI-15:BMU':'BM', // BERMUDA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:BRB':'BB', // BARBADOS AS OF 2013-05-10
    'ge:ISO1:3:VI-15:BVT':'BV', // BOUVET ISLAND AS OF 2013-05-10
    'ge:ISO1:3:VI-15:GIB':'GI', // GIBRALTAR AS OF 2013-05-10
    'ge:ISO1:3:VI-15:GRD':'GD', // GRENADA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:GRL':'GL', // GREENLAND AS OF 2013-05-10
    'ge:ISO1:3:VI-15:HUN':'HU', // HUNGARY AS OF 2013-05-10
    'ge:ISO1:3:VI-15:IMN':'IM', // ISLE OF MAN AS OF 2013-05-10
    'ge:ISO1:3:VI-15:IRL':'IE', // IRELAND AS OF 2013-05-10
    'ge:ISO1:3:VI-15:JAM':'JM', // JAMAICA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:JPN':'JP', // JAPAN AS OF 2013-05-10
    'ge:ISO1:3:VI-15:LBY':'LY', // LIBYA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:LCA':'LC', // SAINT LUCIA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:MNE':'ME', // MONTENEGRO AS OF 2013-05-10
    'ge:ISO1:3:VI-15:MSR':'MS', // MONTSERRAT AS OF 2013-05-10
    'ge:ISO1:3:VI-15:NIU':'NU', // NIUE AS OF 2013-05-10
    'ge:ISO1:3:VI-15:PNG':'PG', // PAPUA NEW GUINEA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:ROU':'RO', // ROMANIA AS OF 2013-05-10
    'ge:ISO1:3:VI-15:TKL':'TK', // TOKELAU AS OF 2013-05-10
    'ge:ISO1:3:VI-15:TKM':'TM', // TURKMENISTAN AS OF 2013-05-10
    'ge:ISO1:3:VI-15:TUV':'TV', // TUVALU AS OF 2013-05-10
    'other':'other', // Other
  },
  // ##### End of ge4List #####

  // ##### Start of Thematic Group Rules #####
  thematicGroupList : {
    'AAA010':'IndustrySrf',  // EXTRACTION_MINE_S
    'AAA020':'IndustrySrf',  // MINE_SHAFT_SUPERSTRUCTURE_S
    'AAA040':'IndustrySrf',  // RIG_S
    'AAA052':'IndustrySrf',  // HYDROCARBONS_FIELD_S
    'AAB000':'IndustrySrf',  // DISPOSAL_SITE_S
    'AAB010':'IndustrySrf',  // RECYCLING_SITE_S
    'AAB040':'IndustrySrf',  // AERATION_BASIN_S
    'AAB507':'IndustrySrf',  // WASTE_HEAP_S
    'AAC010':'IndustrySrf',  // BLAST_FURNACE_S
    'AAC020':'IndustrySrf',  // CATALYTIC_CRACKER_S
    'AAC030':'IndustrySrf',  // SETTLING_POND_S
    'AAC040':'IndustrySrf',  // HYDROCARBON_PROD_FACILITY_S
    'AAC060':'IndustrySrf',  // INDUSTRIAL_FURNACE_S
    'AAC507':'UtilityInfrastructureSrf',  // SEWAGE_TREATMENT_PLANT_S
    'AAD010':'UtilityInfrastructureSrf',  // ELECTRIC_POWER_STATION_S
    'AAD020':'UtilityInfrastructureSrf',  // SOLAR_PANEL_S
    'AAD025':'UtilityInfrastructureSrf',  // SOLAR_FARM_S
    'AAD030':'UtilityInfrastructureSrf',  // POWER_SUBSTATION_S
    'AAD041':'UtilityInfrastructureSrf',  // NUCLEAR_REACTOR_CONTAINMENT_S
    'AAD050':'UtilityInfrastructureSrf',  // HEATING_FACILITY_S
    'AAD055':'UtilityInfrastructureSrf',  // COOLING_FACILITY_S
    'AAD060':'UtilityInfrastructureSrf',  // WIND_FARM_S
    'AAF030':'UtilityInfrastructureSrf',  // COOLING_TOWER_S
    'AAF040':'IndustrySrf',  // CRANE_S
    'AAF060':'StructureSrf',  // ENGINE_TEST_CELL_S
    'AAG030':'FacilitySrf',  // SHOPPING_COMPLEX_S
    'AAG040':'FacilitySrf',  // OFFICE_PARK_S
    'AAH025':'MilitarySrf',  // ENGINEERED_EARTHWORK_S
    'AAH055':'MilitarySrf',  // FORTIFIED_BUILDING_S
    'AAH060':'MilitarySrf',  // UNDERGROUND_BUNKER_S
    'AAI020':'SettlementSrf',  // CARAVAN_PARK_S
    'AAI021':'SettlementSrf',  // MANUFACTURED_HOME_PARK_S
    'AAI030':'SettlementSrf',  // CAMP_S
    'AAJ030':'AgricultureSrf',  // HOLDING_PEN_S
    'AAJ050':'AgricultureSrf',  // WINDMILL_S
    'AAJ055':'IndustrySrf',  // WATER_MILL_S
    'AAJ080':'AgricultureSrf',  // STABLE_S
    'AAJ085':'AgricultureSrf',  // BARN_S
    'AAJ110':'AgricultureSrf',  // GREENHOUSE_S
    'AAK020':'RecreationSrf',  // AMUSEMENT_PARK_ATTRACTION_S
    'AAK030':'RecreationSrf',  // AMUSEMENT_PARK_S
    'AAK040':'RecreationSrf',  // SPORTS_GROUND_S
    'AAK060':'RecreationSrf',  // CAMP_SITE_S
    'AAK061':'RecreationSrf',  // PICNIC_SITE_S
    'AAK070':'RecreationSrf',  // DRIVE_IN_THEATRE_S
    'AAK090':'RecreationSrf',  // FAIRGROUND_S
    'AAK100':'RecreationSrf',  // GOLF_COURSE_S
    'AAK101':'RecreationSrf',  // GOLF_DRIVING_RANGE_S
    'AAK110':'RecreationSrf',  // GRANDSTAND_S
    'AAK120':'CultureSrf',  // PARK_S
    'AAK121':'CultureSrf',  // LOOKOUT_S
    'AAK130':'RecreationSrf',  // RACETRACK_S
    'AAK155':'RecreationSrf',  // SKI_RUN_S
    'AAK160':'RecreationSrf',  // STADIUM_S
    'AAK164':'RecreationSrf',  // AMPHITHEATRE_S
    'AAK170':'RecreationSrf',  // SWIMMING_POOL_S
    'AAK180':'RecreationSrf',  // ZOO_S
    'AAL010':'FacilitySrf',  // FACILITY_S
    'AAL011':'FacilitySrf',  // INSTALLATION_S
    'AAL012':'CultureSrf',  // ARCHAEOLOGICAL_SITE_S
    'AAL013':'StructureSrf',  // BUILDING_S
    'AAL014':'StructureSrf',  // NON_BUILDING_STRUCTURE_S
    'AAL018':'StructureSrf',  // BUILDING_SUPERSTRUCTURE_S
    'AAL019':'StructureSrf',  // SHED_S
    'AAL020':'SettlementSrf',  // BUILT_UP_AREA_S
    'AAL024':'SettlementSrfExt',  // NEIGHBOURHOOD_S
    'AAL030':'CultureSrf',  // CEMETERY_S
    'AAL036':'CultureSrf',  // TOMB_S
    'AAL060':'MilitarySrf',  // DRAGONS_TEETH_S
    'AAL065':'MilitarySrf',  // MINEFIELD_S
    'AAL099':'StructureSrf',  // HUT_S
    'AAL105':'SettlementSrf',  // SETTLEMENT_S
    'AAL120':'MilitarySrf',  // MISSILE_SITE_S
    'AAL121':'MilitarySrfExt',  // ANTI_AIRCRAFT_ARTILLERY_SITE_S
    'AAL130':'CultureSrf',  // MEMORIAL_MONUMENT_S
    'AAL140':'StructureSrf',  // PARTICLE_ACCELERATOR_S
    'AAL142':'StructureSrf',  // ASTRONOMICAL_OBSERVATORY_S
    'AAL170':'RecreationSrf',  // PUBLIC_SQUARE_S
    'AAL175':'CultureSrf',  // COURTYARD_S
    'AAL180':'CultureSrf',  // RETAIL_STAND_S
    'AAL195':'TransportationGroundSrf',  // RAMP_S
    'AAL200':'CultureSrf',  // RUINS_S
    'AAL201':'CultureSrf',  // INTEREST_SITE_S
    'AAL208':'SettlementSrf',  // SHANTY_TOWN_S
    'AAL211':'TransportationGroundSrf',  // TRANS_ROUTE_PROTECT_STRUCT_S
    'AAL241':'StructureSrf',  // TOWER_S
    'AAL270':'AgricultureSrf',  // INDUSTRIAL_FARM_S
    'AAL351':'AeronauticSrf',  // SPACE_FACILITY_S
    'AAL371':'StructureSrf',  // MANOR_HOUSE_S
    'AAL375':'MilitarySrf',  // CASTLE_S
    'AAL376':'MilitarySrf',  // CASTLE_COMPLEX_S
    'AAM010':'StorageSrf',  // STORAGE_DEPOT_S
    'AAM011':'StorageSrf',  // SHIPPING_CONTAINER_S
    'AAM020':'AgricultureSrf',  // GRAIN_STORAGE_STRUCTURE_S
    'AAM030':'StorageSrf',  // GRAIN_ELEVATOR_S
    'AAM040':'IndustrySrf',  // MINERAL_PILE_S
    'AAM060':'MilitarySrf',  // SURFACE_BUNKER_S
    'AAM065':'StorageSrf',  // MUNITION_STORAGE_FACILITY_S
    'AAM070':'StorageSrf',  // STORAGE_TANK_S
    'AAM071':'StorageSrf',  // TANK_FARM_S
    'AAM075':'StorageSrf',  // FUEL_STORAGE_FACILITY_S
    'AAM080':'StorageSrf',  // WATER_TOWER_S
    'AAN060':'TransportationGroundSrf',  // RAILWAY_YARD_S
    'AAN075':'TransportationGroundSrf',  // RAILWAY_TURNTABLE_S
    'AAN076':'TransportationGroundSrf',  // ROUNDHOUSE_S
    'AAP030':'TransportationGroundSrf',  // ROAD_S
    'AAP055':'TransportationGroundSrf',  // TANK_TRAIL_S
    'AAP056':'TransportationGroundSrf',  // TANK_CROSSING_S
    'AAQ040':'TransportationGroundSrf',  // BRIDGE_S
    'AAQ045':'TransportationGroundSrf',  // BRIDGE_SPAN_S
    'AAQ050':'TransportationGroundSrf',  // BRIDGE_SUPERSTRUCTURE_S
    'AAQ056':'TransportationGroundSrf',  // BRIDGE_PIER_S
    'AAQ060':'AeronauticSrf',  // CONTROL_TOWER_S
    'AAQ063':'TransportationGroundSrf',  // CAUSEWAY_STRUCTURE_S
    'AAQ068':'TransportationGroundSrf',  // TRANSPORTATION_BLOCK_S
    'AAQ080':'TransportationWaterSrf',  // FERRY_STATION_S
    'AAQ116':'UtilityInfrastructureSrf',  // PUMPING_STATION_S
    'AAQ125':'TransportationGroundSrf',  // TRANSPORTATION_STATION_S
    'AAQ130':'TransportationGroundSrf',  // TUNNEL_S
    'AAQ135':'TransportationGroundSrf',  // ROADSIDE_REST_AREA_S
    'AAQ140':'TransportationGroundSrf',  // VEHICLE_LOT_S
    'AAQ141':'TransportationGroundSrf',  // PARKING_GARAGE_S
    'AAQ150':'StructureSrf',  // STAIR_S
    'AAQ151':'TransportationGroundSrf',  // ARCADE_S
    'AAQ170':'TransportationGroundSrf',  // MOTOR_VEHICLE_STATION_S
    'AAT012':'UtilityInfrastructureSrf',  // AERIAL_FARM_S
    'AAT045':'FacilitySrf',  // RADAR_STATION_S
    'ABA023':'PhysiographySrfExt',  // FORESHORE_S
    'ABA030':'PhysiographySrf',  // ISLAND_S
    'ABA040':'HydrographySrf',  // TIDAL_WATER_S
    'ABA050':'PhysiographySrfExt',  // BEACH_S
    'ABB005':'PortHarbourSrf',  // HARBOUR_S
    'ABB008':'HydrographySrfExt',  // HARBOUR_WATERS_S
    'ABB009':'PortHarbourSrf',  // PORT_S
    'ABB010':'PortHarbourSrfExt',  // ANCHORAGE_S
    'ABB081':'PortHarbourSrf',  // SHORELINE_CONSTRUCTION_S
    'ABB082':'PortHarbourSrf',  // SHORELINE_RAMP_S
    'ABB090':'PortHarbourSrf',  // DRY_DOCK_S
    'ABB110':'HydrographySrf',  // FISH_WEIR_S
    'ABB115':'PortHarbourSrfExt',  // NAUTICAL_GRIDIRON_S
    'ABB150':'MilitarySrfExt',  // BEACH_LANDING_SITE_S
    'ABB199':'PortHarbourSrf',  // FLOATING_DRY_DOCK_S
    'ABB201':'PortHarbourSrf',  // SMALL_CRAFT_FACILITY_S
    'ABB241':'PortHarbourSrf',  // SHIPYARD_S
    'ABC050':'HydroAidNavigationSrf',  // LIGHTHOUSE_S
    'ABD020':'HydrographySrfExt',  // CRIB_S
    'ABD030':'HydrographySrfExt',  // DISCOLOURED_WATER_S
    'ABD050':'HydrographySrfExt',  // FOUL_GROUND_S
    'ABD061':'HydrographySrfExt',  // AQUATIC_VEGETATION_S
    'ABD100':'PortHarbourSrf',  // STRUCTURAL_PILE_S
    'ABD115':'HydrographySrf',  // OFFSHORE_CONSTRUCTION_S
    'ABD120':'HydrographySrfExt',  // REEF_S
    'ABD140':'HydrographySrf',  // SNAG_S
    'ABD180':'HydrographySrfExt',  // WRECK_S
    'ABE019':'HydrographySrfExt',  // DEPTH_AREA_S
    'ABF010':'HydrographySrfExt',  // BOTTOM_CHARACTER_REGION_S
    'ABG012':'HydrographySrfExt',  // WATER_TURBULENCE_S
    'ABH010':'HydrographySrf',  // AQUEDUCT_S
    'ABH015':'VegetationSrf',  // BOG_S
    'ABH020':'TransportationWaterSrf',  // NAVIGABLE_CANAL_S
    'ABH030':'HydrographySrf',  // DITCH_S
    'ABH040':'IndustrySrf',  // WATER_TREATMENT_BED_S
    'ABH050':'HydrographySrfExt',  // MARICULTURE_SITE_S
    'ABH051':'AgricultureSrf',  // FISH_FARM_FACILITY_S
    'ABH070':'TransportationGroundSrf',  // FORD_S
    'ABH075':'CultureSrf',  // FOUNTAIN_S
    'ABH077':'VegetationSrf',  // HUMMOCK_S
    'ABH082':'HydrographySrf',  // INLAND_WATERBODY_S
    'ABH090':'HydrographySrf',  // LAND_SUBJECT_TO_INUNDATION_S
    'ABH100':'HydrographySrf',  // MOAT_S
    'ABH116':'SubterraneanSrf',  // AQUIFER_S
    'ABH120':'HydrographySrf',  // RAPIDS_S
    'ABH135':'AgricultureSrf',  // RICE_FIELD_S
    'ABH140':'HydrographySrf',  // RIVER_S
    'ABH150':'PhysiographySrf',  // SALT_FLAT_S
    'ABH155':'IndustrySrf',  // SALT_EVAPORATOR_S
    'ABH160':'PhysiographySrf',  // SABKHA_S
    'ABH165':'HydrographySrf',  // SPILLWAY_S
    'ABH170':'HydrographySrf',  // NATURAL_POOL_S
    'ABH190':'HydrographySrfExt',  // LAGOON_S
    'ABH220':'UtilityInfrastructureSrf',  // WATERWORK_S
    'ABH230':'HydrographySrf',  // WATER_WELL_S
    'ABI005':'PortHarbourSrf',  // VESSEL_LIFT_S
    'ABI006':'TransportationWaterSrf',  // SHIP_ELEVATOR_S
    'ABI020':'HydrographySrf',  // DAM_S
    'ABI030':'TransportationWaterSrf',  // LOCK_S
    'ABI044':'HydrographySrf',  // FLOOD_CONTROL_STRUCTURE_S
    'ABI050':'HydrographySrf',  // WATER_INTAKE_TOWER_S
    'ABJ020':'PhysiographySrf',  // MORAINE_S
    'ABJ030':'PhysiographySrf',  // GLACIER_S
    'ABJ031':'PhysiographySrf',  // CREVASSE_S
    'ABJ065':'PhysiographySrf',  // ICE_SHELF_S
    'ABJ070':'PhysiographySrfExt',  // PACK_ICE_S
    'ABJ080':'PhysiographySrf',  // POLAR_ICE_S
    'ABJ099':'PhysiographySrf',  // ICE_CAP_S
    'ABJ100':'PhysiographySrf',  // SNOW_ICE_FIELD_S
    'ABJ110':'VegetationSrf',  // TUNDRA_S
    'ADA005':'PhysiographySrf',  // ASPHALT_LAKE_S
    'ADA010':'PhysiographySrf',  // SOIL_SURFACE_REGION_S
    'ADB028':'SubterraneanSrf',  // CAVE_CHAMBER_S
    'ADB061':'PhysiographySrf',  // CREVICE_S
    'ADB080':'PhysiographySrf',  // DEPRESSION_S
    'ADB090':'PhysiographySrf',  // EMBANKMENT_S
    'ADB115':'PhysiographySrf',  // GEOTHERMAL_OUTLET_S
    'ADB160':'PhysiographySrf',  // ROCK_FORMATION_S
    'ADB170':'PhysiographySrf',  // SAND_DUNES_S
    'ADB180':'PhysiographySrf',  // VOLCANO_S
    'ADB185':'PhysiographySrfExt',  // CRATER_S
    'ADB211':'PhysiographySrf',  // LANDSLIDE_MASS_S
    'AEA010':'AgricultureSrf',  // CROP_LAND_S
    'AEA030':'AgricultureSrf',  // PLANT_NURSERY_S
    'AEA031':'CultureSrf',  // BOTANIC_GARDEN_S
    'AEA040':'AgricultureSrf',  // ORCHARD_S
    'AEA050':'AgricultureSrf',  // VINEYARD_S
    'AEA055':'AgricultureSrf',  // HOP_FIELD_S
    'AEB010':'VegetationSrf',  // GRASSLAND_S
    'AEB020':'VegetationSrf',  // THICKET_S
    'AEB070':'VegetationSrf',  // BRUSH_S
    'AEC010':'AgricultureSrf',  // CANE_S
    'AEC015':'VegetationSrf',  // FOREST_S
    'AEC020':'PhysiographySrf',  // OASIS_S
    'AEC040':'VegetationSrf',  // CLEARED_WAY_S
    'AEC050':'VegetationSrfExt',  // GROVE_S
    'AEC060':'VegetationSrf',  // FOREST_CLEARING_S
    'AED010':'VegetationSrf',  // MARSH_S
    'AED020':'VegetationSrf',  // SWAMP_S
    'AEE010':'VegetationSrf',  // LOGGING_SITE_S
    'AEE030':'PhysiographySrf',  // DESERT_S
    'AFA002':'BoundarySrfExt',  // GEOPOLITICAL_ENTITY_S
    'AFA003':'BoundarySrfExt',  // ADMINISTRATIVE_SUBDIVISION_S
    'AFA012':'CultureSrf',  // CONTAMINATED_REGION_S
    'AFA015':'MilitarySrf',  // FIRING_RANGE_S
    'AFA100':'MilitarySrf',  // TEST_SITE_S
    'AFA165':'MilitarySrf',  // TRAINING_SITE_S
    'AFA210':'CultureSrf',  // CONSERVATION_AREA_S
    'AFC034':'HydrographySrfExt',  // DREDGED_AREA_S
    'AFC035':'HydrographySrfExt',  // WATERBODY_DIVIDER_S
    'AFC037':'HydrographySrfExt',  // MARITIME_CAUTION_AREA_S
    'AFC041':'HydrographySrfExt',  // TRAFFIC_SEPARATION_SCHEME_S
    'AFC165':'HydrographySrfExt',  // MARITIME_ROUTE_S
    'AFC177':'HydrographySrfExt',  // SWEPT_AREA_S
    'AGB005':'AeronauticSrf',  // LAND_AERODROME_S
    'AGB015':'AeronauticSrf',  // APRON_S
    'AGB030':'AeronauticSrf',  // HELIPAD_S
    'AGB035':'AeronauticSrf',  // HELIPORT_S
    'AGB040':'AeronauticSrf',  // LAUNCH_PAD_S
    'AGB045':'AeronauticSrf',  // STOPWAY_S
    'AGB055':'AeronauticSrf',  // RUNWAY_S
    'AGB065':'AeronauticSrf',  // WATER_AERODROME_S
    'AGB070':'AeronauticSrf',  // SEAPLANE_RUN_S
    'AGB075':'AeronauticSrf',  // TAXIWAY_S
    'AGB230':'AeronauticSrf',  // AIRCRAFT_HANGAR_S
    'AGB250':'AeronauticSrf',  // HARDENED_AIRCRAFT_SHELTER_S
    'AIA040':'BoundarySrf',  // LAND_PARCEL_S
    'ANA170':'PhysiographySrfExt',  // DRAINAGE_BASIN_S
    'ANM010':'HydrographyAONExt',  // AQUEDUCT_AON_S
    'ANM020':'TransportationWaterAONExt',  // NAVIGABLE_CANAL_AON_S
    'ANM030':'HydrographyAONExt',  // DITCH_AON_S
    'ANM082':'HydrographyAONExt',  // INLAND_WATERBODY_AON_S
    'ANM140':'HydrographyAONExt',  // RIVER_AON_S
    'ANM160':'PhysiographyAONExt',  // SABKHA_AON_S
    'ANM170':'HydrographyAONExt',  // NATURAL_POOL_AON_S
    'ANM230':'HydrographyAONExt',  // WATER_WELL_AON_S
    'ANU010':'HydrographyAONExt',  // CISTERN_AON_S
    'ANU070':'StorageAONExt',  // STORAGE_TANK_AON_S
    'ANU113':'UtilityInfrastructureAONExt',  // PIPELINE_AON_S
    'ANU116':'UtilityInfrastructureAONExt',  // PUMPING_STATION_AON_S
    'ANU170':'RecreationAONExt',  // SWIMMING_POOL_AON_S
    'ASA050':'PhysiographySrfExt',  // SLOPE_REGION_S
    'ASTB23':'AeronauticSrfExt',  // LANDING_ZONE_S
    'ASU001':'MilitarySrf',  // MILITARY_INSTALLATION_S
    'ASU004':'MilitarySrf',  // CANTONMENT_AREA_S
    'AZC040':'InformationSrfExt',  // LOCAL_MAGNETIC_ANOMALY_S
    'AZD020':'InformationSrf',  // VOID_COLLECTION_AREA_S
    'AZD030':'CultureSrfExt',  // CULTURAL_CONTEXT_LOCATION_S
    'AZD040':'InformationSrfExt',  // NAMED_LOCATION_S
    'AZD045':'InformationSrf',  // ANNOTATED_LOCATION_S
    'AZD070':'HydrographySrf',  // WATER_MEASUREMENT_LOCATION_S
    'AZI031':'ResourceSrf',  // DATASET_S
    'AZI039':'MetadataSrf',  // ENTITY_COLLECTION_METADATA_S
    'LAA011':'IndustryCrv',  // SHEAR_WALL_C
    'LAF020':'IndustryCrv',  // CONVEYOR_C
    'LAF050':'IndustryCrv',  // EXCAVATING_MACHINE_C
    'LAH025':'MilitaryCrv',  // ENGINEERED_EARTHWORK_C
    'LAK020':'RecreationCrv',  // AMUSEMENT_PARK_ATTRACTION_C
    'LAK080':'RecreationCrv',  // OUTDOOR_THEATRE_SCREEN_C
    'LAK130':'RecreationCrv',  // RACETRACK_C
    'LAK150':'RecreationCrv',  // SKI_JUMP_C
    'LAK155':'RecreationCrv',  // SKI_RUN_C
    'LAL018':'StructureCrv',  // BUILDING_SUPERSTRUCTURE_C
    'LAL060':'MilitaryCrv',  // DRAGONS_TEETH_C
    'LAL070':'StructureCrv',  // FENCE_C
    'LAL080':'StructureCrv',  // GANTRY_C
    'LAL130':'CultureCrv',  // MEMORIAL_MONUMENT_C
    'LAL140':'StructureCrv',  // PARTICLE_ACCELERATOR_C
    'LAL155':'TransportationGroundCrv',  // OVERHEAD_OBSTRUCTION_C
    'LAL195':'TransportationGroundCrv',  // RAMP_C
    'LAL211':'TransportationGroundCrv',  // TRANS_ROUTE_PROTECT_STRUCT_C
    'LAL260':'StructureCrv',  // WALL_C
    'LAN010':'TransportationGroundCrv',  // RAILWAY_C
    'LAN050':'TransportationGroundCrv',  // RAILWAY_SIDETRACK_C
    'LAP010':'TransportationGroundCrv',  // CART_TRACK_C
    'LAP030':'TransportationGroundCrv',  // ROAD_C
    'LAP040':'TransportationGroundCrv',  // GATE_C
    'LAP041':'TransportationGroundCrv',  // VEHICLE_BARRIER_C
    'LAP050':'TransportationGroundCrv',  // TRAIL_C
    'LAP055':'TransportationGroundCrv',  // TANK_TRAIL_C
    'LAQ035':'TransportationGroundCrv',  // SIDEWALK_C
    'LAQ040':'TransportationGroundCrv',  // BRIDGE_C
    'LAQ045':'TransportationGroundCrv',  // BRIDGE_SPAN_C
    'LAQ050':'TransportationGroundCrv',  // BRIDGE_SUPERSTRUCTURE_C
    'LAQ056':'TransportationGroundCrv',  // BRIDGE_PIER_C
    'LAQ059':'TransportationGroundCrv',  // TRANS_ROUTE_CHARACTER_CHANGE_C
    'LAQ063':'TransportationGroundCrv',  // CAUSEWAY_STRUCTURE_C
    'LAQ065':'TransportationGroundCrv',  // CULVERT_C
    'LAQ070':'TransportationWaterCrv',  // FERRY_CROSSING_C
    'LAQ075':'TransportationGroundCrv',  // ICE_ROUTE_C
    'LAQ113':'UtilityInfrastructureCrv',  // PIPELINE_C
    'LAQ120':'TransportationGroundCrv',  // STEEP_GRADE_C
    'LAQ130':'TransportationGroundCrv',  // TUNNEL_C
    'LAQ150':'StructureCrv',  // STAIR_C
    'LAQ151':'TransportationGroundCrv',  // ARCADE_C
    'LAT005':'UtilityInfrastructureCrv',  // CABLE_C
    'LAT041':'TransportationGroundCrv',  // CABLEWAY_C
    'LBA010':'PhysiographyCrv',  // LAND_WATER_BOUNDARY_C
    'LBA024':'PhysiographyCrvExt',  // SHORELINE_C
    'LBB081':'PortHarbourCrv',  // SHORELINE_CONSTRUCTION_C
    'LBB082':'PortHarbourCrv',  // SHORELINE_RAMP_C
    'LBB100':'HydrographyCrvExt',  // FISHING_STAKES_C
    'LBD120':'HydrographyCrvExt',  // REEF_C
    'LBE010':'HydrographyCrvExt',  // DEPTH_CURVE_C
    'LBE015':'HydrographyCrvExt',  // DEPTH_CONTOUR_C
    'LBE050':'PhysiographyCrvExt',  // BEACH_PROFILE_C
    'LBG012':'HydrographyCrvExt',  // WATER_TURBULENCE_C
    'LBH010':'HydrographyCrv',  // AQUEDUCT_C
    'LBH020':'TransportationWaterCrv',  // NAVIGABLE_CANAL_C
    'LBH030':'HydrographyCrv',  // DITCH_C
    'LBH065':'HydrographyCrv',  // WATER_RACE_C
    'LBH070':'TransportationGroundCrv',  // FORD_C
    'LBH100':'HydrographyCrv',  // MOAT_C
    'LBH110':'HydrographyCrv',  // PENSTOCK_C
    'LBH120':'HydrographyCrv',  // RAPIDS_C
    'LBH140':'HydrographyCrv',  // RIVER_C
    'LBH165':'HydrographyCrv',  // SPILLWAY_C
    'LBH180':'HydrographyCrv',  // WATERFALL_C
    'LBI006':'TransportationWaterCrv',  // SHIP_ELEVATOR_C
    'LBI020':'HydrographyCrv',  // DAM_C
    'LBI030':'TransportationWaterCrv',  // LOCK_C
    'LBI040':'HydrographyCrv',  // SLUICE_GATE_C
    'LBI044':'HydrographyCrv',  // FLOOD_CONTROL_STRUCTURE_C
    'LBI045':'TransportationWaterCrv',  // BASIN_GATE_C
    'LBI060':'HydrographyCrv',  // FISH_LADDER_C
    'LBJ031':'PhysiographyCrv',  // CREVASSE_C
    'LBJ040':'PhysiographyCrv',  // ICE_CLIFF_C
    'LBJ070':'PhysiographyCrvExt',  // PACK_ICE_C
    'LCA010':'HypsographyCrv',  // ELEVATION_CONTOUR_C
    'LDB010':'PhysiographyCrv',  // STEEP_TERRAIN_FACE_C
    'LDB061':'PhysiographyCrv',  // CREVICE_C
    'LDB070':'PhysiographyCrv',  // CUT_C
    'LDB071':'PhysiographyCrv',  // CUT_LINE_C
    'LDB090':'PhysiographyCrv',  // EMBANKMENT_C
    'LDB100':'PhysiographyCrv',  // ESKER_C
    'LDB110':'PhysiographyCrv',  // GEOLOGIC_FAULT_C
    'LDB190':'PhysiographyCrv',  // VOLCANIC_DYKE_C
    'LEA020':'VegetationCrv',  // HEDGEROW_C
    'LEC015':'VegetationCrv',  // FOREST_C
    'LEC040':'VegetationCrv',  // CLEARED_WAY_C
    'LFA000':'BoundaryCrvExt',  // ADMINISTRATIVE_BOUNDARY_C
    'LFA110':'BoundaryCrvExt',  // INTERNATIONAL_DATE_LINE_C
    'LFC021':'HydrographyCrvExt',  // MARITIME_LIMIT_C
    'LFC041':'HydrographyCrvExt',  // TRAFFIC_SEPARATION_SCHEME_C
    'LFC100':'HydrographyCrvExt',  // MEASURED_DISTANCE_LINE_C
    'LFC130':'HydroAidNavigationCrvExt',  // MARITIME_RADAR_REF_LINE_C
    'LFC165':'HydrographyCrvExt',  // MARITIME_ROUTE_C
    'LGB050':'MilitaryCrv',  // DEFENSIVE_REVETMENT_C
    'LGB075':'AeronauticCrv',  // TAXIWAY_C
    'LSTBM1':'MilitaryCrvExt',  // MILITARY_BOUNDARY_C
    'LSU030':'MilitaryCrv',  // INSTALLATION_BOUNDARY_C
    'LZD040':'InformationCrvExt',  // NAMED_LOCATION_C
    'LZD045':'InformationCrv',  // ANNOTATED_LOCATION_C
    'PAA010':'IndustryPnt',  // EXTRACTION_MINE_P
    'PAA020':'IndustryPnt',  // MINE_SHAFT_SUPERSTRUCTURE_P
    'PAA040':'IndustryPnt',  // RIG_P
    'PAA045':'IndustryPnt',  // BOREHOLE_P
    'PAA052':'IndustryPntExt',  // HYDROCARBONS_FIELD_P
    'PAA054':'IndustryPnt',  // NON_WATER_WELL_P
    'PAB000':'IndustryPnt',  // DISPOSAL_SITE_P
    'PAB021':'UtilityInfrastructurePnt',  // LIQUID_DIFFUSER_P
    'PAB507':'IndustryPnt',  // WASTE_HEAP_P
    'PAC010':'IndustryPnt',  // BLAST_FURNACE_P
    'PAC020':'IndustryPnt',  // CATALYTIC_CRACKER_P
    'PAC040':'IndustryPnt',  // HYDROCARBON_PROD_FACILITY_P
    'PAC060':'IndustryPnt',  // INDUSTRIAL_FURNACE_P
    'PAC507':'UtilityInfrastructurePnt',  // SEWAGE_TREATMENT_PLANT_P
    'PAD010':'UtilityInfrastructurePnt',  // ELECTRIC_POWER_STATION_P
    'PAD020':'UtilityInfrastructurePnt',  // SOLAR_PANEL_P
    'PAD025':'UtilityInfrastructurePnt',  // SOLAR_FARM_P
    'PAD030':'UtilityInfrastructurePnt',  // POWER_SUBSTATION_P
    'PAD041':'UtilityInfrastructurePnt',  // NUCLEAR_REACTOR_CONTAINMENT_P
    'PAD050':'UtilityInfrastructurePnt',  // HEATING_FACILITY_P
    'PAD055':'UtilityInfrastructurePnt',  // COOLING_FACILITY_P
    'PAD060':'UtilityInfrastructurePnt',  // WIND_FARM_P
    'PAF010':'UtilityInfrastructurePnt',  // SMOKESTACK_P
    'PAF020':'IndustryPnt',  // CONVEYOR_P
    'PAF030':'UtilityInfrastructurePnt',  // COOLING_TOWER_P
    'PAF040':'IndustryPnt',  // CRANE_P
    'PAF050':'IndustryPnt',  // EXCAVATING_MACHINE_P
    'PAF060':'StructurePnt',  // ENGINE_TEST_CELL_P
    'PAF070':'IndustryPnt',  // FLARE_PIPE_P
    'PAF080':'IndustryPnt',  // HOPPER_P
    'PAG050':'StructurePnt',  // BILLBOARD_P
    'PAH055':'MilitaryPnt',  // FORTIFIED_BUILDING_P
    'PAH060':'MilitaryPnt',  // UNDERGROUND_BUNKER_P
    'PAH070':'TransportationGroundPnt',  // CHECKPOINT_P
    'PAI030':'SettlementPnt',  // CAMP_P
    'PAJ030':'AgriculturePnt',  // HOLDING_PEN_P
    'PAJ050':'AgriculturePnt',  // WINDMILL_P
    'PAJ051':'UtilityInfrastructurePnt',  // WIND_TURBINE_P
    'PAJ055':'IndustryPnt',  // WATER_MILL_P
    'PAJ080':'AgriculturePnt',  // STABLE_P
    'PAJ085':'AgriculturePnt',  // BARN_P
    'PAJ110':'AgriculturePnt',  // GREENHOUSE_P
    'PAK020':'RecreationPnt',  // AMUSEMENT_PARK_ATTRACTION_P
    'PAK030':'RecreationPnt',  // AMUSEMENT_PARK_P
    'PAK040':'RecreationPnt',  // SPORTS_GROUND_P
    'PAK060':'RecreationPnt',  // CAMP_SITE_P
    'PAK080':'RecreationPnt',  // OUTDOOR_THEATRE_SCREEN_P
    'PAK110':'RecreationPnt',  // GRANDSTAND_P
    'PAK121':'CulturePnt',  // LOOKOUT_P
    'PAK150':'RecreationPnt',  // SKI_JUMP_P
    'PAK160':'RecreationPnt',  // STADIUM_P
    'PAK161':'RecreationPnt',  // SCOREBOARD_P
    'PAK164':'RecreationPnt',  // AMPHITHEATRE_P
    'PAK170':'RecreationPnt',  // SWIMMING_POOL_P
    'PAK180':'RecreationPnt',  // ZOO_P
    'PAL010':'FacilityPnt',  // FACILITY_P
    'PAL011':'FacilityPnt',  // INSTALLATION_P
    'PAL012':'CulturePnt',  // ARCHAEOLOGICAL_SITE_P
    'PAL013':'StructurePnt',  // BUILDING_P
    'PAL014':'StructurePnt',  // NON_BUILDING_STRUCTURE_P
    'PAL017':'UtilityInfrastructurePnt',  // FIRE_HYDRANT_P
    'PAL018':'StructurePnt',  // BUILDING_SUPERSTRUCTURE_P
    'PAL019':'StructurePnt',  // SHED_P
    'PAL020':'SettlementPnt',  // BUILT_UP_AREA_P
    'PAL025':'CulturePnt',  // CAIRN_P
    'PAL030':'CulturePnt',  // CEMETERY_P
    'PAL036':'CulturePnt',  // TOMB_P
    'PAL073':'StructurePnt',  // FLAGPOLE_P
    'PAL080':'StructurePnt',  // GANTRY_P
    'PAL099':'StructurePnt',  // HUT_P
    'PAL105':'SettlementPnt',  // SETTLEMENT_P
    'PAL110':'StructurePnt',  // LIGHT_SUPPORT_STRUCTURE_P
    'PAL120':'MilitaryPnt',  // MISSILE_SITE_P
    'PAL121':'MilitaryPntExt',  // ANTI_AIRCRAFT_ARTILLERY_SITE_P
    'PAL130':'CulturePnt',  // MEMORIAL_MONUMENT_P
    'PAL140':'StructurePntExt',  // PARTICLE_ACCELERATOR_P
    'PAL142':'StructurePnt',  // ASTRONOMICAL_OBSERVATORY_P
    'PAL155':'TransportationGroundPnt',  // OVERHEAD_OBSTRUCTION_P
    'PAL165':'TransportationGroundPnt',  // PIPELINE_CROSSING_POINT_P
    'PAL170':'RecreationPnt',  // PUBLIC_SQUARE_P
    'PAL200':'CulturePnt',  // RUINS_P
    'PAL201':'CulturePnt',  // INTEREST_SITE_P
    'PAL208':'SettlementPnt',  // SHANTY_TOWN_P
    'PAL211':'TransportationGroundPnt',  // TRANS_ROUTE_PROTECT_STRUCT_P
    'PAL241':'StructurePnt',  // TOWER_P
    'PAL250':'StructurePnt',  // UNDERGROUND_DWELLING_P
    'PAL270':'AgriculturePnt',  // INDUSTRIAL_FARM_P
    'PAL351':'AeronauticPnt',  // SPACE_FACILITY_P
    'PAL371':'StructurePnt',  // MANOR_HOUSE_P
    'PAL375':'MilitaryPnt',  // CASTLE_P
    'PAL376':'MilitaryPnt',  // CASTLE_COMPLEX_P
    'PAL510':'AeronauticPnt',  // TETHERED_BALLOON_P
    'PAM010':'StoragePnt',  // STORAGE_DEPOT_P
    'PAM011':'StoragePnt',  // SHIPPING_CONTAINER_P
    'PAM020':'AgriculturePnt',  // GRAIN_STORAGE_STRUCTURE_P
    'PAM030':'StoragePnt',  // GRAIN_ELEVATOR_P
    'PAM040':'IndustryPnt',  // MINERAL_PILE_P
    'PAM060':'MilitaryPnt',  // SURFACE_BUNKER_P
    'PAM065':'StoragePnt',  // MUNITION_STORAGE_FACILITY_P
    'PAM070':'StoragePnt',  // STORAGE_TANK_P
    'PAM071':'StoragePnt',  // TANK_FARM_P
    'PAM075':'StoragePnt',  // FUEL_STORAGE_FACILITY_P
    'PAM080':'StoragePnt',  // WATER_TOWER_P
    'PAN060':'TransportationGroundPntExt',  // RAILWAY_YARD_P
    'PAN075':'TransportationGroundPnt',  // RAILWAY_TURNTABLE_P
    'PAN076':'TransportationGroundPnt',  // ROUNDHOUSE_P
    'PAN085':'TransportationGroundPnt',  // RAILWAY_SIGNAL_P
    'PAP020':'TransportationGroundPnt',  // ROAD_INTERCHANGE_P
    'PAP033':'TransportationGroundPnt',  // ENGINEERED_TURNAROUND_SITE_P
    'PAP040':'TransportationGroundPnt',  // GATE_P
    'PAP041':'TransportationGroundPnt',  // VEHICLE_BARRIER_P
    'PAP056':'TransportationGroundPnt',  // TANK_CROSSING_P
    'PAQ040':'TransportationGroundPnt',  // BRIDGE_P
    'PAQ045':'TransportationGroundPnt',  // BRIDGE_SPAN_P
    'PAQ055':'TransportationGroundPnt',  // BRIDGE_TOWER_P
    'PAQ056':'TransportationGroundPnt',  // BRIDGE_PIER_P
    'PAQ059':'TransportationGroundPnt',  // TRANS_ROUTE_CHARACTER_CHANGE_P
    'PAQ060':'AeronauticPnt',  // CONTROL_TOWER_P
    'PAQ062':'TransportationGroundPnt',  // CROSSING_P
    'PAQ065':'TransportationGroundPnt',  // CULVERT_P
    'PAQ068':'TransportationGroundPnt',  // TRANSPORTATION_BLOCK_P
    'PAQ070':'TransportationWaterPntExt',  // FERRY_CROSSING_P
    'PAQ080':'TransportationWaterPnt',  // FERRY_STATION_P
    'PAQ095':'TransportationGroundPnt',  // TUNNEL_MOUTH_P
    'PAQ110':'AeronauticPnt',  // MOORING_MAST_P
    'PAQ111':'TransportationWaterPnt',  // PREPARED_WATERCOURSE_CROSS_P
    'PAQ114':'UtilityInfrastructurePnt',  // STORM_DRAIN_P
    'PAQ115':'UtilityInfrastructurePnt',  // UTILITY_ACCESS_POINT_P
    'PAQ116':'UtilityInfrastructurePnt',  // PUMPING_STATION_P
    'PAQ118':'TransportationGroundPnt',  // SHARP_CURVE_P
    'PAQ125':'TransportationGroundPnt',  // TRANSPORTATION_STATION_P
    'PAQ135':'TransportationGroundPnt',  // ROADSIDE_REST_AREA_P
    'PAQ141':'TransportationGroundPnt',  // PARKING_GARAGE_P
    'PAQ160':'TransportationGroundPnt',  // TRAFFIC_LIGHT_P
    'PAQ161':'TransportationGroundPnt',  // STREET_LAMP_P
    'PAQ162':'TransportationGroundPnt',  // STREET_SIGN_P
    'PAQ170':'TransportationGroundPnt',  // MOTOR_VEHICLE_STATION_P
    'PAT010':'UtilityInfrastructurePnt',  // DISH_AERIAL_P
    'PAT011':'UtilityInfrastructurePnt',  // AERIAL_P
    'PAT012':'UtilityInfrastructurePnt',  // AERIAL_FARM_P
    'PAT042':'UtilityInfrastructurePnt',  // PYLON_P
    'PAT045':'FacilityPnt',  // RADAR_STATION_P
    'PBA030':'PhysiographyPnt',  // ISLAND_P
    'PBB009':'PortHarbourPnt',  // PORT_P
    'PBB010':'PortHarbourPntExt',  // ANCHORAGE_P
    'PBB019':'PortHarbourPntExt',  // ANCHOR_P
    'PBB020':'PortHarbourPntExt',  // BERTH_P
    'PBB050':'PortHarbourPntExt',  // CALLING_IN_POINT_P
    'PBB080':'PortHarbourPntExt',  // DOLPHIN_P
    'PBB110':'HydrographyPnt',  // FISH_WEIR_P
    'PBB150':'MilitaryPntExt',  // BEACH_LANDING_SITE_P
    'PBB155':'HydroAidNavigationPntExt',  // MARITIME_SIGNAL_STATION_P
    'PBB201':'PortHarbourPnt',  // SMALL_CRAFT_FACILITY_P
    'PBB241':'PortHarbourPnt',  // SHIPYARD_P
    'PBC010':'HydroAidNavigationPntExt',  // MARITIME_NAVIGATION_BEACON_P
    'PBC020':'HydroAidNavigationPntExt',  // BUOY_P
    'PBC034':'HydroAidNavigationPntExt',  // MARITIME_RADIOBEACON_P
    'PBC040':'HydroAidNavigationPntExt',  // MARITIME_NAVIGATION_LIGHT_P
    'PBC041':'HydroAidNavigationPntExt',  // MARITIME_NAV_LIGHT_SUPPORT_P
    'PBC050':'HydroAidNavigationPnt',  // LIGHTHOUSE_P
    'PBC055':'HydroAidNavigationPntExt',  // MARITIME_NAVIGATION_MARKER_P
    'PBC060':'HydroAidNavigationPntExt',  // LIGHT_SECTOR_P
    'PBC070':'HydroAidNavigationPnt',  // LIGHT_VESSEL_P
    'PBC080':'HydroAidNavigationPntExt',  // INSUBSTANTIAL_NAV_MARK_P
    'PBD020':'HydrographyPntExt',  // CRIB_P
    'PBD030':'HydrographyPntExt',  // DISCOLOURED_WATER_P
    'PBD050':'HydrographyPntExt',  // FOUL_GROUND_P
    'PBD061':'HydrographyPntExt',  // AQUATIC_VEGETATION_P
    'PBD100':'PortHarbourPnt',  // STRUCTURAL_PILE_P
    'PBD115':'HydrographyPnt',  // OFFSHORE_CONSTRUCTION_P
    'PBD130':'HydrographyPntExt',  // HAZARDOUS_ROCK_P
    'PBD140':'HydrographyPnt',  // SNAG_P
    'PBD180':'HydrographyPntExt',  // WRECK_P
    'PBD181':'HydrographyPnt',  // HULK_P
    'PBE020':'HydrographyPntExt',  // SOUNDING_P
    'PBF010':'HydrographyPntExt',  // BOTTOM_CHARACTER_REGION_P
    'PBG010':'HydrographyPntExt',  // WATER_MOVEMENT_DATA_LOCATION_P
    'PBG012':'HydrographyPntExt',  // WATER_TURBULENCE_P
    'PBG030':'HydrographyPntExt',  // TIDAL_STREAM_OBSERVE_STATION_P
    'PBH012':'HydrographyPnt',  // QANAT_SHAFT_P
    'PBH051':'AgriculturePnt',  // FISH_FARM_FACILITY_P
    'PBH070':'TransportationGroundPnt',  // FORD_P
    'PBH075':'CulturePnt',  // FOUNTAIN_P
    'PBH082':'HydrographyPnt',  // INLAND_WATERBODY_P
    'PBH120':'HydrographyPnt',  // RAPIDS_P
    'PBH145':'HydrographyPnt',  // VANISHING_POINT_P
    'PBH155':'IndustryPnt',  // SALT_EVAPORATOR_P
    'PBH170':'HydrographyPnt',  // NATURAL_POOL_P
    'PBH180':'HydrographyPnt',  // WATERFALL_P
    'PBH220':'UtilityInfrastructurePnt',  // WATERWORK_P
    'PBH230':'HydrographyPnt',  // WATER_WELL_P
    'PBI006':'TransportationWaterPnt',  // SHIP_ELEVATOR_P
    'PBI010':'HydrographyPnt',  // CISTERN_P
    'PBI020':'HydrographyPnt',  // DAM_P
    'PBI030':'TransportationWaterPnt',  // LOCK_P
    'PBI040':'HydrographyPnt',  // SLUICE_GATE_P
    'PBI044':'HydrographyPnt',  // FLOOD_CONTROL_STRUCTURE_P
    'PBI045':'TransportationWaterPnt',  // BASIN_GATE_P
    'PBI050':'HydrographyPnt',  // WATER_INTAKE_TOWER_P
    'PBI070':'HydrographyPnt',  // GAUGING_STATION_P
    'PBJ060':'PhysiographyPnt',  // ICE_PEAK_P
    'PCA030':'HypsographyPnt',  // SPOT_ELEVATION_P
    'PDB029':'PhysiographyPnt',  // CAVE_MOUTH_P
    'PDB080':'PhysiographyPntExt',  // DEPRESSION_P
    'PDB115':'PhysiographyPnt',  // GEOTHERMAL_OUTLET_P
    'PDB150':'PhysiographyPnt',  // MOUNTAIN_PASS_P
    'PDB160':'PhysiographyPnt',  // ROCK_FORMATION_P
    'PDB180':'PhysiographyPnt',  // VOLCANO_P
    'PDB185':'PhysiographyPntExt',  // CRATER_P
    'PEA040':'AgriculturePntExt',  // ORCHARD_P
    'PEA050':'AgriculturePntExt',  // VINEYARD_P
    'PEC005':'VegetationPnt',  // TREE_P
    'PEC020':'PhysiographyPnt',  // OASIS_P
    'PEC050':'VegetationPntExt',  // GROVE_P
    'PFA012':'CulturePnt',  // CONTAMINATED_REGION_P
    'PFA015':'MilitaryPnt',  // FIRING_RANGE_P
    'PFA165':'MilitaryPnt',  // TRAINING_SITE_P
    'PFC041':'HydrographyPntExt',  // TRAFFIC_SEPARATION_SCHEME_P
    'PGA033':'AeronauticPntExt',  // AERO_RADIO_NAV_INSTALLATION_P
    'PGB005':'AeronauticPnt',  // LAND_AERODROME_P
    'PGB013':'AeronauticPntExt',  // AERODROME_BEACON_P
    'PGB030':'AeronauticPnt',  // HELIPAD_P
    'PGB035':'AeronauticPnt',  // HELIPORT_P
    'PGB040':'AeronauticPnt',  // LAUNCH_PAD_P
    'PGB065':'AeronauticPnt',  // WATER_AERODROME_P
    'PGB230':'AeronauticPnt',  // AIRCRAFT_HANGAR_P
    'PGB250':'AeronauticPnt',  // HARDENED_AIRCRAFT_SHELTER_P
    'PSTB23':'AeronauticPntExt',  // LANDING_ZONE_P
    'PSTDPP':'UtilityInfrastructurePntExt',  // STANDPIPE_P
    'PSU001':'MilitaryPnt',  // MILITARY_INSTALLATION_P
    'PT0181':'UtilityInfrastructurePntExt',  // PUMP_P
    'PZB020':'HypsographyPntExt',  // BENCHMARK_P
    'PZB030':'BoundaryPnt',  // BOUNDARY_MONUMENT_P
    'PZB036':'TransportationGroundPntExt',  // DISTANCE_MARK_P
    'PZB050':'HypsographyPnt',  // SURVEY_POINT_P
    'PZB060':'HypsographyPntExt',  // GEODETIC_POINT_P
    'PZD030':'CulturePntExt',  // CULTURAL_CONTEXT_LOCATION_P
    'PZD040':'InformationPnt',  // NAMED_LOCATION_P
    'PZD045':'InformationPnt',  // ANNOTATED_LOCATION_P
    'PZD070':'HydrographyPnt',  // WATER_MEASUREMENT_LOCATION_P
  }, // End of thematicGroupList
  // ##### End of Thematic Group Rules #####

  // ##### Start of ESRI FCSubtype Rules #####
  subtypeList : {
    'AA010':'100001', // EXTRACTION_MINE
    'AA011':'100002', // SHEAR_WALL
    'AA020':'100003', // MINE_SHAFT_SUPERSTRUCTURE
    'AA040':'100004', // RIG
    'AA045':'100696', // BOREHOLE
    'AA052':'100006', // HYDROCARBONS_FIELD
    'AA054':'155023', // NON_WATER_WELL
    'AB000':'100007', // DISPOSAL_SITE
    'AB010':'100008', // RECYCLING_SITE
    'AB021':'100009', // LIQUID_DIFFUSER
    'AB040':'100010', // AERATION_BASIN
    'AB507':'177962', // WASTE_HEAP
    'AC010':'100012', // BLAST_FURNACE
    'AC020':'100013', // CATALYTIC_CRACKER
    'AC030':'100014', // SETTLING_POND
    'AC040':'100015', // HYDROCARBON_PROD_FACILITY
    'AC060':'100016', // INDUSTRIAL_FURNACE
    'AC507':'134665', // SEWAGE_TREATMENT_PLANT
    'AD010':'100018', // ELECTRIC_POWER_STATION
    'AD020':'100019', // SOLAR_PANEL
    'AD025':'100020', // SOLAR_FARM
    'AD030':'100021', // POWER_SUBSTATION
    'AD041':'100022', // NUCLEAR_REACTOR_CONTAINMENT
    'AD050':'100023', // HEATING_FACILITY
    'AD055':'100697', // COOLING_FACILITY
    'AD060':'100687', // WIND_FARM
    'AF010':'100025', // SMOKESTACK
    'AF020':'100026', // CONVEYOR
    'AF030':'100028', // COOLING_TOWER
    'AF040':'100029', // CRANE
    'AF050':'100030', // EXCAVATING_MACHINE
    'AF060':'100031', // ENGINE_TEST_CELL
    'AF070':'100032', // FLARE_PIPE
    'AF080':'100033', // HOPPER
    'AG030':'100034', // SHOPPING_COMPLEX
    'AG040':'100035', // OFFICE_PARK
    'AG050':'121638', // BILLBOARD
    'AH025':'132596', // ENGINEERED_EARTHWORK
    'AH055':'132626', // FORTIFIED_BUILDING
    'AH060':'100038', // UNDERGROUND_BUNKER
    'AH070':'100039', // CHECKPOINT
    'AI020':'100040', // CARAVAN_PARK
    'AI021':'133168', // MANUFACTURED_HOME_PARK
    'AI030':'100041', // CAMP
    'AJ030':'100043', // HOLDING_PEN
    'AJ050':'100044', // WINDMILL
    'AJ051':'100045', // WIND_TURBINE
    'AJ055':'100046', // WATER_MILL
    'AJ080':'100049', // STABLE
    'AJ085':'100691', // BARN
    'AJ110':'100052', // GREENHOUSE
    'AK020':'100053', // AMUSEMENT_PARK_ATTRACTION
    'AK030':'100054', // AMUSEMENT_PARK
    'AK040':'100055', // SPORTS_GROUND
    'AK060':'100057', // CAMP_SITE
    'AK061':'100058', // PICNIC_SITE
    'AK070':'100059', // DRIVE_IN_THEATRE
    'AK080':'100060', // OUTDOOR_THEATRE_SCREEN
    'AK090':'100061', // FAIRGROUND
    'AK100':'100062', // GOLF_COURSE
    'AK101':'100063', // GOLF_DRIVING_RANGE
    'AK110':'100064', // GRANDSTAND
    'AK120':'100065', // PARK
    'AK121':'100066', // LOOKOUT
    'AK130':'100069', // RACETRACK
    'AK150':'100072', // SKI_JUMP
    'AK155':'100073', // SKI_RUN
    'AK160':'154703', // STADIUM
    'AK161':'121747', // SCOREBOARD
    'AK164':'100074', // AMPHITHEATRE
    'AK170':'100077', // SWIMMING_POOL
    'AK180':'100078', // ZOO
    'AL010':'100080', // FACILITY
    'AL011':'100081', // INSTALLATION
    'AL012':'100082', // ARCHAEOLOGICAL_SITE
    'AL013':'100083', // BUILDING
    'AL014':'100084', // NON_BUILDING_STRUCTURE
    'AL017':'100086', // FIRE_HYDRANT
    'AL018':'100087', // BUILDING_SUPERSTRUCTURE
    'AL019':'100088', // SHED
    'AL020':'100089', // BUILT_UP_AREA
    'AL024':'191547', // NEIGHBOURHOOD
    'AL025':'100091', // CAIRN
    'AL030':'100092', // CEMETERY
    'AL036':'100094', // TOMB
    'AL060':'100096', // DRAGONS_TEETH
    'AL065':'100097', // MINEFIELD
    'AL070':'100098', // FENCE
    'AL073':'100099', // FLAGPOLE
    'AL080':'100101', // GANTRY
    'AL099':'100103', // HUT
    'AL105':'100104', // SETTLEMENT
    'AL110':'100105', // LIGHT_SUPPORT_STRUCTURE
    'AL120':'100106', // MISSILE_SITE
    'AL121':'100107', // ANTI_AIRCRAFT_ARTILLERY_SITE
    'AL130':'100108', // MEMORIAL_MONUMENT
    'AL140':'100110', // PARTICLE_ACCELERATOR
    'AL142':'100111', // ASTRONOMICAL_OBSERVATORY
    'AL155':'100112', // OVERHEAD_OBSTRUCTION
    'AL165':'100113', // PIPELINE_CROSSING_POINT
    'AL170':'100114', // PUBLIC_SQUARE
    'AL175':'100688', // COURTYARD
    'AL180':'100689', // RETAIL_STAND
    'AL195':'100115', // RAMP
    'AL200':'100116', // RUINS
    'AL201':'100117', // INTEREST_SITE
    'AL208':'100118', // SHANTY_TOWN
    'AL211':'130921', // TRANS_ROUTE_PROTECT_STRUCT
    'AL241':'100122', // TOWER
    'AL250':'100123', // UNDERGROUND_DWELLING
    'AL260':'100124', // WALL
    'AL270':'100129', // INDUSTRIAL_FARM
    'AL351':'100126', // SPACE_FACILITY
    'AL371':'180086', // MANOR_HOUSE
    'AL375':'100128', // CASTLE
    'AL376':'132642', // CASTLE_COMPLEX
    'AL510':'100130', // TETHERED_BALLOON
    'AM010':'100131', // STORAGE_DEPOT
    'AM011':'100132', // SHIPPING_CONTAINER
    'AM020':'100133', // GRAIN_STORAGE_STRUCTURE
    'AM030':'100134', // GRAIN_ELEVATOR
    'AM040':'100136', // MINERAL_PILE
    'AM060':'100137', // SURFACE_BUNKER
    'AM065':'100138', // MUNITION_STORAGE_FACILITY
    'AM070':'100139', // STORAGE_TANK
    'AM071':'100140', // TANK_FARM
    'AM075':'100141', // FUEL_STORAGE_FACILITY
    'AM080':'100142', // WATER_TOWER
    'AN010':'100143', // RAILWAY
    'AN050':'100144', // RAILWAY_SIDETRACK
    'AN060':'100145', // RAILWAY_YARD
    'AN075':'100146', // RAILWAY_TURNTABLE
    'AN076':'100147', // ROUNDHOUSE
    'AN085':'100149', // RAILWAY_SIGNAL
    'AP010':'100150', // CART_TRACK
    'AP020':'100151', // ROAD_INTERCHANGE
    'AP030':'100152', // ROAD
    'AP033':'179969', // ENGINEERED_TURNAROUND_SITE
    'AP040':'100154', // GATE
    'AP041':'100155', // VEHICLE_BARRIER
    'AP050':'100156', // TRAIL
    'AP055':'179906', // TANK_TRAIL
    'AP056':'180006', // TANK_CROSSING
    'AQ035':'100159', // SIDEWALK
    'AQ040':'100161', // BRIDGE
    'AQ045':'100162', // BRIDGE_SPAN
    'AQ050':'100163', // BRIDGE_SUPERSTRUCTURE
    'AQ055':'100164', // BRIDGE_TOWER
    'AQ056':'100165', // BRIDGE_PIER
    'AQ059':'131083', // TRANS_ROUTE_CHARACTER_CHANGE
    'AQ060':'100167', // CONTROL_TOWER
    'AQ062':'100168', // CROSSING
    'AQ063':'130381', // CAUSEWAY_STRUCTURE
    'AQ065':'100170', // CULVERT
    'AQ068':'100171', // TRANSPORTATION_BLOCK
    'AQ070':'100172', // FERRY_CROSSING
    'AQ075':'100173', // ICE_ROUTE
    'AQ080':'100174', // FERRY_STATION
    'AQ095':'100176', // TUNNEL_MOUTH
    'AQ110':'100177', // MOORING_MAST
    'AQ111':'100178', // PREPARED_WATERCOURSE_CROSS
    'AQ113':'100179', // PIPELINE
    'AQ114':'100180', // STORM_DRAIN
    'AQ115':'100181', // UTILITY_ACCESS_POINT
    'AQ116':'100182', // PUMPING_STATION
    'AQ118':'100183', // SHARP_CURVE
    'AQ120':'100185', // STEEP_GRADE
    'AQ125':'100186', // TRANSPORTATION_STATION
    'AQ130':'100187', // TUNNEL
    'AQ135':'100188', // ROADSIDE_REST_AREA
    'AQ140':'100189', // VEHICLE_LOT
    'AQ141':'100190', // PARKING_GARAGE
    'AQ150':'100191', // STAIR
    'AQ151':'100192', // ARCADE
    'AQ160':'100194', // TRAFFIC_LIGHT
    'AQ161':'100195', // STREET_LAMP
    'AQ162':'100196', // STREET_SIGN
    'AQ170':'100197', // MOTOR_VEHICLE_STATION
    'AT005':'100199', // CABLE
    'AT010':'100200', // DISH_AERIAL
    'AT011':'100201', // AERIAL
    'AT012':'100202', // AERIAL_FARM
    'AT041':'100206', // CABLEWAY
    'AT042':'100558', // PYLON
    'AT045':'100207', // RADAR_STATION
    'BA010':'100212', // LAND_WATER_BOUNDARY
    'BA023':'100215', // FORESHORE
    'BA024':'100216', // SHORELINE
    'BA030':'100217', // ISLAND
    'BA040':'100218', // TIDAL_WATER
    'BA050':'100219', // BEACH
    'BB005':'100222', // HARBOUR
    'BB008':'103', // HARBOUR_WATERS
    'BB009':'100223', // PORT
    'BB010':'100224', // ANCHORAGE
    'BB019':'100226', // ANCHOR
    'BB020':'100227', // BERTH
    'BB050':'100229', // CALLING_IN_POINT
    'BB080':'100230', // DOLPHIN
    'BB081':'100231', // SHORELINE_CONSTRUCTION
    'BB082':'100232', // SHORELINE_RAMP
    'BB090':'100233', // DRY_DOCK
    'BB100':'100235', // FISHING_STAKES
    'BB110':'100236', // FISH_WEIR
    'BB115':'100237', // NAUTICAL_GRIDIRON
    'BB150':'100239', // BEACH_LANDING_SITE
    'BB155':'100241', // MARITIME_SIGNAL_STATION
    'BB199':'100243', // FLOATING_DRY_DOCK
    'BB201':'100244', // SMALL_CRAFT_FACILITY
    'BB241':'100245', // SHIPYARD
    'BC010':'100247', // MARITIME_NAVIGATION_BEACON
    'BC020':'100248', // BUOY
    'BC034':'100251', // MARITIME_RADIOBEACON
    'BC040':'100252', // MARITIME_NAVIGATION_LIGHT
    'BC041':'134663', // MARITIME_NAV_LIGHT_SUPPORT
    'BC050':'100253', // LIGHTHOUSE
    'BC055':'100254', // MARITIME_NAVIGATION_MARKER
    'BC060':'100255', // LIGHT_SECTOR
    'BC070':'100256', // LIGHT_VESSEL
    'BC080':'100257', // INSUBSTANTIAL_NAV_MARK
    'BD020':'100264', // CRIB
    'BD030':'100265', // DISCOLOURED_WATER
    'BD050':'100266', // FOUL_GROUND
    'BD061':'100692', // AQUATIC_VEGETATION
    'BD100':'100271', // STRUCTURAL_PILE
    'BD115':'100272', // OFFSHORE_CONSTRUCTION
    'BD120':'100273', // REEF
    'BD130':'100276', // HAZARDOUS_ROCK
    'BD140':'100277', // SNAG
    'BD180':'100278', // WRECK
    'BD181':'100279', // HULK
    'BE010':'100280', // DEPTH_CURVE
    'BE015':'100281', // DEPTH_CONTOUR
    'BE019':'100282', // DEPTH_AREA
    'BE020':'100283', // SOUNDING
    'BE050':'100287', // BEACH_PROFILE
    'BF010':'100288', // BOTTOM_CHARACTER_REGION
    'BG010':'100289', // WATER_MOVEMENT_DATA_LOCATION
    'BG012':'100290', // WATER_TURBULENCE
    'BG030':'100293', // TIDAL_STREAM_OBSERVE_STATION
    'BH010':'100295', // AQUEDUCT
    'BH012':'131749', // QANAT_SHAFT
    'BH015':'100296', // BOG
    'BH020':'100297', // NAVIGABLE_CANAL
    'BH030':'100298', // DITCH
    'BH040':'100299', // WATER_TREATMENT_BED
    'BH050':'191874', // MARICULTURE_SITE
    'BH051':'191951', // FISH_FARM_FACILITY
    'BH065':'131810', // WATER_RACE
    'BH070':'100302', // FORD
    'BH075':'100303', // FOUNTAIN
    'BH077':'100304', // HUMMOCK
    'BH082':'130384', // INLAND_WATERBODY
    'BH090':'100307', // LAND_SUBJECT_TO_INUNDATION
    'BH100':'100309', // MOAT
    'BH110':'100310', // PENSTOCK
    'BH116':'154640', // AQUIFER
    'BH120':'100311', // RAPIDS
    'BH135':'100313', // RICE_FIELD
    'BH140':'100314', // RIVER
    'BH145':'100315', // VANISHING_POINT
    'BH150':'100316', // SALT_FLAT
    'BH155':'100317', // SALT_EVAPORATOR
    'BH160':'100318', // SABKHA
    'BH165':'100319', // SPILLWAY
    'BH170':'100320', // NATURAL_POOL
    'BH180':'100321', // WATERFALL
    'BH190':'100322', // LAGOON
    'BH220':'100325', // WATERWORK
    'BH230':'100326', // WATER_WELL
    'BI005':'100328', // VESSEL_LIFT
    'BI006':'132749', // SHIP_ELEVATOR
    'BI010':'100329', // CISTERN
    'BI020':'100330', // DAM
    'BI030':'100331', // LOCK
    'BI040':'100334', // SLUICE_GATE
    'BI044':'131207', // FLOOD_CONTROL_STRUCTURE
    'BI045':'131206', // BASIN_GATE
    'BI050':'100337', // WATER_INTAKE_TOWER
    'BI060':'100338', // FISH_LADDER
    'BI070':'100339', // GAUGING_STATION
    'BJ020':'100340', // MORAINE
    'BJ030':'100341', // GLACIER
    'BJ031':'100342', // CREVASSE
    'BJ040':'100343', // ICE_CLIFF
    'BJ060':'100344', // ICE_PEAK
    'BJ065':'100345', // ICE_SHELF
    'BJ070':'100346', // PACK_ICE
    'BJ080':'100347', // POLAR_ICE
    'BJ099':'100348', // ICE_CAP
    'BJ100':'100349', // SNOW_ICE_FIELD
    'BJ110':'100350', // TUNDRA
    'CA010':'100353', // ELEVATION_CONTOUR
    'CA030':'100355', // SPOT_ELEVATION
    'DA005':'100356', // ASPHALT_LAKE
    'DA010':'100358', // SOIL_SURFACE_REGION
    'DB010':'100362', // STEEP_TERRAIN_FACE
    'DB028':'154959', // CAVE_CHAMBER
    'DB029':'154961', // CAVE_MOUTH
    'DB061':'100365', // CREVICE
    'DB070':'100366', // CUT
    'DB071':'192101', // CUT_LINE
    'DB080':'100367', // DEPRESSION
    'DB090':'100368', // EMBANKMENT
    'DB100':'100369', // ESKER
    'DB110':'100370', // GEOLOGIC_FAULT
    'DB115':'100371', // GEOTHERMAL_OUTLET
    'DB150':'100372', // MOUNTAIN_PASS
    'DB160':'100373', // ROCK_FORMATION
    'DB170':'100374', // SAND_DUNES
    'DB180':'100375', // VOLCANO
    'DB185':'100376', // CRATER
    'DB190':'100377', // VOLCANIC_DYKE
    'DB211':'100379', // LANDSLIDE_MASS
    'EA010':'100380', // CROP_LAND
    'EA020':'100381', // HEDGEROW
    'EA030':'100382', // PLANT_NURSERY
    'EA031':'100383', // BOTANIC_GARDEN
    'EA040':'100384', // ORCHARD
    'EA050':'100385', // VINEYARD
    'EA055':'100386', // HOP_FIELD
    'EB010':'100387', // GRASSLAND
    'EB020':'100388', // THICKET
    'EB070':'100390', // BRUSH
    'EC005':'100392', // TREE
    'EC010':'100393', // CANE
    'EC015':'130380', // FOREST
    'EC020':'100394', // OASIS
    'EC040':'100396', // CLEARED_WAY
    'EC050':'100397', // GROVE
    'EC060':'100398', // FOREST_CLEARING
    'ED010':'100399', // MARSH
    'ED020':'100400', // SWAMP
    'EE010':'100401', // LOGGING_SITE
    'EE030':'100403', // DESERT
    'FA000':'100405', // ADMINISTRATIVE_BOUNDARY
    'FA002':'100406', // GEOPOLITICAL_ENTITY
    'FA003':'100407', // ADMINISTRATIVE_SUBDIVISION
    'FA012':'100409', // CONTAMINATED_REGION
    'FA015':'100410', // FIRING_RANGE
    'FA100':'100414', // TEST_SITE
    'FA110':'100415', // INTERNATIONAL_DATE_LINE
    'FA165':'100416', // TRAINING_SITE
    'FA210':'100417', // CONSERVATION_AREA
    'FC021':'100418', // MARITIME_LIMIT
    'FC034':'106241', // DREDGED_AREA
    'FC035':'100420', // WATERBODY_DIVIDER
    'FC037':'100419', // MARITIME_CAUTION_AREA
    'FC041':'100422', // TRAFFIC_SEPARATION_SCHEME
    'FC100':'100426', // MEASURED_DISTANCE_LINE
    'FC130':'100429', // MARITIME_RADAR_REF_LINE
    'FC165':'100430', // MARITIME_ROUTE
    'FC177':'100432', // SWEPT_AREA
    'GA033':'171313', // AERO_RADIO_NAV_INSTALLATION
    'GB005':'100436', // LAND_AERODROME
    'GB013':'133409', // AERODROME_BEACON
    'GB015':'100438', // APRON
    'GB030':'100441', // HELIPAD
    'GB035':'100442', // HELIPORT
    'GB040':'100443', // LAUNCH_PAD
    'GB045':'100444', // STOPWAY
    'GB050':'100446', // DEFENSIVE_REVETMENT
    'GB055':'100448', // RUNWAY
    'GB065':'100452', // WATER_AERODROME
    'GB070':'100453', // SEAPLANE_RUN
    'GB075':'100454', // TAXIWAY
    'GB230':'100456', // AIRCRAFT_HANGAR
    'GB250':'100457', // HARDENED_AIRCRAFT_SHELTER
    'IA040':'100458', // LAND_PARCEL
    'NA170':'1813853', // DRAINAGE_BASIN
    'NM010':'200', // AQUEDUCT_AON
    'NM020':'201', // NAVIGABLE_CANAL_AON
    'NM030':'203', // DITCH_AON
    'NM082':'204', // INLAND_WATERBODY_AON
    'NM140':'208', // RIVER_AON
    'NM160':'209', // SABKHA_AON
    'NM170':'205', // NATURAL_POOL_AON
    'NM230':'212', // WATER_WELL_AON
    'NU010':'202', // CISTERN_AON
    'NU070':'210', // STORAGE_TANK_AON
    'NU113':'206', // PIPELINE_AON
    'NU116':'207', // PUMPING_STATION_AON
    'NU170':'211', // SWIMMING_POOL_AON
    'SA050':'100460', // SLOPE_REGION
    'STB23':'100', // LANDING_ZONE
    'STBM1':'102', // MILITARY_BOUNDARY
    'STDPP':'110', // STANDPIPE
    'SU001':'100462', // MILITARY_INSTALLATION
    'SU004':'170162', // CANTONMENT_AREA
    'SU030':'180061', // INSTALLATION_BOUNDARY
    'T0181':'107', // PUMP
    'ZB020':'100464', // BENCHMARK
    'ZB030':'100465', // BOUNDARY_MONUMENT
    'ZB036':'100467', // DISTANCE_MARK
    'ZB050':'177997', // SURVEY_POINT
    'ZB060':'100468', // GEODETIC_POINT
    'ZC040':'100469', // LOCAL_MAGNETIC_ANOMALY
    'ZD020':'100473', // VOID_COLLECTION_AREA
    'ZD030':'100474', // CULTURAL_CONTEXT_LOCATION
    'ZD040':'100475', // NAMED_LOCATION
    'ZD045':'100476', // ANNOTATED_LOCATION
    'ZD070':'164755', // WATER_MEASUREMENT_LOCATION
    'ZI031':'121591', // DATASET
    'ZI039':'132721', // ENTITY_COLLECTION_METADATA
  }, // End of subtypeList
  // ##### End of ESRI FCSubtype Rules #####

}; // End of ggdm30.rules
