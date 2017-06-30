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
     ['F_CODE','AA010','landuse','quarry'], // Extraction Mine
     ['F_CODE','AK120','leisure','playground'], // Park
     ['F_CODE','AK120','landuse','recreation_ground'], // Park
     ['F_CODE','AK120','leisure','common'], // Park - close but not great
     ['F_CODE','AL020','landuse','residential'], // From OSM - Map to BUA
     ['F_CODE','AL020','landuse','industrial'], // From OSM - Map to BUA
     ['F_CODE','AL024','place','neighborhood'], // Neighbourhood - US spelling
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
     'ZI001_SDP':'source:description', // Source : Source Description
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
     'LC1':'bridge:load_class1', // Load Class Type 1
     'LC2':'bridge:load_class2', // Load Class Type 2
     'LC3':'bridge:load_class3', // Load Class Type 3
     'LC4':'bridge:load_class4', // Load Class Type 4
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
     'ZI005_GNR':'raw:ZI005_GNR', // Geo Name Info : Geographic Name Rank
     'ZI005_GNR2':'raw:ZI005_GNR2', // Geo Name Info : Geographic Name Rank [2]
     'ZI005_GNR3':'raw:ZI005_GNR3', // Geo Name Info : Geographic Name Rank [3]
     'ZI015_GCUL':'raw:ZI015_GCUL', // Geo Name Collection : Geographic Name Cartographic Usability Range <lower value>
     'ZI015_GCUU':'raw:ZI015_GCUU', // Geo Name Collection : Geographic Name Cartographic Usability Range <upper value>
     'ZI016_WD1':'width:minimum_traveled_way', // Route Pavement Information : Route Minimum Travelled Way Width
     'ZI017_GAW':'gauge', // Track Information : Railway Gauge
     'ZI018_BRF':'raw:ZI018_BRF', // Wireless : Broadcast Frequency
     'ZI024_ASE':'water:arsnic_concentration', // Water : Arsenic Concentration
     'ZI024_AWO':'raw:ZI024_AWO', // Water : Predominant Water Flow
     'ZI024_AWOI':'raw:ZI024_AWOI', // Water : Predominant Water Flow - Maximum
     'ZI024_AWOX':'raw:ZI024_AWOX', // Water : Predominant Water Flow - Minimum
     'ZI024_CFR':'water:coliform_concentration', // Water : Coliform Concentration
     'ZI024_CLO':'water:chloride_concentration', // Water : Chloride Concentration
     'ZI024_CYN':'water:cyanide_concentration', // Water : Cyanide Concentration
     'ZI024_FMR':'raw:ZI024_FMR', // Water : Freshwater Production Maximum Rate
     'ZI024_FRR':'raw:ZI024_FRR', // Water : Freshwater Production Rate
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
//     'FCSUBTYPE':'etds:fcsubtype', // Very ESRI Specific. Ignored for now
     }, // End numBiased


    // Common one2one rules. Used for both import and export
    // taken from TDSv60 with the NGA TDSv61 additions
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
     // ['APT2','-999999',undefined,undefined], // No Information
     ['APT2','1','aeroway:use2','major_airfield'], // Major Airfield
     ['APT2','2','aeroway:use2','minor_airfield'], // Minor Airfield
     ['APT2','3','aeroway:use2','general_aviation_aircraft_operating_only'], // General Aviation Aircraft Operating Only
     ['APT2','5','aeroway:use2','glider_site'], // Glider Site
     ['APT2','6','aeroway:use2','ultralight_site'], // Ultralight Site
     ['APT2','7','aeroway:use2','hang_glider_site'], // Hang Glider Site
     ['APT2','8','aeroway:use2','winch_launched_hang_glider_site'], // Winch Launched Hang Glider Site
     ['APT2','12','aeroway:use2','emergency'], // Emergency
     ['APT2','13','aeroway:use2','parascending_site'], // Parascending Site
     ['APT2','16','aeroway:use2','search_and_rescue_airfield'], // Search and Rescue Airfield
     ['APT2','999','aeroway:use2','other'], // Other

     // APT3 - Airfield Use [3]
     // ['APT3','-999999',undefined,undefined], // No Information
     ['APT3','1','aeroway:use:3','major_airfield'], // Major Airfield
     ['APT3','2','aeroway:use:3','minor_airfield'], // Minor Airfield
     ['APT3','3','aeroway:use:3','general_aviation_aircraft_operating_only'], // General Aviation Aircraft Operating Only
     ['APT3','5','aeroway:use:3','glider_site'], // Glider Site
     ['APT3','6','aeroway:use:3','ultralight_site'], // Ultralight Site
     ['APT3','7','aeroway:use:3','hang_glider_site'], // Hang Glider Site
     ['APT3','8','aeroway:use:3','winch_launched_hang_glider_site'], // Winch Launched Hang Glider Site
     ['APT3','12','aeroway:use:3','emergency'], // Emergency
     ['APT3','13','aeroway:use:3','parascending_site'], // Parascending Site
     ['APT3','16','aeroway:use:3','search_and_rescue_airfield'], // Search and Rescue Airfield
     ['APT3','999','aeroway:use:3','other'], // Other

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
     // ['APU2','-999999',undefined,undefined], // No Information
     ['APU2','1','apron:use2','parking'], // Parking
     ['APU2','2','apron:use2','loading_and/or_unloading'], // Loading and/or Unloading
     ['APU2','3','apron:use2','fuelling'], // Fuelling
     ['APU2','4','apron:use2','transient'], // Transient
     ['APU2','5','apron:use2','compass_rose'], // Compass Rose
     ['APU2','6','apron:use2','engine_run_up'], // Engine Run Up
     ['APU2','7','apron:use2','maintenance'], // Maintenance
     ['APU2','8','apron:use2','alert'], // Alert
     ['APU2','9','apron:use2','firing-in'], // Firing-In
     ['APU2','10','apron:use2','hot_refuelling'], // Hot Refuelling
     ['APU2','11','apron:use2','weapon_loading'], // Weapon Loading
     ['APU2','12','apron:use2','de-icing_and/or_anti-icing'], // De-icing and/or Anti-icing
     ['APU2','13','apron:use2','ins_alignment'], // INS Alignment
     ['APU2','14','apron:use2','decontamination'], // Decontamination
     ['APU2','999','apron:use2','other'], // Other

     // APU3 - Apron Usage [3]
     // ['APU3','-999999',undefined,undefined], // No Information
     ['APU3','1','apron:use:3','parking'], // Parking
     ['APU3','2','apron:use:3','loading_and/or_unloading'], // Loading and/or Unloading
     ['APU3','3','apron:use:3','fuelling'], // Fuelling
     ['APU3','4','apron:use:3','transient'], // Transient
     ['APU3','5','apron:use:3','compass_rose'], // Compass Rose
     ['APU3','6','apron:use:3','engine_run_up'], // Engine Run Up
     ['APU3','7','apron:use:3','maintenance'], // Maintenance
     ['APU3','8','apron:use:3','alert'], // Alert
     ['APU3','9','apron:use:3','firing-in'], // Firing-In
     ['APU3','10','apron:use:3','hot_refuelling'], // Hot Refuelling
     ['APU3','11','apron:use:3','weapon_loading'], // Weapon Loading
     ['APU3','12','apron:use:3','de-icing_and/or_anti-icing'], // De-icing and/or Anti-icing
     ['APU3','13','apron:use:3','ins_alignment'], // INS Alignment
     ['APU3','14','apron:use:3','decontamination'], // Decontamination
     ['APU3','999','apron:use:3','other'], // Other

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
     // ['AQO2','-999999',undefined,undefined], // No Information
     ['AQO2','1','aquifer:overburden2','basalt'], // Basalt
     ['AQO2','2','aquifer:overburden2','boulders'], // Boulders
     ['AQO2','3','aquifer:overburden2','clay'], // Clay
     ['AQO2','4','aquifer:overburden2','conglomerate'], // Conglomerate
     ['AQO2','5','aquifer:overburden2','dolomite'], // Dolomite
     ['AQO2','6','aquifer:overburden2','granite'], // Granite
     ['AQO2','7','aquifer:overburden2','gravel'], // Gravel
     ['AQO2','8','aquifer:overburden2','igneous_rock'], // Igneous Rock
     ['AQO2','9','aquifer:overburden2','karst'], // Karst
     ['AQO2','10','aquifer:overburden2','limestone'], // Limestone
     ['AQO2','11','aquifer:overburden2','marl'], // Marl
     ['AQO2','12','aquifer:overburden2','metamorphic_rock'], // Metamorphic Rock
     ['AQO2','13','aquifer:overburden2','mud'], // Mud
     ['AQO2','14','aquifer:overburden2','sand'], // Sand
     ['AQO2','15','aquifer:overburden2','schist'], // Schist
     ['AQO2','16','aquifer:overburden2','unconsolidated'], // Unconsolidated
     ['AQO2','17','aquifer:overburden2','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['AQO2','18','aquifer:overburden2','volcanic_rock'], // Volcanic Rock
     ['AQO2','19','aquifer:overburden2','sandstone'], // Sandstone
     ['AQO2','20','aquifer:overburden2','shale'], // Shale
     ['AQO2','999','aquifer:overburden2','other'], // Other

     // AQO3 - Aquifer Overburden [3]
     // ['AQO3','-999999',undefined,undefined], // No Information
     ['AQO3','1','aquifer:overburden3','basalt'], // Basalt
     ['AQO3','2','aquifer:overburden3','boulders'], // Boulders
     ['AQO3','3','aquifer:overburden3','clay'], // Clay
     ['AQO3','4','aquifer:overburden3','conglomerate'], // Conglomerate
     ['AQO3','5','aquifer:overburden3','dolomite'], // Dolomite
     ['AQO3','6','aquifer:overburden3','granite'], // Granite
     ['AQO3','7','aquifer:overburden3','gravel'], // Gravel
     ['AQO3','8','aquifer:overburden3','igneous_rock'], // Igneous Rock
     ['AQO3','9','aquifer:overburden3','karst'], // Karst
     ['AQO3','10','aquifer:overburden3','limestone'], // Limestone
     ['AQO3','11','aquifer:overburden3','marl'], // Marl
     ['AQO3','12','aquifer:overburden3','metamorphic_rock'], // Metamorphic Rock
     ['AQO3','13','aquifer:overburden3','mud'], // Mud
     ['AQO3','14','aquifer:overburden3','sand'], // Sand
     ['AQO3','15','aquifer:overburden3','schist'], // Schist
     ['AQO3','16','aquifer:overburden3','unconsolidated'], // Unconsolidated
     ['AQO3','17','aquifer:overburden3','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['AQO3','18','aquifer:overburden3','volcanic_rock'], // Volcanic Rock
     ['AQO3','19','aquifer:overburden3','sandstone'], // Sandstone
     ['AQO3','20','aquifer:overburden3','shale'], // Shale
     ['AQO3','999','aquifer:overburden3','other'], // Other

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
     // ['AQP2','-999999',undefined,undefined], // No Information
     ['AQP2','1','aquifer:composition2','basalt'], // Basalt
     ['AQP2','2','aquifer:composition2','boulders'], // Boulders
     ['AQP2','3','aquifer:composition2','clay'], // Clay
     ['AQP2','4','aquifer:composition2','conglomerate'], // Conglomerate
     ['AQP2','5','aquifer:composition2','dolomite'], // Dolomite
     ['AQP2','6','aquifer:composition2','granite'], // Granite
     ['AQP2','7','aquifer:composition2','gravel'], // Gravel
     ['AQP2','8','aquifer:composition2','igneous_rock'], // Igneous Rock
     ['AQP2','9','aquifer:composition2','karst'], // Karst
     ['AQP2','10','aquifer:composition2','limestone'], // Limestone
     ['AQP2','11','aquifer:composition2','marl'], // Marl
     ['AQP2','12','aquifer:composition2','metamorphic_rock'], // Metamorphic Rock
     ['AQP2','13','aquifer:composition2','mud'], // Mud
     ['AQP2','14','aquifer:composition2','sand'], // Sand
     ['AQP2','15','aquifer:composition2','schist'], // Schist
     ['AQP2','16','aquifer:composition2','unconsolidated'], // Unconsolidated
     ['AQP2','17','aquifer:composition2','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['AQP2','18','aquifer:composition2','volcanic_rock'], // Volcanic Rock
     ['AQP2','19','aquifer:composition2','sandstone'], // Sandstone
     ['AQP2','20','aquifer:composition2','shale'], // Shale
     ['AQP2','999','aquifer:composition2','other'], // Other

     // AQP3 - Aquifer Composition [3]
     // ['AQP3','-999999',undefined,undefined], // No Information
     ['AQP3','1','aquifer:composition3','basalt'], // Basalt
     ['AQP3','2','aquifer:composition3','boulders'], // Boulders
     ['AQP3','3','aquifer:composition3','clay'], // Clay
     ['AQP3','4','aquifer:composition3','conglomerate'], // Conglomerate
     ['AQP3','5','aquifer:composition3','dolomite'], // Dolomite
     ['AQP3','6','aquifer:composition3','granite'], // Granite
     ['AQP3','7','aquifer:composition3','gravel'], // Gravel
     ['AQP3','8','aquifer:composition3','igneous_rock'], // Igneous Rock
     ['AQP3','9','aquifer:composition3','karst'], // Karst
     ['AQP3','10','aquifer:composition3','limestone'], // Limestone
     ['AQP3','11','aquifer:composition3','marl'], // Marl
     ['AQP3','12','aquifer:composition3','metamorphic_rock'], // Metamorphic Rock
     ['AQP3','13','aquifer:composition3','mud'], // Mud
     ['AQP3','14','aquifer:composition3','sand'], // Sand
     ['AQP3','15','aquifer:composition3','schist'], // Schist
     ['AQP3','16','aquifer:composition3','unconsolidated'], // Unconsolidated
     ['AQP3','17','aquifer:composition3','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['AQP3','18','aquifer:composition3','volcanic_rock'], // Volcanic Rock
     ['AQP3','19','aquifer:composition3','sandstone'], // Sandstone
     ['AQP3','20','aquifer:composition3','shale'], // Shale
     ['AQP3','999','aquifer:composition3','other'], // Other

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
//      ['ASU','20','raw:ASU','wood'], // Wood
//      ['ASU','21','raw:ASU','asphalt_over_concrete'], // Asphalt Over Concrete
//      ['ASU','999','raw:ASU','other'], // Other

     // ASU2 - Aerodrome Movement Area Surface Composition [2]  - See ZI019_ASU2
     // ['ASU2','-999999',undefined,undefined], // No Information
//      ['ASU2','1','raw:ASU2','asphalt'], // Asphalt
//      ['ASU2','2','raw:ASU2','bituminous_mix'], // Bituminous Mix
//      ['ASU2','3','raw:ASU2','brick'], // Brick
//      ['ASU2','4','raw:ASU2','clay'], // Clay
//      ['ASU2','5','raw:ASU2','concrete'], // Concrete
//      ['ASU2','6','raw:ASU2','coral'], // Coral
//      ['ASU2','7','raw:ASU2','earthen'], // Earthen
//      ['ASU2','8','raw:ASU2','gravel'], // Gravel
//      ['ASU2','9','raw:ASU2','ice'], // Ice
//      ['ASU2','10','raw:ASU2','landing_mat'], // Landing Mat
//      ['ASU2','11','raw:ASU2','laterite'], // Laterite
//      ['ASU2','12','raw:ASU2','macadam'], // Macadam
//      ['ASU2','13','raw:ASU2','membrane'], // Membrane
//      ['ASU2','14','raw:ASU2','non-bituminous_mix'], // Non-bituminous Mix
//      ['ASU2','15','raw:ASU2','pierced_steel_planking'], // Pierced Steel Planking
//      ['ASU2','16','raw:ASU2','sand'], // Sand
//      ['ASU2','17','raw:ASU2','snow'], // Snow
//      ['ASU2','18','raw:ASU2','stone'], // Stone
//      ['ASU2','20','raw:ASU2','wood'], // Wood
//      ['ASU2','21','raw:ASU2','asphalt_over_concrete'], // Asphalt Over Concrete
//      ['ASU2','999','raw:ASU2','other'], // Other

     // ASU3 - Aerodrome Movement Area Surface Composition [3] - See ZI019_ASU3
     // ['ASU3','-999999',undefined,undefined], // No Information
//      ['ASU3','1','raw:ASU3','asphalt'], // Asphalt
//      ['ASU3','2','raw:ASU3','bituminous_mix'], // Bituminous Mix
//      ['ASU3','3','raw:ASU3','brick'], // Brick
//      ['ASU3','4','raw:ASU3','clay'], // Clay
//      ['ASU3','5','raw:ASU3','concrete'], // Concrete
//      ['ASU3','6','raw:ASU3','coral'], // Coral
//      ['ASU3','7','raw:ASU3','earthen'], // Earthen
//      ['ASU3','8','raw:ASU3','gravel'], // Gravel
//      ['ASU3','9','raw:ASU3','ice'], // Ice
//      ['ASU3','10','raw:ASU3','landing_mat'], // Landing Mat
//      ['ASU3','11','raw:ASU3','laterite'], // Laterite
//      ['ASU3','12','raw:ASU3','macadam'], // Macadam
//      ['ASU3','13','raw:ASU3','membrane'], // Membrane
//      ['ASU3','14','raw:ASU3','non-bituminous_mix'], // Non-bituminous Mix
//      ['ASU3','15','raw:ASU3','pierced_steel_planking'], // Pierced Steel Planking
//      ['ASU3','16','raw:ASU3','sand'], // Sand
//      ['ASU3','17','raw:ASU3','snow'], // Snow
//      ['ASU3','18','raw:ASU3','stone'], // Stone
//      ['ASU3','20','raw:ASU3','wood'], // Wood
//      ['ASU3','21','raw:ASU3','asphalt_over_concrete'], // Asphalt Over Concrete
//      ['ASU3','999','raw:ASU3','other'], // Other

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

     // AT005_CAB2 - Cable : Cable Type [2] - See CAB2
     // ['AT005_CAB2','-999999',undefined,undefined], // No Information
//      ['AT005_CAB2','2','raw:AT005_CAB2','power_distribution_line'], // Power Distribution Line
//      ['AT005_CAB2','3','raw:AT005_CAB2','telephone'], // Telephone
//      ['AT005_CAB2','4','raw:AT005_CAB2','telegraph'], // Telegraph
//      ['AT005_CAB2','6','raw:AT005_CAB2','power_transmission_line'], // Power Transmission Line
//      ['AT005_CAB2','7','raw:AT005_CAB2','digital_communication_line'], // Digital Communication Line
//      ['AT005_CAB2','8','raw:AT005_CAB2','communication_line'], // Communication Line
//      ['AT005_CAB2','9','raw:AT005_CAB2','fibre-optic'], // Fibre-optic
//      ['AT005_CAB2','10','raw:AT005_CAB2','tethering'], // Tethering
//      ['AT005_CAB2','11','raw:AT005_CAB2','load_bearing'], // Load Bearing
//      ['AT005_CAB2','12','raw:AT005_CAB2','guide'], // Guide
//      ['AT005_CAB2','13','raw:AT005_CAB2','barrier'], // Barrier
//      ['AT005_CAB2','14','raw:AT005_CAB2','restraining'], // Restraining
//      ['AT005_CAB2','15','raw:AT005_CAB2','towing'], // Towing
//      ['AT005_CAB2','19','raw:AT005_CAB2','cableway'], // Cableway
//      ['AT005_CAB2','999','raw:AT005_CAB2','other'], // Other

     // AT005_CAB3 - Cable : Cable Type [3] - See CAB3
     // ['AT005_CAB3','-999999',undefined,undefined], // No Information
//      ['AT005_CAB3','2','raw:AT005_CAB3','power_distribution_line'], // Power Distribution Line
//      ['AT005_CAB3','3','raw:AT005_CAB3','telephone'], // Telephone
//      ['AT005_CAB3','4','raw:AT005_CAB3','telegraph'], // Telegraph
//      ['AT005_CAB3','6','raw:AT005_CAB3','power_transmission_line'], // Power Transmission Line
//      ['AT005_CAB3','7','raw:AT005_CAB3','digital_communication_line'], // Digital Communication Line
//      ['AT005_CAB3','8','raw:AT005_CAB3','communication_line'], // Communication Line
//      ['AT005_CAB3','9','raw:AT005_CAB3','fibre-optic'], // Fibre-optic
//      ['AT005_CAB3','10','raw:AT005_CAB3','tethering'], // Tethering
//      ['AT005_CAB3','11','raw:AT005_CAB3','load_bearing'], // Load Bearing
//      ['AT005_CAB3','12','raw:AT005_CAB3','guide'], // Guide
//      ['AT005_CAB3','13','raw:AT005_CAB3','barrier'], // Barrier
//      ['AT005_CAB3','14','raw:AT005_CAB3','restraining'], // Restraining
//      ['AT005_CAB3','15','raw:AT005_CAB3','towing'], // Towing
//      ['AT005_CAB3','19','raw:AT005_CAB3','cableway'], // Cableway
//      ['AT005_CAB3','999','raw:AT005_CAB3','other'], // Other

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
     // ['BBMCB2','-999999',undefined,undefined], // No Information
     ['BBMCB2','1','waterway:bank2:bottom:2','clay_and_silt'], // Clay and Silt
     ['BBMCB2','2','waterway:bank2:bottom:2','silty_sands'], // Silty Sands
     ['BBMCB2','3','waterway:bank2:bottom:2','sand_and_gravel'], // Sand and Gravel
     ['BBMCB2','4','waterway:bank2:bottom:2','gravel_and_cobble'], // Gravel and Cobble
     ['BBMCB2','5','waterway:bank2:bottom:2','rocks_and_boulders'], // Rocks and Boulders
     ['BBMCB2','6','waterway:bank2:bottom:2','bedrock'], // Bedrock
     ['BBMCB2','7','waterway:bank2:bottom:2','paved'], // Paved
     ['BBMCB2','8','waterway:bank2:bottom:2','peat'], // Peat
     ['BBMCB2','9','waterway:bank2:bottom:2','sand_over_mud'], // Sand over Mud
     ['BBMCB2','11','waterway:bank2:bottom:2','coral'], // Coral
     ['BBMCB2','14','waterway:bank2:bottom:2','sand'], // Sand
     ['BBMCB2','15','waterway:bank2:bottom:2','concrete'], // Concrete
     ['BBMCB2','16','waterway:bank2:bottom:2','masonry'], // Masonry
     ['BBMCB2','17','waterway:bank2:bottom:2','reinforced_concrete'], // Reinforced Concrete
     ['BBMCB2','18','waterway:bank2:bottom:2','soil'], // Soil
     ['BBMCB2','999','waterway:bank2:bottom:2','other'], // Other

     // BBMCB3 - Wtrbdy Bank (2) : Bottom Material Type (second bank) [3]
     // ['BBMCB3','-999999',undefined,undefined], // No Information
     ['BBMCB3','1','waterway:bank2:bottom:3','clay_and_silt'], // Clay and Silt
     ['BBMCB3','2','waterway:bank2:bottom:3','silty_sands'], // Silty Sands
     ['BBMCB3','3','waterway:bank2:bottom:3','sand_and_gravel'], // Sand and Gravel
     ['BBMCB3','4','waterway:bank2:bottom:3','gravel_and_cobble'], // Gravel and Cobble
     ['BBMCB3','5','waterway:bank2:bottom:3','rocks_and_boulders'], // Rocks and Boulders
     ['BBMCB3','6','waterway:bank2:bottom:3','bedrock'], // Bedrock
     ['BBMCB3','7','waterway:bank2:bottom:3','paved'], // Paved
     ['BBMCB3','8','waterway:bank2:bottom:3','peat'], // Peat
     ['BBMCB3','9','waterway:bank2:bottom:3','sand_over_mud'], // Sand over Mud
     ['BBMCB3','11','waterway:bank2:bottom:3','coral'], // Coral
     ['BBMCB3','14','waterway:bank2:bottom:3','sand'], // Sand
     ['BBMCB3','15','waterway:bank2:bottom:3','concrete'], // Concrete
     ['BBMCB3','16','waterway:bank2:bottom:3','masonry'], // Masonry
     ['BBMCB3','17','waterway:bank2:bottom:3','reinforced_concrete'], // Reinforced Concrete
     ['BBMCB3','18','waterway:bank2:bottom:3','soil'], // Soil
     ['BBMCB3','999','waterway:bank2:bottom:3','other'], // Other

     // BC010_REF - Mar Nav Bcn : Radar Reflector Present - See REF
     // ['BC010_REF','-999999',undefined,undefined], // No Information
//      ['BC010_REF','1000','raw:BC010_REF','no'],
//      ['BC010_REF','1001','raw:BC010_REF','yes'],

     // BC010_TZP - Mar Nav Bcn : Topmark Shape - See TZP
     // ['BC010_TZP','-999999',undefined,undefined], // No Information
//      ['BC010_TZP','1','raw:BC010_TZP','cone_point_upward'], // Cone Point Upward
//      ['BC010_TZP','2','raw:BC010_TZP','cone_point_downward'], // Cone Point Downward
//      ['BC010_TZP','3','raw:BC010_TZP','sphere'], // Sphere
//      ['BC010_TZP','4','raw:BC010_TZP','2_spheres_one_over_other'], // 2 Spheres One over Other
//      ['BC010_TZP','5','raw:BC010_TZP','cylinder'], // Cylinder
//      ['BC010_TZP','6','raw:BC010_TZP','board'], // Board
//      ['BC010_TZP','7','raw:BC010_TZP','x-shaped'], // X-shaped
//      ['BC010_TZP','8','raw:BC010_TZP','upright_cross'], // Upright Cross
//      ['BC010_TZP','9','raw:BC010_TZP','cube_point_upward'], // Cube Point Upward
//      ['BC010_TZP','10','raw:BC010_TZP','2_cones_point_to_point'], // 2 Cones Point to Point
//      ['BC010_TZP','11','raw:BC010_TZP','2_cones_base_to_base'], // 2 Cones Base to Base
//      ['BC010_TZP','12','raw:BC010_TZP','diamond'], // Diamond
//      ['BC010_TZP','13','raw:BC010_TZP','2_cones_points_upward'], // 2 Cones Points Upward
//      ['BC010_TZP','14','raw:BC010_TZP','2_cones_points_downward'], // 2 Cones Points Downward
//      ['BC010_TZP','15','raw:BC010_TZP','besom_point_upward'], // Besom Point Upward
//      ['BC010_TZP','16','raw:BC010_TZP','besom_point_downward'], // Besom Point Downward
//      ['BC010_TZP','17','raw:BC010_TZP','flag'], // Flag
//      ['BC010_TZP','18','raw:BC010_TZP','sphere_over_diamond'], // Sphere over Diamond
//      ['BC010_TZP','19','raw:BC010_TZP','square'], // Square
//      ['BC010_TZP','20','raw:BC010_TZP','horizontal_rectangle'], // Horizontal Rectangle
//      ['BC010_TZP','21','raw:BC010_TZP','vertical_rectangle'], // Vertical Rectangle
//      ['BC010_TZP','22','raw:BC010_TZP','trapezium_short_upward'], // Trapezium Short Upward
//      ['BC010_TZP','23','raw:BC010_TZP','trapezium_short_downward'], // Trapezium Short Downward
//      ['BC010_TZP','24','raw:BC010_TZP','triangle_point_upward'], // Triangle Point Upward
//      ['BC010_TZP','25','raw:BC010_TZP','triangle_point_downward'], // Triangle Point Downward
//      ['BC010_TZP','26','raw:BC010_TZP','circle'], // Circle
//      ['BC010_TZP','27','raw:BC010_TZP','2_upright_crosses'], // 2 Upright Crosses
//      ['BC010_TZP','28','raw:BC010_TZP','t-shaped'], // T-shaped
//      ['BC010_TZP','29','raw:BC010_TZP','triangle_point_upward_over_circle'], // Triangle Point Upward over Circle
//      ['BC010_TZP','30','raw:BC010_TZP','upright_cross_over_circle'], // Upright Cross over Circle
//      ['BC010_TZP','31','raw:BC010_TZP','diamond_over_circle'], // Diamond over Circle
//      ['BC010_TZP','32','raw:BC010_TZP','circle_over_triangle_point_upward'], // Circle over Triangle Point Upward
//      ['BC010_TZP','33','raw:BC010_TZP','ball_over_cone'], // Ball over Cone
//      ['BC010_TZP','34','raw:BC010_TZP','can_over_ball'], // Can over Ball
//      ['BC010_TZP','35','raw:BC010_TZP','cone_over_ball'], // Cone over Ball
//      ['BC010_TZP','36','raw:BC010_TZP','cross_over_ball'], // Cross over Ball
//      ['BC010_TZP','37','raw:BC010_TZP','diamond_over_ball'], // Diamond over Ball
//      ['BC010_TZP','998','raw:BC010_TZP','not_applicable'], // Not Applicable
//      ['BC010_TZP','999','raw:BC010_TZP','other'], // Other

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
     // ['BC034_MRT2','-999999',undefined,undefined], // No Information
     ['BC034_MRT2','1','seamark:radio_station:category:2','ais'], // Automated Identification System (AIS)
     ['BC034_MRT2','2','seamark:radio_station:category:2','omnidirectional'], // Circular Radiobeacon
     ['BC034_MRT2','3','seamark:radio_station:category:2','directional'], // Directional Radiobeacon
     ['BC034_MRT2','4','seamark:radio_station:category:2','loran'], // Long Range Air Navigation System (LORAN) C
     ['BC034_MRT2','5','seamark:radio_station:category:2','qtg'], // QTG Station
     ['BC034_MRT2','6','seamark:radio_station:category:2','ramark'], // Radar Marker (RAMARK)
     ['BC034_MRT2','7','seamark:radio_station:category:2','racon'], // Radar Responder Beacon (RACON)
     ['BC034_MRT2','8','seamark:radio_station:category:2','rdf'], // Radio Direction Finding Station
     ['BC034_MRT2','9','seamark:radio_station:category:2','rotating_pattern'], // Rotating Pattern Radiobeacon
     ['BC034_MRT2','999','seamark:radio_station:category:2','other'], // Other

     // BC034_MRT3 - Mar Radiobeacon : Maritime Radiobeacon Type [3]
     // ['BC034_MRT3','-999999',undefined,undefined], // No Information
     ['BC034_MRT3','1','seamark:radio_station:category:3','ais'], // Automated Identification System (AIS)
     ['BC034_MRT3','2','seamark:radio_station:category:3','omnidirectional'], // Circular Radiobeacon
     ['BC034_MRT3','3','seamark:radio_station:category:3','directional'], // Directional Radiobeacon
     ['BC034_MRT3','4','seamark:radio_station:category:3','loran'], // Long Range Air Navigation System (LORAN) C
     ['BC034_MRT3','5','seamark:radio_station:category:3','qtg_station'], // QTG Station
     ['BC034_MRT3','6','seamark:radio_station:category:3','ramark'], // Radar Marker (RAMARK)
     ['BC034_MRT3','7','seamark:radio_station:category:3','racon'], // Radar Responder Beacon (RACON)
     ['BC034_MRT3','8','seamark:radio_station:category:3','rdf'], // Radio Direction Finding Station
     ['BC034_MRT3','9','seamark:radio_station:category:3','rotating_pattern'], // Rotating Pattern Radiobeacon
     ['BC034_MRT3','999','seamark:radio_station:category:3','other'], // Other

     // BC040_CAA - Mar Nav Lt : Controlling Authority - See CAA
     // ['BC040_CAA','-999999',undefined,undefined], // No Information
//      ['BC040_CAA','3','raw:BC040_CAA','private'], // Private
//      ['BC040_CAA','5','raw:BC040_CAA','military'], // Military
//      ['BC040_CAA','7','raw:BC040_CAA','joint_military_and_civilian'], // Joint Military and Civilian
//      ['BC040_CAA','16','raw:BC040_CAA','civilian'], // Civilian
//      ['BC040_CAA','999','raw:BC040_CAA','other'], // Other

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
     // ['BH010_CAA','-999999',undefined,undefined], // No Information
//      ['BH010_CAA','3','raw:BH010_CAA','private'], // Private
//      ['BH010_CAA','5','raw:BH010_CAA','military'], // Military
//      ['BH010_CAA','7','raw:BH010_CAA','joint_military_and_civilian'], // Joint Military and Civilian
//      ['BH010_CAA','16','raw:BH010_CAA','civilian'], // Civilian
//      ['BH010_CAA','17','raw:BH010_CAA','public'], // Public
//      ['BH010_CAA','999','raw:BH010_CAA','other'], // Other

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
     // ['BMC2','-999999',undefined,undefined], // No Information
     ['BMC2','1','waterway:bottom:2','clay_and_silt'], // Clay and Silt
     ['BMC2','2','waterway:bottom:2','silty_sands'], // Silty Sands
     ['BMC2','3','waterway:bottom:2','sand_and_gravel'], // Sand and Gravel
     ['BMC2','4','waterway:bottom:2','gravel_and_cobble'], // Gravel and Cobble
     ['BMC2','5','waterway:bottom:2','rocks_and_boulders'], // Rocks and Boulders
     ['BMC2','6','waterway:bottom:2','bedrock'], // Bedrock
     ['BMC2','7','waterway:bottom:2','paved'], // Paved
     ['BMC2','8','waterway:bottom:2','peat'], // Peat
     ['BMC2','9','waterway:bottom:2','sand_over_mud'], // Sand over Mud
     ['BMC2','11','waterway:bottom:2','coral'], // Coral
     ['BMC2','14','waterway:bottom:2','sand'], // Sand
     ['BMC2','15','waterway:bottom:2','concrete'], // Concrete
     ['BMC2','16','waterway:bottom:2','masonry'], // Masonry
     ['BMC2','17','waterway:bottom:2','reinforced_concrete'], // Reinforced Concrete
     ['BMC2','18','waterway:bottom:2','soil'], // Soil
     ['BMC2','999','waterway:bottom:2','other'], // Other

     // BMC3 - Bottom Material Type [3]
     // ['BMC3','-999999',undefined,undefined], // No Information
     ['BMC3','1','waterway:bottom:3','clay_and_silt'], // Clay and Silt
     ['BMC3','2','waterway:bottom:3','silty_sands'], // Silty Sands
     ['BMC3','3','waterway:bottom:3','sand_and_gravel'], // Sand and Gravel
     ['BMC3','4','waterway:bottom:3','gravel_and_cobble'], // Gravel and Cobble
     ['BMC3','5','waterway:bottom:3','rocks_and_boulders'], // Rocks and Boulders
     ['BMC3','6','waterway:bottom:3','bedrock'], // Bedrock
     ['BMC3','7','waterway:bottom:3','paved'], // Paved
     ['BMC3','8','waterway:bottom:3','peat'], // Peat
     ['BMC3','9','waterway:bottom:3','sand_over_mud'], // Sand over Mud
     ['BMC3','11','waterway:bottom:3','coral'], // Coral
     ['BMC3','14','waterway:bottom:3','sand'], // Sand
     ['BMC3','15','waterway:bottom:3','concrete'], // Concrete
     ['BMC3','16','waterway:bottom:3','masonry'], // Masonry
     ['BMC3','17','waterway:bottom:3','reinforced_concrete'], // Reinforced Concrete
     ['BMC3','18','waterway:bottom:3','soil'], // Soil
     ['BMC3','999','waterway:bottom:3','other'], // Other

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
     ['BSC','19','bridge:structure','bowstring-bridge'], // Bowstring-bridge
     ['BSC','22','bridge:structure','covered'], // Covered
     ['BSC','26','bridge:structure','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC','27','bridge:structure','cable_stayed'], // Cable Stayed
     ['BSC','28','bridge:structure','viaduct'], // Viaduct
     ['BSC','31','bridge:structure','arch'], // Arch
     ['BSC','32','bridge:structure','trestle'], // Trestle
     ['BSC','999','bridge:structure','other'], // Other

     // BSC2 - Bridge Structure Type [2]
     // ['BSC2','-999999',undefined,undefined], // No Information
     ['BSC2','1','bridge:structure2','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC2','2','bridge:structure2','cantilever'], // Cantilever
     ['BSC2','5','bridge:structure2','floating'], // Floating
     ['BSC2','6','bridge:structure2','girder'], // Girder
     ['BSC2','7','bridge:structure2','tower_suspension'], // Tower Suspension
     ['BSC2','8','bridge:structure2','truss'], // Truss
     ['BSC2','9','bridge:structure2','suspension'], // Suspension
     ['BSC2','12','bridge:structure2','transporter'], // Transporter
     ['BSC2','15','bridge:structure2','slab'], // Slab
     ['BSC2','16','bridge:structure2','stringer_beam'], // Stringer Beam
     ['BSC2','19','bridge:structure2','bowstring-bridge'], // Bowstring-bridge
     ['BSC2','22','bridge:structure2','covered'], // Covered
     ['BSC2','26','bridge:structure2','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC2','27','bridge:structure2','cable_stayed'], // Cable Stayed
     ['BSC2','31','bridge:structure2','arch'], // Arch
     ['BSC2','32','bridge:structure2','trestle'], // Trestle
     ['BSC2','999','bridge:structure2','other'], // Other

     // BSC3 - Bridge Structure Type [3]
     // ['BSC3','-999999',undefined,undefined], // No Information
     ['BSC3','1','bridge:structure:3','open_spandrel_arch'], // Open Spandrel Arch
     ['BSC3','2','bridge:structure:3','cantilever'], // Cantilever
     ['BSC3','5','bridge:structure:3','floating'], // Floating
     ['BSC3','6','bridge:structure:3','girder'], // Girder
     ['BSC3','7','bridge:structure:3','tower_suspension'], // Tower Suspension
     ['BSC3','8','bridge:structure:3','truss'], // Truss
     ['BSC3','9','bridge:structure:3','suspension'], // Suspension
     ['BSC3','12','bridge:structure:3','transporter'], // Transporter
     ['BSC3','15','bridge:structure:3','slab'], // Slab
     ['BSC3','16','bridge:structure:3','stringer_beam'], // Stringer Beam
     ['BSC3','19','bridge:structure:3','bowstring-bridge'], // Bowstring-bridge
     ['BSC3','22','bridge:structure:3','covered'], // Covered
     ['BSC3','26','bridge:structure:3','closed_spandrel_arch'], // Closed Spandrel Arch
     ['BSC3','27','bridge:structure:3','cable_stayed'], // Cable Stayed
     ['BSC3','31','bridge:structure:3','arch'], // Arch
     ['BSC3','32','bridge:structure:3','trestle'], // Trestle
     ['BSC3','999','bridge:structure:3','other'], // Other

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
     ['BUT','3','seamark:buoy','isolated_danger'], // Isolated Danger
     ['BUT','4','seamark:buoy','large_automatic_navigation_buoy'], // Large Automatic Navigation Buoy
     ['BUT','6','seamark:buoy','light_float'], // Light Float
     ['BUT','7','seamark:buoy','mooring'], // Mooring
     ['BUT','10','seamark:buoy','ocean_data_acquisition_system_(odas)_buoy'], // Ocean Data Acquisition System (ODAS) Buoy
     ['BUT','11','seamark:buoy','landfall'], // Landfall
     ['BUT','15','seamark:buoy','special'], // Special
     ['BUT','18','seamark:buoy','safe_water'], // Safe Water
     ['BUT','19','seamark:buoy','anchorage'], // Anchorage
     ['BUT','20','seamark:buoy','fairway'], // Fairway
     ['BUT','21','seamark:buoy','mid-channel'], // Mid-Channel
     ['BUT','22','seamark:buoy','bifurcation'], // Bifurcation
     ['BUT','23','seamark:buoy','junction'], // Junction
     ['BUT','24','seamark:buoy','wreck'], // Wreck
     ['BUT','25','seamark:buoy','obstruction'], // Obstruction
     ['BUT','26','seamark:buoy','telegraph_cable'], // Telegraph Cable
     ['BUT','27','seamark:buoy','warping'], // Warping
     ['BUT','28','seamark:buoy','quarantine'], // Quarantine
     ['BUT','29','seamark:buoy','exercise_area'], // Exercise Area
     ['BUT','30','seamark:buoy','explosive_anchorage'], // Explosive Anchorage
     ['BUT','31','seamark:buoy','aeronautical_anchorage'], // Aeronautical Anchorage
     ['BUT','32','seamark:buoy','compass_adjustment'], // Compass Adjustment
     ['BUT','33','seamark:buoy','fish_trap'], // Fish Trap
     ['BUT','34','seamark:buoy','spoil_ground'], // Spoil Ground
     ['BUT','35','seamark:buoy','articulated_light'], // Articulated Light
     ['BUT','39','seamark:buoy','trot'], // Trot
     ['BUT','81','seamark:buoy','diving'], // Diving
     ['BUT','82','seamark:buoy','notice'], // Notice
     ['BUT','84','seamark:buoy','apex_float'], // APEX Float
     ['BUT','85','seamark:buoy','general_warning'], // General Warning
     ['BUT','86','seamark:buoy','private'], // Private
     ['BUT','87','seamark:buoy','swim'], // Swim
     ['BUT','88','seamark:buoy','control'], // Control
     ['BUT','89','seamark:buoy','entry_prohibited'], // Entry Prohibited
     ['BUT','91','seamark:buoy','preferred_channel_to_port'], // Preferred Channel to Port
     ['BUT','92','seamark:buoy','preferred_channel_to_starboard'], // Preferred Channel to Starboard
     ['BUT','93','seamark:buoy','starboard-hand_edge'], // Starboard-hand Edge
     ['BUT','94','seamark:buoy','port-hand_edge'], // Port-hand Edge
     ['BUT','95','seamark:buoy','west_cardinal'], // West Cardinal
     ['BUT','96','seamark:buoy','south_cardinal'], // South Cardinal
     ['BUT','97','seamark:buoy','east_cardinal'], // East Cardinal
     ['BUT','98','seamark:buoy','north_cardinal'], // North Cardinal
     ['BUT','99','seamark:buoy','installation'], // Installation
     ['BUT','100','seamark:buoy','accelerometer'], // Accelerometer
     ['BUT','101','seamark:buoy','wavemeter'], // Wavemeter
     ['BUT','102','seamark:buoy','navigation,_communication_and_control_buoy_(nccb)'], // Navigation, communication and control buoy (NCCB)
     ['BUT','103','seamark:buoy','ice_buoy'], // Ice Buoy
     ['BUT','104','seamark:buoy','firing_danger_area'], // Firing Danger Area
     ['BUT','105','seamark:buoy','target'], // Target
     ['BUT','106','seamark:buoy','marker_ship'], // Marker Ship
     ['BUT','107','seamark:buoy','degaussing_range'], // Degaussing Range
     ['BUT','108','seamark:buoy','barge'], // Barge
     ['BUT','109','seamark:buoy','cable'], // Cable
     ['BUT','110','seamark:buoy','outfall'], // Outfall
     ['BUT','111','seamark:buoy','recording'], // Recording
     ['BUT','112','seamark:buoy','recreation_zone'], // Recreation Zone
     ['BUT','113','seamark:buoy','leading_line'], // Leading Line
     ['BUT','114','seamark:buoy','measured_distance'], // Measured Distance
     ['BUT','115','seamark:buoy','tss'], // TSS
     ['BUT','116','seamark:buoy','anchoring_prohibited'], // Anchoring Prohibited
     ['BUT','117','seamark:buoy','berthing_prohibited'], // Berthing Prohibited
     ['BUT','118','seamark:buoy','overtaking_prohibited'], // Overtaking Prohibited
     ['BUT','119','seamark:buoy','two-way_traffic_prohibited'], // Two-Way Traffic Prohibited
     ['BUT','120','seamark:buoy','reduced_wake'], // Reduced Wake
     ['BUT','121','seamark:buoy','speed_limit'], // Speed Limit
     ['BUT','122','seamark:buoy','stop'], // Stop
     ['BUT','123','seamark:buoy','sound_ship"s_siren'], // Sound Ship"s Siren
     ['BUT','124','seamark:buoy','restricted_vertical_clearance'], // Restricted Vertical Clearance
     ['BUT','125','seamark:buoy','maximum_vessel"s_draft'], // Maximum Vessel"s Draft
     ['BUT','126','seamark:buoy','restricted_horizontal_clearance'], // Restricted Horizontal Clearance
     ['BUT','127','seamark:buoy','strong_current_warning'], // Strong Current Warning
     ['BUT','128','seamark:buoy','berthing_permitted'], // Berthing Permitted
     ['BUT','129','seamark:buoy','overhead_power_cable'], // Overhead Power Cable
     ['BUT','130','seamark:buoy','channel_edge_gradient'], // Channel Edge Gradient
     ['BUT','131','seamark:buoy','telephone'], // Telephone
     ['BUT','132','seamark:buoy','ferry_crossing'], // Ferry Crossing
     ['BUT','133','seamark:buoy','pipeline'], // Pipeline
     ['BUT','134','seamark:buoy','clearing_line'], // Clearing Line
     ['BUT','135','seamark:buoy','refuge'], // Refuge
     ['BUT','136','seamark:buoy','foul_ground'], // Foul Ground
     ['BUT','137','seamark:buoy','yachting'], // Yachting
     ['BUT','138','seamark:buoy','heliport'], // Heliport
     ['BUT','139','seamark:buoy','gps_mark'], // GPS Mark
     ['BUT','140','seamark:buoy','work_in_progress'], // Work In Progress
     ['BUT','141','seamark:buoy','artificial_reef'], // Artificial Reef
     ['BUT','142','seamark:buoy','marine_farm'], // Marine Farm
     ['BUT','143','seamark:buoy','seaplane_landing'], // Seaplane Landing
     ['BUT','144','seamark:buoy','wellhead'], // Wellhead
     ['BUT','145','seamark:buoy','water_column_sensor'], // Water Column Sensor
     ['BUT','149','seamark:buoy','emergency_wreck'], // Emergency Wreck
     ['BUT','150','seamark:buoy','mark_of_unknown_intent'], // Mark of Unknown Intent
     ['BUT','999','seamark:buoy','other'], // Other

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
     // ['CAB2','-999999',undefined,undefined], // No Information
     ['CAB2','2','cable:type2','power'], // Power Distribution Line
     ['CAB2','3','cable:type2','telephone'], // Telephone
     ['CAB2','4','cable:type2','telegraph'], // Telegraph
     ['CAB2','6','cable:type2','transmission'], // Power Transmission Line
     ['CAB2','7','cable:type2','digital_communication'], // Digital Communication Line
     ['CAB2','8','cable:type2','communication_line'], // Communication Line
     ['CAB2','9','cable:type2','fibre-optic'], // Fibre-optic
     ['CAB2','10','cable:type2','tethering'], // Tethering
     ['CAB2','11','cable:type2','load_bearing'], // Load Bearing
     ['CAB2','12','cable:type2','guide'], // Guide
     ['CAB2','13','cable:type2','barrier'], // Barrier
     ['CAB2','14','cable:type2','restraining'], // Restraining
     ['CAB2','15','cable:type2','towing'], // Towing
     ['CAB2','19','cable:type2','cableway'], // Cableway
     ['CAB2','999','cable:type2','other'], // Other

     // CAB3 - Cable Type [3]
     // ['CAB3','-999999',undefined,undefined], // No Information
     ['CAB3','2','cable:type:3','power'], // Power Distribution Line
     ['CAB3','3','cable:type:3','telephone'], // Telephone
     ['CAB3','4','cable:type:3','telegraph'], // Telegraph
     ['CAB3','6','cable:type:3','transmission'], // Power Transmission Line
     ['CAB3','7','cable:type:3','digital_communication'], // Digital Communication Line
     ['CAB3','8','cable:type:3','communication_line'], // Communication Line
     ['CAB3','9','cable:type:3','fibre-optic'], // Fibre-optic
     ['CAB3','10','cable:type:3','tethering'], // Tethering
     ['CAB3','11','cable:type:3','load_bearing'], // Load Bearing
     ['CAB3','12','cable:type:3','guide'], // Guide
     ['CAB3','13','cable:type:3','barrier'], // Barrier
     ['CAB3','14','cable:type:3','restraining'], // Restraining
     ['CAB3','15','cable:type:3','towing'], // Towing
     ['CAB3','19','cable:type:3','cableway'], // Cableway
     ['CAB3','999','cable:type:3','other'], // Other

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
     ['CTC','3','culvert:type','box_with_load'], // Box with Load
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
     // ['CVT2','-999999',undefined,undefined], // No Information
     ['CVT','1','conveyor:type2','not_visible'], // Not Visible
     ['CVT','2','conveyor:type2','belt'], // Belt
     ['CVT','3','conveyor:type2','rollers'], // Rollers
     ['CVT','4','conveyor:type2','bucket'], // Bucket
     ['CVT','5','conveyor:type2','screw_auger'], // Screw Auger
     ['CVT','6','conveyor:type2','apron'], // Apron
     ['CVT','7','conveyor:type2','flight'], // Flight
     ['CVT','999','conveyor:type2','other'], // Other

     // CVT3 - Conveyor Type [3]
     // ['CVT3','-999999',undefined,undefined], // No Information
     ['CVT','1','conveyor:type:3','not_visible'], // Not Visible
     ['CVT','2','conveyor:type:3','belt'], // Belt
     ['CVT','3','conveyor:type:3','rollers'], // Rollers
     ['CVT','4','conveyor:type:3','bucket'], // Bucket
     ['CVT','5','conveyor:type:3','screw_auger'], // Screw Auger
     ['CVT','6','conveyor:type:3','apron'], // Apron
     ['CVT','7','conveyor:type:3','flight'], // Flight
     ['CVT','999','conveyor:type:3','other'], // Other

     // CWT - Contained within Tunnel
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
     // ['DFU2','-999999',undefined,undefined], // No Information
     ['DFU2','1','dam:type2','impoundment'], // Impoundment
     ['DFU2','2','dam:type2','navigation'], // Navigation
     ['DFU2','3','dam:type2','flood_control'], // Flood Control
     ['DFU2','4','dam:type2','hydroelectric_power_generation'], // Hydroelectric Power Generation
     ['DFU2','5','dam:type2','weir'], // Weir
     ['DFU2','6','dam:type2','irrigation'], // Irrigation
     ['DFU2','7','dam:type2','recreation'], // Recreation
     ['DFU2','8','dam:type2','water_supply'], // Water Supply
     ['DFU2','999','dam:type2','other'], // Other

     // DFU3 - Dam Function [3]
     // ['DFU3','-999999',undefined,undefined], // No Information
     ['DFU3','1','dam:type:3','impoundment'], // Impoundment
     ['DFU3','2','dam:type:3','navigation'], // Navigation
     ['DFU3','3','dam:type:3','flood_control'], // Flood Control
     ['DFU3','4','dam:type:3','hydroelectric_power_generation'], // Hydroelectric Power Generation
     ['DFU3','5','dam:type:3','weir'], // Weir
     ['DFU3','6','dam:type:3','irrigation'], // Irrigation
     ['DFU3','7','dam:type:3','recreation'], // Recreation
     ['DFU3','8','dam:type:3','water_supply'], // Water Supply
     ['DFU3','999','dam:type:3','other'], // Other

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
     // ['DKC','-999999',undefined,undefined], // No Information
//      ['DKC','1','raw:DKC','depth_known'], // Depth Known
//      ['DKC','3','raw:DKC','safe_clearance_known'], // Safe Clearance Known
//      ['DKC','4','raw:DKC','depth_unknown'], // Depth Unknown
//      ['DKC','5','raw:DKC','depth_doubtful'], // Depth Doubtful
//      ['DKC','6','raw:DKC','depth_unreliable'], // Depth Unreliable
//      ['DKC','8','raw:DKC','least_depth'], // Least Depth
//      ['DKC','9','raw:DKC','reported_depth'], // Reported Depth
//      ['DKC','998','raw:DKC','not_applicable'], // Not Applicable
//      ['DKC','999','raw:DKC','other'], // Other

     // DKC2 - Bathymetric Measurement Quality Category [2] - See ZHDP_DKC2
     // ['DKC2','-999999',undefined,undefined], // No Information
//      ['DKC2','1','raw:DKC2','depth_known'], // Depth Known
//      ['DKC2','3','raw:DKC2','safe_clearance_known'], // Safe Clearance Known
//      ['DKC2','4','raw:DKC2','depth_unknown'], // Depth Unknown
//      ['DKC2','5','raw:DKC2','depth_doubtful'], // Depth Doubtful
//      ['DKC2','6','raw:DKC2','depth_unreliable'], // Depth Unreliable
//      ['DKC2','8','raw:DKC2','least_depth'], // Least Depth
//      ['DKC2','9','raw:DKC2','reported_depth'], // Reported Depth
//      ['DKC2','998','raw:DKC2','not_applicable'], // Not Applicable
//      ['DKC2','999','raw:DKC2','other'], // Other

     // DKC3 - Bathymetric Measurement Quality Category [3] - See ZHDP_DKC3
     // ['DKC3','-999999',undefined,undefined], // No Information
//      ['DKC3','1','raw:DKC3','depth_known'], // Depth Known
//      ['DKC3','3','raw:DKC3','safe_clearance_known'], // Safe Clearance Known
//      ['DKC3','4','raw:DKC3','depth_unknown'], // Depth Unknown
//      ['DKC3','5','raw:DKC3','depth_doubtful'], // Depth Doubtful
//      ['DKC3','6','raw:DKC3','depth_unreliable'], // Depth Unreliable
//      ['DKC3','8','raw:DKC3','least_depth'], // Least Depth
//      ['DKC3','9','raw:DKC3','reported_depth'], // Reported Depth
//      ['DKC3','998','raw:DKC3','not_applicable'], // Not Applicable
//      ['DKC3','999','raw:DKC3','other'], // Other

     // DKS - Daymark Shape
     // ['DKS','-999999',undefined,undefined], // No Information
     ['DKS','1','daymark_shape','rectangle'], // Rectangle
     ['DKS','2','daymark_shape','square'], // Square
     ['DKS','3','daymark_shape','triangle'], // Triangle
     ['DKS','4','daymark_shape','cage'], // Cage
     ['DKS','5','daymark_shape','diamond'], // Diamond
     ['DKS','6','daymark_shape','circular'], // Circular
     ['DKS','7','daymark_shape','trapezoidal'], // Trapezoidal

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
     // ['DPG2','-999999',undefined,undefined], // No Information
     ['DPG2','1','seamark:dumping_ground:category:2','hazardous_material'], // Hazardous Material
     ['DPG2','2','seamark:dumping_ground:category:2','chemical_waste'], // Chemical Waste
     ['DPG2','3','seamark:dumping_ground:category:2','nuclear_waste'], // Nuclear Waste
     ['DPG2','4','seamark:dumping_ground:category:2','explosives'], // Explosives
     ['DPG2','5','seamark:dumping_ground:category:2','spoil'], // Spoil
     ['DPG2','6','seamark:dumping_ground:category:2','vessels'], // Vessels
     ['DPG2','999','seamark:dumping_ground:category:2','other'], // Other

     // DPG3 - Dumping Ground Type [3]
     // ['DPG3','-999999',undefined,undefined], // No Information
     ['DPG3','1','seamark:dumping_ground:category:3','hazardous_material'], // Hazardous Material
     ['DPG3','2','seamark:dumping_ground:category:3','chemical_waste'], // Chemical Waste
     ['DPG3','3','seamark:dumping_ground:category:3','nuclear_waste'], // Nuclear Waste
     ['DPG3','4','seamark:dumping_ground:category:3','explosives'], // Explosives
     ['DPG3','5','seamark:dumping_ground:category:3','spoil'], // Spoil
     ['DPG3','6','seamark:dumping_ground:category:3','vessels'], // Vessels
     ['DPG3','999','seamark:dumping_ground:category:3','other'], // Other

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
     ['EET','1','earthwork:type','battery'], // Battery
     ['EET','2','earthwork:type','military_parapet'], // Military Parapet
     ['EET','3','earthwork:type','military_trench'], // Military Trench
     ['EET','4','earthwork:type','rampart'], // Rampart
     ['EET','5','earthwork:type','redoubt'], // Redoubt
     ['EET','999','earthwork:type','other'], // Other

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
     ['FFN','2','raw:FFN','agriculture'], // Agriculture
     ['FFN','3','raw:FFN','growing_of_crops'], // Growing of Crops
     ['FFN','9','raw:FFN','raising_of_animals'], // Raising of Animals
     ['FFN','14','raw:FFN','grazing'], // Grazing
     ['FFN','15','raw:FFN','mixed_farming'], // Mixed Farming
     ['FFN','16','raw:FFN','migrant_labour'], // Migrant Labour
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
     ['FFN','829','raw:FFN','maritime_defence'], // Maritime Defence
     ['FFN','830','raw:FFN','public_order,_safety_and_security_services'], // Public Order, Safety and Security Services
     ['FFN','831','raw:FFN','public_order'], // Public Order
     ['FFN','832','raw:FFN','safety'], // Safety
     ['FFN','833','raw:FFN','security_services'], // Security Services
     ['FFN','835','raw:FFN','defence_activities'], // Defence Activities
     ['FFN','836','raw:FFN','armoury'], // Armoury
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
     // ['FHC2','-999999',undefined,undefined], // No Information
     ['FHC2','1','harbour:use:2','ro-ro_terminal'], // Ro-Ro Terminal
     ['FHC2','2','harbour:use:2','timber-yard'], // Timber-yard
     ['FHC2','3','harbour:use:2','ferry_terminal'], // Ferry Terminal
     ['FHC2','4','harbour:use:2','fishing_harbour'], // Fishing Harbour
     ['FHC2','5','harbour:use:2','marina'], // Marina
     ['FHC2','6','harbour:use:2','naval_base'], // Naval Base
     ['FHC2','7','harbour:use:2','tanker_terminal'], // Tanker Terminal
     ['FHC2','8','harbour:use:2','passenger_terminal'], // Passenger Terminal
     ['FHC2','9','harbour:use:2','shipyard'], // Shipyard
     ['FHC2','10','harbour:use:2','container_terminal'], // Container Terminal
     ['FHC2','11','harbour:use:2','bulk_terminal'], // Bulk Terminal
     ['FHC2','999','harbour:use:2','other'], // Other

     // FHC3 - Harbour Facility Function [3]
     // ['FHC3','-999999',undefined,undefined], // No Information
     ['FHC3','1','harbour:use:3','ro-ro_terminal'], // Ro-Ro Terminal
     ['FHC3','2','harbour:use:3','timber-yard'], // Timber-yard
     ['FHC3','3','harbour:use:3','ferry_terminal'], // Ferry Terminal
     ['FHC3','4','harbour:use:3','fishing_harbour'], // Fishing Harbour
     ['FHC3','5','harbour:use:3','marina'], // Marina
     ['FHC3','6','harbour:use:3','naval_base'], // Naval Base
     ['FHC3','7','harbour:use:3','tanker_terminal'], // Tanker Terminal
     ['FHC3','8','harbour:use:3','passenger_terminal'], // Passenger Terminal
     ['FHC3','9','harbour:use:3','shipyard'], // Shipyard
     ['FHC3','10','harbour:use:3','container_terminal'], // Container Terminal
     ['FHC3','11','harbour:use:3','bulk_terminal'], // Bulk Terminal
     ['FHC3','999','harbour:use:3','other'], // Other

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
     // ['FRT2','-999999',undefined,undefined], // No Information
     ['FRT2','1','range:type:2','small_arms'], // Small Arms
     ['FRT2','2','range:type:2','tank'], // Tank
     ['FRT2','3','range:type:2','artillery'], // Field Artillery
     ['FRT2','4','range:type:2','grenade'], // Grenade
     ['FRT2','5','range:type:2','demolition_area'], // Demolition Area
     ['FRT2','6','range:type:2','impact_area'], // Impact Area
     ['FRT2','999','range:type:2','other'], // Other

     // FRT3 - Weapons Range Type [3]
     // ['FRT3','-999999',undefined,undefined], // No Information
     ['FRT3','1','range:type:3','small_arms'], // Small Arms
     ['FRT3','2','range:type:3','tank'], // Tank
     ['FRT3','3','range:type:3','artillery'], // Field Artillery
     ['FRT3','4','range:type:3','grenade'], // Grenade
     ['FRT3','5','range:type:3','demolition_area'], // Demolition Area
     ['FRT3','6','range:type:3','impact_area'], // Impact Area
     ['FRT3','999','range:type:3','other'], // Other

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
     ['GDBV','3','source:schema','tds_6_1'],     // TDS_6_1
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
     ['HST','1','shelter:type','bomb_shelter'], // Bomb Shelter
     ['HST','2','shelter:type','fallout_shelter'], // Fallout Shelter
     ['HST','3','shelter:type','storm_shelter'], // Storm Shelter
     ['HST','999','shelter:type','other'], // Other

     // HST2 - Hazard Shelter Intended Use [2]
     // ['HST2','-999999',undefined,undefined], // No Information
     ['HST2','1','shelter:type:2','bomb_shelter'], // Bomb Shelter
     ['HST2','2','shelter:type:2','fallout_shelter'], // Fallout Shelter
     ['HST2','3','shelter:type:2','storm_shelter'], // Storm Shelter
     ['HST2','999','shelter:type:2','other'], // Other

     // HST3 - Hazard Shelter Intended Use [3]
     // ['HST3','-999999',undefined,undefined], // No Information
     ['HST3','1','shelter:type:3','bomb_shelter'], // Bomb Shelter
     ['HST3','2','shelter:type:3','fallout_shelter'], // Fallout Shelter
     ['HST3','3','shelter:type:3','storm_shelter'], // Storm Shelter
     ['HST3','999','shelter:type:3','other'], // Other

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
     ['IWT','4','water','reservoir'], // Reservoir
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
     // ['LFC2','-999999',undefined,undefined], // No Information
     ['LFC2','1','seamark:light:category:2','directional'], // Directional Function
     ['LFC2','4','seamark:light:category:2','leading'], // Leading Light
     ['LFC2','6','seamark:light:category:2','air_obstruction'], // Air Obstruction Light
     ['LFC2','7','seamark:light:category:2','fog_detector'], // Fog Detector Light
     ['LFC2','8','seamark:light:category:2','flood_light'], // Flood Light
     ['LFC2','9','seamark:light:category:2','strip_light'], // Strip Light
     ['LFC2','10','seamark:light:category:2','subsidiary'], // Subsidiary Light
     ['LFC2','11','seamark:light:category:2','spotlight'], // Spotlight
     ['LFC2','12','seamark:light:category:2','emergency'], // Emergency Light
     ['LFC2','13','seamark:light:category:2','bearing'], // Bearing Light
     ['LFC2','14','seamark:light:category:2','upper'], // Upper
     ['LFC2','16','seamark:light:category:2','lower'], // Lower
     ['LFC2','17','seamark:light:category:2','horizontal'], // Horizontally Disposed
     ['LFC2','18','seamark:light:category:2','vertical'], // Vertically Disposed
     ['LFC2','19','seamark:light:category:2','front'], // Front
     ['LFC2','20','seamark:light:category:2','rear'], // Rear
     ['LFC2','21','seamark:light:category:2','moire'], // Moire Effect Light
     ['LFC2','22','seamark:light:category:2','sectored_light'], // Sectored Light
     ['LFC2','25','seamark:light:category:2','occasional_light'], // Occasional Light
     ['LFC2','999','seamark:light:category:2','other'], // Other

     // LFC3 - Maritime Light Type [3]
     // ['LFC3','-999999',undefined,undefined], // No Information
     ['LFC3','1','seamark:light:category:3','directional'], // Directional Function
     ['LFC3','4','seamark:light:category:3','leading'], // Leading Light
     ['LFC3','6','seamark:light:category:3','air_obstruction'], // Air Obstruction Light
     ['LFC3','7','seamark:light:category:3','fog_detector'], // Fog Detector Light
     ['LFC3','8','seamark:light:category:3','flood_light'], // Flood Light
     ['LFC3','9','seamark:light:category:3','strip_light'], // Strip Light
     ['LFC3','10','seamark:light:category:3','subsidiary'], // Subsidiary Light
     ['LFC3','11','seamark:light:category:3','spotlight'], // Spotlight
     ['LFC3','12','seamark:light:category:3','emergency'], // Emergency Light
     ['LFC3','13','seamark:light:category:3','bearing'], // Bearing Light
     ['LFC3','14','seamark:light:category:3','upper'], // Upper
     ['LFC3','16','seamark:light:category:3','lower'], // Lower
     ['LFC3','17','seamark:light:category:3','horizontal'], // Horizontally Disposed
     ['LFC3','18','seamark:light:category:3','vertical'], // Vertically Disposed
     ['LFC3','19','seamark:light:category:3','front'], // Front
     ['LFC3','20','seamark:light:category:3','rear'], // Rear
     ['LFC3','21','seamark:light:category:3','moire'], // Moire Effect Light
     ['LFC3','22','seamark:light:category:3','sectored_light'], // Sectored Light
     ['LFC3','25','seamark:light:category:3','occasional_light'], // Occasional Light
     ['LFC3','999','seamark:light:category:3','other'], // Other

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
     ['MCC','84','material','rocks'], // Rocks
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
     // ['MCC2','-999999',undefined,undefined], // No Information
     ['MCC2','2','material:2','aluminium'], // Aluminium
     ['MCC2','5','material:2','asphalt'], // Asphalt
     ['MCC2','7','material:2','bedrock'], // Bedrock
     ['MCC2','8','material:2','boulders'], // Boulders
     ['MCC2','9','material:2','brick'], // Brick
     ['MCC2','21','material:2','concrete'], // Concrete
     ['MCC2','23','material:2','copper'], // Copper
     ['MCC2','24','material:2','coral'], // Coral
     ['MCC2','25','material:2','coral_head'], // Coral Head
     ['MCC2','46','material:2','gravel'], // Gravel
     ['MCC2','51','material:2','iron'], // Iron
     ['MCC2','52','material:2','lava'], // Lava
     ['MCC2','54','material:2','lead'], // Lead
     ['MCC2','55','material:2','loess'], // Loess
     ['MCC2','62','material:2','masonry'], // Masonry
     ['MCC2','64','material:2','metal'], // Metal
     ['MCC2','65','material:2','mud'], // Mud
     ['MCC2','74','material:2','plastic'], // Plastic
     ['MCC2','77','material:2','prestressed_concrete'], // Prestressed Concrete
     ['MCC2','83','material:2','reinforced_concrete'], // Reinforced Concrete
     ['MCC2','84','material:2','rocks'], // Rocks
     ['MCC2','86','material:2','rubble'], // Rubble
     ['MCC2','88','material:2','sand'], // Sand
     ['MCC2','103','material:2','frozen_water'], // Frozen Water
     ['MCC2','104','material:2','soil'], // Soil
     ['MCC2','107','material:2','steel'], // Steel
     ['MCC2','108','material:2','stone'], // Stone
     ['MCC2','117','material:2','wood'], // Wood
     ['MCC2','119','material:2','evaporite'], // Evaporite
     ['MCC2','120','material:2','glass_reinforced_plastic_(grp)'], // Glass Reinforced Plastic (GRP)
     ['MCC2','131','material:2','ceramic'], // Ceramic
     ['MCC2','150','material:2','treated_timber'], // Treated Timber
     ['MCC2','152','material:2','fibreglass'], // Fibreglass
     ['MCC2','160','material:2','earthen'], // Earthen
     ['MCC2','999','material:2','other'], // Other

     // MCC3 - Structural Material Type [3]
     // ['MCC3','-999999',undefined,undefined], // No Information
     ['MCC3','2','material:3','aluminium'], // Aluminium
     ['MCC3','5','material:3','asphalt'], // Asphalt
     ['MCC3','7','material:3','bedrock'], // Bedrock
     ['MCC3','8','material:3','boulders'], // Boulders
     ['MCC3','9','material:3','brick'], // Brick
     ['MCC3','21','material:3','concrete'], // Concrete
     ['MCC3','23','material:3','copper'], // Copper
     ['MCC3','24','material:3','coral'], // Coral
     ['MCC3','25','material:3','coral_head'], // Coral Head
     ['MCC3','46','material:3','gravel'], // Gravel
     ['MCC3','51','material:3','iron'], // Iron
     ['MCC3','52','material:3','lava'], // Lava
     ['MCC3','54','material:3','lead'], // Lead
     ['MCC3','55','material:3','loess'], // Loess
     ['MCC3','62','material:3','masonry'], // Masonry
     ['MCC3','64','material:3','metal'], // Metal
     ['MCC3','65','material:3','mud'], // Mud
     ['MCC3','74','material:3','plastic'], // Plastic
     ['MCC3','77','material:3','prestressed_concrete'], // Prestressed Concrete
     ['MCC3','83','material:3','reinforced_concrete'], // Reinforced Concrete
     ['MCC3','84','material:3','rocks'], // Rocks
     ['MCC3','86','material:3','rubble'], // Rubble
     ['MCC3','88','material:3','sand'], // Sand
     ['MCC3','103','material:3','frozen_water'], // Frozen Water
     ['MCC3','104','material:3','soil'], // Soil
     ['MCC3','107','material:3','steel'], // Steel
     ['MCC3','108','material:3','stone'], // Stone
     ['MCC3','117','material:3','wood'], // Wood
     ['MCC3','119','material:3','evaporite'], // Evaporite
     ['MCC3','120','material:3','glass_reinforced_plastic_(grp)'], // Glass Reinforced Plastic (GRP)
     ['MCC3','131','material:3','ceramic'], // Ceramic
     ['MCC3','150','material:3','treated_timber'], // Treated Timber
     ['MCC3','152','material:3','fibreglass'], // Fibreglass
     ['MCC3','160','material:3','earthen'], // Earthen
     ['MCC3','999','material:3','other'], // Other

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
     // ['MCY2','-999999',undefined,undefined], // No Information
     ['MCY2','1','seamark:precautionary_area:2','cargo_transshipment'], // Cargo Transshipment
     ['MCY2','2','seamark:precautionary_area:2','ice_advisory'], // Ice Advisory
     ['MCY2','3','seamark:precautionary_area:2','intake_area'], // Intake Area
     ['MCY2','4','seamark:precautionary_area:2','outfall_area'], // Outfall Area
     ['MCY2','5','seamark:precautionary_area:2','pollution_zone'], // Pollution Zone
     ['MCY2','6','seamark:precautionary_area:2','submarine_cable'], // Submarine Cable
     ['MCY2','7','seamark:precautionary_area:2','submarine_pipeline'], // Submarine Pipeline
     ['MCY2','8','seamark:precautionary_area:2','submarine_sewer'], // Submarine Sewer
     ['MCY2','9','seamark:precautionary_area:2','unexploded_ordnance'], // Unexploded Ordnance
     ['MCY2','10','seamark:precautionary_area:2','clearing_line'], // Clearing Line
     ['MCY2','11','seamark:precautionary_area:2','colregs_demarcation_line'], // COLREGS Demarcation Line
     ['MCY2','12','seamark:precautionary_area:2','harbour_area'], // Harbour Area
     ['MCY2','13','seamark:precautionary_area:2','ocean_current_margin'], // Ocean Current Margin
     ['MCY2','14','seamark:precautionary_area:2','works_in_progress_area'], // Works in Progress Area
     ['MCY2','15','seamark:precautionary_area:2','unsurveyed_area'], // Unsurveyed Area
     ['MCY2','999','seamark:precautionary_area:2','other'], // Other

     // MCY3 - Maritime Caution Type [3]
     // ['MCY3','-999999',undefined,undefined], // No Information
     ['MCY3','1','seamark:precautionary_area:3','cargo_transshipment'], // Cargo Transshipment
     ['MCY3','2','seamark:precautionary_area:3','ice_advisory'], // Ice Advisory
     ['MCY3','3','seamark:precautionary_area:3','intake_area'], // Intake Area
     ['MCY3','4','seamark:precautionary_area:3','outfall_area'], // Outfall Area
     ['MCY3','5','seamark:precautionary_area:3','pollution_zone'], // Pollution Zone
     ['MCY3','6','seamark:precautionary_area:3','submarine_cable'], // Submarine Cable
     ['MCY3','7','seamark:precautionary_area:3','submarine_pipeline'], // Submarine Pipeline
     ['MCY3','8','seamark:precautionary_area:3','submarine_sewer'], // Submarine Sewer
     ['MCY3','9','seamark:precautionary_area:3','unexploded_ordnance'], // Unexploded Ordnance
     ['MCY3','10','seamark:precautionary_area:3','clearing_line'], // Clearing Line
     ['MCY3','11','seamark:precautionary_area:3','colregs_demarcation_line'], // COLREGS Demarcation Line
     ['MCY3','12','seamark:precautionary_area:3','harbour_area'], // Harbour Area
     ['MCY3','13','seamark:precautionary_area:3','ocean_current_margin'], // Ocean Current Margin
     ['MCY3','14','seamark:precautionary_area:3','works_in_progress_area'], // Works in Progress Area
     ['MCY3','15','seamark:precautionary_area:3','unsurveyed_area'], // Unsurveyed Area
     ['MCY3','999','seamark:precautionary_area:3','other'], // Other

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
     // ['MGL2','-999999',undefined,undefined], // No Information
     ['MGL2','1','geopolitical_line:maritime:2','international_boundary'], // International Boundary
     ['MGL2','2','geopolitical_line:maritime:2','territorial_sea_baseline'], // Territorial Sea Baseline
     ['MGL2','3','geopolitical_line:maritime:2','three_nautical_mile_limit'], // Three Nautical Mile Limit
     ['MGL2','4','geopolitical_line:maritime:2','territorial_waters_limit_of_sovereignty'], // Territorial Waters Limit of Sovereignty
     ['MGL2','5','geopolitical_line:maritime:2','contiguous_zone_limit'], // Contiguous Zone Limit
     ['MGL2','6','geopolitical_line:maritime:2','continental_shelf_limit'], // Continental Shelf Limit
     ['MGL2','7','geopolitical_line:maritime:2','extended_continental_shelf_limit'], // Extended Continental Shelf Limit
     ['MGL2','8','geopolitical_line:maritime:2','exclusive_economic_zone_limit'], // Exclusive Economic Zone Limit
     ['MGL2','9','geopolitical_line:maritime:2','customs_boundary'], // Customs Boundary
     ['MGL2','999','geopolitical_line:maritime:2','other'], // Other

     // MGL3 - Maritime Geopolitical Limit Type [3]
     // ['MGL3','-999999',undefined,undefined], // No Information
     ['MGL3','1','geopolitical_line:maritime:3','international_boundary'], // International Boundary
     ['MGL3','2','geopolitical_line:maritime:3','territorial_sea_baseline'], // Territorial Sea Baseline
     ['MGL3','3','geopolitical_line:maritime:3','three_nautical_mile_limit'], // Three Nautical Mile Limit
     ['MGL3','4','geopolitical_line:maritime:3','territorial_waters_limit_of_sovereignty'], // Territorial Waters Limit of Sovereignty
     ['MGL3','5','geopolitical_line:maritime:3','contiguous_zone_limit'], // Contiguous Zone Limit
     ['MGL3','6','geopolitical_line:maritime:3','continental_shelf_limit'], // Continental Shelf Limit
     ['MGL3','7','geopolitical_line:maritime:3','extended_continental_shelf_limit'], // Extended Continental Shelf Limit
     ['MGL3','8','geopolitical_line:maritime:3','exclusive_economic_zone_limit'], // Exclusive Economic Zone Limit
     ['MGL3','9','geopolitical_line:maritime:3','customs_boundary'], // Customs Boundary
     ['MGL3','999','geopolitical_line:maritime:3','other'], // Other

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
     // ['MNR2','-999999',undefined,undefined], // No Information
     ['MNR2','1','seamark:precautionary_area:restriction:2','anchoring_prohibited'], // Anchoring Prohibited
     ['MNR2','2','seamark:precautionary_area:restriction:2','anchoring_restricted'], // Anchoring Restricted
     ['MNR2','3','seamark:precautionary_area:restriction:2','fishing_prohibited'], // Fishing Prohibited
     ['MNR2','4','seamark:precautionary_area:restriction:2','fishing_restricted'], // Fishing Restricted
     ['MNR2','5','seamark:precautionary_area:restriction:2','trawling_prohibited'], // Trawling Prohibited
     ['MNR2','6','seamark:precautionary_area:restriction:2','trawling_restricted'], // Trawling Restricted
     ['MNR2','7','seamark:precautionary_area:restriction:2','entry_prohibited'], // Entry Prohibited
     ['MNR2','8','seamark:precautionary_area:restriction:2','entry_restricted'], // Entry Restricted
     ['MNR2','9','seamark:precautionary_area:restriction:2','dredging_prohibited'], // Dredging Prohibited
     ['MNR2','10','seamark:precautionary_area:restriction:2','dredging_restricted'], // Dredging Restricted
     ['MNR2','11','seamark:precautionary_area:restriction:2','diving_prohibited'], // Diving Prohibited
     ['MNR2','12','seamark:precautionary_area:restriction:2','diving_restricted'], // Diving Restricted
     ['MNR2','13','seamark:precautionary_area:restriction:2','reduced_wake_area'], // Reduced Wake Area
     ['MNR2','14','seamark:precautionary_area:restriction:2','avoidance_area'], // Avoidance Area
     ['MNR2','15','seamark:precautionary_area:restriction:2','construction_prohibited'], // Construction Prohibited
     ['MNR2','16','seamark:precautionary_area:restriction:2','discharging_prohibited'], // Discharging Prohibited
     ['MNR2','17','seamark:precautionary_area:restriction:2','discharging_restricted'], // Discharging Restricted
     ['MNR2','18','seamark:precautionary_area:restriction:2','exploration_and/or_development_prohibited'], // Exploration and/or Development Prohibited
     ['MNR2','19','seamark:precautionary_area:restriction:2','exploration_and/or_development_restricted'], // Exploration and/or Development Restricted
     ['MNR2','20','seamark:precautionary_area:restriction:2','drilling_prohibited'], // Drilling Prohibited
     ['MNR2','21','seamark:precautionary_area:restriction:2','drilling_restricted'], // Drilling Restricted
     ['MNR2','22','seamark:precautionary_area:restriction:2','artefact_removal_prohibited'], // Artefact Removal Prohibited
     ['MNR2','23','seamark:precautionary_area:restriction:2','cargo_transshipment_prohibited'], // Cargo Transshipment Prohibited
     ['MNR2','24','seamark:precautionary_area:restriction:2','dragging_prohibited'], // Dragging Prohibited
     ['MNR2','25','seamark:precautionary_area:restriction:2','stopping_prohibited'], // Stopping Prohibited
     ['MNR2','26','seamark:precautionary_area:restriction:2','landing_prohibited'], // Landing Prohibited
     ['MNR2','27','seamark:precautionary_area:restriction:2','limited_speed_area'], // Limited Speed Area
     ['MNR2','28','seamark:precautionary_area:restriction:2','navigational_aid_safety_zone'], // Navigational Aid Safety Zone
     ['MNR2','29','seamark:precautionary_area:restriction:2','offshore_safety_zone'], // Offshore Safety Zone
     ['MNR2','30','seamark:precautionary_area:restriction:2','shipping_prohibited'], // Shipping Prohibited
     ['MNR2','31','seamark:precautionary_area:restriction:2','swimming_prohibited'], // Swimming Prohibited
     ['MNR2','999','seamark:precautionary_area:restriction:2','other'], // Other

     // MNR3 - Maritime Navigation Restriction [3]
     // ['MNR3','-999999',undefined,undefined], // No Information
     ['MNR3','1','seamark:precautionary_area:restriction:3','anchoring_prohibited'], // Anchoring Prohibited
     ['MNR3','2','seamark:precautionary_area:restriction:3','anchoring_restricted'], // Anchoring Restricted
     ['MNR3','3','seamark:precautionary_area:restriction:3','fishing_prohibited'], // Fishing Prohibited
     ['MNR3','4','seamark:precautionary_area:restriction:3','fishing_restricted'], // Fishing Restricted
     ['MNR3','5','seamark:precautionary_area:restriction:3','trawling_prohibited'], // Trawling Prohibited
     ['MNR3','6','seamark:precautionary_area:restriction:3','trawling_restricted'], // Trawling Restricted
     ['MNR3','7','seamark:precautionary_area:restriction:3','entry_prohibited'], // Entry Prohibited
     ['MNR3','8','seamark:precautionary_area:restriction:3','entry_restricted'], // Entry Restricted
     ['MNR3','9','seamark:precautionary_area:restriction:3','dredging_prohibited'], // Dredging Prohibited
     ['MNR3','10','seamark:precautionary_area:restriction:3','dredging_restricted'], // Dredging Restricted
     ['MNR3','11','seamark:precautionary_area:restriction:3','diving_prohibited'], // Diving Prohibited
     ['MNR3','12','seamark:precautionary_area:restriction:3','diving_restricted'], // Diving Restricted
     ['MNR3','13','seamark:precautionary_area:restriction:3','reduced_wake_area'], // Reduced Wake Area
     ['MNR3','14','seamark:precautionary_area:restriction:3','avoidance_area'], // Avoidance Area
     ['MNR3','15','seamark:precautionary_area:restriction:3','construction_prohibited'], // Construction Prohibited
     ['MNR3','16','seamark:precautionary_area:restriction:3','discharging_prohibited'], // Discharging Prohibited
     ['MNR3','17','seamark:precautionary_area:restriction:3','discharging_restricted'], // Discharging Restricted
     ['MNR3','18','seamark:precautionary_area:restriction:3','exploration_and/or_development_prohibited'], // Exploration and/or Development Prohibited
     ['MNR3','19','seamark:precautionary_area:restriction:3','exploration_and/or_development_restricted'], // Exploration and/or Development Restricted
     ['MNR3','20','seamark:precautionary_area:restriction:3','drilling_prohibited'], // Drilling Prohibited
     ['MNR3','21','seamark:precautionary_area:restriction:3','drilling_restricted'], // Drilling Restricted
     ['MNR3','22','seamark:precautionary_area:restriction:3','artefact_removal_prohibited'], // Artefact Removal Prohibited
     ['MNR3','23','seamark:precautionary_area:restriction:3','cargo_transshipment_prohibited'], // Cargo Transshipment Prohibited
     ['MNR3','24','seamark:precautionary_area:restriction:3','dragging_prohibited'], // Dragging Prohibited
     ['MNR3','25','seamark:precautionary_area:restriction:3','stopping_prohibited'], // Stopping Prohibited
     ['MNR3','26','seamark:precautionary_area:restriction:3','landing_prohibited'], // Landing Prohibited
     ['MNR3','27','seamark:precautionary_area:restriction:3','limited_speed_area'], // Limited Speed Area
     ['MNR3','28','seamark:precautionary_area:restriction:3','navigational_aid_safety_zone'], // Navigational Aid Safety Zone
     ['MNR3','29','seamark:precautionary_area:restriction:3','offshore_safety_zone'], // Offshore Safety Zone
     ['MNR3','30','seamark:precautionary_area:restriction:3','shipping_prohibited'], // Shipping Prohibited
     ['MNR3','31','seamark:precautionary_area:restriction:3','swimming_prohibited'], // Swimming Prohibited
     ['MNR3','999','seamark:precautionary_area:restriction:3','other'], // Other

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
     // ['MRR2','-999999',undefined,undefined], // No Information
     ['MRR2','1','seamark:restricted_area:category:2','unrestricted_area'], // Unrestricted Area
     ['MRR2','2','seamark:restricted_area:category:2','bird_sanctuary'], // Bird Sanctuary
     ['MRR2','3','seamark:restricted_area:category:2','conservation_and_management_zone'], // Conservation and Management Zone
     ['MRR2','4','seamark:restricted_area:category:2','ecological_reserve'], // Ecological Reserve
     ['MRR2','5','seamark:restricted_area:category:2','fish_sanctuary'], // Fish Sanctuary
     ['MRR2','6','seamark:restricted_area:category:2','game_preserve'], // Game Preserve
     ['MRR2','7','seamark:restricted_area:category:2','marine_nature_reserve'], // Marine Nature Reserve
     ['MRR2','8','seamark:restricted_area:category:2','nature_reserve'], // Nature Reserve
     ['MRR2','9','seamark:restricted_area:category:2','seal_sanctuary'], // Seal Sanctuary
     ['MRR2','10','seamark:restricted_area:category:2','fishing_ground'], // Fishing Ground
     ['MRR2','11','seamark:restricted_area:category:2','fishery_zone'], // Fishery Zone
     ['MRR2','12','seamark:restricted_area:category:2','foreign_fishing_rights_zone'], // Foreign Fishing Rights Zone
     ['MRR2','13','seamark:restricted_area:category:2','unilateral_fishing_zone'], // Unilateral Fishing Zone
     ['MRR2','14','seamark:restricted_area:category:2','international_council_for_the_seas_(ices)_fishery_zone'], // International Council for the Seas (ICES) Fishery Zone
     ['MRR2','15','seamark:restricted_area:category:2','compass_adjustment'], // Compass Adjustment
     ['MRR2','16','seamark:restricted_area:category:2','degaussing_range'], // Degaussing Range
     ['MRR2','17','seamark:restricted_area:category:2','dredging_area'], // Dredging Area
     ['MRR2','18','seamark:restricted_area:category:2','fairway'], // Fairway
     ['MRR2','19','seamark:restricted_area:category:2','historic_wreck_area'], // Historic Wreck Area
     ['MRR2','20','seamark:restricted_area:category:2','incineration_area'], // Incineration Area
     ['MRR2','21','seamark:restricted_area:category:2','joint_economic_development_zone'], // Joint Economic Development Zone
     ['MRR2','22','seamark:restricted_area:category:2','maritime_mass_grave'], // Maritime Mass Grave
     ['MRR2','23','seamark:restricted_area:category:2','maritime_pollution_(marpol)_reporting_area'], // Maritime Pollution (MARPOL) Reporting Area
     ['MRR2','24','seamark:restricted_area:category:2','mine_danger_area'], // Mine Danger Area
     ['MRR2','25','seamark:restricted_area:category:2','former_mine_danger_area'], // Former Mine Danger Area
     ['MRR2','26','seamark:restricted_area:category:2','natural_gas_field'], // Natural Gas Field
     ['MRR2','27','seamark:restricted_area:category:2','petroleum_field'], // Petroleum Field
     ['MRR2','28','seamark:restricted_area:category:2','reclamation_area'], // Reclamation Area
     ['MRR2','29','seamark:restricted_area:category:2','research'], // Research Area
     ['MRR2','30','seamark:restricted_area:category:2','stranding_danger_area'], // Stranding Danger Area
     ['MRR2','31','seamark:restricted_area:category:2','swimming_area'], // Swimming Area
     ['MRR2','32','seamark:restricted_area:category:2','swinging_area'], // Swinging Area
     ['MRR2','33','seamark:restricted_area:category:2','waiting_area'], // Waiting Area
     ['MRR2','34','seamark:restricted_area:category:2','water_skiing_area'], // Water Skiing Area
     ['MRR2','35','seamark:restricted_area:category:2','exclusive_economic_zone'], // Exclusive Economic Zone
     ['MRR2','36','seamark:restricted_area:category:2','extended_continental_shelf_area'], // Extended Continental Shelf Area
     ['MRR2','37','seamark:restricted_area:category:2','continental_shelf_area'], // Continental Shelf Area
     ['MRR2','38','seamark:restricted_area:category:2','contiguous_zone'], // Contiguous Zone
     ['MRR2','39','seamark:restricted_area:category:2','territorial_sea_area'], // Territorial Sea Area
     ['MRR2','40','seamark:restricted_area:category:2','internal_waters'], // Internal Waters
     ['MRR2','41','seamark:restricted_area:category:2','no_wake_area'], // No Wake Area
     ['MRR2','42','seamark:restricted_area:category:2','traffic_services_limit'], // Traffic Services Limit
     ['MRR2','45','seamark:restricted_area:category:2','environmentally_sensitive_sea_area'], // Environmentally Sensitive Sea Area
     ['MRR2','46','seamark:restricted_area:category:2','particularly_sensitive_sea_area'], // Particularly Sensitive Sea Area
     ['MRR2','47','seamark:restricted_area:category:2','military_area'], // Military Area
     ['MRR2','52','seamark:restricted_area:category:2','disengagement_area'], // Disengagement Area
     ['MRR2','999','seamark:restricted_area:category:2','other'], // Other

     // MRR3 - Maritime Area Restriction [3]
     // ['MRR3','-999999',undefined,undefined], // No Information
     ['MRR3','1','seamark:restricted_area:category:3','unrestricted_area'], // Unrestricted Area
     ['MRR3','2','seamark:restricted_area:category:3','bird_sanctuary'], // Bird Sanctuary
     ['MRR3','3','seamark:restricted_area:category:3','conservation_and_management_zone'], // Conservation and Management Zone
     ['MRR3','4','seamark:restricted_area:category:3','ecological_reserve'], // Ecological Reserve
     ['MRR3','5','seamark:restricted_area:category:3','fish_sanctuary'], // Fish Sanctuary
     ['MRR3','6','seamark:restricted_area:category:3','game_preserve'], // Game Preserve
     ['MRR3','7','seamark:restricted_area:category:3','marine_nature_reserve'], // Marine Nature Reserve
     ['MRR3','8','seamark:restricted_area:category:3','nature_reserve'], // Nature Reserve
     ['MRR3','9','seamark:restricted_area:category:3','seal_sanctuary'], // Seal Sanctuary
     ['MRR3','10','seamark:restricted_area:category:3','fishing_ground'], // Fishing Ground
     ['MRR3','11','seamark:restricted_area:category:3','fishery_zone'], // Fishery Zone
     ['MRR3','12','seamark:restricted_area:category:3','foreign_fishing_rights_zone'], // Foreign Fishing Rights Zone
     ['MRR3','13','seamark:restricted_area:category:3','unilateral_fishing_zone'], // Unilateral Fishing Zone
     ['MRR3','14','seamark:restricted_area:category:3','international_council_for_the_seas_(ices)_fishery_zone'], // International Council for the Seas (ICES) Fishery Zone
     ['MRR3','15','seamark:restricted_area:category:3','compass_adjustment'], // Compass Adjustment
     ['MRR3','16','seamark:restricted_area:category:3','degaussing_range'], // Degaussing Range
     ['MRR3','17','seamark:restricted_area:category:3','dredging_area'], // Dredging Area
     ['MRR3','18','seamark:restricted_area:category:3','fairway'], // Fairway
     ['MRR3','19','seamark:restricted_area:category:3','historic_wreck_area'], // Historic Wreck Area
     ['MRR3','20','seamark:restricted_area:category:3','incineration_area'], // Incineration Area
     ['MRR3','21','seamark:restricted_area:category:3','joint_economic_development_zone'], // Joint Economic Development Zone
     ['MRR3','22','seamark:restricted_area:category:3','maritime_mass_grave'], // Maritime Mass Grave
     ['MRR3','23','seamark:restricted_area:category:3','maritime_pollution_(marpol)_reporting_area'], // Maritime Pollution (MARPOL) Reporting Area
     ['MRR3','24','seamark:restricted_area:category:3','mine_danger_area'], // Mine Danger Area
     ['MRR3','25','seamark:restricted_area:category:3','former_mine_danger_area'], // Former Mine Danger Area
     ['MRR3','26','seamark:restricted_area:category:3','natural_gas_field'], // Natural Gas Field
     ['MRR3','27','seamark:restricted_area:category:3','petroleum_field'], // Petroleum Field
     ['MRR3','28','seamark:restricted_area:category:3','reclamation_area'], // Reclamation Area
     ['MRR3','29','seamark:restricted_area:category:3','research'], // Research Area
     ['MRR3','30','seamark:restricted_area:category:3','stranding_danger_area'], // Stranding Danger Area
     ['MRR3','31','seamark:restricted_area:category:3','swimming_area'], // Swimming Area
     ['MRR3','32','seamark:restricted_area:category:3','swinging_area'], // Swinging Area
     ['MRR3','33','seamark:restricted_area:category:3','waiting_area'], // Waiting Area
     ['MRR3','34','seamark:restricted_area:category:3','water_skiing_area'], // Water Skiing Area
     ['MRR3','35','seamark:restricted_area:category:3','exclusive_economic_zone'], // Exclusive Economic Zone
     ['MRR3','36','seamark:restricted_area:category:3','extended_continental_shelf_area'], // Extended Continental Shelf Area
     ['MRR3','37','seamark:restricted_area:category:3','continental_shelf_area'], // Continental Shelf Area
     ['MRR3','38','seamark:restricted_area:category:3','contiguous_zone'], // Contiguous Zone
     ['MRR3','39','seamark:restricted_area:category:3','territorial_sea_area'], // Territorial Sea Area
     ['MRR3','40','seamark:restricted_area:category:3','internal_waters'], // Internal Waters
     ['MRR3','41','seamark:restricted_area:category:3','no_wake_area'], // No Wake Area
     ['MRR3','42','seamark:restricted_area:category:3','traffic_services_limit'], // Traffic Services Limit
     ['MRR3','45','seamark:restricted_area:category:3','environmentally_sensitive_sea_area'], // Environmentally Sensitive Sea Area
     ['MRR3','46','seamark:restricted_area:category:3','particularly_sensitive_sea_area'], // Particularly Sensitive Sea Area
     ['MRR3','47','seamark:restricted_area:category:3','military_area'], // Military Area
     ['MRR3','52','seamark:restricted_area:category:3','disengagement_area'], // Disengagement Area
     ['MRR3','999','seamark:restricted_area:category:3','other'], // Other

     // MRT - Maritime Radiobeacon Type
     // ['MRT','-999999',undefined,undefined], // No Information
//      ['MRT','1','raw:MRT','automated_identification_system_(ais)'], // Automated Identification System (AIS)
//      ['MRT','2','raw:MRT','circular_radiobeacon'], // Circular Radiobeacon
//      ['MRT','3','raw:MRT','directional_radiobeacon'], // Directional Radiobeacon
//      ['MRT','4','raw:MRT','long_range_air_navigation_system_(loran)_c'], // Long Range Air Navigation System (LORAN) C
//      ['MRT','5','raw:MRT','qtg_station'], // QTG Station
//      ['MRT','6','raw:MRT','radar_marker_(ramark)'], // Radar Marker (RAMARK)
//      ['MRT','7','raw:MRT','radar_responder_beacon_(racon)'], // Radar Responder Beacon (RACON)
//      ['MRT','8','raw:MRT','radio_direction_finding_station'], // Radio Direction Finding Station
//      ['MRT','9','raw:MRT','rotating_pattern_radiobeacon'], // Rotating Pattern Radiobeacon
//      ['MRT','999','raw:MRT','other'], // Other

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
     // ['MST2','-999999',undefined,undefined], // No Information
     ['MST2','1','missile:2','anti-ballistic_missile_(abm)'], // Anti-ballistic Missile (ABM)
     ['MST2','2','missile:2','intercontinental_ballistic_missile_(icbm)'], // Intercontinental Ballistic Missile (ICBM)
     ['MST2','3','missile:2','intermediate-range_ballistic_missile_(irbm)'], // Intermediate-range Ballistic Missile (IRBM)
     ['MST2','4','missile:2','sa-1_guild'], // SA-1 Guild
     ['MST2','5','missile:2','sa-2_guideline'], // SA-2 Guideline
     ['MST2','6','missile:2','sa-3_goa'], // SA-3 Goa
     ['MST2','7','missile:2','sa-4_ganef'], // SA-4 Ganef
     ['MST2','8','missile:2','sa-5_gammon'], // SA-5 Gammon
     ['MST2','9','missile:2','sa-6_gainful'], // SA-6 Gainful
     ['MST2','10','missile:2','sa-7_grail'], // SA-7 Grail
     ['MST2','11','missile:2','sa-8_gecko'], // SA-8 Gecko
     ['MST2','12','missile:2','sa-9_gaskin'], // SA-9 Gaskin
     ['MST2','13','missile:2','medium-range_ballistic_missile_(mrbm)'], // Medium-range Ballistic Missile (MRBM)
     ['MST2','14','missile:2','surface-to-surface_missile_(ssm)'], // Surface-to-surface Missile (SSM)
     ['MST2','15','missile:2','surface-to-air_missile_(sam)'], // Surface-to-air Missile (SAM)
     ['MST2','17','missile:2','sa-10_grumble'], // SA-10 Grumble
     ['MST2','18','missile:2','sa-11_gadfly'], // SA-11 Gadfly
     ['MST2','19','missile:2','sa-12_gladiator/giant'], // SA-12 Gladiator/Giant
     ['MST2','20','missile:2','sa-13_gopher'], // SA-13 Gopher
     ['MST2','21','missile:2','sa-14_gremlin'], // SA-14 Gremlin
     ['MST2','22','missile:2','sa-15_gauntlet'], // SA-15 Gauntlet
     ['MST2','23','missile:2','sa-16_gimlet'], // SA-16 Gimlet
     ['MST2','24','missile:2','sa-17_grizzly'], // SA-17 Grizzly
     ['MST2','25','missile:2','sa-18_grouse'], // SA-18 Grouse
     ['MST2','26','missile:2','sa-19_grisom'], // SA-19 Grisom
     ['MST2','999','missile:2','other'], // Other

     // MST3 - Missile Site Type [3]
     // ['MST3','-999999',undefined,undefined], // No Information
     ['MST3','1','missile:3','anti-ballistic_missile_(abm)'], // Anti-ballistic Missile (ABM)
     ['MST3','2','missile:3','intercontinental_ballistic_missile_(icbm)'], // Intercontinental Ballistic Missile (ICBM)
     ['MST3','3','missile:3','intermediate-range_ballistic_missile_(irbm)'], // Intermediate-range Ballistic Missile (IRBM)
     ['MST3','4','missile:3','sa-1_guild'], // SA-1 Guild
     ['MST3','5','missile:3','sa-2_guideline'], // SA-2 Guideline
     ['MST3','6','missile:3','sa-3_goa'], // SA-3 Goa
     ['MST3','7','missile:3','sa-4_ganef'], // SA-4 Ganef
     ['MST3','8','missile:3','sa-5_gammon'], // SA-5 Gammon
     ['MST3','9','missile:3','sa-6_gainful'], // SA-6 Gainful
     ['MST3','10','missile:3','sa-7_grail'], // SA-7 Grail
     ['MST3','11','missile:3','sa-8_gecko'], // SA-8 Gecko
     ['MST3','12','missile:3','sa-9_gaskin'], // SA-9 Gaskin
     ['MST3','13','missile:3','medium-range_ballistic_missile_(mrbm)'], // Medium-range Ballistic Missile (MRBM)
     ['MST3','14','missile:3','surface-to-surface_missile_(ssm)'], // Surface-to-surface Missile (SSM)
     ['MST3','15','missile:3','surface-to-air_missile_(sam)'], // Surface-to-air Missile (SAM)
     ['MST3','17','missile:3','sa-10_grumble'], // SA-10 Grumble
     ['MST3','18','missile:3','sa-11_gadfly'], // SA-11 Gadfly
     ['MST3','19','missile:3','sa-12_gladiator/giant'], // SA-12 Gladiator/Giant
     ['MST3','20','missile:3','sa-13_gopher'], // SA-13 Gopher
     ['MST3','21','missile:3','sa-14_gremlin'], // SA-14 Gremlin
     ['MST3','22','missile:3','sa-15_gauntlet'], // SA-15 Gauntlet
     ['MST3','23','missile:3','sa-16_gimlet'], // SA-16 Gimlet
     ['MST3','24','missile:3','sa-17_grizzly'], // SA-17 Grizzly
     ['MST3','25','missile:3','sa-18_grouse'], // SA-18 Grouse
     ['MST3','26','missile:3','sa-19_grisom'], // SA-19 Grisom
     ['MST3','999','missile:3','other'], // Other

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
     // ['NDX2','-999999',undefined,undefined], // No Information
     ['NDX2','1','neighbourhood:characteristic:2','economic_activity'], // Economic Activity
     ['NDX2','2','neighbourhood:characteristic:2','ethnicity'], // Ethnicity
     ['NDX2','3','neighbourhood:characteristic:2','historical'], // Historical
     ['NDX2','4','neighbourhood:characteristic:2','linguistics'], // Linguistics
     ['NDX2','5','neighbourhood:characteristic:2','physical_structure'], // Physical Structure
     ['NDX2','6','neighbourhood:characteristic:2','religion'], // Religion
     ['NDX2','7','neighbourhood:characteristic:2','socioeconomics'], // Socioeconomics
     ['NDX2','999','neighbourhood:characteristic:2','other'], // Other

     // NDX3 - Neighbourhood-defining Characteristic [3]
     // ['NDX3','-999999',undefined,undefined], // No Information
     ['NDX3','1','neighbourhood:characteristic:3','economic_activity'], // Economic Activity
     ['NDX3','2','neighbourhood:characteristic:3','ethnicity'], // Ethnicity
     ['NDX3','3','neighbourhood:characteristic:3','historical'], // Historical
     ['NDX3','4','neighbourhood:characteristic:3','linguistics'], // Linguistics
     ['NDX3','5','neighbourhood:characteristic:3','physical_structure'], // Physical Structure
     ['NDX3','6','neighbourhood:characteristic:3','religion'], // Religion
     ['NDX3','7','neighbourhood:characteristic:3','socioeconomics'], // Socioeconomics
     ['NDX3','999','neighbourhood:characteristic:3','other'], // Other

     // NFP - Naval Firing and/or Practice Type
     // ['NFP','-999999',undefined,undefined], // No Information
     ['NFP','4','naval:practice_type','firing_danger_area'], // Firing Danger Area
     ['NFP','6','naval:practice_type','mine_laying_practice_area'], // Mine Laying Practice Area
     ['NFP','999','naval:practice_type','other'], // Other

     // NFP2 - Naval Firing and/or Practice Type [2]
     // ['NFP2','-999999',undefined,undefined], // No Information
     ['NFP2','4','naval:practice_type:2','firing_danger_area'], // Firing Danger Area
     ['NFP2','6','naval:practice_type:2','mine_laying_practice_area'], // Mine Laying Practice Area
     ['NFP2','999','naval:practice_type:2','other'], // Other

     // NFP3 - Naval Firing and/or Practice Type [3]
     // ['NFP3','-999999',undefined,undefined], // No Information
     ['NFP3','4','naval:practice_type:3','firing_danger_area'], // Firing Danger Area
     ['NFP3','6','naval:practice_type:3','mine_laying_practice_area'], // Mine Laying Practice Area
     ['NFP3','999','naval:practice_type:3','other'], // Other

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
     // ['NMC2','-999999',undefined,undefined], // No Information
     ['NMC2','1','seamark:navigation_mark:colour:2','white'], // White
     ['NMC2','2','seamark:navigation_mark:colour:2','black'], // Black
     ['NMC2','3','seamark:navigation_mark:colour:2','red'], // Red
     ['NMC2','4','seamark:navigation_mark:colour:2','green'], // Green
     ['NMC2','5','seamark:navigation_mark:colour:2','blue'], // Blue
     ['NMC2','6','seamark:navigation_mark:colour:2','yellow'], // Yellow
     ['NMC2','7','seamark:navigation_mark:colour:2','grey'], // Grey
     ['NMC2','8','seamark:navigation_mark:colour:2','brown'], // Brown
     ['NMC2','9','seamark:navigation_mark:colour:2','amber'], // Amber
     ['NMC2','10','seamark:navigation_mark:colour:2','violet'], // Violet
     ['NMC2','11','seamark:navigation_mark:colour:2','orange'], // Orange
     ['NMC2','12','seamark:navigation_mark:colour:2','magenta'], // Magenta
     ['NMC2','13','seamark:navigation_mark:colour:2','pink'], // Pink
     ['NMC2','999','seamark:navigation_mark:colour:2','other'], // Other

     // NMC3 - Navigation Mark Colour [3]
     // ['NMC3','-999999',undefined,undefined], // No Information
     ['NMC3','1','seamark:navigation_mark:colour:3','white'], // White
     ['NMC3','2','seamark:navigation_mark:colour:3','black'], // Black
     ['NMC3','3','seamark:navigation_mark:colour:3','red'], // Red
     ['NMC3','4','seamark:navigation_mark:colour:3','green'], // Green
     ['NMC3','5','seamark:navigation_mark:colour:3','blue'], // Blue
     ['NMC3','6','seamark:navigation_mark:colour:3','yellow'], // Yellow
     ['NMC3','7','seamark:navigation_mark:colour:3','grey'], // Grey
     ['NMC3','8','seamark:navigation_mark:colour:3','brown'], // Brown
     ['NMC3','9','seamark:navigation_mark:colour:3','amber'], // Amber
     ['NMC3','10','seamark:navigation_mark:colour:3','violet'], // Violet
     ['NMC3','11','seamark:navigation_mark:colour:3','orange'], // Orange
     ['NMC3','12','seamark:navigation_mark:colour:3','magenta'], // Magenta
     ['NMC3','13','seamark:navigation_mark:colour:3','pink'], // Pink
     ['NMC3','999','seamark:navigation_mark:colour:3','other'], // Other

     // NOA - Naval Operations Type
     // ['NOA','-999999',undefined,undefined], // No Information
     ['NOA','14','naval:operation_type','submarine_exercise_area'], // Submarine Exercise Area
     ['NOA','27','naval:operation_type','foracs_v_limit'], // FORACS V Limit
     ['NOA','999','naval:operation_type','other'], // Other

     // NOA2 - Naval Operations Type [2]
     // ['NOA2','-999999',undefined,undefined], // No Information
     ['NOA2','14','naval:operation_type:2','submarine_exercise_area'], // Submarine Exercise Area
     ['NOA2','27','naval:operation_type:2','foracs_v_limit'], // FORACS V Limit
     ['NOA2','999','naval:operation_type:2','other'], // Other

     // NOA3 - Naval Operations Type [3]
     // ['NOA3','-999999',undefined,undefined], // No Information
     ['NOA3','14','naval:operation_type:3','submarine_exercise_area'], // Submarine Exercise Area
     ['NOA3','27','naval:operation_type:3','foracs_v_limit'], // FORACS V Limit
     ['NOA3','999','naval:operation_type:3','other'], // Other

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
     // ['ORS2','-999999',undefined,undefined], // No Information
     ['ORS2','3','seamark:precautionary_area:operating_restriction:2','no_restriction'], // No Restriction
     ['ORS2','4','seamark:precautionary_area:operating_restriction:2','snow'], // Snow
     ['ORS2','5','seamark:precautionary_area:operating_restriction:2','icing'], // Icing
     ['ORS2','6','seamark:precautionary_area:operating_restriction:2','flooding'], // Flooding
     ['ORS2','7','seamark:precautionary_area:operating_restriction:2','special_restriction'], // Special Restriction
     ['ORS2','8','seamark:precautionary_area:operating_restriction:2','wind'], // Wind
     ['ORS2','9','seamark:precautionary_area:operating_restriction:2','reduced_visibility'], // Reduced Visibility
     ['ORS2','10','seamark:precautionary_area:operating_restriction:2','time'], // Time
     ['ORS2','999','seamark:precautionary_area:operating_restriction:2','other'], // Other

     // ORS3 - Operating Restriction [3]
     // ['ORS3','-999999',undefined,undefined], // No Information
     ['ORS3','3','seamark:precautionary_area:operating_restriction:3','no_restriction'], // No Restriction
     ['ORS3','4','seamark:precautionary_area:operating_restriction:3','snow'], // Snow
     ['ORS3','5','seamark:precautionary_area:operating_restriction:3','icing'], // Icing
     ['ORS3','6','seamark:precautionary_area:operating_restriction:3','flooding'], // Flooding
     ['ORS3','7','seamark:precautionary_area:operating_restriction:3','special_restriction'], // Special Restriction
     ['ORS3','8','seamark:precautionary_area:operating_restriction:3','wind'], // Wind
     ['ORS3','9','seamark:precautionary_area:operating_restriction:3','reduced_visibility'], // Reduced Visibility
     ['ORS3','10','seamark:precautionary_area:operating_restriction:3','time'], // Time
     ['ORS3','999','seamark:precautionary_area:operating_restriction:3','other'], // Other

     // OWO - Waterbody Overhead Obstruction
     // ['OWO','-999999',undefined,undefined], // No Information
     ['OWO','1000','waterway:overhead_obstruction','no'],
     ['OWO','1001','waterway:overhead_obstruction','yes'],

     // PBY - By-product
     // ['PBY','-999999',undefined,undefined], // No Information - See ZI014_PBY
//      ['PBY','8','raw:PBY','refuse'], // Refuse
//      ['PBY','15','raw:PBY','slag'], // Slag
//      ['PBY','16','raw:PBY','sludge'], // Sludge
//      ['PBY','17','raw:PBY','spoil'], // Spoil
//      ['PBY','999','raw:PBY','other'], // Other

     // PBY2 - By-product [2] - See ZI014_PBY2
     // ['PBY2','-999999',undefined,undefined], // No Information
//      ['PBY2','8','raw:PBY2','refuse'], // Refuse
//      ['PBY2','15','raw:PBY2','slag'], // Slag
//      ['PBY2','16','raw:PBY2','sludge'], // Sludge
//      ['PBY2','17','raw:PBY2','spoil'], // Spoil
//      ['PBY2','999','raw:PBY2','other'], // Other

     // PBY3 - By-product [3] - See ZI014_PBY3
     // ['PBY3','-999999',undefined,undefined], // No Information
//      ['PBY3','8','raw:PBY3','refuse'], // Refuse
//      ['PBY3','15','raw:PBY3','slag'], // Slag
//      ['PBY3','16','raw:PBY3','sludge'], // Sludge
//      ['PBY3','17','raw:PBY3','spoil'], // Spoil
//      ['PBY3','999','raw:PBY3','other'], // Other

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
     // ['PLT2','-999999',undefined,undefined], // No Information
     ['PLT2','1','pipeline:type:2','transport_pipe'], // Transport Pipe
     ['PLT2','2','pipeline:type:2','outfall_pipe'], // Outfall Pipe
     ['PLT2','3','pipeline:type:2','intake_pipe'], // Intake Pipe
     ['PLT2','4','pipeline:type:2','sewer'], // Sewer
     ['PLT2','5','pipeline:type:2','pipeline_valve'], // Pipeline Valve
     ['PLT2','7','pipeline:type:2','bubbler_system'], // Bubbler System
     ['PLT2','999','pipeline:type:2','other'], // Other

     // PLT3 - Pipeline Type [3]
     // ['PLT3','-999999',undefined,undefined], // No Information
     ['PLT3','1','pipeline:type:3','transport_pipe'], // Transport Pipe
     ['PLT3','2','pipeline:type:3','outfall_pipe'], // Outfall Pipe
     ['PLT3','3','pipeline:type:3','intake_pipe'], // Intake Pipe
     ['PLT3','4','pipeline:type:3','sewer'], // Sewer
     ['PLT3','5','pipeline:type:3','pipeline_valve'], // Pipeline Valve
     ['PLT3','7','pipeline:type:3','bubbler_system'], // Bubbler System
     ['PLT3','999','pipeline:type:3','other'], // Other

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
     // ['POS2','-999999',undefined,undefined], // No Information
     ['POS2','1','generator:source:2','geothermal'], // Geothermal
     ['POS2','2','generator:source:2','hydro-electric'], // Hydro-electric
     ['POS2','3','generator:source:2','nuclear'], // Nuclear
     ['POS2','4','generator:source:2','thermal'], // Thermal
     ['POS2','5','generator:source:2','tidal'], // Tidal
     ['POS2','999','generator:source:2','other'], // Other

     // POS3 - Power Source [3]
     // ['POS3','-999999',undefined,undefined], // No Information
     ['POS3','1','generator:source:3','geothermal'], // Geothermal
     ['POS3','2','generator:source:3','hydro-electric'], // Hydro-electric
     ['POS3','3','generator:source:3','nuclear'], // Nuclear
     ['POS3','4','generator:source:3','thermal'], // Thermal
     ['POS3','5','generator:source:3','tidal'], // Tidal
     ['POS3','999','generator:source:3','other'], // Other

     // PPO - Physical Product - See ZI014_PPO
     // ['PPO','-999999',undefined,undefined], // No Information
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
//      ['PPO','49','raw:PPO','grain'], // Grain
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
//      ['PPO','70','raw:PPO','milk'], // Milk
//      ['PPO','72','raw:PPO','mussels'], // Mussels
//      ['PPO','77','raw:PPO','oysters'], // Oysters
//      ['PPO','83','raw:PPO','petroleum'], // Petroleum
//      ['PPO','85','raw:PPO','porphyry'], // Porphyry
//      ['PPO','88','raw:PPO','pumice'], // Pumice
//      ['PPO','89','raw:PPO','quartz'], // Quartz
//      ['PPO','90','raw:PPO','radioactive_material'], // Radioactive Material
//      ['PPO','93','raw:PPO','rocks'], // Rocks
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
//      ['PPO','123','raw:PPO','wine'], // Wine
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

     // PPO2 - Physical Product [2] - See ZI014_PPO2
     // ['PPO2','-999999',undefined,undefined], // No Information
//      ['PPO2','1','raw:PPO2','aircraft'], // Aircraft
//      ['PPO2','3','raw:PPO2','ammunition'], // Ammunition
//      ['PPO2','8','raw:PPO2','basalt'], // Basalt
//      ['PPO2','9','raw:PPO2','bauxite'], // Bauxite
//      ['PPO2','11','raw:PPO2','bivalve_mollusc'], // Bivalve Mollusc
//      ['PPO2','13','raw:PPO2','brick'], // Brick
//      ['PPO2','15','raw:PPO2','cement'], // Cement
//      ['PPO2','16','raw:PPO2','chemical'], // Chemical
//      ['PPO2','17','raw:PPO2','clay'], // Clay
//      ['PPO2','18','raw:PPO2','coal'], // Coal
//      ['PPO2','19','raw:PPO2','cobbles'], // Cobbles
//      ['PPO2','21','raw:PPO2','coke'], // Coke
//      ['PPO2','26','raw:PPO2','copper'], // Copper
//      ['PPO2','29','raw:PPO2','crustacean'], // Crustacean
//      ['PPO2','30','raw:PPO2','cultivated_shellfish'], // Cultivated Shellfish
//      ['PPO2','33','raw:PPO2','diamond'], // Diamond
//      ['PPO2','34','raw:PPO2','diatomaceous_earth'], // Diatomaceous Earth
//      ['PPO2','35','raw:PPO2','dolomite'], // Dolomite
//      ['PPO2','37','raw:PPO2','electric_power'], // Electric Power
//      ['PPO2','38','raw:PPO2','explosive'], // Explosive
//      ['PPO2','39','raw:PPO2','fish'], // Fish
//      ['PPO2','45','raw:PPO2','gas'], // Gas
//      ['PPO2','46','raw:PPO2','petrol'], // Petrol
//      ['PPO2','48','raw:PPO2','gold'], // Gold
//      ['PPO2','49','raw:PPO2','grain'], // Grain
//      ['PPO2','50','raw:PPO2','granite'], // Granite
//      ['PPO2','53','raw:PPO2','gravel'], // Gravel
//      ['PPO2','58','raw:PPO2','iron'], // Iron
//      ['PPO2','59','raw:PPO2','lead'], // Lead
//      ['PPO2','60','raw:PPO2','lime'], // Lime
//      ['PPO2','61','raw:PPO2','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
//      ['PPO2','62','raw:PPO2','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
//      ['PPO2','63','raw:PPO2','lumber'], // Lumber
//      ['PPO2','65','raw:PPO2','manganese'], // Manganese
//      ['PPO2','66','raw:PPO2','marble'], // Marble
//      ['PPO2','70','raw:PPO2','milk'], // Milk
//      ['PPO2','72','raw:PPO2','mussels'], // Mussels
//      ['PPO2','77','raw:PPO2','oysters'], // Oysters
//      ['PPO2','83','raw:PPO2','petroleum'], // Petroleum
//      ['PPO2','85','raw:PPO2','porphyry'], // Porphyry
//      ['PPO2','88','raw:PPO2','pumice'], // Pumice
//      ['PPO2','89','raw:PPO2','quartz'], // Quartz
//      ['PPO2','90','raw:PPO2','radioactive_material'], // Radioactive Material
//      ['PPO2','93','raw:PPO2','rocks'], // Rocks
//      ['PPO2','95','raw:PPO2','salt'], // Salt
//      ['PPO2','96','raw:PPO2','sand'], // Sand
//      ['PPO2','97','raw:PPO2','sandstone'], // Sandstone
//      ['PPO2','101','raw:PPO2','sewage'], // Sewage
//      ['PPO2','105','raw:PPO2','silver'], // Silver
//      ['PPO2','110','raw:PPO2','stone'], // Stone
//      ['PPO2','116','raw:PPO2','timber'], // Timber
//      ['PPO2','117','raw:PPO2','tobacco'], // Tobacco
//      ['PPO2','118','raw:PPO2','travertine'], // Travertine
//      ['PPO2','120','raw:PPO2','uranium'], // Uranium
//      ['PPO2','122','raw:PPO2','water'], // Water
//      ['PPO2','123','raw:PPO2','wine'], // Wine
//      ['PPO2','126','raw:PPO2','zinc'], // Zinc
//      ['PPO2','130','raw:PPO2','chalk'], // Chalk
//      ['PPO2','146','raw:PPO2','heating_steam_and/or_water'], // Heating Steam and/or Water
//      ['PPO2','149','raw:PPO2','brine'], // Brine
//      ['PPO2','151','raw:PPO2','chromium'], // Chromium
//      ['PPO2','152','raw:PPO2','nickel'], // Nickel
//      ['PPO2','153','raw:PPO2','tin'], // Tin
//      ['PPO2','158','raw:PPO2','pottery'], // Pottery
//      ['PPO2','159','raw:PPO2','charcoal'], // Charcoal
//      ['PPO2','160','raw:PPO2','milled_grain'], // Milled Grain
//      ['PPO2','161','raw:PPO2','coalbed_methane'], // Coalbed Methane
//      ['PPO2','162','raw:PPO2','natural_gas_condensate'], // Natural Gas Condensate
//      ['PPO2','163','raw:PPO2','helium'], // Helium
//      ['PPO2','164','raw:PPO2','hydrothermal_fluid'], // Hydrothermal Fluid
//      ['PPO2','170','raw:PPO2','beverage'], // Beverage
//      ['PPO2','173','raw:PPO2','fluorite'], // Fluorite
//      ['PPO2','204','raw:PPO2','phosphate'], // Phosphate
//      ['PPO2','214','raw:PPO2','biodiesel'], // Biodiesel
//      ['PPO2','224','raw:PPO2','vanadium'], // Vanadium
//      ['PPO2','283','raw:PPO2','mica'], // Mica
//      ['PPO2','339','raw:PPO2','selenium'], // Selenium
//      ['PPO2','999','raw:PPO2','other'], // Other

     // PPO3 - Physical Product [3] - See ZI014_PPO3
     // ['PPO3','-999999',undefined,undefined], // No Information
//      ['PPO3','1','raw:PPO3','aircraft'], // Aircraft
//      ['PPO3','3','raw:PPO3','ammunition'], // Ammunition
//      ['PPO3','8','raw:PPO3','basalt'], // Basalt
//      ['PPO3','9','raw:PPO3','bauxite'], // Bauxite
//      ['PPO3','11','raw:PPO3','bivalve_mollusc'], // Bivalve Mollusc
//      ['PPO3','13','raw:PPO3','brick'], // Brick
//      ['PPO3','15','raw:PPO3','cement'], // Cement
//      ['PPO3','16','raw:PPO3','chemical'], // Chemical
//      ['PPO3','17','raw:PPO3','clay'], // Clay
//      ['PPO3','18','raw:PPO3','coal'], // Coal
//      ['PPO3','19','raw:PPO3','cobbles'], // Cobbles
//      ['PPO3','21','raw:PPO3','coke'], // Coke
//      ['PPO3','26','raw:PPO3','copper'], // Copper
//      ['PPO3','29','raw:PPO3','crustacean'], // Crustacean
//      ['PPO3','30','raw:PPO3','cultivated_shellfish'], // Cultivated Shellfish
//      ['PPO3','33','raw:PPO3','diamond'], // Diamond
//      ['PPO3','34','raw:PPO3','diatomaceous_earth'], // Diatomaceous Earth
//      ['PPO3','35','raw:PPO3','dolomite'], // Dolomite
//      ['PPO3','37','raw:PPO3','electric_power'], // Electric Power
//      ['PPO3','38','raw:PPO3','explosive'], // Explosive
//      ['PPO3','39','raw:PPO3','fish'], // Fish
//      ['PPO3','45','raw:PPO3','gas'], // Gas
//      ['PPO3','46','raw:PPO3','petrol'], // Petrol
//      ['PPO3','48','raw:PPO3','gold'], // Gold
//      ['PPO3','49','raw:PPO3','grain'], // Grain
//      ['PPO3','50','raw:PPO3','granite'], // Granite
//      ['PPO3','53','raw:PPO3','gravel'], // Gravel
//      ['PPO3','58','raw:PPO3','iron'], // Iron
//      ['PPO3','59','raw:PPO3','lead'], // Lead
//      ['PPO3','60','raw:PPO3','lime'], // Lime
//      ['PPO3','61','raw:PPO3','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
//      ['PPO3','62','raw:PPO3','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
//      ['PPO3','63','raw:PPO3','lumber'], // Lumber
//      ['PPO3','65','raw:PPO3','manganese'], // Manganese
//      ['PPO3','66','raw:PPO3','marble'], // Marble
//      ['PPO3','70','raw:PPO3','milk'], // Milk
//      ['PPO3','72','raw:PPO3','mussels'], // Mussels
//      ['PPO3','77','raw:PPO3','oysters'], // Oysters
//      ['PPO3','83','raw:PPO3','petroleum'], // Petroleum
//      ['PPO3','85','raw:PPO3','porphyry'], // Porphyry
//      ['PPO3','88','raw:PPO3','pumice'], // Pumice
//      ['PPO3','89','raw:PPO3','quartz'], // Quartz
//      ['PPO3','90','raw:PPO3','radioactive_material'], // Radioactive Material
//      ['PPO3','93','raw:PPO3','rocks'], // Rocks
//      ['PPO3','95','raw:PPO3','salt'], // Salt
//      ['PPO3','96','raw:PPO3','sand'], // Sand
//      ['PPO3','97','raw:PPO3','sandstone'], // Sandstone
//      ['PPO3','101','raw:PPO3','sewage'], // Sewage
//      ['PPO3','105','raw:PPO3','silver'], // Silver
//      ['PPO3','110','raw:PPO3','stone'], // Stone
//      ['PPO3','116','raw:PPO3','timber'], // Timber
//      ['PPO3','117','raw:PPO3','tobacco'], // Tobacco
//      ['PPO3','118','raw:PPO3','travertine'], // Travertine
//      ['PPO3','120','raw:PPO3','uranium'], // Uranium
//      ['PPO3','122','raw:PPO3','water'], // Water
//      ['PPO3','123','raw:PPO3','wine'], // Wine
//      ['PPO3','126','raw:PPO3','zinc'], // Zinc
//      ['PPO3','130','raw:PPO3','chalk'], // Chalk
//      ['PPO3','146','raw:PPO3','heating_steam_and/or_water'], // Heating Steam and/or Water
//      ['PPO3','149','raw:PPO3','brine'], // Brine
//      ['PPO3','151','raw:PPO3','chromium'], // Chromium
//      ['PPO3','152','raw:PPO3','nickel'], // Nickel
//      ['PPO3','153','raw:PPO3','tin'], // Tin
//      ['PPO3','158','raw:PPO3','pottery'], // Pottery
//      ['PPO3','159','raw:PPO3','charcoal'], // Charcoal
//      ['PPO3','160','raw:PPO3','milled_grain'], // Milled Grain
//      ['PPO3','161','raw:PPO3','coalbed_methane'], // Coalbed Methane
//      ['PPO3','162','raw:PPO3','natural_gas_condensate'], // Natural Gas Condensate
//      ['PPO3','163','raw:PPO3','helium'], // Helium
//      ['PPO3','164','raw:PPO3','hydrothermal_fluid'], // Hydrothermal Fluid
//      ['PPO3','170','raw:PPO3','beverage'], // Beverage
//      ['PPO3','173','raw:PPO3','fluorite'], // Fluorite
//      ['PPO3','204','raw:PPO3','phosphate'], // Phosphate
//      ['PPO3','214','raw:PPO3','biodiesel'], // Biodiesel
//      ['PPO3','224','raw:PPO3','vanadium'], // Vanadium
//      ['PPO3','283','raw:PPO3','mica'], // Mica
//      ['PPO3','339','raw:PPO3','selenium'], // Selenium
//      ['PPO3','999','raw:PPO3','other'], // Other

     // PRM - Permanent
     // ['PRM','-999999',undefined,undefined], // No Information
     ['PRM','1000','permanent','no'],
     ['PRM','1001','permanent','yes'],

     // PRW - Raw Material - See ZI014_PRW
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

     // PRW2 - Raw Material [2] - See ZI014_PRW
     // ['PRW2','-999999',undefined,undefined], // No Information
//      ['PRW2','12','raw:PRW2','coal'], // Coal
//      ['PRW2','14','raw:PRW2','coke'], // Coke
//      ['PRW2','34','raw:PRW2','gravel'], // Gravel
//      ['PRW2','54','raw:PRW2','ore'], // Ore
//      ['PRW2','67','raw:PRW2','salt'], // Salt
//      ['PRW2','68','raw:PRW2','sand'], // Sand
//      ['PRW2','84','raw:PRW2','stone'], // Stone
//      ['PRW2','154','raw:PRW2','sulphur'], // Sulphur
//      ['PRW2','999','raw:PRW2','other'], // Other

     // PRW3 - Raw Material [3] - See ZI014_PRW
     // ['PRW3','-999999',undefined,undefined], // No Information
//      ['PRW3','12','raw:PRW3','coal'], // Coal
//      ['PRW3','14','raw:PRW3','coke'], // Coke
//      ['PRW3','34','raw:PRW3','gravel'], // Gravel
//      ['PRW3','54','raw:PRW3','ore'], // Ore
//      ['PRW3','67','raw:PRW3','salt'], // Salt
//      ['PRW3','68','raw:PRW3','sand'], // Sand
//      ['PRW3','84','raw:PRW3','stone'], // Stone
//      ['PRW3','154','raw:PRW3','sulphur'], // Sulphur
//      ['PRW3','999','raw:PRW3','other'], // Other

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
     // ['RCG','-999999',undefined,undefined], // No Information
//      ['RCG','1','raw:RCG','u.s._army'], // U.S. Army
//      ['RCG','2','raw:RCG','u.s._navy'], // U.S. Navy
//      ['RCG','3','raw:RCG','u.s._air_force'], // U.S. Air Force
//      ['RCG','4','raw:RCG','u.s._marine_corps'], // U.S. Marine Corps
//      ['RCG','5','raw:RCG','u.s._coast_guard'], // U.S. Coast Guard
//      ['RCG','6','raw:RCG','u.s._africa_command_(usafricom)'], // U.S. Africa Command (USAFRICOM)
//      ['RCG','7','raw:RCG','u.s._central_command_(uscentcom)'], // U.S. Central Command (USCENTCOM)
//      ['RCG','8','raw:RCG','u.s._european_command_(useucom)'], // U.S. European Command (USEUCOM)
//      ['RCG','9','raw:RCG','u.s._joint_forces_command_(usjfcom)'], // U.S. Joint Forces Command (USJFCOM)
//      ['RCG','10','raw:RCG','u.s._northern_command_(usnorthcom)'], // U.S. Northern Command (USNORTHCOM)
//      ['RCG','11','raw:RCG','u.s._pacific_command_(pacom)'], // U.S. Pacific Command (PACOM)
//      ['RCG','12','raw:RCG','u.s._special_operations_command_(ussocom)'], // U.S. Special Operations Command (USSOCOM)
//      ['RCG','13','raw:RCG','u.s._southern_command_(ussouthcom)'], // U.S. Southern Command (USSOUTHCOM)
//      ['RCG','14','raw:RCG','u.s._strategic_command_(usstratcom)'], // U.S. Strategic Command (USSTRATCOM)
//      ['RCG','15','raw:RCG','u.s._transportation_command_(ustranscom)'], // U.S. Transportation Command (USTRANSCOM)
//      ['RCG','16','raw:RCG','u.s._central_intelligence_agency_(cia)'], // U.S. Central Intelligence Agency (CIA)
//      ['RCG','17','raw:RCG','u.s._defense_intelligence_agency_(dia)'], // U.S. Defense Intelligence Agency (DIA)
//      ['RCG','18','raw:RCG','u.s._national_security_agency_(nsa)'], // U.S. National Security Agency (NSA)
//      ['RCG','19','raw:RCG','u.s._national_geospatial-intelligence_agency_(nga)'], // U.S. National Geospatial-Intelligence Agency (NGA)
//      ['RCG','20','raw:RCG','u.s._national_reconnaissance_office_(nro)'], // U.S. National Reconnaissance Office (NRO)
//      ['RCG','21','raw:RCG','u.s._department_of_state'], // U.S. Department of State
//      ['RCG','22','raw:RCG','u.s._department_of_homeland_security_(dhs)'], // U.S. Department of Homeland Security (DHS)
//      ['RCG','23','raw:RCG','u.s._department_of_energy_(doe)'], // U.S. Department of Energy (DOE)
//      ['RCG','24','raw:RCG','u.s._federal_bureau_of_investigation_(fbi)'], // U.S. Federal Bureau of Investigation (FBI)
//      ['RCG','25','raw:RCG','u.s._geological_survey_(usgs)'], // U.S. Geological Survey (USGS)
//      ['RCG','26','raw:RCG','u.s._national_civil_applications_program_(ncap)'], // U.S. National Civil Applications Program (NCAP)
//      ['RCG','27','raw:RCG','u.s._national_oceanic_and_atmospheric_administration'], // U.S. National Oceanic and Atmospheric Administration
//      ['RCG','28','raw:RCG','defence_imagery_and_geospatial_organisation_(australia)'], // Defence Imagery and Geospatial Organisation (Australia)
//      ['RCG','29','raw:RCG','geographic_service_(belgium)'], // Geographic Service (Belgium)
//      ['RCG','30','raw:RCG','military_topographic_service_(bulgaria)'], // Military Topographic Service (Bulgaria)
//      ['RCG','31','raw:RCG','mapping_and_charting_establishment_(canada)'], // Mapping and Charting Establishment (Canada)
//      ['RCG','32','raw:RCG','geographic_service_of_the_czech_armed_forces_(czech_republic)'], // Geographic Service of the Czech Armed Forces (Czech Republic)
//      ['RCG','33','raw:RCG','defence_acquisition_and_logistics_organization_(denmark)'], // Defence Acquisition and Logistics Organization (Denmark)
//      ['RCG','34','raw:RCG','military_geographic_group_(estonia)'], // Military Geographic Group (Estonia)
//      ['RCG','35','raw:RCG','topographic_service_(finland)'], // Topographic Service (Finland)
//      ['RCG','36','raw:RCG','joint_geography_bureau_(france)'], // Joint Geography Bureau (France)
//      ['RCG','37','raw:RCG','bundeswehr_geoinformation_office_(germany)'], // Bundeswehr Geoinformation Office (Germany)
//      ['RCG','38','raw:RCG','hellenic_military_geographic_service_(greece)'], // Hellenic Military Geographic Service (Greece)
//      ['RCG','39','raw:RCG','mapping_service_(hungary)'], // Mapping Service (Hungary)
//      ['RCG','40','raw:RCG','defense_information_security_(italy)'], // Defense Information Security (Italy)
//      ['RCG','41','raw:RCG','geospatial_information_agency_(latvia)'], // Geospatial Information Agency (Latvia)
//      ['RCG','42','raw:RCG','military_mapping_centre_(lithuania)'], // Military Mapping Centre (Lithuania)
//      ['RCG','43','raw:RCG','national_army_topographic_service_(moldova)'], // National Army Topographic Service (Moldova)
//      ['RCG','44','raw:RCG','army_geographic_agency_(netherlands)'], // Army Geographic Agency (Netherlands)
//      ['RCG','45','raw:RCG','geospatial_intelligence_organisation_(new_zealand)'], // Geospatial Intelligence Organisation (New Zealand)
//      ['RCG','46','raw:RCG','military_geographic_service_(norway)'], // Military Geographic Service (Norway)
//      ['RCG','47','raw:RCG','military_geography_division_(poland)'], // Military Geography Division (Poland)
//      ['RCG','48','raw:RCG','army_geographic_institute_(portugal)'], // Army Geographic Institute (Portugal)
//      ['RCG','49','raw:RCG','military_topographic_directorate_(romania)'], // Military Topographic Directorate (Romania)
//      ['RCG','50','raw:RCG','topographic_institute_(slovakia)'], // Topographic Institute (Slovakia)
//      ['RCG','51','raw:RCG','army_geographic_centre_(spain)'], // Army Geographic Centre (Spain)
//      ['RCG','52','raw:RCG','swedish_armed_forces_(sweden)'], // Swedish Armed Forces (Sweden)
//      ['RCG','53','raw:RCG','general_command_of_mapping_(turkey)'], // General Command of Mapping (Turkey)
//      ['RCG','54','raw:RCG','defence_geographic_centre_intelligence_collection_group_(united_kingdom)'], // Defence Geographic Centre Intelligence Collection Group (United Kingdom)
//      ['RCG','55','raw:RCG','u.s._army_geospatial_center_(agc)'], // U.S. Army Geospatial Center (AGC)
//      ['RCG','56','raw:RCG','army_(australia)'], // Army (Australia)
//      ['RCG','998','raw:RCG','not_applicable'], // Not Applicable
//      ['RCG','999','raw:RCG','other'], // Other

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
     // ['RIN_ROI2','-999999',undefined,undefined], // No Information
     ['RIN_ROI2','1','ref:road:class:2','international'], // International
     ['RIN_ROI2','2','ref:road:class:2','national_motorway'], // National Motorway
     ['RIN_ROI2','3','ref:road:class:2','national'], // National
     ['RIN_ROI2','4','ref:road:class:2','secondary'], // Secondary
     ['RIN_ROI2','5','ref:road:class:2','local'], // Local
     ['RIN_ROI2','999','ref:road:class:2','other'], // Other

     // RIN_ROI3 - Route Identification [3] <route designation type>
     // ['RIN_ROI3','-999999',undefined,undefined], // No Information
     ['RIN_ROI3','1','ref:road:class:3','international'], // International
     ['RIN_ROI3','2','ref:road:class:3','national_motorway'], // National Motorway
     ['RIN_ROI3','3','ref:road:class:3','national'], // National
     ['RIN_ROI3','4','ref:road:class:3','secondary'], // Secondary
     ['RIN_ROI3','5','ref:road:class:3','local'], // Local
     ['RIN_ROI3','999','ref:road:class:3','other'], // Other

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
     ['RLE','998',undefined,undefined], // Not Applicable

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
     // ['RRC2','-999999',undefined,undefined], // No Information
     ['RRC2','2','railway:2','carline'], // Carline
     ['RRC2','6','railway:2','subway'], // Underground Railway
     ['RRC2','8','railway:2','logging'], // Logging
     ['RRC2','11','railway:2','rapid_transit'], // Rail Rapid Transit
     ['RRC2','13','railway:2','marine_railway'], // Marine Railway
     ['RRC2','14','railway:2','tram'], // Tramway
     ['RRC2','15','railway:2','funicular'], // Funicular
     ['RRC2','24','railway:2','museum'], // Museum
     ['RRC2','32','railway:2','automated_transit_system'], // Automated Transit System
     ['RRC2','33','railway:2','longhaul'], // Long-haul
     ['RRC2','999','railway:2','other'], // Other

     // RRC3 - Railway Use [3]
     // ['RRC3','-999999',undefined,undefined], // No Information
     ['RRC3','2','railway:3','carline'], // Carline
     ['RRC3','6','railway:3','subway'], // Underground Railway
     ['RRC3','8','railway:3','logging'], // Logging
     ['RRC3','11','railway:3','rapid_transit'], // Rail Rapid Transit
     ['RRC3','13','railway:3','marine_railway'], // Marine Railway
     ['RRC3','14','railway:3','tram'], // Tramway
     ['RRC3','15','railway:3','funicular'], // Funicular
     ['RRC3','24','railway:3','museum'], // Museum
     ['RRC3','32','railway:3','automated_transit_system'], // Automated Transit System
     ['RRC3','33','railway:3','longhaul'], // Long-haul
     ['RRC3','999','railway:3','other'], // Other

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
     // ['SBT2','-999999',undefined,undefined], // No Information
     ['SBT2','1','sunstation:2','switched_substation'], // Switched Substation
     ['SBT2','2','sunstation:2','transformer_substation'], // Transformer Substation
     ['SBT2','3','sunstation:2','converter_substation'], // Converter Substation
     ['SBT2','999','sunstation:2','other'], // Other

     // SBT3 - Substation Type [3]
     // ['SBT3','-999999',undefined,undefined], // No Information
     ['SBT3','1','sunstation:3','switched_substation'], // Switched Substation
     ['SBT3','2','sunstation:3','transformer_substation'], // Transformer Substation
     ['SBT3','3','sunstation:3','converter_substation'], // Converter Substation
     ['SBT3','999','sunstation:3','other'], // Other

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
     // ['SLT','998','shoreline:type','not_applicable'], // Not Applicable
     ['SLT','998',undefined,undefined], // Not Applicable
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
     // ['SSR2','-999999',undefined,undefined], // No Information
     ['SSR2','6','roof:shape:2','conical'], // Conical
     ['SSR2','7','roof:shape:2','pyramidal'], // Pyramidal
     ['SSR2','38','roof:shape:2','semi-cylindrical'], // Semi-cylindrical
     ['SSR2','40','roof:shape:2','domed'], // Domed
     ['SSR2','41','roof:shape:2','flat'], // Flat
     ['SSR2','42','roof:shape:2','pitched'], // Pitched
     ['SSR2','47','roof:shape:2','sawtoothed'], // Sawtoothed
     ['SSR2','50','roof:shape:2','with_clerestory'], // With Clerestory
     ['SSR2','55','roof:shape:2','flat_with_clerestory'], // Flat with Clerestory
     ['SSR2','64','roof:shape:2','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR2','999','roof:shape:2','other'], // Other

     // SSR3 - Roof Shape [3]
     // ['SSR3','-999999',undefined,undefined], // No Information
     ['SSR3','6','roof:shape:3','conical'], // Conical
     ['SSR3','7','roof:shape:3','pyramidal'], // Pyramidal
     ['SSR3','38','roof:shape:3','semi-cylindrical'], // Semi-cylindrical
     ['SSR3','40','roof:shape:3','domed'], // Domed
     ['SSR3','41','roof:shape:3','flat'], // Flat
     ['SSR3','42','roof:shape:3','pitched'], // Pitched
     ['SSR3','47','roof:shape:3','sawtoothed'], // Sawtoothed
     ['SSR3','50','roof:shape:3','with_clerestory'], // With Clerestory
     ['SSR3','55','roof:shape:3','flat_with_clerestory'], // Flat with Clerestory
     ['SSR3','64','roof:shape:3','pitched_with_clerestory'], // Pitched with Clerestory
     ['SSR3','999','roof:shape:3','other'], // Other

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
     // ['STA2','-999999',undefined,undefined], // No Information
     ['STA2','1','seamark:signal_station:2','coast_guard_station'], // Coast Guard Station
     ['STA2','2','seamark:signal_station:2','fireboat_station'], // Fireboat Station
     ['STA2','3','seamark:signal_station:2','water-police_station'], // Water-police Station
     ['STA2','4','seamark:signal_station:2','ice_signal_station'], // Ice Signal Station
     ['STA2','5','seamark:signal_station:2','rescue_station'], // Rescue Station
     ['STA2','6','seamark:signal_station:2','port_control_station'], // Port Control Station
     ['STA2','11','seamark:signal_station:2','pilot_station'], // Pilot Station
     ['STA2','13','seamark:signal_station:2','signal_station'], // Signal Station
     ['STA2','14','seamark:signal_station:2','signal_mast'], // Signal Mast
     ['STA2','15','seamark:signal_station:2','storm_signal_station'], // Storm Signal Station
     ['STA2','17','seamark:signal_station:2','tide_station'], // Tide Station
     ['STA2','18','seamark:signal_station:2','time_ball_station'], // Time Ball Station
     ['STA2','19','seamark:signal_station:2','time_signal_station'], // Time Signal Station
     ['STA2','20','seamark:signal_station:2','unmanned_oceanographic_station'], // Unmanned Oceanographic Station
     ['STA2','21','seamark:signal_station:2','weather_signal_station'], // Weather Signal Station
     ['STA2','22','seamark:signal_station:2','fog_signal'], // Fog Signal
     ['STA2','27','seamark:signal_station:2','tidal_stream_station'], // Tidal Stream Station
     ['STA2','28','seamark:signal_station:2','traffic_signal_station'], // Traffic Signal Station
     ['STA2','29','seamark:signal_station:2','bridge_signal_station'], // Bridge Signal Station
     ['STA2','30','seamark:signal_station:2','lock_signal_station'], // Lock Signal Station
     ['STA2','32','seamark:signal_station:2','international_port_signals_station'], // International Port Signals Station
     ['STA2','33','seamark:signal_station:2','military_practice_signal_station'], // Military Practice Signal Station
     ['STA2','35','seamark:signal_station:2','warning_signal_station'], // Warning Signal Station
     ['STA2','36','seamark:signal_station:2','radar_surveillance_station'], // Radar Surveillance Station
     ['STA2','37','seamark:signal_station:2','pilot_lookout_station'], // Pilot Lookout Station
     ['STA2','48','seamark:signal_station:2','semaphore_station'], // Semaphore Station
     ['STA2','49','seamark:signal_station:2','berthing_control_station'], // Berthing Control Station
     ['STA2','50','seamark:signal_station:2','cable_warning_station'], // Cable Warning Station
     ['STA2','51','seamark:signal_station:2','clock_station'], // Clock Station
     ['STA2','52','seamark:signal_station:2','danger_warning_station'], // Danger Warning Station
     ['STA2','53','seamark:signal_station:2','distress_warning_station'], // Distress Warning Station
     ['STA2','54','seamark:signal_station:2','diving_warning_station'], // Diving Warning Station
     ['STA2','55','seamark:signal_station:2','dock_control_station'], // Dock Control Station
     ['STA2','56','seamark:signal_station:2','dredging_operations_station'], // Dredging Operations Station
     ['STA2','57','seamark:signal_station:2','flood_barrage_control_station'], // Flood Barrage Control Station
     ['STA2','58','seamark:signal_station:2','maritime_obstruction_warning_station'], // Maritime Obstruction Warning Station
     ['STA2','59','seamark:signal_station:2','port_entry_and_departure_control_station'], // Port Entry and Departure Control Station
     ['STA2','60','seamark:signal_station:2','tide_scale_station'], // Tide Scale Station
     ['STA2','999','seamark:signal_station:2','other'], // Other

     // STA3 - Maritime Station Type [3]
     // ['STA3','-999999',undefined,undefined], // No Information
     ['STA3','1','seamark:signal_station:3','coast_guard_station'], // Coast Guard Station
     ['STA3','2','seamark:signal_station:3','fireboat_station'], // Fireboat Station
     ['STA3','3','seamark:signal_station:3','water-police_station'], // Water-police Station
     ['STA3','4','seamark:signal_station:3','ice_signal_station'], // Ice Signal Station
     ['STA3','5','seamark:signal_station:3','rescue_station'], // Rescue Station
     ['STA3','6','seamark:signal_station:3','port_control_station'], // Port Control Station
     ['STA3','11','seamark:signal_station:3','pilot_station'], // Pilot Station
     ['STA3','13','seamark:signal_station:3','signal_station'], // Signal Station
     ['STA3','14','seamark:signal_station:3','signal_mast'], // Signal Mast
     ['STA3','15','seamark:signal_station:3','storm_signal_station'], // Storm Signal Station
     ['STA3','17','seamark:signal_station:3','tide_station'], // Tide Station
     ['STA3','18','seamark:signal_station:3','time_ball_station'], // Time Ball Station
     ['STA3','19','seamark:signal_station:3','time_signal_station'], // Time Signal Station
     ['STA3','20','seamark:signal_station:3','unmanned_oceanographic_station'], // Unmanned Oceanographic Station
     ['STA3','21','seamark:signal_station:3','weather_signal_station'], // Weather Signal Station
     ['STA3','22','seamark:signal_station:3','fog_signal'], // Fog Signal
     ['STA3','27','seamark:signal_station:3','tidal_stream_station'], // Tidal Stream Station
     ['STA3','28','seamark:signal_station:3','traffic_signal_station'], // Traffic Signal Station
     ['STA3','29','seamark:signal_station:3','bridge_signal_station'], // Bridge Signal Station
     ['STA3','30','seamark:signal_station:3','lock_signal_station'], // Lock Signal Station
     ['STA3','32','seamark:signal_station:3','international_port_signals_station'], // International Port Signals Station
     ['STA3','33','seamark:signal_station:3','military_practice_signal_station'], // Military Practice Signal Station
     ['STA3','35','seamark:signal_station:3','warning_signal_station'], // Warning Signal Station
     ['STA3','36','seamark:signal_station:3','radar_surveillance_station'], // Radar Surveillance Station
     ['STA3','37','seamark:signal_station:3','pilot_lookout_station'], // Pilot Lookout Station
     ['STA3','48','seamark:signal_station:3','semaphore_station'], // Semaphore Station
     ['STA3','49','seamark:signal_station:3','berthing_control_station'], // Berthing Control Station
     ['STA3','50','seamark:signal_station:3','cable_warning_station'], // Cable Warning Station
     ['STA3','51','seamark:signal_station:3','clock_station'], // Clock Station
     ['STA3','52','seamark:signal_station:3','danger_warning_station'], // Danger Warning Station
     ['STA3','53','seamark:signal_station:3','distress_warning_station'], // Distress Warning Station
     ['STA3','54','seamark:signal_station:3','diving_warning_station'], // Diving Warning Station
     ['STA3','55','seamark:signal_station:3','dock_control_station'], // Dock Control Station
     ['STA3','56','seamark:signal_station:3','dredging_operations_station'], // Dredging Operations Station
     ['STA3','57','seamark:signal_station:3','flood_barrage_control_station'], // Flood Barrage Control Station
     ['STA3','58','seamark:signal_station:3','maritime_obstruction_warning_station'], // Maritime Obstruction Warning Station
     ['STA3','59','seamark:signal_station:3','port_entry_and_departure_control_station'], // Port Entry and Departure Control Station
     ['STA3','60','seamark:signal_station:3','tide_scale_station'], // Tide Scale Station
     ['STA3','999','seamark:signal_station:3','other'], // Other

     // STL - Nomadic Seasonal Location
     // ['STL','-999999',undefined,undefined], // No Information
     ['STL','1','seasonal_location','winter'], // Winter
     ['STL','2','seasonal_location','summer'], // Summer
     ['STL','3','seasonal_location','spring'], // Spring
     ['STL','4','seasonal_location','autumn'], // Autumn
     ['STL','999','seasonal_location','other'], // Other

     // STL2 - Nomadic Seasonal Location [2]
     // ['STL2','-999999',undefined,undefined], // No Information
     ['STL2','1','seasonal_location:2','winter'], // Winter
     ['STL2','2','seasonal_location:2','summer'], // Summer
     ['STL2','3','seasonal_location:2','spring'], // Spring
     ['STL2','4','seasonal_location:2','autumn'], // Autumn
     ['STL2','999','seasonal_location:2','other'], // Other

     // STL3 - Nomadic Seasonal Location [3]
     // ['STL3','-999999',undefined,undefined], // No Information
     ['STL3','1','seasonal_location:3','winter'], // Winter
     ['STL3','2','seasonal_location:3','summer'], // Summer
     ['STL3','3','seasonal_location:3','spring'], // Spring
     ['STL3','4','seasonal_location:3','autumn'], // Autumn
     ['STL3','999','seasonal_location:3','other'], // Other

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
     // ['TRS2','-999999',undefined,undefined], // No Information
     ['TRS2','1','transport:type:2','air'],
     ['TRS2','2','transport:type:2','aqueduct'], // Aqueduct
     ['TRS2','3','transport:type:2','automotive'], // Automotive
     ['TRS2','4','transport:type:2','bus'], // Bus
     ['TRS2','5','transport:type:2','canal'], // Canal
     ['TRS2','6','transport:type:2','caravan_route'], // Caravan Route
     ['TRS2','7','transport:type:2','maritime'],
     ['TRS2','8',undefined,undefined], // No Transportation System
     ['TRS2','9','transport:type:2','pedestrian'], // Pedestrian
     ['TRS2','10','transport:type:2','pipeline'], // Pipeline
     ['TRS2','11','transport:type:2','portage'], // Portage
     ['TRS2','12','transport:type:2','railway'], // Railway
     ['TRS2','13','transport:type:2','road'], // Road
     ['TRS2','16','transport:type:2','underground_railway'], // Underground Railway
     ['TRS2','17','transport:type:2','inland_waterway'], // Inland Waterway
     ['TRS2','18','transport:type:2','pack-road'], // Pack-road
     ['TRS2','20','transport:type:2','taxiway'], // Taxiway
     ['TRS2','23','transport:type:2','drove'], // Drove
     ['TRS2','25','transport:type:2','runway'], // Runway
     ['TRS2','999','transport:type:2','other'], // Other

     // TRS3 - Transportation System Type [3]
     // ['TRS3','-999999',undefined,undefined], // No Information
     ['TRS3','1','transport:type:3','air'],
     ['TRS3','2','transport:type:3','aqueduct'], // Aqueduct
     ['TRS3','3','transport:type:3','automotive'], // Automotive
     ['TRS3','4','transport:type:3','bus'], // Bus
     ['TRS3','5','transport:type:3','canal'], // Canal
     ['TRS3','6','transport:type:3','caravan_route'], // Caravan Route
     ['TRS3','7','transport:type:3','maritime'],
     ['TRS3','8',undefined,undefined], // No Transportation System
     ['TRS3','9','transport:type:3','pedestrian'], // Pedestrian
     ['TRS3','10','transport:type:3','pipeline'], // Pipeline
     ['TRS3','11','transport:type:3','portage'], // Portage
     ['TRS3','12','transport:type:3','railway'], // Railway
     ['TRS3','13','transport:type:3','road'], // Road
     ['TRS3','16','transport:type:3','underground_railway'], // Underground Railway
     ['TRS3','17','transport:type:3','inland_waterway'], // Inland Waterway
     ['TRS3','18','transport:type:3','pack-road'], // Pack-road
     ['TRS3','20','transport:type:3','taxiway'], // Taxiway
     ['TRS3','23','transport:type:3','drove'], // Drove
     ['TRS3','25','transport:type:3','runway'], // Runway
     ['TRS3','999','transport:type:3','other'], // Other

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
     // ['TSM2','-999999',undefined,undefined], // No Information
     ['TSM2','1','desert_surface:2','asphalt'], // Asphalt
     ['TSM2','2','desert_surface:2','bedrock'], // Bedrock
     ['TSM2','3','desert_surface:2','boulders'], // Boulders
     ['TSM2','4','desert_surface:2','concrete'], // Concrete
     ['TSM2','5','desert_surface:2','evaporite'], // Evaporite
     ['TSM2','6','desert_surface:2','frozen_water'], // Frozen Water
     ['TSM2','7','desert_surface:2','gravel'], // Gravel
     ['TSM2','8','desert_surface:2','lava_flow'], // Lava Flow
     ['TSM2','9','desert_surface:2','loess'], // Loess
     ['TSM2','10','desert_surface:2','mud'], // Mud
     ['TSM2','11','desert_surface:2','paved'], // Paved
     ['TSM2','12','desert_surface:2','rocks'], // Rocks
     ['TSM2','13','desert_surface:2','sand'], // Sand
     ['TSM2','14','desert_surface:2','soil'], // Soil
     ['TSM2','999','desert_surface:2','other'], // Other

     // TSM3 - Terrain Surface Material [3]
     // ['TSM3','-999999',undefined,undefined], // No Information
     ['TSM3','1','desert_surface:3','asphalt'], // Asphalt
     ['TSM3','2','desert_surface:3','bedrock'], // Bedrock
     ['TSM3','3','desert_surface:3','boulders'], // Boulders
     ['TSM3','4','desert_surface:3','concrete'], // Concrete
     ['TSM3','5','desert_surface:3','evaporite'], // Evaporite
     ['TSM3','6','desert_surface:3','frozen_water'], // Frozen Water
     ['TSM3','7','desert_surface:3','gravel'], // Gravel
     ['TSM3','8','desert_surface:3','lava_flow'], // Lava Flow
     ['TSM3','9','desert_surface:3','loess'], // Loess
     ['TSM3','10','desert_surface:3','mud'], // Mud
     ['TSM3','11','desert_surface:3','paved'], // Paved
     ['TSM3','12','desert_surface:3','rocks'], // Rocks
     ['TSM3','13','desert_surface:3','sand'], // Sand
     ['TSM3','14','desert_surface:3','soil'], // Soil
     ['TSM3','999','desert_surface:3','other'], // Other

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
     // ['TSSM2','-999999',undefined,undefined], // No Information
     ['TSSM2','1','well:subsurface:2','basalt'], // Basalt
     ['TSSM2','2','well:subsurface:2','boulders'], // Boulders
     ['TSSM2','3','well:subsurface:2','clay'], // Clay
     ['TSSM2','4','well:subsurface:2','conglomerate'], // Conglomerate
     ['TSSM2','5','well:subsurface:2','dolomite'], // Dolomite
     ['TSSM2','6','well:subsurface:2','granite'], // Granite
     ['TSSM2','7','well:subsurface:2','gravel'], // Gravel
     ['TSSM2','8','well:subsurface:2','igneous_rock'], // Igneous Rock
     ['TSSM2','9','well:subsurface:2','karst'], // Karst
     ['TSSM2','10','well:subsurface:2','limestone'], // Limestone
     ['TSSM2','11','well:subsurface:2','marl'], // Marl
     ['TSSM2','12','well:subsurface:2','metamorphic_rock'], // Metamorphic Rock
     ['TSSM2','13','well:subsurface:2','mud'], // Mud
     ['TSSM2','14','well:subsurface:2','sand'], // Sand
     ['TSSM2','15','well:subsurface:2','schist'], // Schist
     ['TSSM2','16','well:subsurface:2','unconsolidated'], // Unconsolidated
     ['TSSM2','17','well:subsurface:2','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['TSSM2','18','well:subsurface:2','volcanic_rock'], // Volcanic Rock
     ['TSSM2','19','well:subsurface:2','sandstone'], // Sandstone
     ['TSSM2','20','well:subsurface:2','shale'], // Shale
     ['TSSM2','999','well:subsurface:2','other'], // Other

     // TSSM3 - Terrain Subsurface Material [3]
     // ['TSSM3','-999999',undefined,undefined], // No Information
     ['TSSM3','1','well:subsurface:3','basalt'], // Basalt
     ['TSSM3','2','well:subsurface:3','boulders'], // Boulders
     ['TSSM3','3','well:subsurface:3','clay'], // Clay
     ['TSSM3','4','well:subsurface:3','conglomerate'], // Conglomerate
     ['TSSM3','5','well:subsurface:3','dolomite'], // Dolomite
     ['TSSM3','6','well:subsurface:3','granite'], // Granite
     ['TSSM3','7','well:subsurface:3','gravel'], // Gravel
     ['TSSM3','8','well:subsurface:3','igneous_rock'], // Igneous Rock
     ['TSSM3','9','well:subsurface:3','karst'], // Karst
     ['TSSM3','10','well:subsurface:3','limestone'], // Limestone
     ['TSSM3','11','well:subsurface:3','marl'], // Marl
     ['TSSM3','12','well:subsurface:3','metamorphic_rock'], // Metamorphic Rock
     ['TSSM3','13','well:subsurface:3','mud'], // Mud
     ['TSSM3','14','well:subsurface:3','sand'], // Sand
     ['TSSM3','15','well:subsurface:3','schist'], // Schist
     ['TSSM3','16','well:subsurface:3','unconsolidated'], // Unconsolidated
     ['TSSM3','17','well:subsurface:3','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['TSSM3','18','well:subsurface:3','volcanic_rock'], // Volcanic Rock
     ['TSSM3','19','well:subsurface:3','sandstone'], // Sandstone
     ['TSSM3','20','well:subsurface:3','shale'], // Shale
     ['TSSM3','999','well:subsurface:3','other'], // Other

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
     // ['TTC2','-999999',undefined,undefined], // No Information
     ['TTC2','2','tower:type:2','observation'], // Observation Tower
     ['TTC2','10','tower:type:2','lookout'], // Lookout Tower
     ['TTC2','12','tower:type:2','fire'], // Fire Tower
     ['TTC2','20','tower:type:2','communication'], // Telecommunication Tower
     ['TTC2','21','tower:type:2','guard'], // Guard Tower
     ['TTC2','22','tower:type:2','industrial'], // Industrial Tower
     ['TTC2','23','tower:type:2','drop'], // Drop Tower
     ['TTC2','24','tower:type:2','solar_power'], // Solar Power Tower
     ['TTC2','999','tower:type:2','other'], // Other

     // TTC3 - Tower Type [3]
     // ['TTC3','-999999',undefined,undefined], // No Information
     ['TTC3','2','tower:type:3','observation'], // Observation Tower
     ['TTC3','10','tower:type:3','lookout'], // Lookout Tower
     ['TTC3','12','tower:type:3','fire_tower'], // Fire Tower
     ['TTC3','20','tower:type:3','telecommunication'], // Telecommunication Tower
     ['TTC3','21','tower:type:3','guard'], // Guard Tower
     ['TTC3','22','tower:type:3','industrial'], // Industrial Tower
     ['TTC3','23','tower:type:3','drop'], // Drop Tower
     ['TTC3','24','tower:type:3','solar_power'], // Solar Power Tower
     ['TTC3','999','tower:type:3','other'], // Other

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
     ['TXP','1','taxiway:type','parallel_taxiway'], // Parallel Taxiway
     ['TXP','2','taxiway:type','stub_taxiway'], // Stub Taxiway
     ['TXP','3','taxiway:type','rapid_exit_and/or_turnoff_taxiway'], // Rapid Exit and/or Turnoff Taxiway
     ['TXP','4','taxiway:type','turnaround_taxiway'], // Turnaround Taxiway
     ['TXP','5','taxiway:type','dispersal'], // Dispersal
     ['TXP','6','taxiway:type','loop'], // Loop
     ['TXP','7','taxiway:type','perimeter'], // Perimeter
     ['TXP','8','taxiway:type','apron_taxiway'], // Apron Taxiway
     ['TXP','9','taxiway:type','aircraft_stand_taxilane'], // Aircraft Stand Taxilane
     ['TXP','10','taxiway:type','lead-in_taxilane'], // Lead-in Taxilane
     ['TXP','11','taxiway:type','lead-out_taxilane'], // Lead-out Taxilane
     ['TXP','12','taxiway:type','air_taxiway'], // Air Taxiway
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
     // ['UMA','998','mine:access','not_applicable'], // Not Applicable
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

     // UTY2 - Accessible Utility Type [2]
     // ['UTY2','-999999',undefined,undefined], // No Information
     ['UTY2','1','manhole:2','cable_television'], // Cable Television
     ['UTY2','2','manhole:2','cooling_fluid_circulation'], // Cooling Fluid Circulation
     ['UTY2','3','manhole:2','digital_fibre-optic_system'], // Digital Fibre-optic System
     ['UTY2','4','manhole:2','electric_power_distribution'], // Electric Power Distribution
     ['UTY2','5','manhole:2','heating_fluid_circulation'], // Heating Fluid Circulation
     ['UTY2','6','manhole:2','natural_gas_distribution'], // Natural Gas Distribution
     ['UTY2','7','manhole:2','sewage:2'], // Sewage
     ['UTY2','8','manhole:2','storm_sewer'], // Storm Sewer
     ['UTY2','9','manhole:2','street_light'], // Street Light
     ['UTY2','10','manhole:2','telegraph'], // Telegraph
     ['UTY2','11','manhole:2','telephone:2'], // Telephone
     ['UTY2','12','manhole:2','traffic_light'], // Traffic Light
     ['UTY2','13','manhole:2','water_distribution'], // Water Distribution
     ['UTY2','999','manhole:2','other'], // Other

     // UTY3 - Accessible Utility Type [3]
     // ['UTY3','-999999',undefined,undefined], // No Information
     ['UTY3','1','manhole:3','cable_television'], // Cable Television
     ['UTY3','2','manhole:3','cooling_fluid_circulation'], // Cooling Fluid Circulation
     ['UTY3','3','manhole:3','digital_fibre-optic_system'], // Digital Fibre-optic System
     ['UTY3','4','manhole:3','electric_power_distribution'], // Electric Power Distribution
     ['UTY3','5','manhole:3','heating_fluid_circulation'], // Heating Fluid Circulation
     ['UTY3','6','manhole:3','natural_gas_distribution'], // Natural Gas Distribution
     ['UTY3','7','manhole:3','sewage'], // Sewage
     ['UTY3','8','manhole:3','storm_sewer'], // Storm Sewer
     ['UTY3','9','manhole:3','street_light'], // Street Light
     ['UTY3','10','manhole:3','telegraph'], // Telegraph
     ['UTY3','11','manhole:3','telephone'], // Telephone
     ['UTY3','12','manhole:3','traffic_light'], // Traffic Light
     ['UTY3','13','manhole:3','water_distribution'], // Water Distribution
     ['UTY3','999','manhole:3','other'], // Other

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
     // ['VCA2','-999999',undefined,undefined], // No Information
     ['VCA2','1','void_collection:reason:2','not_requested'], // Not Requested
     ['VCA2','2','void_collection:reason:2','inaccessible'], // Inaccessible
     ['VCA2','3','void_collection:reason:2','no_available_imagery'], // No Available Imagery
     ['VCA2','6','void_collection:reason:2','no_available_map_source'], // No Available Map Source
     ['VCA2','7','void_collection:reason:2','no_suitable_imagery'], // No Suitable Imagery
     ['VCA2','8','void_collection:reason:2','not_required'], // Not Required
     ['VCA2','9','void_collection:reason:2','cloud_cover'], // Cloud Cover
     ['VCA2','10','void_collection:reason:2','snow_cover'], // Snow Cover
     ['VCA2','11','void_collection:reason:2','dark_shade'], // Dark Shade
     ['VCA2','12','void_collection:reason:2','vegetation_cover'], // Vegetation Cover
     ['VCA2','13','void_collection:reason:2','flooded'], // Flooded
     ['VCA2','16','void_collection:reason:2','no_available_survey'], // No Available Survey
     ['VCA2','999','void_collection:reason:2','other'], // Other

     // VCA3 - Void Collection Reason [3]
     // ['VCA3','-999999',undefined,undefined], // No Information
     ['VCA3','1','void_collection:reason:3','not_requested'], // Not Requested
     ['VCA3','2','void_collection:reason:3','inaccessible'], // Inaccessible
     ['VCA3','3','void_collection:reason:3','no_available_imagery'], // No Available Imagery
     ['VCA3','6','void_collection:reason:3','no_available_map_source'], // No Available Map Source
     ['VCA3','7','void_collection:reason:3','no_suitable_imagery'], // No Suitable Imagery
     ['VCA3','8','void_collection:reason:3','not_required'], // Not Required
     ['VCA3','9','void_collection:reason:3','cloud_cover'], // Cloud Cover
     ['VCA3','10','void_collection:reason:3','snow_cover'], // Snow Cover
     ['VCA3','11','void_collection:reason:3','dark_shade'], // Dark Shade
     ['VCA3','12','void_collection:reason:3','vegetation_cover'], // Vegetation Cover
     ['VCA3','13','void_collection:reason:3','flooded'], // Flooded
     ['VCA3','16','void_collection:reason:3','no_available_survey'], // No Available Survey
     ['VCA3','999','void_collection:reason:3','other'], // Other

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

     // VCM2 - Vertical Construction Material [2]
     // ['VCM2','-999999',undefined,undefined], // No Information
     ['VCM2','1','material:vertical:2','adobe_brick'], // Adobe Brick
     ['VCM2','2','material:vertical:2','aluminium'], // Aluminium
     ['VCM2','3','material:vertical:2','brick'], // Brick
     ['VCM2','4','material:vertical:2','concrete'], // Concrete
     ['VCM2','5','material:vertical:2','fibreglass'], // Fibreglass
     ['VCM2','6','material:vertical:2','glass'], // Glass
     ['VCM2','7','material:vertical:2','iron'], // Iron
     ['VCM2','8','material:vertical:2','masonry'], // Masonry
     ['VCM2','9','material:vertical:2','metal'], // Metal
     ['VCM2','10','material:vertical:2','mud-based_construction'], // Mud-based Construction
     ['VCM2','11','material:vertical:2','plant_material'], // Plant Material
     ['VCM2','12','material:vertical:2','prestressed_concrete'], // Prestressed Concrete
     ['VCM2','13','material:vertical:2','reinforced_concrete'], // Reinforced Concrete
     ['VCM2','14','material:vertical:2','sod'], // Sod
     ['VCM2','15','material:vertical:2','steel'], // Steel
     ['VCM2','16','material:vertical:2','stone'], // Stone
     ['VCM2','17','material:vertical:2','treated_timber'], // Treated Timber
     ['VCM2','18','material:vertical:2','wood'], // Wood
     ['VCM2','999','material:vertical:2','other'], // Other

     // VCM3 - Vertical Construction Material [3]
     // ['VCM3','-999999',undefined,undefined], // No Information
     ['VCM3','1','material:vertical:3','adobe_brick'], // Adobe Brick
     ['VCM3','2','material:vertical:3','aluminium'], // Aluminium
     ['VCM3','3','material:vertical:3','brick'], // Brick
     ['VCM3','4','material:vertical:3','concrete'], // Concrete
     ['VCM3','5','material:vertical:3','fibreglass'], // Fibreglass
     ['VCM3','6','material:vertical:3','glass'], // Glass
     ['VCM3','7','material:vertical:3','iron'], // Iron
     ['VCM3','8','material:vertical:3','masonry'], // Masonry
     ['VCM3','9','material:vertical:3','metal'], // Metal
     ['VCM3','10','material:vertical:3','mud-based_construction'], // Mud-based Construction
     ['VCM3','11','material:vertical:3','plant_material'], // Plant Material
     ['VCM3','12','material:vertical:3','prestressed_concrete'], // Prestressed Concrete
     ['VCM3','13','material:vertical:3','reinforced_concrete'], // Reinforced Concrete
     ['VCM3','14','material:vertical:3','sod'], // Sod
     ['VCM3','15','material:vertical:3','steel'], // Steel
     ['VCM3','16','material:vertical:3','stone'], // Stone
     ['VCM3','17','material:vertical:3','treated_timber'], // Treated Timber
     ['VCM3','18','material:vertical:3','wood'], // Wood
     ['VCM3','999','material:vertical:3','other'], // Other

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
     // ['VCT2','-999999',undefined,undefined], // No Information
     ['VCT2','1','void_collection:type:2','hypsography'], // Hypsography
     ['VCT2','3','void_collection:type:2','bathymetry'], // Bathymetry
     ['VCT2','4','void_collection:type:2','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT2','999','void_collection:type:2','other'], // Other

     // VCT3 - Void Collection Type [3]
     // ['VCT3','-999999',undefined,undefined], // No Information
     ['VCT3','1','void_collection:type:3','hypsography'], // Hypsography
     ['VCT3','3','void_collection:type:3','bathymetry'], // Bathymetry
     ['VCT3','4','void_collection:type:3','waterbody_bottom_composition'], // Waterbody Bottom Composition
     ['VCT3','999','void_collection:type:3','other'], // Other

     // VDT - Vertical Datum
     // ['VDT','-999999',undefined,undefined], // No Information
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
     // ['VIS2','-999999',undefined,undefined], // No Information
     ['VIS2','1','light_visibility:2','high_intensity'], // High Intensity
     ['VIS2','2','light_visibility:2','low_intensity'], // Low Intensity
     ['VIS2','3','light_visibility:2','faint'], // Faint
     ['VIS2','4','light_visibility:2','intensified'], // Intensified
     ['VIS2','5','light_visibility:2','unintensified'], // Unintensified
     ['VIS2','6','light_visibility:2','visibility_deliberately_restricted'], // Visibility Deliberately Restricted
     ['VIS2','7','light_visibility:2','obscured'], // Obscured
     ['VIS2','8','light_visibility:2','partially_obscured'], // Partially Obscured
     ['VIS2','999','light_visibility:2','other'], // Other

     // VIS3 - Light Visibility [3]
     // ['VIS3','-999999',undefined,undefined], // No Information
     ['VIS3','1','light_visibility:3','high_intensity'], // High Intensity
     ['VIS3','2','light_visibility:3','low_intensity'], // Low Intensity
     ['VIS3','3','light_visibility:3','faint'], // Faint
     ['VIS3','4','light_visibility:3','intensified'], // Intensified
     ['VIS3','5','light_visibility:3','unintensified'], // Unintensified
     ['VIS3','6','light_visibility:3','visibility_deliberately_restricted'], // Visibility Deliberately Restricted
     ['VIS3','7','light_visibility:3','obscured'], // Obscured
     ['VIS3','8','light_visibility:3','partially_obscured'], // Partially Obscured
     ['VIS3','999','light_visibility:3','other'], // Other

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
     // ['VSP2','-999999',undefined,undefined], // No Information
     ['VSP2','8','vegetation:type:2','cypress'], // Cypress
     ['VSP2','17','vegetation:type:2','kelp'], // Kelp
     ['VSP2','19','vegetation:type:2','mangrove'], // Mangrove
     ['VSP2','22','vegetation:type:2','nipa'], // Nipa
     ['VSP2','25','vegetation:type:2','palm'], // Palm
     ['VSP2','28','vegetation:type:2','posidonia'], // Posidonia
     ['VSP2','29','vegetation:type:2','reed'], // Reed
     ['VSP2','30','vegetation:type:2','sargassum'], // Sargassum
     ['VSP2','31','vegetation:type:2','sea-tangle'], // Sea-tangle
     ['VSP2','34','vegetation:type:2','bamboo'], // Bamboo
     ['VSP2','35','vegetation:type:2','water_hyacinth'], // Water Hyacinth
     ['VSP2','36','vegetation:type:2','algae'], // Algae
     ['VSP2','64','vegetation:type:2','swamp_cypress'], // Swamp Cypress
     ['VSP2','999','vegetation:type:2','other'], // Other

     // VSP3 - Vegetation Species [3]
     // ['VSP3','-999999',undefined,undefined], // No Information
     ['VSP3','8','vegetation:type:3','cypress'], // Cypress
     ['VSP3','17','vegetation:type:3','kelp'], // Kelp
     ['VSP3','19','vegetation:type:3','mangrove'], // Mangrove
     ['VSP3','22','vegetation:type:3','nipa'], // Nipa
     ['VSP3','25','vegetation:type:3','palm'], // Palm
     ['VSP3','28','vegetation:type:3','posidonia'], // Posidonia
     ['VSP3','29','vegetation:type:3','reed'], // Reed
     ['VSP3','30','vegetation:type:3','sargassum'], // Sargassum
     ['VSP3','31','vegetation:type:3','sea-tangle'], // Sea-tangle
     ['VSP3','34','vegetation:type:3','bamboo'], // Bamboo
     ['VSP3','35','vegetation:type:3','water_hyacinth'], // Water Hyacinth
     ['VSP3','36','vegetation:type:3','algae'], // Algae
     ['VSP3','64','vegetation:type:3','swamp_cypress'], // Swamp Cypress
     ['VSP3','999','vegetation:type:3','other'], // Other

     // WCC - Watercourse Channel Type
     // ['WCC','-999999',undefined,undefined], // No Information
     ['WCC','1','waterway','stream'], // Channelized Stream
     ['WCC','2','waterway','braided_stream'], // Braided Stream
     ['WCC','3','waterway','gorge'], // Gorge
     ['WCC','4','waterway','wadi'], // Wadi
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
     // ['WEQ2','-999999',undefined,undefined], // No Information
     ['WEQ2','1','well:equipment:2','wellhead'], // Wellhead
     ['WEQ2','2','well:equipment:2','christmas_tree'], // Christmas Tree
     ['WEQ2','3','well:equipment:2','manifold'], // Manifold
     ['WEQ2','4','well:equipment:2','protective_structure'], // Protective Structure
     ['WEQ2','5','well:equipment:2','pump'], // Pump
     ['WEQ2','6','well:equipment:2','rod_pump'], // Rod Pump
     ['WEQ2','7','well:equipment:2','separator'], // Separator
     ['WEQ2','8','well:equipment:2','stock_tank'], // Stock Tank
     ['WEQ2','9','well:equipment:2','treater'], // Treater
     ['WEQ2','10','well:equipment:2','capped'], // Capped
     ['WEQ2','999','well:equipment:2','other'], // Other

     // WEQ3 - Well Equipment [3]
     // ['WEQ3','-999999',undefined,undefined], // No Information
     ['WEQ3','1','well:equipment:3','wellhead'], // Wellhead
     ['WEQ3','2','well:equipment:3','christmas_tree'], // Christmas Tree
     ['WEQ3','3','well:equipment:3','manifold'], // Manifold
     ['WEQ3','4','well:equipment:3','protective_structure'], // Protective Structure
     ['WEQ3','5','well:equipment:3','pump'], // Pump
     ['WEQ3','6','well:equipment:3','rod_pump'], // Rod Pump
     ['WEQ3','7','well:equipment:3','separator'], // Separator
     ['WEQ3','8','well:equipment:3','stock_tank'], // Stock Tank
     ['WEQ3','9','well:equipment:3','treater'], // Treater
     ['WEQ3','10','well:equipment:3','capped'], // Capped
     ['WEQ3','999','well:equipment:3','other'], // Other

     // WFT - Well Type
     // ['WFT','-999999',undefined,undefined], // No Information
     ['WFT','2','well:type','walled-in'], // Walled-in
     ['WFT','3','well:type','artesian'], // Artesian
     ['WFT','5','well:type','dug_or_drilled'], // Dug or Drilled
     ['WFT','6','well:type','dug'], // Dug
     ['WFT','7','well:type','drilled'], // Drilled
     ['WFT','999','well:type','other'], // Other

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
     // ['WPC2','-999999',undefined,undefined], // No Information
     ['WPC2','1','coastal_work_type:2','land_reclamation'], // Land Reclamation
     ['WPC2','2','coastal_work_type:2','construction_of_structures'], // Construction of Structures
     ['WPC2','999','coastal_work_type:2','other'], // Other

     // WPC3 - Coastal Work Type [3]
     // ['WPC3','-999999',undefined,undefined], // No Information
     ['WPC3','1','coastal_work_type:3','land_reclamation'], // Land Reclamation
     ['WPC3','2','coastal_work_type:3','construction_of_structures'], // Construction of Structures
     ['WPC3','999','coastal_work_type:3','other'], // Other

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
     // ['WWM2','-999999',undefined,undefined], // No Information
     ['WWM2','1','well:subsurface_stratum:2','basalt'], // Basalt
     ['WWM2','2','well:subsurface_stratum:2','boulders'], // Boulders
     ['WWM2','3','well:subsurface_stratum:2','clay'], // Clay
     ['WWM2','4','well:subsurface_stratum:2','conglomerate'], // Conglomerate
     ['WWM2','5','well:subsurface_stratum:2','dolomite'], // Dolomite
     ['WWM2','6','well:subsurface_stratum:2','granite'], // Granite
     ['WWM2','7','well:subsurface_stratum:2','gravel'], // Gravel
     ['WWM2','8','well:subsurface_stratum:2','igneous_rock'], // Igneous Rock
     ['WWM2','9','well:subsurface_stratum:2','karst'], // Karst
     ['WWM2','10','well:subsurface_stratum:2','limestone'], // Limestone
     ['WWM2','11','well:subsurface_stratum:2','marl'], // Marl
     ['WWM2','12','well:subsurface_stratum:2','metamorphic_rock'], // Metamorphic Rock
     ['WWM2','13','well:subsurface_stratum:2','mud'], // Mud
     ['WWM2','14','well:subsurface_stratum:2','sand'], // Sand
     ['WWM2','15','well:subsurface_stratum:2','schist'], // Schist
     ['WWM2','16','well:subsurface_stratum:2','unconsolidated'], // Unconsolidated
     ['WWM2','17','well:subsurface_stratum:2','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['WWM2','18','well:subsurface_stratum:2','volcanic_rock'], // Volcanic Rock
     ['WWM2','19','well:subsurface_stratum:2','sandstone'], // Sandstone
     ['WWM2','20','well:subsurface_stratum:2','shale'], // Shale
     ['WWM2','999','well:subsurface_stratum:2','other'], // Other

     // WWM3 - Sub-Surface Stratum Type [3]
     // ['WWM3','-999999',undefined,undefined], // No Information
     ['WWM3','1','well:subsurface_stratum:3','basalt'], // Basalt
     ['WWM3','2','well:subsurface_stratum:3','boulders'], // Boulders
     ['WWM3','3','well:subsurface_stratum:3','clay'], // Clay
     ['WWM3','4','well:subsurface_stratum:3','conglomerate'], // Conglomerate
     ['WWM3','5','well:subsurface_stratum:3','dolomite'], // Dolomite
     ['WWM3','6','well:subsurface_stratum:3','granite'], // Granite
     ['WWM3','7','well:subsurface_stratum:3','gravel'], // Gravel
     ['WWM3','8','well:subsurface_stratum:3','igneous_rock'], // Igneous Rock
     ['WWM3','9','well:subsurface_stratum:3','karst'], // Karst
     ['WWM3','10','well:subsurface_stratum:3','limestone'], // Limestone
     ['WWM3','11','well:subsurface_stratum:3','marl'], // Marl
     ['WWM3','12','well:subsurface_stratum:3','metamorphic_rock'], // Metamorphic Rock
     ['WWM3','13','well:subsurface_stratum:3','mud'], // Mud
     ['WWM3','14','well:subsurface_stratum:3','sand'], // Sand
     ['WWM3','15','well:subsurface_stratum:3','schist'], // Schist
     ['WWM3','16','well:subsurface_stratum:3','unconsolidated'], // Unconsolidated
     ['WWM3','17','well:subsurface_stratum:3','semi-consolidated_and_consolidated_volcanic_ash'], // Semi-consolidated and Consolidated Volcanic Ash
     ['WWM3','18','well:subsurface_stratum:3','volcanic_rock'], // Volcanic Rock
     ['WWM3','19','well:subsurface_stratum:3','sandstone'], // Sandstone
     ['WWM3','20','well:subsurface_stratum:3','shale'], // Shale
     ['WWM3','999','well:subsurface_stratum:3','other'], // Other

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

     // ZHBH_DCR - Base Hgt : Snd Meta : Vessel Draft Correction - See ZHDP_DCR
     // ['ZHBH_DCR','-999999',undefined,undefined], // No Information
//      ['ZHBH_DCR','1000','raw:ZHBH_DCR','no'],
//      ['ZHBH_DCR','1001','raw:ZHBH_DCR','yes'],

     // ZHBH_DKC - Base Hgt : Snd Meta : Bathymetric Measurement Quality Category - See ZHDP_DKC
     // ['ZHBH_DKC','-999999',undefined,undefined], // No Information
//      ['ZHBH_DKC','1','raw:ZHBH_DKC','depth_known'], // Depth Known
//      ['ZHBH_DKC','3','raw:ZHBH_DKC','safe_clearance_known'], // Safe Clearance Known
//      ['ZHBH_DKC','4','raw:ZHBH_DKC','depth_unknown'], // Depth Unknown
//      ['ZHBH_DKC','5','raw:ZHBH_DKC','depth_doubtful'], // Depth Doubtful
//      ['ZHBH_DKC','6','raw:ZHBH_DKC','depth_unreliable'], // Depth Unreliable
//      ['ZHBH_DKC','7','raw:ZHBH_DKC','no_bottom'], // No Bottom
//      ['ZHBH_DKC','8','raw:ZHBH_DKC','least_depth'], // Least Depth
//      ['ZHBH_DKC','9','raw:ZHBH_DKC','reported_depth'], // Reported Depth
//      ['ZHBH_DKC','10','raw:ZHBH_DKC','unconfirmed_depth'], // Unconfirmed Depth
//      ['ZHBH_DKC','11','raw:ZHBH_DKC','maintained_depth'], // Maintained Depth
//      ['ZHBH_DKC','12','raw:ZHBH_DKC','unmaintained_depth'], // Unmaintained Depth
//      ['ZHBH_DKC','998','raw:ZHBH_DKC','not_applicable'], // Not Applicable
//      ['ZHBH_DKC','999','raw:ZHBH_DKC','other'], // Other

     // ZHBH_DKC2 - Base Hgt : Snd Meta : Bathymetric Measurement Quality Category [2]  - See ZHDP_DKC2
     // ['ZHBH_DKC2','-999999',undefined,undefined], // No Information
//      ['ZHBH_DKC2','1','raw:ZHBH_DKC2','depth_known'], // Depth Known
//      ['ZHBH_DKC2','3','raw:ZHBH_DKC2','safe_clearance_known'], // Safe Clearance Known
//      ['ZHBH_DKC2','4','raw:ZHBH_DKC2','depth_unknown'], // Depth Unknown
//      ['ZHBH_DKC2','5','raw:ZHBH_DKC2','depth_doubtful'], // Depth Doubtful
//      ['ZHBH_DKC2','6','raw:ZHBH_DKC2','depth_unreliable'], // Depth Unreliable
//      ['ZHBH_DKC2','7','raw:ZHBH_DKC2','no_bottom'], // No Bottom
//      ['ZHBH_DKC2','8','raw:ZHBH_DKC2','least_depth'], // Least Depth
//      ['ZHBH_DKC2','9','raw:ZHBH_DKC2','reported_depth'], // Reported Depth
//      ['ZHBH_DKC2','10','raw:ZHBH_DKC2','unconfirmed_depth'], // Unconfirmed Depth
//      ['ZHBH_DKC2','11','raw:ZHBH_DKC2','maintained_depth'], // Maintained Depth
//      ['ZHBH_DKC2','12','raw:ZHBH_DKC2','unmaintained_depth'], // Unmaintained Depth
//      ['ZHBH_DKC2','998','raw:ZHBH_DKC2','not_applicable'], // Not Applicable
//      ['ZHBH_DKC2','999','raw:ZHBH_DKC2','other'], // Other

     // ZHBH_DKC3 - Base Hgt : Snd Meta : Bathymetric Measurement Quality Category [3] - See ZHDP_DKC3
     // ['ZHBH_DKC3','-999999',undefined,undefined], // No Information
//      ['ZHBH_DKC3','1','raw:ZHBH_DKC3','depth_known'], // Depth Known
//      ['ZHBH_DKC3','3','raw:ZHBH_DKC3','safe_clearance_known'], // Safe Clearance Known
//      ['ZHBH_DKC3','4','raw:ZHBH_DKC3','depth_unknown'], // Depth Unknown
//      ['ZHBH_DKC3','5','raw:ZHBH_DKC3','depth_doubtful'], // Depth Doubtful
//      ['ZHBH_DKC3','6','raw:ZHBH_DKC3','depth_unreliable'], // Depth Unreliable
//      ['ZHBH_DKC3','7','raw:ZHBH_DKC3','no_bottom'], // No Bottom
//      ['ZHBH_DKC3','8','raw:ZHBH_DKC3','least_depth'], // Least Depth
//      ['ZHBH_DKC3','9','raw:ZHBH_DKC3','reported_depth'], // Reported Depth
//      ['ZHBH_DKC3','10','raw:ZHBH_DKC3','unconfirmed_depth'], // Unconfirmed Depth
//      ['ZHBH_DKC3','11','raw:ZHBH_DKC3','maintained_depth'], // Maintained Depth
//      ['ZHBH_DKC3','12','raw:ZHBH_DKC3','unmaintained_depth'], // Unmaintained Depth
//      ['ZHBH_DKC3','998','raw:ZHBH_DKC3','not_applicable'], // Not Applicable
//      ['ZHBH_DKC3','999','raw:ZHBH_DKC3','other'], // Other

     // ZHBH_SVC - Base Hgt : Snd Meta : Sounding Velocity Correction Method
     // ['ZHBH_SVC','-999999',undefined,undefined], // No Information
     ['ZHBH_SVC','1','raw:ZHBH_SVC','sounder_4800_calibrated'], // Sounder 4800 Calibrated
     ['ZHBH_SVC','2','raw:ZHBH_SVC','sounder_1500_calibrated'], // Sounder 1500 Calibrated
     ['ZHBH_SVC','3','raw:ZHBH_SVC','mathews_tables'], // Mathews Tables
     ['ZHBH_SVC','4','raw:ZHBH_SVC','sound_velocity_meter'], // Sound Velocity Meter
     ['ZHBH_SVC','5','raw:ZHBH_SVC','sounder_other_calibrated'], // Sounder Other Calibrated
     ['ZHBH_SVC','998','raw:ZHBH_SVC','not_applicable'], // Not Applicable
     ['ZHBH_SVC','999','raw:ZHBH_SVC','other'], // Other

     // ZHBH_TEC - Base Hgt : Snd Meta : Bathymetric Measurement Technique
     // ['ZHBH_TEC','-999999',undefined,undefined], // No Information
     ['ZHBH_TEC','1','raw:ZHBH_TEC','echo_sounder'], // Echo Sounder
     ['ZHBH_TEC','2','raw:ZHBH_TEC','side-scan_sonar'], // Side-scan Sonar
     ['ZHBH_TEC','3','raw:ZHBH_TEC','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZHBH_TEC','4','raw:ZHBH_TEC','diver'], // Diver
     ['ZHBH_TEC','5','raw:ZHBH_TEC','lead-line'], // Lead-line
     ['ZHBH_TEC','6','raw:ZHBH_TEC','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZHBH_TEC','8','raw:ZHBH_TEC','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZHBH_TEC','9','raw:ZHBH_TEC','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZHBH_TEC','10','raw:ZHBH_TEC','photogrammetry'], // Photogrammetry
     ['ZHBH_TEC','11','raw:ZHBH_TEC','satellite_imagery'], // Satellite Imagery
     ['ZHBH_TEC','12','raw:ZHBH_TEC','levelling'], // Levelling
     ['ZHBH_TEC','13','raw:ZHBH_TEC','computer_generated'], // Computer Generated
     ['ZHBH_TEC','14','raw:ZHBH_TEC','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZHBH_TEC','15','raw:ZHBH_TEC','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZHBH_TEC','17','raw:ZHBH_TEC','singlebeam'], // Singlebeam
     ['ZHBH_TEC','18','raw:ZHBH_TEC','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZHBH_TEC','999','raw:ZHBH_TEC','other'], // Other

     // ZHBH_TEC2 - Base Hgt : Snd Meta : Bathymetric Measurement Technique [2]
     // ['ZHBH_TEC2','-999999',undefined,undefined], // No Information
     ['ZHBH_TEC2','1','raw:ZHBH_TEC2','echo_sounder'], // Echo Sounder
     ['ZHBH_TEC2','2','raw:ZHBH_TEC2','side-scan_sonar'], // Side-scan Sonar
     ['ZHBH_TEC2','3','raw:ZHBH_TEC2','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZHBH_TEC2','4','raw:ZHBH_TEC2','diver'], // Diver
     ['ZHBH_TEC2','5','raw:ZHBH_TEC2','lead-line'], // Lead-line
     ['ZHBH_TEC2','6','raw:ZHBH_TEC2','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZHBH_TEC2','8','raw:ZHBH_TEC2','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZHBH_TEC2','9','raw:ZHBH_TEC2','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZHBH_TEC2','10','raw:ZHBH_TEC2','photogrammetry'], // Photogrammetry
     ['ZHBH_TEC2','11','raw:ZHBH_TEC2','satellite_imagery'], // Satellite Imagery
     ['ZHBH_TEC2','12','raw:ZHBH_TEC2','levelling'], // Levelling
     ['ZHBH_TEC2','13','raw:ZHBH_TEC2','computer_generated'], // Computer Generated
     ['ZHBH_TEC2','14','raw:ZHBH_TEC2','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZHBH_TEC2','15','raw:ZHBH_TEC2','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZHBH_TEC2','17','raw:ZHBH_TEC2','singlebeam'], // Singlebeam
     ['ZHBH_TEC2','18','raw:ZHBH_TEC2','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZHBH_TEC2','999','raw:ZHBH_TEC2','other'], // Other

     // ZHBH_TEC3 - Base Hgt : Snd Meta : Bathymetric Measurement Technique [3]
     // ['ZHBH_TEC3','-999999',undefined,undefined], // No Information
     ['ZHBH_TEC3','1','raw:ZHBH_TEC3','echo_sounder'], // Echo Sounder
     ['ZHBH_TEC3','2','raw:ZHBH_TEC3','side-scan_sonar'], // Side-scan Sonar
     ['ZHBH_TEC3','3','raw:ZHBH_TEC3','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZHBH_TEC3','4','raw:ZHBH_TEC3','diver'], // Diver
     ['ZHBH_TEC3','5','raw:ZHBH_TEC3','lead-line'], // Lead-line
     ['ZHBH_TEC3','6','raw:ZHBH_TEC3','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZHBH_TEC3','8','raw:ZHBH_TEC3','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZHBH_TEC3','9','raw:ZHBH_TEC3','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZHBH_TEC3','10','raw:ZHBH_TEC3','photogrammetry'], // Photogrammetry
     ['ZHBH_TEC3','11','raw:ZHBH_TEC3','satellite_imagery'], // Satellite Imagery
     ['ZHBH_TEC3','12','raw:ZHBH_TEC3','levelling'], // Levelling
     ['ZHBH_TEC3','13','raw:ZHBH_TEC3','computer_generated'], // Computer Generated
     ['ZHBH_TEC3','14','raw:ZHBH_TEC3','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZHBH_TEC3','15','raw:ZHBH_TEC3','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZHBH_TEC3','17','raw:ZHBH_TEC3','singlebeam'], // Singlebeam
     ['ZHBH_TEC3','18','raw:ZHBH_TEC3','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZHBH_TEC3','999','raw:ZHBH_TEC3','other'], // Other

     // ZHBH_VDC - Base Hgt : Snd Meta : Sounding Datum
     // ['ZHBH_VDC','-999999',undefined,undefined], // No Information
     ['ZHBH_VDC','2','raw:ZHBH_VDC','high_water'], // High Water
     ['ZHBH_VDC','3','raw:ZHBH_VDC','higher_high_water'], // Higher High Water
     ['ZHBH_VDC','4','raw:ZHBH_VDC','indian_spring_low_water'], // Indian Spring Low Water
     ['ZHBH_VDC','5','raw:ZHBH_VDC','low_water'], // Low Water
     ['ZHBH_VDC','6','raw:ZHBH_VDC','lower_low_water'], // Lower Low Water
     ['ZHBH_VDC','7','raw:ZHBH_VDC','mean_high_water'], // Mean High Water
     ['ZHBH_VDC','8','raw:ZHBH_VDC','mean_high_water_neaps'], // Mean High Water Neaps
     ['ZHBH_VDC','9','raw:ZHBH_VDC','mean_high_water_springs'], // Mean High Water Springs
     ['ZHBH_VDC','10','raw:ZHBH_VDC','mean_higher_high_water'], // Mean Higher High Water
     ['ZHBH_VDC','11','raw:ZHBH_VDC','mean_low_water'], // Mean Low Water
     ['ZHBH_VDC','12','raw:ZHBH_VDC','mean_low_water_neaps'], // Mean Low Water Neaps
     ['ZHBH_VDC','13','raw:ZHBH_VDC','mean_low_water_springs'], // Mean Low Water Springs
     ['ZHBH_VDC','14','raw:ZHBH_VDC','mean_lower_low_water'], // Mean Lower Low Water
     ['ZHBH_VDC','15','raw:ZHBH_VDC','mean_sea_level'], // Mean Sea Level
     ['ZHBH_VDC','16','raw:ZHBH_VDC','mean_tide_level'], // Mean Tide Level
     ['ZHBH_VDC','17','raw:ZHBH_VDC','neap_tide'], // Neap Tide
     ['ZHBH_VDC','18','raw:ZHBH_VDC','spring_tide'], // Spring Tide
     ['ZHBH_VDC','19','raw:ZHBH_VDC','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
     ['ZHBH_VDC','20','raw:ZHBH_VDC','lowest_astronomical_tide'], // Lowest Astronomical Tide
     ['ZHBH_VDC','21','raw:ZHBH_VDC','chart_datum_(unspecified)'], // Chart Datum (Unspecified)
     ['ZHBH_VDC','22','raw:ZHBH_VDC','highest_astronomical_tide'], // Highest Astronomical Tide
     ['ZHBH_VDC','24','raw:ZHBH_VDC','mean_higher_high_water_springs'], // Mean Higher High Water Springs
     ['ZHBH_VDC','28','raw:ZHBH_VDC','highest_high_water'], // Highest High Water
     ['ZHBH_VDC','30','raw:ZHBH_VDC','indian_spring_high_water'], // Indian Spring High Water
     ['ZHBH_VDC','90','raw:ZHBH_VDC','lowest_low_water'], // Lowest Low Water
     ['ZHBH_VDC','91','raw:ZHBH_VDC','lowest_low_water_springs'], // Lowest Low Water Springs
     ['ZHBH_VDC','92','raw:ZHBH_VDC','approximate_mean_low_water_springs'], // Approximate Mean Low Water Springs
     ['ZHBH_VDC','93','raw:ZHBH_VDC','low_water_springs'], // Low Water Springs
     ['ZHBH_VDC','94','raw:ZHBH_VDC','approximate_lowest_astronomical_tide'], // Approximate Lowest Astronomical Tide
     ['ZHBH_VDC','95','raw:ZHBH_VDC','nearly_lowest_low_water'], // Nearly Lowest Low Water
     ['ZHBH_VDC','96','raw:ZHBH_VDC','approximate_mean_low_water'], // Approximate Mean Low Water
     ['ZHBH_VDC','97','raw:ZHBH_VDC','approximate_mean_lower_low_water'], // Approximate Mean Lower Low Water
     ['ZHBH_VDC','98','raw:ZHBH_VDC','approximate_mean_sea_level'], // Approximate Mean Sea Level
     ['ZHBH_VDC','99','raw:ZHBH_VDC','high_water_springs'], // High Water Springs
     ['ZHBH_VDC','100','raw:ZHBH_VDC','equinoctial_spring_low_water'], // Equinoctial Spring Low Water
     ['ZHBH_VDC','101','raw:ZHBH_VDC','local_datum'], // Local Datum
     ['ZHBH_VDC','102','raw:ZHBH_VDC','international_great_lakes_datum_1985'], // International Great Lakes Datum 1985
     ['ZHBH_VDC','103','raw:ZHBH_VDC','mean_water_level'], // Mean Water Level
     ['ZHBH_VDC','104','raw:ZHBH_VDC','lower_low_water_large_tide'], // Lower Low Water Large Tide
     ['ZHBH_VDC','105','raw:ZHBH_VDC','higher_high_water_large_tide'], // Higher High Water Large Tide
     ['ZHBH_VDC','107','raw:ZHBH_VDC','nearly_highest_high_water'], // Nearly Highest High Water
     ['ZHBH_VDC','999','raw:ZHBH_VDC','other'], // Other

     // ZHDP_DCR - Hydro Dpth : Snd Meta : Vessel Draft Correction
     // ['ZHDP_DCR','-999999',undefined,undefined], // No Information
     ['ZHDP_DCR','1000','raw:ZHDP_DCR','no'],
     ['ZHDP_DCR','1001','raw:ZHDP_DCR','yes'],

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
     // ['ZHDP_DKC2','-999999',undefined,undefined], // No Information
     ['ZHDP_DKC2','1','depth:hydrographic:quality:2','depth_known'], // Depth Known
     ['ZHDP_DKC2','3','depth:hydrographic:quality:2','safe_clearance_known'], // Safe Clearance Known
     ['ZHDP_DKC2','4','depth:hydrographic:quality:2','depth_unknown'], // Depth Unknown
     ['ZHDP_DKC2','5','depth:hydrographic:quality:2','depth_doubtful'], // Depth Doubtful
     ['ZHDP_DKC2','6','depth:hydrographic:quality:2','depth_unreliable'], // Depth Unreliable
     ['ZHDP_DKC2','7','depth:hydrographic:quality:2','no_bottom'], // No Bottom
     ['ZHDP_DKC2','8','depth:hydrographic:quality:2','least_depth'], // Least Depth
     ['ZHDP_DKC2','9','depth:hydrographic:quality:2','reported_depth'], // Reported Depth
     ['ZHDP_DKC2','10','depth:hydrographic:quality:2','unconfirmed_depth'], // Unconfirmed Depth
     ['ZHDP_DKC2','11','depth:hydrographic:quality:2','maintained_depth'], // Maintained Depth
     ['ZHDP_DKC2','12','depth:hydrographic:quality:2','unmaintained_depth'], // Unmaintained Depth
     ['ZHDP_DKC2','998','depth:hydrographic:quality:2','not_applicable'], // Not Applicable
     ['ZHDP_DKC2','999','depth:hydrographic:quality:2','other'], // Other

     // ZHDP_DKC3 - Hydro Dpth : Snd Meta : Bathymetric Measurement Quality Category [3]
     // ['ZHDP_DKC3','-999999',undefined,undefined], // No Information
     ['ZHDP_DKC3','1','depth:hydrographic:quality:3','depth_known'], // Depth Known
     ['ZHDP_DKC3','3','depth:hydrographic:quality:3','safe_clearance_known'], // Safe Clearance Known
     ['ZHDP_DKC3','4','depth:hydrographic:quality:3','depth_unknown'], // Depth Unknown
     ['ZHDP_DKC3','5','depth:hydrographic:quality:3','depth_doubtful'], // Depth Doubtful
     ['ZHDP_DKC3','6','depth:hydrographic:quality:3','depth_unreliable'], // Depth Unreliable
     ['ZHDP_DKC3','7','depth:hydrographic:quality:3','no_bottom'], // No Bottom
     ['ZHDP_DKC3','8','depth:hydrographic:quality:3','least_depth'], // Least Depth
     ['ZHDP_DKC3','9','depth:hydrographic:quality:3','reported_depth'], // Reported Depth
     ['ZHDP_DKC3','10','depth:hydrographic:quality:3','unconfirmed_depth'], // Unconfirmed Depth
     ['ZHDP_DKC3','11','depth:hydrographic:quality:3','maintained_depth'], // Maintained Depth
     ['ZHDP_DKC3','12','depth:hydrographic:quality:3','unmaintained_depth'], // Unmaintained Depth
     ['ZHDP_DKC3','998','depth:hydrographic:quality:3','not_applicable'], // Not Applicable
     ['ZHDP_DKC3','999','depth:hydrographic:quality:3','other'], // Other

     // ZHDP_SVC - Hydro Dpth : Snd Meta : Sounding Velocity Correction Method
     // ['ZHDP_SVC','-999999',undefined,undefined], // No Information
     ['ZHDP_SVC','1','raw:ZHDP_SVC','sounder_4800_calibrated'], // Sounder 4800 Calibrated
     ['ZHDP_SVC','2','raw:ZHDP_SVC','sounder_1500_calibrated'], // Sounder 1500 Calibrated
     ['ZHDP_SVC','3','raw:ZHDP_SVC','mathews_tables'], // Mathews Tables
     ['ZHDP_SVC','4','raw:ZHDP_SVC','sound_velocity_meter'], // Sound Velocity Meter
     ['ZHDP_SVC','5','raw:ZHDP_SVC','sounder_other_calibrated'], // Sounder Other Calibrated
     ['ZHDP_SVC','998','raw:ZHDP_SVC','not_applicable'], // Not Applicable
     ['ZHDP_SVC','999','raw:ZHDP_SVC','other'], // Other

     // ZHDP_TEC - Hydro Dpth : Snd Meta : Bathymetric Measurement Technique
     // ['ZHDP_TEC','-999999',undefined,undefined], // No Information
     ['ZHDP_TEC','1','raw:ZHDP_TEC','echo_sounder'], // Echo Sounder
     ['ZHDP_TEC','2','raw:ZHDP_TEC','side-scan_sonar'], // Side-scan Sonar
     ['ZHDP_TEC','3','raw:ZHDP_TEC','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZHDP_TEC','4','raw:ZHDP_TEC','diver'], // Diver
     ['ZHDP_TEC','5','raw:ZHDP_TEC','lead-line'], // Lead-line
     ['ZHDP_TEC','6','raw:ZHDP_TEC','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZHDP_TEC','8','raw:ZHDP_TEC','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZHDP_TEC','9','raw:ZHDP_TEC','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZHDP_TEC','10','raw:ZHDP_TEC','photogrammetry'], // Photogrammetry
     ['ZHDP_TEC','11','raw:ZHDP_TEC','satellite_imagery'], // Satellite Imagery
     ['ZHDP_TEC','12','raw:ZHDP_TEC','levelling'], // Levelling
     ['ZHDP_TEC','13','raw:ZHDP_TEC','computer_generated'], // Computer Generated
     ['ZHDP_TEC','14','raw:ZHDP_TEC','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZHDP_TEC','15','raw:ZHDP_TEC','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZHDP_TEC','17','raw:ZHDP_TEC','singlebeam'], // Singlebeam
     ['ZHDP_TEC','18','raw:ZHDP_TEC','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZHDP_TEC','999','raw:ZHDP_TEC','other'], // Other

     // ZHDP_TEC2 - Hydro Dpth : Snd Meta : Bathymetric Measurement Technique [2]
     // ['ZHDP_TEC2','-999999',undefined,undefined], // No Information
     ['ZHDP_TEC2','1','raw:ZHDP_TEC2','echo_sounder'], // Echo Sounder
     ['ZHDP_TEC2','2','raw:ZHDP_TEC2','side-scan_sonar'], // Side-scan Sonar
     ['ZHDP_TEC2','3','raw:ZHDP_TEC2','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZHDP_TEC2','4','raw:ZHDP_TEC2','diver'], // Diver
     ['ZHDP_TEC2','5','raw:ZHDP_TEC2','lead-line'], // Lead-line
     ['ZHDP_TEC2','6','raw:ZHDP_TEC2','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZHDP_TEC2','8','raw:ZHDP_TEC2','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZHDP_TEC2','9','raw:ZHDP_TEC2','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZHDP_TEC2','10','raw:ZHDP_TEC2','photogrammetry'], // Photogrammetry
     ['ZHDP_TEC2','11','raw:ZHDP_TEC2','satellite_imagery'], // Satellite Imagery
     ['ZHDP_TEC2','12','raw:ZHDP_TEC2','levelling'], // Levelling
     ['ZHDP_TEC2','13','raw:ZHDP_TEC2','computer_generated'], // Computer Generated
     ['ZHDP_TEC2','14','raw:ZHDP_TEC2','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZHDP_TEC2','15','raw:ZHDP_TEC2','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZHDP_TEC2','17','raw:ZHDP_TEC2','singlebeam'], // Singlebeam
     ['ZHDP_TEC2','18','raw:ZHDP_TEC2','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZHDP_TEC2','999','raw:ZHDP_TEC2','other'], // Other

     // ZHDP_TEC3 - Hydro Dpth : Snd Meta : Bathymetric Measurement Technique [3]
     // ['ZHDP_TEC3','-999999',undefined,undefined], // No Information
     ['ZHDP_TEC3','1','raw:ZHDP_TEC3','echo_sounder'], // Echo Sounder
     ['ZHDP_TEC3','2','raw:ZHDP_TEC3','side-scan_sonar'], // Side-scan Sonar
     ['ZHDP_TEC3','3','raw:ZHDP_TEC3','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZHDP_TEC3','4','raw:ZHDP_TEC3','diver'], // Diver
     ['ZHDP_TEC3','5','raw:ZHDP_TEC3','lead-line'], // Lead-line
     ['ZHDP_TEC3','6','raw:ZHDP_TEC3','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZHDP_TEC3','8','raw:ZHDP_TEC3','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZHDP_TEC3','9','raw:ZHDP_TEC3','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZHDP_TEC3','10','raw:ZHDP_TEC3','photogrammetry'], // Photogrammetry
     ['ZHDP_TEC3','11','raw:ZHDP_TEC3','satellite_imagery'], // Satellite Imagery
     ['ZHDP_TEC3','12','raw:ZHDP_TEC3','levelling'], // Levelling
     ['ZHDP_TEC3','13','raw:ZHDP_TEC3','computer_generated'], // Computer Generated
     ['ZHDP_TEC3','14','raw:ZHDP_TEC3','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZHDP_TEC3','15','raw:ZHDP_TEC3','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZHDP_TEC3','17','raw:ZHDP_TEC3','singlebeam'], // Singlebeam
     ['ZHDP_TEC3','18','raw:ZHDP_TEC3','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZHDP_TEC3','999','raw:ZHDP_TEC3','other'], // Other

     // ZHDP_VDC - Hydro Dpth : Snd Meta : Sounding Datum
     // ['ZHDP_VDC','-999999',undefined,undefined], // No Information
     ['ZHDP_VDC','2','raw:ZHDP_VDC','high_water'], // High Water
     ['ZHDP_VDC','3','raw:ZHDP_VDC','higher_high_water'], // Higher High Water
     ['ZHDP_VDC','4','raw:ZHDP_VDC','indian_spring_low_water'], // Indian Spring Low Water
     ['ZHDP_VDC','5','raw:ZHDP_VDC','low_water'], // Low Water
     ['ZHDP_VDC','6','raw:ZHDP_VDC','lower_low_water'], // Lower Low Water
     ['ZHDP_VDC','7','raw:ZHDP_VDC','mean_high_water'], // Mean High Water
     ['ZHDP_VDC','8','raw:ZHDP_VDC','mean_high_water_neaps'], // Mean High Water Neaps
     ['ZHDP_VDC','9','raw:ZHDP_VDC','mean_high_water_springs'], // Mean High Water Springs
     ['ZHDP_VDC','10','raw:ZHDP_VDC','mean_higher_high_water'], // Mean Higher High Water
     ['ZHDP_VDC','11','raw:ZHDP_VDC','mean_low_water'], // Mean Low Water
     ['ZHDP_VDC','12','raw:ZHDP_VDC','mean_low_water_neaps'], // Mean Low Water Neaps
     ['ZHDP_VDC','13','raw:ZHDP_VDC','mean_low_water_springs'], // Mean Low Water Springs
     ['ZHDP_VDC','14','raw:ZHDP_VDC','mean_lower_low_water'], // Mean Lower Low Water
     ['ZHDP_VDC','15','raw:ZHDP_VDC','mean_sea_level'], // Mean Sea Level
     ['ZHDP_VDC','16','raw:ZHDP_VDC','mean_tide_level'], // Mean Tide Level
     ['ZHDP_VDC','17','raw:ZHDP_VDC','neap_tide'], // Neap Tide
     ['ZHDP_VDC','18','raw:ZHDP_VDC','spring_tide'], // Spring Tide
     ['ZHDP_VDC','19','raw:ZHDP_VDC','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
     ['ZHDP_VDC','20','raw:ZHDP_VDC','lowest_astronomical_tide'], // Lowest Astronomical Tide
     ['ZHDP_VDC','21','raw:ZHDP_VDC','chart_datum_(unspecified)'], // Chart Datum (Unspecified)
     ['ZHDP_VDC','22','raw:ZHDP_VDC','highest_astronomical_tide'], // Highest Astronomical Tide
     ['ZHDP_VDC','24','raw:ZHDP_VDC','mean_higher_high_water_springs'], // Mean Higher High Water Springs
     ['ZHDP_VDC','28','raw:ZHDP_VDC','highest_high_water'], // Highest High Water
     ['ZHDP_VDC','30','raw:ZHDP_VDC','indian_spring_high_water'], // Indian Spring High Water
     ['ZHDP_VDC','90','raw:ZHDP_VDC','lowest_low_water'], // Lowest Low Water
     ['ZHDP_VDC','91','raw:ZHDP_VDC','lowest_low_water_springs'], // Lowest Low Water Springs
     ['ZHDP_VDC','92','raw:ZHDP_VDC','approximate_mean_low_water_springs'], // Approximate Mean Low Water Springs
     ['ZHDP_VDC','93','raw:ZHDP_VDC','low_water_springs'], // Low Water Springs
     ['ZHDP_VDC','94','raw:ZHDP_VDC','approximate_lowest_astronomical_tide'], // Approximate Lowest Astronomical Tide
     ['ZHDP_VDC','95','raw:ZHDP_VDC','nearly_lowest_low_water'], // Nearly Lowest Low Water
     ['ZHDP_VDC','96','raw:ZHDP_VDC','approximate_mean_low_water'], // Approximate Mean Low Water
     ['ZHDP_VDC','97','raw:ZHDP_VDC','approximate_mean_lower_low_water'], // Approximate Mean Lower Low Water
     ['ZHDP_VDC','98','raw:ZHDP_VDC','approximate_mean_sea_level'], // Approximate Mean Sea Level
     ['ZHDP_VDC','99','raw:ZHDP_VDC','high_water_springs'], // High Water Springs
     ['ZHDP_VDC','100','raw:ZHDP_VDC','equinoctial_spring_low_water'], // Equinoctial Spring Low Water
     ['ZHDP_VDC','101','raw:ZHDP_VDC','local_datum'], // Local Datum
     ['ZHDP_VDC','102','raw:ZHDP_VDC','international_great_lakes_datum_1985'], // International Great Lakes Datum 1985
     ['ZHDP_VDC','103','raw:ZHDP_VDC','mean_water_level'], // Mean Water Level
     ['ZHDP_VDC','104','raw:ZHDP_VDC','lower_low_water_large_tide'], // Lower Low Water Large Tide
     ['ZHDP_VDC','105','raw:ZHDP_VDC','higher_high_water_large_tide'], // Higher High Water Large Tide
     ['ZHDP_VDC','107','raw:ZHDP_VDC','nearly_highest_high_water'], // Nearly Highest High Water
     ['ZHDP_VDC','999','raw:ZHDP_VDC','other'], // Other

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
     ['ZI005_GNT','1','raw:ZI005_GNT','conventional'], // Conventional
     ['ZI005_GNT','2','raw:ZI005_GNT','native_transliterated'], // Native Transliterated
     ['ZI005_GNT','3','raw:ZI005_GNT','variant_transliterated'], // Variant Transliterated
     ['ZI005_GNT','4','raw:ZI005_GNT','unverified_transliterated'], // Unverified Transliterated
     ['ZI005_GNT','5','raw:ZI005_GNT','native_original'], // Native Original
     ['ZI005_GNT','6','raw:ZI005_GNT','variant_original'], // Variant Original
     ['ZI005_GNT','7','raw:ZI005_GNT','unverified_original'], // Unverified Original
     ['ZI005_GNT','8','raw:ZI005_GNT','historical_transliterated'], // Historical Transliterated
     ['ZI005_GNT','9','raw:ZI005_GNT','historical_original'], // Historical Original
     ['ZI005_GNT','10','raw:ZI005_GNT','provisional'], // Provisional
     ['ZI005_GNT','999','raw:ZI005_GNT','other'], // Other

     // ZI005_GNT2 - Geo Name Info : Geographic Name Type [2]
     // ['ZI005_GNT2','-999999',undefined,undefined], // No Information
     ['ZI005_GNT2','1','raw:ZI005_GNT2','conventional'], // Conventional
     ['ZI005_GNT2','2','raw:ZI005_GNT2','native_transliterated'], // Native Transliterated
     ['ZI005_GNT2','3','raw:ZI005_GNT2','variant_transliterated'], // Variant Transliterated
     ['ZI005_GNT2','4','raw:ZI005_GNT2','unverified_transliterated'], // Unverified Transliterated
     ['ZI005_GNT2','5','raw:ZI005_GNT2','native_original'], // Native Original
     ['ZI005_GNT2','6','raw:ZI005_GNT2','variant_original'], // Variant Original
     ['ZI005_GNT2','7','raw:ZI005_GNT2','unverified_original'], // Unverified Original
     ['ZI005_GNT2','8','raw:ZI005_GNT2','historical_transliterated'], // Historical Transliterated
     ['ZI005_GNT2','9','raw:ZI005_GNT2','historical_original'], // Historical Original
     ['ZI005_GNT2','10','raw:ZI005_GNT2','provisional'], // Provisional
     ['ZI005_GNT2','999','raw:ZI005_GNT2','other'], // Other

     // ZI005_GNT3 - Geo Name Info : Geographic Name Type [3]
     // ['ZI005_GNT3','-999999',undefined,undefined], // No Information
     ['ZI005_GNT3','1','raw:ZI005_GNT3','conventional'], // Conventional
     ['ZI005_GNT3','2','raw:ZI005_GNT3','native_transliterated'], // Native Transliterated
     ['ZI005_GNT3','3','raw:ZI005_GNT3','variant_transliterated'], // Variant Transliterated
     ['ZI005_GNT3','4','raw:ZI005_GNT3','unverified_transliterated'], // Unverified Transliterated
     ['ZI005_GNT3','5','raw:ZI005_GNT3','native_original'], // Native Original
     ['ZI005_GNT3','6','raw:ZI005_GNT3','variant_original'], // Variant Original
     ['ZI005_GNT3','7','raw:ZI005_GNT3','unverified_original'], // Unverified Original
     ['ZI005_GNT3','8','raw:ZI005_GNT3','historical_transliterated'], // Historical Transliterated
     ['ZI005_GNT3','9','raw:ZI005_GNT3','historical_original'], // Historical Original
     ['ZI005_GNT3','10','raw:ZI005_GNT3','provisional'], // Provisional
     ['ZI005_GNT3','999','raw:ZI005_GNT3','other'], // Other

     // ZI005_GNTA - Geo Name Info (1) : Geographic Name Type (first side)
     // ['ZI005_GNTA','-999999',undefined,undefined], // No Information
     ['ZI005_GNTA','1','raw:ZI005_GNTA','conventional'], // Conventional
     ['ZI005_GNTA','2','raw:ZI005_GNTA','native_transliterated'], // Native Transliterated
     ['ZI005_GNTA','3','raw:ZI005_GNTA','variant_transliterated'], // Variant Transliterated
     ['ZI005_GNTA','4','raw:ZI005_GNTA','unverified_transliterated'], // Unverified Transliterated
     ['ZI005_GNTA','5','raw:ZI005_GNTA','native_original'], // Native Original
     ['ZI005_GNTA','6','raw:ZI005_GNTA','variant_original'], // Variant Original
     ['ZI005_GNTA','7','raw:ZI005_GNTA','unverified_original'], // Unverified Original
     ['ZI005_GNTA','8','raw:ZI005_GNTA','historical_transliterated'], // Historical Transliterated
     ['ZI005_GNTA','9','raw:ZI005_GNTA','historical_original'], // Historical Original
     ['ZI005_GNTA','10','raw:ZI005_GNTA','provisional'], // Provisional
     ['ZI005_GNTA','999','raw:ZI005_GNTA','other'], // Other

     // ZI005_GNTB - Geo Name Info (2) : Geographic Name Type (second side)
     // ['ZI005_GNTB','-999999',undefined,undefined], // No Information
     ['ZI005_GNTB','1','raw:ZI005_GNTB','conventional'], // Conventional
     ['ZI005_GNTB','2','raw:ZI005_GNTB','native_transliterated'], // Native Transliterated
     ['ZI005_GNTB','3','raw:ZI005_GNTB','variant_transliterated'], // Variant Transliterated
     ['ZI005_GNTB','4','raw:ZI005_GNTB','unverified_transliterated'], // Unverified Transliterated
     ['ZI005_GNTB','5','raw:ZI005_GNTB','native_original'], // Native Original
     ['ZI005_GNTB','6','raw:ZI005_GNTB','variant_original'], // Variant Original
     ['ZI005_GNTB','7','raw:ZI005_GNTB','unverified_original'], // Unverified Original
     ['ZI005_GNTB','8','raw:ZI005_GNTB','historical_transliterated'], // Historical Transliterated
     ['ZI005_GNTB','9','raw:ZI005_GNTB','historical_original'], // Historical Original
     ['ZI005_GNTB','10','raw:ZI005_GNTB','provisional'], // Provisional
     ['ZI005_GNTB','999','raw:ZI005_GNTB','other'], // Other

     // ZI012_DCR - Dpth Crv : Snd Meta : Vessel Draft Correction
     // ['ZI012_DCR','-999999',undefined,undefined], // No Information
     ['ZI012_DCR','1000','raw:ZI012_DCR','no'],
     ['ZI012_DCR','1001','raw:ZI012_DCR','yes'],

     // ZI012_DKC - Dpth Crv : Snd Meta : Bathymetric Measurement Quality Category
     // ['ZI012_DKC','-999999',undefined,undefined], // No Information
     ['ZI012_DKC','1','raw:ZI012_DKC','depth_known'], // Depth Known
     ['ZI012_DKC','3','raw:ZI012_DKC','safe_clearance_known'], // Safe Clearance Known
     ['ZI012_DKC','4','raw:ZI012_DKC','depth_unknown'], // Depth Unknown
     ['ZI012_DKC','5','raw:ZI012_DKC','depth_doubtful'], // Depth Doubtful
     ['ZI012_DKC','6','raw:ZI012_DKC','depth_unreliable'], // Depth Unreliable
     ['ZI012_DKC','7','raw:ZI012_DKC','no_bottom'], // No Bottom
     ['ZI012_DKC','8','raw:ZI012_DKC','least_depth'], // Least Depth
     ['ZI012_DKC','9','raw:ZI012_DKC','reported_depth'], // Reported Depth
     ['ZI012_DKC','10','raw:ZI012_DKC','unconfirmed_depth'], // Unconfirmed Depth
     ['ZI012_DKC','11','raw:ZI012_DKC','maintained_depth'], // Maintained Depth
     ['ZI012_DKC','12','raw:ZI012_DKC','unmaintained_depth'], // Unmaintained Depth
     ['ZI012_DKC','998','raw:ZI012_DKC','not_applicable'], // Not Applicable
     ['ZI012_DKC','999','raw:ZI012_DKC','other'], // Other

     // ZI012_DKC2 - Dpth Crv : Snd Meta : Bathymetric Measurement Quality Category [2]
     // ['ZI012_DKC2','-999999',undefined,undefined], // No Information
     ['ZI012_DKC2','1','raw:ZI012_DKC2','depth_known'], // Depth Known
     ['ZI012_DKC2','3','raw:ZI012_DKC2','safe_clearance_known'], // Safe Clearance Known
     ['ZI012_DKC2','4','raw:ZI012_DKC2','depth_unknown'], // Depth Unknown
     ['ZI012_DKC2','5','raw:ZI012_DKC2','depth_doubtful'], // Depth Doubtful
     ['ZI012_DKC2','6','raw:ZI012_DKC2','depth_unreliable'], // Depth Unreliable
     ['ZI012_DKC2','7','raw:ZI012_DKC2','no_bottom'], // No Bottom
     ['ZI012_DKC2','8','raw:ZI012_DKC2','least_depth'], // Least Depth
     ['ZI012_DKC2','9','raw:ZI012_DKC2','reported_depth'], // Reported Depth
     ['ZI012_DKC2','10','raw:ZI012_DKC2','unconfirmed_depth'], // Unconfirmed Depth
     ['ZI012_DKC2','11','raw:ZI012_DKC2','maintained_depth'], // Maintained Depth
     ['ZI012_DKC2','12','raw:ZI012_DKC2','unmaintained_depth'], // Unmaintained Depth
     ['ZI012_DKC2','998','raw:ZI012_DKC2','not_applicable'], // Not Applicable
     ['ZI012_DKC2','999','raw:ZI012_DKC2','other'], // Other

     // ZI012_DKC3 - Dpth Crv : Snd Meta : Bathymetric Measurement Quality Category [3]
     // ['ZI012_DKC3','-999999',undefined,undefined], // No Information
     ['ZI012_DKC3','1','raw:ZI012_DKC3','depth_known'], // Depth Known
     ['ZI012_DKC3','3','raw:ZI012_DKC3','safe_clearance_known'], // Safe Clearance Known
     ['ZI012_DKC3','4','raw:ZI012_DKC3','depth_unknown'], // Depth Unknown
     ['ZI012_DKC3','5','raw:ZI012_DKC3','depth_doubtful'], // Depth Doubtful
     ['ZI012_DKC3','6','raw:ZI012_DKC3','depth_unreliable'], // Depth Unreliable
     ['ZI012_DKC3','7','raw:ZI012_DKC3','no_bottom'], // No Bottom
     ['ZI012_DKC3','8','raw:ZI012_DKC3','least_depth'], // Least Depth
     ['ZI012_DKC3','9','raw:ZI012_DKC3','reported_depth'], // Reported Depth
     ['ZI012_DKC3','10','raw:ZI012_DKC3','unconfirmed_depth'], // Unconfirmed Depth
     ['ZI012_DKC3','11','raw:ZI012_DKC3','maintained_depth'], // Maintained Depth
     ['ZI012_DKC3','12','raw:ZI012_DKC3','unmaintained_depth'], // Unmaintained Depth
     ['ZI012_DKC3','998','raw:ZI012_DKC3','not_applicable'], // Not Applicable
     ['ZI012_DKC3','999','raw:ZI012_DKC3','other'], // Other

     // ZI012_SVC - Dpth Crv : Snd Meta : Sounding Velocity Correction Method
     // ['ZI012_SVC','-999999',undefined,undefined], // No Information
     ['ZI012_SVC','1','raw:ZI012_SVC','sounder_4800_calibrated'], // Sounder 4800 Calibrated
     ['ZI012_SVC','2','raw:ZI012_SVC','sounder_1500_calibrated'], // Sounder 1500 Calibrated
     ['ZI012_SVC','3','raw:ZI012_SVC','mathews_tables'], // Mathews Tables
     ['ZI012_SVC','4','raw:ZI012_SVC','sound_velocity_meter'], // Sound Velocity Meter
     ['ZI012_SVC','5','raw:ZI012_SVC','sounder_other_calibrated'], // Sounder Other Calibrated
     ['ZI012_SVC','998','raw:ZI012_SVC','not_applicable'], // Not Applicable
     ['ZI012_SVC','999','raw:ZI012_SVC','other'], // Other

     // ZI012_TEC - Dpth Crv : Snd Meta : Bathymetric Measurement Technique
     // ['ZI012_TEC','-999999',undefined,undefined], // No Information
     ['ZI012_TEC','1','raw:ZI012_TEC','echo_sounder'], // Echo Sounder
     ['ZI012_TEC','2','raw:ZI012_TEC','side-scan_sonar'], // Side-scan Sonar
     ['ZI012_TEC','3','raw:ZI012_TEC','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZI012_TEC','4','raw:ZI012_TEC','diver'], // Diver
     ['ZI012_TEC','5','raw:ZI012_TEC','lead-line'], // Lead-line
     ['ZI012_TEC','6','raw:ZI012_TEC','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZI012_TEC','8','raw:ZI012_TEC','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZI012_TEC','9','raw:ZI012_TEC','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZI012_TEC','10','raw:ZI012_TEC','photogrammetry'], // Photogrammetry
     ['ZI012_TEC','11','raw:ZI012_TEC','satellite_imagery'], // Satellite Imagery
     ['ZI012_TEC','12','raw:ZI012_TEC','levelling'], // Levelling
     ['ZI012_TEC','13','raw:ZI012_TEC','computer_generated'], // Computer Generated
     ['ZI012_TEC','14','raw:ZI012_TEC','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZI012_TEC','15','raw:ZI012_TEC','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZI012_TEC','17','raw:ZI012_TEC','singlebeam'], // Singlebeam
     ['ZI012_TEC','18','raw:ZI012_TEC','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZI012_TEC','999','raw:ZI012_TEC','other'], // Other

     // ZI012_TEC2 - Dpth Crv : Snd Meta : Bathymetric Measurement Technique [2]
     // ['ZI012_TEC2','-999999',undefined,undefined], // No Information
     ['ZI012_TEC2','1','raw:ZI012_TEC2','echo_sounder'], // Echo Sounder
     ['ZI012_TEC2','2','raw:ZI012_TEC2','side-scan_sonar'], // Side-scan Sonar
     ['ZI012_TEC2','3','raw:ZI012_TEC2','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZI012_TEC2','4','raw:ZI012_TEC2','diver'], // Diver
     ['ZI012_TEC2','5','raw:ZI012_TEC2','lead-line'], // Lead-line
     ['ZI012_TEC2','6','raw:ZI012_TEC2','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZI012_TEC2','8','raw:ZI012_TEC2','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZI012_TEC2','9','raw:ZI012_TEC2','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZI012_TEC2','10','raw:ZI012_TEC2','photogrammetry'], // Photogrammetry
     ['ZI012_TEC2','11','raw:ZI012_TEC2','satellite_imagery'], // Satellite Imagery
     ['ZI012_TEC2','12','raw:ZI012_TEC2','levelling'], // Levelling
     ['ZI012_TEC2','13','raw:ZI012_TEC2','computer_generated'], // Computer Generated
     ['ZI012_TEC2','14','raw:ZI012_TEC2','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZI012_TEC2','15','raw:ZI012_TEC2','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZI012_TEC2','17','raw:ZI012_TEC2','singlebeam'], // Singlebeam
     ['ZI012_TEC2','18','raw:ZI012_TEC2','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZI012_TEC2','999','raw:ZI012_TEC2','other'], // Other

     // ZI012_TEC3 - Dpth Crv : Snd Meta : Bathymetric Measurement Technique [3]
     // ['ZI012_TEC3','-999999',undefined,undefined], // No Information
     ['ZI012_TEC3','1','raw:ZI012_TEC3','echo_sounder'], // Echo Sounder
     ['ZI012_TEC3','2','raw:ZI012_TEC3','side-scan_sonar'], // Side-scan Sonar
     ['ZI012_TEC3','3','raw:ZI012_TEC3','multi-beam_echo_sounder'], // Multi-beam Echo Sounder
     ['ZI012_TEC3','4','raw:ZI012_TEC3','diver'], // Diver
     ['ZI012_TEC3','5','raw:ZI012_TEC3','lead-line'], // Lead-line
     ['ZI012_TEC3','6','raw:ZI012_TEC3','area_swept_by_wire-drag'], // Area Swept by Wire-drag
     ['ZI012_TEC3','8','raw:ZI012_TEC3','area_swept_by_vertical_acoustic_system'], // Area Swept by Vertical Acoustic System
     ['ZI012_TEC3','9','raw:ZI012_TEC3','electromagnetic_sensor'], // Electromagnetic Sensor
     ['ZI012_TEC3','10','raw:ZI012_TEC3','photogrammetry'], // Photogrammetry
     ['ZI012_TEC3','11','raw:ZI012_TEC3','satellite_imagery'], // Satellite Imagery
     ['ZI012_TEC3','12','raw:ZI012_TEC3','levelling'], // Levelling
     ['ZI012_TEC3','13','raw:ZI012_TEC3','computer_generated'], // Computer Generated
     ['ZI012_TEC3','14','raw:ZI012_TEC3','area_swept_by_side-scan_sonar'], // Area Swept by Side-scan Sonar
     ['ZI012_TEC3','15','raw:ZI012_TEC3','bathymetric_lidar'], // Bathymetric LiDAR
     ['ZI012_TEC3','17','raw:ZI012_TEC3','singlebeam'], // Singlebeam
     ['ZI012_TEC3','18','raw:ZI012_TEC3','underwater_utility_vehicle'], // Underwater Utility Vehicle
     ['ZI012_TEC3','999','raw:ZI012_TEC3','other'], // Other

     // ZI012_VDC - Dpth Crv : Snd Meta : Sounding Datum
     // ['ZI012_VDC','-999999',undefined,undefined], // No Information
     ['ZI012_VDC','2','raw:ZI012_VDC','high_water'], // High Water
     ['ZI012_VDC','3','raw:ZI012_VDC','higher_high_water'], // Higher High Water
     ['ZI012_VDC','4','raw:ZI012_VDC','indian_spring_low_water'], // Indian Spring Low Water
     ['ZI012_VDC','5','raw:ZI012_VDC','low_water'], // Low Water
     ['ZI012_VDC','6','raw:ZI012_VDC','lower_low_water'], // Lower Low Water
     ['ZI012_VDC','7','raw:ZI012_VDC','mean_high_water'], // Mean High Water
     ['ZI012_VDC','8','raw:ZI012_VDC','mean_high_water_neaps'], // Mean High Water Neaps
     ['ZI012_VDC','9','raw:ZI012_VDC','mean_high_water_springs'], // Mean High Water Springs
     ['ZI012_VDC','10','raw:ZI012_VDC','mean_higher_high_water'], // Mean Higher High Water
     ['ZI012_VDC','11','raw:ZI012_VDC','mean_low_water'], // Mean Low Water
     ['ZI012_VDC','12','raw:ZI012_VDC','mean_low_water_neaps'], // Mean Low Water Neaps
     ['ZI012_VDC','13','raw:ZI012_VDC','mean_low_water_springs'], // Mean Low Water Springs
     ['ZI012_VDC','14','raw:ZI012_VDC','mean_lower_low_water'], // Mean Lower Low Water
     ['ZI012_VDC','15','raw:ZI012_VDC','mean_sea_level'], // Mean Sea Level
     ['ZI012_VDC','16','raw:ZI012_VDC','mean_tide_level'], // Mean Tide Level
     ['ZI012_VDC','17','raw:ZI012_VDC','neap_tide'], // Neap Tide
     ['ZI012_VDC','18','raw:ZI012_VDC','spring_tide'], // Spring Tide
     ['ZI012_VDC','19','raw:ZI012_VDC','mean_lower_low_water_springs'], // Mean Lower Low Water Springs
     ['ZI012_VDC','20','raw:ZI012_VDC','lowest_astronomical_tide'], // Lowest Astronomical Tide
     ['ZI012_VDC','21','raw:ZI012_VDC','chart_datum_(unspecified)'], // Chart Datum (Unspecified)
     ['ZI012_VDC','22','raw:ZI012_VDC','highest_astronomical_tide'], // Highest Astronomical Tide
     ['ZI012_VDC','24','raw:ZI012_VDC','mean_higher_high_water_springs'], // Mean Higher High Water Springs
     ['ZI012_VDC','28','raw:ZI012_VDC','highest_high_water'], // Highest High Water
     ['ZI012_VDC','30','raw:ZI012_VDC','indian_spring_high_water'], // Indian Spring High Water
     ['ZI012_VDC','90','raw:ZI012_VDC','lowest_low_water'], // Lowest Low Water
     ['ZI012_VDC','91','raw:ZI012_VDC','lowest_low_water_springs'], // Lowest Low Water Springs
     ['ZI012_VDC','92','raw:ZI012_VDC','approximate_mean_low_water_springs'], // Approximate Mean Low Water Springs
     ['ZI012_VDC','93','raw:ZI012_VDC','low_water_springs'], // Low Water Springs
     ['ZI012_VDC','94','raw:ZI012_VDC','approximate_lowest_astronomical_tide'], // Approximate Lowest Astronomical Tide
     ['ZI012_VDC','95','raw:ZI012_VDC','nearly_lowest_low_water'], // Nearly Lowest Low Water
     ['ZI012_VDC','96','raw:ZI012_VDC','approximate_mean_low_water'], // Approximate Mean Low Water
     ['ZI012_VDC','97','raw:ZI012_VDC','approximate_mean_lower_low_water'], // Approximate Mean Lower Low Water
     ['ZI012_VDC','98','raw:ZI012_VDC','approximate_mean_sea_level'], // Approximate Mean Sea Level
     ['ZI012_VDC','99','raw:ZI012_VDC','high_water_springs'], // High Water Springs
     ['ZI012_VDC','100','raw:ZI012_VDC','equinoctial_spring_low_water'], // Equinoctial Spring Low Water
     ['ZI012_VDC','101','raw:ZI012_VDC','local_datum'], // Local Datum
     ['ZI012_VDC','102','raw:ZI012_VDC','international_great_lakes_datum_1985'], // International Great Lakes Datum 1985
     ['ZI012_VDC','103','raw:ZI012_VDC','mean_water_level'], // Mean Water Level
     ['ZI012_VDC','104','raw:ZI012_VDC','lower_low_water_large_tide'], // Lower Low Water Large Tide
     ['ZI012_VDC','105','raw:ZI012_VDC','higher_high_water_large_tide'], // Higher High Water Large Tide
     ['ZI012_VDC','107','raw:ZI012_VDC','nearly_highest_high_water'], // Nearly Highest High Water
     ['ZI012_VDC','999','raw:ZI012_VDC','other'], // Other

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
     // ['ZI013_CSP2','-999999',undefined,undefined], // No Information
     ['ZI013_CSP2','1','crop:2','almond'], // Almond
     ['ZI013_CSP2','2','crop:2','apple'], // Apple
     ['ZI013_CSP2','3','crop:2','barley'], // Barley
     ['ZI013_CSP2','4','crop:2','berry'], // Berry
     ['ZI013_CSP2','5','crop:2','carob'], // Carob
     ['ZI013_CSP2','6','crop:2','chestnut'], // Chestnut
     ['ZI013_CSP2','7','crop:2','citrus'], // Citrus
     ['ZI013_CSP2','8','crop:2','cork-oak'], // Cork-Oak
     ['ZI013_CSP2','9','crop:2','cacao'], // Cacao
     ['ZI013_CSP2','10','crop:2','coffee'], // Coffee
     ['ZI013_CSP2','11','crop:2','maize'], // Maize
     ['ZI013_CSP2','13','crop:2','dry_crop'], // Dry Crop
     ['ZI013_CSP2','14','crop:2','fibre_crop'], // Fibre Crop
     ['ZI013_CSP2','15','crop:2','fruit_tree'], // Fruit Tree
     ['ZI013_CSP2','17','crop:2','hazelnut'], // Hazelnut
     ['ZI013_CSP2','19','crop:2','maple'], // Maple
     ['ZI013_CSP2','20','crop:2','millet'], // Millet
     ['ZI013_CSP2','21','crop:2','oat'], // Oat
     ['ZI013_CSP2','22','crop:2','oil_crop'], // Oil Crop
     ['ZI013_CSP2','23','crop:2','olive'], // Olive
     ['ZI013_CSP2','24','crop:2','peach'], // Peach
     ['ZI013_CSP2','25','crop:2','peanut'], // Peanut
     ['ZI013_CSP2','26','crop:2','potato'], // Potato
     ['ZI013_CSP2','27','crop:2','pulse'], // Pulse
     ['ZI013_CSP2','29','crop:2','rubber'], // Rubber
     ['ZI013_CSP2','30','crop:2','rye'], // Rye
     ['ZI013_CSP2','31','crop:2','sisal'], // Sisal
     ['ZI013_CSP2','32','crop:2','sorghum'], // Sorghum
     ['ZI013_CSP2','33','crop:2','sugar_crop'], // Sugar Crop
     ['ZI013_CSP2','34','crop:2','tea'], // Tea
     ['ZI013_CSP2','35','crop:2','tobacco'], // Tobacco
     ['ZI013_CSP2','36','crop:2','tuber'], // Tuber
     ['ZI013_CSP2','37','crop:2','vegetable_crop'], // Vegetable Crop
     ['ZI013_CSP2','38','crop:2','walnut'], // Walnut
     ['ZI013_CSP2','39','crop:2','wheat'], // Wheat
     ['ZI013_CSP2','40','crop:2','date'], // Date
     ['ZI013_CSP2','41','crop:2','banana'], // Banana
     ['ZI013_CSP2','42','crop:2','oil_palm'], // Oil Palm
     ['ZI013_CSP2','43','crop:2','coconut'], // Coconut
     ['ZI013_CSP2','44','crop:2','sugar_cane'], // Sugar Cane
     ['ZI013_CSP2','45','crop:2','cotton'], // Cotton
     ['ZI013_CSP2','46','crop:2','bamboo'], // Bamboo
     ['ZI013_CSP2','47','crop:2','palmetto'], // Palmetto
     ['ZI013_CSP2','48','crop:2','timber'], // Timber
     ['ZI013_CSP2','117','crop:2','opium_poppy'], // Opium Poppy
     ['ZI013_CSP2','152','crop:2','cannabis'], // Cannabis
     ['ZI013_CSP2','159','crop:2','ornamental_crop'], // Ornamental Crop
     ['ZI013_CSP2','999','crop:2','other'], // Other

     // ZI013_CSP3 - Crop Information : Crop Species [3]
     // ['ZI013_CSP3','-999999',undefined,undefined], // No Information
     ['ZI013_CSP3','1','crop:3','almond'], // Almond
     ['ZI013_CSP3','2','crop:3','apple'], // Apple
     ['ZI013_CSP3','3','crop:3','barley'], // Barley
     ['ZI013_CSP3','4','crop:3','berry'], // Berry
     ['ZI013_CSP3','5','crop:3','carob'], // Carob
     ['ZI013_CSP3','6','crop:3','chestnut'], // Chestnut
     ['ZI013_CSP3','7','crop:3','citrus'], // Citrus
     ['ZI013_CSP3','8','crop:3','cork-oak'], // Cork-Oak
     ['ZI013_CSP3','9','crop:3','cacao'], // Cacao
     ['ZI013_CSP3','10','crop:3','coffee'], // Coffee
     ['ZI013_CSP3','11','crop:3','maize'], // Maize
     ['ZI013_CSP3','13','crop:3','dry_crop'], // Dry Crop
     ['ZI013_CSP3','14','crop:3','fibre_crop'], // Fibre Crop
     ['ZI013_CSP3','15','crop:3','fruit_tree'], // Fruit Tree
     ['ZI013_CSP3','17','crop:3','hazelnut'], // Hazelnut
     ['ZI013_CSP3','19','crop:3','maple'], // Maple
     ['ZI013_CSP3','20','crop:3','millet'], // Millet
     ['ZI013_CSP3','21','crop:3','oat'], // Oat
     ['ZI013_CSP3','22','crop:3','oil_crop'], // Oil Crop
     ['ZI013_CSP3','23','crop:3','olive'], // Olive
     ['ZI013_CSP3','24','crop:3','peach'], // Peach
     ['ZI013_CSP3','25','crop:3','peanut'], // Peanut
     ['ZI013_CSP3','26','crop:3','potato'], // Potato
     ['ZI013_CSP3','27','crop:3','pulse'], // Pulse
     ['ZI013_CSP3','29','crop:3','rubber'], // Rubber
     ['ZI013_CSP3','30','crop:3','rye'], // Rye
     ['ZI013_CSP3','31','crop:3','sisal'], // Sisal
     ['ZI013_CSP3','32','crop:3','sorghum'], // Sorghum
     ['ZI013_CSP3','33','crop:3','sugar_crop'], // Sugar Crop
     ['ZI013_CSP3','34','crop:3','tea'], // Tea
     ['ZI013_CSP3','35','crop:3','tobacco'], // Tobacco
     ['ZI013_CSP3','36','crop:3','tuber'], // Tuber
     ['ZI013_CSP3','37','crop:3','vegetable_crop'], // Vegetable Crop
     ['ZI013_CSP3','38','crop:3','walnut'], // Walnut
     ['ZI013_CSP3','39','crop:3','wheat'], // Wheat
     ['ZI013_CSP3','40','crop:3','date'], // Date
     ['ZI013_CSP3','41','crop:3','banana'], // Banana
     ['ZI013_CSP3','42','crop:3','oil_palm'], // Oil Palm
     ['ZI013_CSP3','43','crop:3','coconut'], // Coconut
     ['ZI013_CSP3','44','crop:3','sugar_cane'], // Sugar Cane
     ['ZI013_CSP3','45','crop:3','cotton'], // Cotton
     ['ZI013_CSP3','46','crop:3','bamboo'], // Bamboo
     ['ZI013_CSP3','47','crop:3','palmetto'], // Palmetto
     ['ZI013_CSP3','48','crop:3','timber'], // Timber
     ['ZI013_CSP3','117','crop:3','opium_poppy'], // Opium Poppy
     ['ZI013_CSP3','152','crop:3','cannabis'], // Cannabis
     ['ZI013_CSP3','159','crop:3','ornamental_crop'], // Ornamental Crop
     ['ZI013_CSP3','999','crop:3','other'], // Other

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
     // ['ZI013_FFP2','-999999',undefined,undefined], // No Information
     ['ZI013_FFP2','1','farm:pattern:2','linear'], // Linear
     ['ZI013_FFP2','2','farm:pattern:2','regular'], // Regular
     ['ZI013_FFP2','3','farm:pattern:2','terraced'], // Terraced
     ['ZI013_FFP2','4','farm:pattern:2','intermingled_woods'], // Intermingled Woods
     ['ZI013_FFP2','5','farm:pattern:2','intermingled_trees'], // Intermingled Trees
     ['ZI013_FFP2','6','farm:pattern:2','treeless'], // Treeless
     ['ZI013_FFP2','7','farm:pattern:2','trellised'], // Trellised
     ['ZI013_FFP2','8','farm:pattern:2','irregular'], // Irregular
     ['ZI013_FFP2','999','farm:pattern:2','other'], // Other

     // ZI013_FFP3 - Crop Information : Farming Pattern [3]
     // ['ZI013_FFP3','-999999',undefined,undefined], // No Information
     ['ZI013_FFP3','1','farm:pattern:3','linear'], // Linear
     ['ZI013_FFP3','2','farm:pattern:3','regular'], // Regular
     ['ZI013_FFP3','3','farm:pattern:3','terraced'], // Terraced
     ['ZI013_FFP3','4','farm:pattern:3','intermingled_woods'], // Intermingled Woods
     ['ZI013_FFP3','5','farm:pattern:3','intermingled_trees'], // Intermingled Trees
     ['ZI013_FFP3','6','farm:pattern:3','treeless'], // Treeless
     ['ZI013_FFP3','7','farm:pattern:3','trellised'], // Trellised
     ['ZI013_FFP3','8','farm:pattern:3','irregular'], // Irregular
     ['ZI013_FFP3','999','farm:pattern:3','other'], // Other

     // ZI013_FMM - Crop Information : Farming Method
     // ['ZI013_FMM','-999999',undefined,undefined], // No Information
     ['ZI013_FMM','1','farm:method','fallow'], // Fallow
     ['ZI013_FMM','2','farm:method','grazing'], // Grazing
     ['ZI013_FMM','3','farm:method','permanent'], // Permanent
     ['ZI013_FMM','4','farm:method','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM','999','farm:method','other'], // Other

     // ZI013_FMM2 - Crop Information : Farming Method [2]
     // ['ZI013_FMM2','-999999',undefined,undefined], // No Information
     ['ZI013_FMM2','1','farm:method:2','fallow'], // Fallow
     ['ZI013_FMM2','2','farm:method:2','grazing'], // Grazing
     ['ZI013_FMM2','3','farm:method:2','permanent'], // Permanent
     ['ZI013_FMM2','4','farm:method:2','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM2','999','farm:method:2','other'], // Other

     // ZI013_FMM3 - Crop Information : Farming Method [3]
     // ['ZI013_FMM3','-999999',undefined,undefined], // No Information
     ['ZI013_FMM3','1','farm:method:3','fallow'], // Fallow
     ['ZI013_FMM3','2','farm:method:3','grazing'], // Grazing
     ['ZI013_FMM3','3','farm:method:3','permanent'], // Permanent
     ['ZI013_FMM3','4','farm:method:3','slash_and_burn'], // Slash and Burn
     ['ZI013_FMM3','999','farm:method:3','other'], // Other

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
     // ['ZI014_PBY2','-999999',undefined,undefined], // No Information
     ['ZI014_PBY2','1','by_product:2','ash'], // Ash
     ['ZI014_PBY2','2','by_product:2','cinders'], // Cinders
     ['ZI014_PBY2','6','by_product:2','no_byproduct'], // No Byproduct
     ['ZI014_PBY2','7','by_product:2','radioactive_material'], // Radioactive Material
     ['ZI014_PBY2','8','by_product:2','refuse'], // Refuse
     ['ZI014_PBY2','12','by_product:2','scrap-metal'], // Scrap-metal
     ['ZI014_PBY2','13','by_product:2','sewage'], // Sewage
     ['ZI014_PBY2','15','by_product:2','slag'], // Slag
     ['ZI014_PBY2','16','by_product:2','sludge'], // Sludge - PBY
     ['ZI014_PBY2','17','by_product:2','spoil'], // Spoil - PBY
     ['ZI014_PBY2','89','by_product:2','timber'], // Timber
     ['ZI014_PBY2','999','by_product:2','other'], // Other

     // ZI014_PBY3 - Manufacturing Information : By-product [3]
     // ['ZI014_PBY3','-999999',undefined,undefined], // No Information
     ['ZI014_PBY3','1','by_product:3','ash'], // Ash
     ['ZI014_PBY3','2','by_product:3','cinders'], // Cinders
     ['ZI014_PBY3','6','by_product:3','no_byproduct'], // No Byproduct
     ['ZI014_PBY3','7','by_product:3','radioactive_material'], // Radioactive Material
     ['ZI014_PBY3','8','by_product:3','refuse'], // Refuse
     ['ZI014_PBY3','12','by_product:3','scrap-metal'], // Scrap-metal
     ['ZI014_PBY3','13','by_product:3','sewage'], // Sewage
     ['ZI014_PBY3','15','by_product:3','slag'], // Slag
     ['ZI014_PBY3','16','by_product','sludge'], // Sludge - PBY
     ['ZI014_PBY3','17','by_product','spoil'], // Spoil - PBY
     ['ZI014_PBY3','89','by_product:3','timber'], // Timber
     ['ZI014_PBY3','999','by_product:3','other'], // Other

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
     // ['ZI014_PPO2','-999999',undefined,undefined], // No Information
     ['ZI014_PPO2','1','product2','aircraft'], // Aircraft
     ['ZI014_PPO2','2','product2','aluminium'], // Aluminium
     ['ZI014_PPO2','3','product2','ammunition'], // Ammunition
     ['ZI014_PPO2','4','product2','asphalt'], // Asphalt
     ['ZI014_PPO2','5','product2','motor_vehicle'], // Motor Vehicle
     ['ZI014_PPO2','13','product2','brick'], // Brick
     ['ZI014_PPO2','15','product2','cement'], // Cement
     ['ZI014_PPO2','16','product2','chemical'], // Chemical
     ['ZI014_PPO2','18','product2','coal'], // Coal
     ['ZI014_PPO2','20','product2','coffee'], // Coffee
     ['ZI014_PPO2','21','product2','coke'], // Coke
     ['ZI014_PPO2','23','product2','concrete'], // Concrete
     ['ZI014_PPO2','25','product2','consumer_goods'], // Consumer Goods
     ['ZI014_PPO2','26','product2','copper'], // Copper
     ['ZI014_PPO2','28','product2','cotton'], // Cotton
     ['ZI014_PPO2','32','product2','desalinated_water'], // Desalinated Water
     ['ZI014_PPO2','37','product2','electric_power'], // Electric Power
     ['ZI014_PPO2','38','product2','explosive'], // Explosive
     ['ZI014_PPO2','39','product2','fish'], // Fish
     ['ZI014_PPO2','41','product2','food'], // Food
     ['ZI014_PPO2','43','product2','frozen_water'], // Frozen Water
     ['ZI014_PPO2','44','product2','fruit'], // Fruit
     ['ZI014_PPO2','45','product2','gas'], // Gas
     ['ZI014_PPO2','46','product2','petrol'], // Petrol
     ['ZI014_PPO2','47','product2','glass'], // Glass
     ['ZI014_PPO2','48','product2','gold'], // Gold
     ['ZI014_PPO2','57','product2','ice'], // Ice
     ['ZI014_PPO2','58','product2','iron'], // Iron
     ['ZI014_PPO2','59','product2','lead'], // Lead
     ['ZI014_PPO2','60','product2','lime'], // Lime
     ['ZI014_PPO2','61','product2','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['ZI014_PPO2','62','product2','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['ZI014_PPO2','63','product2','lumber'], // Lumber
     ['ZI014_PPO2','65','product2','manganese'], // Manganese
     ['ZI014_PPO2','69','product2','metal'], // Metal
     ['ZI014_PPO2','70','product2','milk'], // Milk
     ['ZI014_PPO2','73','product2','no_product'], // No Product
     ['ZI014_PPO2','74','product2','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
     ['ZI014_PPO2','80','product2','paper'], // Paper
     ['ZI014_PPO2','83','product2','petroleum'], // Petroleum
     ['ZI014_PPO2','84','product2','plastic'], // Plastic
     ['ZI014_PPO2','87','product2','prestressed_concrete'], // Prestressed Concrete
     ['ZI014_PPO2','90','product2','radioactive_material'], // Radioactive Material
     ['ZI014_PPO2','92','product2','rice'], // Rice
     ['ZI014_PPO2','94','product2','rubber'], // Rubber
     ['ZI014_PPO2','95','product2','salt'], // Salt
     ['ZI014_PPO2','101','product2','sewage'], // Sewage
     ['ZI014_PPO2','105','product2','silver'], // Silver
     ['ZI014_PPO2','106','product2','snow'], // Snow
     ['ZI014_PPO2','109','product2','steel'], // Steel
     ['ZI014_PPO2','111','product2','sugar'], // Sugar
     ['ZI014_PPO2','114','product2','textile'], // Textile
     ['ZI014_PPO2','117','product2','tobacco'], // Tobacco
     ['ZI014_PPO2','120','product2','uranium'], // Uranium
     ['ZI014_PPO2','121','product2','vegetation_product'], // Vegetation Product
     ['ZI014_PPO2','122','product2','water'], // Water
     ['ZI014_PPO2','123','product2','wine'], // Wine
     ['ZI014_PPO2','126','product2','zinc'], // Zinc
     ['ZI014_PPO2','136','product2','biochemical'], // Biochemical
     ['ZI014_PPO2','137','product2','petrochemical'], // Petrochemical
     ['ZI014_PPO2','146','product2','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['ZI014_PPO2','147','product2','electronic_equipment'], // Electronic Equipment
     ['ZI014_PPO2','148','product2','electrical_equipment'], // Electrical Equipment
     ['ZI014_PPO2','150','product2','fertilizer'], // Fertilizer
     ['ZI014_PPO2','154','product2','munitions'], // Munitions
     ['ZI014_PPO2','155','product2','olive_oil'], // Olive Oil
     ['ZI014_PPO2','156','product2','whale_products'], // Whale Products
     ['ZI014_PPO2','157','product2','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PPO2','160','product2','milled_grain'], // Milled Grain
     ['ZI014_PPO2','165','product2','clothing'], // Clothing
     ['ZI014_PPO2','192','product2','petroleum_lubricant'], // Petroleum Lubricant
     ['ZI014_PPO2','200','product2','iron_ingot'], // Iron Ingot
     ['ZI014_PPO2','214','product2','biodiesel'], // Biodiesel
     ['ZI014_PPO2','279','product2','fluorine'], // Fluorine
     ['ZI014_PPO2','325','product2','phosphorus'], // Phosphorus
     ['ZI014_PPO2','435','product2','nuclear_fuel'], // Nuclear Fuel
     ['ZI014_PPO2','999','product2','other'], // Other

     // ZI014_PPO3 - Manufacturing Information : Physical Product [3]
     // ['ZI014_PPO3','-999999',undefined,undefined], // No Information
     ['ZI014_PPO3','1','product3','aircraft'], // Aircraft
     ['ZI014_PPO3','2','product3','aluminium'], // Aluminium
     ['ZI014_PPO3','3','product3','ammunition'], // Ammunition
     ['ZI014_PPO3','4','product3','asphalt'], // Asphalt
     ['ZI014_PPO3','5','product3','motor_vehicle'], // Motor Vehicle
     ['ZI014_PPO3','13','product3','brick'], // Brick
     ['ZI014_PPO3','15','product3','cement'], // Cement
     ['ZI014_PPO3','16','product3','chemical'], // Chemical
     ['ZI014_PPO3','18','product3','coal'], // Coal
     ['ZI014_PPO3','20','product3','coffee'], // Coffee
     ['ZI014_PPO3','21','product3','coke'], // Coke
     ['ZI014_PPO3','23','product3','concrete'], // Concrete
     ['ZI014_PPO3','25','product3','consumer_goods'], // Consumer Goods
     ['ZI014_PPO3','26','product3','copper'], // Copper
     ['ZI014_PPO3','28','product3','cotton'], // Cotton
     ['ZI014_PPO3','32','product3','desalinated_water'], // Desalinated Water
     ['ZI014_PPO3','37','product3','electric_power'], // Electric Power
     ['ZI014_PPO3','38','product3','explosive'], // Explosive
     ['ZI014_PPO3','39','product3','fish'], // Fish
     ['ZI014_PPO3','41','product3','food'], // Food
     ['ZI014_PPO3','43','product3','frozen_water'], // Frozen Water
     ['ZI014_PPO3','44','product3','fruit'], // Fruit
     ['ZI014_PPO3','45','product3','gas'], // Gas
     ['ZI014_PPO3','46','product3','petrol'], // Petrol
     ['ZI014_PPO3','47','product3','glass'], // Glass
     ['ZI014_PPO3','48','product3','gold'], // Gold
     ['ZI014_PPO3','57','product3','ice'], // Ice
     ['ZI014_PPO3','58','product3','iron'], // Iron
     ['ZI014_PPO3','59','product3','lead'], // Lead
     ['ZI014_PPO3','60','product3','lime'], // Lime
     ['ZI014_PPO3','61','product3','liquefied_natural_gas_(lng)'], // Liquefied Natural Gas (LNG)
     ['ZI014_PPO3','62','product3','liquefied_petroleum_gas_(lpg)'], // Liquefied Petroleum Gas (LPG)
     ['ZI014_PPO3','63','product3','lumber'], // Lumber
     ['ZI014_PPO3','65','product3','manganese'], // Manganese
     ['ZI014_PPO3','69','product3','metal'], // Metal
     ['ZI014_PPO3','70','product3','milk'], // Milk
     ['ZI014_PPO3','73','product3','no_product'], // No Product
     ['ZI014_PPO3','74','product3','non-solid_hydrocarbon_fuel'], // Non-solid Hydrocarbon Fuel
     ['ZI014_PPO3','80','product3','paper'], // Paper
     ['ZI014_PPO3','83','product3','petroleum'], // Petroleum
     ['ZI014_PPO3','84','product3','plastic'], // Plastic
     ['ZI014_PPO3','87','product3','prestressed_concrete'], // Prestressed Concrete
     ['ZI014_PPO3','90','product3','radioactive_material'], // Radioactive Material
     ['ZI014_PPO3','92','product3','rice'], // Rice
     ['ZI014_PPO3','94','product3','rubber'], // Rubber
     ['ZI014_PPO3','95','product3','salt'], // Salt
     ['ZI014_PPO3','101','product3','sewage'], // Sewage
     ['ZI014_PPO3','105','product3','silver'], // Silver
     ['ZI014_PPO3','106','product3','snow'], // Snow
     ['ZI014_PPO3','109','product3','steel'], // Steel
     ['ZI014_PPO3','111','product3','sugar'], // Sugar
     ['ZI014_PPO3','114','product3','textile'], // Textile
     ['ZI014_PPO3','117','product3','tobacco'], // Tobacco
     ['ZI014_PPO3','120','product3','uranium'], // Uranium
     ['ZI014_PPO3','121','product3','vegetation_product'], // Vegetation Product
     ['ZI014_PPO3','122','product3','water'], // Water
     ['ZI014_PPO3','123','product3','wine'], // Wine
     ['ZI014_PPO3','126','product3','zinc'], // Zinc
     ['ZI014_PPO3','136','product3','biochemical'], // Biochemical
     ['ZI014_PPO3','137','product3','petrochemical'], // Petrochemical
     ['ZI014_PPO3','146','product3','heating_steam_and/or_water'], // Heating Steam and/or Water
     ['ZI014_PPO3','147','product3','electronic_equipment'], // Electronic Equipment
     ['ZI014_PPO3','148','product3','electrical_equipment'], // Electrical Equipment
     ['ZI014_PPO3','150','product3','fertilizer'], // Fertilizer
     ['ZI014_PPO3','154','product3','munitions'], // Munitions
     ['ZI014_PPO3','155','product3','olive_oil'], // Olive Oil
     ['ZI014_PPO3','156','product3','whale_products'], // Whale Products
     ['ZI014_PPO3','157','product3','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PPO3','160','product3','milled_grain'], // Milled Grain
     ['ZI014_PPO3','165','product3','clothing'], // Clothing
     ['ZI014_PPO3','192','product3','petroleum_lubricant'], // Petroleum Lubricant
     ['ZI014_PPO3','200','product3','iron_ingot'], // Iron Ingot
     ['ZI014_PPO3','214','product3','biodiesel'], // Biodiesel
     ['ZI014_PPO3','279','product3','fluorine'], // Fluorine
     ['ZI014_PPO3','325','product3','phosphorus'], // Phosphorus
     ['ZI014_PPO3','435','product3','nuclear_fuel'], // Nuclear Fuel
     ['ZI014_PPO3','999','product3','other'], // Other

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
     // ['ZI014_PRW2','-999999',undefined,undefined], // No Information
     ['ZI014_PRW2','1','raw_material:2','aluminium'], // Aluminium
     ['ZI014_PRW2','2','raw_material:2','asphalt'], // Asphalt
     ['ZI014_PRW2','5','raw_material:2','bauxite'], // Bauxite
     ['ZI014_PRW2','9','raw_material:2','cement'], // Cement
     ['ZI014_PRW2','10','raw_material:2','chemical'], // Chemical
     ['ZI014_PRW2','11','raw_material:2','clay'], // Clay
     ['ZI014_PRW2','12','raw_material:2','coal'], // Coal
     ['ZI014_PRW2','14','raw_material:2','coke'], // Coke
     ['ZI014_PRW2','16','raw_material:2','copper'], // Copper
     ['ZI014_PRW2','18','raw_material:2','cotton'], // Cotton
     ['ZI014_PRW2','27','raw_material:2','gas'], // Gas
     ['ZI014_PRW2','28','raw_material:2','glass'], // Glass
     ['ZI014_PRW2','29','raw_material:2','gold'], // Gold
     ['ZI014_PRW2','30','raw_material:2','grain'], // Grain
     ['ZI014_PRW2','33','raw_material:2','plant_material'], // Plant Material
     ['ZI014_PRW2','38','raw_material:2','ice'], // Ice
     ['ZI014_PRW2','39','raw_material:2','iron'], // Iron
     ['ZI014_PRW2','41','raw_material:2','lead'], // Lead
     ['ZI014_PRW2','45','raw_material:2','lumber'], // Lumber
     ['ZI014_PRW2','46','raw_material:2','manganese'], // Manganese
     ['ZI014_PRW2','48','raw_material:2','metal'], // Metal
     ['ZI014_PRW2','50','raw_material:2','no_raw_material'], // No Raw Material
     ['ZI014_PRW2','52','raw_material:2','oil'], // Oil
     ['ZI014_PRW2','54','raw_material:2','ore'], // Ore
     ['ZI014_PRW2','57','raw_material:2','paper'], // Paper
     ['ZI014_PRW2','60','raw_material:2','plastic'], // Plastic
     ['ZI014_PRW2','64','raw_material:2','radioactive_material'], // Radioactive Material
     ['ZI014_PRW2','66','raw_material:2','rubber'], // Rubber
     ['ZI014_PRW2','68','raw_material:2','sand'], // Sand
     ['ZI014_PRW2','70','raw_material:2','wood_fragments'], // Wood Fragments
     ['ZI014_PRW2','75','raw_material:2','sewage'], // Sewage
     ['ZI014_PRW2','78','raw_material:2','silver'], // Silver
     ['ZI014_PRW2','79','raw_material:2','snow'], // Snow
     ['ZI014_PRW2','83','raw_material:2','steel'], // Steel
     ['ZI014_PRW2','84','raw_material:2','stone'], // Stone
     ['ZI014_PRW2','85','raw_material:2','sugar'], // Sugar
     ['ZI014_PRW2','87','raw_material:2','textile'], // Textile
     ['ZI014_PRW2','90','raw_material:2','tobacco'], // Tobacco
     ['ZI014_PRW2','93','raw_material:2','uranium'], // Uranium
     ['ZI014_PRW2','94','raw_material:2','vegetation'], // Vegetation
     ['ZI014_PRW2','96','raw_material:2','water'], // Water
     ['ZI014_PRW2','97','raw_material:2','wood'], // Wood
     ['ZI014_PRW2','99','raw_material:2','zinc'], // Zinc
     ['ZI014_PRW2','118','raw_material:2','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PRW2','119','raw_material:2','biodiesel'], // Biodiesel
     ['ZI014_PRW2','999','raw_material:2','other'], // Other

     // ZI014_PRW3 - Manufacturing Information : Raw Material [3]
     // ['ZI014_PRW3','-999999',undefined,undefined], // No Information
     ['ZI014_PRW3','1','raw_material:3','aluminium'], // Aluminium
     ['ZI014_PRW3','2','raw_material:3','asphalt'], // Asphalt
     ['ZI014_PRW3','5','raw_material:3','bauxite'], // Bauxite
     ['ZI014_PRW3','9','raw_material:3','cement'], // Cement
     ['ZI014_PRW3','10','raw_material:3','chemical'], // Chemical
     ['ZI014_PRW3','11','raw_material:3','clay'], // Clay
     ['ZI014_PRW3','12','raw_material:3','coal'], // Coal
     ['ZI014_PRW3','14','raw_material:3','coke'], // Coke
     ['ZI014_PRW3','16','raw_material:3','copper'], // Copper
     ['ZI014_PRW3','18','raw_material:3','cotton'], // Cotton
     ['ZI014_PRW3','27','raw_material:3','gas'], // Gas
     ['ZI014_PRW3','28','raw_material:3','glass'], // Glass
     ['ZI014_PRW3','29','raw_material:3','gold'], // Gold
     ['ZI014_PRW3','30','raw_material:3','grain'], // Grain
     ['ZI014_PRW3','33','raw_material:3','plant_material'], // Plant Material
     ['ZI014_PRW3','38','raw_material:3','ice'], // Ice
     ['ZI014_PRW3','39','raw_material:3','iron'], // Iron
     ['ZI014_PRW3','41','raw_material:3','lead'], // Lead
     ['ZI014_PRW3','45','raw_material:3','lumber'], // Lumber
     ['ZI014_PRW3','46','raw_material:3','manganese'], // Manganese
     ['ZI014_PRW3','48','raw_material:3','metal'], // Metal
     ['ZI014_PRW3','50','raw_material:3','no_raw_material'], // No Raw Material
     ['ZI014_PRW3','52','raw_material:3','oil'], // Oil
     ['ZI014_PRW3','54','raw_material:3','ore'], // Ore
     ['ZI014_PRW3','57','raw_material:3','paper'], // Paper
     ['ZI014_PRW3','60','raw_material:3','plastic'], // Plastic
     ['ZI014_PRW3','64','raw_material:3','radioactive_material'], // Radioactive Material
     ['ZI014_PRW3','66','raw_material:3','rubber'], // Rubber
     ['ZI014_PRW3','68','raw_material:3','sand'], // Sand
     ['ZI014_PRW3','70','raw_material:3','wood_fragments'], // Wood Fragments
     ['ZI014_PRW3','75','raw_material:3','sewage'], // Sewage
     ['ZI014_PRW3','78','raw_material:3','silver'], // Silver
     ['ZI014_PRW3','79','raw_material:3','snow'], // Snow
     ['ZI014_PRW3','83','raw_material:3','steel'], // Steel
     ['ZI014_PRW3','84','raw_material:3','stone'], // Stone
     ['ZI014_PRW3','85','raw_material:3','sugar'], // Sugar
     ['ZI014_PRW3','87','raw_material:3','textile'], // Textile
     ['ZI014_PRW3','90','raw_material:3','tobacco'], // Tobacco
     ['ZI014_PRW3','93','raw_material:3','uranium'], // Uranium
     ['ZI014_PRW3','94','raw_material:3','vegetation'], // Vegetation
     ['ZI014_PRW3','96','raw_material:3','water'], // Water
     ['ZI014_PRW3','97','raw_material:3','wood'], // Wood
     ['ZI014_PRW3','99','raw_material:3','zinc'], // Zinc
     ['ZI014_PRW3','118','raw_material:3','petroleum_and/or_natural_gas'], // Petroleum and/or Natural Gas
     ['ZI014_PRW3','119','raw_material:3','biodiesel'], // Biodiesel
     ['ZI014_PRW3','999','raw_material:3','other'], // Other

     // ZI015_GCUC - Geo Name Collection : Geographic Name Cartographic Usability Range <interval closure>
     ['ZI015_GCUC','2','raw:ZI015_GCUC','open_interval'], // Open Interval
     ['ZI015_GCUC','3','raw:ZI015_GCUC','greater-than_or_equal_to_less-than_interval'], // Greater-than-or-equal to Less-than Interval
     ['ZI015_GCUC','4','raw:ZI015_GCUC','greater-than_to_less-than_or_equal_interval'], // Greater-than to Less-than-or-equal Interval
     ['ZI015_GCUC','5','raw:ZI015_GCUC','closed_interval'], // Closed Interval
     ['ZI015_GCUC','6','raw:ZI015_GCUC','greater-than_semi-interval'], // Greater-than Semi-interval
     ['ZI015_GCUC','7','raw:ZI015_GCUC','greater-than_or_equal_semi-interval'], // Greater-than or Equal Semi-interval
     ['ZI015_GCUC','8','raw:ZI015_GCUC','less-than_semi-interval'], // Less-than Semi-interval
     ['ZI015_GCUC','9','raw:ZI015_GCUC','less-than_or_equal_semi-interval'], // Less-than or Equal Semi-interval

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
     // ['ZI019_ASP2','-999999',undefined,undefined], // No Information
     ['ZI019_ASP2','1','aeroway:pavement:preparation:2','aggregate_seal_coat'], // Aggregate Seal Coat
     ['ZI019_ASP2','2','aeroway:pavement:preparation:2','graded'], // Graded
     ['ZI019_ASP2','3','aeroway:pavement:preparation:2','grass'], // Grass
     ['ZI019_ASP2','4','aeroway:pavement:preparation:2','grooved'], // Grooved
     ['ZI019_ASP2','5','aeroway:pavement:preparation:2','oiled'], // Oiled
     ['ZI019_ASP2','6','aeroway:pavement:preparation:2','porous_friction_course'], // Porous Friction Course
     ['ZI019_ASP2','7','aeroway:pavement:preparation:2','rolled'], // Rolled
     ['ZI019_ASP2','8','aeroway:pavement:preparation:2','rubberized_seal_coat'], // Rubberized Seal Coat
     ['ZI019_ASP2','999','aeroway:pavement:preparation:2','other'], // Other

     // ZI019_ASP3 - Aero Pvmt : Aerodrome Movement Area Surface Preparation Method [3]
     // ['ZI019_ASP3','-999999',undefined,undefined], // No Information
     ['ZI019_ASP3','1','aeroway:pavement:preparation:3','aggregate_seal_coat'], // Aggregate Seal Coat
     ['ZI019_ASP3','2','aeroway:pavement:preparation:3','graded'], // Graded
     ['ZI019_ASP3','3','aeroway:pavement:preparation:3','grass'], // Grass
     ['ZI019_ASP3','4','aeroway:pavement:preparation:3','grooved'], // Grooved
     ['ZI019_ASP3','5','aeroway:pavement:preparation:3','oiled'], // Oiled
     ['ZI019_ASP3','6','aeroway:pavement:preparation:3','porous_friction_course'], // Porous Friction Course
     ['ZI019_ASP3','7','aeroway:pavement:preparation:3','rolled'], // Rolled
     ['ZI019_ASP3','8','aeroway:pavement:preparation:3','rubberized_seal_coat'], // Rubberized Seal Coat
     ['ZI019_ASP3','999','aeroway:pavement:preparation:3','other'], // Other

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
     // ['ZI019_ASU2','-999999',undefined,undefined], // No Information
     ['ZI019_ASU2','1','aeroway:surface:2','asphalt'], // Asphalt
     ['ZI019_ASU2','2','aeroway:surface:2','bituminous_mix'], // Bituminous Mix
     ['ZI019_ASU2','3','aeroway:surface:2','brick'], // Brick
     ['ZI019_ASU2','4','aeroway:surface:2','clay'], // Clay
     ['ZI019_ASU2','5','aeroway:surface:2','concrete'], // Concrete
     ['ZI019_ASU2','6','aeroway:surface:2','coral'], // Coral
     ['ZI019_ASU2','7','aeroway:surface:2','earthen'], // Earthen
     ['ZI019_ASU2','8','aeroway:surface:2','gravel'], // Gravel
     ['ZI019_ASU2','9','aeroway:surface:2','ice'], // Ice
     ['ZI019_ASU2','10','aeroway:surface:2','landing_mat'], // Landing Mat
     ['ZI019_ASU2','11','aeroway:surface:2','laterite'], // Laterite
     ['ZI019_ASU2','12','aeroway:surface:2','macadam'], // Macadam
     ['ZI019_ASU2','13','aeroway:surface:2','membrane'], // Membrane
     ['ZI019_ASU2','14','aeroway:surface:2','non-bituminous_mix'], // Non-bituminous Mix
     ['ZI019_ASU2','15','aeroway:surface:2','pierced_steel_planking'], // Pierced Steel Planking
     ['ZI019_ASU2','16','aeroway:surface:2','sand'], // Sand
     ['ZI019_ASU2','17','aeroway:surface:2','snow'], // Snow
     ['ZI019_ASU2','18','aeroway:surface:2','stone'], // Stone
     ['ZI019_ASU2','20','aeroway:surface:2','wood'], // Wood
     ['ZI019_ASU2','21','aeroway:surface:2','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ZI019_ASU2','999','aeroway:surface:2','other'], // Other

     // ZI019_ASU3 - Aero Pvmt : Aerodrome Movement Area Surface Composition [3]
     // ['ZI019_ASU3','-999999',undefined,undefined], // No Information
     ['ZI019_ASU3','1','aeroway:surface:3','asphalt'], // Asphalt
     ['ZI019_ASU3','2','aeroway:surface:3','bituminous_mix'], // Bituminous Mix
     ['ZI019_ASU3','3','aeroway:surface:3','brick'], // Brick
     ['ZI019_ASU3','4','aeroway:surface:3','clay'], // Clay
     ['ZI019_ASU3','5','aeroway:surface:3','concrete'], // Concrete
     ['ZI019_ASU3','6','aeroway:surface:3','coral'], // Coral
     ['ZI019_ASU3','7','aeroway:surface:3','earthen'], // Earthen
     ['ZI019_ASU3','8','aeroway:surface:3','gravel'], // Gravel
     ['ZI019_ASU3','9','aeroway:surface:3','ice'], // Ice
     ['ZI019_ASU3','10','aeroway:surface:3','landing_mat'], // Landing Mat
     ['ZI019_ASU3','11','aeroway:surface:3','laterite'], // Laterite
     ['ZI019_ASU3','12','aeroway:surface:3','macadam'], // Macadam
     ['ZI019_ASU3','13','aeroway:surface:3','membrane'], // Membrane
     ['ZI019_ASU3','14','aeroway:surface:3','non-bituminous_mix'], // Non-bituminous Mix
     ['ZI019_ASU3','15','aeroway:surface:3','pierced_steel_planking'], // Pierced Steel Planking
     ['ZI019_ASU3','16','aeroway:surface:3','sand'], // Sand
     ['ZI019_ASU3','17','aeroway:surface:3','snow'], // Snow
     ['ZI019_ASU3','18','aeroway:surface:3','stone'], // Stone
     ['ZI019_ASU3','20','aeroway:surface:3','wood'], // Wood
     ['ZI019_ASU3','21','aeroway:surface:3','asphalt_over_concrete'], // Asphalt Over Concrete
     ['ZI019_ASU3','999','aeroway:surface:3','other'], // Other

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

     // ZI020_GE4 - Designation : GENC Short URN-based Identifier
     ['ZI020_GE4','ge:GENC:3:1-2:ABW','is_in:country_code','AW'], // ARUBA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AFG','is_in:country_code','AF'], // AFGHANISTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AGO','is_in:country_code','AO'], // ANGOLA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ALB','is_in:country_code','AL'], // ALBANIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AND','is_in:country_code','AD'], // ANDORRA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ARE','is_in:country_code','AE'], // UNITED ARAB EMIRATES AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ARG','is_in:country_code','AR'], // ARGENTINA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ARM','is_in:country_code','AM'], // ARMENIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ASM','is_in:country_code','AS'], // AMERICAN SAMOA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ATF','is_in:country_code','TF'], // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ATG','is_in:country_code','AG'], // ANTIGUA AND BARBUDA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AUS','is_in:country_code','AU'], // AUSTRALIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AUT','is_in:country_code','AT'], // AUSTRIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AX1','is_in:country_code','A1'], // UNKNOWN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AX2','is_in:country_code','A2'], // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:AZE','is_in:country_code','AZ'], // AZERBAIJAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BDI','is_in:country_code','BI'], // BURUNDI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BEL','is_in:country_code','BE'], // BELGIUM AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BEN','is_in:country_code','BJ'], // BENIN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BES','is_in:country_code','BQ'], // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BFA','is_in:country_code','BF'], // BURKINA FASO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BGD','is_in:country_code','BD'], // BANGLADESH AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BGR','is_in:country_code','BG'], // BULGARIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BHR','is_in:country_code','BH'], // BAHRAIN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BHS','is_in:country_code','BS'], // BAHAMAS, THE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BLM','is_in:country_code','BL'], // SAINT BARTHELEMY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BLR','is_in:country_code','BY'], // BELARUS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BOL','is_in:country_code','BO'], // BOLIVIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BRA','is_in:country_code','BR'], // BRAZIL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BRN','is_in:country_code','BN'], // BRUNEI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BTN','is_in:country_code','BT'], // BHUTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:BWA','is_in:country_code','BW'], // BOTSWANA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CAF','is_in:country_code','CF'], // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CAN','is_in:country_code','CA'], // CANADA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CCK','is_in:country_code','CC'], // COCOS (KEELING) ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CHE','is_in:country_code','CH'], // SWITZERLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CHL','is_in:country_code','CL'], // CHILE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CHN','is_in:country_code','CN'], // CHINA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CIV','is_in:country_code','CI'], // COTE D"IVOIRE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CMR','is_in:country_code','CM'], // CAMEROON AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:COD','is_in:country_code','CD'], // CONGO (KINSHASA) AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:COG','is_in:country_code','CG'], // CONGO (BRAZZAVILLE) AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:COK','is_in:country_code','CK'], // COOK ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:COL','is_in:country_code','CO'], // COLOMBIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:COM','is_in:country_code','KM'], // COMOROS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CPT','is_in:country_code','CP'], // CLIPPERTON ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CPV','is_in:country_code','CV'], // CAPE VERDE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CRI','is_in:country_code','CR'], // COSTA RICA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CUB','is_in:country_code','CU'], // CUBA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CUW','is_in:country_code','CW'], // CURACAO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CXR','is_in:country_code','CX'], // CHRISTMAS ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CYM','is_in:country_code','KY'], // CAYMAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CYP','is_in:country_code','CY'], // CYPRUS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:CZE','is_in:country_code','CZ'], // CZECH REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DEU','is_in:country_code','DE'], // GERMANY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DGA','is_in:country_code','DG'], // DIEGO GARCIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DJI','is_in:country_code','DJ'], // DJIBOUTI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DMA','is_in:country_code','DM'], // DOMINICA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DNK','is_in:country_code','DK'], // DENMARK AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DOM','is_in:country_code','DO'], // DOMINICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:DZA','is_in:country_code','DZ'], // ALGERIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ECU','is_in:country_code','EC'], // ECUADOR AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:EGY','is_in:country_code','EG'], // EGYPT AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ERI','is_in:country_code','ER'], // ERITREA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ESH','is_in:country_code','EH'], // WESTERN SAHARA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ESP','is_in:country_code','ES'], // SPAIN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:EST','is_in:country_code','EE'], // ESTONIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ETH','is_in:country_code','ET'], // ETHIOPIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:FIN','is_in:country_code','FI'], // FINLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:FJI','is_in:country_code','FJ'], // FIJI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:FLK','is_in:country_code','FK'], // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:FRA','is_in:country_code','FR'], // FRANCE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:FRO','is_in:country_code','FO'], // FAROE ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:FSM','is_in:country_code','FM'], // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GAB','is_in:country_code','GA'], // GABON AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GBR','is_in:country_code','GB'], // UNITED KINGDOM AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GEO','is_in:country_code','GE'], // GEORGIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GGY','is_in:country_code','GG'], // GUERNSEY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GHA','is_in:country_code','GH'], // GHANA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GIN','is_in:country_code','GN'], // GUINEA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GLP','is_in:country_code','GP'], // GUADELOUPE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GMB','is_in:country_code','GM'], // GAMBIA, THE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GNB','is_in:country_code','GW'], // GUINEA-BISSAU AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GNQ','is_in:country_code','GQ'], // EQUATORIAL GUINEA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GRC','is_in:country_code','GR'], // GREECE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GTM','is_in:country_code','GT'], // GUATEMALA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GUF','is_in:country_code','GF'], // FRENCH GUIANA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GUM','is_in:country_code','GU'], // GUAM AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:GUY','is_in:country_code','GY'], // GUYANA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:HKG','is_in:country_code','HK'], // HONG KONG AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:HMD','is_in:country_code','HM'], // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:HND','is_in:country_code','HN'], // HONDURAS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:HRV','is_in:country_code','HR'], // CROATIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:HTI','is_in:country_code','HT'], // HAITI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:IDN','is_in:country_code','ID'], // INDONESIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:IND','is_in:country_code','IN'], // INDIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:IOT','is_in:country_code','IO'], // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:IRN','is_in:country_code','IR'], // IRAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:IRQ','is_in:country_code','IQ'], // IRAQ AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ISL','is_in:country_code','IS'], // ICELAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ISR','is_in:country_code','IL'], // ISRAEL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ITA','is_in:country_code','IT'], // ITALY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:JEY','is_in:country_code','JE'], // JERSEY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:JOR','is_in:country_code','jordan'], // JORDAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KAZ','is_in:country_code','kazakhstan'], // KAZAKHSTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KEN','is_in:country_code','kenya'], // KENYA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KGZ','is_in:country_code','kyrgyzstan'], // KYRGYZSTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KHM','is_in:country_code','cambodia'], // CAMBODIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KIR','is_in:country_code','kiribati'], // KIRIBATI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KNA','is_in:country_code','saint_kitts_and_nevis'], // SAINT KITTS AND NEVIS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KOR','is_in:country_code','korea,_south'], // KOREA, SOUTH AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:KWT','is_in:country_code','kuwait'], // KUWAIT AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LAO','is_in:country_code','laos'], // LAOS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LBN','is_in:country_code','lebanon'], // LEBANON AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LBR','is_in:country_code','liberia'], // LIBERIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LIE','is_in:country_code','liechtenstein'], // LIECHTENSTEIN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LKA','is_in:country_code','sri_lanka'], // SRI LANKA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LSO','is_in:country_code','lesotho'], // LESOTHO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LTU','is_in:country_code','lithuania'], // LITHUANIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LUX','is_in:country_code','luxembourg'], // LUXEMBOURG AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:LVA','is_in:country_code','latvia'], // LATVIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MAC','is_in:country_code','macau'], // MACAU AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MAF','is_in:country_code','saint_martin'], // SAINT MARTIN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MAR','is_in:country_code','morocco'], // MOROCCO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MCO','is_in:country_code','monaco'], // MONACO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MDA','is_in:country_code','moldova'], // MOLDOVA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MDG','is_in:country_code','madagascar'], // MADAGASCAR AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MDV','is_in:country_code','maldives'], // MALDIVES AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MEX','is_in:country_code','mexico'], // MEXICO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MHL','is_in:country_code','marshall_islands'], // MARSHALL ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MKD','is_in:country_code','macedonia'], // MACEDONIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MLI','is_in:country_code','mali'], // MALI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MLT','is_in:country_code','malta'], // MALTA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MMR','is_in:country_code','burma'], // BURMA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MNG','is_in:country_code','mongolia'], // MONGOLIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MNP','is_in:country_code','northern_mariana_islands'], // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MOZ','is_in:country_code','mozambique'], // MOZAMBIQUE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MRT','is_in:country_code','mauritania'], // MAURITANIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MTQ','is_in:country_code','martinique'], // MARTINIQUE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MUS','is_in:country_code','mauritius'], // MAURITIUS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MWI','is_in:country_code','malawi'], // MALAWI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MYS','is_in:country_code','malaysia'], // MALAYSIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:MYT','is_in:country_code','mayotte'], // MAYOTTE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NAM','is_in:country_code','namibia'], // NAMIBIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NCL','is_in:country_code','new_caledonia'], // NEW CALEDONIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NER','is_in:country_code','niger'], // NIGER AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NFK','is_in:country_code','norfolk_island'], // NORFOLK ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NGA','is_in:country_code','nigeria'], // NIGERIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NIC','is_in:country_code','nicaragua'], // NICARAGUA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NLD','is_in:country_code','netherlands'], // NETHERLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NOR','is_in:country_code','norway'], // NORWAY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NPL','is_in:country_code','nepal'], // NEPAL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NRU','is_in:country_code','nauru'], // NAURU AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:NZL','is_in:country_code','new_zealand'], // NEW ZEALAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:OMN','is_in:country_code','oman'], // OMAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PAK','is_in:country_code','pakistan'], // PAKISTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PAN','is_in:country_code','panama'], // PANAMA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PCN','is_in:country_code','pitcairn_islands'], // PITCAIRN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PER','is_in:country_code','peru'], // PERU AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PHL','is_in:country_code','philippines'], // PHILIPPINES AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PLW','is_in:country_code','palau'], // PALAU AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:POL','is_in:country_code','poland'], // POLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PRI','is_in:country_code','puerto_rico'], // PUERTO RICO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PRK','is_in:country_code','korea,_north'], // KOREA, NORTH AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PRT','is_in:country_code','portugal'], // PORTUGAL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PRY','is_in:country_code','paraguay'], // PARAGUAY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PSE','is_in:country_code','palestinian_territory'], // PALESTINIAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:PYF','is_in:country_code','french_polynesia'], // FRENCH POLYNESIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:QAT','is_in:country_code','qatar'], // QATAR AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:REU','is_in:country_code','reunion'], // REUNION AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:RUS','is_in:country_code','russia'], // RUSSIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:RWA','is_in:country_code','rwanda'], // RWANDA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SAU','is_in:country_code','saudi_arabia'], // SAUDI ARABIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SDN','is_in:country_code','sudan'], // SUDAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SEN','is_in:country_code','senegal'], // SENEGAL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SGP','is_in:country_code','singapore'], // SINGAPORE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SGS','is_in:country_code','south_georgia_and_south_sandwich_islands'], // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SHN','is_in:country_code','saint_helena,_ascension,_and_tristan_da_cunha'], // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SLB','is_in:country_code','solomon_islands'], // SOLOMON ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SLE','is_in:country_code','sierra_leone'], // SIERRA LEONE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SLV','is_in:country_code','el_salvador'], // EL SALVADOR AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SMR','is_in:country_code','san_marino'], // SAN MARINO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SOM','is_in:country_code','somalia'], // SOMALIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SPM','is_in:country_code','saint_pierre_and_miquelon'], // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SRB','is_in:country_code','serbia'], // SERBIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SSD','is_in:country_code','south_sudan'], // SOUTH SUDAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:STP','is_in:country_code','sao_tome_and_principe'], // SAO TOME AND PRINCIPE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SUR','is_in:country_code','suriname'], // SURINAME AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SVK','is_in:country_code','slovakia'], // SLOVAKIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SVN','is_in:country_code','slovenia'], // SLOVENIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SWE','is_in:country_code','sweden'], // SWEDEN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SWZ','is_in:country_code','swaziland'], // SWAZILAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SXM','is_in:country_code','sint_maarten'], // SINT MAARTEN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SYC','is_in:country_code','seychelles'], // SEYCHELLES AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:SYR','is_in:country_code','syria'], // SYRIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TCA','is_in:country_code','turks_and_caicos_islands'], // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TCD','is_in:country_code','chad'], // CHAD AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TGO','is_in:country_code','togo'], // TOGO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:THA','is_in:country_code','thailand'], // THAILAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TJK','is_in:country_code','tajikistan'], // TAJIKISTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TLS','is_in:country_code','timor-leste'], // TIMOR-LESTE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TON','is_in:country_code','tonga'], // TONGA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TTO','is_in:country_code','trinidad_and_tobago'], // TRINIDAD AND TOBAGO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TUN','is_in:country_code','tunisia'], // TUNISIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TUR','is_in:country_code','turkey'], // TURKEY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TWN','is_in:country_code','taiwan'], // TAIWAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:TZA','is_in:country_code','tanzania'], // TANZANIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:UGA','is_in:country_code','uganda'], // UGANDA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:UKR','is_in:country_code','ukraine'], // UKRAINE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:URY','is_in:country_code','uruguay'], // URUGUAY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:USA','is_in:country_code','united_states'], // UNITED STATES AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:UZB','is_in:country_code','uzbekistan'], // UZBEKISTAN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VAT','is_in:country_code','vatican_city'], // VATICAN CITY AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VCT','is_in:country_code','saint_vincent_and_the_grenadines'], // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VEN','is_in:country_code','venezuela'], // VENEZUELA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VGB','is_in:country_code','virgin_islands,_british'], // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VIR','is_in:country_code','virgin_islands,_u.s.'], // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VNM','is_in:country_code','vietnam'], // VIETNAM AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:VUT','is_in:country_code','vanuatu'], // VANUATU AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:WLF','is_in:country_code','wallis_and_futuna'], // WALLIS AND FUTUNA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:WSM','is_in:country_code','samoa'], // SAMOA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XAC','is_in:country_code','ashmore_and_cartier_islands'], // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XAZ','is_in:country_code','entity_1'], // ENTITY 1 AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XBI','is_in:country_code','bassas_da_india'], // BASSAS DA INDIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XBK','is_in:country_code','baker_island'], // BAKER ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XCR','is_in:country_code','entity_2'], // ENTITY 2 AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XCS','is_in:country_code','coral_sea_islands'], // CORAL SEA ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XCY','is_in:country_code','entity_3'], // ENTITY 3 AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XEU','is_in:country_code','europa_island'], // EUROPA ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XGL','is_in:country_code','glorioso_islands'], // GLORIOSO ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XGZ','is_in:country_code','gaza_strip'], // GAZA STRIP AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XHO','is_in:country_code','howland_island'], // HOWLAND ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XJA','is_in:country_code','johnston_atoll'], // JOHNSTON ATOLL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XJM','is_in:country_code','jan_mayen'], // JAN MAYEN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XJN','is_in:country_code','juan_de_nova_island'], // JUAN DE NOVA ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XJV','is_in:country_code','jarvis_island'], // JARVIS ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XKM','is_in:country_code','entity_4'], // ENTITY 4 AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XKN','is_in:country_code','entity_5'], // ENTITY 5 AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XKR','is_in:country_code','kingman_reef'], // KINGMAN REEF AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XKS','is_in:country_code','kosovo'], // KOSOVO AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XMW','is_in:country_code','midway_islands'], // MIDWAY ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XNV','is_in:country_code','navassa_island'], // NAVASSA ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XPL','is_in:country_code','palmyra_atoll'], // PALMYRA ATOLL AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XPR','is_in:country_code','paracel_islands'], // PARACEL ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XQP','is_in:country_code','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XQZ','is_in:country_code','akrotiri'], // AKROTIRI AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XSP','is_in:country_code','spratly_islands'], // SPRATLY ISLANDS AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XSV','is_in:country_code','svalbard'], // SVALBARD AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XTR','is_in:country_code','tromelin_island'], // TROMELIN ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XWB','is_in:country_code','west_bank'], // WEST BANK AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XWK','is_in:country_code','wake_island'], // WAKE ISLAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XXD','is_in:country_code','dhekelia'], // DHEKELIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:XXX','is_in:country_code','no_mans_land'], // NO MAN"S LAND AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:YEM','is_in:country_code','yemen'], // YEMEN AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ZAF','is_in:country_code','south_africa'], // SOUTH AFRICA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ZMB','is_in:country_code','zambia'], // ZAMBIA AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:1-2:ZWE','is_in:country_code','zimbabwe'], // ZIMBABWE AS OF 2013-06-30
     ['ZI020_GE4','ge:GENC:3:ed3:AX3','is_in:country_code','entity_6'], // ENTITY 6 AS OF 2015-06-30
     ['ZI020_GE4','ge:ISO1:3:VI-15:AIA','is_in:country_code','anguilla'], // ANGUILLA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:ATA','is_in:country_code','antarctica'], // ANTARCTICA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:BIH','is_in:country_code','bosnia_and_herzegovina'], // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:BLZ','is_in:country_code','belize'], // BELIZE AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:BMU','is_in:country_code','bermuda'], // BERMUDA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:BRB','is_in:country_code','barbados'], // BARBADOS AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:BVT','is_in:country_code','bouvet_island'], // BOUVET ISLAND AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:GIB','is_in:country_code','gibraltar'], // GIBRALTAR AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:GRD','is_in:country_code','grenada'], // GRENADA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:GRL','is_in:country_code','greenland'], // GREENLAND AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:HUN','is_in:country_code','hungary'], // HUNGARY AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:IMN','is_in:country_code','isle_of_man'], // ISLE OF MAN AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:IRL','is_in:country_code','ireland'], // IRELAND AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:JAM','is_in:country_code','jamaica'], // JAMAICA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:JPN','is_in:country_code','japan'], // JAPAN AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:LBY','is_in:country_code','libya'], // LIBYA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:LCA','is_in:country_code','saint_lucia'], // SAINT LUCIA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:MNE','is_in:country_code','montenegro'], // MONTENEGRO AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:MSR','is_in:country_code','montserrat'], // MONTSERRAT AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:NIU','is_in:country_code','niue'], // NIUE AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:PNG','is_in:country_code','papua_new_guinea'], // PAPUA NEW GUINEA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:ROU','is_in:country_code','romania'], // ROMANIA AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:TKL','is_in:country_code','tokelau'], // TOKELAU AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:TKM','is_in:country_code','turkmenistan'], // TURKMENISTAN AS OF 2013-05-10
     ['ZI020_GE4','ge:ISO1:3:VI-15:TUV','is_in:country_code','tuvalu'], // TUVALU AS OF 2013-05-10
     ['ZI020_GE4','noInformation','is_in:country_code','no_information'], // No Information
     ['ZI020_GE4','other','is_in:country_code','other'], // Other

     // ZI020_GE42 - Designation : GENC Short URN-based Identifier [2]
     ['ZI020_GE42','ge:GENC:3:1-2:ABW','is_in:country_code:2','aruba'], // ARUBA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AFG','is_in:country_code:2','afghanistan'], // AFGHANISTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AGO','is_in:country_code:2','angola'], // ANGOLA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ALB','is_in:country_code:2','albania'], // ALBANIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AND','is_in:country_code:2','andorra'], // ANDORRA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ARE','is_in:country_code:2','united_arab_emirates'], // UNITED ARAB EMIRATES AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ARG','is_in:country_code:2','argentina'], // ARGENTINA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ARM','is_in:country_code:2','armenia'], // ARMENIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ASM','is_in:country_code:2','american_samoa'], // AMERICAN SAMOA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ATF','is_in:country_code:2','french_southern_and_antarctic_lands'], // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ATG','is_in:country_code:2','antigua_and_barbuda'], // ANTIGUA AND BARBUDA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AUS','is_in:country_code:2','australia'], // AUSTRALIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AUT','is_in:country_code:2','austria'], // AUSTRIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AX1','is_in:country_code:2','unknown'], // UNKNOWN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AX2','is_in:country_code:2','guantanamo_bay_naval_base'], // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:AZE','is_in:country_code:2','azerbaijan'], // AZERBAIJAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BDI','is_in:country_code:2','burundi'], // BURUNDI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BEL','is_in:country_code:2','belgium'], // BELGIUM AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BEN','is_in:country_code:2','benin'], // BENIN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BES','is_in:country_code:2','bonaire,_sint_eustatius,_and_saba'], // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BFA','is_in:country_code:2','burkina_faso'], // BURKINA FASO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BGD','is_in:country_code:2','bangladesh'], // BANGLADESH AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BGR','is_in:country_code:2','bulgaria'], // BULGARIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BHR','is_in:country_code:2','bahrain'], // BAHRAIN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BHS','is_in:country_code:2','bahamas,_the'], // BAHAMAS, THE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BLM','is_in:country_code:2','saint_barthelemy'], // SAINT BARTHELEMY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BLR','is_in:country_code:2','belarus'], // BELARUS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BOL','is_in:country_code:2','bolivia'], // BOLIVIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BRA','is_in:country_code:2','brazil'], // BRAZIL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BRN','is_in:country_code:2','brunei'], // BRUNEI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BTN','is_in:country_code:2','bhutan'], // BHUTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:BWA','is_in:country_code:2','botswana'], // BOTSWANA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CAF','is_in:country_code:2','central_african_republic'], // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CAN','is_in:country_code:2','canada'], // CANADA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CCK','is_in:country_code:2','cocos_(keeling)_islands'], // COCOS (KEELING) ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CHE','is_in:country_code:2','switzerland'], // SWITZERLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CHL','is_in:country_code:2','chile'], // CHILE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CHN','is_in:country_code:2','china'], // CHINA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CIV','is_in:country_code:2','cote_d"ivoire'], // COTE D"IVOIRE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CMR','is_in:country_code:2','cameroon'], // CAMEROON AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:COD','is_in:country_code:2','congo_(kinshasa)'], // CONGO (KINSHASA) AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:COG','is_in:country_code:2','congo_(brazzaville)'], // CONGO (BRAZZAVILLE) AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:COK','is_in:country_code:2','cook_islands'], // COOK ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:COL','is_in:country_code:2','colombia'], // COLOMBIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:COM','is_in:country_code:2','comoros'], // COMOROS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CPT','is_in:country_code:2','clipperton_island'], // CLIPPERTON ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CPV','is_in:country_code:2','cape_verde'], // CAPE VERDE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CRI','is_in:country_code:2','costa_rica'], // COSTA RICA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CUB','is_in:country_code:2','cuba'], // CUBA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CUW','is_in:country_code:2','curacao'], // CURACAO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CXR','is_in:country_code:2','christmas_island'], // CHRISTMAS ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CYM','is_in:country_code:2','cayman_islands'], // CAYMAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CYP','is_in:country_code:2','cyprus'], // CYPRUS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:CZE','is_in:country_code:2','czech_republic'], // CZECH REPUBLIC AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DEU','is_in:country_code:2','germany'], // GERMANY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DGA','is_in:country_code:2','diego_garcia'], // DIEGO GARCIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DJI','is_in:country_code:2','djibouti'], // DJIBOUTI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DMA','is_in:country_code:2','dominica'], // DOMINICA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DNK','is_in:country_code:2','denmark'], // DENMARK AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DOM','is_in:country_code:2','dominican_republic'], // DOMINICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:DZA','is_in:country_code:2','algeria'], // ALGERIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ECU','is_in:country_code:2','ecuador'], // ECUADOR AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:EGY','is_in:country_code:2','egypt'], // EGYPT AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ERI','is_in:country_code:2','eritrea'], // ERITREA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ESH','is_in:country_code:2','western_sahara'], // WESTERN SAHARA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ESP','is_in:country_code:2','spain'], // SPAIN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:EST','is_in:country_code:2','estonia'], // ESTONIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ETH','is_in:country_code:2','ethiopia'], // ETHIOPIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:FIN','is_in:country_code:2','finland'], // FINLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:FJI','is_in:country_code:2','fiji'], // FIJI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:FLK','is_in:country_code:2','falkland_islands_(islas_malvinas)'], // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:FRA','is_in:country_code:2','france'], // FRANCE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:FRO','is_in:country_code:2','faroe_islands'], // FAROE ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:FSM','is_in:country_code:2','micronesia,_federated_states_of'], // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GAB','is_in:country_code:2','gabon'], // GABON AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GBR','is_in:country_code:2','united_kingdom'], // UNITED KINGDOM AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GEO','is_in:country_code:2','georgia'], // GEORGIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GGY','is_in:country_code:2','guernsey'], // GUERNSEY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GHA','is_in:country_code:2','ghana'], // GHANA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GIN','is_in:country_code:2','guinea'], // GUINEA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GLP','is_in:country_code:2','guadeloupe'], // GUADELOUPE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GMB','is_in:country_code:2','gambia,_the'], // GAMBIA, THE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GNB','is_in:country_code:2','guinea-bissau'], // GUINEA-BISSAU AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GNQ','is_in:country_code:2','equatorial_guinea'], // EQUATORIAL GUINEA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GRC','is_in:country_code:2','greece'], // GREECE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GTM','is_in:country_code:2','guatemala'], // GUATEMALA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GUF','is_in:country_code:2','french_guiana'], // FRENCH GUIANA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GUM','is_in:country_code:2','guam'], // GUAM AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:GUY','is_in:country_code:2','guyana'], // GUYANA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:HKG','is_in:country_code:2','hong_kong'], // HONG KONG AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:HMD','is_in:country_code:2','heard_island_and_mcdonald_islands'], // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:HND','is_in:country_code:2','honduras'], // HONDURAS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:HRV','is_in:country_code:2','croatia'], // CROATIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:HTI','is_in:country_code:2','haiti'], // HAITI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:IDN','is_in:country_code:2','indonesia'], // INDONESIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:IND','is_in:country_code:2','india'], // INDIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:IOT','is_in:country_code:2','british_indian_ocean_territory'], // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:IRN','is_in:country_code:2','iran'], // IRAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:IRQ','is_in:country_code:2','iraq'], // IRAQ AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ISL','is_in:country_code:2','iceland'], // ICELAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ISR','is_in:country_code:2','israel'], // ISRAEL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ITA','is_in:country_code:2','italy'], // ITALY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:JEY','is_in:country_code:2','jersey'], // JERSEY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:JOR','is_in:country_code:2','jordan'], // JORDAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KAZ','is_in:country_code:2','kazakhstan'], // KAZAKHSTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KEN','is_in:country_code:2','kenya'], // KENYA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KGZ','is_in:country_code:2','kyrgyzstan'], // KYRGYZSTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KHM','is_in:country_code:2','cambodia'], // CAMBODIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KIR','is_in:country_code:2','kiribati'], // KIRIBATI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KNA','is_in:country_code:2','saint_kitts_and_nevis'], // SAINT KITTS AND NEVIS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KOR','is_in:country_code:2','korea,_south'], // KOREA, SOUTH AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:KWT','is_in:country_code:2','kuwait'], // KUWAIT AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LAO','is_in:country_code:2','laos'], // LAOS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LBN','is_in:country_code:2','lebanon'], // LEBANON AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LBR','is_in:country_code:2','liberia'], // LIBERIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LIE','is_in:country_code:2','liechtenstein'], // LIECHTENSTEIN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LKA','is_in:country_code:2','sri_lanka'], // SRI LANKA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LSO','is_in:country_code:2','lesotho'], // LESOTHO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LTU','is_in:country_code:2','lithuania'], // LITHUANIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LUX','is_in:country_code:2','luxembourg'], // LUXEMBOURG AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:LVA','is_in:country_code:2','latvia'], // LATVIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MAC','is_in:country_code:2','macau'], // MACAU AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MAF','is_in:country_code:2','saint_martin'], // SAINT MARTIN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MAR','is_in:country_code:2','morocco'], // MOROCCO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MCO','is_in:country_code:2','monaco'], // MONACO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MDA','is_in:country_code:2','moldova'], // MOLDOVA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MDG','is_in:country_code:2','madagascar'], // MADAGASCAR AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MDV','is_in:country_code:2','maldives'], // MALDIVES AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MEX','is_in:country_code:2','mexico'], // MEXICO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MHL','is_in:country_code:2','marshall_islands'], // MARSHALL ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MKD','is_in:country_code:2','macedonia'], // MACEDONIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MLI','is_in:country_code:2','mali'], // MALI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MLT','is_in:country_code:2','malta'], // MALTA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MMR','is_in:country_code:2','burma'], // BURMA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MNG','is_in:country_code:2','mongolia'], // MONGOLIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MNP','is_in:country_code:2','northern_mariana_islands'], // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MOZ','is_in:country_code:2','mozambique'], // MOZAMBIQUE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MRT','is_in:country_code:2','mauritania'], // MAURITANIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MTQ','is_in:country_code:2','martinique'], // MARTINIQUE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MUS','is_in:country_code:2','mauritius'], // MAURITIUS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MWI','is_in:country_code:2','malawi'], // MALAWI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MYS','is_in:country_code:2','malaysia'], // MALAYSIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:MYT','is_in:country_code:2','mayotte'], // MAYOTTE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NAM','is_in:country_code:2','namibia'], // NAMIBIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NCL','is_in:country_code:2','new_caledonia'], // NEW CALEDONIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NER','is_in:country_code:2','niger'], // NIGER AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NFK','is_in:country_code:2','norfolk_island'], // NORFOLK ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NGA','is_in:country_code:2','nigeria'], // NIGERIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NIC','is_in:country_code:2','nicaragua'], // NICARAGUA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NLD','is_in:country_code:2','netherlands'], // NETHERLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NOR','is_in:country_code:2','norway'], // NORWAY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NPL','is_in:country_code:2','nepal'], // NEPAL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NRU','is_in:country_code:2','nauru'], // NAURU AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:NZL','is_in:country_code:2','new_zealand'], // NEW ZEALAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:OMN','is_in:country_code:2','oman'], // OMAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PAK','is_in:country_code:2','pakistan'], // PAKISTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PAN','is_in:country_code:2','panama'], // PANAMA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PCN','is_in:country_code:2','pitcairn_islands'], // PITCAIRN ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PER','is_in:country_code:2','peru'], // PERU AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PHL','is_in:country_code:2','philippines'], // PHILIPPINES AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PLW','is_in:country_code:2','palau'], // PALAU AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:POL','is_in:country_code:2','poland'], // POLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PRI','is_in:country_code:2','puerto_rico'], // PUERTO RICO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PRK','is_in:country_code:2','korea,_north'], // KOREA, NORTH AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PRT','is_in:country_code:2','portugal'], // PORTUGAL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PRY','is_in:country_code:2','paraguay'], // PARAGUAY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PSE','is_in:country_code:2','palestinian_territory'], // PALESTINIAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:PYF','is_in:country_code:2','french_polynesia'], // FRENCH POLYNESIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:QAT','is_in:country_code:2','qatar'], // QATAR AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:REU','is_in:country_code:2','reunion'], // REUNION AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:RUS','is_in:country_code:2','russia'], // RUSSIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:RWA','is_in:country_code:2','rwanda'], // RWANDA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SAU','is_in:country_code:2','saudi_arabia'], // SAUDI ARABIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SDN','is_in:country_code:2','sudan'], // SUDAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SEN','is_in:country_code:2','senegal'], // SENEGAL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SGP','is_in:country_code:2','singapore'], // SINGAPORE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SGS','is_in:country_code:2','south_georgia_and_south_sandwich_islands'], // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SHN','is_in:country_code:2','saint_helena,_ascension,_and_tristan_da_cunha'], // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SLB','is_in:country_code:2','solomon_islands'], // SOLOMON ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SLE','is_in:country_code:2','sierra_leone'], // SIERRA LEONE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SLV','is_in:country_code:2','el_salvador'], // EL SALVADOR AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SMR','is_in:country_code:2','san_marino'], // SAN MARINO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SOM','is_in:country_code:2','somalia'], // SOMALIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SPM','is_in:country_code:2','saint_pierre_and_miquelon'], // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SRB','is_in:country_code:2','serbia'], // SERBIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SSD','is_in:country_code:2','south_sudan'], // SOUTH SUDAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:STP','is_in:country_code:2','sao_tome_and_principe'], // SAO TOME AND PRINCIPE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SUR','is_in:country_code:2','suriname'], // SURINAME AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SVK','is_in:country_code:2','slovakia'], // SLOVAKIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SVN','is_in:country_code:2','slovenia'], // SLOVENIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SWE','is_in:country_code:2','sweden'], // SWEDEN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SWZ','is_in:country_code:2','swaziland'], // SWAZILAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SXM','is_in:country_code:2','sint_maarten'], // SINT MAARTEN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SYC','is_in:country_code:2','seychelles'], // SEYCHELLES AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:SYR','is_in:country_code:2','syria'], // SYRIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TCA','is_in:country_code:2','turks_and_caicos_islands'], // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TCD','is_in:country_code:2','chad'], // CHAD AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TGO','is_in:country_code:2','togo'], // TOGO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:THA','is_in:country_code:2','thailand'], // THAILAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TJK','is_in:country_code:2','tajikistan'], // TAJIKISTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TLS','is_in:country_code:2','timor-leste'], // TIMOR-LESTE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TON','is_in:country_code:2','tonga'], // TONGA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TTO','is_in:country_code:2','trinidad_and_tobago'], // TRINIDAD AND TOBAGO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TUN','is_in:country_code:2','tunisia'], // TUNISIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TUR','is_in:country_code:2','turkey'], // TURKEY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TWN','is_in:country_code:2','taiwan'], // TAIWAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:TZA','is_in:country_code:2','tanzania'], // TANZANIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:UGA','is_in:country_code:2','uganda'], // UGANDA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:UKR','is_in:country_code:2','ukraine'], // UKRAINE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:URY','is_in:country_code:2','uruguay'], // URUGUAY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:USA','is_in:country_code:2','united_states'], // UNITED STATES AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:UZB','is_in:country_code:2','uzbekistan'], // UZBEKISTAN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VAT','is_in:country_code:2','vatican_city'], // VATICAN CITY AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VCT','is_in:country_code:2','saint_vincent_and_the_grenadines'], // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VEN','is_in:country_code:2','venezuela'], // VENEZUELA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VGB','is_in:country_code:2','virgin_islands,_british'], // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VIR','is_in:country_code:2','virgin_islands,_u.s.'], // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VNM','is_in:country_code:2','vietnam'], // VIETNAM AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:VUT','is_in:country_code:2','vanuatu'], // VANUATU AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:WLF','is_in:country_code:2','wallis_and_futuna'], // WALLIS AND FUTUNA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:WSM','is_in:country_code:2','samoa'], // SAMOA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XAC','is_in:country_code:2','ashmore_and_cartier_islands'], // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XAZ','is_in:country_code:2','entity_1'], // ENTITY 1 AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XBI','is_in:country_code:2','bassas_da_india'], // BASSAS DA INDIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XBK','is_in:country_code:2','baker_island'], // BAKER ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XCR','is_in:country_code:2','entity_2'], // ENTITY 2 AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XCS','is_in:country_code:2','coral_sea_islands'], // CORAL SEA ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XCY','is_in:country_code:2','entity_3'], // ENTITY 3 AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XEU','is_in:country_code:2','europa_island'], // EUROPA ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XGL','is_in:country_code:2','glorioso_islands'], // GLORIOSO ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XGZ','is_in:country_code:2','gaza_strip'], // GAZA STRIP AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XHO','is_in:country_code:2','howland_island'], // HOWLAND ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XJA','is_in:country_code:2','johnston_atoll'], // JOHNSTON ATOLL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XJM','is_in:country_code:2','jan_mayen'], // JAN MAYEN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XJN','is_in:country_code:2','juan_de_nova_island'], // JUAN DE NOVA ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XJV','is_in:country_code:2','jarvis_island'], // JARVIS ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XKM','is_in:country_code:2','entity_4'], // ENTITY 4 AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XKN','is_in:country_code:2','entity_5'], // ENTITY 5 AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XKR','is_in:country_code:2','kingman_reef'], // KINGMAN REEF AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XKS','is_in:country_code:2','kosovo'], // KOSOVO AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XMW','is_in:country_code:2','midway_islands'], // MIDWAY ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XNV','is_in:country_code:2','navassa_island'], // NAVASSA ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XPL','is_in:country_code:2','palmyra_atoll'], // PALMYRA ATOLL AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XPR','is_in:country_code:2','paracel_islands'], // PARACEL ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XQP','is_in:country_code:2','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XQZ','is_in:country_code:2','akrotiri'], // AKROTIRI AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XSP','is_in:country_code:2','spratly_islands'], // SPRATLY ISLANDS AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XSV','is_in:country_code:2','svalbard'], // SVALBARD AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XTR','is_in:country_code:2','tromelin_island'], // TROMELIN ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XWB','is_in:country_code:2','west_bank'], // WEST BANK AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XWK','is_in:country_code:2','wake_island'], // WAKE ISLAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XXD','is_in:country_code:2','dhekelia'], // DHEKELIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:XXX','is_in:country_code:2','no_mans_land'], // NO MAN"S LAND AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:YEM','is_in:country_code:2','yemen'], // YEMEN AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ZAF','is_in:country_code:2','south_africa'], // SOUTH AFRICA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ZMB','is_in:country_code:2','zambia'], // ZAMBIA AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:1-2:ZWE','is_in:country_code:2','zimbabwe'], // ZIMBABWE AS OF 2013-06-30
     ['ZI020_GE42','ge:GENC:3:ed3:AX3','is_in:country_code:2','entity_6'], // ENTITY 6 AS OF 2015-06-30
     ['ZI020_GE42','ge:ISO1:3:VI-15:AIA','is_in:country_code:2','anguilla'], // ANGUILLA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:ATA','is_in:country_code:2','antarctica'], // ANTARCTICA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:BIH','is_in:country_code:2','bosnia_and_herzegovina'], // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:BLZ','is_in:country_code:2','belize'], // BELIZE AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:BMU','is_in:country_code:2','bermuda'], // BERMUDA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:BRB','is_in:country_code:2','barbados'], // BARBADOS AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:BVT','is_in:country_code:2','bouvet_island'], // BOUVET ISLAND AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:GIB','is_in:country_code:2','gibraltar'], // GIBRALTAR AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:GRD','is_in:country_code:2','grenada'], // GRENADA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:GRL','is_in:country_code:2','greenland'], // GREENLAND AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:HUN','is_in:country_code:2','hungary'], // HUNGARY AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:IMN','is_in:country_code:2','isle_of_man'], // ISLE OF MAN AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:IRL','is_in:country_code:2','ireland'], // IRELAND AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:JAM','is_in:country_code:2','jamaica'], // JAMAICA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:JPN','is_in:country_code:2','japan'], // JAPAN AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:LBY','is_in:country_code:2','libya'], // LIBYA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:LCA','is_in:country_code:2','saint_lucia'], // SAINT LUCIA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:MNE','is_in:country_code:2','montenegro'], // MONTENEGRO AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:MSR','is_in:country_code:2','montserrat'], // MONTSERRAT AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:NIU','is_in:country_code:2','niue'], // NIUE AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:PNG','is_in:country_code:2','papua_new_guinea'], // PAPUA NEW GUINEA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:ROU','is_in:country_code:2','romania'], // ROMANIA AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:TKL','is_in:country_code:2','tokelau'], // TOKELAU AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:TKM','is_in:country_code:2','turkmenistan'], // TURKMENISTAN AS OF 2013-05-10
     ['ZI020_GE42','ge:ISO1:3:VI-15:TUV','is_in:country_code:2','tuvalu'], // TUVALU AS OF 2013-05-10
     ['ZI020_GE42','noInformation','is_in:country_code:2','no_information'], // No Information
     ['ZI020_GE42','other','is_in:country_code:2','other'], // Other

     // ZI020_GE43 - Designation : GENC Short URN-based Identifier [3]
     ['ZI020_GE43','ge:GENC:3:1-2:ABW','is_in:country_code:3','aruba'], // ARUBA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AFG','is_in:country_code:3','afghanistan'], // AFGHANISTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AGO','is_in:country_code:3','angola'], // ANGOLA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ALB','is_in:country_code:3','albania'], // ALBANIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AND','is_in:country_code:3','andorra'], // ANDORRA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ARE','is_in:country_code:3','united_arab_emirates'], // UNITED ARAB EMIRATES AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ARG','is_in:country_code:3','argentina'], // ARGENTINA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ARM','is_in:country_code:3','armenia'], // ARMENIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ASM','is_in:country_code:3','american_samoa'], // AMERICAN SAMOA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ATF','is_in:country_code:3','french_southern_and_antarctic_lands'], // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ATG','is_in:country_code:3','antigua_and_barbuda'], // ANTIGUA AND BARBUDA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AUS','is_in:country_code:3','australia'], // AUSTRALIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AUT','is_in:country_code:3','austria'], // AUSTRIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AX1','is_in:country_code:3','unknown'], // UNKNOWN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AX2','is_in:country_code:3','guantanamo_bay_naval_base'], // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:AZE','is_in:country_code:3','azerbaijan'], // AZERBAIJAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BDI','is_in:country_code:3','burundi'], // BURUNDI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BEL','is_in:country_code:3','belgium'], // BELGIUM AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BEN','is_in:country_code:3','benin'], // BENIN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BES','is_in:country_code:3','bonaire,_sint_eustatius,_and_saba'], // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BFA','is_in:country_code:3','burkina_faso'], // BURKINA FASO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BGD','is_in:country_code:3','bangladesh'], // BANGLADESH AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BGR','is_in:country_code:3','bulgaria'], // BULGARIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BHR','is_in:country_code:3','bahrain'], // BAHRAIN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BHS','is_in:country_code:3','bahamas,_the'], // BAHAMAS, THE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BLM','is_in:country_code:3','saint_barthelemy'], // SAINT BARTHELEMY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BLR','is_in:country_code:3','belarus'], // BELARUS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BOL','is_in:country_code:3','bolivia'], // BOLIVIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BRA','is_in:country_code:3','brazil'], // BRAZIL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BRN','is_in:country_code:3','brunei'], // BRUNEI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BTN','is_in:country_code:3','bhutan'], // BHUTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:BWA','is_in:country_code:3','botswana'], // BOTSWANA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CAF','is_in:country_code:3','central_african_republic'], // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CAN','is_in:country_code:3','canada'], // CANADA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CCK','is_in:country_code:3','cocos_(keeling)_islands'], // COCOS (KEELING) ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CHE','is_in:country_code:3','switzerland'], // SWITZERLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CHL','is_in:country_code:3','chile'], // CHILE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CHN','is_in:country_code:3','china'], // CHINA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CIV','is_in:country_code:3','cote_d"ivoire'], // COTE D"IVOIRE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CMR','is_in:country_code:3','cameroon'], // CAMEROON AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:COD','is_in:country_code:3','congo_(kinshasa)'], // CONGO (KINSHASA) AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:COG','is_in:country_code:3','congo_(brazzaville)'], // CONGO (BRAZZAVILLE) AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:COK','is_in:country_code:3','cook_islands'], // COOK ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:COL','is_in:country_code:3','colombia'], // COLOMBIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:COM','is_in:country_code:3','comoros'], // COMOROS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CPT','is_in:country_code:3','clipperton_island'], // CLIPPERTON ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CPV','is_in:country_code:3','cape_verde'], // CAPE VERDE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CRI','is_in:country_code:3','costa_rica'], // COSTA RICA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CUB','is_in:country_code:3','cuba'], // CUBA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CUW','is_in:country_code:3','curacao'], // CURACAO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CXR','is_in:country_code:3','christmas_island'], // CHRISTMAS ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CYM','is_in:country_code:3','cayman_islands'], // CAYMAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CYP','is_in:country_code:3','cyprus'], // CYPRUS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:CZE','is_in:country_code:3','czech_republic'], // CZECH REPUBLIC AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DEU','is_in:country_code:3','germany'], // GERMANY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DGA','is_in:country_code:3','diego_garcia'], // DIEGO GARCIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DJI','is_in:country_code:3','djibouti'], // DJIBOUTI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DMA','is_in:country_code:3','dominica'], // DOMINICA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DNK','is_in:country_code:3','denmark'], // DENMARK AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DOM','is_in:country_code:3','dominican_republic'], // DOMINICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:DZA','is_in:country_code:3','algeria'], // ALGERIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ECU','is_in:country_code:3','ecuador'], // ECUADOR AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:EGY','is_in:country_code:3','egypt'], // EGYPT AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ERI','is_in:country_code:3','eritrea'], // ERITREA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ESH','is_in:country_code:3','western_sahara'], // WESTERN SAHARA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ESP','is_in:country_code:3','spain'], // SPAIN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:EST','is_in:country_code:3','estonia'], // ESTONIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ETH','is_in:country_code:3','ethiopia'], // ETHIOPIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:FIN','is_in:country_code:3','finland'], // FINLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:FJI','is_in:country_code:3','fiji'], // FIJI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:FLK','is_in:country_code:3','falkland_islands_(islas_malvinas)'], // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:FRA','is_in:country_code:3','france'], // FRANCE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:FRO','is_in:country_code:3','faroe_islands'], // FAROE ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:FSM','is_in:country_code:3','micronesia,_federated_states_of'], // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GAB','is_in:country_code:3','gabon'], // GABON AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GBR','is_in:country_code:3','united_kingdom'], // UNITED KINGDOM AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GEO','is_in:country_code:3','georgia'], // GEORGIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GGY','is_in:country_code:3','guernsey'], // GUERNSEY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GHA','is_in:country_code:3','ghana'], // GHANA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GIN','is_in:country_code:3','guinea'], // GUINEA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GLP','is_in:country_code:3','guadeloupe'], // GUADELOUPE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GMB','is_in:country_code:3','gambia,_the'], // GAMBIA, THE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GNB','is_in:country_code:3','guinea-bissau'], // GUINEA-BISSAU AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GNQ','is_in:country_code:3','equatorial_guinea'], // EQUATORIAL GUINEA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GRC','is_in:country_code:3','greece'], // GREECE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GTM','is_in:country_code:3','guatemala'], // GUATEMALA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GUF','is_in:country_code:3','french_guiana'], // FRENCH GUIANA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GUM','is_in:country_code:3','guam'], // GUAM AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:GUY','is_in:country_code:3','guyana'], // GUYANA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:HKG','is_in:country_code:3','hong_kong'], // HONG KONG AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:HMD','is_in:country_code:3','heard_island_and_mcdonald_islands'], // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:HND','is_in:country_code:3','honduras'], // HONDURAS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:HRV','is_in:country_code:3','croatia'], // CROATIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:HTI','is_in:country_code:3','haiti'], // HAITI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:IDN','is_in:country_code:3','indonesia'], // INDONESIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:IND','is_in:country_code:3','india'], // INDIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:IOT','is_in:country_code:3','british_indian_ocean_territory'], // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:IRN','is_in:country_code:3','iran'], // IRAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:IRQ','is_in:country_code:3','iraq'], // IRAQ AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ISL','is_in:country_code:3','iceland'], // ICELAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ISR','is_in:country_code:3','israel'], // ISRAEL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ITA','is_in:country_code:3','italy'], // ITALY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:JEY','is_in:country_code:3','jersey'], // JERSEY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:JOR','is_in:country_code:3','jordan'], // JORDAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KAZ','is_in:country_code:3','kazakhstan'], // KAZAKHSTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KEN','is_in:country_code:3','kenya'], // KENYA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KGZ','is_in:country_code:3','kyrgyzstan'], // KYRGYZSTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KHM','is_in:country_code:3','cambodia'], // CAMBODIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KIR','is_in:country_code:3','kiribati'], // KIRIBATI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KNA','is_in:country_code:3','saint_kitts_and_nevis'], // SAINT KITTS AND NEVIS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KOR','is_in:country_code:3','korea,_south'], // KOREA, SOUTH AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:KWT','is_in:country_code:3','kuwait'], // KUWAIT AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LAO','is_in:country_code:3','laos'], // LAOS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LBN','is_in:country_code:3','lebanon'], // LEBANON AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LBR','is_in:country_code:3','liberia'], // LIBERIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LIE','is_in:country_code:3','liechtenstein'], // LIECHTENSTEIN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LKA','is_in:country_code:3','sri_lanka'], // SRI LANKA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LSO','is_in:country_code:3','lesotho'], // LESOTHO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LTU','is_in:country_code:3','lithuania'], // LITHUANIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LUX','is_in:country_code:3','luxembourg'], // LUXEMBOURG AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:LVA','is_in:country_code:3','latvia'], // LATVIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MAC','is_in:country_code:3','macau'], // MACAU AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MAF','is_in:country_code:3','saint_martin'], // SAINT MARTIN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MAR','is_in:country_code:3','morocco'], // MOROCCO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MCO','is_in:country_code:3','monaco'], // MONACO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MDA','is_in:country_code:3','moldova'], // MOLDOVA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MDG','is_in:country_code:3','madagascar'], // MADAGASCAR AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MDV','is_in:country_code:3','maldives'], // MALDIVES AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MEX','is_in:country_code:3','mexico'], // MEXICO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MHL','is_in:country_code:3','marshall_islands'], // MARSHALL ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MKD','is_in:country_code:3','macedonia'], // MACEDONIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MLI','is_in:country_code:3','mali'], // MALI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MLT','is_in:country_code:3','malta'], // MALTA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MMR','is_in:country_code:3','burma'], // BURMA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MNG','is_in:country_code:3','mongolia'], // MONGOLIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MNP','is_in:country_code:3','northern_mariana_islands'], // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MOZ','is_in:country_code:3','mozambique'], // MOZAMBIQUE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MRT','is_in:country_code:3','mauritania'], // MAURITANIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MTQ','is_in:country_code:3','martinique'], // MARTINIQUE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MUS','is_in:country_code:3','mauritius'], // MAURITIUS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MWI','is_in:country_code:3','malawi'], // MALAWI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MYS','is_in:country_code:3','malaysia'], // MALAYSIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:MYT','is_in:country_code:3','mayotte'], // MAYOTTE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NAM','is_in:country_code:3','namibia'], // NAMIBIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NCL','is_in:country_code:3','new_caledonia'], // NEW CALEDONIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NER','is_in:country_code:3','niger'], // NIGER AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NFK','is_in:country_code:3','norfolk_island'], // NORFOLK ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NGA','is_in:country_code:3','nigeria'], // NIGERIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NIC','is_in:country_code:3','nicaragua'], // NICARAGUA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NLD','is_in:country_code:3','netherlands'], // NETHERLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NOR','is_in:country_code:3','norway'], // NORWAY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NPL','is_in:country_code:3','nepal'], // NEPAL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NRU','is_in:country_code:3','nauru'], // NAURU AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:NZL','is_in:country_code:3','new_zealand'], // NEW ZEALAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:OMN','is_in:country_code:3','oman'], // OMAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PAK','is_in:country_code:3','pakistan'], // PAKISTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PAN','is_in:country_code:3','panama'], // PANAMA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PCN','is_in:country_code:3','pitcairn_islands'], // PITCAIRN ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PER','is_in:country_code:3','peru'], // PERU AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PHL','is_in:country_code:3','philippines'], // PHILIPPINES AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PLW','is_in:country_code:3','palau'], // PALAU AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:POL','is_in:country_code:3','poland'], // POLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PRI','is_in:country_code:3','puerto_rico'], // PUERTO RICO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PRK','is_in:country_code:3','korea,_north'], // KOREA, NORTH AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PRT','is_in:country_code:3','portugal'], // PORTUGAL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PRY','is_in:country_code:3','paraguay'], // PARAGUAY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PSE','is_in:country_code:3','palestinian_territory'], // PALESTINIAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:PYF','is_in:country_code:3','french_polynesia'], // FRENCH POLYNESIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:QAT','is_in:country_code:3','qatar'], // QATAR AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:REU','is_in:country_code:3','reunion'], // REUNION AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:RUS','is_in:country_code:3','russia'], // RUSSIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:RWA','is_in:country_code:3','rwanda'], // RWANDA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SAU','is_in:country_code:3','saudi_arabia'], // SAUDI ARABIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SDN','is_in:country_code:3','sudan'], // SUDAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SEN','is_in:country_code:3','senegal'], // SENEGAL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SGP','is_in:country_code:3','singapore'], // SINGAPORE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SGS','is_in:country_code:3','south_georgia_and_south_sandwich_islands'], // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SHN','is_in:country_code:3','saint_helena,_ascension,_and_tristan_da_cunha'], // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SLB','is_in:country_code:3','solomon_islands'], // SOLOMON ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SLE','is_in:country_code:3','sierra_leone'], // SIERRA LEONE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SLV','is_in:country_code:3','el_salvador'], // EL SALVADOR AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SMR','is_in:country_code:3','san_marino'], // SAN MARINO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SOM','is_in:country_code:3','somalia'], // SOMALIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SPM','is_in:country_code:3','saint_pierre_and_miquelon'], // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SRB','is_in:country_code:3','serbia'], // SERBIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SSD','is_in:country_code:3','south_sudan'], // SOUTH SUDAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:STP','is_in:country_code:3','sao_tome_and_principe'], // SAO TOME AND PRINCIPE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SUR','is_in:country_code:3','suriname'], // SURINAME AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SVK','is_in:country_code:3','slovakia'], // SLOVAKIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SVN','is_in:country_code:3','slovenia'], // SLOVENIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SWE','is_in:country_code:3','sweden'], // SWEDEN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SWZ','is_in:country_code:3','swaziland'], // SWAZILAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SXM','is_in:country_code:3','sint_maarten'], // SINT MAARTEN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SYC','is_in:country_code:3','seychelles'], // SEYCHELLES AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:SYR','is_in:country_code:3','syria'], // SYRIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TCA','is_in:country_code:3','turks_and_caicos_islands'], // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TCD','is_in:country_code:3','chad'], // CHAD AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TGO','is_in:country_code:3','togo'], // TOGO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:THA','is_in:country_code:3','thailand'], // THAILAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TJK','is_in:country_code:3','tajikistan'], // TAJIKISTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TLS','is_in:country_code:3','timor-leste'], // TIMOR-LESTE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TON','is_in:country_code:3','tonga'], // TONGA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TTO','is_in:country_code:3','trinidad_and_tobago'], // TRINIDAD AND TOBAGO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TUN','is_in:country_code:3','tunisia'], // TUNISIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TUR','is_in:country_code:3','turkey'], // TURKEY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TWN','is_in:country_code:3','taiwan'], // TAIWAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:TZA','is_in:country_code:3','tanzania'], // TANZANIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:UGA','is_in:country_code:3','uganda'], // UGANDA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:UKR','is_in:country_code:3','ukraine'], // UKRAINE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:URY','is_in:country_code:3','uruguay'], // URUGUAY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:USA','is_in:country_code:3','united_states'], // UNITED STATES AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:UZB','is_in:country_code:3','uzbekistan'], // UZBEKISTAN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VAT','is_in:country_code:3','vatican_city'], // VATICAN CITY AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VCT','is_in:country_code:3','saint_vincent_and_the_grenadines'], // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VEN','is_in:country_code:3','venezuela'], // VENEZUELA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VGB','is_in:country_code:3','virgin_islands,_british'], // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VIR','is_in:country_code:3','virgin_islands,_u.s.'], // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VNM','is_in:country_code:3','vietnam'], // VIETNAM AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:VUT','is_in:country_code:3','vanuatu'], // VANUATU AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:WLF','is_in:country_code:3','wallis_and_futuna'], // WALLIS AND FUTUNA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:WSM','is_in:country_code:3','samoa'], // SAMOA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XAC','is_in:country_code:3','ashmore_and_cartier_islands'], // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XAZ','is_in:country_code:3','entity_1'], // ENTITY 1 AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XBI','is_in:country_code:3','bassas_da_india'], // BASSAS DA INDIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XBK','is_in:country_code:3','baker_island'], // BAKER ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XCR','is_in:country_code:3','entity_2'], // ENTITY 2 AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XCS','is_in:country_code:3','coral_sea_islands'], // CORAL SEA ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XCY','is_in:country_code:3','entity_3'], // ENTITY 3 AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XEU','is_in:country_code:3','europa_island'], // EUROPA ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XGL','is_in:country_code:3','glorioso_islands'], // GLORIOSO ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XGZ','is_in:country_code:3','gaza_strip'], // GAZA STRIP AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XHO','is_in:country_code:3','howland_island'], // HOWLAND ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XJA','is_in:country_code:3','johnston_atoll'], // JOHNSTON ATOLL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XJM','is_in:country_code:3','jan_mayen'], // JAN MAYEN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XJN','is_in:country_code:3','juan_de_nova_island'], // JUAN DE NOVA ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XJV','is_in:country_code:3','jarvis_island'], // JARVIS ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XKM','is_in:country_code:3','entity_4'], // ENTITY 4 AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XKN','is_in:country_code:3','entity_5'], // ENTITY 5 AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XKR','is_in:country_code:3','kingman_reef'], // KINGMAN REEF AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XKS','is_in:country_code:3','kosovo'], // KOSOVO AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XMW','is_in:country_code:3','midway_islands'], // MIDWAY ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XNV','is_in:country_code:3','navassa_island'], // NAVASSA ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XPL','is_in:country_code:3','palmyra_atoll'], // PALMYRA ATOLL AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XPR','is_in:country_code:3','paracel_islands'], // PARACEL ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XQP','is_in:country_code:3','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XQZ','is_in:country_code:3','akrotiri'], // AKROTIRI AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XSP','is_in:country_code:3','spratly_islands'], // SPRATLY ISLANDS AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XSV','is_in:country_code:3','svalbard'], // SVALBARD AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XTR','is_in:country_code:3','tromelin_island'], // TROMELIN ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XWB','is_in:country_code:3','west_bank'], // WEST BANK AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XWK','is_in:country_code:3','wake_island'], // WAKE ISLAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XXD','is_in:country_code:3','dhekelia'], // DHEKELIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:XXX','is_in:country_code:3','no_mans_land'], // NO MAN"S LAND AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:YEM','is_in:country_code:3','yemen'], // YEMEN AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ZAF','is_in:country_code:3','south_africa'], // SOUTH AFRICA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ZMB','is_in:country_code:3','zambia'], // ZAMBIA AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:1-2:ZWE','is_in:country_code:3','zimbabwe'], // ZIMBABWE AS OF 2013-06-30
     ['ZI020_GE43','ge:GENC:3:ed3:AX3','is_in:country_code:3','entity_6'], // ENTITY 6 AS OF 2015-06-30
     ['ZI020_GE43','ge:ISO1:3:VI-15:AIA','is_in:country_code:3','anguilla'], // ANGUILLA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:ATA','is_in:country_code:3','antarctica'], // ANTARCTICA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:BIH','is_in:country_code:3','bosnia_and_herzegovina'], // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:BLZ','is_in:country_code:3','belize'], // BELIZE AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:BMU','is_in:country_code:3','bermuda'], // BERMUDA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:BRB','is_in:country_code:3','barbados'], // BARBADOS AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:BVT','is_in:country_code:3','bouvet_island'], // BOUVET ISLAND AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:GIB','is_in:country_code:3','gibraltar'], // GIBRALTAR AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:GRD','is_in:country_code:3','grenada'], // GRENADA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:GRL','is_in:country_code:3','greenland'], // GREENLAND AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:HUN','is_in:country_code:3','hungary'], // HUNGARY AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:IMN','is_in:country_code:3','isle_of_man'], // ISLE OF MAN AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:IRL','is_in:country_code:3','ireland'], // IRELAND AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:JAM','is_in:country_code:3','jamaica'], // JAMAICA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:JPN','is_in:country_code:3','japan'], // JAPAN AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:LBY','is_in:country_code:3','libya'], // LIBYA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:LCA','is_in:country_code:3','saint_lucia'], // SAINT LUCIA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:MNE','is_in:country_code:3','montenegro'], // MONTENEGRO AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:MSR','is_in:country_code:3','montserrat'], // MONTSERRAT AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:NIU','is_in:country_code:3','niue'], // NIUE AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:PNG','is_in:country_code:3','papua_new_guinea'], // PAPUA NEW GUINEA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:ROU','is_in:country_code:3','romania'], // ROMANIA AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:TKL','is_in:country_code:3','tokelau'], // TOKELAU AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:TKM','is_in:country_code:3','turkmenistan'], // TURKMENISTAN AS OF 2013-05-10
     ['ZI020_GE43','ge:ISO1:3:VI-15:TUV','is_in:country_code:3','tuvalu'], // TUVALU AS OF 2013-05-10
     ['ZI020_GE43','noInformation','is_in:country_code:3','no_information'], // No Information
     ['ZI020_GE43','other','is_in:country_code:3','other'], // Other

     // ZI020_GE44 - Designation : GENC Short URN-based Identifier [4]
     ['ZI020_GE44','ge:GENC:3:1-2:ABW','is_in:country_code:4','aruba'], // ARUBA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AFG','is_in:country_code:4','afghanistan'], // AFGHANISTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AGO','is_in:country_code:4','angola'], // ANGOLA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ALB','is_in:country_code:4','albania'], // ALBANIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AND','is_in:country_code:4','andorra'], // ANDORRA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ARE','is_in:country_code:4','united_arab_emirates'], // UNITED ARAB EMIRATES AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ARG','is_in:country_code:4','argentina'], // ARGENTINA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ARM','is_in:country_code:4','armenia'], // ARMENIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ASM','is_in:country_code:4','american_samoa'], // AMERICAN SAMOA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ATF','is_in:country_code:4','french_southern_and_antarctic_lands'], // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ATG','is_in:country_code:4','antigua_and_barbuda'], // ANTIGUA AND BARBUDA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AUS','is_in:country_code:4','australia'], // AUSTRALIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AUT','is_in:country_code:4','austria'], // AUSTRIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AX1','is_in:country_code:4','unknown'], // UNKNOWN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AX2','is_in:country_code:4','guantanamo_bay_naval_base'], // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:AZE','is_in:country_code:4','azerbaijan'], // AZERBAIJAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BDI','is_in:country_code:4','burundi'], // BURUNDI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BEL','is_in:country_code:4','belgium'], // BELGIUM AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BEN','is_in:country_code:4','benin'], // BENIN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BES','is_in:country_code:4','bonaire,_sint_eustatius,_and_saba'], // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BFA','is_in:country_code:4','burkina_faso'], // BURKINA FASO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BGD','is_in:country_code:4','bangladesh'], // BANGLADESH AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BGR','is_in:country_code:4','bulgaria'], // BULGARIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BHR','is_in:country_code:4','bahrain'], // BAHRAIN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BHS','is_in:country_code:4','bahamas,_the'], // BAHAMAS, THE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BLM','is_in:country_code:4','saint_barthelemy'], // SAINT BARTHELEMY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BLR','is_in:country_code:4','belarus'], // BELARUS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BOL','is_in:country_code:4','bolivia'], // BOLIVIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BRA','is_in:country_code:4','brazil'], // BRAZIL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BRN','is_in:country_code:4','brunei'], // BRUNEI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BTN','is_in:country_code:4','bhutan'], // BHUTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:BWA','is_in:country_code:4','botswana'], // BOTSWANA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CAF','is_in:country_code:4','central_african_republic'], // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CAN','is_in:country_code:4','canada'], // CANADA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CCK','is_in:country_code:4','cocos_(keeling)_islands'], // COCOS (KEELING) ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CHE','is_in:country_code:4','switzerland'], // SWITZERLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CHL','is_in:country_code:4','chile'], // CHILE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CHN','is_in:country_code:4','china'], // CHINA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CIV','is_in:country_code:4','cote_d"ivoire'], // COTE D"IVOIRE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CMR','is_in:country_code:4','cameroon'], // CAMEROON AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:COD','is_in:country_code:4','congo_(kinshasa)'], // CONGO (KINSHASA) AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:COG','is_in:country_code:4','congo_(brazzaville)'], // CONGO (BRAZZAVILLE) AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:COK','is_in:country_code:4','cook_islands'], // COOK ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:COL','is_in:country_code:4','colombia'], // COLOMBIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:COM','is_in:country_code:4','comoros'], // COMOROS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CPT','is_in:country_code:4','clipperton_island'], // CLIPPERTON ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CPV','is_in:country_code:4','cape_verde'], // CAPE VERDE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CRI','is_in:country_code:4','costa_rica'], // COSTA RICA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CUB','is_in:country_code:4','cuba'], // CUBA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CUW','is_in:country_code:4','curacao'], // CURACAO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CXR','is_in:country_code:4','christmas_island'], // CHRISTMAS ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CYM','is_in:country_code:4','cayman_islands'], // CAYMAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CYP','is_in:country_code:4','cyprus'], // CYPRUS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:CZE','is_in:country_code:4','czech_republic'], // CZECH REPUBLIC AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DEU','is_in:country_code:4','germany'], // GERMANY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DGA','is_in:country_code:4','diego_garcia'], // DIEGO GARCIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DJI','is_in:country_code:4','djibouti'], // DJIBOUTI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DMA','is_in:country_code:4','dominica'], // DOMINICA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DNK','is_in:country_code:4','denmark'], // DENMARK AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DOM','is_in:country_code:4','dominican_republic'], // DOMINICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:DZA','is_in:country_code:4','algeria'], // ALGERIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ECU','is_in:country_code:4','ecuador'], // ECUADOR AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:EGY','is_in:country_code:4','egypt'], // EGYPT AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ERI','is_in:country_code:4','eritrea'], // ERITREA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ESH','is_in:country_code:4','western_sahara'], // WESTERN SAHARA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ESP','is_in:country_code:4','spain'], // SPAIN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:EST','is_in:country_code:4','estonia'], // ESTONIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ETH','is_in:country_code:4','ethiopia'], // ETHIOPIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:FIN','is_in:country_code:4','finland'], // FINLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:FJI','is_in:country_code:4','fiji'], // FIJI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:FLK','is_in:country_code:4','falkland_islands_(islas_malvinas)'], // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:FRA','is_in:country_code:4','france'], // FRANCE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:FRO','is_in:country_code:4','faroe_islands'], // FAROE ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:FSM','is_in:country_code:4','micronesia,_federated_states_of'], // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GAB','is_in:country_code:4','gabon'], // GABON AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GBR','is_in:country_code:4','united_kingdom'], // UNITED KINGDOM AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GEO','is_in:country_code:4','georgia'], // GEORGIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GGY','is_in:country_code:4','guernsey'], // GUERNSEY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GHA','is_in:country_code:4','ghana'], // GHANA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GIN','is_in:country_code:4','guinea'], // GUINEA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GLP','is_in:country_code:4','guadeloupe'], // GUADELOUPE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GMB','is_in:country_code:4','gambia,_the'], // GAMBIA, THE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GNB','is_in:country_code:4','guinea-bissau'], // GUINEA-BISSAU AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GNQ','is_in:country_code:4','equatorial_guinea'], // EQUATORIAL GUINEA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GRC','is_in:country_code:4','greece'], // GREECE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GTM','is_in:country_code:4','guatemala'], // GUATEMALA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GUF','is_in:country_code:4','french_guiana'], // FRENCH GUIANA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GUM','is_in:country_code:4','guam'], // GUAM AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:GUY','is_in:country_code:4','guyana'], // GUYANA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:HKG','is_in:country_code:4','hong_kong'], // HONG KONG AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:HMD','is_in:country_code:4','heard_island_and_mcdonald_islands'], // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:HND','is_in:country_code:4','honduras'], // HONDURAS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:HRV','is_in:country_code:4','croatia'], // CROATIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:HTI','is_in:country_code:4','haiti'], // HAITI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:IDN','is_in:country_code:4','indonesia'], // INDONESIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:IND','is_in:country_code:4','india'], // INDIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:IOT','is_in:country_code:4','british_indian_ocean_territory'], // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:IRN','is_in:country_code:4','iran'], // IRAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:IRQ','is_in:country_code:4','iraq'], // IRAQ AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ISL','is_in:country_code:4','iceland'], // ICELAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ISR','is_in:country_code:4','israel'], // ISRAEL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ITA','is_in:country_code:4','italy'], // ITALY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:JEY','is_in:country_code:4','jersey'], // JERSEY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:JOR','is_in:country_code:4','jordan'], // JORDAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KAZ','is_in:country_code:4','kazakhstan'], // KAZAKHSTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KEN','is_in:country_code:4','kenya'], // KENYA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KGZ','is_in:country_code:4','kyrgyzstan'], // KYRGYZSTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KHM','is_in:country_code:4','cambodia'], // CAMBODIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KIR','is_in:country_code:4','kiribati'], // KIRIBATI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KNA','is_in:country_code:4','saint_kitts_and_nevis'], // SAINT KITTS AND NEVIS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KOR','is_in:country_code:4','korea,_south'], // KOREA, SOUTH AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:KWT','is_in:country_code:4','kuwait'], // KUWAIT AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LAO','is_in:country_code:4','laos'], // LAOS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LBN','is_in:country_code:4','lebanon'], // LEBANON AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LBR','is_in:country_code:4','liberia'], // LIBERIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LIE','is_in:country_code:4','liechtenstein'], // LIECHTENSTEIN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LKA','is_in:country_code:4','sri_lanka'], // SRI LANKA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LSO','is_in:country_code:4','lesotho'], // LESOTHO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LTU','is_in:country_code:4','lithuania'], // LITHUANIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LUX','is_in:country_code:4','luxembourg'], // LUXEMBOURG AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:LVA','is_in:country_code:4','latvia'], // LATVIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MAC','is_in:country_code:4','macau'], // MACAU AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MAF','is_in:country_code:4','saint_martin'], // SAINT MARTIN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MAR','is_in:country_code:4','morocco'], // MOROCCO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MCO','is_in:country_code:4','monaco'], // MONACO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MDA','is_in:country_code:4','moldova'], // MOLDOVA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MDG','is_in:country_code:4','madagascar'], // MADAGASCAR AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MDV','is_in:country_code:4','maldives'], // MALDIVES AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MEX','is_in:country_code:4','mexico'], // MEXICO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MHL','is_in:country_code:4','marshall_islands'], // MARSHALL ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MKD','is_in:country_code:4','macedonia'], // MACEDONIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MLI','is_in:country_code:4','mali'], // MALI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MLT','is_in:country_code:4','malta'], // MALTA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MMR','is_in:country_code:4','burma'], // BURMA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MNG','is_in:country_code:4','mongolia'], // MONGOLIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MNP','is_in:country_code:4','northern_mariana_islands'], // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MOZ','is_in:country_code:4','mozambique'], // MOZAMBIQUE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MRT','is_in:country_code:4','mauritania'], // MAURITANIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MTQ','is_in:country_code:4','martinique'], // MARTINIQUE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MUS','is_in:country_code:4','mauritius'], // MAURITIUS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MWI','is_in:country_code:4','malawi'], // MALAWI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MYS','is_in:country_code:4','malaysia'], // MALAYSIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:MYT','is_in:country_code:4','mayotte'], // MAYOTTE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NAM','is_in:country_code:4','namibia'], // NAMIBIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NCL','is_in:country_code:4','new_caledonia'], // NEW CALEDONIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NER','is_in:country_code:4','niger'], // NIGER AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NFK','is_in:country_code:4','norfolk_island'], // NORFOLK ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NGA','is_in:country_code:4','nigeria'], // NIGERIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NIC','is_in:country_code:4','nicaragua'], // NICARAGUA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NLD','is_in:country_code:4','netherlands'], // NETHERLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NOR','is_in:country_code:4','norway'], // NORWAY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NPL','is_in:country_code:4','nepal'], // NEPAL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NRU','is_in:country_code:4','nauru'], // NAURU AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:NZL','is_in:country_code:4','new_zealand'], // NEW ZEALAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:OMN','is_in:country_code:4','oman'], // OMAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PAK','is_in:country_code:4','pakistan'], // PAKISTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PAN','is_in:country_code:4','panama'], // PANAMA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PCN','is_in:country_code:4','pitcairn_islands'], // PITCAIRN ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PER','is_in:country_code:4','peru'], // PERU AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PHL','is_in:country_code:4','philippines'], // PHILIPPINES AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PLW','is_in:country_code:4','palau'], // PALAU AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:POL','is_in:country_code:4','poland'], // POLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PRI','is_in:country_code:4','puerto_rico'], // PUERTO RICO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PRK','is_in:country_code:4','korea,_north'], // KOREA, NORTH AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PRT','is_in:country_code:4','portugal'], // PORTUGAL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PRY','is_in:country_code:4','paraguay'], // PARAGUAY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PSE','is_in:country_code:4','palestinian_territory'], // PALESTINIAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:PYF','is_in:country_code:4','french_polynesia'], // FRENCH POLYNESIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:QAT','is_in:country_code:4','qatar'], // QATAR AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:REU','is_in:country_code:4','reunion'], // REUNION AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:RUS','is_in:country_code:4','russia'], // RUSSIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:RWA','is_in:country_code:4','rwanda'], // RWANDA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SAU','is_in:country_code:4','saudi_arabia'], // SAUDI ARABIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SDN','is_in:country_code:4','sudan'], // SUDAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SEN','is_in:country_code:4','senegal'], // SENEGAL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SGP','is_in:country_code:4','singapore'], // SINGAPORE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SGS','is_in:country_code:4','south_georgia_and_south_sandwich_islands'], // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SHN','is_in:country_code:4','saint_helena,_ascension,_and_tristan_da_cunha'], // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SLB','is_in:country_code:4','solomon_islands'], // SOLOMON ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SLE','is_in:country_code:4','sierra_leone'], // SIERRA LEONE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SLV','is_in:country_code:4','el_salvador'], // EL SALVADOR AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SMR','is_in:country_code:4','san_marino'], // SAN MARINO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SOM','is_in:country_code:4','somalia'], // SOMALIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SPM','is_in:country_code:4','saint_pierre_and_miquelon'], // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SRB','is_in:country_code:4','serbia'], // SERBIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SSD','is_in:country_code:4','south_sudan'], // SOUTH SUDAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:STP','is_in:country_code:4','sao_tome_and_principe'], // SAO TOME AND PRINCIPE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SUR','is_in:country_code:4','suriname'], // SURINAME AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SVK','is_in:country_code:4','slovakia'], // SLOVAKIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SVN','is_in:country_code:4','slovenia'], // SLOVENIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SWE','is_in:country_code:4','sweden'], // SWEDEN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SWZ','is_in:country_code:4','swaziland'], // SWAZILAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SXM','is_in:country_code:4','sint_maarten'], // SINT MAARTEN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SYC','is_in:country_code:4','seychelles'], // SEYCHELLES AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:SYR','is_in:country_code:4','syria'], // SYRIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TCA','is_in:country_code:4','turks_and_caicos_islands'], // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TCD','is_in:country_code:4','chad'], // CHAD AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TGO','is_in:country_code:4','togo'], // TOGO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:THA','is_in:country_code:4','thailand'], // THAILAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TJK','is_in:country_code:4','tajikistan'], // TAJIKISTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TLS','is_in:country_code:4','timor-leste'], // TIMOR-LESTE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TON','is_in:country_code:4','tonga'], // TONGA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TTO','is_in:country_code:4','trinidad_and_tobago'], // TRINIDAD AND TOBAGO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TUN','is_in:country_code:4','tunisia'], // TUNISIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TUR','is_in:country_code:4','turkey'], // TURKEY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TWN','is_in:country_code:4','taiwan'], // TAIWAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:TZA','is_in:country_code:4','tanzania'], // TANZANIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:UGA','is_in:country_code:4','uganda'], // UGANDA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:UKR','is_in:country_code:4','ukraine'], // UKRAINE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:URY','is_in:country_code:4','uruguay'], // URUGUAY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:USA','is_in:country_code:4','united_states'], // UNITED STATES AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:UZB','is_in:country_code:4','uzbekistan'], // UZBEKISTAN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VAT','is_in:country_code:4','vatican_city'], // VATICAN CITY AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VCT','is_in:country_code:4','saint_vincent_and_the_grenadines'], // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VEN','is_in:country_code:4','venezuela'], // VENEZUELA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VGB','is_in:country_code:4','virgin_islands,_british'], // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VIR','is_in:country_code:4','virgin_islands,_u.s.'], // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VNM','is_in:country_code:4','vietnam'], // VIETNAM AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:VUT','is_in:country_code:4','vanuatu'], // VANUATU AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:WLF','is_in:country_code:4','wallis_and_futuna'], // WALLIS AND FUTUNA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:WSM','is_in:country_code:4','samoa'], // SAMOA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XAC','is_in:country_code:4','ashmore_and_cartier_islands'], // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XAZ','is_in:country_code:4','entity_1'], // ENTITY 1 AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XBI','is_in:country_code:4','bassas_da_india'], // BASSAS DA INDIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XBK','is_in:country_code:4','baker_island'], // BAKER ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XCR','is_in:country_code:4','entity_2'], // ENTITY 2 AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XCS','is_in:country_code:4','coral_sea_islands'], // CORAL SEA ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XCY','is_in:country_code:4','entity_3'], // ENTITY 3 AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XEU','is_in:country_code:4','europa_island'], // EUROPA ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XGL','is_in:country_code:4','glorioso_islands'], // GLORIOSO ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XGZ','is_in:country_code:4','gaza_strip'], // GAZA STRIP AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XHO','is_in:country_code:4','howland_island'], // HOWLAND ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XJA','is_in:country_code:4','johnston_atoll'], // JOHNSTON ATOLL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XJM','is_in:country_code:4','jan_mayen'], // JAN MAYEN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XJN','is_in:country_code:4','juan_de_nova_island'], // JUAN DE NOVA ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XJV','is_in:country_code:4','jarvis_island'], // JARVIS ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XKM','is_in:country_code:4','entity_4'], // ENTITY 4 AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XKN','is_in:country_code:4','entity_5'], // ENTITY 5 AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XKR','is_in:country_code:4','kingman_reef'], // KINGMAN REEF AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XKS','is_in:country_code:4','kosovo'], // KOSOVO AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XMW','is_in:country_code:4','midway_islands'], // MIDWAY ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XNV','is_in:country_code:4','navassa_island'], // NAVASSA ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XPL','is_in:country_code:4','palmyra_atoll'], // PALMYRA ATOLL AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XPR','is_in:country_code:4','paracel_islands'], // PARACEL ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XQP','is_in:country_code:4','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XQZ','is_in:country_code:4','akrotiri'], // AKROTIRI AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XSP','is_in:country_code:4','spratly_islands'], // SPRATLY ISLANDS AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XSV','is_in:country_code:4','svalbard'], // SVALBARD AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XTR','is_in:country_code:4','tromelin_island'], // TROMELIN ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XWB','is_in:country_code:4','west_bank'], // WEST BANK AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XWK','is_in:country_code:4','wake_island'], // WAKE ISLAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XXD','is_in:country_code:4','dhekelia'], // DHEKELIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:XXX','is_in:country_code:4','no_mans_land'], // NO MAN"S LAND AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:YEM','is_in:country_code:4','yemen'], // YEMEN AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ZAF','is_in:country_code:4','south_africa'], // SOUTH AFRICA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ZMB','is_in:country_code:4','zambia'], // ZAMBIA AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:1-2:ZWE','is_in:country_code:4','zimbabwe'], // ZIMBABWE AS OF 2013-06-30
     ['ZI020_GE44','ge:GENC:3:ed3:AX3','is_in:country_code:4','entity_6'], // ENTITY 6 AS OF 2015-06-30
     ['ZI020_GE44','ge:ISO1:3:VI-15:AIA','is_in:country_code:4','anguilla'], // ANGUILLA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:ATA','is_in:country_code:4','antarctica'], // ANTARCTICA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:BIH','is_in:country_code:4','bosnia_and_herzegovina'], // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:BLZ','is_in:country_code:4','belize'], // BELIZE AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:BMU','is_in:country_code:4','bermuda'], // BERMUDA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:BRB','is_in:country_code:4','barbados'], // BARBADOS AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:BVT','is_in:country_code:4','bouvet_island'], // BOUVET ISLAND AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:GIB','is_in:country_code:4','gibraltar'], // GIBRALTAR AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:GRD','is_in:country_code:4','grenada'], // GRENADA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:GRL','is_in:country_code:4','greenland'], // GREENLAND AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:HUN','is_in:country_code:4','hungary'], // HUNGARY AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:IMN','is_in:country_code:4','isle_of_man'], // ISLE OF MAN AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:IRL','is_in:country_code:4','ireland'], // IRELAND AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:JAM','is_in:country_code:4','jamaica'], // JAMAICA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:JPN','is_in:country_code:4','japan'], // JAPAN AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:LBY','is_in:country_code:4','libya'], // LIBYA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:LCA','is_in:country_code:4','saint_lucia'], // SAINT LUCIA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:MNE','is_in:country_code:4','montenegro'], // MONTENEGRO AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:MSR','is_in:country_code:4','montserrat'], // MONTSERRAT AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:NIU','is_in:country_code:4','niue'], // NIUE AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:PNG','is_in:country_code:4','papua_new_guinea'], // PAPUA NEW GUINEA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:ROU','is_in:country_code:4','romania'], // ROMANIA AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:TKL','is_in:country_code:4','tokelau'], // TOKELAU AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:TKM','is_in:country_code:4','turkmenistan'], // TURKMENISTAN AS OF 2013-05-10
     ['ZI020_GE44','ge:ISO1:3:VI-15:TUV','is_in:country_code:4','tuvalu'], // TUVALU AS OF 2013-05-10
     ['ZI020_GE44','noInformation','is_in:country_code:4','no_information'], // No Information
     ['ZI020_GE44','other','is_in:country_code:4','other'], // Other

     // ZI020_GE4A - Designation : GENC Short URN-based Identifier
     ['ZI020_GE4A','ge:GENC:3:1-2:ABW','is_in:country_codeA','aruba'], // ARUBA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AFG','is_in:country_codeA','afghanistan'], // AFGHANISTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AGO','is_in:country_codeA','angola'], // ANGOLA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ALB','is_in:country_codeA','albania'], // ALBANIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AND','is_in:country_codeA','andorra'], // ANDORRA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ARE','is_in:country_codeA','united_arab_emirates'], // UNITED ARAB EMIRATES AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ARG','is_in:country_codeA','argentina'], // ARGENTINA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ARM','is_in:country_codeA','armenia'], // ARMENIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ASM','is_in:country_codeA','american_samoa'], // AMERICAN SAMOA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ATF','is_in:country_codeA','french_southern_and_antarctic_lands'], // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ATG','is_in:country_codeA','antigua_and_barbuda'], // ANTIGUA AND BARBUDA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AUS','is_in:country_codeA','australia'], // AUSTRALIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AUT','is_in:country_codeA','austria'], // AUSTRIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AX1','is_in:country_codeA','unknown'], // UNKNOWN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AX2','is_in:country_codeA','guantanamo_bay_naval_base'], // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:AZE','is_in:country_codeA','azerbaijan'], // AZERBAIJAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BDI','is_in:country_codeA','burundi'], // BURUNDI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BEL','is_in:country_codeA','belgium'], // BELGIUM AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BEN','is_in:country_codeA','benin'], // BENIN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BES','is_in:country_codeA','bonaire,_sint_eustatius,_and_saba'], // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BFA','is_in:country_codeA','burkina_faso'], // BURKINA FASO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BGD','is_in:country_codeA','bangladesh'], // BANGLADESH AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BGR','is_in:country_codeA','bulgaria'], // BULGARIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BHR','is_in:country_codeA','bahrain'], // BAHRAIN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BHS','is_in:country_codeA','bahamas,_the'], // BAHAMAS, THE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BLM','is_in:country_codeA','saint_barthelemy'], // SAINT BARTHELEMY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BLR','is_in:country_codeA','belarus'], // BELARUS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BOL','is_in:country_codeA','bolivia'], // BOLIVIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BRA','is_in:country_codeA','brazil'], // BRAZIL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BRN','is_in:country_codeA','brunei'], // BRUNEI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BTN','is_in:country_codeA','bhutan'], // BHUTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:BWA','is_in:country_codeA','botswana'], // BOTSWANA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CAF','is_in:country_codeA','central_african_republic'], // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CAN','is_in:country_codeA','canada'], // CANADA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CCK','is_in:country_codeA','cocos_(keeling)_islands'], // COCOS (KEELING) ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CHE','is_in:country_codeA','switzerland'], // SWITZERLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CHL','is_in:country_codeA','chile'], // CHILE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CHN','is_in:country_codeA','china'], // CHINA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CIV','is_in:country_codeA','cote_d"ivoire'], // COTE D"IVOIRE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CMR','is_in:country_codeA','cameroon'], // CAMEROON AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:COD','is_in:country_codeA','congo_(kinshasa)'], // CONGO (KINSHASA) AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:COG','is_in:country_codeA','congo_(brazzaville)'], // CONGO (BRAZZAVILLE) AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:COK','is_in:country_codeA','cook_islands'], // COOK ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:COL','is_in:country_codeA','colombia'], // COLOMBIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:COM','is_in:country_codeA','comoros'], // COMOROS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CPT','is_in:country_codeA','clipperton_island'], // CLIPPERTON ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CPV','is_in:country_codeA','cape_verde'], // CAPE VERDE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CRI','is_in:country_codeA','costa_rica'], // COSTA RICA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CUB','is_in:country_codeA','cuba'], // CUBA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CUW','is_in:country_codeA','curacao'], // CURACAO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CXR','is_in:country_codeA','christmas_island'], // CHRISTMAS ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CYM','is_in:country_codeA','cayman_islands'], // CAYMAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CYP','is_in:country_codeA','cyprus'], // CYPRUS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:CZE','is_in:country_codeA','czech_republic'], // CZECH REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DEU','is_in:country_codeA','germany'], // GERMANY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DGA','is_in:country_codeA','diego_garcia'], // DIEGO GARCIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DJI','is_in:country_codeA','djibouti'], // DJIBOUTI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DMA','is_in:country_codeA','dominica'], // DOMINICA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DNK','is_in:country_codeA','denmark'], // DENMARK AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DOM','is_in:country_codeA','dominican_republic'], // DOMINICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:DZA','is_in:country_codeA','algeria'], // ALGERIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ECU','is_in:country_codeA','ecuador'], // ECUADOR AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:EGY','is_in:country_codeA','egypt'], // EGYPT AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ERI','is_in:country_codeA','eritrea'], // ERITREA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ESH','is_in:country_codeA','western_sahara'], // WESTERN SAHARA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ESP','is_in:country_codeA','spain'], // SPAIN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:EST','is_in:country_codeA','estonia'], // ESTONIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ETH','is_in:country_codeA','ethiopia'], // ETHIOPIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:FIN','is_in:country_codeA','finland'], // FINLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:FJI','is_in:country_codeA','fiji'], // FIJI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:FLK','is_in:country_codeA','falkland_islands_(islas_malvinas)'], // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:FRA','is_in:country_codeA','france'], // FRANCE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:FRO','is_in:country_codeA','faroe_islands'], // FAROE ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:FSM','is_in:country_codeA','micronesia,_federated_states_of'], // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GAB','is_in:country_codeA','gabon'], // GABON AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GBR','is_in:country_codeA','united_kingdom'], // UNITED KINGDOM AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GEO','is_in:country_codeA','georgia'], // GEORGIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GGY','is_in:country_codeA','guernsey'], // GUERNSEY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GHA','is_in:country_codeA','ghana'], // GHANA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GIN','is_in:country_codeA','guinea'], // GUINEA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GLP','is_in:country_codeA','guadeloupe'], // GUADELOUPE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GMB','is_in:country_codeA','gambia,_the'], // GAMBIA, THE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GNB','is_in:country_codeA','guinea-bissau'], // GUINEA-BISSAU AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GNQ','is_in:country_codeA','equatorial_guinea'], // EQUATORIAL GUINEA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GRC','is_in:country_codeA','greece'], // GREECE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GTM','is_in:country_codeA','guatemala'], // GUATEMALA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GUF','is_in:country_codeA','french_guiana'], // FRENCH GUIANA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GUM','is_in:country_codeA','guam'], // GUAM AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:GUY','is_in:country_codeA','guyana'], // GUYANA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:HKG','is_in:country_codeA','hong_kong'], // HONG KONG AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:HMD','is_in:country_codeA','heard_island_and_mcdonald_islands'], // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:HND','is_in:country_codeA','honduras'], // HONDURAS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:HRV','is_in:country_codeA','croatia'], // CROATIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:HTI','is_in:country_codeA','haiti'], // HAITI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:IDN','is_in:country_codeA','indonesia'], // INDONESIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:IND','is_in:country_codeA','india'], // INDIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:IOT','is_in:country_codeA','british_indian_ocean_territory'], // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:IRN','is_in:country_codeA','iran'], // IRAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:IRQ','is_in:country_codeA','iraq'], // IRAQ AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ISL','is_in:country_codeA','iceland'], // ICELAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ISR','is_in:country_codeA','israel'], // ISRAEL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ITA','is_in:country_codeA','italy'], // ITALY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:JEY','is_in:country_codeA','jersey'], // JERSEY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:JOR','is_in:country_codeA','jordan'], // JORDAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KAZ','is_in:country_codeA','kazakhstan'], // KAZAKHSTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KEN','is_in:country_codeA','kenya'], // KENYA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KGZ','is_in:country_codeA','kyrgyzstan'], // KYRGYZSTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KHM','is_in:country_codeA','cambodia'], // CAMBODIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KIR','is_in:country_codeA','kiribati'], // KIRIBATI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KNA','is_in:country_codeA','saint_kitts_and_nevis'], // SAINT KITTS AND NEVIS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KOR','is_in:country_codeA','korea,_south'], // KOREA, SOUTH AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:KWT','is_in:country_codeA','kuwait'], // KUWAIT AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LAO','is_in:country_codeA','laos'], // LAOS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LBN','is_in:country_codeA','lebanon'], // LEBANON AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LBR','is_in:country_codeA','liberia'], // LIBERIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LIE','is_in:country_codeA','liechtenstein'], // LIECHTENSTEIN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LKA','is_in:country_codeA','sri_lanka'], // SRI LANKA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LSO','is_in:country_codeA','lesotho'], // LESOTHO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LTU','is_in:country_codeA','lithuania'], // LITHUANIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LUX','is_in:country_codeA','luxembourg'], // LUXEMBOURG AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:LVA','is_in:country_codeA','latvia'], // LATVIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MAC','is_in:country_codeA','macau'], // MACAU AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MAF','is_in:country_codeA','saint_martin'], // SAINT MARTIN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MAR','is_in:country_codeA','morocco'], // MOROCCO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MCO','is_in:country_codeA','monaco'], // MONACO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MDA','is_in:country_codeA','moldova'], // MOLDOVA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MDG','is_in:country_codeA','madagascar'], // MADAGASCAR AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MDV','is_in:country_codeA','maldives'], // MALDIVES AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MEX','is_in:country_codeA','mexico'], // MEXICO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MHL','is_in:country_codeA','marshall_islands'], // MARSHALL ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MKD','is_in:country_codeA','macedonia'], // MACEDONIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MLI','is_in:country_codeA','mali'], // MALI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MLT','is_in:country_codeA','malta'], // MALTA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MMR','is_in:country_codeA','burma'], // BURMA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MNG','is_in:country_codeA','mongolia'], // MONGOLIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MNP','is_in:country_codeA','northern_mariana_islands'], // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MOZ','is_in:country_codeA','mozambique'], // MOZAMBIQUE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MRT','is_in:country_codeA','mauritania'], // MAURITANIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MTQ','is_in:country_codeA','martinique'], // MARTINIQUE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MUS','is_in:country_codeA','mauritius'], // MAURITIUS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MWI','is_in:country_codeA','malawi'], // MALAWI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MYS','is_in:country_codeA','malaysia'], // MALAYSIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:MYT','is_in:country_codeA','mayotte'], // MAYOTTE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NAM','is_in:country_codeA','namibia'], // NAMIBIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NCL','is_in:country_codeA','new_caledonia'], // NEW CALEDONIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NER','is_in:country_codeA','niger'], // NIGER AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NFK','is_in:country_codeA','norfolk_island'], // NORFOLK ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NGA','is_in:country_codeA','nigeria'], // NIGERIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NIC','is_in:country_codeA','nicaragua'], // NICARAGUA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NLD','is_in:country_codeA','netherlands'], // NETHERLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NOR','is_in:country_codeA','norway'], // NORWAY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NPL','is_in:country_codeA','nepal'], // NEPAL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NRU','is_in:country_codeA','nauru'], // NAURU AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:NZL','is_in:country_codeA','new_zealand'], // NEW ZEALAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:OMN','is_in:country_codeA','oman'], // OMAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PAK','is_in:country_codeA','pakistan'], // PAKISTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PAN','is_in:country_codeA','panama'], // PANAMA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PCN','is_in:country_codeA','pitcairn_islands'], // PITCAIRN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PER','is_in:country_codeA','peru'], // PERU AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PHL','is_in:country_codeA','philippines'], // PHILIPPINES AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PLW','is_in:country_codeA','palau'], // PALAU AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:POL','is_in:country_codeA','poland'], // POLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PRI','is_in:country_codeA','puerto_rico'], // PUERTO RICO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PRK','is_in:country_codeA','korea,_north'], // KOREA, NORTH AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PRT','is_in:country_codeA','portugal'], // PORTUGAL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PRY','is_in:country_codeA','paraguay'], // PARAGUAY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PSE','is_in:country_codeA','palestinian_territory'], // PALESTINIAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:PYF','is_in:country_codeA','french_polynesia'], // FRENCH POLYNESIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:QAT','is_in:country_codeA','qatar'], // QATAR AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:REU','is_in:country_codeA','reunion'], // REUNION AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:RUS','is_in:country_codeA','russia'], // RUSSIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:RWA','is_in:country_codeA','rwanda'], // RWANDA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SAU','is_in:country_codeA','saudi_arabia'], // SAUDI ARABIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SDN','is_in:country_codeA','sudan'], // SUDAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SEN','is_in:country_codeA','senegal'], // SENEGAL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SGP','is_in:country_codeA','singapore'], // SINGAPORE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SGS','is_in:country_codeA','south_georgia_and_south_sandwich_islands'], // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SHN','is_in:country_codeA','saint_helena,_ascension,_and_tristan_da_cunha'], // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SLB','is_in:country_codeA','solomon_islands'], // SOLOMON ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SLE','is_in:country_codeA','sierra_leone'], // SIERRA LEONE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SLV','is_in:country_codeA','el_salvador'], // EL SALVADOR AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SMR','is_in:country_codeA','san_marino'], // SAN MARINO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SOM','is_in:country_codeA','somalia'], // SOMALIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SPM','is_in:country_codeA','saint_pierre_and_miquelon'], // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SRB','is_in:country_codeA','serbia'], // SERBIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SSD','is_in:country_codeA','south_sudan'], // SOUTH SUDAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:STP','is_in:country_codeA','sao_tome_and_principe'], // SAO TOME AND PRINCIPE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SUR','is_in:country_codeA','suriname'], // SURINAME AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SVK','is_in:country_codeA','slovakia'], // SLOVAKIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SVN','is_in:country_codeA','slovenia'], // SLOVENIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SWE','is_in:country_codeA','sweden'], // SWEDEN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SWZ','is_in:country_codeA','swaziland'], // SWAZILAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SXM','is_in:country_codeA','sint_maarten'], // SINT MAARTEN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SYC','is_in:country_codeA','seychelles'], // SEYCHELLES AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:SYR','is_in:country_codeA','syria'], // SYRIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TCA','is_in:country_codeA','turks_and_caicos_islands'], // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TCD','is_in:country_codeA','chad'], // CHAD AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TGO','is_in:country_codeA','togo'], // TOGO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:THA','is_in:country_codeA','thailand'], // THAILAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TJK','is_in:country_codeA','tajikistan'], // TAJIKISTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TLS','is_in:country_codeA','timor-leste'], // TIMOR-LESTE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TON','is_in:country_codeA','tonga'], // TONGA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TTO','is_in:country_codeA','trinidad_and_tobago'], // TRINIDAD AND TOBAGO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TUN','is_in:country_codeA','tunisia'], // TUNISIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TUR','is_in:country_codeA','turkey'], // TURKEY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TWN','is_in:country_codeA','taiwan'], // TAIWAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:TZA','is_in:country_codeA','tanzania'], // TANZANIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:UGA','is_in:country_codeA','uganda'], // UGANDA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:UKR','is_in:country_codeA','ukraine'], // UKRAINE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:URY','is_in:country_codeA','uruguay'], // URUGUAY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:USA','is_in:country_codeA','united_states'], // UNITED STATES AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:UZB','is_in:country_codeA','uzbekistan'], // UZBEKISTAN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VAT','is_in:country_codeA','vatican_city'], // VATICAN CITY AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VCT','is_in:country_codeA','saint_vincent_and_the_grenadines'], // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VEN','is_in:country_codeA','venezuela'], // VENEZUELA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VGB','is_in:country_codeA','virgin_islands,_british'], // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VIR','is_in:country_codeA','virgin_islands,_u.s.'], // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VNM','is_in:country_codeA','vietnam'], // VIETNAM AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:VUT','is_in:country_codeA','vanuatu'], // VANUATU AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:WLF','is_in:country_codeA','wallis_and_futuna'], // WALLIS AND FUTUNA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:WSM','is_in:country_codeA','samoa'], // SAMOA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XAC','is_in:country_codeA','ashmore_and_cartier_islands'], // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XAZ','is_in:country_codeA','entity_1'], // ENTITY 1 AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XBI','is_in:country_codeA','bassas_da_india'], // BASSAS DA INDIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XBK','is_in:country_codeA','baker_island'], // BAKER ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XCR','is_in:country_codeA','entity_2'], // ENTITY 2 AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XCS','is_in:country_codeA','coral_sea_islands'], // CORAL SEA ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XCY','is_in:country_codeA','entity_3'], // ENTITY 3 AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XEU','is_in:country_codeA','europa_island'], // EUROPA ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XGL','is_in:country_codeA','glorioso_islands'], // GLORIOSO ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XGZ','is_in:country_codeA','gaza_strip'], // GAZA STRIP AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XHO','is_in:country_codeA','howland_island'], // HOWLAND ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XJA','is_in:country_codeA','johnston_atoll'], // JOHNSTON ATOLL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XJM','is_in:country_codeA','jan_mayen'], // JAN MAYEN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XJN','is_in:country_codeA','juan_de_nova_island'], // JUAN DE NOVA ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XJV','is_in:country_codeA','jarvis_island'], // JARVIS ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XKM','is_in:country_codeA','entity_4'], // ENTITY 4 AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XKN','is_in:country_codeA','entity_5'], // ENTITY 5 AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XKR','is_in:country_codeA','kingman_reef'], // KINGMAN REEF AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XKS','is_in:country_codeA','kosovo'], // KOSOVO AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XMW','is_in:country_codeA','midway_islands'], // MIDWAY ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XNV','is_in:country_codeA','navassa_island'], // NAVASSA ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XPL','is_in:country_codeA','palmyra_atoll'], // PALMYRA ATOLL AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XPR','is_in:country_codeA','paracel_islands'], // PARACEL ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XQP','is_in:country_codeA','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XQZ','is_in:country_codeA','akrotiri'], // AKROTIRI AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XSP','is_in:country_codeA','spratly_islands'], // SPRATLY ISLANDS AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XSV','is_in:country_codeA','svalbard'], // SVALBARD AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XTR','is_in:country_codeA','tromelin_island'], // TROMELIN ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XWB','is_in:country_codeA','west_bank'], // WEST BANK AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XWK','is_in:country_codeA','wake_island'], // WAKE ISLAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XXD','is_in:country_codeA','dhekelia'], // DHEKELIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:XXX','is_in:country_codeA','no_mans_land'], // NO MAN"S LAND AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:YEM','is_in:country_codeA','yemen'], // YEMEN AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ZAF','is_in:country_codeA','south_africa'], // SOUTH AFRICA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ZMB','is_in:country_codeA','zambia'], // ZAMBIA AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:1-2:ZWE','is_in:country_codeA','zimbabwe'], // ZIMBABWE AS OF 2013-06-30
     ['ZI020_GE4A','ge:GENC:3:ed3:AX3','is_in:country_codeA','entity_6'], // ENTITY 6 AS OF 2015-06-30
     ['ZI020_GE4A','ge:ISO1:3:VI-15:AIA','is_in:country_codeA','anguilla'], // ANGUILLA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:ATA','is_in:country_codeA','antarctica'], // ANTARCTICA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:BIH','is_in:country_codeA','bosnia_and_herzegovina'], // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:BLZ','is_in:country_codeA','belize'], // BELIZE AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:BMU','is_in:country_codeA','bermuda'], // BERMUDA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:BRB','is_in:country_codeA','barbados'], // BARBADOS AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:BVT','is_in:country_codeA','bouvet_island'], // BOUVET ISLAND AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:GIB','is_in:country_codeA','gibraltar'], // GIBRALTAR AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:GRD','is_in:country_codeA','grenada'], // GRENADA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:GRL','is_in:country_codeA','greenland'], // GREENLAND AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:HUN','is_in:country_codeA','hungary'], // HUNGARY AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:IMN','is_in:country_codeA','isle_of_man'], // ISLE OF MAN AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:IRL','is_in:country_codeA','ireland'], // IRELAND AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:JAM','is_in:country_codeA','jamaica'], // JAMAICA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:JPN','is_in:country_codeA','japan'], // JAPAN AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:LBY','is_in:country_codeA','libya'], // LIBYA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:LCA','is_in:country_codeA','saint_lucia'], // SAINT LUCIA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:MNE','is_in:country_codeA','montenegro'], // MONTENEGRO AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:MSR','is_in:country_codeA','montserrat'], // MONTSERRAT AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:NIU','is_in:country_codeA','niue'], // NIUE AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:PNG','is_in:country_codeA','papua_new_guinea'], // PAPUA NEW GUINEA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:ROU','is_in:country_codeA','romania'], // ROMANIA AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:TKL','is_in:country_codeA','tokelau'], // TOKELAU AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:TKM','is_in:country_codeA','turkmenistan'], // TURKMENISTAN AS OF 2013-05-10
     ['ZI020_GE4A','ge:ISO1:3:VI-15:TUV','is_in:country_codeA','tuvalu'], // TUVALU AS OF 2013-05-10
     ['ZI020_GE4A','noInformation','is_in:country_codeA','no_information'], // No Information
     ['ZI020_GE4A','other','is_in:country_codeA','other'], // Other

     // ZI020_GE4B - Designation : GENC Short URN-based Identifier
     ['ZI020_GE4B','ge:GENC:3:1-2:ABW','is_in:country_codeB','aruba'], // ARUBA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AFG','is_in:country_codeB','afghanistan'], // AFGHANISTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AGO','is_in:country_codeB','angola'], // ANGOLA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ALB','is_in:country_codeB','albania'], // ALBANIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AND','is_in:country_codeB','andorra'], // ANDORRA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ARE','is_in:country_codeB','united_arab_emirates'], // UNITED ARAB EMIRATES AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ARG','is_in:country_codeB','argentina'], // ARGENTINA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ARM','is_in:country_codeB','armenia'], // ARMENIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ASM','is_in:country_codeB','american_samoa'], // AMERICAN SAMOA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ATF','is_in:country_codeB','french_southern_and_antarctic_lands'], // FRENCH SOUTHERN AND ANTARCTIC LANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ATG','is_in:country_codeB','antigua_and_barbuda'], // ANTIGUA AND BARBUDA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AUS','is_in:country_codeB','australia'], // AUSTRALIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AUT','is_in:country_codeB','austria'], // AUSTRIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AX1','is_in:country_codeB','unknown'], // UNKNOWN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AX2','is_in:country_codeB','guantanamo_bay_naval_base'], // GUANTANAMO BAY NAVAL BASE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:AZE','is_in:country_codeB','azerbaijan'], // AZERBAIJAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BDI','is_in:country_codeB','burundi'], // BURUNDI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BEL','is_in:country_codeB','belgium'], // BELGIUM AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BEN','is_in:country_codeB','benin'], // BENIN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BES','is_in:country_codeB','bonaire,_sint_eustatius,_and_saba'], // BONAIRE, SINT EUSTATIUS, AND SABA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BFA','is_in:country_codeB','burkina_faso'], // BURKINA FASO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BGD','is_in:country_codeB','bangladesh'], // BANGLADESH AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BGR','is_in:country_codeB','bulgaria'], // BULGARIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BHR','is_in:country_codeB','bahrain'], // BAHRAIN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BHS','is_in:country_codeB','bahamas,_the'], // BAHAMAS, THE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BLM','is_in:country_codeB','saint_barthelemy'], // SAINT BARTHELEMY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BLR','is_in:country_codeB','belarus'], // BELARUS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BOL','is_in:country_codeB','bolivia'], // BOLIVIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BRA','is_in:country_codeB','brazil'], // BRAZIL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BRN','is_in:country_codeB','brunei'], // BRUNEI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BTN','is_in:country_codeB','bhutan'], // BHUTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:BWA','is_in:country_codeB','botswana'], // BOTSWANA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CAF','is_in:country_codeB','central_african_republic'], // CENTRAL AFRICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CAN','is_in:country_codeB','canada'], // CANADA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CCK','is_in:country_codeB','cocos_(keeling)_islands'], // COCOS (KEELING) ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CHE','is_in:country_codeB','switzerland'], // SWITZERLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CHL','is_in:country_codeB','chile'], // CHILE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CHN','is_in:country_codeB','china'], // CHINA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CIV','is_in:country_codeB','cote_d"ivoire'], // COTE D"IVOIRE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CMR','is_in:country_codeB','cameroon'], // CAMEROON AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:COD','is_in:country_codeB','congo_(kinshasa)'], // CONGO (KINSHASA) AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:COG','is_in:country_codeB','congo_(brazzaville)'], // CONGO (BRAZZAVILLE) AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:COK','is_in:country_codeB','cook_islands'], // COOK ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:COL','is_in:country_codeB','colombia'], // COLOMBIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:COM','is_in:country_codeB','comoros'], // COMOROS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CPT','is_in:country_codeB','clipperton_island'], // CLIPPERTON ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CPV','is_in:country_codeB','cape_verde'], // CAPE VERDE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CRI','is_in:country_codeB','costa_rica'], // COSTA RICA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CUB','is_in:country_codeB','cuba'], // CUBA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CUW','is_in:country_codeB','curacao'], // CURACAO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CXR','is_in:country_codeB','christmas_island'], // CHRISTMAS ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CYM','is_in:country_codeB','cayman_islands'], // CAYMAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CYP','is_in:country_codeB','cyprus'], // CYPRUS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:CZE','is_in:country_codeB','czech_republic'], // CZECH REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DEU','is_in:country_codeB','germany'], // GERMANY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DGA','is_in:country_codeB','diego_garcia'], // DIEGO GARCIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DJI','is_in:country_codeB','djibouti'], // DJIBOUTI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DMA','is_in:country_codeB','dominica'], // DOMINICA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DNK','is_in:country_codeB','denmark'], // DENMARK AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DOM','is_in:country_codeB','dominican_republic'], // DOMINICAN REPUBLIC AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:DZA','is_in:country_codeB','algeria'], // ALGERIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ECU','is_in:country_codeB','ecuador'], // ECUADOR AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:EGY','is_in:country_codeB','egypt'], // EGYPT AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ERI','is_in:country_codeB','eritrea'], // ERITREA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ESH','is_in:country_codeB','western_sahara'], // WESTERN SAHARA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ESP','is_in:country_codeB','spain'], // SPAIN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:EST','is_in:country_codeB','estonia'], // ESTONIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ETH','is_in:country_codeB','ethiopia'], // ETHIOPIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:FIN','is_in:country_codeB','finland'], // FINLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:FJI','is_in:country_codeB','fiji'], // FIJI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:FLK','is_in:country_codeB','falkland_islands_(islas_malvinas)'], // FALKLAND ISLANDS (ISLAS MALVINAS) AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:FRA','is_in:country_codeB','france'], // FRANCE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:FRO','is_in:country_codeB','faroe_islands'], // FAROE ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:FSM','is_in:country_codeB','micronesia,_federated_states_of'], // MICRONESIA, FEDERATED STATES OF AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GAB','is_in:country_codeB','gabon'], // GABON AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GBR','is_in:country_codeB','united_kingdom'], // UNITED KINGDOM AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GEO','is_in:country_codeB','georgia'], // GEORGIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GGY','is_in:country_codeB','guernsey'], // GUERNSEY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GHA','is_in:country_codeB','ghana'], // GHANA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GIN','is_in:country_codeB','guinea'], // GUINEA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GLP','is_in:country_codeB','guadeloupe'], // GUADELOUPE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GMB','is_in:country_codeB','gambia,_the'], // GAMBIA, THE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GNB','is_in:country_codeB','guinea-bissau'], // GUINEA-BISSAU AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GNQ','is_in:country_codeB','equatorial_guinea'], // EQUATORIAL GUINEA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GRC','is_in:country_codeB','greece'], // GREECE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GTM','is_in:country_codeB','guatemala'], // GUATEMALA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GUF','is_in:country_codeB','french_guiana'], // FRENCH GUIANA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GUM','is_in:country_codeB','guam'], // GUAM AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:GUY','is_in:country_codeB','guyana'], // GUYANA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:HKG','is_in:country_codeB','hong_kong'], // HONG KONG AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:HMD','is_in:country_codeB','heard_island_and_mcdonald_islands'], // HEARD ISLAND AND MCDONALD ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:HND','is_in:country_codeB','honduras'], // HONDURAS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:HRV','is_in:country_codeB','croatia'], // CROATIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:HTI','is_in:country_codeB','haiti'], // HAITI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:IDN','is_in:country_codeB','indonesia'], // INDONESIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:IND','is_in:country_codeB','india'], // INDIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:IOT','is_in:country_codeB','british_indian_ocean_territory'], // BRITISH INDIAN OCEAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:IRN','is_in:country_codeB','iran'], // IRAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:IRQ','is_in:country_codeB','iraq'], // IRAQ AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ISL','is_in:country_codeB','iceland'], // ICELAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ISR','is_in:country_codeB','israel'], // ISRAEL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ITA','is_in:country_codeB','italy'], // ITALY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:JEY','is_in:country_codeB','jersey'], // JERSEY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:JOR','is_in:country_codeB','jordan'], // JORDAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KAZ','is_in:country_codeB','kazakhstan'], // KAZAKHSTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KEN','is_in:country_codeB','kenya'], // KENYA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KGZ','is_in:country_codeB','kyrgyzstan'], // KYRGYZSTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KHM','is_in:country_codeB','cambodia'], // CAMBODIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KIR','is_in:country_codeB','kiribati'], // KIRIBATI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KNA','is_in:country_codeB','saint_kitts_and_nevis'], // SAINT KITTS AND NEVIS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KOR','is_in:country_codeB','korea,_south'], // KOREA, SOUTH AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:KWT','is_in:country_codeB','kuwait'], // KUWAIT AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LAO','is_in:country_codeB','laos'], // LAOS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LBN','is_in:country_codeB','lebanon'], // LEBANON AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LBR','is_in:country_codeB','liberia'], // LIBERIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LIE','is_in:country_codeB','liechtenstein'], // LIECHTENSTEIN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LKA','is_in:country_codeB','sri_lanka'], // SRI LANKA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LSO','is_in:country_codeB','lesotho'], // LESOTHO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LTU','is_in:country_codeB','lithuania'], // LITHUANIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LUX','is_in:country_codeB','luxembourg'], // LUXEMBOURG AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:LVA','is_in:country_codeB','latvia'], // LATVIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MAC','is_in:country_codeB','macau'], // MACAU AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MAF','is_in:country_codeB','saint_martin'], // SAINT MARTIN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MAR','is_in:country_codeB','morocco'], // MOROCCO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MCO','is_in:country_codeB','monaco'], // MONACO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MDA','is_in:country_codeB','moldova'], // MOLDOVA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MDG','is_in:country_codeB','madagascar'], // MADAGASCAR AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MDV','is_in:country_codeB','maldives'], // MALDIVES AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MEX','is_in:country_codeB','mexico'], // MEXICO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MHL','is_in:country_codeB','marshall_islands'], // MARSHALL ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MKD','is_in:country_codeB','macedonia'], // MACEDONIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MLI','is_in:country_codeB','mali'], // MALI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MLT','is_in:country_codeB','malta'], // MALTA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MMR','is_in:country_codeB','burma'], // BURMA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MNG','is_in:country_codeB','mongolia'], // MONGOLIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MNP','is_in:country_codeB','northern_mariana_islands'], // NORTHERN MARIANA ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MOZ','is_in:country_codeB','mozambique'], // MOZAMBIQUE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MRT','is_in:country_codeB','mauritania'], // MAURITANIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MTQ','is_in:country_codeB','martinique'], // MARTINIQUE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MUS','is_in:country_codeB','mauritius'], // MAURITIUS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MWI','is_in:country_codeB','malawi'], // MALAWI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MYS','is_in:country_codeB','malaysia'], // MALAYSIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:MYT','is_in:country_codeB','mayotte'], // MAYOTTE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NAM','is_in:country_codeB','namibia'], // NAMIBIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NCL','is_in:country_codeB','new_caledonia'], // NEW CALEDONIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NER','is_in:country_codeB','niger'], // NIGER AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NFK','is_in:country_codeB','norfolk_island'], // NORFOLK ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NGA','is_in:country_codeB','nigeria'], // NIGERIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NIC','is_in:country_codeB','nicaragua'], // NICARAGUA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NLD','is_in:country_codeB','netherlands'], // NETHERLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NOR','is_in:country_codeB','norway'], // NORWAY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NPL','is_in:country_codeB','nepal'], // NEPAL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NRU','is_in:country_codeB','nauru'], // NAURU AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:NZL','is_in:country_codeB','new_zealand'], // NEW ZEALAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:OMN','is_in:country_codeB','oman'], // OMAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PAK','is_in:country_codeB','pakistan'], // PAKISTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PAN','is_in:country_codeB','panama'], // PANAMA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PCN','is_in:country_codeB','pitcairn_islands'], // PITCAIRN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PER','is_in:country_codeB','peru'], // PERU AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PHL','is_in:country_codeB','philippines'], // PHILIPPINES AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PLW','is_in:country_codeB','palau'], // PALAU AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:POL','is_in:country_codeB','poland'], // POLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PRI','is_in:country_codeB','puerto_rico'], // PUERTO RICO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PRK','is_in:country_codeB','korea,_north'], // KOREA, NORTH AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PRT','is_in:country_codeB','portugal'], // PORTUGAL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PRY','is_in:country_codeB','paraguay'], // PARAGUAY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PSE','is_in:country_codeB','palestinian_territory'], // PALESTINIAN TERRITORY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:PYF','is_in:country_codeB','french_polynesia'], // FRENCH POLYNESIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:QAT','is_in:country_codeB','qatar'], // QATAR AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:REU','is_in:country_codeB','reunion'], // REUNION AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:RUS','is_in:country_codeB','russia'], // RUSSIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:RWA','is_in:country_codeB','rwanda'], // RWANDA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SAU','is_in:country_codeB','saudi_arabia'], // SAUDI ARABIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SDN','is_in:country_codeB','sudan'], // SUDAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SEN','is_in:country_codeB','senegal'], // SENEGAL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SGP','is_in:country_codeB','singapore'], // SINGAPORE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SGS','is_in:country_codeB','south_georgia_and_south_sandwich_islands'], // SOUTH GEORGIA AND SOUTH SANDWICH ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SHN','is_in:country_codeB','saint_helena,_ascension,_and_tristan_da_cunha'], // SAINT HELENA, ASCENSION, AND TRISTAN DA CUNHA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SLB','is_in:country_codeB','solomon_islands'], // SOLOMON ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SLE','is_in:country_codeB','sierra_leone'], // SIERRA LEONE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SLV','is_in:country_codeB','el_salvador'], // EL SALVADOR AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SMR','is_in:country_codeB','san_marino'], // SAN MARINO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SOM','is_in:country_codeB','somalia'], // SOMALIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SPM','is_in:country_codeB','saint_pierre_and_miquelon'], // SAINT PIERRE AND MIQUELON AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SRB','is_in:country_codeB','serbia'], // SERBIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SSD','is_in:country_codeB','south_sudan'], // SOUTH SUDAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:STP','is_in:country_codeB','sao_tome_and_principe'], // SAO TOME AND PRINCIPE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SUR','is_in:country_codeB','suriname'], // SURINAME AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SVK','is_in:country_codeB','slovakia'], // SLOVAKIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SVN','is_in:country_codeB','slovenia'], // SLOVENIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SWE','is_in:country_codeB','sweden'], // SWEDEN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SWZ','is_in:country_codeB','swaziland'], // SWAZILAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SXM','is_in:country_codeB','sint_maarten'], // SINT MAARTEN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SYC','is_in:country_codeB','seychelles'], // SEYCHELLES AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:SYR','is_in:country_codeB','syria'], // SYRIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TCA','is_in:country_codeB','turks_and_caicos_islands'], // TURKS AND CAICOS ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TCD','is_in:country_codeB','chad'], // CHAD AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TGO','is_in:country_codeB','togo'], // TOGO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:THA','is_in:country_codeB','thailand'], // THAILAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TJK','is_in:country_codeB','tajikistan'], // TAJIKISTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TLS','is_in:country_codeB','timor-leste'], // TIMOR-LESTE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TON','is_in:country_codeB','tonga'], // TONGA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TTO','is_in:country_codeB','trinidad_and_tobago'], // TRINIDAD AND TOBAGO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TUN','is_in:country_codeB','tunisia'], // TUNISIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TUR','is_in:country_codeB','turkey'], // TURKEY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TWN','is_in:country_codeB','taiwan'], // TAIWAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:TZA','is_in:country_codeB','tanzania'], // TANZANIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:UGA','is_in:country_codeB','uganda'], // UGANDA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:UKR','is_in:country_codeB','ukraine'], // UKRAINE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:URY','is_in:country_codeB','uruguay'], // URUGUAY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:USA','is_in:country_codeB','united_states'], // UNITED STATES AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:UZB','is_in:country_codeB','uzbekistan'], // UZBEKISTAN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VAT','is_in:country_codeB','vatican_city'], // VATICAN CITY AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VCT','is_in:country_codeB','saint_vincent_and_the_grenadines'], // SAINT VINCENT AND THE GRENADINES AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VEN','is_in:country_codeB','venezuela'], // VENEZUELA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VGB','is_in:country_codeB','virgin_islands,_british'], // VIRGIN ISLANDS, BRITISH AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VIR','is_in:country_codeB','virgin_islands,_u.s.'], // VIRGIN ISLANDS, U.S. AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VNM','is_in:country_codeB','vietnam'], // VIETNAM AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:VUT','is_in:country_codeB','vanuatu'], // VANUATU AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:WLF','is_in:country_codeB','wallis_and_futuna'], // WALLIS AND FUTUNA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:WSM','is_in:country_codeB','samoa'], // SAMOA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XAC','is_in:country_codeB','ashmore_and_cartier_islands'], // ASHMORE AND CARTIER ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XAZ','is_in:country_codeB','entity_1'], // ENTITY 1 AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XBI','is_in:country_codeB','bassas_da_india'], // BASSAS DA INDIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XBK','is_in:country_codeB','baker_island'], // BAKER ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XCR','is_in:country_codeB','entity_2'], // ENTITY 2 AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XCS','is_in:country_codeB','coral_sea_islands'], // CORAL SEA ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XCY','is_in:country_codeB','entity_3'], // ENTITY 3 AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XEU','is_in:country_codeB','europa_island'], // EUROPA ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XGL','is_in:country_codeB','glorioso_islands'], // GLORIOSO ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XGZ','is_in:country_codeB','gaza_strip'], // GAZA STRIP AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XHO','is_in:country_codeB','howland_island'], // HOWLAND ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XJA','is_in:country_codeB','johnston_atoll'], // JOHNSTON ATOLL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XJM','is_in:country_codeB','jan_mayen'], // JAN MAYEN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XJN','is_in:country_codeB','juan_de_nova_island'], // JUAN DE NOVA ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XJV','is_in:country_codeB','jarvis_island'], // JARVIS ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XKM','is_in:country_codeB','entity_4'], // ENTITY 4 AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XKN','is_in:country_codeB','entity_5'], // ENTITY 5 AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XKR','is_in:country_codeB','kingman_reef'], // KINGMAN REEF AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XKS','is_in:country_codeB','kosovo'], // KOSOVO AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XMW','is_in:country_codeB','midway_islands'], // MIDWAY ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XNV','is_in:country_codeB','navassa_island'], // NAVASSA ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XPL','is_in:country_codeB','palmyra_atoll'], // PALMYRA ATOLL AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XPR','is_in:country_codeB','paracel_islands'], // PARACEL ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XQP','is_in:country_codeB','etorofu,_habomai,_kunashiri,_and_shikotan_islands'], // ETOROFU, HABOMAI, KUNASHIRI, AND SHIKOTAN ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XQZ','is_in:country_codeB','akrotiri'], // AKROTIRI AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XSP','is_in:country_codeB','spratly_islands'], // SPRATLY ISLANDS AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XSV','is_in:country_codeB','svalbard'], // SVALBARD AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XTR','is_in:country_codeB','tromelin_island'], // TROMELIN ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XWB','is_in:country_codeB','west_bank'], // WEST BANK AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XWK','is_in:country_codeB','wake_island'], // WAKE ISLAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XXD','is_in:country_codeB','dhekelia'], // DHEKELIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:XXX','is_in:country_codeB','no_mans_land'], // NO MAN"S LAND AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:YEM','is_in:country_codeB','yemen'], // YEMEN AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ZAF','is_in:country_codeB','south_africa'], // SOUTH AFRICA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ZMB','is_in:country_codeB','zambia'], // ZAMBIA AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:1-2:ZWE','is_in:country_codeB','zimbabwe'], // ZIMBABWE AS OF 2013-06-30
     ['ZI020_GE4B','ge:GENC:3:ed3:AX3','is_in:country_codeB','entity_6'], // ENTITY 6 AS OF 2015-06-30
     ['ZI020_GE4B','ge:ISO1:3:VI-15:AIA','is_in:country_codeB','anguilla'], // ANGUILLA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:ATA','is_in:country_codeB','antarctica'], // ANTARCTICA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:BIH','is_in:country_codeB','bosnia_and_herzegovina'], // BOSNIA AND HERZEGOVINA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:BLZ','is_in:country_codeB','belize'], // BELIZE AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:BMU','is_in:country_codeB','bermuda'], // BERMUDA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:BRB','is_in:country_codeB','barbados'], // BARBADOS AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:BVT','is_in:country_codeB','bouvet_island'], // BOUVET ISLAND AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:GIB','is_in:country_codeB','gibraltar'], // GIBRALTAR AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:GRD','is_in:country_codeB','grenada'], // GRENADA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:GRL','is_in:country_codeB','greenland'], // GREENLAND AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:HUN','is_in:country_codeB','hungary'], // HUNGARY AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:IMN','is_in:country_codeB','isle_of_man'], // ISLE OF MAN AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:IRL','is_in:country_codeB','ireland'], // IRELAND AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:JAM','is_in:country_codeB','jamaica'], // JAMAICA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:JPN','is_in:country_codeB','japan'], // JAPAN AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:LBY','is_in:country_codeB','libya'], // LIBYA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:LCA','is_in:country_codeB','saint_lucia'], // SAINT LUCIA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:MNE','is_in:country_codeB','montenegro'], // MONTENEGRO AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:MSR','is_in:country_codeB','montserrat'], // MONTSERRAT AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:NIU','is_in:country_codeB','niue'], // NIUE AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:PNG','is_in:country_codeB','papua_new_guinea'], // PAPUA NEW GUINEA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:ROU','is_in:country_codeB','romania'], // ROMANIA AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:TKL','is_in:country_codeB','tokelau'], // TOKELAU AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:TKM','is_in:country_codeB','turkmenistan'], // TURKMENISTAN AS OF 2013-05-10
     ['ZI020_GE4B','ge:ISO1:3:VI-15:TUV','is_in:country_codeB','tuvalu'], // TUVALU AS OF 2013-05-10
     ['ZI020_GE4B','noInformation','is_in:country_codeB','no_information'], // No Information
     ['ZI020_GE4B','other','is_in:country_codeB','other'], // Other

     // ZI024_DRL - Water : Device Relative Location
     // ['ZI024_DRL','-999999',undefined,undefined], // No Information
     ['ZI024_DRL','17','raw:ZI024_DRL','on_waterbody_bottom'], // On Waterbody Bottom
     ['ZI024_DRL','40','raw:ZI024_DRL','below_ground_surface'], // Below Ground Surface
     ['ZI024_DRL','44','raw:ZI024_DRL','on_surface'], // On Surface
     ['ZI024_DRL','45','raw:ZI024_DRL','above_surface'], // Above Surface
     ['ZI024_DRL','50','raw:ZI024_DRL','submerged'], // Submerged
     ['ZI024_DRL','999','raw:ZI024_DRL','other'], // Other

     // ZI024_DVPT - Water : Device Power Type
     // ['ZI024_DVPT','-999999',undefined,undefined], // No Information
     ['ZI024_DVPT','1','raw:ZI024_DVPT','electricity'], // Electricity
     ['ZI024_DVPT','2','raw:ZI024_DVPT','petroleum_product'], // Petroleum Product
     ['ZI024_DVPT','3','raw:ZI024_DVPT','manual'], // Manual
     ['ZI024_DVPT','4','raw:ZI024_DVPT','gravity'], // Gravity
     ['ZI024_DVPT','5','raw:ZI024_DVPT','compressed_air'], // Compressed Air
     ['ZI024_DVPT','6','raw:ZI024_DVPT','steam'], // Steam
     ['ZI024_DVPT','7','raw:ZI024_DVPT','solar'], // Solar
     ['ZI024_DVPT','999','raw:ZI024_DVPT','other'], // Other

     // ZI024_HYP - Water : Hydrologic Persistence
     // ['ZI024_HYP','-999999',undefined,undefined], // No Information
     ['ZI024_HYP','1','intermittent','no'], // Perennial
     ['ZI024_HYP','2','intermittent','yes'], // Intermittent
     ['ZI024_HYP','4','intermittent','dry'], // Dry

     // ZI024_PMP - Water : Pump Type
     // ['ZI024_PMP','-999999',undefined,undefined], // No Information
     ['ZI024_PMP','1','raw:ZI024_PMP','airlift'], // Airlift
     ['ZI024_PMP','2','raw:ZI024_PMP','centrifugal'], // Centrifugal
     ['ZI024_PMP','3','raw:ZI024_PMP','jet'], // Jet
     ['ZI024_PMP','4','raw:ZI024_PMP','reciprocating'], // Reciprocating
     ['ZI024_PMP','5','raw:ZI024_PMP','turbine'], // Turbine
     ['ZI024_PMP','6','raw:ZI024_PMP','gravity'], // Gravity
     ['ZI024_PMP','7','raw:ZI024_PMP','vacuum'], // Vacuum
     ['ZI024_PMP','8','raw:ZI024_PMP','rotary'], // Rotary
     ['ZI024_PMP','999','raw:ZI024_PMP','other'], // Other

     // ZI024_PUR - Water : Purification Process
     // ['ZI024_PUR','-999999',undefined,undefined], // No Information
     ['ZI024_PUR','1','raw:ZI024_PUR','desalination'], // Desalination
     ['ZI024_PUR','2','raw:ZI024_PUR','filtration'], // Filtration
     ['ZI024_PUR','3','raw:ZI024_PUR','sedimentation/coagulation'], // Sedimentation/coagulation
     ['ZI024_PUR','4','raw:ZI024_PUR','reverse_osmosis'], // Reverse Osmosis
     ['ZI024_PUR','5','raw:ZI024_PUR','electrolysis'], // Electrolysis
     ['ZI024_PUR','6','raw:ZI024_PUR','disinfection'], // Disinfection
     ['ZI024_PUR','7','raw:ZI024_PUR','blending'], // Blending
     ['ZI024_PUR','8','raw:ZI024_PUR','freezing'], // Freezing
     ['ZI024_PUR','9','raw:ZI024_PUR','multi-stage_flash'], // Multi-stage flash
     ['ZI024_PUR','10','raw:ZI024_PUR','multi-effect_evaporation'], // Multi-effect evaporation
     ['ZI024_PUR','11','raw:ZI024_PUR','vapor_compression'], // Vapor Compression
     ['ZI024_PUR','999','raw:ZI024_PUR','other'], // Other

     // ZI024_PUR2 - Water : Purification Process [2]
     // ['ZI024_PUR2','-999999',undefined,undefined], // No Information
     ['ZI024_PUR2','1','raw:ZI024_PUR2','desalination'], // Desalination
     ['ZI024_PUR2','2','raw:ZI024_PUR2','filtration'], // Filtration
     ['ZI024_PUR2','3','raw:ZI024_PUR2','sedimentation/coagulation'], // Sedimentation/coagulation
     ['ZI024_PUR2','4','raw:ZI024_PUR2','reverse_osmosis'], // Reverse Osmosis
     ['ZI024_PUR2','5','raw:ZI024_PUR2','electrolysis'], // Electrolysis
     ['ZI024_PUR2','6','raw:ZI024_PUR2','disinfection'], // Disinfection
     ['ZI024_PUR2','7','raw:ZI024_PUR2','blending'], // Blending
     ['ZI024_PUR2','8','raw:ZI024_PUR2','freezing'], // Freezing
     ['ZI024_PUR2','9','raw:ZI024_PUR2','multi-stage_flash'], // Multi-stage flash
     ['ZI024_PUR2','10','raw:ZI024_PUR2','multi-effect_evaporation'], // Multi-effect evaporation
     ['ZI024_PUR2','11','raw:ZI024_PUR2','vapor_compression'], // Vapor Compression
     ['ZI024_PUR2','999','raw:ZI024_PUR2','other'], // Other

     // ZI024_PUR3 - Water : Purification Process [3]
     // ['ZI024_PUR3','-999999',undefined,undefined], // No Information
     ['ZI024_PUR3','1','raw:ZI024_PUR3','desalination'], // Desalination
     ['ZI024_PUR3','2','raw:ZI024_PUR3','filtration'], // Filtration
     ['ZI024_PUR3','3','raw:ZI024_PUR3','sedimentation/coagulation'], // Sedimentation/coagulation
     ['ZI024_PUR3','4','raw:ZI024_PUR3','reverse_osmosis'], // Reverse Osmosis
     ['ZI024_PUR3','5','raw:ZI024_PUR3','electrolysis'], // Electrolysis
     ['ZI024_PUR3','6','raw:ZI024_PUR3','disinfection'], // Disinfection
     ['ZI024_PUR3','7','raw:ZI024_PUR3','blending'], // Blending
     ['ZI024_PUR3','8','raw:ZI024_PUR3','freezing'], // Freezing
     ['ZI024_PUR3','9','raw:ZI024_PUR3','multi-stage_flash'], // Multi-stage flash
     ['ZI024_PUR3','10','raw:ZI024_PUR3','multi-effect_evaporation'], // Multi-effect evaporation
     ['ZI024_PUR3','11','raw:ZI024_PUR3','vapor_compression'], // Vapor Compression
     ['ZI024_PUR3','999','raw:ZI024_PUR3','other'], // Other

     // ZI024_PUR4 - Water : Purification Process [4]
     // ['ZI024_PUR4','-999999',undefined,undefined], // No Information
     ['ZI024_PUR4','1','raw:ZI024_PUR4','desalination'], // Desalination
     ['ZI024_PUR4','2','raw:ZI024_PUR4','filtration'], // Filtration
     ['ZI024_PUR4','3','raw:ZI024_PUR4','sedimentation/coagulation'], // Sedimentation/coagulation
     ['ZI024_PUR4','4','raw:ZI024_PUR4','reverse_osmosis'], // Reverse Osmosis
     ['ZI024_PUR4','5','raw:ZI024_PUR4','electrolysis'], // Electrolysis
     ['ZI024_PUR4','6','raw:ZI024_PUR4','disinfection'], // Disinfection
     ['ZI024_PUR4','7','raw:ZI024_PUR4','blending'], // Blending
     ['ZI024_PUR4','8','raw:ZI024_PUR4','freezing'], // Freezing
     ['ZI024_PUR4','9','raw:ZI024_PUR4','multi-stage_flash'], // Multi-stage flash
     ['ZI024_PUR4','10','raw:ZI024_PUR4','multi-effect_evaporation'], // Multi-effect evaporation
     ['ZI024_PUR4','11','raw:ZI024_PUR4','vapor_compression'], // Vapor Compression
     ['ZI024_PUR4','999','raw:ZI024_PUR4','other'], // Other

     // ZI024_PUR5 - Water : Purification Process [5]
     // ['ZI024_PUR5','-999999',undefined,undefined], // No Information
     ['ZI024_PUR5','1','raw:ZI024_PUR5','desalination'], // Desalination
     ['ZI024_PUR5','2','raw:ZI024_PUR5','filtration'], // Filtration
     ['ZI024_PUR5','3','raw:ZI024_PUR5','sedimentation/coagulation'], // Sedimentation/coagulation
     ['ZI024_PUR5','4','raw:ZI024_PUR5','reverse_osmosis'], // Reverse Osmosis
     ['ZI024_PUR5','5','raw:ZI024_PUR5','electrolysis'], // Electrolysis
     ['ZI024_PUR5','6','raw:ZI024_PUR5','disinfection'], // Disinfection
     ['ZI024_PUR5','7','raw:ZI024_PUR5','blending'], // Blending
     ['ZI024_PUR5','8','raw:ZI024_PUR5','freezing'], // Freezing
     ['ZI024_PUR5','9','raw:ZI024_PUR5','multi-stage_flash'], // Multi-stage flash
     ['ZI024_PUR5','10','raw:ZI024_PUR5','multi-effect_evaporation'], // Multi-effect evaporation
     ['ZI024_PUR5','11','raw:ZI024_PUR5','vapor_compression'], // Vapor Compression
     ['ZI024_PUR5','999','raw:ZI024_PUR5','other'], // Other

     // ZI024_RTP - Water : Reservoir Type
     // ['ZI024_RTP','-999999',undefined,undefined], // No Information
     ['ZI024_RTP','1','raw:ZI024_RTP','constructed_basin'], // Constructed Basin
     ['ZI024_RTP','2','raw:ZI024_RTP','back-up_water_impounded_by_a_dam'], // Back-up Water Impounded by a Dam
     ['ZI024_RTP','999','raw:ZI024_RTP','other'], // Other

     // ZI024_SCC - Water : Water Type
     // ['ZI024_SCC','-999999',undefined,undefined], // No Information
     ['ZI024_SCC','1','water:type','alkaline'], // Alkaline
     ['ZI024_SCC','4','water:type','mineral'], // Mineral
     ['ZI024_SCC','10','water:type','saline'], // Saline
     ['ZI024_SCC','11','water:type','fresh'], // Fresh
     ['ZI024_SCC','12','water:type','brackish'], // Brackish
     ['ZI024_SCC','13','water:type','seawater'], // Seawater
     ['ZI024_SCC','14','water:type','brine'], // Brine
     // ['ZI024_SCC','998','water:type','not_applicable'], // Not Applicable
     ['ZI024_SCC','998',undefined,undefined], // Not Applicable

     // ZI024_SQN - Water : Water Flow Rate Category
     // ['ZI024_SQN','-999999',undefined,undefined], // No Information
     ['ZI024_SQN','1','raw:ZI024_SQN','small'], // Small
     ['ZI024_SQN','2','raw:ZI024_SQN','moderate'], // Moderate
     ['ZI024_SQN','3','raw:ZI024_SQN','large'], // Large
     ['ZI024_SQN','999','raw:ZI024_SQN','other'], // Other

     // ZI024_WPAC - Water : Water Pump Accessibility
     // ['ZI024_WPAC','-999999',undefined,undefined], // No Information
     ['ZI024_WPAC','100','raw:ZI024_WPAC','from_left'], // From Left
     ['ZI024_WPAC','101','raw:ZI024_WPAC','from_right'], // From Right
     ['ZI024_WPAC','102','raw:ZI024_WPAC','from_all_sides'], // From All Sides
     ['ZI024_WPAC','103','raw:ZI024_WPAC','no_accessibility'], // No Accessibility
     ['ZI024_WPAC','104','raw:ZI024_WPAC','from_north'], // From North
     ['ZI024_WPAC','105','raw:ZI024_WPAC','from_south'], // From South
     ['ZI024_WPAC','106','raw:ZI024_WPAC','from_east'], // From East
     ['ZI024_WPAC','107','raw:ZI024_WPAC','from_west'], // From West

     // ZI024_WSG - Water : Water Stage
     // ['ZI024_WSG','-999999',undefined,undefined], // No Information
     ['ZI024_WSG','1','raw:ZI024_WSG','low'], // Low
     ['ZI024_WSG','2','raw:ZI024_WSG','mean'], // Mean
     ['ZI024_WSG','3','raw:ZI024_WSG','high'], // High
     ['ZI024_WSG','999','raw:ZI024_WSG','other'], // Other

     // ZI024_WUR - Water : Water Use
     // ['ZI024_WUR','-999999',undefined,undefined], // No Information
     ['ZI024_WUR','1','raw:ZI024_WUR','agricultural_irrigation'], // Agricultural Irrigation
     ['ZI024_WUR','2','raw:ZI024_WUR','commercial'], // Commercial
     ['ZI024_WUR','3','raw:ZI024_WUR','domestic_irrigation'], // Domestic Irrigation
     ['ZI024_WUR','4','raw:ZI024_WUR','industrial'], // Industrial
     ['ZI024_WUR','5','raw:ZI024_WUR','institutional'], // Institutional
     ['ZI024_WUR','6','raw:ZI024_WUR','livestock'], // Livestock
     ['ZI024_WUR','7','raw:ZI024_WUR','municipal'], // Municipal
     ['ZI024_WUR','8','raw:ZI024_WUR','power_generation'], // Power Generation
     ['ZI024_WUR','9','raw:ZI024_WUR','recreational'], // Recreational
     ['ZI024_WUR','10','raw:ZI024_WUR','sanitary_domestic'], // Sanitary Domestic
     ['ZI024_WUR','11','raw:ZI024_WUR','military'], // Military
     ['ZI024_WUR','999','raw:ZI024_WUR','other'], // Other

     // ZI024_WUR2 - Water : Water Use [2]
     // ['ZI024_WUR2','-999999',undefined,undefined], // No Information
     ['ZI024_WUR2','1','raw:ZI024_WUR2','agricultural_irrigation'], // Agricultural Irrigation
     ['ZI024_WUR2','2','raw:ZI024_WUR2','commercial'], // Commercial
     ['ZI024_WUR2','3','raw:ZI024_WUR2','domestic_irrigation'], // Domestic Irrigation
     ['ZI024_WUR2','4','raw:ZI024_WUR2','industrial'], // Industrial
     ['ZI024_WUR2','5','raw:ZI024_WUR2','institutional'], // Institutional
     ['ZI024_WUR2','6','raw:ZI024_WUR2','livestock'], // Livestock
     ['ZI024_WUR2','7','raw:ZI024_WUR2','municipal'], // Municipal
     ['ZI024_WUR2','8','raw:ZI024_WUR2','power_generation'], // Power Generation
     ['ZI024_WUR2','9','raw:ZI024_WUR2','recreational'], // Recreational
     ['ZI024_WUR2','10','raw:ZI024_WUR2','sanitary_domestic'], // Sanitary Domestic
     ['ZI024_WUR2','11','raw:ZI024_WUR2','military'], // Military
     ['ZI024_WUR2','999','raw:ZI024_WUR2','other'], // Other

     // ZI024_WUR3 - Water : Water Use [3]
     // ['ZI024_WUR3','-999999',undefined,undefined], // No Information
     ['ZI024_WUR3','1','raw:ZI024_WUR3','agricultural_irrigation'], // Agricultural Irrigation
     ['ZI024_WUR3','2','raw:ZI024_WUR3','commercial'], // Commercial
     ['ZI024_WUR3','3','raw:ZI024_WUR3','domestic_irrigation'], // Domestic Irrigation
     ['ZI024_WUR3','4','raw:ZI024_WUR3','industrial'], // Industrial
     ['ZI024_WUR3','5','raw:ZI024_WUR3','institutional'], // Institutional
     ['ZI024_WUR3','6','raw:ZI024_WUR3','livestock'], // Livestock
     ['ZI024_WUR3','7','raw:ZI024_WUR3','municipal'], // Municipal
     ['ZI024_WUR3','8','raw:ZI024_WUR3','power_generation'], // Power Generation
     ['ZI024_WUR3','9','raw:ZI024_WUR3','recreational'], // Recreational
     ['ZI024_WUR3','10','raw:ZI024_WUR3','sanitary_domestic'], // Sanitary Domestic
     ['ZI024_WUR3','11','raw:ZI024_WUR3','military'], // Military
     ['ZI024_WUR3','999','raw:ZI024_WUR3','other'], // Other

     // ZI024_WUR4 - Water : Water Use [4]
     // ['ZI024_WUR4','-999999',undefined,undefined], // No Information
     ['ZI024_WUR4','1','raw:ZI024_WUR4','agricultural_irrigation'], // Agricultural Irrigation
     ['ZI024_WUR4','2','raw:ZI024_WUR4','commercial'], // Commercial
     ['ZI024_WUR4','3','raw:ZI024_WUR4','domestic_irrigation'], // Domestic Irrigation
     ['ZI024_WUR4','4','raw:ZI024_WUR4','industrial'], // Industrial
     ['ZI024_WUR4','5','raw:ZI024_WUR4','institutional'], // Institutional
     ['ZI024_WUR4','6','raw:ZI024_WUR4','livestock'], // Livestock
     ['ZI024_WUR4','7','raw:ZI024_WUR4','municipal'], // Municipal
     ['ZI024_WUR4','8','raw:ZI024_WUR4','power_generation'], // Power Generation
     ['ZI024_WUR4','9','raw:ZI024_WUR4','recreational'], // Recreational
     ['ZI024_WUR4','10','raw:ZI024_WUR4','sanitary_domestic'], // Sanitary Domestic
     ['ZI024_WUR4','11','raw:ZI024_WUR4','military'], // Military
     ['ZI024_WUR4','999','raw:ZI024_WUR4','other'], // Other

     // ZI024_YWQ - Water : Water Potability
     // ['ZI024_YWQ','-999999',undefined,undefined], // No Information
     ['ZI024_YWQ','1','water:potable','yes'], // Potable
     ['ZI024_YWQ','2','water:potable','treatable'], // Treatable
     ['ZI024_YWQ','3','water:potable','contaminated'], // Contaminated
     ['ZI024_YWQ','4','water:potable','no'], // Nonpotable
     ['ZI024_YWQ','999','water:potable','other'], // Other

     // ZI025_MAN - Vert Pos : Maritime Navigation Marked
     // ['ZI025_MAN','-999999',undefined,undefined], // No Information
     ['ZI025_MAN','1000','raw:ZI025_MAN','no'],
     ['ZI025_MAN','1001','raw:ZI025_MAN','yes'],

     // ZI025_UHS - Vert Pos : Uncovering Height Known
     // ['ZI025_UHS','-999999',undefined,undefined], // No Information
     ['ZI025_UHS','1000','raw:ZI025_UHS','no'],
     ['ZI025_UHS','1001','raw:ZI025_UHS','yes'],

     // ZI025_WLE - Vert Pos : Water Level Effect
     // ['ZI025_WLE','-999999',undefined,undefined], // No Information
     ['ZI025_WLE','1','raw:ZI025_WLE','partly_submerged'], // Partly Submerged
     ['ZI025_WLE','2','raw:ZI025_WLE','always_dry'], // Always Dry
     ['ZI025_WLE','3','raw:ZI025_WLE','always_submerged'], // Always Submerged
     ['ZI025_WLE','4','raw:ZI025_WLE','covers_and_uncovers'], // Covers and Uncovers
     ['ZI025_WLE','5','raw:ZI025_WLE','awash_at_low_water'], // Awash at Low Water
     ['ZI025_WLE','9','raw:ZI025_WLE','awash_at_chart_datum'], // Awash at Chart Datum
     ['ZI025_WLE','999','raw:ZI025_WLE','other'], // Other

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

     // ZI032_GUG - Pylon Information : Guyed
     // ['ZI032_GUG','-999999',undefined,undefined], // No Information
     ['ZI032_GUG','1000','raw:ZI032_GUG','no'],
     ['ZI032_GUG','1001','raw:ZI032_GUG','yes'],

     // ZI032_PYC - Pylon Information : Pylon Configuration
     // ['ZI032_PYC','-999999',undefined,undefined], // No Information
     ['ZI032_PYC','1','raw:ZI032_PYC','a"'], // A"
     ['ZI032_PYC','2','raw:ZI032_PYC','h"'], // H"
     ['ZI032_PYC','3','raw:ZI032_PYC','i"'], // I"
     ['ZI032_PYC','4','raw:ZI032_PYC','y"'], // Y"
     ['ZI032_PYC','5','raw:ZI032_PYC','t"'], // T"
     ['ZI032_PYC','999','raw:ZI032_PYC','other'], // Other

     // ZI032_PYM - Pylon Information : Pylon Material
     // ['ZI032_PYM','-999999',undefined,undefined], // No Information
     ['ZI032_PYM','1','raw:ZI032_PYM','aluminium'], // Aluminium
     ['ZI032_PYM','3','raw:ZI032_PYM','concrete'], // Concrete
     ['ZI032_PYM','4','raw:ZI032_PYM','masonry'], // Masonry
     ['ZI032_PYM','5','raw:ZI032_PYM','metal'], // Metal
     ['ZI032_PYM','7','raw:ZI032_PYM','wood'], // Wood
     ['ZI032_PYM','8','raw:ZI032_PYM','steel'], // Steel
     ['ZI032_PYM','9','raw:ZI032_PYM','fibreglass'], // Fibreglass
     ['ZI032_PYM','10','raw:ZI032_PYM','iron'], // Iron
     ['ZI032_PYM','999','raw:ZI032_PYM','other'], // Other

     // ZI032_TOS - Pylon Information : Tower Shape
     // ['ZI032_TOS','-999999',undefined,undefined], // No Information
     ['ZI032_TOS','6','raw:ZI032_TOS','mast'], // Mast
     ['ZI032_TOS','8','raw:ZI032_TOS','pole'], // Pole
     ['ZI032_TOS','11','raw:ZI032_TOS','tripod'], // Tripod
     ['ZI032_TOS','12','raw:ZI032_TOS','truss'], // Truss
     ['ZI032_TOS','13','raw:ZI032_TOS','tubular'], // Tubular
     ['ZI032_TOS','999','raw:ZI032_TOS','other'], // Other

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

     // ZI071_FFN - Cave Information : Feature Function
     // ['ZI071_FFN','-999999',undefined,undefined], // No Information
     ['ZI071_FFN','530','raw:ZI071_FFN','warehousing_and_storage'], // Warehousing and Storage
     ['ZI071_FFN','563','raw:ZI071_FFN','residence'], // Residence
     ['ZI071_FFN','835','raw:ZI071_FFN','defence_activities'], // Defence Activities
     ['ZI071_FFN','930','raw:ZI071_FFN','religious_activities'], // Religious Activities
     ['ZI071_FFN','999','raw:ZI071_FFN','other'], // Other

     // ZI071_FFN2 - Cave Information : Feature Function [2]
     // ['ZI071_FFN2','-999999',undefined,undefined], // No Information
     ['ZI071_FFN2','530','raw:ZI071_FFN2','warehousing_and_storage'], // Warehousing and Storage
     ['ZI071_FFN2','563','raw:ZI071_FFN2','residence'], // Residence
     ['ZI071_FFN2','835','raw:ZI071_FFN2','defence_activities'], // Defence Activities
     ['ZI071_FFN2','930','raw:ZI071_FFN2','religious_activities'], // Religious Activities
     ['ZI071_FFN2','999','raw:ZI071_FFN2','other'], // Other

     // ZI071_FFN3 - Cave Information : Feature Function [3]
     // ['ZI071_FFN3','-999999',undefined,undefined], // No Information
     ['ZI071_FFN3','530','raw:ZI071_FFN3','warehousing_and_storage'], // Warehousing and Storage
     ['ZI071_FFN3','563','raw:ZI071_FFN3','residence'], // Residence
     ['ZI071_FFN3','835','raw:ZI071_FFN3','defence_activities'], // Defence Activities
     ['ZI071_FFN3','930','raw:ZI071_FFN3','religious_activities'], // Religious Activities
     ['ZI071_FFN3','999','raw:ZI071_FFN3','other'], // Other

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

     // ZVH_VDT - Highest Elevation <vertical datum>
     // ['ZVH_VDT','-999999',undefined,undefined], // No Information
     ['ZVH_VDT','1','raw:ZVH_VDT','wgs_84_ellipsoid'], // WGS 84 Ellipsoid
     ['ZVH_VDT','2','raw:ZVH_VDT','wgs_84_egm96_geoid'], // WGS 84 EGM96 Geoid
     ['ZVH_VDT','3','raw:ZVH_VDT','mean_sea_level_(msl)'], // Mean Sea Level (MSL)
     ['ZVH_VDT','4','raw:ZVH_VDT','north_american_vertical_datum_(navd)_1988'], // North American Vertical Datum (NAVD) 1988
     ['ZVH_VDT','5','raw:ZVH_VDT','national_geodetic_vertical_datum_(ngvd)_1929'], // National Geodetic Vertical Datum (NGVD) 1929
     ['ZVH_VDT','6','raw:ZVH_VDT','ground_level'], // Ground Level
     ['ZVH_VDT','7','raw:ZVH_VDT','wgs_84_egm08_geoid'], // WGS 84 EGM08 Geoid
     ['ZVH_VDT','998','raw:ZVH_VDT','not_applicable'], // Not Applicable
     ['ZVH_VDT','999','raw:ZVH_VDT','other'], // Other

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
    // ##### End of intList#####


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

} // End of ggdm30.rules
