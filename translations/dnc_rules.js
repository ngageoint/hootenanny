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
    DNC One2one rules
*/

dnc.rules = {
    // ##### Start of One2One Rules #####

    // FCODE rules for Import
    // This is where anything "unique" to DNC goes.
    fcodeOne2oneIn : [
        ['F_CODE','AA010','landuse','surface_mining'], // Extraction Mine
        ['F_CODE','AB000','landuse','landfill'], // Disposal Site / Waste Pile
        ['F_CODE','AB010','amenity','recycling'], // Wrecking Yard/Scrap Yard, Recycling Site
        ['F_CODE','AC000','facility','yes'], // Processing Plant - Superseded by AL010
        ['F_CODE','AC030','water','settling_basin'], // Settling Basin/Sludge Pond, Settling Pond
        ['F_CODE','AD010','power','plant'], // Electric Power Station, Power Plant
        ['F_CODE','AD030','power','substation'], // Power Substation, Substation/Transformer Yard
        ['F_CODE','AF010','man_made','chimney'], // Chimney/Smokestack
        ['F_CODE','AF030','tower:type','cooling'], // Cooling Tower
        ['F_CODE','AF040','man_made','crane'], // Crane
        ['F_CODE','AF070','man_made','flare'], // Flare Pipe
        ['F_CODE','AH010','bastion','yes'], // Bastion/Rampart - not in NAS
        ['F_CODE','AH050','site:type','fortification'], // Fortification - not in NAS
        ['F_CODE','AJ050','man_made','windmill'], // Windmill
        ['F_CODE','AK020','tourism','theme_park_attraction'], // Amusement Park Attraction
        ['F_CODE','AK040','leisure','pitch'], // Athletic Field, Sports Ground
        ['F_CODE','AK080','man_made','outdoor_theatre_screen'], // Outdoor Theatre Screen
        ['F_CODE','AK090','leisure','fairground'], // Fairground
        ['F_CODE','AK100','leisure','golf_course'], // Golf Course
        ['F_CODE','AK120','leisure','park'], // Park
        ['F_CODE','AK130','leisure','track'], // Racetrack
        ['F_CODE','AK160','leisure','stadium'], // Stadium/Amphitheater
        ['F_CODE','AK180','tourism','zoo'], // Zoo/Safari Park
        ['F_CODE','AL005','boundary','protected_area'], // Animal Sanctuary - not in NAS
        ['F_CODE','AL015','building','yes'], // Building - NFDD AL013
        ['F_CODE','AL018','building:superstructure','yes'], // Building Superstructure Addition
        ['F_CODE','AL020','landuse','built_up_area'], // Built-up Area
        ['F_CODE','AL025','man_made','cairn'], // Cairn
        ['F_CODE','AL030','landuse','cemetery'], // Cemetery
        ['F_CODE','AL050','display_sign','yes'], // Display Sign
        ['F_CODE','AL070','barrier','fence'], // Fence
        ['F_CODE','AL073','man_made','flagpole'], // Flagpole
        ['F_CODE','AL130','historic','monument'], // Memorial Monument
        ['F_CODE','AL200','historic','ruins'], // Ruins
        ['F_CODE','AL240','man_made','tower'], // Tower (Non-Communication) - NFDD AL241
        ['F_CODE','AL260','barrier','wall'], // Wall
        ['F_CODE','AM010','landuse','depot'], // Storage Depot
        ['F_CODE','AM020','man_made','silo'], // Grain Bin/Silo, Grain Storage Structure
        ['F_CODE','AM030','man_made','grain_elevator'], // Grain Elevator
        ['F_CODE','AM040','landuse','mineral_pile'], // Mineral Pile
        ['F_CODE','AM070','man_made','storage_tank'], // Storage Tank
        ['F_CODE','AM080','man_made','water_tower'], // Water Tower
        ['F_CODE','AN010','railway','rail'], // Railway
        ['F_CODE','AN060','service','yard'], // Railway Yard
        ['F_CODE','AP020','junction','yes'], // Road Interchange - See RIT
        ['F_CODE','AP030','highway','road'], // Road - See Custom rules
        ['F_CODE','AQ010','cableway','yes'], // Aerial Cableway / Ski Lift Lines - Superseded by AT041
        ['F_CODE','AQ020','aerialway','pylon'], // Aerial Cableway Pylon/Ski Pylon
        ['F_CODE','AQ040','bridge','yes'], // Bridge
        ['F_CODE','AQ045','bridge:span','yes'], // Bridge Span
        ['F_CODE','AQ060','control_tower','yes'], // Control Tower
        ['F_CODE','AQ070','route','ferry'], // Ferry Crossing
        ['F_CODE','AQ080','amenity','ferry_terminal'], // Ferry Station, Ferry Site
        ['F_CODE','AQ110','seamark:type','mooring'], // Mooring Mast
        ['F_CODE','AQ113','man_made','pipeline'], // Pipeline
        ['F_CODE','AQ116','man_made','pumping_station'], // Pumping Station
        ['F_CODE','AQ130','tunnel','yes'], // Tunnel
        ['F_CODE','AT005','cable','yes'], // Cable
        ['F_CODE','AT010','landmark','dish_aerial'], // Dish Aerial
        ['F_CODE','AT020','radar:use','early_warning'], // Early Warning Radar Site - not in NAS
        ['F_CODE','AT030','power','line'], // Power Line - Superseded by AT005
        ['F_CODE','AT040','pylon','yes'], // Power Transmission Pylon
        ['F_CODE','AT045','man_made','radar_station'], // Radar Station, Radar Transmitter
        ['F_CODE','AT050','use','communication'], // Communication Building
        ['F_CODE','AT060','cable:type','communication'], // Communication Line - NFDD AT005
        ['F_CODE','AT080','tower:type','communication'], // Communications Tower - NFDD AL241
        ['F_CODE','BA010','natural','coastline'], // Coastline/Shoreline, Land Water Boundary
        ['F_CODE','BA020','foreshore','yes'], // Foreshore Region - NFDD BA023
        ['F_CODE','BA030','place','island'], // Island
        ['F_CODE','BA040','water','tidal'], // Tidal Water
        ['F_CODE','BB010','seamark:type','anchorage'], // GGDM3/NAS7 Anchorage
        ['F_CODE','BB020','seamark:type','berth'], // GGDM3/NAS7 BERTH
        ['F_CODE','BB030','barrier','bollard'], // Bollard
        ['F_CODE','BB040','man_made','breakwater'], // Breakwater/Groyne
        ['F_CODE','BB050','seamark:type','calling-in_point'], // GGDM3/NAS7 CALLING_IN_POINT
        ['F_CODE','BB080','seamark:mooring:category','dolphin'], // GGDM3/NAS7 DOLPHIN
        ['F_CODE','BB090','dock','drydock'], // Dry Dock
        ['F_CODE','BB100','seamark:fishing_facility:category','stake'], // GGDM3/NAS7 Fishing Stakes
        ['F_CODE','BB105','seamark:harbour:category','fishing'], // Fishing Harbor
        ['F_CODE','BB110','man_made','fish_traps'], // Fish Traps/Fish Weirs
        ['F_CODE','BB115','seamark:type','gridiron'], // GGDM3/NAS7 NAUTICAL_GRIDIRON
        ['F_CODE','BB140','man_made','pier'], // Jetty
        ['F_CODE','BB150','beach_landing_site','yes'], // GGDM3/NAS7 BEACH_LANDING_SITE
        ['F_CODE','BB160','mooring','ring'], // Mooring Ring
        ['F_CODE','BB170','seamark:platform:category','loading'], // Offshore Loading Facility
        ['F_CODE','BB180','produce','oysters'], // Oyster Bed/Mussel Bed
        ['F_CODE','BB190','waterway','dock'], // Pier /Wharf /Quay
        ['F_CODE','BB220','seamark:shoreline_construction:category','ramp'], // Ramp (Maritime) - How is this different from BB240??
        ['F_CODE','BB230','wall','seawall'], // Seawall - NFDD BB081
        ['F_CODE','BB240','leisure','slipway'], // Slipway/Patent Slip  - NFDD BB082
        ['F_CODE','BC010','seamark:type','beacon'], // GGDM3/NAS7 Beacon
        ['F_CODE','BC020','seamark:buoy','yes'], // GGDM3/NAS7 BUOY
        ['F_CODE','BC040','seamark:light','yes'], // GGDM3/NAS7 MARITIME_NAVIGATION_LIGHT
        ['F_CODE','BC055','seamark:type','navigation_marker'], // GGDM3/NAS7 MARITIME_NAVIGATION_MARKER
        ['F_CODE','BC060','seamark:type','light_sector'], // GGDM3/NAS7 LIGHT_SECTOR
        ['F_CODE','BC070','seamark:type','light_vessel'], // Light Vessel
        ['F_CODE','BC080','seamark:type','insubstatial_navigation_marker'], // GGDM3/NAS7 INSUBSTANTIAL_NAV_MARK
        ['F_CODE','BC100','leading_line','yes'], // Leading Line
        ['F_CODE','BD000','underwater_hazard','yes'], // Underwater Danger / Underwater Hazard - Not in NAS
        ['F_CODE','BD010','seamark:water_turbulence:category','breakers'], // Breakers - not in NAS
        ['F_CODE','BD020','seamark:obstruction:category','crib'], // GGDM3/NAS7 Crib
        ['F_CODE','BD030','discolored_water','yes'], // GGDM3/NAS7 Discolored Water
        ['F_CODE','BD040','seamark:water_turbulence:category','eddies'], // Eddies
        ['F_CODE','BD050','foul_ground','yes'], // GGDM3/NAS7 Foul Ground
        ['F_CODE','BD060','seamark:vegetation:category','kelp'], // Kelp/Seaweed
        ['F_CODE','BD080','seamark:water_turbulence:category','overfalls'], // Overfalls / Tide Rips
        ['F_CODE','BD100','structural_pile','yes'], // Structural Pile
        ['F_CODE','BD110','man_made','offshore_construction'], // Offshore Platform - NFDD BD115
        ['F_CODE','BD120','natural','reef'], // GGDM3/NAS7 Reef
        ['F_CODE','BD130','seamark:obstruction:category','rock'], // GGDM3/NAS7 Hazardous Rock
        ['F_CODE','BD140','seamark:obstruction:category','stump'], // Snag / Stump
        ['F_CODE','BD180','historic','wreck'], // GGDM3/NAS7 Wreck
        ['F_CODE','BE010','seamark:type','depth_curve'], // GGDM3/NAS7 DEPTH_CURVE
        ['F_CODE','BE020','seamark:type','sounding'], // GGDM3/NAS7 SOUNDING
        ['F_CODE','BF010','seamark:type','seabed_area'], // GGDM3/NAS7 BOTTOM_CHARACTER_REGION
        ['F_CODE','BG010','seamark:type','water_movement'], // GGDM3/NAS7 WATER_MOVEMENT_DATA_LOCATION
        ['F_CODE','BG020','monitoring:tide_gauge','yes'], // Tide Gauge
        ['F_CODE','BG030','seamark:type','observation_station'], // GGDM3/NAS7 TIDAL_STREAM_OBSERVE_STATION
        ['F_CODE','BG040','current_diagram','yes'], // Current Diagram
        ['F_CODE','BH010','bridge','aqueduct'], // Aqueduct
        ['F_CODE','BH020','waterway','canal'], // Canal
        ['F_CODE','BH050','landuse','aquaculture'], // GGDM3/NAS7 MARICULTURE_SITE
        ['F_CODE','BH080','water','lake'], // Lake - NFDD BH082
        ['F_CODE','BH090','water','intermittent'], // Land Subject to Inundation
        ['F_CODE','BH095','natural','wetland'], // Wetland
        ['F_CODE','BH120','waterway','rapids'], // Rapids
        ['F_CODE','BH130','water','reservoir'], // Reservoir - NFDD BH082
        ['F_CODE','BH140','waterway','river'], // River - See custom rules
        ['F_CODE','BH155','landuse','salt_pond'], // Salt Evaporator
        ['F_CODE','BH180','waterway','waterfall'], // Waterfall
        ['F_CODE','BH210','shoreline','yes'], // Inland Shoreline - NFDD BA010 Land/water boundary
        ['F_CODE','BI020','waterway','dam'], // Dam/Weir
        ['F_CODE','BI030','lock','yes'], // Lock
        ['F_CODE','BI040','waterway','flow_control'], // Sluice Gate
        ['F_CODE','BJ030','natural','glacier'], // Glacier
        ['F_CODE','BJ060','natural','ice_peak'], // Ice Peak
        ['F_CODE','BJ065','natural','ice_shelf'], // Ice Shelf
        ['F_CODE','BJ100','landcover','snowfield'], // Snow Field and/or Ice-field
        ['F_CODE','BJ110','natural','tundra'], // Tundra
        ['F_CODE','CA010','contour','elevation'], // Elevation Contour
        ['F_CODE','CA030','geological','spot_elevation'], // Spot Elevation
        ['F_CODE','DA010','geological','soil_surface_region'], // Soil Surface Region
        ['F_CODE','DB090','embankment','yes'], // Embankment, Embankment/Fill
        ['F_CODE','DB170','natural','dune'], // Sand Dune/Sand Hills
        ['F_CODE','DB180','natural','volcano'], // Volcano
        ['F_CODE','EA030','landuse','plant_nursery'], // Plant Nursery
        ['F_CODE','EA040','landuse','orchard'], // Orchard/Plantation
        ['F_CODE','EA050','landuse','vineyard'], // Vineyard
        ['F_CODE','EC030','landuse','forest'], // Wood - NFDD EC015
        ['F_CODE','FA000','boundary','administrative'], // GGDM3/NAS7 Administrative Boundary
        ['F_CODE','FA001','boundary','administrative_area'], // Administrative Area
        ['F_CODE','FA020','boundary','armistice_line'], // Armistice Line
        ['F_CODE','FA040','boundary','claim_line'], // Claim Line
        ['F_CODE','FA060','boundary','defacto'], // Defacto Boundary
        ['F_CODE','FA110','boundary','date_line'], // GGDM3/NAS7 International Date Line
        ['F_CODE','FC021','seamark:type','boundary'], // GGDM3/NAS7 MARITIME_LIMIT
        ['F_CODE','FC031','maritime_area','yes'], // Maritime Area
        ['F_CODE','FC036','restricted_area','yes'], // Restricted Area
        ['F_CODE','FC041','seamark:type','traffic_separation_scheme'], // GGDM3/NAS7 TRAFFIC_SEPARATION_SCHEME
        ['F_CODE','FC100','seamark:type','measured_distance_line'], // GGDM3/NAS7 MEASURED_DISTANCE_LINE
        ['F_CODE','FC130','seamark:type','radar_line'], // GGDM3/NAS7 MARITIME_RADAR_REF_LINE
        ['F_CODE','FC165','seamark:type','route'], // GGDM3/NAS7 MARITIME_ROUTE
        ['F_CODE','FC170','seamark:type','fairway'], // Safety Fairway
        ['F_CODE','FC177','waterway','swept_area'], // GGDM3/NAS7 SWEPT_AREA
        ['F_CODE','GA020','aeroway:light','obstacle'], // Air Obstruction Light
        ['F_CODE','GA035','aeroway','navigationaid'], // Aeronautical NAVAID - FCODE Retired
        ['F_CODE','GB005','aeroway','aerodrome'], // Land Aerodrome
        ['F_CODE','GB010','aeroway:light','yes'], // Airport Lights
        ['F_CODE','GB055','aeroway','runway'], // Runway
        ['F_CODE','GB070','seamark:type','seaplane_landing_area'], // Seaplane Run
        ['F_CODE','ZC040','natural','magnetic_anomoly'], // GGDM3/NAS7 LOCAL_MAGNETIC_ANOMALY
        ['F_CODE','ZD040','named_location','yes'] // Named Location
    ], // End fcodeOne2oneIn

    // FCODE rules for Export
    // This is for exporting "unique" DNC features.
    fcodeOne2oneOut : [
        ['F_CODE','AD010','power','generator'], // Electric Power Station, Power Plant
        ['F_CODE','AK120','leisure','common'], // Park
        ['F_CODE','AK120','leisure','playground'], // Park
        ['F_CODE','AL020','landuse','residential'], // Built Up Area
        ['F_CODE','AL260','barrier','retaining_wall'], // Wall
        ['F_CODE','AP030','highway','track'], // Cart Track - mapped to Road for this translation
        ['F_CODE','BA030','place','islet'], // Island
        ['F_CODE','BA040','natural','water'], // Water
        ['F_CODE','BD120','subsea','barrier-reef'], // Reef
        ['F_CODE','BD120','subsea','reef'], // Reef
        ['F_CODE','BD120','subsea','coral_reef'], // Reef
        ['F_CODE','BH080','water','pond'], // Lake / Pond
        ['F_CODE','EC030','natural','tree'], // Wood - NFDD EC005
        ['F_CODE','EC030','natural','wood'], // Wood - NFDD EC015
        ['F_CODE','ZD040','natural','bay'], // Named Location
        ['F_CODE','ZD040','tourism','viewpoint'], // Named Location
        ['F_CODE','DQ','source:metadata','dataset'] // Fake F_CODE for the Data Quality layer
    ], // End fcodeOne2oneOut

    // One2one rules for Text Fields
    txtBiased : {
        'BER':'berth_identifier', // Berth Identifier
        'BRR':'seamark:bearing:category', // Bearing and Reciprocal Category
        'COL':'seamark:light:character', // Character of Light
        'COMP_DATE':'source:datetime', // DQ First Edition Date
        'DAN':'navigationaid:description', // Description of Aids to Navigation
        'DAT':'report:date', // Date
        'DATUM':'source:chart:datum', // DQ Chart Datum
        'DRP':'reference_point:description', // Description of Reference Point
        'EDITION':'source:chart:edition', // DQ Chart Edition Number
        'LINEAGE':'source:chart:lineage', // DQ Linage
        'LSA':'seamark:light:sector:angle', // Light Sector Angle
        'MCA':'morse_code_attribute', // Morse Code Attribute
        'MLR':'seamark:light:ranges', // Multiple Light Ranges
        'NAM':'name', // Geographic Name Information : Full Name
        'NAME':'source:chart:name', // DQ Chart Name
        'NM3':'name:three', // Name 3
        'NM4':'name:four', // Name 4
        'OSM_UUID':'uuid', // NOTE: This is not in the DNC spec but is in our OSM data
        'PRINT_DATE':'source:chart:print_date', // DQ Chart Print date
        'SCALE':'source:scale', // DQ Chart Scale
        'SOURCE_ID':'source:chart:identifier', // DQ Chart Identifier
        'SOURCE_INFO':'source:description', // DQ Source Information
        'TXT':'note' // Text
     }, // End txtBiased

    // One2one ruled for Number fields
    numBiased : {
        'ABS_HORIZ_ACC':'source:accuracy:horizontal', // Absolute Horizontal Accuracy
        'ABS_VERT_ACC':'source:accuracy:vertical', // Absolute Vertical Accuracy
        'BR2':'seamark:radio_station:frequency:2', // Broadcast Frequency 2
        'BRF':'seamark:radio_station:frequency', // Broadcast Frequency
        'BRG':'bearing:to_object', // Bearing of Object
        'BRS':'bearing:from_seaward', // Bearing from Seaward
        'C80':'water:current:speed', // Rate of Current
        'C81':'water:current:speed:1', // Rate of Current
        'C82':'water:current:speed:2', // Rate of Current
        'C83':'water:current:speed:3', // Rate of Current
        'C84':'water:current:speed:4', // Rate of Current
        'C85':'water:current:speed:5', // Rate of Current
        'C86':'water:current:speed:6', // Rate of Current
        'C87':'water:current:speed:7', // Rate of Current
        'C88':'water:current:speed:8', // Rate of Current
        'C89':'water:current:speed:9', // Rate of Current
        'C90':'water:current:speed:10', // Rate of Current
        'C91':'water:current:speed:11', // Rate of Current
        'CRN':'water:current:speed:min', // Current Rate Minimum
        'CRV':'depth:value', // Depth Curve or Contour Value
        'CRX':'water:current:speed:max', // Current Rate Maximum
        'CVH':'depth:value:max', // Depth Curve or Contour Value - High
        'CVL':'depth:value:min', // Depth Curve or Contour Value - Low
        'D80':'water:current:direction', // Direction of Current
        'D81':'water:current:direction:1', // Direction of Current
        'D82':'water:current:direction:2', // Direction of Current
        'D83':'water:current:direction:3', // Direction of Current
        'D84':'water:current:direction:4', // Direction of Current
        'D85':'water:current:direction:5', // Direction of Current
        'D86':'water:current:direction:6', // Direction of Current
        'D87':'water:current:direction:7', // Direction of Current
        'D88':'water:current:direction:8', // Direction of Current
        'D89':'water:current:direction:9', // Direction of Current
        'D90':'water:current:direction:10', // Direction of Current
        'D91':'water:current:direction:11', // Direction of Current
        'DEP':'depth', // Depth
        'DF1':'seamark:calling-in_point:traffic_flow:direction:1', // Direction of Traffic - 1
        'DF2':'seamark:calling-in_point:traffic_flow:direction:2', // Direction of Traffic - 2
        'DF3':'seamark:calling-in_point:traffic_flow:direction:3', // Direction of Traffic - 3
        'DF4':'seamark:calling-in_point:traffic_flow:direction:4', // Direction of Traffic - 4
        'DOF':'flow_direction', // Direction of Flow
        'EOL':'seamark:light:height', // Elevation of Light
        'HDH':'hydrographic_drying_height', // Hydrographic Drying Height
        'HDP':'depth:hydrographic', // Hydrographic Depth
        'HSB':'height:above_sea_bottom', // Height above sea bottom
        'HGT':'height', // Height Above Surface Level
        'LVN':'seamark:light:radius', // Maritime Navigation Nominal Range
        'MVC':'bridge:under_clearance', // Maximum Vertical Clearance
        'OHC':'max_clearance', // Overhead Clearance
        'OR2':'seamark:radio_station:range:2', // Operating Range Category (2)
        'ORC':'seamark:radio_station:range', // Operating Range Category
        'PER':'seamark:light:period', // Period of Light
        'REF_LAT':'source:chart:reference_latitude', // DQ Chart Reference Latitude
        'SHC':'safe_clearance:horizontal', // Safe Horizontal Clearance
        'TIM':'activity_duration', // Time Attribute
        'VAL':'value', // Value - a generic value
        'VAV':'magnetic_variation', // Variation Anomaly Value
        'WID':'width', // Width
        'ZV2':'ele' // Highest Z-Value
     }, // End numBiased


    // One2one rules. Used for import. Taken from the DNC spec
    one2one : [
        // ACC - Horizontal Accuracy Category
        ['ACC','1','source:accuracy:horizontal:category','accurate'],
        ['ACC','2','source:accuracy:horizontal:category','approximate'],
        ['ACC','3','source:accuracy:horizontal:category','doubtful'],
        ['ACC','5','source:accuracy:horizontal:category','disputed'],
        ['ACC','6','source:accuracy:horizontal:category','undisputed'],


        // AHC - Associated Hydrographic Category
        // NOTE: Deconflict with HYC
        ['AHC','1','hydrographic_category:2','perennial'], // Perennial
        ['AHC','2','hydrographic_category:2','intermittent'], // Intermittent
        ['AHC','3','hydrographic_category:2','ephemeral'], // Ephemeral - not great...


        // APT - Airfield Type
        ['APT','1','aeroway:use','major_airfield'], // Major
        ['APT','2','aeroway:use','minor_airfield'], // Minor
        ['APT','4','aeroway:use','seaplane'], // Seaplane
        ['APT','9','aeroway','heliport'], // Heliport


        // ATN - Aids to Navigation
        // ['ATN','0','navigation_aid','unknown'], // Unknown
        ['ATN','1','navigation_aid','marked'], // Marked
        ['ATN','2','navigation_aid','unmarked'], // Unmarked


        // BFC - Building Function Category
        ['BFC','0','building','yes'],
        ['BFC','1','use','manufacturing'], // Fabrication Structures
        ['BFC','2','office','government'],
        ['BFC','3','building','capitol'], // Capitol Building
        ['BFC','4','building','castle'], // Castle
        ['BFC','5','amenity','government_administration_building'],
        ['BFC','6','building','hospital'],
        ['BFC','7','amenity','place_of_worship'],
        ['BFC','8','military','administration'],
        ['BFC','9','tourism','museum'],
        ['BFC','10','building','observatory'], // Observatory
        ['BFC','11','building','palace'],
        ['BFC','12','amenity','police'],
        ['BFC','13','amenity','prison'],
        ['BFC','14','amenity','ranger_station'], // Ranger Station
        ['BFC','15','building','school'],
        ['BFC','16','building','residential'],
        ['BFC','17','building:multi_unit','yes'], // Multi Unit Dwelling
        ['BFC','18','building','cemetery_building'],
        ['BFC','19','building','farm'],
        ['BFC','20','building','greenhouse'], // Greenhouse
        ['BFC','21','building','garage'],
        ['BFC','22','building','watermill'], // Watermill/Gristmill
        ['BFC','23','building','wind_tunnel'], // Wind Tunnel
        ['BFC','24','building','warehouse'],
        ['BFC','25','railway','roundhouse'], // Roundhouse
        ['BFC','26','repair','railway_vehicle'],
        ['BFC','27','man_made','depot'], // Depot Terminal
        ['BFC','28','office','administrative'],
        ['BFC','29','repair','aircraft'], // Aircraft Maintenance Shop
        ['BFC','30','building','hangar'],
        ['BFC','31','building','customs_house'], // Customs House
        ['BFC','33','building','healthcare'],
        ['BFC','35','amenity','post_office'],
        ['BFC','36','military','barracks'],
        ['BFC','37','amenity','fire_station'],
        ['BFC','53','amenity','bank'],
        ['BFC','59','use','research'],
        ['BFC','61','amenity','courthouse'],
        ['BFC','77','use','harbor_masters_office'], // Harbor Master's Office
        ['BFC','81','building','maritime_station'], // Maritime Station
        ['BFC','82','man_made','lighthouse'], // Lighthouse
        ['BFC','83','plant','power'],
        ['BFC','85','amenity','newspaper_plant'],
        ['BFC','86','use','main_telephone_exchange'], // Telephone Exchange (Main)
        ['BFC','87','amenity','auditorium'], // Auditorium
        ['BFC','88','tourism','opera_house'], // Opera House
        ['BFC','89','industrial','manufacturing'],
        ['BFC','90','building','pumping'], // Pumphouse
        ['BFC','95','tourism','hotel'],
        ['BFC','96','amenity','diplomatic_building'],
        ['BFC','999','building','other'],


        // BOT - Bridge Opening Type
        ['BOT','0',undefined,undefined], // Unknown
        ['BOT','4','bridge:movable','bascule'], // Draw/Bascule
        ['BOT','10','bridge:movable','swing'],
        ['BOT','11','bridge:movable','lift'],
        ['BOT','12','bridge:movable','retractable'],
        ['BOT','13','bridge:movable','no'], // Not Applicable / Fixed


        // BSC - Bridge Structure Type
        ['BSC','0',undefined,undefined], // Unknown
        ['BSC','1','bridge:structure','open_spandrel_arch'], // Open Spandrel Arch
        ['BSC','2','bridge:structure','cantilever'], // Cantilever
        ['BSC','3','bridge:structure','deck'], // Deck
        ['BSC','5','bridge:structure','floating'], // Floating
        ['BSC','6','bridge:structure','girder'], // Girder
        ['BSC','8','bridge:structure','truss'], // Truss
        ['BSC','9','bridge:structure','suspension'], // Suspension
        ['BSC','12','bridge:structure','transporter'], // Transporter
        ['BSC','15','bridge:structure','slab'], // Slab
        ['BSC','16','bridge:structure','stringer_beam'], // Stringer Beam
        ['BSC','999','bridge:structure','other'], // Other


        // BTC - Buoy Type Category
        // ['BTC','0','seamark:buoy:type','unknown'], // Unknown
        ['BTC','0',undefined,undefined], // Unknown
        ['BTC','4','seamark:buoy_special_purpose:category','lanby'], // Large Navigational Buoy (LANBY)
        ['BTC','6','seamark:buoy:type','light_float'], // Light Float
        ['BTC','7','seamark:mooring:category','buoy'], // Mooring
        ['BTC','10','seamark:buoy_special_purpose:category','odas'], // Ocean Data Acquisition System (ODAS)
        ['BTC','35','seamark:type','articulated_lights'], // Articulated Lights


        // BSM - Bridge Span Mobility Type
        ['BSM','0',undefined,undefined], // Unknown
        ['BSM','1','bridge:mobile_span','yes'], // Moveable Span


        // CCC - Color Code Category
        // ['CCC','0','seamark:buoy:colour','unknown'], // Unknown
        ['CCC','0',undefined,undefined], // Unknown
        ['CCC','1','seamark:buoy:colour','black'], // Black
        ['CCC','2','seamark:buoy:colour','blue'], // Blue
        ['CCC','3','seamark:buoy:colour','brown'], // Brown
        ['CCC','4','seamark:buoy:colour','gray'], // Gray
        ['CCC','5','seamark:buoy:colour','green'], // Green
        ['CCC','9','seamark:buoy:colour','orange'], // Orange
        ['CCC','12','seamark:buoy:colour','red'], // Red
        ['CCC','14','seamark:buoy:colour','violet'], // Violet
        ['CCC','15','seamark:buoy:colour','white'], // White
        ['CCC','19','seamark:buoy:colour','yellow'], // Yellow
        ['CCC','20','seamark:buoy:colour','red_and_white_(rw)'], // Red & White (RW)
        ['CCC','21','seamark:buoy:colour','red_and_green_(rg)'], // Red & Green (RG)
        ['CCC','22','seamark:buoy:colour','red_and_black_(rb)'], // Red & Black (RB)
        ['CCC','23','seamark:buoy:colour','red-green-red_(rgr)'], // Red-Green-Red (RGR)
        ['CCC','24','seamark:buoy:colour','green_and_white_(gw)'], // Green & White (GW)
        ['CCC','25','seamark:buoy:colour','green_and_red_(gr)'], // Green & Red (GR)
        ['CCC','26','seamark:buoy:colour','green_and_black_(gb)'], // Green & Black (GB)
        ['CCC','27','seamark:buoy:colour','green-red-green_(grg)'], // Green-Red-Green (GRG)
        ['CCC','28','seamark:buoy:colour','green-yellow-black_(gyb)'], // Green-Yellow-Black (GYB)
        ['CCC','29','seamark:buoy:colour','yellow_and_black_(yb)'], // Yellow & Black (YB)
        ['CCC','30','seamark:buoy:colour','yellow-black-yellow_(yby)'], // Yellow-Black-Yellow (YBY)
        ['CCC','31','seamark:buoy:colour','yellow_and_red_(yr)'], // Yellow & Red (YR)
        ['CCC','32','seamark:buoy:colour','yellow_and_green_(yg)'], // Yellow & Green (YG)
        ['CCC','33','seamark:buoy:colour','yellow-red-white_(yrw)'], // Yellow-Red-White (YRW)
        ['CCC','34','seamark:buoy:colour','black_and_yellow_(by)'], // Black & Yellow (BY)
        ['CCC','35','seamark:buoy:colour','black-yellow-black_(byb)'], // Black-Yellow-Black (BYB)
        ['CCC','36','seamark:buoy:colour','black-red-black_(brb)'], // Black-Red-Black (BRB)
        ['CCC','37','seamark:buoy:colour','black_and_white_(bw)'], // Black & White (BW)
        ['CCC','38','seamark:buoy:colour','black_and_red_(br)'], // Black & Red (BR)
        ['CCC','39','seamark:buoy:colour','black_and_green_(bg)'], // Black & Green (BG)
        ['CCC','40','seamark:buoy:colour','white_and_red_(wr)'], // White & Red (WR)
        ['CCC','41','seamark:buoy:colour','white_and_orange_(wor)'], // White & Orange (WOr)
        ['CCC','42','seamark:buoy:colour','white_and_green_(wg)'], // White & Green (WG)
        ['CCC','43','seamark:buoy:colour','white_and_black_(wb)'], // White & Black (WB)
        ['CCC','44','seamark:buoy:colour','white_and_yellow_(wy)'], // White & Yellow (WY)
        ['CCC','45','seamark:buoy:colour','white-red-green_(wrg)'], // White-Red-Green (WRG)
        ['CCC','46','seamark:buoy:colour','white-green-white_(wgw)'], // White-Green-White (WGW)


        // CHA - Light Characteristic Category
        ['CHA','21','beacon:light','lighted'], // Lighted
        ['CHA','23','beacon:light','unlighted'], // Unlighted


        // COD - Certainty of Delineation
        ['COD','1','deliniation','known'], // Limits and Information Known
        ['COD','2','deliniation','unknown'], // Limits and Information Unknown


        // CSM - Secondary Material Characteristics
        // ['CSM','0','material:characteristic','unknown'], // Unknown
        ['CSM','0',undefined,undefined], // Unknown
        ['CSM','1','material:characteristic','broken'], // Broken
        ['CSM','2','material:characteristic','coarse'], // Coarse
        ['CSM','3','material:characteristic','decayed'], // Decayed
        ['CSM','4','material:characteristic','fine'], // Fine, Minute Particles
        ['CSM','5','material:characteristic','gritty'], // Gritty
        ['CSM','6','material:characteristic','hard'], // Hard
        ['CSM','7','material:characteristic','rotten'], // Rotten
        ['CSM','8','material:characteristic','soft'], // Soft
        ['CSM','9','material:characteristic','sticky'], // Sticky
        ['CSM','10','material:characteristic','stiff'], // Stiff
        ['CSM','11','material:characteristic','streaky'], // Streaky
        ['CSM','12','material:characteristic','tenacious'], // Tenacious
        ['CSM','13','material:characteristic','uneven'], // Uneven
        ['CSM','17','material:characteristic','calcareous'], // Calcareous
        ['CSM','18','material:characteristic','flinty'], // Flinty
        ['CSM','19','material:characteristic','glacial'], // Glacial
        ['CSM','20','material:characteristic','ground'], // Ground
        ['CSM','21','material:characteristic','large'], // Large
        ['CSM','22','material:characteristic','rocky'], // Rocky
        ['CSM','23','material:characteristic','small'], // Small
        ['CSM','24','material:characteristic','speckled'], // Speckled
        ['CSM','25','material:characteristic','varied'], // Varied
        ['CSM','26','material:characteristic','volcanic'], // Volcanic
        ['CSM','27','material:characteristic','medium'], // Medium


        // CUR - Current Type Category
        // ['CUR','0','current','unknown'], // Unknown
        ['CUR','0',undefined,undefined], // Unknown
        ['CUR','1','current','ebb_stream'], // Ebb
        ['CUR','2','current','flood_stream'], // Flood
        ['CUR','3','current','general_flow'], // General Flow
        ['CUR','5','current','ocean_current'], // Ocean Flow


        // ELA - Elevation Accuracy Category
        ['ELA','1','source:accuracy:vertical:category','accurate'],
        ['ELA','2','source:accuracy:vertical:category','approximate'],


        // EXS - Existence Category
        // NOTE: Many of these values are "defaults" for different features. Why???
        ['EXS','0','existence','unknown'], // Unknown
        ['EXS','1','existence','definite'],
        ['EXS','2','existence','doubtful'],
        ['EXS','3','existence','reported'],
        ['EXS','5','condition','construction'],
        ['EXS','6','condition','abandoned'],
        ['EXS','7','condition','destroyed'],
        ['EXS','8','condition','dismantled'], // Dismantled
        ['EXS','22','oneway','yes'], // One Way
        ['EXS','23','oneway','no'], // Two Way
        ['EXS','28','condition','functional'], // Fully Functional - default
        ['EXS','30','isolated','no'], // Not Isolated ???
        ['EXS','31','isolated','yes'], // Isolated
        ['EXS','32','navigation','yes'], // Navigable
        ['EXS','42','operational_status','continuous_operation'], // Continuous Operation
        ['EXS','45','artificial','no'], // Natural
        ['EXS','46','artificial','yes'], // Man-Made
        ['EXS','50','tidal','no'], // Non Tidal
        ['EXS','51','tidal','yes'], // Tidal / Tidal Fluctuation


        // HDI - Hydrographic Depth/Height Information
        ['HDI','9','depth:hydrographic:information','depth_known_by_other_than_wire_drag'],
        ['HDI','10','depth:hydrographic:information','depth_known_by_wire_drag'],
        ['HDI','11','depth:hydrographic:information','depth_unknown_but_safe_to_depth_shown'],
        ['HDI','12','depth:hydrographic:information','depth_unknown'],
        ['HDI','13','depth:hydrographic:information','uncovering_height_known'],
        ['HDI','14','depth:hydrographic:information','uncovering_height_unknown'],
        ['HDI','15',undefined,undefined], // Not Applicable



        // HLT - Hydrographic Light Type
        // ['HLT','0','seamark:light:category','unknown'], // Unknown
        ['HLT','0',undefined,undefined], // Unknown
        ['HLT','1','seamark:light:category','sectored_light'], // Sectored Light
        ['HLT','3','seamark:light:category','moire'], // Moir Effect Light
        ['HLT','4','seamark:light:category','strip_light'], // Strip Light
        ['HLT','5','seamark:light:category','occasional'], // Occasional


        // HQC - Hypsography Portrayal Type
        ['HQC','1','contour:type','index_contour'], // Index Contour
        ['HQC','2','contour:type','intermediate_contour'], // Intermediate Contour
        ['HQC','3','contour:type','half_auxiliary_contour'], // Half Auxiliary Contour
        ['HQC','4','contour:type','form_line'], // Form Line
        ['HQC','7','contour:type','approximate_index_contour'], // Approximate Index Contour
        ['HQC','12','contour:type','approximate_intermediate_contour'], // Approximate Intermediate Contour
        ['HQC','13','contour:type','approximate_auxiliary_contour'], // Approximate Auxiliary Contour


        // HS1 - Current Information (1)
        ['HS1','0',undefined,undefined], // Unknown/Not Applicable
        ['HS1','1','water:current:appearance','jan'], // Jan
        ['HS1','2','water:current:appearance','feb'], // Feb
        ['HS1','3','water:current:appearance','mar'], // Mar
        ['HS1','4','water:current:appearance','apr'], // Apr
        ['HS1','5','water:current:appearance','may'], // May
        ['HS1','6','water:current:appearance','jun'], // Jun
        ['HS1','7','water:current:appearance','jul'], // Jul
        ['HS1','8','water:current:appearance','aug'], // Aug
        ['HS1','9','water:current:appearance','sep'], // Sep
        ['HS1','10','water:current:appearance','oct'], // Oct
        ['HS1','11','water:current:appearance','nov'], // Nov
        ['HS1','12','water:current:appearance','dec'], // Dec


        // HS2 - Current Information (2)
        ['HS2','0',undefined,undefined], // Unknown/Not Applicable
        ['HS2','1','water:current:disappearance','jan'], // Jan
        ['HS2','2','water:current:disappearance','feb'], // Feb
        ['HS2','3','water:current:disappearance','mar'], // Mar
        ['HS2','4','water:current:disappearance','apr'], // Apr
        ['HS2','5','water:current:disappearance','may'], // May
        ['HS2','6','water:current:disappearance','jun'], // Jun
        ['HS2','7','water:current:disappearance','jul'], // Jul
        ['HS2','8','water:current:disappearance','aug'], // Aug
        ['HS2','9','water:current:disappearance','sep'], // Sep
        ['HS2','10','water:current:disappearance','oct'], // Oct
        ['HS2','11','water:current:disappearance','nov'], // Nov
        ['HS2','12','water:current:disappearance','dec'], // Dec


        // HWT - House of Worship Type
        ['HWT','0',undefined,undefined],
        ['HWT','2','building','cathedral'],
        ['HWT','3','building','chapel'],
        ['HWT','4','building','church'],
        ['HWT','5','building','marabout'],
        ['HWT','6','building','minaret'],
        ['HWT','7','building','religious_community'],
        ['HWT','9','building','mosque'],
        ['HWT','11','building','pagoda'],
        ['HWT','14','building','shrine'],
        ['HWT','15','building','tabernacle'],
        ['HWT','16','building','temple'],
        ['HWT','20','building','synagogue'],
        ['HWT','21','building','stupa'],
        ['HWT','22',undefined,undefined],


        // HYC - Hydrologic Category
        ['HYC','3','intermittent','dry'], // Dry
        ['HYC','6','intermittent','yes'], // Non-Perennial
        ['HYC','8','intermittent','no'], // Perennial


        // IAC - IALA Aid Category
        // ['IAC','0','iala_navigation_aid','unknown'], // Unknown
        ['IAC','0',undefined,undefined], // Unknown
        ['IAC','1','iala_navigation_aid','no'], // Non-IALA Aid
        ['IAC','2','iala_navigation_aid','yes'], // IALA Aid - Region A


        // IAS - IMO Approval Status
        ['IAS','1','imo_adopted','yes'], // Approved
        ['IAS','2','imo_adopted','no'], // Not Approved


        // LAF - Line Associated Features
        // ['LAF','0','navigation_line_feature','unknown'], // Unknown
        ['LAF','0',undefined,undefined], // Unknown
        ['LAF','1','navigation_line_feature','one_other_than_directional_light'], // One Object (Other than a Directional Light)
        ['LAF','2','navigation_line_feature','directional_light'], // Directional Light
        ['LAF','3','navigation_line_feature','two_lights_or_more'], // Two or more lights
        ['LAF','4','navigation_line_feature','two_beacons_or_more'], // Two or more beacons
        ['LAF','5','navigation_line_feature','two_other_than_two_lights_or_beacons'], // Two or More Objects (Other Than Two Lights or Beacons)
        ['LAF','6','navigation_line_feature','measured_distance_markers'], // Measured Distance Markers
        ['LAF','7','navigation_line_feature','directional_radiobeacon'], // Directional Radiobeacon
        ['LAF','8','navigation_line_feature','moir_effect_light'], // Moir Effect Light


        // LOC - Vertical Relative Location.
        // ['LOC','0','location','unknown'], // Unknown
        ['LOC','0',undefined,undefined], // Unknown
        ['LOC','4','location','underwater'], // Below Water Surface
        ['LOC','8','location','surface'], // On Ground Surface
        ['LOC','13','location','hull_showing'], // Hull Showing
        ['LOC','14','location','masts_showing'], // Masts Showing
        ['LOC','15','location','on_water_surface/floating'], // On Water Surface/Floating
        ['LOC','17','location','on_waterbody_bottom'], // No OSM
        ['LOC','20','location','funnel_showing'], // Funnel Showing
        ['LOC','21','location','superstructuure_showing'], // Superstructure Showing
        ['LOC','22','location','offshore'], // Off Shore
        ['LOC','23','location','below_waterbody_bottom'], // No OSM
        ['LOC','24','location','suspended_or_elevated_above_sea_bottom'], // Suspended or Elevated above sea bottom
        ['LOC','25','location','overground'], // Suspended /Elevated Above Ground or Water Surface
        ['LOC','28','location','masts_and_funnel_showing'], // Masts and Funnel Showing
        ['LOC','30','location','non-floating'], // Non-Floating


        // MAS - Maintenance Status
        ['MAS','1','maintained','yes'], // Maintained
        ['MAS','2','maintained','no'], // Not Maintained


        // MAC - Maritime Area Category
        // ['MAC','0','maritime_area:type','unknown'], // Unknown
        ['MAC','0',undefined,undefined], // Unknown
        ['MAC','2','maritime_area:type','dredged_channel/dredged_area'], // Dredged Channel/Dredged Area
        ['MAC','4','maritime_area:type','mine_danger_area'], // Mine Danger Area
        ['MAC','5','maritime_area:type','prohibited_shipping_area/entry_prohibited'], // Prohibited Shipping Area/Entry Prohibited
        ['MAC','9','maritime_area:type','works_in_progress_area'], // Works in Progress Area
        ['MAC','11','maritime_area:type','anchorage_(general)'], // Anchorage (general)
        ['MAC','12','maritime_area:type','anchoring_berths'], // Anchoring Berths
        ['MAC','13','maritime_area:type','explosives_anchorage'], // Explosives anchorage
        ['MAC','14','maritime_area:type','large_vessel/deep_water/deep_draft_anchorage'], // Large Vessel/Deep Water/Deep Draft anchorage
        ['MAC','15','maritime_area:type','anchoring_prohibited'], // Anchoring Prohibited
        ['MAC','16','maritime_area:type','quarantine_anchorage'], // Quarantine Anchorage
        ['MAC','17','maritime_area:type','reserved_anchorage'], // Reserved Anchorage
        ['MAC','19','maritime_area:type','tanker_anchorage'], // Tanker Anchorage
        ['MAC','20','maritime_area:type','submarine_cable_area'], // Submarine Cable Area
        ['MAC','21','maritime_area:type','pipeline_area'], // Pipeline Area
        ['MAC','22','maritime_area:type','fishing_prohibited'], // Fishing Prohibited
        ['MAC','23','maritime_area:type','cable_and_pipeline_area'], // Cable and Pipeline Area
        ['MAC','24','maritime_area:type','turning_area/swinging_circle'], // Turning Area/Swinging Circle
        ['MAC','25','maritime_area:type','spoil_area/spoil_ground_(see_ops_for_status)'], // Spoil Area/Spoil Ground (See OPS for status)
        ['MAC','26','maritime_area:type','unsurveyed_area'], // Unsurveyed Area
        ['MAC','27','maritime_area:type','submarine_exercise_area'], // Submarine Exercise Area
        ['MAC','28','maritime_area:type','mine_laying_practice_area'], // Mine Laying Practice Area
        ['MAC','29','maritime_area:type','firing_danger_area'], // Firing Danger Area
        ['MAC','30','maritime_area:type','dumping_ground_for_hazardous_materials'], // Dumping Ground for Hazardous Materials
        ['MAC','31','maritime_area:type','incineration_area'], // Incineration Area
        ['MAC','32','maritime_area:type','oil_field'], // Oil Field
        ['MAC','33','maritime_area:type','gas_field'], // Gas Field
        ['MAC','35','maritime_area:type','explosive_dumping_ground'], // Explosive Dumping Ground
        ['MAC','37','maritime_area:type','safety_zone'], // Safety Zone
        ['MAC','40','maritime_area:type','roundabout_zone_(tss)'], // Roundabout Zone (TSS)
        ['MAC','41','maritime_area:type','inshore_traffic_zone_(tss)'], // Inshore Traffic Zone (TSS)
        ['MAC','42','maritime_area:type','precautionary_area'], // Precautionary Area
        ['MAC','43','maritime_area:type','area_to_be_avoided'], // Area to be avoided
        ['MAC','44','maritime_area:type','degaussing_range'], // Degaussing Range
        ['MAC','45','maritime_area:type','outfall_area'], // Outfall Area
        ['MAC','46','maritime_area:type','intake_area'], // Intake Area
        ['MAC','48','maritime_area:type','pilot_boarding_area'], // Pilot Boarding Area
        ['MAC','49','maritime_area:type','cargo_transshipment_area'], // Cargo Transshipment Area
        ['MAC','53','maritime_area:type','seaplane'], // Seaplane
        ['MAC','54','maritime_area:type','time_limited'], // Time Limited
        ['MAC','55','maritime_area:type','fairway'], // Fairway
        ['MAC','56','maritime_area:type','fish_trap_area'], // Fish Trap Area
        ['MAC','57','maritime_area:type','marine_farm'], // Marine Farm
        ['MAC','58','maritime_area:type','dredging_area'], // Dredging Area
        ['MAC','61','maritime_area:type','sewer_area'], // Sewer Area
        ['MAC','999','maritime_area:type','other'], // Other


        // MCC - Material Composition Type
        ['MCC','-32768',undefined,undefined], // Null
        ['MCC','0',undefined,undefined], // No Information
        ['MCC','4','material','ash'], // Ash
        ['MCC','8','material','boulders'], // Boulders
        ['MCC','12','material','chalk'], // Chalk
        ['MCC','14','material','cinders'], // Cinders
        ['MCC','15','material','cirripedia'], // Cirripedia
        ['MCC','16','material','clay'], // Clay
        ['MCC','18','material','cobble'], // Cobble
        ['MCC','24','material','coral'], // Coral
        ['MCC','25','material','coral_head'], // Coral Head
        ['MCC','28','material','diatoms'], // Diatoms
        ['MCC','36','material','foraminifers'], // Foraminifers
        ['MCC','37','material','fucus'], // Fucus
        ['MCC','41','material','globigerina'], // Globingerina
        ['MCC','45','material','grass'], // Grass / Thatch
        ['MCC','46','material','gravel'], // Gravel
        ['MCC','48','material','ground'], // Ground
        ['MCC','52','material','lava'], // Lava
        ['MCC','58','material','madrepores'], // Madrepores
        ['MCC','59','material','manganese'], // Manganese
        ['MCC','61','material','marl'], // Marl
        ['MCC','63','material','mattes'], // mattes
        ['MCC','65','material','mud'], // Mud
        ['MCC','66','material','mussels'], // Mussels
        ['MCC','69','material','ooze'], // ooze
        ['MCC','70','material','oysters'], // Oysters
        ['MCC','73','material','pebbles'], // Pebbles
        ['MCC','74','material','plastic'], // Plastic
        ['MCC','75','material','polyzoa'], // Polyzoa
        ['MCC','78','material','pteropods'], // Pteropods
        ['MCC','79','material','pumice'], // Pumice
        ['MCC','80','material','quartz'], // Quartz
        ['MCC','81','material','radiolaria'], // Radiolaria
        ['MCC','84','material','rock'], // Rock
        ['MCC','88','material','sand'], // Sand
        ['MCC','90','material','schist'], // Schist
        ['MCC','92','material','scoria'], // Scoria
        ['MCC','93','material','sea_tangle'], // Sea Tangle
        ['MCC','94','material','seaweed'], // Seaweed
        ['MCC','96','material','shells'], // Shells
        ['MCC','98','material','shingle'], // Shingle
        ['MCC','99','material','silt'], // Silt
        ['MCC','105','material','spicules'], // Spicules
        ['MCC','106','material','sponge'], // Sponge
        ['MCC','108','material','stone'], // Stone
        ['MCC','111','material','tufa'], // Tufa
        ['MCC','999','material','other'], // Other


        // MCS - Material Composition Secondary
        // ['MCS','0','material:2','unknown'], // Unknown
        ['MCS','0',undefined,undefined], // Unknown
        ['MCS','4','material:2','ash'], // Ash
        ['MCS','8','material:2','boulders'], // Boulders
        ['MCS','12','material:2','chalk'], // Chalk
        ['MCS','14','material:2','cinders'], // Cinders
        ['MCS','15','material:2','cirripedia'], // Cirripedia
        ['MCS','16','material:2','clay'], // Clay
        ['MCS','18','material:2','cobble'], // Cobble
        ['MCS','24','material:2','coral'], // Coral
        ['MCS','25','material:2','coral_head'], // Coral Head
        ['MCS','28','material:2','diatoms'], // Diatoms
        ['MCS','36','material:2','foraminifera'], // Foraminifera
        ['MCS','37','material:2','fucus'], // Fucus
        ['MCS','41','material:2','globigerina'], // Globigerina
        ['MCS','45','material:2','grass_/thatch'], // Grass /Thatch
        ['MCS','46','material:2','gravel'], // Gravel
        ['MCS','48','material:2','ground'], // Ground
        ['MCS','52','material:2','lava'], // Lava
        ['MCS','58','material:2','madrepores'], // Madrepores
        ['MCS','59','material:2','manganese'], // Manganese
        ['MCS','61','material:2','marl'], // Marl
        ['MCS','63','material:2','mattes'], // Mattes
        ['MCS','65','material:2','mud'], // Mud
        ['MCS','66','material:2','mussels'], // Mussels
        ['MCS','69','material:2','ooze'], // Ooze
        ['MCS','70','material:2','oysters'], // Oysters
        ['MCS','73','material:2','pebbles'], // Pebbles
        ['MCS','75','material:2','polyzoa'], // Polyzoa
        ['MCS','78','material:2','pteropods'], // Pteropods
        ['MCS','79','material:2','pumice'], // Pumice
        ['MCS','80','material:2','quartz'], // Quartz
        ['MCS','81','material:2','radiolaria'], // Radiolaria
        ['MCS','84','material:2','rock'], // Rock/Rocky
        ['MCS','88','material:2','sand'], // Sand
        ['MCS','90','material:2','schist'], // Schist
        ['MCS','92','material:2','scoria'], // Scoria
        ['MCS','93','material:2','sea_tangle'], // Sea Tangle
        ['MCS','94','material:2','seaweed'], // Seaweed
        ['MCS','96','material:2','shells'], // Shells
        ['MCS','98','material:2','shingle'], // Shingle
        ['MCS','99','material:2','silt'], // Silt
        ['MCS','104','material:2','soil'], // Soil
        ['MCS','105','material:2','spicules'], // Spicules
        ['MCS','106','material:2','sponge'], // Sponge
        ['MCS','108','material:2','stone'], // Stone
        ['MCS','111','material:2','tufa'], // Tufa


        // MCU - Material Composition Underlying
        // ['MCU','0','material:underlying','unknown'], // Unknown
        ['MCU','0',undefined,undefined], // Unknown
        ['MCU','4','material:underlying','ash'], // Ash
        ['MCU','8','material:underlying','boulders'], // Boulders
        ['MCU','12','material:underlying','chalk'], // Chalk
        ['MCU','14','material:underlying','cinders'], // Cinders
        ['MCU','15','material:underlying','cirripedia'], // Cirripedia
        ['MCU','16','material:underlying','clay'], // Clay
        ['MCU','18','material:underlying','cobble'], // Cobble
        ['MCU','24','material:underlying','coral'], // Coral
        ['MCU','25','material:underlying','coral_head'], // Coral Head
        ['MCU','28','material:underlying','diatoms'], // Diatoms
        ['MCU','36','material:underlying','foraminifera'], // Foraminifera
        ['MCU','37','material:underlying','fucus'], // Fucus
        ['MCU','41','material:underlying','globigerina'], // Globigerina
        ['MCU','45','material:underlying','grass_/thatch'], // Grass /Thatch
        ['MCU','46','material:underlying','gravel'], // Gravel
        ['MCU','48','material:underlying','ground'], // Ground
        ['MCU','52','material:underlying','lava'], // Lava
        ['MCU','58','material:underlying','madrepores'], // Madrepores
        ['MCU','59','material:underlying','manganese'], // Manganese
        ['MCU','61','material:underlying','marl'], // Marl
        ['MCU','63','material:underlying','mattes'], // Mattes
        ['MCU','65','material:underlying','mud'], // Mud
        ['MCU','66','material:underlying','mussels'], // Mussels
        ['MCU','69','material:underlying','ooze'], // Ooze
        ['MCU','70','material:underlying','oysters'], // Oysters
        ['MCU','73','material:underlying','pebbles'], // Pebbles
        ['MCU','75','material:underlying','polyzoa'], // Polyzoa
        ['MCU','78','material:underlying','pteropods'], // Pteropods
        ['MCU','79','material:underlying','pumice'], // Pumice
        ['MCU','80','material:underlying','quartz'], // Quartz
        ['MCU','81','material:underlying','radiolaria'], // Radiolaria
        ['MCU','84','material:underlying','rock'], // Rock/Rocky
        ['MCU','88','material:underlying','sand'], // Sand
        ['MCU','90','material:underlying','schist'], // Schist
        ['MCU','92','material:underlying','scoria'], // Scoria
        ['MCU','93','material:underlying','sea_tangle'], // Sea Tangle
        ['MCU','94','material:underlying','seaweed'], // Seaweed
        ['MCU','96','material:underlying','shells'], // Shells
        ['MCU','98','material:underlying','shingle'], // Shingle
        ['MCU','99','material:underlying','silt'], // Silt
        ['MCU','105','material:underlying','spicules'], // Spicules
        ['MCU','106','material:underlying','sponge'], // Sponge
        ['MCU','108','material:underlying','stone'], // Stone
        ['MCU','111','material:underlying','tufa'], // Tufa


        // NS2 - Navigation System Types (2)
        // ['NS2','0','beacon:type:2','unknown'], // Unknown
        ['NS2','0',undefined,undefined], // Unknown
        ['NS2','1','beacon:type:2','circular_radio_beacon'], // Circular Radio Beacon
        ['NS2','4','beacon:type:2','radio_direction_finding'], // Radio Direction Finding
        ['NS2','5','beacon:type:2','directional_radio_beacon'], // Directional Radio Beacon
        ['NS2','10','beacon:type:2','radar_responder_beacon_(racon)'], // Radar Responder Beacon (RACON)
        ['NS2','17','beacon:type:2','non-directional_radio_beacon_(ndb)'], // Non-Directional Radio Beacon (NDB)
        ['NS2','41','beacon:type:2','rotating_loop_radio_beacon'], // Rotating Loop Radio Beacon
        ['NS2','45','beacon:type:2','radar_station'], // Radar Station
        ['NS2','51','beacon:type:2','radiobeacon,_type_unknown'], // Radiobeacon, Type Unknown
        ['NS2','52','beacon:type:2','none'], // None
        ['NS2','53','beacon:type:2','qtg_station_(r)'], // QTG Station (R)
        ['NS2','54','beacon:type:2','ramark_(ramark)'], // Ramark (Ramark)


        // NST - Navigation System Types
        // ['NST','0','beacon:type','unknown'], // Unknown
        ['NST','0',undefined,undefined], // Unknown
        ['NST','1','beacon:type','circular_radio_beacon'], // Circular Radio Beacon
        ['NST','2','beacon:type','consol'], // CONSOL
        ['NST','3','beacon:type','decca'], // DECCA
        ['NST','4','beacon:type','radio_direction_finding'], // Radio Direction Finding
        ['NST','5','beacon:type','directional_radio_beacon'], // Directional Radio Beacon
        ['NST','7','beacon:type','loran'], // Long Range Air Navigation System (LORAN)
        ['NST','8','beacon:type','omega'], // OMEGA
        ['NST','10','beacon:type','radar_responder_beacon_(racon)'], // Radar Responder Beacon (RACON)
        ['NST','12','beacon:type','radio'], // Radio
        ['NST','13','beacon:type','radio_telephone'], // Radio Telephone
        ['NST','15','beacon:type','television'], // TV
        ['NST','16','beacon:type','microwave'], // Microwave
        ['NST','17','beacon:type','ndb'], // Non-Directional Radio Beacon (NDB)
        ['NST','19','beacon:type','radio_range'], // Radio Range (RNG)
        ['NST','33','beacon:type','radio_telegraph'], // Radio Telegraph
        ['NST','41','beacon:type','rotating_loop_radio_beacon'], // Rotating Loop Radio Beacon
        ['NST','45','beacon:type','radar_station'], // Radar Station
        ['NST','51','beacon:type','radiobeacon,_type_unknown'], // Radiobeacon, Type Unknown
        ['NST','52','beacon:type','none'], // None
        ['NST','53','beacon:type','qtg_station_(r)'], // QTG Station (R)
        ['NST','54','beacon:type','ramark_(ramark)'], // Ramark (Ramark)
        ['NST','999','beacon:type','other'], // Other


        // OLQ - Obstruction Light Quality
        // ['OLQ','0','obstruction_light','unknown'], // Unknown
        ['OLQ','0',undefined,undefined], // Unknown
        ['OLQ','1','obstruction_light','one_light'], // One Light Present
        ['OLQ','2','obstruction_light','multiple_lights'], // Multiple Lights Present


        // OPS - Operational Status
        ['OPS','1','operational_status','operational'], // Operational
        ['OPS','2','operational_status','non-operational'], // Non Operational


        // OWO - Over Water Obstruction
        ['OWO','1','waterway:overhead_obstruction','yes'], // Feature crosses navigable water
        ['OWO','2','waterway:overhead_obstruction','no'], // Feature does not cross navigable water


        // PBV - Pilot Boarding Vehicle
        // ['PBV','0','pilot_boarding_vehicle','unknown'], // Unknown
        ['PBV','0',undefined,undefined], // Unknown
        ['PBV','1','pilot_boarding_vehicle','boat'], // By Boat
        ['PBV','2','pilot_boarding_vehicle','helicopter'], // By Helicopter


        // PLT - Pipeline Type
        // ['PLT','0','pipeline:type','unknown'], // Unknown
        ['PLT','0',undefined,undefined], // Unknown
        ['PLT','1','pipeline:type','transport'], // Transport
        ['PLT','2','pipeline:type','outfall'], // Outfall
        ['PLT','3','pipeline:type','intake'], // Intake


        // PRO - Product
        // NOTE: Check the defaults on export: 0 & 130
        // ['PRO','0','product','unknown'], // Unknown
        ['PRO','0',undefined,undefined], // Unknown
        ['PRO','3','product','ammunition'], // Ammunition
        ['PRO','13','product','chemical'], // Chemical
        ['PRO','17','product','coal'], // Coal
        ['PRO','19','product','coke'], // Coke
        ['PRO','31','product','electric_power'], // Electric Power
        ['PRO','33','product','explosive'], // Explosive
        ['PRO','38','product','gas'], // Gas
        ['PRO','39','product','petrol'], // Petrol (Gasoline)
        ['PRO','46','product','gravel'], // Gravel -- From PPO
        ['PRO','67','product','oil'], // Oil
        ['PRO','82','product','radioactive_material'], // Radioactive Material
        ['PRO','87','product','salt'], // Salt
        ['PRO','88','product','sand'], // Sand
        ['PRO','95','product','sewage'], // Sewage
        ['PRO','108','product','stone'], // Stone
        ['PRO','116','product','water'], // Water
        ['PRO','130',undefined,undefined], // None
        ['PRO','999','product','other'], // Other


        // PSC - Physical Surface Characteristics
        // ['PSC','0','physical:characteristic','unknown'], // Unknown
        ['PSC','0',undefined,undefined], // Unknown
        ['PSC','1','physical:characteristic','broken'], // Broken
        ['PSC','2','physical:characteristic','coarse'], // Coarse
        ['PSC','3','physical:characteristic','decayed'], // Decayed
        ['PSC','4','physical:characteristic','fine'], // Fine, minute particles
        ['PSC','5','physical:characteristic','gritty'], // Gritty
        ['PSC','6','physical:characteristic','hard'], // Hard
        ['PSC','7','physical:characteristic','rotten'], // Rotten
        ['PSC','8','physical:characteristic','soft'], // Soft
        ['PSC','9','physical:characteristic','sticky'], // Sticky
        ['PSC','10','physical:characteristic','stiff'], // Stiff
        ['PSC','11','physical:characteristic','streaky'], // Streaky
        ['PSC','12','physical:characteristic','tenacious'], // Tenacious
        ['PSC','13','physical:characteristic','uneven'], // Uneven
        ['PSC','17','physical:characteristic','calcareous'], // Calcareous
        ['PSC','18','physical:characteristic','flinty'], // Flinty
        ['PSC','19','physical:characteristic','glacial'], // Glacial
        ['PSC','20','physical:characteristic','ground'], // Ground
        ['PSC','21','physical:characteristic','large'], // Large
        ['PSC','22','physical:characteristic','rocky'], // Rocky
        ['PSC','23','physical:characteristic','small'], // Small
        ['PSC','24','physical:characteristic','speckled'], // Speckled
        ['PSC','25','physical:characteristic','varied'], // Varied
        ['PSC','26','physical:characteristic','volcanic'], // Volcanic
        ['PSC','27','physical:characteristic','medium'], // Medium
        ['PSC','28','physical:characteristic','springs_in_seabed'], // Springs in Seabed
        ['PSC','29','physical:characteristic','mobile_bottom'], // Mobile Bottom


        // RPA - Required Port Access
        // ['RPA','0','port:access','unknown'], // Unknown
        ['RPA','0',undefined,undefined], // Unknown
        ['RPA','1','port:access','required'], // Required
        ['RPA','2','port:access','not_required'], // Not Required


        // REF - Radar Reflector Attribute
        ['REF','1','radar_reflector','yes'], // Radar Reflector Present
        ['REF','2','radar_reflector','no'], // Radar Reflector Absent


        // RRC - Railway Use
        ['RRC','0','railway','rail'], // Unknown
        ['RRC','2','railway','carline'], // Carline
        ['RRC','3','railway','monorail'], // Carline
        ['RRC','8','railway','logging'], // Logging
        ['RRC','13','railway','marine_railway'], // Marine Railway
        ['RRC','14','railway','tram'], // Tramway
        ['RRC','15','railway','funicular'], // Funicular
        ['RRC','16','usage','main'], // Main Line
        ['RRC','17','usage','branch'], // Branch-line
        ['RRC','21','railway:in_road','yes'],


        // RTT - Route Intended Use
        // ['RTT','0','seamark:route','unknown'], // Unknown
        ['RTT','0',undefined,undefined], // Unknown
        ['RTT','2','seamark:route','recommended_track_for_other_than_deep_draft_vessels'], // Recommended for other than deep draft vessels
        ['RTT','3','seamark:route','recommended_track_for_deep_draft_vessels'], // Recommended for deep draft vessels
        ['RTT','4','seamark:route','deep_water_route'], // Deep Water Route
        ['RTT','5','seamark:route','transit_route'], // Transit Route
        ['RTT','11','seamark:route','two_way_route'], // Two Way Route
        ['RTT','13','seamark:route','recommended_direction_of_traffic_flow'], // Recommended direction of traffic flow


        // SFC - Sea Floor Category
        ['SFC','1','sea_floor_category','unknown_obstruction'], // Unknown Obstruction - Not in FACC
        ['SFC','2','sea_floor_category','other'], // Other
        ['SFC','3','sea_floor_category','fish_haven'], // Fish Haven
        ['SFC','4','sea_floor_category','Well'], // Well
        ['SFC','5','sea_floor_category','submerged_production_platform'], // Submerged Production Platform


        // SIC - Snow / Ice Category
        // ['SIC','0','landcover','unknown'], // Unknown
        ['SIC','0',undefined,undefined], // Unknown
        ['SIC','1','landcover','snowfield'], // Snowfield
        ['SIC','2','landcover','ice-field'], // Ice-field


        // SLT - Shoreline Type
        ['SLT','0',undefined,undefined], // Unknown
        ['SLT','6','shoreline:type','mangrove'], // Mangrove
        ['SLT','8','shoreline:type','marsh'], // Marshy
        ['SLT','10','shoreline:type','stony'], // Stony
        ['SLT','11','shoreline:type','building_rubble'], // Building Rubble
        ['SLT','13','shoreline:type','sandy'], // Sandy
        ['SLT','14','shoreline:type','shingly'], // Shingly
        ['SLT','15','shoreline:type','other'], // Other


        // SND - Sounding Category
        ['SND','1','sounding:type','drying_heights'], // Drying Heights
        ['SND','2','sounding:type','no_bottom_found'], // No Bottom Found
        ['SND','9','sounding:type','slant'], // Slant
        ['SND','10','sounding:type','ordinary'], // Ordinary
        ['SND','999','sounding:type','unknown'], // Unknown


        // SOH - Severity of Hazard
        ['SOH','1','hazard:severity','dangerous'], // Dangerous
        ['SOH','2','hazard:severity','non_dangerous'], // Non Dangerous


        // SSC - Structure Shape
        // ['SSC','0','shape','unknown'], // Unknown
        ['SSC','0',undefined,undefined], // Unknown
        ['SSC','1','shape','barrel_ton'], // Barrel, Ton
        ['SSC','6','shape','conical'], // Conical
        ['SSC','7','shape','cylindrical'], // Cylindrical
        ['SSC','10','shape','pillar'], // Pillar
        ['SSC','12','shape','pyramidal'], // Pyramidal
        ['SSC','16','shape','spar'], // Spar
        ['SSC','17','shape','spherical'], // Spherical
        ['SSC','21','attraction','artificial_mountain'], // Artificial Mountain
        ['SSC','23','attraction','big_wheel'], // Ferris Wheel
        ['SSC','25','attraction','roller_coaster'], // Roller Coaster
        ['SSC','60','shape','mast'], // Mast
        ['SSC','73','shape','superbuoy'], // Superbuoy
        ['SSC','77','shape','arched'], // Arched
        ['SSC','85','shape','diamond_shaped_buoy'], // Diamond shaped buoy
        ['SSC','87','shape','domed'], // Domed
        ['SSC','107','shape','tower'], // Tower
        ['SSC','108','shape','scanner'], // Scanner
        ['SSC','109','shape','obelisk'], // Obelisk
        ['SSC','999','shape','other'], // Other


        // SSR - Structure Shape of Roof
        ['SSR','0','building:roof_shape','unknown'],
        ['SSR','40','building:roof_shape','domed'],
        ['SSR','41','building:roof_shape','flat'],
        ['SSR','51','building:roof_shape','with_steeple'],
        ['SSR','77','building:roof_shape','with_cupola'],
        ['SSR','79','building:roof_shape','with_tower'],
        ['SSR','80','building:roof_shape','with_minaret'],
        ['SSR','999','building:roof_shape','other'],


        // SST - Sound Signal Type
        ['SST','0','sound_signal','unknown'], // Unknown
        ['SST','1','sound_signal','bell'], // Bell
        ['SST','2','sound_signal','diaphone'], // Diaphone
        ['SST','3','sound_signal','explosive'], // Explosive Fog Signal
        ['SST','4','sound_signal','gong'], // Gong
        ['SST','6','sound_signal','horn'], // Horn
        ['SST','9','sound_signal','siren'], // Siren
        ['SST','14','sound_signal','whistle'], // Whistle
        ['SST','15','sound_signal','reed'], // Reed
        ['SST','16','sound_signal','none'], // None


        // STA - Station Type Category (Maritime)
        ['STA','0','seamark:signal_station','unknown'], // Unknown
        ['STA','1','seamark:signal_station','coast_guard'], // Coast Guard
        ['STA','2','seamark:signal_station','fireboat'], // Fireboat
        ['STA','3','seamark:signal_station','marine_police'], // Marine Police
        ['STA','4','seamark:signal_station','ice_signal'], // Ice Signal
        ['STA','5','seamark:signal_station','lifeboat/rescue'], // Lifeboat/Rescue
        ['STA','6','seamark:signal_station','port_control'], // Port Control
        ['STA','11','seamark:signal_station','pilot'], // Pilot
        ['STA','13','seamark:signal_station','signal'], // Signal
        ['STA','15','seamark:signal_station','storm_signal'], // Storm Signal
        ['STA','17','seamark:signal_station','tide_signal'], // Tide Signal
        ['STA','19','seamark:signal_station','time_signal'], // Time Signal
        ['STA','21','seamark:signal_station','weather_signal'], // Weather Signal
        ['STA','22','seamark:signal_station','fog_signal'], // Fog Signal
        ['STA','25','seamark:signal_station','semaphore'], // Semaphore
        ['STA','27','seamark:signal_station','tidal_current_signal'], // Tidal Current Signal
        ['STA','28','seamark:signal_station','traffic_signal'], // Traffic Signal
        ['STA','29','seamark:signal_station','bridge_signal'], // Bridge Signal
        ['STA','30','seamark:signal_station','lock_signal'], // Lock Signal
        ['STA','32','seamark:signal_station','international_port_signals'], // International Port Signals
        ['STA','33','seamark:signal_station','firing_practice_signal'], // Firing Practice Signal Station


        // SVC - Sounding Velocity
        // ['SVC','0','sounding:correction','unknown'], // Unknown
        ['SVC','0',undefined,undefined], // Unknown
        ['SVC','1','sounding:correction','echo_sounder_calibrated_at_4800_ft/sec_uncorrected'], // Echo Sounder Calibrated at 4800 ft/sec Uncorrected
        ['SVC','2','sounding:correction','echo_sounder_calibrated_at_1500_m/sec_uncorrected'], // Echo Sounder Calibrated at 1500 m/sec Uncorrected
        ['SVC','3','sounding:correction','mathews_tables_(np_139)_corrected'], // Mathews Tables (NP 139) Corrected
        ['SVC','4','sounding:correction','sound_velocity_meter_(svm)_corrected'], // Sound Velocity Meter (SVM) Corrected
        ['SVC','5','sounding:correction','corrected_by_other_means_of_calibration'], // Corrected by other means of calibration


        // TRE - Tree Category
        // ['TRE','0','wood','unknown'], // Unknown
        ['TRE','0',undefined,undefined], // Unknown
        ['TRE','1','wood','deciduous'], // Deciduous
        ['TRE','2','wood','evergreen'], // Evergreen
        ['TRE','3','wood','mixed'], // Mixed


        // TMC - Top Mark Characteristic
        ['TMC','0','seamark:topmark:shape','unknown'], // Unknown
        ['TMC','1','seamark:topmark:shape','east_mark_(2_cones_-_base_together)'], // East Mark (2 cones - base together)
        ['TMC','2','seamark:topmark:shape','isolated_danger_(2_balls)'], // Isolated Danger (2 balls)
        ['TMC','3','seamark:topmark:shape','north_mark_(2_cones_-_pointing_up)'], // North Mark (2 cones - pointing up)
        ['TMC','6','seamark:topmark:shape','special_(x)'], // Special (X)
        ['TMC','8','seamark:topmark:shape','south_mark_(2_cones_-_pointing_down)'], // South Mark (2 cones - pointing down)
        ['TMC','9','seamark:topmark:shape','west_mark_(2_cones_-_points_together)'], // West Mark (2 cones - points together)
        ['TMC','15','seamark:topmark:shape','ball_over_cone'], // Ball Over Cone
        ['TMC','16','seamark:topmark:shape','cone_over_ball'], // Cone Over Ball
        ['TMC','17','seamark:topmark:shape','broom_point_up'], // Broom Point Up
        ['TMC','18','seamark:topmark:shape','perch'], // Perch
        ['TMC','19','seamark:topmark:shape','diamond'], // Diamond
        ['TMC','21','seamark:topmark:shape','cone,_point_up'], // Cone (Point Upwards)
        ['TMC','22','seamark:topmark:shape','cone,_point_down'], // Cone (Point Downwards)
        ['TMC','23','seamark:topmark:shape','cross'], // Upright Cross
        ['TMC','25','seamark:topmark:shape','can_(open)'], // Can (Open)
        ['TMC','26','seamark:topmark:shape','can_(filled)'], // Can (Filled)
        ['TMC','27','seamark:topmark:shape','ball_(open)'], // Ball (Open)
        ['TMC','28','seamark:topmark:shape','ball_(filled)'], // Ball (Filled)
        ['TMC','29','seamark:topmark:shape','can_over_ball_(open)'], // Can Over Ball (Open)
        ['TMC','30','seamark:topmark:shape','cross_over_ball_(filled)'], // Cross Over Ball (Filled)
        ['TMC','31','seamark:topmark:shape','diamond_over_ball_(filled)'], // Diamond Over Ball (Filled)
        ['TMC','32','seamark:topmark:shape','double_cone,_points_apart_(open)'], // Double Cone, Points Apart (Open)
        ['TMC','33','seamark:topmark:shape','none'], // None


        // TSP - Traffic Scheme Part
        ['TSP','1','seamark:traffic_separation_scheme','arrow'], // Arrow
        ['TSP','2','seamark:traffic_separation_scheme','outer_boundary'], // Outer Boundary
        ['TSP','3','seamark:traffic_separation_scheme','separation_zone_area'], // Separation Zone Area
        ['TSP','4','seamark:traffic_separation_scheme','separation_zone_line'], // Separation Zone Line
        ['TSP','5','seamark:traffic_separation_scheme','separation_zone_point'], // Separation Zone Point
        ['TSP','6','seamark:traffic_separation_scheme','inbound_area'], // Inbound Area
        ['TSP','7','seamark:traffic_separation_scheme','outbound_area'], // Outbound Area


        // TUC - Transportation Use Category
        // ['TUC','0','transport:type','unknown'], // Unknown
        ['TUC','0',undefined,undefined], // Unknown
        ['TUC','1','transport:type','road_and_railway'], // Both Road and Railway
        ['TUC','3','transport:type','railway'], // Railroad
        ['TUC','4','transport:type','road'], // Road
        ['TUC','12','transport:type','marine'], // Marine
        ['TUC','13','transport:type','air'], // Air
        ['TUC','17','transport:type','pedestrian'], // Pedestrian
        ['TUC','37','transport:type','aqueduct'], // Aqueduct
        ['TUC','38','transport:type','canal'], // Canal


        // UMC - Underlying Material Characteristics
        // ['UMC','0','underlying:characteristic','unknown'], // Unknown
        ['UMC','0',undefined,undefined], // Unknown
        ['UMC','1','underlying:characteristic','broken'], // Broken
        ['UMC','2','underlying:characteristic','coarse'], // Coarse
        ['UMC','3','underlying:characteristic','decayed'], // Decayed
        ['UMC','4','underlying:characteristic','fine'], // Fine, Minute Particles
        ['UMC','5','underlying:characteristic','gritty'], // Gritty
        ['UMC','6','underlying:characteristic','hard'], // Hard
        ['UMC','7','underlying:characteristic','rotten'], // Rotten
        ['UMC','8','underlying:characteristic','soft'], // Soft
        ['UMC','9','underlying:characteristic','sticky'], // Sticky
        ['UMC','10','underlying:characteristic','stiff'], // Stiff
        ['UMC','11','underlying:characteristic','streaky'], // Streaky
        ['UMC','12','underlying:characteristic','tenacious'], // Tenacious
        ['UMC','13','underlying:characteristic','uneven'], // Uneven
        ['UMC','17','underlying:characteristic','calcareous'], // Calcareous
        ['UMC','18','underlying:characteristic','flinty'], // Flinty
        ['UMC','19','underlying:characteristic','glacial'], // Glacial
        ['UMC','20','underlying:characteristic','ground'], // Ground
        ['UMC','21','underlying:characteristic','large'], // Large
        ['UMC','22','underlying:characteristic','rocky'], // Rocky
        ['UMC','23','underlying:characteristic','small'], // Small
        ['UMC','24','underlying:characteristic','speckled'], // Speckled
        ['UMC','25','underlying:characteristic','varied'], // Varied
        ['UMC','26','underlying:characteristic','volcanic'], // Volcanic
        ['UMC','27','underlying:characteristic','medium'], // Medium


        // USE - Usage
        ['USE','0',undefined,undefined], // Unknown
        ['USE','23','use','international'], // International
        ['USE','32','use','insular'], // insular
        ['USE','51','use','telegraph'], // Telegraph
        ['USE','52','use','telephone'], // Telephone
        ['USE','53','use','power'], // Power
        ['USE','57','use','marine'], // Marine
        ['USE','69','use','levee/dike'], // Levee / Dike
        ['USE','119','use','berthing_vessels'], // Single Point Mooring
        ['USE','120','use','recreational'], // Recreational
        ['USE','127','use','causeway'], // As a Causeway
        ['USE','131','use','flood_barrage'], // flood Barrage
        ['USE','132','use','container'], // Container
        ['USE','133','use','single_point_mooring'], // Single Point Mooring
        ['USE','999','use','other'], // Single Point Mooring


        // VEG - Vegetation Characteristics
        // ['VEG','0','vegetation:type','unknown'], // Unknown
        ['VEG','0',undefined,undefined], // Unknown
        ['VEG','11','vegetation:type','casuarina'], // Casuarina
        ['VEG','12','vegetation:type','conifer'], // Conifer
        ['VEG','16','vegetation:type','nipa'], // Nipa
        ['VEG','17','vegetation:type','palm'], // Palm
        ['VEG','18','vegetation:type','filao'], // Filao
        ['VEG','19','vegetation:type','mangrove'], // Mangrove
        ['VEG','29','vegetation:type','eucalyptus'], // Eucalyptus
        ['VEG','38','vegetation:type','cypress'], // Cypress
        ['VEG','999','vegetation:type','other'], // Other


        // VRR - Vertical Reference Category
        // ['VRR','0','hydrographic_vertical_position','unknown'], // Unknown
        ['VRR','0',undefined,undefined], // Unknown
        ['VRR','1','hydrographic_vertical_position','always_dry'], // Above Surface/Does Not Cover (at High Water)
        ['VRR','2','hydrographic_vertical_position','awash_at_chart_datum'], // Awash at Sounding Datum
        ['VRR','4','hydrographic_vertical_position','always_submerged'], // Below Surface/Submerged
        ['VRR','8','hydrographic_vertical_position','covers_and_uncovers'], // Covers and Uncovers
        // ['VRR','9','hydrographic_vertical_position','not_applicable'], // Not Applicable


        // WPC - Work in Progress Category
        // ['WPC','0','work:type','unknown'], // Unknown
        ['WPC','0',undefined,undefined], // Unknown
        ['WPC','1','work:type','land_reclamation'], // Land Reclamation
        ['WPC','2','work:type','construction_of_structures'], // Construction of Structures

    ], // ##### End of One2One Rules #####


    // ##### Start of intList #####
    // intList - The list of fields that have to be integer
    intList : [
    'ABS_HORIZ_ACC', 'ABS_VERT_ACC', 'BR2', 'BRF', 'D80', 'D80', 'D81', 'D82', 'D83', 'D84', 'D85', 'D86', 'D87',
    'D88', 'D89', 'D90', 'D91', 'DAT', 'DF1', 'DF2', 'DF3', 'DF4', 'DOF', 'EOL', 'HGT', 'LOR', 'LVN', 'OR2',
    'ORC', 'SCALE', 'SOURCE_ID', 'TIM', 'VAL', 'VAV', 'WID', 'WID', 'ZV2'
    ],
    // ##### End of intList #####


    // ##### Start of txtLength #####
    // This list is for validateing the lengths of text attributes prior to export
    // NOTE: DAN,TXT and NAM have been made wider which is not exactly to spec
    txtLength : {
    'BER':25, 'BRR':11, 'COL':10, 'DATUM':40, 'DRP':30, 'LINAGE':100, 'LSA':11, 'MCA':5, 'MLR':10, 'NAME':40,
    'NM3':30, 'NM4':30, 'PRINT_DATE':20, 'SOURCE_INFO':100
    },
    // ##### End of txtLength #####


    // Lookup table for setting default attributes for layers with F_CODE specific defaults
    defaultList : {
        'AC000A':{'NAM':'N/A','ZV2':'-2147483648'},
        'AF010P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','SSC':'-32768','TUC':'-32768','USE':'-32768'},
        'AF040P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','SSC':'-32768','ZV2':'-2147483648'},
        'AF070P':{'COL':'N/A','HGT':'-2147483648','NAM':'N/A','PRO':'-32768','SSC':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AH050A':{'PRO':'-32768','ZV2':'-2147483648'},
        'AH050P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','PRO':'-32768','SSC':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AJ050P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','NAM':'N/A','SSC':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AK020L':{'NAM':'N/A'},
        'AK020P':{'COL':'N/A','LOC':'-32768','NAM':'N/A','PRO':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AK130L':{'SSC':'-32678'},
        'AK160A':{'PRO':'-32768','ZV2':'-2147483648'},
        'AL015P':{'SSR':'-32768'},
        'AL018P':{'NAM':'N/A'},
        'AL130P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','PRO':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AL240P':{'HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','SSC':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AM040A':{'NAM':'N/A','ZV2':'-2147483648'},
        'AM070A':{'NAM':'N/A'},
        'AM070P':{'HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','SSC':'-32768','TUC':'-32768','USE':'-32768'},
        'AN060A':{'APT':'-32768','NAM':'N/A'},
        'AQ060P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','SSC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AQ080P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','SSC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AT045P':{'COL':'N/A','HGT':'-2147483648','LOC':'-32768','NAM':'N/A','PRO':'-32768','TUC':'-32768','USE':'-32768','ZV2':'-2147483648'},
        'AT050P':{'COL':'N/A'},
        'BB010A':{'EXS':'-32678','OPS':'-32768','PRO':'-32768','TXT':'N/A'},
        'BB010L':{'DRP':'N/A','EXS':'-32768','MBL':'-32768','OPS':'-32768','PBV':'-32768','PRO':'-32768','TXT':'N/A'},
        'BB010P':{'TXT':'N/A'},
        'BB090A':{'USE':'-32768'},
        'BB100L':{'EXS':'-32768','LOC':'-32768','USE':'-32768'},
        'BB110A':{'NAM':'N/A'},
        'BB115A':{'VRR':'-32768'},
        'BB170L':{'TUC':'-32768'},
        'BB180A':{'NAM':'N/A'},
        'BB190L':{'VRR':'-32768'},
        'BB240L':{'USE':'-32768'},
        'BC010P':{'BTC':'-32768','SSC':'-32768'},
        'BC020P':{'ACC':'-32768'},
        'BC055P':{'USE':'-32768'},
        'BC070P':{'ACC':'-32768','BTC':'-32768','CCC':'-32768','SSC':'-32768'},
        'BC080A':{'NAM':'N/A'},
        'BD000A':{'COD':'-32768','LOC':'-32768'},
        'BD000P':{'COD':'-32768','LOC':'-32768','MCC':'-32768','NAM':'N/A'},
        'BD010A':{'NAM':'N/A'},
        'BD010L':{'EXS':'-32768','LOC':'-32768','USE':'-32768'},
        'BD010P':{'VRR':'-32768'},
        'BD020A':{'COD':'-32768','LOC':'-32768','SFC':'-32768','SOH':'-32768','TXT':'N/A'},
        'BD020P':{'COD':'-32768','LOC':'-32768','MCC':'-32768','NAM':'N/A','SFC':'-32768','SOH':'-32768','TXT':'N/A'},
        'BD030A':{'VRR':'-32768'},
        'BD030P':{'VRR':'-32768'},
        'BD040A':{'NAM':'N/A'},
        'BD050A':{'VRR':'-32768'},
        'BD050P':{'VRR':'-32768'},
        'BD060A':{'NAM':'N/A'},
        'BD100P':{'USE':'-32768'},
        'BD130P':{'COD':'-32768','LOC':'-32768','SFC':'-32768','TXT':'N/A'},
        'BD180A':{'ACC':'-32768','DAT':'-32768','EXS':'-32768','SFC':'-32768','TXT':'N/A','VAL':'-32768'},
        'BD180P':{'MCC':'-32768','NAM':'N/A','SFC':'-32768','TXT':'N/A'},
        'BG020P':{'NAM':'N/A'},
        'BH090A':{'EXS':'-32768','NAM':'N/A','WID':'-2147483648'},
        'BH095A':{'USE':'-32768'},
        'BH120L':{'EXS':'-32768','NAM':'N/A'},
        'BH130A':{'HYC':'-32768','RPA':'-32678'},
        'BH140L':{'ACC':'-32768','AHC':'-32768','SLT':'-32768'},
        'BH155A':{'NAM':'N/A','WID':'-2147483648'},
        'BH180L':{'EXS':'-32768'},
        'BH210L':{'HYC':'-32768','NAM':'N/A'},
        'BI030A':{'EXS':'-32768'},
        'BI040L':{'NAM':'N/A'},
        'BJ030A':{'SIC':'-32768'},
        'BJ065A':{'SIC':'-32768'},
        'BJ100A':{'SIC':'-32768'},
        'DB090A':{'EXS':'-32768'},
        'DB170A':{'EXS':'-32768','USE':'-32768','VRR':'-32678'},
        'FA020L':{'USE':'-32768'},
        'FA040L':{'NM4':'N/A','USE':'-32768'},
        'FA110L':{'NM3':'N/A','NM4':'N/A','USE':'-32768'},
        'FC021P':{'COD':'-32768','MAC':'0','WID':'-2147483648','TIM':'-32768','TXT':'N/A'},
        'FC036A':{'COD':'-32768','EXS':'-32678','NAM':'N/A','OPS':'-32768'},
        'FC036L':{'COD':'-32768','DRP':'N/A','EXS':'-32768','MBL':'-32768','NAM':'N/A','OPS':'-32768','PBV':'-32768','PRO':'0'},
        'FC036P':{'COD':'-32768','NAM':'N/A','TIM':'-32768'},
        'FC041A':{'EXS':'-32678','OPS':'-32768','WID':'-2147483648'},
        'FC041L':{'EXS':'-32768','UNI':'-32768','OPS':'-32768','WID':'-2147483648'},
        'FC130L':{'LOR':'-32768','UNI':'-32768'},
        'FC170A':{'EXS':'-32678','IAS':'-32768','OPS':'-32768','TSP':'-32768','WID':'-2147483648'},
        'FC170L':{'EXS':'-32768','IAS':'-32768','OPS':'-32768','TSP':'-32768'},
        'GA020P':{'APT':'-32768','BRF':'-2147483648','COL':'N/A','EXS':'-32768','MCA':'N/A','NAM':'N/A','NST':'-32768','ORC':'-32768'},
        'GA035P':{'APT':'-32768','COL':'N/A','EXS':'42','NAM':'N/A','OLQ':'-32768'},
        'GB005P':{'BRF':'-2147483648','COL':'N/A','EXS':'28','MCA':'N/A','NST':'-32768','OLQ':'-32768','ORC':'-32768'},
        'GB010P':{'APT':'-32768','BRF':'-2147483648','EXS':'-32768','MCA':'N/A','NAM':'N/A','NST':'-32768','OLQ':'-32768','ORC':'-32768'},
        'GB055A':{'APT':'-32768','NAM':'N/A'},
        'GB070A':{'TSP':'-32768','WID':'-2147483648'},
        'GB070L':{'IAS':'-32768','TSP':'-32768','WID':'-2147483648'},
    }, // End defaultList

    // Lookup tables for linking an F_CODE to a Layer Name
    layerList : {
        'AA010A':'LANDMRKA',
        'AA010P':'LANDMRKP', // Mine /Quarry
        'AB000A':'LANDMRKA',
        'AB010A':'LANDMRKA',
        'AC000A':'INDUSTA',
        'AC030A':'LANDMRKA',
        'AD010A':'LANDMRKA',
        'AD030A':'LANDMRKA',
        'AD030P':'LANDMRKP', // Substation/Transformer Yard
        'AF010P':'INDUSTP', // Chimney/Smokestack
        'AF030P':'LANDMRKP', // Cooling Tower
        'AF040P':'INDUSTP', // Crane
        'AF070P':'INDUSTP', // Flare Pipe
        'AH010A':'LANDMRKA',
        'AH010P':'LANDMRKP', // Bastion /Rampart/Fortification
        'AH050A':'INDUSTA',
        'AH050P':'INDUSTP', // Fortification
        'AJ050P':'INDUSTP', // Windmill
        'AK020L':'PARKL',
        'AK020P':'INDUSTP', // Amusement Park Attraction
        'AK040A':'PARKA',
        'AK080P':'LANDMRKP', // Drive-In Theater Screen
        'AK090A':'PARKA',
        'AK100A':'PARKA',
        'AK120A':'PARKA',
        'AK130L':'PARKL',
        'AK160A':'INDUSTA',
        'AK160P':'LANDMRKP', // Stadium /Amphitheater
        'AK180A':'PARKA',
        'AL005A':'LANDMRKA',
        'AL015A':'BUILDNGA',
        'AL015P':'BUILDNGP',
        'AL018P':'BUILDNGP',
        'AL020A':'LANDMRKA',
        'AL020P':'BUILTUPP',
        'AL020P':'COAPOINT',
        'AL025P':'LANDMRKP', // Cairn
        'AL030A':'LANDMRKA',
        'AL050P':'MARKERP',
        'AL070L':'FENCEL',
        'AL073P':'LANDMRKP', // Flagstaff/Flagpole
        'AL130P':'INDUSTP', // Monument
        'AL200A':'RUINSA',
        'AL200P':'RUINSP',
        'AL240P':'INDUSTP', // Tower (Non-communication)
        'AL260L':'FENCEL',
        'AM010A':'LANDMRKA',
        'AM020P':'LANDMRKP', // Grain Bin /Silo
        'AM030A':'LANDMRKA',
        'AM030P':'LANDMRKP', // Grain Elevator
        'AM040A':'INDUSTA',
        'AM070A':'INDUSTA',
        'AM070P':'INDUSTP', // Tank
        'AM080P':'LANDMRKP', // Water Tower
        'AN010L':'RAILRDL',
        'AN060A':'TRANSA',
        'AP020L':'TRANSL',
        'AP030L':'TRANSL',
        'AQ010L':'POWERL',
        'AQ020P':'POWERP',
        'AQ040A':'BRIDGEA',
        'AQ040L':'BRIDGEL',
        'AQ045A':'BRIDGSPA',
        'AQ045L':'BRIDGSPL',
        'AQ060P':'INDUSTP', // Control Tower
        'AQ070L':'FERRYL',
        'AQ080P':'INDUSTP', // Ferry Site
        'AQ110P':'LANDMRKP', // Mooring Mast
        'AQ113L':'PIPELINL',
        'AQ116A':'LANDMRKA',
        'AQ116P':'LANDMRKP', // Pumping Station
        'AQ130L':'TUNNELL',
        'AT005L':'HAZARDL',
        'AT010P':'LANDMRKP', // Disk/Dish
        'AT020A':'LANDMRKA',
        'AT030L':'POWERL',
        'AT040P':'POWERP',
        'AT045P':'INDUSTP', // Radar Transmitter
        'AT050P':'COMMP',
        'AT060L':'POWERL',
        'AT080P':'COMMP',
        'BA010L':'COALINE',
        'BA010L':'COASTL',
        'BA020A':'FORESHOA',
        'BA020L':'FORESHOL',
        'BA020P':'FORESHOP',
        'BA030A':'ECRAREA',
        'BA030P':'ISLANDP',
        'BA040A':'COAAREA',
        'BA040A':'ECRAREA',
        'BB010A':'LIMBNDYA',
        'BB010L':'LIMBNDYL',
        'BB010P':'LIMBNDYP',
        'BB020P':'BERTHP',
        'BB030P':'MOORINGP',
        'BB040A':'STRUCTRA',
        'BB040L':'STRUCTRL',
        'BB050P':'CALLINP',
        'BB080P':'MOORINGP',
        'BB090A':'PIERA',
        'BB100L':'HAZARDL',
        'BB105P':'OBSTRUCP',
        'BB110A':'OBSTRUCA',
        'BB115A':'STRUCTRA',
        'BB140A':'STRUCTRA',
        'BB140L':'STRUCTRL',
        'BB150A':'LANDINGA',
        'BB150P':'LANDINGP',
        'BB160P':'MOORINGP',
        'BB170A':'LOADINGA',
        'BB170L':'TUNNELL',
        'BB170P':'LOADINGP',
        'BB180A':'OBSTRUCA',
        'BB180P':'OBSTRUCP',
        'BB190A':'PIERA',
        'BB190L':'PIERL',
        'BB220A':'STRUCTRA',
        'BB220L':'STRUCTRL',
        'BB230L':'SEAWALLL',
        'BB240L':'PIERL',
        'BC010P':'BUOYBCNP',
        'BC020P':'BUOYBCNP',
        'BC040L':'LIGHTSL',
        'BC040P':'LIGHTSP',
        'BC055P':'MARKERP',
        'BC060A':'SECTORA',
        'BC070P':'BUOYBCNP',
        'BC080A':'OBSTRUCA',
        'BC080P':'OBSTRUCP',
        'BC100L':'LEADINGL',
        'BD000A':'HAZARDA',
        'BD000P':'HAZARDP',
        'BD010A':'OBSTRUCA',
        'BD010L':'HAZARDL',
        'BD010P':'DANGERP',
        'BD020A':'HAZARDA',
        'BD020P':'HAZARDP',
        'BD030A':'DANGERA',
        'BD030P':'DANGERP',
        'BD040A':'OBSTRUCA',
        'BD040P':'OBSTRUCP',
        'BD050A':'DANGERA',
        'BD050P':'DANGERP',
        'BD060A':'OBSTRUCA',
        'BD060P':'OBSTRUCP',
        'BD080A':'OBSTRUCA',
        'BD080P':'OBSTRUCP',
        'BD100A':'DANGERA',
        'BD100P':'DANGERP',
        'BD110P':'LOADINGP',
        'BD120A':'REEFA',
        'BD130P':'HAZARDP',
        'BD140A':'DANGERA',
        'BD140P':'DANGERP',
        'BD180A':'HAZARDA',
        'BD180P':'HAZARDP',
        'BE010A':'HYDAREA',
        'BE010L':'HYDLINE',
        'BE020P':'SOUNDP',
        'BF010P':'BOTCHARP',
        'BG010P':'CURRFLP',
        'BG020P':'TIDEP',
        'BG030P':'TIDEP',
        'BG040P':'CURRDIAP',
        'BH010L':'AQUEDCTL',
        'BH020A':'CANALA',
        'BH020L':'CANALL',
        'BH050A':'FISHHATA',
        'BH080A':'LAKEA',
        'BH090A':'MISCIWYA',
        'BH095A':'EMBANKA',
        'BH120L':'BARRIERL',
        'BH130A':'LAKEA',
        'BH140A':'RIVERA',
        'BH140L':'RIVERL',
        'BH155A':'MISCIWYA',
        'BH180L':'BARRIERL',
        'BH210L':'RIVERL',
        'BI020L':'DAML',
        'BI030A':'MISCIWYA',
        'BI040L':'BARRIERL',
        'BJ030A':'SNOWICEA',
        'BJ060P':'SNOWICEP',
        'BJ065A':'SNOWICEA',
        'BJ100A':'SNOWICEA',
        'BJ110A':'SNOWICEA',
        'CA010L':'RELLINE',
        'CA030P':'RELPOINT',
        'DA010A':'ECRAREA',
        'DB090A':'EMBANKA',
        'DB090L':'LCRLINE',
        'DB170A':'EMBANKA',
        'DB180A':'VOLCANOA',
        'DQA':'DQYAREA',  // Fake F_CODE since DQ doesn't have an F_CODE
        'EA030A':'ORCHARDA',
        'EA040A':'ORCHARDA',
        'EA050A':'ORCHARDA',
        'EC030A':'TREEA',
        'EC030P':'TREEP',
        'FA000L':'ADMINL',
        'FA000XL':'COALINE', // Fixing this in the manyFeatures and validateAttributesfunctions
        'FA001A':'COAAREA',
        'FA020L':'BOUNDRYL',
        'FA040L':'BOUNDRYL',
        'FA060L':'BOUNDRYL',
        'FA110L':'BOUNDRYL',
        'FC021A':'LIMBNDYA',
        'FC021L':'LIMBNDYL',
        'FC021P':'LIMBNDYP',
        'FC031A':'MARITIMA',
        'FC031L':'MARITIML',
        'FC031P':'MARITIMP',
        'FC036A':'LIMBNDYA',
        'FC036L':'LIMBNDYL',
        'FC036P':'LIMBNDYP',
        'FC041A':'SEPARTNA',
        'FC041L':'SEPARTNL',
        'FC041P':'SEPARTNP',
        'FC100L':'DISTL',
        'FC130L':'DISTL',
        'FC165A':'ROUTEA',
        'FC165L':'ROUTEL',
        'FC165P':'ROUTEP',
        'FC170A':'SEPARTNA',
        'FC170L':'SEPARTNL',
        'FC177A':'SWEPTA',
        'FC177L':'SWEPTL',
        'GA020P':'AEROP',
        'GA035P':'AEROP',
        'GB005A':'TRANSA',
        'GB005P':'AEROP',
        'GB010P':'AEROP',
        'GB055L':'TRANSL',
        'GB055A':'TRANSA',
        'GB070A':'SEPARTNA',
        'GB070L':'SEPARTNL',
        'ZC040A':'ENVAREA',
        'ZD040P':'ECRTEXT', // NOTE: This is a TEXT table
    }, // End layerList
} // End of dnc.rules
